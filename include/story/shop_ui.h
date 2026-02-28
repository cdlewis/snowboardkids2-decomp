#pragma once

#include "common.h"

typedef struct {
    s16 x;
    s16 y;
    s16 palette;
    s16 unk06;
    void *text;
} StatTextLabel;

typedef struct {
    void *progressBarAsset;
    s16 priceLabelX;
    s16 priceLabelY;
    void *spriteAsset;
    s16 priceSpriteIndex;
    s16 unk0E;
    StatTextLabel statLabels[3];
    s16 progressBarX;
    s16 progressBarY;
    char statBuffers[3][3];
} ItemStatsDisplay;

typedef struct {
    s16 x;
    s16 y;
    void *asset;
    s16 spriteIndex;
    s8 paletteIndex;
    s8 unkB;
} SpriteDisplayState;

typedef struct {
    SpriteDisplayState items[4];
    s16 titleX;
    s16 titleY;
    void *titleData;
    void *titleAsset;
    s16 titleAlpha1;
    s16 titleAlpha2;
    s8 titlePalette;
} UnlockScreenItemIconsState;

typedef struct {
    s16 x;
    s16 y;
    void *asset;
    s16 spriteIndex;
    s16 alpha;
    s8 unkC;
    u8 unkD;
    s8 unkE;
    s8 unkF;
} ScrollArrowSprite;

typedef struct {
    ScrollArrowSprite arrows[2];
    s8 animationCounter;
} UnlockScreenScrollArrowsState;

void initUnlockScreenScrollArrows(UnlockScreenScrollArrowsState *arg0);
void initStoryMapShopItemPriceDisplay(SpriteDisplayState *arg0);
void initStoryMapShopItemStatsDisplay(ItemStatsDisplay *arg0);
void cleanupStoryMapShopItemStatsDisplay(ItemStatsDisplay *arg0);
void initStoryMapShopSoldOutLabel(SpriteDisplayState *arg0);
void initUnlockScreenItemIcons(UnlockScreenItemIconsState *arg0);
