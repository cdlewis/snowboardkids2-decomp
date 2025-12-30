#include "cutscene/cutscene_sys.h"
#include "1DFAA0.h"
#include "6E840.h"
#include "CBA0.h"
#include "DC90.h"
#include "task_scheduler.h"
extern u8 D_800BA5C0_1E7670[];
extern s32 gButtonsPressed[];

typedef struct {
    CutsceneManager *unk0;
    u8 padding[0xA];
    s16 unkE;
} cutsceneSysWait_exec_task;

typedef struct {
    u8 padding[0xFF7];
    /* 0xFF7 */ s8 unkFF7;
} func_800B2550_1DF600_arg;

void cutsceneSysDisp_init(void) {
}

s32 cutsceneSysDisp_validate(void) {
    return 0;
}

void cutsceneSysDisp_exec(u8 *arg0, cutsceneSysDisp_exec_arg *arg1) {
    arg1->unkFF5 = (*arg0);
}

void cutsceneSysFadeIn_init(void) {
}

s32 cutsceneSysFadeIn_validate(void) {
    return 0;
}

void cutsceneSysFadeIn_exec(cutsceneSysFadeIn_exec_arg *arg0, Node_70B00 *arg1) {
    u8 temp_a1 = D_800BA5C0_1E7670[arg0->unk2];
    func_8006FE28_70A28(arg1->unk0.next, temp_a1, temp_a1, temp_a1);
    func_8006FDA0_709A0(arg1->unk0.next, 0, arg0->unk1);
}

s32 cutsceneSysFadeIn_isDone(void) {
    return 0;
}

void cutsceneSysFadeOut_init(void) {
}

s32 cutsceneSysFadeOut_validate(void) {
    return 0;
}

void cutsceneSysFadeOut_exec(cutsceneSysFadeOut_exec_arg *arg0, Node_70B00 *arg1) {
    u8 temp_a1;

    temp_a1 = D_800BA5C0_1E7670[arg0->unk2];
    func_8006FE28_70A28(arg1->unk0.next, temp_a1, temp_a1, temp_a1);
    func_8006FDA0_709A0(arg1->unk0.next, 0xFF, arg0->unk1);
}

s32 cutsceneSysFadeOut_isDone(void) {
    return 0;
}

void cutsceneSysFlash_init(void) {
}

s32 cutsceneSysFlash_validate(void) {
    return 0;
}

void cutsceneSysFlash_setup(func_800B2A24_1DFAD4_arg_item *arg0, CutsceneManager *arg1, s8 arg2) {
    CutsceneSlot *temp_v0;

    temp_v0 = func_800B2A78_1DFB28(arg1, arg2);
    temp_v0->unk0.One.unk0 = arg0->unk0.One.unk0;
    temp_v0->unk0.One.unkA = 0xFF;
    memcpy(&temp_v0->unk0.One.unk2[0], &arg0->unk0.One.unk2[0], 4);
    memcpy(&temp_v0->unk0.One.unk2[1], &arg0->unk0.One.unk2[1], 4);
    func_800B2A24_1DFAD4(arg1, arg2);
}

void cutsceneSysFlash_update(CutsceneManager *a0, s8 a1) {
    CutsceneSlot *s0;
    s32 s1 = 0xFF;
    s32 temp;
    u16 *check_ptr;

    s0 = func_800B2A78_1DFB28(a0, a1);

    if (s0->unk0.One.unk0 > 0) {
        temp = s0->unk0.One.unkA & 1;
        func_8006FE28_70A28(
            a0->uiResource,
            s0->unk0.One.unk2[temp].unk0,
            s0->unk0.One.unk2[temp].unk1,
            s0->unk0.One.unk2[temp].unk2
        );

        if (s0->unk0.One.unkA != 0) {
            s0->unk0.One.unkA = 0;
            check_ptr = (u16 *)&s0->unk0.One.unk2[1];
            if (*check_ptr == 0) {
                s1 = -(s0->unk0.Two.unk8 != 0);
            }
        } else {
            s0->unk0.One.unkA = s1;
        }

        func_8006FDA0_709A0(a0->uiResource, s1 & 0xFF, 0);
        s0->unk0.One.unk0--;
    } else {
        func_8006FDA0_709A0(a0->uiResource, 0, 0);
        func_800B2A50_1DFB00(a0, a1);
    }
}

s32 cutsceneSysFlash_isDone(void) {
    return 0;
}

