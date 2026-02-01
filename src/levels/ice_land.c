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
    s16 rollAngle;
    s16 currentWaypointIndex;
    s16 pad6C;
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

s32 gIceLandPlatformForwardVec[] = { 0, 0, 0x00080000, 0 };

void updateIceLandMovingPlatform(IceLandMovingPlatformTask *arg0);
void cleanupIceLandMovingPlatform(IceLandMovingPlatformCleanupArgs *arg0);
void scheduleIceLandMovingPlatform(IceLandMovingPlatformSchedulerTask *arg0);

void initIceLandMovingPlatform(IceLandMovingPlatformTask *platform) {
    Allocation *allocation;
    WaypointEntry *waypoints;
    unsigned int angle;

    allocation = getCurrentAllocation();
    platform->displayLists = &getSkyDisplayLists3ByIndex(allocation->memoryPoolId)->sceneryDisplayLists1;
    platform->segment1 = loadUncompressedAssetByIndex(allocation->memoryPoolId);
    platform->segment2 = loadCompressedSegment2AssetByIndex(allocation->memoryPoolId);
    platform->currentWaypointIndex = 0;
    platform->unk2C = 0;
    platform->pad6C = 0;
    memcpy(&platform->posX, &(platform->waypoints - (-platform->currentWaypointIndex))->x, 12);
    platform->currentWaypointIndex++;
    waypoints = platform->waypoints;
    platform->angleX = 0;
    angle = computeAngleToPosition(
        waypoints[platform->currentWaypointIndex].x,
        waypoints[platform->currentWaypointIndex].z,
        platform->posX,
        platform->posZ
    );
    platform->angleY = angle & 0x1FFF;
    createYRotationMatrix(&platform->transform, platform->angleY);
    createCombinedRotationMatrix(&platform->matrix3C, platform->angleX, platform->angleY);
    setCleanupCallback(cleanupIceLandMovingPlatform);
    setCallback(updateIceLandMovingPlatform);
}

typedef struct {
    s32 pad_low[24];
    Vec3i movement;
    s32 pad7C;
    Vec3i relativePos;
    s32 pad_high[6];
} PlatformUpdateStack;

void updateIceLandMovingPlatform(IceLandMovingPlatformTask *platform) {
    PlatformUpdateStack sp;
    GameState *gameState;
    s16 *matrix;
    s16 deltaAngle;
    s16 computedAngle;
    s32 distanceToWaypoint;
    s32 playerIndex;
    s32 playerOffset;

    gameState = getCurrentAllocation();
    if (gameState->gamePaused == 0) {
        matrix = platform->matrix3C;
        transformVectorRelative(
            &platform->waypoints[platform->currentWaypointIndex].x,
            matrix,
            &sp.relativePos
        );
        // Calculate and clamp yaw angle delta to face next waypoint
        computedAngle = atan2Fixed(-sp.relativePos.x, -sp.relativePos.z) & 0x1FFF;
        deltaAngle = computedAngle;
        if (computedAngle >= 0x1000) {
            deltaAngle = computedAngle | 0xE000;
        }
        if (deltaAngle >= 0x41) {
            deltaAngle = 0x40;
        }
        if (deltaAngle < -0x40) {
            deltaAngle = -0x40;
        }
        platform->angleY = (platform->angleY + deltaAngle) & 0x1FFF;

        // Calculate and clamp pitch angle delta
        computedAngle = atan2Fixed(sp.relativePos.y, -distance_2d(sp.relativePos.x, sp.relativePos.z)) & 0x1FFF;
        deltaAngle = computedAngle;
        if (computedAngle >= 0x1000) {
            deltaAngle = computedAngle | 0xE000;
        }
        if (deltaAngle >= 0x41) {
            deltaAngle = 0x40;
        }
        if (deltaAngle < -0x40) {
            deltaAngle = -0x40;
        }

        platform->angleX = (platform->angleX + deltaAngle) & 0x1FFF;
        platform->rollAngle = platform->rollAngle + 0x100;
        createCombinedRotationMatrix(platform, platform->rollAngle, platform->angleY);
        createCombinedRotationMatrix(matrix, platform->angleX, platform->angleY);

        transformVector2(gIceLandPlatformForwardVec, matrix, &sp.movement);

        platform->posX = platform->posX + sp.movement.x;
        platform->posY = platform->posY + sp.movement.y;
        platform->posZ = platform->posZ + sp.movement.z;

        distanceToWaypoint = distance_3d(
            platform->posX - platform->waypoints[platform->currentWaypointIndex].x,
            platform->posY - platform->waypoints[platform->currentWaypointIndex].y,
            platform->posZ - platform->waypoints[platform->currentWaypointIndex].z
        );

        if (distanceToWaypoint <= 0x60000) {
            if (platform->waypoints[platform->currentWaypointIndex].isLastWaypoint != 0) {
                terminateCurrentTask();
                return;
            }
            platform->currentWaypointIndex = platform->currentWaypointIndex + 1;
        }
    }

    memcpy(&platform->transform.translation, &platform->posX, 0xC);
    platform->transform.translation.y += 0x280000;

    playerIndex = 0;
    if (gameState->gamePaused == 0) {
        s32 numPlayers = gameState->numPlayers;
        if (numPlayers > 0) {
            playerOffset = 0;
            do {
                if (isPointInPlayerCollisionSphere((Player *)((u8 *)gameState->players + playerOffset), &platform->transform.translation, 0x280000) != 0) {
                    setPlayerBouncedBackState((Player *)((u8 *)gameState->players + playerOffset));
                }
                playerIndex++;
                playerOffset += 0xBE8;
            } while (playerIndex < gameState->numPlayers);
            playerIndex = 0;
        }
    }

    do {
        enqueueDisplayListWithFrustumCull(playerIndex, (DisplayListObject *)platform);
        playerIndex++;
    } while (playerIndex < 4);
}

void cleanupIceLandMovingPlatform(IceLandMovingPlatformCleanupArgs *args) {
    args->segment1 = freeNodeMemory(args->segment1);
    args->segment2 = freeNodeMemory(args->segment2);
}

void initIceLandMovingPlatformScheduler(IceLandMovingPlatformSchedulerTask *scheduler) {
    scheduler->timer = 0x78;
    scheduler->pathIndex = 0;
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
