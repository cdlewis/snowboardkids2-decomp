#include "session_manager.h"
#include "D_800AFE8C_A71FC_type.h"
#include "EepromSaveData_type.h"
#include "common.h"
#include "effects/cutscene_keyframes.h"
#include "graphics/graphics.h"
#include "race/race_session.h"
#include "system/task_scheduler.h"
#include "ui/save_data.h"

typedef struct {
    s16 unk0;
    u8 unk2;
} TaskData_15690;

extern u8 D_800AB1C8_A2538;
extern u8 gStoryCompleted;
extern u8 gDebugUnlockEnabled;
extern u8 D_8009F200_9FE00;

void returnToMainMenu(void);
void loadSaveSlotScreen(void);
void awaitSaveDataLoad(void);
void awaitUnlockCutscene(void);
void func_80014B1C_1571C(void);
void loadSaveData(void);
void awaitStoryMapSelection(void);
void loadStoryMapScreen(void);
void loadLevelSelectScreen(void);
void func_80014C3C_1583C(void);
void awaitPreRaceCutscene(void);
void loadPreRaceCutscene(void);
void loadRace(void);
void awaitRaceResult(void);
void loadPostRaceCutscene(void);
void loadStoryCompleteCutscene(void);
void loadCreditsSequence(void);
void awaitPostRaceCutscene(void);
void awaitStoryCompleteCutscene(void);
void loadUnlockCutscene(void);
void awaitCreditsSequence(void);
void loadPostCreditsSaveScreen(void);
void awaitPostCreditsSaveScreen(void);
void setStoryMapCameraMode(s32);

void initStoryMode(void) {
    TaskData_15690 *data = allocateTaskMemory(4);
    data->unk2 = 0;
    data->unk0 = 0;
    D_800AB1C8_A2538 = 0;
    D_800AFE8C_A71FC->numPlayers = 1;
    D_800AFE8C_A71FC->playerBoardIds[0] = 0;
    setStoryMapCameraMode(0);
    setGameStateHandlerWithContinue(loadSaveSlotScreen);
}

void loadSaveSlotScreen(void) {
    createTaskQueue(initSaveSlotScreen, 0x96);
    setGameStateHandler(func_80014B1C_1571C);
}

void func_80014B1C_1571C(void) {
    s16 result = getSchedulerReturnValue();

    if (result != 0) {
        if (result == 0xFE) {
            terminateSchedulerWithCallback(returnToMainMenu);
        } else {
            setGameStateHandler(loadSaveData);
        }
    }
}

void loadStoryMapScreen(void) {
    createTaskQueue(initStoryMapState, 0x96);
    setGameStateHandler(awaitStoryMapSelection);
}

void awaitStoryMapSelection(void) {
    s16 result;

    getCurrentAllocation();
    result = getSchedulerReturnValue();

    if (result != 0) {
        if (result == 0x44 || result == 0xFF) {
            setGameStateHandler(loadLevelSelectScreen);
        } else {
            terminateSchedulerWithCallback(returnToMainMenu);
        }
    }
}

void loadLevelSelectScreen(void) {
    createTaskQueue(initLevelSelectBasicState, 0x96);
    setGameStateHandler(func_80014C3C_1583C);
}

void func_80014C3C_1583C(void) {
    s16 result = getSchedulerReturnValue();

    if (result != 0) {
        if (result == 0xFF) {
            setGameStateHandler(loadStoryMapScreen);
        } else if (result == 1) {
            setGameStateHandler(loadPreRaceCutscene);
        }
    }
}

void loadSaveData(void) {
    createTaskQueue(initControllerMotors, 0x96);
    setGameStateHandler(awaitSaveDataLoad);
}

void awaitSaveDataLoad(void) {
    if ((getSchedulerReturnValue() << 16) != 0) {
        setGameStateHandler(loadStoryMapScreen);
    }
}

void loadPreRaceCutscene(void) {
    setCutsceneSelection(D_800AFE8C_A71FC->saveSlotIndex, 0);
    createTaskQueue(loadCutsceneOverlay, 0x96);
    setGameStateHandler(awaitPreRaceCutscene);
}

void awaitPreRaceCutscene(void) {
    if ((getSchedulerReturnValue() << 16) != 0) {
        setStoryMapCameraMode(3);
        setGameStateHandler(loadRace);
    }
}

void loadRace(void) {
    createTaskQueue(initRace, 100);
    setGameStateHandler(awaitRaceResult);
}

