#include "19E80.h"
#include "B040.h"
#include "C870.h"
#include "common.h"

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    u8 unk12;
    u8 unk13;
    u8 unk14;
    u8 pad15[3];
} SubStruct9D0;

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    u8 unk12;
    u8 unk13;
    u8 unk14;
    u8 pad15[3];
} SubStructB80;

typedef struct {
    u8 pad0[0x968];
    Table_B934 *unk968;
    u8 pad96C[0x9B8 - 0x96C];
    DataTable_19E80 *unk9B8[6];
    SubStruct9D0 unk9D0[18];
    SubStructB80 unkB80[18];
    s32 unkD30;
    u8 padD34[0xD38 - 0xD34];
    s16 unkD38;
    s16 unkD3A;
    PaletteContext paletteContexts[6];
    s32 unkD84;
    s32 unkD88;
    s16 unkD8C;
    u8 unkD8E;
    u8 unkD8F;
    void *unkD90[8];
    s16 unkDB0[8];
    s16 unkDC0[8];
    s16 unkDD0[8];
    s16 unkDE0;
    u8 padDE2[2];
    s32 unkDE4;
    s32 unkDE8;
} func_800B0760_arg;

void func_800B0760(func_800B0760_arg *arg0) {
    s32 i;
    void *temp_v0;
    s16 yOffset;

    arg0->unkD3A = 0;
    arg0->unkD8C = 0;
    arg0->unkD30 = 0;
    arg0->unkD38 = 0;
    arg0->unkD8E = 0;

    for (i = 0; i < 0x12; i++) {
        arg0->unk9D0[i].unk0 = 0;
        arg0->unk9D0[i].unk2 = -0x60 + i * 8;
        arg0->unk9D0[i].unk4 = arg0->unk9B8[arg0->unkD3A];
        arg0->unk9D0[i].unk8 = i;
        arg0->unk9D0[i].unkC = 0x400;
        arg0->unk9D0[i].unkA = 0x400;
        arg0->unk9D0[i].unkE = 0;
        arg0->unk9D0[i].unk10 = 0xFF;
        arg0->unk9D0[i].unk12 = 0;
        arg0->unk9D0[i].unk13 = 0;
        arg0->unk9D0[i].unk14 = 0;
        arg0->unkB80[i].unk0 = 0;
        arg0->unkB80[i].unk2 = -0x60 + i * 8;
        arg0->unkB80[i].unk4 = arg0->unk9B8[arg0->unkD3A];
        arg0->unkB80[i].unk8 = i;
        arg0->unkB80[i].unkC = 0x400;
        arg0->unkB80[i].unkA = 0x400;
        arg0->unkB80[i].unkE = 0;
        arg0->unkB80[i].unk10 = 0x64;
        arg0->unkB80[i].unk12 = 0;
        arg0->unkB80[i].unk13 = 0x11;
        arg0->unkB80[i].unk14 = 0;
    }

    for (i = 0; i < 6; i++) {
        initPaletteContext(&arg0->paletteContexts[i], arg0->unk9B8[i]);
        applyPaletteShift(&arg0->paletteContexts[i], 0xF00000, 0xB80000, 0x800000, 0x640000);
    }

    arg0->unkD84 = 0;
    arg0->unkD88 = 0;
    arg0->unkDE0 = 0;
    arg0->unkDE4 = 0;
    arg0->unkDE8 = 0;
    arg0->unkD8F = 0;

    for (i = 0; i < 8; i++) {
        temp_v0 = getTable2DEntry(arg0->unk968, arg0->unkDE0, 0);
        arg0->unkD90[i] = temp_v0;
        arg0->unkDB0[i] = -(getMaxLinePixelWidth(temp_v0) / 2);
        arg0->unkDC0[i] = -0x18;
        arg0->unkDD0[i] = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/1DC900", func_800B0930);
