#include "288A0.h"
#include "3E160.h"
#include "6E840.h"
#include "common.h"
#include "task_scheduler.h"

extern s32 gControllerInputs;

typedef struct {
    Node_70B00 unk0;
    u16 waitCounter;
} PostUnlockLocationIntroAllocation;

void postUnlockLocationWaitForDismiss(void);
void func_8001C900_1D500(void);

void initPostUnlockLocationIntro(void) {
    PostUnlockLocationIntroAllocation *allocation = (PostUnlockLocationIntroAllocation *)allocateTaskMemory(0x1E0);
    setupTaskSchedulerNodes(0x14, 0, 0, 0, 0, 0, 0, 0);
    allocation->waitCounter = 0;
    func_80027CA0_288A0((Node_70B00 *)allocation, 0, 0xA, 0);
    scheduleTask(&storyMapLocationTextTask, 0, 0, 0x5A);
    setGameStateHandler(&postUnlockLocationWaitForDismiss);
}

void postUnlockLocationWaitForDismiss(void) {
    PostUnlockLocationIntroAllocation *allocation = (PostUnlockLocationIntroAllocation *)getCurrentAllocation();

    allocation->waitCounter = allocation->waitCounter + 1;
    __asm__ volatile("" ::: "memory");

    if (gControllerInputs & A_BUTTON) {
        allocation->waitCounter = 60;
    }

    if (!(allocation->waitCounter < 60)) {
        unlinkNode(&allocation->unk0);
        terminateSchedulerWithCallback(&func_8001C900_1D500);
    }
}

void func_8001C900_1D500(void) {
    func_800697F4_6A3F4(1);
}
