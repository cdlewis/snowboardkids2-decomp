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

void func_800B6130_1E31E0(func_800B5E64_1E2F14_arg0 *arg0, s16 arg1, s16 arg2);
