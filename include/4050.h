#pragma once

#include "common.h"

void* __udiv_w_sdiv(void);
void awaitCutsceneTransitionComplete(void);
void beginCutsceneSequence(void);
void loadCutsceneOverlay(void);
void runCutsceneFrame(void);
void setCutsceneSelection(s16 slotIndex, s16 cutsceneType);
void setCutsceneParameters(s16 slotIndex, s16 cutsceneType, s16 frameIndex);
