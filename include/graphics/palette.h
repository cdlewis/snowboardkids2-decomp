#pragma once

#include "data/data_table.h"

typedef struct {
    DataTable_19E80 *dataTable;
    s16 *workingPalette;
    s16 *originalPalette;
} PaletteContext;

void initPaletteContext(PaletteContext *ctx, DataTable_19E80 *dataTable);
void resetPaletteContext(PaletteContext *ctx);
void applyPaletteShift(PaletteContext *ctx, s32 redWeight, s32 greenWeight, s32 blueWeight, s32 intensity);
