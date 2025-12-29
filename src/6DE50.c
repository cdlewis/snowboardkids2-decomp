#include "6DE50.h"
#include "common.h"

extern s16 gGraphicsMode;
extern Gfx *gRegionAllocPtr;

typedef struct {
    /* 0x00 */ s16 clipLeft;
    /* 0x02 */ s16 clipTop;
    /* 0x04 */ s16 clipRight;
    /* 0x06 */ s16 clipBottom;
    /* 0x08 */ s16 offsetX;
    /* 0x0A */ s16 offsetY;
} TextClipAndOffsetData;

extern TextClipAndOffsetData gTextClipAndOffsetData;
extern s16 gTextureEnabled;

typedef struct {
    /* 0x00 */ u16 x;
    /* 0x02 */ u16 y;
    /* 0x04 */ u16 width;
    /* 0x06 */ u16 height;
    /* 0x08 */ u8 red;
    /* 0x09 */ u8 green;
    /* 0x0A */ u8 blue;
} RectColorArg;
void func_8006D820_6E420(RectColorArg *arg0);

void func_8006D250_6DE50(RectColorArg *arg0) {
    s16 left, top, right, bottom;
    s32 tempTop, tempBottom;
    u16 temp_a3;
    u16 temp_a2;
    s32 new_var2;

    if (gTextureEnabled == FALSE) {
        func_8006D820_6E420(arg0);
        return;
    }

    new_var2 = arg0->x + arg0->width;

    temp_a3 = gTextClipAndOffsetData.offsetX;
    temp_a2 = gTextClipAndOffsetData.offsetY;

    left = arg0->x + temp_a3;
    tempTop = arg0->y + temp_a2;
    top = tempTop;
    right = temp_a3 + new_var2;
    tempBottom = temp_a2 + (arg0->y + arg0->height);
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
        gDPSetEnvColor(gRegionAllocPtr++, arg0->red, arg0->green, arg0->blue, 0xFF);
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

void func_8006D7B0_6E3B0(
    void *arg0,
    s16 arg1,
    s16 arg2,
    s16 arg3,
    s16 arg4,
    u8 arg5,
    u8 arg6,
    u8 arg7,
    u8 arg8,
    u8 arg9
) {
    func_8006D4B8_6E0B8(arg0, arg1, arg2, arg3, (s32)arg4, arg5, arg6, 0, 0xFF, 0, arg7, arg8, arg9);
}

void func_8006D820_6E420(RectColorArg *arg0) {
    s16 left, top, right, bottom;
    u16 temp_a3;
    u16 temp_a2;
    u16 temp_x, temp_y;
    Gfx *gfx;
    Gfx *temp_gfx;
    s32 color;
    s32 result;
    u8 r, g, b;

    temp_x = arg0->x;
    temp_a2 = gTextClipAndOffsetData.offsetX;
    temp_y = arg0->y;
    temp_a3 = gTextClipAndOffsetData.offsetY;

    left = temp_x + temp_a2;
    top = temp_y + temp_a3;
    right = temp_a2 + (temp_x + arg0->width);
    bottom = temp_a3 + (temp_y + arg0->height);

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
        temp_gfx = gRegionAllocPtr;
        gRegionAllocPtr = temp_gfx + 1;
        temp_gfx[0].words.w1 = G_CYC_FILL;
        gGraphicsMode = 0;
        temp_gfx[0].words.w0 = 0xE3000A01;
        gRegionAllocPtr = temp_gfx + 2;
        temp_gfx[1].words.w0 = 0xE200001C;
        temp_gfx[1].words.w1 = 0;
    }

    gfx = gRegionAllocPtr;
    gRegionAllocPtr = gfx + 1;
    gfx->words.w0 = 0xF7000000;
    r = arg0->red;
    g = arg0->green;
    b = arg0->blue;
    gRegionAllocPtr = gfx + 2;
    gfx[1].words.w0 = (0xF6000000 | ((right & 0x3FF) << 14) | ((bottom & 0x3FF) << 2));
    gfx[1].words.w1 = ((left & 0x3FF) << 14) | ((top & 0x3FF) << 2);

    color = ((r << 8) & 0xF800) | ((g << 3) & 0x7C0) | ((b >> 2) & 0x3E);
    result = (color | 1) << 16;
    result |= 1;
    result |= color;
    gfx->words.w1 = result;
}
