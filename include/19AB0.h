#ifndef _19AB0_H_
#define _19AB0_H_

#include "common.h"

typedef struct {
    s16 unk0;
    u8 fromGameState;
    u8 unk3;
    u8 previousSaveSlot;
    u8 unk5;
} StoryMapStateData;

void initStoryMapState(void);

#endif
