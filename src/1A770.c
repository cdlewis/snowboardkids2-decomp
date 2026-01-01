#include "1A770.h"

#include "15690.h"
#include "20F0.h"
#include "288A0.h"
#include "3A1F0.h"
#include "3E160.h"
#include "4050.h"
#include "68CF0.h"
#include "6E840.h"
#include "D_800AFE8C_A71FC_type.h"
#include "EepromSaveData_type.h"
#include "common.h"
#include "gamestate.h"
#include "graphics.h"
#include "task_scheduler.h"

typedef struct {
    Node_70B00 unk0;
    Node_70B00 unk1D8;
    Node_70B00 unk3B0;
    Node_70B00 unk588;
    s32 *unk760;
    void *unk764;
    void *unk768;
    void *unk76C;
    void *unk770;
    void *unk774;
    void *unk778;
    u16 unk77C;
    u8 padding[0x1C];
    u8 unk79A;
} allocation_1B8C8;

extern void onStoryModeRaceCancelled(void);
extern void awaitStoryMapLocationIntro(void);
extern void awaitStoryMapDecorReady(void);
extern void cleanupStoryMapDecorModel(applyTransformToModel_arg1 *);
extern void setupStoryMapDecorModel(applyTransformToModel_arg1 *);
extern s32 identityMatrix;
extern D_8008D7FC_8E3FC_item D_8008D7FC_8E3FC[];
extern u8 storyMapLocationIndex;
extern void awaitFadeLoadCharacterSelect(void);
extern s32 gControllerInputs[4];
void awaitFadeLoadBoardShop(void);
extern void func_8001A478_1B078(void);

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
    if (func_8006FE10_70A10(0) == 0) {
        createTaskQueue(&initLevelSelectWithDetail, 100);
        setGameStateHandler(&awaitStoryModeRaceLevelSelect);
    }
}

void awaitStoryModeRaceLevelSelect(void) {
    s16 selectionResult = func_80069810_6A410();

    if (selectionResult != 0) {
        if (selectionResult == 0xFF) {
            terminateSchedulerWithCallback(&onStoryModeRaceCancelled);
            return;
        }

        setGameStateHandler(&awaitFadeLoadCharacterSelect);
    }
}

void awaitFadeLoadCharacterSelect(void) {
    if (func_8006FE10_70A10(0) == 0) {
        func_800574A0_580A0(2);
        createTaskQueue(&func_800226F0_232F0, 100);
        setGameStateHandler(&awaitStoryModeCharacterSelect);
    }
}

void awaitStoryModeCharacterSelect(void) {
    s16 selectionResult = func_80069810_6A410();
    if (selectionResult != 0) {
        void *nextHandler = &awaitFadeLoadPreRaceCutscene;
        if (selectionResult == 0xFF) {
            nextHandler = &awaitFadeLoadStoryMap;
        }
        setGameStateHandler(nextHandler);
    }
}

void awaitFadeLoadPreRaceCutscene(void) {
    if (func_8006FE10_70A10(0) == 0) {
        setCutsceneSelection(D_800AFE8C_A71FC->saveSlotIndex, 0);
        createTaskQueue(&loadCutsceneOverlay, 150);
        setGameStateHandler(&awaitStoryModePreRaceCutscene);
    }
}

void awaitStoryModePreRaceCutscene(void) {
    if ((func_80069810_6A410() << 0x10) != 0) {
        setGameStateHandler(&loadStoryModeRace);
    }
}

void loadStoryModeRace(void) {
    createTaskQueue(&initRace, 100);
    setGameStateHandler(&awaitStoryModeRaceResult);
}

void awaitStoryModeRaceResult(void) {
    s16 raceResult;
    D_800AFE8C_A71FC_type *gameState;
    u8 saveSlotIndex;

    raceResult = func_80069810_6A410();

    if (raceResult == 0) {
        return;
    }

    if (raceResult >= 3) {
        if (raceResult == 5 || raceResult >= 7) {
            saveSlotIndex = D_800AFE8C_A71FC->saveSlotIndex;
            do {
                EepromSaveData->save_slot_status[saveSlotIndex] = 1;
            } while (0);
            func_80038090_38C90(raceResult);
        } else {
            gameState = D_800AFE8C_A71FC;
            saveSlotIndex = gameState->saveSlotIndex;
            if (EepromSaveData->save_slot_status[saveSlotIndex] != 1) {
                EepromSaveData->save_slot_status[saveSlotIndex] = 4;
            }
        }
    }

    func_80015254_15E54();

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
    if ((func_80069810_6A410() << 0x10) != 0) {
        D_800AFE8C_A71FC->pendingUnlockCutscene = 0;
        terminateSchedulerWithCallback(&onStoryModeRaceCancelled);
    }
}

