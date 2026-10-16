#include "animation/model_transition.h"
#include "assets.h"
#include "common.h"
#include "data/data_table.h"
#include "gbi.h"
#include "graphics/graphics.h"
#include "math/geometry.h"
#include "system/task_scheduler.h"
#include "ui/level_preview_3d.h"

/* Segment 2 is rebound to state->frameData by renderPalettedTexture. */
#define MODEL_TRANSITION_FRAME_SEGMENT 2

Gfx gModelTransitionEffectDisplayList[] = {
    gsSPClearGeometryMode(
        G_ZBUFFER | G_SHADE | G_CULL_BOTH | G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR | G_SHADING_SMOOTH
    ),
    gsSPSetGeometryMode(G_ZBUFFER | G_SHADE | G_FOG | G_SHADING_SMOOTH),
    gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
    gsSPVertex((Vtx *)(MODEL_TRANSITION_FRAME_SEGMENT << 24), 24, 0),
    gsDPPipeSync(),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsDPSetCycleType(G_CYC_2CYCLE),
    gsDPSetCombineMode(G_CC_MODULATEIDECALA, G_CC_PASS2),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP2Triangles(6, 7, 8, 0, 9, 10, 11, 0),
    gsSP2Triangles(12, 13, 14, 0, 15, 16, 17, 0),
    gsSP2Triangles(18, 19, 20, 0, 21, 22, 23, 0),
    gsSPEndDisplayList(),
};

void setupModelTransitionVariant(ModelTransitionEffectState *);
void updateModelTransitionEffect(ModelTransitionEffectState *);
void cleanupModelTransitionEffect(ModelTransitionEffectState *);
void updateCameraRotationTask(CameraRotationTaskState *);
void cleanupCameraRotationTask(void);
void renderPalettedTexture(ModelTransitionEffectState *);

void initCameraRotationTask(CameraRotationTaskState *state) {
    setCleanupCallback(&cleanupCameraRotationTask);
    state->rotationZ = 0;
    state->rotationY = 0;
    state->rotationX = 0;
    state->angularAccelerationZ = 0;
    state->angularAccelerationY = 0;
    state->angularAccelerationX = 0;
    state->angularVelocityZ = 0;
    state->angularVelocityY = 0;
    state->angularVelocityX = 0;
    setCallback(&updateCameraRotationTask);
}

void updateCameraRotationTask(CameraRotationTaskState *state) {
    Transform3D cameraTransform;

    if (state->model->isDestroyed == 1) {
        terminateCurrentTask();
        return;
    }

    switch (state->model->actionMode) {
        default:
        case 0:
            state->rotationZ = 0;
            state->rotationY = 0;
            state->rotationX = 0;
            state->angularAccelerationZ = 0;
            state->angularAccelerationY = 0;
            state->angularAccelerationX = 0;
            state->angularVelocityZ = 0;
            state->angularVelocityY = 0;
            state->angularVelocityX = 0;
            break;
        case 1:
            if (state->angularVelocityZ > 0) {
                if (state->rotationZ >= 0xE4) {
                    state->angularVelocityZ = -0x5B;
                }
            } else if (state->angularVelocityZ < 0) {
                if (state->rotationZ < -0xE3) {
                    state->angularVelocityZ = 0x5B;
                }
            } else {
                state->angularVelocityZ = 0x44;
            }
            break;
        case 2:
            state->angularVelocityZ = 0x222;
            break;
        case 3:
            state->angularVelocityZ = -0x222;
            break;
    }

    state->rotationX = (u16)state->rotationX + (u16)state->angularVelocityX;
    state->rotationY = (u16)state->rotationY + (u16)state->angularVelocityY;
    state->rotationZ = (u16)state->rotationZ + (u16)state->angularVelocityZ;

    gScaleMatrix.translation.x = 0;
    gScaleMatrix.translation.y = -0x4CCCC;
    gScaleMatrix.translation.z = 0;

    createRotationMatrixXYZ((s16 *)&cameraTransform, state->rotationX, state->rotationY, state->rotationZ);

    cameraTransform.translation.x = 0;
    cameraTransform.translation.y = 0x4CCCC;
    cameraTransform.translation.z = 0;

    composeTransform3D(&gScaleMatrix, &cameraTransform, &state->model->baseTransform);
}

void cleanupCameraRotationTask(void) {
}

