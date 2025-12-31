#include "3E160.h"
#include "common.h"
#include "graphics.h"
#include "task_scheduler.h"

void awaitCharacterSelect(void);
void awaitLevelSelectBasic(void);
void onLevelSelectProceed(void);

void initLevelSelectBasicState(void) {
    createTaskQueue(&initLevelSelectBasic, 100);
    setGameStateHandler(&awaitLevelSelectBasic);
}

void loadCharacterSelect(void);
void onLevelSelectCancel(void);

void awaitLevelSelectBasic(void) {
    s16 result = func_80069810_6A410();

    if (result == 0xFF) {
        terminateSchedulerWithCallback(&onLevelSelectCancel);
    } else if (result == 1) {
        setGameStateHandler(&loadCharacterSelect);
    }
}

void loadCharacterSelect(void) {
    func_800574A0_580A0(2);
    createTaskQueue(&func_800226F0_232F0, 100);
    setGameStateHandler(&awaitCharacterSelect);
}

void awaitCharacterSelect(void) {
    s16 result = func_80069810_6A410();

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
    func_800697F4_6A3F4(1);
}

void onLevelSelectCancel(void) {
    func_800697F4_6A3F4(0xFF);
}
