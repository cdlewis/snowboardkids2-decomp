#include "20F0.h"
#include "3E160.h"
#include "6E840.h"
#include "D_800AFE8C_A71FC_type.h"
#include "common.h"
#include "gamestate.h"
#include "geometry.h"
#include "graphics.h"
#include "task_scheduler.h"

extern u8 identityMatrix[];

typedef struct {
    u8 padding[0x3B0];
    applyTransformToModel_arg1 transform;
    u8 padding2[0x2C];
    u8 animState;
    u8 rotationComplete;
    u8 padding3[0x1];
    u8 animIndex;
    u8 unk404;
    u8 padding4[0x24];
    u8 isLoading;
} StoryMapCharacterState;

typedef struct {
    SceneModel *model;
    u8 currentAnimState;
    u8 currentAnimIndex;
} StoryMapCharacterArg;

void awaitStoryMapCharacterReady(StoryMapCharacterArg *arg0);
void initStoryMapCharacter(StoryMapCharacterArg *arg0);
void updateStoryMapCharacter(StoryMapCharacterArg *arg0);
void cleanupStoryMapCharacter(StoryMapCharacterArg *arg0);

void func_80018800_19400(void **arg0) {
    void *allocation = getCurrentAllocation();

    *arg0 = createSceneModel(D_800AFE8C_A71FC->unk9[0] + 0x50, allocation);
    setCleanupCallback(&cleanupStoryMapCharacter);
    setCallback(&awaitStoryMapCharacterReady);
}

void awaitStoryMapCharacterReady(StoryMapCharacterArg *arg0) {
    StoryMapCharacterState *state = (StoryMapCharacterState *)getCurrentAllocation();

    if (state->isLoading == 0) {
        arg0->currentAnimIndex = 0;
        setCallback(&initStoryMapCharacter);
    }
}

void initStoryMapCharacter(StoryMapCharacterArg *arg0) {
    StoryMapCharacterState *state = (StoryMapCharacterState *)getCurrentAllocation();

    clearModelPendingDestroy(arg0->model);
    applyTransformToModel(arg0->model, &state->transform.unk0);
    arg0->currentAnimState = state->animState;
    setModelAnimation(arg0->model, state->animState);
    updateModelGeometry(arg0->model);
    setCallback(&updateStoryMapCharacter);
}

void updateStoryMapCharacter(StoryMapCharacterArg *arg0) {
    StoryMapCharacterState *state = (StoryMapCharacterState *)getCurrentAllocation();
    s32 rotationResult;
    u8 anim;

    applyTransformToModel(arg0->model, &state->transform.unk0);

    anim = state->animIndex;
    if (anim != arg0->currentAnimIndex) {
        arg0->currentAnimIndex = anim;
        setAnimationIndex(arg0->model, (s8)anim);
    }

    if (state->animState != arg0->currentAnimState && arg0->currentAnimState != 1) {
        arg0->currentAnimState = state->animState;
        if (state->animState == 2 || state->animState == 4) {
            setModelAnimationEx(arg0->model, state->animState, -1, -1, -1, 0);
        } else {
            setModelAnimation(arg0->model, state->animState);
        }
    }

    rotationResult = clearModelRotation(arg0->model);
    updateModelGeometry(arg0->model);

    if (rotationResult != 0) {
        state->rotationComplete = 1;
        if (arg0->currentAnimState == 1) {
            state->unk404 = 0;
            arg0->currentAnimState = state->animState;
            if (state->animState == 2 || state->animState == 4) {
                setModelAnimationEx(arg0->model, state->animState, -1, -1, -1, 0);
            } else {
                setModelAnimation(arg0->model, state->animState);
            }
        }
    }
}

void cleanupStoryMapCharacter(StoryMapCharacterArg *arg0) {
    setModelPendingDestroy(arg0->model);
    arg0->model = destroySceneModel(arg0->model);
}

typedef struct {
    /* 0x00 */ s16 matrix[9]; // 3x3 matrix (0x12 bytes)
    /* 0x12 */ s16 pad12;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s16 unk30;
    /* 0x32 */ s16 unk32;
    /* 0x34 */ s16 pad34[2];
    /* 0x38 */ s16 unk38;
    /* 0x3A */ s16 unk3A;
    /* 0x3C */ s16 unk3C;
    /* 0x3E */ s16 pad3E;
    /* 0x40 */ s16 *unk40;
    /* 0x44 */ s16 frameData[1];
} func_80018A90_19690_arg;

void func_80018B90_19790(func_80018A90_19690_arg *arg0);

void func_80018A90_19690(func_80018A90_19690_arg *arg0) {
    GameState *allocation;
    s16 temp;
    Transform3D sp20;
    Transform3D sp40;

    allocation = (GameState *)getCurrentAllocation();

    arg0->unk20 = 0;
    arg0->unk24 = 0x200000;
    arg0->unk28 = 0;
    memcpy(arg0, identityMatrix, 0x20);

    arg0->unk14 = 0;
    arg0->unk18 = 0;
    arg0->unk1C = 0x280000;
    arg0->unk2C = 0;
    arg0->unk30 = 0;

    temp = allocation->unk3F4;
    arg0->unk32 = temp;
    if (temp >= 0x1001) {
        arg0->unk32 = temp - 0x2000;
    }

    arg0->unk38 = -0x88;
    arg0->unk3A = -0x60;
    arg0->unk3C = 0;
    arg0->unk40 = arg0->frameData;

    func_8006BEDC_6CADC(&sp20, arg0->unk20, arg0->unk24, arg0->unk28, arg0->unk2C, arg0->unk30, arg0->unk32);
    func_8006B084_6BC84(arg0, &sp20, &sp40);
    func_8006FD3C_7093C(allocation->unkDA, &sp40);
    setCallback(&func_80018B90_19790);
}

void func_80018B90_19790(func_80018A90_19690_arg *arg0) {
    GameState *allocation;
    s16 temp;
    s16 currentAngle;
    s16 diff;
    Transform3D sp20;
    Transform3D sp40;

    allocation = (GameState *)getCurrentAllocation();

    temp = allocation->unk3F4;
    if (temp >= 0x1001) {
        temp -= 0x2000;
    }
    currentAngle = temp;

    diff = abs(currentAngle - arg0->unk32);

    if (diff >= 0x100) {
        if (currentAngle < arg0->unk32) {
            arg0->unk32 = temp + 0x100;
        } else {
            arg0->unk32 = temp - 0x100;
        }
    }

    if (arg0->unk32 >= 0x660) {
        arg0->unk32 = 0x65F;
    } else if (arg0->unk32 < -0x65F) {
        arg0->unk32 = -0x65F;
    }

    func_8006BEDC_6CADC(&sp20, arg0->unk20, arg0->unk24, arg0->unk28, arg0->unk2C, arg0->unk30, arg0->unk32);
    func_8006B084_6BC84(arg0, &sp20, &sp40);
    func_80056914_57514(&sp40);
    func_8006FD3C_7093C(allocation->unkDA, &sp40);
}

void func_80018C9C_1989C(void) {
}
