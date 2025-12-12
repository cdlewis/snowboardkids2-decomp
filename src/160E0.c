#include "3A1F0.h"
#include "3E160.h"
#include "6E840.h"
#include "D_800AFE8C_A71FC_type.h"
#include "EepromSaveData_type.h"
#include "common.h"
#include "gamestate.h"
#include "graphics.h"
#include "task_scheduler.h"

extern s32 gControllerInputs;
extern s32 gButtonsPressed;
extern u8 D_800AB47A_A27EA;

void func_80015CA4_168A4(void);
void func_80015DF4_169F4(void);
void func_80016070_16C70(void);
void func_80016150_16D50(void);

void func_800156AC_162AC(void);

extern u8 D_8009F210_9FE10;

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

    if ((input & 0x20) != 0) {
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

    if ((input & 0x10800) != 0) {
        if (unk3BB != 0) {
            state->unk3BB = unk3BB - 1;
        } else {
            state->unk3BB = state->unk3BC - 1;
        }
    } else if ((input & 0x20400) != 0) {
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

    if ((gControllerInputs & 0x9000) != 0) {
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

    if ((input & 0x10800) == 0) {
        goto case1_check2;
    }
    if (unk3BB != 0) {
        state->unk3BB = unk3BB - 1;
        goto case1_done_nav;
    }
    state->unk3BB = 1;
    goto case1_done_nav;

case1_check2:
    if ((input & 0x20400) == 0) {
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

    if ((temp & 0x9000) != 0) {
        if (D_800AB47A_A27EA != 0) {
            if ((gButtonsPressed & 0x100) != 0) {
                func_80016070_16C70();
            } else if ((gButtonsPressed & 0x200) != 0) {
                func_80016150_16D50();
            }
        }
        func_80058220_58E20(0x2C, 1);
        func_80057564_58164(0x20);
        func_8006FDA0_709A0(0, 0xFF, 8);
        func_8006983C_6A43C(func_80015504_16104);
    }

end:
    func_80015CA4_168A4();
    func_80015DF4_169F4();
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

INCLUDE_ASM("asm/nonmatchings/160E0", func_80015A18_16618);

void func_80015CA4_168A4(void) {
    GameState *state;
    s32 buttons;
    u8 state_var;

    state = getCurrentAllocation();
    state_var = state->unk3BF;

    if (state_var < 5) {
        switch (state_var) {
            case 0:
                if (gButtonsPressed == 0x10) {
                    state->unk3BF = state->unk3BF + 1;
                }
                break;

            case 1:
                buttons = gButtonsPressed;
                if ((buttons != 0x10) && (buttons != 0)) {
                    if (buttons == 0x400) {
                        state->unk3BF = state->unk3BF + 1;
                    } else {
                        state->unk3BF = 0xFF;
                    }
                }
                break;

            case 2:
                buttons = gButtonsPressed;
                if ((buttons != 0x400) && (buttons != 0)) {
                    if (buttons == 0x20) {
                        state->unk3BF = state->unk3BF + 1;
                    } else {
                        state->unk3BF = 0xFF;
                    }
                }
                break;

            case 3:
                buttons = gButtonsPressed;
                if ((buttons != 0x20) && (buttons != 0)) {
                    if (buttons == 0x10000) {
                        state->unk3BF = state->unk3BF + 1;
                    } else {
                        state->unk3BF = 0xFF;
                    }
                }
                break;

            case 4:
                buttons = gButtonsPressed;
                if ((buttons != 0x10000) && (buttons != 0)) {
                    if (buttons == 0x1000) {
                        func_80016150_16D50();
                        func_800585C8_591C8(0xDC);
                        func_800585C8_591C8(0xDC);
                        state->unk3BF = 0xF0;
                    } else {
                        state->unk3BF = 0xFF;
                    }
                }
                break;
        }
    }
}

void func_80015DF4_169F4(void) {
    GameState *state;
    u8 cheatState;
    s32 buttons;
    s32 temp_v1;

    state = getCurrentAllocation();
    cheatState = state->unk3C0;

    if (cheatState < 0xA) {
        switch (cheatState) {
            case 0:
                temp_v1 = gButtonsPressed;
                if (temp_v1 == 0x2000) {
                    state->unk3C0++;
                }
                break;

            case 1:
                buttons = gButtonsPressed;
                if ((buttons != 0x2000) && (buttons != 0)) {
                    if (buttons != 0x4000) {
                        state->unk3C0 = 0xFF;
                    } else {
                        state->unk3C0++;
                    }
                }
                break;

            case 2:
                buttons = gButtonsPressed;
                if ((buttons != 0x4000) && (buttons != 0)) {
                    if (buttons != 0x8) {
                        state->unk3C0 = 0xFF;
                    } else {
                        state->unk3C0++;
                    }
                }
                break;

            case 3:
                buttons = gButtonsPressed;
                if ((buttons != 0x8) && (buttons != 0)) {
                    if (buttons != 0x400) {
                        state->unk3C0 = 0xFF;
                    } else {
                        state->unk3C0++;
                    }
                }
                break;

            case 4:
                buttons = gButtonsPressed;
                if ((buttons != 0x400) && (buttons != 0)) {
                    if (buttons != 0x80000) {
                        state->unk3C0 = 0xFF;
                    } else {
                        state->unk3C0++;
                    }
                }
                break;

            case 5:
                buttons = gButtonsPressed;
                if ((buttons != 0x80000) && (buttons != 0)) {
                    if (buttons != 0x40000) {
                        state->unk3C0 = 0xFF;
                    } else {
                        state->unk3C0++;
                    }
                }
                break;

            case 6:
                buttons = gButtonsPressed;
                if ((buttons != 0x40000) && (buttons != 0)) {
                    if (buttons != 0x800) {
                        state->unk3C0 = 0xFF;
                    } else {
                        state->unk3C0++;
                    }
                }
                break;

            case 7:
                buttons = gButtonsPressed;
                if ((buttons != 0x800) && (buttons != 0)) {
                    if (buttons != 0x10) {
                        state->unk3C0 = 0xFF;
                    } else {
                        state->unk3C0++;
                    }
                }
                break;

            case 8:
                buttons = gButtonsPressed;
                if ((buttons != 0x10) && (buttons != 0)) {
                    if (buttons != 0x2000) {
                        state->unk3C0 = 0xFF;
                    } else {
                        state->unk3C0++;
                    }
                }
                break;

            case 9:
                buttons = gButtonsPressed;
                if ((buttons != 0x2000) && (buttons != 0)) {
                    if (buttons == 0x8000) {
                        func_80016070_16C70();
                        func_800585C8_591C8(0x110);
                        func_800585C8_591C8(0x110);
                        func_800585C8_591C8(0x110);
                        func_800585C8_591C8(0x110);
                        state->unk3C0 = 0xF0;
                    } else {
                        state->unk3C0 = 0xFF;
                    }
                }
                break;
        }
    }

    cheatState = state->unk3C0;
    if (cheatState == 0xFF) {
        state->unk3C0 = 0xFF;
    }
}

void func_80016070_16C70(void) {
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

void func_80016150_16D50(void) {
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
