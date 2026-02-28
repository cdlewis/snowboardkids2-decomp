#pragma once

#include "ui/level_preview_3d.h"
#include "story/npc_dialogue.h"
#include "common.h"
#include "math/geometry.h"

typedef struct Func2E024Arg Func2E024Arg;
struct Func2E024Arg {
    /* 0x00 */ Func297D8Arg elements[2]; /* 2 * 0x64 = 0xC8 */
    /* 0xC8 */ void (*callback)(Func2E024Arg *);
    /* 0xCC */ u16 unkCC[2];
    /* 0xD0 */ u8 ctrl[2];
    /* 0xD2 */ u8 unkD2;
    /* 0xD3 */ u8 unkD3;
    /* 0xD4 */ u8 unkD4;
    /* 0xD5 */ u8 unkD5;
};

void updateStoryMapRareEventMagicShow(Func2E024Arg *);
void initStoryMapRareEventWave(Func2E024Arg *);
void prepareStoryMapNpcDialogue(void *);
s32 getPlayerGold(void);
void addPlayerGold(s32 amount);