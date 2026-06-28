#pragma once

#include "common.h"

// Bytes from &Node::payload through the end of a scheduler node's inline payload storage.
#define TASK_NODE_INLINE_PAYLOAD_SIZE 0x100

typedef struct Node {
    /* 0x00 */ struct Node *prev;
    /* 0x04 */ struct Node *next;
    /* 0x08 */ struct Node *freeNext;
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 field_D;
    /* 0x0E */ u8 unkE;
    /* 0x0F */ u8 priority;
    /* 0x10 */ u8 continueFlag;
    /* 0x11 */ u8 hasAllocatedMemory;
    /* 0x12 */ u8 pad12[6];
    /* 0x18 */ void *unk18;
    /* 0x1C */ void *unk1C;
    /* 0x20 */ void (*callback)(void *);
    /* 0x24 */ void (*cleanupCallback)(void *);
    /* 0x28 */ void *payload;
} Node;

typedef struct SchedulerNodeWithPayload SchedulerNodeWithPayload;

typedef struct TaskScheduler {
    /* 0x00 */ struct TaskScheduler *prev;
    /* 0x04 */ struct TaskScheduler *next;
    /* 0x08 */ struct TaskScheduler *freeNext;
    /* 0x0C */ struct TaskScheduler *parentScheduler;
    /* 0x10 */ void (*gamestateHandler)(void);
    /* 0x14 */ void (*schedulerCleanupCallback)(void);
    /* 0x18 */ u8 schedulerState;
    /* 0x19 */ u8 priority;
    /* 0x1A */ u8 renderContext;
    /* 0x1B */ u8 handlerContinueFlag;
    /* 0x1C */ s32 cleanupFrameCounter;
    /* 0x20 */ void *latestDmaSequenceNumber;
    /* 0x24 */ void *latestDmaNode;
    /* 0x28 */ void *allocatedState;
    /* 0x2C */ SchedulerNodeWithPayload *nodes;
    /* 0x30 */ Node *activeList;
    /* 0x34 */ Node *freeList;
    /* 0x38 */ s16 counters[8];
    /* 0x48 */ s16 total;
    /* 0x4A */ s16 returnValue;
    /* 0x4C */ s16 childSchedulerCount;
} TaskScheduler;

extern TaskScheduler *gActiveScheduler;

void setGameStateHandler(void *arg0);

void *loadCompressedData(void *romStart, void *romEnd, s32 decompressedSize);

void *loadUncompressedData(void *start, void *end);

void *allocateNodeMemory(s32 size);

void *scheduleTask(void *a0, u8, u8, u8);

void setCallback(void *callback);

void setCallbackWithContinue(void *callback);

void setCleanupCallback(void *callback);

void *freeNodeMemory(void *);

void runTaskSchedulers(void);

void *getCurrentAllocation(void);

void *allocateTaskMemory(s32 size);

void setupTaskSchedulerNodes(s16 arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s32 arg5, s32 arg6, s32 arg7);

void *loadDataSegment(void *start, void *end, s32 size, void *dramAddr);

void terminateTasksByTypeAndID(s32 taskType, s32 taskID);

void terminateTasksByType(s32 taskType);

s32 hasActiveTasks(void);

void processActiveTasks(void);

void terminateAllTasks(void);

void initTaskScheduler(void);

void terminateSchedulerWithCallback(void (*arg0)(void));

void createTaskQueue(void (*arg0)(void), s32 arg1);

void createRootTaskScheduler(void (*gamestateHandler)(void), s32 priority);

void returnToParentScheduler(s32 arg0);

s16 getSchedulerReturnValue(void);

void setGameStateHandlerWithContinue(void (*gamestateHandler)(void));

void setRenderContext(s8 value);

u8 getRenderContext(void);

void terminateCurrentTask(void);

s32 getFreeNodeCount(s32 arg0);
