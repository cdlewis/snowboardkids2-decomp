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
    /* 0x00 */ SceneModel *model;
    /* 0x04 */ u8 transformMatrix[0x20];
    /* 0x24 */ void *renderSettings;
    /* 0x28 */ void *displayList;
    /* 0x2C */ void *vertexData;
    /* 0x30 */ s32 renderFlag;
    /* 0x34 */ u8 _pad34[0x8];
} QuadDisplayListElement; // Size: 0x3C (60 bytes)

extern s32 D_8009A8A4_9B4A4;
extern u8 identityMatrix[];
extern u8 D_80088640_89240[];

void updateQuadDisplayList(void);
void cleanupQuadDisplayList(QuadDisplayListElement *);
void updateRotationController(RotationControllerState *);
void cleanupRotationController(void);

void initializeQuadDisplayList(QuadDisplayListElement *elements) {
    s32 i;

    for (i = 0; i < 4; i++) {
        memcpy(elements[i].transformMatrix, identityMatrix, 0x20);
        elements[i].displayList = loadAssetGroupDisplayList(elements->model);
        elements[i].vertexData = loadAssetGroupVertexData(elements->model);
        elements[i].renderFlag = 0;
        elements[i].renderSettings = D_80088640_89240;
    }
    setCleanupCallback(cleanupQuadDisplayList);
    setCallback(updateQuadDisplayList);
}

INCLUDE_ASM("asm/nonmatchings/69B0", updateQuadDisplayList);

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
        func_80069CF8_6A8F8();
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

    positionPtr = &D_8009A8A4_9B4A4;
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
