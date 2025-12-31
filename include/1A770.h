#pragma once

#include "common.h"

typedef struct {
    u8 padding[0x20];
    void *model;
} StoryMapDecorModelState;

void storyMapDecorModelTask(StoryMapDecorModelState *arg0);
void initStoryModeRace(void);