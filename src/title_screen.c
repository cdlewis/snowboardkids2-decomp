#include "10AD0.h"
#include "16FA0.h"
#include "288A0.h"
#include "3A1F0.h"
#include "3E160.h"
#include "4050.h"
#include "413E0.h"
#include "6E840.h"
#include "D_800AFE8C_A71FC_type.h"
#include "EepromSaveData_type.h"
#include "common.h"
#include "gamestate.h"
#include "graphics.h"
#include "overlay.h"
#include "task_scheduler.h"

typedef struct {
    Node_70B00 node1;
    Node_70B00 node2;
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

USE_ASSET(_414CF0);
USE_ASSET(_418520);

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
extern u8 gTitleCameraSettings;
extern s8 gTitleInitialized;

void exitTitleToNextMode(void) {
    func_800697F4_6A3F4(gTitleExitMode);
}

void cleanupTitleAndTransition(void) {
    TitleState *state;
    u8 menuSelection;
    u8 menuMode;

    state = getCurrentAllocation();

    if (func_8006FE10_70A10(0) != 0) {
        return;
    }

    func_8006FDA0_709A0(0, 0, 0);
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

    unlinkNode((Node_70B00 *)state);
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
        func_800585C8_591C8(0x2C);
    } else {
        func_800585C8_591C8(0x2B);
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
            func_800574A0_580A0(0x1C);
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
        func_80058220_58E20(0x2B, 1);
        frameCounter = state->frameCounter;
        if (frameCounter >= 0x2D1) {
            state->frameCounter = 0x2D0;
        }
    }

    if ((gControllerInputs & (A_BUTTON | START_BUTTON)) != 0) {
        func_80058220_58E20(0x2C, 1);
        if (state->menuSelection == 0) {
            state->menuMode = 1;
            goto end;
        }
        func_80057564_58164(0x20);
        func_8006FDA0_709A0(0, 0xFF, 0x10);
        func_8006983C_6A43C(cleanupTitleAndTransition);
        goto end;
    }

    if (state->frameCounter == 0x384) {
        func_80057564_58164(0x20);
        func_8006FDA0_709A0(0, 0xFF, 0x10);
        func_8006983C_6A43C(cleanupTitleAndTransition);
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
        func_80058220_58E20(0x2B, 1);
    }

    temp = gControllerInputs;

    if ((temp & B_BUTTON) != 0) {
        func_800585C8_591C8(0x2E);
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
        func_80058220_58E20(0x2C, 1);
        func_80057564_58164(0x20);
        func_8006FDA0_709A0(0, 0xFF, 8);
        func_8006983C_6A43C(cleanupTitleAndTransition);
    }

end:
    checkPartialUnlockCheatCode();
    checkUnlockAllCheatCode();
}

void onTitleFadeInComplete(void) {
    if (func_8006FE10_70A10(0) == 0) {
        func_8006FE28_70A28(0, 0, 0, 0);
        func_80058220_58E20(0x135, 0);
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
            func_8006FDA0_709A0(NULL, 0, 0x10);
            setGameStateHandler(onTitleFadeInComplete);
        }
    }
}

void initTitleScreen(void) {
    TitleState *state;
    Node_70B00 *node2;
    void *dmaResult;
    s32 i;
    void *checkResult;

    state = (TitleState *)allocateTaskMemory(0x3C8);
    func_8006FDA0_709A0(NULL, 0xFF, 0);
    node2 = &state->node2;
    initDefaultFontPalette();
    setupTaskSchedulerNodes(0x14, 0, 0, 0, 0, 0, 0, 0);
    func_8006FAA4_706A4((Node_70B00 *)state, NULL, 8, 10, 0);
    setModelCameraTransform(state, 0, 0, -0xA0, -0x78, 0x9F, 0x77);
    initMenuCameraNode(node2, 0, 8, 0);
    func_8006FA0C_7060C(node2, 40.0f, 1.3333334f, 10.0f, 10000.0f);
    func_8006FD3C_7093C(node2->id, &gTitleCameraSettings);
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
    scheduleTask(&func_80016488_17088, 0, 0, 0x62);
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
                    func_800585C8_591C8(0xDC);
                    func_800585C8_591C8(0xDC);
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
                        func_800585C8_591C8(0x110);
                        func_800585C8_591C8(0x110);
                        func_800585C8_591C8(0x110);
                        func_800585C8_591C8(0x110);
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
