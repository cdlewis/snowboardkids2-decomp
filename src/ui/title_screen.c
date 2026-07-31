#include "assets.h"
#include "audio/audio.h"
#include "common.h"
#include "common_bss.h"
#include "effects/cutscene_keyframes.h"
#include "gamestate.h"
#include "graphics/graphics.h"
#include "graphics/sprite_rdp.h"
#include "os_cont.h"
#include "race/position_markers.h"
#include "race/race_session.h"
#include "story/map_events.h"
#include "story/map_extra_intro.h"
#include "story/race_state_machine.h"
#include "system/controller_io.h"
#include "system/rom_loader.h"
#include "system/task_scheduler.h"
#include "ui/gallery.h"
#include "ui/save_data.h"
#include "ui/title_ui_elements.h"
#include "ui/unlock_screen.h"

extern s32 gButtonsPressed;
extern u8 gDebugUnlockEnabled;
extern s8 gTitleInitialized;

void resetSaveDataToDefaults(void);
void waitForTitleAssetsReady(void);
void checkPartialUnlockCheatCode(void);
void checkUnlockAllCheatCode(void);
void unlockAllContent(void);
void unlockPartialContent(void);
void handleTitleMenuInput(void);

u8 gTitleExitMode BSS = 0;
static u8 sTitleScreenBssPadding0[0xC] BSS = { 0 };
s16 storyMapExitCode BSS = 0;
static u8 sTitleScreenBssPadding1[0xC] BSS = { 0 };

s32 gTitleCameraSettings[64] = {
    0xF8BE0000, 0x1F2B07B8, 0x1F0001CC, 0xE1CF07ED, 0xF8F90000, 0xFF6D2B6E, 0x003D9000, 0xFFDDCF14,
    0x00C00000, 0x1B300000, 0x1B330000, 0xE4D00000, 0x00C00000, 0x0018A67A, 0x00000000, 0xFFF42D86,
    0x0AA50000, 0x19070000, 0x1B330000, 0xE6F90000, 0x0AA50000, 0x000BB2DA, 0x00000000, 0xFFE07296,
    0xECE70000, 0x135D0000, 0x1B330000, 0xECA30000, 0xECE70000, 0xFFFE414A, 0x00000000, 0x00283266,
    0xE9590000, 0x0F0D0000, 0x1B330000, 0xF0F30000, 0xE9590000, 0xFFE526BA, 0x00000000, 0x002AB996,
    0x14630000, 0x12000000, 0x1B330000, 0xEE000000, 0x14630000, 0xFFFF477A, 0x00000000, 0xFFCF1276,
    0xF1310000, 0x16D00000, 0x1B330000, 0xE9300000, 0xF1310000, 0x00107E5A, 0x00000000, 0x001B4646,
    0xF9C00000, 0x1A780000, 0x1B330000, 0xE5880000, 0xF9C00000, 0x0018FB7A, 0x00000000, 0x0008D966,
};

s32 titleAnimSequence0[2] = {
    0x00040004,
    0xFFFF0000,
};

s32 titleAnimSequence1[3] = {
    0x00050006,
    0x00060007,
    0xFFFF0000,
};

s32 *gTitleCharacterAnimSequences[2] = {
    titleAnimSequence0,
    titleAnimSequence1,
};

s32 titleEffectSequence0[3] = {
    0xFFFE004C,
    0x004F002B,
    0x0053FFFF,
};

s32 titleEffectSequence1[4] = {
    0x00530069,
    0x002B0055,
    0x003F006E,
    0xFFFF0000,
};

s32 titleEffectSequence2[4] = {
    0xFFFB0044,
    0x0082004B,
    0x003D003F,
    0xFFFF0000,
};

s32 titleEffectSequence3[3] = {
    0xFFFE0043,
    0x00790036,
    0x004FFFFF,
};

s32 titleEffectSequence4[3] = {
    0x004C0053,
    0x002B0067,
    0x002BFFFF,
};

s32 titleEffectSequenceTable[17] = {
    0xFFFE007B,
    0x00530068,
    0xFFFF0000,
    (s32)titleEffectSequence0,
    (s32)titleEffectSequence1,
    (s32)titleEffectSequence2,
    (s32)titleEffectSequence3,
    (s32)titleEffectSequence4,
    (s32)titleEffectSequenceTable,
    0xF8BE0000,
    0x1F2B07B8,
    0x1F0001CC,
    0xE1CF07ED,
    0xF8F90000,
    0xFF6D2B6E,
    0x003D9000,
    0xFFDDCF14,
};

