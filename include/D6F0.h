#pragma once

#include "common.h"
#include "geometry.h"

void *loadSpriteEffectTextureData(void);
void *loadSpriteEffectModelData(void);
void *freeSpriteEffectTextureData(void *data);
void *freeSpriteEffectModelData(void *data);
void scheduleSpriteEffectTask(void *startPos, void *endPos, Vec3i *velocity, s32 textureIndex);