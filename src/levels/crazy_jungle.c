#include "audio/audio.h"
#include "common.h"
#include "data/course_data.h"
#include "gamestate.h"
#include "graphics/displaylist.h"
#include "math/geometry.h"
#include "math/rand.h"
#include "race/hit_reactions.h"
#include "race/obstacle_sprites.h"
#include "race/track_collision.h"
#include "system/task_scheduler.h"

typedef struct {
    u8 padding[0x24];
    void *segment1;
    void *segment2;
} DisplayListObjectSegments;

typedef struct {
    Transform3D matrix;
    DisplayLists *displayLists;
    void *segment1;
    void *segment2;
    s32 unk2C;
    u8 PAD[0xC];
    s16 timer;
    s16 state;
} StarLauncherTask;

typedef struct {
    u16 rotX;
    u16 rotY;
    s32 position[3];
} RockPositionEntry;

RockPositionEntry crazyJungleRockPositions[] = {
    { 0x1F40, 0x0700, { 0xF709C531, 0x0DB737C5, 0xE4B9BEC4 } },
    { 0x1FC0, 0x0E40, { 0xF25858F9, 0x0D580AD7, 0xE659153B } },
    { 0x1F80, 0x01C0, { 0xF33313A1, 0x0D65CB16, 0xE4C69A27 } },
    { 0x1F40, 0x0A80, { 0xF5893DCB, 0x0D97C3DA, 0xE5B46F47 } },
    { 0x1F00, 0x0600, { 0xEE28EA33, 0x0CF2BB24, 0xE2F1BD0B } },
    { 0x1F40, 0x0F00, { 0xF460777D, 0x0D80E7E7, 0xE63BBF07 } },
    { 0x0000, 0x0000, { 0xF2326B3E, 0x0D5028E5, 0xE3B5BDAD } },
    { 0x1F40, 0x09C0, { 0xF48282FC, 0x0D84F238, 0xE66CBFFB } },
    { 0x1F40, 0x09C0, { 0xEF6655EC, 0x0D1CA6B0, 0xE5013F13 } },
    { 0x1F40, 0x0580, { 0xEFE5D5AA, 0x0D158CF2, 0xE3249855 } },
};

typedef struct {
    /* 0x00 */ DisplayListObject node1;
    /* 0x3C */ DisplayListObject node2;
    s32 rotationMatrix;
    s8 pad_7C[0x10];
    s32 posX;
    s32 posY;
    s32 posZ;
    s32 renderOffsetX;
    s32 animTimer;
    s32 renderOffsetZ;
    s16 xRotation;
    s16 positionIndex;
    s16 respawnTimer;
} FallingRockHazard;

typedef struct {
    Node n;
    u8 padding[0x7A];
    s16 positionIndex;
} NodeWithPayload;

typedef struct {
    /* 0x0 */ DisplayListObject node;
    u16 timer;
    s16 state;
} StarLauncherTaskUpdate;

void renderFallingRockHazard(FallingRockHazard *rock);
void updateFallingRockHazard(FallingRockHazard *rock);
void fallingRockImpactCallback(FallingRockHazard *rock);
void fallingRockRespawnCallback(FallingRockHazard *rock);
void cleanupStarLauncherTask(DisplayListObjectSegments *arg0);
void freeDisplayListSegments(DisplayListObjectSegments *);
void updateStarLauncherTask(StarLauncherTaskUpdate *arg0);

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
    rock->renderOffsetX = 0;
    rock->animTimer = 0;
    rock->renderOffsetZ = 0;
    rock->node2.segment1 = rock->node1.segment1;
    rock->node2.segment2 = rock->node1.segment2;
    rock->node2.segment3 = rock->node1.segment3;
    memcpy(&rock->posX, &crazyJungleRockPositions[rock->positionIndex].position, 0xC);
    createCombinedRotationMatrix(
        &rock->rotationMatrix,
        crazyJungleRockPositions[rock->positionIndex].rotX,
        crazyJungleRockPositions[rock->positionIndex].rotY
    );
    rock->xRotation = 0;

    setCleanupCallback(freeDisplayListSegments);
    setCallback(updateFallingRockHazard);
}

void renderFallingRockHazard(FallingRockHazard *rock) {
    s32 matrix[8];
    s32 i;

    memcpy(&gScaleMatrix.translation, &rock->renderOffsetX, 0xC);
    func_8006B084_6BC84(
        (Transform3D *)((s32 *)&gScaleMatrix.translation - 5),
        (Transform3D *)&rock->rotationMatrix,
        (Transform3D *)rock
    );
    createXRotationMatrix((s16(*)[3])matrix, rock->xRotation);

    matrix[6] = 0x3b333;
    matrix[5] = 0;
    matrix[7] = 0x170000;

    func_8006B084_6BC84((Transform3D *)&matrix[0], (Transform3D *)rock, &rock->node2.transform);

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
        zDiff = gs->players[i].worldPos.z - rock->posZ;
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

void freeDisplayListSegments(DisplayListObjectSegments *arg0) {
    arg0->segment1 = freeNodeMemory(arg0->segment1);
    arg0->segment2 = freeNodeMemory(arg0->segment2);
}

void initStarLauncherTask(StarLauncherTask *arg0) {
    GameState *gs = (GameState *)getCurrentAllocation();
    arg0->segment1 = loadUncompressedAssetByIndex(gs->memoryPoolId);
    arg0->segment2 = loadCompressedSegment2AssetByIndex(gs->memoryPoolId);
    arg0->unk2C = 0;
    createYRotationMatrix(&arg0->matrix, 0x6C0);
    arg0->matrix.translation.x = 0xDD196FEA;
    arg0->matrix.translation.y = 0x0ABD4CA3;
    arg0->matrix.translation.z = 0xE270649E;
    arg0->timer = 0x12C;
    arg0->state = 0;
    arg0->displayLists = &getSkyDisplayLists3ByIndex(gs->memoryPoolId)->sceneryDisplayLists3;
    setCleanupCallback(&cleanupStarLauncherTask);
    setCallback(&updateStarLauncherTask);
}

void updateStarLauncherTask(StarLauncherTaskUpdate *arg0) {
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
                        arg0->node.displayLists =
                            &getSkyDisplayLists3ByIndex(gameState->memoryPoolId)->sceneryDisplayLists4;
                        randVal = randA();
                        randVal = randVal - 0x60;
                        i = randVal << 1;
                        i = i + randVal;
                        i = i + 0x6C0;
                        spawnFallingStarProjectile(i, ((randA()) << 12) | 0x100000);
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
                    arg0->node.displayLists =
                        &getSkyDisplayLists3ByIndex(gameState->memoryPoolId)->sceneryDisplayLists3;
                    arg0->timer = 0x14;
                }
            }
            break;
    }

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, &arg0->node);
    }
}

void cleanupStarLauncherTask(DisplayListObjectSegments *arg0) {
    arg0->segment1 = freeNodeMemory(arg0->segment1);
    arg0->segment2 = freeNodeMemory(arg0->segment2);
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
