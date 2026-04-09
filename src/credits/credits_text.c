#include "credits/credits_text.h"
#include "animation/easing_state.h"
#include "data/data_table.h"
#include "graphics/graphics.h"
#include "graphics/palette.h"

typedef struct {
    u16 frame;
    s8 command;
    u8 param;
} CreditsCommand;

CreditsCommand D_800B0EE0_1DD080[] = {
    { 0x0000, 0x02, 0x00 },
    { 0x001E, 0x03, 0x00 },
    { 0x00F0, 0x04, 0x00 },
    { 0x010E, 0x00, 0x04 },
    { 0x01E0, 0x01, 0x01 },
    { 0x023A, 0x03, 0x01 },
    { 0x02D0, 0x03, 0x02 },
    { 0x03C0, 0x03, 0x03 },
    { 0x0456, 0x03, 0x04 },
    { 0x050A, 0x04, 0x00 },
    { 0x0564, 0x00, 0x03 },
    { 0x0636, 0x01, 0x01 },
    { 0x0672, 0x03, 0x05 },
    { 0x0762, 0x03, 0x06 },
    { 0x0870, 0x03, 0x07 },
    { 0x0960, 0x03, 0x08 },
    { 0x0A14, 0x04, 0x00 },
    { 0x0A6E, 0x00, 0x02 },
    { 0x0B40, 0x01, 0x01 },
    { 0x0B7C, 0x03, 0x09 },
    { 0x0C4E, 0x03, 0x0A },
    { 0x0D20, 0x03, 0x0B },
    { 0x0DF2, 0x03, 0x0C },
    { 0x0EC4, 0x04, 0x00 },
    { 0x0F3C, 0x00, 0x01 },
    { 0x1068, 0x01, 0x01 },
    { 0x10A4, 0x03, 0x0D },
    { 0x1176, 0x03, 0x0E },
    { 0x1248, 0x03, 0x0F },
    { 0x12FC, 0x04, 0x00 },
    { 0x1338, 0x00, 0x05 },
    { 0x13EC, 0x01, 0x01 },
    { 0x1428, 0x03, 0x10 },
    { 0x14DC, 0x03, 0x11 },
    { 0x1590, 0x03, 0x12 },
    { 0x1644, 0x03, 0x13 },
    { 0x16F8, 0x04, 0x00 },
    { 0x1752, 0x00, 0x00 },
    { 0x1842, 0x01, 0x01 },
    { 0x187E, 0x03, 0x14 },
    { 0x1932, 0x03, 0x15 },
    { 0x19E6, 0x04, 0x00 },
    { 0x19E6, 0x02, 0x00 },
};

s16 D_800B0F8C_1DD12C = 0x002B;

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

extern void func_80011924_12524(void);
extern void
func_80035260_35E60(void *arg0, void *arg1, s16 arg2, s16 arg3, u8 arg4, u8 arg5, u8 arg6, u8 arg7, u8 arg8);

