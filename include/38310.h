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
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
    s16 unkA;
    u8 unkC;
    u8 unkD;
    u8 padE[2];
} Entry1;

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    void *unk8;
    s16 unkC;
    s16 unkE;
    u8 unk10;
    u8 pad11[3];
} Entry2;

typedef struct {
    Entry1 entries1[4];
    Entry2 entries2[4];
    void *unk90;
} func_80037BFC_387FC_arg;

void initOptionsMenuTitle(OptionsMenuTitleState *arg0);
void initOptionsMenuToggles(void *);
void func_80037BFC_387FC(func_80037BFC_387FC_arg *arg0);
