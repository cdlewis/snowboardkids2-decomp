#include "race/spray_particles.h"
#include "common.h"
#include "data/asset_metadata.h"
#include "data/data_table.h"
#include "data/global_frame_counter.h"
#include "gamestate.h"
#include "gbi.h"
#include "graphics/displaylist.h"
#include "graphics/graphics.h"
#include "graphics/sprite_rdp.h"
#include "math/geometry.h"
#include "math/rand.h"
#include "race/race_session.h"
#include "system/memory_allocator.h"
#include "system/task_scheduler.h"

typedef struct {
    DataTable_19E80 *assetTable;
    BillboardSprite particle;
    Vec3i velocity;
    s32 baseAssetIndex;
    s32 frameCounter;
} SprayEffectTask;

typedef struct {
    DataTable_19E80 *assetTable;
    BillboardSprite particles[2];
    Vec3i velocity;
    s16 frameCounter;
    s16 particleBufferIndex;
    s16 baseAssetIndex;
} DualSnowSprayTask;

typedef struct {
    Player *sourcePlayer;
    DataTable_19E80 *assetTable;
    BillboardSprite particle;
} GlintEffectTask;

typedef struct {
    Player *player;
    DataTable_19E80 *assetTable;
    BillboardSprite particleLeft;
    BillboardSprite particleRight;
    Vec3i skiOffsets[2];
    s16 frameCounter;
    s16 particleIndex;
} SkiTrailTask;

typedef struct {
    /* 0x00 */ DataTable_19E80 *assetTable;
    /* 0x04 */ BillboardSprite particle;
    /* 0x24 */ s16 baseAssetIndex;
    /* 0x26 */ u16 animFrame;
    /* 0x28 */ Vec3i velocity;
    /* 0x34 */ Player *sourcePlayer;
    /* 0x38 */ s16 positionMode;
} CharacterTrailParticleTask;

typedef struct {
    DataTable_19E80 *assetTable;
    BillboardSprite particle;
    s32 baseAssetIndex;
    s32 frameCounter;
} ImpactStarTask;

typedef struct {
    BillboardSprite particles[6];
    DataTable_19E80 *assetTable;
    Player *sourcePlayer;
    Vec3i positionOffsets;
    s8 baseAssetIndex;
    u8 frameCounter;
    u8 isVariant; // 0 = variant A, 1 = variant B (different alpha and offsets)
} CharacterAttackEffectState;

void loadFirstSprayParticle(SprayEffectTask *);
void cleanupSprayEffect(void **);
void updateSprayEffect(SprayEffectTask *);
void initDualSnowSprayTask(DualSnowSprayTask *);
void initDualSnowSprayTask_SingleSlot(DualSnowSprayTask *);
void updateDualSnowSprayParticles(DualSnowSprayTask *);
void cleanupDualSnowSprayAssetNode(DualSnowSprayTask *);
void initCharacterTrailParticleTask(MemoryAllocatorNode **);
void loadCharacterTrailParticleAsset(CharacterTrailParticleTask *);
void updateCharacterTrailParticle(CharacterTrailParticleTask *);
void cleanupCharacterTrailParticleTask(s32 **);
void loadImpactStarAsset(ImpactStarTask *);
void updateImpactStar(ImpactStarTask *);
void cleanupImpactStar(void **);
void updateFloatingItemSprite(FloatingItemSpriteTask *);
void cleanupFloatingItemSpriteTask(FloatingItemSpriteTask *);
void updateDualSnowSprayParticles_SingleSlot(DualSnowSprayTask *);
void cleanupDualSnowSprayTask(DualSnowSprayTask *);
void updateGlintEffect(GlintEffectTask *);
void cleanupGlintEffect(GlintEffectTask *);
void loadCharacterAttackEffectAssets(CharacterAttackEffectState *);
void updateCharacterAttackEffect(CharacterAttackEffectState *);
void cleanupCharacterAttackEffectTask(CharacterAttackEffectState *);
void updateSkiTrailTask(SkiTrailTask *arg0);
void cleanupSkiTrailTask(SkiTrailTask *task);

u8 gCharacterParticleTypeMap[16] = {
    0xFF, 0x08, 0x26, 0x12, 0x17, 0x1C, 0xFF, 0x0D, 0x21, 0x0D, 0x17, 0x12, 0xFF, 0xFF, 0xFF, 0xFF,
};

