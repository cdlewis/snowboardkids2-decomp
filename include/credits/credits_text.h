#ifndef _1DC900_H_
#define _1DC900_H_

#include "animation/easing_state.h"
#include "data/data_table.h"
#include "graphics/palette.h"

typedef struct {
    s16 unk0;
    s16 y;
    void *dataTable;
    s16 index;
    s16 width;
    s16 height;
    s16 unkE;
    s16 alpha;
    u8 unk12;
    u8 flags;
    u8 unk14;
    u8 pad15[3];
} CreditsTextEntry;

typedef struct {
    u8 pad0[2];
    s16 unk2;
    u8 pad4[0x960 - 4];
    void *textAsset;
    u8 pad964[4];
    Table_B934 *textTable;
    u8 pad96C[0x9B8 - 0x96C];
    DataTable_19E80 *paletteDataTable[6];
    CreditsTextEntry textEntriesA[18];
    CreditsTextEntry textEntriesB[18];
    s32 currentPaletteIndex;
    s32 unkD34;
    s16 unkD38;
    s16 currentDataIndex;
    PaletteContext paletteContexts[6];
    s32 unkD84;
    s32 unkD88;
    s16 unkD8C;
    s8 unkD8E;
    s8 unkD8F;
    void *textData[8];
    s16 textXOffset[8];
    s16 textYOffset[8];
    s16 textScrollOffset[8];
    s16 textRowIndex;
    u8 padDE2[2];
    s32 unkDE4;
    s32 unkDE8;
} CreditsScrollerState;

void initCreditsScrollingTextEffects(void *state);
void func_800B0930(CreditsScrollerState *s);

#endif
