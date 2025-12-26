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
    void *unk3B0;
    void *unk3B4;
    u16 unk3B8;
    u8 unk3BA;
    u8 unk3BB;
    u8 unk3BC;
    u8 unk3BD;
    u8 unk3BE;
    u8 partialUnlockCheatProgress;
    u8 unlockAllCheatProgress;
    u8 unk3C1;
} TitleState;

USE_ASSET(_414CF0);
USE_ASSET(_418520);

void func_800161F4_16DF4(void);
void func_800159AC_165AC(void);
void checkPartialUnlockCheatCode(void);
void checkUnlockAllCheatCode(void);
void unlockAllContent(void);
void unlockPartialContent(void);
void func_800156AC_162AC(void);

extern u8 D_8009F210_9FE10;
extern s32 gControllerInputs;
extern s32 gButtonsPressed;
extern u8 D_800AB47A_A27EA;
extern u8 D_8008D420_8E020;
extern s8 D_800A8A98_9FE08;

void func_800154E0_160E0(void) {
    func_800697F4_6A3F4(D_8009F210_9FE10);
}

void func_80015504_16104(void) {
    GameState *state;
    u8 unk3BB;
    u8 unk3BD;

    state = getCurrentAllocation();

    if (func_8006FE10_70A10(0) != 0) {
        return;
    }

    func_8006FDA0_709A0(0, 0, 0);
    D_8009F210_9FE10 = 1;

    unk3BB = state->unk3BB;

    if (unk3BB == 3) {
        D_800AFE8C_A71FC->unk4 = 0xFF;
    } else {
        unk3BD = state->unk3BD;
        if (unk3BD == 1) {
            D_800AFE8C_A71FC->unk4 = unk3BB;
            D_800AFE8C_A71FC->saveSlotIndex = 0;
        } else if (unk3BB == 1) {
            D_8009F210_9FE10 = 2;
        } else {
            D_8009F210_9FE10 = 3;
            D_800AFE8C_A71FC->unk4 = 0xFE;
        }
    }

    if (state->unk3B8 == 0x384) {
        D_800AFE8C_A71FC->unk4 = 0;
        D_800AFE8C_A71FC->saveSlotIndex = 0;
        D_8009F210_9FE10 = 4;
    }

    unlinkNode((Node_70B00 *)state);
    unlinkNode((Node_70B00 *)&state->audioPlayer2);

    state->unk3B0 = freeNodeMemory(state->unk3B0);
    state->unk3B4 = freeNodeMemory(state->unk3B4);

    terminateSchedulerWithCallback(func_800154E0_160E0);
}

void func_8001563C_1623C(void) {
    u8 buffer[0x200];
    s32 i;
    s32 result;

    for (i = 0x1FF; i >= 0; i--) {
        buffer[i] = 0;
    }

    func_8003B47C_3C07C(buffer);

    do {
        result = (s32)func_8003B510_3C110();
    } while (result == -1);

    if (result == 0) {
        func_800585C8_591C8(0x2C);
    } else {
        func_800585C8_591C8(0x2B);
    }

    setGameStateHandler(func_800156AC_162AC);
}

