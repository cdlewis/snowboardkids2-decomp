#pragma once

#include "common.h"
#include "38310.h"
#include "gamestate.h"

void initRace(void);
void stopSoundEffectChannel(s32 channelIndex, s32 stoppingSpeed);
void func_800B4B30_1E1BE0(s16 arg0, s16 arg1, s16 arg2, s16 arg3);
void initOptionsMenuCursors(OptionsMenuLabelIconEntry *arg0);
void playSoundEffect(s32 soundId);
