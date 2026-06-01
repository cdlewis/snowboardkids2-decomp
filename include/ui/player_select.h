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
    ViewportNode node;
    void *assetData1;
    void *assetData2;
    s16 frameCounter;
    union {
        u16 selectedPlayerIndex;
        struct {
            u8 selectedPlayerIndexHi;
            u8 selectedPlayerIndexLo;
        } bytes;
    } playerCount;
    u8 connectedControllerCount;
    u8 menuResult;
    u8 menuState;
} PlayerCountSelectState;

void initPlayerCountSelectState(void);
