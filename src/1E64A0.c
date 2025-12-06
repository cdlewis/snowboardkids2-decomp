#include "common.h"
#include "cutscene/cutscene_sys2.h"
#include "task_scheduler.h"

extern void func_800B9440_1E64F0(void *);
extern void func_800585C8_591C8(s32);
extern void *func_8000B714_C314(void *, s32, s32);
extern void *func_80009EF8_AAF8(s16);
extern void *func_80035F80_36B80(s32);
extern void *func_8000B6B8_C2B8(s32);
extern void func_800B99A0_1E6A50(void);
extern u16 D_800BADF0_1E7EA0[];
extern u16 D_800BADF2_1E7EA2[];

void func_800B9C20_1E6CD0(cutsceneSys2Wait_exec_asset *arg0);

typedef struct {
    u8 _pad[0x74];
    s32 unk74;
    u8 _pad2[0x4];
    s32 unk7C;
    s32 unk80;
} func_800B9680_1E6730_arg;

s32 func_800B93F0_1E64A0(s32 arg0) {
    s32 result;

    if (arg0 == 0) {
        arg0 = 1;
    }

    result = 0x4000000 / arg0;

    if (result >= 0x4000) {
        result = 0x4000;
    }

    return result;
}

INCLUDE_ASM("asm/nonmatchings/1E64A0", func_800B9440_1E64F0);

INCLUDE_ASM("asm/nonmatchings/1E64A0", func_800B956C_1E661C);

s32 func_800B9680_1E6730(func_800B9680_1E6730_arg *arg0) {
    arg0->unk80 = 0x6600;
    arg0->unk7C = 0x8000;
    func_800B9440_1E64F0(arg0);
    func_800585C8_591C8(0x2C);
    return 2;
}

s32 func_800B96B4_1E6764(func_800B9680_1E6730_arg *arg0) {
    s32 result = 2;

    arg0->unk74 += arg0->unk80;

    if (arg0->unk74 > 0x18000) {
        arg0->unk74 = 0x18000;
        result = 3;
    }

    func_800B9440_1E64F0(arg0);

    return result;
}

s32 func_800B9708_1E67B8(func_800B9680_1E6730_arg *arg0) {
    s32 temp_a1;
    s32 temp_v0;
    s32 result;

    temp_a1 = arg0->unk74;
    temp_v0 = ((0x10000 - temp_a1) >> 8) * (arg0->unk7C >> 8);
    result = 3;

    if (temp_v0 != 0) {
        arg0->unk74 = temp_a1 + temp_v0;
    } else {
        arg0->unk74 = 0x10000;
        result = 4;
    }

    func_800B9440_1E64F0(arg0);

    return result;
}

void *func_800B9774_1E6824(cutsceneSys2Wait_exec_asset *arg0) {
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

void func_800B993C_1E69EC(cutsceneSys2Wait_exec_asset *arg0) {
    arg0->unk84 = 0;
    arg0->unk88 = 0;
    arg0->unk8 = func_80009EF8_AAF8(0);
    arg0->unkA0 = func_80035F80_36B80(1);
    arg0->unkC = func_8000B6B8_C2B8(0);
    setCleanupCallback(&func_800B9C20_1E6CD0);
    setCallback(&func_800B99A0_1E6A50);
}

INCLUDE_ASM("asm/nonmatchings/1E64A0", func_800B99A0_1E6A50);

void func_800B9C20_1E6CD0(cutsceneSys2Wait_exec_asset *arg0) {
    arg0->unkA0 = freeNodeMemory(arg0->unkA0);
    arg0->unk8 = freeNodeMemory(arg0->unk8);
    arg0->unkC = freeNodeMemory(arg0->unkC);
}
