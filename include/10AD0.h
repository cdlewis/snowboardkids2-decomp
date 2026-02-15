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
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ SpriteSheetData *spriteData;
    /* 0x08 */ u16 frameIndex;
    /* 0x0A */ u8 paletteIndex;
    /* 0x0B */ u8 alpha;
    /* 0x0C */ u8 tileMode;
    /* 0x0D */ u8 overridePaletteCount;
    /* 0x0E */ u8 transparency;
} TextRenderArg;

typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ SpriteSheetData *spriteData;
    /* 0x08 */ u16 frameIndex;
    /* 0x0A */ u8 r;
    /* 0x0B */ u8 g;
    /* 0x0C */ u8 b;
    /* 0x0D */ u8 a;
    /* 0x0E */ u8 mode;
    /* 0x0F */ u8 paletteOverride;
    /* 0x10 */ u8 primColor;
} TintedSpriteArg;

void renderSpriteFrame(SpriteRenderArg *arg0);
void renderSpriteFrameWithPalette(SpriteRenderArg *arg0);
void renderHalfSizeSpriteFrame(SpriteRenderArg *arg0);
void renderHalfSizeSpriteWithCustomPalette(SpriteRenderArg *arg0);
void func_80010C98_11898(void *arg0);
void renderAlphaBlendedTextSprite(TextRenderArg *arg0);
void initDefaultFontPalette(void);
void func_80013EA0_14AA0(s32, u16, u16, u16, s32);
void func_80012A58_13658(TintedSpriteArg *arg0);
void func_80012518_13118(TextRenderArg *arg0);
void func_800136E0_142E0(void);
void renderTextSprite(TextRenderArg *arg0);
void func_80011924_12524(void);
