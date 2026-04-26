#include "story/race_state_machine.h"

#include "D_800AFE8C_A71FC_type.h"
#include "EepromSaveData_type.h"
#include "audio/audio.h"
#include "common.h"
#include "core/session_manager.h"
#include "effects/cutscene_keyframes.h"
#include "gamestate.h"
#include "graphics/graphics.h"
#include "math/geometry.h"
#include "os_cont.h"
#include "race/race_session.h"
#include "race/track_geometry.h"
#include "story/map_events.h"
#include "system/rom_loader.h"
#include "system/task_scheduler.h"
#include "text/font_assets.h"
#include "text/font_render.h"
#include "ui/level_preview_3d.h"

typedef struct {
    /* 0x000 */ ViewportNode mainViewport;
    /* 0x1D8 */ ViewportNode secondaryViewport;
    /* 0x3B0 */ ViewportNode tertiaryViewport;
    /* 0x588 */ ViewportNode quaternaryViewport;
    /* 0x760 */ void *assetSlot0;
    /* 0x764 */ void *assetSlot1;
    /* 0x768 */ void *assetSlot2;
    /* 0x76C */ void *assetSlot3;
    /* 0x770 */ void *assetSlot4;
    /* 0x774 */ void *textRenderAsset;
    /* 0x778 */ void *assetSlot5;
    /* 0x77C */ u16 delayTimer;
    /* 0x77E */ u16 shopkeeperAnimIndex;
    /* 0x780 */ u8 pad780[4];
    /* 0x784 */ u8 boardDisplayIndices[4];
    /* 0x788 */ u8 boardIndexMap[13];
    /* 0x795 */ u8 unlockedBoardsInCategory[3];
    /* 0x798 */ u8 totalBoardCount;
    /* 0x799 */ u8 selectedSlot;
    /* 0x79A */ u8 exitMode;
    /* 0x79B */ u8 shopState;
    /* 0x79C */ u8 scrollDirection;
    /* 0x79D */ s8 transitionDirection;
    /* 0x79E */ u8 newTransitionIndex;
    /* 0x79F */ u8 oldTransitionIndex;
    /* 0x7A0 */ u8 scrollOutBoardIndex;
    /* 0x7A1 */ s8 selectedCategoryIndex;
    /* 0x7A2 */ u8 selectedBoardIndex;
    /* 0x7A3 */ u8 forceShopkeeperAnimUpdate;
    /* 0x7A4 */ u8 viewMode;
} BoardShopState;

s16 boardShopPrices[] = { 0x0064, 0x0064, 0x0064, 0x00FA, 0x00FA, 0x012C, 0x012C, 0x0190,
                          0x0190, 0x01C2, 0x01C2, 0x01F4, 0x01F4, 0x0320, 0x03E8, 0x04B0 };

extern D_8008D7FC_8E3FC_item D_8008D7FC_8E3FC[];
extern u8 storyMapLocationIndex;
extern s32 gControllerInputs[4];

USE_ASSET(_4237C0);
USE_ASSET(_426EF0);
USE_ASSET(_4488E0);
USE_ASSET(_4547D0);
USE_ASSET(_419C60);
USE_ASSET(_41A1D0);
USE_ASSET(font_race_timer);
USE_ASSET(_3F6BB0);
USE_ASSET(_3F6670);

