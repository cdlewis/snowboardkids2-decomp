#include "6E840.h"
#include "common.h"

typedef struct {
    s16 unk00;
    s16 unk02;
    s16 unk04;
    s16 unk06;
    s16 unk08;
    s16 unk0A;
    s16 unk0C;
    s16 unk0E;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    s16 unk1A;
    void *unk1C;
    void *unk20;
    void *unk24;
    void *unk28;
} StructA;

typedef struct {
    u16 unk00;
    u16 unk02;
    u8 unk04;
    u8 unk05;
    u16 unk06;
    u8 pad08[2];
    u16 unk0A;
    u16 unk0C;
    u16 unk0E;
    u8 data10[1];
} StructB;

typedef struct {
    s16 clipLeft;
    s16 clipTop;
    s16 clipRight;
    s16 clipBottom;
    s16 offsetX;
    s16 offsetY;
} TextClipAndOffsetData;

typedef struct {
    u16 unk00;
    u8 unk02;
    u8 unk03;
} TileEntry;

typedef struct {
    u8 data[32];
} TexData32;

typedef struct {
    s16 unk00;
    s16 unk02;
    u16 unk04;
    u16 unk06;
    u16 unk08;
    u16 unk0A;
    u16 unk0C;
    u16 unk0E;
    u16 unk10;
    u16 unk12;
    u16 unk14;
    u16 unk16;
    s16 unk18;
    s16 unk1A;
    s32 unk1C;
    s32 unk20;
    TileEntry *unk24;
    TexData32 *unk28;
} TextureArg;

extern TextClipAndOffsetData gTextClipAndOffsetData;
extern Gfx *gRegionAllocPtr;
extern Gfx gSpriteRDPSetupDL[];
extern s16 D_8008D3C0_8DFC0[];

