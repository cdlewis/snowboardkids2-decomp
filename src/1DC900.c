#include "1DC900.h"
#include "19E80.h"
#include "B040.h"
#include "C870.h"

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

void initCreditsScrollingTextEffects(void *state) {
    CreditsScrollerState *s = (CreditsScrollerState *)state;
    s32 i;
    void *textData;
    s16 yOffset;

    s->currentDataIndex = 0;
    s->unkD8C = 0;
    s->currentPaletteIndex = 0;
    s->unkD38 = 0;
    s->unkD8E = 0;

    for (i = 0; i < 0x12; i++) {
        s->textEntriesA[i].unk0 = 0;
        s->textEntriesA[i].y = -0x60 + i * 8;
        s->textEntriesA[i].dataTable = s->paletteDataTable[s->currentDataIndex];
        s->textEntriesA[i].index = i;
        s->textEntriesA[i].height = 0x400;
        s->textEntriesA[i].width = 0x400;
        s->textEntriesA[i].unkE = 0;
        s->textEntriesA[i].alpha = 0xFF;
        s->textEntriesA[i].unk12 = 0;
        s->textEntriesA[i].flags = 0;
        s->textEntriesA[i].unk14 = 0;
        s->textEntriesB[i].unk0 = 0;
        s->textEntriesB[i].y = -0x60 + i * 8;
        s->textEntriesB[i].dataTable = s->paletteDataTable[s->currentDataIndex];
        s->textEntriesB[i].index = i;
        s->textEntriesB[i].height = 0x400;
        s->textEntriesB[i].width = 0x400;
        s->textEntriesB[i].unkE = 0;
        s->textEntriesB[i].alpha = 0x64;
        s->textEntriesB[i].unk12 = 0;
        s->textEntriesB[i].flags = 0x11;
        s->textEntriesB[i].unk14 = 0;
    }

    for (i = 0; i < 6; i++) {
        initPaletteContext(&s->paletteContexts[i], s->paletteDataTable[i]);
        applyPaletteShift(&s->paletteContexts[i], 0xF00000, 0xB80000, 0x800000, 0x640000);
    }

    s->unkD84 = 0;
    s->unkD88 = 0;
    s->textRowIndex = 0;
    s->unkDE4 = 0;
    s->unkDE8 = 0;
    s->unkD8F = 0;

    for (i = 0; i < 8; i++) {
        textData = getTable2DEntry(s->textTable, s->textRowIndex, 0);
        s->textData[i] = textData;
        s->textXOffset[i] = -(getMaxLinePixelWidth(textData) / 2);
        s->textYOffset[i] = -0x18;
        s->textScrollOffset[i] = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/1DC900", func_800B0930);