void awaitFadeLoadCharacterSelect(void);
void awaitFadeLoadBoardShop(void);
void awaitBoardShopDmaComplete(void);
extern void initBoardShopShopkeeper(void);
extern void initBoardShopCharacterPreview(void);
extern void loadBoardShopBackground(void);
extern void initBoardShopExitOverlay(void);
extern void initBoardShopComparisonIcons(void);
extern void initBoardShopRowSelectorArrow(void);
extern void initBoardShopColumnSelectorArrow(void);
extern void initBoardShopTitleText(void);
extern void initBoardShopTitleCorners(void);
extern void initBoardShopCharacterTransition(void);
extern void initBoardShopBoardIcons(void);
extern void initBoardShopSnowflakeSlideIn(void);
extern void initBoardShopPreviewWipe(void);
extern void initBoardShopSnowParticles(void);
void updateBoardShop(void);
void awaitBoardShopExitDelay(void);
u8 countOwnedBoardsInCategory(void);
void advanceBoardDisplaySlots(void);
void onStoryModeRaceCancelled(void);
void awaitStoryMapLocationIntro(void);
void awaitStoryMapDecorReady(void);
void cleanupStoryMapDecorModel(applyTransformToModel_arg1 *);
void setupStoryMapDecorModel(applyTransformToModel_arg1 *);
void awaitFadeCleanupBoardShop(void);
void onBoardShopCleanupComplete(void);
void awaitStoryModeUnlockCutscene(void);
void loadStoryModeUnlockCutscene(void);
void awaitStoryModeRaceResult(void);
void awaitStoryModePreRaceCutscene(void);
void awaitStoryModeRaceLevelSelect(void);
void updateStoryMapDecorModel(applyTransformToModel_arg1 *arg0);
void enqueueStoryMapLocationText(TextData *arg0);
void awaitFadeLoadStoryMap(void);
void awaitStoryModeCharacterSelect(void);
void awaitFadeLoadPreRaceCutscene(void);
void loadStoryModeRace(void);
void onStoryMapLocationIntroComplete(void);
void startBoardShopFadeOut(void);

void storyMapDecorModelTask(StoryMapDecorModelState *arg0) {
    arg0->model = createSceneModel(0x13, getCurrentAllocation());
    setCleanupCallback(&cleanupStoryMapDecorModel);
    setCallback(&awaitStoryMapDecorReady);
}

void awaitStoryMapDecorReady(void) {
    if (((GameState *)getCurrentAllocation())->unk429 == 0) {
        setCallback(&setupStoryMapDecorModel);
    }
}

void setupStoryMapDecorModel(applyTransformToModel_arg1 *arg0) {
    getCurrentAllocation();
    memcpy(arg0, &identityMatrix, 0x20);
    arg0->unk0.translation.x = 0x830000;
    arg0->unk0.translation.y = 0x340000;
    arg0->unk0.translation.z = 0xFF7D0000;
    applyTransformToModel(arg0->unk20_u.unk20, &arg0->unk0);
    setModelAnimation(arg0->unk20_u.unk20, 0);
    updateModelGeometry(arg0->unk20_u.unk20);
    setCleanupCallback(&cleanupStoryMapDecorModel);
    setCallback(&updateStoryMapDecorModel);
}

void updateStoryMapDecorModel(applyTransformToModel_arg1 *arg0) {
    getCurrentAllocation();
    clearModelRotation(arg0->unk20_u.unk20);
    updateModelGeometry(arg0->unk20_u.unk20);
}

void cleanupStoryMapDecorModel(applyTransformToModel_arg1 *arg0) {
    destroySceneModel(arg0->unk20_u.unk20);
}

void storyMapLocationTextTask(func_80019CD0_1A8D0_arg *arg0) {
    arg0->unk0 = -0x28;
    arg0->unk2 = 0;
    arg0->unk4 = 0;
    arg0->unk8 = &D_8008D7FC_8E3FC[storyMapLocationIndex];
    setCallback(&enqueueStoryMapLocationText);
}

void enqueueStoryMapLocationText(TextData *arg0) {
    debugEnqueueCallback(0, 7, &renderTextPalette, arg0);
}

void initStoryModeRace(void) {
    clearMemory(allocateTaskMemory(0x1E0), 0x1E0);
    setGameStateHandler(&awaitFadeLoadStoryMap);
}

void awaitFadeLoadStoryMap(void) {
    if (getViewportFadeMode(0) == 0) {
        createTaskQueue(&initLevelSelectWithDetail, 100);
        setGameStateHandler(&awaitStoryModeRaceLevelSelect);
    }
}

void awaitStoryModeRaceLevelSelect(void) {
    s16 selectionResult = getSchedulerReturnValue();

    if (selectionResult != 0) {
        if (selectionResult == 0xFF) {
            terminateSchedulerWithCallback(&onStoryModeRaceCancelled);
            return;
        }

        setGameStateHandler(&awaitFadeLoadCharacterSelect);
    }
}

void awaitFadeLoadCharacterSelect(void) {
    if (getViewportFadeMode(0) == 0) {
        playMusicTrack(2);
        createTaskQueue(&initCharacterSelectScreen, 100);
        setGameStateHandler(&awaitStoryModeCharacterSelect);
    }
}

