#include "common.h"
#include "src/task_scheduler.h"

extern void func_80027CA0_288A0(void *, s32, s32, s32);
extern void func_80019CD0_1A8D0(void);
void func_8001C898_1D498(void);

typedef struct {
    u8 pad[0x1D8];
    s16 unk1D8;
} func_8001C810_allocation;

void func_8001C810_1D410(void) {
    func_8001C810_allocation *temp_s0 = (func_8001C810_allocation *)allocateTaskMemory(0x1E0);
    setupTaskSchedulerNodes(0x14, 0, 0, 0, 0, 0, 0, 0);
    temp_s0->unk1D8 = 0;
    func_80027CA0_288A0(temp_s0, 0, 0xA, 0);
    scheduleTask(&func_80019CD0_1A8D0, 0, 0, 0x5A);
    setGameStateHandler(&func_8001C898_1D498);
}

INCLUDE_ASM("asm/nonmatchings/1D410", func_8001C898_1D498);

void func_8001C900_1D500(void) {
    func_800697F4_6A3F4(1);
}
