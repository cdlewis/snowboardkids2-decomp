#pragma once

#include "common.h"
#include "graphics/graphics.h"

typedef enum {
    PLAYER_COUNT_MENU_SELECTING = 0,
    PLAYER_COUNT_MENU_CONFIRM_WAIT = 1,
    PLAYER_COUNT_MENU_CONFIRM_OK = 2,
    PLAYER_COUNT_MENU_CANCEL_OK = 10,
} PlayerCountMenuState;

typedef enum {
    PLAYER_COUNT_RESULT_NONE = 0,
    PLAYER_COUNT_RESULT_PROCEED = 1,
    PLAYER_COUNT_RESULT_CANCEL = 0x63,
} PlayerCountMenuResult;

typedef struct {
    /* 0x00 */ ViewportNode viewport;
    /* 0x30 */ void *portraitSpriteData;
    /* 0x34 */ void *promptSpriteData;
    /* 0x38 */ s16 frameCounter;
    union {
        u16 selectedOptionIndex;
        struct {
            u8 padding;
            u8 selectedOptionIndex;
        } bytes;
    } playerCount;
    /* 0x3C */ u8 connectedControllerCount;
    /* 0x3D */ u8 menuResult;
    /* 0x3E */ u8 menuState;
} PlayerCountSelectState;

void initPlayerCountSelectState(void);
