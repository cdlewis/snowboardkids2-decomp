#include "288A0.h"
#include "3E160.h"
#include "6E840.h"
#include "common.h"
#include "task_scheduler.h"

extern s32 gControllerInputs;

typedef struct {
    Node_70B00 unk0;
    u16 unk1D8;
} func_8001C810_allocation;

void func_8001C898_1D498(void);
void func_8001C900_1D500(void);

void func_8001C810_1D410(void) {
    func_8001C810_allocation *temp_s0 = (func_8001C810_allocation *)allocateTaskMemory(0x1E0);
    setupTaskSchedulerNodes(0x14, 0, 0, 0, 0, 0, 0, 0);
    temp_s0->unk1D8 = 0;
    func_80027CA0_288A0((Node_70B00 *)temp_s0, 0, 0xA, 0);
    scheduleTask(&storyMapLocationTextTask, 0, 0, 0x5A);
    setGameStateHandler(&func_8001C898_1D498);
}

void func_8001C898_1D498(void) {
    func_8001C810_allocation *allocation = (func_8001C810_allocation *)getCurrentAllocation();

    allocation->unk1D8 = allocation->unk1D8 + 1;
    __asm__ volatile("" ::: "memory");

    if (gControllerInputs & A_BUTTON) {
        allocation->unk1D8 = 60;
    }

    if (!(allocation->unk1D8 < 60)) {
        unlinkNode(&allocation->unk0);
        terminateSchedulerWithCallback(&func_8001C900_1D500);
    }
}

void func_8001C900_1D500(void) {
    func_800697F4_6A3F4(1);
}
