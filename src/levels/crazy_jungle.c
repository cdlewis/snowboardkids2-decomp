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
    /* 0x00 */ DisplayListObject launcherObject;
    /* 0x3C */ s16 stateTimer;
    /* 0x3E */ s16 launchState;
} StarLauncherTask;

typedef struct {
    u16 rotX;
    u16 rotY;
    Vec3i position;
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
    /* 0x00 */ DisplayListObject baseObject;
    /* 0x3C */ DisplayListObject fallingObject;
    /* 0x78 */ Transform3D spawnTransform;
    /* 0x98 */ Vec3i fallOffset;
    /* 0xA4 */ s16 xRotation;
    /* 0xA6 */ s16 positionIndex;
    /* 0xA8 */ s16 respawnTimer;
} FallingRockHazard;

void renderFallingRockHazard(FallingRockHazard *rock);
void updateFallingRockHazard(FallingRockHazard *rock);
void fallingRockImpactCallback(FallingRockHazard *rock);
void fallingRockRespawnCallback(FallingRockHazard *rock);
void cleanupStarLauncherTask(StarLauncherTask *task);
void freeDisplayListSegments(DisplayListObject *displayObject);
void updateStarLauncherTask(StarLauncherTask *task);

void initFallingRockHazard(FallingRockHazard *rock) {
    GameState *gameState;
    s32 randVal;
    LevelDisplayLists *result;

    gameState = (GameState *)getCurrentAllocation();
    randVal = randA();

    rock->positionIndex += (randVal & 1);
    result = getSkyDisplayLists3ByIndex(gameState->memoryPoolId);
    rock->baseObject.displayLists = &result->sceneryDisplayLists1;
    result = getSkyDisplayLists3ByIndex(gameState->memoryPoolId);
    rock->fallingObject.displayLists = &result->sceneryDisplayLists2;
    rock->baseObject.segment1 = loadUncompressedAssetByIndex(gameState->memoryPoolId);
    rock->baseObject.segment2 = loadCompressedSegment2AssetByIndex(gameState->memoryPoolId);
    rock->baseObject.segment3 = 0;
    rock->fallOffset.x = 0;
    rock->fallOffset.y = 0;
    rock->fallOffset.z = 0;
    rock->fallingObject.segment1 = rock->baseObject.segment1;
    rock->fallingObject.segment2 = rock->baseObject.segment2;
    rock->fallingObject.segment3 = rock->baseObject.segment3;
    memcpy(&rock->spawnTransform.translation, &crazyJungleRockPositions[rock->positionIndex].position, sizeof(Vec3i));
    createCombinedRotationMatrix(
        &rock->spawnTransform,
        crazyJungleRockPositions[rock->positionIndex].rotX,
        crazyJungleRockPositions[rock->positionIndex].rotY
    );
    rock->xRotation = 0;

    setCleanupCallback(freeDisplayListSegments);
    setCallback(updateFallingRockHazard);
}

