#include "text/text_layout.h"
#include "common.h"
#include "gbi.h"
#include "graphics/graphics.h"
#include "graphics/sprite_rdp.h"
#include "gs2dex.h"
#include "system/task_scheduler.h"

typedef struct {
    s16 x;
    s16 y;
    /* 0x04 */ void *spriteData;
    /* 0x08 */ s16 frameIndex;
    /* 0x0A */ s16 alpha;
    /* 0x0C */ u8 tileMode;
    /* 0x0D */ u8 paletteIndex;
    /* 0x0E */ u8 transparency;
} TextElementState;

void initHudElementState(TextElementState *arg0);
void renderShadedTextSprite(s32, s32, u16, u16, u16, u16, SpriteSheetData *);

extern SpriteFrameEntry *gCachedPaletteAddr;
extern s32 gCachedTextureAddr;
extern s16 gGraphicsMode;
extern Gfx *gDisplayListAllocPtr;
extern u16 gDefaultFontPalette[];
extern Gfx gSpriteRDPSetupDL[];

void enqueueTextLayout(
    void *fontAsset,
    void *textData,
    s16 startX,
    s16 startY,
    u8 alpha,
    u8 transparency,
    u8 paletteIndex,
    u8 priority,
    u8 flags
) {
    s16 x = startX;
    s16 y = startY;
    u16 palette = paletteIndex;
    TextElementState *elem;
    u16 *ptr = (u16 *)textData;

    while ((*ptr) != 0xFFFF) {
        if (*ptr == 0xFFFD) {
            x = startX;
            y += 16;
        } else if (*ptr == 0xFFFE || *ptr == 0xFFFB) {
            if (*ptr == 0xFFFE) {
                x += 4;
            } else {
                x += 4;
            }
        } else if (*ptr == 0xFFFC) {
            ptr++;
            if (paletteIndex == 0) {
                palette = *ptr;
            }
        } else if (*ptr == 0xFFF0) {
            ptr += 3;
        } else if (*ptr != 0xFFF1) {
            u16 cmd = *ptr;
            s16 width = ((cmd & 0xF000) >> 12);
            if (width == 0) {
                width = 12;
            }

            elem = advanceLinearAlloc(sizeof(TextElementState));
            if (elem != NULL) {
                initHudElementState(elem);
                elem->transparency = transparency;
                elem->alpha = alpha;
                elem->spriteData = fontAsset;
                elem->paletteIndex = palette + 1;
                elem->x = x;
                elem->y = y;
                elem->frameIndex = cmd & 0xFFF;
                enqueueCallbackBySlotIndex(priority, flags, renderTextSpriteWithTransparency, elem);
            }
            x += width;
        }
        ptr++;
    }
}

void renderTextLayout(TextLayoutArg *arg0) {
    u16 *ptr;
    s16 x;
    s16 y;
    u16 paletteOverride;
    s16 width;

    ptr = arg0->textData;
    x = arg0->startX;
    y = arg0->startY;
    paletteOverride = arg0->paletteIndex;

    while (*ptr != 0xFFFF) {
        if (*ptr == 0xFFFD) {
            x = arg0->startX;
            y += 16;
        } else if ((*ptr == 0xFFFE) || (*ptr == 0xFFFB)) {
            if (*ptr == 0xFFFE) {
                x += 4;
            } else {
                x += 4;
            }
        } else if (*ptr == 0xFFFC) {
            ptr++;
            if (arg0->paletteIndex == 0) {
                paletteOverride = *ptr;
            }
        } else if (*ptr == 0xFFF0) {
            ptr += 3;
        } else {
            if (*ptr != 0xFFF1) {
                width = (*ptr & 0xF000) >> 12;
                if (width == 0) {
                    width = 12;
                }
                renderShadedTextSprite(
                    x,
                    y,
                    *ptr & 0xFFF,
                    arg0->shade,
                    arg0->textAlpha,
                    paletteOverride,
                    arg0->fontAsset
                );
                x += width;
            }
        }

        ptr++;
    }
}

