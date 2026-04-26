#include "assets.h"
#include "common.h"
#include "data/data_table.h"
#include "gbi.h"
#include "graphics/graphics.h"
#include "math/geometry.h"
#include "system/task_scheduler.h"
#include "ui/level_preview_3d.h"

typedef struct {
    s32 unk0;
    u8 padding[0x17C];
} TransitionEffectFrame;

typedef struct {
    /* 0x00 */ SceneModel *model;
    /* 0x04 */ void *animationTable;
    /* 0x08 */ OutputStruct_19E80 tableEntry;
    /* 0x14 */ Transform3D transformMatrix;
    /* 0x34 */ Mtx *lookAtMatrix;
    /* 0x38 */ void *segmentData;
    /* 0x3C */ s32 currentFrame;
    /* 0x40 */ s32 effectVariant;
    /* 0x44 */ TransitionEffectFrame *frameBuffer;
} ModelTransitionEffectState;

extern s16 gGraphicsMode;
extern Gfx *gRegionAllocPtr;

void setupModelTransitionVariant(ModelTransitionEffectState *);
void updateModelTransitionEffect(ModelTransitionEffectState *);
void cleanupModelTransitionEffect(ModelTransitionEffectState *);
void updateCameraRotationTask(CameraRotationTaskState *);
void cleanupCameraRotationTask(void);
void renderPalettedTexture(ModelTransitionEffectState *);

s32 D_8008C200_8CE00[] = {
    0xD9D0F9FA, 0x00000000, 0xD9FFFFFF, 0x00210005, 0xD7000002, 0x80008000, 0x01018030,
    0x02000000, 0xE7000000, 0x00000000, 0xE3001201, 0x00002000, 0xE3000A01, 0x00100000,
    0xFC127FFF, 0xFFFFF238, 0xE200001C, 0xC8112078, 0x06000204, 0x0006080A, 0x060C0E10,
    0x00121416, 0x06181A1C, 0x001E2022, 0x06242628, 0x002A2C2E, 0xDF000000, 0x00000000,
};

void initCameraRotationTask(CameraRotationTaskState *state) {
    setCleanupCallback(&cleanupCameraRotationTask);
    state->rotationZ = 0;
    state->rotationY = 0;
    state->rotationX = 0;
    state->unkE = 0;
    state->unkC = 0;
    state->unkA = 0;
    state->angularVelocityZ = 0;
    state->angularVelocityY = 0;
    state->angularVelocityX = 0;
    setCallback(&updateCameraRotationTask);
}

