#include "assets.h"
#include "audio/audio.h"
#include "common.h"
#include "data/course_data.h"
#include "font_encoding.h"
#include "gamestate.h"
#include "os_cont.h"
#include "race/race_session.h"
#include "system/controller_io.h"
#include "system/task_scheduler.h"
#include "text/font_render.h"

extern void renderTiledSprite3x3(void *, s16, s16, s16, s16, u8, u8, u8, u8, u8);

/* Data segment - forward declarations */
u8 trainingStartAndJumpPromptText[176];
u8 trainingCorneringPromptText[156];
u8 trainingCoinPromptText[196];
u8 trainingWeaponsPromptText[184];
u8 trainingSpecialTechniquePromptText[180];
u8 trainingItemPromptText[188];
u8 trainingCornering2PromptText[208];
u8 trainingGrabTrickPromptText[248];
u8 trainingGrabTrickMoneyPromptText[76];
u8 trainingSpinningTrickPromptText[236];
u8 trainingSpinningTrickMoneyPromptText[176];
u8 trainingCombinationTricksPromptText[248];
u8 trainingContinuousSpinTrickPromptText[276];
u8 trainingContinuousSpinTapPromptText[180];
u8 trainingSpinDirectionPromptText[204];
u8 trainingWarningSignPromptText[204];
u8 trainingWarningSignDodgePromptText[184];
u8 trainingLiftPromptText[260];
u8 trainingAccelerationTechniquePromptText[240];
u8 trainingMaxSpeedWarningPromptText[296];
u8 trainingSessionCompletePromptText[196];
s8 D_800BCB60_B7440[];
s8 D_800BCB64_B7444[];
s8 D_800BCB68_B7448[];
s8 D_800BCB6C_B744C[];
s8 D_800BCB70_B7450[];
s8 D_800BCB74_B7454[];
s8 D_800BCB78_B7458[];
s8 D_800BCB7C_B745C[];
s8 D_800BCB80_B7460[];
s8 D_800BCB84_B7464[];
s8 D_800BCB88_B7468[];
s8 D_800BCB8C_B746C[];
s8 D_800BCB90_B7470[];

typedef struct {
    void *uiAsset;
    void *textRenderContext;
    void *messageData;
    s16 panelIndex;
} TrainingInstructionState;

