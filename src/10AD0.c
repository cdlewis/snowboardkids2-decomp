#include "10AD0.h"
#include "common.h"

extern u16 gSpritePaletteModes[];
extern u16 gSpriteTextureFormats[];
extern Gfx gSpriteRDPSetupDL[];
extern s32 gCachedPaletteAddr;
extern s32 gCachedTextureAddr;
extern s16 gGraphicsMode;
extern Gfx *gRegionAllocPtr;
extern TextClipAndOffsetData gTextClipAndOffsetData;

void func_80013EA0_14AA0(s32, u16, u16, u16, s32);

void renderSpriteFrame(SpriteRenderArg *arg0) {
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
    s32 clipOffsetX;
    s32 clipOffsetY;
    s32 paletteBase;
    s32 paletteCacheAddr;
    u16 paletteMode;
    u16 format;
    u16 framePaletteIndex;
    SpriteSheetData *spriteData;
    SpriteFrameEntry *frameEntry;

    clipOffsetX = 0;
    clipOffsetY = 0;
    spriteData = arg0->spriteData;
    frameEntry = spriteData->frames;
    left = arg0->x + gTextClipAndOffsetData.offsetX;
    paletteBase = (s32)frameEntry + (spriteData->numFrames * 0x10);
    top = arg0->y + gTextClipAndOffsetData.offsetY;
    frameEntry = &frameEntry[arg0->frameIndex];
    framePaletteIndex = frameEntry->paletteIndex;
    paletteMode = gSpritePaletteModes[frameEntry->paletteTableIndex];
    format = gSpriteTextureFormats[frameEntry->formatIndex];
    right = left + frameEntry->width;
    bottom = top + frameEntry->height;

    if (left < gTextClipAndOffsetData.clipLeft) {
        clipOffsetX = gTextClipAndOffsetData.clipLeft - left;
        left = gTextClipAndOffsetData.clipLeft;
    }

    if (top < gTextClipAndOffsetData.clipTop) {
        clipOffsetY = gTextClipAndOffsetData.clipTop - top;
        top = gTextClipAndOffsetData.clipTop;
    }

    if ((gTextClipAndOffsetData.clipRight >= left) && (!(gTextClipAndOffsetData.clipBottom < top)) && (left < right) &&
        (top < bottom)) {

        if (gGraphicsMode != 0x100) {
            gGraphicsMode = 0x100;
            gCachedPaletteAddr = 0;
            gCachedTextureAddr = 0;
            gSPDisplayList(gRegionAllocPtr++, gSpriteRDPSetupDL);
        }

        if ((s32)arg0->spriteData + frameEntry->textureOffset != gCachedTextureAddr) {
            gCachedTextureAddr = (s32)arg0->spriteData + frameEntry->textureOffset;
            func_80013EA0_14AA0(
                (s32)arg0->spriteData + frameEntry->textureOffset,
                frameEntry->width,
                frameEntry->height,
                format,
                paletteMode
            );
        }

        paletteCacheAddr = paletteBase + (framePaletteIndex << 5);
        if (paletteCacheAddr != gCachedPaletteAddr) {
            gCachedPaletteAddr = paletteCacheAddr;
            if ((paletteMode & 0xFFFF) == 2) {
                if (format == 0) {
                    gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, paletteCacheAddr);
                } else {
                    gDPLoadTLUT_pal256(gRegionAllocPtr++, paletteCacheAddr);
                }
            }
        }

        gSPTextureRectangle(
            gRegionAllocPtr++,
            left << 2,
            top << 2,
            right << 2,
            bottom << 2,
            G_TX_RENDERTILE,
            clipOffsetX << 5,
            clipOffsetY << 5,
            0x0400,
            0x0400
        );
    }
}

