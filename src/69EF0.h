#include "common.h"
#include "gamestate.h"

void setGameStateHandler(void *arg0);

void *dmaRequestAndUpdateStateWithSize(void *, void *, s32);
void *dmaRequestAndUpdateState(void *start, void *end);

GameState *allocateGameStateMemory(s32 arg0);

s32 *scheduleTask(void *a0, u8, u8, u8);