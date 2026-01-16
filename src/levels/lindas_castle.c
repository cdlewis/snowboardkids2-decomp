#include "levels/lindas_castle.h"
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

typedef struct {
    s32 x;
    s32 z;
} Waypoint;

typedef struct {
    s32 unk0;
    Waypoint unk4[2];
} WaypointGroup;

extern WaypointGroup g_FlyingEnemyWaypoints[];
extern Vec3i *g_FlyingEnemyDirection;

void updateFlyingEnemyHighJump(FlyingEnemyTaskArg *task);
void pullPlayersInRange(FlyingEnemyTaskArg *arg0);
void updateFlyingEnemyLowJump(FlyingEnemyTaskArg *task);

typedef struct {
    u8 pad[0x24];
    void *displayList2;
    void *displayList3;
} FlyingEnemyCleanupArg;

void cleanupFlyingEnemyTask(FlyingEnemyCleanupArg *arg0);

typedef struct {
    u8 pad0[0x30];
    void *gameData;
    u8 pad34[0x28];
    u8 memoryPoolId;
} GameState_AB304;

typedef struct {
    u8 pad0[0x20];
    void *displayList1;
    void *displayList2;
    void *displayList3;
    s32 initialY;
    u8 pad30[0xC];
    s32 waypointX;
    s32 posY;
    s32 waypointZ;
    s32 velocityY;
    s32 gravity;
    s16 waypointIndex;
    s16 targetWaypointIndex;
    u16 rotationAngle;
    s16 surfaceType;
} FlyingEnemyTask;

void func_800BB2B0(FlyingEnemyTaskArg *arg0) {
    s32 i;

    createYRotationMatrix(&arg0->matrix, arg0->rotationAngle);
    memcpy(&arg0->matrix.translation.x, arg0->targetPosition, 0xC);
    arg0->matrix.translation.y += arg0->velocityY;

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
    }
}

void pullPlayersInRange(FlyingEnemyTaskArg *arg0) {
    GameState *gs;
    Vec3i pos;
    s32 pullTarget[3];
    s32 i;
    Player *player;

    gs = getCurrentAllocation();
    memcpy(&pos, &arg0->targetPosition[0], 0xC);
    pos.y += 0x1C0000 + arg0->velocityY;

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
    GameState_AB304 *gamestate;
    void *temp;
    s16 index;
    s32 initialVelocity;
    void (*updateCallback)(void);

    gamestate = (GameState_AB304 *)getCurrentAllocation();

    temp = func_80055E68_56A68(gamestate->memoryPoolId);
    task->displayList1 = (void *)((u32)temp + 0x90);

    task->displayList2 = loadUncompressedAssetByIndex(gamestate->memoryPoolId);
    task->displayList3 = loadCompressedSegment2AssetByIndex(gamestate->memoryPoolId);

    index = task->waypointIndex;

    task->initialY = 0;
    task->targetWaypointIndex = 1;

    task->surfaceType = g_FlyingEnemyWaypoints[task->waypointIndex].unk0;

    task->waypointX = g_FlyingEnemyWaypoints[task->waypointIndex].unk4[0].x;

    task->waypointZ = g_FlyingEnemyWaypoints[task->waypointIndex].unk4[0].z;

    task->posY = func_80061A64_62664(&gamestate->gameData, task->surfaceType, &task->waypointX);

    index = task->waypointIndex;

    task->rotationAngle = func_8006D21C_6DE1C(
        g_FlyingEnemyWaypoints[index].unk4[1].x,
        g_FlyingEnemyWaypoints[index].unk4[1].z,
        task->waypointX,
        task->waypointZ
    );

    task->velocityY = 0;
    task->gravity = 0;

    setCleanupCallback(cleanupFlyingEnemyTask);

    if ((randA() & 0xFF) < 0xB3) {
        updateCallback = (void (*)(void))updateFlyingEnemyHighJump;
        task->velocityY = 0;
        initialVelocity = 0x40000;
    } else {
        initialVelocity = 0x18000;
        updateCallback = (void (*)(void))updateFlyingEnemyLowJump;
        task->velocityY = 0;
    }

    task->gravity = initialVelocity;
    setCallback(updateCallback);
}

void updateFlyingEnemyHighJump(FlyingEnemyTaskArg *task) {
    GameState *gs;
    Vec3i rotatedVec;
    s16 angleDelta;

    gs = (GameState *)getCurrentAllocation();

    if (gs->gamePaused == 0) {
        angleDelta = func_8006D21C_6DE1C(
            g_FlyingEnemyWaypoints[task->waypointIndex].unk4[task->targetWaypointIndex].x,
            g_FlyingEnemyWaypoints[task->waypointIndex].unk4[task->targetWaypointIndex].z,
            task->targetPosition[0],
            task->targetPosition[2]
        );
        angleDelta = (angleDelta - task->rotationAngle) & 0x1FFF;

        if (angleDelta >= 0x1001) {
            angleDelta = angleDelta | 0xE000;
        }

        if (angleDelta >= 0x81) {
            angleDelta = 0x80;
        }

        if (angleDelta < -0x80) {
            angleDelta = -0x80;
        }

        task->rotationAngle = task->rotationAngle + angleDelta;

        rotateVectorY(&g_FlyingEnemyWaypoints[4], task->rotationAngle, &rotatedVec);

        task->targetPosition[0] += rotatedVec.x;
        task->targetPosition[2] += rotatedVec.z;

        task->surfaceType = func_80060A3C_6163C(&gs->gameData, task->surfaceType, &task->targetPosition[0]);
        task->targetPosition[1] = func_80061A64_62664(&gs->gameData, task->surfaceType, &task->targetPosition[0]);

        task->velocityY += task->gravity;
        task->gravity = task->gravity - 0x8000;

        if (task->velocityY == 0) {
            task->gravity = 0x40000;
        }

        rotatedVec.x = g_FlyingEnemyWaypoints[task->waypointIndex].unk4[task->targetWaypointIndex].x - task->targetPosition[0];
        rotatedVec.y = g_FlyingEnemyWaypoints[task->waypointIndex].unk4[task->targetWaypointIndex].z - task->targetPosition[2];

        if ((u32)(rotatedVec.x + 0xFFFFF) <= 0x1FFFFEU && (u32)(rotatedVec.z + 0xFFFFF) <= 0x1FFFFEU) {
            func_80069CF8_6A8F8();
        }

        pullPlayersInRange(task);
    }

    func_800BB2B0(task);
}

