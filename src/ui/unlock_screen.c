#include "ui/unlock_screen.h"

#include "assets.h"
#include "audio/audio.h"
#include "common_bss.h"
#include "gamestate.h"
#include "graphics/graphics.h"
#include "math/geometry.h"
#include "os_cont.h"
#include "race/track_geometry.h"
#include "story/map_events.h"
#include "story/shop_ui.h"
#include "system/rom_loader.h"
#include "system/task_scheduler.h"
#include "ui/save_data.h"

extern s32 storyMapLocationNames[];

void unlockScreenScheduleDisplayTasks(void);
void updateUnlockScreen(void);
void onUnlockScreenExit(void);
void unlockScreenCleanupAndExit(void);
void unlockScreenAwaitUserDismiss(void);
void unlockScreenAwaitFadeIn(void);
void unlockScreenCountdownToExit(void);
void waitForUnlocksAssetsReady(void);

s32 D_8008D960_8E560[] = { 0x00000000, 0x0000C350, 0x000186A0, 0x00000000, 0x0000AFC8, 0x00015F90, 0x00000000,
                           0x0000EA60, 0x0001D4C0, 0x000249F0, 0x00001388, 0x000003E8, 0x00013880, 0x00000064,
                           0x00013880, 0x0000C350, 0x00030D40, 0x00061A80, 0x00000000, 0x00000000 };

void initUnlockScreen(void) {
    GameState *state;
    ViewportNode *viewports;
    u8 lightBuffer[0x20];
    volatile s32 pad;
    StoryMapShopItemCardState *card;
    s32 unlockResult;
    u32 count;
    s32 i;

    state = allocateTaskMemory(0x5E0);
    viewports = (ViewportNode *)state;
    setupTaskSchedulerNodes(0x44, 6, 0, 0, 0, 0, 0, 0);
    state->modeData.unlockScreen.frameCounter = 0;
    state->modeData.unlockScreen.screenPhase = 0;
    state->modeData.unlockScreen.scrollDirection = 0;
    state->modeData.unlockScreen.completedScrollSteps = 0;
    state->modeData.unlockScreen.unlockedItemCount = 0;
    state->modeData.unlockScreen.pendingFairyAnimation = 0;
    state->modeData.unlockScreen.showItemIcons = 1;
    initMenuCameraNode(&viewports[0], 0, 10, 0);
    initMenuCameraNode(&viewports[1], 8, 15, 1);
    initMenuCameraNode(&viewports[2], 1, 8, 1);
    createViewportTransform(lightBuffer, 0, 0, 0x600000, 0, 0, 0);
    setViewportTransformById(((ViewportNode *)state)->viewportId, lightBuffer);
    setViewportFadeValue(NULL, 0xFF, 0);
    memcpy(&state->modeData.unlockScreen.itemRotation, &identityMatrix, sizeof(Transform3D));
    state->modeData.unlockScreen.itemRotationAngle = 0;
    state->modeData.unlockScreen.arrowSpriteAsset =
        loadCompressedData(&menuUiSprites_ROM_START, &playerCountSelectSprites_ROM_START, 0x8A08);
    state->modeData.unlockScreen.backgroundAsset =
        loadCompressedData(&okPromptSprites_ROM_START, &characterSelectBoardTexture_ROM_START, 0x1B48);
    state->modeData.unlockScreen.itemIconAsset =
        loadCompressedData(&shopBackgroundAsset_ROM_START, &levelSelectPortraits_ROM_START, 0x14410);
    state->modeData.unlockScreen.digitSpriteAsset =
        loadCompressedData(&uiCornerSprites_ROM_START, &uiCornerSprites_ROM_END, 0x1548);
    state->modeData.unlockScreen.goldIconAsset =
        loadCompressedData(&digit_sprite_ROM_START, &COSTUME_SLOT_00_COMPRESSED_DATA_ROM_START, 0x508);
    state->modeData.unlockScreen.itemLabelAsset =
        loadCompressedData(&goldIconSprite_ROM_START, &goldIconSprite_ROM_END, 0x388);
    scheduleTask(initStoryMapShopFairyModel, 0, 0, 0x5A);
    unlockResult = getLockedShopItemIndices(state->modeData.unlockScreen.itemIds);
    __asm__("" : "=r"(count) : "0"(unlockResult & 0xFF));
    state->modeData.unlockScreen.unlockedItemCount = unlockResult;
    if (count >= 3) {
        count = 3;
    }

    i = 0;
    if (count != 0) {
        do {
            card = (StoryMapShopItemCardState *)scheduleTask(initStoryMapShopItemCard, 0, 0, 0x5A);
            if (card != NULL) {
                card->itemIndex = i;
                card->slotPosition = i;
                if (state->modeData.unlockScreen.unlockedItemCount == 2) {
                    card->slotPosition = i + 1;
                }
            }
            i++;
        } while (i < (s32)count);
    }

    if (state->modeData.unlockScreen.unlockedItemCount >= 3) {
        scheduleTask(initSlideInStoryMapShopItemCard, 0, 0, 0x5A);
    }
    scheduleTask(loadStoryMapShopBackground, 0, 0, 0x5A);
    scheduleTask(initStoryMapShopGoldDisplay, 0, 0, 0x5A);
    if (state->modeData.unlockScreen.unlockedItemCount != 0) {
        scheduleTask(initStoryMapShopItemIcon, 1, 0, 0x5A);
        scheduleTask(initStoryMapShopItemStatLabel, 1, 0, 0x5A);
        scheduleTask(initStoryMapShopExitOverlay, 0, 0, 0x5A);
    }
    if ((s32)count >= 3) {
        state->modeData.unlockScreen.selectedItemIndex = 1;
    } else {
        state->modeData.unlockScreen.selectedItemIndex = 0;
    }
    setGameStateHandler(waitForUnlocksAssetsReady);
}

