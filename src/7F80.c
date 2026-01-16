#include "19E80.h"
#include "20F0.h"
#include "6E840.h"
#include "common.h"
#include "geometry.h"
#include "rom_loader.h"
#include "task_scheduler.h"

USE_ASSET(_49B500);
USE_ASSET(_646CD0);

typedef struct {
    s32 unk0;
    u8 padding[0x17C];
} TransitionEffectFrame;

typedef struct {
    SceneModel *model;
    void *animationTable;
    OutputStruct_19E80 tableEntry;
    Transform3D transformMatrix;
    s32 frameIndex;
    s32 *frameData;
    s32 currentFrame;
    s32 effectVariant;
    TransitionEffectFrame *frameBuffer;
} ModelTransitionEffectState;

typedef struct {
    s32 unk0;
    s32 unk4;
    /* 0x08 */ void *textureData;
    /* 0x0C */ void *paletteData;
    /* 0x10 */ u16 width;
    /* 0x12 */ u16 height;
    /* 0x14 */ Transform3D rotationMatrix;
    /* 0x34 */ Mtx *lookAtMatrix;
    /* 0x38 */ void *segmentData;
} PalettedTextureState;

typedef struct {
    SceneModel *model;
    s16 rotationX;
    s16 rotationY;
    s16 rotationZ;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 angularVelocityX;
    s16 angularVelocityY;
    s16 angularVelocityZ;
    s16 unk16;
} CameraRotationTaskState;

void setupModelTransitionVariant(ModelTransitionEffectState *);
void updateModelTransitionEffect(ModelTransitionEffectState *);
void cleanupModelTransitionEffect(ModelTransitionEffectState *);
void func_800073E0_7FE0(CameraRotationTaskState *);
void cleanupCameraRotationTask(void);
void renderPalettedTexture(PalettedTextureState *);

extern u8 identityMatrix[];
extern s32 D_8008C200_8CE00[];
extern s16 gGraphicsMode;
extern Gfx *gRegionAllocPtr;

extern s32 D_8009A8A4_9B4A4;
extern s32 D_8009A8A8_9B4A8;
extern s32 D_8009A8AC_9B4AC;
void createRotationMatrixXYZ(Transform3D *, u16, u16, u16);

void initCameraRotationTask(CameraRotationTaskState *arg0) {
    setCleanupCallback(&cleanupCameraRotationTask);
    arg0->rotationZ = 0;
    arg0->rotationY = 0;
    arg0->rotationX = 0;
    arg0->unkE = 0;
    arg0->unkC = 0;
    arg0->unkA = 0;
    arg0->angularVelocityZ = 0;
    arg0->angularVelocityY = 0;
    arg0->angularVelocityX = 0;
    setCallback(&func_800073E0_7FE0);
}

void func_800073E0_7FE0(CameraRotationTaskState *state) {
    Transform3D rotationMatrix;
    s16 velocity;
    s32 *positionPtr;

    if (state->model->isDestroyed == 1) {
        func_80069CF8_6A8F8();
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

    positionPtr = &D_8009A8A4_9B4A4;
    *positionPtr = 0;
    *(positionPtr + 1) = (s32)0xFFFB3334;
    *(positionPtr + 2) = 0;

    createRotationMatrixXYZ(&rotationMatrix, state->rotationX, state->rotationY, state->rotationZ);

    rotationMatrix.translation.x = 0;
    rotationMatrix.translation.y = 0x4CCCC;
    rotationMatrix.translation.z = 0;

    func_8006B084_6BC84(positionPtr - 5, &rotationMatrix, (u8 *)state->model + 0xF0);
}

void cleanupCameraRotationTask(void) {
}

void renderPalettedTexture(PalettedTextureState *state) {
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

    gDPSetTextureImage(gRegionAllocPtr++, G_IM_FMT_CI, G_IM_SIZ_16b, 1, state->textureData);

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
    widthDiv16 = state->width >> 4;
    dxtBase = 0x800;
    if (widthDiv16 != 0) {
        dxtBase = widthDiv16 + 0x7FF;
    }
    lrs = (((s32)((state->width * state->height) + 3)) >> 2) - 1;
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

    line = (((state->width >> 1) + 7) >> 3) & 0x1FF;
    new_var = G_TX_NOMIRROR;
    gDPSetTile(gRegionAllocPtr++, G_IM_FMT_CI, G_IM_SIZ_4b, line, 0, G_TX_RENDERTILE, 0, 0, 0, 0, 0, 0, 0);

    gDPSetTileSize(gRegionAllocPtr++, G_TX_RENDERTILE, 0, 0, (state->width - 1) << 2, (state->height - 1) << 2);

    gDPSetTextureImage(gRegionAllocPtr++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, state->paletteData);

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
        transform3DToN64Mtx(&state->rotationMatrix, state->lookAtMatrix);
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
        func_80069CF8_6A8F8();
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
                memcpy(&state->transformMatrix, &state->model->unk18, 0x20);
                scaleMatrix(&state->transformMatrix, 0x1000, 0x1000, 0x1000);
            }
        } while (0);
        setModelDisplayEnabled(state->model, 0);
    }

    if (state->model->actionMode == 1) {
        frameBeforeIncrement = state->currentFrame;
        if (state->currentFrame < 7) {
            state->frameIndex = 0;
            state->frameData = &state->frameBuffer[frameBeforeIncrement].unk0;
            debugEnqueueCallback(0, 1, &renderPalettedTexture, state);
            state->currentFrame++;
        }
    }
}

void cleanupModelTransitionEffect(ModelTransitionEffectState *state) {
    state->animationTable = freeNodeMemory(state->animationTable);
    state->frameBuffer = freeNodeMemory(state->frameBuffer);
}
