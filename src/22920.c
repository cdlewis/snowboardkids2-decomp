#include "common.h"
#include "graphics.h"
#include "task_scheduler.h"

INCLUDE_ASM("asm/nonmatchings/22920", func_80021D20_22920);

extern void func_80022130_22D30(void);
void func_80021D88_22988(void);

void func_80021D50_22950(void) {
    func_800574A0_580A0(2);
    createTaskQueue(func_80022130_22D30, 100);
    setGameStateHandler(func_80021D88_22988);
}

extern void func_800226F0_232F0(void);

void func_80021DE8_229E8(void);
void func_80021EFC_22AFC(void);
void func_80021FB8_22BB8(void);
void func_80022108_22D08(void);

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

INCLUDE_ASM("asm/nonmatchings/22920", func_80021DE8_229E8);

INCLUDE_ASM("asm/nonmatchings/22920", func_80021E18_22A18);

INCLUDE_ASM("asm/nonmatchings/22920", func_80021E6C_22A6C);

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

INCLUDE_ASM("asm/nonmatchings/22920", func_80021EFC_22AFC);

INCLUDE_ASM("asm/nonmatchings/22920", func_80021F2C_22B2C);

void func_80021F80_22B80(void) {
    func_800574A0_580A0(2);
    createTaskQueue(func_800226F0_232F0, 100);
    setGameStateHandler(func_80021FB8_22BB8);
}

INCLUDE_ASM("asm/nonmatchings/22920", func_80021FB8_22BB8);

INCLUDE_ASM("asm/nonmatchings/22920", func_80022018_22C18);

INCLUDE_ASM("asm/nonmatchings/22920", func_80022048_22C48);

INCLUDE_ASM("asm/nonmatchings/22920", func_8002207C_22C7C);

INCLUDE_ASM("asm/nonmatchings/22920", func_800220AC_22CAC);

INCLUDE_ASM("asm/nonmatchings/22920", func_80022108_22D08);
