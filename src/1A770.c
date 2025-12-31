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

extern void func_8001A0F4_1ACF4(void);
extern void func_8001AF18_1BB18(void);
extern void awaitStoryMapDecorReady(void);
extern void cleanupStoryMapDecorModel(applyTransformToModel_arg1 *);
extern void setupStoryMapDecorModel(applyTransformToModel_arg1 *);
extern s32 identityMatrix;
extern D_8008D7FC_8E3FC_item D_8008D7FC_8E3FC[];
extern u8 storyMapLocationIndex;
extern void awaitFadeLoadCharacterSelect(void);
extern s32 gControllerInputs[4];
extern void func_8001A478_1B078(void);

void func_8001ACC8_1B8C8(void);
void func_8001AD80_1B980(void);
void func_8001A0B4_1ACB4(void);
void func_8001A070_1AC70(void);
void func_80019F60_1AB60(void);
void func_80019EFC_1AAFC(void);
void awaitStoryModeRaceLevelSelect(void);
void updateStoryMapDecorModel(applyTransformToModel_arg1 *arg0);
void enqueueStoryMapLocationText(TextData *arg0);
void awaitFadeLoadStoryMap(void);
void awaitStoryModeCharacterSelect(void);
void awaitFadeLoadPreRaceCutscene(void);
void func_80019F30_1AB30(void);
void func_8001AF80_1BB80(void);
void func_8001A434_1B034(void);
void func_8001AC70_1B870(void);

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
        createTaskQueue(&func_8001E590_1F190, 100);
        setGameStateHandler(&awaitStoryModeRaceLevelSelect);
    }
}

void awaitStoryModeRaceLevelSelect(void) {
    s16 selectionResult = func_80069810_6A410();

    if (selectionResult != 0) {
        if (selectionResult == 0xFF) {
            terminateSchedulerWithCallback(&func_8001A0F4_1ACF4);
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
        setGameStateHandler(&func_80019EFC_1AAFC);
    }
}

void func_80019EFC_1AAFC(void) {
    if ((func_80069810_6A410() << 0x10) != 0) {
        setGameStateHandler(&func_80019F30_1AB30);
    }
}

void func_80019F30_1AB30(void) {
    createTaskQueue(&initRace, 100);
    setGameStateHandler(&func_80019F60_1AB60);
}

void func_80019F60_1AB60(void) {
    s16 saveOperationResult;
    D_800AFE8C_A71FC_type *gameStatePtr;
    u8 saveSlotIndex;

    saveOperationResult = func_80069810_6A410();

    if (saveOperationResult == 0) {
        return;
    }

    if (saveOperationResult >= 3) {
        if (saveOperationResult == 5 || saveOperationResult >= 7) {
            saveSlotIndex = D_800AFE8C_A71FC->saveSlotIndex;
            do {
                EepromSaveData->save_slot_status[saveSlotIndex] = 1;
            } while (0);
            func_80038090_38C90(saveOperationResult);
        } else {
            gameStatePtr = D_800AFE8C_A71FC;
            saveSlotIndex = gameStatePtr->saveSlotIndex;
            if (EepromSaveData->save_slot_status[saveSlotIndex] != 1) {
                EepromSaveData->save_slot_status[saveSlotIndex] = 4;
            }
        }
    }

    func_80015254_15E54();

    if (saveOperationResult == 1) {
        setGameStateHandler(func_80019F30_1AB30);
        return;
    }

    if (D_800AFE8C_A71FC->pendingUnlockCutscene != 0) {
        setGameStateHandler(func_8001A070_1AC70);
        return;
    }

    terminateSchedulerWithCallback(func_8001A0F4_1ACF4);
}

void func_8001A070_1AC70(void) {
    setCutsceneSelection(D_800AFE8C_A71FC->pendingUnlockCutscene, 2);
    createTaskQueue(&loadCutsceneOverlay, 0x64);
    setGameStateHandler(&func_8001A0B4_1ACB4);
}

void func_8001A0B4_1ACB4(void) {
    if ((func_80069810_6A410() << 0x10) != 0) {
        D_800AFE8C_A71FC->pendingUnlockCutscene = 0;
        terminateSchedulerWithCallback(&func_8001A0F4_1ACF4);
    }
}

void func_8001A0F4_1ACF4(void) {
    func_800697F4_6A3F4(0xFF);
}

INCLUDE_ASM("asm/nonmatchings/1A770", func_8001A110_1AD10);

void func_8001A3D8_1AFD8(void) {
    GameState *temp_v0;

    temp_v0 = (GameState *)getCurrentAllocation();
    if (temp_v0->unk77C != 0U) {
        temp_v0->unk77C--;
        return;
    }
    if (func_8003BB5C_3C75C() == 0) {
        func_8006FDA0_709A0(0, 0, 0x10);
        setGameStateHandler(&func_8001A434_1B034);
    }
}

void func_8001A434_1B034(void) {
    if (func_8006FE10_70A10(0) == 0) {
        scheduleTask(&func_80032330_32F30, 0, 0, 0x5A);
        setGameStateHandler(&func_8001A478_1B078);
    }
}

INCLUDE_ASM("asm/nonmatchings/1A770", func_8001A478_1B078);

void func_8001AC30_1B830(void) {
    u16 temp_v1;
    allocation_1B8C8 *temp_v0 = (allocation_1B8C8 *)getCurrentAllocation();
    temp_v1 = temp_v0->unk77C - 1;
    temp_v0->unk77C = temp_v1;
    if (!(temp_v1 & 0xFFFF)) {
        setGameStateHandler(&func_8001AC70_1B870);
    }
}

void func_8001AC70_1B870(void) {
    u8 temp_v1 = ((allocation_1B8C8 *)getCurrentAllocation())->unk79A;

    if (temp_v1 != 0) {
        if (temp_v1 == 2) {
            func_8006FDA0_709A0(NULL, 0xFF, 0xA);

        } else {
            func_8006FDA0_709A0(NULL, 0xFF, 0x10);
        }

        setGameStateHandler(&func_8001ACC8_1B8C8);
    }
}

void func_8001ACC8_1B8C8(void) {
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
        terminateSchedulerWithCallback(&func_8001AD80_1B980);
    }
}

