#pragma once

#include "common.h"
#include "graphics/displaylist.h"
#include "graphics/sprite_rdp.h"
#include "graphics/tiled_sprite_grid.h"
#include "math/geometry.h"
#include "ui/unlock_screen.h"

typedef TileMapRenderTaskState StoryMapShopBackgroundState;
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

extern const char gIntegerFormatString[];

extern const char gGoldFormatString7d[];

void initUnlockScreenScrollArrows(UnlockScreenScrollArrowsState *arg0);
void initStoryMapShopItemPriceDisplay(SpriteRenderArg *arg0);
void initStoryMapShopItemStatsDisplay(UnlockScreenItemStatsDisplay *arg0);
void cleanupStoryMapShopItemStatsDisplay(UnlockScreenItemStatsDisplay *arg0);
void initStoryMapShopSoldOutLabel(SpriteRenderArg *arg0);
void initUnlockScreenItemIcons(UnlockScreenItemIconsState *arg0);

s32 getLockedShopItemIndices(u8 *);
void initSlideInStoryMapShopItemCard(StoryMapShopItemCardState *card);
void initStoryMapShopExitOverlay(SpriteRenderArg *state);
void initStoryMapShopFairyModel(StoryMapShopFairyState *state);
void initStoryMapShopGoldDisplay(StoryMapShopGoldDisplayState *state);
void initStoryMapShopItemCard(StoryMapShopItemCardState *card);
void initStoryMapShopItemIcon(TextRenderArg *state);
void initStoryMapShopItemStatLabel(TextRenderArg *state);
void loadStoryMapShopBackground(StoryMapShopBackgroundState *state);
