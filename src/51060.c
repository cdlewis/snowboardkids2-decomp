#include "51060.h"
#include "10AD0.h"
#include "36BE0.h"
#include "5E590.h"
#include "common.h"
#include "displaylist.h"
#include "gamestate.h"
#include "geometry.h"
#include "graphics.h"
#include "race_session.h"
#include "rand.h"
#include "task_scheduler.h"

typedef struct {
    DataTable_19E80 *assetTable;
    loadAssetMetadata_arg particle;
    u8 padding2[0x10];
    s32 particleType;
    s32 iteration;
    s32 velX;
} SprayEffectTask;

typedef struct {
    s32 padding0;
    loadAssetMetadata_arg particle;
    s32 padding2;
    s32 velX;
    s32 velY;
    s32 velZ;
    s32 padding3;
    s32 iteration;
} SprayEffectUpdateTask;

typedef struct {
    loadAssetMetadata_arg lam;
    u8 padding[0x4];
} AssetWrapper;

typedef struct {
    MemoryAllocatorNode *assetTable;
    AssetWrapper assets[2];
    s32 velX;
    s32 velY;
    s32 velZ;
    s16 frameCounter;
    s16 padding;
    s16 particleType;
} DualSnowSprayUpdateTask;

typedef struct {
    MemoryAllocatorNode *assetTable;
    void *particleSlot;
    Vec3i pos1;
    u8 padding1[0xA];
    s8 alpha1;
    u8 padding2[0x5];
    void *particleSlot2;
    Vec3i pos2;
    u8 padding3[0xA];
    s8 alpha2;
    u8 padding4[0x5];
    s32 velX;
    s32 velY;
    s32 velZ;
    s16 frameCounter;
    s16 slotIndex;
    s16 particleType;
} DualSnowSprayTask;

typedef struct {
    s16 x;
    s16 y;
    MemoryAllocatorNode *assetTable;
    s16 baseAssetIndex;
    s8 assetType;
    u8 paddingB;
    s32 paddingC;
    s32 frameCounter;
    u16 renderPriority;
    s16 halfSizeRender;
} FloatingItemSpriteTask;

typedef struct {
    u8 padding[0x164];
    s16 rotation[3];
} GlintEffectSource;

typedef struct {
    GlintEffectSource *source;
    void *assetTable;
    loadAssetMetadata_arg particle;
} GlintEffectTask;

typedef struct {
    s32 unk0;
    void *unk4;
} func_80051688_52288_arg;

typedef struct {
    Player *player;
    DataTable_19E80 *assetTable;
    loadAssetMetadata_arg particleLeft;
    u8 padding24[0x4];
    loadAssetMetadata_arg particleRight;
    u8 padding44[0x4];
    Vec3i skiOffsets[2];
    s16 frameCounter;
    s16 particleIndex;
} SkiTrailTask;

typedef struct {
    u8 padding[0xC0];
    MemoryAllocatorNode *assetTable;
} CharacterAttackEffectTask;

typedef struct {
    Node n;
    s32 unk2C;
    s32 unk30;
} NodeWithPayload;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} SprayEffectSpawnTask;

typedef struct {
    MemoryAllocatorNode *assetTable;
} DualSnowSprayAssetNode;

typedef struct {
    u8 padding[0xC4];
    void *sourcePlayer;
    u8 padding2[0xC];
    u8 particleType;
    u8 padding3;
    u8 isVariant;
} CharacterAttackEffectSpawnTask;

typedef struct {
    u8 padding[0x12C0];
    void *unk12C0;
} GameStateUnk44_Ext;

typedef struct {
    u8 padding[0x4C];
    Vec3i *unk4C;
    u8 padding2[0x434 - 0x50];
    Vec3i *unk434;
} func_80050C00_51800_Task_unk34;

