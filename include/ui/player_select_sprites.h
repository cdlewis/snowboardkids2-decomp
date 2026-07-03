#pragma once

#include "common.h"
#include "graphics/sprite_rdp.h"

typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ SpriteSheetData *spriteData;
    /* 0x08 */ s16 frameIndex;
    /* 0x0A */ u16 renderWidth;
    /* 0x0C */ u16 renderHeight;
    /* 0x0E */ s16 flags;
    /* 0x10 */ s16 shade;
    /* 0x12 */ u8 tileMode;
    /* 0x13 */ u8 overridePaletteCount;
} PlayerSelectPortraitSprite;

typedef struct {
    /* 0x00 */ PlayerSelectPortraitSprite sprites[2];
    /* 0x28 */ u8 selectedAnimFrame;
    /* 0x29 */ u8 selectedAnimFrameTimer;
    /* 0x2A */ u8 lastSelectedPlayerIndex;
    /* 0x2B */ u8 slotIndex;
    /* 0x2C */ u8 phase;
    /* 0x2D */ u8 phaseTimer;
} PlayerSelectState;

typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ void *spriteData;
    /* 0x08 */ s16 frameIndex;
    /* 0x0A */ s16 alpha;
    /* 0x0C */ u8 slotIndex;
    /* 0x0D */ u8 playerIndex;
    /* 0x0E */ char pad[2];
} PlayerSelectSpriteTask;

void initPlayerSelectSprites(PlayerSelectState *state);
void enqueuePlayerSelectSpritesRender(PlayerSelectSpriteTask *arg0);
void cleanupPlayerCountSelectSprites(PlayerSelectSpriteTask *arg0);
void initPlayerCountHeaderSprite(PlayerSelectSpriteTask *arg0);
void updatePlayerCountHeaderSprite(PlayerSelectSpriteTask *arg0);
void initPlayerIndicatorSprite(PlayerSelectSpriteTask *arg0);
void updatePlayerIndicatorSprite(PlayerSelectSpriteTask *arg0);
void cleanupPlayerCountHeaderSprite(PlayerSelectSpriteTask *arg0);
void cleanupPlayerIndicatorSprite(PlayerSelectSpriteTask *arg0);
void initCharacterReadyIndicator(PlayerSelectSpriteTask *arg0);
void updateCharacterReadyIndicator(void *arg0);
void cleanupCharacterReadyIndicator(PlayerSelectSpriteTask *arg0);
void initPlayerCountSelectSprites(PlayerSelectSpriteTask *arg0);
void updatePlayerSelectAnim(PlayerSelectState *);
