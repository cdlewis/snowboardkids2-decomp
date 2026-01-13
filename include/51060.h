#pragma once

#include "common.h"
#include "gamestate.h"
#include "geometry.h"

void spawnCharacterTrailParticle(void *arg0);
void spawnPlayerCharacterTrailParticle(Player *player, s32 characterId);
void spawnImpactStar(Vec3i *arg0);
void func_80051978_52578(void);
void func_80051BB8_527B8(void *arg0);
void func_80051C08_52808(void *arg0, s32 arg1);
void func_8005182C_5242C(void *arg0);
void spawnDualSnowSprayEffect(Vec3i *pos1, Vec3i *pos2, Vec3i *velocity, s16 slotIndex, s32 characterId);
void spawnDualSnowSprayEffect_SingleSlot(Vec3i *pos1, Vec3i *pos2, Vec3i *velocity, s32 particleType);
void spawnSprayEffect(Vec3i *arg0, Vec3i *arg1, s32 arg2);
