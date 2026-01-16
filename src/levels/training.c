#include "../race_session.h"
#include "36B80.h"
#include "56910.h"
#include "common.h"
#include "graphics.h"
#include "rom_loader.h"
#include "task_scheduler.h"

USE_ASSET(_40E1C0);

extern void func_8006D7B0_6E3B0(s32, s16, s16, s16, s16, u8, u8, u8, u8, u8);
extern s32 gControllerInputs;
extern s8 *s_trainingPanelMessageTables[];

typedef struct {
    void *uiAsset;
    void *textRenderContext;
    void *messageData;
    s16 panelIndex;
} TrainingInstructionState;

typedef struct {
    s32 uiAsset;
    void *textRenderContext;
    s32 *messageData;
    s16 panelIndex;
    s16 messageIndex;
    s16 panelWidth;
    s16 panelHeight;
    s16 alphaColor;
    u8 scale;
    u8 colorIndex;
} TrainingInstructionRuntimeState;

#define GET_ALPHA_COLOR_HIGH_BYTE(arg0) (((u8 *)&(arg0)->alphaColor)[1])

void shrinkTrainingInstructionPanelWidth(TrainingInstructionRuntimeState *arg0);

void cleanupTrainingInstructionTask(TrainingInstructionState *arg0);

void checkTrainingInstructionCheckpoint(TrainingInstructionRuntimeState *arg0);

void initTrainingInstructionTask(TrainingInstructionState *arg0) {
    getCurrentAllocation();
    arg0->textRenderContext = loadTextRenderAsset(1);
    arg0->uiAsset = loadAsset_34F7E0();
    arg0->messageData = loadCompressedData(&_40E1C0_ROM_START, &_40E1C0_ROM_END, 0x1130);
    arg0->panelIndex = 0;
    setCleanupCallback(cleanupTrainingInstructionTask);
    setCallback(checkTrainingInstructionCheckpoint);
}

void expandTrainingInstructionPanelWidth(TrainingInstructionRuntimeState *arg0);

void checkTrainingInstructionCheckpoint(TrainingInstructionRuntimeState *arg0) {
    GameState *state = getCurrentAllocation();

    if (state->gamePaused == 0) {
        switch (arg0->panelIndex) {
        case 0:
            if (state->raceIntroState != 0) {
                break;
            }
            goto do_action;
        case 1:
            if ((state->players->sectorIndex < 3) || (state->players->unkB84 & 1)) {
                break;
            }
            goto do_action;
        case 2:
            if (state->players->sectorIndex < 0xE) {
                break;
            }
            goto do_action;
        case 3:
            if ((state->players->sectorIndex < 0x12) || (state->players->unkB84 & 1)) {
                break;
            }
            goto do_action;
        case 4:
            if (state->players->sectorIndex < 0x14) {
                break;
            }
            goto do_action;
        case 5:
            if (state->players->sectorIndex < 0x1A) {
                break;
            }
            goto do_action;
        case 6:
            if (state->players->sectorIndex < 0x28) {
                break;
            }
            goto do_action;
        case 7:
            if (state->players->sectorIndex < 0x2D) {
                break;
            }
            goto do_action;
        case 8:
            if (state->players->sectorIndex < 0x30) {
                break;
            }
            goto do_action;
        case 9:
            if (state->players->sectorIndex < 0x32) {
                break;
            }
            goto do_action;
        case 10:
            if (state->players->sectorIndex < 0x35) {
                break;
            }
            goto do_action;
        case 11:
            if (state->players->sectorIndex < 0x38) {
                break;
            }
            goto do_action;
        case 12:
            if ((state->players->currentLap == 0) || (state->players->sectorIndex == 0)) {
                break;
            }
            goto do_action;
        case 13:
            break;
        default:
        do_action:
            state->trainingPanelState = 1;
            playSoundEffect(0x2C);
            arg0->panelWidth = 1;
            arg0->panelHeight = 1;
            arg0->alphaColor = 0xF0;
            arg0->messageIndex = 0;
            *(s16 *)&arg0->scale = 0xC0;
            setCallback(expandTrainingInstructionPanelWidth);
            break;
        }
    }
}

void expandTrainingInstructionPanelHeight(TrainingInstructionRuntimeState *arg0);
void displayTrainingInstructionAndWaitForInput(TrainingInstructionRuntimeState *arg0);
void shrinkTrainingInstructionPanelForNextInstruction(TrainingInstructionRuntimeState *arg0);

void expandTrainingInstructionPanelWidth(TrainingInstructionRuntimeState *arg0) {
    s16 width;
    s16 height;

    arg0->panelWidth += 2;

    if (arg0->alphaColor != 0x40) {
        arg0->alphaColor -= 0x10;
    }

    if (arg0->panelWidth == 0xD) {
        setCallback(expandTrainingInstructionPanelHeight);
    }

    width = arg0->panelWidth;
    height = arg0->panelHeight;

    func_8006D7B0_6E3B0(
        (s32)arg0->uiAsset,
        ((-width) << 19) >> 16,
        ((-height) << 19) >> 16,
        width,
        height,
        0,
        GET_ALPHA_COLOR_HIGH_BYTE(arg0),
        arg0->colorIndex,
        0xC,
        0x6
    );
}

