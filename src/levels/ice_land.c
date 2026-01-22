#include "levels/ice_land.h"
#include "56910.h"
#include "5AA90.h"
#include "common.h"
#include "displaylist.h"
#include "gamestate.h"
#include "geometry.h"
#include "task_scheduler.h"

extern void setPlayerBouncedBackState(Player *player);

typedef struct {
    u8 isLastWaypoint;
    u8 pad1[3];
    s32 x;
    s32 y;
    s32 z;
} WaypointEntry;

typedef struct {
    u8 padding[0x5C];
    WaypointEntry *waypoints;
} ScheduledTask;

typedef struct {
    Transform3D transform;
    DisplayLists *displayLists;
    void *segment1;
    void *segment2;
    s32 unk2C;
    u8 pad30[0xC];
    s16 matrix3C[9];
    u8 pad4E[2];
    s32 posX;
    s32 posY;
    s32 posZ;
    WaypointEntry *waypoints;
    u8 pad60[4];
    s16 angleX;
    s16 angleY;
    s16 unk68;
    s16 currentWaypointIndex;
    s16 unk6C;
} IceLandMovingPlatformTask;

typedef struct {
    u8 pad0[0x5C];
    u8 memoryPoolId;
} Allocation;

WaypointEntry iceLandWaypoints1[] = {
    { 0, { 0 }, 0xF0B2C5A7, 0x299EB9C0, 0x2CA726AB },
    { 0, { 0 }, 0xEE729266, 0x2913033D, 0x2E4BB956 },
    { 0, { 0 }, 0xEB2DD542, 0x28CC433D, 0x3039DEC8 },
    { 0, { 0 }, 0xE7EB68A2, 0x283D833D, 0x2F348172 },
    { 0, { 0 }, 0xE9125C02, 0x281FC33D, 0x2CBBBE24 },
    { 0, { 0 }, 0xE7CE6DA4, 0x2792833D, 0x2A5485CA },
    { 0, { 0 }, 0xE6B2762E, 0x26D4C33D, 0x28DA8968 },
    { 0, { 0 }, 0xE534B0DC, 0x2650C33D, 0x27E2307A },
    { 0, { 0 }, 0xE3E7EA22, 0x2521028F, 0x27C8BD88 },
    { 0, { 0 }, 0xE1AEF454, 0x24C9E116, 0x288EC920 },
    { 1, { 0 }, 0xDFC825C4, 0x23168E64, 0x29C722E4 },
};

WaypointEntry iceLandWaypoints2[] = {
    { 0, { 0 }, 0xF0B2C5A7, 0x299EB9C0, 0x2CA726AB },
    { 0, { 0 }, 0xEE729266, 0x2913033D, 0x2E4BB956 },
    { 0, { 0 }, 0xEB2DD542, 0x28CC433D, 0x3039DEC8 },
    { 0, { 0 }, 0xE7EB68A2, 0x283D833D, 0x2F348172 },
    { 0, { 0 }, 0xE9125C02, 0x281FC33D, 0x2CBBBE24 },
    { 0, { 0 }, 0xE7CE6DA4, 0x2792833D, 0x2A5485CA },
    { 0, { 0 }, 0xE6B2762E, 0x26D4C33D, 0x28DA8968 },
    { 0, { 0 }, 0xE43D4A70, 0x25FA52E9, 0x26DC1F5C },
    { 0, { 0 }, 0xE321F9C4, 0x24E212E9, 0x26F4C3A8 },
    { 0, { 0 }, 0xE0C26AE2, 0x24839F8B, 0x2771F2D6 },
    { 1, { 0 }, 0xDDEFF7E2, 0x2272EF9C, 0x27C802AE },
};