typedef struct {
    /* 0x00 */ void *assetTable;
    /* 0x04 */ loadAssetMetadata_arg particle;
    /* 0x20 */ u8 padding_20[0x4];
    /* 0x24 */ s16 particleType;
    /* 0x26 */ u16 animFrame;
    /* 0x28 */ s32 velX;
    /* 0x2C */ s32 velY;
    /* 0x30 */ s32 velZ;
    /* 0x34 */ func_80050C00_51800_Task_unk34 *sourceObj;
    /* 0x38 */ s16 positionSelector;
} CharacterTrailParticleTask;

typedef struct {
    u8 padding[0x44C];
    s32 velocity;
    s32 unk450;
    s32 unk454;
} func_80050C80_51880_arg0;

typedef struct {
    u8 padding[0x44];
    void *unk44;
} func_80050C80_51880_allocation;

typedef struct {
    void *unk0;
    void *unk4;
    u8 padding[0x1C];
    s32 unk24;
    s32 unk28;
} func_80050DB0_519B0_arg;

typedef struct {
    loadAssetMetadata_arg particle;
    u8 _pad2[0x4];
} CharacterAttackEffectParticle;

typedef struct {
    CharacterAttackEffectParticle particles[6];
    MemoryAllocatorNode *assetTable;
    void *unkC4;
    Vec3i positionOffsets;
    s8 particleType;
    u8 frameCounter;
    u8 isVariant; // 0 = variant A, 1 = variant B (different alpha and offsets)
} CharacterAttackEffectState;

void loadFirstSprayParticle(SprayEffectTask *);
void cleanupSprayEffect(void **);
void updateSprayEffect(SprayEffectUpdateTask *);
void initDualSnowSprayTask(DualSnowSprayTask *);
void initDualSnowSprayTask_SingleSlot(DualSnowSprayTask *);
void updateDualSnowSprayParticles(DualSnowSprayUpdateTask *);
void cleanupDualSnowSprayAssetNode(DualSnowSprayAssetNode *);
void initCharacterTrailParticleTask(MemoryAllocatorNode **);
void loadCharacterTrailParticleAsset(CharacterTrailParticleTask *);
void updateCharacterTrailParticle(CharacterTrailParticleTask *);
void cleanupCharacterTrailParticleTask(s32 **);

void func_80050DB0_519B0(func_80050DB0_519B0_arg *);
void func_80050E08_51A08(func_80050DB0_519B0_arg *);
void func_80050EA0_51AA0(void **);
void updateFloatingItemSprite(FloatingItemSpriteTask *);
void cleanupFloatingItemSpriteTask(FloatingItemSpriteTask *);
void updateDualSnowSprayParticles_SingleSlot(DualSnowSprayUpdateTask *);
void cleanupDualSnowSprayTask(DualSnowSprayTask *);
void updateGlintEffect(GlintEffectTask *);
void cleanupGlintEffect(GlintEffectTask *);
void loadCharacterAttackEffectAssets(CharacterAttackEffectState *);
void cleanupCharacterAttackEffectTask(CharacterAttackEffectTask *);

extern loadAssetMetadata_arg gGlintEffectAssetTemplate;
extern loadAssetMetadata_arg gCharacterAttackEffectAssetTemplate;
extern Vec3i gCharacterAttackEffectPositionOffsetsA;
extern Vec3i gCharacterAttackEffectPositionOffsetsB;
extern u8 gCharacterParticleTypeMap[];
extern s16 gSkiTrailOffsetTransformsForward[];
extern s16 gSkiTrailOffsetTransformsBackward[];
extern s16 gGlintEffectTransform[];
extern u16 D_8009ADE0_9B9E0;

void initSprayEffectTask(void **node) {
    *node = load_3ECE40();
    setCleanupCallback(&cleanupSprayEffect);
    setCallbackWithContinue(&loadFirstSprayParticle);
}

void loadFirstSprayParticle(SprayEffectTask *arg0) {
    GameState *gs = (GameState *)getCurrentAllocation();
    loadAssetMetadata(&arg0->particle, arg0->assetTable, arg0->particleType);
    arg0->particle.alpha = 0xE0;
    arg0->particle.assetTemplate = &gs->unk44->unkFC0->asset;
    arg0->iteration = 0;
    setCallbackWithContinue(&updateSprayEffect);
}

