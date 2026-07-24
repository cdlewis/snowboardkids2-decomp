#pragma once

#include "common.h"
#include "cutscene/cutscene_manager.h"

typedef struct CurrentCommand CurrentCommand;
typedef struct CommandData CommandData;

s32 getCategorySkipValue(u8 categoryIndex);
s32 initializeSlotState(StateEntry *state, CutsceneManager *cutsceneManager, s8 slotIndex);
s32 updateSlotData(CutsceneManager *cutsceneManager, s8 slotIndex);

void initializeCutsceneCommand(
    CurrentCommand *currentCommand,
    CommandData *commandData,
    u8 commandCategory,
    u8 commandIndex,
    s8 frameIndex
);
