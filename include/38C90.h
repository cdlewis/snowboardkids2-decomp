#pragma once

#include "common.h"

void processRaceUnlocks(s16 raceResult);
s32 areCharacterPalettesUnlocked(s32 count);
s32 tryAddUnlockedCutsceneId(u8 cutsceneId);
void tryAddUnlockedBoardId(u8 boardId);
void renderTiledTexture(void *);
void initTiledTextureRenderState(void *, s32);
