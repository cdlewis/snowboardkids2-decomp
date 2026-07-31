#pragma once

#include "common.h"
#include "graphics/displaylist.h"
#include "graphics/sprite_rdp.h"
#include "math/geometry.h"

typedef struct {
    s16 x;
    s16 y;
    s16 palette;
    s16 unk06;
    void *text;
} StatTextLabel;

typedef struct {
    void *progressBarAsset;
    SpriteRenderArg priceLabelSprite;
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

typedef struct StoryMapShopBackgroundState StoryMapShopBackgroundState;
typedef struct StoryMapShopFairyState StoryMapShopFairyState;
typedef struct StoryMapShopGoldDisplayState StoryMapShopGoldDisplayState;
typedef struct StoryMapShopItemCardState {
    DisplayListObject displayList;
    Transform3D transform;
    s32 translationStep;
    u8 updateCounter;
    s8 itemIndex;
    s8 slotPosition;
} StoryMapShopItemCardState;
typedef struct StoryMapShopItemIconState StoryMapShopItemIconState;

extern const char gIntegerFormatString[];

extern const char gGoldFormatString7d[];

void initUnlockScreenScrollArrows(UnlockScreenScrollArrowsState *arg0);
void initStoryMapShopItemPriceDisplay(SpriteDisplayState *arg0);
void initStoryMapShopItemStatsDisplay(ItemStatsDisplay *arg0);
void cleanupStoryMapShopItemStatsDisplay(ItemStatsDisplay *arg0);
void initStoryMapShopSoldOutLabel(SpriteDisplayState *arg0);
void initUnlockScreenItemIcons(UnlockScreenItemIconsState *arg0);

s32 getLockedShopItemIndices(u8 *);
void initSlideInStoryMapShopItemCard(StoryMapShopItemCardState *card);
void initStoryMapShopExitOverlay(SpriteDisplayState *state);
void initStoryMapShopFairyModel(StoryMapShopFairyState *state);
void initStoryMapShopGoldDisplay(StoryMapShopGoldDisplayState *state);
void initStoryMapShopItemCard(StoryMapShopItemCardState *card);
void initStoryMapShopItemIcon(StoryMapShopItemIconState *state);
void initStoryMapShopItemStatLabel(ScrollArrowSprite *state);
void loadStoryMapShopBackground(StoryMapShopBackgroundState *state);
