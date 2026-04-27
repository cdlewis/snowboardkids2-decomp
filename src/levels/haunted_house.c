#include "levels/haunted_house.h"
#include "audio/audio.h"
#include "common.h"
#include "data/asset_metadata.h"
#include "data/course_data.h"
#include "data/data_table.h"
#include "gamestate.h"
#include "gbi.h"
#include "graphics/displaylist.h"
#include "graphics/graphics.h"
#include "math/geometry.h"
#include "math/rand.h"
#include "race/obstacle_sprites.h"
#include "race/particle_items.h"
#include "race/race_session.h"
#include "race/track_collision.h"
#include "race/track_height.h"
#include "system/task_scheduler.h"

typedef struct {
    s16 unk0;
    s16 unk2;
} func_800BB74C_AF43C_arg;

typedef struct {
    /* 0x00 */ void *assetData;
    /* 0x04 */ void *assetMetadata;
    /* 0x08 */ s8 renderPositionBuffer[0x16];
    /* 0x1E */ u8 alpha;
    /* 0x1F */ s8 pad1F[0x5];
    /* 0x24 */ s32 posX;
    /* 0x28 */ s32 posY;
    /* 0x2C */ s32 posZ;
    /* 0x30 */ Vec3i velocity;
    /* 0x3C */ s16 entityType;
    /* 0x3E */ s16 collisionHeight;
    /* 0x40 */ s16 animTimer;
    /* 0x42 */ s16 lifetime;
    /* 0x44 */ s16 animFrameIndex;
    /* 0x46 */ u8 fadeDirection;
} AnimatedGhostEntity;

typedef struct {
    u8 pad[0x30];
    void *unk30;
} AllocationUnk30;

typedef struct {
    u8 pad[0x10];
    Player *players;
    u8 pad14[0x1C];
    void *gameData;
    u8 pad34[0x2A];
    u8 memoryPoolId;
    u8 pad5F[0x17];
    u8 gamePaused;
} Allocation;

typedef struct {
    u8 pad[0x24];
    s32 unk24;
} Task;

typedef struct {
    /* 0x00 */ void *spriteAsset;
    /* 0x04 */ void *spriteMetadata;
    /* 0x08 */ s32 currentX;
    /* 0x0C */ s32 currentY;
    /* 0x10 */ s32 currentZ;
    /* 0x14 */ u8 pad14[0xA];
    /* 0x1E */ u8 alpha;
    /* 0x1F */ s8 pad1F[0x5];
    /* 0x24 */ s32 yOffset;
    /* 0x28 */ s16 lifetime;
} FloatingBillboard;

typedef struct {
    s8 frameDuration;
    s8 assetIndex;
} AnimationData;

typedef struct {
    void *matrixData;
    void *textureTable;
    u8 textureIndices[8];
} GhostRenderState;

typedef struct {
    /* 0x00 */ u8 _pad[0x24];
    /* 0x24 */ s16 variantIndex;
} FloatingSpriteTask;

typedef struct {
    void *spriteAsset;
    void *spriteMetadata;
    s32 posX;
    s32 posY;
    s32 posZ;
    u8 pad14[0x10];
    s16 variantIndex;
    s16 animPhase;
} FloatingSpriteEntity;

typedef struct {
    s32 x;
    s32 z;
} GhostSpawnPos;

typedef struct {
    void *unk00;
    void *unk04;
    s8 unk08[0x16];
    u8 unk1E;
    s8 unk1F[0x5];
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s16 unk3C;
    s16 unk3E;
    s16 unk40;
    s16 unk42;
    s16 unk44;
    u8 unk46;
} initAnimatedGhost_arg;

u32 g_GhostDefaultAssetMetadata[] = {
    0xFFED0013, 0x00000000, 0xFFF0FFF0, 0xFFFFFFFF, 0x00130013, 0x00000000, 0x07F0FFF0, 0xFFFFFFFF,
    0x0013FFED, 0x00000000, 0x07F007F0, 0xFFFFFFFF, 0xFFEDFFED, 0x00000000, 0xFFF007F0, 0xFFFFFFFF,
};

