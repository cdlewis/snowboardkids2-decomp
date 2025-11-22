#include "common.h"
#include "task_scheduler.h"

extern void func_800B9440_1E64F0(void *);
extern void func_800585C8_591C8(s32);
extern void *func_8000B714_C314(void *, s16, s16);

typedef struct {
    u8 _pad[0x7C];
    s32 unk7C;
    s32 unk80;
} func_800B9680_1E6730_arg;

typedef struct {
    u8 _pad0[0x8];
    void *unk8;
    void *unkC;
    u8 _pad1[0x90];
    void *unkA0;
} func_800B9C20_1E6CD0_arg;

typedef struct {
    u8 _pad0[0xC];
    void *unkC;
    u8 _pad1[0x74];
    s16 unk84;
    s16 unk86;
    s16 unk88;
    u8 _pad2[0x6];
    void *unk90;
} func_800B9774_1E6824_arg;

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

void *func_800B9774_1E6824(func_800B9774_1E6824_arg *arg0) {
    void *result;

    result = func_8000B714_C314(arg0->unkC, arg0->unk86, arg0->unk88);

    if (result == NULL) {
        arg0->unk84 = 6;
    }

    arg0->unk90 = result;
    arg0->unk84 = 5;

    return result;
}

INCLUDE_ASM("asm/nonmatchings/1E64A0", func_800B97C8_1E6878);

INCLUDE_ASM("asm/nonmatchings/1E64A0", func_800B993C_1E69EC);

INCLUDE_ASM("asm/nonmatchings/1E64A0", func_800B99A0_1E6A50);

void func_800B9C20_1E6CD0(func_800B9C20_1E6CD0_arg *arg0) {
    arg0->unkA0 = freeNodeMemory(arg0->unkA0);
    arg0->unk8 = freeNodeMemory(arg0->unk8);
    arg0->unkC = freeNodeMemory(arg0->unkC);
}
