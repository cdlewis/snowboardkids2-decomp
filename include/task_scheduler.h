#include "common.h"
#include "gamestate.h"
#include "memory_allocator.h"

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

// Extended Node structure used by func_8003D0F4_3DCF4
typedef struct {
    u8 pad0[0xC];
    u8 unkC[0xC];
    void* unk18;
    void* unk1C;
    u16 unk20;
    u16 unk22;
    u8 unk24;
    u8 unk25;
    s8 unk26;
} NodeExt;

void setGameStateHandler(void *arg0);

void *dmaRequestAndUpdateStateWithSize(void *, void *, s32);

void *dmaRequestAndUpdateState(void *start, void *end);

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

void func_800693C4_69FC4(void (*arg0)(void), s32 arg1);

void func_800697F4_6A3F4(s32 arg0);

s16 func_80069810_6A410(void);

void func_8006983C_6A43C(void (*arg0)(void));

void func_800698CC_6A4CC(s8 value);

u8 func_800698DC_6A4DC(void);

void func_80069CF8_6A8F8(void);

s32 getFreeNodeCount(s32 arg0);