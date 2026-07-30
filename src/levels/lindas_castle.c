#include "levels/lindas_castle.h"
#include "audio/audio.h"
#include "common.h"
#include "data/course_data.h"
#include "gamestate.h"
#include "graphics/displaylist.h"
#include "math/geometry.h"
#include "math/rand.h"
#include "race/hit_reactions.h"
#include "race/track_collision.h"
#include "system/task_scheduler.h"

FlyingEnemyPath g_FlyingEnemyWaypoints[5] = {
    { 0x65, { { 0x19892B6F, 0xD8134270 }, { 0x1B502799, 0xDAD50296 } } },
    { 0x65, { { 0x18F06693, 0xD8AC06B4 }, { 0x1AF2F573, 0xDB26E8F8 } } },
    { 0x68, { { 0x1F210F1B, 0xD9AAE8E0 }, { 0x1E5C75AD, 0xD657D502 } } },
    { 0x68, { { 0x1FC5DBD9, 0xD995D9FE }, { 0x1F16CDC1, 0xD6300CAE } } },
    { 0,    { { 0, 0x50000 }, { 0, 0 } }                               },
};
s32 D_800BBC24 = 0x000A0000;

void updateFlyingEnemyHighJump(FlyingEnemyTask *task);
void pullPlayersInRange(FlyingEnemyTask *task);
void updateFlyingEnemyLowJump(FlyingEnemyTask *task);
void cleanupFlyingEnemyTask(FlyingEnemyTask *task);
void updateLindasCastleLapCounter(s16 *lapCounter);

void renderFlyingEnemy(FlyingEnemyTask *task) {
    s32 i;

    createYRotationMatrix(&task->displayObject.transform, task->yawAngle);
    memcpy(&task->displayObject.transform.translation, &task->trackPosition, sizeof(Vec3i));
    task->displayObject.transform.translation.y += task->heightOffset;

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, &task->displayObject);
    }
}

void pullPlayersInRange(FlyingEnemyTask *task) {
    GameState *gs;
    Vec3i pos;
    s32 pullTarget[3];
    s32 i;
    Player *player;

    gs = getCurrentAllocation();
    memcpy(&pos, &task->trackPosition, sizeof(Vec3i));
    pos.y += 0x1C0000 + task->heightOffset;

    for (i = 0; i < gs->numPlayers; i++) {
        player = &gs->players[i];
        if (isPlayerInRangeAndPull(&pos, 0x1C0000, player) != 0) {
            pullTarget[0] = ((player->worldPos.x + player->collisionOffset.x - pos.x) / 2) + pos.x;
            pullTarget[1] = ((player->worldPos.y + player->collisionOffset.y - pos.x) / 2) + pos.y;
            pullTarget[2] = ((player->worldPos.z + player->collisionOffset.z - pos.x) / 2) + pos.z;
            setPlayerPullState(&gs->players[i], &pullTarget[0]);
        }
    }
}

