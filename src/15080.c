#include "15690.h"
#include "3E160.h"
#include "4050.h"
#include "6E840.h"
#include "D_800AFE8C_A71FC_type.h"
#include "EepromSaveData_type.h"
#include "common.h"
#include "memory_allocator.h"
#include "task_scheduler.h"
extern u8 gDebugUnlockEnabled;

void func_80014990_15590(void);

void func_800144DC_150DC(void);
void func_8001489C_1549C(void);
void func_80014690_15290(void);
void func_80014660_15260(void);
void waitForLogoScreen(void);
void startLogoScreen(void);
void func_800148CC_154CC(void);
void func_8001474C_1534C(void);
void func_8001478C_1538C(void);
void startDemoRace(void);
void waitForDemoRace(void);
void func_80014900_15500(void);
void func_80014808_15408(void);
void func_80014854_15454(void);
void func_80014958_15558(void);

typedef struct {
    s16 unk0;
    u8 unk2;
    u8 pad;
} TaskData_1512C;

void func_80014480_15080(void) {
    u8 allocation;

    D_800AFE8C_A71FC = allocateMemoryNode(0, 0x28, &allocation);
    EepromSaveData = allocateMemoryNode(0, 0x5C, &allocation);
    gDebugUnlockEnabled = 0;
    func_80014958_15558();
    func_8006983C_6A43C(func_800144DC_150DC);
}

void func_800144DC_150DC(void) {
    TaskData_1512C *data = allocateTaskMemory(4);
    func_8006FDA0_709A0(NULL, 0, 0);
    data->unk0 = 0;
    data->unk2 = 0;
    func_80014990_15590();
    func_8006983C_6A43C(startLogoScreen);
}

void func_8001452C_1512C(void) {
    TaskData_1512C *data = allocateTaskMemory(4);
    func_8006FDA0_709A0(NULL, 0, 0);
    data->unk0 = 0;
    data->unk2 = 0;
    func_80014990_15590();
    func_8006983C_6A43C(func_80014660_15260);
}

void startLogoScreen(void) {
    createTaskQueue(initLogoScreen, 0x64);
    setGameStateHandler(waitForLogoScreen);
}

void waitForLogoScreen(void) {
    if ((func_80069810_6A410() << 16) != 0) {
        setGameStateHandler(startDemoRace);
    }
}

void startDemoRace(void) {
    D_800AFE8C_A71FC->unk4 = 3;
    createTaskQueue(initRace, 0x96);
    setGameStateHandler(waitForDemoRace);
}

void waitForDemoRace(void) {
    if ((func_80069810_6A410() << 16) != 0) {
        D_800AFE8C_A71FC->unk4 = 0;
        setGameStateHandler(func_80014660_15260);
    }
}

void func_80014660_15260(void) {
    createTaskQueue(initTitleScreen, 0x64);
    setGameStateHandler(func_80014690_15290);
}

void func_80014690_15290(void) {
    s16 result = func_80069810_6A410();

    if (result == 0) {
        return;
    }

    if (result == 4) {
        D_800AFE8C_A71FC_type *ptr = D_800AFE8C_A71FC;
        u8 val = ptr->unk5;

        if (val >= 3) {
            ptr->unk5 = val & 0xF;
            setGameStateHandler(startDemoRace);
        } else {
            setGameStateHandler(func_8001474C_1534C);
        }
    } else if (result == 2) {
        setGameStateHandler(func_80014808_15408);
    } else if (result == 3) {
        setGameStateHandler(func_8001489C_1549C);
    } else {
        terminateSchedulerWithCallback(func_80014900_15500);
    }
}

void func_8001474C_1534C(void) {
    D_800AFE8C_A71FC->unk4 = 2;
    createTaskQueue(initRace, 100);
    setGameStateHandler(func_8001478C_1538C);
}

void func_8001478C_1538C(void) {
    if ((func_80069810_6A410() << 16) != 0) {
        D_800AFE8C_A71FC->unk5 = (D_800AFE8C_A71FC->unk5 + 1) % 3;
        D_800AFE8C_A71FC->unk5 |= 0xF0;
        setGameStateHandler(func_80014660_15260);
    }
}

void func_80014808_15408(void) {
    D_800AFE8C_A71FC->unk4 = 0;
    D_800AFE8C_A71FC->saveSlotIndex = 0xF;
    createTaskQueue(initRace, 100);
    setGameStateHandler(func_80014854_15454);
}

void func_80014854_15454(void) {
    s16 result = func_80069810_6A410();

    if (result == 0) {
        return;
    }

    if (result == 1) {
        setGameStateHandler(func_80014808_15408);
    } else {
        setGameStateHandler(func_80014660_15260);
    }
}

void func_8001489C_1549C(void) {
    createTaskQueue(func_80037270_37E70, 0x64);
    setGameStateHandler(func_800148CC_154CC);
}

void func_800148CC_154CC(void) {
    if ((func_80069810_6A410() << 16) != 0) {
        setGameStateHandler(func_80014660_15260);
    }
}

void func_80014900_15500(void) {
    u8 val = D_800AFE8C_A71FC->unk4;

    if (val == 0xFF) {
        func_800693C4_69FC4(__udiv_w_sdiv(), 0xC8);
    } else if (val == 0) {
        func_800693C4_69FC4(func_80014A90_15690, 0xC8);
    } else {
        func_800693C4_69FC4(func_80021D20_22920, 0xC8);
    }
}

void func_80014958_15558(void) {
    D_800AFE8C_A71FC->unk1F = 0;
    D_800AFE8C_A71FC->unk20 = 0;
    D_800AFE8C_A71FC->unk21 = 0;
    D_800AFE8C_A71FC->unk22 = 3;
}

void func_80014990_15590(void) {
    s32 i;

    D_800AFE8C_A71FC->unk4 = 0;
    D_800AFE8C_A71FC->saveSlotIndex = 0;

    if (D_800AFE8C_A71FC->unk21 == 1) {
        D_800AFE8C_A71FC->unk9[0x10] = D_800AFE8C_A71FC->unk22;
    } else {
        D_800AFE8C_A71FC->unk9[0x10] = 3;
    }

    D_800AFE8C_A71FC->unk8 = 0;
    D_800AFE8C_A71FC->padding = 0;

    for (i = 0; i < 4; i++) {
        D_800AFE8C_A71FC->unk9[i] = 0;
        D_800AFE8C_A71FC->unk9[i + 4] = 0;
        D_800AFE8C_A71FC->unk9[i + 0xC] = 0;
        D_800AFE8C_A71FC->unk9[i + 8] = 0;
        D_800AFE8C_A71FC->unk9[i + 0x11] = 0;
    }

    D_800AFE8C_A71FC->unk9[0x15] = 0;
    D_800AFE8C_A71FC->unk5 = 0;
    D_800AFE8C_A71FC->errorFlag = 0;
    D_800AFE8C_A71FC->unk24 = 0;
    D_800AFE8C_A71FC->unk25 = 0;
}
