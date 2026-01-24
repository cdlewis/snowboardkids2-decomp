#pragma once

#include "common.h"
#include "1DFAA0.h"

void playFanSoundAtPosition(s16 fanIndex, s16 arg1, s16 duration, CutsceneSlotData *cutsceneSlotData);
void playFanSoundOnChannel0(void);
void stopFanSoundOnChannel0(void);
void spawnFanEffect(SceneModel *, s16);
