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
    /* 0x77E */ u16 unk77E;
    /* 0x780 */ u8 pad780[4];
    /* 0x784 */ s8 boardDisplayIndices[4];
    /* 0x788 */ u8 boardIndexMap[13];
    /* 0x795 */ u8 unlockedBoardsInCategory[3];
    /* 0x798 */ u8 totalBoardCount;
    /* 0x799 */ u8 unk799;
    /* 0x79A */ u8 exitMode;
    /* 0x79B */ u8 unk79B;
    /* 0x79C */ u8 scrollDirection;
    /* 0x79D */ u8 pad79D;
    /* 0x79E */ u8 unk79E;
    /* 0x79F */ u8 unk79F;
    /* 0x7A0 */ u8 pad7A0;
    /* 0x7A1 */ s8 selectedCategoryIndex;
    /* 0x7A2 */ u8 unk7A2;
    /* 0x7A3 */ u8 pad7A3;
    /* 0x7A4 */ u8 unk7A4;
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
void func_8001A478_1B078(void);
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
    u8 lightBuffer[0x20];
    u8 *ptr;
    s32 i;

    state = (BoardShopState *)allocateTaskMemory(0x7A8);
    setupTaskSchedulerNodes(0x14, 0, 0, 0, 0, 0, 0, 0);
    state->delayTimer = 0;
    state->unk799 = 0;
    state->exitMode = 0;
    state->unk7A2 = 0;
    state->selectedCategoryIndex = 0;
    state->unk79F = 0;
    state->unk79E = 0;
    state->unk77E = 0;
    state->unk79B = 0;
    initMenuCameraNode(&state->mainViewport, 0, 0xA, 0);
    initMenuCameraNode(&state->secondaryViewport, 2, 0x14, 0);
    initMenuCameraNode(&state->tertiaryViewport, 8, 0x14, 1);
    initMenuCameraNode(&state->quaternaryViewport, 9, 5, 1);
    createViewportTransform(lightBuffer, 0, 0, 0x580000, 0, 0, 0);
    setViewportTransformById(state->mainViewport.id, lightBuffer);
    setViewportTransformById(state->secondaryViewport.id, lightBuffer);
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
    state->unk7A4 = 0;
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
        setGameStateHandler(&func_8001A478_1B078);
    }
}

INCLUDE_ASM("asm/nonmatchings/story/race_state_machine", func_8001A478_1B078);

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