void func_80038420_39020(TextureArg *arg0) {
    s16 xStart;
    s16 yStart;
    s16 clipLeft;
    s16 clipTop;
    s16 clipRight;
    s16 clipBottom;
    u16 rowOffset;
    u16 colOffset;
    s16 numRows;
    s16 numCols;
    s16 row;
    s16 col;
    u16 rowStart;
    u16 colStart;
    s16 origLeft;
    s16 origTop;
    s16 right;
    s16 bottom;
    s16 texU;
    s16 texV;
    s16 calcIndex;
    s16 tileIndex;
    s16 lastTile;
    s16 rowRem;
    s16 colRem;
    s32 sp44;
    s32 sp48;
    s32 sp4C;
    s16 sp50;
    s16 sp52;
    u16 sp54;
    u16 sp56;
    s32 padding1;
    s32 tileCount;
    Gfx *displayList;
    Gfx *displayListHead;
    TileEntry *tileArr;
    TexData32 *texDataArr;
    TexData32 *curTexData;

    sp48 = arg0->unk20;
    sp4C = arg0->unk1C;
    tileArr = arg0->unk24;
    texDataArr = arg0->unk28;
    curTexData = 0;
    tileCount = 0;
    lastTile = -1;
    clipLeft = arg0->unk10;
    xStart = arg0->unk00;

    if ((displayList = arenaAlloc16(0x1B30))) {
        displayListHead = displayList;

        if (clipLeft < gTextClipAndOffsetData.clipLeft) {
            clipLeft = gTextClipAndOffsetData.clipLeft;
            xStart = xStart + (clipLeft - arg0->unk10);
        }

        clipTop = arg0->unk12;
        yStart = arg0->unk02;

        if (clipTop < gTextClipAndOffsetData.clipTop) {
            clipTop = gTextClipAndOffsetData.clipTop;
            yStart = yStart + (clipTop - arg0->unk12);
        }

        clipRight = arg0->unk10 + arg0->unk14;

        if (clipRight > gTextClipAndOffsetData.clipRight) {
            clipRight = gTextClipAndOffsetData.clipRight;
        }

        clipBottom = arg0->unk12 + arg0->unk16;

        if (clipBottom > gTextClipAndOffsetData.clipBottom) {
            clipBottom = gTextClipAndOffsetData.clipBottom;
        }

        sp54 = arg0->unk04 - 1;

        if (arg0->unk04 == 16) {
            sp56 = 4;
        } else {
            sp56 = 5;
        }

        rowOffset = clipLeft - (xStart & sp54);
        colOffset = clipTop - (yStart & sp54);

        numRows = (clipRight - clipLeft + (sp54 - 1)) >> sp56;
        numCols = (clipBottom - clipTop + (sp54 - 1)) >> sp56;

        if (xStart & sp54) {
            ++numRows;
        }

        if (yStart & sp54) {
            ++numCols;
        }

        colStart = colOffset;
        colRem = (yStart >> sp56) % arg0->unk0E;
        if (colRem < 0) {
            colRem = colRem + arg0->unk0E;
        }

        gSPDisplayList(gRegionAllocPtr++, (u32)gSpriteRDPSetupDL);

        for (col = 0; col < numCols; ++col) {
            rowStart = rowOffset;
            rowRem = (xStart >> sp56) % arg0->unk0C;
            if (rowRem < 0) {
                rowRem = rowRem + arg0->unk0C;
            }

            for (row = 0; row < numRows; ++row) {
                calcIndex = rowRem + colRem * arg0->unk08;
                tileIndex = ((u16 *)sp48)[calcIndex];
                sp44 = tileArr[tileIndex].unk02;
                calcIndex = tileArr[tileIndex].unk00;

                if (tileIndex != 0) {
                    ++tileCount;
                    sp50 = D_8008D3C0_8DFC0[tileArr[tileIndex].unk03 * 2];
                    sp52 = D_8008D3C0_8DFC0[tileArr[tileIndex].unk03 * 2 + 1];

                    origLeft = rowStart;
                    origTop = colStart;
                    right = rowStart + arg0->unk04;
                    bottom = colStart + arg0->unk06;
                    texU = 0;
                    texV = 0;

                    if (sp50 == -1) {
                        texU = arg0->unk04 - 1;
                    }

                    if (sp52 == -1) {
                        texV = arg0->unk06 - 1;
                    }

                    if (origLeft < clipRight && origTop < clipBottom && right >= clipLeft && bottom >= clipTop) {
                        if (origLeft < clipLeft) {
                            texU = clipLeft - origLeft;
                            if (sp50 == -1) {
                                texU = (arg0->unk04 - 1) - texU;
                            }
                            origLeft = clipLeft;
                        }

                        if (origTop < clipTop) {
                            texV = clipTop - origTop;
                            if (sp52 == -1) {
                                texV = (arg0->unk06 - 1) - texV;
                            }
                            origTop = clipTop;
                        }

                        if (right >= clipRight) {
                            right = clipRight - 1;
                        }

                        if (bottom >= clipBottom) {
                            bottom = clipBottom - 1;
                        }

                        if (arg0->unk18 == 0) {
                            if (&texDataArr[sp44] != curTexData) {
                                gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, (s32)&texDataArr[sp44]);
                                curTexData = &texDataArr[sp44];
                            }

                            if ((calcIndex - 1) != lastTile) {
                                gDPLoadTextureTile_4b(
                                    gRegionAllocPtr++,
                                    /*timg*/ sp4C + ((((calcIndex - 1) * arg0->unk04 * arg0->unk06) / 4) * 2),
                                    G_IM_FMT_CI,
                                    /*width*/ (arg0->unk04),
                                    /*height*/ 0,
                                    /*uls*/ 0,
                                    /*ult*/ 0,
                                    /*lrs*/ (arg0->unk04 - 1),
                                    /*lrt*/ (arg0->unk06 - 1),
                                    /*pal*/ 0,
                                    /*cms*/ 2,
                                    /*cmt*/ 2,
                                    /*masks*/ 0,
                                    /*maskt*/ 0,
                                    /*shifts*/ 0,
                                    /*shiftt*/ 0
                                );

                                lastTile = calcIndex - 1;
                            }
                        } else {
                            if (&texDataArr[sp44] != curTexData) {
                                gDPLoadTLUT_pal256(gRegionAllocPtr++, (s32)&texDataArr[sp44]);

                                curTexData = &texDataArr[sp44];
                            }

                            if ((calcIndex - 1) != lastTile) {
                                gDPLoadTextureTile(
                                    gRegionAllocPtr++,
                                    /*timg*/ (sp4C + ((((calcIndex - 1) * arg0->unk04 * arg0->unk06) / 2) * 2)),
                                    G_IM_FMT_CI,
                                    G_IM_SIZ_8b,
                                    /*width*/ (arg0->unk04),
                                    /*height*/ 0,
                                    /*uls*/ 0,
                                    /*ult*/ 0,
                                    /*lrs*/ (arg0->unk04 - 1),
                                    /*lrt*/ (arg0->unk06 - 1),
                                    /*pal*/ 0,
                                    /*cms*/ 2,
                                    /*cmt*/ 2,
                                    /*masks*/ 0,
                                    /*maskt*/ 0,
                                    /*shifts*/ 0,
                                    /*shiftt*/ 0
                                );
                                lastTile = calcIndex - 1;
                            }
                        }

                        gSPTextureRectangle(
                            gRegionAllocPtr++,
                            origLeft << 2,
                            origTop << 2,
                            right << 2,
                            bottom << 2,
                            0,
                            texU << 5,
                            texV << 5,
                            sp50 << 10,
                            sp52 << 10
                        );
                    }
                }

                rowStart = rowStart + arg0->unk04;
                rowRem = (rowRem + 1) % arg0->unk0C;
            }

            colStart = colStart + arg0->unk06;
            colRem = (colRem + 1) % arg0->unk0E;
        }

        gSPEndDisplayList(displayList++);

        gSPDisplayList(gRegionAllocPtr++, (u32)displayListHead);
    }
}

void func_800394BC_3A0BC(StructA *arg0, StructB *arg1) {
    StructB *temp;
    s32 padding1;
    s32 padding2;
    temp = arg1;

    arg0->unk00 = 0;
    arg0->unk02 = 0;
    arg0->unk08 = arg1->unk00;
    arg0->unk0A = arg1->unk02;
    arg0->unk0C = arg0->unk08;
    arg0->unk0E = arg0->unk0A;
    arg0->unk04 = arg1->unk04;
    arg0->unk06 = arg1->unk05;
    arg0->unk18 = arg1->unk06;
    arg0->unk10 = 0;
    arg0->unk12 = 0;
    arg0->unk14 = 0x140;
    arg0->unk16 = 0xF0;
    arg0->unk20 = (u8 *)temp + arg1->unk0A;
    arg0->unk1C = (u8 *)temp + arg1->unk0E;
    arg0->unk24 = &arg1->data10;
    arg0->unk28 = (u8 *)temp + arg1->unk0C;
}