#include "levels/sunny_mountain.h"
#include "56910.h"
#include "common.h"
#include "displaylist.h"
#include "geometry.h"
#include "rand.h"
#include "task_scheduler.h"

extern u8 D_800BBB68_B5468[];
extern u8 D_800BBB6C_B546C[];
extern u8 D_800BBB88_B5488[];
extern u8 D_800BBB8C_B548C[];

void cleanupSunnyMountainFlyingBirdTask(SunnyMountainFlyingBirdTask *arg0);

typedef struct {
    u8 _pad[0x48];
    u8 *positionData; /* 0x48: Source position data for display objects */
    u8 _pad2[0x10];
    u8 memoryPoolId; /* 0x5C: ID for memory pool and display list lookup */
} SunnyMountainAllocation;

typedef struct {
    u8 _pad[0x38];
    void *displayList;     /* 0x38: Main display list (offset 0x90 from result) */
    void *assetData;       /* 0x3C: Shared asset data pointer for all display objects */
    void *unk40;           /* 0x40: Shared pointer for all display objects */
    s32 unk44;             /* 0x44: Always set to 0 */
    u8 _pad2[0xC];
    u8 *displayObjects;    /* 0x54: Array of 4 DisplayListObjects (0xF0 bytes) */
    u8 _pad3[0x80];
    s16 unkD8;             /* 0xD8: Always set to 0 */
} SunnyMountainTaskState;

extern void func_800BB488_B4D88(void);
void cleanupSunnyMountainDisplayObjectsTask(SunnyMountainTaskState *arg0);
void updateSunnyMountainDisplayObjectsTask(s32 *arg0);

/**
 * Initializes the Sunny Mountain level task state.
 *
 * This function sets up display list objects for rendering elements in the
 * Sunny Mountain level. It allocates memory for 4 display objects and
 * initializes each with display list pointers and copies position data
 * from the allocation.
 *
 * The function:
 * 1. Gets the current allocation containing level data and memory pool ID
 * 2. Sets up the main display list pointer (offset 0x90)
 * 3. Allocates 0xF0 bytes for 4 DisplayListObject entries (0x3C bytes each)
 * 4. For each of the 4 objects:
 *    - Sets display list pointer (offset 0xA0)
 *    - Sets shared assetData and unk40 pointers
 *    - Copies 12 bytes of position data from the allocation
 * 5. Registers cleanup and update callbacks
 */
void initSunnyMountainDisplayObjectsTask(SunnyMountainTaskState *taskState) {
    s32 loopCounter;
    s32 srcPositionOffset;
    SunnyMountainTaskState *destPositionPtr;
    u8 *destPositionAddr;
    s32 displayObjectOffset;
    LevelDisplayLists *displayListResult;
    SunnyMountainAllocation *allocation;

    allocation = (SunnyMountainAllocation *)getCurrentAllocation();

    loopCounter = 0;
    displayListResult = getSkyDisplayLists3ByIndex(allocation->memoryPoolId);
    taskState->displayList = (void *)((u32)displayListResult + 0x90);

    srcPositionOffset = 0;
    taskState->assetData = loadUncompressedAssetByIndex(allocation->memoryPoolId);

    destPositionPtr = taskState;
    displayObjectOffset = 0;
    taskState->unk40 = loadCompressedSegment2AssetByIndex(allocation->memoryPoolId);

    taskState->unk44 = 0;
    taskState->unkD8 = 0;
    taskState->displayObjects = allocateNodeMemory(0xF0);

    do {
        s32 objectBaseAddr;
        loopCounter++;

        displayListResult = getSkyDisplayLists3ByIndex(allocation->memoryPoolId);
        objectBaseAddr = displayObjectOffset + (s32)taskState->displayObjects;
        *(void **)(objectBaseAddr + 0x20) = (void *)((u32)displayListResult + 0xA0);

        destPositionAddr = (u8 *)destPositionPtr;
        destPositionAddr = destPositionAddr + 0x6C;

        *(void **)(displayObjectOffset + (s32)taskState->displayObjects + 0x24) = taskState->assetData;

        *(void **)(displayObjectOffset + (s32)taskState->displayObjects + 0x28) = taskState->unk40;

        *(void **)(displayObjectOffset + (s32)taskState->displayObjects + 0x2C) = 0;

        memcpy(destPositionAddr, (u8 *)(srcPositionOffset + (s32)allocation->positionData) + 0x30, 0xC);

        destPositionPtr = (SunnyMountainTaskState *)((u8 *)destPositionPtr + 0x20);
        displayObjectOffset += 0x3C;
        srcPositionOffset += 0xC;
    } while (loopCounter < 4);

    setCleanupCallback(cleanupSunnyMountainDisplayObjectsTask);
    setCallback(updateSunnyMountainDisplayObjectsTask);
}

void updateSunnyMountainDisplayObjectsTask(s32 *arg0) {
    GameState *state = (GameState *)getCurrentAllocation();
    s32 i;

    for (i = 0; i < state->unk5F; i++) {
        if (state->players[i].sectorIndex >= 0x3D) {
            arg0[0] = 0x1C84A5B9;
            arg0[1] = 0x0B1F0000;
            arg0[2] = 0x02C792C4;
            arg0[3] = 0x1CE44F4D;
            arg0[4] = 0x0B1F0000;
            arg0[5] = 0x02E17D96;
            setCallbackWithContinue(func_800BB488_B4D88);
            return;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/levels/sunny_mountain", func_800BB488_B4D88);

void cleanupSunnyMountainDisplayObjectsTask(SunnyMountainTaskState *arg0) {
    arg0->assetData = freeNodeMemory(arg0->assetData);
    arg0->unk40 = freeNodeMemory(arg0->unk40);
    arg0->displayObjects = freeNodeMemory(arg0->displayObjects);
}

extern u16 D_800BBBA8_B54A8[];
extern s16 identityMatrix[];

void resetSunnyMountainFlyingBirdPath(SunnyMountainFlyingBirdTask *arg0);
void updateSunnyMountainFlyingBird(SunnyMountainFlyingBirdTask *arg0);

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

    memcpy(arg0, identityMatrix, 0x20);

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
