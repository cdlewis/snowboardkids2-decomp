#pragma once

#include "common.h"
#include "data/data_table.h"

#include "gbi.h"

extern Gfx gFontDisplayListSetup[];

typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ u16 palette;
    /* 0x08 */ u8 *string;
} TextData;

typedef struct {
    /* 0x00 */ u16 x;
    /* 0x02 */ u16 y;
    /* 0x04 */ u16 palette;
    /* 0x06 */ union {
        u16 value;
        struct {
            u8 padding;
            u8 shade;
        } components;
    } shade;
    /* 0x08 */ u8 *string;
} ColoredTextRenderArg;

void enqueueTextRender(s16 x, s16 y, s16 palette, u8 *target_string, s32 arg4, s32 arg5);

void renderTextPalette(TextData *);

void renderTextColored(ColoredTextRenderArg *arg0);

void loadFontAssetsFromDataTable(DataTable_19E80 *dataTable);
