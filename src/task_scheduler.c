#include "task_scheduler.h"

#include "3A1F0.h"
#include "gamestate.h"

#define SCHEDULER_STATE_INACTIVE 0
#define SCHEDULER_STATE_RUNNING 1
#define SCHEDULER_STATE_TERMINATING 2
#define SCHEDULER_STATE_DMA_PENDING 3
#define SCHEDULER_STATE_CLEANUP 4

extern Node *gDMAOverlay;

typedef struct {
    /* 0x00 */ Node n;
    u8 padding[0xFC]; // probably the max possible payload
} NodeWithPayload;

typedef struct TaskNode {
    /* 0x00 */ s8 padding_00[8];
    /* 0x08 */ void *unk08;
    /* 0x0C */ s8 padding_0C[0x18];
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s16 counters[8];
    /* 0x48 */ s16 unk48;
    /* 0x4A */ s8 padding_4A[6];
} TaskNode;

typedef struct gActiveScheduler_type {
    /* 0x0 */ struct gActiveScheduler_type *prev;
    /* 0x4 */ struct gActiveScheduler_type *next;
    TaskNode *unk8;
    struct gActiveScheduler_type *unkC;
    void (*unk10)(void);
    void (*unk14)(void);
    u8 schedulerState;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    s32 unk1C;
    /* 0x20 */ void *latestDmaSequenceNumber;
    /* 0x24 */ void *latestDmaNode;
    /* 0x28 */ void *allocatedState;
    /* 0x2C */ NodeWithPayload *nodes;
    /* 0x30 */ Node *activeList;
    /* 0x34 */ Node *freeList;
    /* 0x38 */ s16 counters[8];
    /* 0x48 */ s16 total;
    s16 unk4A;
    s16 unk4C;
} gActiveScheduler_type;
extern gActiveScheduler_type *gActiveScheduler;

extern TaskNode gSchedulerPool[16];
extern gActiveScheduler_type gSchedulerListSentinel;
extern TaskNode *gFreeSchedulerList;
extern gActiveScheduler_type *gActiveSchedulerList;
extern s32 gFrameCounter;
extern s32 gBufferedFrameCounter;

void initTaskScheduler(void) {
    s32 i;
    s32 j;
    void *prevHead;

    gActiveSchedulerList = NULL;
    gSchedulerListSentinel.prev = NULL;
    gActiveScheduler = NULL;
    gDMAOverlay = NULL;
    gFreeSchedulerList = NULL;

    for (i = 0; i < 16; i++) {
        gSchedulerPool[i].unk24 = 0;
        gSchedulerPool[i].unk28 = 0;
        gSchedulerPool[i].unk30 = 0;
        gSchedulerPool[i].unk34 = 0;
        gSchedulerPool[i].unk2C = 0;
        gSchedulerPool[i].unk48 = 0;

        for (j = 0; j < 8; j++) {
            gSchedulerPool[i].counters[j] = 0;
        }

        prevHead = gFreeSchedulerList;
        gSchedulerPool[i].unk08 = prevHead;
        gFreeSchedulerList = &gSchedulerPool[i];
    }
}

void func_800693C4_69FC4(void (*arg0)(void), s32 arg1) {
    gActiveScheduler_type *temp_a2;
    gActiveScheduler_type *temp_v1;
    gActiveScheduler_type *var_a3;
    temp_a2 = (gActiveScheduler_type *)gFreeSchedulerList;
    var_a3 = &gSchedulerListSentinel;
    gFreeSchedulerList = temp_a2->unk8;

    if (gActiveSchedulerList != 0) {
        while (TRUE) {
            temp_v1 = var_a3->next;
            if ((u8)arg1 >= (u8)temp_v1->unk19) {
                var_a3 = temp_v1;
                if (temp_v1->next != NULL) {
                    continue;
                }
            }

            break;
        }
    }

    temp_a2->prev = var_a3;
    temp_a2->next = var_a3->next;
    var_a3->next = temp_a2;

    if (temp_a2->next != NULL) {
        temp_a2->next->prev = temp_a2;
    }

    temp_a2->unk1A = 0;
    temp_a2->unk10 = arg0;
    temp_a2->schedulerState = SCHEDULER_STATE_INACTIVE;
    temp_a2->unk19 = arg1;
    temp_a2->allocatedState = 0;
    temp_a2->activeList = 0;
    temp_a2->freeList = 0;
    temp_a2->nodes = 0;
    temp_a2->total = 0;
    temp_a2->unk1C = -1;
    temp_a2->latestDmaSequenceNumber = 0;
    temp_a2->unkC = 0;
    temp_a2->unk4C = 0;
    temp_a2->unk4A = 0;
}

