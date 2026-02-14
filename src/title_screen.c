#include "10AD0.h"
#include "16FA0.h"
#include "288A0.h"
#include "4050.h"
#include "413E0.h"
#include "D_800AFE8C_A71FC_type.h"
#include "EepromSaveData_type.h"
#include "assets.h"
#include "audio.h"
#include "common.h"
#include "controller_io.h"
#include "graphics.h"
#include "race_session.h"
#include "rom_loader.h"
#include "task_scheduler.h"

typedef struct {
    s16 x;
    s16 z;
} CoordPair;

typedef struct {
    ViewportNode node1;
    ViewportNode node2;
    void *titleLogoData;
    void *menuGraphicsData;
    u16 frameCounter;
    u8 unk3BA;
    u8 menuSelection;
    u8 menuOptionCount;
    u8 menuMode;
    u8 unk3BE;
    u8 partialUnlockCheatProgress;
    u8 unlockAllCheatProgress;
    u8 initialSoundDelay;
} TitleState;

void resetSaveDataToDefaults(void);
void waitForTitleAssetsReady(void);
void checkPartialUnlockCheatCode(void);
void checkUnlockAllCheatCode(void);
void unlockAllContent(void);
void unlockPartialContent(void);
void handleTitleMenuInput(void);

extern u8 gTitleExitMode;
extern s32 gControllerInputs;
extern s32 gButtonsPressed;
extern u8 gDebugUnlockEnabled;
extern s8 gTitleInitialized;

// Data segment definitions
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

s32 D_8008D520_8E120[2] = {
    0x00040004,
    0xFFFF0000,
};

s32 D_8008D528_8E128[3] = {
    0x00050006,
    0x00060007,
    0xFFFF0000,
};

s32 *gTitleCharacterAnimSequences[2] = {
    D_8008D520_8E120,
    D_8008D528_8E128,
};

s32 D_8008D53C_8E13C[3] = {
    0xFFFE004C,
    0x004F002B,
    0x0053FFFF,
};

s32 D_8008D548_8E148[4] = {
    0x00530069,
    0x002B0055,
    0x003F006E,
    0xFFFF0000,
};

s32 D_8008D558_8E158[4] = {
    0xFFFB0044,
    0x0082004B,
    0x003D003F,
    0xFFFF0000,
};

s32 D_8008D568_8E168[3] = {
    0xFFFE0043,
    0x00790036,
    0x004FFFFF,
};

s32 D_8008D574_8E174[3] = {
    0x004C0053,
    0x002B0067,
    0x002BFFFF,
};

