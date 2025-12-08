#include "3E160.h"
#include "common.h"
#include "graphics.h"
#include "task_scheduler.h"

void func_80021C90_22890(void);
void func_80021C00_22800(void);
void func_80021CE4_228E4(void);

void func_80021BD0_227D0(void) {
    createTaskQueue(&func_8001E5C0_1F1C0, 100);
    setGameStateHandler(&func_80021C00_22800);
}

void func_80021C58_22858(void);
void func_80021D00_22900(void);

void func_80021C00_22800(void) {
    s16 result = func_80069810_6A410();

    if (result == 0xFF) {
        terminateSchedulerWithCallback(&func_80021D00_22900);
    } else if (result == 1) {
        setGameStateHandler(&func_80021C58_22858);
    }
}

void func_80021C58_22858(void) {
    func_800574A0_580A0(2);
    createTaskQueue(&func_800226F0_232F0, 100);
    setGameStateHandler(&func_80021C90_22890);
}

void func_80021C90_22890(void) {
    s16 result = func_80069810_6A410();

    if (result == 0) {
        return;
    }

    if (result == 0xFF) {
        setGameStateHandler(&func_80021BD0_227D0);
    } else {
        terminateSchedulerWithCallback(&func_80021CE4_228E4);
    }
}

void func_80021CE4_228E4(void) {
    func_800697F4_6A3F4(1);
}

void func_80021D00_22900(void) {
    func_800697F4_6A3F4(0xFF);
}
