#include "graphics/clip_text_render.h"
#include "common.h"
#include "gbi.h"
#include "graphics/graphics.h"
#include "graphics/sprite_rdp.h"

typedef struct {
    /* 0x00 */ u16 x;
    /* 0x02 */ u16 y;
    /* 0x04 */ u16 width;
    /* 0x06 */ u16 height;
    /* 0x08 */ u8 red;
    /* 0x09 */ u8 green;
    /* 0x0A */ u8 blue;
} ColorRect;

extern s16 gGraphicsMode;
extern Gfx *gRegionAllocPtr;
extern s32 gFrameCounter;
extern TextClipAndOffsetData gTextClipAndOffsetData;
extern s16 gTextureEnabled;

void drawColorRectFill(ColorRect *rect);

void drawColorRect(ColorRect *rect) {
    s16 left, top, right, bottom;
    s32 tempTop, tempBottom;
    s32 rectRight;
    u16 offsetX;
    u16 offsetY;

    if (gTextureEnabled == FALSE) {
        drawColorRectFill(rect);
        return;
    }

    rectRight = rect->x + rect->width;

    offsetX = gTextClipAndOffsetData.offsetX;
    offsetY = gTextClipAndOffsetData.offsetY;

    left = rect->x + offsetX;
    tempTop = rect->y + offsetY;
    top = tempTop;
    right = offsetX + rectRight;
    tempBottom = offsetY + (rect->y + rect->height);
    bottom = tempBottom;

    if (left < gTextClipAndOffsetData.clipLeft) {
        left = gTextClipAndOffsetData.clipLeft;
    }

    if (top < gTextClipAndOffsetData.clipTop) {
        top = gTextClipAndOffsetData.clipTop;
    }

    if (gTextClipAndOffsetData.clipRight < right) {
        right = gTextClipAndOffsetData.clipRight;
    }

    if (gTextClipAndOffsetData.clipBottom < bottom) {
        bottom = gTextClipAndOffsetData.clipBottom;
    }

    if (left > right) {
        return;
    }

    if (top > bottom) {
        return;
    }

    gDPPipeSync(gRegionAllocPtr++);

    if (gGraphicsMode != 0) {
        gGraphicsMode = 0;
        gDPPipeSync(gRegionAllocPtr++);
        gDPSetCycleType(gRegionAllocPtr++, G_CYC_1CYCLE);
        gDPSetEnvColor(gRegionAllocPtr++, rect->red, rect->green, rect->blue, 0xFF);
        gDPSetCombineLERP(
            gRegionAllocPtr++,
            1,
            0,
            ENVIRONMENT,
            0,
            1,
            0,
            ENVIRONMENT,
            0,
            1,
            0,
            ENVIRONMENT,
            0,
            1,
            0,
            ENVIRONMENT,
            0
        );
        gDPSetRenderMode(gRegionAllocPtr++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
    }

    gDPFillRectangle(gRegionAllocPtr++, left, top, right + 1, bottom + 1);
}

void renderTintedSpriteGrid(
    SpriteSheetData *spriteData,
    s16 savedX,
    s16 savedY,
    s16 cols,
    s16 rows,
    u8 showIndicator,
    u8 intensity,
    u8 colorR,
    u8 colorG,
    u8 colorB,
    u8 colorA,
    u8 priority,
    u8 layer
) {
    s16 row;
    s16 tileX;
    s16 tileY;
    s16 col;
    s32 tileSize;

    tileX = savedX - 8;
    tileY = savedY - 8;

    if (showIndicator != 0) {
        SpriteRenderArg *cursor = (SpriteRenderArg *)advanceLinearAlloc(sizeof(SpriteRenderArg));
        if (cursor != NULL) {
            tileSize = 0x10;
            cursor->x = savedX + (cols * tileSize - tileSize);
            cursor->y = savedY + (rows * tileSize - tileSize);
            cursor->spriteData = spriteData;
            if (gFrameCounter & 8) {
                cursor->frameIndex = 5;
            } else {
                cursor->frameIndex = 6;
            }
            debugEnqueueCallback(priority, layer, (void *)renderSpriteFrame, cursor);
        }
    }

    for (row = 0; row <= rows; row++) {
        for (col = 0; col <= cols; col++) {
            TintedSpriteArg *tile = (TintedSpriteArg *)advanceLinearAlloc(sizeof(TintedSpriteArg));
            if (tile != NULL) {
                tile->tileMode = 0;
                tile->paletteOverrideCount = 0;
                tile->a = colorA;
                tile->r = (colorR * intensity) / 256;
                tile->g = (colorG * intensity) / 256;
                tile->b = (colorB * intensity) / 256;
                tile->x = tileX;
                tileX += 16;
                tile->spriteData = spriteData;
                tile->primColor = 0xFF;
                tile->frameIndex = 4;

                tile->y = tileY;
                if ((row == 0) & (col == 0)) {
                    tile->frameIndex = 0;
                }

                if ((row == 0) & (col == cols)) {
                    tile->frameIndex = 1;
                }
                if ((row == rows) & (col == 0)) {
                    tile->frameIndex = 2;
                }
                if ((row == rows) & (col == cols)) {
                    tile->frameIndex = 3;
                }
                debugEnqueueCallback(priority, layer, (void *)renderTintedSprite, tile);
            }
        }
        tileX = savedX - 8;
        tileY += 16;
    }
}

void renderTiledSprite3x3(
    void *asset,
    s16 x,
    s16 y,
    s16 width,
    s16 height,
    u8 flag,
    u8 red,
    u8 green,
    u8 blue,
    u8 alpha
) {
    renderTintedSpriteGrid(asset, x, y, width, (s32)height, flag, red, 0, 0xFF, 0, green, blue, alpha);
}

void drawColorRectFill(ColorRect *rect) {
    s16 left, top, right, bottom;
    u16 offsetX;
    u16 offsetY;
    u16 rectX;
    u16 rectY;
    Gfx *gfx;
    Gfx *tempGfx;
    s32 color;
    s32 packedColor;
    u8 r, g, b;

    rectX = rect->x;
    offsetX = gTextClipAndOffsetData.offsetX;
    rectY = rect->y;
    offsetY = gTextClipAndOffsetData.offsetY;

    left = rectX + offsetX;
    top = rectY + offsetY;
    right = offsetX + (rectX + rect->width);
    bottom = offsetY + (rectY + rect->height);

    if (left < gTextClipAndOffsetData.clipLeft) {
        left = gTextClipAndOffsetData.clipLeft;
    }

    if (top < gTextClipAndOffsetData.clipTop) {
        top = gTextClipAndOffsetData.clipTop;
    }

    if (gTextClipAndOffsetData.clipRight < right) {
        right = gTextClipAndOffsetData.clipRight;
    }

    if (gTextClipAndOffsetData.clipBottom < bottom) {
        bottom = gTextClipAndOffsetData.clipBottom;
    }

    if (left >= right) {
        return;
    }

    if (top >= bottom) {
        return;
    }

    gDPPipeSync(gRegionAllocPtr++);

    if (gGraphicsMode != 0) {
        tempGfx = gRegionAllocPtr;
        gRegionAllocPtr = tempGfx + 1;
        tempGfx[0].words.w1 = G_CYC_FILL;
        gGraphicsMode = 0;
        tempGfx[0].words.w0 = 0xE3000A01;
        gRegionAllocPtr = tempGfx + 2;
        tempGfx[1].words.w0 = 0xE200001C;
        tempGfx[1].words.w1 = 0;
    }

    gfx = gRegionAllocPtr;
    gRegionAllocPtr = gfx + 1;
    gfx->words.w0 = 0xF7000000;
    r = rect->red;
    g = rect->green;
    b = rect->blue;
    gRegionAllocPtr = gfx + 2;
    gfx[1].words.w0 = (0xF6000000 | ((right & 0x3FF) << 14) | ((bottom & 0x3FF) << 2));
    gfx[1].words.w1 = ((left & 0x3FF) << 14) | ((top & 0x3FF) << 2);

    color = ((r << 8) & 0xF800) | ((g << 3) & 0x7C0) | ((b >> 2) & 0x3E);
    packedColor = (color | 1) << 16;
    packedColor |= 1;
    packedColor |= color;
    gfx->words.w1 = packedColor;
}