void createTaskQueue(void (*arg0)(void), s32 arg1) {
    gActiveScheduler_type *temp_a2;
    gActiveScheduler_type *var_a3;
    gActiveScheduler_type *temp_v1;
    gActiveScheduler_type *temp_v0;

    temp_a2 = (gActiveScheduler_type *)gFreeSchedulerList;
    var_a3 = &gSchedulerListSentinel;
    gFreeSchedulerList = temp_a2->unk8;

    if (gActiveSchedulerList != NULL) {
        while (TRUE) {
            temp_v1 = (gActiveScheduler_type *)var_a3->next;
            if ((u8)arg1 < (u8)temp_v1->unk19) {
                break;
            }
            var_a3 = temp_v1;
            if (temp_v1->next == NULL) {
                break;
            }
        }
    }

    temp_a2->prev = var_a3;
    temp_a2->next = var_a3->next;
    var_a3->next = temp_a2;
    temp_v0 = temp_a2->next;

    if (temp_v0 != 0) {
        temp_v0->prev = temp_a2;
    }

    temp_a2->unk1A = 0;
    temp_a2->schedulerState = SCHEDULER_STATE_INACTIVE;
    temp_a2->unk19 = arg1;
    temp_a2->unk10 = arg0;
    temp_a2->allocatedState = 0;
    temp_a2->activeList = 0;
    temp_a2->freeList = 0;
    temp_a2->nodes = 0;
    temp_a2->total = 0;
    temp_a2->unk1C = -1;
    temp_a2->latestDmaSequenceNumber = 0;
    temp_a2->unk4C = 0;
    temp_a2->unk4A = 0;
    temp_a2->unkC = (gActiveScheduler_type *)gActiveScheduler;
    gActiveScheduler->unk4C++;
}

