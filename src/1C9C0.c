#include "2F990.h"
#include "3A1F0.h"
#include "6E840.h"
#include "common.h"
#include "graphics.h"
#include "task_scheduler.h"

extern s32 gControllerInputs;

void func_8001C1E0_1CDE0(void);
void func_8001C2FC_1CEFC(void);

void func_8001C7E8_1D3E8(void);
void func_8001C744_1D344(void);
void func_8001C28C_1CE8C(void);
void unlockScreenAwaitFadeIn(void);

typedef struct {
    /* 0x000 */ u8 pad0[0x1D8];
    /* 0x1D8 */ u8 pad1D8[0x1D8];
    /* 0x3B0 */ u8 pad3B0[0x1D8];
    /* 0x588 */ void *unk588;
    /* 0x58C */ void *unk58C;
    /* 0x590 */ void *unk590;
    /* 0x594 */ void *unk594;
    /* 0x598 */ void *unk598;
    /* 0x59C */ void *unk59C;
    /* 0x5A0 */ u8 pad5A0[0x20];
    /* 0x5C0 */ u16 waitCounter;
    /* 0x5C2 */ u8 pad5C2[0x3];
    /* 0x5C5 */ u8 unk5C5;
    /* 0x5C6 */ u8 pad5C6[0x3];
    /* 0x5C9 */ u8 unlockCount;
    /* 0x5CA */ u8 pad5CA[0xC];
    /* 0x5D6 */ u8 transitionState;
} Allocation_1C9C0;

INCLUDE_ASM("asm/nonmatchings/1C9C0", func_8001BDC0_1C9C0);

void waitForUnlocksAssetsReady(void) {
    Allocation_1C9C0 *allocation = (Allocation_1C9C0 *)getCurrentAllocation();

    allocation->waitCounter++;
    if (allocation->waitCounter < 3) {
        return;
    }

    allocation->waitCounter = 2;
    if (func_8003BB5C_3C75C() != 0) {
        return;
    }

    allocation->waitCounter = 0;
    func_8006FDA0_709A0(0, 0, 0xE);
    setGameStateHandler(unlockScreenAwaitFadeIn);
}

void unlockScreenAwaitFadeIn(void) {
    Allocation_1C9C0 *allocation = (Allocation_1C9C0 *)getCurrentAllocation();

    if (func_8006FE10_70A10(NULL) != 0) {
        return;
    }

    if (allocation->unlockCount != 0) {
        allocation->transitionState = 1;
        func_80058220_58E20(0xEA, 1);
        setGameStateHandler(func_8001C1E0_1CDE0);
    } else {
        allocation->transitionState = 3;
        func_80058220_58E20(0xEE, 1);
        setGameStateHandler(func_8001C28C_1CE8C);
        scheduleTask(func_800308FC_314FC, 0, 0, 0x5A);
    }
}

void func_8001C1E0_1CDE0(void) {
    Allocation_1C9C0 *allocation = (Allocation_1C9C0 *)getCurrentAllocation();

    if (allocation->transitionState != 0) {
        return;
    }

    allocation->unk5C5 = 0x14;
    scheduleTask(func_800304B8_310B8, 1, 0, 0x5A);
    scheduleTask(func_80030694_31294, 1, 0, 0x5A);
    scheduleTask(func_80030A00_31600, 0, 0, 0x5A);

    if (allocation->unlockCount >= 2) {
        scheduleTask(func_8002FA9C_3069C, 1, 0, 0x5A);
    }

    setGameStateHandler(func_8001C2FC_1CEFC);
}

void func_8001C28C_1CE8C(void) {
    Allocation_1C9C0 *allocation = (Allocation_1C9C0 *)getCurrentAllocation();

    if (allocation->transitionState != 0) {
        return;
    }

    if (gControllerInputs & 0xD000) {
        func_80058220_58E20(0xED, 1);
        allocation->transitionState = 2;
        func_8006FDA0_709A0(0, 0xFF, 0x10);
        setGameStateHandler(func_8001C744_1D344);
    }
}

INCLUDE_ASM("asm/nonmatchings/1C9C0", func_8001C2FC_1CEFC);

void func_8001C6E4_1D2E4(void) {
    Allocation_1C9C0 *allocation = (Allocation_1C9C0 *)getCurrentAllocation();

    allocation->waitCounter--;
    if (allocation->waitCounter == 0) {
        func_8006FDA0_709A0(0, 0xFF, 0x10);
        setGameStateHandler(func_8001C744_1D344);
    }

    if (allocation->transitionState != 0) {
        allocation->transitionState = 0;
    }
}

void func_8001C744_1D344(void) {
    Allocation_1C9C0 *allocation = (Allocation_1C9C0 *)getCurrentAllocation();

    if (func_8006FE10_70A10(NULL) != 0) {
        return;
    }

    unlinkNode((Node_70B00 *)allocation);
    unlinkNode((Node_70B00 *)&allocation->pad1D8);
    unlinkNode((Node_70B00 *)&allocation->pad3B0);

    allocation->unk588 = freeNodeMemory(allocation->unk588);
    allocation->unk58C = freeNodeMemory(allocation->unk58C);
    allocation->unk590 = freeNodeMemory(allocation->unk590);
    allocation->unk594 = freeNodeMemory(allocation->unk594);
    allocation->unk598 = freeNodeMemory(allocation->unk598);
    allocation->unk59C = freeNodeMemory(allocation->unk59C);

    terminateSchedulerWithCallback(func_8001C7E8_1D3E8);
}

void func_8001C7E8_1D3E8(void) {
    func_800697F4_6A3F4(1);
}
