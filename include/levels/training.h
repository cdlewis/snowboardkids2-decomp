#pragma once

#include "common.h"

typedef union {
    s16 value;
    struct {
        u8 high;
        u8 low;
    } bytes;
} TrainingPanelColor;

typedef struct {
    /* 0x00 */ void *panelSpriteAsset;
    /* 0x04 */ void *textRenderAsset;
    /* 0x08 */ s32 *messageData;
    /* 0x0C */ s16 instructionIndex;
    /* 0x0E */ s16 messageIndex;
    /* 0x10 */ s16 panelWidth;
    /* 0x12 */ s16 panelHeight;
    /* 0x14 */ TrainingPanelColor panelRed;
    /* 0x16 */ TrainingPanelColor panelGreen;
} TrainingInstructionTask;

void initTrainingInstructionTask(TrainingInstructionTask *task);
