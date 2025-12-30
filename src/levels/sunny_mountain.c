#include "56910.h"
#include "common.h"
#include "displaylist.h"
#include "geometry.h"
#include "rand.h"
#include "task_scheduler.h"

typedef struct {
    u8 _pad[0x20];
    void *unk20;
    void *unk24;
    void *unk28;
    s32 unk2C;
    u8 _pad2[0x4C - 0x30];
    s16 unk4C;
} func_800BB814_B5114_arg;

typedef struct {
    u8 _pad[0x3C];
    void *unk3C;
    void *unk40;
    u8 _pad2[0x10];
    void *unk54;
} func_800BB7D0_arg;

extern u8 D_800BBB68_B5468[];
extern u8 D_800BBB6C_B546C[];
extern u8 D_800BBB88_B5488[];
extern u8 D_800BBB8C_B548C[];

void func_800BBB18_B5418(func_800BB814_B5114_arg *arg0);

typedef struct {
    u8 _pad[0x48];
    u8 *positionData;           /* 0x48: Source position data for display objects */
    u8 _pad2[0x10];
    u8 memoryPoolId;            /* 0x5C: ID for memory pool and display list lookup */
} SunnyMountainAllocation;

typedef struct {
    u8 _pad[0x38];
    void *displayList;          /* 0x38: Main display list (offset 0x90 from result) */
    void *unk3C;                /* 0x3C: Shared pointer for all display objects */
    void *unk40;                /* 0x40: Shared pointer for all display objects */
    s32 unk44;                  /* 0x44: Always set to 0 */
    u8 _pad2[0xC];
    u8 *displayObjects;         /* 0x54: Array of 4 DisplayListObjects (0xF0 bytes) */
    u8 _pad3[0x80];
    s16 unkD8;                  /* 0xD8: Always set to 0 */
} SunnyMountainTaskState;

extern void func_800BB488_B4D88(void);
void func_800BB7D0_B50D0(func_800BB7D0_arg *arg0);
void func_800BB3D8_B4CD8(s32 *arg0);

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
 *    - Sets shared unk3C and unk40 pointers
 *    - Copies 12 bytes of position data from the allocation
 * 5. Registers cleanup and update callbacks
 */
void func_800BB2B0_B4BB0(SunnyMountainTaskState *taskState) {
    s32 loopCounter;
    s32 srcPositionOffset;
    SunnyMountainTaskState *destPositionPtr;
    u8 *destPositionAddr;
    s32 displayObjectOffset;
    func_80055E68_56A68_result *displayListResult;
    SunnyMountainAllocation *allocation;

    allocation = (SunnyMountainAllocation *)getCurrentAllocation();

    loopCounter = 0;
    displayListResult = func_80055E68_56A68(allocation->memoryPoolId);
    taskState->displayList = (void *)((u32)displayListResult + 0x90);

    srcPositionOffset = 0;
    taskState->unk3C = func_80055DC4_569C4(allocation->memoryPoolId);

    destPositionPtr = taskState;
    displayObjectOffset = 0;
    taskState->unk40 = func_80055DF8_569F8(allocation->memoryPoolId);

    taskState->unk44 = 0;
    taskState->unkD8 = 0;
    taskState->displayObjects = allocateNodeMemory(0xF0);

    do {
        s32 objectBaseAddr;
        loopCounter++;

        displayListResult = func_80055E68_56A68(allocation->memoryPoolId);
        objectBaseAddr = displayObjectOffset + (s32)taskState->displayObjects;
        *(void **)(objectBaseAddr + 0x20) = (void *)((u32)displayListResult + 0xA0);

        destPositionAddr = (u8 *)destPositionPtr;
        destPositionAddr = destPositionAddr + 0x6C;

        *(void **)(displayObjectOffset + (s32)taskState->displayObjects + 0x24) = taskState->unk3C;

        *(void **)(displayObjectOffset + (s32)taskState->displayObjects + 0x28) = taskState->unk40;

        *(void **)(displayObjectOffset + (s32)taskState->displayObjects + 0x2C) = 0;

        memcpy(destPositionAddr, (u8 *)(srcPositionOffset + (s32)allocation->positionData) + 0x30, 0xC);

        destPositionPtr = (SunnyMountainTaskState *)((u8 *)destPositionPtr + 0x20);
        displayObjectOffset += 0x3C;
        srcPositionOffset += 0xC;
    } while (loopCounter < 4);

    setCleanupCallback(func_800BB7D0_B50D0);
    setCallback(func_800BB3D8_B4CD8);
}