void waitForUnlocksAssetsReady(void) {
    GameState *state = getCurrentAllocation();

    state->modeData.unlockScreen.frameCounter++;
    if (state->modeData.unlockScreen.frameCounter < 3) {
        return;
    }

    state->modeData.unlockScreen.frameCounter = 2;
    if (getPendingDmaCount() != 0) {
        return;
    }

    state->modeData.unlockScreen.frameCounter = 0;
    setViewportFadeValue(0, 0, 0xE);
    setGameStateHandler(unlockScreenAwaitFadeIn);
}

void unlockScreenAwaitFadeIn(void) {
    GameState *state = getCurrentAllocation();

    if (getViewportFadeMode(NULL) != 0) {
        return;
    }

    if (state->modeData.unlockScreen.unlockedItemCount != 0) {
        state->modeData.unlockScreen.pendingFairyAnimation = 1;
        playSoundEffectOnChannelNoPriority(0xEA, 1);
        setGameStateHandler(unlockScreenScheduleDisplayTasks);
    } else {
        state->modeData.unlockScreen.pendingFairyAnimation = 3;
        playSoundEffectOnChannelNoPriority(0xEE, 1);
        setGameStateHandler(unlockScreenAwaitUserDismiss);
        scheduleTask(initStoryMapShopSoldOutLabel, 0, 0, 0x5A);
    }
}

void unlockScreenScheduleDisplayTasks(void) {
    GameState *state = getCurrentAllocation();

    if (state->modeData.unlockScreen.pendingFairyAnimation != 0) {
        return;
    }

    state->modeData.unlockScreen.screenPhase = 0x14;
    scheduleTask(initStoryMapShopItemPriceDisplay, 1, 0, 0x5A);
    scheduleTask(initStoryMapShopItemStatsDisplay, 1, 0, 0x5A);
    scheduleTask(initUnlockScreenItemIcons, 0, 0, 0x5A);

    if (state->modeData.unlockScreen.unlockedItemCount >= 2) {
        scheduleTask(initUnlockScreenScrollArrows, 1, 0, 0x5A);
    }

    setGameStateHandler(updateUnlockScreen);
}

void unlockScreenAwaitUserDismiss(void) {
    GameState *state = getCurrentAllocation();

    if (state->modeData.unlockScreen.pendingFairyAnimation != 0) {
        return;
    }

    if (gControllerInputs[0] & (CONT_A | CONT_B | CONT_START)) {
        playSoundEffectOnChannelNoPriority(0xED, 1);
        state->modeData.unlockScreen.pendingFairyAnimation = 2;
        setViewportFadeValue(0, 0xFF, 0x10);
        setGameStateHandler(unlockScreenCleanupAndExit);
    }
}

