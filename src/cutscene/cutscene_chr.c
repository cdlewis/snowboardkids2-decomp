#include "cutscene/cutscene_chr.h"
#include "1DFAA0.h"
#include "1E36C0.h"
#include "20F0.h"

void cutsceneChrPosition_init(void) {
}

s32 cutsceneChrPosition_validate(void) {
    return 0;
}

void cutsceneChrPosition_exec(cutsceneChrPosition_exec_arg *arg, CutsceneManager *manager, s8 slotIndex) {
    CutsceneSlot *slot;
    CutsceneSlotData *slotData;

    slot = &manager->slots[slotIndex];
    slotData = &slot->slotData;

    initSlotPosition(slotData, arg->x, arg->y, arg->z, arg->rotY);
    setupSlotTransform(slotData);
    applyTransformToModel(slot->model, &slot->slotData.unk04);
    setModelAnimationEx(slot->model, arg->animIndex, arg->transitionAnimIndex, arg->loopCount, -1, arg->animQueued);
    setModelActionMode(slot->model, arg->actionMode);
}

s32 cutsceneChrPosition_isDone(void) {
    return 0;
}

void cutsceneChrMove_init(void) {
}

s32 cutsceneChrMove_validate(void) {
    return 0;
}

void cutsceneChrMove_exec(cutsceneChrMove_exec_arg *arg0, CutsceneManager *arg1, s8 arg2) {
    s32 temp_s0;
    s32 animId;
    u16 temp_s4;
    s32 result;
    CutsceneSlot *slot;
    s32 pad[4];

    slot = &arg1->slots[arg2];

    temp_s4 = getCurrentStateEntryItem(arg2)->unk4;

    temp_s0 = 1;
    animId = -1;

    result = setupSlotMoveToEx(
        &slot->slotData,
        slot->model,
        arg0->unk0,
        arg0->unk4,
        arg0->unk8,
        arg0->unkE,
        arg0->unkC,
        0,
        temp_s0
    );

    switch (result) {
        case 1:
            animId = getWalkAnimationId(temp_s4);
            break;
        case 2:
            animId = getRunAnimationId(temp_s4);
            break;
    }

    setModelAnimationEx(slot->model, arg0->unk10, arg0->unk14, arg0->unk13, animId, arg0->unk16);

    setModelActionMode(slot->model, arg0->unk12);
}

s32 cutsceneChrMove_isDone(void) {
    return 0;
}

void cutsceneChrAnime_init(void) {
}

s32 cutsceneChrAnime_validate(void) {
    return 0;
}

void cutsceneChrAnime_exec(cutsceneChrAnime_exec_arg *arg, CutsceneManager *manager, s8 slotIndex) {
    CutsceneSlot *slot = &manager->slots[slotIndex];

    slot->slotData.unk0.Two = 0;
    setModelAnimationEx(slot->model, arg->animIndex, arg->transitionAnimIndex, arg->loopCount, -1, -1);
    setModelActionMode(slot->model, arg->actionMode);
}

s32 cutsceneChrAnime_isDone(void) {
    return 0;
}

void cutsceneChrDisp_init(void) {
}

s32 cutsceneChrDisp_validate(void) {
    return 0;
}

void cutsceneChrDisp_exec(cutsceneChrDisp_exec_arg *arg, CutsceneManager *cutsceneManager, s8 slotIndex) {
    CutsceneSlot *slot = &cutsceneManager->slots[slotIndex];
    setModelDisplayEnabled(slot->model, arg->displayEnabled);
}

void cutsceneChrItem_init(void) {
}

s32 cutsceneChrItem_validate(void) {
    return 0;
}

void cutsceneChrItem_exec(cutsceneChrDisp_exec_arg *arg, CutsceneManager *cutsceneManager, s8 slotIndex) {
    CutsceneSlot *slot = &cutsceneManager->slots[slotIndex];
    setItemDisplayEnabled(slot->model, arg->displayEnabled);
}

void cutsceneChrTurn_init(void) {
}

s32 cutsceneChrTurn_validate(void) {
    return 0;
}