void func_800156AC_162AC(void) {
    GameState *state;
    s32 input;
    u8 unk3C1;
    u8 unk3BD;
    u8 unk3BB;
    u16 unk3B8;
    s32 temp;

    state = getCurrentAllocation();

    unk3C1 = state->unk3C1;
    if (unk3C1 != 0) {
        unk3C1--;
        state->unk3C1 = unk3C1;
        if (unk3C1 == 0) {
            func_800574A0_580A0(0x1C);
        }
    }

    input = gControllerInputs;

    if ((input & L_TRIG) != 0) {
        if (((&gControllerInputs)[3] & R_TRIG) != 0) {
            goto end;
        }
    }

    unk3BD = state->unk3BD;

    if (unk3BD == 0) {
        goto case_0;
    } else if (unk3BD == 1) {
        goto case_1;
    } else {
        goto end;
    }

case_0:
    unk3BB = state->unk3BB;

    if ((input & (STICK_UP | U_JPAD)) != 0) {
        if (unk3BB != 0) {
            state->unk3BB = unk3BB - 1;
        } else {
            state->unk3BB = state->unk3BC - 1;
        }
    } else if ((input & (STICK_DOWN | D_JPAD)) != 0) {
        if (unk3BB < state->unk3BC - 1) {
            state->unk3BB = unk3BB + 1;
        } else {
            state->unk3BB = 0;
        }
    }

    unk3B8 = state->unk3B8 + 1;
    state->unk3B8 = unk3B8;

    if (unk3BB != state->unk3BB) {
        func_80058220_58E20(0x2B, 1);
        unk3B8 = state->unk3B8;
        if (unk3B8 >= 0x2D1) {
            state->unk3B8 = 0x2D0;
        }
    }

    if ((gControllerInputs & (A_BUTTON | START_BUTTON)) != 0) {
        func_80058220_58E20(0x2C, 1);
        if (state->unk3BB == 0) {
            state->unk3BD = 1;
            goto end;
        }
        func_80057564_58164(0x20);
        func_8006FDA0_709A0(0, 0xFF, 0x10);
        func_8006983C_6A43C(func_80015504_16104);
        goto end;
    }

    if (state->unk3B8 == 0x384) {
        func_80057564_58164(0x20);
        func_8006FDA0_709A0(0, 0xFF, 0x10);
        func_8006983C_6A43C(func_80015504_16104);
    }
    goto end;

case_1:
    unk3BB = state->unk3BB;

    if ((input & (STICK_UP | U_JPAD)) == 0) {
        goto case1_check2;
    }
    if (unk3BB != 0) {
        state->unk3BB = unk3BB - 1;
        goto case1_done_nav;
    }
    state->unk3BB = 1;
    goto case1_done_nav;

case1_check2:
    if ((input & (STICK_DOWN | D_JPAD)) == 0) {
        goto case1_done_nav;
    }
    if (unk3BB == 0) {
        state->unk3BB = unk3BB + 1;
    } else {
        state->unk3BB = 0;
    }

case1_done_nav:
    if (unk3BB != state->unk3BB) {
        func_80058220_58E20(0x2B, 1);
    }

    temp = gControllerInputs;

    if ((temp & B_BUTTON) != 0) {
        func_800585C8_591C8(0x2E);
        state->unk3BB = 0;
        state->unk3BD = 0;
        goto end;
    }

    if ((temp & (A_BUTTON | START_BUTTON)) != 0) {
        if (D_800AB47A_A27EA != 0) {
            if ((gButtonsPressed & R_JPAD) != 0) {
                unlockAllContent();
            } else if ((gButtonsPressed & L_JPAD) != 0) {
                unlockPartialContent();
            }
        }
        func_80058220_58E20(0x2C, 1);
        func_80057564_58164(0x20);
        func_8006FDA0_709A0(0, 0xFF, 8);
        func_8006983C_6A43C(func_80015504_16104);
    }

end:
    checkPartialUnlockCheatCode();
    checkUnlockAllCheatCode();
}

void func_80015960_16560(void) {
    if (func_8006FE10_70A10(0) == 0) {
        func_8006FE28_70A28(0, 0, 0, 0);
        func_80058220_58E20(0x135, 0);
        setGameStateHandler(func_800156AC_162AC);
    }
}

void func_800159AC_165AC(void) {
    GameState *state = (GameState *)getCurrentAllocation();

    state->unk3B8++;

    if (state->unk3B8 >= 3) {
        state->unk3B8 = 2;
        if (func_8003BB5C_3C75C() == 0) {
            state->unk3B8 = 0;
            func_8006FDA0_709A0(NULL, 0, 0x10);
            setGameStateHandler(func_80015960_16560);
        }
    }
}

