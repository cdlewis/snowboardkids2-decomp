#pragma once

#include "common.h"

typedef struct {
    /* 0x00 */ u16 textureIndex;
    /* 0x02 */ u8 paletteIndex;
    /* 0x03 */ u8 flipMode;
} TileMapEntry;

typedef struct {
    /* 0x00 */ u8 data[32];
} PaletteChunk32;

typedef struct {
    /* 0x00 */ u16 tileGridWidth;
    /* 0x02 */ u16 tileGridHeight;
    /* 0x04 */ u8 tileWidth;
    /* 0x05 */ u8 tileHeight;
    /* 0x06 */ u16 ciMode;
    /* 0x08 */ u8 pad08[2];
    /* 0x0A */ u16 tileIndexDataOffset;
    /* 0x0C */ u16 paletteDataOffset;
    /* 0x0E */ u16 textureDataOffset;
    /* 0x10 */ u8 tileEntries[1];
} TileMapTextureAsset;

typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ u16 tileWidth;
    /* 0x06 */ u16 tileHeight;
    /* 0x08 */ u16 tilesPerRow;
    /* 0x0A */ u16 tilesPerCol;
    /* 0x0C */ u16 tileGridWidth;
    /* 0x0E */ u16 tileGridHeight;
    /* 0x10 */ u16 clipX;
    /* 0x12 */ u16 clipY;
    /* 0x14 */ u16 clipWidth;
    /* 0x16 */ u16 clipHeight;
    /* 0x18 */ s16 ciMode;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ u8 *textureData;
    /* 0x20 */ u16 *tileIndexData;
    /* 0x24 */ TileMapEntry *tileEntries;
    /* 0x28 */ PaletteChunk32 *paletteData;
} TileMapScrollRenderState;

void renderTiledTextureMap(TileMapScrollRenderState *state);
void initScrollingTileMapState(TileMapScrollRenderState *state, TileMapTextureAsset *asset);
