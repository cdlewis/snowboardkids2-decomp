#include "levels/jingle_town.h"
#include "common.h"
#include "data/course_data.h"
#include "gamestate.h"
#include "graphics/displaylist.h"
#include "math/geometry.h"
#include "math/rand.h"
#include "race/hit_reactions.h"
#include "race/track_collision.h"
#include "system/task_scheduler.h"

JingleTownTrainPath gJingleTownTrainPaths[] = {
    { 0x5D, { { 0xD8299448, 0xE2107DBE }, { 0xD92AB84E, 0xE175C07E } } },
    { 0x5E, { { 0xD9E9BEB6, 0xE04396B0 }, { 0xD8116972, 0xE07CA8B0 } } },
    { 0x5E, { { 0xD8AA2DA0, 0xDF7121BA }, { 0xD9AE7B18, 0xDF42484E } } },
    { 0x5E, { { 0xDA6127BC, 0xDE082E92 }, { 0xD7B579FE, 0xDE1DE230 } } },
    { 0x5F, { { 0xD83B6DDC, 0xDC80E5A4 }, { 0xD9CDD302, 0xDC78BC8C } } },
    { 0x5F, { { 0xDA55775A, 0xDA896736 }, { 0xD817065C, 0xDACAF1E2 } } },
};

Vec3i gJingleTownTrainForwardVector = { 0, 0, 0x20000 };
Vec3i gJingleTownTrainForwardVector2 = { 0, 0, 0x40000 };

void initJingleTownTrain(JingleTownTrain *);
void handleTrainIdleState(JingleTownTrain *);
void handleTrainHopBehavior(JingleTownTrain *);
void handleTrainJumpBehavior(JingleTownTrain *);
void cleanupJingleTownTrain(DisplayListObject *);

void renderJingleTownTrain(JingleTownTrain *train) {
    s32 i;

    createYRotationMatrix(&train->displayObject.transform, train->yawAngle);
    memcpy(&train->displayObject.transform.translation, &train->trackPosition, sizeof(Vec3i));
    train->displayObject.transform.translation.y += train->heightOffset;

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, &train->displayObject);
    }
}

void pullPlayersToTrain(JingleTownTrain *train) {
    GameState *gs;
    Vec3i pos;
    s32 target[3];
    s32 i;
    Player *player;

    gs = getCurrentAllocation();
    memcpy(&pos, &train->trackPosition, sizeof(Vec3i));
    pos.y += 0xC0000 + train->heightOffset;

    for (i = 0; i < gs->numPlayers; i++) {
        player = &gs->players[i];
        if (isPlayerInRangeAndPull(&pos, 0xC0000, player) != 0) {
            target[0] = ((player->worldPos.x + player->collisionOffset.x - pos.x) / 2) + pos.x;
            target[1] = ((player->worldPos.y + player->collisionOffset.y - pos.x) / 2) + pos.y;
            target[2] = ((player->worldPos.z + player->collisionOffset.z - pos.x) / 2) + pos.z;
            setPlayerPullState(&gs->players[i], &target[0]);
        }
    }
}

void initJingleTownTrain(JingleTownTrain *train) {
    GameState *gameState;
    LevelDisplayLists *temp;
    s32 temp2;
    s32 temp3;
    s16 temp4;

    gameState = getCurrentAllocation();
    temp = getSkyDisplayLists3ByIndex(gameState->memoryPoolId);
    train->displayObject.displayLists = &temp->sceneryDisplayLists1;
    train->displayObject.segment1 = loadUncompressedAssetByIndex(gameState->memoryPoolId);
    train->displayObject.segment2 = loadCompressedSegment2AssetByIndex(gameState->memoryPoolId);
    train->displayObject.segment3 = 0;
    train->targetWaypointIndex = 1;

    temp2 = gJingleTownTrainPaths[train->pathIndex].initialTrackSector;
    train->trackSector = temp2;
    temp3 = gJingleTownTrainPaths[train->pathIndex].waypoints[0].x;
    train->trackPosition.x = temp3;
    train->trackPosition.z = gJingleTownTrainPaths[train->pathIndex].waypoints[0].z;

    train->trackPosition.y =
        getTrackHeightAtPosition(&gameState->gameData, train->trackSector, &train->trackPosition.x);

    temp4 = train->pathIndex;
    train->yawAngle = computeAngleToPosition(
        gJingleTownTrainPaths[temp4].waypoints[1].x,
        gJingleTownTrainPaths[temp4].waypoints[1].z,
        train->trackPosition.x,
        train->trackPosition.z
    );

    train->heightOffset = 0;
    train->verticalVelocity = 0;

    train->idleTimer = ((randA() & 0xFF) >> 1) + 0xA;

    setCleanupCallback(cleanupJingleTownTrain);
    setCallbackWithContinue(handleTrainIdleState);
}

