#include "5520.h"
#include "common.h"
#include "displaylist.h"
#include "graphics.h"
#include "task_scheduler.h"

extern u8 identityMatrix[];
extern void *D_800885D0_891D0;
extern void *D_800885E0_891E0;
extern void *D_800885F0_891F0;

typedef struct {
    /* 0x00 */ void *model;
    /* 0x04 */ u8 opaqueMatrix[0x20];
    /* 0x24 */ void *opaqueSettings;
    /* 0x28 */ void *opaqueDisplayList;
    /* 0x2C */ void *opaqueVertexData;
    /* 0x30 */ s32 opaqueFlag;
    /* 0x34 */ u8 pad34[0xC];
    /* 0x40 */ u8 transparentMatrix[0x20];
    /* 0x60 */ void *transparentSettings;
    /* 0x64 */ void *transparentDisplayList;
    /* 0x68 */ void *transparentVertexData;
    /* 0x6C */ s32 transparentFlag;
    /* 0x70 */ u8 pad70[0xC];
    /* 0x7C */ s32 scale;
    /* 0x80 */ s16 rotationY;
    /* 0x82 */ s8 transparentEnabled;
    /* 0x83 */ s8 overlayAnimState;
    /* 0x84 */ s32 overlayScaleX;
    /* 0x88 */ s32 overlayScaleY;
    /* 0x8C */ s32 oscillationOffset;
    /* 0x90 */ s16 oscillationAngle;
    /* 0x92 */ s8 oscillationEnabled;
    /* 0x93 */ s8 scaleAnimState;
    /* 0x94 */ u8 overlayMatrix[0x20];
    /* 0xB4 */ void *overlaySettings;
    /* 0xB8 */ void *overlayDisplayList;
    /* 0xBC */ void *overlayVertexData;
    /* 0xC0 */ s32 overlayFlag;
    /* 0xC4 */ u8 padC4[0xC];
    /* 0xD0 */ void *soundData;
    /* 0xD4 */ u8 padD4[0x4];
    /* 0xD8 */ s16 scrollU;
    /* 0xDA */ s16 scrollV;
    /* 0xDC */ s16 scrollSpeedU;
    /* 0xDE */ s16 scrollSpeedV;
    /* 0xE0 */ s16 unkE0;
    /* 0xE2 */ s16 unkE2;
    /* 0xE4 */ s16 unkE4;
} RotatingLogoState;

void renderOpaqueTiledTexture(DisplayListObject *arg0);
void renderTransparentTiledTexture(s32 arg0);
void renderOverlayTiledTexture(s32 arg0);
void loadScrollingTiledTexture(ScrollingTextureState *);
void loadScrollingTexture(ScrollingTextureState *);
void cleanupRotatingLogo(RotatingLogoState *);
void func_8000595C_655C(void);
void *loadAssetGroupDisplayList(void *);
void *loadAssetGroupCompressedData(void *);
void *loadAssetGroupSoundData(void *);

extern s16 gGraphicsMode;
extern Gfx *gRegionAllocPtr;

void loadScrollingTexture(ScrollingTextureState *state) {
    s32 tempWidth;
    s32 tempHeight;
    s32 widthShift;
    s32 heightShift;
    void *paletteAddr;
    OutputStruct_19E80 tableEntry;

    gDPPipeSync(gRegionAllocPtr++);
    gDPSetTextureLUT(gRegionAllocPtr++, G_TT_RGBA16);
    gGraphicsMode = -1;

    getTableEntryByU16Index(state->textureTable, state->textureIndex, &tableEntry);

    tempWidth = tableEntry.field1;
    widthShift = 0;
loop_1:
    if (!(tempWidth & 1)) {
        do {
            widthShift += 1;
            if (widthShift < 16) {
                tempWidth >>= 1;
                goto loop_1;
            }
        } while (0);
    }

    tempHeight = tableEntry.field2;
    heightShift = 0;
loop_2:
    if (!(tempHeight & 1)) {
        do {
            heightShift += 1;
            if (heightShift < 16) {
                tempHeight >>= 1;
                goto loop_2;
            }
        } while (0);
    }

    if (state->paletteMode == 0) {
        paletteAddr = (void *)((u8 *)tableEntry.index_ptr + (state->paletteIndex << 5));

        gDPLoadTextureBlock_4b(
            gRegionAllocPtr++,
            tableEntry.data_ptr,
            G_IM_FMT_CI,
            tableEntry.field1,
            tableEntry.field2,
            0,
            0,
            0,
            widthShift,
            heightShift,
            0,
            0
        );

        gDPSetTileSize(
            gRegionAllocPtr++,
            G_TX_RENDERTILE,
            state->tileScrollU,
            state->tileScrollV,
            ((tableEntry.field1 + (s16)state->tileScrollU - 1) << 2),
            ((tableEntry.field2 + (s16)state->tileScrollV - 1) << 2)
        );

        gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, paletteAddr);
    } else {
        gDPLoadTextureBlock(
            gRegionAllocPtr++,
            tableEntry.data_ptr,
            G_IM_FMT_CI,
            G_IM_SIZ_8b,
            tableEntry.field1,
            tableEntry.field2,
            0,
            0,
            0,
            widthShift,
            heightShift,
            0,
            0
        );

        gDPSetTileSize(
            gRegionAllocPtr++,
            G_TX_RENDERTILE,
            state->tileScrollU,
            state->tileScrollV,
            ((tableEntry.field1 + (s16)state->tileScrollU - 1) << 2),
            ((tableEntry.field2 + (s16)state->tileScrollV - 1) << 2)
        );

        gDPLoadTLUT_pal256(gRegionAllocPtr++, tableEntry.index_ptr);
    }
}

