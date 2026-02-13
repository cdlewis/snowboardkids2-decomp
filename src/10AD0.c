#include "10AD0.h"
#include "common.h"
#include "gbi.h"
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

extern s32 gCachedPaletteAddr;
extern s32 gCachedTextureAddr;
extern s16 gGraphicsMode;
extern Gfx *gRegionAllocPtr;
extern u16 gDefaultFontPalette[];
extern TextClipAndOffsetData gTextClipAndOffsetData;

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

void renderHalfSizeSpriteWithCustomPalette(SpriteRenderArg *arg0) {
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

void renderTextSprite(TextRenderArg *arg0) {
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
    s32 clipOffsetY;
    s32 clipOffsetX;
    s32 paletteBase;
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

    frameEntry = arg0->spriteData->frames;
    paletteBase = (s32)frameEntry + (arg0->spriteData->numFrames * 0x10);
    frameEntry = &frameEntry[arg0->frameIndex];

    if (arg0->overridePaletteCount == 0) {
        paletteIndex = frameEntry->paletteIndex;
    } else {
        paletteIndex = arg0->overridePaletteCount - 1;
    }

    tileMode = arg0->tileMode & 3;
    arg0->tileMode = tileMode;
    left = arg0->x + gTextClipAndOffsetData.offsetX;
    top = arg0->y + gTextClipAndOffsetData.offsetY;
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
            gCachedPaletteAddr = 0;
            gSPDisplayList(gRegionAllocPtr++, gSpriteRDPSetupDL);
        }

        gfx = gRegionAllocPtr;
        setOtherModeCmd = 0xFC11FE23;
        pipeSyncCmd = 0xE7000000;
        gRegionAllocPtr = (Gfx *)((s32)gfx + 8);
        __asm__ volatile("" : : "r"(gRegionAllocPtr) : "memory");
        gRegionAllocPtr = (Gfx *)((s32)gfx + 0x10);
        *(s32 *)((s32)gfx + 0xC) = -0xC07;
        gRegionAllocPtr = (Gfx *)((s32)gfx + 0x18);

        gfx->words.w0 = pipeSyncCmd;
        (gfx + 0)->words.w1 = 0;
        (gfx + 1)->words.w0 = setOtherModeCmd;
        (gfx + 2)->words.w0 = 0xFA000000;

        {
            u8 alpha = arg0->alpha;
            u32 color = (alpha << 24) | (alpha << 16) | (alpha << 8) | 0xFF;
            (gfx + 2)->words.w1 = color;
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

        paletteMode &= 0xFFFF;
        if (paletteMode == 2) {
            paletteIndex &= 0xFFFF;
            if (paletteIndex == 0xFE) {
                if (gCachedPaletteAddr != (s32)gDefaultFontPalette) {
                    gCachedPaletteAddr = (s32)gDefaultFontPalette;
                    if (format == 0) {
                        gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, gDefaultFontPalette);
                    } else {
                        gDPLoadTLUT_pal256(gRegionAllocPtr++, gDefaultFontPalette);
                    }
                }
            } else {
                s32 paletteAddr = paletteBase + (paletteIndex << 5);
                if (paletteAddr != gCachedPaletteAddr) {
                    gCachedPaletteAddr = paletteAddr;
                    if (format == 0) {
                        gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, paletteAddr);
                    } else {
                        gDPLoadTLUT_pal256(gRegionAllocPtr++, paletteAddr);
                    }
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
            scaleS << 10,
            scaleT << 10
        );

        gDPPipeSync(gRegionAllocPtr++);
        gDPSetCombineMode(gRegionAllocPtr++, G_CC_DECALRGBA, G_CC_DECALRGBA);
        gDPSetPrimColor(gRegionAllocPtr++, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
    }
}

INCLUDE_ASM("asm/nonmatchings/10AD0", func_80012518_13118);

INCLUDE_ASM("asm/nonmatchings/10AD0", func_80012A58_13658);

void renderAlphaBlendedTextSprite(TextRenderArg *arg0) {
    s32 right;
    s32 bottom;
    s32 left;
    s32 top;
    s32 clipOffsetY;
    s32 clipOffsetX;
    s32 paletteBase;
    u16 paletteMode;
    u16 format;
    u16 paletteIndex;
    u8 tileMode;
    SpriteFrameEntry *frameEntry;
    s16 scaleS;
    s16 scaleT;

    frameEntry = arg0->spriteData->frames;
    paletteBase = (s32)frameEntry + (arg0->spriteData->numFrames * 0x10);
    frameEntry = &frameEntry[arg0->frameIndex];

    if (arg0->overridePaletteCount == 0) {
        paletteIndex = frameEntry->paletteIndex;
    } else {
        paletteIndex = arg0->overridePaletteCount - 1;
    }

    tileMode = arg0->tileMode & 3;
    arg0->tileMode = tileMode;
    clipOffsetY = 0;
    left = arg0->x + gTextClipAndOffsetData.offsetX;
    top = arg0->y + gTextClipAndOffsetData.offsetY;
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
            gCachedPaletteAddr = 0;
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

        if ((paletteMode & 0xFFFF) == 2) {
            if ((paletteIndex & 0xFFFF) == 0xFE) {
                if (gCachedPaletteAddr != (s32)gDefaultFontPalette) {
                    gCachedPaletteAddr = (s32)gDefaultFontPalette;
                    if (format == 0) {
                        gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, gDefaultFontPalette);
                    } else {
                        gDPLoadTLUT_pal256(gRegionAllocPtr++, gDefaultFontPalette);
                    }
                }
            } else {
                gCachedPaletteAddr = 0;
                if (format == 0) {
                    gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, paletteBase + ((paletteIndex & 0xFFFF) << 5));
                } else {
                    gDPLoadTLUT_pal256(gRegionAllocPtr++, paletteBase + ((paletteIndex & 0xFFFF) << 5));
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
            scaleS << 10,
            scaleT << 10
        );
        gDPPipeSync(gRegionAllocPtr++);
        gDPSetRenderMode(gRegionAllocPtr++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
        gDPSetCombineMode(gRegionAllocPtr++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
        gDPSetPrimColor(gRegionAllocPtr++, 0, 0, 0xFF, 0xFF, 0xFF, arg0->alpha);

        gCachedPaletteAddr = (s32)gDefaultFontPalette;
        if (format == 0) {
            gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, gDefaultFontPalette);
        } else {
            gDPLoadTLUT_pal256(gRegionAllocPtr++, gDefaultFontPalette);
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
            scaleS << 10,
            scaleT << 10
        );
        gDPPipeSync(gRegionAllocPtr++);
        gDPSetCombineMode(gRegionAllocPtr++, G_CC_DECALRGBA, G_CC_DECALRGBA);
        gDPSetPrimColor(gRegionAllocPtr++, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
    }
}

INCLUDE_ASM("asm/nonmatchings/10AD0", func_800136E0_142E0);

INCLUDE_ASM("asm/nonmatchings/10AD0", func_80013EA0_14AA0);

void initDefaultFontPalette(void) {
    s32 i;

    gDefaultFontPalette[0] = 0;
    for (i = 1; i < 256; i++) {
        gDefaultFontPalette[i] = 0xFFFF;
    }
}