void renderSpriteFrameWithPalette(SpriteRenderArg *arg0) {
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
    s32 clipOffsetX;
    s32 clipOffsetY;
    s32 paletteBase;
    s32 paletteCacheAddr;
    u16 paletteMode;
    u16 format;
    u8 paletteIndex;
    SpriteSheetData *spriteData;
    SpriteFrameEntry *frameEntry;

    clipOffsetX = 0;
    clipOffsetY = 0;
    spriteData = arg0->spriteData;
    frameEntry = spriteData->frames;
    left = arg0->x + gTextClipAndOffsetData.offsetX;
    paletteBase = (s32)frameEntry + (spriteData->numFrames * 0x10);
    top = arg0->y + gTextClipAndOffsetData.offsetY;
    frameEntry = &frameEntry[arg0->frameIndex];
    paletteIndex = arg0->paletteIndex;
    paletteMode = gSpritePaletteModes[frameEntry->paletteTableIndex];
    format = gSpriteTextureFormats[frameEntry->formatIndex];
    right = left + frameEntry->width;
    bottom = top + frameEntry->height;

    if (left < gTextClipAndOffsetData.clipLeft) {
        clipOffsetX = gTextClipAndOffsetData.clipLeft - left;
        left = gTextClipAndOffsetData.clipLeft;
    }

    if (top < gTextClipAndOffsetData.clipTop) {
        clipOffsetY = gTextClipAndOffsetData.clipTop - top;
        top = gTextClipAndOffsetData.clipTop;
    }

    if ((gTextClipAndOffsetData.clipRight >= left) && (!(gTextClipAndOffsetData.clipBottom < top)) && (left < right) &&
        (top < bottom)) {

        if (gGraphicsMode != 0x100) {
            gGraphicsMode = 0x100;
            gCachedPaletteAddr = 0;
            gCachedTextureAddr = 0;
            gSPDisplayList(gRegionAllocPtr++, gSpriteRDPSetupDL);
        }

        if (gCachedTextureAddr != (s32)arg0->spriteData + frameEntry->textureOffset) {
            gCachedTextureAddr = (s32)arg0->spriteData + frameEntry->textureOffset;
            func_80013EA0_14AA0(
                (s32)arg0->spriteData + frameEntry->textureOffset,
                frameEntry->width,
                frameEntry->height,
                format,
                paletteMode
            );
        }

        paletteCacheAddr = paletteBase + (paletteIndex << 5);
        if (paletteCacheAddr != gCachedPaletteAddr) {
            gCachedPaletteAddr = paletteCacheAddr;
            if ((paletteMode & 0xFFFF) == 2) {
                if (format == 0) {
                    gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, paletteCacheAddr);
                } else {
                    gDPLoadTLUT_pal256(gRegionAllocPtr++, paletteCacheAddr);
                }
            }
        }

        gSPTextureRectangle(
            gRegionAllocPtr++,
            left << 2,
            top << 2,
            right << 2,
            bottom << 2,
            G_TX_RENDERTILE,
            clipOffsetX << 5,
            clipOffsetY << 5,
            0x0400,
            0x0400
        );
    }
}

void renderHalfSizeSpriteFrame(SpriteRenderArg *arg0) {
    s32 clipOffsetY;
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
    s32 paletteCacheAddr;
    u16 paletteMode;
    u16 format;
    u16 framePaletteIndex;
    SpriteSheetData *spriteData;
    SpriteFrameEntry *frameEntry;
    s32 paletteBase;
    s32 clipOffsetX;

    clipOffsetY = 0;
    spriteData = arg0->spriteData;
    frameEntry = spriteData->frames;
    left = arg0->x + gTextClipAndOffsetData.offsetX;
    paletteBase = (s32)frameEntry + (spriteData->numFrames * 16);
    frameEntry = frameEntry + arg0->frameIndex;
    framePaletteIndex = frameEntry->paletteIndex;
    top = arg0->y + gTextClipAndOffsetData.offsetY;
    paletteMode = gSpritePaletteModes[frameEntry->paletteTableIndex];
    format = gSpriteTextureFormats[frameEntry->formatIndex];
    right = left + (frameEntry->width / 2);
    bottom = top + (frameEntry->height / 2);
    clipOffsetX = 0;

    if (left < gTextClipAndOffsetData.clipLeft) {
        clipOffsetX = gTextClipAndOffsetData.clipLeft - left;
        left = gTextClipAndOffsetData.clipLeft;
    }

    if (top < gTextClipAndOffsetData.clipTop) {
        clipOffsetY = gTextClipAndOffsetData.clipTop - top;
        top = gTextClipAndOffsetData.clipTop;
    }

    if ((gTextClipAndOffsetData.clipRight >= left) && (!(gTextClipAndOffsetData.clipBottom < top)) && (left < right) &&
        (top < bottom)) {

        if (gGraphicsMode != 0x100) {
            gGraphicsMode = 0x100;
            gCachedPaletteAddr = 0;
            gCachedTextureAddr = 0;
            gSPDisplayList(gRegionAllocPtr++, gSpriteRDPSetupDL);
        }

        if ((s32)arg0->spriteData + frameEntry->textureOffset != gCachedTextureAddr) {
            gCachedTextureAddr = (s32)arg0->spriteData + frameEntry->textureOffset;
            func_80013EA0_14AA0(
                (s32)arg0->spriteData + frameEntry->textureOffset,
                frameEntry->width,
                frameEntry->height,
                format,
                paletteMode
            );
        }

        paletteCacheAddr = paletteBase + (framePaletteIndex << 5);
        if (paletteCacheAddr != gCachedPaletteAddr) {
            gCachedPaletteAddr = paletteCacheAddr;
            if ((paletteMode & 0xFFFF) == 2) {
                if (format == 0) {
                    gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, paletteCacheAddr);
                } else {
                    gDPLoadTLUT_pal256(gRegionAllocPtr++, paletteCacheAddr);
                }
            }
        }

        gSPTextureRectangle(
            gRegionAllocPtr++,
            left * 4,
            top * 4,
            right * 4,
            bottom * 4,
            G_TX_RENDERTILE,
            clipOffsetX << 5,
            clipOffsetY << 5,
            0x0800,
            0x0800
        );
    }
}

