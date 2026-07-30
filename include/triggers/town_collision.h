#pragma once

#include "common.h"
#include "math/geometry.h"
#include "story/map_character_anim.h"

s32 checkTownPlayerCollision(s32 posX, s32 posZ, u8 characterIndex);
s32 checkTownLamppostCollision(s32 posX, s32 posZ, s16 collisionRadius);
void resolveTownLamppostCollision(Vec3i *position, s16 collisionRadius, s32 lamppostIndex);
s32 checkTownNPCCollision(s32 posX, s32 posZ, s32 characterIndex);
void resolveTownNPCCollision(StoryMapCameraState *camera, Vec3i *position, s32 characterIndex);
