#pragma once

#include "common.h"
#include "geometry.h"
#include "20F0.h"
#include "297B0.h"

typedef struct Func2E024Arg Func2E024Arg;
struct Func2E024Arg {
    /* 0x00 */ Func297D8Arg elements[2];          /* 2 * 0x64 = 0xC8 */
    /* 0xC8 */ void (*callback)(Func2E024Arg *);
    /* 0xCC */ u16 unkCC[2];
    /* 0xD0 */ u8 ctrl[2];
    /* 0xD2 */ u8 unkD2;
    /* 0xD3 */ u8 unkD3;
    /* 0xD4 */ u8 unkD4;
    /* 0xD5 */ u8 unkD5;
};

void updateStoryMapRareEventMagicShow(Func2E024Arg *);
void prepareStoryMapNpcDialogue(void *);
s32 getPlayerGold(void);
void addPlayerGold(s32 amount);