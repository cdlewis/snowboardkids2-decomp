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
#include "race/track_collision.h"
#include "race/track_height.h"
#include "system/task_scheduler.h"

typedef struct {
    s8 frameDuration;
    s8 assetIndex;
} AnimationData;

typedef struct {
    s32 x;
    s32 z;
} GhostSpawnPos;

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

extern Gfx *gDisplayListAllocPtr;

void initAnimatedGhost(AnimatedGhostEntity *);
void cleanupAnimatedGhost(void **);
void fadeInGhost(AnimatedGhostEntity *);
void oscillateGhostFade(AnimatedGhostEntity *);
void fadeOutGhost(AnimatedGhostEntity *);
void updateGhostSpawner(GhostSpawnerTask *);
void updateSwingingPendulumTrap(SwingingPendulumTrap *);
void initFloatingBillboard(FloatingBillboard *);
void updateFloatingBillboardSpawner(s16 *);
void freeFloatingBillboard(void **);
void updateFloatingBillboard(FloatingBillboard *);
void initFloatingSpriteEntity(FloatingSpriteEntity *);
void updateFloatingSpriteEntity(FloatingSpriteEntity *);
void initGhostTransformations(GhostManager *);
void updateGhostSlotStates(GhostManager *ghostManager);
void cleanupGhostManager(GhostManager *);
void updateLapCounter(s16 *);
void updateGhostAnimation(AnimatedGhostEntity *);
void renderGhosts(GhostManager *);
void cleanupFloatingSpriteEntity(void **);

