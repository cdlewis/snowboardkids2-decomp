#include "D_800AFE8C_A71FC_type.h"
#include "EepromSaveData_type.h"
#include "audio/audio.h"
#include "graphics/graphics.h"
#include "math/geometry.h"
#include "race/track_geometry.h"
#include "story/shop_ui.h"
#include "system/rom_loader.h"
#include "system/task_scheduler.h"

extern s32 gControllerInputs;
extern s32 storyMapLocationNames[];

void unlockScreenScheduleDisplayTasks(void);
void updateUnlockScreen(void);

void onUnlockScreenExit(void);
void unlockScreenCleanupAndExit(void);
void unlockScreenAwaitUserDismiss(void);
void unlockScreenAwaitFadeIn(void);
void unlockScreenCountdownToExit(void);

s32 D_8008D960_8E560[] = { 0x00000000, 0x0000C350, 0x000186A0, 0x00000000, 0x0000AFC8, 0x00015F90, 0x00000000,
                           0x0000EA60, 0x0001D4C0, 0x000249F0, 0x00001388, 0x000003E8, 0x00013880, 0x00000064,
                           0x00013880, 0x0000C350, 0x00030D40, 0x00061A80, 0x00000000, 0x00000000 };

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
    /* 0x5C2 */ u16 rotationAngle;
    /* 0x5C4 */ u8 pad5C4;
    /* 0x5C5 */ u8 statePhase;
    /* 0x5C6 */ u8 scrollDirection;
    /* 0x5C7 */ u8 scrollStep;
    /* 0x5C8 */ s8 cursorIndex;
    /* 0x5C9 */ u8 unlockCount;
    /* 0x5CA */ u8 itemSlots[12];
    /* 0x5D6 */ u8 transitionState;
    /* 0x5D7 */ u8 delayCounter;
    /* 0x5D8 */ u8 unk5D8;
} UnlockScreenState;

INCLUDE_ASM("asm/nonmatchings/ui/unlock_screen", initUnlockScreen);

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

void updateUnlockScreen(void) {
    UnlockScreenState *state;
    s32 exitFlag;
    s32 sound;
    u8 prevCursor;
    u8 itemId;
    s32 channel;

    state = (UnlockScreenState *)getCurrentAllocation();
    exitFlag = 0;

    switch (state->statePhase) {
        case 1:
            prevCursor = state->cursorIndex;
            if (gControllerInputs & 0x80200) {
                state->cursorIndex = prevCursor - 1;
                state->scrollDirection = 1;
                if (state->cursorIndex < 0) {
                    if (state->unlockCount < 3) {
                        state->cursorIndex = prevCursor;
                        state->scrollDirection = 0;
                    } else {
                        state->cursorIndex = state->unlockCount - 1;
                    }
                }
            } else if (gControllerInputs & 0x40100) {
                state->cursorIndex = prevCursor + 1;
                state->scrollDirection = 2;
                if (state->cursorIndex == state->unlockCount) {
                    if (state->unlockCount >= 3) {
                        state->cursorIndex = 0;
                    } else {
                        state->cursorIndex = prevCursor;
                        state->scrollDirection = 0;
                    }
                }
            }

            sound = 0x2B;
            if (state->cursorIndex != prevCursor) {
                channel = 0;
                state->statePhase = 2;
                state->scrollStep = 0;
                goto play_sound;
            }

            itemId = state->itemSlots[state->cursorIndex];
            if ((itemId & 0xFF) < 0x80) {
                state->rotationAngle += 0x10;
            } else {
                state->rotationAngle = 0;
            }

            if (gControllerInputs & 0x8000) {
                if ((itemId & 0xFF) >= 0x80) {
                    state->statePhase = 8;
                    playSoundEffectOnChannelNoPriority(0xEE, 1);
                    state->transitionState = 3;
                } else if (D_800AFE8C_A71FC->gold >= D_8008D960_8E560[itemId & 0xFF]) {
                    sound = 0x2C;
                    channel = 0;
                    state->statePhase = 3;
                    state->frameCounter = 0;
                play_sound:
                    state->rotationAngle = 0;
                    playSoundEffectOnChannelNoPriority(sound, channel);
                } else {
                    playSoundEffectOnChannelNoPriority(0xEC, 1);
                    state->transitionState = 3;
                    state->statePhase = 7;
                }
            } else if (gControllerInputs & 0x4000) {
                exitFlag = 1;
                state->statePhase = 9;
                state->unk5D8 = 0;
                terminateTasksByType(1);
            }

            state->rotationAngle &= 0x1FFF;
            createYRotationMatrix((Transform3D *)state->rotationMatrix, state->rotationAngle);
            break;

        case 2:
            if (state->unlockCount >= 3) {
                itemId = 4;
            } else {
                itemId = state->unlockCount;
            }
            if (state->scrollStep != itemId) {
                break;
            }
        case 7:
        case 8:
            state->statePhase = 1;
        default:
            break;

        case 3:
            state->frameCounter++;
            if ((state->frameCounter & 0xFFFF) == 0x11) {
                state->frameCounter = 0;
                state->statePhase = 4;
                playSoundEffectOnChannelNoPriority(0xEF, 1);
            }
            break;

        case 4:
            if (gControllerInputs & 0x4000) {
                playSoundEffectOnChannelNoPriority(0x2E, 0);
                state->statePhase = 1;
            } else if (gControllerInputs & 0x8000) {
                playSoundEffectOnChannelNoPriority(0xEB, 1);
                state->transitionState = 1;
                state->statePhase = 5;
            }
            break;

        case 5:
            state->frameCounter++;
            if ((state->frameCounter & 0xFFFF) == 0x11) {
                state->frameCounter = 0;
                state->statePhase = 6;
            }
            break;

        case 6:
            itemId = state->itemSlots[state->cursorIndex];
            state->itemSlots[state->cursorIndex] = itemId + 0x80;
            if ((itemId & 0xFF) < 9) {
                EepromSaveData->character_or_settings[itemId & 0xFF] = (itemId & 0xFF) / 3 + 1;
            } else {
                EepromSaveData->character_or_settings[itemId & 0xFF] = itemId + 7;
            }
            addPlayerGold(-storyMapLocationNames[state->itemSlots[state->cursorIndex] + 19]);
            state->statePhase = 1;
            break;

        case 20:
            state->delayCounter++;
            if ((state->delayCounter & 0xFF) == 3) {
                state->statePhase = 1;
                scheduleTask(initStoryMapShopSoldOutLabel, 0, 0, 0x5A);
            }
            break;
    }

    if ((exitFlag & 0xFF) != 0) {
        state->transitionState = 2;
        playSoundEffectOnChannelNoPriority(0xED, 1);
        state->frameCounter = 0x28;
        setGameStateHandler(unlockScreenCountdownToExit);
    }
}

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

    unlinkNode((ViewportNode *)state);
    unlinkNode((ViewportNode *)&state->cameraNode1);
    unlinkNode((ViewportNode *)&state->cameraNode2);

    state->arrowSpriteAsset = freeNodeMemory(state->arrowSpriteAsset);
    state->backgroundAsset = freeNodeMemory(state->backgroundAsset);
    state->itemIconAsset = freeNodeMemory(state->itemIconAsset);
    state->digitSpriteAsset = freeNodeMemory(state->digitSpriteAsset);
    state->goldIconAsset = freeNodeMemory(state->goldIconAsset);
    state->itemLabelAsset = freeNodeMemory(state->itemLabelAsset);

    terminateSchedulerWithCallback(onUnlockScreenExit);
}

void onUnlockScreenExit(void) {
    returnToParentScheduler(1);
}
