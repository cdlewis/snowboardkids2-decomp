#pragma once

#include "common.h"
#include "1DFAA0.h"

void scheduleRepeatingFanSoundEffect(s16 fanIndex, s16 volume, s16 pan, s16 repeatCount, s16 interval, s16 minInterval);
void scheduleRepeatingFanSoundAtPosition(s16 fanIndex, s16 volume, s16 repeatCount, s16 pan, s16 interval, CutsceneSlotData *cutsceneSlotData);
