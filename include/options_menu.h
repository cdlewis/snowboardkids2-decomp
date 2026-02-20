#pragma once

#include "common.h"

typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ void *textData;
    /* 0x08 */ void *textAsset;
    /* 0x0C */ s16 primaryColor;
    /* 0x0E */ s16 secondaryColor;
    /* 0x10 */ u8 textStyle;
    /* 0x11 */ u8 pad11[3];
} OptionsMenuTitleTextEntry;

typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ void *spriteAsset;
    /* 0x08 */ s16 frameIndex;
    /* 0x0A */ s16 alpha;
    /* 0x0C */ u8 blinkState;
    /* 0x0D */ u8 unkD;
    /* 0x0E */ u8 padE[2];
} OptionsMenuTitleIconEntry;

typedef struct {
    /* 0x00 */ OptionsMenuTitleTextEntry titleText;
    /* 0x14 */ OptionsMenuTitleIconEntry leftIcon;
    /* 0x24 */ OptionsMenuTitleIconEntry rightIcon;
} OptionsMenuTitleState;

typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ void *spriteAsset;
    /* 0x08 */ s16 frameIndex;
    /* 0x0A */ s16 highlightValue;
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 alpha;
    /* 0x0E */ u8 padE[2];
} OptionsMenuLabelIconEntry;

typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ void *textData;
    /* 0x08 */ void *textAsset;
    /* 0x0C */ s16 highlight;
    /* 0x0E */ s16 alpha;
    /* 0x10 */ u8 textStyle;
    /* 0x11 */ u8 pad11[3];
} OptionsMenuLabelTextEntry;

typedef struct {
    /* 0x00 */ OptionsMenuLabelIconEntry iconEntries[4];
    /* 0x40 */ OptionsMenuLabelTextEntry textEntries[4];
    /* 0x90 */ void *textRenderAsset;
} OptionsMenuLabelsState;

void initOptionsMenuTitle(OptionsMenuTitleState *arg0);
void initOptionsMenuToggles(void *);
void initOptionsMenuLabels(OptionsMenuLabelsState *arg0);
