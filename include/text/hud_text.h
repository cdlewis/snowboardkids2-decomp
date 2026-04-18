#pragma once

#include "common.h"

typedef struct {
    /* 0x00 */ s16 startX;
    /* 0x02 */ s16 startY;
    /* 0x04 */ u16 *textData;
    /* 0x08 */ void *fontAsset;
    /* 0x0C */ u16 shade;
    /* 0x0E */ u16 textAlpha;
    /* 0x10 */ u8 paletteIndex;
} HudTextLayoutArg;

typedef struct {
    /* 0x00 */ s16 startX;
    /* 0x02 */ s16 startY;
    /* 0x04 */ u16 *textData;
    /* 0x08 */ void *fontAsset;
    /* 0x0C */ u16 shade;
    /* 0x0E */ u16 textAlpha;
    /* 0x10 */ u8 paletteIndex;
    /* 0x11 */ u8 maxIterations;
} HudTextLayoutCappedArg;

void renderHudTextLayout(HudTextLayoutArg *arg0);
void func_80035548_36148(HudTextLayoutCappedArg *arg0);
void func_80035DE0_369E0(void *, void *, s16, s16, u8, u8, u8, u8);
void enqueueHudTextLayout(void *fontAsset, void *textData, s16 startX, s16 startY, u8 alpha, u8 transparency, u8 paletteIndex, u8 priority, u8 flags);