void updateGhostAnimation(AnimatedGhostEntity *ghost) {
    s32 viewport;

    ghost->animationTimer--;

    if (ghost->animationTimer == 0) {
        loadAssetMetadata(&ghost->sprite, ghost->spriteAsset, D_800BC830_B0520[ghost->animationFrameIndex].assetIndex);

        ghost->animationTimer = D_800BC830_B0520[ghost->animationFrameIndex].frameDuration;
        ghost->animationFrameIndex++;

        if (D_800BC830_B0520[ghost->animationFrameIndex].frameDuration == 0) {
            ghost->animationFrameIndex = 0;
        }
    }

    memcpy(&ghost->sprite.position, &ghost->worldPosition, sizeof(Vec3i));

    for (viewport = 0; viewport < 4; viewport++) {
        enqueueAlphaBillboardSprite(viewport, &ghost->sprite);
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

    ghost->spriteAsset = load_3ECE40();
    ghost->sprite.assetTemplate = (loadAssetMetadata_arg *)&g_GhostDefaultAssetMetadata;
    ghost->sprite.alpha = 0;

    randomSpeed = randA() & 0xFF;
    randomSpeed = randomSpeed << 10;
    direction.z = randomSpeed + 0x20000;

    rotationAngle = randA() & 0xFF;
    rotationAngle = rotationAngle << 5;

    rotateVectorY(directionPtr, rotationAngle, &ghost->velocity);

    if (ghost->ghostType == 0) {
        ghost->trackSector = 0x1A;
    } else {
        ghost->trackSector = 0x58;
    }

    ghost->animationTimer = 1;
    ghost->animationFrameIndex = 0;
    ghost->lifetime = 0x3C;

    setCleanupCallback(cleanupAnimatedGhost);
    setCallbackWithContinue(fadeInGhost);
}

void cleanupAnimatedGhost(void **ghostEntity) {
    *ghostEntity = freeNodeMemory(*ghostEntity);
}

s32 updateGhostPositionAndCheckEnd(AnimatedGhostEntity *ghost) {
    Vec3i surfaceNormal;
    GameState *gameState;
    void *collisionContext;
    s32 *posPtr;
    s32 shouldEnd;
    u16 newHeight;

    shouldEnd = 0;
    gameState = getCurrentAllocation();

    if (gameState->gamePaused == 0) {
        collisionContext = &gameState->gameData;

        ghost->worldPosition.x = ghost->worldPosition.x + ghost->velocity.x;
        ghost->worldPosition.z = ghost->worldPosition.z + ghost->velocity.z;

        posPtr = &ghost->worldPosition.x;

        newHeight = findTrackSector(collisionContext, ghost->trackSector, posPtr);
        ghost->trackSector = newHeight;

        resolveTrackWallCollision(collisionContext, newHeight, posPtr, 0x80000, &surfaceNormal);

        ghost->worldPosition.y =
            getTrackHeightInSector(collisionContext, (u16)ghost->trackSector, posPtr, 0x100000) + 0x180000;

        if (ghost->ghostType == 0) {
            shouldEnd = (ghost->trackSector != 0x1A);
        } else {
            u16 sector = ghost->trackSector;
            if ((u16)(sector - 0x58) >= 2) {
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
    ghost->sprite.alpha += 0x10;

    if (ghost->sprite.alpha == 0xE0) {
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
        ghost->sprite.alpha -= 0x10;
        if (ghost->sprite.alpha < 0x41) {
            ghost->fadeDirection = 0;
        }
    } else {
        ghost->sprite.alpha += 0x10;
        if (ghost->sprite.alpha >= 0xE0) {
            ghost->fadeDirection = 1;
        }
    }

    if (updateGhostPositionAndCheckEnd(ghost) != 0) {
        setCallback(fadeOutGhost);
    }

    nearbyPlayer = findVulnerablePlayerNearPosition(&ghost->worldPosition.x, -1, 0x100000);

    if (nearbyPlayer != NULL) {
        if (nearbyPlayer->slowdownLevel < 3) {
            spawnStarEffectImmediate(nearbyPlayer);
        }

        setCallback(fadeOutGhost);
    }

    updateGhostAnimation(ghost);
}

void fadeOutGhost(AnimatedGhostEntity *ghost) {
    ghost->sprite.alpha -= 0x10;

    if (ghost->sprite.alpha == 0) {
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

void updateGhostSpawner(GhostSpawnerTask *spawner) {
    GameState *gameState;
    int new_var;
    AnimatedGhostEntity *ghost;
    s32 count;
    s32 inRange;
    s32 randIdx;
    s16 countdown;
    gameState = getCurrentAllocation();
    if (gameState->gamePaused != 0) {
        return;
    }
    countdown = spawner->spawnTimer;
    if (countdown == 0) {
        for (count = 0; count < gameState->numPlayers; count++) {
            if (spawner->ghostType == 0) {
                inRange = (gameState->players[count].sectorIndex - 0x18) < 6U;
            } else {
                inRange = (gameState->players[count].sectorIndex - 0x53) < 8U;
            }
            if (inRange != 0) {
                break;
            }
        }

        if (count < gameState->numPlayers) {
            ghost = (AnimatedGhostEntity *)scheduleTask(initAnimatedGhost, 0, 0, 0xDC);
            if (ghost != 0) {
                if (spawner->ghostType == 0) {
                    ghost->ghostType = 0;
                    randIdx = randA();
                    randIdx &= 7;
                    count = randIdx;
                    ghost->worldPosition.x = D_800BC844_B0534[count].x;
                    ghost->worldPosition.z = D_800BC844_B0534[randIdx].z;
                } else {
                    ghost->ghostType = 1;
                    randIdx = randA();
                    randIdx &= 7;
                    count = randIdx;
                    ghost->worldPosition.x = D_800BC884_B0574[count].x;
                    ghost->worldPosition.z = D_800BC884_B0574[count].z;
                }
            }
        }
        spawner->spawnTimer = 4;
        spawner->ghostType = (spawner->ghostType + 1) & 1;
    } else {
        new_var = 1;
        spawner->spawnTimer = countdown - new_var;
    }
}

void initSwingingPendulumTrap(SwingingPendulumTrap *arg0) {
    LevelDisplayLists *temp_v0_3;
    u16 temp_a1;

    arg0->renderObject.segment1 = loadUncompressedAssetByIndex(9);
    arg0->renderObject.segment2 = loadCompressedSegment2AssetByIndex(9);
    arg0->renderObject.segment3 = NULL;

    temp_v0_3 = getSkyDisplayLists3ByIndex(9);
    arg0->renderObject.displayLists = &temp_v0_3->sceneryDisplayLists1;

    arg0->phaseTimer = 0xF;
    arg0->pivotX = 0xFD71ABE1;
    arg0->pivotY = 0x1E37049C;
    arg0->pivotZ = 0xF61F3676;
    arg0->bobPhase = 0;
    arg0->swingPhase = 0;
    arg0->swingAngle = 0x400;
    arg0->fireProjectileCooldown = 0x1E;

    temp_a1 = arg0->swingAngle + 0x2A0;
    createYRotationMatrix(&arg0->renderObject.transform, temp_a1 & 0xFFFF);
    setCleanupCallback(cleanupSwingingPendulumTrap);
    setCallback(updateSwingingPendulumTrap);
}

void updateSwingingPendulumTrap(SwingingPendulumTrap *arg0) {
    GameState *gameState;
    s32 i;
    Vec3i *pos;
    Player *player;

    gameState = getCurrentAllocation();

    if (gameState->gamePaused == 0) {
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
        if (gameState->numPlayers > i) {
            do {
                isPlayerInRangeAndPull(&arg0->renderObject.transform.translation, 0x200000, gameState->players + i);
                i += 1;
            } while (i < gameState->numPlayers);
        }

        createYRotationMatrix(&arg0->renderObject.transform, (arg0->swingAngle + 0x2A0) & 0xFFFF);

        if (arg0->fireProjectileCooldown == 0) {
            arg0->fireProjectileCooldown = (randA() & 0xF) + 0xF;

            for (i = 0; i < gameState->numPlayers; i++) {
                player = gameState->players + i;
                if ((u16)(player->sectorIndex - 0x38) < 5) {
                    break;
                }
            }

            if (i < gameState->numPlayers) {
                pos = &arg0->renderObject.transform.translation;
                spawnShrinkProjectileTask(0, (s16)(arg0->swingAngle + 0x2A0), pos);
                spawnShrinkProjectileTask(1, (s16)(arg0->swingAngle + 0x2A0), pos);
            }
        }
    }

    memcpy(&arg0->renderObject.transform.translation, &arg0->pivotX, sizeof(Vec3i));
    arg0->renderObject.transform.translation.y =
        arg0->renderObject.transform.translation.y + approximateSin(arg0->bobPhase) * 0x1C0;

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, &arg0->renderObject);
    }
}

void cleanupSwingingPendulumTrap(SwingingPendulumTrap *trap) {
    trap->renderObject.segment1 = freeNodeMemory(trap->renderObject.segment1);
    trap->renderObject.segment2 = freeNodeMemory(trap->renderObject.segment2);
}

void initFloatingBillboard(FloatingBillboard *billboard) {
    s32 initialY;

    billboard->spriteAsset = loadSpriteAssetByIndex(9);
    billboard->sprite.assetTemplate = (loadAssetMetadata_arg *)&g_FloatingBillboardSpriteMetadata;
    billboard->sprite.alpha = 0xFF;
    memcpy(&billboard->sprite.position, &g_FloatingBillboardInitialPos, sizeof(Vec3i));
    initialY = billboard->sprite.position.y + billboard->targetYOffset;
    billboard->lifetime = 0xB4;
    billboard->sprite.position.y = initialY;
    setCleanupCallback(freeFloatingBillboard);
    setCallbackWithContinue(updateFloatingBillboard);
}

void freeFloatingBillboard(void **billboard) {
    *billboard = freeNodeMemory(*billboard);
}

void updateFloatingBillboard(FloatingBillboard *arg0) {
    GameState *gameState;
    s32 i;
    s32 *ptr8;
    s32 *ptrC;
    s32 *ptr10;

    gameState = getCurrentAllocation();

    if (gameState->gamePaused == 0) {
        arg0->lifetime -= 1;

        if (arg0->lifetime == 0) {
            terminateCurrentTask();
            return;
        }

        ptr8 = &arg0->sprite.position.x;
        *ptr8 += (g_FloatingBillboardTarget.x - *ptr8) / arg0->lifetime;

        ptrC = &arg0->sprite.position.y;
        *ptrC += (g_FloatingBillboardTarget.y - (*ptrC + arg0->targetYOffset)) / arg0->lifetime;

        ptr10 = &arg0->sprite.position.z;
        *ptr10 += (g_FloatingBillboardTarget.z - *ptr10) / arg0->lifetime;
    }

    loadAssetMetadata(&arg0->sprite, arg0->spriteAsset, 5);

    for (i = 0; i < 4; i++) {
        enqueueTexturedBillboardSprite(i, (TexturedBillboardSprite *)&arg0->sprite);
    }
}

void initFloatingBillboardSpawner(s16 *spawnTimer) {
    *spawnTimer = 0xB4;
    setCallback(updateFloatingBillboardSpawner);
}

void updateFloatingBillboardSpawner(s16 *spawnTimer) {
    GameState *gameState;
    FloatingBillboard *billboard;
    u8 randYOffset;
    s32 yOffset;

    gameState = getCurrentAllocation();
    if (gameState->gamePaused != 0) {
        return;
    }

    *spawnTimer -= 1;
    if (*spawnTimer != 0) {
        return;
    }

    billboard = (FloatingBillboard *)scheduleTask(initFloatingBillboard, 0, 0, 0xC8);
    if (billboard != NULL) {
        randYOffset = randA() & 0xFF;
        yOffset = (randYOffset * 5) << 13;
        billboard->targetYOffset = yOffset;
    }

    randYOffset = randA() & 0x1F;
    *spawnTimer = randYOffset + 0xB4;
}

void initFloatingSpriteEntity(FloatingSpriteEntity *arg0) {
    s16 index;

    arg0->spriteAsset = loadSpriteAssetByIndex(9);
    arg0->sprite.assetTemplate = (loadAssetMetadata_arg *)&D_800BC920_B0610;

    index = arg0->variantIndex;
    if (D_800BC9DC_B06CC[index] == 7) {
        arg0->sprite.assetTemplate = (loadAssetMetadata_arg *)&D_800BC960_B0650;
    }

    setCleanupCallback(cleanupFloatingSpriteEntity);
    setCallback(updateFloatingSpriteEntity);
}

void updateFloatingSpriteEntity(FloatingSpriteEntity *arg0) {
    GameState *gameState;
    s32 i;
    s16 index;
    s32 sinResult;
    s16 multiplier;
    s32 *src;

    gameState = getCurrentAllocation();

    if (gameState->gamePaused == 0) {
        index = arg0->variantIndex;
        arg0->animationPhase += D_800BC9F4_B06E4[index];
    }

    index = arg0->variantIndex;
    src = D_800BC9A0_B0690;
    memcpy(&arg0->sprite.position, &src[index * 3], sizeof(Vec3i));

    sinResult = approximateSin(arg0->animationPhase);

    index = arg0->variantIndex;
    multiplier = D_800BC9E8_B06D8[index];
    arg0->sprite.position.y += sinResult * multiplier;

    index = arg0->variantIndex;
    loadAssetMetadata(&arg0->sprite, arg0->spriteAsset, D_800BC9DC_B06CC[index]);

    checkPositionPlayerCollisionWithPull(&arg0->sprite.position.x, 0x140000, 0x300000);

    for (i = 0; i < 4; i++) {
        enqueueTexturedBillboardSprite(i, (TexturedBillboardSprite *)&arg0->sprite);
    }
}

void cleanupFloatingSpriteEntity(void **entity) {
    *entity = freeNodeMemory(*entity);
}

void scheduleFloatingSpriteEntity(s16 variantIndex) {
    FloatingSpriteEntity *entity = (FloatingSpriteEntity *)scheduleTask(initFloatingSpriteEntity, 0, 0, 0xC8);
    if (entity != NULL) {
        entity->variantIndex = variantIndex;
    }
}

void initGhostManager(GhostManager *ghostManager) {
    ghostManager->ghostSpriteAsset = loadSpriteAssetByIndex(9);
    ghostManager->ghostMatrices = NULL;
    setCleanupCallback(cleanupGhostManager);
    setCallback(initGhostTransformations);
}

void initGhostTransformations(GhostManager *ghostManager) {
    s32 slotIndex;
    s32 *transformSrc;
    Vec3i *spawnPos;

    ghostManager->ghostMatrices = allocateNodeMemory(0x200);

    slotIndex = 0;
    transformSrc = (s32 *)&gScaleMatrix.translation;
    spawnPos = g_GhostSpawnPositions;

    while (slotIndex < 8) {
        memcpy(transformSrc, spawnPos, sizeof(Vec3i));
        transform3DToMtx(transformSrc - 5, (void *)((u8 *)ghostManager->ghostMatrices + (slotIndex << 6)));
        spawnPos++;
        slotIndex++;
    }

    setCallback(updateGhostSlotStates);
}

void updateGhostSlotStates(GhostManager *ghostManager) {
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
            ghostManager->textureIndices[i] = 8;
            i++;
        } while (i < activeGhostCount);
    }

    i = activeGhostCount;
    while (i < 8) {
        ghostManager->textureIndices[i] = 9;
        i++;
    }

    for (i = 0; i < 4; i++) {
        enqueueCallbackBySlotIndex((u16)i, 4, renderGhosts, ghostManager);
    }
}

void cleanupGhostManager(GhostManager *ghostManager) {
    ghostManager->ghostMatrices = freeNodeMemory(ghostManager->ghostMatrices);
    ghostManager->ghostSpriteAsset = freeNodeMemory(ghostManager->ghostSpriteAsset);
}

void renderGhosts(GhostManager *ghostManager) {
    OutputStruct_19E80 textureTableEntry;
    s32 currentTextureIndex;
    s32 i;

    currentTextureIndex = -1;
    gGraphicsMode = -1;
    gSPDisplayList(gDisplayListAllocPtr++, D_8009A780_9B380);

    for (i = 0; i < 8; i++) {
        if (isObjectCulled(&g_GhostSpawnPositions[i]) == 0) {
            u8 textureIdx = ghostManager->textureIndices[i];

            if (textureIdx != currentTextureIndex) {
                currentTextureIndex = textureIdx;
                getTableEntryByU16Index(ghostManager->ghostSpriteAsset, (u16)currentTextureIndex, &textureTableEntry);

                gDPLoadMultiBlock_4b(
                    gDisplayListAllocPtr++,
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

                gDPLoadTLUT_pal16(gDisplayListAllocPtr++, 0, textureTableEntry.index_ptr);
            }

            gSPMatrix(
                gDisplayListAllocPtr++,
                (u8 *)ghostManager->ghostMatrices + (i << 6),
                G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW
            );

            gSPMatrix(gDisplayListAllocPtr++, gLookAtPtr, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);

            gSPVertex(gDisplayListAllocPtr++, D_800BCA60_B0750, 4, 0);

            gSP2Triangles(gDisplayListAllocPtr++, 0, 3, 2, 0, 2, 1, 0, 0);
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
