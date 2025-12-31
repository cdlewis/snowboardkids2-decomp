#include "15690.h"
#include "38C90.h"
#include "3E160.h"
#include "4050.h"
#include "6E840.h"
#include "D_800AFE8C_A71FC_type.h"
#include "EepromSaveData_type.h"
#include "common.h"
#include "task_scheduler.h"
void returnToMainMenu(void);
void awaitSaveDataLoad(void);
void awaitUnlockCutscene(void);
void setStoryMapCameraMode(s32);

typedef struct {
    s16 unk0;
    u8 unk2;
} TaskData_15690;

extern u8 D_800AB1C8_A2538;

void loadSaveSlotScreen(void);

void initStoryMode(void) {
    TaskData_15690 *data = allocateTaskMemory(4);
    data->unk2 = 0;
    data->unk0 = 0;
    D_800AB1C8_A2538 = 0;
    D_800AFE8C_A71FC->unk8 = 1;
    D_800AFE8C_A71FC->unk9[0] = 0;
    setStoryMapCameraMode(0);
    func_8006983C_6A43C(loadSaveSlotScreen);
}

void func_80014B1C_1571C(void);

void loadSaveSlotScreen(void) {
    createTaskQueue(func_8001C920_1D520, 0x96);
    setGameStateHandler(func_80014B1C_1571C);
}

void loadSaveData(void);

void func_80014B1C_1571C(void) {
    s16 result = func_80069810_6A410();

    if (result != 0) {
        if (result == 0xFE) {
            terminateSchedulerWithCallback(returnToMainMenu);
        } else {
            setGameStateHandler(loadSaveData);
        }
    }
}

void awaitStoryMapSelection(void);

void loadStoryMapScreen(void) {
    createTaskQueue(initStoryMapState, 0x96);
    setGameStateHandler(awaitStoryMapSelection);
}

void loadLevelSelectScreen(void);

void awaitStoryMapSelection(void) {
    s16 result;

    getCurrentAllocation();
    result = func_80069810_6A410();

    if (result != 0) {
        if (result == 0x44 || result == 0xFF) {
            setGameStateHandler(loadLevelSelectScreen);
        } else {
            terminateSchedulerWithCallback(returnToMainMenu);
        }
    }
}

void func_80014C3C_1583C(void);
void loadPreRaceCutscene(void);

void loadLevelSelectScreen(void) {
    createTaskQueue(func_80021BD0_227D0, 0x96);
    setGameStateHandler(func_80014C3C_1583C);
}

void func_80014C3C_1583C(void) {
    s16 result = func_80069810_6A410();

    if (result != 0) {
        if (result == 0xFF) {
            setGameStateHandler(loadStoryMapScreen);
        } else if (result == 1) {
            setGameStateHandler(loadPreRaceCutscene);
        }
    }
}

void loadSaveData(void) {
    createTaskQueue(func_80033200_33E00, 0x96);
    setGameStateHandler(awaitSaveDataLoad);
}

void awaitSaveDataLoad(void) {
    if ((func_80069810_6A410() << 16) != 0) {
        setGameStateHandler(loadStoryMapScreen);
    }
}

void awaitPreRaceCutscene(void);
void loadRace(void);

void loadPreRaceCutscene(void) {
    setCutsceneSelection(D_800AFE8C_A71FC->saveSlotIndex, 0);
    createTaskQueue(loadCutsceneOverlay, 0x96);
    setGameStateHandler(awaitPreRaceCutscene);
}

void awaitPreRaceCutscene(void) {
    if ((func_80069810_6A410() << 16) != 0) {
        setStoryMapCameraMode(3);
        setGameStateHandler(loadRace);
    }
}

void awaitRaceResult(void);

void loadRace(void) {
    createTaskQueue(initRace, 100);
    setGameStateHandler(awaitRaceResult);
}

extern u8 gStoryCompleted;
extern u8 gDebugUnlockEnabled;

void loadPostRaceCutscene(void);
void loadStoryCompleteCutscene(void);

