#include "3A1F0.h"
#include "6E840.h"
#include "D_800AFE8C_A71FC_type.h"
#include "EepromSaveData_type.h"
#include "common.h"
#include "gamestate.h"
#include "overlay.h"
#include "task_scheduler.h"

USE_OVERLAY(_1DA660)

extern void func_8001B020_1BC20(void);
extern void func_8001B3E8_1BFE8(void);

void onStoryMapExitToMainMenu(void);
void onStoryMapNormalExit(void);
void storyMapAwaitFadeIn(void);

void loadOverlay_1BBA0(void) {
    LOAD_OVERLAY(_1DA660)
    setGameStateHandler(&func_8001B020_1BC20);
}

INCLUDE_ASM("asm/nonmatchings/1BBA0", func_8001B020_1BC20);

void storyMapInitFadeIn(void) {
    GameState *state = (GameState *)getCurrentAllocation();
    if (state->isStoryMapInitializing != 0) {
        return;
    }

    if (func_8003BB5C_3C75C() != 0) {
        return;
    }

    func_8006FDA0_709A0(NULL, 0, 0x10);

    setGameStateHandler(storyMapAwaitFadeIn);
}

void storyMapAwaitFadeIn(void) {
    if (!func_8006FE10_70A10(0)) {
        setGameStateHandler(&func_8001B3E8_1BFE8);
    }
}

INCLUDE_ASM("asm/nonmatchings/1BBA0", func_8001B3E8_1BFE8);

void storyMapAwaitFadeOutAndCleanup(void) {
    void *exitCallback;
    Node_70B00 *state = (Node_70B00 *)getCurrentAllocation();

    if (func_8006FE10_70A10(0) == 0) {
        unlinkNode(&state[0]);
        unlinkNode(&state[1]);
        unlinkNode(&state[2]);

        osViExtendVStart(0);

        state[3].unk0.next = freeNodeMemory(state[3].unk0.next);
        state[3].prev = freeNodeMemory(state[3].prev);

        if (state[3].unk8.callback_selector != 0) {
            exitCallback = &onStoryMapExitToMainMenu;
        } else {
            exitCallback = &onStoryMapNormalExit;
        }

        terminateSchedulerWithCallback(exitCallback);
    }
}

void onStoryMapNormalExit(void) {
    s32 exitCode;

    exitCode = 1;
    if ((D_800AFE8C_A71FC->unk4 == 0) && (EepromSaveData->save_slot_status[0] == 5)) {
        exitCode = 0x44;
    }
    func_800697F4_6A3F4(exitCode);
}

void onStoryMapExitToMainMenu(void) {
    func_800697F4_6A3F4(0xFF);
}

void func_8001BCDC_1C8DC(void) {
    GameState *state = getCurrentAllocation();
    s32 i;
    s32 j;
    u8 count;

    for (i = 0; i < 9; i++) {
        count = 0;

        for (j = 0; j < D_800AFE8C_A71FC->unk8; j++) {
            if (state->unk59A[j] == 1 || state->unk59A[j] == 3) {
                count++;
            }
        }

        if (count == 0) {
            state->unk5B8[i] = 0;
        }
    }
}

void func_8001BD74_1C974(s8 *a0) {
    if (*a0 == 3) {
        *a0 = 2;
    } else if (*a0 == 8) {
        *a0 = 7;
    } else if (*a0 != 4) {
    } else {
        *a0 = 0;
    }
}
