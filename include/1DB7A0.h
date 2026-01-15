#pragma once

#include "common.h"

typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ void *spriteData;
    /* 0x08 */ s16 frameIndex;
    /* 0x0A */ s16 scaleX;
    /* 0x0C */ s16 scaleY;
    /* 0x0E */ s16 flags;
    /* 0x10 */ s16 alpha;
    /* 0x12 */ u8 unk12;
    /* 0x13 */ u8 unk13;
} PlayerSelectSprite; // size 0x14

typedef struct {
    /* 0x00 */ PlayerSelectSprite sprites[2];
    /* 0x28 */ u8 animState;
    /* 0x29 */ u8 animCounter;
    /* 0x2A */ u8 playerIndex;
    /* 0x2B */ u8 slotIndex;
    /* 0x2C */ u8 unk2C;
    /* 0x2D */ u8 unk2D;
} PlayerSelectState; // size 0x2E

typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ void *spriteData;
    /* 0x08 */ s16 frameIndex;
    /* 0x0A */ s16 alpha;
    /* 0x0C */ u8 slotIndex;
    /* 0x0D */ u8 playerIndex;
    /* 0x0E */ char pad[2];
} PlayerSelectSpriteTask; // size 0x10

void initPlayerSelectSprites(PlayerSelectState *state);
void enqueuePlayerSelectSpritesRender(PlayerSelectSpriteTask *arg0);
void cleanupPlayerCountSelectSprites(PlayerSelectSpriteTask *arg0);
void initPlayerCountHeaderSprite(PlayerSelectSpriteTask *arg0);
void updatePlayerCountHeaderSprite(PlayerSelectSpriteTask *arg0);
void func_800B07A0_1DBE80(PlayerSelectSpriteTask *arg0);
