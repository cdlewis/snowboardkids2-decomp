#include "audio/audio.h"
#include "common.h"
#include "common_bss.h"
#include "gamestate.h"
#include "graphics/graphics.h"
#include "math/geometry.h"
#include "os_cont.h"
#include "os_vi.h"
#include "story/map_events.h"
#include "system/rom_loader.h"
#include "system/task_scheduler.h"
#include "ui/character_select_sprites.h"
#include "ui/save_data.h"

USE_OVERLAY(CHARACTER_SELECT_SPRITES_OVERLAY)
USE_OVERLAY(characterSelectBoardTexture)
USE_OVERLAY(okPromptSprites)

typedef struct {
    u8 pad[0x24];
    s16 locationId;
} PreviewTaskNode;

u8 storyMapLocationOrder1[] = { 0x04, 0x00, 0x02, 0x08, 0x07, 0x03, 0x05, 0x01, 0x06, 0x00, 0x00, 0x00 };
u8 storyMapLocationOrder2[] = { 0x01, 0x07, 0x02, 0x05, 0x00, 0x06, 0x08, 0x04, 0x03, 0x00, 0x00, 0x00 };
s16 storyMapLocationPositions[] = { 0x00BC, 0x0050, 0x0084, 0x00DE, 0x00F2, 0x00A7, 0x00C7, 0x006C, 0x0102, 0x0000 };
s16 storyMapLocationSpriteIds[] = { 0x0126, 0x0122, 0x0124, 0x0128, 0x0129, 0x0125, 0x0127, 0x0123, 0x012A, 0x0000 };
u8 storyMapLocationConfig[] = { 0x0A, 0x0A, 0x0F, 0x0A, 0x19, 0x0A, 0x0D, 0x0A, 0x19, 0x00, 0x00, 0x00 };
s16 storyMapLocationOrder3[] = { 0x0000, 0x0002, 0x0004, 0x0008, 0x0007, 0x0003, 0x0005, 0x0001, 0x0006, 0x0000 };

extern u8 gTitleInitialized;

void storyMapHandlePlayerInput(void);
void initStoryMap(void);
void onStoryMapExitToMainMenu(void);
void onStoryMapNormalExit(void);
void storyMapInitFadeIn(void);
void storyMapAwaitFadeIn(void);
void storyMapAwaitFadeOutAndCleanup(void);
void storyMapClearLocationArrivalsIfNoActivePlayers(void);
void storyMapRevertBlockedLocationIndex(s8 *);

void loadOverlay_1BBA0(void) {
    LOAD_OVERLAY(CHARACTER_SELECT_SPRITES_OVERLAY)
    setGameStateHandler(&initStoryMap);
}

