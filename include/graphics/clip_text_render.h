#pragma once

#include "common.h"
#include "graphics/sprite_rdp.h"

void func_8006D4B8_6E0B8(
    SpriteSheetData *spriteData,
    s16 savedX,
    s16 savedY,
    s16 cols,
    s16 rows,
    u8 arg5,
    u8 intensity,
    u8 colorR,
    u8 colorG,
    u8 colorB,
    u8 colorA,
    u8 priority,
    u8 layer
);

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