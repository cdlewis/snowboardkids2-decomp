#include "graphics/tiled_texture.h"
#include "common.h"
#include "data/data_table.h"
#include "gbi.h"
#include "graphics/displaylist.h"
#include "graphics/graphics.h"
#include "math/geometry.h"
#include "system/task_scheduler.h"
#include "ui/level_preview_3d.h"

extern s16 gGraphicsMode;
extern Gfx *gRegionAllocPtr;

u32 D_800885D0_891D0[] = { 0x00000000, 0x00000000, 0x010005F0, 0x00000000 };
u32 D_800885E0_891E0[] = { 0x00000000, 0x00000000, 0x01000718, 0x00000000 };
u32 D_800885F0_891F0[] = { 0x00000000, 0x00000000, 0x00000000, 0x010007C8 };
u32 D_80088600[] = { 0x00000000, 0x01000B68, 0x01000B78, 0x00000000 };

extern s32 isModelVisible(func_80002B50_3750_arg *model);

void loadScrollingTexture(ScrollingTextureState *);
void loadScrollingTiledTexture(ScrollingTextureState *);
void renderOpaqueTiledTexture(DisplayListObject *arg0);
void renderTransparentTiledTexture(s32 arg0);
void renderOverlayTiledTexture(s32 arg0);
void updateRotatingLogoState(RotatingLogoState *state);
void cleanupRotatingLogo(RotatingLogoState *);

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

    tempWidth = tableEntry.width;
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

    tempHeight = tableEntry.height;
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
            tableEntry.width,
            tableEntry.height,
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
            ((tableEntry.width + (s16)state->tileScrollU - 1) << 2),
            ((tableEntry.height + (s16)state->tileScrollV - 1) << 2)
        );

        gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, paletteAddr);
    } else {
        gDPLoadTextureBlock(
            gRegionAllocPtr++,
            tableEntry.data_ptr,
            G_IM_FMT_CI,
            G_IM_SIZ_8b,
            tableEntry.width,
            tableEntry.height,
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
            ((tableEntry.width + (s16)state->tileScrollU - 1) << 2),
            ((tableEntry.height + (s16)state->tileScrollV - 1) << 2)
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
        tempWidth = tableEntry.width;
        widthShift = 0;
    loop_1:
        if (!(tempWidth & 1)) {
            widthShift += 1;
            if (widthShift < 16) {
                tempWidth >>= 1;
                goto loop_1;
            }
        }

        tempHeight = tableEntry.height;
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
            tableEntry.width,
            tableEntry.height,
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
            ((tableEntry.width + (s16)arg0->tileScrollU - 1) << 2),
            ((tableEntry.height + (s16)arg0->tileScrollV - 1) << 2)
        );

        gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, paletteAddr);
    } else {
        gDPLoadTextureBlock(
            gRegionAllocPtr++,
            tableEntry.data_ptr,
            G_IM_FMT_CI,
            G_IM_SIZ_8b,
            tableEntry.width,
            tableEntry.height,
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
            ((tableEntry.width + (s16)arg0->tileScrollU - 1) << 2),
            ((tableEntry.height + (s16)arg0->tileScrollV - 1) << 2)
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
    memcpy(&state->opaqueMatrix, &identityMatrix, 0x20);
    state->opaqueDisplayList = loadAssetGroupDisplayList(state->model);
    state->opaqueVertexData = loadAssetGroupCompressedData(state->model);
    state->opaqueFlag = 0;
    state->opaqueSettings = &D_800885D0_891D0;
    memcpy(&state->transparentMatrix, &identityMatrix, 0x20);
    state->transparentDisplayList = loadAssetGroupDisplayList(state->model);
    state->transparentVertexData = loadAssetGroupCompressedData(state->model);
    state->transparentFlag = 0;
    state->transparentSettings = &D_800885E0_891E0;
    memcpy(&state->overlayMatrix, &identityMatrix, 0x20);
    state->overlayDisplayList = loadAssetGroupDisplayList(state->model);
    state->overlayVertexData = loadAssetGroupCompressedData(state->model);
    state->overlaySettings = &D_800885F0_891F0;
    state->overlayFlag = 0;
    state->scale.val = 0x2000;
    state->rotationY = 0;
    state->transparentEnabled = 0;
    state->overlayAnimState = 0;
    state->overlayScaleX.val = 0;
    state->overlayScaleY.val = 0;
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
    setCallback(&updateRotatingLogoState);
}

void updateRotatingLogoState(RotatingLogoState *state) {
    Transform3D rotationMatrix;
    Transform3D transparentMatrix;
    s32 shouldOscillate;
    s32 newScale;
    s8 actionMode;

    memcpy(&rotationMatrix, &identityMatrix, 0x20);
    memcpy(&transparentMatrix, &identityMatrix, 0x20);

    shouldOscillate = 0;

    if (state->model->isDestroyed == 1) {
        terminateCurrentTask();
        return;
    }

    actionMode = state->model->actionMode;
    switch (actionMode) {
        case 0:
        default:
            state->scale.val = 0x2000;
            state->scaleAnimState = 0;
            break;
        case 1:
            break;
        case 2:
            state->scaleAnimState = 2;
            break;
        case 3:
            state->scaleAnimState = 3;
            state->scale.val = 0;
            break;
        case 4:
            state->transparentEnabled = 1;
            break;
        case 5:
            state->transparentEnabled = 0;
            break;
        case 6:
            state->overlayAnimState = 1;
            break;
        case 7:
            state->overlayAnimState = 2;
            break;
        case 8:
            if (state->oscillationEnabled == 0) {
                state->oscillationAngle = 0;
            }
            state->oscillationEnabled = 1;
            break;
        case 9:
            state->oscillationEnabled = 0;
            break;
    }

    switch (state->scaleAnimState) {
        case 1:
            newScale = state->scale.val - 0x111;
            state->scale.val = newScale;
            if (newScale < 0) {
                state->scale.val = 0;
            }
            break;
        case 2:
            newScale = state->scale.val + 0x111;
            state->scale.val = newScale;
            if (newScale >= 0x2001) {
                state->scale.val = 0x2000;
            }
            break;
    }

    state->oscillationAngle += 0x88;

    if (state->oscillationEnabled != 0) {
        shouldOscillate = 1;
    } else {
        state->oscillationOffset = 0;
        state->oscillationAngle = 0;
    }

    if (shouldOscillate != 0) {
        state->oscillationOffset = (s32)(((s32)(approximateSin(state->oscillationAngle) * 4) >> 8) << 11);
    }

    state->model->unk44 = state->oscillationOffset;

    switch (state->overlayAnimState) {
        case 1:
            state->overlayScaleX.val += 0x222;
            state->overlayScaleY.val += 0x111;
            if (state->overlayScaleX.val >= 0x4001) {
                state->overlayScaleX.val = 0x4000;
            }
            if (state->overlayScaleY.val >= 0x2001) {
                state->overlayScaleY.val = 0x2000;
            }
            break;
        case 2:
            state->overlayScaleX.val -= 0x222;
            state->overlayScaleY.val -= 0x111;
            if (state->overlayScaleX.val < 0) {
                state->overlayScaleX.val = 0;
            }
            if (state->overlayScaleY.val < 0) {
                state->overlayScaleY.val = 0;
            }
            if (state->overlayScaleX.val == 0 && state->overlayScaleY.val == 0) {
                state->overlayAnimState = 0;
            }
            break;
    }

    createYRotationMatrix(&rotationMatrix, 0x400);
    func_8006B084_6BC84(&rotationMatrix, &state->model->matrix18, &state->opaqueMatrix);
    memcpy(&state->opaqueMatrix.translation, &state->model->matrix18.translation, sizeof(Vec3i));

    state->opaqueMatrix.translation.y += state->oscillationOffset;
    scaleMatrix(&state->opaqueMatrix, 0x2000, state->scale.halves[1], 0x2000);

    if (state->scale.val != 0) {
        enqueueModelDisplayList((func_80002B50_3750_arg *)state->model, (DisplayListObject *)&state->opaqueMatrix);
    }

    if (state->transparentEnabled == 0) {
        s16 tempAngle;

        state->rotationY -= 0xF;
        tempAngle = state->rotationY;
        createYRotationMatrix(&transparentMatrix, tempAngle);
        func_8006B084_6BC84(&transparentMatrix, &state->model->matrix18, &state->transparentMatrix);

        state->transparentMatrix.translation.y += 0x166666;
        state->transparentMatrix.translation.y += state->oscillationOffset;
        enqueueModelDisplayList((func_80002B50_3750_arg *)state->model, (DisplayListObject *)&state->transparentMatrix);
    }

    if (state->overlayAnimState != 0 && isModelVisible((func_80002B50_3750_arg *)state->model) != 0) {
        memcpy(&state->overlayMatrix, &state->model->matrix18, sizeof(Transform3D));

        state->overlayMatrix.translation.y += state->oscillationOffset;
        scaleMatrix(
            &state->overlayMatrix,
            state->overlayScaleY.halves[1],
            state->overlayScaleX.halves[1],
            state->overlayScaleY.halves[1]
        );

        state->scrollU += state->scrollSpeedU;
        state->scrollV += state->scrollSpeedV;
        *(s16 *)((u8 *)state + 0xD8) = *(u8 *)((u8 *)state + 0xD9);
        *(s16 *)((u8 *)state + 0xDA) = *(u8 *)((u8 *)state + 0xDB);
        enqueueScrollingTextureRender(state->model->unk10->unk16, (DisplayListObject *)&state->overlayMatrix);
    }
}

void cleanupRotatingLogo(RotatingLogoState *state) {
    state->soundData = freeNodeMemory(state->soundData);
    state->overlayDisplayList = freeNodeMemory(state->overlayDisplayList);
    state->overlayVertexData = freeNodeMemory(state->overlayVertexData);
    state->opaqueDisplayList = freeNodeMemory(state->opaqueDisplayList);
    state->opaqueVertexData = freeNodeMemory(state->opaqueVertexData);
    state->transparentDisplayList = freeNodeMemory(state->transparentDisplayList);
    state->transparentVertexData = freeNodeMemory(state->transparentVertexData);
}