s16 gSkiTrailOffsetTransformsForward[12] = {
    4, 0, 0, 0, 13, 0, -4, 0, 0, 0, 13, 0,
};

s16 gSkiTrailOffsetTransformsBackward[12] = {
    -4, 0, 0, 0, -13, 0, 4, 0, 0, 0, -13, 0,
};

s16 gGlintEffectTransform[8] = {
    0, 0, 6, 0, 0x24, 0, 0, 0,
};

Vtx gGlintEffectAssetTemplate[4] = {
    { { { -96, 96, 0 }, 0, { -16, -16 }, { 255, 255, 255, 255 } } },
    { { { 96, 96, 0 }, 0, { 1008, -16 }, { 255, 255, 255, 255 } } },
    { { { 96, -96, 0 }, 0, { 1008, 1008 }, { 255, 255, 255, 255 } } },
    { { { -96, -96, 0 }, 0, { -16, 1008 }, { 255, 255, 255, 255 } } },
};

Vtx gCharacterAttackEffectAssetTemplate[4] = {
    { { { -9, 9, 0 }, 0, { -16, -16 }, { 255, 255, 255, 255 } } },
    { { { 9, 9, 0 }, 0, { 1008, -16 }, { 255, 255, 255, 255 } } },
    { { { 9, -9, 0 }, 0, { 1008, 1008 }, { 255, 255, 255, 255 } } },
    { { { -9, -9, 0 }, 0, { -16, 1008 }, { 255, 255, 255, 255 } } },
};

Vec3i gCharacterAttackEffectPositionOffsetsA = { 0, 0, 0x18000 };

s32 gCharacterAttackEffectPositionOffsetsB[5] = { 0, 0, 0x80000, 0, 0 };

void initSprayEffectTask(void **node) {
    *node = load_3ECE40();
    setCleanupCallback(&cleanupSprayEffect);
    setCallbackWithContinue(&loadFirstSprayParticle);
}

void loadFirstSprayParticle(SprayEffectTask *task) {
    GameState *gs = (GameState *)getCurrentAllocation();
    loadAssetMetadata(&task->particle, task->assetTable, task->baseAssetIndex);
    task->particle.alpha = 0xE0;
    task->particle.vertices = (Vtx *)&gs->unk44->unkFC0->asset;
    task->frameCounter = 0;
    setCallbackWithContinue(&updateSprayEffect);
}

void updateSprayEffect(SprayEffectTask *task) {
    GameState *gs;
    s32 i;
    gs = (GameState *)getCurrentAllocation();
    i = 0;

    if (gs->gamePaused == 0) {
        GameStateUnk44 *base;

        base = gs->unk44;
        task->particle.vertices = (Vtx *)&base->unkFC0[task->frameCounter].asset;

        task->frameCounter = task->frameCounter + 1;
        if (task->frameCounter == 4) {
            terminateCurrentTask();
            return;
        }
        task->particle.alpha = task->particle.alpha - 0x30;
        task->particle.position.x += task->velocity.x;
        task->particle.position.y += task->velocity.y;
        task->particle.position.z += task->velocity.z;
    }

    for (i = 0; i < 4; i++) {
        enqueueAlphaBillboardSprite(i, &task->particle);
    }
}

void cleanupSprayEffect(void **arg0) {
    *arg0 = freeNodeMemory(*arg0);
}

void spawnSprayEffect(Vec3i *position, Vec3i *velocity, s32 baseAssetIndex) {
    SprayEffectTask *task = (SprayEffectTask *)scheduleTask(&initSprayEffectTask, 2, 0, 0xDC);
    if (task != NULL) {
        memcpy(&task->particle.position, position, sizeof(Vec3i));
        task->baseAssetIndex = baseAssetIndex;
        task->velocity.x = velocity->x / 2;
        task->velocity.y = velocity->y / 2;
        task->velocity.z = velocity->z / 2;
    }
}

