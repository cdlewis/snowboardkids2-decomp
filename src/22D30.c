#include "288A0.h"
#include "6E840.h"
#include "9FF70.h"
#include "D_800AFE8C_A71FC_type.h"
#include "common.h"
#include "graphics.h"
#include "overlay.h"
#include "task_scheduler.h"

USE_ASSET(_426EF0);
USE_ASSET(_41A1D0);
USE_OVERLAY(_1DB7A0);

extern volatile s32 gControllerInputs;

typedef struct {
    Node_70B00 node;
    void *assetData1;
    void *assetData2;
    s16 frameCounter;
    union {
        u16 selectedPlayerIndex;
        struct {
            u8 selectedPlayerIndexHi;
            u8 selectedPlayerIndexLo;
        } bytes;
    } playerCount;
    u8 connectedControllerCount;
    u8 menuResult;
    u8 menuState;
} PlayerCountSelectState;

extern u8 D_8008DCC0_8E8C0[];
extern void D_800B0690(void);
extern void D_800B07A0(void);
extern void *D_800B054C;
extern void *D_800B08A8;
extern u8 gConnectedControllerMask;

void handlePlayerCountSelectInput(void);
void func_800225C8_231C8(void);
void func_800226B0_232B0(void);
void func_800226CC_232CC(void);
void func_80022304_22F04(void);

void initPlayerCountSelectState(void) {
    PlayerCountSelectState *state;
    s32 i;
    u8 temp;
    u8 numControllers;

    state = allocateTaskMemory(0x1E8);

    LOAD_OVERLAY(_1DB7A0);

    setupTaskSchedulerNodes(0x14, 0, 0, 0, 0, 0, 0, 0);
    state->frameCounter = 0;
    state->menuResult = 0;
    state->connectedControllerCount = 0;
    state->menuState = 0;
    for (i = 0; i < 4; i++) {
        if ((gConnectedControllerMask >> i) & 1) {
            state->connectedControllerCount = state->connectedControllerCount + 1;
        }
    }
    temp = D_800AFE8C_A71FC->numPlayers;
    if (temp != 0) {
        state->playerCount.selectedPlayerIndex = temp - 1;
    } else {
        state->playerCount.selectedPlayerIndex = 0;
    }
    numControllers = state->connectedControllerCount;
    if (numControllers < state->playerCount.selectedPlayerIndex + 1) {
        state->playerCount.selectedPlayerIndex = numControllers - 1;
    }
    func_80027CA0_288A0(&state->node, 8, 0xA, 1);
    func_8006FDA0_709A0(&state->node, 0xFF, 0);
    func_8006FDA0_709A0(&state->node, 0, 0x10);
    state->assetData1 = loadCompressedData(&_426EF0_ROM_START, &_426EF0_ROM_END, 0xEEE8);
    state->assetData2 = loadCompressedData(&_41A1D0_ROM_START, &_41A1D0_ROM_END, 0x1B48);
    scheduleTask(&D_800B054C, 0, 0, 0x5A);
    scheduleTask(&D_800B08A8, 0, 0, 0x5A);
    setGameStateHandler(func_80022304_22F04);
}

void func_80022304_22F04(void) {
    PlayerCountSelectState *state;
    s32 result;
    u8 *task;
    u16 temp;

    state = (PlayerCountSelectState *)getCurrentAllocation();
    result = func_8006FE10_70A10(&state->node);

    if (result == 0) {
        state->frameCounter = 0;
        setGameStateHandler(&handlePlayerCountSelectInput);
        scheduleTask(&D_800B0690, 0, 0, 0x5A);
        scheduleTask(&D_800B07A0, 0, 0, 0x5A);
    } else {
        temp = state->frameCounter;
        if (temp < 4) {
            if (result == (0xC - (temp * 2))) {
                task = (u8 *)scheduleTask(&func_800B00C0_9FF70, 0, 0, 0x5A);
                task[0x2B] = (u8)state->frameCounter;
                state->frameCounter = state->frameCounter + 1;
            }
        }
    }
}

void handlePlayerCountSelectInput(void) {
    PlayerCountSelectState *state;
    s32 inputs;

    state = (PlayerCountSelectState *)getCurrentAllocation();

    switch (state->menuState) {
        case 0:
            inputs = gControllerInputs;
            if (inputs & B_BUTTON) {
                func_800585C8_591C8(0x2E);
                state->menuState = 0xA;
            } else {
                if (inputs & 0x40100) {
                    if (state->playerCount.selectedPlayerIndex < (state->connectedControllerCount - 1)) {
                        state->playerCount.selectedPlayerIndex = state->playerCount.selectedPlayerIndex + 1;
                        func_80058220_58E20(0x2B, 0);
                        break;
                    }
                }
                if (gControllerInputs & 0x80200) {
                    if (state->playerCount.selectedPlayerIndex != 0) {
                        state->playerCount.selectedPlayerIndex = state->playerCount.selectedPlayerIndex - 1;
                        func_80058220_58E20(0x2B, 0);
                        break;
                    }
                }
                if (gControllerInputs & A_BUTTON) {
                    state->menuState = 1;
                    state->frameCounter = 0;
                    func_80058220_58E20(0x2C, 0);
                }
            }
            break;
        case 1:
            state->frameCounter = state->frameCounter + 1;
            if ((u16)state->frameCounter == 0x11) {
                state->frameCounter = 0;
                state->menuState = 2;
            }
            break;
        case 2:
            inputs = gControllerInputs;
            if (inputs & B_BUTTON) {
                goto common_exit;
            } else if (inputs & A_BUTTON) {
                state->menuResult = 1;
                func_80058220_58E20(0x2D, 0);
            }
            break;
        case 0xA:
            inputs = gControllerInputs;
            if (inputs & B_BUTTON) {
            common_exit:
                func_800585C8_591C8(0x2E);
                state->menuState = 0;
            } else if (inputs & A_BUTTON) {
                state->menuResult = 0x63;
            }
            break;
    }

    if (state->menuResult != 0) {
        func_80057564_58164(0xA);
        if (state->menuResult == 0x63) {
            func_8006FDA0_709A0((Node_70B00 *)state, 0xFF, 8);
        } else {
            func_8006FDA0_709A0((Node_70B00 *)state, 0xFF, 0x10);
        }
        setGameStateHandler(&func_800225C8_231C8);
    }
}

void func_800225C8_231C8(void) {
    PlayerCountSelectState *state;
    s32 i;

    state = (PlayerCountSelectState *)getCurrentAllocation();
    if (func_8006FE10_70A10(&state->node) == 0) {
        terminateAllTasks();
        unlinkNode(&state->node);
        state->assetData1 = freeNodeMemory(state->assetData1);
        state->assetData2 = freeNodeMemory(state->assetData2);
        if (state->menuResult == 1) {
            terminateSchedulerWithCallback(func_800226B0_232B0);
            D_800AFE8C_A71FC->numPlayers = state->playerCount.bytes.selectedPlayerIndexLo + 1;
            for (i = 0; i < D_800AFE8C_A71FC->numPlayers; i++) {
                D_800AFE8C_A71FC->unk9[i] = D_8008DCC0_8E8C0[i];
            }
        } else {
            terminateSchedulerWithCallback(func_800226CC_232CC);
        }
    }
}

void func_800226B0_232B0(void) {
    func_800697F4_6A3F4(1);
}

void func_800226CC_232CC(void) {
    func_800697F4_6A3F4(0xFF);
}