s32 gTitleCharacterTransforms[59] = {
    0x00C00000, 0x1B300000, 0x1B330000, 0xE4D00000, 0x00C00000, 0x0018A67A, 0x00000000, 0xFFF42D86, 0x0AA50000,
    0x19070000, 0x1B330000, 0xE6F90000, 0x0AA50000, 0x000BB2DA, 0x00000000, 0xFFE07296, 0xECE70000, 0x135D0000,
    0x1B330000, 0xECA30000, 0xECE70000, 0xFFFE414A, 0x00000000, 0x00283266, 0xE9590000, 0x0F0D0000, 0x1B330000,
    0xF0F30000, 0xE9590000, 0xFFE526BA, 0x00000000, 0x002AB996, 0x14630000, 0x12000000, 0x1B330000, 0xEE000000,
    0x14630000, 0xFFFF477A, 0x00000000, 0xFFCF1276, 0xF1310000, 0x16D00000, 0x1B330000, 0xE9300000, 0xF1310000,
    0x00107E5A, 0x00000000, 0x001B4646, 0xF9C00000, 0x1A780000, 0x1B330000, 0xE5880000, 0xF9C00000, 0x0018FB7A,
    0x00000000, 0x0008D966, 0x00000000, 0x00000000, 0x00000000,
};

s16 storyMapLocationModelIds[10] = {
    0x000F, 0x000F, 0x0013, 0x0013, 0x0013, 0x000F, 0x000F, 0x000F, 0x0013, 0x0000,
};

// storyMapAngleBounds: 20 shorts (40 bytes)
// storyMapLocationCoords follows immediately: 10 Vec2sXZ values (40 bytes)
// Total: 80 bytes = 0x50 as shown in symbol_addrs.txt
s16 storyMapAngleBounds[20] = {
    0xF93E, 0xF83E, 0xFC84, 0xFB84, 0xFF26, 0xFE26, 0x01CC, 0x00CC, 0x047E, 0x037E,
    0x072A, 0x062A, 0x0020, 0xFF80, 0xFDB8, 0xFCF8, 0x0374, 0x0294, 0x05C2, 0x04E2,
};

// storyMapLocationCoords must immediately follow storyMapAngleBounds
Vec2sXZ storyMapLocationCoords[10] = {
    { 0x009D, 0xFFEF },
    { 0x008E, 0xFF82 },
    { 0x002B, 0xFF66 },
    { 0xFFD6, 0xFF66 },
    { 0xFF90, 0xFF8D },
    { 0xFF65, 0xFFD5 },
    { 0x0006, 0xFF78 },
    { 0x0044, 0xFF8A },
    { 0xFFD3, 0xFFBE },
    { 0xFF8B, 0xFFBC },
};

// Location names: 6 groups of 20 bytes each (total 120 bytes)
s8 storyMapLocationNames[6][20] = {
    "PAINT", "U.F.O.", "TIMES", "SCHOOL", "BOARDSHOP", "TEACHER",
};

void (*storyMapLocationHandlers[])(void) = {
    NULL,
    initBoardShopDisplay,
    initGalleryMenu,
    initStoryMapLocationIntro,
    loadOverlay_1BBA0,
    initUnlockScreen,
    initStoryMapLocationIntro,
    initSaveSlotScreen,
    initStoryMapExtraIntro,
    initStoryMapLocationIntro,
    initStoryMapLocationIntro,
    NULL,
    NULL,
};

void exitTitleToNextMode(void) {
    returnToParentScheduler(gTitleExitMode);
}

void cleanupTitleAndTransition(void) {
    TitleScreenState *state;
    u8 menuSelection;
    u8 menuMode;

    state = getCurrentAllocation();

    if (getViewportFadeMode(0) != 0) {
        return;
    }

    setViewportFadeValue(0, 0, 0);
    gTitleExitMode = 1;

    menuSelection = state->menuSelection;

    if (menuSelection == 3) {
        gGameSessionContext->gameMode = 0xFF;
    } else {
        menuMode = state->menuMode;
        if (menuMode == 1) {
            gGameSessionContext->gameMode = menuSelection;
            gGameSessionContext->currentLevel = 0;
        } else if (menuSelection == 1) {
            gTitleExitMode = 2;
        } else {
            gTitleExitMode = 3;
            gGameSessionContext->gameMode = 0xFE;
        }
    }

    if (state->idleFrameCounter == 0x384) {
        gGameSessionContext->gameMode = 0;
        gGameSessionContext->currentLevel = 0;
        gTitleExitMode = 4;
    }

    unlinkNode(&state->mainViewport);
    unlinkNode(&state->menuViewport);

    state->titleLogoData = freeNodeMemory(state->titleLogoData);
    state->menuGraphicsData = freeNodeMemory(state->menuGraphicsData);

    terminateSchedulerWithCallback(exitTitleToNextMode);
}

