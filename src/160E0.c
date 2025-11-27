#include "3A1F0.h"
#include "6E840.h"
#include "common.h"
#include "gamestate.h"
#include "graphics.h"
#include "task_scheduler.h"

extern void func_80058220_58E20(s32, s32);
extern void func_800156AC_162AC(void);

extern void func_8003B47C_3C07C(void *);
extern void *func_8003B510_3C110(void);

extern u8 D_8009F210_9FE10;

void func_800154E0_160E0(void) {
    func_800697F4_6A3F4(D_8009F210_9FE10);
}

INCLUDE_ASM("asm/nonmatchings/160E0", func_80015504_16104);

void func_8001563C_1623C(void) {
    u8 buffer[0x200];
    s32 i;
    s32 result;

    for (i = 0x1FF; i >= 0; i--) {
        buffer[i] = 0;
    }

    func_8003B47C_3C07C(buffer);

    do {
        result = (s32)func_8003B510_3C110();
    } while (result == -1);

    if (result == 0) {
        func_800585C8_591C8(0x2C);
    } else {
        func_800585C8_591C8(0x2B);
    }

    setGameStateHandler(func_800156AC_162AC);
}

INCLUDE_ASM("asm/nonmatchings/160E0", func_800156AC_162AC);

void func_80015960_16560(void) {
    if (func_8006FE10_70A10(0) == 0) {
        func_8006FE28_70A28(0, 0, 0, 0);
        func_80058220_58E20(0x135, 0);
        setGameStateHandler(func_800156AC_162AC);
    }
}

void func_800159AC_165AC(void) {
    GameState *state = (GameState *)getCurrentAllocation();
    u16 *counter = (u16 *)&state->audioPlayer5;

    (*counter)++;

    if (*counter >= 3) {
        *counter = 2;
        if (func_8003BB5C_3C75C() == 0) {
            *counter = 0;
            func_8006FDA0_709A0(NULL, 0, 0x10);
            setGameStateHandler(func_80015960_16560);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/160E0", func_80015A18_16618);

INCLUDE_ASM("asm/nonmatchings/160E0", func_80015CA4_168A4);

INCLUDE_ASM("asm/nonmatchings/160E0", func_80015DF4_169F4);

INCLUDE_ASM("asm/nonmatchings/160E0", func_80016070_16C70);

INCLUDE_ASM("asm/nonmatchings/160E0", func_80016150_16D50);

INCLUDE_ASM("asm/nonmatchings/160E0", func_800161F4_16DF4);
