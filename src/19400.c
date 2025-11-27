#include "20F0.h"
#include "D_800AFE8C_A71FC_type.h"
#include "common.h"
#include "task_scheduler.h"

extern void func_80018A5C_1965C(void);
extern void func_80018858_19458(void);

void func_80018800_19400(void **arg0) {
    void *allocation = getCurrentAllocation();

    *arg0 = func_8000198C_258C(D_800AFE8C_A71FC->unk9 + 0x50, allocation);
    setCleanupCallback(&func_80018A5C_1965C);
    setCallback(&func_80018858_19458);
}

INCLUDE_ASM("asm/nonmatchings/19400", func_80018858_19458);

extern void func_80001650_2250(SceneModel *arg0);
extern void func_80018904_19504(void *arg0);

typedef struct {
    u8 padding[0x3B0];
    applyTransformToModel_arg1 unk3B0;
    u8 padding2[0x2C];
    u8 unk400;
} func_80018898_19498_state;

typedef struct {
    SceneModel *unk0;
    u8 unk4;
} func_80018898_19498_arg;

void func_80018898_19498(func_80018898_19498_arg *arg0) {
    func_80018898_19498_state *state = (func_80018898_19498_state *)getCurrentAllocation();

    func_80001650_2250(arg0->unk0);
    applyTransformToModel(arg0->unk0, &state->unk3B0);
    arg0->unk4 = state->unk400;
    func_800021B8_2DB8(arg0->unk0, state->unk400);
    updateModelGeometry(arg0->unk0);
    setCallback(&func_80018904_19504);
}

INCLUDE_ASM("asm/nonmatchings/19400", func_80018904_19504);

INCLUDE_ASM("asm/nonmatchings/19400", func_80018A5C_1965C);

INCLUDE_ASM("asm/nonmatchings/19400", func_80018A90_19690);

INCLUDE_ASM("asm/nonmatchings/19400", func_80018B90_19790);

INCLUDE_ASM("asm/nonmatchings/19400", func_80018C9C_1989C);
