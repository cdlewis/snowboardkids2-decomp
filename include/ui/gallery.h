#pragma once

#include "common.h"
#include "graphics/graphics.h"
#include "graphics/sprite_rdp.h"
#include "graphics/tiled_sprite_grid.h"
#include "text/font_assets.h"
#include "ui/level_preview_3d.h"

typedef union {
    s32 value;
    struct {
        u8 padding;
        u8 alpha;
        u8 padding2[2];
    } components;
} GalleryMenuTextColor;

typedef struct {
    /* 0x00 */ s8 state;
    /* 0x01 */ s8 navigationState;
    /* 0x02 */ s8 cursorIndex;
    /* 0x03 */ u8 pageUpCursorDestination;
    /* 0x04 */ u8 pageDownCursorDestination;
    /* 0x05 */ u8 padding5[3];
    /* 0x08 */ s32 cursorAlpha;
    /* 0x0C */ s32 cursorAlphaVelocity;
    /* 0x10 */ s32 pageIndicatorAlpha;
    /* 0x14 */ s32 pageIndicatorAlphaVelocity;
} GalleryViewerState;

typedef struct {
    /* 0x000 */ s8 menuState;
    /* 0x001 */ s8 selectedOption;
    /* 0x002 */ s8 menuType;
    /* 0x003 */ s8 viewerComplete;
    /* 0x004 */ GalleryMenuTextColor menuTextColor;
    /* 0x008 */ void *textRenderer;
    /* 0x00C */ void *textTable;
    /* 0x010 */ s16 bgmFadeTimer;
    /* 0x012 */ s16 fadeTimer;
    /* 0x014 */ s16 animTimer;
    /* 0x016 */ u8 isExiting;
    /* 0x017 */ u8 padding17;
    /* 0x018 */ SpriteSheetData *spriteAsset;
    /* 0x01C */ u8 padding1C[4];
    /* 0x020 */ ViewportNode overlayViewport;
    /* 0x1F8 */ ViewportNode fadeViewport;
    /* 0x3D0 */ ViewportNode menuViewport;
    /* 0x5A8 */ ColorData lightColors[3];
    /* 0x5C0 */ u8 ambientColor[4];
    /* 0x5C4 */ TileMapTextureAsset *backgroundTileMapAsset;
    /* 0x5C8 */ TileMapScrollRenderState backgroundTileMap;
    /* 0x5F4 */ TileMapTextureAsset *overlayTileMapAsset;
    /* 0x5F8 */ TileMapScrollRenderState overlayTileMap;
    /* 0x624 */ SceneModel *menuModel;
    /* 0x628 */ FrameSpriteEntry menuOptionSprites[6];
    /* 0x6B8 */ FrameSpriteEntry menuOptionLabels[6];
    /* 0x748 */ s32 menuOptionAlpha[6];
    /* 0x760 */ u8 menuOptionAnimFrames[6];
    /* 0x766 */ s8 menuOptionAnimTimers[6];
    /* 0x76C */ TextRenderArg itemSprites[27];
    /* 0x91C */ TextRenderArg boardOverlaySprites[27];
    /* 0xACC */ TextRenderArg extraItemSprites[5];
    /* 0xB1C */ TextRenderArg selectionCursor;
    /* 0xB2C */ TextRenderArg pageIndicator;
    /* 0xB3C */ SpriteRenderArg prizeIcon;
    /* 0xB48 */ SpriteRenderArg flashingPrizeIcon;
    /* 0xB54 */ ColoredTextRenderArg itemLabels[27];
    /* 0xC98 */ TextData itemStatsText;
    /* 0xCA4 */ u8 itemStatsBuffer[0x1C];
} GalleryMenuState;

void initGalleryMenu(void);
void initGalleryViewer(GalleryViewerState *state);
void onGalleryMenuExit(void);
void onGalleryViewerCleanup(void);
void updateGalleryViewer(GalleryViewerState *state);
