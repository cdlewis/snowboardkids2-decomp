#include "common.h"

extern s16 D_800BAD0E_1E7DBE;
extern s16 D_800BAA60_1E7B10[];
extern void func_80057ABC_586BC(s32, s32);

s16 func_800B4AF0_1E1BA0(void) {
    return D_800BAD0E_1E7DBE;
}

s16 func_800B4AFC_1E1BAC(s16 arg0) {
    s16 temp_v1;
    s16 temp_v0;
    s16 index;

    temp_v1 = D_800BAD0E_1E7DBE;
    temp_v0 = arg0;

    if (temp_v0 < temp_v1) {
        index = arg0;
    } else {
        index = 0;
    }

    return D_800BAA60_1E7B10[index];
}

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
