#pragma once

#include "common.h"

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    void *unk8;
    s16 unkC;
    s16 unkE;
    u8 unk10;
    u8 pad11[3];
    s16 unk14;
    s16 unk16;
    void *unk18;
    s16 unk1C;
    s16 unk1E;
    u8 unk20;
    u8 unk21;
    u8 pad22[2];
    s16 unk24;
    s16 unk26;
    void *unk28;
    s16 unk2C;
    s16 unk2E;
    u8 unk30;
    u8 unk31;
} func_80037710_38310_arg;

void func_80037710_38310(func_80037710_38310_arg *arg0);