void runTaskSchedulers(void) {
    gActiveScheduler_type *temp_v0;
    gActiveScheduler_type *temp_v0_2;
    TaskNode *temp_a0;

    gActiveScheduler = gActiveSchedulerList;
    if (gActiveSchedulerList != NULL) {
        do {
            if (gActiveScheduler->schedulerState == SCHEDULER_STATE_INACTIVE) {
                gActiveScheduler->schedulerState = SCHEDULER_STATE_RUNNING;
            }

            gActiveScheduler = gActiveScheduler->next;
        } while (gActiveScheduler != NULL);
    }

    gActiveScheduler = gActiveSchedulerList;
    if (gActiveSchedulerList != NULL) {
        do {
            switch (gActiveScheduler->schedulerState) {
                case SCHEDULER_STATE_INACTIVE:
                    break;

                case SCHEDULER_STATE_RUNNING:
                loop_9:
                    gActiveScheduler->unk1B = 0;

                    gActiveScheduler->unk10();

                    if (gActiveScheduler->unk1B != 0) {
                        if (gActiveScheduler->schedulerState == SCHEDULER_STATE_RUNNING) {
                            goto loop_9;
                        }
                    }

                    if (gActiveScheduler->schedulerState == SCHEDULER_STATE_RUNNING) {
                        processActiveTasks();
                    }

                    break;

                case SCHEDULER_STATE_TERMINATING:
                    if (gActiveScheduler->unk4C == 0) {
                        terminateAllTasks();
                        processActiveTasks();

                        if (hasActiveTasks() == 0) {
                            gActiveScheduler->nodes =
                                (NodeWithPayload *)decrementNodeRefCount((s32 *)gActiveScheduler->nodes);
                            gActiveScheduler->allocatedState =
                                (void *)decrementNodeRefCount((s32 *)gActiveScheduler->allocatedState);
                            gActiveScheduler->unk1C = gFrameCounter;
                            gActiveScheduler->schedulerState = SCHEDULER_STATE_CLEANUP;
                        }
                    }
                    break;

                case SCHEDULER_STATE_CLEANUP:
                    if (gActiveScheduler->unk1C > 0) {
                        if (((gBufferedFrameCounter - gActiveScheduler->unk1C) & 0x0FFFFFFF) <= 0x07FFFFFF) {
                            gActiveScheduler->unk1C = -1;
                        }
                        if (gActiveScheduler->unk1C <= 0) {
                            goto block_20;
                        }
                    } else {
                    block_20:
                        if (gActiveScheduler->unk14 != NULL) {
                            gActiveScheduler->unk14();
                        }

                        if (gActiveScheduler->unkC != 0) {
                            gActiveScheduler->unkC->unk4C--;
                        }

                        if (gActiveScheduler->next != NULL) {
                            gActiveScheduler->next->prev = gActiveScheduler->prev;
                        }

                        gActiveScheduler->prev->next = gActiveScheduler->next;
                        temp_a0 = gFreeSchedulerList;
                        gFreeSchedulerList = (TaskNode *)gActiveScheduler;
                        gActiveScheduler->unk8 = temp_a0;
                    }
                    break;

                case SCHEDULER_STATE_DMA_PENDING:
                    if (getNodeOwner(gActiveScheduler->latestDmaNode) == NULL) {
                        gActiveScheduler->schedulerState = SCHEDULER_STATE_RUNNING;
                    }
                    break;
            }

            gActiveScheduler = gActiveScheduler->next;
        } while (gActiveScheduler != NULL);
    }
}

void terminateSchedulerWithCallback(void (*arg0)(void)) {
    if (gActiveScheduler != NULL) {
        gActiveScheduler->schedulerState = SCHEDULER_STATE_TERMINATING;
        gActiveScheduler->unk14 = arg0;
    }
}

void func_800697F4_6A3F4(s32 arg0) {
    gActiveScheduler_type *temp_v0;

    temp_v0 = gActiveScheduler->unkC;
    if (temp_v0 != NULL) {
        temp_v0->unk4A = arg0;
    }
}

s16 func_80069810_6A410(void) {
    s16 temp_v0;

    temp_v0 = gActiveScheduler->unk4A;
    gActiveScheduler->unk4A = 0;

    return temp_v0;
}

void setGameStateHandler(void *arg0) {
    gActiveScheduler->unk10 = arg0;
}

void func_8006983C_6A43C(void (*arg0)(void)) {
    gActiveScheduler->unk10 = arg0;
    gActiveScheduler->unk1B = 1;
}

void *allocateTaskMemory(s32 size) {
    u8 node_exists;
    u32 temp_a0;
    void *temp_v0;
    s8 *var_v1;

    temp_v0 = allocateMemoryNode(0, size, &node_exists);
    gActiveScheduler->allocatedState = temp_v0;
    if (size != 0) {
        var_v1 = (s8 *)temp_v0;
        temp_a0 = (size + (s32)var_v1);
        do {
            *var_v1 = 0;
            var_v1 += 1;
        } while ((u32)var_v1 < (u32)temp_a0);
    }
    return gActiveScheduler->allocatedState;
}

void *getCurrentAllocation(void) {
    return gActiveScheduler->allocatedState;
}

void func_800698CC_6A4CC(s8 value) {
    gActiveScheduler->unk1A = value;
}

u8 func_800698DC_6A4DC(void) {
    return gActiveScheduler->unk1A;
}

