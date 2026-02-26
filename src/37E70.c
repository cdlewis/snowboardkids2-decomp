#include "288A0.h"
#include "36B80.h"
#include "D_800AFE8C_A71FC_type.h"
#include "assets.h"
#include "audio.h"
#include "common.h"
#include "graphics.h"
#include "options_menu.h"
#include "race_session.h"
#include "rom_loader.h"
#include "task_scheduler.h"

extern s32 gControllerInputs;

void func_800373E0_37FE0(void);
void onOptionsMenuFadeInComplete(void);
void onOptionsMenuFadeOutComplete(void);
void onOptionsMenuExit(void);

typedef struct {
    /* 0x000 */ u8 pad0[0x1D8];
    /* 0x1D8 */ void *assetData;
    /* 0x1DC */ void *graphicsData;
    /* 0x1E0 */ u16 frameCounter;
    /* 0x1E2 */ u16 menuState;
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

void func_800373E0_37FE0(void) {
    MenuAllocation *state;
    u8 shouldExit;
    u8 prevIndex;
    u16 prevCompare;
    u8 curIndex;
    s32 i;

    state = (MenuAllocation *)getCurrentAllocation();
    shouldExit = 0;

    switch (state->menuState) {
        case 0:
            prevIndex = state->selectedIndex;
            prevCompare = prevIndex;
            if (gControllerInputs & 0x10800) {
                if (state->selectedIndex != 0) {
                    state->selectedIndex--;
                }
            } else if (gControllerInputs & 0x20400) {
                if (state->selectedIndex != 3) {
                    state->selectedIndex++;
                }
            }

            curIndex = state->selectedIndex;
            if (prevCompare != curIndex) {
                state->itemTimers[curIndex] = 0;
                playSoundEffect(0x2B);
                break;
            }

            if (gControllerInputs & 0x9000) {
                if (curIndex == 3) {
                    playSoundEffect(0x2C);
                    state->menuState = 1;
                    goto update_items;
                }
            }

            if (gControllerInputs & 0xC8300) {
                curIndex = state->selectedIndex;
                if (curIndex == 3) {

                } else {
                    if (curIndex == 0) {
                        D_800AFE8C_A71FC->optionToggle1 = (D_800AFE8C_A71FC->optionToggle1 + 1) & 1;
                    } else if (curIndex == 1) {
                        D_800AFE8C_A71FC->optionToggle2 = (D_800AFE8C_A71FC->optionToggle2 + 1) & 1;
                    } else if (curIndex == 2) {
                        D_800AFE8C_A71FC->customLapEnabled = (D_800AFE8C_A71FC->customLapEnabled + 1) & 1;
                        if (D_800AFE8C_A71FC->customLapEnabled == 0) {
                            D_800AFE8C_A71FC->customLapCount = 3;
                            D_800AFE8C_A71FC->playerBoardIds[0x10] = 3;
                        }
                    }
                    playSoundEffect(0x2B);
                }
            } else if (gControllerInputs & 0x4000) {
                playSoundEffect(0x2E);
                shouldExit = 1;
            }

        update_items:
            for (i = 0; i < 4; i++) {
                if (i == state->selectedIndex) {
                    state->itemTimers[i] = state->itemTimers[i] + 1;
                    if (state->itemTimers[state->selectedIndex] < 0x11) {
                        state->itemValues[i] = state->itemValues[i] + 0xE;
                    } else {
                        state->itemValues[i] = state->itemValues[i] - 0xE;
                    }
                    if (state->itemTimers[state->selectedIndex] != 0x20) {
                        continue;
                    }
                }
                state->itemTimers[i] = 0;
                state->itemValues[i] = 0;
            }
            break;

        case 1:
            state->frameCounter++;
            if ((u16)state->frameCounter == 0x11) {
                state->frameCounter = 0;
                shouldExit = 1;
            }
            break;
    }

    if (shouldExit) {
        setMusicFadeOut(0x20);
        setViewportFadeValue(0, 0xFF, 0x10);
        setGameStateHandler(onOptionsMenuFadeOutComplete);
    }
}

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
