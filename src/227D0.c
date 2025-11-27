#include "common.h"
#include "graphics.h"
#include "task_scheduler.h"

extern void func_800226F0_232F0(void);
extern void func_80021BD0_227D0(void);
extern void func_80021CE4_228E4(void);

void func_80021C90_22890(void);

INCLUDE_ASM("asm/nonmatchings/227D0", func_80021BD0_227D0);

INCLUDE_ASM("asm/nonmatchings/227D0", func_80021C00_22800);

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

INCLUDE_ASM("asm/nonmatchings/227D0", func_80021CE4_228E4);

INCLUDE_ASM("asm/nonmatchings/227D0", func_80021D00_22900);
