#include "20F0.h"
#include "common.h"
#include "geometry.h"
#include "task_scheduler.h"

typedef struct {
    /* 0x00 */ SceneModel *model;
    /* 0x04 */ s16 currentRotationX;
    /* 0x06 */ s16 currentRotationY;
    /* 0x08 */ s16 targetRotationX;
    /* 0x0A */ s16 targetRotationY;
} RotationControllerState;

typedef struct {
    /* 0x00 */ func_80002B50_3750_arg *model;
    /* 0x04 */ QuadDisplayListElement elements[4];
    /* 0xF4 */ u16 rotationAngle;
    /* 0xF6 */ u16 rotationSpeed;
} QuadDisplayListState;

extern s32 gTempPosition;
extern Transform3D identityMatrix;
extern u8 D_80088640_89240[];
extern s32 D_8008C120_8CD20[];

void updateQuadDisplayList(QuadDisplayListState *);
void cleanupQuadDisplayList(QuadDisplayListElement *);
void updateRotationController(RotationControllerState *);
void cleanupRotationController(void);

void initializeQuadDisplayList(QuadDisplayListElement *elements) {
    s32 i;

    for (i = 0; i < 4; i++) {
        memcpy(elements[i].transformMatrix, &identityMatrix, 0x20);
        elements[i].displayList = loadAssetGroupDisplayList(elements->model);
        elements[i].vertexData = loadAssetGroupCompressedData(elements->model);
        elements[i].renderFlag = 0;
        elements[i].renderSettings = D_80088640_89240;
    }
    setCleanupCallback(cleanupQuadDisplayList);
    setCallback(updateQuadDisplayList);
}

void updateQuadDisplayList(QuadDisplayListState *state) {
    Transform3D rotationMatrix;
    func_80002B50_3750_arg *model;
    s32 i;
    u8 *displayListElement;
    s32 elementOffset;
    s32 *translationData;
    s32 positiveRotationIndex;

    memcpy(&rotationMatrix, &identityMatrix, 0x20);

    model = state->model;
    if (model->isDestroyed == 1) {
        terminateCurrentTask();
        return;
    }

    switch (model->actionMode) {
        default:
        case 0:
            state->rotationSpeed = 0;
            state->rotationAngle = 0;
            break;
        case 1: {
            u16 newSpeed = state->rotationSpeed + 0x5B;
            state->rotationSpeed = newSpeed;
            if ((s16)newSpeed >= 0x2AB) {
                state->rotationSpeed = 0x2AA;
            }
            state->rotationAngle = state->rotationAngle + state->rotationSpeed;
        } break;
    }

    i = 0;
    positiveRotationIndex = 2;
    elementOffset = 4;
    translationData = D_8008C120_8CD20;
loop:
    if (i == 0) {
        goto positive;
    }
    if (i != positiveRotationIndex) {
        goto negative;
    }
positive:
    createCombinedRotationMatrix(&rotationMatrix, state->rotationAngle, 0);
    goto after;
negative:
    createCombinedRotationMatrix(&rotationMatrix, -state->rotationAngle, 0x1000);
after:
    displayListElement = (u8 *)state + elementOffset;
    memcpy(&rotationMatrix.translation, translationData, 0xC);
    func_8006B084_6BC84(&rotationMatrix, state->model->matrix18, displayListElement);
    elementOffset += 0x3C;
    translationData += 3;
    enqueueModelDisplayList(state->model, (DisplayListObject *)displayListElement);
    if (++i < 4) {
        goto loop;
    }
}

void cleanupQuadDisplayList(QuadDisplayListElement *elements) {
    s32 i = 0;

    do {
        elements[i].displayList = freeNodeMemory(elements[i].displayList);
        elements[i].vertexData = freeNodeMemory(elements[i].vertexData);
    } while (++i < 4);
}

void initializeRotationController(RotationControllerState *state) {
    setCleanupCallback(cleanupRotationController);
    state->targetRotationY = 0;
    state->targetRotationX = 0;
    state->currentRotationY = 0;
    state->currentRotationX = 0;
    setCallback(updateRotationController);
}

void updateRotationController(RotationControllerState *state) {
    Transform3D rotationMatrix;
    SceneModel *model;
    s16 currentX;
    s32 deltaX;
    s32 deltaY;
    s32 diffX;
    s32 diffY;
    s16 targetValue;
    s32 *positionPtr;

    model = state->model;
    if (model->isDestroyed == 1) {
        terminateCurrentTask();
        return;
    }

    switch (model->actionMode) {
        default:
        case 0:
            state->targetRotationY = 0;
            state->targetRotationX = 0;
            state->currentRotationY = 0;
            state->currentRotationX = 0;
            break;
        case 1:
            state->targetRotationX = 0x2000;
            break;
        case 2:
            state->targetRotationX = -0x2000;
            break;
        case 3:
            targetValue = 0x2000;
            goto setTargetY;
        case 4:
            targetValue = -0x2000;
        setTargetY:
            state->targetRotationY = targetValue;
            break;
    }

    currentX = state->currentRotationX;
    diffX = state->targetRotationX - currentX;
    if (diffX < 0) {
        diffX += 7;
    }
    diffY = state->targetRotationY - state->currentRotationY;
    deltaX = diffX >> 3;
    if (diffY < 0) {
        diffY += 7;
    }
    state->currentRotationX = currentX + deltaX;
    deltaY = diffY >> 3;
    state->currentRotationY = (u16)state->currentRotationY + deltaY;
    if (deltaX == 0) {
        state->targetRotationX = 0;
        state->currentRotationX = 0;
    }
    if (deltaY == 0) {
        state->targetRotationY = 0;
        state->currentRotationY = 0;
    }

    positionPtr = &gTempPosition;
    *positionPtr = 0;
    *(positionPtr + 1) = (s32)0xFFF00000;
    *(positionPtr + 2) = 0;

    createRotationMatrixYX(&rotationMatrix, (u16)state->currentRotationY, (u16)state->currentRotationX);

    rotationMatrix.translation.x = 0;
    rotationMatrix.translation.y = 0x100000;
    rotationMatrix.translation.z = 0;

    func_8006B084_6BC84(positionPtr - 5, &rotationMatrix, (u8 *)state->model + 0xF0);
}

void cleanupRotationController(void) {
}
