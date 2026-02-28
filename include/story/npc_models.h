#pragma once

#include "common.h"

typedef struct {
    void *model;
    u8 pad4[0x56];
    s16 respawnTimer;
    u8 eventTypeIndex;
    u8 itemType;
} StoryMapItem;

void initStoryMapItem(StoryMapItem *arg0);