void initDualSnowSprayTask(DualSnowSprayTask *arg0) {
    GameState *gs = (GameState *)getCurrentAllocation();
    arg0->assetTable = load_3ECE40();
    arg0->particles[0].vertices = (Vtx *)&gs->unk44->unk1080[arg0->particleBufferIndex];
    arg0->particles[0].alpha = (u8)((randA() & 0x1F) + 0x70);
    arg0->frameCounter = 0;
    arg0->particles[1].vertices = arg0->particles[0].vertices;
    arg0->particles[1].alpha = arg0->particles[0].alpha;
    setCleanupCallback(&cleanupDualSnowSprayAssetNode);
    setCallbackWithContinue(&updateDualSnowSprayParticles);
}

void updateDualSnowSprayParticles(DualSnowSprayTask *arg0) {
    GameState *gs;
    s32 i;

    gs = (GameState *)getCurrentAllocation();
    loadAssetMetadata(&arg0->particles[0], arg0->assetTable, arg0->baseAssetIndex + arg0->frameCounter);

    arg0->particles[1].textureData = arg0->particles[0].textureData;
    arg0->particles[1].paletteData = arg0->particles[0].paletteData;
    arg0->particles[1].textureWidth = arg0->particles[0].textureWidth;
    arg0->particles[1].textureHeight = arg0->particles[0].textureHeight;

    for (i = 0; i < 4; i++) {
        enqueueAlphaSprite(i, &arg0->particles[0]);
        enqueueAlphaSprite(i, &arg0->particles[1]);
    }

    if (gs->gamePaused == 0) {
        if (arg0->frameCounter != 0) {
            for (i = 0; i < 2; i++) {
                arg0->particles[i].position.x += arg0->velocity.x;
                arg0->particles[i].position.y += arg0->velocity.y;
                arg0->particles[i].position.z += arg0->velocity.z;
            }
        }

        arg0->frameCounter++;
        if (arg0->frameCounter == 5) {
            terminateCurrentTask();
        }
    }
}

void cleanupDualSnowSprayAssetNode(DualSnowSprayTask *arg0) {
    arg0->assetTable = freeNodeMemory(arg0->assetTable);
}

void spawnDualSnowSprayEffect(Vec3i *pos1, Vec3i *pos2, Vec3i *velocity, s32 slotIndex, s32 characterId) {
    DualSnowSprayTask *task;
    s32 velX;
    s32 velY;
    s32 velZ;
    u32 signX;
    u32 signY;
    u32 signZ;
    u8 particleType;

    if (gCharacterParticleTypeMap[characterId] == 0xFF) {
        return;
    }

    task = (DualSnowSprayTask *)scheduleTask(&initDualSnowSprayTask, 2, 0, 0xDD);
    if (task == NULL) {
        return;
    }

    memcpy(&task->particles[0].position, pos1, sizeof(Vec3i));
    memcpy(&task->particles[1].position, pos2, sizeof(Vec3i));

    particleType = gCharacterParticleTypeMap[characterId];
    task->particleBufferIndex = slotIndex;
    task->baseAssetIndex = particleType;

    velX = velocity->x;
    signX = (u32)velX >> 31;
    velX += signX;
    velX >>= 1;
    task->velocity.x = velX;

    velY = velocity->y;
    signY = (u32)velY >> 31;
    velY += signY;
    velY >>= 1;
    task->velocity.y = velY;

    velZ = velocity->z;
    signZ = (u32)velZ >> 31;
    velZ += signZ;
    velZ >>= 1;
    task->velocity.z = velZ;
}

void initCharacterTrailParticleTask(MemoryAllocatorNode **node) {
    *node = load_3ECE40();
    setCleanupCallback(&cleanupCharacterTrailParticleTask);
    setCallbackWithContinue(&loadCharacterTrailParticleAsset);
}

void loadCharacterTrailParticleAsset(CharacterTrailParticleTask *task) {
    s32 temp;
    s32 shift9;
    int new_var2;
    s32 new_var;
    getCurrentAllocation();
    if (task->positionMode >= 0) {
        memcpy(
            &task->particle.position,
            &task->sourcePlayer->bodyPartDisplayObjects[0].transform.translation,
            sizeof(Vec3i)
        );
    } else {
        memcpy(&task->particle.position, &task->sourcePlayer->worldPos, sizeof(Vec3i));
        task->particle.position.y += 0x80000;
    }
    temp = (randA() & 0xFF) - 0x80;
    shift9 = (new_var = temp << 9);
    task->particle.position.x += ((temp << 11) + shift9) << 1;
    new_var2 = (randA() & 0xFF) - 0x80;
    temp = new_var2;
    shift9 = temp << 9;
    task->animFrame = 0;
    task->particle.position.z += ((temp << 11) + shift9) << 1;
    loadAssetMetadata(&task->particle, task->assetTable, task->baseAssetIndex);
    setCallbackWithContinue(updateCharacterTrailParticle);
}

