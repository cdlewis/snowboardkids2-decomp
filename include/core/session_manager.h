#pragma once

#include "common.h"

typedef struct {
    /* 0x0 */ s16 state;
    /* 0x2 */ u8 substate;
    /* 0x3 */ u8 pad3;
} GameModeTaskState;

void initStoryMode(void);
u8 getStoryMapCameraMode(void);
void setStoryMapCameraMode(s32 arg0);
void updateStorySlotUnlockStatus(void);
void clearMemory(s8 *dest, u16 size);
