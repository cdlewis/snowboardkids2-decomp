#pragma once

#include "common.h"
#include "graphics/sprite_rdp.h"

void renderTintedSpriteGrid(
    SpriteSheetData *spriteData,
    s16 savedX,
    s16 savedY,
    s16 cols,
    s16 rows,
    u8 showIndicator,
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