void updateCharacterTrailParticle(CharacterTrailParticleTask *arg0) {
    GameState *gs;
    s32 i;
    s16 temp;

    gs = (GameState *)getCurrentAllocation();

    if (gs->gamePaused == 0) {
        temp = arg0->animFrame;

        if (temp % 3 == 0) {
            loadAssetMetadata((&arg0->particle), arg0->assetTable, arg0->baseAssetIndex + (temp >> 2));
        }

        arg0->animFrame++;
        if ((s16)arg0->animFrame >= 0xF) {
            terminateCurrentTask();
        }

        arg0->particle.position.x += arg0->velocity.x;
        arg0->particle.position.y += arg0->velocity.y;
        arg0->particle.position.z += arg0->velocity.z;
    }

    i = 0;
    if (arg0->particle.alpha == 0xFF) {
        do {
            enqueueTexturedBillboardSprite(i, (BillboardSprite *)&arg0->particle);
            i++;
        } while (i < 4);
    } else {
        do {
            enqueueAlphaBillboardSprite(i, (&arg0->particle));
            i++;
        } while (i < 4);
    }
}

void cleanupCharacterTrailParticleTask(s32 **arg0) {
    *arg0 = freeNodeMemory(*arg0);
}

void spawnCharacterTrailParticle(Player *player) {
    GameState *allocation;
    CharacterTrailParticleTask *task;

    allocation = (GameState *)getCurrentAllocation();
    task = (CharacterTrailParticleTask *)scheduleTask(&initCharacterTrailParticleTask, 2, 0, 0xEA);
    if (task != NULL) {
        task->baseAssetIndex = 0x35;
        task->sourcePlayer = player;
        task->particle.alpha = 0xFF;
        task->velocity.x = 0;
        task->velocity.y = 0;
        task->velocity.z = 0;
        task->positionMode = 0;
        task->particle.vertices = (Vtx *)&allocation->unk44->unkFC0;
    }
}

void spawnPlayerCharacterTrailParticle(Player *player, s32 characterId) {
    GameState *allocation;
    CharacterTrailParticleTask *task;
    u8 temp;

    allocation = (GameState *)getCurrentAllocation();
    temp = gCharacterParticleTypeMap[characterId];

    if (temp == 0xFF) {
        return;
    }

    if (gGlobalFrameCounter & 1) {
        return;
    }

    task = (CharacterTrailParticleTask *)scheduleTask(&initCharacterTrailParticleTask, 2, 0, 0xEA);

    if (task != NULL) {
        u8 temp2;
        task->sourcePlayer = player;
        temp2 = gCharacterParticleTypeMap[characterId];
        task->particle.alpha = 0x80;
        task->baseAssetIndex = temp2;
        task->velocity.x = player->velocity.x / 2;
        task->velocity.y = player->velocity.y / 2;
        task->velocity.z = player->velocity.z / 2;
        task->positionMode = -1;
        task->particle.vertices = allocation->unk44->characterTrailVertices;
    }
}

void initImpactStarTask(MemoryAllocatorNode **node) {
    *node = load_3ECE40();
    setCleanupCallback(&cleanupImpactStar);
    setCallbackWithContinue(&loadImpactStarAsset);
}

void loadImpactStarAsset(ImpactStarTask *task) {
    GameState *allocation;
    Vtx *vertices;

    allocation = getCurrentAllocation();
    vertices = allocation->unk44->impactStarVertices;
    task->baseAssetIndex = 0x40;
    task->particle.vertices = vertices;
    task->frameCounter = 0;

    loadAssetMetadata(&task->particle, task->assetTable, task->baseAssetIndex);

    setCallbackWithContinue(&updateImpactStar);
}

