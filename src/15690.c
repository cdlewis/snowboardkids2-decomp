#include "15690.h"
#include "3E160.h"
#include "4050.h"
#include "D_800AFE8C_A71FC_type.h"
#include "EepromSaveData_type.h"
#include "common.h"
#include "task_scheduler.h"
void func_80015218_15E18(void);
void func_80014CC4_158C4(void);
void func_800150DC_15CDC(void);
void func_80015248_15E48(s32);

typedef struct {
    s16 unk0;
    u8 unk2;
} TaskData_15690;

extern u8 D_800AB1C8_A2538;

void func_80014AEC_156EC(void);

void func_80014A90_15690(void) {
    TaskData_15690 *data = allocateTaskMemory(4);
    data->unk2 = 0;
    data->unk0 = 0;
    D_800AB1C8_A2538 = 0;
    D_800AFE8C_A71FC->unk8 = 1;
    D_800AFE8C_A71FC->unk9[0] = 0;
    func_80015248_15E48(0);
    func_8006983C_6A43C(func_80014AEC_156EC);
}

void func_80014B1C_1571C(void);

void func_80014AEC_156EC(void) {
    createTaskQueue(func_8001C920_1D520, 0x96);
    setGameStateHandler(func_80014B1C_1571C);
}

void func_80014C94_15894(void);

void func_80014B1C_1571C(void) {
    s16 result = func_80069810_6A410();

    if (result != 0) {
        if (result == 0xFE) {
            terminateSchedulerWithCallback(func_80015218_15E18);
        } else {
            setGameStateHandler(func_80014C94_15894);
        }
    }
}

void func_80014BA0_157A0(void);

void func_80014B70_15770(void) {
    createTaskQueue(func_80018EB0_19AB0, 0x96);
    setGameStateHandler(func_80014BA0_157A0);
}

void func_80014C0C_1580C(void);

void func_80014BA0_157A0(void) {
    s16 result;

    getCurrentAllocation();
    result = func_80069810_6A410();

    if (result != 0) {
        if (result == 0x44 || result == 0xFF) {
            setGameStateHandler(func_80014C0C_1580C);
        } else {
            terminateSchedulerWithCallback(func_80015218_15E18);
        }
    }
}

void func_80014C3C_1583C(void);
void func_80014CF8_158F8(void);

void func_80014C0C_1580C(void) {
    createTaskQueue(func_80021BD0_227D0, 0x96);
    setGameStateHandler(func_80014C3C_1583C);
}

void func_80014C3C_1583C(void) {
    s16 result = func_80069810_6A410();

    if (result != 0) {
        if (result == 0xFF) {
            setGameStateHandler(func_80014B70_15770);
        } else if (result == 1) {
            setGameStateHandler(func_80014CF8_158F8);
        }
    }
}

void func_80014C94_15894(void) {
    createTaskQueue(func_80033200_33E00, 0x96);
    setGameStateHandler(func_80014CC4_158C4);
}

void func_80014CC4_158C4(void) {
    if ((func_80069810_6A410() << 16) != 0) {
        setGameStateHandler(func_80014B70_15770);
    }
}

void func_80014D3C_1593C(void);
void func_80014D78_15978(void);

void func_80014CF8_158F8(void) {
    func_80003450_4050(D_800AFE8C_A71FC->saveSlotIndex, 0);
    createTaskQueue(loadCutsceneOverlay, 0x96);
    setGameStateHandler(func_80014D3C_1593C);
}

void func_80014D3C_1593C(void) {
    if ((func_80069810_6A410() << 16) != 0) {
        func_80015248_15E48(3);
        setGameStateHandler(func_80014D78_15978);
    }
}

void func_80014DA8_159A8(void);

void func_80014D78_15978(void) {
    createTaskQueue(initRace, 100);
    setGameStateHandler(func_80014DA8_159A8);
}

INCLUDE_ASM("asm/nonmatchings/15690", func_80014DA8_159A8);

extern u8 D_800A8CC9_A0039;

void func_80014FA4_15BA4(void);
void func_80015028_15C28(void);
void func_8001511C_15D1C(void);
void func_80015098_15C98(void);

void func_80014F60_15B60(void) {
    func_80003450_4050(D_800AFE8C_A71FC->saveSlotIndex, 1);
    createTaskQueue(loadCutsceneOverlay, 0x64);
    setGameStateHandler(func_80014FA4_15BA4);
}

void func_80014FA4_15BA4(void) {
    void *handler;

    if ((func_80069810_6A410() << 16) != 0) {
        if (D_800A8CC9_A0039 != 0) {
            handler = func_80015028_15C28;
        } else if (D_800AFE8C_A71FC->saveSlotIndex == 0xB) {
            handler = func_8001511C_15D1C;
        } else {
            handler = func_80014B70_15770;
            if (D_800AFE8C_A71FC->errorFlag != 0) {
                handler = func_80015098_15C98;
            }
        }
        setGameStateHandler(handler);
    }
}

void func_80015064_15C64(void);

void func_80015028_15C28(void) {
    func_80003450_4050(0xA, 2);
    createTaskQueue(&loadCutsceneOverlay, 0x64);
    setGameStateHandler(func_80015064_15C64);
}

void func_80015064_15C64(void) {
    if ((func_80069810_6A410() << 16) != 0) {
        setGameStateHandler(func_80014B70_15770);
    }
}

void func_80015098_15C98(void) {
    func_80003450_4050(D_800AFE8C_A71FC->errorFlag, 2);
    createTaskQueue(&loadCutsceneOverlay, 0x64);
    setGameStateHandler(func_800150DC_15CDC);
}

void func_800150DC_15CDC(void) {
    if ((func_80069810_6A410() << 16) != 0) {
        D_800AFE8C_A71FC->errorFlag = 0;
        setGameStateHandler(func_80014B70_15770);
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
