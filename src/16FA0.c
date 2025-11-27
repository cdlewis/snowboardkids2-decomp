#include "common.h"

extern void *getCurrentAllocation(void);
extern void func_8000056C_116C(s32, s32, void *);
extern void setCleanupCallback(void *);
extern void func_80000460_1060(s32, void *, void *);
extern void setCallback(void *);

void func_800168BC_174BC(void);
void func_800168D8_174D8(void);

INCLUDE_ASM("asm/nonmatchings/16FA0", func_800163A0_16FA0);

INCLUDE_ASM("asm/nonmatchings/16FA0", func_800163CC_16FCC);

INCLUDE_ASM("asm/nonmatchings/16FA0", func_800163F8_16FF8);

INCLUDE_ASM("asm/nonmatchings/16FA0", func_80016434_17034);

INCLUDE_ASM("asm/nonmatchings/16FA0", func_80016488_17088);

INCLUDE_ASM("asm/nonmatchings/16FA0", func_800165D8_171D8);

INCLUDE_ASM("asm/nonmatchings/16FA0", func_800166FC_172FC);

INCLUDE_ASM("asm/nonmatchings/16FA0", func_80016728_17328);

INCLUDE_ASM("asm/nonmatchings/16FA0", func_800167B0_173B0);

INCLUDE_ASM("asm/nonmatchings/16FA0", func_80016834_17434);

void func_80016860_17460(s32 arg0) {
    s32 sp28;
    s32 sp10[6];
    void *alloc;

    alloc = getCurrentAllocation();
    func_8000056C_116C(arg0, 2, (u8 *)alloc + 0x1D8);
    setCleanupCallback(func_800168D8_174D8);
    func_80000460_1060(arg0, sp10, &sp28);
    setCallback(func_800168BC_174BC);
}

INCLUDE_ASM("asm/nonmatchings/16FA0", func_800168BC_174BC);

INCLUDE_ASM("asm/nonmatchings/16FA0", func_800168D8_174D8);

INCLUDE_ASM("asm/nonmatchings/16FA0", func_800168F4_174F4);

INCLUDE_ASM("asm/nonmatchings/16FA0", func_80016964_17564);

INCLUDE_ASM("asm/nonmatchings/16FA0", func_80016A00_17600);

INCLUDE_ASM("asm/nonmatchings/16FA0", func_80016B68_17768);

INCLUDE_ASM("asm/nonmatchings/16FA0", func_80016C28_17828);

INCLUDE_ASM("asm/nonmatchings/16FA0", func_80016D0C_1790C);

INCLUDE_ASM("asm/nonmatchings/16FA0", func_80016DE0_179E0);

INCLUDE_ASM("asm/nonmatchings/16FA0", func_80016E70_17A70);
