#pragma once

#include "common.h"
#include "graphics/sprite_rdp.h"

typedef struct {
    /* 0x00 */ ScaledSpriteArg portraits[2];
    /* 0x28 */ u8 selectedAnimFrame;
    /* 0x29 */ u8 selectedAnimFrameTimer;
    /* 0x2A */ u8 lastSelectedOptionIndex;
    /* 0x2B */ u8 playerCountOptionIndex;
    /* 0x2C */ u8 phase;
    /* 0x2D */ u8 activationDelayTimer;
} PlayerSelectState;

void initPlayerSelectSprites(PlayerSelectState *state);
void cleanupPlayerSelectTask(PlayerSelectState *state);
void enqueuePlayerSelectSpritesRender(TextRenderArg *sprites);
void cleanupPlayerCountSelectSprites(TextRenderArg *sprites);
void initPlayerCountHeaderSprite(SpriteRenderArg *sprite);
void updatePlayerCountHeaderSprite(SpriteRenderArg *sprite);
void initPlayerIndicatorSprite(SpriteRenderArg *sprite);
void updatePlayerIndicatorSprite(SpriteRenderArg *sprite);
void cleanupPlayerCountHeaderSprite(SpriteRenderArg *sprite);
void cleanupPlayerIndicatorSprite(SpriteRenderArg *sprite);
void initCharacterReadyIndicator(SpriteRenderArg *sprite);
void updateCharacterReadyIndicator(SpriteRenderArg *sprite);
void cleanupCharacterReadyIndicator(SpriteRenderArg *sprite);
void initPlayerCountSelectSprites(TextRenderArg *sprites);
void updatePlayerSelectAnim(PlayerSelectState *);
