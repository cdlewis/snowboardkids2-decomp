#include "C870.h"
#include "19E80.h"
#include "common.h"

typedef struct {
    /* 0x000 */ u8 originalPalette[0x200];
    /* 0x200 */ u8 workingPalette[0x200];
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

INCLUDE_ASM("asm/nonmatchings/C870", applyPaletteShift);
