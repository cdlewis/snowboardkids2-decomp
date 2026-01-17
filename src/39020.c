#include "6E840.h"
#include "common.h"

typedef struct {
    s16 x;
    s16 y;
    s16 tileWidth;
    s16 tileHeight;
    s16 tilesPerRow;
    s16 tilesPerCol;
    s16 tileGridWidth;
    s16 tileGridHeight;
    s16 clipX;
    s16 clipY;
    s16 clipWidth;
    s16 clipHeight;
    s16 ciMode;
    s16 unk1A;
    void *textureData;
    void *tileIndexData;
    void *tileEntries;
    void *paletteData;
} TiledTextureRenderState;

typedef struct {
    u16 tileGridWidth;
    u16 tileGridHeight;
    u8 tileWidth;
    u8 tileHeight;
    u16 ciMode;
    u8 pad08[2];
    u16 tileIndexDataOffset;
    u16 paletteDataOffset;
    u16 textureDataOffset;
    u8 tileEntries[1];
} TiledTextureAsset;

typedef struct {
    s16 clipLeft;
    s16 clipTop;
    s16 clipRight;
    s16 clipBottom;
    s16 offsetX;
    s16 offsetY;
} TextClipAndOffsetData;

typedef struct {
    u16 textureIndex;
    u8 paletteIndex;
    u8 flipMode;
} TileEntry;

typedef struct {
    u8 data[32];
} TexData32;

typedef struct {
    s16 x;
    s16 y;
    u16 tileWidth;
    u16 tileHeight;
    u16 tilesPerRow;
    u16 tilesPerCol;
    u16 tileGridWidth;
    u16 tileGridHeight;
    u16 clipX;
    u16 clipY;
    u16 clipWidth;
    u16 clipHeight;
    s16 ciMode;
    s16 unk1A;
    s32 textureData;
    s32 tileIndexData;
    TileEntry *tileEntries;
    TexData32 *paletteData;
} TiledTextureRenderState2;

extern TextClipAndOffsetData gTextClipAndOffsetData;
extern Gfx *gRegionAllocPtr;
extern Gfx gSpriteRDPSetupDL[];
extern s16 gTileTextureFlipTable[];

