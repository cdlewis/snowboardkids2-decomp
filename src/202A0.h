#pragma once

#include "common.h"

typedef struct {
    s32 unk0;        // 0x00
    s32 unk4;        // 0x04
    s32 unk8;        // 0x08
    s32 unkC;        // 0x0C
    s32 unk10;       // 0x10
    s32 unk14;       // 0x14
    void *unk18;     // 0x18
    u8 _pad1C[0x10]; // 0x1C
    void *unk2C;     // 0x2C
    u8 _pad30[0x20]; // 0x30
    s16 unk50;       // 0x50
    s16 unk52;       // 0x52
    s16 unk54;       // 0x54
    s16 unk56;       // 0x56
    u8 _pad58[0x4];  // 0x58
    s32 unk5C;       // 0x5C
    s32 unk60;       // 0x60
    s32 unk64;       // 0x64
    s32 unk68;       // 0x68
    u8 _pad6C[0x6];  // 0x6C
    s16 unk72;       // 0x72
    s16 unk74;       // 0x74
} Func8001F6A0Arg;

void func_8001F6A0_202A0(Func8001F6A0Arg *arg0);
