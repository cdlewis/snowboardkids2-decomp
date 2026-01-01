#include "288A0.h"
#include "36B80.h"
#include "38310.h"
#include "3E160.h"
#include "6E840.h"
#include "common.h"
#include "graphics.h"
#include "overlay.h"
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
    /* 0x1E4 */ u8 pad1E4[4]; // itemTimers - zeroed via offset trick
    /* 0x1E8 */ u8 itemValues[4];
    /* 0x1EC */ u8 selectedIndex;
} MenuAllocation;

void initOptionsMenu(void) {
    MenuAllocation *allocation;
    s32 i;
    MenuAllocation *ptr;

    allocation = allocateTaskMemory(0x1F0);
    setupTaskSchedulerNodes(0x14, 0, 0, 0, 0, 0, 0, 0);
    initMenuCameraNode((Node_70B00 *)allocation, 8, 0xF, 1);
    func_8006FDA0_709A0(0, 0xFF, 0);
    func_8006FDA0_709A0(0, 0, 0x10);
    allocation->assetData = loadCompressedData(&_4196E0_ROM_START, &_4196E0_ROM_END, 0xBB8);
    allocation->graphicsData = loadTextRenderAsset(1);
    allocation->frameCounter = 0;
    allocation->menuState = 0;
    allocation->selectedIndex = 0;
    i = 3;
    ptr = (MenuAllocation *)((u8 *)allocation + 3);
    do {
        ptr->pad1E4[0] = 0;
        i--;
        ptr = (MenuAllocation *)((u8 *)ptr - 1);
    } while (i >= 0);
    scheduleTask(initOptionsMenuTitle, 0, 0, 0x5A);
    scheduleTask(initOptionsMenuToggles, 0, 0, 0x5A);
    scheduleTask(initOptionsMenuLabels, 0, 0, 0x5A);
    scheduleTask(func_80037E78_38A78, 0, 0, 0x5A);
    func_800574A0_580A0(6);
    setGameStateHandler(onOptionsMenuFadeInComplete);
}

void onOptionsMenuFadeInComplete(void) {
    if (func_8006FE10_70A10(0) == 0) {
        setGameStateHandler(func_800373E0_37FE0);
    }
}

INCLUDE_ASM("asm/nonmatchings/37E70", func_800373E0_37FE0);

void onOptionsMenuFadeOutComplete(void) {
    MenuAllocation *allocation = (MenuAllocation *)getCurrentAllocation();

    if (func_8006FE10_70A10(NULL) != 0) {
        return;
    }

    unlinkNode((Node_70B00 *)allocation);
    allocation->assetData = freeNodeMemory(allocation->assetData);
    allocation->graphicsData = freeNodeMemory(allocation->graphicsData);
    terminateSchedulerWithCallback(onOptionsMenuExit);
}

void onOptionsMenuExit(void) {
    func_800697F4_6A3F4(1);
}
