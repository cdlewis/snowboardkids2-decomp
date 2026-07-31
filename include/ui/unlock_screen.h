#pragma once

#include "common.h"
#include "graphics/sprite_rdp.h"
#include "text/font_assets.h"
#include "text/text_layout.h"

typedef struct {
    void *progressBarAsset;
    SpriteRenderArg priceLabelSprite;
    TextData statLabels[3];
    s16 progressBarX;
    s16 progressBarY;
    char statBuffers[3][3];
} UnlockScreenItemStatsDisplay;

typedef struct {
    SpriteRenderArg items[4];
    TextLayoutArg title;
} UnlockScreenItemIconsState;

typedef struct {
    TextRenderArg arrows[2];
    s8 animationCounter;
} UnlockScreenScrollArrowsState;

void initUnlockScreen(void);
