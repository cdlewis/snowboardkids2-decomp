#pragma once

#include "common.h"

typedef struct {
    u8 padding[0x20];
    void *model;
} StoryMapDecorModelState;

void storyMapDecorModelTask(StoryMapDecorModelState *arg0);
void func_80019D50_1A950(void);