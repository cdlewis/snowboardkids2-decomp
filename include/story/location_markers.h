#pragma once

#include "common.h"

typedef struct {
    u8 padding0[0x34];
    s32 positionX;
    u8 padding1[0x4];
    s32 positionZ;
} StoryMapPlayerState;

typedef struct {
    u8 baseLocationIndex;
    u8 padding;
} StoryMapSpecialLocationTriggerState;

void initDiscoveryDisplaySystem(s8 *arg0);
void initTownExitTrigger(void *);
s32 checkStoryMapLocationSelection(StoryMapPlayerState *);