WaypointEntry iceLandWaypoints3[] = {
    { 0, { 0 }, 0xF0B2C5A7, 0x299EB9C0, 0x2CA726AB },
    { 0, { 0 }, 0xEE729266, 0x2913033D, 0x2E4BB956 },
    { 0, { 0 }, 0xEB2DD542, 0x28CC433D, 0x3039DEC8 },
    { 0, { 0 }, 0xE7EB68A2, 0x283D833D, 0x2F348172 },
    { 0, { 0 }, 0xE9125C02, 0x281FC33D, 0x2CBBBE24 },
    { 0, { 0 }, 0xE7CE6DA4, 0x2792833D, 0x2A5485CA },
    { 0, { 0 }, 0xE6B2762E, 0x26D4C33D, 0x28DA8968 },
    { 0, { 0 }, 0xE309661A, 0x259483F2, 0x2567DD9C },
    { 0, { 0 }, 0xE1FA575A, 0x2482C3F2, 0x25B2FAAC },
    { 0, { 0 }, 0xDFC1B0D2, 0x2434FF30, 0x2633E7C4 },
    { 1, { 0 }, 0xDD8A659C, 0x2217FF28, 0x278AD968 },
};

s32 D_800BB9C0_B2740[] = { 0, 0, 0x00080000, 0 };

void func_800BB3A0_B2120(IceLandMovingPlatformTask *arg0);
void cleanupIceLandMovingPlatform(IceLandMovingPlatformCleanupArgs *arg0);
void scheduleIceLandMovingPlatform(IceLandMovingPlatformSchedulerTask *arg0);

void initIceLandMovingPlatform(IceLandMovingPlatformTask *arg0) {
    Allocation *allocation;
    WaypointEntry *waypoints;
    unsigned int angle;
    allocation = getCurrentAllocation();
    arg0->displayLists = &getSkyDisplayLists3ByIndex(allocation->memoryPoolId)->sceneryDisplayLists1;
    arg0->segment1 = loadUncompressedAssetByIndex(allocation->memoryPoolId);
    arg0->segment2 = loadCompressedSegment2AssetByIndex(allocation->memoryPoolId);
    arg0->currentWaypointIndex = 0;
    arg0->unk2C = 0;
    arg0->unk6C = 0;
    memcpy(&arg0->posX, &(arg0->waypoints - (-arg0->currentWaypointIndex))->x, 12);
    arg0->currentWaypointIndex++;
    waypoints = arg0->waypoints;
    arg0->angleX = 0;
    angle = computeAngleToPosition(
        waypoints[arg0->currentWaypointIndex].x,
        waypoints[arg0->currentWaypointIndex].z,
        arg0->posX,
        arg0->posZ
    );
    arg0->angleY = angle & 0x1FFF;
    createYRotationMatrix(&arg0->transform, arg0->angleY);
    createCombinedRotationMatrix(&arg0->matrix3C, arg0->angleX, arg0->angleY);
    setCleanupCallback(cleanupIceLandMovingPlatform);
    setCallback(func_800BB3A0_B2120);
}

typedef struct {
    s32 pad_low[24];
    Vec3i movement;
    s32 pad7C;
    Vec3i relativePos;
    s32 pad_high[6];
} StackLocals;

