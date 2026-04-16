#include "common.h"
#include "data/data_table.h"
#include "graphics/graphics.h"
#include "graphics/sprite_rdp.h"

#include "gbi.h"

extern Gfx gFontDisplayListSetup[];

typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ u16 palette;
    /* 0x08 */ u8 *string;
} TextData;

void enqueueTextRender(s16 x, s16 y, s16 palette, u8 *target_string, s32 arg4, s32 arg5);

void renderTextPalette(TextData *);

void renderTextColored(TextRenderArg *arg0);

void loadFontAssetsFromDataTable(DataTable_19E80 *dataTable);