void renderOpaqueScrollingTexture(DisplayListObject *arg0) {
    loadScrollingTexture((ScrollingTextureState *)arg0);
    renderOpaqueDisplayListCallback(arg0);
}

void renderTransparentScrollingTexture(DisplayListObject *arg0) {
    loadScrollingTexture((ScrollingTextureState *)arg0);
    renderTransparentDisplayListCallback(arg0);
}

void renderOverlayScrollingTexture(DisplayListObject *arg0) {
    loadScrollingTexture((ScrollingTextureState *)arg0);
    renderOverlayDisplayListCallback(arg0);
}

void enqueueScrollingTextureRender(u16 renderLayer, DisplayListObject *displayListObj) {
    displayListObj->transformMatrix = 0;

    if (displayListObj->displayLists->opaqueDisplayList != NULL) {
        debugEnqueueCallback(renderLayer, 1, &renderOpaqueScrollingTexture, displayListObj);
    }

    if (displayListObj->displayLists->transparentDisplayList != NULL) {
        debugEnqueueCallback(renderLayer, 3, &renderTransparentScrollingTexture, displayListObj);
    }

    if (displayListObj->displayLists->overlayDisplayList != NULL) {
        debugEnqueueCallback(renderLayer, 5, &renderOverlayScrollingTexture, displayListObj);
    }
}

void loadScrollingTiledTexture(ScrollingTextureState *arg0) {
    OutputStruct_19E80 tableEntry;
    s32 tempWidth;
    s32 tempHeight;
    s32 widthShift;
    s32 heightShift;
    void *paletteAddr;

    gDPPipeSync(gRegionAllocPtr++);
    gDPSetTextureLUT(gRegionAllocPtr++, G_TT_RGBA16);
    gGraphicsMode = -1;

    getTableEntryByU16Index(arg0->textureTable, arg0->textureIndex, &tableEntry);

    do {
        tempWidth = tableEntry.field1;
        widthShift = 0;
    loop_1:
        if (!(tempWidth & 1)) {
            widthShift += 1;
            if (widthShift < 16) {
                tempWidth >>= 1;
                goto loop_1;
            }
        }

        tempHeight = tableEntry.field2;
        heightShift = 0;
    loop_2:
        if (!(tempHeight & 1)) {
            heightShift += 1;
            if (heightShift < 16) {
                tempHeight >>= 1;
                goto loop_2;
            }
        }
    } while (0);

    if (arg0->paletteMode == 0) {
        paletteAddr = (void *)((u8 *)tableEntry.index_ptr + (arg0->paletteIndex << 5));

        gDPLoadTextureBlock_4b(
            gRegionAllocPtr++,
            tableEntry.data_ptr,
            G_IM_FMT_CI,
            tableEntry.field1,
            tableEntry.field2,
            0,
            0,
            0,
            widthShift,
            heightShift,
            0,
            0
        );

        gDPSetTileSize(
            gRegionAllocPtr++,
            G_TX_RENDERTILE,
            arg0->tileScrollU,
            arg0->tileScrollV,
            ((tableEntry.field1 + (s16)arg0->tileScrollU - 1) << 2),
            ((tableEntry.field2 + (s16)arg0->tileScrollV - 1) << 2)
        );

        gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, paletteAddr);
    } else {
        gDPLoadTextureBlock(
            gRegionAllocPtr++,
            tableEntry.data_ptr,
            G_IM_FMT_CI,
            G_IM_SIZ_8b,
            tableEntry.field1,
            tableEntry.field2,
            0,
            0,
            0,
            widthShift,
            heightShift,
            0,
            0
        );

        gDPSetTileSize(
            gRegionAllocPtr++,
            G_TX_RENDERTILE,
            arg0->tileScrollU,
            arg0->tileScrollV,
            ((tableEntry.field1 + (s16)arg0->tileScrollU - 1) << 2),
            ((tableEntry.field2 + (s16)arg0->tileScrollV - 1) << 2)
        );

        gDPLoadTLUT_pal256(gRegionAllocPtr++, tableEntry.index_ptr);
    }
}

