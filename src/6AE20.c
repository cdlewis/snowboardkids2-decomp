#include "common.h"

extern s32 D_800AB130_A24A0;
extern s8 D_8018F800;
extern s32 D_800AB064_A23D4;

void func_8006A220_6AE20(void) {
    s32 var_v1;
    s8 *var_a0;

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

s32 func_8006A3FC_6AFFC(s32 *arg0) {
    if (arg0 == NULL) {
        return 0;
    }
    arg0[-5]--;
    arg0[-2] = D_800AB064_A23D4;
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/6AE20", func_8006A428_6B028);

void func_8006A4DC_6B0DC(s32 *arg0) {
    arg0[-0x3] = 1;
}

void func_8006A4E8_6B0E8(s32 *arg0) {
    u32 mask = osSetIntMask(1);
    arg0[-3] = 0;
    osSetIntMask(mask);
}

s32 func_8006A51C_6B11C(void *arg0, void *arg1) {
    return (s32)(*((void **)((u8 *)arg0 - 0xC)));
}

void func_8006A524_6B124(void *arg0, void *arg1) {
    *((void **)((u8 *)arg0 - 4)) = arg1;
}

s32 func_8006A52C_6B12C(void *arg0) {
    return (s32) * ((void **)((u8 *)arg0 - 4));
}
