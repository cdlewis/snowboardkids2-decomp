#include "1E2BE0.h"
#include "common.h"
#include "geometry.h"
#include "graphics.h"
#include "task_scheduler.h"

extern Mat3x3Padded D_8009A890_9B490;

void func_800B5B30_1E2BE0(func_800B5E64_1E2F14_arg0 *arg0, s8 arg1) {
    arg0->unk71 = arg1;
}

void func_800B5B7C_1E2C2C(func_800B5E64_1E2F14_arg0 *, u16);

void *func_800B5B38_1E2BE8(u16 arg0) {
    func_800B5E64_1E2F14_arg0 *temp = allocateNodeMemory(0x74);
    func_800B5B7C_1E2C2C(temp, arg0);
    return temp;
}

void func_800B5B7C_1E2C2C(func_800B5E64_1E2F14_arg0 *arg0, u16 arg1) {
    arg0->unk20 = 0;
    arg0->unk22 = 0;
    arg0->unk24 = 0;
    arg0->unk26 = 0;
    arg0->unk28 = 0;
    arg0->unk2A = 0;
    arg0->unk2C = 0;
    arg0->unk2E = 0;
    arg0->unk30 = 0;
    arg0->unk32 = 0;
    arg0->unk34 = 0;
    arg0->unk38 = 0;
    arg0->unk3C = 0;
    arg0->unk40 = 0;
    arg0->unk44 = 0;
    arg0->unk48 = 0;
    arg0->unk4C = 0;
    arg0->unk50 = 0;
    arg0->unk54 = 0;
    arg0->unk58 = 0;
    arg0->unk5A = 0;
    arg0->unk5C = 0;
    arg0->unk5E = 0;
    arg0->unk60 = 0;
    arg0->unk62 = 0;
    arg0->unk64 = 0;
    arg0->unk68 = 0;
    arg0->unk6C = 0;
    arg0->unk6E = arg1;
    arg0->unk70 = 0;
    arg0->unk71 = 0;
}

void func_800B5BFC_1E2CAC(void *arg0) {
    freeNodeMemory(arg0);
}

typedef struct {
    u8 _pad[0x20];
    s16 unk20;
    s16 unk22;
    s16 unk24;
    s16 unk26;
    s16 unk28;
    s16 unk2A;
    s16 unk2C;
    s16 unk2E;
    s16 unk30;
    s16 unk32;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    s32 unk54;
    s16 unk58;
    s16 unk5A;
    s16 unk5C;
    s16 unk5E;
    s16 unk60;
    s16 unk62;
    s32 unk64;
    s32 unk68;
    s16 unk6C;
    s16 unk6E;
    s8 unk70;
    s8 unk71;
} AnimationLoopArg;

void finalizeAnimationLoop(AnimationLoopArg *arg0) {
    Mat3x3Padded sp10;
    Mat3x3Padded sp30;
    Mat3x3Padded sp50;
    Mat3x3Padded sp70;
    Mat3x3Padded *temp_s0;

    D_8009A890_9B490.unk14 = arg0->unk34;
    D_8009A890_9B490.unk18 = arg0->unk38 + arg0->unk64;
    D_8009A890_9B490.unk1C = arg0->unk3C;

    createYRotationMatrix(&sp70, arg0->unk22);

    sp70.unk14 = 0;
    sp70.unk18 = 0;
    sp70.unk1C = 0;

    func_8006B084_6BC84(&D_8009A890_9B490, &sp70, &sp30);

    createXRotationMatrix(sp50.m, arg0->unk20);

    sp50.unk14 = 0;
    sp50.unk18 = 0;
    sp50.unk1C = 0;

    func_8006B084_6BC84(&sp50, &sp30, &sp10);

    temp_s0 = &sp10;
    memcpy(arg0, temp_s0, 0x20);
    func_80056914_57514(temp_s0);
    func_8006FD3C_7093C(arg0->unk6E, temp_s0);
}

extern s32 gControllerInputs[];
extern s32 gButtonsPressed[];
extern s8 D_800AB04B;
extern s8 D_800AB044_A23B4;
extern u8 D_800AB048_A23B8;

void func_800B5CFC_1E2DAC(func_800B5E64_1E2F14_arg0 *arg0) {
    s8 mode = arg0->unk71;
    s32 temp;
    s32 buttonCheck;

    switch (mode) {
        case 0:
            if (gControllerInputs[3] & Z_TRIG) {
                arg0->unk34 = 0;
            } else {
                temp = arg0->unk34;
                temp += D_800AB04B << 12;
                arg0->unk34 = temp;
            }

            buttonCheck = gButtonsPressed[0];

            if (buttonCheck & R_TRIG) {
                temp = arg0->unk38;
                temp += D_800AB044_A23B4 << 12;
                arg0->unk38 = temp;
            } else if (buttonCheck & Z_TRIG) {
                temp = arg0->unk3C;
                temp -= D_800AB044_A23B4 << 12;
                arg0->unk3C = temp;
            } else if (buttonCheck & L_TRIG) {
                u16 temp_angle = arg0->unk20;
                temp_angle -= D_800AB044_A23B4;
                arg0->unk20 = temp_angle & 0x1FFF;
            } else {
                u16 temp_angle = arg0->unk22;
                temp_angle -= (s8)D_800AB048_A23B8;
                arg0->unk22 = temp_angle & 0x1FFF;
            }
            break;

        case 1:
            if (gControllerInputs[3] & Z_TRIG) {
                arg0->unk34 = 0;
            } else if (gButtonsPressed[3] & R_TRIG) {
                temp = arg0->unk34;
                temp += D_800AB04B << 16;
                arg0->unk34 = temp;
            } else {
                temp = arg0->unk34;
                temp += D_800AB04B << 12;
                arg0->unk34 = temp;
            }

            if (gButtonsPressed[0] & R_TRIG) {
                temp = arg0->unk38;
                temp += D_800AB044_A23B4 << 12;
                arg0->unk38 = temp;
            }
            break;
    }
}

