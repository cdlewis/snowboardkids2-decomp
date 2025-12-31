#include "15690.h"
#include "199C0.h"
#include "1A770.h"
#include "4050.h"
#include "D_800AFE8C_A71FC_type.h"
#include "EepromSaveData_type.h"
#include "common.h"
#include "game_state_init.h"
#include "graphics.h"
#include "task_scheduler.h"

extern u8 gTitleInitialized;
extern u8 storyMapLocationIndex;
void handleGameStateComplete(void);
void handleStoryMapLocationComplete(void);
void awaitIntroCutsceneComplete(void);
void onStoryMapExitBack(void);
void onStoryMapExitToLevelSelect(void);
void onStoryMapExitToMenu(void);

typedef struct {
    s16 unk0;
    u8 fromGameState;
    u8 unk3;
    u8 previousSaveSlot;
    u8 unk5;
} StoryMapStateData;

void initStoryMapState(void) {
    StoryMapStateData *state;

    state = (StoryMapStateData *)allocateTaskMemory(6);
    state->unk0 = 0;
    state->fromGameState = 0;
    state->unk3 = 0;

    if (getStoryMapCameraMode() != 0) {
        state->fromGameState = 1;
        gTitleInitialized = 1;
        storyMapLocationIndex = 0;
        createTaskQueue(initializeGameState, 100);
        setGameStateHandler(handleGameStateComplete);
    } else {
        storyMapLocationIndex = 4;
        createTaskQueue(initStoryMapLocation, 100);
        setGameStateHandler(handleStoryMapLocationComplete);
    }
}

void handleGameStateComplete(void) {
    StoryMapStateData *state;
    s16 result;
    u8 locationIndex;

    state = (StoryMapStateData *)getCurrentAllocation();
    result = func_80069810_6A410();

    if (result == 0) {
        return;
    }

    if (result == 0xFF) {
        terminateSchedulerWithCallback(onStoryMapExitToMenu);
        return;
    }

    locationIndex = storyMapLocationIndex;
    if (locationIndex == 3 || locationIndex == 9 || locationIndex == 6) {
        state->previousSaveSlot = D_800AFE8C_A71FC->saveSlotIndex;

        if (storyMapLocationIndex == 3) {
            D_800AFE8C_A71FC->saveSlotIndex = 0xD;
        } else if (storyMapLocationIndex == 9) {
            D_800AFE8C_A71FC->saveSlotIndex = 0xC;
        } else {
            D_800AFE8C_A71FC->saveSlotIndex = 0xE;
        }

        createTaskQueue(initStoryModeRace, 100);
    } else {
        createTaskQueue(initStoryMapLocation, 100);
        if (storyMapLocationIndex == 7) {
            func_800585C8_591C8(0x22);
        }
    }

    setGameStateHandler(handleStoryMapLocationComplete);
}

void handleStoryMapLocationComplete(void) {
    StoryMapStateData *state;
    s16 result;

    state = (StoryMapStateData *)getCurrentAllocation();
    result = func_80069810_6A410();

    if (result != 0) {
        func_80057564_58164(0xA);
        if (result == 0x44) {
            terminateSchedulerWithCallback(onStoryMapExitToLevelSelect);
        } else if (result == 1) {
            if (EepromSaveData->unk51 != 0) {
                setCutsceneSelection(8, 2);
                createTaskQueue(loadCutsceneOverlay, 0x64);
                setGameStateHandler(awaitIntroCutsceneComplete);
            } else {
                state->fromGameState = 1;
                gTitleInitialized = 1;
                createTaskQueue(initializeGameState, 0x64);
                setGameStateHandler(handleGameStateComplete);
            }
        } else if (result == 0xFE) {
            terminateSchedulerWithCallback(onStoryMapExitBack);
        } else if (result == 0xFF && state->fromGameState == 0) {
            terminateSchedulerWithCallback(onStoryMapExitBack);
        } else {
            if (D_800AFE8C_A71FC->saveSlotIndex >= 0xC) {
                D_800AFE8C_A71FC->saveSlotIndex = state->previousSaveSlot;
            }
            createTaskQueue(initializeGameState, 0x64);
            setGameStateHandler(handleGameStateComplete);
        }
    }
}

void awaitIntroCutsceneComplete(void) {
    StoryMapStateData *state = (StoryMapStateData *)getCurrentAllocation();

    if (func_80069810_6A410() != 0) {
        EepromSaveData->unk51 = 0;
        state->fromGameState = 1;
        gTitleInitialized = 1;
        createTaskQueue(initializeGameState, 100);
        setGameStateHandler(handleGameStateComplete);
    }
}

void onStoryMapExitToMenu(void) {
    func_800697F4_6A3F4(0xFF);
}

void onStoryMapExitBack(void) {
    func_800697F4_6A3F4(0xFE);
}

void onStoryMapExitToLevelSelect(void) {
    func_800697F4_6A3F4(0x44);
}
