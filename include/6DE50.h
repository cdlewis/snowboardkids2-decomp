#pragma once

#include "common.h"

typedef struct {
    /* 0x00 */ u16 x;
    /* 0x02 */ u16 y;
    /* 0x04 */ u16 width;
    /* 0x06 */ u16 height;
    /* 0x08 */ u8 red;
    /* 0x09 */ u8 green;
    /* 0x0A */ u8 blue;
} ColorRect;

void func_8006D4B8_6E0B8(void *, s16, s16, s16, s32, u8, u8, u8, u8, u8, u8, u8, u8);

void renderTiledSprite3x3(
    void *asset,
    s16 x,
    s16 y,
    s16 width,
    s16 height,
    u8 flag,
    u8 red,
    u8 green,
    u8 blue,
    u8 alpha
);

void drawColorRect(ColorRect *rect);
void drawColorRectFill(ColorRect *rect);