void func_80015A18_16618(void) {
    TitleState *state;
    Node_70B00 *node2;
    void *dmaResult;
    s32 i;
    void *checkResult;

    state = (TitleState *)allocateTaskMemory(0x3C8);
    func_8006FDA0_709A0(NULL, 0xFF, 0);
    node2 = &state->node2;
    func_80014440_15040();
    setupTaskSchedulerNodes(0x14, 0, 0, 0, 0, 0, 0, 0);
    func_8006FAA4_706A4((Node_70B00 *)state, NULL, 8, 10, 0);
    setModelCameraTransform(state, 0, 0, -0xA0, -0x78, 0x9F, 0x77);
    func_80027CA0_288A0(node2, 0, 8, 0);
    func_8006FA0C_7060C(node2, 40.0f, 1.3333334f, 10.0f, 10000.0f);
    func_8006FD3C_7093C(node2->id, &D_8008D420_8E020);
    state->unk3B0 = loadCompressedData(&_414CF0_ROM_START, &_414CF0_ROM_END, 0x7B50);
    dmaResult = loadCompressedData(&_418520_ROM_START, &_418520_ROM_END, 0x2238);
    state->unk3BB = 0;
    state->unk3BC = 0;
    state->unk3BD = 0;
    state->unk3BE = 0;
    state->unk3C1 = 0x3C;
    state->partialUnlockCheatProgress = 0;
    state->unlockAllCheatProgress = 0;
    state->unk3B4 = dmaResult;
    state->unk3B8 = 0;

    if (D_800AFE8C_A71FC->unk4 == 0xFE) {
        state->unk3BB = 2;
    } else if (D_800AFE8C_A71FC->saveSlotIndex == 0xF) {
        state->unk3BB = 1;
    }

    func_800161F4_16DF4();

    for (i = 0; i < 4; i++) {
        D_800AFE8C_A71FC->unk9[0x11 + i] = 0;
    }

    D_800A8A98_9FE08 = 0;

    checkResult = __udiv_w_sdiv();
    if (checkResult != NULL) {
        state->unk3BC = 4;
    } else {
        state->unk3BC = 3;
    }

    scheduleTask(&func_80016434_17034, 0, 0, 0x64);
    scheduleTask(&func_80016488_17088, 0, 0, 0x62);
    scheduleTask(&func_80016728_17328, 0, 0, 0x62);

    for (i = 0; i < 7; i++) {
        dmaResult = scheduleTask(&func_800168F4_174F4, 0, 0, 0x62);
        if (dmaResult != NULL) {
            ((Struct16B68 *)dmaResult)->unk2F = i;
        }
    }

    scheduleTask(&func_80016860_17460, 0, 0, 0x62);
    func_80040F6C_41B6C(1, 0x20, 0, 0, 8, 7);
    setGameStateHandler(func_800159AC_165AC);
}

void checkPartialUnlockCheatCode(void) {
    GameState *state;
    s32 buttons;
    u8 state_var;

    state = getCurrentAllocation();
    state_var = state->partialUnlockCheatProgress;

    if (state_var < 5) {
        switch (state_var) {
            case 0:
                if (gButtonsPressed == R_TRIG) {
                    state->partialUnlockCheatProgress = state->partialUnlockCheatProgress + 1;
                }
                break;

            case 1:
                buttons = gButtonsPressed;
                if ((buttons != R_TRIG) && (buttons != 0)) {
                    if (buttons == D_JPAD) {
                        state->partialUnlockCheatProgress = state->partialUnlockCheatProgress + 1;
                    } else {
                        state->partialUnlockCheatProgress = 0xFF;
                    }
                }
                break;

            case 2:
                buttons = gButtonsPressed;
                if ((buttons != D_JPAD) && (buttons != 0)) {
                    if (buttons == L_TRIG) {
                        state->partialUnlockCheatProgress = state->partialUnlockCheatProgress + 1;
                    } else {
                        state->partialUnlockCheatProgress = 0xFF;
                    }
                }
                break;

            case 3:
                buttons = gButtonsPressed;
                if ((buttons != L_TRIG) && (buttons != 0)) {
                    if (buttons == STICK_UP) {
                        state->partialUnlockCheatProgress = state->partialUnlockCheatProgress + 1;
                    } else {
                        state->partialUnlockCheatProgress = 0xFF;
                    }
                }
                break;

            case 4:
                buttons = gButtonsPressed;
                if ((buttons != STICK_UP) && (buttons != 0)) {
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
}

void checkUnlockAllCheatCode(void) {
    GameState *state;
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
                    if (buttons != 0x8) {
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
                    if (buttons != CONT_R) {
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

void func_800161F4_16DF4(void) {
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
