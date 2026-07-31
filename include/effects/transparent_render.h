#pragma once

#include "common.h"
#include "cutscene/cutscene_manager.h"
#include "graphics/tiled_sprite_grid.h"

typedef struct {
    /* 0x00 */ CutsceneManager *cutsceneManager;
    /* 0x04 */ TileMapTextureAsset **loadedAssets;
    /* 0x08 */ s32 parallaxScale;
    /* 0x0C */ s32 horizontalScroll;
    /* 0x10 */ s32 screenY;
    /* 0x14 */ TileMapScrollRenderState tileMaps[4];
    /* 0xC4 */ u8 assetGroupIndex;
    /* 0xC5 */ s8 initialized;
} ScrollingTileGroupTaskData;

void scheduleDualAssetGroupLoad(void *context, u8 groupIndex1, s32 param1, u8 groupIndex2, s32 param2);