void cutsceneChrTurn_exec(cutsceneChrTurn_exec_arg *arg0, CutsceneManager *arg1, s8 arg2) {
    CutsceneSlot *slot;
    cutsceneChrTurn_exec_arg *temp_arg0;
    s16 animId;
    u16 temp_s0;
    s32 result;

    temp_arg0 = arg0;
    animId = -1;
    slot = &arg1->slots[arg2];

    temp_s0 = getCurrentStateEntryItem(arg2)->unk4;

    result = setupSlotRotateTo(&slot->slotData, slot->model, temp_arg0->targetAngle);

    switch (result) {
        case 1:
            animId = getWalkAnimationId(temp_s0);
            break;
        case 2:
            animId = getRunAnimationId(temp_s0);
            break;
    }

    setModelAnimationQueued(slot->model, temp_arg0->animIndex, temp_arg0->transitionAnimIndex, -1, animId);
}

void cutsceneChrMove2_init(void) {
}

s32 cutsceneChrMove2_validate(void) {
    return 0;
}

void cutsceneChrMove2_exec(cutsceneChrMove2_exec_arg *arg, CutsceneManager *cutsceneManager, s8 slotIndex) {
    s32 decelRate;
    s16 animId;
    u16 characterId;
    s32 result;
    CutsceneSlot *slot;

    slot = &cutsceneManager->slots[slotIndex];

    characterId = getCurrentStateEntryItem(slotIndex)->unk4;

    decelRate = 1;
    animId = -1;

    result = setupSlotMoveToEx(
        &slot->slotData,
        slot->model,
        arg->targetX,
        arg->targetY,
        arg->targetZ,
        arg->duration,
        arg->fallbackRotY,
        decelRate,
        arg->decelRate
    );

    switch (result) {
        case 1:
            animId = getWalkAnimationId(characterId);
            break;
        case 2:
            animId = getRunAnimationId(characterId);
            break;
    }

    setModelAnimationEx(slot->model, arg->animIndex, arg->transitionAnimIndex, arg->loopCount, animId, arg->animQueued);

    setModelActionMode(slot->model, arg->actionMode);
}

s32 cutsceneChrMove2_isDone(void) {
    return 0;
}

void cutsceneChrZoom_init(void) {
}

s32 cutsceneChrZoom_validate(void) {
    return 0;
}

void cutsceneChrZoom_exec(cutsceneChrZoom_exec_arg *arg0, CutsceneManager *arg1, s8 arg2) {
    CutsceneSlotData *slotData;
    s32 scaleFixed;

    slotData = &arg1->slots[arg2].slotData;
    scaleFixed = (arg0->scalePercent << 16) / 100;

    interpolateSlotScaleX(slotData, scaleFixed, arg0->duration);
    interpolateSlotScaleY(slotData, scaleFixed, arg0->duration);
    interpolateSlotScaleZ(slotData, scaleFixed, arg0->duration);
}

s32 cutsceneChrZoom_isDone(void) {
    return 0;
}

void cutsceneChrBoardMove_init(void) {
}

s32 cutsceneChrBoardMove_validate(void) {
    return 0;
}

void cutsceneChrBoardMove_exec(cutsceneChrMove2_exec_arg *arg, CutsceneManager *cutsceneManager, s8 slotIndex) {
    CutsceneSlot *slot = &cutsceneManager->slots[slotIndex];

    getCurrentStateEntryItem(slotIndex);

    setupSlotWalkTo(
        &slot->slotData,
        slot->model,
        arg->targetX,
        arg->targetY,
        arg->targetZ,
        arg->duration,
        arg->fallbackRotY,
        arg->animIndex,
        arg->decelRate,
        arg->animQueued
    );

    setModelAnimationQueued(slot->model, arg->animIndex, arg->transitionAnimIndex, arg->loopCount, -1);

    setModelActionMode(slot->model, arg->actionMode);
}

s32 cutsceneChrBoardMove_isDone(void) {
    return 0;
}

void cutsceneChrTurn2_init(void) {
}

s32 cutsceneChrTurn2_validate(void) {
    return 0;
}

void cutsceneChrTurn2_exec(cutsceneChrTurn2_exec_arg *arg, CutsceneManager *cutsceneManager, s8 slotIndex) {
    CutsceneSlot *slot;
    u16 characterId;
    s32 animId;
    s32 result;

    slot = &cutsceneManager->slots[slotIndex];

    characterId = getCurrentStateEntryItem(slotIndex)->unk4;

    animId = -1;

    result = setupSlotRotateToWithDir(&slot->slotData, slot->model, arg->targetRotY, arg->direction, arg->duration);

    switch (result) {
        case 1:
            animId = getWalkAnimationId(characterId);
            break;
        case 2:
            animId = getRunAnimationId(characterId);
            break;
    }

    setModelAnimationQueued(slot->model, arg->animIndex, arg->transitionAnimIndex, -1, animId);
}