void func_80010924_11524(SpriteRenderArg *arg0) {
    s32 clipOffsetY;
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
    s32 paletteCacheAddr;
    u16 paletteMode;
    u16 format;
    u8 paletteIndex;
    SpriteSheetData *spriteData;
    SpriteFrameEntry *frameEntry;
    s32 paletteBase;
    s32 clipOffsetX;

    clipOffsetY = 0;
    spriteData = arg0->spriteData;
    frameEntry = spriteData->frames;
    left = arg0->x + gTextClipAndOffsetData.offsetX;
    paletteBase = (s32)frameEntry + (spriteData->numFrames * 16);
    frameEntry = frameEntry + arg0->frameIndex;
    paletteIndex = arg0->paletteIndex;
    top = arg0->y + gTextClipAndOffsetData.offsetY;
    paletteMode = gSpritePaletteModes[frameEntry->paletteTableIndex];
    format = gSpriteTextureFormats[frameEntry->formatIndex];
    right = left + (frameEntry->width / 2);
    bottom = top + (frameEntry->height / 2);
    clipOffsetX = 0;

    if (left < gTextClipAndOffsetData.clipLeft) {
        clipOffsetX = gTextClipAndOffsetData.clipLeft - left;
        left = gTextClipAndOffsetData.clipLeft;
    }

    if (top < gTextClipAndOffsetData.clipTop) {
        clipOffsetY = gTextClipAndOffsetData.clipTop - top;
        top = gTextClipAndOffsetData.clipTop;
    }

    if ((gTextClipAndOffsetData.clipRight >= left) && (!(gTextClipAndOffsetData.clipBottom < top)) && (left < right) &&
        (top < bottom)) {

        if (gGraphicsMode != 0x100) {
            gGraphicsMode = 0x100;
            gCachedPaletteAddr = 0;
            gCachedTextureAddr = 0;
            gSPDisplayList(gRegionAllocPtr++, gSpriteRDPSetupDL);
        }

        if ((s32)arg0->spriteData + frameEntry->textureOffset != gCachedTextureAddr) {
            gCachedTextureAddr = (s32)arg0->spriteData + frameEntry->textureOffset;
            func_80013EA0_14AA0(
                (s32)arg0->spriteData + frameEntry->textureOffset,
                frameEntry->width,
                frameEntry->height,
                format,
                paletteMode
            );
        }

        paletteCacheAddr = paletteBase + (paletteIndex << 5);
        if (paletteCacheAddr != gCachedPaletteAddr) {
            gCachedPaletteAddr = paletteCacheAddr;
            if ((paletteMode & 0xFFFF) == 2) {
                if (format == 0) {
                    gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, paletteCacheAddr);
                } else {
                    gDPLoadTLUT_pal256(gRegionAllocPtr++, paletteCacheAddr);
                }
            }
        }

        gSPTextureRectangle(
            gRegionAllocPtr++,
            left * 4,
            top * 4,
            right * 4,
            bottom * 4,
            G_TX_RENDERTILE,
            clipOffsetX << 5,
            clipOffsetY << 5,
            0x0800,
            0x0800
        );
    }
}

INCLUDE_ASM("asm/nonmatchings/10AD0", func_80010C98_11898);

INCLUDE_ASM("asm/nonmatchings/10AD0", func_800112AC_11EAC);

INCLUDE_ASM("asm/nonmatchings/10AD0", func_80011924_12524);

INCLUDE_ASM("asm/nonmatchings/10AD0", func_80012004_12C04);

INCLUDE_ASM("asm/nonmatchings/10AD0", func_80012518_13118);

INCLUDE_ASM("asm/nonmatchings/10AD0", func_80012A58_13658);

INCLUDE_ASM("asm/nonmatchings/10AD0", func_80012FA8_13BA8);

INCLUDE_ASM("asm/nonmatchings/10AD0", func_800136E0_142E0);

INCLUDE_ASM("asm/nonmatchings/10AD0", func_80013EA0_14AA0);

extern u16 gDefaultFontPalette[];

void initDefaultFontPalette(void) {
    s32 i;

    gDefaultFontPalette[0] = 0;
    for (i = 1; i < 256; i++) {
        gDefaultFontPalette[i] = 0xFFFF;
    }
}