void func_8001AD80_1B980(void) {
    func_800697F4_6A3F4(1);
}

u8 func_8001AD9C_1B99C(void) {
    GameState *state;
    u8 result;
    s32 i;

    state = getCurrentAllocation();
    result = 0;
    for (i = 0; i < 3; i++) {
        if (EepromSaveData->character_or_settings[state->unk7A1 * 3 + i] != 0) {
            result++;
        }
    }

    return result;
}

void func_8001ADFC_1B9FC(void) {
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

void func_8001AE80_1BA80(void) {
    allocation_1B8C8 *temp_s0 = (allocation_1B8C8 *)allocateTaskMemory(0x1E0);
    setupTaskSchedulerNodes(0x14, 0, 0, 0, 0, 0, 0, 0);
    temp_s0->unk1D8.unk0.callback_selector = 0;
    func_80027CA0_288A0((Node_70B00 *)temp_s0, 0, 0xA, 0);
    func_8006FDA0_709A0(0, 0, 8);
    scheduleTask(&storyMapLocationTextTask, 0U, 0U, 0x5AU);
    setGameStateHandler(&func_8001AF18_1BB18);
}

void func_8001AF18_1BB18(void) {
    Node_70B00 *temp_v0 = (Node_70B00 *)getCurrentAllocation();

    temp_v0[1].unk0.callback_selector++;

    do {
        if (gControllerInputs[0] & A_BUTTON) {
            temp_v0[1].unk0.callback_selector = 0x3C;
        }
    } while (0);

    if (temp_v0[1].unk0.callback_selector >= 0x3C) {
        unlinkNode(temp_v0);
        terminateSchedulerWithCallback(&func_8001AF80_1BB80);
    }
}

void func_8001AF80_1BB80(void) {
    func_800697F4_6A3F4(1);
}
