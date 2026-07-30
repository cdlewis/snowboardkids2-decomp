#pragma once

#include "credits/credits_state.h"

typedef enum {
    CREDITS_TEXT_COMMAND_SELECT_PALETTE = 0,
    CREDITS_TEXT_COMMAND_SET_OVERLAY_FADE = 1,
    CREDITS_TEXT_COMMAND_SET_PALETTE_FADE = 2,
    CREDITS_TEXT_COMMAND_SELECT_TEXT_ROW = 3,
    CREDITS_TEXT_COMMAND_SET_TEXT_FADE = 4,
} CreditsTextCommandType;

typedef struct {
    /* 0x0 */ u16 triggerFrame;
    /* 0x2 */ s8 commandType;
    /* 0x3 */ u8 parameter;
} CreditsTextCommand;

void initCreditsScrollingTextEffects(CreditsState *state);
void updateCreditsScrollingTextEffects(CreditsState *state);
