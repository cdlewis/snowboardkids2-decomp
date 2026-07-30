#pragma once

#include "common.h"

typedef union {
    u16 value;
    struct {
        u8 high;
        u8 low;
    } bytes;
} TextLayoutColorValue;

typedef struct {
    /* 0x00 */ s16 startX;
    /* 0x02 */ s16 startY;
    /* 0x04 */ u16 *textData;
    /* 0x08 */ void *fontAsset;
    /* 0x0C */ TextLayoutColorValue shade;
    /* 0x0E */ TextLayoutColorValue textAlpha;
    /* 0x10 */ u8 paletteIndex;
} TextLayoutArg;

typedef struct {
    /* 0x00 */ s16 startX;
    /* 0x02 */ s16 startY;
    /* 0x04 */ u16 *textData;
    /* 0x08 */ void *fontAsset;
    /* 0x0C */ TextLayoutColorValue shade;
    /* 0x0E */ TextLayoutColorValue textAlpha;
    /* 0x10 */ u8 paletteIndex;
    /* 0x11 */ u8 maxIterations;
} TextLayoutCappedArg;

void renderTextLayout(TextLayoutArg *arg0);
void renderTextLayoutCapped(TextLayoutCappedArg *arg0);
void enqueueTextLayoutAlphaBlended(
    void *fontAsset,
    void *textData,
    s16 startX,
    s16 startY,
    u8 alpha,
    u8 paletteIndex,
    u8 priority,
    u8 flags
);
void enqueueTextLayout(
    void *fontAsset,
    void *textData,
    s16 startX,
    s16 startY,
    u8 alpha,
    u8 transparency,
    u8 paletteIndex,
    u8 priority,
    u8 flags
);
void enqueueTextLayoutCapped(
    void *fontAsset,
    u16 *textData,
    u16 startX,
    s16 startY,
    u8 alpha,
    u8 transparency,
    u8 paletteIndex,
    u8 maxIterations,
    u8 priority,
    u8 flags
);