void awaitStoryModeCharacterSelect(void) {
    s16 selectionResult = getSchedulerReturnValue();
    if (selectionResult != 0) {
        void *nextHandler = &awaitFadeLoadPreRaceCutscene;
        if (selectionResult == 0xFF) {
            nextHandler = &awaitFadeLoadStoryMap;
        }
        setGameStateHandler(nextHandler);
    }
}

void awaitFadeLoadPreRaceCutscene(void) {
    if (getViewportFadeMode(0) == 0) {
        setCutsceneSelection(D_800AFE8C_A71FC->saveSlotIndex, 0);
        createTaskQueue(&loadCutsceneOverlay, 150);
        setGameStateHandler(&awaitStoryModePreRaceCutscene);
    }
}

void awaitStoryModePreRaceCutscene(void) {
    if ((getSchedulerReturnValue() << 0x10) != 0) {
        setGameStateHandler(&loadStoryModeRace);
    }
}

void loadStoryModeRace(void) {
    createTaskQueue(&initRace, 100);
    setGameStateHandler(&awaitStoryModeRaceResult);
}

void awaitStoryModeRaceResult(void) {
    s16 raceResult;
    GameSessionContext *gameState;
    u8 saveSlotIndex;

    raceResult = getSchedulerReturnValue();

    if (raceResult == 0) {
        return;
    }

    if (raceResult >= 3) {
        if (raceResult == 5 || raceResult >= 7) {
            saveSlotIndex = D_800AFE8C_A71FC->saveSlotIndex;
            do {
                EepromSaveData->save_slot_status[saveSlotIndex] = 1;
            } while (0);
            processRaceUnlocks(raceResult);
        } else {
            gameState = D_800AFE8C_A71FC;
            saveSlotIndex = gameState->saveSlotIndex;
            if (EepromSaveData->save_slot_status[saveSlotIndex] != 1) {
                EepromSaveData->save_slot_status[saveSlotIndex] = 4;
            }
        }
    }

    updateStorySlotUnlockStatus();

    if (raceResult == 1) {
        setGameStateHandler(loadStoryModeRace);
        return;
    }

    if (D_800AFE8C_A71FC->pendingUnlockCutscene != 0) {
        setGameStateHandler(loadStoryModeUnlockCutscene);
        return;
    }

    terminateSchedulerWithCallback(onStoryModeRaceCancelled);
}

void loadStoryModeUnlockCutscene(void) {
    setCutsceneSelection(D_800AFE8C_A71FC->pendingUnlockCutscene, 2);
    createTaskQueue(&loadCutsceneOverlay, 0x64);
    setGameStateHandler(&awaitStoryModeUnlockCutscene);
}

void awaitStoryModeUnlockCutscene(void) {
    if ((getSchedulerReturnValue() << 0x10) != 0) {
        D_800AFE8C_A71FC->pendingUnlockCutscene = 0;
        terminateSchedulerWithCallback(&onStoryModeRaceCancelled);
    }
}

void onStoryModeRaceCancelled(void) {
    returnToParentScheduler(0xFF);
}

