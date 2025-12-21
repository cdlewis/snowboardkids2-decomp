#pragma once

#include "common.h"

typedef struct {
    s32 unk0;        // 0x00
    s32 unk4;        // 0x04
    s32 unk8;        // 0x08
    s32 unkC;        // 0x0C
    s32 unk10;       // 0x10
    s32 unk14;       // 0x14
    u8 unk18[0x14];  // 0x18 - embedded GameDataLayout
    void *unk2C;     // 0x2C
    u8 unk30[0x20];  // 0x30 - matrix
    u16 unk50;       // 0x50
    u16 unk52;       // 0x52
    s16 unk54;       // 0x54
    u16 unk56;       // 0x56
    u8 _pad58[0x2];  // 0x58
    u16 unk5A;       // 0x5A
    s32 unk5C;       // 0x5C
    s32 unk60;       // 0x60
    s32 unk64;       // 0x64
    s32 unk68;       // 0x68
    u8 _pad6C[0x6];  // 0x6C
    s16 unk72;       // 0x72
    s16 unk74;       // 0x74
    u8 unk76;        // 0x76
} Func8001F6A0Arg;

void func_8001F6A0_202A0(Func8001F6A0Arg *arg0);