void renderPalettedTexture(ModelTransitionEffectState *state) {
    s32 dxtBase;
    s32 new_var;
    u32 line;
    s32 lrs;
    u16 dxt;
    u16 widthDiv16;
    Gfx *loadBlockCmd;
    long loadBlockWord;
    volatile u8 padding[0x10];

    gDPPipeSync(gDisplayListAllocPtr++);

    gDPSetTextureLUT(gDisplayListAllocPtr++, G_TT_RGBA16);

    gDPSetTextureImage(gDisplayListAllocPtr++, G_IM_FMT_CI, G_IM_SIZ_16b, 1, state->texture.data_ptr);

    gDPSetTile(
        gDisplayListAllocPtr++,
        G_IM_FMT_CI,
        G_IM_SIZ_16b,
        0,
        0x0000,
        G_TX_LOADTILE,
        0,
        G_TX_NOMIRROR | G_TX_WRAP,
        G_TX_NOMASK,
        G_TX_NOLOD,
        G_TX_NOMIRROR | G_TX_WRAP,
        G_TX_NOMASK,
        G_TX_NOLOD
    );

    gDPLoadSync(gDisplayListAllocPtr++);

    loadBlockCmd = gDisplayListAllocPtr++;
    loadBlockCmd->words.w0 = 0xF3000000;
    gGraphicsMode = GRAPHICS_MODE_INVALID;
    widthDiv16 = state->texture.width >> 4;
    dxtBase = 0x800;
    if (widthDiv16 != 0) {
        dxtBase = widthDiv16 + 0x7FF;
    }
    lrs = (((s32)((state->texture.width * state->texture.height) + 3)) >> 2) - 1;
    if (lrs < 0x800) {
    } else {
        lrs = 0x7FF;
    }
    line = lrs & 0xFFF;
    new_var = (line << 12) | 0x07000000;
    loadBlockWord = new_var;
    if (widthDiv16 != 0) {
        loadBlockWord |= (dxtBase / widthDiv16) & 0xFFF;
    } else {
        loadBlockWord |= dxtBase & 0xFFF;
    }
    loadBlockCmd->words.w1 = loadBlockWord;

    gDPPipeSync(gDisplayListAllocPtr++);

    line = (((state->texture.width >> 1) + 7) >> 3) & 0x1FF;
    new_var = G_TX_NOMIRROR;
    gDPSetTile(gDisplayListAllocPtr++, G_IM_FMT_CI, G_IM_SIZ_4b, line, 0, G_TX_RENDERTILE, 0, 0, 0, 0, 0, 0, 0);

    gDPSetTileSize(
        gDisplayListAllocPtr++,
        G_TX_RENDERTILE,
        0,
        0,
        (state->texture.width - 1) << 2,
        (state->texture.height - 1) << 2
    );

    gDPSetTextureImage(gDisplayListAllocPtr++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, state->texture.index_ptr);

    gDPTileSync(gDisplayListAllocPtr++);

    gDPSetTile(
        gDisplayListAllocPtr++,
        G_IM_FMT_RGBA,
        G_IM_SIZ_4b,
        0,
        0x0100,
        G_TX_LOADTILE,
        0,
        G_TX_NOMIRROR | G_TX_WRAP,
        G_TX_NOMASK,
        G_TX_NOLOD,
        G_TX_NOMIRROR | G_TX_WRAP,
        G_TX_NOMASK,
        G_TX_NOLOD
    );

    gDPLoadSync(gDisplayListAllocPtr++);

    gDPLoadTLUTCmd(gDisplayListAllocPtr++, G_TX_LOADTILE, 15);

    gDPPipeSync(gDisplayListAllocPtr++);

    if (state->n64Transform == 0) {
        state->n64Transform = graphicsAlloc(sizeof(Mtx));
        if (state->n64Transform == 0) {
            return;
        }
        transform3DToN64Mtx(&state->transform, state->n64Transform);
    }

    gDPPipeSync(gDisplayListAllocPtr++);

    gDPSetTexturePersp(gDisplayListAllocPtr++, G_TP_PERSP);

    gSPSegment(gDisplayListAllocPtr++, MODEL_TRANSITION_FRAME_SEGMENT, state->frameData);

    gSPMatrix(gDisplayListAllocPtr++, state->n64Transform, (G_MTX_NOPUSH | G_MTX_LOAD) | G_MTX_MODELVIEW);

    gSPDisplayList(gDisplayListAllocPtr++, gModelTransitionEffectDisplayList);
}

void initModelTransitionEffect(ModelTransitionEffectState *state) {
    setCleanupCallback(&cleanupModelTransitionEffect);
    state->textureTable =
        loadCompressedData(&modelTransitionAnimationTable_ROM_START, &modelTransitionAnimationTable_ROM_END, 0xE8);
    state->frames =
        loadCompressedData(&modelTransitionFrameBuffer_ROM_START, &modelTransitionFrameBuffer_ROM_END, 0xC00);
    if (state->model->index == 0x3E) {
        state->textureIndex = 0;
    } else {
        state->textureIndex = 1;
    }
    setCallback(&setupModelTransitionVariant);
}

void setupModelTransitionVariant(ModelTransitionEffectState *state) {
    getTableEntryByU16Index(state->textureTable, state->textureIndex != 0, &state->texture);
    state->frameIndex = 0;
    setCallback(&updateModelTransitionEffect);
}

void updateModelTransitionEffect(ModelTransitionEffectState *state) {
    u8 unusedMatrixBuffer[0x20];
    s32 frameBeforeIncrement;
    s8 actionMode;
    void *unused;

    memcpy(&unusedMatrixBuffer, &identityMatrix, sizeof(Transform3D));

    if (state->model->isDestroyed == 1) {
        terminateCurrentTask();
        return;
    }

    actionMode = state->model->actionMode;
    if (state->model->actionMode == 0) {
        state->frameIndex = 0;
    } else if (actionMode < 0 || state->model->actionMode >= 3) {
        state->frameIndex = 0;
    } else {
        do {
            if (state->frameIndex == 0) {
                memcpy(&state->transform, &state->model->transform, sizeof(Transform3D));
                scaleMatrix(&state->transform, 0x1000, 0x1000, 0x1000);
            }
        } while (0);
        setModelDisplayEnabled(state->model, 0);
    }

    if (state->model->actionMode == 1) {
        frameBeforeIncrement = state->frameIndex;
        if (state->frameIndex < 7) {
            state->n64Transform = NULL;
            state->frameData = &state->frames[frameBeforeIncrement];
            pushViewportCallbackBySlot(0, VIEWPORT_CALLBACK_LAYER_OPAQUE, &renderPalettedTexture, state);
            state->frameIndex++;
        }
    }
}

void cleanupModelTransitionEffect(ModelTransitionEffectState *state) {
    state->textureTable = freeNodeMemory(state->textureTable);
    state->frames = freeNodeMemory(state->frames);
}
