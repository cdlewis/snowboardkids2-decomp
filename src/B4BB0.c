#include "56910.h"
#include "common.h"
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

extern void func_800BB890_B5190(void);
void func_800BBB18_B5418(func_800BB814_B5114_arg *arg0);

INCLUDE_ASM("asm/nonmatchings/B4BB0", func_800BB2B0_B4BB0);

INCLUDE_ASM("asm/nonmatchings/B4BB0", func_800BB3D8_B4CD8);

INCLUDE_ASM("asm/nonmatchings/B4BB0", func_800BB488_B4D88);

void func_800BB7D0_B50D0(func_800BB7D0_arg *arg0) {
    arg0->unk3C = freeNodeMemory(arg0->unk3C);
    arg0->unk40 = freeNodeMemory(arg0->unk40);
    arg0->unk54 = freeNodeMemory(arg0->unk54);
}

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

INCLUDE_ASM("asm/nonmatchings/B4BB0", func_800BB890_B5190);

INCLUDE_ASM("asm/nonmatchings/B4BB0", func_800BBA34_B5334);

void func_800BBB18_B5418(func_800BB814_B5114_arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}
