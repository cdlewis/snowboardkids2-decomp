#include "common.h"

extern void func_800B9440_1E64F0(void *);
extern void func_800585C8_591C8(s32);

typedef struct {
    u8 _pad[0x7C];
    s32 unk7C;
    s32 unk80;
} func_800B9680_1E6730_arg;

INCLUDE_ASM("asm/nonmatchings/1E64A0", func_800B93F0_1E64A0);

INCLUDE_ASM("asm/nonmatchings/1E64A0", func_800B9440_1E64F0);

INCLUDE_ASM("asm/nonmatchings/1E64A0", func_800B956C_1E661C);

s32 func_800B9680_1E6730(func_800B9680_1E6730_arg *arg0) {
    arg0->unk80 = 0x6600;
    arg0->unk7C = 0x8000;
    func_800B9440_1E64F0(arg0);
    func_800585C8_591C8(0x2C);
    return 2;
}

INCLUDE_ASM("asm/nonmatchings/1E64A0", func_800B96B4_1E6764);

INCLUDE_ASM("asm/nonmatchings/1E64A0", func_800B9708_1E67B8);

INCLUDE_ASM("asm/nonmatchings/1E64A0", func_800B9774_1E6824);

INCLUDE_ASM("asm/nonmatchings/1E64A0", func_800B97C8_1E6878);

INCLUDE_ASM("asm/nonmatchings/1E64A0", func_800B993C_1E69EC);

INCLUDE_ASM("asm/nonmatchings/1E64A0", func_800B99A0_1E6A50);

INCLUDE_ASM("asm/nonmatchings/1E64A0", func_800B9C20_1E6CD0);
