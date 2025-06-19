#include "common.h"
#include "geometry.h"

typedef struct {
    u8 padding[0x3C];
    s32 unk3C;
    u8 padding2[0x38];
    s32 unk78;
    u8 padding3[0x1C];
    s32 unk98;
    u8 padding4[0x8];
    u16 unkA4;
} func_800BB3B8_B75F8_arg;

typedef struct {
    u8 padding[0x24];
    s32 unk24;
    s32 unk28;
} func_800BBA60_B7CA0_arg;

void func_80063788_64388(s32, void *);
extern void func_8006B084_6BC84(void *, void *, void *);
extern s32 D_8009A8A4_9B4A4;

INCLUDE_ASM("asm/nonmatchings/B74F0", func_800BB2B0_B74F0);

void func_800BB3B8_B75F8(func_800BB3B8_B75F8_arg *arg0) {
    s32 matrix[8];  // this should be matrix[9] but it causes stack issues
    s32 i;

    memcpy(&D_8009A8A4_9B4A4, &arg0->unk98, 0xC);
    func_8006B084_6BC84(&D_8009A8A4_9B4A4 - 5, &arg0->unk78, arg0);
    createXRotationMatrix(&matrix, arg0->unkA4);

    matrix[6] = 0x3b333;
    matrix[5] = 0;
    matrix[7] = 0x170000;

    func_8006B084_6BC84(&matrix[0], arg0, &arg0->unk3C);

    for (i = 0; i < 4; i++) {
        func_80063788_64388(i, arg0);
        func_80063788_64388(i, &arg0->unk3C);
    }
}

INCLUDE_ASM("asm/nonmatchings/B74F0", func_800BB468_B76A8);

INCLUDE_ASM("asm/nonmatchings/B74F0", func_800BB658_B7898);

INCLUDE_ASM("asm/nonmatchings/B74F0", func_800BB71C_B795C);

void func_800BB7D0_B7A10(func_800BBA60_B7CA0_arg *arg0) {
    arg0->unk24 = freeGameStateMemory(arg0->unk24);
    arg0->unk28 = freeGameStateMemory(arg0->unk28);
}

INCLUDE_ASM("asm/nonmatchings/B74F0", func_800BB808_B7A48);

INCLUDE_ASM("asm/nonmatchings/B74F0", func_800BB8B8_B7AF8);

void func_800BBA60_B7CA0(func_800BBA60_B7CA0_arg *arg0) {
    arg0->unk24 = freeGameStateMemory(arg0->unk24);
    arg0->unk28 = freeGameStateMemory(arg0->unk28);
}

INCLUDE_ASM("asm/nonmatchings/B74F0", func_800BBA98);