void updateImpactStar(ImpactStarTask *arg0) {
    GameState *alloc;
    s32 i;

    alloc = getCurrentAllocation();
    if (alloc->gamePaused == 0) {
        if ((arg0->frameCounter & 1) == 0) {
            loadAssetMetadata(&arg0->particle, arg0->assetTable, arg0->baseAssetIndex + (arg0->frameCounter >> 1));
        }
        arg0->frameCounter++;
        if (arg0->frameCounter >= 0xA) {
            terminateCurrentTask();
        }
    }

    for (i = 0; i < 4; i++) {
        enqueueTexturedBillboardSprite(i, &arg0->particle);
    }
}

void cleanupImpactStar(void **arg0) {
    *arg0 = freeNodeMemory(*arg0);
}

void spawnImpactStar(Vec3i *arg0) {
    Node *task;

    task = scheduleTask(&initImpactStarTask, 2, 0, 0xFA);
    if (task != NULL) {
        memcpy(&task->freeNext, arg0, sizeof(Vec3i));
    }
}

void initFloatingItemSpriteTask(FloatingItemSpriteTask *arg0) {
    arg0->sprite.spriteData = load_3ECE40();
    arg0->sprite.frameIndex = 0x45;
    arg0->frameCounter = 0;
    setCleanupCallback(&cleanupFloatingItemSpriteTask);
    setCallbackWithContinue(&updateFloatingItemSprite);
}

void updateFloatingItemSprite(FloatingItemSpriteTask *arg0) {
    arg0->sprite.frameIndex = (arg0->frameCounter >> 1) + 0x45;
    arg0->frameCounter = arg0->frameCounter + 1;

    if (arg0->frameCounter == 0x10) {
        terminateCurrentTask();
    }

    if (arg0->halfSizeRender == 0) {
        pushViewportCallbackBySlot(
            arg0->renderPriority,
            VIEWPORT_CALLBACK_LAYER_OPAQUE,
            renderSpriteFrameWithPalette,
            &arg0->sprite
        );
    } else {
        pushViewportCallbackBySlot(
            arg0->renderPriority,
            VIEWPORT_CALLBACK_LAYER_OPAQUE,
            renderHalfSizeSpriteWithCustomPalette,
            &arg0->sprite
        );
    }
}

void cleanupFloatingItemSpriteTask(FloatingItemSpriteTask *arg0) {
    arg0->sprite.spriteData = freeNodeMemory(arg0->sprite.spriteData);
}

void spawnFloatingItemSprite(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    FloatingItemSpriteTask *task;

    task = (FloatingItemSpriteTask *)scheduleTask(&initFloatingItemSpriteTask, 2, 0, 0xE6);
    if (task != NULL) {
        task->sprite.x = arg0;
        task->sprite.y = arg1;
        if (arg2 != 0) {
            task->sprite.paletteIndex = 0x11;
        } else {
            task->sprite.paletteIndex = 0x10;
        }
        task->renderPriority = arg3;
        task->halfSizeRender = arg4;
    }
}

void initDualSnowSprayTask_SingleSlot(DualSnowSprayTask *arg0) {
    GameState *gs = (GameState *)getCurrentAllocation();
    arg0->assetTable = load_3ECE40();
    arg0->particles[0].vertices = gs->unk44->dualSnowSprayVertices;
    arg0->particles[0].alpha = (u8)((randA() & 0x1F) + 0x70);
    arg0->frameCounter = 0;
    arg0->particles[1].vertices = arg0->particles[0].vertices;
    arg0->particles[1].alpha = arg0->particles[0].alpha;
    setCleanupCallback(&cleanupDualSnowSprayTask);
    setCallbackWithContinue(&updateDualSnowSprayParticles_SingleSlot);
}

void updateDualSnowSprayParticles_SingleSlot(DualSnowSprayTask *arg0) {
    GameState *gs;
    s32 i;

    gs = (GameState *)getCurrentAllocation();
    loadAssetMetadata(&arg0->particles[0], arg0->assetTable, (arg0->frameCounter / 4) + 8);

    arg0->particles[1].textureData = arg0->particles[0].textureData;
    arg0->particles[1].paletteData = arg0->particles[0].paletteData;
    arg0->particles[1].textureWidth = arg0->particles[0].textureWidth;
    arg0->particles[1].textureHeight = arg0->particles[0].textureHeight;

    for (i = 0; i < 4; i++) {
        enqueueAlphaSprite(i, &arg0->particles[0]);
        enqueueAlphaSprite(i, &arg0->particles[1]);
    }

    if (gs->gamePaused == 0) {
        if (arg0->frameCounter != 0) {
            for (i = 0; i < 2; i++) {
                arg0->particles[i].position.x += arg0->velocity.x;
                arg0->particles[i].position.y += arg0->velocity.y;
                arg0->particles[i].position.z += arg0->velocity.z;
            }
        }

        arg0->frameCounter++;
        if (arg0->frameCounter == 0x14) {
            terminateCurrentTask();
        }
    }
}