void initFlyingEnemyTask(FlyingEnemyTask *task) {
    GameState *gamestate;
    LevelDisplayLists *temp;
    s16 index;
    s32 initialVelocity;
    void (*updateCallback)(void);

    gamestate = (GameState *)getCurrentAllocation();

    temp = getSkyDisplayLists3ByIndex(gamestate->memoryPoolId);
    task->displayObject.displayLists = &temp->sceneryDisplayLists1;
    task->displayObject.segment1 = loadUncompressedAssetByIndex(gamestate->memoryPoolId);
    task->displayObject.segment2 = loadCompressedSegment2AssetByIndex(gamestate->memoryPoolId);

    index = task->pathIndex;

    task->displayObject.segment3 = NULL;
    task->targetWaypointIndex = 1;

    task->surfaceType = g_FlyingEnemyWaypoints[task->pathIndex].surfaceType;

    task->trackPosition.x = g_FlyingEnemyWaypoints[task->pathIndex].waypoints[0].x;

    task->trackPosition.z = g_FlyingEnemyWaypoints[task->pathIndex].waypoints[0].z;

    task->trackPosition.y = getTrackHeightAtPosition(&gamestate->gameData, task->surfaceType, &task->trackPosition.x);

    index = task->pathIndex;

    task->yawAngle = computeAngleToPosition(
        g_FlyingEnemyWaypoints[index].waypoints[1].x,
        g_FlyingEnemyWaypoints[index].waypoints[1].z,
        task->trackPosition.x,
        task->trackPosition.z
    );

    task->heightOffset = 0;
    task->verticalVelocity = 0;

    setCleanupCallback(cleanupFlyingEnemyTask);

    if ((randA() & 0xFF) < 0xB3) {
        updateCallback = (void (*)(void))updateFlyingEnemyHighJump;
        task->heightOffset = 0;
        initialVelocity = 0x40000;
    } else {
        initialVelocity = 0x18000;
        updateCallback = (void (*)(void))updateFlyingEnemyLowJump;
        task->heightOffset = 0;
    }

    task->verticalVelocity = initialVelocity;
    setCallback(updateCallback);
}

void updateFlyingEnemyHighJump(FlyingEnemyTask *task) {
    GameState *gs;
    Vec3i rotatedVec;
    s16 angleDelta;
    gs = (GameState *)getCurrentAllocation();
    if (gs->gamePaused == 0) {
        angleDelta = computeAngleToPosition(
            g_FlyingEnemyWaypoints[task->pathIndex].waypoints[task->targetWaypointIndex].x,
            g_FlyingEnemyWaypoints[task->pathIndex].waypoints[task->targetWaypointIndex].z,
            task->trackPosition.x,
            task->trackPosition.z
        );
        angleDelta = (angleDelta - task->yawAngle) & 0x1FFF;
        if (angleDelta >= 0x1001) {
            angleDelta = angleDelta | 0xE000;
        }
        if (angleDelta >= 0x81) {
            angleDelta = 0x80;
        }
        if (angleDelta < (-0x80)) {
            angleDelta = -0x80;
        }

        task->yawAngle = task->yawAngle + angleDelta;

        rotateVectorY(&g_FlyingEnemyWaypoints[4], task->yawAngle, &rotatedVec);

        task->trackPosition.x += rotatedVec.x;
        task->trackPosition.z += rotatedVec.z;
        task->surfaceType = findTrackSector(&gs->gameData, task->surfaceType, &task->trackPosition.x);
        task->trackPosition.y = getTrackHeightAtPosition(&gs->gameData, task->surfaceType, &task->trackPosition.x);
        task->heightOffset += task->verticalVelocity;

        task->verticalVelocity = task->verticalVelocity - 0x8000;
        if (task->heightOffset == 0) {
            task->verticalVelocity = 0x40000;
        }

        rotatedVec.x = g_FlyingEnemyWaypoints[task->pathIndex].waypoints[angleDelta = task->targetWaypointIndex].x -
                       task->trackPosition.x;
        rotatedVec.y =
            g_FlyingEnemyWaypoints[task->pathIndex].waypoints[task->targetWaypointIndex].z - task->trackPosition.z;
        if ((((u32)(rotatedVec.x + 0xFFFFF)) <= 0x1FFFFEU) && (((u32)(rotatedVec.z + 0xFFFFF)) <= 0x1FFFFEU)) {
            terminateCurrentTask();
        }

        pullPlayersInRange(task);
    }

    renderFlyingEnemy(task);
}