void shrinkTrainingInstructionPanelForNextMessage(TrainingInstructionRuntimeState *arg0) {
    s16 width;
    s16 height;

    arg0->panelHeight--;
    arg0->alphaColor += 0x10;

    if (arg0->panelHeight == 1) {
        setCallback(expandTrainingInstructionPanelHeight);
    }

    width = arg0->panelWidth;
    height = arg0->panelHeight;

    func_8006D7B0_6E3B0(
        (s32)arg0->uiAsset,
        ((-width) << 19) >> 16,
        ((-height) << 19) >> 16,
        width,
        height,
        0,
        (u8)arg0->alphaColor,
        arg0->colorIndex,
        0xC,
        0x6
    );
}

void expandTrainingInstructionPanelHeight(TrainingInstructionRuntimeState *arg0) {
    s16 width;
    s16 height;

    arg0->panelHeight++;

    if (arg0->alphaColor != 0x40) {
        arg0->alphaColor -= 0x10;
    }

    if (arg0->panelHeight == 6) {
        setCallback(displayTrainingInstructionAndWaitForInput);
    }

    width = arg0->panelWidth;
    height = arg0->panelHeight;

    func_8006D7B0_6E3B0(
        (s32)arg0->uiAsset,
        ((-width) << 19) >> 16,
        ((-height) << 19) >> 16,
        width,
        height,
        0,
        GET_ALPHA_COLOR_HIGH_BYTE(arg0),
        arg0->colorIndex,
        0xC,
        0x6
    );
}

void displayTrainingInstructionAndWaitForInput(TrainingInstructionRuntimeState *arg0) {
    s8 *table_ptr;
    s32 temp_v1_2;
    s16 temp_v0;
    s8 temp_v1_3;
    s32 s0_var = 6;
    s32 s1_var = 0xC;

    table_ptr = s_trainingPanelMessageTables[arg0->panelIndex];
    temp_v1_2 = arg0->messageData[table_ptr[arg0->messageIndex]];
    func_80035260_35E60(arg0->textRenderContext, (void *)arg0->messageData + temp_v1_2, -0x68, -0x30, 0xFF, 0xFF, 0, s1_var, s0_var);

    func_8006D7B0_6E3B0(arg0->uiAsset, -0x68, -0x30, 0xD, s0_var, 1, GET_ALPHA_COLOR_HIGH_BYTE(arg0), arg0->colorIndex, s1_var, s0_var);

    if (gControllerInputs & A_BUTTON) {
        temp_v0 = arg0->messageIndex + 1;
        arg0->messageIndex = temp_v0;
        table_ptr = s_trainingPanelMessageTables[arg0->panelIndex];
        temp_v1_3 = table_ptr[temp_v0];
        if (temp_v1_3 == -1) {
            playSoundEffect(0x2D);
            arg0->panelIndex = arg0->panelIndex + 1;
            setCallback(shrinkTrainingInstructionPanelForNextInstruction);
        } else {
            playSoundEffect(0x2B);
            setCallback(shrinkTrainingInstructionPanelForNextMessage);
        }
    }
}

#define GET_SCALE_AS_S16(arg0) (*(s16 *)&(arg0)->scale)
#define SET_SCALE_AS_S16(arg0, val) (*(s16 *)&(arg0)->scale = (val))

void shrinkTrainingInstructionPanelForNextInstruction(TrainingInstructionRuntimeState *arg0) {
    s16 scale;
    s16 width;
    s16 height;

    arg0->panelHeight--;

    scale = GET_SCALE_AS_S16(arg0);
    if (scale != 0) {
        SET_SCALE_AS_S16(arg0, scale - 0x10);
    }

    if (arg0->panelHeight == 1) {
        setCallback(shrinkTrainingInstructionPanelWidth);
    }

    width = arg0->panelWidth;
    height = arg0->panelHeight;

    func_8006D7B0_6E3B0(
        (s32)arg0->uiAsset,
        ((-width) << 19) >> 16,
        ((-height) << 19) >> 16,
        width,
        height,
        0,
        (u8)arg0->alphaColor,
        arg0->colorIndex,
        0xC,
        0x6
    );
}

void shrinkTrainingInstructionPanelWidth(TrainingInstructionRuntimeState *arg0) {
    s16 scale;
    s16 width;
    s16 height;
    GameState *state;

    state = getCurrentAllocation();
    arg0->panelWidth--;

    scale = GET_SCALE_AS_S16(arg0);
    if (scale != 0) {
        SET_SCALE_AS_S16(arg0, scale - 0x10);
    }

    if (arg0->panelWidth == 1) {
        state->trainingPanelState = -1;
        setCallback(checkTrainingInstructionCheckpoint);
    }

    width = arg0->panelWidth;
    height = arg0->panelHeight;

    func_8006D7B0_6E3B0(
        arg0->uiAsset,
        ((-width) << 19) >> 16,
        ((-height) << 19) >> 16,
        width,
        height,
        0,
        GET_ALPHA_COLOR_HIGH_BYTE(arg0),
        arg0->colorIndex,
        0xC,
        0x6
    );
}

void cleanupTrainingInstructionTask(TrainingInstructionState *arg0) {
    arg0->uiAsset = freeNodeMemory(arg0->uiAsset);
    arg0->textRenderContext = freeNodeMemory(arg0->textRenderContext);
    arg0->messageData = freeNodeMemory(arg0->messageData);
}