void renderFallingRockHazard(FallingRockHazard *rock) {
    Transform3D matrix;
    s32 i;

    memcpy(&gScaleMatrix.translation, &rock->fallOffset, sizeof(Vec3i));
    composeTransform3D(&gScaleMatrix, &rock->spawnTransform, &rock->baseObject.transform);
    createXRotationMatrix(matrix.m, rock->xRotation);

    matrix.translation.y = 0x3b333;
    matrix.translation.x = 0;
    matrix.translation.z = 0x170000;

    composeTransform3D(&matrix, &rock->baseObject.transform, &rock->fallingObject.transform);

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, &rock->baseObject);
        enqueueDisplayListWithFrustumCull(i, &rock->fallingObject);
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
        xDiff = gs->players[i].worldPos.x - rock->spawnTransform.translation.x;
        zDiff = gs->players[i].worldPos.z - rock->spawnTransform.translation.z;
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
            if (rock->fallOffset.y != 0x60000) {
                rock->fallOffset.y += 0x20000;
            }

            if (rock->xRotation != (-0x600)) {
                rock->xRotation -= 0x100;
            }
        }

        renderFallingRockHazard(rock);

        if (gs->gamePaused == 0) {
            for (i = 0; i < gs->numPlayers; i++) {
                if (isPlayerInRangeAndPull(&rock->fallingObject.transform.translation, 0x12A000, &gs->players[i]) !=
                    0) {
                    if (isPlayerInRangeAndPull(&rock->fallingObject.transform.translation, 0x1E3000, &gs->players[i]) !=
                        0) {
                        setPlayerState50(&gs->players[i]);
                        queueSoundAtPosition(&rock->fallingObject.transform.translation, 0x2A);
                        setCallback(fallingRockImpactCallback);
                    }
                }
            }
        }
    } else {
        if (!gs->gamePaused) {
            if (rock->fallOffset.y > 0) {
                rock->fallOffset.y += 0xFFFE0000;
            }

            if (rock->fallOffset.y < 0) {
                rock->fallOffset.y += 0x20000;
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
    if (rock->fallOffset.y != 0xFFF00000) {
        if (gs->gamePaused == FALSE) {
            rock->fallOffset.y = rock->fallOffset.y - 0x8000;
        }
    } else {
        rock->respawnTimer = 0x12C;
        setCallback(&fallingRockRespawnCallback);
    }
    rock->xRotation = 0;
    renderFallingRockHazard(rock);

    for (i = 0; i < gs->numPlayers; i++) {
        isPlayerInRangeAndPull(&rock->fallingObject.transform.translation, 0x12A000, &gs->players[i]);
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
        memcpy(
            &rock->spawnTransform.translation,
            &crazyJungleRockPositions[rock->positionIndex].position,
            sizeof(Vec3i)
        );
        createCombinedRotationMatrix(
            &rock->spawnTransform,
            crazyJungleRockPositions[rock->positionIndex].rotX,
            crazyJungleRockPositions[rock->positionIndex].rotY
        );
        setCallback(&updateFallingRockHazard);
    }
}

void freeDisplayListSegments(DisplayListObject *displayObject) {
    displayObject->segment1 = freeNodeMemory(displayObject->segment1);
    displayObject->segment2 = freeNodeMemory(displayObject->segment2);
}

void initStarLauncherTask(StarLauncherTask *task) {
    GameState *gs = (GameState *)getCurrentAllocation();
    task->launcherObject.segment1 = loadUncompressedAssetByIndex(gs->memoryPoolId);
    task->launcherObject.segment2 = loadCompressedSegment2AssetByIndex(gs->memoryPoolId);
    task->launcherObject.segment3 = 0;
    createYRotationMatrix(&task->launcherObject.transform, 0x6C0);
    task->launcherObject.transform.translation.x = 0xDD196FEA;
    task->launcherObject.transform.translation.y = 0x0ABD4CA3;
    task->launcherObject.transform.translation.z = 0xE270649E;
    task->stateTimer = 0x12C;
    task->launchState = 0;
    task->launcherObject.displayLists = &getSkyDisplayLists3ByIndex(gs->memoryPoolId)->sceneryDisplayLists3;
    setCleanupCallback(&cleanupStarLauncherTask);
    setCallback(&updateStarLauncherTask);
}

void updateStarLauncherTask(StarLauncherTask *task) {
    GameState *gameState;
    s16 state;
    s32 i;
    u8 numPlayers;
    s32 randVal;

    gameState = getCurrentAllocation();
    state = task->launchState;

    switch (state) {
        case 0:
            if (!gameState->gamePaused) {
                task->stateTimer--;
                if ((task->stateTimer << 16) == 0) {
                    task->launchState++;
                }
            }
            break;
        case 1:
            if (gameState->gamePaused == 0) {
                numPlayers = gameState->numPlayers;

                for (i = 0; i < numPlayers; i++) {
                    if ((u32)gameState->players[i].sectorIndex - 0x60 < 6) {
                        task->launcherObject.displayLists =
                            &getSkyDisplayLists3ByIndex(gameState->memoryPoolId)->sceneryDisplayLists4;
                        randVal = randA();
                        randVal = randVal - 0x60;
                        i = randVal << 1;
                        i = i + randVal;
                        i = i + 0x6C0;
                        spawnFallingStarProjectile(i, ((randA()) << 12) | 0x100000);
                        queueSoundAtPosition(&task->launcherObject.transform.translation, 0x23);
                        task->stateTimer = 0x18;
                        task->launchState++;
                        break;
                    }
                }
            }
            break;
        case 2:
            if (gameState->gamePaused == 0) {
                task->stateTimer--;
                if ((task->stateTimer << 16) == 0) {
                    task->launchState = 0;
                    task->launcherObject.displayLists =
                        &getSkyDisplayLists3ByIndex(gameState->memoryPoolId)->sceneryDisplayLists3;
                    task->stateTimer = 0x14;
                }
            }
            break;
    }

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, &task->launcherObject);
    }
}

void cleanupStarLauncherTask(StarLauncherTask *task) {
    task->launcherObject.segment1 = freeNodeMemory(task->launcherObject.segment1);
    task->launcherObject.segment2 = freeNodeMemory(task->launcherObject.segment2);
}

void initCrazyJungleHazards(void) {
    FallingRockHazard *rock;

    rock = (FallingRockHazard *)scheduleTask(&initFallingRockHazard, 0, 0, 0x32);
    if (rock != NULL) {
        rock->positionIndex = 0;
    }

    rock = (FallingRockHazard *)scheduleTask(&initFallingRockHazard, 0, 0, 0x32);
    if (rock != NULL) {
        rock->positionIndex = 2;
    }

    rock = (FallingRockHazard *)scheduleTask(&initFallingRockHazard, 0, 0, 0x32);
    if (rock != NULL) {
        rock->positionIndex = 4;
    }

    rock = (FallingRockHazard *)scheduleTask(&initFallingRockHazard, 0, 0, 0x32);
    if (rock != NULL) {
        rock->positionIndex = 6;
    }

    rock = (FallingRockHazard *)scheduleTask(&initFallingRockHazard, 0, 0, 0x32);
    if (rock != NULL) {
        rock->positionIndex = 8;
    }

    scheduleTask(&initStarLauncherTask, 0, 0, 0xC8);
}
