#include "levels/sunny_mountain.h"
#include "common.h"
#include "data/course_data.h"
#include "gamestate.h"
#include "graphics/displaylist.h"
#include "math/geometry.h"
#include "math/rand.h"
#include "system/task_scheduler.h"

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
void startSunnyMountainChairLift(SunnyMountainChairLiftTask *taskState);
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
    SunnyMountainChairLiftTask *chairTransformCursor;
    u8 *chairTranslation;
    s32 displayObjectOffset;
    LevelDisplayLists *displayLists;
    GameState *gameState;

    gameState = (GameState *)getCurrentAllocation();

    i = 0;
    displayLists = getSkyDisplayLists3ByIndex(gameState->memoryPoolId);
    taskState->liftDisplayObject.displayLists = &displayLists->sceneryDisplayLists1;

    srcPositionOffset = 0;
    taskState->liftDisplayObject.segment1 = loadUncompressedAssetByIndex(gameState->memoryPoolId);

    chairTransformCursor = taskState;
    displayObjectOffset = 0;
    taskState->liftDisplayObject.segment2 = loadCompressedSegment2AssetByIndex(gameState->memoryPoolId);

    taskState->liftDisplayObject.segment3 = NULL;
    taskState->waypointIndex = 0;
    taskState->chairDisplayObjects = allocateNodeMemory(4 * sizeof(DisplayListObject));

    do {
        i++;
        displayLists = getSkyDisplayLists3ByIndex(gameState->memoryPoolId);
        ((DisplayListObject *)(displayObjectOffset + (s32)taskState->chairDisplayObjects))->displayLists =
            &displayLists->sceneryDisplayLists2;
        ((DisplayListObject *)(displayObjectOffset + (s32)taskState->chairDisplayObjects))->segment1 =
            taskState->liftDisplayObject.segment1;
        ((DisplayListObject *)(displayObjectOffset + (s32)taskState->chairDisplayObjects))->segment2 =
            taskState->liftDisplayObject.segment2;
        ((DisplayListObject *)(displayObjectOffset + (s32)taskState->chairDisplayObjects))->segment3 = NULL;
        chairTranslation = (u8 *)chairTransformCursor;
        chairTranslation += (u32) & (((SunnyMountainChairLiftTask *)0)->chairTransforms[0].translation);
        memcpy(chairTranslation, (u8 *)(srcPositionOffset + (s32)gameState->raceTransformData) + 0x30, sizeof(Vec3i));
        chairTransformCursor = (SunnyMountainChairLiftTask *)((u8 *)chairTransformCursor + sizeof(Transform3D));
        displayObjectOffset += 0x3C;
        srcPositionOffset += 0xC;
    } while (i < 4);

    setCleanupCallback(cleanupSunnyMountainChairLiftTask);
    setCallback(startSunnyMountainChairLift);
}