void initStoryMap(void) {
    GameState *state;
    ViewportNode *viewports;
    Transform3D sp20;
    s32 i;
    s32 j;
    s16 *locationPtr;
    PreviewTaskNode *task;

    state = allocateTaskMemory(0x5C8);
    viewports = (ViewportNode *)state;
    playMusicTrack(0x19);
    setupTaskSchedulerNodes(0x14, 0x14, 0, 0, 0, 0, 0, 0);
    state->modeData.storyMap.stateTimer = 0;
    state->modeData.storyMap.isStoryMapInitializing = 1;
    initMenuCameraNode(&viewports[0], 0, 0xA, 0);
    initMenuCameraNode(&viewports[1], 8, 0x14, 1);
    initMenuCameraNode(&viewports[2], 9, 5, 1);
    setViewportFadeValue(NULL, 0xFF, 0);
    createViewportTransform(&sp20, 0, 0x80000, 0x60000, 0, 0, 0);
    setViewportTransformById(viewports[0].viewportId, &sp20);
    osViExtendVStart(0);

    for (i = 0; i < gGameSessionContext->numPlayers; i++) {
        state->modeData.storyMap.playerArrived[i] = 0;
        state->modeData.storyMap.selectionState[i] = 0;
        state->modeData.storyMap.playerAtLocation[i] = 0;
        state->modeData.storyMap.locationIds[i] = storyMapLocationOrder2[gGameSessionContext->characterIds[i]];
        state->modeData.storyMap.playerCountAtLocation[i] = 0;
        for (j = 0; j < i + 1; j++) {
            if (state->modeData.storyMap.locationIds[i] == state->modeData.storyMap.locationIds[j]) {
                state->modeData.storyMap.playerCountAtLocation[i]++;
            }
        }
    }

    state->modeData.storyMap.locationBlocked[0] = 0;
    state->modeData.storyMap.locationBlocked[1] = 0;
    state->modeData.storyMap.locationBlocked[2] = 0;
    state->modeData.storyMap.locationBlocked[3] = 0;
    state->modeData.storyMap.locationBlocked[4] = 0;
    state->modeData.storyMap.locationBlocked[5] = 0;
    state->modeData.storyMap.locationBlocked[6] = 0;
    state->modeData.storyMap.locationBlocked[7] = 0;
    state->modeData.storyMap.locationBlocked[8] = 0;

    if (gGameSessionContext->gameMode == 0) {
        state->modeData.storyMap.locationBlocked[8] = -1;
        state->modeData.storyMap.locationBlocked[4] = -1;
        state->modeData.storyMap.locationBlocked[3] = -1;
    } else {
        if (EepromSaveData->specialBoardUnlocked[0] == 0) {
            state->modeData.storyMap.locationBlocked[8] = -1;
        }
        if (EepromSaveData->specialBoardUnlocked[1] == 0) {
            state->modeData.storyMap.locationBlocked[4] = -1;
        }
        if (EepromSaveData->specialBoardUnlocked[2] == 0) {
            state->modeData.storyMap.locationBlocked[3] = -1;
        }
    }

    i = 0;
    locationPtr = storyMapLocationOrder3;
    do {
        task = scheduleTask(initCharacterPreview, 1, 0, 0x5A);
        task->locationId = *locationPtr;
        locationPtr++;
        state->modeData.storyMap.locationHasPlayers[i] = 0;
        i++;
    } while (i < 9);

    scheduleTask(initCharacterSelectBoardTask, 1, 0, 0x5A);
    state->modeData.storyMap.portraitAsset =
        loadCompressedData(&okPromptSprites_ROM_START, &okPromptSprites_ROM_END, 0x1B48);
    state->modeData.storyMap.imageAsset =
        loadCompressedData(&characterSelectBoardTexture_ROM_START, &characterSelectBoardTexture_ROM_END, 0x13FF0);
    scheduleTask(initCharacterSelectSprites, 1, 0, 0x5A);
    scheduleTask(initCharacterSelectTextureDataLoad, 1, 0, 0x5A);

    for (i = 0; i < gGameSessionContext->numPlayers; i++) {
        state->modeData.storyMap.characterIds[i] = gGameSessionContext->characterIds[i];
    }

    setGameStateHandler(storyMapInitFadeIn);
}

void storyMapInitFadeIn(void) {
    GameState *state = (GameState *)getCurrentAllocation();
    if (state->modeData.storyMap.isStoryMapInitializing != 0) {
        return;
    }

    if (getPendingDmaCount() != 0) {
        return;
    }

    setViewportFadeValue(NULL, 0, 0x10);

    setGameStateHandler(storyMapAwaitFadeIn);
}

void storyMapAwaitFadeIn(void) {
    if (!getViewportFadeMode(0)) {
        setGameStateHandler(&storyMapHandlePlayerInput);
    }
}

