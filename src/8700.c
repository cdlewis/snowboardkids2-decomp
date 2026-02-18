#include "20F0.h"
#include "common.h"
#include "geometry.h"
#include "rand.h"
#include "task_scheduler.h"

typedef struct {
    func_80002B50_3750_arg *model;
    u8 transformMatrix[0x20];
    s32 velocity;
    s16 rotationAngle;
    s16 delayTimer;
    s16 bounceCount;
} ModelScaleAnimationState;

void cleanupModelScaleAnimation(void);
void updateModelScaleAnimation(ModelScaleAnimationState *state);

void initModelScaleAnimation(ModelScaleAnimationState *state) {
    memcpy(&state->transformMatrix, &identityMatrix, 0x20);
    state->velocity = 0;
    state->rotationAngle = 0;
    state->delayTimer = 0;
    setCleanupCallback(cleanupModelScaleAnimation);
    setCallback(updateModelScaleAnimation);
}

void updateModelScaleAnimation(ModelScaleAnimationState *state) {
    func_80002B50_3750_arg *model;
    s32 scale;
    s32 newScale;
    s32 velocity;
    s16 angle;
    s16 delay;

    model = state->model;

    if (model->isDestroyed == 1) {
        terminateCurrentTask();
        return;
    }

    switch (model->actionMode) {
        case 0:
        default:
            state->velocity = 0;
            state->rotationAngle = 0;
            memcpy(&state->transformMatrix, &identityMatrix, 0x20);
            break;

        case 1:
            scale = *(s32 *)(state->transformMatrix + 0x18);
            state->rotationAngle = 0;
            state->velocity += 0xFFFF0000;
            memcpy(&state->transformMatrix, &identityMatrix, 0x20);
            *(s32 *)(state->transformMatrix + 0x18) = scale;
            velocity = state->velocity;
            newScale = scale + velocity;
            *(s32 *)(state->transformMatrix + 0x18) = newScale;
            if (newScale > 0) {
                break;
            }
            *(s32 *)(state->transformMatrix + 0x18) = 0;
            state->velocity = 0x8C000;
            break;

        case 2:
            scale = *(s32 *)(state->transformMatrix + 0x18);
            state->rotationAngle = 0;
            state->velocity += 0xFFFE0000;
            memcpy(&state->transformMatrix, &identityMatrix, 0x20);
            *(s32 *)(state->transformMatrix + 0x18) = scale;
            velocity = state->velocity;
            newScale = scale + velocity;
            *(s32 *)(state->transformMatrix + 0x18) = newScale;
            if (newScale > 0) {
                break;
            }
            *(s32 *)(state->transformMatrix + 0x18) = 0;
            state->velocity = 0x8C000;
            break;

        case 3:
            memcpy(&state->transformMatrix, &identityMatrix, 0x20);
            angle = state->rotationAngle - 0x111;
            state->rotationAngle = angle;
            if (angle < -0x800) {
                state->rotationAngle = -0x800;
            }
            createXRotationMatrix((s16(*)[3])state->transformMatrix, state->rotationAngle);
            *(s32 *)(state->transformMatrix + 0x18) += 0x99999;
            break;

        case 4:
            delay = state->delayTimer;
            if (delay == 0) {
                scale = *(s32 *)(state->transformMatrix + 0x18);
                state->rotationAngle = 0;
                state->velocity += 0xFFFF0000;
                memcpy(&state->transformMatrix, &identityMatrix, 0x20);
                *(s32 *)(state->transformMatrix + 0x18) = scale;
                velocity = state->velocity;
                newScale = scale + velocity;
                *(s32 *)(state->transformMatrix + 0x18) = newScale;
                if (newScale > 0) {
                    break;
                }
                *(s32 *)(state->transformMatrix + 0x18) = 0;
                state->velocity = 0x2EAAA;
                state->delayTimer = (((randA() & 0xFF) % 25) & 0xFF) + 0xA;
            } else {
                state->delayTimer = delay - 1;
            }
            break;

        case 5:
            state->bounceCount = 3;
            state->model->actionMode = 6;
            break;

        case 6:
            scale = *(s32 *)(state->transformMatrix + 0x18);
            state->rotationAngle = 0;
            state->velocity += 0xFFFD0000;
            memcpy(&state->transformMatrix, &identityMatrix, 0x20);
            *(s32 *)(state->transformMatrix + 0x18) = scale;
            velocity = state->velocity;
            newScale = scale + velocity;
            *(s32 *)(state->transformMatrix + 0x18) = newScale;
            if (newScale > 0) {
                break;
            }
            state->bounceCount--;
            *(s32 *)(state->transformMatrix + 0x18) = 0;
            if (state->bounceCount == 0) {
                state->model->actionMode = 0;
            } else {
                state->velocity = 0x146AAA;
            }
            break;
    }

    memcpy(&state->model->unkF0, &state->transformMatrix, 0x20);
}

void cleanupModelScaleAnimation(void) {
}
