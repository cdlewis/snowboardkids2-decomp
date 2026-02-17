#include "D_800AFE8C_A71FC_type.h"
#include "EepromSaveData_type.h"
#include "common.h"
#include "gamestate.h"
#include "graphics.h"
#include "rom_loader.h"
#include "task_scheduler.h"

USE_OVERLAY(_1DA660)

u8 storyMapLocationOrder1[] = { 0x04, 0x00, 0x02, 0x08, 0x07, 0x03, 0x05, 0x01, 0x06, 0x00, 0x00, 0x00 };
u8 storyMapLocationOrder2[] = { 0x01, 0x07, 0x02, 0x05, 0x00, 0x06, 0x08, 0x04, 0x03, 0x00, 0x00, 0x00 };
s16 storyMapLocationPositions[] = { 0x00BC, 0x0050, 0x0084, 0x00DE, 0x00F2, 0x00A7, 0x00C7, 0x006C, 0x0102, 0x0000 };
s16 storyMapLocationSpriteIds[] = { 0x0126, 0x0122, 0x0124, 0x0128, 0x0129, 0x0125, 0x0127, 0x0123, 0x012A, 0x0000 };
u8 storyMapLocationConfig[] = { 0x0A, 0x0A, 0x0F, 0x0A, 0x19, 0x0A, 0x0D, 0x0A, 0x19, 0x00, 0x00, 0x00 };
s16 storyMapLocationOrder3[] = { 0x0000, 0x0002, 0x0004, 0x0008, 0x0007, 0x0003, 0x0005, 0x0001, 0x0006, 0x0000 };

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

    if (getPendingDmaCount() != 0) {
        return;
    }

    setViewportFadeValue(NULL, 0, 0x10);

    setGameStateHandler(storyMapAwaitFadeIn);
}

void storyMapAwaitFadeIn(void) {
    if (!getViewportFadeMode(0)) {
        setGameStateHandler(&func_8001B3E8_1BFE8);
    }
}

INCLUDE_ASM("asm/nonmatchings/1BBA0", func_8001B3E8_1BFE8);

void storyMapAwaitFadeOutAndCleanup(void) {
    void *exitCallback;
    ViewportNode *state = (ViewportNode *)getCurrentAllocation();

    if (getViewportFadeMode(0) == 0) {
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
    returnToParentScheduler(exitCode);
}

void onStoryMapExitToMainMenu(void) {
    returnToParentScheduler(0xFF);
}

void storyMapClearLocationArrivalsIfNoActivePlayers(void) {
    GameState *state = getCurrentAllocation();
    s32 locationIdx;
    s32 playerIdx;
    u8 activePlayerCount;

    for (locationIdx = 0; locationIdx < 9; locationIdx++) {
        activePlayerCount = 0;

        for (playerIdx = 0; playerIdx < D_800AFE8C_A71FC->numPlayers; playerIdx++) {
            if (state->playerSlotState[playerIdx] == 1 || state->playerSlotState[playerIdx] == 3) {
                activePlayerCount++;
            }
        }

        if (activePlayerCount == 0) {
            state->unk5B8[locationIdx] = 0;
        }
    }
}

void storyMapRevertBlockedLocationIndex(s8 *locationIndex) {
    if (*locationIndex == 3) {
        *locationIndex = 2;
    } else if (*locationIndex == 8) {
        *locationIndex = 7;
    } else if (*locationIndex != 4) {
    } else {
        *locationIndex = 0;
    }
}
