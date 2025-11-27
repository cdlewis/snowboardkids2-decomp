#include "common.h"
#include "graphics.h"
#include "task_scheduler.h"

extern s16 D_8009F220_9FE20;
extern void func_80018E80_19A80(void);

INCLUDE_ASM("asm/nonmatchings/199C0", func_80018DC0_199C0);

void func_80018E2C_19A2C(void) {
    s16 temp;

    getCurrentAllocation();
    temp = func_80069810_6A410();
    if (temp != 0) {
        func_80057564_58164(0xA);
        D_8009F220_9FE20 = temp;
        terminateSchedulerWithCallback(func_80018E80_19A80);
    }
}

INCLUDE_ASM("asm/nonmatchings/199C0", func_80018E80_19A80);