void cutsceneChrRotate_init(void) {
}

s32 cutsceneChrRotate_validate(void) {
    return 0;
}

void cutsceneChrRotate_exec(cutsceneChrRotate_exec_arg *arg, CutsceneManager *manager, s8 slotIndex) {
    CutsceneSlot *slot;

    slot = &manager->slots[slotIndex];

    setupSlotOrbit(&slot->slotData, arg->rotationSpeed, arg->duration, arg->orbitSpeed);
    setModelAnimationLooped(slot->model, arg->animationId, arg->animationLoopCount, -1);
}

s32 cutsceneChrRotate_isDone(void) {
    return 0;
}

void cutsceneChrHop_init(void) {
}

s32 cutsceneChrHop_validate(void) {
    return 0;
}

void cutsceneChrHop_exec(cutsceneChrHop_exec_arg *arg, CutsceneManager *manager, s8 slotIndex) {
    CutsceneSlot *slot = &manager->slots[slotIndex];

    setupSlotProjectile(&slot->slotData, arg->horizontalSpeed, arg->verticalVelocity, arg->gravity);
    setModelAnimationLooped(slot->model, arg->animationId, arg->animationLoopCount, 1);
}

void cutsceneChrMoveToFacing_init(void) {
}

s32 cutsceneChrMoveToFacing_validate(void) {
    return 0;
}

void cutsceneChrMoveToFacing_exec(cutsceneChrMoveToFacing_exec_arg *arg, CutsceneManager *manager, s8 slotIndex) {
    CutsceneSlot *slot = &manager->slots[slotIndex];

    setupSlotMoveToFacing(&slot->slotData, arg->targetX, arg->targetY, arg->targetZ, arg->duration);
    setModelAnimationQueued(slot->model, arg->animationId, arg->transitionAnimId, arg->animationLoopCount, -1);
    setModelActionMode(slot->model, arg->actionMode);
}

s32 cutsceneChrMoveToFacing_isDone(void) {
    return 0;
}

void cutsceneChrZoom2_init(void) {
}

s32 cutsceneChrZoom2_validate(void) {
    return 0;
}

void cutsceneChrZoom2_exec(cutsceneChrZoom2_exec_arg *arg0, CutsceneManager *arg1, s8 arg2) {
    CutsceneSlotData *slotData;
    s32 scaleFixedX;
    s32 scaleFixedY;
    s32 scaleFixedZ;

    slotData = &arg1->slots[arg2].slotData;
    scaleFixedX = (arg0->scalePercentX << 16) / 100;
    scaleFixedY = (arg0->scalePercentY << 16) / 100;
    scaleFixedZ = (arg0->scalePercentZ << 16) / 100;

    interpolateSlotScaleX(slotData, scaleFixedX, arg0->duration);
    interpolateSlotScaleY(slotData, scaleFixedY, arg0->duration);
    interpolateSlotScaleZ(slotData, scaleFixedZ, arg0->duration);
}

s32 cutsceneChrZoom2_isDone(void) {
    return 0;
}

void cutsceneChrUpDown_init(void) {
}

s32 cutsceneChrUpDown_validate(void) {
    return 0;
}

void cutsceneChrUpDown_exec(cutsceneChrUpDown_exec_arg *arg0, CutsceneManager *arg1, s8 arg2) {
    CutsceneSlot *slot;

    slot = &arg1->slots[arg2];

    getCurrentStateEntryItem(arg2);

    setupSlotMoveToNoRotation(
        &slot->slotData,
        slot->model,
        arg0->targetX,
        arg0->targetY,
        arg0->targetZ,
        arg0->duration
    );

    setModelAnimationEx(slot->model, arg0->animIndex, arg0->transitionAnimIndex, arg0->loopCount, -1, arg0->animQueued);

    setModelActionMode(slot->model, arg0->actionMode);
}

s32 cutsceneChrUpDown_isDone(void) {
    return 0;
}

void cutsceneChrFace_init(void) {
}

s32 cutsceneChrFace_validate(void) {
    return 0;
}