s32 D_8008D580_8E180[17] = {
    0xFFFE007B,
    0x00530068,
    0xFFFF0000,
    (s32)D_8008D53C_8E13C,
    (s32)D_8008D548_8E148,
    (s32)D_8008D558_8E158,
    (s32)D_8008D568_8E168,
    (s32)D_8008D574_8E174,
    (s32)D_8008D580_8E180,
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

s16 D_8008D6B0_8E2B0[10] = {
    0x000F, 0x000F, 0x0013, 0x0013, 0x0013, 0x000F, 0x000F, 0x000F, 0x0013, 0x0000,
};

// D_8008D6C4_8E2C4: 20 shorts (40 bytes)
// D_8008D6EC_8E2EC follows immediately: 10 CoordPairs (40 bytes)
// Total: 80 bytes = 0x50 as shown in symbol_addrs.txt
s16 D_8008D6C4_8E2C4[20] = {
    0xF93E, 0xF83E, 0xFC84, 0xFB84, 0xFF26, 0xFE26, 0x01CC, 0x00CC, 0x047E, 0x037E,
    0x072A, 0x062A, 0x0020, 0xFF80, 0xFDB8, 0xFCF8, 0x0374, 0x0294, 0x05C2, 0x04E2,
};

// D_8008D6EC_8E2EC must immediately follow D_8008D6C4_8E2C4
CoordPair D_8008D6EC_8E2EC[10] = {
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
s8 D_8008D714_8E314[6][20] = {
    "PAINT", "U.F.O.", "TIMES", "SCHOOL", "BOARDSHOP", "TEACHER",
};

extern void func_8001A110_1AD10(void);
extern void initGalleryMenu(void);
extern void initStoryMapLocationIntro(void);
extern void initUnlockScreen(void);
extern void initStoryMapExtraIntro(void);

void (*storyMapLocationHandlers[])(void) = {
    NULL,
    func_8001A110_1AD10,
    initGalleryMenu,
    initStoryMapLocationIntro,
    loadOverlay_1BBA0,
    initUnlockScreen,
    initStoryMapLocationIntro,
    func_8001C920_1D520,
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
    TitleState *state;
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
        D_800AFE8C_A71FC->unk4 = 0xFF;
    } else {
        menuMode = state->menuMode;
        if (menuMode == 1) {
            D_800AFE8C_A71FC->unk4 = menuSelection;
            D_800AFE8C_A71FC->saveSlotIndex = 0;
        } else if (menuSelection == 1) {
            gTitleExitMode = 2;
        } else {
            gTitleExitMode = 3;
            D_800AFE8C_A71FC->unk4 = 0xFE;
        }
    }

    if (state->frameCounter == 0x384) {
        D_800AFE8C_A71FC->unk4 = 0;
        D_800AFE8C_A71FC->saveSlotIndex = 0;
        gTitleExitMode = 4;
    }

    unlinkNode((ViewportNode *)state);
    unlinkNode((&state->node2));

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
    TitleState *state;
    s32 input;
    u8 soundDelay;
    u8 menuMode;
    u8 menuSelection;
    u16 frameCounter;
    s32 temp;

    state = getCurrentAllocation();

    soundDelay = state->initialSoundDelay;
    if (soundDelay != 0) {
        soundDelay--;
        state->initialSoundDelay = soundDelay;
        if (soundDelay == 0) {
            playMusicTrack(0x1C);
        }
    }

    input = gControllerInputs;

    if ((input & L_TRIG) != 0) {
        if (((&gControllerInputs)[3] & R_TRIG) != 0) {
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

    frameCounter = state->frameCounter + 1;
    state->frameCounter = frameCounter;

    if (menuSelection != state->menuSelection) {
        playSoundEffectOnChannelNoPriority(0x2B, 1);
        frameCounter = state->frameCounter;
        if (frameCounter >= 0x2D1) {
            state->frameCounter = 0x2D0;
        }
    }

    if ((gControllerInputs & (A_BUTTON | START_BUTTON)) != 0) {
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

    if (state->frameCounter == 0x384) {
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

    temp = gControllerInputs;

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
    TitleState *state = (TitleState *)getCurrentAllocation();

    state->frameCounter++;

    if (state->frameCounter >= 3) {
        state->frameCounter = 2;
        if (getPendingDmaCount() == 0) {
            state->frameCounter = 0;
            setViewportFadeValue(NULL, 0, 0x10);
            setGameStateHandler(onTitleFadeInComplete);
        }
    }
}

void initTitleScreen(void) {
    TitleState *state;
    ViewportNode *node2;
    void *dmaResult;
    s32 i;
    void *checkResult;

    state = (TitleState *)allocateTaskMemory(0x3C8);
    setViewportFadeValue(NULL, 0xFF, 0);
    node2 = &state->node2;
    initDefaultFontPalette();
    setupTaskSchedulerNodes(0x14, 0, 0, 0, 0, 0, 0, 0);
    initViewportNode((ViewportNode *)state, NULL, 8, 10, 0);
    setModelCameraTransform(state, 0, 0, -0xA0, -0x78, 0x9F, 0x77);
    initMenuCameraNode(node2, 0, 8, 0);
    func_8006FA0C_7060C(node2, 40.0f, 1.3333334f, 10.0f, 10000.0f);
    setViewportTransformById(node2->id, &gTitleCameraSettings);
    state->titleLogoData = loadCompressedData(&_414CF0_ROM_START, &_414CF0_ROM_END, 0x7B50);
    dmaResult = loadCompressedData(&_418520_ROM_START, &_418520_ROM_END, 0x2238);
    state->menuSelection = 0;
    state->menuOptionCount = 0;
    state->menuMode = 0;
    state->unk3BE = 0;
    state->initialSoundDelay = 0x3C;
    state->partialUnlockCheatProgress = 0;
    state->unlockAllCheatProgress = 0;
    state->menuGraphicsData = dmaResult;
    state->frameCounter = 0;

    if (D_800AFE8C_A71FC->unk4 == 0xFE) {
        state->menuSelection = 2;
    } else if (D_800AFE8C_A71FC->saveSlotIndex == 0xF) {
        state->menuSelection = 1;
    }

    resetSaveDataToDefaults();

    for (i = 0; i < 4; i++) {
        D_800AFE8C_A71FC->unk9[0x11 + i] = 0;
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
    scheduleTask(&func_80016728_17328, 0, 0, 0x62);

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
    TitleState *state = getCurrentAllocation();
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
    TitleState *state;
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

    // First loop: initialize save_slot_status and save_slot_data
    for (i = 0; i < 16; i++) {
        EepromSaveData->save_slot_status[i] = 1;
        EepromSaveData->save_slot_data[i] = 1;
    }

    // Second loop: initialize character_or_settings and related arrays
    for (i = 0; i < 9; i++) {
        EepromSaveData->character_or_settings[i] = i + 1;
        EepromSaveData->character_or_settings[i + 9] = i + 0x11;
        EepromSaveData->u.setting_42[i] = i + 9;
    }

    // Set individual bytes
    EepromSaveData->setting_4B[0] = 0xD;
    EepromSaveData->setting_4B[1] = 0xE;
    EepromSaveData->setting_4B[2] = 0xF;
    EepromSaveData->setting_4E = 1;
    EepromSaveData->setting_4F = 1;
    EepromSaveData->setting_50 = 1;
}

void unlockPartialContent(void) {
    s32 i;

    for (i = 0; i < 16; i++) {
        EepromSaveData->save_slot_status[i] = 1;
        EepromSaveData->save_slot_data[i] = 1;
    }

    EepromSaveData->setting_4E = 1;
    EepromSaveData->setting_4F = 1;
    EepromSaveData->setting_50 = 0;

    for (i = 0; i < 9; i++) {
        EepromSaveData->character_or_settings[i] = i + 1;
    }

    EepromSaveData->setting_4B[0] = 0xD;
    EepromSaveData->setting_4B[1] = 0xF;
}

void resetSaveDataToDefaults(void) {
    s32 i;

    // Zero header_data (8 bytes)
    for (i = 0; i < 8; i++) {
        EepromSaveData->header_data[i] = 0;
    }

    // Zero checksum and unknown_0C
    EepromSaveData->checksum = 0;
    *(u32 *)&EepromSaveData->unknown_0C = 0;

    // Zero save_slot_status and save_slot_data (16 bytes each)
    for (i = 0; i < 16; i++) {
        EepromSaveData->save_slot_status[i] = 0;
        EepromSaveData->save_slot_data[i] = 0;
    }

    // Set first save_slot_status to 5
    EepromSaveData->save_slot_status[0] = 5;

    // Set first 3 pairs of save_slot_data to 5
    for (i = 0; i < 3; i++) {
        EepromSaveData->save_slot_data[i] = 5;
        EepromSaveData->save_slot_data[i + 4] = 5;
    }

    // Zero character_or_settings (18 bytes)
    for (i = 0; i < 18; i++) {
        EepromSaveData->character_or_settings[i] = 0;
    }

    // Set specific character_or_settings values
    EepromSaveData->character_or_settings[0] = 1;
    EepromSaveData->character_or_settings[3] = 2;
    EepromSaveData->character_or_settings[6] = 3;

    // Zero setting_42 (9 bytes)
    for (i = 0; i < 9; i++) {
        EepromSaveData->u.setting_42[i] = 0;
    }

    // Zero setting_4B (3 bytes)
    for (i = 0; i < 3; i++) {
        EepromSaveData->setting_4B[i] = 0;
    }

    // Zero individual settings
    EepromSaveData->setting_4E = 0;
    EepromSaveData->setting_4F = 0;
    EepromSaveData->setting_50 = 0;
    EepromSaveData->unk51 = 0;

    // Zero padding
    for (i = 0; i < 8; i++) {
        EepromSaveData->padding_52[i] = 0;
    }
}
