#include "common.h"
#include "gamestate.h"
#include "task_scheduler.h"

extern s16 func_8006D21C_6DE1C(s32, s32, s32, s32);
extern s32 distance_2d(s32, s32);

INCLUDE_ASM("asm/nonmatchings/2AF90", func_8002A390_2AF90);

INCLUDE_ASM("asm/nonmatchings/2AF90", func_8002A4AC_2B0AC);

INCLUDE_ASM("asm/nonmatchings/2AF90", func_8002A7CC_2B3CC);

s32 func_8002ACFC_2B8FC(s32 arg0, s32 arg1, s16 arg2) {
    GameState *state;
    s16 angle;

    state = getCurrentAllocation();
    angle = func_8006D21C_6DE1C(state->unk3EC, state->unk3F0, arg0, arg1);

    if (arg2 - 0x238 < angle && angle < arg2 + 0x238) {
        if (distance_2d(state->unk3EC - arg0, state->unk3F0 - arg1) <= 0x280000) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/2AF90", func_8002ADB4_2B9B4);

INCLUDE_ASM("asm/nonmatchings/2AF90", func_8002AE80_2BA80);

INCLUDE_ASM("asm/nonmatchings/2AF90", func_8002B248_2BE48);

s16 func_8002B4B8_2C0B8(u16 arg0, u16 arg1) {
    s16 diff;

    arg0 &= 0x1FFF;
    arg1 &= 0x1FFF;
    diff = (arg1 - arg0) & 0x1FFF;

    if (diff >= 0x1001) {
        diff |= 0xE000;
    }

    return diff;
}

INCLUDE_ASM("asm/nonmatchings/2AF90", func_8002B4E4_2C0E4);

INCLUDE_ASM("asm/nonmatchings/2AF90", func_8002B598_2C198);

INCLUDE_ASM("asm/nonmatchings/2AF90", func_8002B760_2C360);

INCLUDE_ASM("asm/nonmatchings/2AF90", func_8002B94C_2C54C);

INCLUDE_ASM("asm/nonmatchings/2AF90", func_8002BAEC_2C6EC);
