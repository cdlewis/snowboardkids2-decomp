#pragma once

#include "common.h"

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
} func_80004FF8_5BF8_arg1_unk20;

typedef struct {
    u8 padding[0x20];
    func_80004FF8_5BF8_arg1_unk20 *unk20;
    void *unk24;
    void *unk28;
    s32 unk2C;
    s32 unk30;
    u8 padding2[0x8];
    void *unk3C;
    s32 unk40;
    s16 unk44;
    s16 unk46;
    s16 unk48;
    s16 unk4A;
    s16 unk4C;
    s16 unk4E;
    s16 unk50;
    s16 unk52;
    s16 unk54;
    s16 unk56;
    s16 unk58;
} func_80004FF8_5BF8_arg1;

void func_80004FF8_5BF8(u16 arg0, func_80004FF8_5BF8_arg1 *arg1);