void func_800BB3D8_B4CD8(s32 *arg0) {
    GameState *state = (GameState *)getCurrentAllocation();
    s32 i;

    for (i = 0; i < state->unk5F; i++) {
        if (state->players[i].unkB94 >= 0x3D) {
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

void func_800BB7D0_B50D0(func_800BB7D0_arg *arg0) {
    arg0->unk3C = freeNodeMemory(arg0->unk3C);
    arg0->unk40 = freeNodeMemory(arg0->unk40);
    arg0->unk54 = freeNodeMemory(arg0->unk54);
}

extern u16 D_800BBBA8_B54A8[];
extern s16 identityMatrix[];

typedef struct {
    DisplayListObject unk0;
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;
    s16 unk4C;
    u16 unk4E;
} func_800BBA34_B5334_arg;

void func_800BB890_B5190(func_800BBA34_B5334_arg *arg0);
void func_800BBA34_B5334(func_800BBA34_B5334_arg *arg0);

void func_800BB814_B5114(func_800BB814_B5114_arg *arg0) {
    GameState *state = (GameState *)getCurrentAllocation();

    arg0->unk20 = (void *)((u32)func_80055E68_56A68(state->memoryPoolId) + 0xB0);
    arg0->unk24 = func_80055DC4_569C4(state->memoryPoolId);
    arg0->unk28 = func_80055DF8_569F8(state->memoryPoolId);
    arg0->unk2C = 0;
    arg0->unk4C = 0x3C;
    setCleanupCallback(func_800BBB18_B5418);
    setCallback(func_800BB890_B5190);
}

void func_800BB890_B5190(func_800BBA34_B5334_arg *arg0) {
    s32 offset;
    s32 dx;
    s32 dy;
    s32 endX;
    s32 endY;
    s32 startX;
    s32 startY;
    s32 temp;

    if (arg0->unk4C != 0) {
        arg0->unk4C = arg0->unk4C - 1;
        return;
    }

    memcpy(arg0, identityMatrix, 0x20);

    offset = (randA() & 3) * 8;

    endX = *(s32 *)(D_800BBB88_B5488 + offset);
    startX = *(s32 *)(D_800BBB68_B5468 + offset);
    arg0->unk40 = (endX - startX) / 60;

    endY = *(s32 *)(D_800BBB8C_B548C + offset);
    startY = *(s32 *)(D_800BBB6C_B546C + offset);
    arg0->unk44 = (endY - startY) / 60;

    arg0->unk4E = func_8006D21C_6DE1C(*(s32 *)(D_800BBB88_B5488 + offset), *(s32 *)(D_800BBB8C_B548C + offset),
                                       *(s32 *)(D_800BBB68_B5468 + offset), *(s32 *)(D_800BBB6C_B546C + offset));

    dx = arg0->unk40;
    dy = arg0->unk44;

    temp = *(s32 *)(D_800BBB68_B5468 + offset);
    arg0->unk0.transform.translation.y = 0x243D1AC3;
    arg0->unk0.transform.translation.x = temp;
    arg0->unk0.transform.translation.z = *(s32 *)(D_800BBB6C_B546C + offset);

    arg0->unk48 = isqrt64((s64)dx * (s64)dx + (s64)dy * (s64)dy);
    arg0->unk3C = 0x300000;
    setCallbackWithContinue(func_800BBA34_B5334);
}

void func_800BBA34_B5334(func_800BBA34_B5334_arg *arg0) {
    int new_var;
    s16 angle;
    s32 temp_a2;
    s32 temp_a0;
    s32 temp_a3;
    s32 temp_v1;
    s32 i;
    s32 temp_a1;
    s32 temp_v0;

    angle = atan2Fixed(arg0->unk3C, -arg0->unk48);

    createCombinedRotationMatrix(arg0, angle, arg0->unk4E);

    temp_v0 = arg0->unk0.transform.translation.x;
    temp_a2 = arg0->unk40;
    temp_a0 = arg0->unk0.transform.translation.z;
    temp_a3 = arg0->unk44;
    temp_a1 = arg0->unk0.transform.translation.y;

    arg0->unk0.transform.translation.x = temp_v0 + temp_a2;
    arg0->unk0.transform.translation.z = temp_a0 + temp_a3;
    arg0->unk0.transform.translation.y = temp_a1 + arg0->unk3C;

    if ((arg0->unk3C = arg0->unk3C + 0xFFFE6667) < ((s32)0xFFD00000)) {
        new_var = randA() & 7;
        arg0->unk4C = D_800BBBA8_B54A8[new_var];
        setCallbackWithContinue(func_800BB890_B5190);
    }

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, &arg0->unk0);
    }
}

void func_800BBB18_B5418(func_800BB814_B5114_arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}
