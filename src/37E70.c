#include "288A0.h"
#include "36B80.h"
#include "38310.h"
#include "audio.h"
#include "common.h"
#include "graphics.h"
#include "race_session.h"
#include "rom_loader.h"
#include "task_scheduler.h"

USE_ASSET(_4196E0);

extern void func_800373E0_37FE0(void);
void onOptionsMenuFadeInComplete(void);
void onOptionsMenuExit(void);

typedef struct {
    /* 0x000 */ u8 pad0[0x1D8];
    /* 0x1D8 */ void *assetData;
    /* 0x1DC */ void *graphicsData;
    /* 0x1E0 */ s16 frameCounter;
    /* 0x1E2 */ s16 menuState;
    /* 0x1E4 */ u8 itemTimers[4];
    /* 0x1E8 */ u8 itemValues[4];
    /* 0x1EC */ u8 selectedIndex;
} MenuAllocation;

void initOptionsMenu(void) {
    MenuAllocation *allocation;
    s32 i;

    allocation = allocateTaskMemory(0x1F0);
    setupTaskSchedulerNodes(0x14, 0, 0, 0, 0, 0, 0, 0);
    initMenuCameraNode((ViewportNode *)allocation, 8, 0xF, 1);
    setViewportFadeValue(0, 0xFF, 0);
    setViewportFadeValue(0, 0, 0x10);
    allocation->assetData = loadCompressedData(&_4196E0_ROM_START, &_4196E0_ROM_END, 0xBB8);
    allocation->graphicsData = loadTextRenderAsset(1);
    allocation->frameCounter = 0;
    allocation->menuState = 0;
    allocation->selectedIndex = 0;
    for (i = 0; i < 4; i++) {
        allocation->itemTimers[i] = 0;
    }
    scheduleTask(initOptionsMenuTitle, 0, 0, 0x5A);
    scheduleTask(initOptionsMenuToggles, 0, 0, 0x5A);
    scheduleTask(initOptionsMenuLabels, 0, 0, 0x5A);
    scheduleTask(initOptionsMenuCursors, 0, 0, 0x5A);
    playMusicTrack(6);
    setGameStateHandler(onOptionsMenuFadeInComplete);
}

void onOptionsMenuFadeInComplete(void) {
    if (getViewportFadeMode(0) == 0) {
        setGameStateHandler(func_800373E0_37FE0);
    }
}

INCLUDE_ASM("asm/nonmatchings/37E70", func_800373E0_37FE0);

void onOptionsMenuFadeOutComplete(void) {
    MenuAllocation *allocation = (MenuAllocation *)getCurrentAllocation();

    if (getViewportFadeMode(NULL) != 0) {
        return;
    }

    unlinkNode((ViewportNode *)allocation);
    allocation->assetData = freeNodeMemory(allocation->assetData);
    allocation->graphicsData = freeNodeMemory(allocation->graphicsData);
    terminateSchedulerWithCallback(onOptionsMenuExit);
}

void onOptionsMenuExit(void) {
    returnToParentScheduler(1);
}
