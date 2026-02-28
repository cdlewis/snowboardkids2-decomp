#include "common.h"
#include "graphics/graphics.h"
#include "race/race_session.h"
#include "story/map_events.h"
#include "system/task_scheduler.h"

extern s32 gControllerInputs;

void awaitStoryMapExtraIntro(void);
void onStoryMapExtraIntroComplete(void);

typedef struct {
    ViewportNode unk0;
    u16 waitCounter;
} StoryMapExtraIntroAllocation;

void initStoryMapExtraIntro(void) {
    StoryMapExtraIntroAllocation *temp_s0 = (StoryMapExtraIntroAllocation *)allocateTaskMemory(0x1E0);
    setupTaskSchedulerNodes(0x14, 0, 0, 0, 0, 0, 0, 0);
    temp_s0->waitCounter = 0;
    initMenuCameraNode(&temp_s0->unk0, 0, 0xA, 0);
    setViewportFadeValue(0, 0, 8);
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
    returnToParentScheduler(1);
}
