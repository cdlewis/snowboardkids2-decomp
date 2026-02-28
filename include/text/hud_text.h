#pragma once

#include "common.h"

typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ u16 *unk4;
    /* 0x08 */ void *unk8;
    /* 0x0C */ u16 unkC;
    /* 0x0E */ u16 unkE;
    /* 0x10 */ u8 unk10;
} Func80035408Arg;

void func_80035408_36008(Func80035408Arg *arg0);
void func_80035DE0_369E0(void *, void *, s16, s16, u8, u8, void *, s32);