void handleTrainIdleState(JingleTownTrain *train) {
    GameState *gameState;
    void (*callback)(JingleTownTrain *);
    s16 val;

    gameState = getCurrentAllocation();

    if (train->idleTimer == 0) {
        if (randA() & 1) {
            callback = handleTrainHopBehavior;
            train->verticalVelocity = 0x30000;
            val = 3;
        } else {
            callback = handleTrainJumpBehavior;
            train->verticalVelocity = 0x20000;
            val = 6;
        }
        train->heightOffset = 0;
        train->remainingJumps = val;
        setCallback(callback);
    }

    if (gameState->gamePaused == 0) {
        train->idleTimer--;
        pullPlayersToTrain(train);
    }

    renderJingleTownTrain(train);
}

void handleTrainHopBehavior(JingleTownTrain *train) {
    GameState *gameState;
    GameDataLayout *gameData;
    s32 *posPtr;
    Vec3i rotResult;
    s16 angleDiff;
    s16 newRotation;
    u16 newTrackSector;
    gameState = getCurrentAllocation();
    angleDiff = gameState->gamePaused;
    if (angleDiff == 0) {
        angleDiff = computeAngleToPosition(
            gJingleTownTrainPaths[train->pathIndex].waypoints[train->targetWaypointIndex].x,
            gJingleTownTrainPaths[train->pathIndex].waypoints[train->targetWaypointIndex].z,
            train->trackPosition.x,
            train->trackPosition.z
        );
        angleDiff = (angleDiff - train->yawAngle) & 0x1FFF;
        if (angleDiff >= 0x1001) {
            angleDiff = angleDiff | 0xE000;
        }
        if (angleDiff >= 0x81) {
            angleDiff = 0x80;
        }
        if (angleDiff < (-0x80)) {
            angleDiff = -0x80;
        }
        newRotation = train->yawAngle + angleDiff;
        train->yawAngle = newRotation;
        rotateVectorY(&gJingleTownTrainForwardVector, newRotation, &rotResult);
        gameData = &gameState->gameData;
        train->trackPosition.x += rotResult.x;
        posPtr = &train->trackPosition.x;
        train->trackPosition.z += rotResult.z;
        newTrackSector = findTrackSector(gameData, train->trackSector, posPtr);
        train->trackSector = newTrackSector;
        train->trackPosition.y = getTrackHeightAtPosition(gameData, newTrackSector, posPtr);
        train->heightOffset += train->verticalVelocity;
        train->verticalVelocity -= 0x8000;
        if (train->heightOffset == 0) {
            train->remainingJumps--;
            if (train->remainingJumps == 0) {
                train->idleTimer = ((randA() & 0xFF) >> 1) + 0xA;
                setCallback(handleTrainIdleState);
            } else {
                train->verticalVelocity = 0x30000;
            }
        }
        rotResult.x =
            gJingleTownTrainPaths[train->pathIndex].waypoints[train->targetWaypointIndex].x - train->trackPosition.x;
        rotResult.y =
            gJingleTownTrainPaths[train->pathIndex].waypoints[train->targetWaypointIndex].z - train->trackPosition.z;
        if ((((u32)(rotResult.x + 0xFFFFF)) <= 0x1FFFFEUL) && (((u32)(rotResult.z + 0xFFFFF)) <= 0x1FFFFEUL)) {
            train->targetWaypointIndex = (train->targetWaypointIndex + 1) & 1;
        }
        pullPlayersToTrain(train);
    }
    renderJingleTownTrain(train);
}

