#include "56910.h"
#include "common.h"
#include "task_scheduler.h"

extern u8 D_40E1C0[];
extern u8 D_40E870[];

extern void *func_80035F80_36B80(s32);
extern void func_800BB320_B5C00(void *);

typedef struct {
    void *unk0;
    void *unk4;
    void *unk8;
    s16 unkC;
} func_800BB2B0_B5B90_state;

void func_800BB9E0_B62C0(func_800BB2B0_B5B90_state *arg0);

void func_800BB2B0_B5B90(func_800BB2B0_B5B90_state *arg0) {
    getCurrentAllocation();
    arg0->unk4 = func_80035F80_36B80(1);
    arg0->unk0 = loadAsset_34F7E0();
    arg0->unk8 = dmaRequestAndUpdateStateWithSize(D_40E1C0, D_40E870, 0x1130);
    arg0->unkC = 0;
    setCleanupCallback(func_800BB9E0_B62C0);
    setCallback(func_800BB320_B5C00);
}

INCLUDE_ASM("asm/nonmatchings/B5B90", func_800BB320_B5C00);

INCLUDE_ASM("asm/nonmatchings/B5B90", func_800BB51C_B5DFC);

INCLUDE_ASM("asm/nonmatchings/B5B90", func_800BB5CC_B5EAC);

INCLUDE_ASM("asm/nonmatchings/B5B90", func_800BB670_B5F50);

INCLUDE_ASM("asm/nonmatchings/B5B90", func_800BB724_B6004);

INCLUDE_ASM("asm/nonmatchings/B5B90", func_800BB87C_B615C);

INCLUDE_ASM("asm/nonmatchings/B5B90", func_800BB928_B6208);

void func_800BB9E0_B62C0(func_800BB2B0_B5B90_state *arg0) {
    arg0->unk0 = freeNodeMemory(arg0->unk0);
    arg0->unk4 = freeNodeMemory(arg0->unk4);
    arg0->unk8 = freeNodeMemory(arg0->unk8);
}