void updateSprayEffect(SprayEffectUpdateTask *arg0) {
    GameState *gs;
    s32 i;
    gs = (GameState *)getCurrentAllocation();
    i = 0;

    if (gs->gamePaused == 0) {
        GameStateUnk44 *base;

        base = gs->unk44;
        arg0->particle.assetTemplate = &base->unkFC0[arg0->iteration].asset;

        arg0->iteration = arg0->iteration + 1;
        if (arg0->iteration == 4) {
            terminateCurrentTask();
            return;
        }
        arg0->particle.alpha = arg0->particle.alpha - 0x30;
        arg0->particle.position.x += arg0->velX;
        arg0->particle.position.y += arg0->velY;
        arg0->particle.position.z += arg0->velZ;
    }

    for (i = 0; i < 4; i++) {
        enqueueAlphaBillboardSprite(i, &arg0->particle);
    }
}

void cleanupSprayEffect(void **arg0) {
    *arg0 = freeNodeMemory(*arg0);
}

void spawnSprayEffect(Vec3i *arg0, Vec3i *arg1, s32 arg2) {
    NodeWithPayload *task = (NodeWithPayload *)scheduleTask(&initSprayEffectTask, 2, 0, 0xDC);
    if (task != NULL) {
        memcpy((void *)&task->n.freeNext, arg0, sizeof(Vec3i));
        task->unk30 = arg2;
        task->n.cleanupCallback = (void *)(arg1->x / 2);
        task->n.payload = (void *)(arg1->y / 2);
        task->unk2C = (arg1->z / 2);
    }
}

void initDualSnowSprayTask(DualSnowSprayTask *arg0) {
    GameState *gs = (GameState *)getCurrentAllocation();
    arg0->assetTable = load_3ECE40();
    arg0->particleSlot = &gs->unk44->unk1080[arg0->slotIndex];
    arg0->alpha1 = (u8)((randA() & 0x1F) + 0x70);
    arg0->frameCounter = 0;
    arg0->particleSlot2 = arg0->particleSlot;
    arg0->alpha2 = (u8)arg0->alpha1;
    setCleanupCallback(&cleanupDualSnowSprayAssetNode);
    setCallbackWithContinue(&updateDualSnowSprayParticles);
}

void updateDualSnowSprayParticles(DualSnowSprayUpdateTask *arg0) {
    GameState *gs;
    s32 i;

    gs = (GameState *)getCurrentAllocation();
    loadAssetMetadata(&arg0->assets[0].lam, arg0->assetTable, arg0->particleType + arg0->frameCounter);

    arg0->assets[1].lam.data_ptr = arg0->assets[0].lam.data_ptr;
    arg0->assets[1].lam.index_ptr = arg0->assets[0].lam.index_ptr;
    arg0->assets[1].lam.unk18 = arg0->assets[0].lam.unk18;
    arg0->assets[1].lam.unk19 = arg0->assets[0].lam.unk19;

    for (i = 0; i < 4; i++) {
        enqueueAlphaSprite(i, &arg0->assets[0].lam);
        enqueueAlphaSprite(i, &arg0->assets[1].lam);
    }

    if (gs->gamePaused == 0) {
        if (arg0->frameCounter != 0) {
            for (i = 0; i < 2; i++) {
                arg0->assets[i].lam.position.x += arg0->velX;
                arg0->assets[i].lam.position.y += arg0->velY;
                arg0->assets[i].lam.position.z += arg0->velZ;
            }
        }

        arg0->frameCounter++;
        if (arg0->frameCounter == 5) {
            terminateCurrentTask();
        }
    }
}

void cleanupDualSnowSprayAssetNode(DualSnowSprayAssetNode *arg0) {
    arg0->assetTable = freeNodeMemory(arg0->assetTable);
}

