#include "20F0.h"
#include "common.h"
#include "geometry.h"
#include "task_scheduler.h"

typedef struct {
    func_80002B50_3750_arg *model;
    DisplayListObject displayObject;
    u16 rotationAngle;
    s16 tiltAngle;
    s8 bobbingEnabled;
    u8 _pad45[3];
    s32 verticalOffset;
    s32 verticalVelocity;
} BobbingModelState;

extern u8 identityMatrix[];
extern void *D_800882B0_88EB0;
void updateBobbingModelTask(BobbingModelState *);

typedef struct {
    SceneModel *model;
    u8 transformMatrix[0x20];
    void *materialData;
    void *displayList;
    void *vertexData;
    s32 renderFlags;
    u8 _pad34[0x10];
    u8 bobbingEnabled;
    u8 _pad45[3];
    s32 verticalOffset;
    s32 verticalVelocity;
} BobbingModelTaskState;

void cleanupBobbingModelTask(BobbingModelTaskState *);

void initBobbingModelTask(BobbingModelTaskState *state) {
    memcpy(state->transformMatrix, identityMatrix, 0x20);
    state->displayList = loadAssetGroupDisplayList(state->model);
    state->vertexData = loadAssetGroupVertexData(state->model);
    state->materialData = &D_800882B0_88EB0;
    state->renderFlags = 0;
    state->bobbingEnabled = 0;
    state->verticalOffset = 0;
    state->verticalVelocity = 0xA3D7;
    setCleanupCallback(&cleanupBobbingModelTask);
    setCallback(&updateBobbingModelTask);
}

void updateBobbingModelTask(BobbingModelState *state) {
    Mat3x3Padded rotationMatrix;
    Mat3x3Padded combinedMatrix;
    s32 delta;
    s32 temp;

    if (state->model->isDestroyed == 1) {
        func_80069CF8_6A8F8();
        return;
    }

    switch (state->model->actionMode) {
        case 0:
        default:
            state->tiltAngle = 0;
            break;
        case 1:
            delta = -state->tiltAngle / 12;
            if (delta == 0) {
                state->tiltAngle = 0;
            } else {
                state->tiltAngle += delta;
            }
            break;
        case 2:
            delta = (0x1C7 - state->tiltAngle) / 12;
            if (delta == 0) {
                state->tiltAngle = 0x1C7;
            } else {
                state->tiltAngle += delta;
            }
            break;
        case 3:
            state->bobbingEnabled = 1;
            break;
        case 4:
            state->bobbingEnabled = 0;
            state->verticalVelocity = 0xA3D7;
            break;
    }

    if (state->bobbingEnabled != 0) {
        state->verticalOffset = state->verticalOffset + state->verticalVelocity;
        if (state->verticalOffset > 0) {
            state->verticalVelocity -= 0x1062;
        } else {
            state->verticalVelocity += 0x1062;
        }
    } else {
        temp = -state->verticalOffset;
        if (-state->verticalOffset < 0) {
            temp += 0xF;
        }
        state->verticalOffset = temp >> 4;
    }

    createXRotationMatrix(state->model->unkF0, state->tiltAngle);
    state->model->unk104 = 0;
    state->model->unk108 = state->verticalOffset;
    state->model->unk10C = 0;

    state->rotationAngle += 0x2AA;
    createYRotationMatrix(&rotationMatrix, state->rotationAngle);

    rotationMatrix.unk14 = 0;
    rotationMatrix.unk18 = 0x499999;
    rotationMatrix.unk1C = 0;

    func_8006B084_6BC84(&rotationMatrix, state->model->unkF0, &combinedMatrix);
    func_8006B084_6BC84(&combinedMatrix, state->model->matrix18, &state->displayObject);
    enqueueModelDisplayList(state->model, &state->displayObject);
}

void cleanupBobbingModelTask(BobbingModelTaskState *state) {
    state->displayList = freeNodeMemory(state->displayList);
    state->vertexData = freeNodeMemory(state->vertexData);
}

void cleanupTiltingModelTask(void);
void updateTiltingModelTask(BobbingModelTaskState *);

void initTiltingModelTask(void) {
    setCleanupCallback(cleanupTiltingModelTask);
    setCallback(updateTiltingModelTask);
}

void updateTiltingModelTask(BobbingModelTaskState *state) {
    if (state->model->isDestroyed == 1) {
        func_80069CF8_6A8F8();
        return;
    }

    memcpy(&state->model->unkF0, identityMatrix, 0x20);

    if (state->model->actionMode == 0) {
        return;
    }
    if (state->model->actionMode == 1) {
        createXRotationMatrix(state->model->unkF0, 0xFCE4);
        state->model->unk10C += 0x66666;
    }
}

void cleanupTiltingModelTask(void) {
}
