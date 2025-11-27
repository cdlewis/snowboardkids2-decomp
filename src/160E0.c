#include "6E840.h"
#include "common.h"
#include "task_scheduler.h"

extern void func_80058220_58E20(s32, s32);
extern void func_800156AC_162AC(void);

extern u8 D_8009F210_9FE10;

void func_800154E0_160E0(void) {
    func_800697F4_6A3F4(D_8009F210_9FE10);
}

INCLUDE_ASM("asm/nonmatchings/160E0", func_80015504_16104);

INCLUDE_ASM("asm/nonmatchings/160E0", func_8001563C_1623C);

INCLUDE_ASM("asm/nonmatchings/160E0", func_800156AC_162AC);

void func_80015960_16560(void) {
    if (func_8006FE10_70A10(0) == 0) {
        func_8006FE28_70A28(0, 0, 0, 0);
        func_80058220_58E20(0x135, 0);
        setGameStateHandler(func_800156AC_162AC);
    }
}

INCLUDE_ASM("asm/nonmatchings/160E0", func_800159AC_165AC);

INCLUDE_ASM("asm/nonmatchings/160E0", func_80015A18_16618);

INCLUDE_ASM("asm/nonmatchings/160E0", func_80015CA4_168A4);

INCLUDE_ASM("asm/nonmatchings/160E0", func_80015DF4_169F4);

INCLUDE_ASM("asm/nonmatchings/160E0", func_80016070_16C70);

INCLUDE_ASM("asm/nonmatchings/160E0", func_80016150_16D50);

INCLUDE_ASM("asm/nonmatchings/160E0", func_800161F4_16DF4);
