#include "2F990.h"
#include "6E840.h"
#include "common.h"
#include "graphics.h"
#include "rom_loader.h"
#include "task_scheduler.h"

extern s32 gControllerInputs;

void unlockScreenScheduleDisplayTasks(void);
void updateUnlockScreen(void);

void onUnlockScreenExit(void);
void unlockScreenCleanupAndExit(void);
void unlockScreenAwaitUserDismiss(void);
void unlockScreenAwaitFadeIn(void);

typedef struct {
    /* 0x000 */ u8 cameraNode0[0x1D8];
    /* 0x1D8 */ u8 cameraNode1[0x1D8];
    /* 0x3B0 */ u8 cameraNode2[0x1D8];
    /* 0x588 */ void *arrowSpriteAsset;
    /* 0x58C */ void *backgroundAsset;
    /* 0x590 */ void *itemIconAsset;
    /* 0x594 */ void *digitSpriteAsset;
    /* 0x598 */ void *goldIconAsset;
    /* 0x59C */ void *itemLabelAsset;
    /* 0x5A0 */ u8 rotationMatrix[0x20];
    /* 0x5C0 */ u16 frameCounter;
    /* 0x5C2 */ u8 pad5C2[0x3];
    /* 0x5C5 */ u8 statePhase;
    /* 0x5C6 */ u8 pad5C6[0x3];
    /* 0x5C9 */ u8 unlockCount;
    /* 0x5CA */ u8 pad5CA[0xC];
    /* 0x5D6 */ u8 transitionState;
} UnlockScreenState;

INCLUDE_ASM("asm/nonmatchings/1C9C0", initUnlockScreen);

void waitForUnlocksAssetsReady(void) {
    UnlockScreenState *state = (UnlockScreenState *)getCurrentAllocation();

    state->frameCounter++;
    if (state->frameCounter < 3) {
        return;
    }

    state->frameCounter = 2;
    if (getPendingDmaCount() != 0) {
        return;
    }

    state->frameCounter = 0;
    setViewportFadeValue(0, 0, 0xE);
    setGameStateHandler(unlockScreenAwaitFadeIn);
}

void unlockScreenAwaitFadeIn(void) {
    UnlockScreenState *state = (UnlockScreenState *)getCurrentAllocation();

    if (getViewportFadeMode(NULL) != 0) {
        return;
    }

    if (state->unlockCount != 0) {
        state->transitionState = 1;
        playSoundEffectOnChannelNoPriority(0xEA, 1);
        setGameStateHandler(unlockScreenScheduleDisplayTasks);
    } else {
        state->transitionState = 3;
        playSoundEffectOnChannelNoPriority(0xEE, 1);
        setGameStateHandler(unlockScreenAwaitUserDismiss);
        scheduleTask(initStoryMapShopSoldOutLabel, 0, 0, 0x5A);
    }
}

void unlockScreenScheduleDisplayTasks(void) {
    UnlockScreenState *state = (UnlockScreenState *)getCurrentAllocation();

    if (state->transitionState != 0) {
        return;
    }

    state->statePhase = 0x14;
    scheduleTask(initStoryMapShopItemPriceDisplay, 1, 0, 0x5A);
    scheduleTask(initStoryMapShopItemStatsDisplay, 1, 0, 0x5A);
    scheduleTask(initUnlockScreenItemIcons, 0, 0, 0x5A);

    if (state->unlockCount >= 2) {
        scheduleTask(initUnlockScreenScrollArrows, 1, 0, 0x5A);
    }

    setGameStateHandler(updateUnlockScreen);
}

void unlockScreenAwaitUserDismiss(void) {
    UnlockScreenState *state = (UnlockScreenState *)getCurrentAllocation();

    if (state->transitionState != 0) {
        return;
    }

    if (gControllerInputs & 0xD000) {
        playSoundEffectOnChannelNoPriority(0xED, 1);
        state->transitionState = 2;
        setViewportFadeValue(0, 0xFF, 0x10);
        setGameStateHandler(unlockScreenCleanupAndExit);
    }
}

INCLUDE_ASM("asm/nonmatchings/1C9C0", updateUnlockScreen);

void unlockScreenCountdownToExit(void) {
    UnlockScreenState *state = (UnlockScreenState *)getCurrentAllocation();

    state->frameCounter--;
    if (state->frameCounter == 0) {
        setViewportFadeValue(0, 0xFF, 0x10);
        setGameStateHandler(unlockScreenCleanupAndExit);
    }

    if (state->transitionState != 0) {
        state->transitionState = 0;
    }
}

void unlockScreenCleanupAndExit(void) {
    UnlockScreenState *state = (UnlockScreenState *)getCurrentAllocation();

    if (getViewportFadeMode(NULL) != 0) {
        return;
    }

    unlinkNode((Node_70B00 *)state);
    unlinkNode((Node_70B00 *)&state->cameraNode1);
    unlinkNode((Node_70B00 *)&state->cameraNode2);

    state->arrowSpriteAsset = freeNodeMemory(state->arrowSpriteAsset);
    state->backgroundAsset = freeNodeMemory(state->backgroundAsset);
    state->itemIconAsset = freeNodeMemory(state->itemIconAsset);
    state->digitSpriteAsset = freeNodeMemory(state->digitSpriteAsset);
    state->goldIconAsset = freeNodeMemory(state->goldIconAsset);
    state->itemLabelAsset = freeNodeMemory(state->itemLabelAsset);

    terminateSchedulerWithCallback(onUnlockScreenExit);
}

void onUnlockScreenExit(void) {
    func_800697F4_6A3F4(1);
}