void awaitRaceResult(void) {
    s16 result;
    void (*handler)(void);
    u8 *eepromBase;
    u8 *temp;
    u8 saveSlotIndex;
    s32 slotIndexInt;

    result = func_80069810_6A410();
    if (result != 0) {
        gStoryCompleted = 0;

        if (result >= 3) {
            eepromBase = (u8 *)EepromSaveData;
            if (EepromSaveData->save_slot_status[0] == 5) {
                gStoryCompleted = 1;
            }

            if ((result == 3) | (result == 5)) {
                *(eepromBase + D_800AFE8C_A71FC->saveSlotIndex + 0x10) = 1;
                func_80038090_38C90(result);
            } else {
                temp = eepromBase + D_800AFE8C_A71FC->saveSlotIndex;
                if (*(temp + 0x10) != 1) {
                    *(temp + 0x10) = 4;
                }
            }
        }

        func_80015254_15E54();
        func_8006FDA0_709A0(NULL, 0, 0);

        if (result == 1) {
            handler = loadRace;
        } else {
            if (EepromSaveData->save_slot_status[0] == 5) {
                setStoryMapCameraMode(0);
                goto set_handler_b70;
            }
            if ((result == 3) || (gDebugUnlockEnabled != 0)) {
                saveSlotIndex = D_800AFE8C_A71FC->saveSlotIndex;
                slotIndexInt = saveSlotIndex;
                if (((slotIndexInt == 2) | (slotIndexInt == 6)) || (slotIndexInt == 10)) {
                    D_800AFE8C_A71FC->saveSlotIndex = saveSlotIndex + 1;
                    handler = loadPreRaceCutscene;
                } else {
                    handler = loadPostRaceCutscene;
                }
            } else if (gStoryCompleted != 0) {
                handler = loadStoryCompleteCutscene;
            } else {
            set_handler_b70:
                handler = loadStoryMapScreen;
            }
        }

        setGameStateHandler(handler);
    }
}

void awaitPostRaceCutscene(void);
void loadCreditsSequence(void);
void loadUnlockCutscene(void);

void loadPostRaceCutscene(void) {
    setCutsceneSelection(D_800AFE8C_A71FC->saveSlotIndex, 1);
    createTaskQueue(loadCutsceneOverlay, 0x64);
    setGameStateHandler(awaitPostRaceCutscene);
}

void awaitPostRaceCutscene(void) {
    void *handler;

    if ((func_80069810_6A410() << 16) != 0) {
        if (gStoryCompleted != 0) {
            handler = loadStoryCompleteCutscene;
        } else if (D_800AFE8C_A71FC->saveSlotIndex == 0xB) {
            handler = loadCreditsSequence;
        } else {
            handler = loadStoryMapScreen;
            if (D_800AFE8C_A71FC->pendingUnlockCutscene != 0) {
                handler = loadUnlockCutscene;
            }
        }
        setGameStateHandler(handler);
    }
}

void awaitStoryCompleteCutscene(void);

void loadStoryCompleteCutscene(void) {
    setCutsceneSelection(0xA, 2);
    createTaskQueue(&loadCutsceneOverlay, 0x64);
    setGameStateHandler(awaitStoryCompleteCutscene);
}

void awaitStoryCompleteCutscene(void) {
    if ((func_80069810_6A410() << 16) != 0) {
        setGameStateHandler(loadStoryMapScreen);
    }
}

void loadUnlockCutscene(void) {
    setCutsceneSelection(D_800AFE8C_A71FC->pendingUnlockCutscene, 2);
    createTaskQueue(&loadCutsceneOverlay, 0x64);
    setGameStateHandler(awaitUnlockCutscene);
}

void awaitUnlockCutscene(void) {
    if ((func_80069810_6A410() << 16) != 0) {
        D_800AFE8C_A71FC->pendingUnlockCutscene = 0;
        setGameStateHandler(loadStoryMapScreen);
    }
}

void awaitCreditsSequence(void);

void loadCreditsSequence(void) {
    createTaskQueue(func_80003EE0_4AE0, 0x64);
    setGameStateHandler(awaitCreditsSequence);
}

void loadPostCreditsSaveScreen(void);

void awaitCreditsSequence(void) {
    if ((func_80069810_6A410() << 16) != 0) {
        if (D_800AFE8C_A71FC->pendingUnlockCutscene == 8) {
            D_800AFE8C_A71FC->pendingUnlockCutscene = 0;
            EepromSaveData->unk51 = 1;
        }
        setGameStateHandler(loadPostCreditsSaveScreen);
    }
}

void awaitPostCreditsSaveScreen(void);

void loadPostCreditsSaveScreen(void) {
    D_800AFE8C_A71FC->creditsCompleted = 1;
    createTaskQueue(func_8001C920_1D520, 0x96);
    setGameStateHandler(awaitPostCreditsSaveScreen);
}

void awaitPostCreditsSaveScreen(void) {
    if ((func_80069810_6A410() << 16) != 0) {
        terminateSchedulerWithCallback(returnToMainMenu);
    }
}

void returnToMainMenu(void) {
    func_800693C4_69FC4(func_8001452C_1512C, 0xC8);
}

extern u8 D_8009F200_9FE00;

u8 getStoryMapCameraMode(void) {
    return D_8009F200_9FE00;
}

void setStoryMapCameraMode(s32 arg0) {
    D_8009F200_9FE00 = arg0;
}

INCLUDE_ASM("asm/nonmatchings/15690", func_80015254_15E54);

void clearMemory(s8 *dest, u16 size) {
    s32 endAddr;
    s32 current;

    if (size == 0) {
        return;
    }

    current = (s32)dest;
    endAddr = size + current;
    do {
        *(s8 *)current = 0;
        current++;
    } while (current < endAddr);
}