AnimationData D_800BC830_B0520[] = {
    { 0x0F, 0x3A },
    { 0x0F, 0x3B },
    { 0x0F, 0x3A },
    { 0x0F, 0x3B },
    { 0x1E, 0x3C },
    { 0x0F, 0x3D },
    { 0x0F, 0x3E },
    { 0x0F, 0x3D },
    { 0x0F, 0x3E },
    { 0x00, 0x00 },
};

GhostSpawnPos D_800BC844_B0534[] = {
    { 0x135B7BF1, 0x1BCF280C },
    { 0x1441AFC1, 0x1CEC431C },
    { 0x13AE7075, 0x1DC5AD88 },
    { 0x13FD7205, 0x1E6CEDA8 },
    { 0x145E1875, 0x1ED15CC8 },
    { 0x13AD37D3, 0x1F6A2736 },
    { 0x1365C56F, 0x1EC52634 },
    { 0x14795965, 0x1D7E3A1C },
};

GhostSpawnPos D_800BC884_B0574[] = {
    { 0xFA0C7CB8,      (s32)0xD326E050 },
    { (s32)0xFA3DC7A8, (s32)0xD22E1B38 },
    { (s32)0xFADC44E0, (s32)0xD290BBB0 },
    { (s32)0xFB5F98B4, (s32)0xD2FEF6E0 },
    { (s32)0xFBA27952, (s32)0xD2089E18 },
    { (s32)0xFABF4C4C, (s32)0xD1AA89F8 },
    { (s32)0xF9A85FCA, (s32)0xD1D046C8 },
    { (s32)0xFA3FD23C, (s32)0xD3366B26 },
};
s32 D_800BC884_B0574_pad = 0;

u32 g_FloatingBillboardSpriteMetadata[] = {
    0xFFE00020, 0x00000000, 0xFFF0FFF0, 0xFFFFFFFF, 0x00200020, 0x00000000, 0x07F0FFF0, 0xFFFFFFFF,
    0x0020FFE0, 0x00000000, 0x07F007F0, 0xFFFFFFFF, 0xFFE0FFE0, 0x00000000, 0xFFF007F0, 0xFFFFFFFF,
};

s8 g_FloatingBillboardInitialPos[12] = {
    0x1D, 0x98, (s8)0xFF, (s8)0xB8, 0x26, 0x5A, (s8)0x9F, 0x14, 0x13, (s8)0x84, 0x6D, 0x2F,
};

Vec3i g_FloatingBillboardTarget = { 0x17A8A90D, 0x25A45868, 0x084B69BF };

u32 D_800BC920_B0610[] = {
    0xFFE80030, 0x00000000, 0xFFF0FFF0, 0xFFFFFFFF, 0x00180030, 0x00000000, 0x07F0FFF0, 0xFFFFFFFF,
    0x00180000, 0x00000000, 0x07F007F0, 0xFFFFFFFF, 0xFFE80000, 0x00000000, 0xFFF007F0, 0xFFFFFFFF,
};

u32 D_800BC960_B0650[] = {
    0xFFE80030, 0x00000000, 0xFFF0FFF0, 0xFFFFFFFF, 0x00180030, 0x00000000, 0x03F0FFF0, 0xFFFFFFFF,
    0x00180000, 0x00000000, 0x03F003F0, 0xFFFFFFFF, 0xFFE80000, 0x00000000, 0xFFF003F0, 0xFFFFFFFF,
};

s32 D_800BC9A0_B0690[] = {
    0x133ABF3D, 0x24530114, 0x0951ED11, 0x12186D79, 0x242698B5, 0x0923670F, 0x11063625, 0x23F96B6F,
    0x08431DD1, 0x11E788F1, 0x23BB7EAD, 0x0669EF7D, 0x1088584D, 0x23A2F6F8, 0x05D11829,
};

s16 D_800BC9DC_B06CC[] = { 0x0006, 0x0006, 0x0006, 0x0006, 0x0007, 0x0000 };

s16 D_800BC9E8_B06D8[] = { 0x0060, 0x0000, 0x0060, 0x0000, 0x0060, 0x0000 };

