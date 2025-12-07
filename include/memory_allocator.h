#ifndef _MEMORY_ALLOCATOR_H_
#define _MEMORY_ALLOCATOR_H_

#include "common.h"

typedef struct MemoryAllocatorNode {
    /* 0x00 */ struct MemoryAllocatorNode *prev;
    /* 0x04 */ struct MemoryAllocatorNode *next;
    /* 0x08 */ s32 ownerID;
    /* 0x0C */ s32 refCount;
    /* 0x10 */ s32 size;
    s32 unk_14;
    /* 0x18 */ s32 cleanupTimestamp;
    s32 unk1C;
} MemoryAllocatorNode;

extern MemoryAllocatorNode *gMemoryAllocatorHead;
extern MemoryAllocatorNode gMemoryHeapBase;

// allocates memory, returning a pointer to the start of the allocated space.
void *allocateMemoryNode(s32, u32, u8 *);

void *getNodeSequenceNumber(void *);
void setNodeSequenceNumber(void *, void *);
void markNodeAsLocked(void *);
s32 getNodeOwner(void *);
void *decrementNodeRefCount(void *node);
void unlockNodeWithInterruptDisable(s32 *);
void initializeMemoryAllocatorRegion(void);
void cleanupUnusedNodes(void);

#endif