void storyMapHandlePlayerInput(void) {
    GameState *state;
    s32 i;
    s32 j;
    s32 diagonal;
    s8 confirmed;
    u8 allConfirmed;
    s8 hDir;
    s8 vDir;
    u8 currentLoc;
    u8 fadeSpeed;
    s32 count;

    state = getCurrentAllocation();
    confirmed = 0;
    diagonal = 0;
    allConfirmed = 0;

    for (i = 0; i < gGameSessionContext->numPlayers; i++) {
        switch (state->modeData.storyMap.selectionState[i]) {
            case 0:
                vDir = 0;
                hDir = 0;
                currentLoc = state->modeData.storyMap.locationIds[i];
                if (gControllerInputs[i] & (STICK_RIGHT | CONT_RIGHT)) {
                    hDir = 1;
                } else if (gControllerInputs[i] & (STICK_LEFT | CONT_LEFT)) {
                    hDir = -1;
                }

                if (gControllerInputs[i] & (STICK_UP | CONT_UP)) {
                    vDir = -5;
                    if ((s8)currentLoc != 8) {
                        vDir = -4;
                    }
                } else if (gControllerInputs[i] & (STICK_DOWN | CONT_DOWN)) {
                    vDir = 5;
                }

                if (hDir != 0 && vDir != 0) {
                    if (hDir < 0 && vDir < 0) {
                        vDir = -5;
                    } else if (hDir < 0 && vDir > 0) {
                        vDir = 4;
                    } else if (hDir > 0 && vDir < 0) {
                        vDir = -4;
                    } else {
                        vDir = 5;
                    }
                    hDir = 0;
                    diagonal = 1;
                }

                if ((currentLoc += hDir + vDir) < 9) {
                    if (((s8)currentLoc != 4 || state->modeData.storyMap.locationIds[i] != 3) &&
                        ((s8)currentLoc != 3 || state->modeData.storyMap.locationIds[i] != 4) &&
                        ((s8)currentLoc != 4 || state->modeData.storyMap.locationIds[i] != 8)) {
                        if (!((s8)currentLoc == 8 && state->modeData.storyMap.locationIds[i] == 4)) {
                            goto noReset;
                        }
                    }
                }

                currentLoc = (u8)state->modeData.storyMap.locationIds[i];
            noReset:
                if (state->modeData.storyMap.locationBlocked[(s8)currentLoc] == -1) {
                    if (hDir == 0 && diagonal ^ 1) {
                        storyMapRevertBlockedLocationIndex((s8 *)&currentLoc);
                    } else {
                        currentLoc = state->modeData.storyMap.locationIds[i];
                    }
                }

                if (state->modeData.storyMap.locationIds[i] != (s8)currentLoc) {
                    state->modeData.storyMap.playerCountAtLocation[i] = 0;
                    playSoundEffectOnChannelNoPriority(0x2B, i);
                }
                state->modeData.storyMap.locationIds[i] = (s8)currentLoc;

                if (state->modeData.storyMap.playerCountAtLocation[i] == 0) {
                    for (j = 0; j < gGameSessionContext->numPlayers; j++) {
                        if ((s8)currentLoc == state->modeData.storyMap.locationIds[j]) {
                            state->modeData.storyMap.playerCountAtLocation[i]++;
                        }
                    }
                }

                if (gControllerInputs[i] & CONT_A) {
                    gGameSessionContext->characterIds[i] =
                        storyMapLocationOrder1[state->modeData.storyMap.locationIds[i]];
                    state->modeData.storyMap.selectionState[i] = 10;
                    state->modeData.storyMap.playerAtLocation[i] = 0;
                    state->modeData.storyMap.animationTimers[i] = 0;
                    playSoundEffectOnChannelNoPriority((u16)storyMapLocationSpriteIds[(s8)currentLoc], i + 8);
                } else if (gControllerInputs[i] & CONT_B) {
                    s32 k;
                    playSoundEffect(0x2E);
                    for (k = 0; k < gGameSessionContext->numPlayers; k++) {
                        gGameSessionContext->characterIds[k] = state->modeData.storyMap.characterIds[k];
                        if (gTitleInitialized != 0) {
                            state->modeData.storyMap.selectionState[i] = 4;
                        } else {
                            state->modeData.storyMap.selectionState[k] = 2;
                        }
                    }
                    scheduleTask(&initPlayer2CharacterSelectIndicator, 0, 0, 0x5B);
                    i = gGameSessionContext->numPlayers;
                }
                break;

            case 1:
                if (gControllerInputs[i] & CONT_B) {
                    playSoundEffectOnChannelNoPriority(0x2E, i);
                    state->modeData.storyMap.selectionState[i] = 0;
                }
                if (state->modeData.storyMap.animationTimers[i] <
                    storyMapLocationConfig[state->modeData.storyMap.locationIds[i]]) {
                    state->modeData.storyMap.animationTimers[i]++;
                    if (state->modeData.storyMap.animationTimers[i] ==
                            storyMapLocationConfig[state->modeData.storyMap.locationIds[i]] &&
                        state->modeData.storyMap.locationHasPlayers[state->modeData.storyMap.locationIds[i]] == 0) {
                        state->modeData.storyMap.locationHasPlayers[state->modeData.storyMap.locationIds[i]] = 1;
                        playSoundEffectOnChannelNoPriority(
                            (u16)storyMapLocationPositions[state->modeData.storyMap.locationIds[i]],
                            i + 4
                        );
                    }
                }
                if (i == gGameSessionContext->numPlayers - 1) {
                    for (j = 0, count = 0; j < gGameSessionContext->numPlayers; j++) {
                        count += (state->modeData.storyMap.selectionState[j] == 1);
                    }
                    if (count == gGameSessionContext->numPlayers) {
                        for (j = 0; j < gGameSessionContext->numPlayers; j++) {
                            state->modeData.storyMap.selectionState[j] = 3;
                            scheduleTask(&initPlayer3CharacterSelectIndicator, 0, 0, 0x5B);
                        }
                    }
                }
                break;

            case 2:
                if (gControllerInputs[i] & CONT_A) {
                    i = gGameSessionContext->numPlayers;
                    confirmed = 1;
                } else if (gControllerInputs[i] & CONT_B) {
#ifdef CC_CHECK
                    s32 k;
#else
                    register s32 k __asm__("$4");
#endif
                    playSoundEffect(0x2E);
                    for (k = 0; k < gGameSessionContext->numPlayers; k++) {
                        state->modeData.storyMap.selectionState[k] = 0;
                    }
                    i = gGameSessionContext->numPlayers;
                }
                break;

            case 3:
                if (state->modeData.storyMap.animationTimers[i] <
                    storyMapLocationConfig[state->modeData.storyMap.locationIds[i]]) {
                    state->modeData.storyMap.animationTimers[i]++;
                    if (state->modeData.storyMap.animationTimers[i] ==
                        storyMapLocationConfig[state->modeData.storyMap.locationIds[i]]) {
                        if (state->modeData.storyMap.locationHasPlayers[state->modeData.storyMap.locationIds[i]] == 0) {
                            state->modeData.storyMap.locationHasPlayers[state->modeData.storyMap.locationIds[i]] = 1;
                            count = 4;
                            playSoundEffectOnChannelNoPriority(
                                (u16)storyMapLocationPositions[state->modeData.storyMap.locationIds[i]],
                                i + count
                            );
                        }
                    }
                }
                if (gControllerInputs[i] & CONT_B) {
#ifdef CC_CHECK
                    s32 k;
#else
                    register s32 k __asm__("$4");
#endif
                    playSoundEffectOnChannelNoPriority(0x2E, i);
                    for (k = 0; k < gGameSessionContext->numPlayers; k++) {
                        if (i == k) {
                            state->modeData.storyMap.selectionState[i] = 0;
                        } else {
                            state->modeData.storyMap.selectionState[k] = 1;
                        }
                    }
                    i = gGameSessionContext->numPlayers;
                } else if (gControllerInputs[i] & CONT_A) {
                    allConfirmed = 1;
                    i = gGameSessionContext->numPlayers;
                    playSoundEffect(0x2D);
                }
                break;

            case 4:
                i = gGameSessionContext->numPlayers;
                confirmed = 1;
                break;

            case 10: {
                state->modeData.storyMap.playerAtLocation[i]++;
                if ((state->modeData.storyMap.playerAtLocation[i] & 0xFF) == 0x11) {
                    state->modeData.storyMap.playerAtLocation[i] = 0;
                    state->modeData.storyMap.selectionState[i] = 1;
                }
                break;
            }
        }
    }

    if (allConfirmed || confirmed) {
        state->modeData.storyMap.stateTimer = confirmed;
        if (confirmed != 0) {
            fadeSpeed = 8;
        } else {
            fadeSpeed = 0xE;
            for (i = 0; i < gGameSessionContext->numPlayers; i++) {
                if (state->modeData.storyMap.characterIds[i] != gGameSessionContext->characterIds[i]) {
                    gGameSessionContext->boardModelIds[i] = 0;
                }
            }
        }
        setViewportFadeValue(NULL, 0xFF, fadeSpeed);
        setMusicFadeOut(0xA);
        setGameStateHandler(storyMapAwaitFadeOutAndCleanup);
    }

    storyMapClearLocationArrivalsIfNoActivePlayers();
}

