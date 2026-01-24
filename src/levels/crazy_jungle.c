#include "52880.h"
#include "56910.h"
#include "594E0.h"
#include "5AA90.h"
#include "common.h"
#include "displaylist.h"
#include "gamestate.h"
#include "geometry.h"
#include "graphics.h"
#include "rand.h"
#include "task_scheduler.h"

// Struct definitions

typedef struct {
    u16 rotX;
    u16 rotY;
    s32 position[3];
} RockPositionEntry;

typedef struct {
    /* 0x00 */ DisplayListObject node;
    /* 0x3C */ u16 timer;
    /* 0x3E */ s16 state;
} StarLauncherTask;

typedef struct {
    /* 0x00 */ DisplayListObject node1;
    /* 0x3C */ DisplayListObject node2;
    /* 0x78 */ s32 rotationMatrix;
    /* 0x7C */ s8 pad_7C[0x10];
    /* 0x8C */ s32 posX;
    /* 0x90 */ s8 pad_90[0x4];
    /* 0x94 */ s32 posY;
    /* 0x98 */ s32 posZ;
    /* 0x9C */ s32 animTimer;
    /* 0xA0 */ s32 posOffset;
    /* 0xA4 */ s16 xRotation;
    /* 0xA6 */ s16 positionIndex;
    /* 0xA8 */ s16 respawnTimer;
} FallingRockHazard;

typedef struct {
    Node n;
    u8 padding[0x7A];
    s16 positionIndex;
} NodeWithPayload;

// Function declarations

void renderFallingRockHazard(FallingRockHazard *rock);
void updateFallingRockHazard(FallingRockHazard *rock);
void fallingRockImpactCallback(FallingRockHazard *rock);
void fallingRockRespawnCallback(FallingRockHazard *rock);
void cleanupStarLauncherTask(StarLauncherTask *arg0);
void freeDisplayListSegments(StarLauncherTask *arg0);
void updateStarLauncherTask(StarLauncherTask *arg0);
void initFallingRockHazard(FallingRockHazard *rock);
void initStarLauncherTask(StarLauncherTask *arg0);
void initCrazyJungleHazards(void);

// Extern declarations

extern RockPositionEntry crazyJungleRockPositions[];
extern s32 D_8009A8A4_9B4A4;

// Function implementations

void initFallingRockHazard(FallingRockHazard *rock) {
    GameState *gameState;
    s32 randVal;
    LevelDisplayLists *result;

    gameState = (GameState *)getCurrentAllocation();
    randVal = randA();

    rock->positionIndex += (randVal & 1);
    result = getSkyDisplayLists3ByIndex(gameState->memoryPoolId);
    rock->node1.displayLists = &result->sceneryDisplayLists1;
    result = getSkyDisplayLists3ByIndex(gameState->memoryPoolId);
    rock->node2.displayLists = &result->sceneryDisplayLists2;
    rock->node1.segment1 = loadUncompressedAssetByIndex(gameState->memoryPoolId);
    rock->node1.segment2 = loadCompressedSegment2AssetByIndex(gameState->memoryPoolId);
    rock->node1.segment3 = 0;
    rock->posZ = 0;
    rock->animTimer = 0;
    rock->posOffset = 0;
    rock->node2.segment1 = rock->node1.segment1;
    rock->node2.segment2 = rock->node1.segment2;
    rock->node2.segment3 = rock->node1.segment3;
    memcpy(&rock->posX, &crazyJungleRockPositions[rock->positionIndex].position, 0xC);
    createCombinedRotationMatrix(&rock->rotationMatrix, crazyJungleRockPositions[rock->positionIndex].rotX, crazyJungleRockPositions[rock->positionIndex].rotY);
    rock->xRotation = 0;

    setCleanupCallback(freeDisplayListSegments);
    setCallback(updateFallingRockHazard);
}

