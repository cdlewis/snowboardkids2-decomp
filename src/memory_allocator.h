#ifndef _MEMORY_ALLOCATOR_H_
#define _MEMORY_ALLOCATOR_H_

#include "common.h"

typedef struct MemoryAllocatorNode {
    /* 0x0 */ struct MemoryAllocatorNode *prev;
    /* 0x4 */ struct MemoryAllocatorNode *next;
    /* 0x8 */ s32 ownerID;
    s32 unk_0C;
    /* 0x10 */ s32 size;
    s32 unk_14;
    s32 unk_18;
    s32 unk1C;
} MemoryAllocatorNode;

extern MemoryAllocatorNode *gMemoryAllocatorHead;
extern MemoryAllocatorNode gMemoryHeapBase;

// allocates memory, returning a pointer to the start of the allocated space.
void *allocateMemoryNode(s32, u32, u8 *);

void *getNodeSequenceNumber(void *);
void setNodeSequenceNumber(void *, void *);
void markNodeAsLocked(s32 *);
s32 getNodeOwner(void *);
s32 decrementNodeRefCount(s32 *);
void unlockNodeWithInterruptDisable(s32 *);
void initializeMemoryAllocatorRegion(void);
void cleanupUnusedNodes(void);

#endif