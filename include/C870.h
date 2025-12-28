#pragma once

#include "19E80.h"

typedef struct {
    DataTable_19E80 *dataTable;
    u8 *workingPalette;
    u8 *originalPalette;
} PaletteContext;

void initPaletteContext(PaletteContext *ctx, DataTable_19E80 *dataTable);
void resetPaletteContext(PaletteContext *ctx);
void applyPaletteShift(PaletteContext *ctx, s32 redWeight, s32 greenWeight, s32 blueWeight, s32 intensity);
