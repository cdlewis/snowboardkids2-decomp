#include "56910.h"
#include "common.h"
#include "task_scheduler.h"

typedef struct {
    u8 _pad[0x14];
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    void *unk20;
    void *unk24;
    void *unk28;
    s32 unk2C;
} func_800BB2B0_B2750_arg;

extern void *freeNodeMemory(void *);

void func_800BB334_B27D4(void);
void func_800BB390_B2830(func_800BB2B0_B2750_arg *arg0);

void func_800BB2B0_B2750(func_800BB2B0_B2750_arg *arg0) {
    GameState *allocation;
    func_80055E68_56A68_result *result;

    allocation = (GameState *)getCurrentAllocation();
    result = func_80055E68_56A68(allocation->memoryPoolId);
    arg0->unk20 = (void *)((u32)result + 0x90);
    arg0->unk24 = func_80055DC4_569C4(0xC);
    arg0->unk28 = func_80055DF8_569F8(0xC);
    arg0->unk14 = 0x25990000;
    arg0->unk18 = 0x1A2B0000;
    arg0->unk2C = 0;
    arg0->unk1C = 0xF7A30000;
    setCleanupCallback(func_800BB390_B2830);
    setCallback(func_800BB334_B27D4);
}

INCLUDE_ASM("asm/nonmatchings/B2750", func_800BB334_B27D4);

void func_800BB390_B2830(func_800BB2B0_B2750_arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}
