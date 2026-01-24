#include "6DE50.h"
#include "10AD0.h"
#include "common.h"

extern s16 gGraphicsMode;
extern Gfx *gRegionAllocPtr;
extern TextClipAndOffsetData gTextClipAndOffsetData;
extern s16 gTextureEnabled;

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

INCLUDE_ASM("asm/nonmatchings/6DE50", func_8006D4B8_6E0B8);

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
    func_8006D4B8_6E0B8(asset, x, y, width, (s32)height, flag, red, 0, 0xFF, 0, green, blue, alpha);
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
