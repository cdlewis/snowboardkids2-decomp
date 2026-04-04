#include "common.h"
#include "data/data_table.h"
#include "graphics/graphics.h"
#include "graphics/sprite_rdp.h"

#include "gbi.h"

extern Gfx gFontDisplayListSetup[];

typedef struct {
    s16 x;
    s16 y;
    s16 palette;
    u8 _pad6[2];
    u8 *string;
} TextData;

void enqueueTextRender(s16 x, s16 y, s16 palette, u8 *target_string, s32 arg4, s32 arg5);

void renderTextPalette(TextData *);

void renderTextColored(TextRenderArg *arg0);

void loadFontAssetsFromDataTable(DataTable_19E80 *dataTable);