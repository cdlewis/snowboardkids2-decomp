#pragma once

#include "common.h"

typedef struct {
    u8 _pad[0x20];
    void *unk20;
    void *unk24;
    void *unk28;
    s32 unk2C;
    u8 _pad2[0x4C - 0x30];
    s16 unk4C;
} func_800BB814_B5114_arg;

void func_800BB814_B5114(func_800BB814_B5114_arg *arg0);