void spawnDualSnowSprayEffect(Vec3i *pos1, Vec3i *pos2, Vec3i *velocity, s16 slotIndex, s32 characterId) {
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

    memcpy(&task->pos1, pos1, 0xC);
    memcpy(&task->pos2, pos2, 0xC);

    particleType = gCharacterParticleTypeMap[characterId];
    task->slotIndex = slotIndex;
    task->particleType = particleType;

    velX = velocity->x;
    signX = (u32)velX >> 31;
    velX += signX;
    velX >>= 1;
    task->velX = velX;

    velY = velocity->y;
    signY = (u32)velY >> 31;
    velY += signY;
    velY >>= 1;
    task->velY = velY;

    velZ = velocity->z;
    signZ = (u32)velZ >> 31;
    velZ += signZ;
    velZ >>= 1;
    task->velZ = velZ;
}

void initCharacterTrailParticleTask(MemoryAllocatorNode **node) {
    *node = load_3ECE40();
    setCleanupCallback(&cleanupCharacterTrailParticleTask);
    setCallbackWithContinue(&loadCharacterTrailParticleAsset);
}

void loadCharacterTrailParticleAsset(CharacterTrailParticleTask *arg0) {
    s32 temp;
    s32 shift9;
    int new_var2;
    s32 new_var;
    getCurrentAllocation();
    if (arg0->positionSelector >= 0) {
        memcpy(&arg0->particle.position, &arg0->sourceObj->unk4C, sizeof(Vec3i));
    } else {
        memcpy(&arg0->particle.position, &arg0->sourceObj->unk434, sizeof(Vec3i));
        arg0->particle.position.y += 0x80000;
    }
    temp = (randA() & 0xFF) - 0x80;
    shift9 = (new_var = temp << 9);
    arg0->particle.position.x += ((temp << 11) + shift9) << 1;
    new_var2 = (randA() & 0xFF) - 0x80;
    temp = new_var2;
    shift9 = temp << 9;
    arg0->animFrame = 0;
    arg0->particle.position.z += ((temp << 11) + shift9) << 1;
    loadAssetMetadata(&arg0->particle, arg0->assetTable, arg0->particleType);
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
            loadAssetMetadata((&arg0->particle), arg0->assetTable, arg0->particleType + (temp >> 2));
        }

        arg0->animFrame++;
        if ((s16)arg0->animFrame >= 0xF) {
            terminateCurrentTask();
        }

        arg0->particle.position.x += arg0->velX;
        arg0->particle.position.y += arg0->velY;
        arg0->particle.position.z += arg0->velZ;
    }

    i = 0;
    if (arg0->particle.alpha == 0xFF) {
        do {
            enqueueTexturedBillboardSprite(i, (TexturedBillboardSprite *)&arg0->particle);
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

void spawnCharacterTrailParticle(void *arg0) {
    GameState *allocation;
    CharacterTrailParticleTask *task;

    allocation = (GameState *)getCurrentAllocation();
    task = (CharacterTrailParticleTask *)scheduleTask(&initCharacterTrailParticleTask, 2, 0, 0xEA);
    if (task != NULL) {
        task->particleType = 0x35;
        task->sourceObj = arg0;
        task->particle.alpha = 0xFF;
        task->velX = 0;
        task->velY = 0;
        task->velZ = 0;
        task->positionSelector = 0;
        task->particle.assetTemplate = (loadAssetMetadata_arg *)&allocation->unk44->unkFC0;
    }
}

void spawnPlayerCharacterTrailParticle(Player *arg0, s32 arg1) {
    func_80050C80_51880_allocation *allocation;
    CharacterTrailParticleTask *task;
    u8 temp;

    allocation = (func_80050C80_51880_allocation *)getCurrentAllocation();
    temp = gCharacterParticleTypeMap[arg1];

    if (temp == 0xFF) {
        return;
    }

    if (D_8009ADE0_9B9E0 & 1) {
        return;
    }

    task = (CharacterTrailParticleTask *)scheduleTask(&initCharacterTrailParticleTask, 2, 0, 0xEA);

    if (task != NULL) {
        u8 temp2;
        task->sourceObj = (func_80050C00_51800_Task_unk34 *)arg0;
        temp2 = gCharacterParticleTypeMap[arg1];
        task->particle.alpha = 0x80;
        task->particleType = temp2;
        task->velX = arg0->velocity.x / 2;
        task->velY = arg0->velocity.y / 2;
        task->velZ = arg0->velocity.z / 2;
        task->positionSelector = -1;
        task->particle.assetTemplate = (void *)((u32)allocation->unk44 + 0x1440);
    }
}

void func_80050D70_51970(MemoryAllocatorNode **node) {
    *node = load_3ECE40();
    setCleanupCallback(&func_80050EA0_51AA0);
    setCallbackWithContinue(&func_80050DB0_519B0);
}

void func_80050DB0_519B0(func_80050DB0_519B0_arg *arg0) {
    GameState *allocation;
    void *temp;

    allocation = getCurrentAllocation();
    temp = &((GameStateUnk44_Ext *)allocation->unk44)->unk12C0;

    arg0->unk24 = 0x40;
    arg0->unk4 = temp;
    arg0->unk28 = 0;

    loadAssetMetadata((loadAssetMetadata_arg *)&arg0->unk4, arg0->unk0, arg0->unk24);

    setCallbackWithContinue(&func_80050E08_51A08);
}

void func_80050E08_51A08(func_80050DB0_519B0_arg *arg0) {
    GameState *alloc;
    s32 i;

    alloc = getCurrentAllocation();
    if (alloc->gamePaused == 0) {
        if ((arg0->unk28 & 1) == 0) {
            loadAssetMetadata((loadAssetMetadata_arg *)&arg0->unk4, arg0->unk0, arg0->unk24 + (arg0->unk28 >> 1));
        }
        arg0->unk28++;
        if (arg0->unk28 >= 0xA) {
            terminateCurrentTask();
        }
    }

    for (i = 0; i < 4; i++) {
        enqueueTexturedBillboardSprite(i, (TexturedBillboardSprite *)&arg0->unk4);
    }
}

void func_80050EA0_51AA0(void **arg0) {
    *arg0 = freeNodeMemory(*arg0);
}

void spawnImpactStar(Vec3i *arg0) {
    Node *task;

    task = scheduleTask(&func_80050D70_51970, 2, 0, 0xFA);
    if (task != NULL) {
        memcpy(&task->freeNext, arg0, sizeof(Vec3i));
    }
}

void initFloatingItemSpriteTask(FloatingItemSpriteTask *arg0) {
    arg0->assetTable = load_3ECE40();
    arg0->baseAssetIndex = 0x45;
    arg0->frameCounter = 0;
    setCleanupCallback(&cleanupFloatingItemSpriteTask);
    setCallbackWithContinue(&updateFloatingItemSprite);
}

void updateFloatingItemSprite(FloatingItemSpriteTask *arg0) {
    arg0->baseAssetIndex = (arg0->frameCounter >> 1) + 0x45;
    arg0->frameCounter = arg0->frameCounter + 1;

    if (arg0->frameCounter == 0x10) {
        terminateCurrentTask();
    }

    if (arg0->halfSizeRender == 0) {
        debugEnqueueCallback(arg0->renderPriority, 1, renderSpriteFrameWithPalette, arg0);
    } else {
        debugEnqueueCallback(arg0->renderPriority, 1, renderHalfSizeSpriteWithCustomPalette, arg0);
    }
}

void cleanupFloatingItemSpriteTask(FloatingItemSpriteTask *arg0) {
    arg0->assetTable = freeNodeMemory(arg0->assetTable);
}

void spawnFloatingItemSprite(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    FloatingItemSpriteTask *task;

    task = (FloatingItemSpriteTask *)scheduleTask(&initFloatingItemSpriteTask, 2, 0, 0xE6);
    if (task != NULL) {
        task->x = arg0;
        task->y = arg1;
        if (arg2 != 0) {
            task->assetType = 0x11;
        } else {
            task->assetType = 0x10;
        }
        task->renderPriority = arg3;
        task->halfSizeRender = arg4;
    }
}

void initDualSnowSprayTask_SingleSlot(DualSnowSprayTask *arg0) {
    GameState *gs = (GameState *)getCurrentAllocation();
    arg0->assetTable = load_3ECE40();
    arg0->particleSlot = &gs->unk44->unk1340;
    arg0->alpha1 = (u8)((randA() & 0x1F) + 0x70);
    arg0->frameCounter = 0;
    arg0->particleSlot2 = arg0->particleSlot;
    arg0->alpha2 = (u8)arg0->alpha1;
    setCleanupCallback(&cleanupDualSnowSprayTask);
    setCallbackWithContinue(&updateDualSnowSprayParticles_SingleSlot);
}

void updateDualSnowSprayParticles_SingleSlot(DualSnowSprayUpdateTask *arg0) {
    GameState *gs;
    s32 i;

    gs = (GameState *)getCurrentAllocation();
    loadAssetMetadata(&arg0->assets[0].lam, arg0->assetTable, (arg0->frameCounter / 4) + 8);

    arg0->assets[1].lam.data_ptr = arg0->assets[0].lam.data_ptr;
    arg0->assets[1].lam.index_ptr = arg0->assets[0].lam.index_ptr;
    arg0->assets[1].lam.unk18 = arg0->assets[0].lam.unk18;
    arg0->assets[1].lam.unk19 = arg0->assets[0].lam.unk19;

    for (i = 0; i < 4; i++) {
        enqueueAlphaSprite(i, &arg0->assets[0].lam);
        enqueueAlphaSprite(i, &arg0->assets[1].lam);
    }

    if (gs->gamePaused == 0) {
        if (arg0->frameCounter != 0) {
            for (i = 0; i < 2; i++) {
                arg0->assets[i].lam.position.x += arg0->velX;
                arg0->assets[i].lam.position.y += arg0->velY;
                arg0->assets[i].lam.position.z += arg0->velZ;
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
        memcpy(&task->pos1, pos1, 0xC);
        memcpy(&task->pos2, pos2, 0xC);
        task->particleType = particleType;
        task->velX = (s32)(velocity->x / 2);
        task->velY = (s32)(velocity->y / 2);
        task->velZ = (s32)(velocity->z / 2);
    }
}

void updateSkiTrailTask(SkiTrailTask *arg0);
void cleanupSkiTrailTask(func_80051688_52288_arg *arg0);

void initSkiTrailTask(SkiTrailTask *task) {
    GameState *gs;
    void *particleAsset;
    volatile SkiTrailTask *iter;
    s32 i;
    s32 offset;
    s16 *transforms;

    gs = (GameState *)getCurrentAllocation();
    task->assetTable = load_3ECE40();
    particleAsset = (void *)((u8 *)gs->unk44 + 0x13C0);
    task->particleLeft.alpha = 0xFF;
    task->particleLeft.assetTemplate = particleAsset;
    task->particleRight.alpha = task->particleLeft.alpha;
    task->particleRight.assetTemplate = task->particleLeft.assetTemplate;

    i = 0;

    if (task->player->unkB84 & 2) {
        iter = task;
        offset = 0x48;
        transforms = gSkiTrailOffsetTransformsForward;
        do {
            transformVector(transforms, (s16 *)&task->player->unk3F8, (void *)((u8 *)task + offset));
            iter->skiOffsets[0].x -= task->player->worldPos.x;
            iter->skiOffsets[0].y -= task->player->worldPos.y;
            offset += 0xC;
            transforms += 6;
            i++;
            iter->skiOffsets[0].z -= task->player->worldPos.z;
            iter = (volatile SkiTrailTask *)((u8 *)iter + 0xC);
        } while (i < 2);
    } else {
        iter = task;
        offset = 0x48;
        transforms = gSkiTrailOffsetTransformsBackward;
        do {
            transformVector(transforms, (s16 *)&task->player->unk3F8, (void *)((u8 *)task + offset));
            iter->skiOffsets[0].x -= task->player->worldPos.x;
            iter->skiOffsets[0].y -= task->player->worldPos.y;
            offset += 0xC;
            transforms += 6;
            i++;
            iter->skiOffsets[0].z -= task->player->worldPos.z;
            iter = (volatile SkiTrailTask *)((u8 *)iter + 0xC);
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
        (loadAssetMetadataByIndex_arg *)&task->particleLeft,
        task->assetTable,
        task->frameCounter + 0x61,
        task->particleIndex
    );

    task->particleRight.data_ptr = task->particleLeft.data_ptr;
    task->particleRight.index_ptr = task->particleLeft.index_ptr;
    task->particleRight.unk18 = task->particleLeft.unk18;
    task->particleRight.unk19 = task->particleLeft.unk19;

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

void cleanupSkiTrailTask(func_80051688_52288_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
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
    arg0->particle.assetTemplate = &gGlintEffectAssetTemplate;
    arg0->particle.alpha = 0xFF;
    loadAssetMetadata(&arg0->particle, arg0->assetTable, 0x6A);
    setCleanupCallback(&cleanupGlintEffect);
    setCallbackWithContinue(&updateGlintEffect);
}

void updateGlintEffect(GlintEffectTask *arg0) {
    s32 i;
    GameState *gs;

    gs = (GameState *)getCurrentAllocation();
    transformVector((s16 *)&gGlintEffectTransform, arg0->source->rotation, &arg0->particle.position);

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

void spawnGlintEffect(void *arg0) {
    void *result = scheduleTask(&initGlintEffect, 2, 0, 0xDD);
    if (result != NULL) {
        *(void **)result = arg0;
    }
}

void initCharacterAttackEffectTask(CharacterAttackEffectTask *task) {
    task->assetTable = load_3ECE40();
    setCleanupCallback(&cleanupCharacterAttackEffectTask);
    setCallbackWithContinue(&loadCharacterAttackEffectAssets);
}

void loadCharacterAttackEffectAssets(CharacterAttackEffectState *arg0) {
    s32 i;

    for (i = 0; i < 6; i++) {
        loadAssetMetadata(&arg0->particles[i].particle, arg0->assetTable, arg0->particleType);
        arg0->particles[i].particle.assetTemplate = &gCharacterAttackEffectAssetTemplate;

        if (arg0->isVariant == 0) {
            memcpy(&arg0->positionOffsets, &gCharacterAttackEffectPositionOffsetsA, 0xC);
            arg0->particles[i].particle.alpha = 0x90;
        } else {
            memcpy(&arg0->positionOffsets, &gCharacterAttackEffectPositionOffsetsB, 0xC);
            arg0->particles[i].particle.alpha = 0xF0;
        }
    }

    arg0->frameCounter = 0;
    setCallbackWithContinue(&updateCharacterAttackEffect);
}

INCLUDE_ASM("asm/nonmatchings/51060", updateCharacterAttackEffect);

void cleanupCharacterAttackEffectTask(CharacterAttackEffectTask *task) {
    task->assetTable = freeNodeMemory(task->assetTable);
}

void spawnCharacterAttackEffect(Player *player) {
    CharacterAttackEffectSpawnTask *task =
        (CharacterAttackEffectSpawnTask *)scheduleTask(&initCharacterAttackEffectTask, 2, 0, 0xE7);
    if (task != NULL) {
        task->sourcePlayer = player;
        task->particleType = 0x12;
        task->isVariant = 0;
    }
}

void spawnCharacterAttackEffectByType(Player *player, s32 characterId) {
    s32 particleType;
    CharacterAttackEffectSpawnTask *task;

    particleType = gCharacterParticleTypeMap[characterId];
    if (particleType == 0xFF) {
        particleType = 0xD;
    }

    task = (CharacterAttackEffectSpawnTask *)scheduleTask(&initCharacterAttackEffectTask, 2, 0, 0xE7);
    if (task != NULL) {
        task->sourcePlayer = player;
        task->particleType = particleType;
        task->isVariant = 1;
    }
}
