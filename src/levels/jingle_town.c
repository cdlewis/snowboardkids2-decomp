#include "common.h"
#include "data/course_data.h"
#include "gamestate.h"
#include "graphics/displaylist.h"
#include "math/geometry.h"
#include "math/rand.h"
#include "race/hit_reactions.h"
#include "race/track_collision.h"
#include "system/task_scheduler.h"

typedef struct {
    u8 _pad0[0x24];
    void *unk24;
    void *unk28;
} B4240Struct;

typedef struct {
    u8 _pad0[0x30];
    void *unk30;
    u8 _pad34[0x28];
    u8 unk5C;
    u8 _pad5D[0x19];
    u8 unk76;
} B4240AllocationStruct;

typedef struct {
    DisplayListObject node;
    s32 posX;
    s32 height;
    s32 posZ;
    s32 yOffset;
    s32 yVelocity;
    s16 trainIndex;
    s16 waypointIndex;
    u16 rotation;
    s16 unk56;
    s16 waitTimer;
    s16 loopCount;
} JingleTownTrain;

typedef struct {
    s32 height;
    struct {
        s32 x;
        s32 z;
    } waypoints[2];
} JingleTownTrainWaypointData;

JingleTownTrainWaypointData gTrainWaypointData[] = {
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
void cleanupJingleTownTrain(B4240Struct *);

void renderJingleTownTrain(JingleTownTrain *train) {
    s32 i;

    createYRotationMatrix((Transform3D *)train, train->rotation);
    memcpy(&train->node.transform.translation.x, &train->posX, 0xC);
    train->node.transform.translation.y += train->yOffset;

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, &train->node);
    }
}

