#pragma once

#include "common.h"

typedef struct {
    s16 unk0;
    s16 unk2;
    u8 pad4[0x20];
    void *unk24;
    void *unk28;
} B2030TaskPayload;

void func_800BB688_B2408(B2030TaskPayload *arg0);
