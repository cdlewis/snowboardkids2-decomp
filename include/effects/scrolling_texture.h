#pragma once

#include "common.h"

typedef struct {
    s8 isComplete;
    s8 activeLayerCount;
    s8 layersRemaining;
    s8 fadeOutTriggered;
} ScreenTransitionState;

void initScreenTransition(ScreenTransitionState *state);
