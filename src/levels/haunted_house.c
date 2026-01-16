#include "levels/haunted_house.h"
#include "race_session.h"
#include "42170.h"
#include "52880.h"
#include "56910.h"
#include "5AA90.h"
#include "5DBC0.h"
#include "5E590.h"
#include "6E840.h"
#include "common.h"
#include "displaylist.h"
#include "gamestate.h"
#include "geometry.h"
#include "graphics.h"
#include "rand.h"
#include "task_scheduler.h"

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

extern s32 D_8009A8A4_9B4A4;
extern void *g_GhostDefaultAssetMetadata;
extern Vec3i g_GhostBaseDirection;
extern AnimationData D_800BC830_B0520[];
extern void *D_800BC8C8_B05B8;
extern s8 g_FloatingBillboardInitialPos[12];
extern void *D_800BC920_B0610;
extern void *D_800BC960_B0650;
extern s32 D_800BC9A0_B0690[];
extern s16 D_800BC9DC_B06CC[];
extern s16 D_800BC9E8_B06D8[];
extern s16 D_800BC9F4_B06E4[];
extern Vec3i g_GhostSpawnPositions[];
extern s32 g_FloatingBillboardTargetX;
extern s32 g_FloatingBillboardTargetY;
extern s32 g_FloatingBillboardTargetZ;
extern Gfx D_8009A780_9B380[];
extern Gfx D_800BCA60_B0750[];
extern s32 D_800A8B14_9FE84;
extern s16 gGraphicsMode;
extern Gfx *gRegionAllocPtr;

void func_800BB388_AF078(AnimatedGhostEntity *);
void cleanupAnimatedGhost(void **);
void fadeInGhost(AnimatedGhostEntity *);
void oscillateGhostFade(AnimatedGhostEntity *);
void fadeOutGhost(AnimatedGhostEntity *);
void func_800BB778_AF468(void);
void updateSwingingPendulumTrap(SwingingPendulumTrap *);
void freeSwingingPendulumTrapAssets(SwingingPendulumTrap *);
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
extern void cleanupFloatingSpriteEntity(void **);

void updateGhostAnimation(AnimatedGhostEntity *ghost) {
    s32 viewport;

    ghost->animTimer--;

    if (ghost->animTimer == 0) {
        loadAssetMetadata((loadAssetMetadata_arg *)&ghost->assetMetadata, ghost->assetData, D_800BC830_B0520[ghost->animFrameIndex].assetIndex);

        ghost->animTimer = D_800BC830_B0520[ghost->animFrameIndex].frameDuration;
        ghost->animFrameIndex++;

        if (D_800BC830_B0520[ghost->animFrameIndex].frameDuration == 0) {
            ghost->animFrameIndex = 0;
        }
    }

    memcpy(&ghost->renderPositionBuffer, &ghost->posX, 0xC);

    for (viewport = 0; viewport < 4; viewport++) {
        enqueueAlphaBillboardSprite(viewport, (loadAssetMetadata_arg *)&ghost->assetMetadata);
    }
}

void func_800BB388_AF078(AnimatedGhostEntity *ghost) {
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

void cleanupAnimatedGhost(void **arg0) {
    *arg0 = freeNodeMemory(*arg0);
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

        newHeight = func_80060A3C_6163C(collisionContext, ghost->collisionHeight, posPtr);
        ghost->collisionHeight = newHeight;

        func_80060CDC_618DC(collisionContext, newHeight, posPtr, 0x80000, &surfaceNormal);

        ghost->posY = getTrackHeightInSector(collisionContext, (u16)ghost->collisionHeight, posPtr, 0x100000) + 0x180000;

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
    u8 unused;
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
        if (nearbyPlayer->unkBCF < 3) {
            spawnStarEffectImmediate(nearbyPlayer);
        }

        setCallback(fadeOutGhost);
    }

    updateGhostAnimation(ghost);
}

void fadeOutGhost(AnimatedGhostEntity *ghost) {
    ghost->alpha -= 0x10;

    if (ghost->alpha == 0) {
        func_80069CF8_6A8F8();
    } else {
        updateGhostPositionAndCheckEnd(ghost);
        updateGhostAnimation(ghost);
    }
}

void initGhostSpawnerTask(GhostSpawnerTask *arg0) {
    arg0->ghostType = 0;
    arg0->spawnTimer = 0x14;
    setCallback(func_800BB778_AF468);
}

INCLUDE_ASM("asm/nonmatchings/levels/haunted_house", func_800BB778_AF468);

void initSwingingPendulumTrap(SwingingPendulumTrap *arg0) {
    func_80055E68_56A68_result *temp_v0_3;
    u16 temp_a1;

    arg0->uncompressedAsset = loadUncompressedAssetByIndex(9);
    arg0->compressedAsset = loadCompressedSegment2AssetByIndex(9);
    arg0->animationFrame = 0;

    temp_v0_3 = func_80055E68_56A68(9);
    arg0->displayListState = &temp_v0_3->unk90;

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
    setCleanupCallback(freeSwingingPendulumTrapAssets);
    setCallback(updateSwingingPendulumTrap);
}

void updateSwingingPendulumTrap(SwingingPendulumTrap *arg0) {
    Allocation *allocation;
    s32 i;
    s32 offset;
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
                if ((s16)arg0->swingAngle != -0x400) {
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
                if ((s16)arg0->swingAngle != 0x400) {
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

    memcpy(&arg0->position, &arg0->pivotX, 0xC);
    arg0->position.y = arg0->position.y + approximateSin(arg0->bobPhase) * 0x1C0;

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
    }
}

void freeSwingingPendulumTrapAssets(SwingingPendulumTrap *trap) {
    trap->uncompressedAsset = freeNodeMemory(trap->uncompressedAsset);
    trap->compressedAsset = freeNodeMemory(trap->compressedAsset);
}

void initFloatingBillboard(FloatingBillboard *billboard) {
    s32 initialY;

    billboard->spriteAsset = loadSpriteAssetByIndex(9);
    billboard->spriteMetadata = &D_800BC8C8_B05B8;
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
            func_80069CF8_6A8F8();
            return;
        }

        ptr8 = &arg0->currentX;
        *ptr8 += (g_FloatingBillboardTargetX - *ptr8) / arg0->lifetime;

        ptrC = &arg0->currentY;
        *ptrC += (g_FloatingBillboardTargetY - (*ptrC + arg0->yOffset)) / arg0->lifetime;

        ptr10 = &arg0->currentZ;
        *ptr10 += (g_FloatingBillboardTargetZ - *ptr10) / arg0->lifetime;
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
    memcpy(&arg0->posX, &src[index * 3], 0xC);

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
    transformSrc = &D_8009A8A4_9B4A4;
    spawnPos = g_GhostSpawnPositions;

    while (slotIndex < 8) {
        memcpy(transformSrc, spawnPos, 0xC);
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
                    textureTableEntry.field1,
                    textureTableEntry.field2,
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

            gSPMatrix(gRegionAllocPtr++, D_800A8B14_9FE84, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);

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

    for (playerIndex = 0; playerIndex < gameState->unk5F; playerIndex++) {
        playerOffset = (u32)(playerIndex * sizeof(Player));
        player = (Player *)((u32)playerOffset + (u32)gameState->players);
        lapValue = *currentLap;

        if (player->currentLap == lapValue) {
            if ((u32)(player->sectorIndex - 7) < 4) {
                *currentLap = lapValue + 1;
                playSoundEffectWithPriorityDefaultVolume(0xC, 6);
            }
        }
    }
}
