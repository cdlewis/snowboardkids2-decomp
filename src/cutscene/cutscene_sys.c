#include "cutscene/cutscene_sys.h"
#include "1DFAA0.h"
#include "6E840.h"
#include "CBA0.h"
#include "DC90.h"
#include "task_scheduler.h"
extern u8 gCutsceneFadeBrightnessTable[];
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

void cutsceneSysDisp_exec(u8 *enableTransparencyValue, cutsceneSysDisp_exec_arg *displayArgs) {
    displayArgs->enableTransparency = *enableTransparencyValue;
}

void cutsceneSysFadeIn_init(void) {
}

s32 cutsceneSysFadeIn_validate(void) {
    return 0;
}

void cutsceneSysFadeIn_exec(cutsceneSysFadeIn_exec_arg *arg0, Node_70B00 *arg1) {
    u8 brightness = gCutsceneFadeBrightnessTable[arg0->brightnessIndex];
    func_8006FE28_70A28(arg1->unk0.next, brightness, brightness, brightness);
    func_8006FDA0_709A0(arg1->unk0.next, 0, arg0->fadeMode);
}

s32 cutsceneSysFadeIn_isDone(void) {
    return 0;
}

void cutsceneSysFadeOut_init(void) {
}

s32 cutsceneSysFadeOut_validate(void) {
    return 0;
}

void cutsceneSysFadeOut_exec(cutsceneSysFadeOut_exec_arg *fadeArgs, Node_70B00 *node) {
    u8 brightness = gCutsceneFadeBrightnessTable[fadeArgs->brightnessIndex];
    func_8006FE28_70A28(node->unk0.next, brightness, brightness, brightness);
    func_8006FDA0_709A0(node->unk0.next, 0xFF, fadeArgs->fadeMode);
}

s32 cutsceneSysFadeOut_isDone(void) {
    return 0;
}

void cutsceneSysFlash_init(void) {
}

s32 cutsceneSysFlash_validate(void) {
    return 0;
}

void cutsceneSysFlash_setup(func_800B2A24_1DFAD4_arg_item *srcItem, CutsceneManager *cutsceneManager, s8 slotIndex) {
    CutsceneSlot *slot;

    slot = getCutsceneSlot(cutsceneManager, slotIndex);
    slot->unk0.One.unk0 = srcItem->unk0.One.unk0;
    slot->unk0.One.unkA = 0xFF;
    memcpy(&slot->unk0.One.unk2[0], &srcItem->unk0.One.unk2[0], 4);
    memcpy(&slot->unk0.One.unk2[1], &srcItem->unk0.One.unk2[1], 4);
    enableSlotUpdate(cutsceneManager, slotIndex);
}

void cutsceneSysFlash_update(CutsceneManager *cutsceneManager, s8 slotIndex) {
    CutsceneSlot *slot;
    s32 transparency = 0xFF;
    s32 colorIndex;
    u16 *color1Check;

    slot = getCutsceneSlot(cutsceneManager, slotIndex);

    if (slot->unk0.One.unk0 > 0) {
        colorIndex = slot->unk0.One.unkA & 1;
        func_8006FE28_70A28(
            cutsceneManager->uiResource,
            slot->unk0.One.unk2[colorIndex].unk0,
            slot->unk0.One.unk2[colorIndex].unk1,
            slot->unk0.One.unk2[colorIndex].unk2
        );

        if (slot->unk0.One.unkA != 0) {
            slot->unk0.One.unkA = 0;
            color1Check = (u16 *)&slot->unk0.One.unk2[1];
            if (*color1Check == 0) {
                transparency = -(slot->unk0.Two.unk8 != 0);
            }
        } else {
            slot->unk0.One.unkA = transparency;
        }

        func_8006FDA0_709A0(cutsceneManager->uiResource, transparency & 0xFF, 0);
        slot->unk0.One.unk0--;
    } else {
        func_8006FDA0_709A0(cutsceneManager->uiResource, 0, 0);
        disableSlotUpdate(cutsceneManager, slotIndex);
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
    cutsceneSysWait_exec_task *temp_v0 = scheduleTask(&initCutsceneWaitMenu, 1, 0, 0x64);
    if (temp_v0 != NULL) {
        enableCutsceneSkip(arg1);
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

    item = getCutsceneSlot(cutsceneManager, idx);

    item->unk0.CurtainPayload.unk0 = (params->unk0 << 16) / 100;

    if (params->unk4) {
        item->unk0.CurtainPayload.unk4 =
            (item->unk0.CurtainPayload.unk0 - cutsceneManager->cameraAnimationTimer) / params->unk4;
        item->unk0.CurtainPayload.unk8 = params->unk4;

        if (item->unk0.CurtainPayload.unk4 != 0) {
            enableSlotUpdate(cutsceneManager, idx);
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

    temp_a0 = getCutsceneSlot(arg0, arg1);
    temp_v1 = arg0->cameraAnimationTimer + temp_a0->unk0.Two.unk4;
    arg0->cameraAnimationTimer = temp_v1;

    if (temp_a0->unk0.Two.unk4 > 0) {
        if (temp_a0->unk0.Two.unk0 < temp_v1) {
            arg0->cameraAnimationTimer = temp_a0->unk0.Two.unk0;
            disableSlotUpdate(arg0, arg1);
        }
    } else {
        if (temp_v1 < temp_a0->unk0.Two.unk0) {
            arg0->cameraAnimationTimer = temp_a0->unk0.Two.unk0;
            disableSlotUpdate(arg0, arg1);
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
