#include "common.h"
#include "gamestate.h"
#include "geometry.h"
#include "task_scheduler.h"

extern s16 func_8006D21C_6DE1C(s32, s32, s32, s32);
extern s32 distance_2d(s32, s32);
extern s32 func_8002A4AC_2B0AC(void *, u8);
extern s32 func_8002A7CC_2B3CC(void *);

s32 func_8002ACFC_2B8FC(s32, s32, s16);

typedef struct {
    /* 0x00 */ void *model;
    /* 0x04 */ Mat3x3Padded matrix;
    /* 0x24 */ u8 pad24[0xA];
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ u8 pad30[0x20];
    /* 0x50 */ u16 unk50;
    /* 0x52 */ u8 pad52[0x2];
    /* 0x54 */ u16 unk54;
    /* 0x56 */ u8 pad56[0x7];
    /* 0x5D */ u8 unk5D;
    /* 0x5E */ u8 unk5E;
    /* 0x5F */ u8 pad5F[0x2];
    /* 0x61 */ u8 unk61;
} Func8002A390Arg;

s32 func_8002A390_2AF90(Func8002A390Arg *arg0) {
    GameState *state;
    s32 stateVal;

    state = getCurrentAllocation();
    stateVal = arg0->unk5E;

    switch (stateVal) {
        case 0:
            if (func_8002A4AC_2B0AC(&arg0->matrix, arg0->unk5D) != 0) {
                return 1;
            }
            if (((u8 *)state->unk420)[1] != 0) {
                return 0;
            }
            if (func_8002ACFC_2B8FC(arg0->matrix.unk14, arg0->matrix.unk1C, arg0->unk2E) == 0) {
                return 0;
            }
            arg0->unk54 = arg0->unk50;
            if (arg0->unk5D == 5) {
                arg0->unk50 = 0x21;
            } else {
                arg0->unk50 = 0;
            }
            arg0->unk61 = arg0->unk5E;
            arg0->unk5E = 0x44;
            break;
        case 1:
        case 2:
        case 3:
        case 4:
            if (func_8002A7CC_2B3CC(arg0) == 0) {
                return 0;
            }
            arg0->unk61 = arg0->unk5E;
            arg0->unk5E = 0;
            break;
        case 0x44:
            if (func_8002A7CC_2B3CC(arg0) == 0) {
                return 0;
            }
            arg0->unk61 = arg0->unk5E;
            arg0->unk5E = 0;
            arg0->unk50 = arg0->unk54;
            break;
    }

    return 0;
}

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