void renderFallingRockHazard(FallingRockHazard *rock) {
    s32 matrix[8];
    s32 i;

    memcpy(&D_8009A8A4_9B4A4, &rock->posZ, 0xC);
    func_8006B084_6BC84(&D_8009A8A4_9B4A4 - 5, &rock->rotationMatrix, rock);
    createXRotationMatrix((s16(*)[3])matrix, rock->xRotation);

    matrix[6] = 0x3b333;
    matrix[5] = 0;
    matrix[7] = 0x170000;

    func_8006B084_6BC84(&matrix[0], rock, &rock->node2);

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, &rock->node1);
        enqueueDisplayListWithFrustumCull(i, &rock->node2);
    }
}

void updateFallingRockHazard(FallingRockHazard *rock) {
    GameState *gs;
    s32 xDiff;
    s32 zDiff;
    s32 playerInRange;
    s32 i;

    gs = (GameState *)getCurrentAllocation();
    playerInRange = 0;

    for (i = 0; i < gs->numPlayers; i++) {
        xDiff = gs->players[i].worldPos.x - rock->posX;
        zDiff = gs->players[i].worldPos.z - rock->posY;
        if (((0x27FFFFE >= ((u32)xDiff) + 0x13FFFFF) & (0x13FFFFF >= zDiff)) == 0) {
            continue;
        }

        if ((s32)0xFF600000 < zDiff) {
            playerInRange = 1;
            break;
        }
    }

    if (playerInRange) {
        if (gs->gamePaused == 0) {
            if (rock->animTimer != 0x60000) {
                rock->animTimer += 0x20000;
            }

            if (rock->xRotation != (-0x600)) {
                rock->xRotation -= 0x100;
            }
        }

        renderFallingRockHazard(rock);

        if (gs->gamePaused == 0) {
            for (i = 0; i < gs->numPlayers; i++) {
                if (isPlayerInRangeAndPull(&rock->node2.transform.translation, 0x12A000, &gs->players[i]) != 0) {
                    if (isPlayerInRangeAndPull(&rock->node2.transform.translation, 0x1E3000, &gs->players[i]) != 0) {
                        setPlayerState50(&gs->players[i]);
                        queueSoundAtPosition(&rock->node2.transform.translation, 0x2A);
                        setCallback(fallingRockImpactCallback);
                    }
                }
            }
        }
    } else {
        if (!gs->gamePaused) {
            if (rock->animTimer > 0) {
                rock->animTimer += 0xFFFE0000;
            }

            if (rock->animTimer < 0) {
                rock->animTimer += 0x20000;
            }

            if (rock->xRotation != 0) {
                rock->xRotation += 0x100;
            }
        }

        renderFallingRockHazard(rock);
    }
}

void fallingRockImpactCallback(FallingRockHazard *rock) {
    GameState *gs;
    s32 i;

    gs = (GameState *)getCurrentAllocation();
    if (rock->animTimer != 0xFFF00000) {
        if (gs->gamePaused == FALSE) {
            rock->animTimer = rock->animTimer - 0x8000;
        }
    } else {
        rock->respawnTimer = 0x12C;
        setCallback(&fallingRockRespawnCallback);
    }
    rock->xRotation = 0;
    renderFallingRockHazard(rock);

    for (i = 0; i < gs->numPlayers; i++) {
        isPlayerInRangeAndPull(&rock->node2.transform.translation, 0x12A000, &gs->players[i]);
    }
}

void fallingRockRespawnCallback(FallingRockHazard *rock) {
    u8 positionOffset;

    if (((GameState *)getCurrentAllocation())->gamePaused == FALSE) {
        rock->respawnTimer--;
    }

    if (!rock->respawnTimer) {
        positionOffset = randA() & 1;
        rock->positionIndex = positionOffset + (rock->positionIndex & 0xFE);
        memcpy(&rock->posX, &crazyJungleRockPositions[rock->positionIndex].position, 0xC);
        createCombinedRotationMatrix(
            &rock->rotationMatrix,
            crazyJungleRockPositions[rock->positionIndex].rotX,
            crazyJungleRockPositions[rock->positionIndex].rotY
        );
        setCallback(&updateFallingRockHazard);
    }
}

void freeDisplayListSegments(StarLauncherTask *arg0) {
    arg0->node.segment1 = freeNodeMemory(arg0->node.segment1);
    arg0->node.segment2 = freeNodeMemory(arg0->node.segment2);
}