void startSunnyMountainChairLift(SunnyMountainChairLiftTask *taskState) {
    GameState *state = (GameState *)getCurrentAllocation();
    s32 i;

    for (i = 0; i < state->playerCount; i++) {
        if (state->players[i].sectorIndex >= 0x3D) {
            taskState->startPosition.x = 0x1C84A5B9;
            taskState->startPosition.y = 0x0B1F0000;
            taskState->startPosition.z = 0x02C792C4;
            taskState->endPosition.x = 0x1CE44F4D;
            taskState->endPosition.y = 0x0B1F0000;
            taskState->endPosition.z = 0x02E17D96;
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

    i = gChairLiftWaypoints[taskState->waypointIndex].x - taskState->startPosition.x;
    dz = gChairLiftWaypoints[taskState->waypointIndex].z - taskState->startPosition.z;

    distance = isqrt64((s64)i * (s64)i + (s64)dz * (s64)dz);

    if (distance > 0x10000) {
        i = (s64)i * 0x10000 / distance;
        dz = (s64)dz * 0x10000 / distance;
        taskState->chairRotationAngle = taskState->chairRotationAngle + 0x40;
    } else if (taskState->waypointIndex != 2) {
        taskState->waypointIndex = taskState->waypointIndex + 1;
    } else {
        i -= i >> 3;
        dz -= dz >> 3;
    }

    taskState->startPosition.x += i;
    taskState->startPosition.z += dz;
    i = taskState->endPosition.x - taskState->startPosition.x;
    dz = taskState->endPosition.z - taskState->startPosition.z;

    distance = isqrt64((s64)i * (s64)i + (s64)dz * (s64)dz);

    i = (s64)i * 0x200000 / distance;
    dz = (s64)dz * 0x200000 / distance;

    taskState->endPosition.x = i + taskState->startPosition.x;
    taskState->endPosition.z = dz + taskState->startPosition.z;

    createYRotationMatrix(
        &taskState->liftDisplayObject.transform,
        computeAngleToPosition(
            taskState->startPosition.x,
            taskState->startPosition.z,
            taskState->endPosition.x,
            taskState->endPosition.z
        ) & 0xFFFF
    );

    taskState->liftDisplayObject.transform.translation.x =
        (taskState->endPosition.x - taskState->startPosition.x) / 2 + taskState->startPosition.x;
    taskState->liftDisplayObject.transform.translation.y =
        (taskState->endPosition.y - taskState->startPosition.y) / 2 + taskState->startPosition.y;
    taskState->liftDisplayObject.transform.translation.z =
        (taskState->endPosition.z - taskState->startPosition.z) / 2 + taskState->startPosition.z;

    i = 0;
    do {
        enqueueDisplayListWithFrustumCull(i, &taskState->liftDisplayObject);

        j = 0;
        displayObjectOffset = 0;
        do {
            if (j < 2) {
                createXRotationMatrix(taskState->chairTransforms[j].m, taskState->chairRotationAngle);
            } else {
                createCombinedRotationMatrix(
                    &taskState->chairTransforms[j],
                    -taskState->chairRotationAngle & 0xFFFF,
                    0x1000
                );
            }
            composeTransform3D(
                &taskState->chairTransforms[j],
                &taskState->liftDisplayObject.transform,
                (Transform3D *)((u8 *)taskState->chairDisplayObjects + displayObjectOffset)
            );
            enqueueDisplayListWithFrustumCull(
                i,
                (DisplayListObject *)((u8 *)taskState->chairDisplayObjects + displayObjectOffset)
            );
            j++;
            displayObjectOffset = j * 0x3C;
        } while (j < 4);

        i++;
    } while (i < 4);
}

void cleanupSunnyMountainChairLiftTask(SunnyMountainChairLiftTask *arg0) {
    arg0->liftDisplayObject.segment1 = freeNodeMemory(arg0->liftDisplayObject.segment1);
    arg0->liftDisplayObject.segment2 = freeNodeMemory(arg0->liftDisplayObject.segment2);
    arg0->chairDisplayObjects = freeNodeMemory(arg0->chairDisplayObjects);
}

void initSunnyMountainFlyingBirdTask(SunnyMountainFlyingBirdTask *arg0) {
    GameState *state = (GameState *)getCurrentAllocation();

    arg0->displayObject.displayLists = &getSkyDisplayLists3ByIndex(state->memoryPoolId)->sceneryDisplayLists3;
    arg0->displayObject.segment1 = loadUncompressedAssetByIndex(state->memoryPoolId);
    arg0->displayObject.segment2 = loadCompressedSegment2AssetByIndex(state->memoryPoolId);
    arg0->displayObject.segment3 = NULL;
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

    memcpy(&arg0->displayObject.transform, &identityMatrix, sizeof(Transform3D));

    offset = (randA() & 3) * 8;

    endX = *(s32 *)(D_800BBB88_B5488 + offset);
    startX = *(s32 *)(D_800BBB68_B5468 + offset);
    arg0->xVelocity = (endX - startX) / 60;

    endZ = *(s32 *)(D_800BBB8C_B548C + offset);
    startZ = *(s32 *)(D_800BBB6C_B546C + offset);
    arg0->zVelocity = (endZ - startZ) / 60;

    arg0->flightAngle = computeAngleToPosition(
        *(s32 *)(D_800BBB88_B5488 + offset),
        *(s32 *)(D_800BBB8C_B548C + offset),
        *(s32 *)(D_800BBB68_B5468 + offset),
        *(s32 *)(D_800BBB6C_B546C + offset)
    );

    dx = arg0->xVelocity;
    dy = arg0->zVelocity;

    temp = *(s32 *)(D_800BBB68_B5468 + offset);
    arg0->displayObject.transform.translation.y = 0x243D1AC3;
    arg0->displayObject.transform.translation.x = temp;
    arg0->displayObject.transform.translation.z = *(s32 *)(D_800BBB6C_B546C + offset);

    arg0->horizontalSpeed = isqrt64((s64)dx * (s64)dx + (s64)dy * (s64)dy);
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

    angle = atan2Fixed(arg0->yVelocity, -arg0->horizontalSpeed);

    createCombinedRotationMatrix(&arg0->displayObject.transform, angle, arg0->flightAngle);

    temp_v0 = arg0->displayObject.transform.translation.x;
    temp_a2 = arg0->xVelocity;
    temp_a0 = arg0->displayObject.transform.translation.z;
    temp_a3 = arg0->zVelocity;
    temp_a1 = arg0->displayObject.transform.translation.y;

    arg0->displayObject.transform.translation.x = temp_v0 + temp_a2;
    arg0->displayObject.transform.translation.z = temp_a0 + temp_a3;
    arg0->displayObject.transform.translation.y = temp_a1 + arg0->yVelocity;

    if ((arg0->yVelocity = arg0->yVelocity + 0xFFFE6667) < ((s32)0xFFD00000)) {
        new_var = randA() & 7;
        arg0->delayTimer = D_800BBBA8_B54A8[new_var];
        setCallbackWithContinue(resetSunnyMountainFlyingBirdPath);
    }

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, &arg0->displayObject);
    }
}

void cleanupSunnyMountainFlyingBirdTask(SunnyMountainFlyingBirdTask *arg0) {
    arg0->displayObject.segment1 = freeNodeMemory(arg0->displayObject.segment1);
    arg0->displayObject.segment2 = freeNodeMemory(arg0->displayObject.segment2);
}