void cutsceneSysIntMode_init(void) {
}

s32 cutsceneSysIntMode_validate(void) {
    return 0;
}

void cutsceneSysIntMode_exec(cutsceneSysIntMode_exec_arg *arg0) {
    osViExtendVStart(arg0->unk0);
}

void cutsceneSysWipeColor_init(void) {
}

s32 cutsceneSysWipeColor_validate(void) {
    return 0;
}

void cutsceneSysWipeColor_exec(cutsceneSysWipeColor_exec_arg0 *arg0, cutsceneSysWipeColor_exec_arg1 *arg1) {
    setNodeWipeColor((NodeWipeColorArg *)&arg1->unkFF8, arg0->unk0, arg0->unk1, arg0->unk2);
}

void func_800B2550_1DF600(func_800B2550_1DF600_arg **arg0) {
    func_800B2550_1DF600_arg *temp_a0 = *arg0;
    if ((temp_a0->unkFF7 != 0) && (gButtonsPressed[0] & A_BUTTON)) {
        temp_a0->unkFF7 = 0;
        func_80069CF8_6A8F8();
    }
}

void cutsceneSysWait_init(void) {
}

s32 cutsceneSysWait_validate(void) {
    return 0;
}

void cutsceneSysWait_exec(u16 *arg0, CutsceneManager *arg1) {
    cutsceneSysWait_exec_task *temp_v0 = scheduleTask(&func_8000D244_DE44, 1, 0, 0x64);
    if (temp_v0 != NULL) {
        func_800B29F0_1DFAA0(arg1);
        temp_v0->unk0 = arg1;
        temp_v0->unkE = (*arg0);
    }
}

void cutsceneSysCurtain_init(void) {
}

s32 cutsceneSysCurtain_validate(void) {
    return 0;
}

void cutsceneSysCurtain_exec(CurtainParams *params, CutsceneManager *cutsceneManager, s8 idx) {
    CutsceneSlot *item;

    item = func_800B2A78_1DFB28(cutsceneManager, idx);

    item->unk0.CurtainPayload.unk0 = (params->unk0 << 16) / 100;

    if (params->unk4) {
        item->unk0.CurtainPayload.unk4 =
            (item->unk0.CurtainPayload.unk0 - cutsceneManager->cameraAnimationTimer) / params->unk4;
        item->unk0.CurtainPayload.unk8 = params->unk4;

        if (item->unk0.CurtainPayload.unk4 != 0) {
            func_800B2A24_1DFAD4(cutsceneManager, idx);
        } else {
            cutsceneManager->cameraAnimationTimer = item->unk0.CurtainPayload.unk0;
        }
    } else {
        cutsceneManager->cameraAnimationTimer = item->unk0.CurtainPayload.unk0;
    }
}

void cutsceneSysCurtain_update(CutsceneManager *arg0, s8 arg1) {
    CutsceneSlot *temp_a0;
    s32 temp_v1;
    s32 temp_a0_val;

    temp_a0 = func_800B2A78_1DFB28(arg0, arg1);
    temp_v1 = arg0->cameraAnimationTimer + temp_a0->unk0.Two.unk4;
    arg0->cameraAnimationTimer = temp_v1;

    if (temp_a0->unk0.Two.unk4 > 0) {
        if (temp_a0->unk0.Two.unk0 < temp_v1) {
            arg0->cameraAnimationTimer = temp_a0->unk0.Two.unk0;
            func_800B2A50_1DFB00(arg0, arg1);
        }
    } else {
        if (temp_v1 < temp_a0->unk0.Two.unk0) {
            arg0->cameraAnimationTimer = temp_a0->unk0.Two.unk0;
            func_800B2A50_1DFB00(arg0, arg1);
        }
    }
}

s32 cutsceneSysCurtain_isDone(void) {
    return 0;
}

void cutsceneSysFilter_init(void) {
}

s32 cutsceneSysFilter_validate(void) {
    return 0;
}

void cutsceneSysFilter_exec(cutsceneSysFilter_exec_arg *arg0, Node_70B00 *arg1) {
    func_8006FE28_70A28(arg1->unk0.next, arg0->unk4, arg0->unk5, arg0->unk6);
    func_8006FDA0_709A0(arg1->unk0.next, arg0->unk3, arg0->unk1);
}

s32 cutsceneSysFilter_isDone(void) {
    return 0;
}
