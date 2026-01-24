#pragma once

#include "common.h"

typedef struct {
    /* 0x00 */ s16 clipLeft;
    /* 0x02 */ s16 clipTop;
    /* 0x04 */ s16 clipRight;
    /* 0x06 */ s16 clipBottom;
    /* 0x08 */ s16 offsetX;
    /* 0x0A */ s16 offsetY;
} TextClipAndOffsetData;

typedef struct {
    /* 0x00 */ s32 textureOffset;
    /* 0x04 */ u16 paletteIndex;
    /* 0x06 */ u16 width;
    /* 0x08 */ u16 height;
    /* 0x0A */ u16 paletteTableIndex;
    /* 0x0C */ u16 formatIndex;
    /* 0x0E */ u16 pad0E;
} SpriteFrameEntry;

typedef struct {
    /* 0x00 */ s32 textureBase;
    /* 0x04 */ s32 numFrames;
    /* 0x08 */ SpriteFrameEntry frames[1];
} SpriteSheetData;

typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ SpriteSheetData *spriteData;
    /* 0x08 */ u16 frameIndex;
    /* 0x0A */ u8 paletteIndex;
} SpriteRenderArg;

typedef struct {
    u16 textureIndex;
    u8 paletteIndex;
    u8 flipMode;
} TileEntry;

typedef struct {
    u8 data[32];
} TexData32;

typedef struct {
    u16 tileGridWidth;
    u16 tileGridHeight;
    u8 tileWidth;
    u8 tileHeight;
    u16 ciMode;
    u8 pad08[2];
    u16 tileIndexDataOffset;
    u16 paletteDataOffset;
    u16 textureDataOffset;
    u8 tileEntries[1];
} TiledTextureAsset;

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
    /* 0x1C */ s32 textureData;
    /* 0x20 */ s32 tileIndexData;
    /* 0x24 */ TileEntry *tileEntries;
    /* 0x28 */ TexData32 *paletteData;
} TiledTextureRenderState;

typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ void *asset;
    /* 0x08 */ s16 spriteIndex;
    /* 0x0A */ s16 scaleX;
    /* 0x0C */ s16 scaleY;
    /* 0x0E */ s16 rotation;
    /* 0x10 */ s16 alpha;
    /* 0x12 */ u8 unk12;
    /* 0x13 */ u8 unk13;
    /* 0x14 */ u8 unk14;
} ScaledSpriteEntry;

void renderSpriteFrame(SpriteRenderArg *arg0);
void renderSpriteFrameWithPalette(SpriteRenderArg *arg0);
void renderHalfSizeSpriteFrame(SpriteRenderArg *arg0);
void renderHalfSizeSpriteWithCustomPalette(SpriteRenderArg *arg0);
void func_80010C98_11898(void *arg0);
void func_80012FA8_13BA8(void);
void initDefaultFontPalette(void);
void func_80013EA0_14AA0(s32, u16, u16, u16, s32);
void func_80012518_13118(void);
void func_800136E0_142E0(void);
void func_80012004_12C04(void);
void func_80011924_12524(void);