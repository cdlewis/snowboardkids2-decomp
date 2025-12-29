#include "common.h"
#include "geometry.h"
#include "task_scheduler.h"

extern u8 identityMatrix[];
extern s32 gSteppedMatrixTranslations;
extern u16 gSteppedMatrixAngles;
extern s32 gSteppedMatrixStepCount;

void cleanupSteppedMatrixController(void);

typedef struct {
    u8 padding[0x3C];
    s8 isDestroyed;
    s8 actionMode;
    u8 padding2[0xB2];
    u8 transformMatrix[0x20];
} SteppedMatrixOwner;

typedef struct {
    SteppedMatrixOwner *owner;
    Transform3D matrix;
    s16 frameDelay;
    u8 padding[0x2];
    s16 stepIndex;
} SteppedMatrixState;

void updateSteppedMatrixController(SteppedMatrixState *state);

void initSteppedMatrixController(SteppedMatrixState *state) {
    memcpy(&state->matrix, identityMatrix, 0x20);
    state->frameDelay = 0;
    state->stepIndex = 0;
    setCleanupCallback(cleanupSteppedMatrixController);
    setCallback(updateSteppedMatrixController);
}

void updateSteppedMatrixController(SteppedMatrixState *state) {
    Transform3D *matrix;
    s32 translation;

    if (state->owner->isDestroyed == 1) {
        func_80069CF8_6A8F8();
        return;
    }

    if (state->owner->actionMode == 0) {
        goto copy_identity;
    }
    if (state->owner->actionMode == 1) {
        goto do_rotation;
    }

copy_identity:
    memcpy(&state->matrix, identityMatrix, 0x20);
    goto final_copy;

do_rotation:
    if (state->frameDelay == 0) {
        matrix = &state->matrix;
        memcpy(matrix, identityMatrix, 0x20);
        createZRotationMatrix(matrix, (&gSteppedMatrixAngles)[state->stepIndex * 4]);
        translation = (&gSteppedMatrixTranslations)[state->stepIndex * 2];
        state->frameDelay = 4;
        state->matrix.translation.y = translation;
        state->stepIndex = (state->stepIndex + 1) % gSteppedMatrixStepCount;
    }
    state->frameDelay -= 1;

final_copy:
    memcpy(state->owner->transformMatrix, &state->matrix, 0x20);
}

void cleanupSteppedMatrixController(void) {
}
