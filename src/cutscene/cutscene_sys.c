#include "cutscene/cutscene_sys.h"
#include "1DFAA0.h"
#include "6E840.h"
#include "CBA0.h"
#include "DC90.h"
#include "task_scheduler.h"
extern u8 gCutsceneFadeBrightnessTable[];
extern s32 gButtonsPressed[];

typedef struct {
    CutsceneManager *cutsceneManager;
    u8 padding[0xA];
    s16 waitFrameCount;
} cutsceneSysWait_exec_task;

typedef struct {
    u8 padding[0xFF7];
    /* 0xFF7 */ s8 skipAnimation;
} skipCutsceneCallback_arg;

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
    setViewportEnvColor(arg1->unk0.next, brightness, brightness, brightness);
    setViewportFadeValue(arg1->unk0.next, 0, arg0->fadeMode);
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
    setViewportEnvColor(node->unk0.next, brightness, brightness, brightness);
    setViewportFadeValue(node->unk0.next, 0xFF, fadeArgs->fadeMode);
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
    slot->unk0.FlashPayload.frameCounter = srcItem->unk0.FlashPayload.frameCounter;
    slot->unk0.FlashPayload.colorToggle = 0xFF;
    memcpy(&slot->unk0.FlashPayload.colors[0], &srcItem->unk0.FlashPayload.colors[0], 4);
    memcpy(&slot->unk0.FlashPayload.colors[1], &srcItem->unk0.FlashPayload.colors[1], 4);
    enableSlotUpdate(cutsceneManager, slotIndex);
}

void cutsceneSysFlash_update(CutsceneManager *cutsceneManager, s8 slotIndex) {
    CutsceneSlot *slot;
    s32 screenAlpha = 0xFF;
    s32 currentColorIndex;
    u16 *secondColorIsNull;

    slot = getCutsceneSlot(cutsceneManager, slotIndex);

    if (slot->unk0.FlashPayload.frameCounter > 0) {
        // Toggle between the two colors each frame
        currentColorIndex = slot->unk0.FlashPayload.colorToggle & 1;
        setViewportEnvColor(
            cutsceneManager->uiResource,
            slot->unk0.FlashPayload.colors[currentColorIndex].unk0,
            slot->unk0.FlashPayload.colors[currentColorIndex].unk1,
            slot->unk0.FlashPayload.colors[currentColorIndex].unk2
        );

        // Alternate colorToggle between 0xFF and 0
        if (slot->unk0.FlashPayload.colorToggle != 0) {
            slot->unk0.FlashPayload.colorToggle = 0;
            // Check if the second color is null (all zeros)
            secondColorIsNull = (u16 *)&slot->unk0.FlashPayload.colors[1];
            if (*secondColorIsNull == 0) {
                // When second color is null, set alpha based on Two.unk8 (overlaps with colors[1].unk2)
                screenAlpha = -(slot->unk0.Two.unk8 != 0);
            }
        } else {
            slot->unk0.FlashPayload.colorToggle = screenAlpha;
        }

        setViewportFadeValue(cutsceneManager->uiResource, screenAlpha & 0xFF, 0);
        slot->unk0.FlashPayload.frameCounter--;
    } else {
        // Flash effect complete - clear the screen overlay
        setViewportFadeValue(cutsceneManager->uiResource, 0, 0);
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
    osViExtendVStart(arg0->vStartExtension);
}

void cutsceneSysWipeColor_init(void) {
}

s32 cutsceneSysWipeColor_validate(void) {
    return 0;
}

void cutsceneSysWipeColor_exec(cutsceneSysWipeColor_exec_arg0 *arg0, cutsceneSysWipeColor_exec_arg1 *arg1) {
    setNodeWipeColor((NodeWipeColorArg *)&arg1->wipeColorNode, arg0->r, arg0->g, arg0->b);
}

void skipCutsceneOnInputCallback(skipCutsceneCallback_arg **arg0) {
    skipCutsceneCallback_arg *taskPayload = *arg0;
    if ((taskPayload->skipAnimation != 0) && (gButtonsPressed[0] & A_BUTTON)) {
        taskPayload->skipAnimation = 0;
        terminateCurrentTask();
    }
}

void cutsceneSysWait_init(void) {
}

s32 cutsceneSysWait_validate(void) {
    return 0;
}

void cutsceneSysWait_exec(u16 *waitFrames, CutsceneManager *cutsceneManager) {
    cutsceneSysWait_exec_task *task = scheduleTask(&initCutsceneWaitMenu, 1, 0, 0x64);
    if (task != NULL) {
        enableCutsceneSkip(cutsceneManager);
        task->cutsceneManager = cutsceneManager;
        task->waitFrameCount = *waitFrames;
    }
}

void cutsceneSysCurtain_init(void) {
}

s32 cutsceneSysCurtain_validate(void) {
    return 0;
}

void cutsceneSysCurtain_exec(CurtainParams *params, CutsceneManager *cutsceneManager, s8 idx) {
    CutsceneSlot *slot;

    slot = getCutsceneSlot(cutsceneManager, idx);

    slot->unk0.CurtainPayload.targetTimer = (params->targetPercent << 16) / 100;

    if (params->duration) {
        slot->unk0.CurtainPayload.stepDelta =
            (slot->unk0.CurtainPayload.targetTimer - cutsceneManager->cameraAnimationTimer) / params->duration;
        slot->unk0.CurtainPayload.framesRemaining = params->duration;

        if (slot->unk0.CurtainPayload.stepDelta != 0) {
            enableSlotUpdate(cutsceneManager, idx);
        } else {
            cutsceneManager->cameraAnimationTimer = slot->unk0.CurtainPayload.targetTimer;
        }
    } else {
        cutsceneManager->cameraAnimationTimer = slot->unk0.CurtainPayload.targetTimer;
    }
}

void cutsceneSysCurtain_update(CutsceneManager *cutsceneManager, s8 slotIndex) {
    CutsceneSlot *slot;

    slot = getCutsceneSlot(cutsceneManager, slotIndex);
    cutsceneManager->cameraAnimationTimer += slot->unk0.CurtainPayload.stepDelta;

    if (slot->unk0.CurtainPayload.stepDelta > 0) {
        if (slot->unk0.CurtainPayload.targetTimer < cutsceneManager->cameraAnimationTimer) {
            cutsceneManager->cameraAnimationTimer = slot->unk0.CurtainPayload.targetTimer;
            disableSlotUpdate(cutsceneManager, slotIndex);
        }
    } else {
        if (cutsceneManager->cameraAnimationTimer < slot->unk0.CurtainPayload.targetTimer) {
            cutsceneManager->cameraAnimationTimer = slot->unk0.CurtainPayload.targetTimer;
            disableSlotUpdate(cutsceneManager, slotIndex);
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

void cutsceneSysFilter_exec(cutsceneSysFilter_exec_arg *filterArgs, Node_70B00 *sceneNode) {
    setViewportEnvColor(
        sceneNode->unk0.next,
        filterArgs->envColorRed,
        filterArgs->envColorGreen,
        filterArgs->envColorBlue
    );
    setViewportFadeValue(sceneNode->unk0.next, filterArgs->cameraFilterParam, filterArgs->cameraFilterEnable);
}

s32 cutsceneSysFilter_isDone(void) {
    return 0;
}
