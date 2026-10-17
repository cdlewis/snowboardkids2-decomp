#pragma once

#include "common.h"

void onSaveSlotReadComplete(u16, s32);
void updateSlotSelectionSlide(void);
void updateSaveSlotRumblePakDialogWiggle(void);
void cleanupSaveSlotSelectionAndExit(void);
void verifySaveSlotChecksum(void);
