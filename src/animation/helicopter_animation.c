#include "animation/helicopter_animation.h"
#include "common.h"
#include "data/model_display_lists.h"
#include "graphics/displaylist.h"
#include "math/geometry.h"
#include "system/task_scheduler.h"
#include "ui/level_preview_3d.h"

void updateBobbingModelTask(BobbingModelTaskState *);
void cleanupBobbingModelTask(BobbingModelTaskState *);
void updateTiltingModelTask(BobbingModelTaskState *);
void cleanupTiltingModelTask(void);

void initBobbingModelTask(BobbingModelTaskState *state) {
    memcpy(&state->displayObject.transform, &identityMatrix, sizeof(Transform3D));
    state->displayObject.segment1 = loadAssetGroupDisplayList(state->model);
    state->displayObject.segment2 = loadAssetGroupCompressedData(state->model);
    state->displayObject.displayLists = &gBobbingModelOverlayDisplayLists;
    state->displayObject.segment3 = NULL;
    state->bobbingEnabled = 0;
    state->verticalOffset = 0;
    state->verticalVelocity = 0xA3D7;
    setCleanupCallback(&cleanupBobbingModelTask);
    setCallback(&updateBobbingModelTask);
}

void updateBobbingModelTask(BobbingModelTaskState *state) {
    Transform3D rotationMatrix;
    Transform3D combinedMatrix;
    s32 delta;
    s32 temp;

    if (state->model->isDestroyed == 1) {
        terminateCurrentTask();
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

    createXRotationMatrix(state->model->baseTransform.m, state->tiltAngle);
    state->model->baseTransform.translation.x = 0;
    state->model->baseTransform.translation.y = state->verticalOffset;
    state->model->baseTransform.translation.z = 0;

    state->rotationAngle += 0x2AA;
    createYRotationMatrix(&rotationMatrix, state->rotationAngle);

    rotationMatrix.translation.x = 0;
    rotationMatrix.translation.y = 0x499999;
    rotationMatrix.translation.z = 0;

    composeTransform3D(&rotationMatrix, &state->model->baseTransform, &combinedMatrix);
    composeTransform3D(&combinedMatrix, &state->model->transform, &state->displayObject.transform);
    enqueueModelDisplayList(state->model, &state->displayObject);
}

void cleanupBobbingModelTask(BobbingModelTaskState *state) {
    state->displayObject.segment1 = freeNodeMemory(state->displayObject.segment1);
    state->displayObject.segment2 = freeNodeMemory(state->displayObject.segment2);
}

void initTiltingModelTask(void) {
    setCleanupCallback(cleanupTiltingModelTask);
    setCallback(updateTiltingModelTask);
}

void updateTiltingModelTask(BobbingModelTaskState *state) {
    if (state->model->isDestroyed == 1) {
        terminateCurrentTask();
        return;
    }

    memcpy(&state->model->baseTransform, &identityMatrix, sizeof(Transform3D));

    if (state->model->actionMode == 0) {
        return;
    }
    if (state->model->actionMode == 1) {
        createXRotationMatrix(state->model->baseTransform.m, 0xFCE4);
        state->model->baseTransform.translation.z += 0x66666;
    }
}

void cleanupTiltingModelTask(void) {
}
