#include "graphics/palette.h"
#include "common.h"
#include "data/data_table.h"

typedef struct {
    /* 0x000 */ s16 originalPalette[0x100];
    /* 0x200 */ s16 workingPalette[0x100];
} PaletteBuffer;

void initPaletteContext(PaletteContext *ctx, DataTable_19E80 *dataTable) {
    OutputStruct_19E80 output;

    getTableEntryByU16Index(dataTable, 0, &output);
    ctx->dataTable = dataTable;
    ctx->workingPalette = ((PaletteBuffer *)output.index_ptr)->workingPalette;
    ctx->originalPalette = ((PaletteBuffer *)output.index_ptr)->originalPalette;
}

void resetPaletteContext(PaletteContext *ctx) {
    memcpy(ctx->workingPalette, ctx->originalPalette, 0x200);
}

void applyPaletteShift(PaletteContext *ctx, s32 redWeight, s32 greenWeight, s32 blueWeight, s32 intensity) {
    s32 i;
    s16 color;
    s32 r;
    s32 g;
    s32 b;
    s32 avg;
    s32 divisor;
    s32 scaledB;
    s32 newR;
    s32 newG;
    s32 newB;

    for (i = 0; i < 0x100; i++) {
        color = ctx->originalPalette[i];
        divisor = ((redWeight + greenWeight + blueWeight) / 3) >> 16;
        r = (color & 0xF800) >> 8;
        g = (color & 0x7C0) >> 3;
        b = (color & 0x3E) << 2;
        avg = ((r + g) + b) / 3;

        if (divisor != 0) {
            scaledB = (blueWeight >> 16) * avg;
            newR = ((redWeight >> 16) * avg) / divisor;
            newG = ((greenWeight >> 16) * avg) / divisor;
            newB = scaledB / divisor;

            newR = r + (((newR - r) * intensity) / 6553600);
            newG = g + (((newG - g) * intensity) / 6553600);
            newB = b + (((newB - b) * intensity) / 6553600);

            if (newR >= 256) {
                newR = 255;
            }
            if (newG >= 256) {
                newG = 255;
            }
            if (newB >= 256) {
                newB = 255;
            }
            r = 0;
            if (newR < r) {
                newR = r;
            }
            if (newG < r) {
                newG = r;
            }
            if (newB < r) {
                newB = r;
            }
        } else {
            newR = r;
            newG = g;
            newB = b;
        }

        avg = ((newR & 0xF1) << 8) | ((newG & 0xF1) << 3) | ((newB & 0xF1) >> 2) | (color & 1);
        ctx->workingPalette[i] = avg;
    }
}
