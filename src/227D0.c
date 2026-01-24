#include "common.h"
#include "graphics.h"
#include "race_session.h"
#include "task_scheduler.h"

void awaitCharacterSelect(void);
void awaitLevelSelectBasic(void);
void loadCharacterSelect(void);
void onLevelSelectCancel(void);
void onLevelSelectProceed(void);

void initLevelSelectBasicState(void) {
    createTaskQueue(&initLevelSelectBasic, 100);
    setGameStateHandler(&awaitLevelSelectBasic);
}

void awaitLevelSelectBasic(void) {
    s16 result = getSchedulerReturnValue();

    if (result == 0xFF) {
        terminateSchedulerWithCallback(&onLevelSelectCancel);
    } else if (result == 1) {
        setGameStateHandler(&loadCharacterSelect);
    }
}

void loadCharacterSelect(void) {
    playMusicTrack(2);
    createTaskQueue(&func_800226F0_232F0, 100);
    setGameStateHandler(&awaitCharacterSelect);
}

void awaitCharacterSelect(void) {
    s16 result = getSchedulerReturnValue();

    if (result == 0) {
        return;
    }

    if (result == 0xFF) {
        setGameStateHandler(&initLevelSelectBasicState);
    } else {
        terminateSchedulerWithCallback(&onLevelSelectProceed);
    }
}

void onLevelSelectProceed(void) {
    returnToParentScheduler(1);
}

void onLevelSelectCancel(void) {
    returnToParentScheduler(0xFF);
}
