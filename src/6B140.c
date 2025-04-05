#include "common.h"

typedef struct {
    s16 m[9];
} Mat3x3;

typedef struct {
    u8 padding[0x14];
    s32 unk14;
    s32 unk18;
    s32 unk1C;
} func_8006AFDC_6BBDC_arg;

s16 approximateSin(s16 inputAngle) {
    u16 temp_a0;
    inputAngle = inputAngle & 0x1FFF;
    temp_a0 = inputAngle;

    if (temp_a0 == 0x800) {
        return 0x2000;
    }

    if (temp_a0 == 0x1800) {
        return -0x2000;
    }

    return (sins(inputAngle << 3) << 0x10) >> 0x12;
}

s16 approximateCos(s16 inputAngle) {
    u16 temp_a0;
    inputAngle = inputAngle & 0x1FFF;
    temp_a0 = inputAngle;

    if (temp_a0 == 0) {
        return 0x2000;
    }

    if (temp_a0 == 0x1000) {
        return -0x2000;
    }

    return (coss(inputAngle << 3) << 0x10) >> 0x12;
}

void createXRotationMatrix(s16 matrix[3][3], s16 angle) {
    s16 sinVal;
    s16 cosVal;
    sinVal = approximateSin(angle);
    sinVal++;
    sinVal--;
    cosVal = approximateCos(angle);
    matrix[0][0] = 0x2000;
    matrix[0][1] = 0;
    matrix[0][2] = 0;
    matrix[1][0] = 0;
    matrix[1][1] = cosVal;
    matrix[1][2] = sinVal;
    matrix[2][0] = 0;
    matrix[2][1] = -sinVal;
    matrix[2][2] = cosVal;
}

void createYRotationMatrix(Mat3x3* matrix, s16 angle) {
    s16 sinTheta = approximateSin(angle);
    s16 cosTheta = approximateCos(angle);

    *matrix = (Mat3x3){
        cosTheta, 0, -sinTheta,
        0, 0x2000, 0,
        sinTheta, 0, cosTheta};
}

void createZRotationMatrix(s16 matrix[3][3], s16 angle) {
    s16 new_var;
    int new_var2;
    int new_var3;
    s16 sinVal = approximateSin(angle);
    s16 cosVal;
    sinVal++;
    sinVal--;
    cosVal = approximateCos(angle);
    matrix[0][0] = cosVal;
    matrix[0][1] = sinVal;
    new_var = cosVal;
    new_var3 = 0;
    new_var2 = 2;
    matrix[0][new_var2] = 0;
    matrix[0, 1][0] = -sinVal;
    matrix[1][1] = new_var;
    matrix[1][new_var2] = 0;
    matrix[new_var2][0] = 0;
    matrix[new_var2][1] = new_var3;
    matrix[new_var2][new_var2] = 0x2000;
}

INCLUDE_ASM("asm/nonmatchings/6B140", func_8006A724_6B324);

INCLUDE_ASM("asm/nonmatchings/6B140", func_8006A838_6B438);

INCLUDE_ASM("asm/nonmatchings/6B140", func_8006A948_6B548);

INCLUDE_ASM("asm/nonmatchings/6B140", func_8006AA5C_6B65C);

INCLUDE_ASM("asm/nonmatchings/6B140", func_8006AB64_6B764);

INCLUDE_ASM("asm/nonmatchings/6B140", func_8006AD9C_6B99C);

void func_8006AFDC_6BBDC(func_8006AFDC_6BBDC_arg* arg0, s32 arg1, s32 arg2, s32 arg3) {
    arg0->unk14 = arg1;
    arg0->unk18 = arg2;
    arg0->unk1C = arg3;
}

INCLUDE_ASM("asm/nonmatchings/6B140", func_8006AFEC_6BBEC);

INCLUDE_ASM("asm/nonmatchings/6B140", func_8006B084_6BC84);

INCLUDE_ASM("asm/nonmatchings/6B140", func_8006B35C_6BF5C);

INCLUDE_ASM("asm/nonmatchings/6B140", func_8006B5A8_6C1A8);

INCLUDE_ASM("asm/nonmatchings/6B140", func_8006B7DC_6C3DC);

INCLUDE_ASM("asm/nonmatchings/6B140", func_8006BA10_6C610);

INCLUDE_ASM("asm/nonmatchings/6B140", func_8006BC6C_6C86C);

INCLUDE_ASM("asm/nonmatchings/6B140", func_8006BDBC_6C9BC);

INCLUDE_ASM("asm/nonmatchings/6B140", func_8006BE4C_6CA4C);

INCLUDE_ASM("asm/nonmatchings/6B140", func_8006BEDC_6CADC);

INCLUDE_ASM("asm/nonmatchings/6B140", func_8006BFB8_6CBB8);

INCLUDE_ASM("asm/nonmatchings/6B140", func_8006C130_6CD30);

INCLUDE_ASM("asm/nonmatchings/6B140", func_8006C2A8_6CEA8);

s32 approximate_sqrt(u32 input) {
    u32 remainder = 0;
    s32 quotient = 0;
    s32 divisor_shift = 30;
    u32 dividend = input;

    while (divisor_shift >= 0) {
        remainder <<= 1;
        quotient <<= 1;

        if (remainder < (dividend >> divisor_shift)) {
            remainder += 1;
            dividend -= (remainder << divisor_shift);
            remainder += 1;
            quotient += 1;
        }
        divisor_shift -= 2;
    }
    return quotient;
}

INCLUDE_ASM("asm/nonmatchings/6B140", func_8006C438_6D038);

INCLUDE_ASM("asm/nonmatchings/6B140", distance_2d);

INCLUDE_ASM("asm/nonmatchings/6B140", func_8006C5E0_6D1E0);

INCLUDE_ASM("asm/nonmatchings/6B140", func_8006C654_6D254);

INCLUDE_ASM("asm/nonmatchings/6B140", func_8006C8FC_6D4FC);

INCLUDE_ASM("asm/nonmatchings/6B140", func_8006CF7C_6DB7C);

s16 func_8006D21C_6DE1C(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    return func_8006CF7C_6DB7C(arg2 - arg0, arg3 - arg1);
}