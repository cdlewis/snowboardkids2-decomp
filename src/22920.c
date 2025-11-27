#include "common.h"
#include "task_scheduler.h"

INCLUDE_ASM("asm/nonmatchings/22920", func_80021D20_22920);

INCLUDE_ASM("asm/nonmatchings/22920", func_80021D50_22950);

void func_80021DE8_229E8(void);
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

INCLUDE_ASM("asm/nonmatchings/22920", func_80021E9C_22A9C);

INCLUDE_ASM("asm/nonmatchings/22920", func_80021EFC_22AFC);

INCLUDE_ASM("asm/nonmatchings/22920", func_80021F2C_22B2C);

INCLUDE_ASM("asm/nonmatchings/22920", func_80021F80_22B80);

INCLUDE_ASM("asm/nonmatchings/22920", func_80021FB8_22BB8);

INCLUDE_ASM("asm/nonmatchings/22920", func_80022018_22C18);

INCLUDE_ASM("asm/nonmatchings/22920", func_80022048_22C48);

INCLUDE_ASM("asm/nonmatchings/22920", func_8002207C_22C7C);

INCLUDE_ASM("asm/nonmatchings/22920", func_800220AC_22CAC);

INCLUDE_ASM("asm/nonmatchings/22920", func_80022108_22D08);
