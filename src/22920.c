#include "3E160.h"
#include "common.h"
#include "graphics.h"
#include "task_scheduler.h"

typedef struct {
    s16 unk0;
    u8 unk2;
} TaskData_22920;

void func_80021D50_22950(void);

void func_80021D20_22920(void) {
    TaskData_22920 *data = allocateTaskMemory(4);
    data->unk0 = 0;
    data->unk2 = 0;
    func_8006983C_6A43C(func_80021D50_22950);
}

extern void func_80022130_22D30(void);
void func_80021D88_22988(void);

void func_80021D50_22950(void) {
    func_800574A0_580A0(2);
    createTaskQueue(func_80022130_22D30, 100);
    setGameStateHandler(func_80021D88_22988);
}

extern void func_800226F0_232F0(void);
extern void func_8001C920_1D520(void);
extern void func_8001452C_1512C(void);

void func_80021DE8_229E8(void);
void func_80021EFC_22AFC(void);
void func_80021FB8_22BB8(void);
void func_80022108_22D08(void);
void func_80021E18_22A18(void);
void func_8002207C_22C7C(void);

void func_80021D88_22988(void) {
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

void func_80022018_22C18(void);

void func_80021E18_22A18(void) {
    s16 result;

    result = func_80069810_6A410();

    if (result != 0) {
        if (result == 0xFE) {
            terminateSchedulerWithCallback(func_80022108_22D08);
        } else {
            setGameStateHandler(func_80022018_22C18);
        }
    }
}

extern void loadOverlay_1BBA0(void);
void func_80021E9C_22A9C(void);

void func_80021E6C_22A6C(void) {
    createTaskQueue(loadOverlay_1BBA0, 100);
    setGameStateHandler(func_80021E9C_22A9C);
}

void func_80021E9C_22A9C(void) {
    s16 result;

    result = func_80069810_6A410();

    if (result == 0xFF) {
        func_80057564_58164(0x14);
        terminateSchedulerWithCallback(func_80022108_22D08);
    } else if (result == 1) {
        setGameStateHandler(func_80021EFC_22AFC);
    }
}

extern void func_8001E5C0_1F1C0(void);
void func_80021F2C_22B2C(void);

void func_80021EFC_22AFC(void) {
    createTaskQueue(func_8001E5C0_1F1C0, 100);
    setGameStateHandler(func_80021F2C_22B2C);
}

INCLUDE_ASM("asm/nonmatchings/22920", func_80021F2C_22B2C);

void func_80021F80_22B80(void) {
    func_800574A0_580A0(2);
    createTaskQueue(func_800226F0_232F0, 100);
    setGameStateHandler(func_80021FB8_22BB8);
}

void func_80021FB8_22BB8(void) {
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

extern void func_80033200_33E00(void);

void func_80022048_22C48(void);
void func_800220AC_22CAC(void);

void func_80022018_22C18(void) {
    createTaskQueue(func_80033200_33E00, 0x96);
    setGameStateHandler(func_80022048_22C48);
}

void func_80022048_22C48(void) {
    if ((func_80069810_6A410() << 16) != 0) {
        setGameStateHandler(func_80021E6C_22A6C);
    }
}

void func_8002207C_22C7C(void) {
    createTaskQueue(func_8003D560_3E160, 100);
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
            setGameStateHandler(func_80021E6C_22A6C);
        }
    }
}

void func_80022108_22D08(void) {
    func_800693C4_69FC4(func_8001452C_1512C, 0xC8);
}