void updateFlyingEnemyLowJump(FlyingEnemyTaskArg *task) {
    GameState *gs;
    Vec3i rotatedVec;
    s16 angleDelta;
    GameDataLayout *gameData;

    gs = (GameState *)getCurrentAllocation();

    if (gs->gamePaused == 0) {
        angleDelta = func_8006D21C_6DE1C(
            g_FlyingEnemyWaypoints[task->waypointIndex].unk4[task->targetWaypointIndex].x,
            g_FlyingEnemyWaypoints[task->waypointIndex].unk4[task->targetWaypointIndex].z,
            task->targetPosition[0],
            task->targetPosition[2]
        );
        angleDelta = angleDelta - task->rotationAngle & 0x1FFF;

        if (angleDelta >= 0x1001) {
            angleDelta = angleDelta | 0xE000;
        }

        if (angleDelta >= 0x81) {
            angleDelta = 0x80;
        }

        if (angleDelta < -0x80) {
            angleDelta = -0x80;
        }

        task->rotationAngle = task->rotationAngle + angleDelta;

        rotateVectorY(&g_FlyingEnemyWaypoints[4].unk4[1].x, task->rotationAngle, &rotatedVec);

        gameData = &gs->gameData;
        task->targetPosition[0] += rotatedVec.x;
        task->targetPosition[2] += rotatedVec.z;

        task->surfaceType = func_80060A3C_6163C(gameData, task->surfaceType, &task->targetPosition[0]);
        task->targetPosition[1] = func_80061A64_62664(gameData, task->surfaceType, &task->targetPosition[0]);

        task->velocityY += task->gravity;
        task->gravity = task->gravity - 0x8000;

        if (task->velocityY == 0) {
            task->gravity = 0x18000;
        }

        rotatedVec.x = g_FlyingEnemyWaypoints[task->waypointIndex].unk4[task->targetWaypointIndex].x - task->targetPosition[0];
        rotatedVec.y = g_FlyingEnemyWaypoints[task->waypointIndex].unk4[task->targetWaypointIndex].z - task->targetPosition[2];

        if (rotatedVec.x + 0xFFFFF <= 0x1FFFFEU && (rotatedVec.z + 0xFFFFF) <= 0x1FFFFEU) {
            func_80069CF8_6A8F8();
        }

        pullPlayersInRange(task);
    }

    func_800BB2B0(task);
}

void cleanupFlyingEnemyTask(FlyingEnemyCleanupArg *arg0) {
    arg0->displayList2 = freeNodeMemory(arg0->displayList2);
    arg0->displayList3 = freeNodeMemory(arg0->displayList3);
}

typedef struct {
    u8 pad[0x50];
    u16 unk50;
} ScheduledTaskWith50;

void func_800BBA54_AB904(func_800BBA28_AB8D8_arg *arg0);

void func_800BBA28_AB8D8(func_800BBA28_AB8D8_arg *arg0) {
    arg0->unk0 = 0;
    arg0->unk2 = 0x14;
    setCallback(func_800BBA54_AB904);
}

void func_800BBA54_AB904(func_800BBA28_AB8D8_arg *arg0) {
    GameState *gameState = getCurrentAllocation();
    ScheduledTaskWith50 *task;
    s16 counter;
    s16 newValue;

    if (gameState->gamePaused) {
        return;
    }

    counter = arg0->unk2;
    if (counter == 0) {
        task = (ScheduledTaskWith50 *)scheduleTask(initFlyingEnemyTask, 0, 0, 0x32);
        if (task != NULL) {
            task->unk50 = arg0->unk0;
        }

        arg0->unk0++;
        if ((s16)arg0->unk0 == 4) {
            arg0->unk0 = 0;
        }

        newValue = (randA() & 0xF) + 0x14;
    } else {
        newValue = counter - 1;
    }
    arg0->unk2 = newValue;
}
void func_800BBB1C_AB9CC(s16 *arg0);

void func_800BBAF8_AB9A8(s16 *arg0) {
    *arg0 = 0;
    setCallback(func_800BBB1C_AB9CC);
}

void func_800BBB1C_AB9CC(s16 *arg0) {
    GameState *gameState = getCurrentAllocation();
    s32 i;

    for (i = 0; i < gameState->unk5F; i++) {
        if (gameState->players[i].currentLap == *arg0) {
            if ((u16)(gameState->players[i].sectorIndex - 0x95) < 0xD) {
                *arg0 = *arg0 + 1;
                playSoundEffectWithPriorityDefaultVolume(0x4D, 6);
            }
        }
    }
}