void func_800BB3A0_B2120(IceLandMovingPlatformTask *arg0) {
    StackLocals sp;
    GameState *allocation;
    s16 *matrix;
    WaypointEntry *waypoint;
    s16 deltaAngle;
    s16 temp;
    s32 dist;
    s32 i;
    s32 playerOffset;

    allocation = getCurrentAllocation();
    if (allocation->gamePaused == 0) {
        matrix = arg0->matrix3C;
        transformVectorRelative(
            &arg0->waypoints[arg0->currentWaypointIndex].x,
            matrix,
            &sp.relativePos
        );
        temp = atan2Fixed(-sp.relativePos.x, -sp.relativePos.z) & 0x1FFF;
        deltaAngle = temp;
        if (temp >= 0x1000) {
            deltaAngle = temp | 0xE000;
        }
        if (deltaAngle >= 0x41) {
            deltaAngle = 0x40;
        }
        if (deltaAngle < -0x40) {
            deltaAngle = -0x40;
        }
        arg0->angleY = (arg0->angleY + deltaAngle) & 0x1FFF;

        temp = atan2Fixed(sp.relativePos.y, -distance_2d(sp.relativePos.x, sp.relativePos.z)) & 0x1FFF;
        deltaAngle = temp;
        if (temp >= 0x1000) {
            deltaAngle = temp | 0xE000;
        }
        if (deltaAngle >= 0x41) {
            deltaAngle = 0x40;
        }
        if (deltaAngle < -0x40) {
            deltaAngle = -0x40;
        }

        arg0->angleX = (arg0->angleX + deltaAngle) & 0x1FFF;
        arg0->unk68 = arg0->unk68 + 0x100;
        createCombinedRotationMatrix(arg0, arg0->unk68, arg0->angleY);
        createCombinedRotationMatrix(matrix, arg0->angleX, arg0->angleY);

        transformVector2(D_800BB9C0_B2740, matrix, &sp.movement);

        arg0->posX = arg0->posX + sp.movement.x;
        arg0->posY = arg0->posY + sp.movement.y;
        arg0->posZ = arg0->posZ + sp.movement.z;

        dist = distance_3d(
            arg0->posX - arg0->waypoints[arg0->currentWaypointIndex].x,
            arg0->posY - arg0->waypoints[arg0->currentWaypointIndex].y,
            arg0->posZ - arg0->waypoints[arg0->currentWaypointIndex].z
        );

        if (dist <= 0x60000) {
            if (arg0->waypoints[arg0->currentWaypointIndex].isLastWaypoint != 0) {
                terminateCurrentTask();
                return;
            }
            arg0->currentWaypointIndex = arg0->currentWaypointIndex + 1;
        }
    }

    memcpy(&arg0->transform.translation, &arg0->posX, 0xC);
    arg0->transform.translation.y += 0x280000;

    i = 0;
    if (allocation->gamePaused == 0) {
        s32 numPlayers = allocation->numPlayers;
        if (numPlayers > 0) {
            playerOffset = 0;
            do {
                if (isPointInPlayerCollisionSphere((Player *)((u8 *)allocation->players + playerOffset), &arg0->transform.translation, 0x280000) != 0) {
                    setPlayerBouncedBackState((Player *)((u8 *)allocation->players + playerOffset));
                }
                i++;
                playerOffset += 0xBE8;
            } while (i < allocation->numPlayers);
            i = 0;
        }
    }

    do {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
        i++;
    } while (i < 4);
}

void cleanupIceLandMovingPlatform(IceLandMovingPlatformCleanupArgs *arg0) {
    arg0->segment1 = freeNodeMemory(arg0->segment1);
    arg0->segment2 = freeNodeMemory(arg0->segment2);
}

void initIceLandMovingPlatformScheduler(IceLandMovingPlatformSchedulerTask *arg0) {
    arg0->timer = 0x78;
    arg0->pathIndex = 0;
    setCallback(scheduleIceLandMovingPlatform);
}

void scheduleIceLandMovingPlatform(IceLandMovingPlatformSchedulerTask *schedulerTask) {
    GameState *gameState;
    ScheduledTask *platformTask;

    gameState = (GameState *)getCurrentAllocation();
    if (gameState->gamePaused == 0) {
        if (schedulerTask->timer == 0) {
            platformTask = (ScheduledTask *)scheduleTask(initIceLandMovingPlatform, 0, 0, 0xD3);
            if (platformTask != NULL) {
                switch (schedulerTask->pathIndex) {
                    case 0:
                        platformTask->waypoints = iceLandWaypoints1;
                        break;
                    case 1:
                        platformTask->waypoints = iceLandWaypoints2;
                        break;
                    case 2:
                        platformTask->waypoints = iceLandWaypoints3;
                        break;
                }
                schedulerTask->pathIndex++;
                if (schedulerTask->pathIndex >= 3) {
                    schedulerTask->pathIndex = 0;
                }
            }
            schedulerTask->timer = 0x78;
        } else {
            schedulerTask->timer--;
        }
    }
}
