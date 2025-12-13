#include "cutscene/cutscene_chr.h"
#include "1DFAA0.h"
#include "1E36C0.h"
#include "20F0.h"

extern void func_800B788C_1E493C(CutsceneSlotData *, s32, s32, s32);
extern s32 func_800B75C4_1E4674(CutsceneSlotData *, SceneModel *, s16, s32, s32, s32);
extern s32 func_800B6FA4_1E4054(CutsceneSlotData *, SceneModel *, s32, s32, s32, s16, s16);
extern void func_800B7B70_1E4C20(CutsceneSlotData *, s32 *, s16, s32, s32);
extern void func_800B6BDC_1E3C8C(CutsceneSlotData *, s32, s32, s32, s16);
extern void func_800B6C04_1E3CB4(CutsceneSlotData *, s32, s32, s32, s16, s16, s16);
extern s32 setupSlotTransform(CutsceneSlotData *);
extern void func_800B7128_1E41D8(
    CutsceneSlotData *arg0,
    SceneModel *arg1,
    s32 arg2,
    s32 arg3,
    s32 arg4,
    s16 arg5,
    s16 arg6,
    s16 arg7,
    s8 arg8,
    s8 arg9
);
extern void func_800B7760_1E4810(CutsceneSlotData *arg0, s32 arg1, s16 arg2);
extern void func_800B77C4_1E4874(CutsceneSlotData *arg0, s32 arg1, s16 arg2);
extern void func_800B7828_1E48D8(CutsceneSlotData *arg0, s32 arg1, s16 arg2);
extern void func_800B7620_1E46D0(CutsceneSlotData *arg0, s16 arg1, s16 arg2, s16 arg3);
extern void func_800B7914_1E49C4(CutsceneSlotData *, s32, s32, s32, s16);
extern StateEntryItem *func_800B34B0_1E0560(s32);
extern s32 func_800B734C_1E43FC(CutsceneSlotData *, SceneModel *, s16);

void cutsceneChrPosition_init(void) {
}

s32 cutsceneChrPosition_validate(void) {
    return 0;
}

void cutsceneChrPosition_exec(cutsceneChrPosition_exec_arg *arg0, CutsceneManager *arg1, s8 arg2) {
    CutsceneSlot *slot;
    CutsceneSlotData *slotData;
    s16 temp;

    temp = arg0->unkC;
    slot = &arg1->slots[arg2];
    slotData = &slot->slotData;

    func_800B6BDC_1E3C8C(slotData, arg0->unk0, arg0->unk4, arg0->unk8, temp);
    setupSlotTransform(slotData);
    applyTransformToModel(slot->model, &slot->slotData.unk04);
    func_80002260_2E60(slot->model, arg0->unkE, arg0->unk12, arg0->unk11, -1, arg0->unk14);
    func_800015CC_21CC(slot->model, arg0->unk10);
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

    temp_s4 = func_800B34B0_1E0560(arg2)->unk4;

    temp_s0 = 1;
    animId = -1;

    result = func_800B6CD8_1E3D88(
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
            animId = func_800015DC_21DC(temp_s4);
            break;
        case 2:
            animId = func_800015E4_21E4(temp_s4);
            break;
    }

    func_80002260_2E60(slot->model, arg0->unk10, arg0->unk14, arg0->unk13, animId, arg0->unk16);

    func_800015CC_21CC(slot->model, arg0->unk12);
}

s32 cutsceneChrMove_isDone(void) {
    return 0;
}

void cutsceneChrAnime_init(void) {
}

s32 cutsceneChrAnime_validate(void) {
    return 0;
}

void cutsceneChrAnime_exec(cutsceneChrAnime_exec_arg *arg0, CutsceneManager *arg1, s8 arg2) {
    CutsceneSlot *slot = &arg1->slots[arg2];
    slot->slotData.unk0.Two = 0;
    func_80002260_2E60(slot->model, arg0->unk0, arg0->unk4, arg0->unk3, -1, -1);
    func_800015CC_21CC(slot->model, arg0->unk2);
}

s32 cutsceneChrAnime_isDone(void) {
    return 0;
}

void cutsceneChrDisp_init(void) {
}

s32 cutsceneChrDisp_validate(void) {
    return 0;
}

void cutsceneChrDisp_exec(s8 *a0, CutsceneManager *arg1, s8 arg2) {
    CutsceneSlot *slot = &arg1->slots[arg2];
    func_80001604_2204(slot->model, a0[1]);
}

void cutsceneChrItem_init(void) {
}

s32 cutsceneChrItem_validate(void) {
    return 0;
}

