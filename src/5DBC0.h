#pragma once

#include "common.h"
#include "gamestate.h"

s32 getTrackHeightInSector(void *gameData, s32 sectorIndex, void *pos, s32 heightOffset);
s32 getTrackHeightInSectorWithOffset(void *gameData, u16 sectorIndex, void *pos, s32 unused, s32 heightOffset);
void func_8005CFFC_5DBFC(void *arg0, u16 arg1, void *arg2, void *arg3, void *arg4);
s32 getSectorTrackHeight(void *gameData, u16 sectorIndex, void *pos, s32 defaultHeight);
void clampPlayerVelocityToMaxSpeed(Player *player);
s32 func_8005D180_5DD80(void *arg0, s32 arg1);
s32 func_8005D308_5DF08(void *arg0, s32 arg1);
void func_8005D48C_5E08C(Player *, s32, s32);
void loadCharacterBodyParts(Player *player);
void func_8005D804_5E404(Player *arg0, u8 arg1, u8 arg2);
s16 func_8005D810_5E410(Player *);
s16 func_8005D818_5E418(Player *);
s32 func_8005D8C8_5E4C8(Player *arg0);
