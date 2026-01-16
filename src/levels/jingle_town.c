#include "56910.h"
#include "594E0.h"
#include "5AA90.h"
#include "common.h"
#include "displaylist.h"
#include "gamestate.h"
#include "geometry.h"
#include "rand.h"
#include "task_scheduler.h"

extern s32 gJingleTownTrainInitialHeights[];
extern s32 gJingleTownTrainWaypointsX[];
extern s32 gJingleTownTrainWaypointsZ[];
extern s32 gJingleTownTrainWaypointsX2[];
extern s32 gJingleTownTrainWaypointsZ2[];
extern Vec3i gJingleTownTrainForwardVector;
extern Vec3i gJingleTownTrainForwardVector2;

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

    temp2 = gJingleTownTrainInitialHeights[arg0->trainIndex * 5];
    arg0->unk56 = temp2;
    temp3 = gJingleTownTrainWaypointsX[arg0->trainIndex * 5];
    arg0->posX = temp3;
    arg0->posZ = gJingleTownTrainWaypointsZ[arg0->trainIndex * 5];

    arg0->height = func_80061A64_62664(&alloc->unk30, arg0->unk56, &arg0->posX);

    temp4 = arg0->trainIndex;
    arg0->rotation =
        computeAngleToPosition(gJingleTownTrainWaypointsX2[temp4 * 5], gJingleTownTrainWaypointsZ2[temp4 * 5], arg0->posX, arg0->posZ);

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
    s32 temp;
    s16 angleDiff;
    s16 newRotation;
    u16 newUnk56;

    alloc = getCurrentAllocation();
    if (alloc->unk76 == 0) {
        temp = (arg0->waypointIndex << 3) + (arg0->trainIndex * 5 << 2);
        angleDiff =
            computeAngleToPosition(gJingleTownTrainWaypointsX[temp >> 2], gJingleTownTrainWaypointsZ[temp >> 2], arg0->posX, arg0->posZ);
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
        newUnk56 = func_80060A3C_6163C(terrainPtr, arg0->unk56, posPtr);
        arg0->unk56 = newUnk56;
        arg0->height = func_80061A64_62664(terrainPtr, newUnk56, posPtr);
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
        rotResult.x = gJingleTownTrainWaypointsX[((arg0->waypointIndex << 3) + (arg0->trainIndex * 5 << 2)) >> 2] - arg0->posX;
        rotResult.y = gJingleTownTrainWaypointsZ[((arg0->waypointIndex << 3) + (arg0->trainIndex * 5 << 2)) >> 2] - arg0->posZ;
        if (((u32)(rotResult.x + 0xFFFFF) <= 0x1FFFFEUL) && ((u32)(rotResult.z + 0xFFFFF) <= 0x1FFFFEUL)) {
            arg0->waypointIndex = (arg0->waypointIndex + 1) & 1;
        }
        pullPlayersToTrain(arg0);
    }
    renderJingleTownTrain(arg0);
}

void handleTrainJumpBehavior(JingleTownTrain *arg0) {
    B4240AllocationStruct *alloc;
    Vec3i rotatedVec;
    s32 waypointOffset;
    s16 angleDiff;
    s16 clampedAngle;
    void *terrainPtr;
    s32 *posPtr;

    alloc = getCurrentAllocation();
    if (alloc->unk76 == 0) {
        waypointOffset = (arg0->waypointIndex * 8) + (arg0->trainIndex * 0x14);
        angleDiff = (computeAngleToPosition(
                     *(s32 *)((u8 *)gJingleTownTrainWaypointsX + waypointOffset),
                     *(s32 *)((u8 *)gJingleTownTrainWaypointsZ + waypointOffset),
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
        if ((s16)clampedAngle >= 0x81) {
            clampedAngle = 0x80;
        }
        if ((s16)clampedAngle < -0x80) {
            clampedAngle = -0x80;
        }
        arg0->rotation = arg0->rotation + clampedAngle;
        rotateVectorY(&gJingleTownTrainForwardVector2, (s16)arg0->rotation, &rotatedVec);

        terrainPtr = &alloc->unk30;
        arg0->posX += rotatedVec.x;
        posPtr = &arg0->posX;
        arg0->posZ += rotatedVec.z;

        arg0->unk56 = func_80060A3C_6163C(terrainPtr, arg0->unk56, posPtr);
        arg0->height = func_80061A64_62664(terrainPtr, (u16)arg0->unk56, posPtr);

        arg0->yOffset += arg0->yVelocity;
        arg0->yVelocity += -0x8000;

        if (arg0->yOffset == 0) {
            arg0->loopCount -= 1;
            if (arg0->loopCount == 0) {
                arg0->waitTimer = ((randA() & 0xFF) >> 1) + 0xA;
                setCallback(handleTrainIdleState);
            } else {
                arg0->yVelocity = 0x20000;
            }
        }

        rotatedVec.x = *(s32 *)((u8 *)gJingleTownTrainWaypointsX + (arg0->waypointIndex * 8) + (arg0->trainIndex * 0x14)) - arg0->posX;
        rotatedVec.y = *(s32 *)((u8 *)gJingleTownTrainWaypointsZ + (arg0->waypointIndex * 8) + (arg0->trainIndex * 0x14)) - arg0->posZ;

        if ((u32)(rotatedVec.x + 0xFFFFF) <= 0x1FFFFEU && (u32)(rotatedVec.z + 0xFFFFF) <= 0x1FFFFEU) {
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
