#pragma once

#include "common.h"
#include "1DFAA0.h"

typedef struct {
    u8 pad[0x218];
    s8 unk218;
} Func8000C2B4Arg;

typedef struct {
    u8 padding[0x214];
    u8 unk214;
    u8 unk215;
    u8 unk216;
} Func8000C2BCArg;

typedef struct {
    u8 pad[0x204];
    s32 unk204;
    s32 unk208;
    s16 unk20C;
} Func8000C2CCArg;

typedef struct {
    u8 pad[0x1E4];
    s32 unk1E4;
    s32 unk1E8;
    s32 unk1EC;
    s32 unk1F0;
    s32 unk1F4;
    s32 unk1F8;
    s16 unk1FC;
    s16 unk1FE;
    s16 unk200;
} Func8000C334Arg;

void func_8000C268_CE68(Func8000C268Arg *arg0);
void func_8000C278_CE78(Func8000C268Arg *arg0);
void func_8000C2B4_CEB4(Func8000C2B4Arg *arg0, s32 arg1);
void func_8000C2BC_CEBC(Func8000C2BCArg *arg0, u8 arg1, u8 arg2, u8 arg3);
void func_8000C2CC_CECC(Func8000C2CCArg *arg0, s32 arg1, s16 arg2);
void func_8000C334_CF34(Func8000C334Arg *arg0, s32 *arg1, s16 arg2);
void func_8000C440_D040(void *arg0, s32 arg1, s32 arg2, u16 arg3, s32 arg4, s32 arg5, s32 arg6);