void cutsceneChrFace_exec(s8 *animIndex, CutsceneManager *manager, s8 slotIndex) {
    CutsceneSlot *slot = &manager->slots[slotIndex];
    setAnimationIndex(slot->model, *animIndex);
}

void cutsceneChrAnime2_init(void) {
}

s32 cutsceneChrAnime2_validate(void) {
    return 0;
}

void cutsceneChrAnime2_exec(cutsceneChrAnime2_exec_arg *arg0, CutsceneManager *arg1, s8 arg2) {
    CutsceneSlot *slot;

    slot = &arg1->slots[arg2];
    slot->slotData.unk0.Two = 1;
    setModelAnimationEx(slot->model, arg0->animIndex, arg0->transitionAnimIndex, arg0->loopCount, -1, -1);
    setModelActionMode(slot->model, arg0->actionMode);
}

s32 cutsceneChrAnime2_isDone(void) {
    return 0;
}

void cutsceneChrMove3_init(void) {
}

s32 cutsceneChrMove3_validate(void) {
    return 0;
}

void cutsceneChrMove3_exec(cutsceneChrMove3_exec_arg *arg, CutsceneManager *cutsceneManager, s8 slotIndex) {
    CutsceneSlot *slot = &cutsceneManager->slots[slotIndex];

    getCurrentStateEntryItem(slotIndex);

    setupSlotMoveToWithRotation(
        &slot->slotData,
        slot->model,
        arg->targetX,
        arg->targetY,
        arg->targetZ,
        arg->duration,
        arg->targetAngle
    );

    setModelAnimationEx(slot->model, arg->animIndex, arg->transitionAnimIndex, arg->loopCount, -1, arg->animQueued);

    setModelActionMode(slot->model, arg->actionMode);
}

s32 cutsceneChrMove3_isDone(void) {
    return 0;
}

void cutsceneChrModelDisp_init(void) {
}

s32 cutsceneChrModelDisp_validate(void) {
    return 0;
}

void cutsceneChrModelDisp_exec(cutsceneChrModelDisp_exec_arg *arg0, CutsceneManager *cutsceneManager, s8 index) {
    CutsceneSlot *slot = &cutsceneManager->slots[index];

    if (arg0->enabled != 0) {
        setModelPartDisplayFlag(slot->model, arg0->partIndex);
    } else {
        clearModelPartDisplayFlag(slot->model, arg0->partIndex);
    }
}

void cutsceneChrMoveSight_init(void) {
}

s32 cutsceneChrMoveSight_validate(void) {
    return 0;
}

void cutsceneChrMoveSight_exec(cutsceneChrMoveSight_exec_arg *arg0, CutsceneManager *cutsceneManager, s8 slotIndex) {
    s32 targetPosition[3];
    CutsceneSlot *slot;

    targetPosition[0] = arg0->targetX;
    targetPosition[1] = arg0->targetY;
    targetPosition[2] = arg0->targetZ;

    slot = &cutsceneManager->slots[slotIndex];

    setupSlotMoveToWithBounce(
        &slot->slotData,
        (s32 *)&targetPosition,
        arg0->duration,
        arg0->verticalVelocity,
        arg0->gravity
    );

    setModelAnimationLooped(slot->model, arg0->animIndex, arg0->transitionAnimIndex, arg0->loopCount);

    setModelActionMode(slot->model, arg0->actionMode);
}

s32 cutsceneChrMoveSight_isDone(void) {
    return 0;
}

void cutsceneChrAlpha_init(void) {
}

s32 cutsceneChrAlpha_validate(void) {
    return 0;
}

void cutsceneChrAlpha_exec(cutsceneChrAlpha_exec_arg *arg0, CutsceneManager *arg1, s8 arg2) {
    CutsceneSlot *slot;
    CutsceneSlot *currentSlot;
    u8 currentAlpha;

    slot = getCutsceneSlot(arg1, arg2);
    currentSlot = &arg1->slots[arg2];
    slot->unk0.ChrPayload.remainingFrames = arg0->duration;

    currentAlpha = getModelAlpha(currentSlot->model);

    if (slot->unk0.ChrPayload.remainingFrames == 0 || arg0->targetAlpha == currentAlpha) {
        setModelAlpha(currentSlot->model, arg0->targetAlpha);
    } else {
        slot->unk0.ChrPayload.easingParam0 = arg0->reserved0 << 16;
        slot->unk0.ChrPayload.easingParam1 = arg0->reserved1 << 16;
        slot->unk0.ChrPayload.easingParam2 = arg0->reserved2 << 16;
        slot->unk0.ChrPayload.targetAlphaFixed = arg0->targetAlpha << 16;

        currentAlpha = getModelAlpha(currentSlot->model);

        slot->unk0.ChrPayload.currentAlphaFixed = currentAlpha << 16;
        slot->unk0.ChrPayload.alphaIncrement =
            (slot->unk0.ChrPayload.targetAlphaFixed - (currentAlpha << 16)) / slot->unk0.ChrPayload.remainingFrames;

        enableSlotUpdate(arg1, arg2);
    }
}