void initStarLauncherTask(StarLauncherTask *arg0) {
    GameState *gs = (GameState *)getCurrentAllocation();
    arg0->node.segment1 = loadUncompressedAssetByIndex(gs->memoryPoolId);
    arg0->node.segment2 = loadCompressedSegment2AssetByIndex(gs->memoryPoolId);
    arg0->node.segment3 = 0;
    createYRotationMatrix(&arg0->node.transform, 0x6C0);
    arg0->node.transform.translation.x = 0xDD196FEA;
    arg0->node.transform.translation.y = 0x0ABD4CA3;
    arg0->node.transform.translation.z = 0xE270649E;
    arg0->timer = 0x12C;
    arg0->state = 0;
    arg0->node.displayLists = &getSkyDisplayLists3ByIndex(gs->memoryPoolId)->sceneryDisplayLists3;
    setCleanupCallback(&cleanupStarLauncherTask);
    setCallback(&updateStarLauncherTask);
}

void updateStarLauncherTask(StarLauncherTask *arg0) {
    GameState *gameState;
    s16 state;
    s32 i;
    u8 numPlayers;
    s32 randVal;

    gameState = getCurrentAllocation();
    state = arg0->state;

    switch (state) {
        case 0:
            if (!gameState->gamePaused) {
                arg0->timer--;
                if ((arg0->timer << 16) == 0) {
                    arg0->state++;
                }
            }
            break;
        case 1:
            if (gameState->gamePaused == 0) {
                numPlayers = gameState->numPlayers;

                for (i = 0; i < numPlayers; i++) {
                    if ((u32)gameState->players[i].sectorIndex - 0x60 < 6) {
                        arg0->node.displayLists = &getSkyDisplayLists3ByIndex(gameState->memoryPoolId)->sceneryDisplayLists4;
                        randVal = (u8)randA();
                        randVal = randVal - 0x60;
                        i = randVal << 1;
                        i = i + randVal;
                        i = i + 0x6C0;
                        spawnFallingStarProjectile(i, (((u8)randA()) << 12) | 0x100000);
                        queueSoundAtPosition(&arg0->node.transform.translation, 0x23);
                        arg0->timer = 0x18;
                        arg0->state++;
                        break;
                    }
                }
            }
            break;
        case 2:
            if (gameState->gamePaused == 0) {
                arg0->timer--;
                if ((arg0->timer << 16) == 0) {
                    arg0->state = 0;
                    arg0->node.displayLists = &getSkyDisplayLists3ByIndex(gameState->memoryPoolId)->sceneryDisplayLists3;
                    arg0->timer = 0x14;
                }
            }
            break;
    }

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, &arg0->node);
    }
}

void cleanupStarLauncherTask(StarLauncherTask *arg0) {
    arg0->node.segment1 = freeNodeMemory(arg0->node.segment1);
    arg0->node.segment2 = freeNodeMemory(arg0->node.segment2);
}

void initCrazyJungleHazards(void) {
    NodeWithPayload *taskNode;

    taskNode = (NodeWithPayload *)scheduleTask(&initFallingRockHazard, 0, 0, 0x32);
    if (taskNode != NULL) {
        taskNode->positionIndex = 0;
    }

    taskNode = (NodeWithPayload *)scheduleTask(&initFallingRockHazard, 0, 0, 0x32);
    if (taskNode != NULL) {
        taskNode->positionIndex = 2;
    }

    taskNode = (NodeWithPayload *)scheduleTask(&initFallingRockHazard, 0, 0, 0x32);
    if (taskNode != NULL) {
        taskNode->positionIndex = 4;
    }

    taskNode = (NodeWithPayload *)scheduleTask(&initFallingRockHazard, 0, 0, 0x32);
    if (taskNode != NULL) {
        taskNode->positionIndex = 6;
    }

    taskNode = (NodeWithPayload *)scheduleTask(&initFallingRockHazard, 0, 0, 0x32);
    if (taskNode != NULL) {
        taskNode->positionIndex = 8;
    }

    scheduleTask(&initStarLauncherTask, 0, 0, 0xC8);
}
