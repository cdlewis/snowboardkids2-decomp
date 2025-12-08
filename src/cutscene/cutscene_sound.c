#include "cutscene/cutscene_sound.h"
#include "3E160.h"

extern s32 gControllerInputs[4];

extern void func_800B4C80_1E1D30(void);
extern void func_800B4CB0_1E1D60(void);
extern void func_800B57F0_1E28A0(s16, s16, s16, s16, s16, s16);
extern void func_800B4BDC_1E1C8C(s16, s16, s16, CutsceneSlotData *);
extern void func_800B598C_1E2A3C(s16, s16, s16, s16, s16, CutsceneSlotData *);

void func_800B20D0_1DF180(void) {
    s32 input = gControllerInputs[0x3];
    if (input & A_BUTTON) {
        func_800B4C80_1E1D30();
        return;
    }

    if (input & B_BUTTON) {
        func_800B4CB0_1E1D60();
    }
}

void cutsceneSePlay_init(void) {
}

s32 cutsceneSePlay_validate(void) {
    return 0;
}

void cutsceneSePlay_exec(cutsceneSePlay_exec_arg *arg0) {
    s16 temp_a3;

    temp_a3 = arg0->unk4;
    if (temp_a3 == 1) {
        func_800B4B30_1E1BE0(arg0->unk0, arg0->unk2, arg0->unkA, arg0->unk8);
        return;
    }

    func_800B57F0_1E28A0(arg0->unk0, arg0->unk2, arg0->unkA, temp_a3, (s32)arg0->unk8, (s32)arg0->unk6);
}

void cutsceneSe3dPlay_init(void) {
}

s32 cutsceneSe3dPlay_validate(void) {
    return 0;
}

void cutsceneSe3dPlay_exec(cutsceneSePlay_exec_arg *arg0, cutsceneSe3dPlay_exec_arg1_item *arg1, s8 arg2) {
    CutsceneSlotData **slotPtr;
    CutsceneSlotData *slotData;

    slotPtr = &arg1[arg2].unkF0;
    slotData = *slotPtr;

    if (arg0->unk4 == 1) {
        func_800B4BDC_1E1C8C(arg0->unk0, arg0->unk2, arg0->unk8, slotData);
        return;
    }

    func_800B598C_1E2A3C(arg0->unk0, arg0->unk2, arg0->unk4, arg0->unk8, arg0->unk6, slotData);
}

void cutsceneSeStop_init(void) {
}

s32 cutsceneSeStop_validate(void) {
    return 0;
}

void cutsceneSeStop_exec(cutsceneSePlay_exec_arg *arg0) {
    func_80057ABC_586BC(arg0->unk2, arg0->unk0);
}
