#include "levels/sunny_mountain.h"
#include "common.h"
#include "data/course_data.h"
#include "gamestate.h"
#include "graphics/displaylist.h"
#include "math/geometry.h"
#include "math/rand.h"
#include "system/task_scheduler.h"

typedef struct {
    u8 _pad[0x48];
    u8 *positionData; /* 0x48: Source position data for display objects */
    u8 _pad2[0x10];
    u8 memoryPoolId; /* 0x5C: ID for memory pool and display list lookup */
} SunnyMountainAllocation;

typedef struct {
    /* 0x00 */ s32 posX;
    /* 0x04 */ s32 posY;
    /* 0x08 */ s32 posZ;
    /* 0x0C */ s32 targetX;
    /* 0x10 */ s32 targetY;
    /* 0x14 */ s32 targetZ;
    /* 0x18 */ Transform3D mainMatrix;
    /* 0x38 */ void *displayList;
    /* 0x3C */ void *assetData;
    /* 0x40 */ void *compressedAssetData;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ u8 _pad48[0xC];
    /* 0x54 */ u8 *displayObjects;
    /* 0x58 */ Transform3D chairMatrices[4];
    /* 0xD8 */ s16 waypointIndex;
    /* 0xDA */ u16 rotationAngle;
} SunnyMountainChairLiftTask;

typedef struct {
    s32 x;
    s32 z;
} ChairLiftWaypoint;

ChairLiftWaypoint gChairLiftWaypoints[] = {
    { 0x1B890F65, 0x02D8ABE0 },
    { 0x1B0B6E25, 0x032D3228 },
    { 0x1A6E1521, 0x0349046C },
};

u8 D_800BBB68_B5468[] = {
    0xFE, 0xDC, 0x72, 0x63, 0x15, 0x8C, 0xC0, 0x1C, 0xF8, 0x99, 0xEF, 0x02, 0x16, 0x34, 0xE3, 0xDF,
    0x03, 0x09, 0xE6, 0x76, 0x12, 0x1F, 0x7F, 0xD1, 0xFB, 0x9B, 0xA7, 0x02, 0x0D, 0x9D, 0x8A, 0x41,
};
#define D_800BBB6C_B546C (D_800BBB68_B5468 + 4)

u8 D_800BBB88_B5488[] = {
    0xF8, 0x8C, 0xBE, 0x95, 0x12, 0xDC, 0x60, 0xCA, 0xFE, 0xBB, 0x6F, 0x02, 0x16, 0x3E, 0x63, 0xDF,
    0xFE, 0x4D, 0x73, 0x74, 0x0D, 0x15, 0x02, 0x07, 0xFF, 0xB8, 0x44, 0xA4, 0x11, 0x9B, 0xB5, 0x1F,
};
#define D_800BBB8C_B548C (D_800BBB88_B5488 + 4)

u16 D_800BBBA8_B54A8[] = {
    0x005A, 0x0078, 0x00B4, 0x00F0, 0x012C, 0x0168, 0x01A4, 0x01E0, 0x0000, 0x0000, 0x0000, 0x0000,
};

void cleanupSunnyMountainChairLiftTask(SunnyMountainChairLiftTask *arg0);
void startSunnyMountainChairLift(s32 *arg0);
void updateSunnyMountainChairLiftMovement(SunnyMountainChairLiftTask *taskState);
void cleanupSunnyMountainFlyingBirdTask(SunnyMountainFlyingBirdTask *arg0);
void resetSunnyMountainFlyingBirdPath(SunnyMountainFlyingBirdTask *arg0);
void updateSunnyMountainFlyingBird(SunnyMountainFlyingBirdTask *arg0);

/**
 * Initializes the Sunny Mountain chair lift task.
 *
 * Sets up display list objects for the chair lift system. Allocates memory
 * for 4 chair display objects and initializes each with display list pointers
 * and position data.
 */