void writeSaveDataToEeprom(void) {
    u8 buffer[0x200];
    s32 i;
    s32 result;

    for (i = 0x1FF; i >= 0; i--) {
        buffer[i] = 0;
    }

    eepromWriteAllAsync(buffer);

    do {
        result = (s32)pollEepromWriteAllAsync();
    } while (result == -1);

    if (result == 0) {
        playSoundEffect(0x2C);
    } else {
        playSoundEffect(0x2B);
    }

    setGameStateHandler(handleTitleMenuInput);
}

void handleTitleMenuInput(void) {
    TitleScreenState *state;
    s32 input;
    u8 soundDelay;
    u8 menuMode;
    u8 menuSelection;
    u16 frameCounter;
    s32 temp;

    state = getCurrentAllocation();

    soundDelay = state->initialMusicDelay;
    if (soundDelay != 0) {
        soundDelay--;
        state->initialMusicDelay = soundDelay;
        if (soundDelay == 0) {
            playMusicTrack(0x1C);
        }
    }

    input = gControllerInputs[0];

    if ((input & L_TRIG) != 0) {
        if (((&gControllerInputs[0])[3] & R_TRIG) != 0) {
            goto end;
        }
    }

    menuMode = state->menuMode;

    if (menuMode == 0) {
        goto case_0;
    } else if (menuMode == 1) {
        goto case_1;
    } else {
        goto end;
    }

case_0:
    menuSelection = state->menuSelection;

    if ((input & (STICK_UP | U_JPAD)) != 0) {
        if (menuSelection != 0) {
            state->menuSelection = menuSelection - 1;
        } else {
            state->menuSelection = state->menuOptionCount - 1;
        }
    } else if ((input & (STICK_DOWN | D_JPAD)) != 0) {
        if (menuSelection < state->menuOptionCount - 1) {
            state->menuSelection = menuSelection + 1;
        } else {
            state->menuSelection = 0;
        }
    }

    frameCounter = state->idleFrameCounter + 1;
    state->idleFrameCounter = frameCounter;

    if (menuSelection != state->menuSelection) {
        playSoundEffectOnChannelNoPriority(0x2B, 1);
        frameCounter = state->idleFrameCounter;
        if (frameCounter >= 0x2D1) {
            state->idleFrameCounter = 0x2D0;
        }
    }

    if ((gControllerInputs[0] & (A_BUTTON | START_BUTTON)) != 0) {
        playSoundEffectOnChannelNoPriority(0x2C, 1);
        if (state->menuSelection == 0) {
            state->menuMode = 1;
            goto end;
        }
        setMusicFadeOut(0x20);
        setViewportFadeValue(0, 0xFF, 0x10);
        setGameStateHandlerWithContinue(cleanupTitleAndTransition);
        goto end;
    }

    if (state->idleFrameCounter == 0x384) {
        setMusicFadeOut(0x20);
        setViewportFadeValue(0, 0xFF, 0x10);
        setGameStateHandlerWithContinue(cleanupTitleAndTransition);
    }
    goto end;

case_1:
    menuSelection = state->menuSelection;

    if ((input & (STICK_UP | U_JPAD)) == 0) {
        goto case1_check2;
    }
    if (menuSelection != 0) {
        state->menuSelection = menuSelection - 1;
        goto case1_done_nav;
    }
    state->menuSelection = 1;
    goto case1_done_nav;

case1_check2:
    if ((input & (STICK_DOWN | D_JPAD)) == 0) {
        goto case1_done_nav;
    }
    if (menuSelection == 0) {
        state->menuSelection = menuSelection + 1;
    } else {
        state->menuSelection = 0;
    }

case1_done_nav:
    if (menuSelection != state->menuSelection) {
        playSoundEffectOnChannelNoPriority(0x2B, 1);
    }

    temp = gControllerInputs[0];

    if ((temp & B_BUTTON) != 0) {
        playSoundEffect(0x2E);
        state->menuSelection = 0;
        state->menuMode = 0;
        goto end;
    }

    if ((temp & (A_BUTTON | START_BUTTON)) != 0) {
        if (gDebugUnlockEnabled != 0) {
            if ((gButtonsPressed & R_JPAD) != 0) {
                unlockAllContent();
            } else if ((gButtonsPressed & L_JPAD) != 0) {
                unlockPartialContent();
            }
        }
        playSoundEffectOnChannelNoPriority(0x2C, 1);
        setMusicFadeOut(0x20);
        setViewportFadeValue(0, 0xFF, 8);
        setGameStateHandlerWithContinue(cleanupTitleAndTransition);
    }

end:
    checkPartialUnlockCheatCode();
    checkUnlockAllCheatCode();
}