s16 D_800BC9F4_B06E4[] = { 0x0080, 0x0000, 0x0070, 0x0000, 0x0060, 0x0000 };

Vec3i g_GhostSpawnPositions[] = {
    { 0x0588A7A0, 0x2E2CB884, 0x29185A96 },
    { 0x04D56484, 0x2E2CC575, 0x27EDF564 },
    { 0x06917BE4, 0x2DDB7EE8, 0x2876FA08 },
    { 0x05DE5088, 0x2DDB9103, 0x274CF0E8 },
    { 0x078E2B74, 0x2D93BB4B, 0x27CB5A48 },
    { 0x06E97406, 0x2D925515, 0x26AC8DEA },
    { 0x089C64C4, 0x2D4E0E65, 0x272B2B28 },
    { 0x07ECB6AC, 0x2D4D6040, 0x25FEEB00 },
};

Vtx D_800BCA60_B0750[] = {
    { { { -16, 8, 0 }, 0, { -16, -16 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 16, 8, 0 }, 0, { 2032, -16 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 16, -8, 0 }, 0, { 2032, 1008 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -16, -8, 0 }, 0, { -16, 1008 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
};

Vec3i g_GhostBaseDirection = { 0, 0, 0 };
s32 g_GhostBaseDirection_pad = 0;

extern Gfx D_8009A780_9B380[];
extern s32 gLookAtPtr;
extern s16 gGraphicsMode;
extern Gfx *gRegionAllocPtr;

void initAnimatedGhost(AnimatedGhostEntity *);
void cleanupAnimatedGhost(void **);
void fadeInGhost(AnimatedGhostEntity *);
void oscillateGhostFade(AnimatedGhostEntity *);
void fadeOutGhost(AnimatedGhostEntity *);
void updateGhostSpawner(func_800BB74C_AF43C_arg *);
void updateSwingingPendulumTrap(SwingingPendulumTrap *);
void initFloatingBillboard(FloatingBillboard *);
void updateFloatingBillboardSpawner(s16 *);
void freeFloatingBillboard(void **);
void updateFloatingBillboard(FloatingBillboard *);
void initFloatingSpriteEntity(FloatingSpriteEntity *);
void updateFloatingSpriteEntity(FloatingSpriteEntity *);
void initGhostTransformations(GhostManager *);
void updateGhostSlotStates(u8 *ghostSlots);
void cleanupGhostManager(GhostManager *);
void updateLapCounter(s16 *);
void updateGhostAnimation(AnimatedGhostEntity *);
void renderGhosts(GhostRenderState *);
void cleanupFloatingSpriteEntity(void **);

void updateGhostAnimation(AnimatedGhostEntity *ghost) {
    s32 viewport;

    ghost->animTimer--;

    if (ghost->animTimer == 0) {
        loadAssetMetadata(
            (loadAssetMetadata_arg *)&ghost->assetMetadata,
            ghost->assetData,
            D_800BC830_B0520[ghost->animFrameIndex].assetIndex
        );

        ghost->animTimer = D_800BC830_B0520[ghost->animFrameIndex].frameDuration;
        ghost->animFrameIndex++;

        if (D_800BC830_B0520[ghost->animFrameIndex].frameDuration == 0) {
            ghost->animFrameIndex = 0;
        }
    }

    memcpy(&ghost->renderPositionBuffer, &ghost->posX, sizeof(Vec3i));

    for (viewport = 0; viewport < 4; viewport++) {
        enqueueAlphaBillboardSprite(viewport, (loadAssetMetadata_arg *)&ghost->assetMetadata);
    }
}

void initAnimatedGhost(AnimatedGhostEntity *ghost) {
    Vec3i direction;
    Vec3i *directionPtr;
    s32 randomSpeed;
    s32 rotationAngle;

    directionPtr = &direction;
    memcpy(directionPtr, &g_GhostBaseDirection, sizeof(Vec3i));

    getCurrentAllocation();

    ghost->assetData = load_3ECE40();
    ghost->assetMetadata = &g_GhostDefaultAssetMetadata;
    ghost->alpha = 0;

    randomSpeed = randA() & 0xFF;
    randomSpeed = randomSpeed << 10;
    direction.z = randomSpeed + 0x20000;

    rotationAngle = randA() & 0xFF;
    rotationAngle = rotationAngle << 5;

    rotateVectorY(directionPtr, rotationAngle, &ghost->velocity);

    if (ghost->entityType == 0) {
        ghost->collisionHeight = 0x1A;
    } else {
        ghost->collisionHeight = 0x58;
    }

    ghost->animTimer = 1;
    ghost->animFrameIndex = 0;
    ghost->lifetime = 0x3C;

    setCleanupCallback(cleanupAnimatedGhost);
    setCallbackWithContinue(fadeInGhost);
}

void cleanupAnimatedGhost(void **ghostEntity) {
    *ghostEntity = freeNodeMemory(*ghostEntity);
}

s32 updateGhostPositionAndCheckEnd(AnimatedGhostEntity *ghost) {
    Vec3i surfaceNormal;
    Allocation *allocation;
    void *collisionContext;
    s32 *posPtr;
    s32 shouldEnd;
    u16 newHeight;

    shouldEnd = 0;
    allocation = (Allocation *)getCurrentAllocation();

    if (allocation->gamePaused == 0) {
        collisionContext = &allocation->gameData;

        ghost->posX = ghost->posX + ghost->velocity.x;
        ghost->posZ = ghost->posZ + ghost->velocity.z;

        posPtr = &ghost->posX;

        newHeight = findTrackSector(collisionContext, ghost->collisionHeight, posPtr);
        ghost->collisionHeight = newHeight;

        func_80060CDC_618DC(collisionContext, newHeight, posPtr, 0x80000, &surfaceNormal);

        ghost->posY =
            getTrackHeightInSector(collisionContext, (u16)ghost->collisionHeight, posPtr, 0x100000) + 0x180000;

        if (ghost->entityType == 0) {
            shouldEnd = (ghost->collisionHeight != 0x1A);
        } else {
            u16 height = ghost->collisionHeight;
            if ((u16)(height - 0x58) >= 2) {
                shouldEnd = 1;
            }
        }

        if (surfaceNormal.x != 0) {
            shouldEnd = 1;
        } else if (surfaceNormal.z != 0) {
            shouldEnd = 1;
        }

        if (ghost->lifetime == 0) {
            shouldEnd = 1;
        } else {
            ghost->lifetime = ghost->lifetime - 1;
        }
    }

    return shouldEnd;
}

void fadeInGhost(AnimatedGhostEntity *ghost) {
    ghost->alpha += 0x10;

    if (ghost->alpha == 0xE0) {
        ghost->fadeDirection = 1;
        setCallback(oscillateGhostFade);
    }

    if (updateGhostPositionAndCheckEnd(ghost)) {
        setCallback(fadeOutGhost);
    }

    updateGhostAnimation(ghost);
}

void oscillateGhostFade(AnimatedGhostEntity *ghost) {
    Player *nearbyPlayer;

    if (ghost->fadeDirection != 0) {
        ghost->alpha -= 0x10;
        if (ghost->alpha < 0x41) {
            ghost->fadeDirection = 0;
        }
    } else {
        ghost->alpha += 0x10;
        if (ghost->alpha >= 0xE0) {
            ghost->fadeDirection = 1;
        }
    }

    if (updateGhostPositionAndCheckEnd(ghost) != 0) {
        setCallback(fadeOutGhost);
    }

    nearbyPlayer = findVulnerablePlayerNearPosition(&ghost->posX, -1, 0x100000);

    if (nearbyPlayer != NULL) {
        if (nearbyPlayer->slowdownLevel < 3) {
            spawnStarEffectImmediate(nearbyPlayer);
        }

        setCallback(fadeOutGhost);
    }

    updateGhostAnimation(ghost);
}

void fadeOutGhost(AnimatedGhostEntity *ghost) {
    ghost->alpha -= 0x10;

    if (ghost->alpha == 0) {
        terminateCurrentTask();
    } else {
        updateGhostPositionAndCheckEnd(ghost);
        updateGhostAnimation(ghost);
    }
}

void initGhostSpawnerTask(GhostSpawnerTask *spawner) {
    spawner->ghostType = 0;
    spawner->spawnTimer = 0x14;
    setCallback(updateGhostSpawner);
}

void updateGhostSpawner(func_800BB74C_AF43C_arg *arg0) {
    Allocation *allocation;
    int new_var;
    initAnimatedGhost_arg *task;
    s32 count;
    s32 inRange;
    s32 randIdx;
    s16 countdown;
    allocation = (Allocation *)getCurrentAllocation();
    if (allocation->gamePaused != 0) {
        return;
    }
    countdown = arg0->unk2;
    if (countdown == 0) {
        for (count = 0; count < allocation->memoryPoolId; count++) {
            if (arg0->unk0 == 0) {
                inRange = (allocation->players[count].sectorIndex - 0x18) < 6U;
            } else {
                inRange = (allocation->players[count].sectorIndex - 0x53) < 8U;
            }
            if (inRange != 0) {
                break;
            }
        }

        if (count < allocation->memoryPoolId) {
            task = (initAnimatedGhost_arg *)scheduleTask(initAnimatedGhost, 0, 0, 0xDC);
            if (task != 0) {
                if (arg0->unk0 == 0) {
                    task->unk3C = 0;
                    randIdx = randA();
                    randIdx &= 7;
                    count = randIdx;
                    task->unk24 = D_800BC844_B0534[count].x;
                    task->unk2C = D_800BC844_B0534[randIdx].z;
                } else {
                    task->unk3C = 1;
                    randIdx = randA();
                    randIdx &= 7;
                    count = randIdx;
                    task->unk24 = D_800BC884_B0574[count].x;
                    task->unk2C = D_800BC884_B0574[count].z;
                }
            }
        }
        arg0->unk2 = 4;
        arg0->unk0 = (arg0->unk0 + 1) & 1;
    } else {
        new_var = 1;
        arg0->unk2 = countdown - new_var;
    }
}

void initSwingingPendulumTrap(SwingingPendulumTrap *arg0) {
    LevelDisplayLists *temp_v0_3;
    u16 temp_a1;

    arg0->uncompressedAsset = loadUncompressedAssetByIndex(9);
    arg0->compressedAsset = loadCompressedSegment2AssetByIndex(9);
    arg0->animationFrame = 0;

    temp_v0_3 = getSkyDisplayLists3ByIndex(9);
    arg0->displayListState = &temp_v0_3->sceneryDisplayLists1;

    arg0->phaseTimer = 0xF;
    arg0->pivotX = 0xFD71ABE1;
    arg0->pivotY = 0x1E37049C;
    arg0->pivotZ = 0xF61F3676;
    arg0->bobPhase = 0;
    arg0->swingPhase = 0;
    arg0->swingAngle = 0x400;
    arg0->fireProjectileCooldown = 0x1E;

    temp_a1 = arg0->swingAngle + 0x2A0;
    createYRotationMatrix((Transform3D *)arg0, temp_a1 & 0xFFFF);
    setCleanupCallback(cleanupSwingingPendulumTrap);
    setCallback(updateSwingingPendulumTrap);
}

void updateSwingingPendulumTrap(SwingingPendulumTrap *arg0) {
    Allocation *allocation;
    s32 i;
    Vec3i *pos;
    Player *player;

    allocation = (Allocation *)getCurrentAllocation();

    if (allocation->gamePaused == 0) {
        switch (arg0->swingPhase) {
            case 0:
                arg0->phaseTimer--;
                if (arg0->phaseTimer != 0) {
                    break;
                }
                arg0->swingPhase++;
                /* fallthrough */
            case 1:
                arg0->fireProjectileCooldown--;
                arg0->swingAngle -= 0x10;
                if (arg0->swingAngle != -0x400) {
                    break;
                }
                arg0->phaseTimer = 0xF;
                arg0->swingPhase++;
                /* fallthrough */
            case 2:
                arg0->phaseTimer--;
                if (arg0->phaseTimer != 0) {
                    break;
                }
                arg0->swingPhase++;
                /* fallthrough */
            case 3:
                arg0->fireProjectileCooldown--;
                arg0->swingAngle += 0x10;
                if (arg0->swingAngle != 0x400) {
                    break;
                }
                arg0->swingPhase = 0;
                arg0->phaseTimer = 0xF;
                break;
        }

        arg0->bobPhase += 0x40;

        i = 0;
        if (allocation->memoryPoolId > i) {
            do {
                isPlayerInRangeAndPull(&arg0->position, 0x200000, allocation->players + i);
                i += 1;
            } while (i < allocation->memoryPoolId);
        }

        createYRotationMatrix((Transform3D *)arg0, (arg0->swingAngle + 0x2A0) & 0xFFFF);

        if (arg0->fireProjectileCooldown == 0) {
            arg0->fireProjectileCooldown = (randA() & 0xF) + 0xF;

            for (i = 0; i < allocation->memoryPoolId; i++) {
                player = allocation->players + i;
                if ((u16)(player->sectorIndex - 0x38) < 5) {
                    break;
                }
            }

            if (i < allocation->memoryPoolId) {
                pos = &arg0->position;
                spawnShrinkProjectileTask(0, (s16)(arg0->swingAngle + 0x2A0), pos);
                spawnShrinkProjectileTask(1, (s16)(arg0->swingAngle + 0x2A0), pos);
            }
        }
    }

    memcpy(&arg0->position, &arg0->pivotX, sizeof(Vec3i));
    arg0->position.y = arg0->position.y + approximateSin(arg0->bobPhase) * 0x1C0;

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
    }
}

void cleanupSwingingPendulumTrap(SwingingPendulumTrap *trap) {
    trap->uncompressedAsset = freeNodeMemory(trap->uncompressedAsset);
    trap->compressedAsset = freeNodeMemory(trap->compressedAsset);
}

void initFloatingBillboard(FloatingBillboard *billboard) {
    s32 initialY;

    billboard->spriteAsset = loadSpriteAssetByIndex(9);
    billboard->spriteMetadata = &g_FloatingBillboardSpriteMetadata;
    billboard->alpha = 0xFF;
    memcpy(&billboard->currentX, &g_FloatingBillboardInitialPos, 12);
    initialY = billboard->currentY + billboard->yOffset;
    billboard->lifetime = 0xB4;
    billboard->currentY = initialY;
    setCleanupCallback(freeFloatingBillboard);
    setCallbackWithContinue(updateFloatingBillboard);
}

void freeFloatingBillboard(void **billboard) {
    *billboard = freeNodeMemory(*billboard);
}

void updateFloatingBillboard(FloatingBillboard *arg0) {
    Allocation *allocation;
    s32 i;
    s32 *ptr8;
    s32 *ptrC;
    s32 *ptr10;

    allocation = getCurrentAllocation();

    if (allocation->gamePaused == 0) {
        arg0->lifetime -= 1;

        if (arg0->lifetime == 0) {
            terminateCurrentTask();
            return;
        }

        ptr8 = &arg0->currentX;
        *ptr8 += (g_FloatingBillboardTarget.x - *ptr8) / arg0->lifetime;

        ptrC = &arg0->currentY;
        *ptrC += (g_FloatingBillboardTarget.y - (*ptrC + arg0->yOffset)) / arg0->lifetime;

        ptr10 = &arg0->currentZ;
        *ptr10 += (g_FloatingBillboardTarget.z - *ptr10) / arg0->lifetime;
    }

    loadAssetMetadata((loadAssetMetadata_arg *)&arg0->spriteMetadata, arg0->spriteAsset, 5);

    for (i = 0; i < 4; i++) {
        enqueueTexturedBillboardSprite(i, (TexturedBillboardSprite *)&arg0->spriteMetadata);
    }
}

void initFloatingBillboardSpawner(s16 *spawnTimer) {
    *spawnTimer = 0xB4;
    setCallback(updateFloatingBillboardSpawner);
}

void updateFloatingBillboardSpawner(s16 *spawnTimer) {
    Allocation *allocation;
    Task *task;
    u8 randYOffset;
    s32 yOffset;

    allocation = (Allocation *)getCurrentAllocation();
    if (allocation->gamePaused != 0) {
        return;
    }

    *spawnTimer -= 1;
    if (*spawnTimer != 0) {
        return;
    }

    task = (Task *)scheduleTask(initFloatingBillboard, 0, 0, 0xC8);
    if (task != NULL) {
        randYOffset = randA() & 0xFF;
        yOffset = (randYOffset * 5) << 13;
        task->unk24 = yOffset;
    }

    randYOffset = randA() & 0x1F;
    *spawnTimer = randYOffset + 0xB4;
}

void initFloatingSpriteEntity(FloatingSpriteEntity *arg0) {
    s16 index;

    arg0->spriteAsset = loadSpriteAssetByIndex(9);
    arg0->spriteMetadata = &D_800BC920_B0610;

    index = arg0->variantIndex;
    if (D_800BC9DC_B06CC[index] == 7) {
        arg0->spriteMetadata = &D_800BC960_B0650;
    }

    setCleanupCallback(cleanupFloatingSpriteEntity);
    setCallback(updateFloatingSpriteEntity);
}

void updateFloatingSpriteEntity(FloatingSpriteEntity *arg0) {
    Allocation *allocation;
    s32 i;
    s16 index;
    s32 sinResult;
    s16 multiplier;
    s32 *src;

    allocation = (Allocation *)getCurrentAllocation();

    if (allocation->gamePaused == 0) {
        index = arg0->variantIndex;
        arg0->animPhase += D_800BC9F4_B06E4[index];
    }

    index = arg0->variantIndex;
    src = D_800BC9A0_B0690;
    memcpy(&arg0->posX, &src[index * 3], sizeof(Vec3i));

    sinResult = approximateSin(arg0->animPhase);

    index = arg0->variantIndex;
    multiplier = D_800BC9E8_B06D8[index];
    arg0->posY += sinResult * multiplier;

    index = arg0->variantIndex;
    loadAssetMetadata((loadAssetMetadata_arg *)&arg0->spriteMetadata, arg0->spriteAsset, D_800BC9DC_B06CC[index]);

    checkPositionPlayerCollisionWithPull(&arg0->posX, 0x140000, 0x300000);

    for (i = 0; i < 4; i++) {
        enqueueTexturedBillboardSprite(i, (TexturedBillboardSprite *)&arg0->spriteMetadata);
    }
}

void cleanupFloatingSpriteEntity(void **entity) {
    *entity = freeNodeMemory(*entity);
}

void scheduleFloatingSpriteEntity(s16 variantIndex) {
    FloatingSpriteTask *task = (FloatingSpriteTask *)scheduleTask(initFloatingSpriteEntity, 0, 0, 0xC8);
    if (task != NULL) {
        task->variantIndex = variantIndex;
    }
}

void initGhostManager(GhostManager *ghostManager) {
    ghostManager->ghostSpriteAsset = loadSpriteAssetByIndex(9);
    ghostManager->ghostSlotData = 0;
    setCleanupCallback(cleanupGhostManager);
    setCallback(initGhostTransformations);
}

void initGhostTransformations(GhostManager *ghostManager) {
    s32 slotIndex;
    s32 *transformSrc;
    Vec3i *spawnPos;

    ghostManager->ghostSlotData = allocateNodeMemory(0x200);

    slotIndex = 0;
    transformSrc = (s32 *)&gScaleMatrix.translation;
    spawnPos = g_GhostSpawnPositions;

    while (slotIndex < 8) {
        memcpy(transformSrc, spawnPos, sizeof(Vec3i));
        transform3DToMtx(transformSrc - 5, (void *)((u8 *)ghostManager->ghostSlotData + (slotIndex << 6)));
        spawnPos++;
        slotIndex++;
    }

    setCallback(updateGhostSlotStates);
}

void updateGhostSlotStates(u8 *ghostSlots) {
    GameState *gameState;
    s32 i;
    s32 maxRaceState;
    s32 activeGhostCount;
    s32 playerCount;
    Player *player;
    s32 numPlayersToCheck;
    s32 raceState;
    s32 pad[4];

    (void)pad;

    gameState = getCurrentAllocation();
    numPlayersToCheck = gameState->numPlayers;
    maxRaceState = 0;
    i = 0;

    if (numPlayersToCheck > 0) {
        playerCount = numPlayersToCheck;
        player = gameState->players;
        do {
            raceState = player->sectorIndex;
            if ((u32)(raceState - 7) < 5U) {
                raceState &= 0xFFFF;
                if (maxRaceState < raceState) {
                    maxRaceState = raceState;
                }
            }
            i++;
            player = (Player *)((u8 *)player + sizeof(Player));
        } while (i < playerCount);
    }

    activeGhostCount = ((maxRaceState < 7) ^ 1) * 2;
    if (maxRaceState >= 8) {
        activeGhostCount = 4;
    }
    if (maxRaceState >= 9) {
        activeGhostCount = 6;
    }
    if (maxRaceState >= 10) {
        activeGhostCount = 8;
    }

    i = 0;
    if (activeGhostCount != 0) {
        do {
            (ghostSlots + i)[8] = 8;
            i++;
        } while (i < activeGhostCount);
    }

    i = activeGhostCount;
    while (i < 8) {
        (ghostSlots + i)[8] = 9;
        i++;
    }

    for (i = 0; i < 4; i++) {
        debugEnqueueCallback((u16)i, 4, renderGhosts, ghostSlots);
    }
}

void cleanupGhostManager(GhostManager *ghostManager) {
    ghostManager->ghostSlotData = freeNodeMemory(ghostManager->ghostSlotData);
    ghostManager->ghostSpriteAsset = freeNodeMemory(ghostManager->ghostSpriteAsset);
}

void renderGhosts(GhostRenderState *state) {
    OutputStruct_19E80 textureTableEntry;
    s32 currentTextureIndex;
    s32 i;

    currentTextureIndex = -1;
    gGraphicsMode = -1;
    gSPDisplayList(gRegionAllocPtr++, D_8009A780_9B380);

    for (i = 0; i < 8; i++) {
        if (isObjectCulled(&g_GhostSpawnPositions[i]) == 0) {
            u8 textureIdx = state->textureIndices[i];

            if (textureIdx != currentTextureIndex) {
                currentTextureIndex = textureIdx;
                getTableEntryByU16Index(state->textureTable, (u16)currentTextureIndex, &textureTableEntry);

                gDPLoadMultiBlock_4b(
                    gRegionAllocPtr++,
                    textureTableEntry.data_ptr,
                    0,
                    G_TX_RENDERTILE,
                    G_IM_FMT_CI,
                    textureTableEntry.width,
                    textureTableEntry.height,
                    0,
                    G_TX_CLAMP,
                    G_TX_CLAMP,
                    G_TX_NOMASK,
                    G_TX_NOMASK,
                    G_TX_NOLOD,
                    G_TX_NOLOD
                );

                gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, textureTableEntry.index_ptr);
            }

            gSPMatrix(
                gRegionAllocPtr++,
                (u8 *)state->matrixData + (i << 6),
                G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW
            );

            gSPMatrix(gRegionAllocPtr++, gLookAtPtr, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);

            gSPVertex(gRegionAllocPtr++, D_800BCA60_B0750, 4, 0);

            gSP2Triangles(gRegionAllocPtr++, 0, 3, 2, 0, 2, 1, 0, 0);
        }
    }
}

void initLapCounterTask(s16 *lapCounter) {
    *lapCounter = 0;
    setCallback(updateLapCounter);
}

void updateLapCounter(s16 *currentLap) {
    s32 playerIndex;
    GameState *gameState;
    Player *player;
    u32 playerOffset;
    s16 lapValue;

    gameState = getCurrentAllocation();

    for (playerIndex = 0; playerIndex < gameState->playerCount; playerIndex++) {
        playerOffset = (u32)(playerIndex * sizeof(Player));
        player = (Player *)(playerOffset + (u32)gameState->players);
        lapValue = *currentLap;

        if (player->currentLap == lapValue) {
            if ((u32)(player->sectorIndex - 7) < 4) {
                *currentLap = lapValue + 1;
                playSoundEffectWithPriorityDefaultVolume(0xC, 6);
            }
        }
    }
}
