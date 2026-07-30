#pragma once

#include "common.h"
#include "graphics/graphics.h"
#include "graphics/sprite_rdp.h"
#include "text/text_layout.h"

typedef struct {
    /* 0x00 */ TextLayoutArg titleText;
    /* 0x14 */ TextRenderArg leftIcon;
    /* 0x24 */ TextRenderArg rightIcon;
} OptionsMenuTitleState;

typedef struct {
    /* 0x00 */ TextRenderArg optionIcons[4];
    /* 0x40 */ TextLayoutArg optionLabels[4];
    /* 0x90 */ void *textRenderAsset;
} OptionsMenuLabelsState;

typedef struct {
    /* 0x00 */ TextRenderArg toggleIcons[6];
    /* 0x60 */ TextLayoutArg toggleLabels[6];
    /* 0xD8 */ void *textRenderAsset;
} OptionsMenuToggleState;

enum OptionsMenuPhase {
    OPTIONS_MENU_SELECTING,
    OPTIONS_MENU_EXIT_DELAY,
};

typedef struct {
    /* 0x000 */ ViewportNode viewport;
    /* 0x1D8 */ void *menuSpriteAsset;
    /* 0x1DC */ void *textRenderAsset;
    /* 0x1E0 */ u16 exitBlinkTimer;
    /* 0x1E2 */ u16 phase;
    /* 0x1E4 */ u8 highlightTimers[4];
    /* 0x1E8 */ u8 highlightAlphas[4];
    /* 0x1EC */ u8 selectedOption;
} OptionsMenuState;

void initOptionsMenuTitle(OptionsMenuTitleState *arg0);
void initOptionsMenuToggles(OptionsMenuToggleState *arg0);
void initOptionsMenuLabels(OptionsMenuLabelsState *arg0);
void initOptionsMenuCursors(TextRenderArg *arg0);
