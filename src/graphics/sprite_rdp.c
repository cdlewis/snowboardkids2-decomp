#include "graphics/sprite_rdp.h"
#include "common.h"
#include "gbi.h"
#include "graphics/graphics.h"
#include "gs2dex.h"

u16 gSpritePaletteModes[] = { 0x0002, 0x0000, 0x0003, 0x0004 };

u16 gSpriteTextureFormats[] = { 0x0000, 0x0001, 0x0002, 0x0000 };

s16 gTileTextureFlipTable[] = { 1, 1, -1, 1, 1, -1, -1, -1 };

Gfx gSpriteRDPSetupDL[] = {
    gsDPPipeSync(),
    gsDPSetCycleType(G_CYC_1CYCLE),
    gsDPSetTexturePersp(G_TP_NONE),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsSPObjRenderMode(G_OBJRM_BILERP),
    gsDPSetTextureLUT(G_TT_RGBA16),
    gsDPSetCombineLERP(1, 0, TEXEL0, 0, 1, 0, TEXEL0, 0, 1, 0, TEXEL0, 0, 1, 0, TEXEL0, 0),
    gsDPSetRenderMode(G_RM_RA_SPRITE | IM_RD, G_RM_RA_SPRITE2),
    gsSPEndDisplayList(),
};

extern SpriteFrameEntry *gCachedPaletteAddr;
extern s32 gCachedTextureAddr;
extern s16 gGraphicsMode;
extern Gfx *gDisplayListAllocPtr;
extern u16 gDefaultFontPalette[];

