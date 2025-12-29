#include "90F0.h"
#include "common.h"
#include "geometry.h"
#include "task_scheduler.h"

extern u8 identityMatrix[];

typedef struct {
    u8 padding[0x16];
    u16 spriteIndex;
} OscillatingModelSpriteInfo;

typedef struct {
    u8 _pad0[0xC];
    s16 unkC;
    u8 _pad0E[0x2];
    OscillatingModelSpriteInfo *spriteInfo;
    u8 _pad14[0x18];
    s32 positionX;
    s32 positionY;
    s32 positionZ;
    u8 _pad38[0x4];
    s8 isDestroyed;
    s8 actionMode;
    s8 unk3E;
    s8 displayEnabled;
    u8 _pad40[0x48];
    s8 isVisible;
    u8 _pad89[0x67];
    Transform3D rotationMatrix;
} OscillatingModel;

typedef struct {
    OscillatingModel *model;
    u16 oscillationAngle;
} OscillatingModelTaskState;

void updateOscillatingModelTask(OscillatingModelTaskState *state);
void cleanupOscillatingModelTask(void);

typedef struct {
    OscillatingModel *model;
    SpriteAssetState spriteState;
    u8 spriteFlipped;
} OscillatingSpriteTaskState;

void updateOscillatingSpriteTask(OscillatingSpriteTaskState *state);
void cleanupOscillatingSpriteTask(OscillatingSpriteTaskState *state);

void initOscillatingModelTask(void) {
    setCleanupCallback(cleanupOscillatingModelTask);
    setCallback(updateOscillatingModelTask);
}

void updateOscillatingModelTask(OscillatingModelTaskState *state) {
    Transform3D local_buffer;
    s32 sinVal;
    s32 cosVal;
    s32 scaledCos;
    s32 scaledSin;

    memcpy(&local_buffer, identityMatrix, 0x20);

    if (state->model->isDestroyed == 1) {
        func_80069CF8_6A8F8();
        return;
    }

    memcpy(&state->model->rotationMatrix, identityMatrix, 0x20);

    if (state->model->actionMode == 0) {
        return;
    }
    if (state->model->actionMode != 1) {
        return;
    }

    state->oscillationAngle += 0x333;
    sinVal = approximateSin(state->oscillationAngle);
    cosVal = approximateCos(state->oscillationAngle);

    scaledCos = ((cosVal * 7 * 8 + cosVal) * 4 - cosVal) >> 12;
    scaledSin = ((sinVal * 7 * 8 + sinVal) * 4 - sinVal) >> 12;

    createRotationMatrixXZ(&state->model->rotationMatrix, -scaledCos, scaledSin);

    state->model->rotationMatrix.translation.y += 0x33333;
}

void cleanupOscillatingModelTask(void) {
}

void initOscillatingSpriteTask(OscillatingSpriteTaskState *state) {
    state->spriteFlipped = 0;
    loadSpriteAsset(&state->spriteState, 9);
    setSpriteAnimation(&state->spriteState, 0x10000, 0, -1);
    setCleanupCallback(cleanupOscillatingSpriteTask);
    setCallback(updateOscillatingSpriteTask);
}

void updateOscillatingSpriteTask(OscillatingSpriteTaskState *state) {
    s8 unused[2] = { 1, -1 };
    OscillatingModel *model;
    s32 posX, posY, posZ;

    model = state->model;

    if (model->isDestroyed == 1) {
        func_80069CF8_6A8F8();
        return;
    }

    switch (model->actionMode) {
        case 0:
            state->spriteFlipped = 0;
            break;
        case 1:
            setSpriteAnimation(&state->spriteState, 0x10000, 0, -1);
            break;
        case 2:
            setSpriteAnimation(&state->spriteState, 0x10000, 1, -1);
            break;
        case 3:
            state->spriteFlipped = 1;
            break;
        default:
            state->spriteFlipped = 0;
            break;
    }

    posX = state->model->positionX;
    posY = state->model->positionY;
    posZ = state->model->positionZ;

    updateSpriteAnimation(&state->spriteState, 0x10000);

    model = state->model;
    if (model->isVisible != 0) {
        if (model->displayEnabled != 0) {
            renderOpaqueSprite(
                &state->spriteState,
                model->spriteInfo->spriteIndex,
                posX,
                posY,
                posZ,
                0x13333,
                0x13333,
                0,
                state->spriteFlipped
            );
        }
    }
}

void cleanupOscillatingSpriteTask(OscillatingSpriteTaskState *state) {
    releaseNodeMemoryRef((void **)&state->spriteState);
}
