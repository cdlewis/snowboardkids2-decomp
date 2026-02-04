#pragma once

#include "common.h"
#include "displaylist.h"
#include "gamestate.h"

s32 getTrackHeightInSector(void *gameData, s32 sectorIndex, void *pos, s32 heightOffset);
s32 getTrackHeightInSectorWithOffset(void *gameData, u16 sectorIndex, void *pos, s32 unused, s32 heightOffset);
void func_8005CFFC_5DBFC(void *arg0, u16 arg1, void *arg2, void *arg3, void *arg4);
s32 getSectorTrackHeight(TrackGeometryFaceData *geom, u16 groupIdx, Vec3i *pos, s32 yOffset);
void clampPlayerVelocityToMaxSpeed(Player *player);
s32 advancePlayerLeanAnimationAuto(void *player, s16 animIndex);
s32 advancePlayerLeanAnimation(void *player, s16 animIndex);
void func_8005D48C_5E08C(Player *, s32, s32);
void setPlayerLeanAnimation(Player *player, s32 animIndex, s32 progress);
void loadCharacterBodyParts(Player *player);
void setPlayerBodyPartAnimState(Player *player, u8 animIndex, u8 animFlags);
s16 onTrickCompletedHook(Player *player);
s16 getTrickType(Player *player);
s32 getTrickRecoveryTimerReduction(Player *player);