void onStoryModeRaceCancelled(void) {
    func_800697F4_6A3F4(0xFF);
}

INCLUDE_ASM("asm/nonmatchings/1A770", func_8001A110_1AD10);

void awaitBoardShopDmaComplete(void) {
    GameState *state;

    state = (GameState *)getCurrentAllocation();
    if (state->unk77C != 0U) {
        state->unk77C--;
        return;
    }
    if (func_8003BB5C_3C75C() == 0) {
        func_8006FDA0_709A0(0, 0, 0x10);
        setGameStateHandler(&awaitFadeLoadBoardShop);
    }
}

void awaitFadeLoadBoardShop(void) {
    if (func_8006FE10_70A10(0) == 0) {
        scheduleTask(&func_80032330_32F30, 0, 0, 0x5A);
        setGameStateHandler(&func_8001A478_1B078);
    }
}

INCLUDE_ASM("asm/nonmatchings/1A770", func_8001A478_1B078);

void awaitBoardShopExitDelay(void) {
    u16 temp_v1;
    allocation_1B8C8 *temp_v0 = (allocation_1B8C8 *)getCurrentAllocation();
    temp_v1 = temp_v0->unk77C - 1;
    temp_v0->unk77C = temp_v1;
    if (!(temp_v1 & 0xFFFF)) {
        setGameStateHandler(&startBoardShopFadeOut);
    }
}

void startBoardShopFadeOut(void) {
    u8 temp_v1 = ((allocation_1B8C8 *)getCurrentAllocation())->unk79A;

    if (temp_v1 != 0) {
        if (temp_v1 == 2) {
            func_8006FDA0_709A0(NULL, 0xFF, 0xA);

        } else {
            func_8006FDA0_709A0(NULL, 0xFF, 0x10);
        }

        setGameStateHandler(&awaitFadeCleanupBoardShop);
    }
}

void awaitFadeCleanupBoardShop(void) {
    allocation_1B8C8 *temp_s0 = (allocation_1B8C8 *)getCurrentAllocation();
    if (func_8006FE10_70A10(0) == 0) {
        unlinkNode(&temp_s0->unk0);
        unlinkNode(&temp_s0->unk1D8);
        unlinkNode(&temp_s0->unk3B0);
        unlinkNode(&temp_s0->unk588);
        temp_s0->unk760 = freeNodeMemory(temp_s0->unk760);
        temp_s0->unk764 = freeNodeMemory(temp_s0->unk764);
        temp_s0->unk768 = freeNodeMemory(temp_s0->unk768);
        temp_s0->unk76C = freeNodeMemory(temp_s0->unk76C);
        temp_s0->unk770 = freeNodeMemory(temp_s0->unk770);
        temp_s0->unk774 = freeNodeMemory(temp_s0->unk774);
        temp_s0->unk778 = freeNodeMemory(temp_s0->unk778);
        terminateSchedulerWithCallback(&onBoardShopCleanupComplete);
    }
}

void onBoardShopCleanupComplete(void) {
    func_800697F4_6A3F4(1);
}

u8 countOwnedBoardsInCategory(void) {
    GameState *state;
    u8 count;
    s32 i;

    state = getCurrentAllocation();
    count = 0;
    for (i = 0; i < 3; i++) {
        if (EepromSaveData->character_or_settings[state->unk7A1 * 3 + i] != 0) {
            count++;
        }
    }

    return count;
}

void advanceBoardDisplaySlots(void) {
    GameState *gameState;
    s32 delta;
    s32 i;
    s8 val = 1;

    gameState = getCurrentAllocation();
    delta = ((gameState->unk79C ^ 1) != 0) ? -val : 0;
    delta = delta | 1;

    for (i = 0; i < 4; i++) {
        gameState->unk784[i] += delta | 1;
        val = gameState->unk784[i];
        if (val < 0) {
            gameState->unk784[i] = gameState->unk798 - 1;
        } else if (val == gameState->unk798) {
            gameState->unk784[i] = 0;
        }
    }
}

void initStoryMapLocationIntro(void) {
    allocation_1B8C8 *temp_s0 = (allocation_1B8C8 *)allocateTaskMemory(0x1E0);
    setupTaskSchedulerNodes(0x14, 0, 0, 0, 0, 0, 0, 0);
    temp_s0->unk1D8.unk0.callback_selector = 0;
    initMenuCameraNode((Node_70B00 *)temp_s0, 0, 0xA, 0);
    func_8006FDA0_709A0(0, 0, 8);
    scheduleTask(&storyMapLocationTextTask, 0U, 0U, 0x5AU);
    setGameStateHandler(&awaitStoryMapLocationIntro);
}

void awaitStoryMapLocationIntro(void) {
    Node_70B00 *state = (Node_70B00 *)getCurrentAllocation();

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
    func_800697F4_6A3F4(1);
}