void updateCameraRotationTask(CameraRotationTaskState *state) {
    Transform3D cameraTransform;
    s32 *positionPtr;

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
            state->unkE = 0;
            state->unkC = 0;
            state->unkA = 0;
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
    positionPtr = (s32 *)&gScaleMatrix.translation;

    createRotationMatrixXYZ((s16 *)&cameraTransform, state->rotationX, state->rotationY, state->rotationZ);

    cameraTransform.translation.x = 0;
    cameraTransform.translation.y = 0x4CCCC;
    cameraTransform.translation.z = 0;

    func_8006B084_6BC84((Transform3D *)(positionPtr - 5), &cameraTransform, &state->model->unkF0);
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

    gDPPipeSync(gRegionAllocPtr++);

    gDPSetTextureLUT(gRegionAllocPtr++, G_TT_RGBA16);

    gDPSetTextureImage(gRegionAllocPtr++, G_IM_FMT_CI, G_IM_SIZ_16b, 1, state->tableEntry.data_ptr);

    gDPSetTile(
        gRegionAllocPtr++,
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

    gDPLoadSync(gRegionAllocPtr++);

    loadBlockCmd = gRegionAllocPtr++;
    loadBlockCmd->words.w0 = 0xF3000000;
    gGraphicsMode = -1;
    widthDiv16 = state->tableEntry.width >> 4;
    dxtBase = 0x800;
    if (widthDiv16 != 0) {
        dxtBase = widthDiv16 + 0x7FF;
    }
    lrs = (((s32)((state->tableEntry.width * state->tableEntry.height) + 3)) >> 2) - 1;
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

    gDPPipeSync(gRegionAllocPtr++);

    line = (((state->tableEntry.width >> 1) + 7) >> 3) & 0x1FF;
    new_var = G_TX_NOMIRROR;
    gDPSetTile(gRegionAllocPtr++, G_IM_FMT_CI, G_IM_SIZ_4b, line, 0, G_TX_RENDERTILE, 0, 0, 0, 0, 0, 0, 0);

    gDPSetTileSize(
        gRegionAllocPtr++,
        G_TX_RENDERTILE,
        0,
        0,
        (state->tableEntry.width - 1) << 2,
        (state->tableEntry.height - 1) << 2
    );

    gDPSetTextureImage(gRegionAllocPtr++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, state->tableEntry.index_ptr);

    gDPTileSync(gRegionAllocPtr++);

    gDPSetTile(
        gRegionAllocPtr++,
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

    gDPLoadSync(gRegionAllocPtr++);

    gDPLoadTLUTCmd(gRegionAllocPtr++, G_TX_LOADTILE, 15);

    gDPPipeSync(gRegionAllocPtr++);

    if (state->lookAtMatrix == 0) {
        state->lookAtMatrix = arenaAlloc16(0x40);
        if (state->lookAtMatrix == 0) {
            return;
        }
        transform3DToN64Mtx(&state->transformMatrix, state->lookAtMatrix);
    }

    gDPPipeSync(gRegionAllocPtr++);

    gDPSetTexturePersp(gRegionAllocPtr++, G_TP_PERSP);

    gSPSegment(gRegionAllocPtr++, 0x02, state->segmentData);

    gSPMatrix(gRegionAllocPtr++, state->lookAtMatrix, (G_MTX_NOPUSH | G_MTX_LOAD) | G_MTX_MODELVIEW);

    gSPDisplayList(gRegionAllocPtr++, &D_8008C200_8CE00);
}

void initModelTransitionEffect(ModelTransitionEffectState *state) {
    setCleanupCallback(&cleanupModelTransitionEffect);
    state->animationTable = loadCompressedData(&_646CD0_ROM_START, &_646CD0_ROM_END, 0xE8);
    state->frameBuffer = loadCompressedData(&_49B500_ROM_START, &_49B500_ROM_END, 0xC00);
    if (state->model->index == 0x3E) {
        state->effectVariant = 0;
    } else {
        state->effectVariant = 1;
    }
    setCallback(&setupModelTransitionVariant);
}

void setupModelTransitionVariant(ModelTransitionEffectState *state) {
    getTableEntryByU16Index(state->animationTable, state->effectVariant != 0, &state->tableEntry);
    state->currentFrame = 0;
    setCallback(&updateModelTransitionEffect);
}

void updateModelTransitionEffect(ModelTransitionEffectState *state) {
    u8 unusedMatrixBuffer[0x20];
    s32 frameBeforeIncrement;
    s8 actionMode;
    void *unused;

    memcpy(&unusedMatrixBuffer, &identityMatrix, 0x20);

    if (state->model->isDestroyed == 1) {
        terminateCurrentTask();
        return;
    }

    actionMode = state->model->actionMode;
    if (state->model->actionMode == 0) {
        state->currentFrame = 0;
    } else if (actionMode < 0 || state->model->actionMode >= 3) {
        state->currentFrame = 0;
    } else {
        do {
            if (state->currentFrame == 0) {
                memcpy(&state->transformMatrix, &state->model->matrix18, 0x20);
                scaleMatrix(&state->transformMatrix, 0x1000, 0x1000, 0x1000);
            }
        } while (0);
        setModelDisplayEnabled(state->model, 0);
    }

    if (state->model->actionMode == 1) {
        frameBeforeIncrement = state->currentFrame;
        if (state->currentFrame < 7) {
            state->lookAtMatrix = NULL;
            state->segmentData = &state->frameBuffer[frameBeforeIncrement].unk0;
            debugEnqueueCallback(0, 1, &renderPalettedTexture, state);
            state->currentFrame++;
        }
    }
}

void cleanupModelTransitionEffect(ModelTransitionEffectState *state) {
    state->animationTable = freeNodeMemory(state->animationTable);
    state->frameBuffer = freeNodeMemory(state->frameBuffer);
}