void renderSpriteFrame(SpriteRenderArg *sprite) {
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
    s32 clipOffsetX;
    s32 clipOffsetY;
    SpriteFrameEntry *paletteBase;
    SpriteFrameEntry *paletteCacheAddr;
    u16 paletteMode;
    u16 format;
    u16 framePaletteIndex;
    SpriteSheetData *spriteData;
    SpriteFrameEntry *frameEntry;

    clipOffsetX = 0;
    clipOffsetY = 0;
    spriteData = sprite->spriteData;
    frameEntry = spriteData->frames;
    left = sprite->x + gTextClipAndOffsetData.offsetX;
    paletteBase = &frameEntry[spriteData->numFrames];
    top = sprite->y + gTextClipAndOffsetData.offsetY;
    frameEntry = &frameEntry[sprite->frameIndex];
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
            gCachedPaletteAddr = NULL;
            gCachedTextureAddr = 0;
            gSPDisplayList(gDisplayListAllocPtr++, gSpriteRDPSetupDL);
        }

        if ((s32)sprite->spriteData + frameEntry->textureOffset != gCachedTextureAddr) {
            gCachedTextureAddr = (s32)sprite->spriteData + frameEntry->textureOffset;
            loadSpriteTexture(
                (s32)sprite->spriteData + frameEntry->textureOffset,
                frameEntry->width,
                frameEntry->height,
                format,
                paletteMode
            );
        }

        paletteCacheAddr = &paletteBase[framePaletteIndex << 1];
        if (paletteCacheAddr != gCachedPaletteAddr) {
            gCachedPaletteAddr = paletteCacheAddr;
            if ((paletteMode & 0xFFFF) == 2) {
                if (format == 0) {
                    gDPLoadTLUT_pal16(gDisplayListAllocPtr++, 0, paletteCacheAddr);
                } else {
                    gDPLoadTLUT_pal256(gDisplayListAllocPtr++, paletteCacheAddr);
                }
            }
        }

        gSPTextureRectangle(
            gDisplayListAllocPtr++,
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

void renderSpriteFrameWithPalette(SpriteRenderArg *sprite) {
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
    s32 clipOffsetX;
    s32 clipOffsetY;
    SpriteFrameEntry *paletteBase;
    SpriteFrameEntry *paletteCacheAddr;
    u16 paletteMode;
    u16 format;
    u8 paletteIndex;
    SpriteSheetData *spriteData;
    SpriteFrameEntry *frameEntry;

    clipOffsetX = 0;
    clipOffsetY = 0;
    spriteData = sprite->spriteData;
    frameEntry = spriteData->frames;
    left = sprite->x + gTextClipAndOffsetData.offsetX;
    paletteBase = &frameEntry[spriteData->numFrames];
    top = sprite->y + gTextClipAndOffsetData.offsetY;
    frameEntry = &frameEntry[sprite->frameIndex];
    paletteIndex = sprite->paletteIndex;
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
            gCachedPaletteAddr = NULL;
            gCachedTextureAddr = 0;
            gSPDisplayList(gDisplayListAllocPtr++, gSpriteRDPSetupDL);
        }

        if (gCachedTextureAddr != (s32)sprite->spriteData + frameEntry->textureOffset) {
            gCachedTextureAddr = (s32)sprite->spriteData + frameEntry->textureOffset;
            loadSpriteTexture(
                (s32)sprite->spriteData + frameEntry->textureOffset,
                frameEntry->width,
                frameEntry->height,
                format,
                paletteMode
            );
        }

        paletteCacheAddr = &paletteBase[paletteIndex << 1];
        if (paletteCacheAddr != gCachedPaletteAddr) {
            gCachedPaletteAddr = paletteCacheAddr;
            if ((paletteMode & 0xFFFF) == 2) {
                if (format == 0) {
                    gDPLoadTLUT_pal16(gDisplayListAllocPtr++, 0, paletteCacheAddr);
                } else {
                    gDPLoadTLUT_pal256(gDisplayListAllocPtr++, paletteCacheAddr);
                }
            }
        }

        gSPTextureRectangle(
            gDisplayListAllocPtr++,
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

void renderHalfSizeSpriteFrame(SpriteRenderArg *sprite) {
    s32 clipOffsetY;
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
    SpriteFrameEntry *paletteCacheAddr;
    u16 paletteMode;
    u16 format;
    u16 framePaletteIndex;
    SpriteSheetData *spriteData;
    SpriteFrameEntry *frameEntry;
    SpriteFrameEntry *paletteBase;
    s32 clipOffsetX;

    clipOffsetY = 0;
    spriteData = sprite->spriteData;
    frameEntry = spriteData->frames;
    left = sprite->x + gTextClipAndOffsetData.offsetX;
    paletteBase = &frameEntry[spriteData->numFrames];
    frameEntry = frameEntry + sprite->frameIndex;
    framePaletteIndex = frameEntry->paletteIndex;
    top = sprite->y + gTextClipAndOffsetData.offsetY;
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
            gCachedPaletteAddr = NULL;
            gCachedTextureAddr = 0;
            gSPDisplayList(gDisplayListAllocPtr++, gSpriteRDPSetupDL);
        }

        if ((s32)sprite->spriteData + frameEntry->textureOffset != gCachedTextureAddr) {
            gCachedTextureAddr = (s32)sprite->spriteData + frameEntry->textureOffset;
            loadSpriteTexture(
                (s32)sprite->spriteData + frameEntry->textureOffset,
                frameEntry->width,
                frameEntry->height,
                format,
                paletteMode
            );
        }

        paletteCacheAddr = &paletteBase[framePaletteIndex << 1];
        if (paletteCacheAddr != gCachedPaletteAddr) {
            gCachedPaletteAddr = paletteCacheAddr;
            if ((paletteMode & 0xFFFF) == 2) {
                if (format == 0) {
                    gDPLoadTLUT_pal16(gDisplayListAllocPtr++, 0, paletteCacheAddr);
                } else {
                    gDPLoadTLUT_pal256(gDisplayListAllocPtr++, paletteCacheAddr);
                }
            }
        }

        gSPTextureRectangle(
            gDisplayListAllocPtr++,
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

void renderHalfSizeSpriteWithCustomPalette(SpriteRenderArg *sprite) {
    s32 clipOffsetY;
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
    SpriteFrameEntry *paletteCacheAddr;
    u16 paletteMode;
    u16 format;
    u8 paletteIndex;
    SpriteSheetData *spriteData;
    SpriteFrameEntry *frameEntry;
    SpriteFrameEntry *paletteBase;
    s32 clipOffsetX;

    clipOffsetY = 0;
    spriteData = sprite->spriteData;
    frameEntry = spriteData->frames;
    left = sprite->x + gTextClipAndOffsetData.offsetX;
    paletteBase = &frameEntry[spriteData->numFrames];
    frameEntry = frameEntry + sprite->frameIndex;
    paletteIndex = sprite->paletteIndex;
    top = sprite->y + gTextClipAndOffsetData.offsetY;
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
            gCachedPaletteAddr = NULL;
            gCachedTextureAddr = 0;
            gSPDisplayList(gDisplayListAllocPtr++, gSpriteRDPSetupDL);
        }

        if ((s32)sprite->spriteData + frameEntry->textureOffset != gCachedTextureAddr) {
            gCachedTextureAddr = (s32)sprite->spriteData + frameEntry->textureOffset;
            loadSpriteTexture(
                (s32)sprite->spriteData + frameEntry->textureOffset,
                frameEntry->width,
                frameEntry->height,
                format,
                paletteMode
            );
        }

        paletteCacheAddr = &paletteBase[paletteIndex << 1];
        if (paletteCacheAddr != gCachedPaletteAddr) {
            gCachedPaletteAddr = paletteCacheAddr;
            if ((paletteMode & 0xFFFF) == 2) {
                if (format == 0) {
                    gDPLoadTLUT_pal16(gDisplayListAllocPtr++, 0, paletteCacheAddr);
                } else {
                    gDPLoadTLUT_pal256(gDisplayListAllocPtr++, paletteCacheAddr);
                }
            }
        }

        gSPTextureRectangle(
            gDisplayListAllocPtr++,
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

void renderCharSelectIconSprite(CharSelectIconEntry *sprite) {
    SpriteFrameEntry *paletteBase;
    u16 paletteMode;
    s32 right;
    s32 bottom;
    s32 left = 0;
    s32 top = 0;
    s16 clipOffsetY;
    s16 clipOffsetX;
    u16 format;
    u16 paletteIndex;
    s16 scaleS;
    s16 scaleT;
    s16 dsdx;
    s16 dtdy;
    s32 clipVal;
    SpriteFrameEntry *frameEntry;
    SpriteFrameEntry *paletteAddr;

    frameEntry = sprite->spriteAsset->frames;
    paletteBase = &frameEntry[sprite->spriteAsset->numFrames];
    frameEntry = &frameEntry[sprite->spriteIndex];
    paletteMode = gSpritePaletteModes[frameEntry->paletteTableIndex];
    format = gSpriteTextureFormats[frameEntry->formatIndex];
    scaleS = gTileTextureFlipTable[sprite->tileMode * 2];
    scaleT = gTileTextureFlipTable[sprite->tileMode * 2 + 1];

    if (sprite->overridePaletteCount == 0) {
        paletteIndex = frameEntry->paletteIndex;
    } else {
        paletteIndex = sprite->overridePaletteCount - 1;
    }

    if ((sprite->scaleX > 0x7FFFU) || (sprite->scaleX == 0)) {
        return;
    }
    if ((sprite->scaleY > 0x7FFFU) || (sprite->scaleY == 0)) {
        return;
    }

    sprite->tileMode &= 3;

    right = left + (s16)(((u16)sprite->currentY << 12) / sprite->scaleX);
    bottom = top + (s16)(((u16)sprite->textureHeight << 12) / sprite->scaleY);

    dsdx = ((u16)sprite->currentY << 12) / sprite->scaleX;
    dtdy = ((u16)sprite->textureHeight << 12) / sprite->scaleY;

    left = (gTextClipAndOffsetData.offsetX + sprite->baseY) * 4;
    top = (gTextClipAndOffsetData.offsetY + sprite->x) * 4;
    right = left + dsdx;
    bottom = top + dtdy;

    clipOffsetX = 0;
    if (scaleS == -1) {
        clipOffsetX = (frameEntry->width * 4) - 4;
    }

    clipVal = gTextClipAndOffsetData.clipLeft;
    clipVal *= 4;
    if (left < clipVal) {
        clipVal = gTextClipAndOffsetData.clipLeft;
        clipVal *= 4;
        if (scaleS == -1) {
            clipOffsetX -= clipVal - left;
        } else {
            clipOffsetX = clipVal - left;
        }
        left = gTextClipAndOffsetData.clipLeft * 4;
    }

    clipOffsetY = 0;
    if (scaleT == -1) {
        clipOffsetY = frameEntry->height * 4 - 4;
    }

    clipVal = gTextClipAndOffsetData.clipTop;
    clipVal *= 4;
    if (top < clipVal) {
        clipVal = gTextClipAndOffsetData.clipTop;
        clipVal *= 4;
        if (scaleT == -1) {
            clipOffsetY -= clipVal - top;
        } else {
            clipOffsetY = clipVal - top;
        }
        top = gTextClipAndOffsetData.clipTop * 4;
    }

    if ((gTextClipAndOffsetData.clipRight * 4 >= left) &&
        (((gTextClipAndOffsetData.clipBottom * 4 >= top)) && (left < right)) && (top < bottom)) {

        dsdx = 0;
        if (gGraphicsMode != 0x100) {
            gGraphicsMode = 0x100;
            gCachedPaletteAddr = 0;
            gCachedTextureAddr = 0;
            gSPDisplayList(gDisplayListAllocPtr++, gSpriteRDPSetupDL);
        }

        gDPPipeSync(gDisplayListAllocPtr++);

        if (sprite->scaleX != 0x400 || sprite->scaleY != sprite->scaleX) {
            gDPSetRenderMode(gDisplayListAllocPtr++, G_RM_TEX_EDGE, G_RM_TEX_EDGE2);
        }

        if ((s32)sprite->spriteAsset + frameEntry->textureOffset != gCachedTextureAddr) {
            gCachedTextureAddr = (s32)sprite->spriteAsset + frameEntry->textureOffset;
            loadSpriteTexture(
                (s32)sprite->spriteAsset + frameEntry->textureOffset,
                frameEntry->width,
                frameEntry->height,
                format,
                paletteMode
            );
        }

        if (paletteIndex == 0xFE) {
            if (gCachedPaletteAddr != (SpriteFrameEntry *)gDefaultFontPalette) {
                gCachedPaletteAddr = (SpriteFrameEntry *)gDefaultFontPalette;
                if (format == 0) {
                    gDPLoadTLUT_pal16(gDisplayListAllocPtr++, 0, gDefaultFontPalette);
                } else {
                    gDPLoadTLUT_pal256(gDisplayListAllocPtr++, gDefaultFontPalette);
                }
            }
        } else {
            paletteAddr = &paletteBase[paletteIndex << 1];
            if (paletteAddr != gCachedPaletteAddr) {
                gCachedPaletteAddr = paletteAddr;
                if (format == 0) {
                    gDPLoadTLUT_pal16(gDisplayListAllocPtr++, 0, paletteAddr);
                } else {
                    gDPLoadTLUT_pal256(gDisplayListAllocPtr++, paletteAddr);
                }
            }
        }

        gSPTextureRectangle(
            gDisplayListAllocPtr++,
            left,
            top,
            right,
            bottom,
            G_TX_RENDERTILE,
            clipOffsetX * 8,
            (clipOffsetY * 8) & 0xFFF8,
            scaleS * sprite->scaleX,
            scaleT * sprite->scaleY
        );

        gDPPipeSync(gDisplayListAllocPtr++);

        if (sprite->scaleX != 0x400 || sprite->scaleY != sprite->scaleX) {
            {
                Gfx *_g = (gDisplayListAllocPtr++);
                _g->words.w0 = 0xE200001C | dsdx;
                _g->words.w1 = G_RM_AA_TEX_TERR | G_RM_AA_TEX_TERR2;
            }
        }
    }
}

void renderScaledShadedSpriteFrame(ScaledSpriteArg *sprite) {
    s32 bottom;
    s16 scaleS;
    s16 scaleT;
    s32 left;
    s32 top;
    s32 right;
    s32 clipOffsetX;
    s32 clipOffsetY;
    SpriteFrameEntry *paletteBase;
    u16 paletteMode;
    u16 format;
    u16 paletteIndex;
    SpriteFrameEntry *frameEntry;
    s32 scaleW;
    s32 scaleH;
    s32 widthTimes4;

    frameEntry = sprite->spriteData->frames;
    paletteBase = &frameEntry[sprite->spriteData->numFrames];
    frameEntry = &frameEntry[sprite->frameIndex];

    paletteMode = gSpritePaletteModes[frameEntry->paletteTableIndex];
    format = gSpriteTextureFormats[frameEntry->formatIndex];
    scaleS = gTileTextureFlipTable[sprite->tileMode * 2];
    scaleT = gTileTextureFlipTable[sprite->tileMode * 2 + 1];

    if (sprite->overridePaletteCount == 0) {
        paletteIndex = frameEntry->paletteIndex;
    } else {
        paletteIndex = sprite->overridePaletteCount - 1;
    }

    if (sprite->renderWidth > 0x7FFF || sprite->renderWidth == 0) {
        return;
    }
    if (sprite->renderHeight > 0x7FFF || sprite->renderHeight == 0) {
        return;
    }

    clipOffsetX = 0;

    sprite->tileMode = sprite->tileMode & 3;

    widthTimes4 = frameEntry->width << 2;
    scaleW = (frameEntry->width << 12) / sprite->renderWidth;
    scaleH = (frameEntry->height << 12) / sprite->renderHeight;

    left = (sprite->x * 4) - ((u32)scaleW >> 1) + (gTextClipAndOffsetData.offsetX * 4);
    top = (sprite->y * 4) - ((u32)scaleH >> 1) + (gTextClipAndOffsetData.offsetY * 4);
    bottom = top + scaleH;
    right = left + scaleW;

    if (scaleS == -1) {
        clipOffsetX = widthTimes4 - 4;
    }

    if (left < gTextClipAndOffsetData.clipLeft * 4) {
        if (scaleS == -1) {
            clipOffsetX -= gTextClipAndOffsetData.clipLeft * 4 - left;
        } else {
            clipOffsetX = gTextClipAndOffsetData.clipLeft * 4 - left;
        }
        left = gTextClipAndOffsetData.clipLeft * 4;
    }

    clipOffsetY = 0;
    if (scaleT == -1) {
        clipOffsetY = frameEntry->height * 4 - 4;
    }

    if (top < gTextClipAndOffsetData.clipTop * 4) {
        if (scaleT == -1) {
            clipOffsetY -= gTextClipAndOffsetData.clipTop * 4 - top;
        } else {
            clipOffsetY = gTextClipAndOffsetData.clipTop * 4 - top;
        }
        top = gTextClipAndOffsetData.clipTop * 4;
    }

    if ((gTextClipAndOffsetData.clipRight * 4 >= left) && (!(gTextClipAndOffsetData.clipBottom * 4 < top)) &&
        (left < right) && (top < bottom)) {

        if (gGraphicsMode != 0x100) {
            gGraphicsMode = 0x100;
            gCachedPaletteAddr = NULL;
            gCachedTextureAddr = 0;
            gSPDisplayList(gDisplayListAllocPtr++, gSpriteRDPSetupDL);
        }

        {
            u32 combineCmd = 0xFC11E223;
            Gfx *gfx = gDisplayListAllocPtr;
            u32 combineArg = 0xFFC7FFFF;

            gDisplayListAllocPtr = (Gfx *)((s32)gfx + 8);
            __asm__ volatile("" : : "r"(gDisplayListAllocPtr) : "memory");
            gfx->words.w0 = 0xE7000000;
            gDisplayListAllocPtr = (Gfx *)((s32)gfx + 0x10);
            __asm__ volatile("" : : "r"(gDisplayListAllocPtr) : "memory");
            gDisplayListAllocPtr = (Gfx *)((s32)gfx + 0x18);

            gfx->words.w1 = 0;
            (gfx + 1)->words.w0 = combineCmd;
            (gfx + 1)->words.w1 = combineArg;
            (gfx + 2)->words.w0 = 0xFA000000;

            {
                u8 shade = sprite->shade;
                (gfx + 2)->words.w1 = (shade << 24) | (shade << 16) | (shade << 8) | 0xFF;
            }

            if (sprite->renderWidth != 0x400 || sprite->renderHeight != sprite->renderWidth) {
                gDisplayListAllocPtr = (Gfx *)((s32)gfx + 0x20);
                (gfx + 3)->words.w0 = 0xE200001C;
                (gfx + 3)->words.w1 = 0x0F0A7008;
            }
        }

        if ((s32)sprite->spriteData + frameEntry->textureOffset != gCachedTextureAddr) {
            gCachedTextureAddr = (s32)sprite->spriteData + frameEntry->textureOffset;
            loadSpriteTexture(
                (s32)sprite->spriteData + frameEntry->textureOffset,
                frameEntry->width,
                frameEntry->height,
                format,
                paletteMode
            );
        }

        {
            u32 palIdx = paletteIndex & 0xFFFF;
            if (palIdx == 0xFE) {
                if (gCachedPaletteAddr != (SpriteFrameEntry *)gDefaultFontPalette) {
                    gCachedPaletteAddr = (SpriteFrameEntry *)gDefaultFontPalette;
                    if (format == 0) {
                        gDPLoadTLUT_pal16(gDisplayListAllocPtr++, 0, gDefaultFontPalette);
                    } else {
                        gDPLoadTLUT_pal256(gDisplayListAllocPtr++, gDefaultFontPalette);
                    }
                }
            } else {
                SpriteFrameEntry *paletteAddr = &paletteBase[palIdx << 1];
                if (paletteAddr != gCachedPaletteAddr) {
                    gCachedPaletteAddr = paletteAddr;
                    if (format == 0) {
                        gDPLoadTLUT_pal16(gDisplayListAllocPtr++, 0, paletteAddr);
                    } else {
                        gDPLoadTLUT_pal256(gDisplayListAllocPtr++, paletteAddr);
                    }
                }
            }
        }

        gSPTextureRectangle(
            gDisplayListAllocPtr++,
            left,
            top,
            right,
            bottom,
            G_TX_RENDERTILE,
            clipOffsetX << 3,
            clipOffsetY << 3,
            (s16)scaleS * sprite->renderWidth,
            (s16)scaleT * sprite->renderHeight
        );

        gDPPipeSync(gDisplayListAllocPtr++);
        gDPSetCombineMode(gDisplayListAllocPtr++, G_CC_DECALRGBA, G_CC_DECALRGBA);

        if (sprite->renderWidth != 0x400 || sprite->renderHeight != sprite->renderWidth) {
            Gfx *_g2 = gDisplayListAllocPtr++;
            _g2->words.w0 = 0xE200001C;
            _g2->words.w1 = 0x503048;
        }
    }
}

void renderScaledAlphaSpriteFrame(FrameSpriteEntry *sprite) {
    SpriteFrameEntry *paletteBase;
    u16 paletteMode;
    s32 tile;
    s32 right;
    s32 bottom;
    s32 left;
    s32 top;
    s16 clipOffsetY;
    s16 clipOffsetX;
    u16 format;
    u16 paletteIndex;
    s16 scaleS;
    s16 scaleT;
    s16 dsdx;
    s16 dtdy;
    s32 clipVal;
    SpriteFrameEntry *frameEntry;

    frameEntry = sprite->spriteData->frames;
    paletteBase = &frameEntry[sprite->spriteData->numFrames];
    frameEntry = &frameEntry[sprite->frameIndex];

    paletteMode = gSpritePaletteModes[frameEntry->paletteTableIndex];
    format = gSpriteTextureFormats[frameEntry->formatIndex];
    scaleS = gTileTextureFlipTable[sprite->tileMode * 2];
    scaleT = gTileTextureFlipTable[sprite->tileMode * 2 + 1];

    if (sprite->overridePaletteCount == 0) {
        paletteIndex = frameEntry->paletteIndex;
    } else {
        paletteIndex = sprite->overridePaletteCount - 1;
    }

    if ((sprite->scaleX > 0x7FFF) || (sprite->scaleX == 0)) {
        return;
    }
    if ((sprite->scaleY > 0x7FFF) || (sprite->scaleY == 0)) {
        return;
    }

    sprite->tileMode &= 3;

    dsdx = (frameEntry->width << 12) / sprite->scaleX;
    dtdy = (frameEntry->height << 12) / sprite->scaleY;

    left = (sprite->x * 4 - dsdx / 2) + gTextClipAndOffsetData.offsetX * 4;
    top = (sprite->y * 4 - dtdy / 2) + gTextClipAndOffsetData.offsetY * 4;
    bottom = top + dtdy;
    right = left + dsdx;

    clipOffsetX = 0;
    if (scaleS == -1) {
        clipOffsetX = frameEntry->width * 4 - 4;
    }

    clipVal = gTextClipAndOffsetData.clipLeft;
    clipVal *= 4;
    if (left < clipVal) {
        clipVal = gTextClipAndOffsetData.clipLeft;
        clipVal *= 4;
        if (scaleS == -1) {
            clipOffsetX -= clipVal - left;
        } else {
            clipOffsetX = clipVal - left;
        }
        left = gTextClipAndOffsetData.clipLeft * 4;
    }

    clipOffsetY = 0;
    if (scaleT == -1) {
        clipOffsetY = frameEntry->height * 4 - 4;
    }

    clipVal = gTextClipAndOffsetData.clipTop;
    clipVal *= 4;
    if (top < clipVal) {
        clipVal = gTextClipAndOffsetData.clipTop;
        clipVal *= 4;
        if (scaleT == -1) {
            clipOffsetY -= clipVal - top;
        } else {
            clipOffsetY = clipVal - top;
        }
        top = gTextClipAndOffsetData.clipTop * 4;
    }

    if (gTextClipAndOffsetData.clipRight * 4 < left) {
        return;
    }

    if (!(gTextClipAndOffsetData.clipBottom * 4 < top) && (left < right) && (top < bottom)) {
        if (gGraphicsMode != 0x100) {
            gGraphicsMode = 0x100;
            gCachedPaletteAddr = NULL;
            gCachedTextureAddr = 0;
            gSPDisplayList(gDisplayListAllocPtr++, gSpriteRDPSetupDL);
        }

        gDPPipeSync(gDisplayListAllocPtr++);

        if (sprite->alpha != 0xFF) {
            gDPSetRenderMode(gDisplayListAllocPtr++, G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);
        } else if (sprite->scaleX != 0x400 || sprite->scaleY != sprite->scaleX) {
            gDPSetRenderMode(gDisplayListAllocPtr++, G_RM_TEX_EDGE, G_RM_TEX_EDGE2);
        }

        gDPSetCombineMode(gDisplayListAllocPtr++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);

        gDPSetPrimColor(gDisplayListAllocPtr++, 0, 0, sprite->shade, sprite->shade, sprite->shade, sprite->alpha);

        if ((s32)sprite->spriteData + frameEntry->textureOffset != gCachedTextureAddr) {
            gCachedTextureAddr = (s32)sprite->spriteData + frameEntry->textureOffset;
            loadSpriteTexture(
                (s32)sprite->spriteData + frameEntry->textureOffset,
                frameEntry->width,
                frameEntry->height,
                format,
                paletteMode
            );
        }

        if (paletteIndex == 0xFE) {
            if (gCachedPaletteAddr != (SpriteFrameEntry *)gDefaultFontPalette) {
                gCachedPaletteAddr = (SpriteFrameEntry *)gDefaultFontPalette;
                if (format == 0) {
                    gDPLoadTLUT_pal16(gDisplayListAllocPtr++, 0, gDefaultFontPalette);
                } else {
                    gDPLoadTLUT_pal256(gDisplayListAllocPtr++, gDefaultFontPalette);
                }
            }
        } else {
            SpriteFrameEntry *paletteAddr = &paletteBase[paletteIndex << 1];
            if (paletteAddr != gCachedPaletteAddr) {
                gCachedPaletteAddr = paletteAddr;
                if (format == 0) {
                    gDPLoadTLUT_pal16(gDisplayListAllocPtr++, 0, paletteAddr);
                } else {
                    tile = 0;
                    gDPLoadTLUT_pal256(gDisplayListAllocPtr++, paletteAddr);
                }
            }
        }

        gSPTextureRectangle(
            gDisplayListAllocPtr++,
            left,
            top,
            right,
            bottom,
            tile,
            clipOffsetX << 3,
            clipOffsetY << 3,
            scaleS * sprite->scaleX,
            scaleT * sprite->scaleY
        );

        gDPPipeSync(gDisplayListAllocPtr++);
        gDPSetCombineMode(gDisplayListAllocPtr++, G_CC_DECALRGBA, G_CC_DECALRGBA);
        gDPSetPrimColor(gDisplayListAllocPtr++, tile, tile, 0xFF, 0xFF, 0xFF, 0xFF);
        gDPSetRenderMode(gDisplayListAllocPtr++, G_RM_AA_TEX_TERR, G_RM_AA_TEX_TERR2);
    }
}

void renderTextSprite(TextRenderArg *sprite) {
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
    s32 clipOffsetY;
    s32 clipOffsetX;
    SpriteFrameEntry *paletteBase;
    u16 paletteMode;
    u16 format;
    u16 paletteIndex;
    u8 tileMode;
    SpriteFrameEntry *frameEntry;
    s16 scaleS;
    s16 scaleT;
    Gfx *gfx;
    u32 setOtherModeCmd;
    u32 pipeSyncCmd;

    frameEntry = sprite->spriteData->frames;
    paletteBase = &frameEntry[sprite->spriteData->numFrames];
    frameEntry = &frameEntry[sprite->frameIndex];

    if (sprite->overridePaletteCount == 0) {
        paletteIndex = frameEntry->paletteIndex;
    } else {
        paletteIndex = sprite->overridePaletteCount - 1;
    }

    tileMode = sprite->tileMode & 3;
    sprite->tileMode = tileMode;
    left = sprite->x + gTextClipAndOffsetData.offsetX;
    top = sprite->y + gTextClipAndOffsetData.offsetY;
    scaleT = gTileTextureFlipTable[tileMode * 2 + 1];
    paletteMode = gSpritePaletteModes[frameEntry->paletteTableIndex];
    format = gSpriteTextureFormats[frameEntry->formatIndex];
    scaleS = gTileTextureFlipTable[tileMode * 2];
    right = left + frameEntry->width;
    bottom = top + frameEntry->height;

    clipOffsetY = 0;
    clipOffsetX = 0;

    if (scaleS == -1) {
        clipOffsetX = frameEntry->width - 1;
    }
    if (scaleT == -1) {
        clipOffsetY = frameEntry->height - 1;
    }

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
            gCachedTextureAddr = 0;
            gCachedPaletteAddr = NULL;
            gSPDisplayList(gDisplayListAllocPtr++, gSpriteRDPSetupDL);
        }

        gfx = gDisplayListAllocPtr;
        setOtherModeCmd = 0xFC11FE23;
        pipeSyncCmd = 0xE7000000;
        gDisplayListAllocPtr = (Gfx *)((s32)gfx + 8);
        __asm__ volatile("" : : "r"(gDisplayListAllocPtr) : "memory");
        gDisplayListAllocPtr = (Gfx *)((s32)gfx + 0x10);
        *(s32 *)((s32)gfx + 0xC) = -0xC07;
        gDisplayListAllocPtr = (Gfx *)((s32)gfx + 0x18);

        gfx->words.w0 = pipeSyncCmd;
        (gfx + 0)->words.w1 = 0;
        (gfx + 1)->words.w0 = setOtherModeCmd;
        (gfx + 2)->words.w0 = 0xFA000000;

        {
            u8 alpha = sprite->alpha;
            u32 color = (alpha << 24) | (alpha << 16) | (alpha << 8) | 0xFF;
            (gfx + 2)->words.w1 = color;
        }

        if ((s32)sprite->spriteData + frameEntry->textureOffset != gCachedTextureAddr) {
            gCachedTextureAddr = (s32)sprite->spriteData + frameEntry->textureOffset;
            loadSpriteTexture(
                (s32)sprite->spriteData + frameEntry->textureOffset,
                frameEntry->width,
                frameEntry->height,
                format,
                paletteMode
            );
        }

        paletteMode &= 0xFFFF;
        if (paletteMode == 2) {
            paletteIndex &= 0xFFFF;
            if (paletteIndex == 0xFE) {
                if (gCachedPaletteAddr != (SpriteFrameEntry *)gDefaultFontPalette) {
                    gCachedPaletteAddr = (SpriteFrameEntry *)gDefaultFontPalette;
                    if (format == 0) {
                        gDPLoadTLUT_pal16(gDisplayListAllocPtr++, 0, gDefaultFontPalette);
                    } else {
                        gDPLoadTLUT_pal256(gDisplayListAllocPtr++, gDefaultFontPalette);
                    }
                }
            } else {
                SpriteFrameEntry *paletteAddr = &paletteBase[paletteIndex << 1];
                if (paletteAddr != gCachedPaletteAddr) {
                    gCachedPaletteAddr = paletteAddr;
                    if (format == 0) {
                        gDPLoadTLUT_pal16(gDisplayListAllocPtr++, 0, paletteAddr);
                    } else {
                        gDPLoadTLUT_pal256(gDisplayListAllocPtr++, paletteAddr);
                    }
                }
            }
        }

        gSPTextureRectangle(
            gDisplayListAllocPtr++,
            left * 4,
            top * 4,
            right * 4,
            bottom * 4,
            G_TX_RENDERTILE,
            clipOffsetX << 5,
            clipOffsetY << 5,
            scaleS << 10,
            scaleT << 10
        );

        gDPPipeSync(gDisplayListAllocPtr++);
        gDPSetCombineMode(gDisplayListAllocPtr++, G_CC_DECALRGBA, G_CC_DECALRGBA);
        gDPSetPrimColor(gDisplayListAllocPtr++, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
    }
}

void renderTextSpriteWithTransparency(TextRenderArg *sprite) {
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
    s32 clipOffsetY;
    s32 clipOffsetX;
    SpriteFrameEntry *paletteBase;
    u16 paletteMode;
    u16 format;
    u16 paletteIndex;
    u8 tileMode;
    SpriteFrameEntry *frameEntry;
    s16 scaleS;
    s16 scaleT;
    Gfx *gfx;
    u32 renderModeCmd;
    u32 renderModeArg;
    u32 combineCmd;
    u32 combineArg;
    u32 pipeSyncCmd;

    frameEntry = sprite->spriteData->frames;
    paletteBase = &frameEntry[sprite->spriteData->numFrames];
    frameEntry = &frameEntry[sprite->frameIndex];

    if (sprite->overridePaletteCount == 0) {
        paletteIndex = frameEntry->paletteIndex;
    } else {
        paletteIndex = sprite->overridePaletteCount - 1;
    }

    tileMode = sprite->tileMode & 3;
    sprite->tileMode = tileMode;
    left = sprite->x + gTextClipAndOffsetData.offsetX;
    top = sprite->y + gTextClipAndOffsetData.offsetY;
    scaleT = gTileTextureFlipTable[tileMode * 2 + 1];
    paletteMode = gSpritePaletteModes[frameEntry->paletteTableIndex];
    format = gSpriteTextureFormats[frameEntry->formatIndex];
    scaleS = gTileTextureFlipTable[tileMode * 2];
    right = left + frameEntry->width;
    bottom = top + frameEntry->height;

    clipOffsetY = 0;
    clipOffsetX = 0;

    if (scaleS == -1) {
        clipOffsetX = frameEntry->width - 1;
    }
    if (scaleT == -1) {
        clipOffsetY = frameEntry->height - 1;
    }

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
            gCachedTextureAddr = 0;
            gCachedPaletteAddr = NULL;
            gSPDisplayList(gDisplayListAllocPtr++, gSpriteRDPSetupDL);
        }

        renderModeCmd = 0xE200001C;
        renderModeArg = 0x504240;
        combineCmd = 0xFC119623;
        gfx = gDisplayListAllocPtr;
        combineArg = 0xFF2FFFFF;
        pipeSyncCmd = 0xE7000000;
        gDisplayListAllocPtr = (Gfx *)((s32)gfx + 8);
        __asm__ volatile(""
                         :
                         : "r"(gDisplayListAllocPtr), "r"(renderModeCmd), "r"(renderModeArg), "r"(combineArg)
                         : "memory");
        gDisplayListAllocPtr = (Gfx *)((s32)gfx + 0x10);
        __asm__ volatile("" : : "r"(gDisplayListAllocPtr) : "memory");
        gDisplayListAllocPtr = (Gfx *)((s32)gfx + 0x18);
        __asm__ volatile("" : : "r"(gDisplayListAllocPtr) : "memory");
        gDisplayListAllocPtr = (Gfx *)((s32)gfx + 0x20);

        gfx->words.w0 = pipeSyncCmd;
        (gfx + 0)->words.w1 = 0;
        (gfx + 1)->words.w0 = renderModeCmd;
        (gfx + 1)->words.w1 = renderModeArg;
        (gfx + 2)->words.w0 = combineCmd;
        (gfx + 2)->words.w1 = combineArg;
        (gfx + 3)->words.w0 = 0xFA000000;

        {
            u8 alpha = sprite->alpha;
            u32 color = (alpha << 24) | (alpha << 16) | (alpha << 8) | sprite->transparency;
            (gfx + 3)->words.w1 = color;
        }

        if ((s32)sprite->spriteData + frameEntry->textureOffset != gCachedTextureAddr) {
            gCachedTextureAddr = (s32)sprite->spriteData + frameEntry->textureOffset;
            loadSpriteTexture(
                (s32)sprite->spriteData + frameEntry->textureOffset,
                frameEntry->width,
                frameEntry->height,
                format,
                paletteMode
            );
        }

        paletteMode &= 0xFFFF;
        if (paletteMode == 2) {
            paletteIndex &= 0xFFFF;
            if (paletteIndex == 0xFE) {
                if (gCachedPaletteAddr != (SpriteFrameEntry *)gDefaultFontPalette) {
                    gCachedPaletteAddr = (SpriteFrameEntry *)gDefaultFontPalette;
                    if (format == 0) {
                        gDPLoadTLUT_pal16(gDisplayListAllocPtr++, 0, gDefaultFontPalette);
                    } else {
                        gDPLoadTLUT_pal256(gDisplayListAllocPtr++, gDefaultFontPalette);
                    }
                }
            } else {
                SpriteFrameEntry *paletteAddr = &paletteBase[paletteIndex << 1];
                if (paletteAddr != gCachedPaletteAddr) {
                    gCachedPaletteAddr = paletteAddr;
                    if (format == 0) {
                        gDPLoadTLUT_pal16(gDisplayListAllocPtr++, 0, paletteAddr);
                    } else {
                        gDPLoadTLUT_pal256(gDisplayListAllocPtr++, paletteAddr);
                    }
                }
            }
        }

        gSPTextureRectangle(
            gDisplayListAllocPtr++,
            left * 4,
            top * 4,
            right * 4,
            bottom * 4,
            G_TX_RENDERTILE,
            clipOffsetX << 5,
            clipOffsetY << 5,
            scaleS << 10,
            scaleT << 10
        );

        gDPPipeSync(gDisplayListAllocPtr++);
        gDPSetCombineMode(gDisplayListAllocPtr++, G_CC_DECALRGBA, G_CC_DECALRGBA);
        gDPSetPrimColor(gDisplayListAllocPtr++, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
    }
}

void renderTintedSprite(TintedSpriteArg *sprite) {
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
    s32 clipOffsetY;
    s32 clipOffsetX;
    SpriteFrameEntry *paletteBase;
    u16 paletteMode;
    u16 format;
    u16 paletteIndex;
    u8 tileMode;
    SpriteFrameEntry *frameEntry;
    s16 scaleS;
    s16 scaleT;

    frameEntry = sprite->spriteData->frames;
    paletteBase = &frameEntry[sprite->spriteData->numFrames];
    frameEntry = &frameEntry[sprite->frameIndex];

    if (sprite->paletteOverrideCount == 0) {
        paletteIndex = frameEntry->paletteIndex;
    } else {
        paletteIndex = sprite->paletteOverrideCount - 1;
    }

    tileMode = sprite->tileMode & 3;
    sprite->tileMode = tileMode;
    clipOffsetY = 0;
    left = sprite->x + gTextClipAndOffsetData.offsetX;
    clipOffsetX = 0;
    top = sprite->y + gTextClipAndOffsetData.offsetY;
    scaleT = gTileTextureFlipTable[tileMode * 2 + 1];
    paletteMode = gSpritePaletteModes[frameEntry->paletteTableIndex];
    format = gSpriteTextureFormats[frameEntry->formatIndex];
    scaleS = gTileTextureFlipTable[tileMode * 2];
    right = left + frameEntry->width;
    bottom = top + frameEntry->height;

    if (scaleS == -1) {
        clipOffsetX = frameEntry->width - 1;
    }
    if (scaleT == -1) {
        clipOffsetY = frameEntry->height - 1;
    }

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
            gCachedTextureAddr = 0;
            gCachedPaletteAddr = NULL;
            gSPDisplayList(gDisplayListAllocPtr++, gSpriteRDPSetupDL);
        }

        gDPPipeSync(gDisplayListAllocPtr++);
        gDPSetRenderMode(gDisplayListAllocPtr++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
        gDPSetEnvColor(gDisplayListAllocPtr++, sprite->r, sprite->g, sprite->b, sprite->a);
        gDPSetPrimColor(gDisplayListAllocPtr++, 0, 0, 0xFF, 0xFF, 0xFF, sprite->primColor);
        gDPSetCombineLERP(
            gDisplayListAllocPtr++,
            ENVIRONMENT,
            TEXEL0,
            PRIMITIVE_ALPHA,
            TEXEL0,
            TEXEL0,
            0,
            ENVIRONMENT,
            0,
            ENVIRONMENT,
            TEXEL0,
            PRIMITIVE_ALPHA,
            TEXEL0,
            TEXEL0,
            0,
            ENVIRONMENT,
            0
        );

        if ((s32)sprite->spriteData + frameEntry->textureOffset != gCachedTextureAddr) {
            gCachedTextureAddr = (s32)sprite->spriteData + frameEntry->textureOffset;
            loadSpriteTexture(
                (s32)sprite->spriteData + frameEntry->textureOffset,
                frameEntry->width,
                frameEntry->height,
                format,
                paletteMode
            );
        }

        if ((paletteMode & 0xFFFF) == 2) {
            if ((paletteIndex & 0xFFFF) == 0xFE) {
                if (gCachedPaletteAddr != (SpriteFrameEntry *)gDefaultFontPalette) {
                    gCachedPaletteAddr = (SpriteFrameEntry *)gDefaultFontPalette;
                    if (format == 0) {
                        gDPLoadTLUT_pal16(gDisplayListAllocPtr++, 0, gDefaultFontPalette);
                    } else {
                        gDPLoadTLUT_pal256(gDisplayListAllocPtr++, gDefaultFontPalette);
                    }
                }
            } else {
                SpriteFrameEntry *paletteAddr = &paletteBase[(paletteIndex & 0xFFFF) << 1];
                if (paletteAddr != gCachedPaletteAddr) {
                    gCachedPaletteAddr = paletteAddr;
                    if (format == 0) {
                        gDPLoadTLUT_pal16(gDisplayListAllocPtr++, 0, paletteAddr);
                    } else {
                        gDPLoadTLUT_pal256(gDisplayListAllocPtr++, paletteAddr);
                    }
                }
            }
        }

        gSPTextureRectangle(
            gDisplayListAllocPtr++,
            left * 4,
            top * 4,
            right * 4,
            bottom * 4,
            G_TX_RENDERTILE,
            clipOffsetX << 5,
            clipOffsetY << 5,
            scaleS << 10,
            scaleT << 10
        );

        gDPPipeSync(gDisplayListAllocPtr++);
        gDPSetCombineMode(gDisplayListAllocPtr++, G_CC_DECALRGBA, G_CC_DECALRGBA);
    }
}

void renderAlphaBlendedTextSprite(TextRenderArg *sprite) {
    s32 right;
    s32 bottom;
    s32 left;
    s32 top;
    s32 clipOffsetY;
    s32 clipOffsetX;
    SpriteFrameEntry *paletteBase;
    u16 paletteMode;
    u16 format;
    u16 paletteIndex;
    u8 tileMode;
    SpriteFrameEntry *frameEntry;
    s16 scaleS;
    s16 scaleT;

    frameEntry = sprite->spriteData->frames;
    paletteBase = &frameEntry[sprite->spriteData->numFrames];
    frameEntry = &frameEntry[sprite->frameIndex];

    if (sprite->overridePaletteCount == 0) {
        paletteIndex = frameEntry->paletteIndex;
    } else {
        paletteIndex = sprite->overridePaletteCount - 1;
    }

    tileMode = sprite->tileMode & 3;
    sprite->tileMode = tileMode;
    clipOffsetY = 0;
    left = sprite->x + gTextClipAndOffsetData.offsetX;
    top = sprite->y + gTextClipAndOffsetData.offsetY;
    scaleT = gTileTextureFlipTable[tileMode * 2 + 1];
    clipOffsetX = 0;
    paletteMode = gSpritePaletteModes[frameEntry->paletteTableIndex];
    format = gSpriteTextureFormats[frameEntry->formatIndex];
    scaleS = gTileTextureFlipTable[tileMode * 2];
    right = left + frameEntry->width;
    bottom = top + frameEntry->height;

    if (scaleS == -1) {
        clipOffsetX = frameEntry->width - 1;
    }
    if (scaleT == -1) {
        clipOffsetY = frameEntry->height - 1;
    }

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
            gCachedTextureAddr = 0;
            gCachedPaletteAddr = NULL;
            gSPDisplayList(gDisplayListAllocPtr++, gSpriteRDPSetupDL);
        }

        if ((s32)sprite->spriteData + frameEntry->textureOffset != gCachedTextureAddr) {
            gCachedTextureAddr = (s32)sprite->spriteData + frameEntry->textureOffset;
            loadSpriteTexture(
                (s32)sprite->spriteData + frameEntry->textureOffset,
                frameEntry->width,
                frameEntry->height,
                format,
                paletteMode
            );
        }

        if ((paletteMode & 0xFFFF) == 2) {
            if ((paletteIndex & 0xFFFF) == 0xFE) {
                if (gCachedPaletteAddr != (SpriteFrameEntry *)gDefaultFontPalette) {
                    gCachedPaletteAddr = (SpriteFrameEntry *)gDefaultFontPalette;
                    if (format == 0) {
                        gDPLoadTLUT_pal16(gDisplayListAllocPtr++, 0, gDefaultFontPalette);
                    } else {
                        gDPLoadTLUT_pal256(gDisplayListAllocPtr++, gDefaultFontPalette);
                    }
                }
            } else {
                gCachedPaletteAddr = NULL;
                if (format == 0) {
                    gDPLoadTLUT_pal16(gDisplayListAllocPtr++, 0, &paletteBase[(paletteIndex & 0xFFFF) << 1]);
                } else {
                    gDPLoadTLUT_pal256(gDisplayListAllocPtr++, &paletteBase[(paletteIndex & 0xFFFF) << 1]);
                }
            }
        }

        gSPTextureRectangle(
            gDisplayListAllocPtr++,
            left * 4,
            top * 4,
            right * 4,
            bottom * 4,
            G_TX_RENDERTILE,
            clipOffsetX << 5,
            clipOffsetY << 5,
            scaleS << 10,
            scaleT << 10
        );
        gDPPipeSync(gDisplayListAllocPtr++);
        gDPSetRenderMode(gDisplayListAllocPtr++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
        gDPSetCombineMode(gDisplayListAllocPtr++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
        gDPSetPrimColor(gDisplayListAllocPtr++, 0, 0, 0xFF, 0xFF, 0xFF, sprite->alpha);

        gCachedPaletteAddr = (SpriteFrameEntry *)gDefaultFontPalette;
        if (format == 0) {
            gDPLoadTLUT_pal16(gDisplayListAllocPtr++, 0, gDefaultFontPalette);
        } else {
            gDPLoadTLUT_pal256(gDisplayListAllocPtr++, gDefaultFontPalette);
        }

        gSPTextureRectangle(
            gDisplayListAllocPtr++,
            left * 4,
            top * 4,
            right * 4,
            bottom * 4,
            G_TX_RENDERTILE,
            clipOffsetX << 5,
            clipOffsetY << 5,
            scaleS << 10,
            scaleT << 10
        );
        gDPPipeSync(gDisplayListAllocPtr++);
        gDPSetCombineMode(gDisplayListAllocPtr++, G_CC_DECALRGBA, G_CC_DECALRGBA);
        gDPSetPrimColor(gDisplayListAllocPtr++, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
    }
}

void renderFlippedScaledSpriteFrame(FlippedScaledSpriteArg *sprite) {
    s32 right;
    s32 bottom;
    s32 left;
    s32 top;
    s16 clipOffsetX;
    s16 clipOffsetY;
    SpriteFrameEntry *paletteBase;
    u16 paletteMode;
    u16 format;
    u16 paletteIndex;
    SpriteFrameEntry *frameEntry;
    s16 scaleS;
    s16 scaleT;
    s16 heightScale;
    s16 widthScale;
    s32 isFlipped;
    s32 clipVal;
    s32 tile;
#ifdef CC_CHECK
    s32 clipOffsetYBase;
    s32 clipOffsetYTemp;
    s32 negOne;
#else
    register s32 clipOffsetYBase __asm__("a0");
    register s32 clipOffsetYTemp __asm__("v0");
    register s32 negOne __asm__("a2");
#endif

    frameEntry = sprite->spriteData->frames;
    paletteBase = &frameEntry[sprite->spriteData->numFrames];
    frameEntry = &frameEntry[sprite->frameIndex];

    paletteMode = gSpritePaletteModes[frameEntry->paletteTableIndex];
    format = gSpriteTextureFormats[frameEntry->formatIndex];
    scaleS = gTileTextureFlipTable[sprite->tileMode * 2];
    scaleT = gTileTextureFlipTable[sprite->tileMode * 2 + 1];

    if (sprite->overridePaletteCount == 0) {
        paletteIndex = frameEntry->paletteIndex;
    } else {
        paletteIndex = sprite->overridePaletteCount - 1;
    }

    if (sprite->scaleX > 0x7FFF || sprite->scaleX == 0) {
        return;
    }

    if (sprite->scaleY > 0x7FFF || sprite->scaleY == 0) {
        return;
    }

    sprite->tileMode = sprite->tileMode & 3;
    heightScale = (30 << 12) / sprite->scaleY;
    widthScale = (frameEntry->width << 12) / sprite->scaleX;

    left = (sprite->x * 4) - (widthScale / 2) + (gTextClipAndOffsetData.offsetX * 4);
    top = (sprite->y * 4) - (heightScale / 2) + (gTextClipAndOffsetData.offsetY * 4);
    right = left + widthScale;
    bottom = top + heightScale;

    clipOffsetX = 0;
    if (scaleS == -1) {
        clipOffsetX = frameEntry->width * 4 - 4;
    }

    clipVal = gTextClipAndOffsetData.clipLeft;
    clipVal *= 4;
    if (left < clipVal) {
        clipVal = gTextClipAndOffsetData.clipLeft;
        clipVal *= 4;
        if (scaleS == -1) {
            clipOffsetX -= clipVal - left;
        } else {
            clipOffsetX = clipVal - left;
        }
        left = gTextClipAndOffsetData.clipLeft * 4;
    }

    clipOffsetYTemp = -((s32)scaleT == -1);
    clipOffsetYBase = clipOffsetYTemp & 0x74;
    __asm("");
    clipVal = gTextClipAndOffsetData.clipTop;
    negOne = -1;
    __asm__ volatile("" : "=r"(negOne) : "0"(negOne));
    clipVal *= 4;
    if (top < clipVal) {
        clipVal = gTextClipAndOffsetData.clipTop;
        clipVal *= 4;
        if (scaleT == negOne) {
            clipOffsetY = clipOffsetYBase - (clipVal - top);
        } else {
            clipOffsetY = clipVal - top;
        }
        top = gTextClipAndOffsetData.clipTop * 4;
    } else {
        clipOffsetY = clipOffsetYBase;
    }

    isFlipped = (sprite->flipX == 1);
    clipOffsetY += isFlipped * 0x80;

    if (gTextClipAndOffsetData.clipRight * 4 >= left && (gTextClipAndOffsetData.clipBottom * 4 >= top) &&
        (left < right) && (top < bottom)) {

        if (gGraphicsMode != 0x100) {
            gGraphicsMode = 0x100;
            gCachedPaletteAddr = NULL;
            gCachedTextureAddr = 0;
            gSPDisplayList(gDisplayListAllocPtr++, gSpriteRDPSetupDL);
        }

        gDPPipeSync(gDisplayListAllocPtr++);

        if (sprite->scaleX >= 0x401 || sprite->scaleY >= 0x401) {
            gSPObjRenderMode(gDisplayListAllocPtr++, G_OBJRM_SHRINKSIZE_1 | G_OBJRM_BILERP | G_OBJRM_ANTIALIAS);
        } else {
            gSPObjRenderMode(gDisplayListAllocPtr++, G_OBJRM_BILERP | G_OBJRM_ANTIALIAS);
        }

        gDPSetCombineMode(gDisplayListAllocPtr++, G_CC_MODULATEIDECALA_PRIM, G_CC_MODULATEIDECALA_PRIM);
        gDPSetPrimColor(gDisplayListAllocPtr++, 0, 0, sprite->alpha, sprite->alpha, sprite->alpha, 0xFF);

        {
            s32 textureAddr = (s32)sprite->spriteData + frameEntry->textureOffset;
            if (textureAddr != gCachedTextureAddr) {
                gCachedTextureAddr = textureAddr;
                loadSpriteTexture(
                    (s32)sprite->spriteData + frameEntry->textureOffset,
                    frameEntry->width,
                    frameEntry->height,
                    format,
                    paletteMode
                );
            }
        }

        {
            SpriteFrameEntry *paletteAddr = &paletteBase[paletteIndex << 1];
            if (paletteAddr != gCachedPaletteAddr) {
                gCachedPaletteAddr = paletteAddr;
                if (paletteIndex == 0xFE) {
                    if (paletteAddr != (SpriteFrameEntry *)gDefaultFontPalette) {
                        gCachedPaletteAddr = (SpriteFrameEntry *)gDefaultFontPalette;
                        if (format == 0) {
                            gDPLoadTLUT_pal16(gDisplayListAllocPtr++, 0, gDefaultFontPalette);
                        } else {
                            gDPLoadTLUT_pal256(gDisplayListAllocPtr++, gDefaultFontPalette);
                        }
                    }
                } else {
                    gCachedPaletteAddr = NULL;
                    if (format == 0) {
                        gDPLoadTLUT_pal16(gDisplayListAllocPtr++, 0, paletteAddr);
                    } else {
                        tile = 0;
                        gDPLoadTLUT_pal256(gDisplayListAllocPtr++, paletteAddr);
                    }
                }
            }
        }

        gSPTextureRectangle(
            gDisplayListAllocPtr++,
            left,
            top,
            right,
            bottom,
            tile,
            clipOffsetX << 3,
            clipOffsetY << 3,
            scaleS * sprite->scaleX,
            scaleT * sprite->scaleY
        );
        gDPPipeSync(gDisplayListAllocPtr++);
        gDPSetCombineMode(gDisplayListAllocPtr++, G_CC_DECALRGBA, G_CC_DECALRGBA);
        gDPSetPrimColor(gDisplayListAllocPtr++, tile, tile, 0xFF, 0xFF, 0xFF, 0xFF);
    }
}

void loadSpriteTexture(s32 textureAddr, u16 width, u16 height, u16 format, s32 paletteMode) {
    s32 fmt;

    fmt = paletteMode;
    if (!(format & 0xFFFF)) {
        if (!(width & 0xF)) {
            gDPLoadTextureBlock_4b(
                gDisplayListAllocPtr++,
                textureAddr,
                fmt,
                width,
                height,
                0,
                G_TX_CLAMP,
                G_TX_CLAMP,
                0,
                0,
                0,
                0
            );
        } else {
            gDPSetTextureImage(gDisplayListAllocPtr++, fmt, G_IM_SIZ_8b, (width >> 1), textureAddr);
            gDPSetTile(
                gDisplayListAllocPtr++,
                fmt,
                G_IM_SIZ_8b,
                (((width >> 1) + 7) >> 3),
                0,
                G_TX_LOADTILE,
                0,
                G_TX_CLAMP,
                0,
                0,
                G_TX_CLAMP,
                0,
                0
            );
            gDPLoadSync(gDisplayListAllocPtr++);
            gDPLoadTile(
                gDisplayListAllocPtr++,
                G_TX_LOADTILE,
                0,
                0,
                ((width)-1) << 1,
                ((height)-1) << G_TEXTURE_IMAGE_FRAC
            );
            gDPPipeSync(gDisplayListAllocPtr++);
            gDPSetTile(
                gDisplayListAllocPtr++,
                fmt,
                G_IM_SIZ_4b,
                (((width >> 1) + 7) >> 3),
                0,
                G_TX_RENDERTILE,
                0,
                G_TX_CLAMP,
                0,
                0,
                G_TX_CLAMP,
                0,
                0
            );
            gDPSetTileSize(
                gDisplayListAllocPtr++,
                G_TX_RENDERTILE,
                0,
                0,
                ((width)-1) << G_TEXTURE_IMAGE_FRAC,
                ((height)-1) << G_TEXTURE_IMAGE_FRAC
            );
        }
    } else {
        if (!(width & 7)) {
            gDPLoadTextureBlock(
                gDisplayListAllocPtr++,
                textureAddr,
                fmt,
                G_IM_SIZ_8b,
                width,
                height,
                0,
                G_TX_CLAMP,
                G_TX_CLAMP,
                0,
                0,
                0,
                0
            );
        } else {
            gDPSetTextureImage(gDisplayListAllocPtr++, fmt, G_IM_SIZ_8b, width, textureAddr);
            gDPSetTile(
                gDisplayListAllocPtr++,
                fmt,
                G_IM_SIZ_8b,
                (((width) + 7) >> 3),
                0,
                G_TX_LOADTILE,
                0,
                G_TX_CLAMP,
                0,
                0,
                G_TX_CLAMP,
                0,
                0
            );
            gDPLoadSync(gDisplayListAllocPtr++);
            gDPLoadTile(
                gDisplayListAllocPtr++,
                G_TX_LOADTILE,
                0,
                0,
                ((width)-1) << G_TEXTURE_IMAGE_FRAC,
                ((height)-1) << G_TEXTURE_IMAGE_FRAC
            );
            gDPPipeSync(gDisplayListAllocPtr++);
            gDPSetTile(
                gDisplayListAllocPtr++,
                fmt,
                G_IM_SIZ_8b,
                (((width) + 7) >> 3),
                0,
                G_TX_RENDERTILE,
                0,
                G_TX_CLAMP,
                0,
                0,
                G_TX_CLAMP,
                0,
                0
            );
            gDPSetTileSize(
                gDisplayListAllocPtr++,
                G_TX_RENDERTILE,
                0,
                0,
                ((width)-1) << G_TEXTURE_IMAGE_FRAC,
                ((height)-1) << G_TEXTURE_IMAGE_FRAC
            );
        }
    }
}

void initDefaultFontPalette(void) {
    s32 i;

    gDefaultFontPalette[0] = 0;
    for (i = 1; i < 256; i++) {
        gDefaultFontPalette[i] = 0xFFFF;
    }
}