typedef struct {
    void *uiAsset;
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

/* Data segment - pointer table */
u8 *trainingInstructionPromptTexts[] = {
    trainingStartAndJumpPromptText,
    trainingStartAndJumpPromptText,
    trainingCorneringPromptText,
    trainingCoinPromptText,
    trainingWeaponsPromptText,
    trainingSpecialTechniquePromptText,
    trainingItemPromptText,
    trainingCornering2PromptText,
    trainingGrabTrickPromptText,
    trainingGrabTrickMoneyPromptText,
    trainingSpinningTrickPromptText,
    trainingSpinningTrickMoneyPromptText,
    trainingCombinationTricksPromptText,
    trainingContinuousSpinTrickPromptText,
    trainingContinuousSpinTapPromptText,
    trainingSpinDirectionPromptText,
    trainingWarningSignPromptText,
    trainingWarningSignDodgePromptText,
    trainingLiftPromptText,
    trainingAccelerationTechniquePromptText,
    trainingMaxSpeedWarningPromptText,
    trainingSessionCompletePromptText,
};

/* Data segment - panel data arrays */
// clang-format off
u8 trainingStartAndJumpPromptText[176] = { _("{0xFFFC}{0x0005}          \"Start & Jump\"\nChase Coach!\nJump by pressing the\nA Button to move forward.") };

u8 trainingCorneringPromptText[156] = { _("{0xFFFC}{0x0005}             \"Cornering\"\nTilt the Control Stick\nto the left/right to turn.") };

u8 trainingCoinPromptText[196] = { _("{0xFFFC}{0x0005}                  \"Coin\"\nA coin is worth {0x8025}{0x8024}{0x8024}G.\nEnter a shop with {0x8025}{0x8024}{0x8024}G\nto buy weapons and items.") };

u8 trainingWeaponsPromptText[184] = { _("{0xFFFC}{0x0005}                \"Weapons\"\nEnter the Red Shop to buy\nweapons.\nPress the Z Button to\nshoot!") };

u8 trainingSpecialTechniquePromptText[180] = { _("{0xFFFC}{0x0005}Special Technique!\nHold down the Control\nStick when using the\nBomb to shoot behind you!") };

u8 trainingItemPromptText[188] = { _("{0xFFFC}{0x0005}                  \"{0xA012}tem\"\nEnter the Blue Shop to buy\nitems!\nPress the B Button to use\nitems!") };

u8 trainingCornering2PromptText[208] = { _("{0xFFFC}{0x0005}            \"Cornering {0x8026}\"\nTo make sharp turns,\nhold down the Control\nStick to the bottom\nleft/right!") };

u8 trainingGrabTrickPromptText[248] = { _("{0xFFFC}{0x0005}            \"Grab trick\"\nPress any C Button while\nin the air to do this trick!\nTap other C Buttons to\ndo multiple grabs!") };

u8 trainingGrabTrickMoneyPromptText[76] = { _("{0xFFFC}{0x0005}Do grab tricks to earn some\nmoney.") };

u8 trainingSpinningTrickPromptText[236] = { _("{0xFFFC}{0x0005}        \"Spinning trick\"\nPress and hold down\nthe A Button, then hold\ndown the Control Stick.\nRelease the A Button.") };

u8 trainingSpinningTrickMoneyPromptText[176] = { _("{0xFFFC}{0x0005}You`ll fall if you try to\nland while spinning.\nDo spinning tricks to earn\nmore money!") };

u8 trainingCombinationTricksPromptText[248] = { _("{0xFFFC}{0x0005}     \"Combination tricks\"\nPress the C Button\nwhile doing spinning tricks\nto perform combo tricks.\nYou`ll earn more money.") };

u8 trainingContinuousSpinTrickPromptText[276] = { _("{0xFFFC}{0x0005}   \"Continuous Spin trick\"\nPress the A Button and\nhold down the Control Stick.\nRelease the A Button, then\ntap the A Button repeatedly.") };

u8 trainingContinuousSpinTapPromptText[180] = { _("{0xFFFC}{0x0005}You will spin as many times\nas you tap the A Button!\nEarn more money by spinning\nmore!") };

u8 trainingSpinDirectionPromptText[204] = { _("{0xFFFC}{0x0005}You can change the\ndirection of the spin if\nyou hold down the Control\nStick in different\npositions!") };

u8 trainingWarningSignPromptText[204] = { _("{0xFFFC}{0x0005}       \"{0xFFFC}{0x0008}{0xF05D}{0xFFFC}{0x0005} Warning Sign\"\nWhen a rival`s shot\napproaches you,you`ll see\n{0xFFFC}{0x0008}{0xF05D}{0xFFFC}{0x0005} sign on the screen.") };

u8 trainingWarningSignDodgePromptText[184] = { _("{0xFFFC}{0x0005}When the {0xFFFC}{0x0008}{0xF05D}{0xFFFC}{0x0005} blinks, jump!\n{0xA012}f the timing is right,\nyou can dodge the attack\nby jumping!") };

u8 trainingLiftPromptText[260] = { _("{0xFFFC}{0x0005}                  \"Lift\"\nApproach the lift when\nyou complete a lap.\nThe lift for this course\nis located on the left-hand\nside.") };

u8 trainingAccelerationTechniquePromptText[240] = { _("{0xFFFC}{0x0005} \"Acceleration Technique\"\nWhen you hit the wall and\nslow down,\nhold the stick upward and\njump to accelerate faster.\n") };

u8 trainingMaxSpeedWarningPromptText[296] = { _("{0xFFFC}{0x0005}But if you jump too high,\na trick will be executed,\nso be careful.\nAlso, you can`t go beyond\nthe max. speed even with the\nControl Stick held up.") };

u8 trainingSessionCompletePromptText[196] = { _("{0xFFFC}{0x0005}This training session\nis now over.\nFor the second lap,\nremember what you learned\nand snowboard!") };

// clang-format on

/* Data segment - message index tables */
s8 D_800BCB60_B7440[] = { 1, -1, 0, 0 };
s8 D_800BCB64_B7444[] = { 2, -1, 0, 0 };
s8 D_800BCB68_B7448[] = { 3, -1, 0, 0 };
s8 D_800BCB6C_B744C[] = { 4, 5, -1, 0 };
s8 D_800BCB70_B7450[] = { 6, -1, 0, 0 };
s8 D_800BCB74_B7454[] = { 7, -1, 0, 0 };
s8 D_800BCB78_B7458[] = { 8, 9, -1, 0 };
s8 D_800BCB7C_B745C[] = { 10, 11, -1, 0 };
s8 D_800BCB80_B7460[] = { 12, -1, 0, 0 };
s8 D_800BCB84_B7464[] = { 13, 14, 15, -1 };
s8 D_800BCB88_B7468[] = { 16, 17, -1, 0 };
s8 D_800BCB8C_B746C[] = { 18, 19, 20, -1 };
s8 D_800BCB90_B7470[] = { 21, -1, 0, 0 };

/* Data segment - panel message table pointers */
s8 *s_trainingPanelMessageTables[] = {
    (s8 *)D_800BCB60_B7440,
    (s8 *)D_800BCB64_B7444,
    (s8 *)D_800BCB68_B7448,
    (s8 *)D_800BCB6C_B744C,
    (s8 *)D_800BCB70_B7450,
    (s8 *)D_800BCB74_B7454,
    (s8 *)D_800BCB78_B7458,
    (s8 *)D_800BCB7C_B745C,
    (s8 *)D_800BCB80_B7460,
    (s8 *)D_800BCB84_B7464,
    (s8 *)D_800BCB88_B7468,
    (s8 *)D_800BCB8C_B746C,
    (s8 *)D_800BCB90_B7470,
    NULL,
    NULL,
};

void shrinkTrainingInstructionPanelWidth(TrainingInstructionRuntimeState *arg0);

void cleanupTrainingInstructionTask(TrainingInstructionState *arg0);

void checkTrainingInstructionCheckpoint(TrainingInstructionRuntimeState *arg0);

void initTrainingInstructionTask(TrainingInstructionState *arg0) {
    getCurrentAllocation();
    arg0->textRenderContext = loadTextRenderAsset(1);
    arg0->uiAsset = loadAsset_34F7E0();
    arg0->messageData = loadCompressedData(&trainingMessageData_ROM_START, &trainingMessageData_ROM_END, 0x1130);
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
                if ((state->players->sectorIndex < 3) || (state->players->animFlags & 1)) {
                    break;
                }
                goto do_action;
            case 2:
                if (state->players->sectorIndex < 0xE) {
                    break;
                }
                goto do_action;
            case 3:
                if ((state->players->sectorIndex < 0x12) || (state->players->animFlags & 1)) {
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

    renderTiledSprite3x3(
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

    renderTiledSprite3x3(
        arg0->uiAsset,
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

    renderTiledSprite3x3(
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

void displayTrainingInstructionAndWaitForInput(TrainingInstructionRuntimeState *arg0) {
    s8 *table_ptr;
    s32 temp_v1_2;
    s16 temp_v0;
    s8 temp_v1_3;
    s32 s0_var = 6;
    s32 s1_var = 0xC;

    table_ptr = s_trainingPanelMessageTables[arg0->panelIndex];
    temp_v1_2 = arg0->messageData[table_ptr[arg0->messageIndex]];
    enqueueTextLayout(
        arg0->textRenderContext,
        (void *)arg0->messageData + temp_v1_2,
        -0x68,
        -0x30,
        0xFF,
        0xFF,
        0,
        s1_var,
        s0_var
    );

    renderTiledSprite3x3(
        arg0->uiAsset,
        -0x68,
        -0x30,
        0xD,
        s0_var,
        1,
        GET_ALPHA_COLOR_HIGH_BYTE(arg0),
        arg0->colorIndex,
        s1_var,
        s0_var
    );

    if (gControllerInputs[0] & A_BUTTON) {
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

    renderTiledSprite3x3(
        arg0->uiAsset,
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

    renderTiledSprite3x3(
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
