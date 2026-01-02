#pragma once

#include "common.h"

typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ void *unk4;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 unkD;
    /* 0x0E */ char pad[2];
} func_800B07A0_1DBE80_arg; // size 0x10

void func_800B0690_1DBD70(func_800B07A0_1DBE80_arg *arg0);
void func_800B070C_1DBDEC(func_800B07A0_1DBE80_arg *arg0);
void func_800B07A0_1DBE80(func_800B07A0_1DBE80_arg *arg0);