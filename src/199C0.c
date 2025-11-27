#include "common.h"
#include "graphics.h"
#include "task_scheduler.h"

typedef struct {
    s16 unk0;
    u8 unk2;
} task_mem_199C0;

extern u8 D_800A8CC8_A0038;
extern void (*D_8008D78C_8E38C[])(void);
extern s16 D_8009F220_9FE20;
void func_80018E2C_19A2C(void);
void func_80018E80_19A80(void);

void func_80018DC0_199C0(void) {
    task_mem_199C0 *mem;

    mem = (task_mem_199C0 *)allocateTaskMemory(4);
    mem->unk2 = 0;
    mem->unk0 = 0;

    if (D_800A8CC8_A0038 != 4) {
        func_800574A0_580A0(2);
    }

    createTaskQueue(D_8008D78C_8E38C[D_800A8CC8_A0038], 0x5A);
    setGameStateHandler(func_80018E2C_19A2C);
}

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

void func_80018E80_19A80(void) {
    func_800697F4_6A3F4(D_8009F220_9FE20);
}
