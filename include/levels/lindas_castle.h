#pragma once

#include "common.h"
#include "geometry.h"

typedef struct {
    /* 0x00 */ Transform3D matrix;
    /* 0x20 */ u8 _pad20[0x1C];
    /* 0x3C */ s32 targetPosition[3];
    /* 0x48 */ s32 velocityY;
    /* 0x4C */ s32 unk4C;
    /* 0x50 */ s16 unk50;
    /* 0x52 */ s16 unk52;
    /* 0x54 */ u16 rotationAngle;
    /* 0x56 */ u16 unk56;
} func_800BB2B0_arg;

typedef struct {
    u16 unk0;
    s16 unk2;
} func_800BBA28_AB8D8_arg;

void func_800BB2B0(func_800BB2B0_arg *arg0);
void func_800BBA50(s32 arg0);
void func_800BBA98(void);
void func_800BBA28_AB8D8(func_800BBA28_AB8D8_arg *arg0);
void func_800BBAF8_AB9A8(s16 *arg0);