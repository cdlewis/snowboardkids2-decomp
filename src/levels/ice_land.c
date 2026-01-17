#include "levels/ice_land.h"
#include "56910.h"
#include "common.h"
#include "gamestate.h"
#include "geometry.h"
#include "task_scheduler.h"

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
    Transform3D unk0;
    DisplayLists *displayLists;
    void *segment1;
    void *segment2;
    s32 unk2C;
    u8 pad30[0xC];
    s16 matrix3C[9];
    u8 pad4E[2];
    s32 position50[3];
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

void func_800BB3A0_B2120(void);
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
    memcpy(&arg0->position50, &(arg0->waypoints - (-arg0->currentWaypointIndex))->x, 12);
    arg0->currentWaypointIndex++;
    waypoints = arg0->waypoints;
    arg0->angleX = 0;
    angle = computeAngleToPosition(
        waypoints[arg0->currentWaypointIndex].x,
        waypoints[arg0->currentWaypointIndex].z,
        arg0->position50[0],
        arg0->position50[2]
    );
    arg0->angleY = angle & 0x1FFF;
    createYRotationMatrix(&arg0->unk0, arg0->angleY);
    createCombinedRotationMatrix(&arg0->matrix3C, arg0->angleX, arg0->angleY);
    setCleanupCallback(cleanupIceLandMovingPlatform);
    setCallback(func_800BB3A0_B2120);
}

INCLUDE_ASM("asm/nonmatchings/levels/ice_land", func_800BB3A0_B2120);

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
