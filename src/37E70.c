#include "36B80.h"
#include "38310.h"
#include "6E840.h"
#include "common.h"
#include "graphics.h"
#include "overlay.h"
#include "task_scheduler.h"

USE_ASSET(_4196E0);

extern void func_80027CA0_288A0(void *, s32, s32, s32);
extern void func_800378AC_384AC(void);
extern void func_80037BFC_387FC(void);
extern void func_80037E78_38A78(void);
extern void func_800373E0_37FE0(void);
void func_800373B0_37FB0(void);
void func_800376EC_382EC(void);

typedef struct {
    /* 0x000 */ u8 pad0[0x1D8];
    /* 0x1D8 */ void *unk1D8;
    /* 0x1DC */ void *unk1DC;
    /* 0x1E0 */ s16 unk1E0;
    /* 0x1E2 */ s16 unk1E2;
    /* 0x1E4 */ u8 unk1E4[4];
    /* 0x1E8 */ u8 pad1E8[0x4];
    /* 0x1EC */ u8 unk1EC;
} Allocation_37E70;

void func_80037270_37E70(void) {
    Allocation_37E70 *allocation;
    s32 i;
    u8 *ptr;

    allocation = allocateTaskMemory(0x1F0);
    setupTaskSchedulerNodes(0x14, 0, 0, 0, 0, 0, 0, 0);
    func_80027CA0_288A0(allocation, 8, 0xF, 1);
    func_8006FDA0_709A0(0, 0xFF, 0);
    func_8006FDA0_709A0(0, 0, 0x10);
    allocation->unk1D8 = dmaRequestAndUpdateStateWithSize(&_4196E0_ROM_START, &_4196E0_ROM_END, 0xBB8);
    allocation->unk1DC = func_80035F80_36B80(1);
    allocation->unk1E0 = 0;
    allocation->unk1E2 = 0;
    allocation->unk1EC = 0;
    i = 3;
    ptr = (u8 *)allocation + 3;
    do {
        ptr[0x1E4] = 0;
        i--;
        ptr--;
    } while (i >= 0);
    scheduleTask(func_80037710_38310, 0, 0, 0x5A);
    scheduleTask(func_800378AC_384AC, 0, 0, 0x5A);
    scheduleTask(func_80037BFC_387FC, 0, 0, 0x5A);
    scheduleTask(func_80037E78_38A78, 0, 0, 0x5A);
    func_800574A0_580A0(6);
    setGameStateHandler(func_800373B0_37FB0);
}

void func_800373B0_37FB0(void) {
    if (func_8006FE10_70A10(0) == 0) {
        setGameStateHandler(func_800373E0_37FE0);
    }
}

INCLUDE_ASM("asm/nonmatchings/37E70", func_800373E0_37FE0);

void func_80037688_38288(void) {
    Allocation_37E70 *allocation = (Allocation_37E70 *)getCurrentAllocation();

    if (func_8006FE10_70A10(NULL) != 0) {
        return;
    }

    unlinkNode((Node_70B00 *)allocation);
    allocation->unk1D8 = freeNodeMemory(allocation->unk1D8);
    allocation->unk1DC = freeNodeMemory(allocation->unk1DC);
    terminateSchedulerWithCallback(func_800376EC_382EC);
}

void func_800376EC_382EC(void) {
    func_800697F4_6A3F4(1);
}