void onTitleFadeInComplete(void) {
    if (getViewportFadeMode(0) == 0) {
        setViewportEnvColor(0, 0, 0, 0);
        playSoundEffectOnChannelNoPriority(0x135, 0);
        setGameStateHandler(handleTitleMenuInput);
    }
}

void waitForTitleAssetsReady(void) {
    TitleScreenState *state = (TitleScreenState *)getCurrentAllocation();

    state->idleFrameCounter++;

    if (state->idleFrameCounter >= 3) {
        state->idleFrameCounter = 2;
        if (getPendingDmaCount() == 0) {
            state->idleFrameCounter = 0;
            setViewportFadeValue(NULL, 0, 0x10);
            setGameStateHandler(onTitleFadeInComplete);
        }
    }
}

void initTitleScreen(void) {
    TitleScreenState *state;
    ViewportNode *menuViewport;
    void *dmaResult;
    s32 i;
    void *checkResult;

    state = (TitleScreenState *)allocateTaskMemory(0x3C8);
    setViewportFadeValue(NULL, 0xFF, 0);
    menuViewport = &state->menuViewport;
    initDefaultFontPalette();
    setupTaskSchedulerNodes(0x14, 0, 0, 0, 0, 0, 0, 0);
    initViewportNode(&state->mainViewport, NULL, 8, 10, 0);
    setModelCameraTransform(&state->mainViewport, 0, 0, -0xA0, -0x78, 0x9F, 0x77);
    initMenuCameraNode(menuViewport, 0, 8, 0);
    setViewportPerspective(menuViewport, 40.0f, 1.3333334f, 10.0f, 10000.0f);
    setViewportTransformById(menuViewport->viewportId, &gTitleCameraSettings);
    state->titleLogoData = loadCompressedData(&titleLogo_ROM_START, &titleLogo_ROM_END, 0x7B50);
    dmaResult = loadCompressedData(&titleScreenSprites_ROM_START, &titleScreenSprites_ROM_END, 0x2238);
    state->menuSelection = 0;
    state->menuOptionCount = 0;
    state->menuMode = 0;
    state->reserved3BE = 0;
    state->initialMusicDelay = 0x3C;
    state->partialUnlockCheatProgress = 0;
    state->unlockAllCheatProgress = 0;
    state->menuGraphicsData = dmaResult;
    state->idleFrameCounter = 0;

    if (gGameSessionContext->gameMode == 0xFE) {
        state->menuSelection = 2;
    } else if (gGameSessionContext->currentLevel == 0xF) {
        state->menuSelection = 1;
    }

    resetSaveDataToDefaults();

    for (i = 0; i < 4; i++) {
        gGameSessionContext->battleScores[i] = 0;
    }

    gTitleInitialized = 0;

    checkResult = __udiv_w_sdiv();
    if (checkResult != NULL) {
        state->menuOptionCount = 4;
    } else {
        state->menuOptionCount = 3;
    }

    scheduleTask(&loadTitleLogoAsset, 0, 0, 0x64);
    scheduleTask(&initControllerSlotDisplay, 0, 0, 0x62);
    scheduleTask(&initPressStartPrompt, 0, 0, 0x62);

    for (i = 0; i < 7; i++) {
        dmaResult = scheduleTask(&initTitleCharacterModel, 0, 0, 0x62);
        if (dmaResult != NULL) {
            ((TitleCharacterState *)dmaResult)->characterIndex = i;
        }
    }

    scheduleTask(&initTitleEffectModel, 0, 0, 0x62);
    spawnPushStartPrompt(1, 0x20, 0, 0, 8, 7);
    setGameStateHandler(waitForTitleAssetsReady);
}