void cutsceneChrItem_exec(s8 *a0, CutsceneManager *arg1, s8 arg2) {
    CutsceneSlot *slot = &arg1->slots[arg2];
    func_8000160C_220C(slot->model, a0[1]);
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

    temp_s0 = func_800B34B0_1E0560(arg2)->unk4;

    result = func_800B734C_1E43FC(&slot->slotData, slot->model, temp_arg0->unk0);

    switch (result) {
        case 1:
            animId = func_800015DC_21DC(temp_s0);
            break;
        case 2:
            animId = func_800015E4_21E4(temp_s0);
            break;
    }

    func_80002220_2E20(slot->model, temp_arg0->unk2, temp_arg0->unk4, -1, animId);
}

void cutsceneChrMove2_init(void) {
}

s32 cutsceneChrMove2_validate(void) {
    return 0;
}

void cutsceneChrMove2_exec(cutsceneChrMove2_exec_arg *arg0, CutsceneManager *arg1, s8 arg2) {
    s32 temp_s0;
    s16 animId;
    u16 temp_s4;
    s32 result;
    CutsceneSlot *slot;

    slot = &arg1->slots[arg2];

    temp_s4 = func_800B34B0_1E0560(arg2)->unk4;

    temp_s0 = 1;
    animId = -1;

    result = func_800B6CD8_1E3D88(
        &slot->slotData,
        slot->model,
        arg0->unk0,
        arg0->unk4,
        arg0->unk8,
        arg0->unkE,
        arg0->unkC,
        temp_s0,
        arg0->unk16
    );

    switch (result) {
        case 1:
            animId = func_800015DC_21DC(temp_s4);
            break;
        case 2:
            animId = func_800015E4_21E4(temp_s4);
            break;
    }

    func_80002260_2E60(slot->model, arg0->unk10, arg0->unk14, arg0->unk13, animId, arg0->unk17);

    func_800015CC_21CC(slot->model, arg0->unk12);
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
    s32 value;

    slotData = &arg1->slots[arg2].slotData;
    value = (arg0->unk0 << 16) / 100;

    func_800B7760_1E4810(slotData, value, arg0->unk4);
    func_800B77C4_1E4874(slotData, value, arg0->unk4);
    func_800B7828_1E48D8(slotData, value, arg0->unk4);
}

s32 cutsceneChrZoom_isDone(void) {
    return 0;
}

void cutsceneChrBoardMove_init(void) {
}

s32 cutsceneChrBoardMove_validate(void) {
    return 0;
}

void cutsceneChrBoardMove_exec(cutsceneChrMove2_exec_arg *arg0, CutsceneManager *arg1, s8 arg2) {
    CutsceneSlot *slot = &arg1->slots[arg2];

    func_800B34B0_1E0560(arg2);

    func_800B7128_1E41D8(
        &slot->slotData,
        slot->model,
        arg0->unk0,
        arg0->unk4,
        arg0->unk8,
        arg0->unkE,
        arg0->unkC,
        arg0->unk10,
        arg0->unk16,
        arg0->unk17
    );

    func_80002220_2E20(slot->model, arg0->unk10, arg0->unk14, arg0->unk13, -1);

    func_800015CC_21CC(slot->model, arg0->unk12);
}

s32 cutsceneChrBoardMove_isDone(void) {
    return 0;
}

void cutsceneChrTurn2_init(void) {
}

s32 cutsceneChrTurn2_validate(void) {
    return 0;
}

extern s32 func_800B7450_1E4500(CutsceneSlotData *, SceneModel *, s16, s16, s32);

void cutsceneChrTurn2_exec(cutsceneChrTurn2_exec_arg *arg0, CutsceneManager *arg1, s8 arg2) {
    CutsceneSlot *slot;
    u16 temp_s3;
    s32 animId;
    s32 result;

    slot = &arg1->slots[arg2];

    temp_s3 = func_800B34B0_1E0560(arg2)->unk4;

    animId = -1;

    result = func_800B7450_1E4500(&slot->slotData, slot->model, arg0->unk0, arg0->unk8, arg0->unk6);

    switch (result) {
        case 1:
            animId = func_800015DC_21DC(temp_s3);
            break;
        case 2:
            animId = func_800015E4_21E4(temp_s3);
            break;
    }

    func_80002220_2E20(slot->model, arg0->unk2, arg0->unk4, -1, animId);
}

void cutsceneChrRotate_init(void) {
}

s32 cutsceneChrRotate_validate(void) {
    return 0;
}