void initBoardShopDisplay(void) {
    BoardShopState *state;
    Transform3D viewportTransform;
    u8 *ptr;
    s32 i;

    state = (BoardShopState *)allocateTaskMemory(0x7A8);
    setupTaskSchedulerNodes(0x14, 0, 0, 0, 0, 0, 0, 0);
    state->delayTimer = 0;
    state->selectedSlot = 0;
    state->exitMode = 0;
    state->selectedBoardIndex = 0;
    state->selectedCategoryIndex = 0;
    state->oldTransitionIndex = 0;
    state->newTransitionIndex = 0;
    state->shopkeeperAnimIndex = 0;
    state->shopState = 0;
    initMenuCameraNode(&state->mainViewport, 0, 0xA, 0);
    initMenuCameraNode(&state->secondaryViewport, 2, 0x14, 0);
    initMenuCameraNode(&state->tertiaryViewport, 8, 0x14, 1);
    initMenuCameraNode(&state->quaternaryViewport, 9, 5, 1);
    createViewportTransform(&viewportTransform, 0, 0, 0x580000, 0, 0, 0);
    setViewportTransformById(state->mainViewport.id, &viewportTransform);
    setViewportTransformById(state->secondaryViewport.id, &viewportTransform);
    state->assetSlot0 = loadCompressedData(&_4237C0_ROM_START, &_426EF0_ROM_START, 0x8A08);
    state->assetSlot1 = loadCompressedData(&_4488E0_ROM_START, &_4488E0_ROM_END, 0x14410);
    state->assetSlot2 = loadCompressedData(&_4547D0_ROM_START, &_4547D0_ROM_END, 0x9488);
    state->assetSlot3 = loadCompressedData(&_419C60_ROM_START, &_41A1D0_ROM_START, 0x1548);
    state->assetSlot4 = loadCompressedData(&font_race_timer_ROM_START, &_3F6BB0_ROM_START, 0x508);
    state->assetSlot5 = loadCompressedData(&_3F6670_ROM_START, &_3F6670_ROM_END, 0x388);
    state->textRenderAsset = loadTextRenderAsset(1);
    i = 3;
    ptr = (u8 *)state + 3;
    do {
        ptr[0x784] = i;
        i--;
        ptr--;
    } while (i >= 0);
    state->totalBoardCount = 0;
    for (i = 0; i < 13; i++) {
        state->totalBoardCount++;
        state->boardIndexMap[i] = i;
    }
    for (i = 0; i < 3; i++) {
        if (EepromSaveData->setting_4B[i] != 0) {
            state->totalBoardCount++;
            state->unlockedBoardsInCategory[i] = EepromSaveData->setting_4B[i];
        }
    }
    scheduleTask(&initBoardShopShopkeeper, 0, 0, 0x63);
    scheduleTask(&initBoardShopCharacterPreview, 0, 0, 0x5A);
    scheduleTask(&loadBoardShopBackground, 0, 0, 0x5A);
    scheduleTask(&initBoardShopExitOverlay, 0, 0, 0x5A);
    setViewportFadeValue(0, 0xFF, 0);
    state->viewMode = 0;
    state->delayTimer = 2;
    setGameStateHandler(&awaitBoardShopDmaComplete);
}

void awaitBoardShopDmaComplete(void) {
    BoardShopState *state;

    state = (BoardShopState *)getCurrentAllocation();
    if (state->delayTimer != 0U) {
        state->delayTimer--;
        return;
    }
    if (getPendingDmaCount() == 0) {
        setViewportFadeValue(0, 0, 0x10);
        setGameStateHandler(&awaitFadeLoadBoardShop);
    }
}

void awaitFadeLoadBoardShop(void) {
    if (getViewportFadeMode(0) == 0) {
        scheduleTask(&initBoardShopGoldDisplay, 0, 0, 0x5A);
        setGameStateHandler(&updateBoardShop);
    }
}

