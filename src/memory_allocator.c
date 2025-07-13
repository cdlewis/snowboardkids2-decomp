#include "memory_allocator.h"

#include "common.h"

extern MemoryAllocatorNode gMemoryHeapEnd;
extern s32 D_800AB064_A23D4;
extern s32 D_800AB12C_A249C;

void initializeMemoryAllocatorRegion() {
    s32 i;
    u8 *current_block;

    gMemoryAllocatorHead = NULL;
    current_block = &gMemoryHeapBase;
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
                node->unk_0C++;
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
        gMemoryHeapBase.unk_0C = 1;
        gMemoryHeapBase.next = NULL;
        gMemoryHeapBase.ownerID = ownerID;
        gMemoryHeapBase.size = requestedSize;
        gMemoryHeapBase.unk_14 = 0;
        gMemoryHeapBase.unk_18 = -1;
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
            new_node->unk_0C = 1;
            new_node->ownerID = ownerID;
            new_node->size = requestedSize;
            new_node->unk_14 = 0;
            new_node->unk_18 = -1;
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
        new_node->unk_0C = 1;
        new_node->next = 0;
        new_node->ownerID = ownerID;
        new_node->size = requestedSize;
        new_node->unk_14 = 0;
        new_node->unk_18 = -1;
        node->next = new_node;
        return ++new_node;
    }

    return NULL;
}

s32 decrementNodeRefCount(s32 *arg0) {
    if (arg0 == NULL) {
        return 0;
    }
    arg0[-5]--;
    arg0[-2] = D_800AB064_A23D4;
    return 0;
}

void cleanupUnusedNodes() {
    s32 globalVal;

    MemoryAllocatorNode *curr = gMemoryAllocatorHead;
    if (curr == 0) {
        return;
    }

    globalVal = D_800AB12C_A249C;

    while (curr != 0) {
        s32 diff = (globalVal - curr->unk_18) & 0x0FFFFFFF;
        if (diff <= 0x07FFFFFF) {
            curr->unk_18 = -1;
        }

        if (((curr->unk_0C == 0) && (curr->unk_14 == 0)) && (curr->unk_18 < 0)) {
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

void markNodeAsLocked(s32 *arg0) {
    arg0[-0x3] = 1;
}

void unlockNodeWithInterruptDisable(s32 *arg0) {
    u32 mask = osSetIntMask(1);
    arg0[-3] = 0;
    osSetIntMask(mask);
}

s32 getNodeOwner(void *arg0) {
    return (s32)(*((void **)((u8 *)arg0 - 0xC)));
}

void setNodeUserData(void *arg0, void *arg1) {
    *((void **)((u8 *)arg0 - 4)) = arg1;
}

void *getNodeUserData(void *arg0) {
    return *((void **)((u8 *)arg0 - 4));
}