void cutsceneChrRotate_exec(cutsceneChrRotate_exec_arg *arg0, CutsceneManager *arg1, s8 arg2) {
    CutsceneSlot *slot;

    slot = &arg1->slots[arg2];

    func_800B7620_1E46D0(&slot->slotData, arg0->unk0, arg0->unk6, arg0->unk8);
    func_800021E8_2DE8(slot->model, arg0->unk2, arg0->unk4, -1);
}

s32 cutsceneChrRotate_isDone(void) {
    return 0;
}

void cutsceneChrHop_init(void) {
}

s32 cutsceneChrHop_validate(void) {
    return 0;
}

void cutsceneChrHop_exec(cutsceneChrHop_exec_arg *a0, CutsceneManager *a1, s8 a2) {
    CutsceneSlot *s1 = &a1->slots[a2];

    func_800B788C_1E493C(&s1->slotData, a0->unk0, a0->unk4, a0->unk8);
    func_800021E8_2DE8(s1->model, a0->unkC, a0->unkE, 1);
}

void cutsceneChrBack_init(void) {
}

s32 cutsceneChrBack_validate(void) {
    return 0;
}

void cutsceneChrBack_exec(cutsceneChrBack_exec_arg *arg0, CutsceneManager *arg1, s8 arg2) {
    CutsceneSlot *slot;

    slot = &arg1->slots[arg2];

    func_800B7914_1E49C4(&slot->slotData, arg0->unk0, arg0->unk4, arg0->unk8, arg0->unkE);
    func_80002220_2E20(slot->model, arg0->unk10, arg0->unk14, arg0->unk13, -1);
    func_800015CC_21CC(slot->model, arg0->unk12);
}

s32 cutsceneChrBack_isDone(void) {
    return 0;
}

void cutsceneChrZoom2_init(void) {
}

s32 cutsceneChrZoom2_validate(void) {
    return 0;
}

void cutsceneChrZoom2_exec(cutsceneChrZoom2_exec_arg *arg0, CutsceneManager *arg1, s8 arg2) {
    CutsceneSlotData *slotData;
    s32 valueX;
    s32 valueY;
    s32 valueZ;

    slotData = &arg1->slots[arg2].slotData;
    valueX = (arg0->unk0 << 16) / 100;
    valueY = (arg0->unk4 << 16) / 100;
    valueZ = (arg0->unk8 << 16) / 100;

    func_800B7760_1E4810(slotData, valueX, arg0->unkC);
    func_800B77C4_1E4874(slotData, valueY, arg0->unkC);
    func_800B7828_1E48D8(slotData, valueZ, arg0->unkC);
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

    func_800B34B0_1E0560(arg2);

    func_800B7A60_1E4B10(&slot->slotData, slot->model, arg0->unk0, arg0->unk4, arg0->unk8, arg0->unkE);

    func_80002260_2E60(slot->model, arg0->unk10, arg0->unk14, arg0->unk13, -1, arg0->unk16);

    func_800015CC_21CC(slot->model, arg0->unk12);
}

s32 cutsceneChrUpDown_isDone(void) {
    return 0;
}

void cutsceneChrFace_init(void) {
}

s32 cutsceneChrFace_validate(void) {
    return 0;
}

void cutsceneChrFace_exec(s8 *arg0, CutsceneManager *arg1, s8 arg2) {
    CutsceneSlot *slot = &arg1->slots[arg2];
    do {
        func_80001688_2288(slot->model, *arg0);
    } while (0);
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
    func_80002260_2E60(slot->model, arg0->unk0, arg0->unk4, arg0->unk3, -1, -1);
    func_800015CC_21CC(slot->model, arg0->unk2);
}

s32 cutsceneChrAnime2_isDone(void) {
    return 0;
}

void cutsceneChrMove3_init(void) {
}

s32 cutsceneChrMove3_validate(void) {
    return 0;
}

void cutsceneChrMove3_exec(cutsceneChrMove3_exec_arg *arg0, CutsceneManager *arg1, s8 arg2) {
    CutsceneSlot *currentSlot = &arg1->slots[arg2];

    func_800B34B0_1E0560(arg2);

    func_800B6FA4_1E4054(
        &currentSlot->slotData,
        currentSlot->model,
        arg0->unk0,
        arg0->unk4,
        arg0->unk8,
        arg0->unkE,
        arg0->unkC
    );

    func_80002260_2E60(currentSlot->model, arg0->unk10, arg0->unk14, arg0->unk13, -1, arg0->unk16);

    func_800015CC_21CC(currentSlot->model, arg0->unk12);
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

    if (arg0->unk2 != 0) {
        func_800016E0_22E0(slot->model, arg0->unk1);
    } else {
        func_800016F8_22F8(slot->model, arg0->unk1);
    }
}

