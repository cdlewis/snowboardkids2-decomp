#include "credits/credits_text.h"
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

s32 D_800B0EE0_1DD080[] = {
    0x00000200, 0x001E0300, 0x00F00400, 0x010E0004, 0x01E00101, 0x023A0301, 0x02D00302, 0x03C00303, 0x04560304,
    0x050A0400, 0x05640003, 0x06360101, 0x06720305, 0x07620306, 0x08700307, 0x09600308, 0x0A140400, 0x0A6E0002,
    0x0B400101, 0x0B7C0309, 0x0C4E030A, 0x0D20030B, 0x0DF2030C, 0x0EC40400, 0x0F3C0001, 0x10680101, 0x10A4030D,
    0x1176030E, 0x1248030F, 0x12FC0400, 0x13380005, 0x13EC0101, 0x14280310, 0x14DC0311, 0x15900312, 0x16440313,
    0x16F80400, 0x17520000, 0x18420101, 0x187E0314, 0x19320315, 0x19E60400, 0x19E60200,
};

s16 D_800B0F8C_1DD12C[] = { 0x002B, 0x0000 };

s32 D_800B0F90_1DD130[] = { 0xFFF77778, 0x00088888 };

s32 D_800B0F98_1DD138[] = { 0xFFFA4FA5, 0x0005B05B };

s32 D_800B0FA0_1DD140[] = { 0xFFEEEEEF, 0x00111111, 0x00000000, 0x00000000 };

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

INCLUDE_ASM("asm/nonmatchings/credits/credits_text", func_800B0930);