void storyMapAwaitFadeOutAndCleanup(void) {
    void *exitCallback;
    ViewportNode *state = (ViewportNode *)getCurrentAllocation();

    if (getViewportFadeMode(0) == 0) {
        unlinkNode(&state[0]);
        unlinkNode(&state[1]);
        unlinkNode(&state[2]);

        osViExtendVStart(0);

        state[3].unk0.next = freeNodeMemory(state[3].unk0.next);
        state[3].prev = freeNodeMemory(state[3].prev);

        if (state[3].unk8.counter != 0) {
            exitCallback = &onStoryMapExitToMainMenu;
        } else {
            exitCallback = &onStoryMapNormalExit;
        }

        terminateSchedulerWithCallback(exitCallback);
    }
}

void onStoryMapNormalExit(void) {
    s32 exitCode;

    exitCode = 1;
    if ((gGameSessionContext->gameMode == 0) && (EepromSaveData->levelUnlockStatus[0] == 5)) {
        exitCode = 0x44;
    }
    returnToParentScheduler(exitCode);
}

void onStoryMapExitToMainMenu(void) {
    returnToParentScheduler(0xFF);
}

void storyMapClearLocationArrivalsIfNoActivePlayers(void) {
    GameState *state = getCurrentAllocation();
    s32 locationIdx;
    s32 playerIdx;
    u8 activePlayerCount;

    for (locationIdx = 0; locationIdx < 9; locationIdx++) {
        activePlayerCount = 0;

        for (playerIdx = 0; playerIdx < gGameSessionContext->numPlayers; playerIdx++) {
            if (state->modeData.storyMap.selectionState[playerIdx] == 1 ||
                state->modeData.storyMap.selectionState[playerIdx] == 3) {
                activePlayerCount++;
            }
        }

        if (activePlayerCount == 0) {
            state->modeData.storyMap.locationHasPlayers[locationIdx] = 0;
        }
    }
}

void storyMapRevertBlockedLocationIndex(s8 *locationIndex) {
    if (*locationIndex == 3) {
        *locationIndex = 2;
    } else if (*locationIndex == 8) {
        *locationIndex = 7;
    } else if (*locationIndex != 4) {
    } else {
        *locationIndex = 0;
    }
}
