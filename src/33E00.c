#include "common.h"
#include "task_scheduler.h"

extern void func_80033254_33E54(void);

extern u8 gControllerPollingEnabled;
extern u8 gMotorInitCompleteMask;

void initControllerMotors(void) {
    u8 *ptr;
    s32 i;

    ptr = allocateTaskMemory(4);
    i = 3;
    ptr += 3;
    gControllerPollingEnabled = 1;
    gMotorInitCompleteMask = 0;
    for (; i >= 0; i--) {
        *ptr-- = 0;
    }
    setGameStateHandler(func_80033254_33E54);
}

INCLUDE_ASM("asm/nonmatchings/33E00", func_80033254_33E54);

void onMotorInitComplete(void) {
    func_800697F4_6A3F4(1);
}