void setupTaskSchedulerNodes(s16 arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s32 arg5, s32 arg6, s32 arg7) {
    s16 i;
    u8 nodeExists;

    gActiveScheduler->total = arg7 + (arg6 + (arg5 + ((((arg0 + arg1) + arg2) + arg3) + arg4)));
    gActiveScheduler->counters[0] = arg0;
    gActiveScheduler->counters[2] = arg2;
    gActiveScheduler->counters[1] = arg1;
    gActiveScheduler->counters[3] = arg3;
    gActiveScheduler->counters[4] = arg4;
    gActiveScheduler->counters[5] = arg5;
    gActiveScheduler->counters[6] = arg6;
    gActiveScheduler->counters[7] = arg7;

    gActiveScheduler->nodes = allocateMemoryNode(0, gActiveScheduler->total * sizeof(NodeWithPayload), &nodeExists);

    gActiveScheduler->nodes = gActiveScheduler->nodes;
    gActiveScheduler->activeList = NULL;
    gActiveScheduler->freeList = NULL;

    for (i = 0; i < gActiveScheduler->total; i++) {
        gActiveScheduler->nodes[i].n.freeNext = gActiveScheduler->freeList;
        gActiveScheduler->freeList = (Node *)&gActiveScheduler->nodes[i];
    }
}

void *scheduleTask(void *callback, u8 nodeType, u8 identifierFlag, u8 priority) {
    Node *newNode;
    Node *active;
    Node *freeNxt;
    gActiveScheduler_type *g = gActiveScheduler;
    s16 *countPtr;
    Node *temp;

    if (gActiveScheduler) {
        if (g->counters[nodeType]) {
            g->counters[nodeType]--;

            newNode = gActiveScheduler->freeList;
            active = gActiveScheduler->activeList;
            freeNxt = newNode->freeNext;
            gActiveScheduler->freeList = freeNxt;

            if (active == NULL) {
                newNode->next = NULL;
                newNode->prev = NULL;
                gActiveScheduler->activeList = newNode;
            } else if (priority < active->priority) {
                newNode->next = active;
                newNode->prev = NULL;
                temp = newNode->next;
                temp->prev = newNode;
                g->activeList = newNode;
            } else {
                Node *it = active;
                while (it->next != NULL) {
                    if (priority < it->next->priority) {
                        break;
                    }
                    it = it->next;
                }

                newNode->next = it->next;
                newNode->prev = it;
                it->next = newNode;
                if (newNode->next) {
                    newNode->next->prev = newNode;
                }
            }

            newNode->unkC = nodeType;
            newNode->field_D = identifierFlag;
            newNode->priority = priority;
            newNode->callback = callback;
            newNode->cleanupCallback = NULL;
            newNode->unkE = 1;
            newNode->hasAllocatedMemory = FALSE;
            newNode->unk18 = 0;

            // This should just be newNode++. It's possible that
            // nodes are possbibly being embedded in larger
            // structs.
            return (void *)((u8 *)newNode + 0x28);
        }
    }

    return NULL;
}

s16 getFreeNodeCount(s32 arg0) {
    return gActiveScheduler->counters[arg0];
}

