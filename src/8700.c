#include "common.h"
#include "task_scheduler.h"

extern u8 identityMatrix[];
void cleanupModelScaleAnimation(void);
extern void updateModelScaleAnimation(void);

typedef struct {
    u8 _pad[0x4];
    u8 transformMatrix[0x20];
    s32 scaleVelocity;
    s16 rotationAngle;
    s16 delayTimer;
} ModelScaleAnimationState;

void initModelScaleAnimation(ModelScaleAnimationState *state) {
    memcpy(&state->transformMatrix, identityMatrix, 0x20);
    state->scaleVelocity = 0;
    state->rotationAngle = 0;
    state->delayTimer = 0;
    setCleanupCallback(cleanupModelScaleAnimation);
    setCallback(updateModelScaleAnimation);
}

INCLUDE_ASM("asm/nonmatchings/8700", updateModelScaleAnimation);

void cleanupModelScaleAnimation(void) {
}
