#include "cutscene/sys_effect.h"

void cutsceneEffectDisp_init(void) {
}

s32 cutsceneEffectDisp_validate(void) {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/cutscene/sys_effect", cutsceneEffectDisp_exec);

s32 cutsceneEffectDisp_isDone(void) {
    return 0;
}

void cutsceneEffectFan_init(void) {
}

s32 cutsceneEffectFan_validate(void) {
    return 0;
}

extern void func_800B4F60_1E2010(s32, s16);

void cutsceneEffectFan_exec(cutsceneEffectFan_exec_arg *arg0, CutsceneSlot *arg1, s8 arg2) {
    func_800B4F60_1E2010(arg1[arg2].slotData.unkA4, arg0->unk0);
}

s32 cutsceneEffectFan_isDone(void) {
    return 0;
}

void cutsceneEffectRocket_init(void) {
}

s32 cutsceneEffectRocket_validate(void) {
    return 0;
}

extern void func_8000BBE0_C7E0(s32, s16);

void cutsceneEffectRocket_exec(cutsceneEffectRocket_exec_arg *arg0, CutsceneSlot *arg1, s8 arg2) {
    func_8000BBE0_C7E0(arg1[arg2].slotData.unkA4, arg0->unk0);
}

s32 cutsceneEffectRocket_isDone(void) {
    return 0;
}

void cutsceneEffectMLight_init(void) {
}

s32 cutsceneEffectMLight_validate(void) {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/cutscene/sys_effect", cutsceneEffectMLight_exec);

s32 cutsceneEffectMLight_isDone(void) {
    return 0;
}

void cutsceneEffectTrick_init(void) {
}

s32 cutsceneEffectTrick_validate(void) {
    return 0;
}

void cutsceneEffectTrick_exec(cutsceneEffectTrick_exec_arg *arg0, CutsceneManager *arg1, s8 arg2) {
    CutsceneSlot *temp_v0 = func_800B2A78_1DFB28(arg1, arg2);
    temp_v0->unk0.TrickPayload.unk0 = arg0->unk0;
    temp_v0->unk0.TrickPayload.unk2 = arg0->unk2;
    func_800B2A24_1DFAD4(arg1, arg2);
}

extern void func_800B523C_1E22EC(void *, s16);

void cutsceneEffectTrick_update(CutsceneManager *arg0, s8 arg1) {
    CutsceneSlot *slot;
    CutsceneSlot *temp_s0;

    slot = &arg0->slots[arg1];
    temp_s0 = func_800B2A78_1DFB28(arg0, arg1);

    if (temp_s0->unk0.TrickPayload.unk0 > 0) {
        func_800B523C_1E22EC(slot->model, temp_s0->unk0.TrickPayload.unk2);
        temp_s0->unk0.TrickPayload.unk0 -= 1;
    } else {
        func_800B2A50_1DFB00(arg0, arg1);
    }
}

s32 cutsceneEffectTrick_isDone(void) {
    return 0;
}

void cutsceneInitTrack_init(void) {
}

s32 cutsceneInitTrack_validate(void) {
    return 0;
}

void cutsceneInitTrack_exec(void) {
}
