#include "10AD0.h"
#include "6E840.h"
#include "common.h"

typedef struct {
    /* 0x00 */ u16 textureIndex;
    /* 0x02 */ u8 paletteIndex;
    /* 0x03 */ u8 flipMode;
} TileEntry;

typedef struct {
    /* 0x00 */ u8 data[32];
} TexData32;

typedef struct {
    /* 0x00 */ u16 tileGridWidth;
    /* 0x02 */ u16 tileGridHeight;
    /* 0x04 */ u8 tileWidth;
    /* 0x05 */ u8 tileHeight;
    /* 0x06 */ u16 ciMode;
    /* 0x08 */ u8 pad08[2];
    /* 0x0A */ u16 tileIndexDataOffset;
    /* 0x0C */ u16 paletteDataOffset;
    /* 0x0E */ u16 textureDataOffset;
    /* 0x10 */ u8 tileEntries[1];
} TiledTextureAsset;

typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ u16 tileWidth;
    /* 0x06 */ u16 tileHeight;
    /* 0x08 */ u16 tilesPerRow;
    /* 0x0A */ u16 tilesPerCol;
    /* 0x0C */ u16 tileGridWidth;
    /* 0x0E */ u16 tileGridHeight;
    /* 0x10 */ u16 clipX;
    /* 0x12 */ u16 clipY;
    /* 0x14 */ u16 clipWidth;
    /* 0x16 */ u16 clipHeight;
    /* 0x18 */ s16 ciMode;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ s32 textureData;
    /* 0x20 */ s32 tileIndexData;
    /* 0x24 */ TileEntry *tileEntries;
    /* 0x28 */ TexData32 *paletteData;
} TiledTextureRenderState;

extern Gfx *gRegionAllocPtr;
extern Gfx gSpriteRDPSetupDL[];
extern s16 gTileTextureFlipTable[];
extern TextClipAndOffsetData gTextClipAndOffsetData;

