#include "geometry.h"

#include "common.h"

typedef struct {
    u8 padding[0x14];
    s32 unk14;
    s32 unk18;
    s32 unk1C;
} func_8006AFDC_6BBDC_arg;

typedef struct {
    s16 unk0;
    u16 unk2;
    s16 unk4;
    s16 unk6;
    u16 unk8;
    s16 unkA;
    s16 unkC;
    u16 unkE;
    s16 unk10;
    s32 unk14;
    u16 unk18;
    u16 unk1A;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
} func_8006C2A8_6CEA8_arg0;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
} func_8006C2A8_6CEA8_arg1;

s32 atan2Fixed(s32, s32);

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

void createXRotationMatrix(s16 matrix[3][3], u16 angle) {
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

void createYRotationMatrix(s16 matrix[3][3], u16 angle) {
    s16 sinTheta = approximateSin(angle);
    s16 cosTheta = approximateCos(angle);

    matrix[0][0] = cosTheta;
    matrix[0][1] = 0;
    matrix[0][2] = -sinTheta;
    matrix[1][0] = 0;
    matrix[1][1] = 0x2000;
    matrix[1][2] = 0;
    matrix[2][0] = sinTheta;
    matrix[2][1] = 0;
    matrix[2][2] = cosTheta;
}

void createZRotationMatrix(s16 matrix[3][3], u16 angle) {
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

INCLUDE_ASM("asm/nonmatchings/geometry", createCombinedRotationMatrix);

INCLUDE_ASM("asm/nonmatchings/geometry", createRotationMatrixYX);

INCLUDE_ASM("asm/nonmatchings/geometry", createRotationMatrixXZ);

INCLUDE_ASM("asm/nonmatchings/geometry", createRotationMatrixYZ);

INCLUDE_ASM("asm/nonmatchings/geometry", createRotationMatrixXYZ);

INCLUDE_ASM("asm/nonmatchings/geometry", createRotationMatrixZYX);

void func_8006AFDC_6BBDC(func_8006AFDC_6BBDC_arg *arg0, s32 arg1, s32 arg2, s32 arg3) {
    arg0->unk14 = arg1;
    arg0->unk18 = arg2;
    arg0->unk1C = arg3;
}

INCLUDE_ASM("asm/nonmatchings/geometry", scaleMatrix);

INCLUDE_ASM("asm/nonmatchings/geometry", func_8006B084_6BC84);

INCLUDE_ASM("asm/nonmatchings/geometry", transformVector);

INCLUDE_ASM("asm/nonmatchings/geometry", transformVector2);

INCLUDE_ASM("asm/nonmatchings/geometry", transformVector3);

INCLUDE_ASM("asm/nonmatchings/geometry", transformVectorRelative);

INCLUDE_ASM("asm/nonmatchings/geometry", rotateVectorY);

INCLUDE_ASM("asm/nonmatchings/geometry", func_8006BDBC_6C9BC);

INCLUDE_ASM("asm/nonmatchings/geometry", func_8006BE4C_6CA4C);

INCLUDE_ASM("asm/nonmatchings/geometry", func_8006BEDC_6CADC);

INCLUDE_ASM("asm/nonmatchings/geometry", func_8006BFB8_6CBB8);

INCLUDE_ASM("asm/nonmatchings/geometry", func_8006C130_6CD30);

void func_8006C2A8_6CEA8(func_8006C2A8_6CEA8_arg0 *arg0, func_8006C2A8_6CEA8_arg1 *arg1) {
    arg1->unk0 = (s32)((arg0->unk0 & 0xFFFF0000) + (((s32)(arg0->unk2 << 0x10) >> 0x1F) & 0xFFFF));
    arg1->unk4 = (s32)(arg0->unk4 & 0xFFFF0000);
    arg1->unk8 = (s32)((arg0->unk6 & 0xFFFF0000) + (((s32)(arg0->unk8 << 0x10) >> 0x1F) & 0xFFFF));
    arg1->unkC = (s32)(arg0->unkA & 0xFFFF0000);
    arg1->unk10 = (s32)((arg0->unkC & 0xFFFF0000) + (((s32)(arg0->unkE << 0x10) >> 0x1F) & 0xFFFF));
    arg1->unk14 = (s32)(arg0->unk10 & 0xFFFF0000);
    arg1->unk18 = (s32)((arg0->unk14 & 0xFFFF0000) + arg0->unk18);
    arg1->unk1C = (s32)((arg0->unk1C & 0xFFFF0000) + 1);
    arg1->unk20 = (s32)(((arg0->unk0 << 0x10) & 0xFFFF0000) + arg0->unk2);
    arg1->unk24 = (s32)((arg0->unk4 << 0x10) & 0xFFFF0000);
    arg1->unk28 = (s32)(((arg0->unk6 << 0x10) & 0xFFFF0000) + arg0->unk8);
    arg1->unk2C = (s32)((arg0->unkA << 0x10) & 0xFFFF0000);
    arg1->unk30 = (s32)(((arg0->unkC << 0x10) & 0xFFFF0000) + arg0->unkE);
    arg1->unk34 = (s32)((arg0->unk10 << 0x10) & 0xFFFF0000);
    arg1->unk38 = (s32)((arg0->unk14 << 0x10) + arg0->unk1A);
    arg1->unk3C = (s32)(arg0->unk1C << 0x10);
}

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

INCLUDE_ASM("asm/nonmatchings/geometry", isqrt64);

INCLUDE_ASM("asm/nonmatchings/geometry", distance_2d);

INCLUDE_ASM("asm/nonmatchings/geometry", distance_3d);

INCLUDE_ASM("asm/nonmatchings/geometry", computeLookAtMatrix);

INCLUDE_ASM("asm/nonmatchings/geometry", matrixToEulerAngles);

INCLUDE_ASM("asm/nonmatchings/geometry", atan2Fixed);

s16 func_8006D21C_6DE1C(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    return atan2Fixed(arg2 - arg0, arg3 - arg1);
}