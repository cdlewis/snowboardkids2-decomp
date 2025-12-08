#pragma once

#include "common.h"
#include "geometry.h"

typedef struct {
    /* 0x00 */ Mat3x3Padded matrix;
    /* 0x20 */ u8 _pad20[0x1C];
    /* 0x3C */ s32 targetPosition[3];
    /* 0x48 */ s32 velocityY;
    /* 0x4C */ u8 _pad4C[0x8];
    /* 0x54 */ u16 rotationAngle;
} func_800BB2B0_arg;

void func_800BB2B0(func_800BB2B0_arg *arg0);
