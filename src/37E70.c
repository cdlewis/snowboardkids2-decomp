#include "6E840.h"
#include "common.h"
#include "task_scheduler.h"

extern void func_800373E0_37FE0(void);
void func_800376EC_382EC(void);

typedef struct {
    /* 0x000 */ u8 pad0[0x1D8];
    /* 0x1D8 */ void *unk1D8;
    /* 0x1DC */ void *unk1DC;
} Allocation_37E70;

INCLUDE_ASM("asm/nonmatchings/37E70", func_80037270_37E70);

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
