#include "1DFAA0.h"
#include "common.h"
#include "fan_effect.h"
#include "race_session.h"
#include "task_scheduler.h"

extern s8 gStopFanSoundTasks;

typedef struct {
    s16 fanIndex;
    s16 volume;
    s16 pan;
    s16 remainingCount;
    s16 channel;
    s16 interval;
    s16 currentFrame;
} RepeatingFanSoundEffectState;

void cleanupRepeatingFanSoundEffectTask(void);
void initRepeatingFanSound3DTask(void);
void cleanupRepeatingFanSound3DTask(void);
void initRepeatingFanSoundEffectTask(void);
void updateRepeatingFanSoundEffect(RepeatingFanSoundEffectState *arg0);

typedef struct {
    s16 fanIndex;
    s16 volume;
    s16 remainingCount;
    s16 pan;
    s16 interval;
    s16 currentFrame;
    CutsceneSlotData *cutsceneSlotData;
} RepeatingFanSound3DState;

void updateRepeatingFanSound3D(RepeatingFanSound3DState *arg0);

void stopFanSoundTasks(void) {
    gStopFanSoundTasks = 1;
}

void scheduleRepeatingFanSoundEffect(
    s16 fanIndex,
    s16 volume,
    s16 pan,
    s16 repeatCount,
    s16 interval,
    s16 minInterval
) {
    s16 *result;

    if (minInterval <= 0) {
        minInterval = 1;
    }

    result = scheduleTask(initRepeatingFanSoundEffectTask, 0, 0, 0);

    if (result != NULL) {
        result[0] = fanIndex;
        result[1] = volume;
        result[2] = pan;
        result[3] = repeatCount;
        result[4] = interval;
        result[5] = minInterval;
        result[6] = 0;
    }
}

void initRepeatingFanSoundEffectTask(void) {
    setCleanupCallback(cleanupRepeatingFanSoundEffectTask);
    gStopFanSoundTasks = 0;
    setCallback(updateRepeatingFanSoundEffect);
}

void updateRepeatingFanSoundEffect(RepeatingFanSoundEffectState *arg0) {
    if (arg0->remainingCount == 0 || gStopFanSoundTasks != 0) {
        terminateCurrentTask();
        return;
    }

    if ((arg0->currentFrame % arg0->interval) == 0) {
        playFanSoundEffect(arg0->fanIndex, arg0->volume, arg0->pan, arg0->channel);
        arg0->remainingCount--;
    }

    arg0->currentFrame++;
}

void cleanupRepeatingFanSoundEffectTask(void) {
}

void scheduleRepeatingFanSoundAtPosition(
    s16 fanIndex,
    s16 volume,
    s16 repeatCount,
    s16 pan,
    s16 interval,
    CutsceneSlotData *cutsceneSlotData
) {
    s16 *result;
    s16 temp_arg4;

    temp_arg4 = interval;
    if (interval <= 0) {
        temp_arg4 = 1;
    }

    result = scheduleTask(initRepeatingFanSound3DTask, 0, 0, 0);

    if (result != NULL) {
        result[0] = fanIndex;
        result[1] = volume;
        result[2] = repeatCount;
        result[3] = pan;
        result[4] = temp_arg4;
        result[5] = 0;
        *(CutsceneSlotData **)&result[6] = cutsceneSlotData;
    }
}

void initRepeatingFanSound3DTask(void) {
    setCleanupCallback(cleanupRepeatingFanSound3DTask);
    gStopFanSoundTasks = 0;
    setCallback(updateRepeatingFanSound3D);
}

void updateRepeatingFanSound3D(RepeatingFanSound3DState *arg0) {
    if (arg0->remainingCount == 0 || gStopFanSoundTasks != 0) {
        terminateCurrentTask();
        return;
    }

    if ((arg0->currentFrame % arg0->interval) == 0) {
        playFanSoundAtPosition(arg0->fanIndex, arg0->volume, arg0->pan, arg0->cutsceneSlotData);
        arg0->remainingCount--;
    }

    arg0->currentFrame++;
}

void cleanupRepeatingFanSound3DTask(void) {
}