void cleanupDualSnowSprayTask(DualSnowSprayTask *arg0) {
    arg0->assetTable = freeNodeMemory(arg0->assetTable);
}

void spawnDualSnowSprayEffect_SingleSlot(Vec3i *pos1, Vec3i *pos2, Vec3i *velocity, s32 particleType) {
    DualSnowSprayTask *task = (DualSnowSprayTask *)scheduleTask(&initDualSnowSprayTask_SingleSlot, 2, 0, 0xDD);
    if (task != NULL) {
        memcpy(&task->particles[0].position, pos1, sizeof(Vec3i));
        memcpy(&task->particles[1].position, pos2, sizeof(Vec3i));
        task->baseAssetIndex = particleType;
        task->velocity.x = (s32)(velocity->x / 2);
        task->velocity.y = (s32)(velocity->y / 2);
        task->velocity.z = (s32)(velocity->z / 2);
    }
}

void initSkiTrailTask(SkiTrailTask *task) {
    GameState *gs;
    Vtx *particleAsset;
    volatile SkiTrailTask *skiSlot;
    s32 i;
    s32 transformOutputOffset;
    s16 *transforms;

    gs = (GameState *)getCurrentAllocation();
    task->assetTable = load_3ECE40();
    particleAsset = gs->unk44->skiTrailVertices;
    task->particleLeft.alpha = 0xFF;
    task->particleLeft.vertices = particleAsset;
    task->particleRight.alpha = task->particleLeft.alpha;
    task->particleRight.vertices = task->particleLeft.vertices;

    i = 0;

    if (task->player->animationFlags & 2) {
        skiSlot = task;
        transformOutputOffset = (u32) & ((SkiTrailTask *)0)->skiOffsets;
        transforms = gSkiTrailOffsetTransformsForward;
        do {
            transformVector(
                transforms,
                (s16 *)&task->player->snowboardDisplayObject.transform,
                (Vec3i *)((u8 *)task + transformOutputOffset)
            );
            skiSlot->skiOffsets[0].x -= task->player->worldPos.x;
            skiSlot->skiOffsets[0].y -= task->player->worldPos.y;
            transformOutputOffset += sizeof(Vec3i);
            transforms += 6;
            i++;
            skiSlot->skiOffsets[0].z -= task->player->worldPos.z;
            skiSlot = (SkiTrailTask *)&skiSlot->particleLeft.position;
        } while (i < 2);
    } else {
        skiSlot = task;
        transformOutputOffset = (u32) & ((SkiTrailTask *)0)->skiOffsets;
        transforms = gSkiTrailOffsetTransformsBackward;
        do {
            transformVector(
                transforms,
                (s16 *)&task->player->snowboardDisplayObject.transform,
                (Vec3i *)((u8 *)task + transformOutputOffset)
            );
            skiSlot->skiOffsets[0].x -= task->player->worldPos.x;
            skiSlot->skiOffsets[0].y -= task->player->worldPos.y;
            transformOutputOffset += sizeof(Vec3i);
            transforms += 6;
            i++;
            skiSlot->skiOffsets[0].z -= task->player->worldPos.z;
            skiSlot = (SkiTrailTask *)&skiSlot->particleLeft.position;
        } while (i < 2);
    }

    task->frameCounter = 0;
    task->particleIndex = -1;

    for (i = 0; i < 8; i++) {
        if ((task->player->tricksPerformedMask >> i) & 1) {
            task->particleIndex++;
        }
    }

    task->particleIndex += 0x15;
    setCleanupCallback(cleanupSkiTrailTask);
    setCallbackWithContinue(updateSkiTrailTask);
}

