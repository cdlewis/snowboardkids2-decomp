#include "common.h"
#include "graphics.h"
#include "task_scheduler.h"

typedef struct {
    s16 frameCounter;
    u8 isComplete;
} StoryMapLocationState;

extern u8 storyMapLocationIndex;
extern void (*storyMapLocationHandlers[])(void);
extern s16 storyMapExitCode;
void awaitStoryMapLocation(void);
void onStoryMapLocationComplete(void);

void initStoryMapLocation(void) {
    StoryMapLocationState *state;

    state = (StoryMapLocationState *)allocateTaskMemory(4);
    state->isComplete = 0;
    state->frameCounter = 0;

    if (storyMapLocationIndex != 4) {
        func_800574A0_580A0(2);
    }

    createTaskQueue(storyMapLocationHandlers[storyMapLocationIndex], 0x5A);
    setGameStateHandler(awaitStoryMapLocation);
}

void awaitStoryMapLocation(void) {
    s16 result;

    getCurrentAllocation();
    result = func_80069810_6A410();
    if (result != 0) {
        func_80057564_58164(0xA);
        storyMapExitCode = result;
        terminateSchedulerWithCallback(onStoryMapLocationComplete);
    }
}

void onStoryMapLocationComplete(void) {
    func_800697F4_6A3F4(storyMapExitCode);
}
