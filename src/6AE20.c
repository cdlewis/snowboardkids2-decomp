#include "common.h"

extern s32 D_800AB130_A24A0;
extern s8 D_8018F800;

void func_8006A220_6AE20(void) {
    s32 var_v1;
    s8* var_a0;

    D_800AB130_A24A0 = 0;
    var_a0 = &D_8018F800;
    var_v1 = 0;
    do {
        *var_a0 = 0;
        var_v1 += 1;
        var_a0 += 1;
    } while (var_v1 <= 0x1FFFFF);
}

INCLUDE_ASM("asm/nonmatchings/6AE20", func_8006A258_6AE58);

INCLUDE_ASM("asm/nonmatchings/6AE20", func_8006A3FC_6AFFC);

INCLUDE_ASM("asm/nonmatchings/6AE20", func_8006A428_6B028);

INCLUDE_ASM("asm/nonmatchings/6AE20", func_8006A4DC_6B0DC);

void func_8006A4E8_6B0E8(s32* arg0) {
    u32 mask = osSetIntMask(1);
    arg0[-3] = 0;
    osSetIntMask(mask);
}

INCLUDE_ASM("asm/nonmatchings/6AE20", func_8006A51C_6B11C);

INCLUDE_ASM("asm/nonmatchings/6AE20", func_8006A524_6B124);

INCLUDE_ASM("asm/nonmatchings/6AE20", func_8006A52C_6B12C);