void checkPartialUnlockCheatCode(void) {
    TitleScreenState *state = getCurrentAllocation();
    s32 buttons;

    switch (state->partialUnlockCheatProgress) {
        case 0:
            if (gButtonsPressed == R_TRIG) {
                state->partialUnlockCheatProgress++;
            }
            break;

        case 1:
            buttons = gButtonsPressed;
            if (buttons != R_TRIG && buttons != 0) {
                if (buttons == D_JPAD) {
                    state->partialUnlockCheatProgress++;
                } else {
                    state->partialUnlockCheatProgress = 0xFF;
                }
            }
            break;

        case 2:
            buttons = gButtonsPressed;
            if (buttons != D_JPAD && buttons != 0) {
                if (buttons == L_TRIG) {
                    state->partialUnlockCheatProgress++;
                } else {
                    state->partialUnlockCheatProgress = 0xFF;
                }
            }
            break;

        case 3:
            buttons = gButtonsPressed;
            if (buttons != L_TRIG && buttons != 0) {
                if (buttons == STICK_UP) {
                    state->partialUnlockCheatProgress++;
                } else {
                    state->partialUnlockCheatProgress = 0xFF;
                }
            }
            break;

        case 4:
            buttons = gButtonsPressed;
            if (buttons != STICK_UP && buttons != 0) {
                if (buttons == START_BUTTON) {
                    unlockPartialContent();
                    playSoundEffect(0xDC);
                    playSoundEffect(0xDC);
                    state->partialUnlockCheatProgress = 0xF0;
                } else {
                    state->partialUnlockCheatProgress = 0xFF;
                }
            }
            break;
    }
}

void checkUnlockAllCheatCode(void) {
    TitleScreenState *state;
    u8 cheatState;
    s32 buttons;
    s32 temp_v1;

    state = getCurrentAllocation();
    cheatState = state->unlockAllCheatProgress;

    if (cheatState < 0xA) {
        switch (cheatState) {
            case 0:
                temp_v1 = gButtonsPressed;
                if (temp_v1 == Z_TRIG) {
                    state->unlockAllCheatProgress++;
                }
                break;

            case 1:
                buttons = gButtonsPressed;
                if ((buttons != Z_TRIG) && (buttons != 0)) {
                    if (buttons != B_BUTTON) {
                        state->unlockAllCheatProgress = 0xFF;
                    } else {
                        state->unlockAllCheatProgress++;
                    }
                }
                break;

            case 2:
                buttons = gButtonsPressed;
                if ((buttons != B_BUTTON) && (buttons != 0)) {
                    if (buttons != U_CBUTTONS) {
                        state->unlockAllCheatProgress = 0xFF;
                    } else {
                        state->unlockAllCheatProgress++;
                    }
                }
                break;

            case 3:
                buttons = gButtonsPressed;
                if ((buttons != U_CBUTTONS) && (buttons != 0)) {
                    if (buttons != D_JPAD) {
                        state->unlockAllCheatProgress = 0xFF;
                    } else {
                        state->unlockAllCheatProgress++;
                    }
                }
                break;

            case 4:
                buttons = gButtonsPressed;
                if ((buttons != D_JPAD) && (buttons != 0)) {
                    if (buttons != STICK_LEFT) {
                        state->unlockAllCheatProgress = 0xFF;
                    } else {
                        state->unlockAllCheatProgress++;
                    }
                }
                break;

            case 5:
                buttons = gButtonsPressed;
                if ((buttons != STICK_LEFT) && (buttons != 0)) {
                    if (buttons != STICK_RIGHT) {
                        state->unlockAllCheatProgress = 0xFF;
                    } else {
                        state->unlockAllCheatProgress++;
                    }
                }
                break;

            case 6:
                buttons = gButtonsPressed;
                if ((buttons != STICK_RIGHT) && (buttons != 0)) {
                    if (buttons != U_JPAD) {
                        state->unlockAllCheatProgress = 0xFF;
                    } else {
                        state->unlockAllCheatProgress++;
                    }
                }
                break;

            case 7:
                buttons = gButtonsPressed;
                if ((buttons != U_JPAD) && (buttons != 0)) {
                    if (buttons != R_TRIG) {
                        state->unlockAllCheatProgress = 0xFF;
                    } else {
                        state->unlockAllCheatProgress++;
                    }
                }
                break;

            case 8:
                buttons = gButtonsPressed;
                if ((buttons != R_TRIG) && (buttons != 0)) {
                    if (buttons != Z_TRIG) {
                        state->unlockAllCheatProgress = 0xFF;
                    } else {
                        state->unlockAllCheatProgress++;
                    }
                }
                break;

            case 9:
                buttons = gButtonsPressed;
                if ((buttons != Z_TRIG) && (buttons != 0)) {
                    if (buttons == A_BUTTON) {
                        unlockAllContent();
                        playSoundEffect(0x110);
                        playSoundEffect(0x110);
                        playSoundEffect(0x110);
                        playSoundEffect(0x110);
                        state->unlockAllCheatProgress = 0xF0;
                    } else {
                        state->unlockAllCheatProgress = 0xFF;
                    }
                }
                break;
        }
    }

    cheatState = state->unlockAllCheatProgress;
    if (cheatState == 0xFF) {
        state->unlockAllCheatProgress = 0xFF;
    }
}