void renderTextLayoutCapped(TextLayoutCappedArg *arg0) {
    u16 *ptr;
    s16 x;
    s16 y;
    u16 paletteOverride;
    s16 width;
    s32 iterationCount;
    u16 cmd;

    ptr = arg0->textData;
    x = arg0->startX;
    y = arg0->startY;
    cmd = *ptr;
    paletteOverride = arg0->paletteIndex;

    if (cmd != 0xFFFF) {
        iterationCount = 0;
        do {
            if ((u8)iterationCount >= arg0->maxIterations) {
                return;
            }
            if (cmd == 0xFFFD) {
                x = arg0->startX;
                y += 16;
            } else if ((cmd == 0xFFFE) || (cmd == 0xFFFB)) {
                if (cmd == 0xFFFE) {
                    x += 4;
                } else {
                    x += 4;
                }
            } else if (cmd == 0xFFFC) {
                ptr++;
                iterationCount++;
                if (arg0->paletteIndex == 0) {
                    paletteOverride = *ptr;
                }
            } else if (cmd == 0xFFF0) {
                ptr += 3;
                iterationCount += 3;
            } else {
                if (cmd != 0xFFF1) {
                    width = (cmd & 0xF000) >> 12;
                    if (width == 0) {
                        width = 12;
                    }
                    renderShadedTextSprite(
                        x,
                        y,
                        cmd & 0xFFF,
                        arg0->shade,
                        arg0->textAlpha,
                        paletteOverride,
                        arg0->fontAsset
                    );
                    x += width;
                }
            }
            ptr++;
            cmd = *ptr;
            iterationCount++;
        } while (cmd != 0xFFFF);
    }
}

void enqueueTextLayoutCapped(
    void *fontAsset,
    u16 *textData,
    u16 startX,
    s16 startY,
    u8 alpha,
    u8 transparency,
    u8 paletteIndex,
    u8 maxIterations,
    u8 priority,
    u8 flags
) {
    s16 x = startX;
    s16 y = startY;
    u16 palette = paletteIndex;
    TextElementState *elem;
    u16 *ptr = textData;
    u16 cmd;
    s32 iterationCount;
    s16 width;

    cmd = *ptr;
    iterationCount = 0;

    while (cmd != 0xFFFF) {
        if ((u8)iterationCount >= maxIterations) {
            return;
        }
        if (cmd == 0xFFFD) {
            x = startX;
            y += 0x10;
        } else if ((cmd == 0xFFFE) || (cmd == 0xFFFB)) {
            if (cmd == 0xFFFE) {
                x += 4;
            } else {
                x += 4;
            }
        } else if (cmd == 0xFFFC) {
            ptr++;
            iterationCount++;
            if (paletteIndex == 0) {
                palette = *ptr;
            }
        } else if (cmd == 0xFFF0) {
            ptr += 3;
            iterationCount += 3;
        } else if (cmd != 0xFFF1) {
            width = (cmd & 0xF000) >> 12;
            if (width == 0) {
                width = 0xC;
            }
            elem = advanceLinearAlloc(sizeof(TextElementState));
            if (elem != NULL) {
                initHudElementState(elem);
                elem->transparency = transparency;
                elem->alpha = alpha;
                elem->spriteData = fontAsset;
                elem->paletteIndex = palette + 1;
                elem->x = x;
                elem->y = y;
                elem->frameIndex = cmd & 0xFFF;
                enqueueCallbackBySlotIndex(priority, flags, renderTextSpriteWithTransparency, elem);
            }
            x += width;
        }
        ptr++;
        cmd = *ptr;
        iterationCount++;
    }
}