void cutsceneChrAlpha_update(CutsceneManager *arg0, s8 arg1) {
    CutsceneSlot *slot;
    CutsceneSlot *managerSlot;
    s32 newValue;

    slot = getCutsceneSlot(arg0, arg1);
    managerSlot = &arg0->slots[arg1];

    if (slot->unk0.ChrPayload.remainingFrames > 0) {
        setModelAlpha(managerSlot->model, slot->unk0.ChrPayload.padding[0xF]);
        slot->unk0.ChrPayload.remainingFrames--;
        newValue = slot->unk0.ChrPayload.currentAlphaFixed + slot->unk0.ChrPayload.alphaIncrement;
        slot->unk0.ChrPayload.currentAlphaFixed = newValue;

        if (slot->unk0.ChrPayload.alphaIncrement > 0) {
            if (slot->unk0.ChrPayload.targetAlphaFixed < newValue) {
                slot->unk0.ChrPayload.remainingFrames = 0;
            }
        } else if (slot->unk0.ChrPayload.alphaIncrement < 0) {
            if (newValue < slot->unk0.ChrPayload.targetAlphaFixed) {
                slot->unk0.ChrPayload.remainingFrames = 0;
            }
        } else {
            slot->unk0.ChrPayload.remainingFrames = 0;
        }
    } else {
        disableSlotUpdate(arg0, arg1);
    }
}

s32 cutsceneChrAlpha_isDone(void) {
    return 0;
}

void cutsceneChrPosition2_init(void) {
}

s32 cutsceneChrPosition2_validate(void) {
    return 0;
}

void cutsceneChrPosition2_exec(cutsceneChrPosition2_exec_arg *arg, CutsceneManager *manager, s8 slotIndex) {
    CutsceneSlot *slot;
    CutsceneSlotData *slotData;

    slot = &manager->slots[slotIndex];
    slotData = &slot->slotData;

    initSlotPositionEx(slotData, arg->x, arg->y, arg->z, arg->rotY, arg->rotX, arg->rotZ);
    setupSlotTransform(slotData);
    applyTransformToModel(slot->model, &slot->slotData.unk04);
    setModelAnimationEx(slot->model, arg->animIndex, arg->transitionAnimIndex, arg->loopCount, -1, arg->animQueued);
    setModelActionMode(slot->model, arg->actionMode);
}

s32 cutsceneChrPosition2_isDone(void) {
    return 0;
}

void cutsceneChrTurn3_init(void) {
}

s32 cutsceneChrTurn3_validate(void) {
    return 0;
}

void cutsceneChrTurn3_exec(cutsceneChrTurn3_exec_arg *arg0, CutsceneManager *arg1, s8 arg2) {
    CutsceneSlot *currentSlot = &arg1->slots[arg2];

    getCurrentStateEntryItem(arg2);

    setupSlotRotateWithSpeed(
        &currentSlot->slotData,
        currentSlot->model,
        arg0->targetAngle,
        arg0->rotationStep,
        arg0->duration,
        arg0->rotationMultiplier
    );

    setModelAnimationQueued(currentSlot->model, arg0->animationIndex, arg0->transitionAnimIndex, -1, -1);
}

s32 cutsceneChrTurn3_isDone(void) {
    return 0;
}

void cutsceneChrKageDisp_init(void) {
}

s32 cutsceneChrKageDisp_validate(void) {
    return 0;
}

void cutsceneChrKageDisp_exec(cutsceneChrKageDisp_exec_arg *arg0, CutsceneManager *arg1, s8 arg2) {
    CutsceneSlot *slot = &arg1->slots[arg2];

    if (arg0->hideShadow != 0) {
        disableModelShadow(slot->model);
    } else {
        enableModelShadow(slot->model);
    }
}