#pragma once

#include "1DFAA0.h"
#include "1E2BE0.h"
#include "common.h"

typedef struct {
    /* 0x00 */ u8 _pad00[0x10];
    /* 0x10 */ s16 xPos;
    /* 0x12 */ s16 yPos;
    /* 0x14 */ s16 width;
    /* 0x16 */ s16 height;
    /* 0x18 */ u8 _pad18[0x14];
} TiledTextureElement;

typedef struct {
    /* 0x00 */ CutsceneManager *cutsceneManager;
    /* 0x04 */ void **loadedAssets;
    /* 0x08 */ s32 scrollSpeedParam;
    /* 0x0C */ s32 scrollOffset;
    /* 0x10 */ s32 yPos;
    /* 0x14 */ TiledTextureElement elements[4];
    /* 0xC4 */ u8 groupIndex;
    /* 0xC5 */ s8 initialized;
} TiledTextureTaskData;

void processRaceUnlocks(s16 raceResult);
s32 areCharacterPalettesUnlocked(s32 count);
s32 tryAddUnlockedCutsceneId(u8 cutsceneId);
void tryAddUnlockedBoardId(u8 boardId);
void renderTiledTexture(void *);
void initTiledTextureRenderState(void *, s32);
void updateTiledTextureAssetDisplay(TiledTextureTaskData *taskData);