void updateUnlockScreen(void) {
    GameState *state;
    s32 exitFlag;
    s32 sound;
    u8 prevCursor;
    u8 itemId;
    s32 channel;

    state = getCurrentAllocation();
    exitFlag = 0;

    switch (state->modeData.unlockScreen.screenPhase) {
        case 1:
            prevCursor = state->modeData.unlockScreen.selectedItemIndex;
            if (gControllerInputs[0] & (STICK_LEFT | CONT_LEFT)) {
                state->modeData.unlockScreen.selectedItemIndex = prevCursor - 1;
                state->modeData.unlockScreen.scrollDirection = 1;
                if (state->modeData.unlockScreen.selectedItemIndex < 0) {
                    if (state->modeData.unlockScreen.unlockedItemCount < 3) {
                        state->modeData.unlockScreen.selectedItemIndex = prevCursor;
                        state->modeData.unlockScreen.scrollDirection = 0;
                    } else {
                        state->modeData.unlockScreen.selectedItemIndex =
                            state->modeData.unlockScreen.unlockedItemCount - 1;
                    }
                }
            } else if (gControllerInputs[0] & (STICK_RIGHT | CONT_RIGHT)) {
                state->modeData.unlockScreen.selectedItemIndex = prevCursor + 1;
                state->modeData.unlockScreen.scrollDirection = 2;
                if (state->modeData.unlockScreen.selectedItemIndex == state->modeData.unlockScreen.unlockedItemCount) {
                    if (state->modeData.unlockScreen.unlockedItemCount >= 3) {
                        state->modeData.unlockScreen.selectedItemIndex = 0;
                    } else {
                        state->modeData.unlockScreen.selectedItemIndex = prevCursor;
                        state->modeData.unlockScreen.scrollDirection = 0;
                    }
                }
            }

            sound = 0x2B;
            if (state->modeData.unlockScreen.selectedItemIndex != prevCursor) {
                channel = 0;
                state->modeData.unlockScreen.screenPhase = 2;
                state->modeData.unlockScreen.completedScrollSteps = 0;
                goto play_sound;
            }

            itemId = state->modeData.unlockScreen.itemIds[state->modeData.unlockScreen.selectedItemIndex];
            if ((itemId & 0xFF) < 0x80) {
                state->modeData.unlockScreen.itemRotationAngle += 0x10;
            } else {
                state->modeData.unlockScreen.itemRotationAngle = 0;
            }

            if (gControllerInputs[0] & CONT_A) {
                if ((itemId & 0xFF) >= 0x80) {
                    state->modeData.unlockScreen.screenPhase = 8;
                    playSoundEffectOnChannelNoPriority(0xEE, 1);
                    state->modeData.unlockScreen.pendingFairyAnimation = 3;
                } else if (gGameSessionContext->gold >= D_8008D960_8E560[itemId & 0xFF]) {
                    sound = 0x2C;
                    channel = 0;
                    state->modeData.unlockScreen.screenPhase = 3;
                    state->modeData.unlockScreen.frameCounter = 0;
                play_sound:
                    state->modeData.unlockScreen.itemRotationAngle = 0;
                    playSoundEffectOnChannelNoPriority(sound, channel);
                } else {
                    playSoundEffectOnChannelNoPriority(0xEC, 1);
                    state->modeData.unlockScreen.pendingFairyAnimation = 3;
                    state->modeData.unlockScreen.screenPhase = 7;
                }
            } else if (gControllerInputs[0] & CONT_B) {
                exitFlag = 1;
                state->modeData.unlockScreen.screenPhase = 9;
                state->modeData.unlockScreen.showItemIcons = 0;
                terminateTasksByType(1);
            }

            state->modeData.unlockScreen.itemRotationAngle &= 0x1FFF;
            createYRotationMatrix(
                &state->modeData.unlockScreen.itemRotation,
                state->modeData.unlockScreen.itemRotationAngle
            );
            break;

        case 2:
            if (state->modeData.unlockScreen.unlockedItemCount >= 3) {
                itemId = 4;
            } else {
                itemId = state->modeData.unlockScreen.unlockedItemCount;
            }
            if (state->modeData.unlockScreen.completedScrollSteps != itemId) {
                break;
            }
        case 7:
        case 8:
            state->modeData.unlockScreen.screenPhase = 1;
        default:
            break;

        case 3:
            state->modeData.unlockScreen.frameCounter++;
            if ((state->modeData.unlockScreen.frameCounter & 0xFFFF) == 0x11) {
                state->modeData.unlockScreen.frameCounter = 0;
                state->modeData.unlockScreen.screenPhase = 4;
                playSoundEffectOnChannelNoPriority(0xEF, 1);
            }
            break;

        case 4:
            if (gControllerInputs[0] & CONT_B) {
                playSoundEffectOnChannelNoPriority(0x2E, 0);
                state->modeData.unlockScreen.screenPhase = 1;
            } else if (gControllerInputs[0] & CONT_A) {
                playSoundEffectOnChannelNoPriority(0xEB, 1);
                state->modeData.unlockScreen.pendingFairyAnimation = 1;
                state->modeData.unlockScreen.screenPhase = 5;
            }
            break;

        case 5:
            state->modeData.unlockScreen.frameCounter++;
            if ((state->modeData.unlockScreen.frameCounter & 0xFFFF) == 0x11) {
                state->modeData.unlockScreen.frameCounter = 0;
                state->modeData.unlockScreen.screenPhase = 6;
            }
            break;

        case 6:
            itemId = state->modeData.unlockScreen.itemIds[state->modeData.unlockScreen.selectedItemIndex];
            state->modeData.unlockScreen.itemIds[state->modeData.unlockScreen.selectedItemIndex] = itemId + 0x80;
            if ((itemId & 0xFF) < 9) {
                EepromSaveData->characterPaletteIds[itemId & 0xFF] = (itemId & 0xFF) / 3 + 1;
            } else {
                EepromSaveData->characterPaletteIds[itemId & 0xFF] = itemId + 7;
            }
            addPlayerGold(
                -storyMapLocationNames
                    [state->modeData.unlockScreen.itemIds[state->modeData.unlockScreen.selectedItemIndex] + 19]
            );
            state->modeData.unlockScreen.screenPhase = 1;
            break;

        case 20:
            state->modeData.unlockScreen.delayCounter++;
            if ((state->modeData.unlockScreen.delayCounter & 0xFF) == 3) {
                state->modeData.unlockScreen.screenPhase = 1;
                scheduleTask(initStoryMapShopSoldOutLabel, 0, 0, 0x5A);
            }
            break;
    }

    if ((exitFlag & 0xFF) != 0) {
        state->modeData.unlockScreen.pendingFairyAnimation = 2;
        playSoundEffectOnChannelNoPriority(0xED, 1);
        state->modeData.unlockScreen.frameCounter = 0x28;
        setGameStateHandler(unlockScreenCountdownToExit);
    }
}

