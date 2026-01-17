#pragma once

#include "common.h"
#include "gamestate.h"
#include "geometry.h"

void spawnCharacterTrailParticle(void *arg0);
void spawnPlayerCharacterTrailParticle(Player *player, s32 characterId);
void spawnImpactStar(Vec3i *arg0);
void updateCharacterAttackEffect(void);
void spawnCharacterAttackEffect(Player *player);
void spawnCharacterAttackEffectByType(void *arg0, s32 characterId);
void spawnGlintEffect(void *arg0);
void spawnDualSnowSprayEffect(Vec3i *pos1, Vec3i *pos2, Vec3i *velocity, s16 slotIndex, s32 characterId);
void spawnDualSnowSprayEffect_SingleSlot(Vec3i *pos1, Vec3i *pos2, Vec3i *velocity, s32 particleType);
void spawnSprayEffect(Vec3i *arg0, Vec3i *arg1, s32 arg2);