void func_800B5E64_1E2F14(func_800B5E64_1E2F14_arg0 *arg0, s16 arg1, s16 arg2, s32 arg3, s32 arg4) {
    arg0->unk20 = arg1;
    arg0->unk22 = arg2;
    arg0->unk24 = arg1;
    arg0->unk26 = arg2;
    arg0->unk28 = 0;
    arg0->unk2A = 0;
    arg0->unk2C = 0;
    arg0->unk2E = 0;
    arg0->unk30 = 0;
    arg0->unk32 = 0;
    arg0->unk38 = arg3;
    arg0->unk44 = arg3;
    arg0->unk4C = 0;
    arg0->unk50 = 0;
    arg0->unk54 = 0;
    arg0->unk5A = 0;
    arg0->unk5C = 0;
    arg0->unk60 = 0;
    arg0->unk62 = 0;
    arg0->unk70 = 0;
    arg0->unk3C = arg4;
    arg0->unk48 = arg4;
}

void func_800B5EC4_1E2F74(func_800B5E64_1E2F14_arg0 *arg0, s16 arg1, s16 arg2, s32 arg3, s32 arg4, s32 arg5) {
    func_800B5E64_1E2F14(arg0, arg1, arg2, arg3, arg4);
    arg0->unk34 = arg5;
    arg0->unk40 = arg5;
    arg0->unk4C = 0;
    arg0->unk58 = 0;
    arg0->unk5E = 0;
}

typedef struct {
    u8 padding[0x34];
    s32 unk34;
} func_800B5F20_1E2FD0_arg0;
typedef struct {
    u8 padding[0x2C];
    s32 unk2C;
} func_800B5F20_1E2FD0_arg1;
void func_800B5F20_1E2FD0(func_800B5F20_1E2FD0_arg0 *arg0, func_800B5F20_1E2FD0_arg1 *arg1) {
    arg0->unk34 = arg1->unk2C;
}

INCLUDE_ASM("asm/nonmatchings/1E2BE0", func_800B5F2C_1E2FDC);

INCLUDE_ASM("asm/nonmatchings/1E2BE0", func_800B5FB0_1E3060);

void func_800B6034_1E30E4(func_800B5E64_1E2F14_arg0 *arg0, s32 arg1, s16 arg2) {
    s32 quotient;

    quotient = (arg1 - arg0->unk34) / arg2;

    arg0->unk70 = 0;
    arg0->unk40 = arg1;
    arg0->unk58 = arg2;
    arg0->unk5E = arg2;
    arg0->unk4C = quotient;
}

void func_800B6088_1E3138(func_800B5E64_1E2F14_arg0 *arg0, s32 arg1, s16 arg2) {
    s32 quotient;

    quotient = (arg1 - arg0->unk38) / arg2;

    arg0->unk70 = 0;
    arg0->unk44 = arg1;
    arg0->unk5A = arg2;
    arg0->unk60 = arg2;
    arg0->unk50 = quotient;
}

void func_800B60DC_1E318C(func_800B5E64_1E2F14_arg0 *arg0, s32 arg1, s16 arg2) {
    s32 quotient;

    quotient = (arg1 - arg0->unk3C) / arg2;

    arg0->unk70 = 0;
    arg0->unk48 = arg1;
    arg0->unk5C = arg2;
    arg0->unk62 = arg2;
    arg0->unk54 = quotient;
}

void func_800B6130_1E31E0(func_800B5E64_1E2F14_arg0 *arg0, s16 arg1, s16 arg2) {
    if ((arg1 == 0) || (arg2 == 0)) {
        arg0->unk70 = 0;
        arg0->unk2A = 0;
        arg0->unk2E = 0;
        arg0->unk32 = 0;
    } else {
        arg0->unk70 = 1;
        arg0->unk2A = arg1;
        arg0->unk2E = arg2;
        arg0->unk32 = arg2;
    }
}

typedef struct {
    u8 _pad[0x68];
    s32 unk68;
    s16 unk6C;
    s16 unk6E;
    u8 unk70;
    u8 unk71;
} func_800B6180_1E3230_arg0;

void func_800B6180_1E3230(func_800B6180_1E3230_arg0 *arg0, s32 arg1, s16 arg2) {
    arg0->unk70 = 0;
    arg0->unk68 = arg1;
    arg0->unk6C = arg2;
}

INCLUDE_ASM("asm/nonmatchings/1E2BE0", func_800B6190_1E3240);

s16 func_800B6544_1E35F4(func_800B5E64_1E2F14_arg0 *arg0) {
    if (arg0->unk32 != 0) {
        arg0->unk22 += arg0->unk2A;
        if (arg0->unk32 > 0) {
            arg0->unk32--;
        }
    } else {
        arg0->unk70 = 2;
    }
    return 0;
}

extern s16 func_800B6190_1E3240(func_800B5E64_1E2F14_arg0 *);

s16 advanceSceneManager(func_800B5E64_1E2F14_arg0 *arg0) {
    s16 result = 0;
    s8 temp;

    if (arg0->unk71 == 1) {
        arg0->unk44 = 0x166666;
        arg0->unk38 = 0x166666;
    }

    temp = arg0->unk70;

    if (temp != 1) {
        if (temp >= 2) {
        } else if (temp == 0) {
            result = func_800B6190_1E3240(arg0);
        }
    } else {
        result = func_800B6544_1E35F4(arg0);
    }

    return result;
}
