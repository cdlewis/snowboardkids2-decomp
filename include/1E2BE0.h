#pragma once

#include "common.h"

typedef struct {
    u8 _pad[0x20];
    s16 unk20;
    s16 unk22;
    s16 unk24;
    s16 unk26;
    s16 unk28;
    s16 unk2A;
    s16 unk2C;
    s16 unk2E;
    s16 unk30;
    s16 unk32;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    s32 unk54;
    s16 unk58;
    s16 unk5A;
    s16 unk5C;
    s16 unk5E;
    s16 unk60;
    s16 unk62;
    s32 unk64;
    s32 unk68;
    s16 unk6C;
    s16 unk6E;
    s8 unk70;
    s8 unk71;
} func_800B5E64_1E2F14_arg0;

typedef func_800B5E64_1E2F14_arg0 AnimationLoopArg;

typedef struct {
    u8 _pad[0x68];
    s32 unk68;
    s16 unk6C;
    s16 unk6E;
    u8 unk70;
    u8 unk71;
} func_800B6180_1E3230_arg0;

void func_800B5B30_1E2BE0(func_800B5E64_1E2F14_arg0 *arg0, s8 arg1);
void func_800B5BFC_1E2CAC(void *arg0);
void func_800B5EC4_1E2F74(func_800B5E64_1E2F14_arg0 *arg0, s16 arg1, s16 arg2, s32 arg3, s32 arg4, s32 arg5);
void func_800B5F2C_1E2FDC(func_800B5E64_1E2F14_arg0 *, s16, s16);
void func_800B5FB0_1E3060(func_800B5E64_1E2F14_arg0 *, s16, s16);
void func_800B6034_1E30E4(func_800B5E64_1E2F14_arg0 *, s32, s16);
void func_800B6088_1E3138(func_800B5E64_1E2F14_arg0 *, s32, s16);
void func_800B60DC_1E318C(func_800B5E64_1E2F14_arg0 *, s32, s16);
void func_800B6130_1E31E0(func_800B5E64_1E2F14_arg0 *arg0, s16 arg1, s16 arg2);
void func_800B6180_1E3230(func_800B6180_1E3230_arg0 *, s32, s16);
void finalizeAnimationLoop(AnimationLoopArg *arg0);