void updateBoardShop(void) {
    BoardShopState *state;
    u8 oldValue;
    u8 boardCount;
    u8 boardIdx;
    s8 transDir;

    state = (BoardShopState *)getCurrentAllocation();

    switch (state->shopState) {
        case 0x00:
            playSoundEffectOnChannelNoPriority(0xEA, 1);
            state->shopState = 0x01;
            break;

        case 0x0C:
            scheduleTask(&initBoardShopComparisonIcons, 0, 0, 0x5A);
            scheduleTask(&initBoardShopRowSelectorArrow, 0, 0, 0x5A);
            scheduleTask(&initBoardShopColumnSelectorArrow, 0, 0, 0x5A);
            state->viewMode = 1;
            scheduleTask(&initBoardShopTitleText, 0, 0, 0x5A);
            scheduleTask(&initBoardShopTitleCorners, 0, 0, 0x5A);
            state->shopState = 0x02;
            break;

        case 0x02:
            oldValue = state->selectedCategoryIndex;
            if (*gControllerInputs & (STICK_LEFT | L_JPAD)) {
                state->scrollDirection = 0;
                state->selectedCategoryIndex = state->selectedCategoryIndex - 1;
                if (state->selectedCategoryIndex < 0) {
                    state->selectedCategoryIndex = 2;
                }
            } else if (*gControllerInputs & (STICK_RIGHT | R_JPAD)) {
                state->scrollDirection = 1;
                state->selectedCategoryIndex = state->selectedCategoryIndex + 1;
                if (state->selectedCategoryIndex >= 3) {
                    state->selectedCategoryIndex = 0;
                }
            }
            if ((s8)oldValue != state->selectedCategoryIndex) {
                playSoundEffectOnChannelNoPriority(0x2B, 0);
                state->oldTransitionIndex = (s8)oldValue * 3;
                state->shopState = 0x03;
                state->transitionDirection = 1;
                state->selectedBoardIndex = 0;
                state->newTransitionIndex = state->selectedCategoryIndex * 3;
                scheduleTask(&initBoardShopCharacterTransition, 0, 0, 0x5A);
            } else {
                if (*gControllerInputs & (A_BUTTON | START_BUTTON)) {
                    playSoundEffectOnChannelNoPriority(0x2C, 0);
                    state->delayTimer = 0;
                    state->shopState = 0x04;
                    break;
                }
                if (*gControllerInputs & B_BUTTON) {
                    state->exitMode = 1;
                }
            }
            break;

        case 0x03:
            transDir = state->transitionDirection;
            if (transDir == 0) {
                state->shopState = 0x02;
                break;
            }
            if (transDir == -2) {
                state->shopState = 0x05;
            }
            break;

        case 0x04:
        case 0x07:
            state->delayTimer = state->delayTimer + 1;
            if ((state->delayTimer & 0xFFFF) == 0x11) {
                state->delayTimer = 0;
                if (state->shopState == 0x07) {
                    state->viewMode = 0;
                    state->shopState = 0x0F;
                    state->delayTimer = 0;
                    scheduleTask(&initBoardShopBoardIcons, 0, 0, 0x5A);
                    {
#ifdef CC_CHECK
                        s32 j = 3;
                        s32 ptr = (s32)state + 3;
#else
                        register s32 j __asm__("$2") = 3;
                        register s32 ptr __asm__("$3") = (s32)state + 3;
#endif
                        do {
                            *(u8 *)(ptr + 0x784) = j;
                            j--;
                            ptr--;
                        } while (j >= 0);
                    }
                } else {
                    state->shopState = 0x05;
                }
            }
            break;

        case 0x05:
            boardCount = countOwnedBoardsInCategory();
            oldValue = state->selectedBoardIndex;
            if (*gControllerInputs & (STICK_LEFT | L_JPAD)) {
                state->scrollDirection = 0;
                state->selectedBoardIndex = state->selectedBoardIndex - 1;
                if ((s8)state->selectedBoardIndex < 0) {
                    state->selectedBoardIndex = boardCount - 1;
                }
            } else if (*gControllerInputs & (STICK_RIGHT | R_JPAD)) {
                state->scrollDirection = 1;
                state->selectedBoardIndex = state->selectedBoardIndex + 1;
                if ((s8)state->selectedBoardIndex == (boardCount & 0xFF)) {
                    state->selectedBoardIndex = 0;
                }
            }
            if ((s8)oldValue != (s8)state->selectedBoardIndex) {
                playSoundEffectOnChannelNoPriority(0x2B, 0);
                state->shopState = 0x03;
                state->transitionDirection = -1;
                state->oldTransitionIndex = oldValue + (state->selectedCategoryIndex * 3);
                state->newTransitionIndex = state->selectedBoardIndex + (state->selectedCategoryIndex * 3);
                scheduleTask(&initBoardShopCharacterTransition, 0, 0, 0x5A);
            } else {
                if (*gControllerInputs & (A_BUTTON | START_BUTTON)) {
                    playSoundEffectOnChannelNoPriority(0x2C, 0);
                    state->delayTimer = 0;
                    state->shopState = 0x07;
                    break;
                }
                if (*gControllerInputs & B_BUTTON) {
                    playSoundEffect(0x2E);
                    state->shopState = 0x02;
                }
            }
            break;

        case 0x0F:
            if (state->delayTimer != 0) {
                state->delayTimer = 0;
                state->selectedSlot = 0;
                setModelCameraTransform(&state->tertiaryViewport, 0, 0, -0x98, -0x4D, 0x97, 0x5A);
                state->shopState = 0x10;
                state->viewMode = 2;
            }
            break;

        case 0x10:
            oldValue = state->selectedSlot;
            state->scrollDirection = 0;
            if (*gControllerInputs & (STICK_UP | U_JPAD)) {
                if (state->selectedSlot == 0) {
                    state->scrollDirection = 2;
                    state->scrollOutBoardIndex = state->boardDisplayIndices[3];
                    advanceBoardDisplaySlots();
                    state->shopState = 0x12;
                } else {
                    state->selectedSlot = state->selectedSlot - 1;
                }
            } else if (*gControllerInputs & (STICK_DOWN | D_JPAD)) {
                if ((state->selectedSlot & 0xFF) == 3) {
                    state->scrollDirection = 1;
                    state->scrollOutBoardIndex = state->boardDisplayIndices[0];
                    advanceBoardDisplaySlots();
                    state->shopState = 0x12;
                } else {
                    state->selectedSlot = state->selectedSlot + 1;
                }
            }
            if (((s8)oldValue != state->selectedSlot) || (state->scrollDirection != 0)) {
                playSoundEffectOnChannelNoPriority(0x2B, 0);
                state->delayTimer = 0;
            } else if (*gControllerInputs & B_BUTTON) {
                playSoundEffect(0x2E);
                state->shopState = 0x11;
                state->viewMode = 0;
                state->delayTimer = 0;
                setModelCameraTransform(&state->tertiaryViewport, 0, 0, -0x98, -0x70, 0x97, 0x6F);
            } else if (*gControllerInputs & (A_BUTTON | START_BUTTON)) {
                if (D_800AFE8C_A71FC->gold >=
                    (s32)(u16)boardShopPrices[state->boardIndexMap[state->boardDisplayIndices[state->selectedSlot]]]) {
                    playSoundEffectOnChannelNoPriority(0x2C, 0);
                    state->shopState = 0x14;
                    state->shopkeeperAnimIndex = 1;
                    state->delayTimer = 0;
                } else {
                    state->delayTimer = state->delayTimer;
                    playSoundEffectOnChannelNoPriority(0xEC, 1);
                    state->shopkeeperAnimIndex = 2;
                }
            } else {
                if (state->delayTimer < 5) {
                    state->delayTimer = state->delayTimer + 1;
                }
            }
            break;

        case 0x11:
            if (state->delayTimer != 0) {
                state->shopState = 0x02;
                state->viewMode = 1;
            }
            break;

        case 0x12:
            scheduleTask(&initBoardShopSnowflakeSlideIn, 0, 0, 0x5A);
            state->shopState = 0x13;
            break;

        case 0x14:
            state->delayTimer = state->delayTimer + 1;
            if ((state->delayTimer & 0xFFFF) == 0x11) {
                playSoundEffectOnChannelNoPriority(0xEF, 1);
                state->delayTimer = 0;
                state->shopState = 0x19;
            }
            break;

        case 0x19:
            if (*gControllerInputs & B_BUTTON) {
                playSoundEffect(0x2E);
                state->shopState = 0x10;
                break;
            }
            if (*gControllerInputs & A_BUTTON) {
                state->shopState = 0x1A;
                state->shopkeeperAnimIndex = 3;
                state->viewMode = 0;
                state->forceShopkeeperAnimUpdate = 1;
                scheduleTask(&initBoardShopPreviewWipe, 0, 0, 0x59);
                scheduleTask(&initBoardShopSnowParticles, 0, 0, 0x5F);
            }
            break;

        case 0x1A:
            state->shopState = 0x1B;
            state->delayTimer = 0;
            break;

        case 0x1B:
            if (state->delayTimer != 0) {
                state->shopState = 0x1C;
                state->delayTimer = 0;
                state->shopkeeperAnimIndex = 1;
                boardIdx = state->boardDisplayIndices[state->selectedSlot];
                boardIdx = state->boardIndexMap[boardIdx];
                EepromSaveData
                    ->character_or_settings[(state->selectedCategoryIndex * 3) + (s8)state->selectedBoardIndex] =
                    boardIdx + 1;
            }
            break;

        case 0x1C:
            state->delayTimer = 0;
            addPlayerGold(
                -(s32)(u16)boardShopPrices[state->boardIndexMap[state->boardDisplayIndices[state->selectedSlot]]]
            );
            state->shopState = 0x1D;
            break;

        case 0x1D:
            playSoundEffectOnChannelNoPriority(0xEB, 1);
            state->shopState = 0x11;
            break;
    }

    if (state->exitMode != 0) {
        playSoundEffectOnChannelNoPriority(0xED, 1);
        state->shopkeeperAnimIndex = 1;
        state->delayTimer = 0x1E;
        state->shopState = 0x32;
        setGameStateHandler(awaitBoardShopExitDelay);
    }
}

