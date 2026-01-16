#include "cutscene/cutscene_sound.h"
#include "1E1BA0.h"
#include "1E2890.h"
#include "race_session.h"

extern s32 gControllerInputs[4];

void debugHandleFanSoundInput(void) {
    s32 input = gControllerInputs[0x3];
    if (input & A_BUTTON) {
        playFanSoundOnChannel0();
        return;
    }

    if (input & B_BUTTON) {
        stopFanSoundOnChannel0();
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
        playFanSoundEffect(arg0->unk0, arg0->unk2, arg0->unkA, arg0->unk8);
        return;
    }

    scheduleRepeatingFanSoundEffect(arg0->unk0, arg0->unk2, arg0->unkA, temp_a3, (s32)arg0->unk8, (s32)arg0->unk6);
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
        playFanSoundAtPosition(arg0->unk0, arg0->unk2, arg0->unk8, slotData);
        return;
    }

    scheduleRepeatingFanSoundAtPosition(arg0->unk0, arg0->unk2, arg0->unk4, arg0->unk8, arg0->unk6, slotData);
}

void cutsceneSeStop_init(void) {
}

s32 cutsceneSeStop_validate(void) {
    return 0;
}

void cutsceneSeStop_exec(cutsceneSePlay_exec_arg *arg0) {
    stopSoundEffectChannel(arg0->unk2, arg0->unk0);
}
