#include "ui/player_select.h"
#include "D_800AFE8C_A71FC_type.h"
#include "assets.h"
#include "audio/audio.h"
#include "common.h"
#include "graphics/graphics.h"
#include "os_cont.h"
#include "race/race_main.h"
#include "story/map_events.h"
#include "system/controller_io.h"
#include "system/rom_loader.h"
#include "system/task_scheduler.h"
#include "ui/player_select_sprites.h"

USE_OVERLAY(_1DB7A0);

u8 gPlayerSlotDefaults[] = { 0x00, 0x02, 0x05, 0x01, 0x00, 0x00, 0x00, 0x00,
                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

extern u8 gConnectedControllerMask;

void handlePlayerCountSelectInput(void);
void exitPlayerCountSelect(void);
void onPlayerCountProceed(void);
void onPlayerCountCancel(void);
void awaitPlayerCountSelectFadeIn(void);

void initPlayerCountSelectState(void) {
    PlayerCountSelectState *state;
    s32 i;
    u8 temp;
    u8 numControllers;

    state = allocateTaskMemory(sizeof(PlayerCountSelectState));

    LOAD_OVERLAY(_1DB7A0);

    setupTaskSchedulerNodes(0x14, 0, 0, 0, 0, 0, 0, 0);
    state->frameCounter = 0;
    state->menuResult = PLAYER_COUNT_RESULT_NONE;
    state->connectedControllerCount = 0;
    state->menuState = PLAYER_COUNT_MENU_SELECTING;

    for (i = 0; i < 4; i++) {
        if ((gConnectedControllerMask >> i) & 1) {
            state->connectedControllerCount = state->connectedControllerCount + 1;
        }
    }

    temp = gGameSessionContext->numPlayers;
    if (temp != 0) {
        state->playerCount.selectedPlayerIndex = temp - 1;
    } else {
        state->playerCount.selectedPlayerIndex = 0;
    }

    numControllers = state->connectedControllerCount;
    if (numControllers < state->playerCount.selectedPlayerIndex + 1) {
        state->playerCount.selectedPlayerIndex = numControllers - 1;
    }

    initMenuCameraNode(&state->node, 8, 0xA, 1);

    setViewportFadeValue(&state->node, 0xFF, 0);
    setViewportFadeValue(&state->node, 0, 0x10);

    state->assetData1 =
        loadCompressedData(&playerCountSelectSprites_ROM_START, &playerCountSelectSprites_ROM_END, 0xEEE8);
    state->assetData2 = loadCompressedData(&okPromptSprites_ROM_START, &okPromptSprites_ROM_END, 0x1B48);

    scheduleTask(&initPlayerCountSelectSprites, 0, 0, 0x5A);
    scheduleTask(&initCharacterReadyIndicator, 0, 0, 0x5A);

    setGameStateHandler(awaitPlayerCountSelectFadeIn);
}

void awaitPlayerCountSelectFadeIn(void) {
    PlayerCountSelectState *state;
    s32 result;
    u8 *task;
    u16 temp;

    state = (PlayerCountSelectState *)getCurrentAllocation();
    result = getViewportFadeMode(&state->node);

    if (result == 0) {
        state->frameCounter = 0;
        setGameStateHandler(&handlePlayerCountSelectInput);
        scheduleTask(&initPlayerCountHeaderSprite, 0, 0, 0x5A);
        scheduleTask(&initPlayerIndicatorSprite, 0, 0, 0x5A);
    } else {
        temp = state->frameCounter;
        if (temp < 4) {
            if (result == (0xC - (temp * 2))) {
                task = (u8 *)scheduleTask(&setPlayerBehaviorMode, 0, 0, 0x5A);
                task[0x2B] = (u8)state->frameCounter;
                state->frameCounter = state->frameCounter + 1;
            }
        }
    }
}

void handlePlayerCountSelectInput(void) {
    PlayerCountSelectState *state;
    s32 inputs;

    state = (PlayerCountSelectState *)getCurrentAllocation();

    switch (state->menuState) {
        case PLAYER_COUNT_MENU_SELECTING:
            inputs = gControllerInputs[0];
            if (inputs & B_BUTTON) {
                playSoundEffect(0x2E);
                state->menuState = PLAYER_COUNT_MENU_CANCEL_OK;
            } else {
                if (inputs & (STICK_RIGHT | R_JPAD)) {
                    if (state->playerCount.selectedPlayerIndex < (state->connectedControllerCount - 1)) {
                        state->playerCount.selectedPlayerIndex = state->playerCount.selectedPlayerIndex + 1;
                        playSoundEffectOnChannelNoPriority(0x2B, 0);
                        break;
                    }
                }

                if (gControllerInputs[0] & (STICK_LEFT | CONT_LEFT)) {
                    if (state->playerCount.selectedPlayerIndex != 0) {
                        state->playerCount.selectedPlayerIndex = state->playerCount.selectedPlayerIndex - 1;
                        playSoundEffectOnChannelNoPriority(0x2B, 0);
                        break;
                    }
                }

                if (gControllerInputs[0] & A_BUTTON) {
                    state->menuState = PLAYER_COUNT_MENU_CONFIRM_WAIT;
                    state->frameCounter = 0;
                    playSoundEffectOnChannelNoPriority(0x2C, 0);
                }
            }
            break;
        case PLAYER_COUNT_MENU_CONFIRM_WAIT:
            state->frameCounter = state->frameCounter + 1;
            if ((u16)state->frameCounter == 0x11) {
                state->frameCounter = 0;
                state->menuState = PLAYER_COUNT_MENU_CONFIRM_OK;
            }
            break;
        case PLAYER_COUNT_MENU_CONFIRM_OK:
            inputs = gControllerInputs[0];
            if (inputs & B_BUTTON) {
                goto common_exit;
            } else if (inputs & A_BUTTON) {
                state->menuResult = PLAYER_COUNT_RESULT_PROCEED;
                playSoundEffectOnChannelNoPriority(0x2D, 0);
            }
            break;
        case PLAYER_COUNT_MENU_CANCEL_OK:
            inputs = gControllerInputs[0];
            if (inputs & B_BUTTON) {
            common_exit:
                playSoundEffect(0x2E);
                state->menuState = PLAYER_COUNT_MENU_SELECTING;
            } else if (inputs & A_BUTTON) {
                state->menuResult = PLAYER_COUNT_RESULT_CANCEL;
            }
            break;
    }

    if (state->menuResult != PLAYER_COUNT_RESULT_NONE) {
        setMusicFadeOut(0xA);

        if (state->menuResult == PLAYER_COUNT_RESULT_CANCEL) {
            setViewportFadeValue(&state->node, 0xFF, 8);
        } else {
            setViewportFadeValue(&state->node, 0xFF, 0x10);
        }

        setGameStateHandler(&exitPlayerCountSelect);
    }
}

void exitPlayerCountSelect(void) {
    PlayerCountSelectState *state;
    s32 i;

    state = (PlayerCountSelectState *)getCurrentAllocation();
    if (getViewportFadeMode(&state->node) == 0) {
        terminateAllTasks();

        unlinkNode(&state->node);

        state->assetData1 = freeNodeMemory(state->assetData1);
        state->assetData2 = freeNodeMemory(state->assetData2);

        if (state->menuResult == PLAYER_COUNT_RESULT_PROCEED) {
            terminateSchedulerWithCallback(onPlayerCountProceed);
            gGameSessionContext->numPlayers = state->playerCount.bytes.selectedPlayerIndexLo + 1;
            for (i = 0; i < gGameSessionContext->numPlayers; i++) {
                gGameSessionContext->playerBoardIds[i] = gPlayerSlotDefaults[i];
            }
        } else {
            terminateSchedulerWithCallback(onPlayerCountCancel);
        }
    }
}

void onPlayerCountProceed(void) {
    returnToParentScheduler(1);
}

void onPlayerCountCancel(void) {
    returnToParentScheduler(0xFF);
}
