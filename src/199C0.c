#include "199C0.h"
#include "graphics.h"
#include "task_scheduler.h"

typedef struct {
    s16 frameCounter;
    u8 isComplete;
} StoryMapLocationState;

static void awaitStoryMapLocation(void);
static void onStoryMapLocationComplete(void);

void initStoryMapLocation(void) {
    StoryMapLocationState *state;

    state = (StoryMapLocationState *)allocateTaskMemory(4);
    state->isComplete = 0;
    state->frameCounter = 0;

    if (storyMapLocationIndex != 4) {
        playMusicTrack(2);
    }

    createTaskQueue(storyMapLocationHandlers[storyMapLocationIndex], 0x5A);
    setGameStateHandler(awaitStoryMapLocation);
}

void awaitStoryMapLocation(void) {
    s16 result;

    getCurrentAllocation();
    result = getSchedulerReturnValue();
    if (result != 0) {
        setMusicFadeOut(0xA);
        storyMapExitCode = result;
        terminateSchedulerWithCallback(onStoryMapLocationComplete);
    }
}

void onStoryMapLocationComplete(void) {
    returnToParentScheduler(storyMapExitCode);
}
