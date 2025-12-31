#include "288A0.h"
#include "3E160.h"
#include "6E840.h"
#include "common.h"
#include "task_scheduler.h"

extern s32 gControllerInputs;

void awaitStoryMapExtraIntro(void);
void onStoryMapExtraIntroComplete(void);

typedef struct {
    Node_70B00 unk0;
    u16 waitCounter;
} StoryMapExtraIntroAllocation;

void initStoryMapExtraIntro(void) {
    StoryMapExtraIntroAllocation *temp_s0 = (StoryMapExtraIntroAllocation *)allocateTaskMemory(0x1E0);
    setupTaskSchedulerNodes(0x14, 0, 0, 0, 0, 0, 0, 0);
    temp_s0->waitCounter = 0;
    func_80027CA0_288A0(&temp_s0->unk0, 0, 0xA, 0);
    func_8006FDA0_709A0(0, 0, 8);
    scheduleTask(&storyMapLocationTextTask, 0, 0, 0x5A);
    setGameStateHandler(&awaitStoryMapExtraIntro);
}

void awaitStoryMapExtraIntro(void) {
    StoryMapExtraIntroAllocation *allocation = (StoryMapExtraIntroAllocation *)getCurrentAllocation();

    allocation->waitCounter = allocation->waitCounter + 1;
    __asm__ volatile("" ::: "memory");

    if (gControllerInputs & A_BUTTON) {
        allocation->waitCounter = 60;
    }

    if (!(allocation->waitCounter < 60)) {
        unlinkNode(&allocation->unk0);
        terminateSchedulerWithCallback(&onStoryMapExtraIntroComplete);
    }
}

void onStoryMapExtraIntroComplete(void) {
    func_800697F4_6A3F4(1);
}
