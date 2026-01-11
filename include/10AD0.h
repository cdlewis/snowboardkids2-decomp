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
    /* 0x04 */ u16 unk4;
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

void func_80010240_10E40(void);
void func_800105B0_111B0(SpriteRenderArg *arg0);
void func_80010C98_11898(void *arg0);
void func_80012FA8_13BA8(void);
void initDefaultFontPalette(void);
void func_8000FED0_10AD0(void);
void func_80013EA0_14AA0(s32, u16, u16, u16, s32);
void func_80012518_13118(void);
void func_800136E0_142E0(void);
void func_80012004_12C04(void);
void func_80011924_12524(void);
void func_80010924_11524(SpriteRenderArg *arg0);