void updateSkiTrailTask(SkiTrailTask *task) {
    GameState *gs;
    s32 i;

    gs = (GameState *)getCurrentAllocation();
    loadAssetMetadataByIndex(
        (BillboardSprite *)&task->particleLeft,
        task->assetTable,
        task->frameCounter + 0x61,
        task->particleIndex
    );

    task->particleRight.textureData = task->particleLeft.textureData;
    task->particleRight.paletteData = task->particleLeft.paletteData;
    task->particleRight.textureWidth = task->particleLeft.textureWidth;
    task->particleRight.textureHeight = task->particleLeft.textureHeight;

    task->particleLeft.position.x = task->skiOffsets[0].x + task->player->worldPos.x;
    task->particleLeft.position.y = task->skiOffsets[0].y + task->player->worldPos.y;
    task->particleLeft.position.z = task->skiOffsets[0].z + task->player->worldPos.z;
    task->particleRight.position.x = task->skiOffsets[1].x + task->player->worldPos.x;
    task->particleRight.position.y = task->skiOffsets[1].y + task->player->worldPos.y;
    task->particleRight.position.z = task->skiOffsets[1].z + task->player->worldPos.z;

    for (i = 0; i < 4; i++) {
        enqueueAlphaSprite(i, &task->particleLeft);
        enqueueAlphaSprite(i, &task->particleRight);
    }

    if (gs->gamePaused == 0) {
        task->particleLeft.alpha -= 0x14;
        task->particleRight.alpha -= 0x14;
        task->frameCounter++;
        if (task->frameCounter == 8) {
            terminateCurrentTask();
        }
    }
}

void cleanupSkiTrailTask(SkiTrailTask *task) {
    task->assetTable = freeNodeMemory(task->assetTable);
}

void spawnSkiTrailTask(Player *player) {
    SkiTrailTask *task = (SkiTrailTask *)scheduleTask(&initSkiTrailTask, 2, 0, 0xDD);
    if (task != NULL) {
        task->player = player;
    }
}

void initGlintEffect(GlintEffectTask *arg0) {
    getCurrentAllocation();
    arg0->assetTable = load_3ECE40();
    arg0->particle.vertices = (Vtx *)&gGlintEffectAssetTemplate;
    arg0->particle.alpha = 0xFF;
    loadAssetMetadata(&arg0->particle, arg0->assetTable, 0x6A);
    setCleanupCallback(&cleanupGlintEffect);
    setCallbackWithContinue(&updateGlintEffect);
}

void updateGlintEffect(GlintEffectTask *arg0) {
    s32 i;
    GameState *gs;

    gs = (GameState *)getCurrentAllocation();
    transformVector(
        (s16 *)&gGlintEffectTransform,
        arg0->sourcePlayer->bodyPartDisplayObjects[5].transform.m[0],
        &arg0->particle.position
    );

    for (i = 0; i < 4; i++) {
        enqueueAlphaSprite(i, &arg0->particle);
    }

    if (gs->gamePaused == 0) {
        arg0->particle.alpha -= 0x10;
        if (arg0->particle.alpha < 0x40) {
            terminateCurrentTask();
        }
    }
}

void cleanupGlintEffect(GlintEffectTask *arg0) {
    arg0->assetTable = freeNodeMemory(arg0->assetTable);
}

void spawnGlintEffect(Player *player) {
    GlintEffectTask *task = (GlintEffectTask *)scheduleTask(&initGlintEffect, 2, 0, 0xDD);
    if (task != NULL) {
        task->sourcePlayer = player;
    }
}

void initCharacterAttackEffectState(CharacterAttackEffectState *task) {
    task->assetTable = load_3ECE40();
    setCleanupCallback(&cleanupCharacterAttackEffectTask);
    setCallbackWithContinue(&loadCharacterAttackEffectAssets);
}

