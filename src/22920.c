#include "3E160.h"
#include "common.h"
#include "graphics.h"
#include "task_scheduler.h"

typedef struct {
    s16 unk0;
    u8 unk2;
} TaskData_22920;

void loadPlayerCountSelect(void);

void initVersusMode(void) {
    TaskData_22920 *data = allocateTaskMemory(4);
    data->unk0 = 0;
    data->unk2 = 0;
    func_8006983C_6A43C(loadPlayerCountSelect);
}

void awaitPlayerCountSelect(void);

void loadPlayerCountSelect(void) {
    func_800574A0_580A0(2);
    createTaskQueue(func_80022130_22D30, 100);
    setGameStateHandler(awaitPlayerCountSelect);
}

void func_80021DE8_229E8(void);
void func_80021EFC_22AFC(void);
void awaitVersusCharacterSelect(void);
void func_80022108_22D08(void);
void func_80021E18_22A18(void);
void func_8002207C_22C7C(void);

void awaitPlayerCountSelect(void) {
    s16 result;

    getCurrentAllocation();
    result = func_80069810_6A410();

    if (result == 1) {
        setGameStateHandler(func_80021DE8_229E8);
    } else if (result == 0xFF) {
        terminateSchedulerWithCallback(func_80022108_22D08);
    }
}

void func_80021DE8_229E8(void) {
    createTaskQueue(func_8001C920_1D520, 100);
    setGameStateHandler(func_80021E18_22A18);
}

void loadVersusSaveData(void);

void func_80021E18_22A18(void) {
    s16 result;

    result = func_80069810_6A410();

    if (result != 0) {
        if (result == 0xFE) {
            terminateSchedulerWithCallback(func_80022108_22D08);
        } else {
            setGameStateHandler(loadVersusSaveData);
        }
    }
}

void awaitVersusMapScreen(void);

void loadVersusMapScreen(void) {
    createTaskQueue(loadOverlay_1BBA0, 100);
    setGameStateHandler(awaitVersusMapScreen);
}

void awaitVersusMapScreen(void) {
    s16 result;

    result = func_80069810_6A410();

    if (result == 0xFF) {
        func_80057564_58164(0x14);
        terminateSchedulerWithCallback(func_80022108_22D08);
    } else if (result == 1) {
        setGameStateHandler(func_80021EFC_22AFC);
    }
}

void awaitVersusLevelSelect(void);
void loadVersusCharacterSelect(void);

void func_80021EFC_22AFC(void) {
    createTaskQueue(initLevelSelectBasic, 100);
    setGameStateHandler(awaitVersusLevelSelect);
}

void awaitVersusLevelSelect(void) {
    s16 result;

    result = func_80069810_6A410();

    if (result == 0xFF) {
        setGameStateHandler(loadVersusMapScreen);
    } else if (result == 1) {
        setGameStateHandler(loadVersusCharacterSelect);
    }
}

void loadVersusCharacterSelect(void) {
    func_800574A0_580A0(2);
    createTaskQueue(func_800226F0_232F0, 100);
    setGameStateHandler(awaitVersusCharacterSelect);
}

void awaitVersusCharacterSelect(void) {
    s16 result;

    result = func_80069810_6A410();

    if (result != 0) {
        if (result == 1) {
            func_80057564_58164(0x10);
            setGameStateHandler(func_8002207C_22C7C);
        } else if (result == 0xFF) {
            setGameStateHandler(func_80021EFC_22AFC);
        }
    }
}

void awaitVersusSaveDataLoad(void);
void func_800220AC_22CAC(void);

void loadVersusSaveData(void) {
    createTaskQueue(func_80033200_33E00, 0x96);
    setGameStateHandler(awaitVersusSaveDataLoad);
}

void awaitVersusSaveDataLoad(void) {
    if ((func_80069810_6A410() << 16) != 0) {
        setGameStateHandler(loadVersusMapScreen);
    }
}

void func_8002207C_22C7C(void) {
    createTaskQueue(initRace, 100);
    setGameStateHandler(func_800220AC_22CAC);
}

void func_800220AC_22CAC(void) {
    s16 result;

    result = func_80069810_6A410();

    if (result != 0) {
        func_8006FDA0_709A0(0, 0, 0);
        if (result == 1) {
            setGameStateHandler(func_8002207C_22C7C);
        } else {
            setGameStateHandler(loadVersusMapScreen);
        }
    }
}

void func_80022108_22D08(void) {
    func_800693C4_69FC4(func_8001452C_1512C, 0xC8);
}
