#include "20F0.h"
#include "D_800AFE8C_A71FC_type.h"
#include "common.h"
#include "task_scheduler.h"

extern void func_80001640_2240(SceneModel *arg0);
extern void func_80001650_2250(SceneModel *arg0);

typedef struct {
    u8 padding[0x3B0];
    applyTransformToModel_arg1 unk3B0;
    u8 padding2[0x2C];
    u8 unk400;
    u8 unk401;
    u8 padding3[0x1];
    u8 unk403;
    u8 unk404;
    u8 padding4[0x24];
    u8 unk429;
} func_80018898_19498_state;

typedef struct {
    SceneModel *unk0;
    u8 unk4;
    u8 unk5;
} func_80018898_19498_arg;

void func_80018858_19458(func_80018898_19498_arg *arg0);
void func_80018898_19498(func_80018898_19498_arg *arg0);
void func_80018904_19504(func_80018898_19498_arg *arg0);
void func_80018A5C_1965C(func_80018898_19498_arg *arg0);

void func_80018800_19400(void **arg0) {
    void *allocation = getCurrentAllocation();

    *arg0 = func_8000198C_258C(D_800AFE8C_A71FC->unk9 + 0x50, allocation);
    setCleanupCallback(&func_80018A5C_1965C);
    setCallback(&func_80018858_19458);
}

void func_80018858_19458(func_80018898_19498_arg *arg0) {
    func_80018898_19498_state *allocation = (func_80018898_19498_state *)getCurrentAllocation();

    if (allocation->unk429 == 0) {
        arg0->unk5 = 0;
        setCallback(&func_80018898_19498);
    }
}

void func_80018898_19498(func_80018898_19498_arg *arg0) {
    func_80018898_19498_state *state = (func_80018898_19498_state *)getCurrentAllocation();

    func_80001650_2250(arg0->unk0);
    applyTransformToModel(arg0->unk0, &state->unk3B0);
    arg0->unk4 = state->unk400;
    func_800021B8_2DB8(arg0->unk0, state->unk400);
    updateModelGeometry(arg0->unk0);
    setCallback(&func_80018904_19504);
}

void func_80018904_19504(func_80018898_19498_arg *arg0) {
    func_80018898_19498_state *state = (func_80018898_19498_state *)getCurrentAllocation();
    s32 rotationResult;
    u8 anim;

    applyTransformToModel(arg0->unk0, &state->unk3B0);

    anim = state->unk403;
    if (anim != arg0->unk5) {
        arg0->unk5 = anim;
        func_80001688_2288(arg0->unk0, (s8)anim);
    }

    if (state->unk400 != arg0->unk4 && arg0->unk4 != 1) {
        arg0->unk4 = state->unk400;
        if (state->unk400 == 2 || state->unk400 == 4) {
            func_80002260_2E60(arg0->unk0, state->unk400, -1, -1, -1, 0);
        } else {
            func_800021B8_2DB8(arg0->unk0, state->unk400);
        }
    }

    rotationResult = clearModelRotation(arg0->unk0);
    updateModelGeometry(arg0->unk0);

    if (rotationResult != 0) {
        state->unk401 = 1;
        if (arg0->unk4 == 1) {
            state->unk404 = 0;
            arg0->unk4 = state->unk400;
            if (state->unk400 == 2 || state->unk400 == 4) {
                func_80002260_2E60(arg0->unk0, state->unk400, -1, -1, -1, 0);
            } else {
                func_800021B8_2DB8(arg0->unk0, state->unk400);
            }
        }
    }
}

void func_80018A5C_1965C(func_80018898_19498_arg *arg0) {
    func_80001640_2240(arg0->unk0);
    arg0->unk0 = func_80002014_2C14(arg0->unk0);
}

INCLUDE_ASM("asm/nonmatchings/19400", func_80018A90_19690);

INCLUDE_ASM("asm/nonmatchings/19400", func_80018B90_19790);

void func_80018C9C_1989C(void) {
}
