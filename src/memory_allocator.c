#include "memory_allocator.h"

#include "common.h"

extern MemoryAllocatorNode gMemoryHeapEnd;
extern s32 gFrameCounter;
extern s32 gBufferedFrameCounter;

void initializeMemoryAllocatorRegion(void) {
    s32 i;
    u8 *current_block;

    gMemoryAllocatorHead = NULL;
    current_block = (u8 *)&gMemoryHeapBase;
    i = 0;
    do {
        *current_block = 0;
        i += 1;
        current_block += 1;
    } while (i <= 0x1FFFFF); // 2mb
}

void *allocateMemoryNode(s32 ownerID, u32 requestedSize, u8 *nodeExists) {
    s32 space_between;
    s32 needed_space;
    MemoryAllocatorNode *new_node;
    MemoryAllocatorNode *node;
    s32 size;

    *nodeExists = FALSE;
    requestedSize = (((requestedSize + 0xF) >> 4) << 4) + 0x20;

    if (ownerID != 0) {
        for (node = gMemoryAllocatorHead; node != NULL; node = node->next) {
            if (node->ownerID == ownerID) {
                s32 node_size = node->size;
                node->refCount++;
                if (node_size != requestedSize) {
                    return NULL;
                }

                *nodeExists = TRUE;
                return ++node;
            }
        }
    }

    if (gMemoryAllocatorHead == NULL) {
        if (0x200000 < requestedSize) {
            return NULL;
        }

        gMemoryAllocatorHead = &gMemoryHeapBase;
        gMemoryHeapBase.prev = 0;
        gMemoryHeapBase.refCount = 1;
        gMemoryHeapBase.next = NULL;
        gMemoryHeapBase.ownerID = ownerID;
        gMemoryHeapBase.size = requestedSize;
        gMemoryHeapBase.unk_14 = 0;
        gMemoryHeapBase.cleanupTimestamp = -1;
        return (&gMemoryHeapBase) + 1;
    }

    for (node = gMemoryAllocatorHead; node->next != 0; node = node->next) {
        MemoryAllocatorNode *next_node;
        u32 needed_space = requestedSize + node->size;
        space_between = ((s32)node->next) - ((s32)node);
        if (space_between >= requestedSize + node->size) {
            new_node = (MemoryAllocatorNode *)(((s8 *)node) + node->size);
            new_node->prev = node;
            next_node = node->next;
            new_node->refCount = 1;
            new_node->ownerID = ownerID;
            new_node->size = requestedSize;
            new_node->unk_14 = 0;
            new_node->cleanupTimestamp = -1;
            new_node->next = next_node;
            node->next = new_node;
            next_node = new_node->next;
            next_node->prev = new_node;
            return ++new_node;
        }
    }

    space_between = (s32)(&gMemoryHeapEnd) - (s32)node;
    if (space_between >= (requestedSize + node->size)) {
        new_node = (MemoryAllocatorNode *)((s8 *)node + node->size);
        new_node->prev = node;
        new_node->refCount = 1;
        new_node->next = 0;
        new_node->ownerID = ownerID;
        new_node->size = requestedSize;
        new_node->unk_14 = 0;
        new_node->cleanupTimestamp = -1;
        node->next = new_node;
        return ++new_node;
    }

    return NULL;
}

void *decrementNodeRefCount(void *allocatedMemory) {
    MemoryAllocatorNode *node;

    if (allocatedMemory == NULL) {
        return NULL;
    }

    node = ((MemoryAllocatorNode *)allocatedMemory) - 1;
    node->refCount--;
    node->cleanupTimestamp = gFrameCounter;

    return NULL;
}

void cleanupUnusedNodes(void) {
    s32 globalVal;

    MemoryAllocatorNode *curr = gMemoryAllocatorHead;
    if (curr == 0) {
        return;
    }

    globalVal = gBufferedFrameCounter;

    while (curr != 0) {
        s32 diff = (globalVal - curr->cleanupTimestamp) & 0x0FFFFFFF;
        if (diff <= 0x07FFFFFF) {
            curr->cleanupTimestamp = -1;
        }

        if ((curr->refCount == 0 && curr->unk_14 == 0) && (curr->cleanupTimestamp < 0)) {
            MemoryAllocatorNode *prev;
            MemoryAllocatorNode *next;
            if (curr->prev != 0) {
                curr->prev->next = curr->next;
            } else {
                gMemoryAllocatorHead = curr->next;
            }
            if (curr->next != 0) {
                curr->next->prev = curr->prev;
            }
        }
        curr = curr->next;
    }
}

void markNodeAsLocked(void *arg0) {
    ((s32 *)arg0)[-0x3] = 1;
}

void unlockNodeWithInterruptDisable(s32 *arg0) {
    u32 mask = osSetIntMask(1);
    arg0[-3] = 0;
    osSetIntMask(mask);
}

s32 getNodeOwner(void *arg0) {
    return (s32)(*((void **)((u8 *)arg0 - 0xC)));
}

void setNodeSequenceNumber(void *arg0, void *arg1) {
    *((void **)((u8 *)arg0 - 4)) = arg1;
}

void *getNodeSequenceNumber(void *arg0) {
    return *((void **)((u8 *)arg0 - 4));
}