void processActiveTasks(void) {
    for (gDMAOverlay = gActiveScheduler->activeList; gDMAOverlay != 0; gDMAOverlay = gDMAOverlay->next) {
        switch (gDMAOverlay->unkE) {
            case 0:
                break;
            case 1:
                while (TRUE) {
                    gDMAOverlay->continueFlag = 0;
                    gDMAOverlay->callback(&gDMAOverlay->payload);
                    if ((gDMAOverlay->continueFlag != 0) && (gDMAOverlay->unkE == 1)) {
                        continue;
                    }
                    break;
                }
                break;

            case 2:
                if (gDMAOverlay->cleanupCallback != NULL) {
                    gDMAOverlay->cleanupCallback(&gDMAOverlay->payload);
                }

                if (gDMAOverlay->prev == NULL) {
                    gActiveScheduler->activeList = gDMAOverlay->next;
                } else {
                    gDMAOverlay->prev->next = gDMAOverlay->next;
                }

                if (gDMAOverlay->next != NULL) {
                    gDMAOverlay->next->prev = gDMAOverlay->prev;
                }
                gDMAOverlay->freeNext = gActiveScheduler->freeList;
                gActiveScheduler->freeList = gDMAOverlay;
                gActiveScheduler->counters[(u8)gDMAOverlay->unkC]++;
                break;

            case 3:
                if (getNodeOwner(gDMAOverlay->unk1C) == 0) {
                    gDMAOverlay->unkE = 1;
                }
                break;

            case 4:
                if (getNodeOwner(gDMAOverlay->unk1C) == 0) {
                    gDMAOverlay->unkE = 2;
                }
                break;
        }
    }
}

void setCallback(void *callback) {
    gDMAOverlay->callback = callback;
}

void setCallbackWithContinue(void *callback) {
    gDMAOverlay->callback = callback;
    gDMAOverlay->continueFlag = 1;
}

void setCleanupCallback(void *callback) {
    gDMAOverlay->cleanupCallback = callback;
}

void func_80069CF8_6A8F8(void) {
    u8 temp = gDMAOverlay->unkE - 3;
    if (temp < 2) {
        gDMAOverlay->unkE = 4;
    } else {
        gDMAOverlay->unkE = 2;
    }
}

s32 hasActiveTasks(void) {
    return gActiveScheduler->activeList != NULL;
}

void terminateAllTasks(void) {
    Node *i;

    for (i = gActiveScheduler->activeList; i != NULL; i = i->next) {
        if ((u32)(i->unkE - 3) < 2) {
            i->unkE = 4;
        } else {
            i->unkE = 2;
        }
    }
}

void terminateTasksByType(s32 taskType) {
    Node *i;

    for (i = gActiveScheduler->activeList; i != NULL; i = i->next) {
        if (i->unkC == (u8)taskType) {
            if ((u32)(i->unkE - 3) < 2) {
                i->unkE = 4;
            } else {
                i->unkE = 2;
            }
        }
    }
}

void terminateTasksByTypeAndID(s32 taskType, s32 taskID) {
    Node *i;

    for (i = gActiveScheduler->activeList; i != NULL; i = i->next) {
        if (i->unkC == (u8)taskType && i->field_D == (u8)taskID) {
            if ((u32)(i->unkE - 3) < 2) {
                i->unkE = 4;
            } else {
                i->unkE = 2;
            }
        }
    }
}

void *dmaRequestAndUpdateState(void *start, void *end) {
    void *allocatedSpaceStart;

    if (gDMAOverlay == NULL) {
        allocatedSpaceStart = queueDmaTransfer(start, end);
        if (getNodeOwner(allocatedSpaceStart) != 0) {
            if (gActiveScheduler->schedulerState != SCHEDULER_STATE_DMA_PENDING) {
                gActiveScheduler->latestDmaSequenceNumber = getNodeSequenceNumber(allocatedSpaceStart);
                gActiveScheduler->latestDmaNode = allocatedSpaceStart;
                gActiveScheduler->schedulerState = SCHEDULER_STATE_DMA_PENDING;
            } else if (gActiveScheduler->latestDmaSequenceNumber < getNodeSequenceNumber(allocatedSpaceStart)) {
                gActiveScheduler->latestDmaSequenceNumber = getNodeSequenceNumber(allocatedSpaceStart);
                gActiveScheduler->latestDmaNode = allocatedSpaceStart;
            }
        }
    } else {
        allocatedSpaceStart = queueDmaTransfer(start, end);
        if (getNodeOwner(allocatedSpaceStart) != 0) {
            if (((u8)(gDMAOverlay->unkE - 3)) >= 2) {
                gDMAOverlay->unk18 = getNodeSequenceNumber(allocatedSpaceStart);
                gDMAOverlay->unk1C = allocatedSpaceStart;
                gDMAOverlay->unkE = 3;
            } else {
                if (gDMAOverlay->unk18 < getNodeSequenceNumber(allocatedSpaceStart)) {
                    gDMAOverlay->unk18 = getNodeSequenceNumber(allocatedSpaceStart);
                    gDMAOverlay->unk1C = allocatedSpaceStart;
                }
            }
        }
        gDMAOverlay->hasAllocatedMemory = TRUE;
    }

    return allocatedSpaceStart;
}