void awaitRaceResult(void) {
    s16 result;
    void (*handler)(void);
    u8 *eepromBase;
    u8 *temp;
    u8 saveSlotIndex;
    s32 slotIndexInt;

    result = getSchedulerReturnValue();
    if (result != 0) {
        gStoryCompleted = 0;

        if (result >= 3) {
            eepromBase = (u8 *)EepromSaveData;
            if (EepromSaveData->save_slot_status[0] == 5) {
                gStoryCompleted = 1;
            }

            if ((result == 3) | (result == 5)) {
                *(eepromBase + D_800AFE8C_A71FC->saveSlotIndex + 0x10) = 1;
                processRaceUnlocks(result);
            } else {
                temp = eepromBase + D_800AFE8C_A71FC->saveSlotIndex;
                if (*(temp + 0x10) != 1) {
                    *(temp + 0x10) = 4;
                }
            }
        }

        func_80015254_15E54();
        setViewportFadeValue(NULL, 0, 0);

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

void loadPostRaceCutscene(void) {
    setCutsceneSelection(D_800AFE8C_A71FC->saveSlotIndex, 1);
    createTaskQueue(loadCutsceneOverlay, 0x64);
    setGameStateHandler(awaitPostRaceCutscene);
}

void awaitPostRaceCutscene(void) {
    void *handler;

    if ((getSchedulerReturnValue() << 16) != 0) {
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

void loadStoryCompleteCutscene(void) {
    setCutsceneSelection(0xA, 2);
    createTaskQueue(&loadCutsceneOverlay, 0x64);
    setGameStateHandler(awaitStoryCompleteCutscene);
}

void awaitStoryCompleteCutscene(void) {
    if ((getSchedulerReturnValue() << 16) != 0) {
        setGameStateHandler(loadStoryMapScreen);
    }
}

void loadUnlockCutscene(void) {
    setCutsceneSelection(D_800AFE8C_A71FC->pendingUnlockCutscene, 2);
    createTaskQueue(&loadCutsceneOverlay, 0x64);
    setGameStateHandler(awaitUnlockCutscene);
}

void awaitUnlockCutscene(void) {
    if ((getSchedulerReturnValue() << 16) != 0) {
        D_800AFE8C_A71FC->pendingUnlockCutscene = 0;
        setGameStateHandler(loadStoryMapScreen);
    }
}

void loadCreditsSequence(void) {
    createTaskQueue(func_80003EE0_4AE0, 0x64);
    setGameStateHandler(awaitCreditsSequence);
}

void awaitCreditsSequence(void) {
    if ((getSchedulerReturnValue() << 16) != 0) {
        if (D_800AFE8C_A71FC->pendingUnlockCutscene == 8) {
            D_800AFE8C_A71FC->pendingUnlockCutscene = 0;
            EepromSaveData->unk51 = 1;
        }
        setGameStateHandler(loadPostCreditsSaveScreen);
    }
}

void loadPostCreditsSaveScreen(void) {
    D_800AFE8C_A71FC->creditsCompleted = 1;
    createTaskQueue(initSaveSlotScreen, 0x96);
    setGameStateHandler(awaitPostCreditsSaveScreen);
}

void awaitPostCreditsSaveScreen(void) {
    if ((getSchedulerReturnValue() << 16) != 0) {
        terminateSchedulerWithCallback(returnToMainMenu);
    }
}

void returnToMainMenu(void) {
    createRootTaskScheduler(func_8001452C_1512C, 0xC8);
}

u8 getStoryMapCameraMode(void) {
    return D_8009F200_9FE00;
}

void setStoryMapCameraMode(s32 arg0) {
    D_8009F200_9FE00 = arg0;
}

void func_80015254_15E54(void) {
    s32 i;
    s32 firstEmpty;
    s32 j;
    u8 slotIndex;

    i = 0;
    for (i = 0; i < 12; i++) {
        if (EepromSaveData->save_slot_status[i] == 0) {
            firstEmpty = i;
            break;
        }
    }

    if (i == 12) {
        return;
    }

    slotIndex = firstEmpty;

    if (slotIndex == 10) {
        j = 0;
        for (j = 0; j < slotIndex; j++) {
            if (EepromSaveData->save_slot_status[j] != 1) {
                break;
            }
        }
        if (j == (u8)firstEmpty) {
            for (j = 12; j < 15; j++) {
                if (EepromSaveData->save_slot_status[j] != 1) {
                    break;
                }
            }
            if (j == 15) {
                EepromSaveData->save_slot_status[(u8)firstEmpty] = 5;
            }
            goto final_loop;
        }
    } else if ((slotIndex == 4) | (slotIndex == 8)) {
        j = 0;
        for (j = 0; j < slotIndex; j++) {
            if (EepromSaveData->save_slot_status[j] != 1) {
                break;
            }
        }
        if (j == (u8)firstEmpty) {
            EepromSaveData->save_slot_status[j] = 5;
            goto final_loop;
        }
    } else if (((slotIndex == 3) | (slotIndex == 7)) || (slotIndex == 11)) {
        if (EepromSaveData->save_slot_status[slotIndex - 1] == 1) {
            EepromSaveData->save_slot_status[slotIndex] = 5;
        }
    } else if ((((u8)(firstEmpty - 1) < 2u) | (slotIndex == 5)) || ((slotIndex == 6) | (slotIndex == 9))) {
        if (EepromSaveData->save_slot_status[slotIndex - 1] != 5) {
            EepromSaveData->save_slot_status[slotIndex] = 5;
            if (slotIndex == 1) {
                for (j = 12; j < 15; j++) {
                    EepromSaveData->save_slot_status[j] = 5;
                }
                goto final_loop;
            }
        } else {
            goto final_loop;
        }
    }

final_loop:
    for (i = 0; i < 15; i++) {
        if (EepromSaveData->save_slot_status[i] != 0) {
            EepromSaveData->save_slot_data[i] = 1;
        }
    }
}

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