void renderOpaqueTiledTexture(DisplayListObject *arg0) {
    loadScrollingTiledTexture((ScrollingTextureState *)arg0);
    buildDisplayListSegment(arg0);
}

void renderTransparentTiledTexture(s32 arg0) {
    loadScrollingTiledTexture((ScrollingTextureState *)arg0);
    buildTransparentDisplayListSegment((DisplayListObject *)arg0);
}

void renderOverlayTiledTexture(s32 arg0) {
    loadScrollingTiledTexture((ScrollingTextureState *)arg0);
    buildOverlayDisplayListSegment((DisplayListObject *)arg0);
}

void enqueueTiledTextureRender(s32 arg0, TrickBurstEffectTask *arg1) {
    arg1->base.transformMatrix = 0;
    if (arg1->base.displayLists->opaqueDisplayList != 0) {
        debugEnqueueCallback(arg0 & 0xFFFF, 1, &renderOpaqueTiledTexture, arg1);
    }
    if (arg1->base.displayLists->transparentDisplayList != 0) {
        debugEnqueueCallback(arg0 & 0xFFFF, 3, &renderTransparentTiledTexture, arg1);
    }
    if (arg1->base.displayLists->overlayDisplayList != 0) {
        debugEnqueueCallback(arg0 & 0xFFFF, 5, &renderOverlayTiledTexture, arg1);
    }
}

void initializeRotatingLogo(RotatingLogoState *state) {
    memcpy(&state->opaqueMatrix, identityMatrix, 0x20);
    state->opaqueDisplayList = loadAssetGroupDisplayList(state->model);
    state->opaqueVertexData = loadAssetGroupCompressedData(state->model);
    state->opaqueFlag = 0;
    state->opaqueSettings = &D_800885D0_891D0;
    memcpy(&state->transparentMatrix, identityMatrix, 0x20);
    state->transparentDisplayList = loadAssetGroupDisplayList(state->model);
    state->transparentVertexData = loadAssetGroupCompressedData(state->model);
    state->transparentFlag = 0;
    state->transparentSettings = &D_800885E0_891E0;
    memcpy(&state->overlayMatrix, identityMatrix, 0x20);
    state->overlayDisplayList = loadAssetGroupDisplayList(state->model);
    state->overlayVertexData = loadAssetGroupCompressedData(state->model);
    state->overlaySettings = &D_800885F0_891F0;
    state->overlayFlag = 0;
    state->scale = 0x2000;
    state->rotationY = 0;
    state->transparentEnabled = 0;
    state->overlayAnimState = 0;
    state->overlayScaleX = 0;
    state->overlayScaleY = 0;
    state->soundData = loadAssetGroupSoundData(state->model);
    state->scrollU = 0;
    state->scrollV = 0;
    state->scrollSpeedU = 0;
    state->scrollSpeedV = -0x10;
    state->unkE0 = 0;
    state->unkE2 = 0;
    state->unkE4 = 0;
    state->oscillationEnabled = 0;
    state->oscillationAngle = 0;
    state->oscillationOffset = 0;
    state->scaleAnimState = 0;
    setCleanupCallback(&cleanupRotatingLogo);
    setCallback(&func_8000595C_655C);
}

INCLUDE_ASM("asm/nonmatchings/5520", func_8000595C_655C);

void cleanupRotatingLogo(RotatingLogoState *state) {
    state->soundData = freeNodeMemory(state->soundData);
    state->overlayDisplayList = freeNodeMemory(state->overlayDisplayList);
    state->overlayVertexData = freeNodeMemory(state->overlayVertexData);
    state->opaqueDisplayList = freeNodeMemory(state->opaqueDisplayList);
    state->opaqueVertexData = freeNodeMemory(state->opaqueVertexData);
    state->transparentDisplayList = freeNodeMemory(state->transparentDisplayList);
    state->transparentVertexData = freeNodeMemory(state->transparentVertexData);
}
