#pragma once

#include "common.h"
#include "geometry.h"

typedef struct {
    /* 0x00 */ Mat3x3Padded matrix;
    /* 0x20 */ u8 _pad20[0x1C];
    /* 0x3C */ s32 targetPosition[3];
    /* 0x48 */ s32 velocityY;
    /* 0x4C */ s32 unk4C;
    /* 0x50 */ s16 unk50;
    /* 0x52 */ s16 unk52;
    /* 0x54 */ u16 rotationAngle;
    /* 0x56 */ u16 unk56;
} func_800BB2B0_arg;

void func_800BB2B0(func_800BB2B0_arg *arg0);
void func_800BBA50(s32 arg0);
void func_800BBA98(void);
