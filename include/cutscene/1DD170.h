#include "common.h"
#include "cutscene/cutscene_manager.h"

s32 getCategorySkipValue(u8 categoryIndex);
s32 initializeSlotState(StateEntry *state, CutsceneManager *cutsceneManager, s8 slotIndex);
s32 updateSlotData(CutsceneManager *cutsceneManager, s8 slotIndex);