// Handle the train's jump behavior - moves train between waypoints while performing jumping animation
void handleTrainJumpBehavior(JingleTownTrain *train) {
    GameState *gameState;
    Vec3i rotatedVec;
    s16 angleDiff;
    s16 clampedAngle;
    GameDataLayout *gameData;
    s32 *posPtr;

    gameState = getCurrentAllocation();
    if (gameState->gamePaused == 0) {
        // Compute angle to target waypoint and clamp to [-128, 128] degrees
        angleDiff = (computeAngleToPosition(
                         gJingleTownTrainPaths[train->pathIndex].waypoints[train->targetWaypointIndex].x,
                         gJingleTownTrainPaths[train->pathIndex].waypoints[train->targetWaypointIndex].z,
                         train->trackPosition.x,
                         train->trackPosition.z
                     ) -
                     train->yawAngle) &
                    0x1FFF;
        clampedAngle = angleDiff;
        if (angleDiff >= 0x1001) {
            clampedAngle = 0xE000;
            clampedAngle = angleDiff | clampedAngle;
        }
        if (clampedAngle >= 0x81) {
            clampedAngle = 0x80;
        }
        if (clampedAngle < -0x80) {
            clampedAngle = -0x80;
        }
        train->yawAngle += clampedAngle;
        rotateVectorY(&gJingleTownTrainForwardVector2, train->yawAngle, &rotatedVec);

        // Move train along its forward vector and update height from terrain
        gameData = &gameState->gameData;
        train->trackPosition.x += rotatedVec.x;
        posPtr = &train->trackPosition.x;
        train->trackPosition.z += rotatedVec.z;

        train->trackSector = findTrackSector(gameData, train->trackSector, posPtr);
        train->trackPosition.y = getTrackHeightAtPosition(gameData, train->trackSector, posPtr);

        // Handle jump physics (gravity affects vertical velocity)
        train->heightOffset += train->verticalVelocity;
        train->verticalVelocity -= 0x8000;

        // When train lands, decrement loop count and either return to idle or continue jumping
        if (train->heightOffset == 0) {
            train->remainingJumps--;
            if (train->remainingJumps == 0) {
                train->idleTimer = ((randA() & 0xFF) >> 1) + 0xA;
                setCallback(handleTrainIdleState);
            } else {
                train->verticalVelocity = 0x20000;
            }
        }

        // Check if train reached waypoint and advance waypoint index if so
        rotatedVec.x =
            gJingleTownTrainPaths[train->pathIndex].waypoints[train->targetWaypointIndex].x - train->trackPosition.x;
        rotatedVec.y =
            gJingleTownTrainPaths[train->pathIndex].waypoints[train->targetWaypointIndex].z - train->trackPosition.z;

        if ((u32)(rotatedVec.x + 0xFFFFF) <= 0x1FFFFE && (u32)(rotatedVec.z + 0xFFFFF) <= 0x1FFFFE) {
            train->targetWaypointIndex = (train->targetWaypointIndex + 1) & 1;
        }

        pullPlayersToTrain(train);
    }
    renderJingleTownTrain(train);
}

void cleanupJingleTownTrain(DisplayListObject *displayObject) {
    displayObject->segment1 = freeNodeMemory(displayObject->segment1);
    displayObject->segment2 = freeNodeMemory(displayObject->segment2);
}

void spawnJingleTownTrains(void) {
    s32 i;

    for (i = 0; i < 6; i++) {
        JingleTownTrain *task = scheduleTask(initJingleTownTrain, 0, 0, 0x32);
        if (task != NULL) {
            task->pathIndex = i;
        }
    }
}