void renderTiledTexture(TiledTextureRenderState *state) {
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
    s16 screenLeft;
    s16 screenTop;
    s16 screenRight;
    s16 screenBottom;
    s16 texU;
    s16 texV;
    s16 textureIndex;
    s16 tileGridIndex;
    s16 lastTile;
    s16 rowRem;
    s16 colRem;
    s32 paletteIndex;
    s32 tileIndexData;
    s32 textureData;
    s16 flipX;
    s16 flipY;
    u16 tileMask;
    u16 tileShift;
    s32 padding1;
    s32 tileCount;
    Gfx *displayList;
    Gfx *displayListHead;
    TileEntry *tileEntries;
    TexData32 *paletteData;
    TexData32 *curPaletteData;

    tileIndexData = state->tileIndexData;
    textureData = state->textureData;
    tileEntries = state->tileEntries;
    paletteData = state->paletteData;
    curPaletteData = 0;
    tileCount = 0;
    lastTile = -1;
    clipLeft = state->clipX;
    xStart = state->x;

    if ((displayList = arenaAlloc16(0x1B30))) {
        displayListHead = displayList;

        if (clipLeft < gTextClipAndOffsetData.clipLeft) {
            clipLeft = gTextClipAndOffsetData.clipLeft;
            xStart = xStart + (clipLeft - state->clipX);
        }

        clipTop = state->clipY;
        yStart = state->y;

        if (clipTop < gTextClipAndOffsetData.clipTop) {
            clipTop = gTextClipAndOffsetData.clipTop;
            yStart = yStart + (clipTop - state->clipY);
        }

        clipRight = state->clipX + state->clipWidth;

        if (clipRight > gTextClipAndOffsetData.clipRight) {
            clipRight = gTextClipAndOffsetData.clipRight;
        }

        clipBottom = state->clipY + state->clipHeight;

        if (clipBottom > gTextClipAndOffsetData.clipBottom) {
            clipBottom = gTextClipAndOffsetData.clipBottom;
        }

        tileMask = state->tileWidth - 1;

        if (state->tileWidth == 16) {
            tileShift = 4;
        } else {
            tileShift = 5;
        }

        rowOffset = clipLeft - (xStart & tileMask);
        colOffset = clipTop - (yStart & tileMask);

        numRows = (clipRight - clipLeft + (tileMask - 1)) >> tileShift;
        numCols = (clipBottom - clipTop + (tileMask - 1)) >> tileShift;

        if (xStart & tileMask) {
            ++numRows;
        }

        if (yStart & tileMask) {
            ++numCols;
        }

        colStart = colOffset;
        colRem = (yStart >> tileShift) % state->tileGridHeight;
        if (colRem < 0) {
            colRem = colRem + state->tileGridHeight;
        }

        gSPDisplayList(gRegionAllocPtr++, (u32)gSpriteRDPSetupDL);

        for (col = 0; col < numCols; ++col) {
            rowStart = rowOffset;
            rowRem = (xStart >> tileShift) % state->tileGridWidth;
            if (rowRem < 0) {
                rowRem = rowRem + state->tileGridWidth;
            }

            for (row = 0; row < numRows; ++row) {
                textureIndex = rowRem + colRem * state->tilesPerRow;
                tileGridIndex = ((u16 *)tileIndexData)[textureIndex];
                paletteIndex = tileEntries[tileGridIndex].paletteIndex;
                textureIndex = tileEntries[tileGridIndex].textureIndex;

                if (tileGridIndex != 0) {
                    ++tileCount;
                    flipX = gTileTextureFlipTable[tileEntries[tileGridIndex].flipMode * 2];
                    flipY = gTileTextureFlipTable[tileEntries[tileGridIndex].flipMode * 2 + 1];

                    screenLeft = rowStart;
                    screenTop = colStart;
                    screenRight = rowStart + state->tileWidth;
                    screenBottom = colStart + state->tileHeight;
                    texU = 0;
                    texV = 0;

                    if (flipX == -1) {
                        texU = state->tileWidth - 1;
                    }

                    if (flipY == -1) {
                        texV = state->tileHeight - 1;
                    }

                    if (screenLeft < clipRight && screenTop < clipBottom && screenRight >= clipLeft &&
                        screenBottom >= clipTop) {
                        if (screenLeft < clipLeft) {
                            texU = clipLeft - screenLeft;
                            if (flipX == -1) {
                                texU = (state->tileWidth - 1) - texU;
                            }
                            screenLeft = clipLeft;
                        }

                        if (screenTop < clipTop) {
                            texV = clipTop - screenTop;
                            if (flipY == -1) {
                                texV = (state->tileHeight - 1) - texV;
                            }
                            screenTop = clipTop;
                        }

                        if (screenRight >= clipRight) {
                            screenRight = clipRight - 1;
                        }

                        if (screenBottom >= clipBottom) {
                            screenBottom = clipBottom - 1;
                        }

                        if (state->ciMode == 0) {
                            if (&paletteData[paletteIndex] != curPaletteData) {
                                gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, (s32)&paletteData[paletteIndex]);
                                curPaletteData = &paletteData[paletteIndex];
                            }

                            if ((textureIndex - 1) != lastTile) {
                                gDPLoadTextureTile_4b(
                                    gRegionAllocPtr++,
                                    /*timg*/ textureData +
                                        ((((textureIndex - 1) * state->tileWidth * state->tileHeight) / 4) * 2),
                                    G_IM_FMT_CI,
                                    /*width*/ (state->tileWidth),
                                    /*height*/ 0,
                                    /*uls*/ 0,
                                    /*ult*/ 0,
                                    /*lrs*/ (state->tileWidth - 1),
                                    /*lrt*/ (state->tileHeight - 1),
                                    /*pal*/ 0,
                                    /*cms*/ 2,
                                    /*cmt*/ 2,
                                    /*masks*/ 0,
                                    /*maskt*/ 0,
                                    /*shifts*/ 0,
                                    /*shiftt*/ 0
                                );

                                lastTile = textureIndex - 1;
                            }
                        } else {
                            if (&paletteData[paletteIndex] != curPaletteData) {
                                gDPLoadTLUT_pal256(gRegionAllocPtr++, (s32)&paletteData[paletteIndex]);

                                curPaletteData = &paletteData[paletteIndex];
                            }

                            if ((textureIndex - 1) != lastTile) {
                                gDPLoadTextureTile(
                                    gRegionAllocPtr++,
                                    /*timg*/
                                    (textureData +
                                     ((((textureIndex - 1) * state->tileWidth * state->tileHeight) / 2) * 2)),
                                    G_IM_FMT_CI,
                                    G_IM_SIZ_8b,
                                    /*width*/ (state->tileWidth),
                                    /*height*/ 0,
                                    /*uls*/ 0,
                                    /*ult*/ 0,
                                    /*lrs*/ (state->tileWidth - 1),
                                    /*lrt*/ (state->tileHeight - 1),
                                    /*pal*/ 0,
                                    /*cms*/ 2,
                                    /*cmt*/ 2,
                                    /*masks*/ 0,
                                    /*maskt*/ 0,
                                    /*shifts*/ 0,
                                    /*shiftt*/ 0
                                );
                                lastTile = textureIndex - 1;
                            }
                        }

                        gSPTextureRectangle(
                            gRegionAllocPtr++,
                            screenLeft << 2,
                            screenTop << 2,
                            screenRight << 2,
                            screenBottom << 2,
                            0,
                            texU << 5,
                            texV << 5,
                            flipX << 10,
                            flipY << 10
                        );
                    }
                }

                rowStart = rowStart + state->tileWidth;
                rowRem = (rowRem + 1) % state->tileGridWidth;
            }

            colStart = colStart + state->tileHeight;
            colRem = (colRem + 1) % state->tileGridHeight;
        }

        gSPEndDisplayList(displayList++);

        gSPDisplayList(gRegionAllocPtr++, (u32)displayListHead);
    }
}

void initTiledTextureRenderState(TiledTextureRenderState *arg0, TiledTextureAsset *arg1) {
    TiledTextureAsset *asset;
    s32 padding1;
    s32 padding2;
    asset = arg1;

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
    arg0->tileIndexData = (s32)((u8 *)asset + arg1->tileIndexDataOffset);
    arg0->textureData = (s32)((u8 *)asset + arg1->textureDataOffset);
    arg0->tileEntries = (TileEntry *)&arg1->tileEntries;
    arg0->paletteData = (TexData32 *)((u8 *)asset + arg1->paletteDataOffset);
}
