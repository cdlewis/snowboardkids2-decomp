#pragma once

#include "common.h"
#include "options_menu.h"
#include "gamestate.h"

void initRace(void);
void stopSoundEffectChannel(s32 channelIndex, s32 stoppingSpeed);
void playFanSoundEffect(s16 fanIndex, s16 volume, s16 pan, s16 channel);
void initOptionsMenuCursors(OptionsMenuLabelIconEntry *arg0);
void playSoundEffect(s32 soundId);
