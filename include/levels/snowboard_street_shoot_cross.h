#pragma once

#include "common.h"

typedef struct {
    u8 _pad[0x14];
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    void *unk20;
    void *unk24;
    void *unk28;
    s32 unk2C;
} AD510Arg;

void func_800BBA90_AD510(AD510Arg *arg0);