void unlockAllContent(void) {
    s32 i;

    // First loop: initialize levelUnlockStatus and versusLevelAvailability
    for (i = 0; i < 16; i++) {
        EepromSaveData->levelUnlockStatus[i] = 1;
        EepromSaveData->versusLevelAvailability[i] = 1;
    }

    // Second loop: initialize characterPaletteIds and related arrays
    for (i = 0; i < 9; i++) {
        EepromSaveData->characterPaletteIds[i] = i + 1;
        EepromSaveData->characterPaletteIds[i + 9] = i + 0x11;
        EepromSaveData->unlockedCutsceneIds[i] = i + 9;
    }

    // Set individual bytes
    EepromSaveData->unlockedBoardIds[0] = 0xD;
    EepromSaveData->unlockedBoardIds[1] = 0xE;
    EepromSaveData->unlockedBoardIds[2] = 0xF;
    EepromSaveData->specialBoardUnlocked[0] = 1;
    EepromSaveData->specialBoardUnlocked[1] = 1;
    EepromSaveData->specialBoardUnlocked[2] = 1;
}

void unlockPartialContent(void) {
    s32 i;

    for (i = 0; i < 16; i++) {
        EepromSaveData->levelUnlockStatus[i] = 1;
        EepromSaveData->versusLevelAvailability[i] = 1;
    }

    EepromSaveData->specialBoardUnlocked[0] = 1;
    EepromSaveData->specialBoardUnlocked[1] = 1;
    EepromSaveData->specialBoardUnlocked[2] = 0;

    for (i = 0; i < 9; i++) {
        EepromSaveData->characterPaletteIds[i] = i + 1;
    }

    EepromSaveData->unlockedBoardIds[0] = 0xD;
    EepromSaveData->unlockedBoardIds[1] = 0xF;
}

void resetSaveDataToDefaults(void) {
    s32 i;

    // Zero magic (8 bytes)
    for (i = 0; i < 8; i++) {
        EepromSaveData->magic[i] = 0;
    }

    // Zero checksum and gold
    EepromSaveData->checksum = 0;
    EepromSaveData->gold = 0;

    // Zero levelUnlockStatus and versusLevelAvailability (16 bytes each)
    for (i = 0; i < 16; i++) {
        EepromSaveData->levelUnlockStatus[i] = 0;
        EepromSaveData->versusLevelAvailability[i] = 0;
    }

    // Set first levelUnlockStatus to 5
    EepromSaveData->levelUnlockStatus[0] = 5;

    // Set first 3 pairs of versusLevelAvailability to 5
    for (i = 0; i < 3; i++) {
        EepromSaveData->versusLevelAvailability[i] = 5;
        EepromSaveData->versusLevelAvailability[i + 4] = 5;
    }

    // Zero characterPaletteIds (18 bytes)
    for (i = 0; i < 18; i++) {
        EepromSaveData->characterPaletteIds[i] = 0;
    }

    // Set specific characterPaletteIds values
    EepromSaveData->characterPaletteIds[0] = 1;
    EepromSaveData->characterPaletteIds[3] = 2;
    EepromSaveData->characterPaletteIds[6] = 3;

    // Zero unlocked cutscene IDs
    for (i = 0; i < 9; i++) {
        EepromSaveData->unlockedCutsceneIds[i] = 0;
    }

    // Zero unlockedBoardIds (3 bytes)
    for (i = 0; i < 3; i++) {
        EepromSaveData->unlockedBoardIds[i] = 0;
    }

    // Zero individual settings
    EepromSaveData->specialBoardUnlocked[0] = 0;
    EepromSaveData->specialBoardUnlocked[1] = 0;
    EepromSaveData->specialBoardUnlocked[2] = 0;
    EepromSaveData->postCreditsCutscenePending = 0;

    // Zero padding
    for (i = 0; i < 8; i++) {
        EepromSaveData->reserved[i] = 0;
    }
}
