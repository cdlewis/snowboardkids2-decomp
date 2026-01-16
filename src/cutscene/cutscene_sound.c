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
    s16 repeatCount;

    repeatCount = arg0->repeatCount;
    if (repeatCount == 1) {
        playFanSoundEffect(arg0->soundEffectId, arg0->volume, arg0->pan, arg0->channelOrInterval);
        return;
    }

    scheduleRepeatingFanSoundEffect(
        arg0->soundEffectId,
        arg0->volume,
        arg0->pan,
        repeatCount,
        (s32)arg0->channelOrInterval,
        (s32)arg0->minInterval
    );
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

    if (arg0->repeatCount == 1) {
        playFanSoundAtPosition(arg0->soundEffectId, arg0->volume, arg0->channelOrInterval, slotData);
        return;
    }

    scheduleRepeatingFanSoundAtPosition(
        arg0->soundEffectId,
        arg0->volume,
        arg0->repeatCount,
        arg0->channelOrInterval,
        arg0->minInterval,
        slotData
    );
}

void cutsceneSeStop_init(void) {
}

s32 cutsceneSeStop_validate(void) {
    return 0;
}

void cutsceneSeStop_exec(cutsceneSePlay_exec_arg *arg0) {
    stopSoundEffectChannel(arg0->volume, arg0->soundEffectId);
}
