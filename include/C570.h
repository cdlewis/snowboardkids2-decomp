#pragma once

#include "1DFAA0.h"
#include "common.h"

typedef struct {
    DisplayListObject unk0;
    DisplayListObject unk3C;
    void **unk78;
    s32 unk7C;
    u16 unk80;
    s16 unk82;
    s16 unk84;
} func_8000BA08_C608_arg;

typedef struct {
    DisplayListObject unk0;
    u8 padding[0x7C - 0x3C];
    s32 unk7C;
} func_8000BB48_C748_arg;

void func_8000BB48_C748(func_8000BB48_C748_arg *arg0);
void func_8000BBE0_C7E0(SceneModel *, s16);
void func_8000BC10_C810(SceneModel *arg0, s16 arg1, u8 arg2, u8 arg3, u8 arg4);
void func_8000BA08_C608(func_8000BA08_C608_arg *arg0);
