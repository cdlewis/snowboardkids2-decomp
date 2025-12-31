#include "15690.h"
#include "38C90.h"
#include "3E160.h"
#include "4050.h"
#include "6E840.h"
#include "D_800AFE8C_A71FC_type.h"
#include "EepromSaveData_type.h"
#include "common.h"
#include "task_scheduler.h"
void func_80015218_15E18(void);
void awaitSaveDataLoad(void);
void func_800150DC_15CDC(void);
void func_80015248_15E48(s32);

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
    func_80015248_15E48(0);
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
            terminateSchedulerWithCallback(func_80015218_15E18);
        } else {
            setGameStateHandler(loadSaveData);
        }
    }
}

void awaitStoryMapSelection(void);

void loadStoryMapScreen(void) {
    createTaskQueue(func_80018EB0_19AB0, 0x96);
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
            terminateSchedulerWithCallback(func_80015218_15E18);
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
        func_80015248_15E48(3);
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
void func_80015028_15C28(void);

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
                func_80015248_15E48(0);
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
                handler = func_80015028_15C28;
            } else {
            set_handler_b70:
                handler = loadStoryMapScreen;
            }
        }

        setGameStateHandler(handler);
    }
}

void awaitPostRaceCutscene(void);
void func_8001511C_15D1C(void);
void func_80015098_15C98(void);

void loadPostRaceCutscene(void) {
    setCutsceneSelection(D_800AFE8C_A71FC->saveSlotIndex, 1);
    createTaskQueue(loadCutsceneOverlay, 0x64);
    setGameStateHandler(awaitPostRaceCutscene);
}

void awaitPostRaceCutscene(void) {
    void *handler;

    if ((func_80069810_6A410() << 16) != 0) {
        if (gStoryCompleted != 0) {
            handler = func_80015028_15C28;
        } else if (D_800AFE8C_A71FC->saveSlotIndex == 0xB) {
            handler = func_8001511C_15D1C;
        } else {
            handler = loadStoryMapScreen;
            if (D_800AFE8C_A71FC->errorFlag != 0) {
                handler = func_80015098_15C98;
            }
        }
        setGameStateHandler(handler);
    }
}

void func_80015064_15C64(void);

void func_80015028_15C28(void) {
    setCutsceneSelection(0xA, 2);
    createTaskQueue(&loadCutsceneOverlay, 0x64);
    setGameStateHandler(func_80015064_15C64);
}

void func_80015064_15C64(void) {
    if ((func_80069810_6A410() << 16) != 0) {
        setGameStateHandler(loadStoryMapScreen);
    }
}

void func_80015098_15C98(void) {
    setCutsceneSelection(D_800AFE8C_A71FC->errorFlag, 2);
    createTaskQueue(&loadCutsceneOverlay, 0x64);
    setGameStateHandler(func_800150DC_15CDC);
}

void func_800150DC_15CDC(void) {
    if ((func_80069810_6A410() << 16) != 0) {
        D_800AFE8C_A71FC->errorFlag = 0;
        setGameStateHandler(loadStoryMapScreen);
    }
}

void func_8001514C_15D4C(void);

void func_8001511C_15D1C(void) {
    createTaskQueue(func_80003EE0_4AE0, 0x64);
    setGameStateHandler(func_8001514C_15D4C);
}

void func_800151A4_15DA4(void);

void func_8001514C_15D4C(void) {
    if ((func_80069810_6A410() << 16) != 0) {
        if (D_800AFE8C_A71FC->errorFlag == 8) {
            D_800AFE8C_A71FC->errorFlag = 0;
            EepromSaveData->unk51 = 1;
        }
        setGameStateHandler(func_800151A4_15DA4);
    }
}

void func_800151E4_15DE4(void);

void func_800151A4_15DA4(void) {
    D_800AFE8C_A71FC->unk25 = 1;
    createTaskQueue(func_8001C920_1D520, 0x96);
    setGameStateHandler(func_800151E4_15DE4);
}

void func_800151E4_15DE4(void) {
    if ((func_80069810_6A410() << 16) != 0) {
        terminateSchedulerWithCallback(func_80015218_15E18);
    }
}

void func_80015218_15E18(void) {
    func_800693C4_69FC4(func_8001452C_1512C, 0xC8);
}

extern u8 D_8009F200_9FE00;

u8 func_8001523C_15E3C(void) {
    return D_8009F200_9FE00;
}

void func_80015248_15E48(s32 arg0) {
    D_8009F200_9FE00 = arg0;
}

INCLUDE_ASM("asm/nonmatchings/15690", func_80015254_15E54);

void func_800154A8_160A8(s8 *arg0, u16 arg1) {
    s32 end;
    s32 ptr;

    if (arg1 == 0) {
        return;
    }

    ptr = (s32)arg0;
    end = arg1 + ptr;
    do {
        *(s8 *)ptr = 0;
        ptr++;
    } while (ptr < end);
}
