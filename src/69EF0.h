#include "common.h"
#include "gamestate.h"
#include "memory_allocator.h"

void setGameStateHandler(void *arg0);

MemoryAllocatorNode *dmaRequestAndUpdateStateWithSize(void *, void *, s32);
MemoryAllocatorNode *dmaRequestAndUpdateState(void *start, void *end);

GameState *allocateGameStateMemory(s32 arg0);

s32 *scheduleTask(void *a0, u8, u8, u8);