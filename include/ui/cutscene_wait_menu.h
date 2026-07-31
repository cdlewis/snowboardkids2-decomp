#pragma once

#include "animation/easing_state.h"
#include "common.h"
#include "cutscene/cutscene_manager.h"
#include "text/text_layout.h"

typedef struct {
    /* 0x00 */ CutsceneManager *cutsceneManager;
    /* 0x04 */ void *panelSpriteAsset;
    /* 0x08 */ Table_B934 *textTable;
    /* 0x0C */ s16 state;
    /* 0x0E */ s16 waitFrameCount;
    /* 0x10 */ s16 textColumn;
    /* 0x12 */ u8 pad12[0x2];
    /* 0x14 */ s16 textX;
    /* 0x16 */ s16 textY;
    /* 0x18 */ void *textData;
    /* 0x1C */ void *activeTextRenderAsset;
    /* 0x20 */ TextLayoutColorValue textAlpha;
    /* 0x22 */ TextLayoutColorValue textTransparency;
    /* 0x24 */ u8 textPaletteIndex;
    /* 0x25 */ u8 pad25[0x3];
    /* 0x28 */ void *textRenderAsset;
    /* 0x2C */ s32 panelX;
    /* 0x30 */ s32 panelY;
    /* 0x34 */ s32 panelWidth;
    /* 0x38 */ s32 panelHeight;
    /* 0x3C */ s16 panelRed;
    /* 0x3E */ s16 panelGreen;
} CutsceneWaitMenuState;

void initCutsceneWaitMenu(CutsceneWaitMenuState *state);
