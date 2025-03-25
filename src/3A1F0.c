#include "common.h"

extern OSMesgQueue D_800A1888_A2488;
extern OSMesgQueue mainStack;
extern u8 D_800AB090_A2400;

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_800395F0_3A1F0);

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_800397CC_3A3CC);

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_80039B88_3A788);

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_80039C34_3A834);

void func_8003A1DC_3ADDC() {
}

int func_8003A1E4_3ADE4() {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_8003A1EC_3ADEC);

void func_8003A284_3AE84() {
}

int func_8003A28C_3AE8C() {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_8003A294_3AE94);

void func_8003A51C_3B11C() {
}

int func_8003A524_3B124() {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_8003A52C_3B12C);

void func_8003A854_3B454() {
}

int func_8003A85C_3B45C() {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_8003A864_3B464);

void func_8003A9DC_3B5DC() {
}

int func_8003A9E4_3B5E4() {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_8003A9EC_3B5EC);

void func_8003AAC4_3B6C4() {
}

int func_8003AACC_3B6CC() {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_8003AAD4_3B6D4);

void func_8003AB74_3B774() {
}

int func_8003AB7C_3B77C() {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_8003AB84_3B784);

void func_8003AC30_3B830() {
}

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_8003AC38_3B838);

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_8003AC58_3B858);

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_8003AF38_3BB38);

void func_8003AF6C_3BB6C() {
    u8* var_v1;
    u8 a1 = 1;

    var_v1 = &D_800AB090_A2400;
    do {
        if (*var_v1 == 0) {
            *var_v1 = a1;
        }
        var_v1 += 1;
    } while ((s32)var_v1 < (s32)(&D_800AB090_A2400 + 4));
}

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_8003AFA0_3BBA0);

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_8003B000_3BC00);

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_8003B098_3BC98);

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_8003B0E8_3BCE8);

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_8003B170_3BD70);

void func_8003B1C0_3BDC0() {
    osSendMesg(
        &D_800A1888_A2488,
        (OSMesg*)osEepromProbe(&mainStack),
        1);
}

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_8003B1F4_3BDF4);

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_8003B28C_3BE8C);

void func_8003B2DC_3BEDC(s32 arg0, u8* arg1) {
    u8 a1 = ((arg0 & 0xFF) * 0x10) & 0xF0;
    osSendMesg(
        &D_800A1888_A2488,
        (OSMesg*)osEepromLongRead(&mainStack, a1, arg1, 0x58),
        1);
}

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_8003B324_3BF24);

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_8003B3B0_3BFB0);

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_8003B400_3C000);

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_8003B47C_3C07C);

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_8003B510_3C110);

void func_8003B560_3C160(u8* arg0) {
    osSendMesg(
        &D_800A1888_A2488,
        (OSMesg*)osEepromLongWrite(&mainStack, 0, arg0, 0x200),
        1);
}

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_8003B5A0_3C1A0);

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_8003B6B4_3C2B4);

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_8003B8F0_3C4F0);

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_8003BA24_3C624);

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_8003BB5C_3C75C);

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_8003BB68_3C768);

void func_8003BC58_3C858() {
}

INCLUDE_ASM("asm/nonmatchings/3A1F0", func_8003BC60_3C860);
