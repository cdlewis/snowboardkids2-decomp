#pragma once

#include "common.h"

void initStoryMode(void);
u8 getStoryMapCameraMode(void);
void setStoryMapCameraMode(s32 arg0);
void updateStorySlotUnlockStatus(void);
void clearMemory(s8 *dest, u16 size);
