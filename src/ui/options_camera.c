#include "D_800AFE8C_A71FC_type.h"
#include "assets.h"
#include "audio/audio.h"
#include "common.h"
#include "common_bss.h"
#include "graphics/graphics.h"
#include "os_cont.h"
#include "race/race_session.h"
#include "story/map_events.h"
#include "system/task_scheduler.h"
#include "text/font_render.h"
#include "ui/options_menu.h"

void updateOptionsMenu(void);
void onOptionsMenuFadeInComplete(void);
void onOptionsMenuFadeOutComplete(void);
void onOptionsMenuExit(void);

void initOptionsMenu(void) {
    OptionsMenuState *allocation;
    s32 i;

    allocation = allocateTaskMemory(0x1F0);
    setupTaskSchedulerNodes(0x14, 0, 0, 0, 0, 0, 0, 0);
    initMenuCameraNode(&allocation->viewport, 8, 0xF, 1);
    setViewportFadeValue(0, 0xFF, 0);
    setViewportFadeValue(0, 0, 0x10);
    allocation->menuSpriteAsset = loadCompressedData(&optionsMenuSprites_ROM_START, &optionsMenuSprites_ROM_END, 0xBB8);
    allocation->textRenderAsset = loadTextRenderAsset(1);
    allocation->exitBlinkTimer = 0;
    allocation->phase = OPTIONS_MENU_SELECTING;
    allocation->selectedOption = 0;
    for (i = 0; i < 4; i++) {
        allocation->highlightTimers[i] = 0;
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
        setGameStateHandler(updateOptionsMenu);
    }
}

void updateOptionsMenu(void) {
    OptionsMenuState *state;
    u8 shouldExit;
    u8 prevIndex;
    u16 prevCompare;
    u8 curIndex;
    s32 i;

    state = (OptionsMenuState *)getCurrentAllocation();
    shouldExit = 0;

    switch (state->phase) {
        case OPTIONS_MENU_SELECTING:
            prevIndex = state->selectedOption;
            prevCompare = prevIndex;
            if (gControllerInputs[0] & (STICK_UP | U_JPAD)) {
                if (state->selectedOption != 0) {
                    state->selectedOption--;
                }
            } else if (gControllerInputs[0] & (STICK_DOWN | D_JPAD)) {
                if (state->selectedOption != 3) {
                    state->selectedOption++;
                }
            }

            curIndex = state->selectedOption;
            if (prevCompare != curIndex) {
                state->highlightTimers[curIndex] = 0;
                playSoundEffect(0x2B);
                break;
            }

            if (gControllerInputs[0] & (A_BUTTON | START_BUTTON)) {
                if (curIndex == 3) {
                    playSoundEffect(0x2C);
                    state->phase = OPTIONS_MENU_EXIT_DELAY;
                    goto update_items;
                }
            }

            if (gControllerInputs[0] & (STICK_LEFT | STICK_RIGHT | L_JPAD | R_JPAD | A_BUTTON)) {
                curIndex = state->selectedOption;
                if (curIndex == 3) {

                } else {
                    if (curIndex == 0) {
                        gGameSessionContext->optionToggle1 = (gGameSessionContext->optionToggle1 + 1) & 1;
                    } else if (curIndex == 1) {
                        gGameSessionContext->optionToggle2 = (gGameSessionContext->optionToggle2 + 1) & 1;
                    } else if (curIndex == 2) {
                        gGameSessionContext->customLapEnabled = (gGameSessionContext->customLapEnabled + 1) & 1;
                        if (gGameSessionContext->customLapEnabled == 0) {
                            gGameSessionContext->customLapCount = 3;
                            gGameSessionContext->playerBoardIds[0x10] = 3;
                        }
                    }
                    playSoundEffect(0x2B);
                }
            } else if (gControllerInputs[0] & B_BUTTON) {
                playSoundEffect(0x2E);
                shouldExit = 1;
            }

        /* Pulse the selected option's highlight alpha up and down over 0x20 frames. */
        update_items:
            for (i = 0; i < 4; i++) {
                if (i == state->selectedOption) {
                    state->highlightTimers[i] = state->highlightTimers[i] + 1;
                    if (state->highlightTimers[state->selectedOption] < 0x11) {
                        state->highlightAlphas[i] = state->highlightAlphas[i] + 0xE;
                    } else {
                        state->highlightAlphas[i] = state->highlightAlphas[i] - 0xE;
                    }
                    if (state->highlightTimers[state->selectedOption] != 0x20) {
                        continue;
                    }
                }
                state->highlightTimers[i] = 0;
                state->highlightAlphas[i] = 0;
            }
            break;

        case OPTIONS_MENU_EXIT_DELAY:
            state->exitBlinkTimer++;
            if (state->exitBlinkTimer == 0x11) {
                state->exitBlinkTimer = 0;
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
    OptionsMenuState *allocation = (OptionsMenuState *)getCurrentAllocation();

    if (getViewportFadeMode(NULL) != 0) {
        return;
    }

    unlinkNode(&allocation->viewport);
    allocation->menuSpriteAsset = freeNodeMemory(allocation->menuSpriteAsset);
    allocation->textRenderAsset = freeNodeMemory(allocation->textRenderAsset);
    terminateSchedulerWithCallback(onOptionsMenuExit);
}

void onOptionsMenuExit(void) {
    returnToParentScheduler(1);
}
