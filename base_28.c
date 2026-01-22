#include "common.h"
#include "gamestate.h"
#include "task_scheduler.h"

extern void initConfettiEffect(void *);

typedef struct {
    u8 _pad0[0x8];
    void *cameraNode;
    u8 _pad1[0xC];
    s16 index;
    s16 particleCount;
    u8 pauseWhenPaused;
} ConfettiSpawnTask;

void func_80041418_42018(void) {
    s32 pad[2];
    GameState *gameState;
    ConfettiSpawnTask *task;
    s32 i;
    s32 count;
    s32 numPlayers;

    gameState = getCurrentAllocation();
    numPlayers = gameState->unk5D;
    (void)pad;
    i = 0;
    if (numPlayers > 0) {
        do {
            task = scheduleTask(initConfettiEffect, 0, 0, 0xF0);
            if (task != NULL) {
                task->index = (s16)i;
                task->cameraNode = (void *)((s32)gameState->unk8 + ((s16)i * 0x1D8));
                count = gameState->unk5D;
                if (count != 2) {
                    if (count < 3) {
                        if (count == 1) {
                            task->particleCount = 0x64;
                        } else {
                            task->particleCount = 0x19;
                        }
                    } else {
                        task->particleCount = 0x19;
                    }
                } else {
                    task->particleCount = 0x32;
                }
                task->pauseWhenPaused = 1;
            }
            i++;
        } while (i < gameState->unk5D);
    }
}
