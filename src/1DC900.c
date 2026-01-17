#include "19E80.h"
#include "B040.h"
#include "C870.h"
#include "common.h"

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
    u8 pad0[0x968];
    Table_B934 *textTable;
    u8 pad96C[0x9B8 - 0x96C];
    DataTable_19E80 *paletteDataTable[6];
    CreditsTextEntry textEntriesA[18];
    CreditsTextEntry textEntriesB[18];
    s32 currentPaletteIndex;
    u8 padD34[0xD38 - 0xD34];
    s16 unkD38;
    s16 currentDataIndex;
    PaletteContext paletteContexts[6];
    s32 unkD84;
    s32 unkD88;
    s16 unkD8C;
    u8 unkD8E;
    u8 unkD8F;
    void *textData[8];
    s16 textXOffset[8];
    s16 textYOffset[8];
    s16 textScrollOffset[8];
    s16 textRowIndex;
    u8 padDE2[2];
    s32 unkDE4;
    s32 unkDE8;
} CreditsScrollerState;

void initCreditsScrollingTextEffects(CreditsScrollerState *state) {
    s32 i;
    void *textData;
    s16 yOffset;

    state->currentDataIndex = 0;
    state->unkD8C = 0;
    state->currentPaletteIndex = 0;
    state->unkD38 = 0;
    state->unkD8E = 0;

    for (i = 0; i < 0x12; i++) {
        state->textEntriesA[i].unk0 = 0;
        state->textEntriesA[i].y = -0x60 + i * 8;
        state->textEntriesA[i].dataTable = state->paletteDataTable[state->currentDataIndex];
        state->textEntriesA[i].index = i;
        state->textEntriesA[i].height = 0x400;
        state->textEntriesA[i].width = 0x400;
        state->textEntriesA[i].unkE = 0;
        state->textEntriesA[i].alpha = 0xFF;
        state->textEntriesA[i].unk12 = 0;
        state->textEntriesA[i].flags = 0;
        state->textEntriesA[i].unk14 = 0;
        state->textEntriesB[i].unk0 = 0;
        state->textEntriesB[i].y = -0x60 + i * 8;
        state->textEntriesB[i].dataTable = state->paletteDataTable[state->currentDataIndex];
        state->textEntriesB[i].index = i;
        state->textEntriesB[i].height = 0x400;
        state->textEntriesB[i].width = 0x400;
        state->textEntriesB[i].unkE = 0;
        state->textEntriesB[i].alpha = 0x64;
        state->textEntriesB[i].unk12 = 0;
        state->textEntriesB[i].flags = 0x11;
        state->textEntriesB[i].unk14 = 0;
    }

    for (i = 0; i < 6; i++) {
        initPaletteContext(&state->paletteContexts[i], state->paletteDataTable[i]);
        applyPaletteShift(&state->paletteContexts[i], 0xF00000, 0xB80000, 0x800000, 0x640000);
    }

    state->unkD84 = 0;
    state->unkD88 = 0;
    state->textRowIndex = 0;
    state->unkDE4 = 0;
    state->unkDE8 = 0;
    state->unkD8F = 0;

    for (i = 0; i < 8; i++) {
        textData = getTable2DEntry(state->textTable, state->textRowIndex, 0);
        state->textData[i] = textData;
        state->textXOffset[i] = -(getMaxLinePixelWidth(textData) / 2);
        state->textYOffset[i] = -0x18;
        state->textScrollOffset[i] = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/1DC900", func_800B0930);
