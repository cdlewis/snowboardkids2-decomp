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

INCLUDE_ASM("asm/nonmatchings/graphics/palette", applyPaletteShift);