void func_800B0930(CreditsScrollerState *s) {
    s32 showPalette;
    s32 i;
    s32 scaledAlpha;
    CreditsCommand *cmd;
    s32 yAccum;
    s32 alphaSum;
    s16 initialFrame;
    s32 height;

    showPalette = 1;
    while (s->unk2 == s->unkD38) {
        if (s->unkD8C >= D_800B0F8C_1DD12C) {
            break;
        }
        cmd = &D_800B0EE0_1DD080[s->unkD8C];
        switch (cmd->command) {
            case 0:
                s->currentDataIndex = (s8)cmd->param;
                s->unkD8E = 1;
                s->unkD34 = D_800B0F90_1DD130[0];
                break;
            case 1:
                s->unkD88 = D_800B0F98_1DD138[(s8)cmd->param];
                break;
            case 2:
                s->unkD34 = D_800B0F90_1DD130[(s8)cmd->param];
                s->unkD8E = 0;
                break;
            case 3:
                s->textRowIndex = (s8)cmd->param;
                s->unkDE8 = D_800B0FA0_1DD140[0];
                s->unkD8F = 1;
                break;
            case 4:
                s->unkDE8 = D_800B0FA0_1DD140[(s8)cmd->param];
                break;
        }

        s->unkD8C++;
        if (s->unkD8C >= D_800B0F8C_1DD12C) {
            break;
        }

        cmd = &D_800B0EE0_1DD080[s->unkD8C];
        s->unkD38 = cmd->frame;
    }

    do {
        s32 alphaSum = s->currentPaletteIndex + s->unkD34;
        s->currentPaletteIndex = alphaSum;
        if (alphaSum > 0xFF0000) {
            s->currentPaletteIndex = 0xFF0000;
        } else if (alphaSum < 0) {
            s->currentPaletteIndex = 0;
            if (s->unkD8E != 0) {
                for (i = 0; i < 0x12; i++) {
                    s->textEntriesA[i].dataTable = s->paletteDataTable[s->currentDataIndex];
                    s->textEntriesB[i].dataTable = s->paletteDataTable[s->currentDataIndex];
                }
                s->unkD34 = D_800B0F90_1DD130[1];
                s->unkD8E = 0;
                s->unkD84 = 0;
                s->unkD88 = 0;
            }
        }
    } while (0);

    if (s->currentPaletteIndex >> 16) {
        s->unkD84 += s->unkD88;
        if (s->unkD84 > 0xFF0000) {
            s->unkD84 = 0xFF0000;
        }
        if (s->unkD84 < 0) {
            s->unkD84 = 0;
        }

        if (s->unkD84 != 0) {
            scaledAlpha = (((s->currentPaletteIndex >> 16) * (s->unkD84 >> 8)) / 255) << 8;
            if (scaledAlpha > 0xFF0000) {
                scaledAlpha = 0xFF0000;
            }
            scaledAlpha &= (~scaledAlpha >> 31);
        }

        for (i = 0; i < 0x12; i++) {
            if (scaledAlpha != 0) {
                s->textEntriesB[i].unk14 = scaledAlpha >> 16;
                debugEnqueueCallback(0, 2, func_80011924_12524, &s->textEntriesB[i]);
            }
            if (s->unkD34 < 0) {
                if (s->unkD84 == 0xFF0000) {
                    showPalette = 0;
                }
            }
            if (showPalette != 0) {
                s->textEntriesA[i].unk14 = s->currentPaletteIndex >> 16;
                debugEnqueueCallback(0, 2, func_80011924_12524, &s->textEntriesA[i]);
            }
        }
    }

    do {
        alphaSum = s->unkDE4 + s->unkDE8;
        s->unkDE4 = alphaSum;
        if (alphaSum > 0xFF0000) {
            s->unkDE4 = 0xFF0000;
        } else if (alphaSum < 0) {
            s->unkDE4 = 0;
            if (s->unkD8F != 0) {
                for (i = 0, yAccum = -0x18; i < 8; i++) {
                    s->textData[i] = getTable2DEntry(s->textTable, s->textRowIndex, i);
                    if (!s->textData[i]) {
                        break;
                    }
                    s->textXOffset[i] = -(getMaxLinePixelWidth(s->textData[i]) / 2);
                    height = getTable2DRowCount(s->textTable, s->textRowIndex, i) * 8 + 8;
                    s->textYOffset[i] = (yAccum + (i * 0x10)) - height;
                    s->unkDE8 = D_800B0F90_1DD130[1];
                }
                s->unkD8F = 0;
            }
        }
    } while (0);

    if (s->unkDE4 >> 16) {
        for (i = 0; i < 8; i++) {
            if (s->textData[i] == NULL) {
                break;
            }
            func_80035260_35E60(
                s->textAsset,
                s->textData[i],
                s->textXOffset[i],
                s->textYOffset[i],
                0xFF,
                s->unkDE4 >> 16,
                0,
                1,
                4
            );
        }
    }
}
