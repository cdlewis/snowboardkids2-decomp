#include "common.h"
#include "graphics.h"
#include "race_session.h"
#include "task_scheduler.h"

typedef struct {
    s16 unk0;
    u8 unk2;
} TaskData_22920;

static void loadPlayerCountSelect(void);
static void awaitPlayerCountSelect(void);
static void func_80021DE8_229E8(void);
static void func_80021EFC_22AFC(void);
static void awaitVersusCharacterSelect(void);
static void exitVersusMode(void);
static void func_80021E18_22A18(void);
static void loadVersusRace(void);
static void loadVersusSaveData(void);
static void awaitVersusMapScreen(void);
static void loadVersusMapScreen(void);
static void awaitVersusLevelSelect(void);
static void loadVersusCharacterSelect(void);
static void awaitVersusSaveDataLoad(void);
static void func_800220AC_22CAC(void);

void initVersusMode(void) {
    TaskData_22920 *data = allocateTaskMemory(4);
    data->unk0 = 0;
    data->unk2 = 0;
    setGameStateHandlerWithContinue(loadPlayerCountSelect);
}

static void loadPlayerCountSelect(void) {
    playMusicTrack(2);
    createTaskQueue(initPlayerCountSelectState, 100);
    setGameStateHandler(awaitPlayerCountSelect);
}

static void awaitPlayerCountSelect(void) {
    s16 result;

    getCurrentAllocation();
    result = getSchedulerReturnValue();

    if (result == 1) {
        setGameStateHandler(func_80021DE8_229E8);
    } else if (result == 0xFF) {
        terminateSchedulerWithCallback(exitVersusMode);
    }
}

static void func_80021DE8_229E8(void) {
    createTaskQueue(func_8001C920_1D520, 100);
    setGameStateHandler(func_80021E18_22A18);
}

static void func_80021E18_22A18(void) {
    s16 result;

    result = getSchedulerReturnValue();

    if (result != 0) {
        if (result == 0xFE) {
            terminateSchedulerWithCallback(exitVersusMode);
        } else {
            setGameStateHandler(loadVersusSaveData);
        }
    }
}

static void loadVersusMapScreen(void) {
    createTaskQueue(loadOverlay_1BBA0, 100);
    setGameStateHandler(awaitVersusMapScreen);
}

static void awaitVersusMapScreen(void) {
    s16 result;

    result = getSchedulerReturnValue();

    if (result == 0xFF) {
        setMusicFadeOut(0x14);
        terminateSchedulerWithCallback(exitVersusMode);
    } else if (result == 1) {
        setGameStateHandler(func_80021EFC_22AFC);
    }
}

static void func_80021EFC_22AFC(void) {
    createTaskQueue(initLevelSelectBasic, 100);
    setGameStateHandler(awaitVersusLevelSelect);
}

static void awaitVersusLevelSelect(void) {
    s16 result;

    result = getSchedulerReturnValue();

    if (result == 0xFF) {
        setGameStateHandler(loadVersusMapScreen);
    } else if (result == 1) {
        setGameStateHandler(loadVersusCharacterSelect);
    }
}

static void loadVersusCharacterSelect(void) {
    playMusicTrack(2);
    createTaskQueue(func_800226F0_232F0, 100);
    setGameStateHandler(awaitVersusCharacterSelect);
}

static void awaitVersusCharacterSelect(void) {
    s16 result;

    result = getSchedulerReturnValue();

    if (result != 0) {
        if (result == 1) {
            setMusicFadeOut(0x10);
            setGameStateHandler(loadVersusRace);
        } else if (result == 0xFF) {
            setGameStateHandler(func_80021EFC_22AFC);
        }
    }
}

static void loadVersusSaveData(void) {
    createTaskQueue(initControllerMotors, 0x96);
    setGameStateHandler(awaitVersusSaveDataLoad);
}

static void awaitVersusSaveDataLoad(void) {
    if ((getSchedulerReturnValue() << 16) != 0) {
        setGameStateHandler(loadVersusMapScreen);
    }
}

static void loadVersusRace(void) {
    createTaskQueue(initRace, 100);
    setGameStateHandler(func_800220AC_22CAC);
}

static void func_800220AC_22CAC(void) {
    s16 result;

    result = getSchedulerReturnValue();

    if (result != 0) {
        setViewportFadeValue(0, 0, 0);
        if (result == 1) {
            setGameStateHandler(loadVersusRace);
        } else {
            setGameStateHandler(loadVersusMapScreen);
        }
    }
}

static void exitVersusMode(void) {
    createRootTaskScheduler(func_8001452C_1512C, 0xC8);
}