void loadCharacterAttackEffectAssets(CharacterAttackEffectState *arg0) {
    s32 i;

    for (i = 0; i < 6; i++) {
        loadAssetMetadata(&arg0->particles[i], arg0->assetTable, arg0->baseAssetIndex);
        arg0->particles[i].vertices = (Vtx *)&gCharacterAttackEffectAssetTemplate;

        if (arg0->isVariant == 0) {
            memcpy(&arg0->positionOffsets, &gCharacterAttackEffectPositionOffsetsA, sizeof(Vec3i));
            arg0->particles[i].alpha = 0x90;
        } else {
            memcpy(&arg0->positionOffsets, &gCharacterAttackEffectPositionOffsetsB, sizeof(Vec3i));
            arg0->particles[i].alpha = 0xF0;
        }
    }

    arg0->frameCounter = 0;
    setCallbackWithContinue(&updateCharacterAttackEffect);
}

void updateCharacterAttackEffect(CharacterAttackEffectState *arg0) {
    Vec3i result;
    Transform3D rotMatrix;
    Transform3D transformed;
    GameState *alloc;
    BillboardSprite *particle;
    Transform3D *rm;
    Transform3D *tf;
    s32 yOffset;
    s32 i;
    s32 j;
    u8 fc;

    alloc = (GameState *)getCurrentAllocation();
    j = 0;

    if (alloc->gamePaused != 0) {
        goto render;
    }

    arg0->positionOffsets.x += gCharacterAttackEffectPositionOffsetsA.x;
    rm = &rotMatrix;
    arg0->positionOffsets.y += gCharacterAttackEffectPositionOffsetsA.y;
    tf = &transformed;
    yOffset = 0x80000;
    arg0->positionOffsets.z += gCharacterAttackEffectPositionOffsetsA.z;

    for (j = 0; j < 6; j++) {
        createYRotationMatrix(rm, (j * 0x555) & 0xFFFF);
        matrixMultiply(rm, &arg0->sourcePlayer->orientationHeadingTransform, tf);
        transformVector2(&arg0->positionOffsets, tf->m, &result);
        arg0->particles[j].position.x = result.x + arg0->sourcePlayer->worldPos.x;
        arg0->particles[j].position.y = result.y + arg0->sourcePlayer->worldPos.y + yOffset;
        arg0->particles[j].position.z = result.z + arg0->sourcePlayer->worldPos.z;

        if (arg0->isVariant == 0) {
            arg0->particles[j].alpha = arg0->particles[j].alpha - 9;
        } else {
            arg0->particles[j].alpha = arg0->particles[j].alpha - 21;
        }
    }

    fc = arg0->frameCounter;
    if (!(fc & 1)) {
        loadAssetMetadata(&arg0->particles[0], arg0->assetTable, arg0->baseAssetIndex + ((s32)(fc << 24) >> 26));
        for (j = 1; j < 6; j++) {
            arg0->particles[j].textureData = arg0->particles[0].textureData;
            arg0->particles[j].paletteData = arg0->particles[0].paletteData;
            arg0->particles[j].textureWidth = arg0->particles[0].textureWidth;
            arg0->particles[j].textureHeight = arg0->particles[0].textureHeight;
        }
    }

    fc = arg0->frameCounter + 1;
    arg0->frameCounter = fc;
    j = 0;
    if ((s8)fc == 10) {
        terminateCurrentTask();
    }

render:
    i = 0;
    do {
        particle = arg0->particles;
        do {
            enqueueAlphaBillboardSprite(j, particle);
            i++;
            particle++;
        } while (i < 6);
        j++;
        i = 0;
    } while (j < 4);
}

void cleanupCharacterAttackEffectTask(CharacterAttackEffectState *task) {
    task->assetTable = freeNodeMemory(task->assetTable);
}

void spawnCharacterAttackEffect(Player *player) {
    CharacterAttackEffectState *task =
        (CharacterAttackEffectState *)scheduleTask(&initCharacterAttackEffectState, 2, 0, 0xE7);
    if (task != NULL) {
        task->sourcePlayer = player;
        task->baseAssetIndex = 0x12;
        task->isVariant = 0;
    }
}

void spawnCharacterAttackEffectByType(Player *player, s32 characterId) {
    s32 particleType;
    CharacterAttackEffectState *task;

    particleType = gCharacterParticleTypeMap[characterId];
    if (particleType == 0xFF) {
        particleType = 0xD;
    }

    task = (CharacterAttackEffectState *)scheduleTask(&initCharacterAttackEffectState, 2, 0, 0xE7);
    if (task != NULL) {
        task->sourcePlayer = player;
        task->baseAssetIndex = particleType;
        task->isVariant = 1;
    }
}