void *dmaRequestAndUpdateStateWithSize(void *romStart, void *romEnd, s32 size) {
    void *allocatedSpaceStart;

    if (gDMAOverlay == NULL) {
        allocatedSpaceStart = dmaQueueRequest(romStart, romEnd, size);
        if (getNodeOwner(allocatedSpaceStart) != NULL) {
            if (gActiveScheduler->schedulerState != SCHEDULER_STATE_DMA_PENDING) {
                gActiveScheduler->latestDmaSequenceNumber = getNodeSequenceNumber(allocatedSpaceStart);
                gActiveScheduler->latestDmaNode = allocatedSpaceStart;
                gActiveScheduler->schedulerState = SCHEDULER_STATE_DMA_PENDING;
            } else {
                if (gActiveScheduler->latestDmaSequenceNumber < getNodeSequenceNumber(allocatedSpaceStart)) {
                    gActiveScheduler->latestDmaSequenceNumber = getNodeSequenceNumber(allocatedSpaceStart);
                    gActiveScheduler->latestDmaNode = allocatedSpaceStart;
                }
            }
        }
    } else {
        allocatedSpaceStart = dmaQueueRequest(romStart, romEnd, size);
        if (getNodeOwner(allocatedSpaceStart) != NULL) {
            if (gDMAOverlay->unkE < 3 || gDMAOverlay->unkE >= 5) {
                gDMAOverlay->unk18 = getNodeSequenceNumber(allocatedSpaceStart);
                gDMAOverlay->unk1C = allocatedSpaceStart;
                gDMAOverlay->unkE = 3;
            } else {
                if (gDMAOverlay->unk18 < getNodeSequenceNumber(allocatedSpaceStart)) {
                    gDMAOverlay->unk18 = getNodeSequenceNumber(allocatedSpaceStart);
                    gDMAOverlay->unk1C = allocatedSpaceStart;
                }
            }
        }

        gDMAOverlay->hasAllocatedMemory = TRUE;
    }

    return allocatedSpaceStart;
}

void *loadDataSegment(void *start, void *end, s32 size, void *dramAddr) {
    void *var_s0 = dramAddr;
    if (gDMAOverlay != NULL) {
        var_s0 = queueDmaTransferToBuffer(start, end, size, dramAddr);
        if (getNodeOwner(var_s0) != 0) {
            if ((u32)(gDMAOverlay->unkE - 3) >= 2U) {
                gDMAOverlay->unk18 = getNodeSequenceNumber(var_s0);
                gDMAOverlay->unk1C = var_s0;
                gDMAOverlay->unkE = 3;
            } else if ((u32)gDMAOverlay->unk18 < (u32)getNodeSequenceNumber(var_s0)) {
                gDMAOverlay->unk18 = getNodeSequenceNumber(var_s0);
                gDMAOverlay->unk1C = var_s0;
            }
        }
        gDMAOverlay->hasAllocatedMemory = TRUE;
    }
    return var_s0;
}

void *allocateNodeMemory(s32 size) {
    u8 exists;
    void *temp_a0;

    temp_a0 = allocateMemoryNode(0, size, &exists);
    if (gDMAOverlay != NULL) {
        gDMAOverlay->hasAllocatedMemory = TRUE;
    }

    return temp_a0;
}

void *freeNodeMemory(void *node) {
    return decrementNodeRefCount(node);
}

static const u32 D_8009EA54_padding[3] = { 0, 0, 0 };