void initSunnyMountainChairLiftTask(SunnyMountainChairLiftTask *taskState) {
    s32 i;
    s32 srcPositionOffset;
    SunnyMountainChairLiftTask *destPositionPtr;
    u8 *destPositionAddr;
    s32 displayObjectOffset;
    LevelDisplayLists *displayLists;
    SunnyMountainAllocation *allocation;

    allocation = (SunnyMountainAllocation *)getCurrentAllocation();

    i = 0;
    displayLists = getSkyDisplayLists3ByIndex(allocation->memoryPoolId);
    taskState->displayList = (void *)((u32)displayLists + 0x90);

    srcPositionOffset = 0;
    taskState->assetData = loadUncompressedAssetByIndex(allocation->memoryPoolId);

    destPositionPtr = taskState;
    displayObjectOffset = 0;
    taskState->compressedAssetData = loadCompressedSegment2AssetByIndex(allocation->memoryPoolId);

    taskState->unk44 = 0;
    taskState->waypointIndex = 0;
    taskState->displayObjects = allocateNodeMemory(0xF0);

    do {
        s32 objectBaseAddr;
        i++;

        displayLists = getSkyDisplayLists3ByIndex(allocation->memoryPoolId);
        objectBaseAddr = displayObjectOffset + (s32)taskState->displayObjects;
        *(void **)(objectBaseAddr + 0x20) = (void *)((u32)displayLists + 0xA0);

        destPositionAddr = (u8 *)destPositionPtr;
        destPositionAddr = destPositionAddr + 0x6C;

        *(void **)(displayObjectOffset + (s32)taskState->displayObjects + 0x24) = taskState->assetData;

        *(void **)(displayObjectOffset + (s32)taskState->displayObjects + 0x28) = taskState->compressedAssetData;

        *(void **)(displayObjectOffset + (s32)taskState->displayObjects + 0x2C) = 0;

        memcpy(destPositionAddr, (u8 *)(srcPositionOffset + (s32)allocation->positionData) + 0x30, 0xC);

        destPositionPtr = (SunnyMountainChairLiftTask *)((u8 *)destPositionPtr + 0x20);
        displayObjectOffset += 0x3C;
        srcPositionOffset += 0xC;
    } while (i < 4);

    setCleanupCallback(cleanupSunnyMountainChairLiftTask);
    setCallback(startSunnyMountainChairLift);
}

void startSunnyMountainChairLift(s32 *arg0) {
    GameState *state = (GameState *)getCurrentAllocation();
    s32 i;

    for (i = 0; i < state->playerCount; i++) {
        if (state->players[i].sectorIndex >= 0x3D) {
            arg0[0] = 0x1C84A5B9;
            arg0[1] = 0x0B1F0000;
            arg0[2] = 0x02C792C4;
            arg0[3] = 0x1CE44F4D;
            arg0[4] = 0x0B1F0000;
            arg0[5] = 0x02E17D96;
            setCallbackWithContinue(updateSunnyMountainChairLiftMovement);
            return;
        }
    }
}

void updateSunnyMountainChairLiftMovement(SunnyMountainChairLiftTask *taskState) {
    s32 i;
    s32 dz;
    s32 distance;
    s32 j;
    s32 displayObjectOffset;

    i = gChairLiftWaypoints[taskState->waypointIndex].x - taskState->posX;
    dz = gChairLiftWaypoints[taskState->waypointIndex].z - taskState->posZ;

    distance = isqrt64((s64)i * (s64)i + (s64)dz * (s64)dz);

    if (distance > 0x10000) {
        i = (s64)i * 0x10000 / distance;
        dz = (s64)dz * 0x10000 / distance;
        taskState->rotationAngle = taskState->rotationAngle + 0x40;
    } else if (taskState->waypointIndex != 2) {
        taskState->waypointIndex = taskState->waypointIndex + 1;
    } else {
        i -= i >> 3;
        dz -= dz >> 3;
    }

    taskState->posX += i;
    taskState->posZ += dz;
    i = taskState->targetX - taskState->posX;
    dz = taskState->targetZ - taskState->posZ;

    distance = isqrt64((s64)i * (s64)i + (s64)dz * (s64)dz);

    i = (s64)i * 0x200000 / distance;
    dz = (s64)dz * 0x200000 / distance;

    taskState->targetX = i + taskState->posX;
    taskState->targetZ = dz + taskState->posZ;

    createYRotationMatrix(
        &taskState->mainMatrix,
        computeAngleToPosition(taskState->posX, taskState->posZ, taskState->targetX, taskState->targetZ) & 0xFFFF
    );

    taskState->mainMatrix.translation.x = (taskState->targetX - taskState->posX) / 2 + taskState->posX;
    taskState->mainMatrix.translation.y = (taskState->targetY - taskState->posY) / 2 + taskState->posY;
    taskState->mainMatrix.translation.z = (taskState->targetZ - taskState->posZ) / 2 + taskState->posZ;

    i = 0;
    do {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)&taskState->mainMatrix);

        j = 0;
        displayObjectOffset = 0;
        do {
            if (j < 2) {
                createXRotationMatrix(taskState->chairMatrices[j].m, taskState->rotationAngle);
            } else {
                createCombinedRotationMatrix(&taskState->chairMatrices[j], -taskState->rotationAngle & 0xFFFF, 0x1000);
            }
            func_8006B084_6BC84(
                &taskState->chairMatrices[j],
                &taskState->mainMatrix,
                (Transform3D *)(taskState->displayObjects + displayObjectOffset)
            );
            enqueueDisplayListWithFrustumCull(
                i,
                (DisplayListObject *)(taskState->displayObjects + displayObjectOffset)
            );
            j++;
            displayObjectOffset = j * 0x3C;
        } while (j < 4);

        i++;
    } while (i < 4);
}

