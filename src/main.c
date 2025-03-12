#include "common.h"

void* func_80009F5C_AB5C(s32);

extern s32 func_8006A200_6AE00(s32);

typedef struct {
    char padding[44];
    s32 unk2C;
    s32 unk30;
} func_80000BF4_17F4_arg;

INCLUDE_ASM("asm/nonmatchings/main", func_80000450_1050);

s32 osVoiceCheckWord(u8* data) {
    return 0xE;
}

INCLUDE_ASM("asm/nonmatchings/main", func_80000460_1060);

INCLUDE_ASM("asm/nonmatchings/main", func_8000056C_116C);

INCLUDE_ASM("asm/nonmatchings/main", func_80000710_1310);

INCLUDE_ASM("asm/nonmatchings/main", func_80000760_1360);

INCLUDE_ASM("asm/nonmatchings/main", func_800007C4_13C4);

INCLUDE_ASM("asm/nonmatchings/main", func_800007F0_13F0);

INCLUDE_ASM("asm/nonmatchings/main", func_800008D0_14D0);

INCLUDE_ASM("asm/nonmatchings/main", func_80000968_1568);

INCLUDE_ASM("asm/nonmatchings/main", func_800009A0_15A0);

INCLUDE_ASM("asm/nonmatchings/main", func_80000A68_1668);

INCLUDE_ASM("asm/nonmatchings/main", __rmonPanic);

INCLUDE_ASM("asm/nonmatchings/main", rmonmisc_text_010C);

void func_80000BF4_17F4(func_80000BF4_17F4_arg* arg0) {
    arg0->unk30 = func_8006A200_6AE00(arg0->unk30);
    arg0->unk2C = func_8006A200_6AE00(arg0->unk2C);
}

INCLUDE_ASM("asm/nonmatchings/main", func_80000C2C_182C);

INCLUDE_ASM("asm/nonmatchings/main", func_80000CAC_18AC);

INCLUDE_ASM("asm/nonmatchings/main", func_80000DA4_19A4);

INCLUDE_ASM("asm/nonmatchings/main", func_80000DC0_19C0);

INCLUDE_ASM("asm/nonmatchings/main", func_80000E84_1A84);

void func_80000F14_1B14(func_80000BF4_17F4_arg* arg0) {
    arg0->unk30 = func_8006A200_6AE00(arg0->unk30);
    arg0->unk2C = func_8006A200_6AE00(arg0->unk2C);
}

INCLUDE_ASM("asm/nonmatchings/main", func_80000F4C_1B4C);

INCLUDE_ASM("asm/nonmatchings/main", func_80001040_1C40);

INCLUDE_ASM("asm/nonmatchings/main", func_80001114_1D14);

INCLUDE_ASM("asm/nonmatchings/main", func_80001158_1D58);

INCLUDE_ASM("asm/nonmatchings/main", func_800011DC_1DDC);

INCLUDE_ASM("asm/nonmatchings/main", func_80001264_1E64);

INCLUDE_ASM("asm/nonmatchings/main", func_80001280_1E80);

INCLUDE_ASM("asm/nonmatchings/main", func_800013B8_1FB8);

void func_800014C8_20C8(s32 arg0) {
    func_80009F5C_AB5C(arg0 + 8);
}