void updateFlyingEnemyLowJump(FlyingEnemyTask *task) {
    GameState *gs;
    Vec3i rotatedVec;
    s16 angleDelta;
    GameDataLayout *gameData;
    gs = (GameState *)getCurrentAllocation();

    if (gs->gamePaused == 0) {
        angleDelta = computeAngleToPosition(
            g_FlyingEnemyWaypoints[task->pathIndex].waypoints[task->targetWaypointIndex].x,
            g_FlyingEnemyWaypoints[task->pathIndex].waypoints[task->targetWaypointIndex].z,
            task->trackPosition.x,
            task->trackPosition.z
        );

        angleDelta = (angleDelta - task->yawAngle) & 0x1FFF;

        if (angleDelta >= 0x1001) {
            angleDelta = angleDelta | 0xE000;
        }
        if (angleDelta >= 0x81) {
            angleDelta = 0x80;
        }
        if (angleDelta < (-0x80)) {
            angleDelta = -0x80;
        }

        task->yawAngle = task->yawAngle + angleDelta;

        rotateVectorY(&g_FlyingEnemyWaypoints[4].waypoints[1].x, task->yawAngle, &rotatedVec);

        gameData = &gs->gameData;
        task->trackPosition.x += rotatedVec.x;
        task->trackPosition.z += rotatedVec.z;

        angleDelta = (task->surfaceType = findTrackSector(gameData, task->surfaceType, &task->trackPosition.x));

        task->trackPosition.y = getTrackHeightAtPosition(gameData, task->surfaceType, &task->trackPosition.x);
        task->heightOffset += task->verticalVelocity;
        task->verticalVelocity = task->verticalVelocity - 0x8000;
        if (task->heightOffset == 0) {
            task->verticalVelocity = 0x18000;
        }

        rotatedVec.x =
            g_FlyingEnemyWaypoints[task->pathIndex].waypoints[task->targetWaypointIndex].x - task->trackPosition.x;
        rotatedVec.y =
            g_FlyingEnemyWaypoints[task->pathIndex].waypoints[task->targetWaypointIndex].z - task->trackPosition.z;

        if (((rotatedVec.x + 0xFFFFF) <= 0x1FFFFEU) && ((rotatedVec.z + 0xFFFFF) <= 0x1FFFFEU)) {
            terminateCurrentTask();
        }
        pullPlayersInRange(task);
    }
    renderFlyingEnemy(task);
}

void cleanupFlyingEnemyTask(FlyingEnemyTask *task) {
    task->displayObject.segment1 = freeNodeMemory(task->displayObject.segment1);
    task->displayObject.segment2 = freeNodeMemory(task->displayObject.segment2);
}

void initFlyingEnemySpawner(FlyingEnemySpawnerState *spawner) {
    spawner->pathIndex = 0;
    spawner->spawnTimer = 0x14;
    setCallback(updateFlyingEnemySpawner);
}

void updateFlyingEnemySpawner(FlyingEnemySpawnerState *spawner) {
    GameState *gameState = getCurrentAllocation();
    FlyingEnemyTask *task;
    s16 counter;
    s16 newValue;

    if (gameState->gamePaused) {
        return;
    }

    counter = spawner->spawnTimer;
    if (counter == 0) {
        task = (FlyingEnemyTask *)scheduleTask(initFlyingEnemyTask, 0, 0, 0x32);
        if (task != NULL) {
            task->pathIndex = spawner->pathIndex;
        }

        spawner->pathIndex++;
        if ((s16)spawner->pathIndex == 4) {
            spawner->pathIndex = 0;
        }

        newValue = (randA() & 0xF) + 0x14;
    } else {
        newValue = counter - 1;
    }

    spawner->spawnTimer = newValue;
}

void initLindasCastleLapCounter(s16 *lapCounter) {
    *lapCounter = 0;
    setCallback(updateLindasCastleLapCounter);
}

void updateLindasCastleLapCounter(s16 *lapCounter) {
    GameState *gameState = getCurrentAllocation();
    s32 i;

    for (i = 0; i < gameState->playerCount; i++) {
        if (gameState->players[i].currentLap == *lapCounter) {
            if ((u16)(gameState->players[i].sectorIndex - 0x95) < 0xD) {
                *lapCounter = *lapCounter + 1;
                playSoundEffectWithPriorityDefaultVolume(0x4D, 6);
            }
        }
    }
}
