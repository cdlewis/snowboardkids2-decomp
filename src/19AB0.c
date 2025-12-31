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
void func_80018F50_19B50(void);
void func_80019078_19C78(void);
void func_800191B4_19DB4(void);
void func_8001923C_19E3C(void);
void func_80019258_19E58(void);
void func_80019220_19E20(void);

typedef struct {
    s16 unk0;
    u8 unk2;
    u8 unk3;
    u8 unk4;
    u8 unk5;
} TaskAllocation;

void func_80018EB0_19AB0(void) {
    TaskAllocation *allocation;

    allocation = (TaskAllocation *)allocateTaskMemory(6);
    allocation->unk0 = 0;
    allocation->unk2 = 0;
    allocation->unk3 = 0;

    if (getStoryMapCameraMode() != 0) {
        allocation->unk2 = 1;
        gTitleInitialized = 1;
        storyMapLocationIndex = 0;
        createTaskQueue(initializeGameState, 100);
        setGameStateHandler(func_80018F50_19B50);
    } else {
        storyMapLocationIndex = 4;
        createTaskQueue(initStoryMapLocation, 100);
        setGameStateHandler(func_80019078_19C78);
    }
}

void func_80018F50_19B50(void) {
    TaskAllocation *allocation;
    s16 result;
    u8 gameState;

    allocation = (TaskAllocation *)getCurrentAllocation();
    result = func_80069810_6A410();

    if (result == 0) {
        return;
    }

    if (result == 0xFF) {
        terminateSchedulerWithCallback(func_80019220_19E20);
        return;
    }

    gameState = storyMapLocationIndex;
    if (gameState == 3 || gameState == 9 || gameState == 6) {
        allocation->unk4 = D_800AFE8C_A71FC->saveSlotIndex;

        if (storyMapLocationIndex == 3) {
            D_800AFE8C_A71FC->saveSlotIndex = 0xD;
        } else if (storyMapLocationIndex == 9) {
            D_800AFE8C_A71FC->saveSlotIndex = 0xC;
        } else {
            D_800AFE8C_A71FC->saveSlotIndex = 0xE;
        }

        createTaskQueue(func_80019D50_1A950, 100);
    } else {
        createTaskQueue(initStoryMapLocation, 100);
        if (storyMapLocationIndex == 7) {
            func_800585C8_591C8(0x22);
        }
    }

    setGameStateHandler(func_80019078_19C78);
}

void func_80019078_19C78(void) {
    TaskAllocation *allocation;
    s16 result;

    allocation = (TaskAllocation *)getCurrentAllocation();
    result = func_80069810_6A410();

    if (result != 0) {
        func_80057564_58164(0xA);
        if (result == 0x44) {
            terminateSchedulerWithCallback(func_80019258_19E58);
        } else if (result == 1) {
            if (EepromSaveData->unk51 != 0) {
                setCutsceneSelection(8, 2);
                createTaskQueue(loadCutsceneOverlay, 0x64);
                setGameStateHandler(func_800191B4_19DB4);
            } else {
                allocation->unk2 = 1;
                gTitleInitialized = 1;
                createTaskQueue(initializeGameState, 0x64);
                setGameStateHandler(func_80018F50_19B50);
            }
        } else if (result == 0xFE) {
            terminateSchedulerWithCallback(func_8001923C_19E3C);
        } else if (result == 0xFF && allocation->unk2 == 0) {
            terminateSchedulerWithCallback(func_8001923C_19E3C);
        } else {
            if (D_800AFE8C_A71FC->saveSlotIndex >= 0xC) {
                D_800AFE8C_A71FC->saveSlotIndex = allocation->unk4;
            }
            createTaskQueue(initializeGameState, 0x64);
            setGameStateHandler(func_80018F50_19B50);
        }
    }
}

void func_800191B4_19DB4(void) {
    TaskAllocation *allocation = (TaskAllocation *)getCurrentAllocation();

    if (func_80069810_6A410() != 0) {
        EepromSaveData->unk51 = 0;
        allocation->unk2 = 1;
        gTitleInitialized = 1;
        createTaskQueue(initializeGameState, 100);
        setGameStateHandler(func_80018F50_19B50);
    }
}

void func_80019220_19E20(void) {
    func_800697F4_6A3F4(0xFF);
}

void func_8001923C_19E3C(void) {
    func_800697F4_6A3F4(0xFE);
}

void func_80019258_19E58(void) {
    func_800697F4_6A3F4(0x44);
}
