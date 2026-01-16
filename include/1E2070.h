#pragma once

#include "common.h"

void spawnTrickSpriteEffect(void *model, s16 effectParam);
void spawnScrollingTextureEffect(u16 renderLayer, void *transformMatrix, s16 colorIndex, s16 effectMode);
void spawnTrickBurstEffect(u16 renderLayer, void *transformMatrix, s16 colorIndex, s32 targetScale);