void renderShadedTextSprite(
    s32 x,
    s32 y,
    u16 frameIndex,
    u16 shade,
    u16 alpha,
    u16 paletteOverride,
    SpriteSheetData *spriteData
) {
    s16 left;
    s16 top;
    s16 right;
    s16 bottom;
    SpriteFrameEntry *paletteBase;
    SpriteFrameEntry *paletteAddr;
    SpriteFrameEntry *frameEntry;
    Gfx *gfx;
    u32 renderModeCmd;
    u32 renderModeArg;
    u32 combineCmd;
    u32 combineArg;
#ifdef CC_CHECK
    u32 pipeSyncCmd;
#else
    register u32 pipeSyncCmd __asm__("$10");
#endif

    frameEntry = spriteData->frames;
    left = (u16)gTextClipAndOffsetData.offsetX + x;
    paletteBase = &frameEntry[spriteData->numFrames];
    frameEntry = &frameEntry[frameIndex];
    top = (u16)gTextClipAndOffsetData.offsetY + y;
    right = left + frameEntry->width;
    bottom = top + frameEntry->height;

    if ((gTextClipAndOffsetData.clipRight >= left) && (!(gTextClipAndOffsetData.clipBottom < top)) && (left < right) &&
        (top < bottom)) {

        if (gGraphicsMode != 0x100) {
            gGraphicsMode = 0x100;
            gCachedPaletteAddr = NULL;
            gCachedTextureAddr = 0;
            gSPDisplayList(gDisplayListAllocPtr++, gSpriteRDPSetupDL);
        }

        renderModeCmd = 0xE200001C;
        renderModeArg = 0x504240;
        combineCmd = 0xFC119623;
        gfx = gDisplayListAllocPtr;
        combineArg = 0xFF2FFFFF;
        pipeSyncCmd = 0xE7000000;
        gDisplayListAllocPtr = (Gfx *)((s32)gfx + 8);
        __asm__ volatile("" ::: "memory");
        gDisplayListAllocPtr = (Gfx *)((s32)gfx + 0x10);
        __asm__ volatile("" ::: "memory");
        gDisplayListAllocPtr = (Gfx *)((s32)gfx + 0x18);
        (gfx + 2)->words.w0 = 0x0B000000;
        (gfx + 2)->words.w1 = 0x0E;
        gDisplayListAllocPtr = (Gfx *)((s32)gfx + 0x20);
        __asm__ volatile("" ::: "memory");
        gDisplayListAllocPtr = (Gfx *)((s32)gfx + 0x28);
        (gfx + 1)->words.w1 = renderModeArg;

        {
            u8 shadeByte = shade;
            (gfx + 4)->words.w0 = 0xFA000000;
            (gfx + 1)->words.w0 = renderModeCmd;
            (gfx + 4)->words.w1 = (shadeByte << 24) | (shadeByte << 16) | (shadeByte << 8) | (alpha & 0xFF);
        }

        gfx->words.w0 = pipeSyncCmd;
        gfx->words.w1 = 0;
        (gfx + 3)->words.w0 = combineCmd;
        (gfx + 3)->words.w1 = combineArg;

        {
            u16 paletteIndex = paletteOverride & 0xFFFF;
            if (paletteIndex == 0xFF) {
                paletteAddr = (SpriteFrameEntry *)gDefaultFontPalette;
                if (gCachedPaletteAddr == (SpriteFrameEntry *)gDefaultFontPalette) {
                    goto skipPalette;
                }
            } else {
                paletteAddr = &paletteBase[paletteIndex << 1];
                if (paletteAddr == gCachedPaletteAddr) {
                    goto skipPalette;
                }
            }

            gDisplayListAllocPtr = (Gfx *)((s32)gfx + 0x30);
            (gfx + 5)->words.w0 = 0xFD100000;
            gDisplayListAllocPtr = (Gfx *)((s32)gfx + 0x38);
            (gfx + 6)->words.w0 = 0xE8000000;
            gDisplayListAllocPtr = (Gfx *)((s32)gfx + 0x40);
            (gfx + 7)->words.w1 = 0x07000000;
            gDisplayListAllocPtr = (Gfx *)((s32)gfx + 0x48);
            (gfx + 8)->words.w0 = 0xE6000000;
            gDisplayListAllocPtr = (Gfx *)((s32)gfx + 0x50);
            (gfx + 9)->words.w0 = 0xF0000000;
            gCachedPaletteAddr = paletteAddr;
            (gfx + 5)->words.w1 = (s32)paletteAddr;
            (gfx + 6)->words.w1 = 0;
            (gfx + 7)->words.w0 = 0xF5000100;
            (gfx + 8)->words.w1 = 0;
            (gfx + 9)->words.w1 = 0x0703C000;
            gDisplayListAllocPtr = (Gfx *)((s32)gfx + 0x58);
            (gfx + 10)->words.w0 = pipeSyncCmd;
            (gfx + 10)->words.w1 = 0;
        }
    skipPalette:

        if ((s32)spriteData + frameEntry->textureOffset != gCachedTextureAddr) {
            gCachedTextureAddr = (s32)spriteData + frameEntry->textureOffset;
            gDPSetTextureImage(
                gDisplayListAllocPtr++,
                G_IM_FMT_CI,
                G_IM_SIZ_8b,
                frameEntry->width >> 1,
                (s32)spriteData + frameEntry->textureOffset
            );
            gDPSetTile(
                gDisplayListAllocPtr++,
                G_IM_FMT_CI,
                G_IM_SIZ_8b,
                ((frameEntry->width >> 1) + 7) >> 3,
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
                (frameEntry->width - 1) << 1,
                (frameEntry->height - 1) << G_TEXTURE_IMAGE_FRAC
            );
            gDPPipeSync(gDisplayListAllocPtr++);
            gDPSetTile(
                gDisplayListAllocPtr++,
                G_IM_FMT_CI,
                G_IM_SIZ_4b,
                ((frameEntry->width >> 1) + 7) >> 3,
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
                (frameEntry->width - 1) << G_TEXTURE_IMAGE_FRAC,
                (frameEntry->height - 1) << G_TEXTURE_IMAGE_FRAC
            );
        }

        gSPTextureRectangle(
            gDisplayListAllocPtr++,
            left << 2,
            top << 2,
            right << 2,
            bottom << 2,
            G_TX_RENDERTILE,
            0,
            0,
            0x0400,
            0x0400
        );
        gDPPipeSync(gDisplayListAllocPtr++);
        gDPSetRenderMode(gDisplayListAllocPtr++, G_RM_AA_TEX_EDGE, G_RM_AA_TEX_EDGE2);
        gDPSetCombineMode(gDisplayListAllocPtr++, G_CC_DECALRGBA, G_CC_DECALRGBA);
        gSPObjRenderMode(gDisplayListAllocPtr++, G_OBJRM_BILERP | G_OBJRM_ANTIALIAS);
        gDPSetPrimColor(gDisplayListAllocPtr++, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
    }
}

void enqueueTextLayoutAlphaBlended(
    void *fontAsset,
    void *textData,
    s16 startX,
    s16 startY,
    u8 alpha,
    u8 paletteIndex,
    u8 priority,
    u8 flags
) {
    s16 x = startX;
    s16 y = startY;
    u16 palette = paletteIndex & 0xFF;
    TextElementState *elem;
    u16 *ptr = (u16 *)textData;

    while ((*ptr) != 0xFFFF) {
        if (*ptr == 0xFFFD) {
            x = startX;
            y += 16;
        } else if (*ptr == 0xFFFE || *ptr == 0xFFFB) {
            if (*ptr == 0xFFFE) {
                x += 4;
            } else {
                x += 4;
            }
        } else if (*ptr == 0xFFFC) {
            ptr++;
            if (paletteIndex == 0) {
                palette = *ptr;
            }
        } else if (*ptr == 0xFFF0) {
            ptr += 3;
        } else if (*ptr != 0xFFF1) {
            u16 cmd = *ptr;
            s16 width = ((cmd & 0xF000) >> 12);
            if (width == 0) {
                width = 12;
            }

            elem = advanceLinearAlloc(sizeof(TextElementState));
            if (elem != NULL) {
                initHudElementState(elem);
                elem->alpha = alpha;
                elem->spriteData = fontAsset;
                elem->paletteIndex = palette + 1;
                elem->x = x;
                elem->y = y;
                elem->frameIndex = cmd & 0xFFF;
                enqueueCallbackBySlotIndex(priority, flags, renderAlphaBlendedTextSprite, elem);
            }
            x += width;
        }
        ptr++;
    }
}
