#pragma once

#include "common.h"

typedef struct {
    u8 padding[0x214];
    u8 unk214;
    u8 unk215;
    u8 unk216;
} Func8000C2BCArg;

void func_8000C268_CE68(void *arg0);
void func_8000C278_CE78(void *arg0);
void func_8000C2BC_CEBC(Func8000C2BCArg *arg0, u8 arg1, u8 arg2, u8 arg3);
void func_8000C440_D040(void *arg0, s32 arg1, s32 arg2, u16 arg3, s32 arg4, s32 arg5, s32 arg6);