void pullPlayersToTrain(JingleTownTrain *arg0) {
    GameState *gs;
    Vec3i pos;
    s32 target[3];
    s32 i;
    Player *player;

    gs = getCurrentAllocation();
    memcpy(&pos, &arg0->posX, 0xC);
    pos.y += 0xC0000 + arg0->yOffset;

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

void initJingleTownTrain(JingleTownTrain *arg0) {
    B4240AllocationStruct *alloc;
    LevelDisplayLists *temp;
    s32 temp2;
    s32 temp3;
    s16 temp4;

    alloc = getCurrentAllocation();
    temp = getSkyDisplayLists3ByIndex(alloc->unk5C);
    arg0->node.displayLists = &temp->sceneryDisplayLists1;
    arg0->node.segment1 = loadUncompressedAssetByIndex(alloc->unk5C);
    arg0->node.segment2 = loadCompressedSegment2AssetByIndex(alloc->unk5C);
    arg0->node.segment3 = 0;
    arg0->waypointIndex = 1;

    temp2 = gTrainWaypointData[arg0->trainIndex].height;
    arg0->unk56 = temp2;
    temp3 = gTrainWaypointData[arg0->trainIndex].waypoints[0].x;
    arg0->posX = temp3;
    arg0->posZ = gTrainWaypointData[arg0->trainIndex].waypoints[0].z;

    arg0->height = getTrackHeightAtPosition(&alloc->unk30, arg0->unk56, &arg0->posX);

    temp4 = arg0->trainIndex;
    arg0->rotation = computeAngleToPosition(
        gTrainWaypointData[temp4].waypoints[1].x,
        gTrainWaypointData[temp4].waypoints[1].z,
        arg0->posX,
        arg0->posZ
    );

    arg0->yOffset = 0;
    arg0->yVelocity = 0;

    arg0->waitTimer = ((randA() & 0xFF) >> 1) + 0xA;

    setCleanupCallback(cleanupJingleTownTrain);
    setCallbackWithContinue(handleTrainIdleState);
}

void handleTrainIdleState(JingleTownTrain *arg0) {
    B4240AllocationStruct *alloc;
    void (*callback)(JingleTownTrain *);
    s16 val;

    alloc = getCurrentAllocation();

    if (arg0->waitTimer == 0) {
        if (randA() & 1) {
            callback = handleTrainHopBehavior;
            arg0->yVelocity = 0x30000;
            val = 3;
        } else {
            callback = handleTrainJumpBehavior;
            arg0->yVelocity = 0x20000;
            val = 6;
        }
        arg0->yOffset = 0;
        arg0->loopCount = val;
        setCallback(callback);
    }

    if (alloc->unk76 == 0) {
        arg0->waitTimer--;
        pullPlayersToTrain(arg0);
    }

    renderJingleTownTrain(arg0);
}

void handleTrainHopBehavior(JingleTownTrain *arg0) {
    B4240AllocationStruct *alloc;
    void *terrainPtr;
    s32 *posPtr;
    Vec3i rotResult;
    s16 angleDiff;
    s16 newRotation;
    u16 newUnk56;

    alloc = getCurrentAllocation();
    if (alloc->unk76 == 0) {
        angleDiff = computeAngleToPosition(
            gTrainWaypointData[arg0->trainIndex].waypoints[arg0->waypointIndex].x,
            gTrainWaypointData[arg0->trainIndex].waypoints[arg0->waypointIndex].z,
            arg0->posX,
            arg0->posZ
        );
        angleDiff = (angleDiff - arg0->rotation) & 0x1FFF;
        if (angleDiff >= 0x1001) {
            angleDiff = angleDiff | 0xE000;
        }
        if (angleDiff >= 0x81) {
            angleDiff = 0x80;
        }
        if (angleDiff < -0x80) {
            angleDiff = -0x80;
        }
        newRotation = arg0->rotation + angleDiff;
        arg0->rotation = newRotation;
        rotateVectorY(&gJingleTownTrainForwardVector, newRotation, &rotResult);
        terrainPtr = &alloc->unk30;
        arg0->posX += rotResult.x;
        posPtr = &arg0->posX;
        arg0->posZ += rotResult.z;
        newUnk56 = findTrackSector(terrainPtr, arg0->unk56, posPtr);
        arg0->unk56 = newUnk56;
        arg0->height = getTrackHeightAtPosition(terrainPtr, newUnk56, posPtr);
        arg0->yOffset += arg0->yVelocity;
        arg0->yVelocity -= 0x8000;
        if (arg0->yOffset == 0) {
            arg0->loopCount--;
            if (arg0->loopCount == 0) {
                arg0->waitTimer = ((randA() & 0xFF) >> 1) + 0xA;
                setCallback(handleTrainIdleState);
            } else {
                arg0->yVelocity = 0x30000;
            }
        }
        rotResult.x = gTrainWaypointData[arg0->trainIndex].waypoints[arg0->waypointIndex].x - arg0->posX;
        rotResult.y = gTrainWaypointData[arg0->trainIndex].waypoints[arg0->waypointIndex].z - arg0->posZ;
        if (((u32)(rotResult.x + 0xFFFFF) <= 0x1FFFFEUL) && ((u32)(rotResult.z + 0xFFFFF) <= 0x1FFFFEUL)) {
            arg0->waypointIndex = (arg0->waypointIndex + 1) & 1;
        }
        pullPlayersToTrain(arg0);
    }
    renderJingleTownTrain(arg0);
}

// Handle the train's jump behavior - moves train between waypoints while performing jumping animation
void handleTrainJumpBehavior(JingleTownTrain *arg0) {
    B4240AllocationStruct *alloc;
    Vec3i rotatedVec;
    s16 angleDiff;
    s16 clampedAngle;
    void *terrainPtr;
    s32 *posPtr;

    alloc = getCurrentAllocation();
    if (alloc->unk76 == 0) {
        // Compute angle to target waypoint and clamp to [-128, 128] degrees
        angleDiff = (computeAngleToPosition(
                         gTrainWaypointData[arg0->trainIndex].waypoints[arg0->waypointIndex].x,
                         gTrainWaypointData[arg0->trainIndex].waypoints[arg0->waypointIndex].z,
                         arg0->posX,
                         arg0->posZ
                     ) -
                     arg0->rotation) &
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
        arg0->rotation += clampedAngle;
        rotateVectorY(&gJingleTownTrainForwardVector2, arg0->rotation, &rotatedVec);

        // Move train along its forward vector and update height from terrain
        terrainPtr = &alloc->unk30;
        arg0->posX += rotatedVec.x;
        posPtr = &arg0->posX;
        arg0->posZ += rotatedVec.z;

        arg0->unk56 = findTrackSector(terrainPtr, arg0->unk56, posPtr);
        arg0->height = getTrackHeightAtPosition(terrainPtr, arg0->unk56, posPtr);

        // Handle jump physics (gravity affects vertical velocity)
        arg0->yOffset += arg0->yVelocity;
        arg0->yVelocity -= 0x8000;

        // When train lands, decrement loop count and either return to idle or continue jumping
        if (arg0->yOffset == 0) {
            arg0->loopCount--;
            if (arg0->loopCount == 0) {
                arg0->waitTimer = ((randA() & 0xFF) >> 1) + 0xA;
                setCallback(handleTrainIdleState);
            } else {
                arg0->yVelocity = 0x20000;
            }
        }

        // Check if train reached waypoint and advance waypoint index if so
        rotatedVec.x = gTrainWaypointData[arg0->trainIndex].waypoints[arg0->waypointIndex].x - arg0->posX;
        rotatedVec.y = gTrainWaypointData[arg0->trainIndex].waypoints[arg0->waypointIndex].z - arg0->posZ;

        if ((u32)(rotatedVec.x + 0xFFFFF) <= 0x1FFFFE && (u32)(rotatedVec.z + 0xFFFFF) <= 0x1FFFFE) {
            arg0->waypointIndex = (arg0->waypointIndex + 1) & 1;
        }

        pullPlayersToTrain(arg0);
    }
    renderJingleTownTrain(arg0);
}

void cleanupJingleTownTrain(B4240Struct *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

void spawnJingleTownTrains(void) {
    s32 i;

    for (i = 0; i < 6; i++) {
        JingleTownTrain *task = scheduleTask(initJingleTownTrain, 0, 0, 0x32);
        if (task != NULL) {
            task->trainIndex = i;
        }
    }
}
