#pragma once

#include "common.h"
#include "gamestate.h"
#include "288A0.h"

typedef struct {
    Node_70B00 unk0;
    u16 waitCounter;
} PostUnlockLocationIntroAllocation;

void initPostUnlockLocationIntro(void);
void postUnlockLocationWaitForDismiss(void);
void onPostUnlockLocationIntroComplete(void);
