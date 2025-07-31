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
    /* 0x11 */ u8 unk11;
    /* 0x12 */ u8 pad12[6];
    /* 0x18 */ void *unk18;
    /* 0x1C */ void *unk1C;
    /* 0x20 */ void (*callback)(void *);
    /* 0x24 */ void (*cleanupCallback)(void *);
    /* 0x28 */ void *payload;
} Node;

void setGameStateHandler(void *arg0);

void *dmaRequestAndUpdateStateWithSize(void *, void *, s32);

void *dmaRequestAndUpdateState(void *start, void *end);

GameState *allocateGameStateMemory(s32 arg0);

Node *scheduleTask(void *a0, u8, u8, u8);

void setCallback(void(callback)(void *));

void setCallbackWithContinue(void(callback)(void *));

void setCleanupCallback(void(callback)(void *));

s32 freeGameStateMemory(s32 *);

void runTaskSchedulers(void);

void *getCurrentAllocation();

void *allocateTaskMemory(s32 size);

void setupTaskSchedulerNodes(s16 arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s32 arg5, s32 arg6, s32 arg7);

void *loadDataSegment(void *start, void *end, s32 size, void *dramAddr);

void terminateTasksByTypeAndID(s32 taskType, s32 taskID);

void terminateTasksByType(s32 taskType);

s32 hasActiveTasks(void);

void processActiveTasks(void);

void terminateAllTasks(void);

void initTaskScheduler(void);

void terminateSchedulerWithCallback(void (*arg0)());

void createTaskQueue(void (*arg0)(), s32 arg1);

void func_800693C4_69FC4(void (*arg0)(), s32 arg1);

void func_800697F4_6A3F4(s32 arg0);

s16 func_80069810_6A410(void);

void func_8006983C_6A43C(void (*arg0)());