void renderTiledTexture(TiledTextureRenderState2 *arg0) {
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

    sp48 = arg0->tileIndexData;
    sp4C = arg0->textureData;
    tileArr = arg0->tileEntries;
    texDataArr = arg0->paletteData;
    curTexData = 0;
    tileCount = 0;
    lastTile = -1;
    clipLeft = arg0->clipX;
    xStart = arg0->x;

    if ((displayList = arenaAlloc16(0x1B30))) {
        displayListHead = displayList;

        if (clipLeft < gTextClipAndOffsetData.clipLeft) {
            clipLeft = gTextClipAndOffsetData.clipLeft;
            xStart = xStart + (clipLeft - arg0->clipX);
        }

        clipTop = arg0->clipY;
        yStart = arg0->y;

        if (clipTop < gTextClipAndOffsetData.clipTop) {
            clipTop = gTextClipAndOffsetData.clipTop;
            yStart = yStart + (clipTop - arg0->clipY);
        }

        clipRight = arg0->clipX + arg0->clipWidth;

        if (clipRight > gTextClipAndOffsetData.clipRight) {
            clipRight = gTextClipAndOffsetData.clipRight;
        }

        clipBottom = arg0->clipY + arg0->clipHeight;

        if (clipBottom > gTextClipAndOffsetData.clipBottom) {
            clipBottom = gTextClipAndOffsetData.clipBottom;
        }

        sp54 = arg0->tileWidth - 1;

        if (arg0->tileWidth == 16) {
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
        colRem = (yStart >> sp56) % arg0->tileGridHeight;
        if (colRem < 0) {
            colRem = colRem + arg0->tileGridHeight;
        }

        gSPDisplayList(gRegionAllocPtr++, (u32)gSpriteRDPSetupDL);

        for (col = 0; col < numCols; ++col) {
            rowStart = rowOffset;
            rowRem = (xStart >> sp56) % arg0->tileGridWidth;
            if (rowRem < 0) {
                rowRem = rowRem + arg0->tileGridWidth;
            }

            for (row = 0; row < numRows; ++row) {
                calcIndex = rowRem + colRem * arg0->tilesPerRow;
                tileIndex = ((u16 *)sp48)[calcIndex];
                sp44 = tileArr[tileIndex].paletteIndex;
                calcIndex = tileArr[tileIndex].textureIndex;

                if (tileIndex != 0) {
                    ++tileCount;
                    sp50 = gTileTextureFlipTable[tileArr[tileIndex].flipMode * 2];
                    sp52 = gTileTextureFlipTable[tileArr[tileIndex].flipMode * 2 + 1];

                    origLeft = rowStart;
                    origTop = colStart;
                    right = rowStart + arg0->tileWidth;
                    bottom = colStart + arg0->tileHeight;
                    texU = 0;
                    texV = 0;

                    if (sp50 == -1) {
                        texU = arg0->tileWidth - 1;
                    }

                    if (sp52 == -1) {
                        texV = arg0->tileHeight - 1;
                    }

                    if (origLeft < clipRight && origTop < clipBottom && right >= clipLeft && bottom >= clipTop) {
                        if (origLeft < clipLeft) {
                            texU = clipLeft - origLeft;
                            if (sp50 == -1) {
                                texU = (arg0->tileWidth - 1) - texU;
                            }
                            origLeft = clipLeft;
                        }

                        if (origTop < clipTop) {
                            texV = clipTop - origTop;
                            if (sp52 == -1) {
                                texV = (arg0->tileHeight - 1) - texV;
                            }
                            origTop = clipTop;
                        }

                        if (right >= clipRight) {
                            right = clipRight - 1;
                        }

                        if (bottom >= clipBottom) {
                            bottom = clipBottom - 1;
                        }

                        if (arg0->ciMode == 0) {
                            if (&texDataArr[sp44] != curTexData) {
                                gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, (s32)&texDataArr[sp44]);
                                curTexData = &texDataArr[sp44];
                            }

                            if ((calcIndex - 1) != lastTile) {
                                gDPLoadTextureTile_4b(
                                    gRegionAllocPtr++,
                                    /*timg*/ sp4C + ((((calcIndex - 1) * arg0->tileWidth * arg0->tileHeight) / 4) * 2),
                                    G_IM_FMT_CI,
                                    /*width*/ (arg0->tileWidth),
                                    /*height*/ 0,
                                    /*uls*/ 0,
                                    /*ult*/ 0,
                                    /*lrs*/ (arg0->tileWidth - 1),
                                    /*lrt*/ (arg0->tileHeight - 1),
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
                                    /*timg*/
                                    (sp4C + ((((calcIndex - 1) * arg0->tileWidth * arg0->tileHeight) / 2) * 2)),
                                    G_IM_FMT_CI,
                                    G_IM_SIZ_8b,
                                    /*width*/ (arg0->tileWidth),
                                    /*height*/ 0,
                                    /*uls*/ 0,
                                    /*ult*/ 0,
                                    /*lrs*/ (arg0->tileWidth - 1),
                                    /*lrt*/ (arg0->tileHeight - 1),
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

                rowStart = rowStart + arg0->tileWidth;
                rowRem = (rowRem + 1) % arg0->tileGridWidth;
            }

            colStart = colStart + arg0->tileHeight;
            colRem = (colRem + 1) % arg0->tileGridHeight;
        }

        gSPEndDisplayList(displayList++);

        gSPDisplayList(gRegionAllocPtr++, (u32)displayListHead);
    }
}

void initTiledTextureRenderState(TiledTextureRenderState *arg0, TiledTextureAsset *arg1) {
    TiledTextureAsset *temp;
    s32 padding1;
    s32 padding2;
    temp = arg1;

    arg0->x = 0;
    arg0->y = 0;
    arg0->tilesPerRow = arg1->tileGridWidth;
    arg0->tilesPerCol = arg1->tileGridHeight;
    arg0->tileGridWidth = arg0->tilesPerRow;
    arg0->tileGridHeight = arg0->tilesPerCol;
    arg0->tileWidth = arg1->tileWidth;
    arg0->tileHeight = arg1->tileHeight;
    arg0->ciMode = arg1->ciMode;
    arg0->clipX = 0;
    arg0->clipY = 0;
    arg0->clipWidth = 0x140;
    arg0->clipHeight = 0xF0;
    arg0->tileIndexData = (u8 *)temp + arg1->tileIndexDataOffset;
    arg0->textureData = (u8 *)temp + arg1->textureDataOffset;
    arg0->tileEntries = &arg1->tileEntries;
    arg0->paletteData = (u8 *)temp + arg1->paletteDataOffset;
}