void awaitBoardShopExitDelay(void) {
    u16 temp_v1;
    BoardShopState *temp_v0 = (BoardShopState *)getCurrentAllocation();
    temp_v1 = temp_v0->delayTimer - 1;
    temp_v0->delayTimer = temp_v1;
    if (!(temp_v1 & 0xFFFF)) {
        setGameStateHandler(&startBoardShopFadeOut);
    }
}

void startBoardShopFadeOut(void) {
    u8 temp_v1 = ((BoardShopState *)getCurrentAllocation())->exitMode;

    if (temp_v1 != 0) {
        if (temp_v1 == 2) {
            setViewportFadeValue(NULL, 0xFF, 0xA);

        } else {
            setViewportFadeValue(NULL, 0xFF, 0x10);
        }

        setGameStateHandler(&awaitFadeCleanupBoardShop);
    }
}

void awaitFadeCleanupBoardShop(void) {
    BoardShopState *temp_s0 = (BoardShopState *)getCurrentAllocation();
    if (getViewportFadeMode(0) == 0) {
        unlinkNode(&temp_s0->mainViewport);
        unlinkNode(&temp_s0->secondaryViewport);
        unlinkNode(&temp_s0->tertiaryViewport);
        unlinkNode(&temp_s0->quaternaryViewport);
        temp_s0->assetSlot0 = freeNodeMemory(temp_s0->assetSlot0);
        temp_s0->assetSlot1 = freeNodeMemory(temp_s0->assetSlot1);
        temp_s0->assetSlot2 = freeNodeMemory(temp_s0->assetSlot2);
        temp_s0->assetSlot3 = freeNodeMemory(temp_s0->assetSlot3);
        temp_s0->assetSlot4 = freeNodeMemory(temp_s0->assetSlot4);
        temp_s0->textRenderAsset = freeNodeMemory(temp_s0->textRenderAsset);
        temp_s0->assetSlot5 = freeNodeMemory(temp_s0->assetSlot5);
        terminateSchedulerWithCallback(&onBoardShopCleanupComplete);
    }
}