void cleanupSunnyMountainChairLiftTask(SunnyMountainChairLiftTask *arg0) {
    arg0->assetData = freeNodeMemory(arg0->assetData);
    arg0->compressedAssetData = freeNodeMemory(arg0->compressedAssetData);
    arg0->displayObjects = freeNodeMemory(arg0->displayObjects);
}

void initSunnyMountainFlyingBirdTask(SunnyMountainFlyingBirdTask *arg0) {
    GameState *state = (GameState *)getCurrentAllocation();

    arg0->displayLists = (void *)((u32)getSkyDisplayLists3ByIndex(state->memoryPoolId) + 0xB0);
    arg0->uncompressedAssetData = loadUncompressedAssetByIndex(state->memoryPoolId);
    arg0->compressedAssetData = loadCompressedSegment2AssetByIndex(state->memoryPoolId);
    arg0->segment3Ptr = 0;
    arg0->delayTimer = 0x3C;
    setCleanupCallback(cleanupSunnyMountainFlyingBirdTask);
    setCallback(resetSunnyMountainFlyingBirdPath);
}

void resetSunnyMountainFlyingBirdPath(SunnyMountainFlyingBirdTask *arg0) {
    s32 offset;
    s32 dx;
    s32 dy;
    s32 endX;
    s32 endZ;
    s32 startX;
    s32 startZ;
    s32 temp;

    if (arg0->delayTimer != 0) {
        arg0->delayTimer = arg0->delayTimer - 1;
        return;
    }

    memcpy(arg0, &identityMatrix, 0x20);

    offset = (randA() & 3) * 8;

    endX = *(s32 *)(D_800BBB88_B5488 + offset);
    startX = *(s32 *)(D_800BBB68_B5468 + offset);
    arg0->xVelocity = (endX - startX) / 60;

    endZ = *(s32 *)(D_800BBB8C_B548C + offset);
    startZ = *(s32 *)(D_800BBB6C_B546C + offset);
    arg0->zVelocity = (endZ - startZ) / 60;

    arg0->rotationAngle = computeAngleToPosition(
        *(s32 *)(D_800BBB88_B5488 + offset),
        *(s32 *)(D_800BBB8C_B548C + offset),
        *(s32 *)(D_800BBB68_B5468 + offset),
        *(s32 *)(D_800BBB6C_B546C + offset)
    );

    dx = arg0->xVelocity;
    dy = arg0->zVelocity;

    temp = *(s32 *)(D_800BBB68_B5468 + offset);
    arg0->transform.translation.y = 0x243D1AC3;
    arg0->transform.translation.x = temp;
    arg0->transform.translation.z = *(s32 *)(D_800BBB6C_B546C + offset);

    arg0->distance = isqrt64((s64)dx * (s64)dx + (s64)dy * (s64)dy);
    arg0->yVelocity = 0x300000;
    setCallbackWithContinue(updateSunnyMountainFlyingBird);
}

void updateSunnyMountainFlyingBird(SunnyMountainFlyingBirdTask *arg0) {
    int new_var;
    s16 angle;
    s32 temp_a2;
    s32 temp_a0;
    s32 temp_a3;
    s32 temp_v1;
    s32 i;
    s32 temp_a1;
    s32 temp_v0;

    angle = atan2Fixed(arg0->yVelocity, -arg0->distance);

    createCombinedRotationMatrix(arg0, angle, arg0->rotationAngle);

    temp_v0 = arg0->transform.translation.x;
    temp_a2 = arg0->xVelocity;
    temp_a0 = arg0->transform.translation.z;
    temp_a3 = arg0->zVelocity;
    temp_a1 = arg0->transform.translation.y;

    arg0->transform.translation.x = temp_v0 + temp_a2;
    arg0->transform.translation.z = temp_a0 + temp_a3;
    arg0->transform.translation.y = temp_a1 + arg0->yVelocity;

    if ((arg0->yVelocity = arg0->yVelocity + 0xFFFE6667) < ((s32)0xFFD00000)) {
        new_var = randA() & 7;
        arg0->delayTimer = D_800BBBA8_B54A8[new_var];
        setCallbackWithContinue(resetSunnyMountainFlyingBirdPath);
    }

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
    }
}

void cleanupSunnyMountainFlyingBirdTask(SunnyMountainFlyingBirdTask *arg0) {
    arg0->uncompressedAssetData = freeNodeMemory(arg0->uncompressedAssetData);
    arg0->compressedAssetData = freeNodeMemory(arg0->compressedAssetData);
}
