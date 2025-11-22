#include "common.h"

extern s16 D_800BAD0E_1E7DBE;
extern void func_80057ABC_586BC(s32, s32);

s16 func_800B4AF0_1E1BA0(void) {
    return D_800BAD0E_1E7DBE;
}

INCLUDE_ASM("asm/nonmatchings/1E1BA0", func_800B4AFC_1E1BAC);

INCLUDE_ASM("asm/nonmatchings/1E1BA0", func_800B4B30_1E1BE0);

INCLUDE_ASM("asm/nonmatchings/1E1BA0", func_800B4BDC_1E1C8C);

INCLUDE_ASM("asm/nonmatchings/1E1BA0", func_800B4C80_1E1D30);

void func_800B4CB0_1E1D60(void) {
    func_80057ABC_586BC(0, 0);
}

INCLUDE_ASM("asm/nonmatchings/1E1BA0", func_800B4CD0_1E1D80);

INCLUDE_ASM("asm/nonmatchings/1E1BA0", func_800B4D74_1E1E24);

INCLUDE_ASM("asm/nonmatchings/1E1BA0", func_800B4E7C_1E1F2C);

INCLUDE_ASM("asm/nonmatchings/1E1BA0", func_800B4F28_1E1FD8);

INCLUDE_ASM("asm/nonmatchings/1E1BA0", func_800B4F60_1E2010);