void onBoardShopCleanupComplete(void) {
    returnToParentScheduler(1);
}

u8 countOwnedBoardsInCategory(void) {
    BoardShopState *state;
    u8 count;
    s32 i;

    state = getCurrentAllocation();
    count = 0;
    for (i = 0; i < 3; i++) {
        if (EepromSaveData->character_or_settings[state->selectedCategoryIndex * 3 + i] != 0) {
            count++;
        }
    }

    return count;
}

void advanceBoardDisplaySlots(void) {
    BoardShopState *gameState;
    s32 delta;
    s32 i;
    s8 val = 1;

    gameState = getCurrentAllocation();
    delta = ((gameState->scrollDirection ^ 1) != 0) ? -val : 0;
    delta = delta | 1;

    for (i = 0; i < 4; i++) {
        gameState->boardDisplayIndices[i] += delta | 1;
        val = gameState->boardDisplayIndices[i];
        if (val < 0) {
            gameState->boardDisplayIndices[i] = gameState->totalBoardCount - 1;
        } else if (val == gameState->totalBoardCount) {
            gameState->boardDisplayIndices[i] = 0;
        }
    }
}

void initStoryMapLocationIntro(void) {
    BoardShopState *temp_s0 = (BoardShopState *)allocateTaskMemory(0x1E0);
    setupTaskSchedulerNodes(0x14, 0, 0, 0, 0, 0, 0, 0);
    temp_s0->secondaryViewport.unk0.callback_selector = 0;
    initMenuCameraNode((ViewportNode *)temp_s0, 0, 0xA, 0);
    setViewportFadeValue(0, 0, 8);
    scheduleTask(&storyMapLocationTextTask, 0U, 0U, 0x5AU);
    setGameStateHandler(&awaitStoryMapLocationIntro);
}

void awaitStoryMapLocationIntro(void) {
    ViewportNode *state = (ViewportNode *)getCurrentAllocation();

    state[1].unk0.callback_selector++;

    do {
        if (gControllerInputs[0] & A_BUTTON) {
            state[1].unk0.callback_selector = 0x3C;
        }
    } while (0);

    if (state[1].unk0.callback_selector >= 0x3C) {
        unlinkNode(state);
        terminateSchedulerWithCallback(&onStoryMapLocationIntroComplete);
    }
}

void onStoryMapLocationIntroComplete(void) {
    returnToParentScheduler(1);
}
