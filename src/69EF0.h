#include "common.h"
#include "gamestate.h"
#include "memory_allocator.h"

typedef struct Node {
    /* 0x00 */ struct Node *prev;
    /* 0x04 */ struct Node *next;
    /* 0x08 */ struct Node *freeNext;
    /* 0x0C */ u8 field_C;
    /* 0x0D */ u8 field_D;
    /* 0x0E */ u8 field_E;
    /* 0x0F */ u8 priority;
    /* 0x10 */ u8 pad10;
    /* 0x11 */ u8 field_11;
    /* 0x12 */ u8 pad12[6];
    /* 0x18 */ u32 field_18;
    /* 0x1C */ u32 pad1C;
    /* 0x20 */ void *field_20;
    /* 0x24 */ u32 field_24;
} Node;

void setGameStateHandler(void *arg0);

MemoryAllocatorNode *dmaRequestAndUpdateStateWithSize(void *, void *, s32);
MemoryAllocatorNode *dmaRequestAndUpdateState(void *start, void *end);

GameState *allocateGameStateMemory(s32 arg0);

Node *scheduleTask(void *a0, u8, u8, u8);