void unlockScreenCountdownToExit(void) {
    GameState *state = getCurrentAllocation();

    state->modeData.unlockScreen.frameCounter--;
    if (state->modeData.unlockScreen.frameCounter == 0) {
        setViewportFadeValue(0, 0xFF, 0x10);
        setGameStateHandler(unlockScreenCleanupAndExit);
    }

    if (state->modeData.unlockScreen.pendingFairyAnimation != 0) {
        state->modeData.unlockScreen.pendingFairyAnimation = 0;
    }
}

void unlockScreenCleanupAndExit(void) {
    GameState *state = getCurrentAllocation();
    ViewportNode *viewports = (ViewportNode *)state;

    if (getViewportFadeMode(NULL) != 0) {
        return;
    }

    unlinkNode(&viewports[0]);
    unlinkNode(&viewports[1]);
    unlinkNode(&viewports[2]);

    state->modeData.unlockScreen.arrowSpriteAsset = freeNodeMemory(state->modeData.unlockScreen.arrowSpriteAsset);
    state->modeData.unlockScreen.backgroundAsset = freeNodeMemory(state->modeData.unlockScreen.backgroundAsset);
    state->modeData.unlockScreen.itemIconAsset = freeNodeMemory(state->modeData.unlockScreen.itemIconAsset);
    state->modeData.unlockScreen.digitSpriteAsset = freeNodeMemory(state->modeData.unlockScreen.digitSpriteAsset);
    state->modeData.unlockScreen.goldIconAsset = freeNodeMemory(state->modeData.unlockScreen.goldIconAsset);
    state->modeData.unlockScreen.itemLabelAsset = freeNodeMemory(state->modeData.unlockScreen.itemLabelAsset);

    terminateSchedulerWithCallback(onUnlockScreenExit);
}

void onUnlockScreenExit(void) {
    returnToParentScheduler(1);
}
