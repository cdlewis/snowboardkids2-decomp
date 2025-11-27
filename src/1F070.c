#include "288A0.h"
#include "6E840.h"
#include "common.h"
#include "task_scheduler.h"

extern void func_80019CD0_1A8D0(void);
extern void func_8001E508_1F108(void);

typedef struct {
    Node_70B00 unk0;
    s16 unk1D8;
} func_8001E470_1F070_allocation;

void func_8001E470_1F070(void) {
    func_8001E470_1F070_allocation *temp_s0 = (func_8001E470_1F070_allocation *)allocateTaskMemory(0x1E0);
    setupTaskSchedulerNodes(0x14, 0, 0, 0, 0, 0, 0, 0);
    temp_s0->unk1D8 = 0;
    func_80027CA0_288A0(&temp_s0->unk0, 0, 0xA, 0);
    func_8006FDA0_709A0(0, 0, 8);
    scheduleTask(&func_80019CD0_1A8D0, 0, 0, 0x5A);
    setGameStateHandler(&func_8001E508_1F108);
}

INCLUDE_ASM("asm/nonmatchings/1F070", func_8001E508_1F108);

void func_8001E570_1F170(void) {
    func_800697F4_6A3F4(1);
}
