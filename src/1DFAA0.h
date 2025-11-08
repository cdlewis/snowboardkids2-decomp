#pragma once

#include "common.h"

typedef struct {
    s32 unk0;
    s32 unk4;
    u8 padding[0x38];
    s8 unk40;
    u8 padding2[0x3];
    s8 unk44;
    u8 padding3[0xA7];
    s8 unkEC;
    u8 padding4[0x3];
    s32 unkF4;
} func_800B2A24_1DFAD4_arg_item;

typedef struct {
    s16 unk0;
    u8 header[0xA6];
    func_800B2A24_1DFAD4_arg_item items[18];
    u8 padding[0x50];
    s32 unk1220;
} func_800B2A24_1DFAD4_arg;

typedef struct {
    u8 padding[0xFF7];
    s8 unkFF7;
} func_800B29F0_1DFAA0_arg;

void resetScriptState(u8 *arg0);

void func_800B29F0_1DFAA0(func_800B29F0_1DFAA0_arg *arg0);

void func_800B2A50_1DFB00(func_800B2A24_1DFAD4_arg *arg0, s16 arg1);

func_800B2A24_1DFAD4_arg_item *func_800B2A78_1DFB28(func_800B2A24_1DFAD4_arg *arg0, s16 arg1);