void cutsceneChrMoveSight_init(void) {
}

s32 cutsceneChrMoveSight_validate(void) {
    return 0;
}

void cutsceneChrMoveSight_exec(cutsceneChrMoveSight_exec_arg *arg0, CutsceneManager *arg1, s8 arg2) {
    s32 stackData[3];
    CutsceneSlot *slot;

    stackData[0] = arg0->unk0;
    stackData[1] = arg0->unk4;
    stackData[2] = arg0->unk8;

    slot = &arg1->slots[arg2];

    func_800B7B70_1E4C20(&slot->slotData, (s32 *)&stackData, arg0->unkE, arg0->unk18, arg0->unk1C);

    func_800021E8_2DE8(slot->model, arg0->unk10, arg0->unk14, arg0->unk13);

    func_800015CC_21CC(slot->model, arg0->unk12);
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

    slot = func_800B2A78_1DFB28(arg1, arg2);
    currentSlot = &arg1->slots[arg2];
    slot->unk0.ChrPayload.unk0 = arg0->unk4;

    currentAlpha = func_80001534_2134(currentSlot->model);

    if (slot->unk0.ChrPayload.unk0 == 0 || arg0->unk3 == currentAlpha) {
        func_8000152C_212C(currentSlot->model, arg0->unk3);
    } else {
        slot->unk0.ChrPayload.unk14 = arg0->unk0 << 16;
        slot->unk0.ChrPayload.unk18 = arg0->unk1 << 16;
        slot->unk0.ChrPayload.unk1C = arg0->unk2 << 16;
        slot->unk0.ChrPayload.unk20 = arg0->unk3 << 16;

        currentAlpha = func_80001534_2134(currentSlot->model);

        slot->unk0.ChrPayload.unk10 = currentAlpha << 16;
        slot->unk0.ChrPayload.unk24 = (slot->unk0.ChrPayload.unk20 - (currentAlpha << 16)) / slot->unk0.ChrPayload.unk0;

        func_800B2A24_1DFAD4(arg1, arg2);
    }
}

void cutsceneChrAlpha_update(CutsceneManager *arg0, s8 arg1) {
    CutsceneSlot *slot;
    CutsceneSlot *managerSlot;
    s32 newValue;

    slot = func_800B2A78_1DFB28(arg0, arg1);
    managerSlot = &arg0->slots[arg1];

    if (slot->unk0.ChrPayload.unk0 > 0) {
        func_8000152C_212C(managerSlot->model, slot->unk0.ChrPayload.padding[0xF]);
        slot->unk0.ChrPayload.unk0--;
        newValue = slot->unk0.ChrPayload.unk10 + slot->unk0.ChrPayload.unk24;
        slot->unk0.ChrPayload.unk10 = newValue;

        if (slot->unk0.ChrPayload.unk24 > 0) {
            if (slot->unk0.ChrPayload.unk20 < newValue) {
                slot->unk0.ChrPayload.unk0 = 0;
            }
        } else if (slot->unk0.ChrPayload.unk24 < 0) {
            if (newValue < slot->unk0.ChrPayload.unk20) {
                slot->unk0.ChrPayload.unk0 = 0;
            }
        } else {
            slot->unk0.ChrPayload.unk0 = 0;
        }
    } else {
        func_800B2A50_1DFB00(arg0, arg1);
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

void cutsceneChrPosition2_exec(cutsceneChrPosition2_exec_arg *arg0, CutsceneManager *arg1, s8 arg2) {
    CutsceneSlot *slot;
    CutsceneSlotData *slotData;

    slot = &arg1->slots[arg2];
    slotData = &slot->slotData;

    func_800B6C04_1E3CB4(slotData, arg0->unk0, arg0->unk4, arg0->unk8, arg0->unkC, arg0->unk16, arg0->unk18);
    setupSlotTransform(slotData);
    applyTransformToModel(slot->model, &slot->slotData.unk04);
    func_80002260_2E60(slot->model, arg0->unkE, arg0->unk12, arg0->unk11, -1, arg0->unk14);
    func_800015CC_21CC(slot->model, arg0->unk10);
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

    func_800B34B0_1E0560(arg2);

    func_800B75C4_1E4674(&currentSlot->slotData, currentSlot->model, arg0->unk0, arg0->unk8, arg0->unk6, arg0->unkA);

    func_80002220_2E20(currentSlot->model, arg0->unk2, arg0->unk4, -1, -1);
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

    if (arg0->unk0 != 0) {
        func_80001524_2124(slot->model);
    } else {
        func_80001518_2118(slot->model);
    }
}