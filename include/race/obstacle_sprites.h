#pragma once

#include "common.h"

s32 spawnAttackProjectile(s32 projectileType, s32 playerIdx, s32 arg2);
void spawnFallingStarProjectile(s16 angle, s32 speed);
void *spawnShrinkProjectileTask(s32, s16, void *);
s32 spawnPlayerGuidedStarProjectile(s16 playerIdx);
s32 spawnRandomEffectProjectile(s32 playerIdx);
