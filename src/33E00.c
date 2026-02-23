#include "D_800AFE8C_A71FC_type.h"
#include "common.h"
#include "task_scheduler.h"

extern void motorInitAsync(s32);
extern s32 pollMotorInitAsync(void);

extern u8 gControllerPollingEnabled;
extern u8 gMotorInitCompleteMask;

void updateControllerMotorInit(void);
void onMotorInitComplete(void);

void initControllerMotors(void) {
    u8 *ptr;
    s32 i;

    ptr = allocateTaskMemory(4);
    i = 3;
    ptr += 3;
    gControllerPollingEnabled = 1;
    gMotorInitCompleteMask = 0;
    for (; i >= 0; i--) {
        *ptr-- = 0;
    }
    setGameStateHandler(updateControllerMotorInit);
}

void updateControllerMotorInit(void) {
    u8 *allocation;
    s32 i;
    s32 completedCount;
    u8 state;
    u8 *ptr;
    u8 *savedPtr;
    s32 mask;
    s32 pollResult;

    allocation = getCurrentAllocation();
    completedCount = 0;
    for (i = 0; i < (s32)D_800AFE8C_A71FC->numPlayers; i++) {
        ptr = allocation + i;
        state = *ptr;
        switch (state) {
            case 0:
                motorInitAsync(i & 0xFFFF);
                state = 1 << i;
                savedPtr = ptr;
                do {
                    pollResult = pollMotorInitAsync();
                } while (pollResult == -1);
                if (pollResult == 0) {
                    gMotorInitCompleteMask |= state;
                    *savedPtr = 2;
                } else {
                    *savedPtr = 2;
                }
                break;
            case 1:
                pollResult = pollMotorInitAsync();
                if (pollResult >= 0) {
                    if (pollResult == 0) {
                        gMotorInitCompleteMask |= state << i;
                    }
                    *ptr = 2;
                }
                break;
            case 2:
                completedCount++;
                break;
            case 3:
                break;
        }
    }
    if (completedCount == D_800AFE8C_A71FC->numPlayers) {
        terminateSchedulerWithCallback(onMotorInitComplete);
    }
}

void onMotorInitComplete(void) {
    returnToParentScheduler(1);
}
