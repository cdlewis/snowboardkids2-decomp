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

s32 approximateSin(s16 inputAngle) {
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

s32 approximateCos(s16 inputAngle) {
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

void createYRotationMatrix(Transform3D *matrix, u16 angle) {
    s16 sinTheta = approximateSin(angle);
    s16 cosTheta = approximateCos(angle);

    matrix->m[0][0] = cosTheta;
    matrix->m[0][1] = 0;
    matrix->m[0][2] = -sinTheta;
    matrix->m[1][0] = 0;
    matrix->m[1][1] = 0x2000;
    matrix->m[1][2] = 0;
    matrix->m[2][0] = sinTheta;
    matrix->m[2][1] = 0;
    matrix->m[2][2] = cosTheta;
}

void createZRotationMatrix(Transform3D *matrix, u16 angle) {
    s16 new_var;
    int new_var2;
    int new_var3;
    s16 sinVal = approximateSin(angle);
    s16 cosVal;
    sinVal++;
    sinVal--;
    cosVal = approximateCos(angle);
    matrix->m[0][0] = cosVal;
    matrix->m[0][1] = sinVal;
    new_var = cosVal;
    new_var3 = 0;
    new_var2 = 2;
    matrix->m[0][new_var2] = 0;
    matrix->m[1][0] = -sinVal;
    matrix->m[1][1] = new_var;
    matrix->m[1][new_var2] = 0;
    matrix->m[new_var2][0] = 0;
    matrix->m[new_var2][1] = new_var3;
    matrix->m[new_var2][new_var2] = 0x2000;
}

void createCombinedRotationMatrix(void *matrix, u16 angleX, u16 angleZ) {
    s16 *m = (s16 *)matrix;
    s32 sinX;
    s32 cosX;
    s32 sinZ;
    s32 cosZ;
    s32 temp1;
    s32 temp2;
    s32 temp3;
    s32 temp4;

    sinX = approximateSin(angleX);
    cosX = approximateCos(angleX);
    sinZ = approximateSin(angleZ);
    cosZ = approximateCos(angleZ);

    temp1 = sinX * sinZ;
    m[0] = cosZ;
    m[1] = 0;
    m[2] = -sinZ;

    if (temp1 < 0) {
        temp1 += 0x1FFF;
    }
    m[3] = temp1 >> 13;

    temp2 = sinX * cosZ;
    m[4] = cosX;

    if (temp2 < 0) {
        temp2 += 0x1FFF;
    }
    m[5] = temp2 >> 13;

    temp3 = cosX * sinZ;
    if (temp3 < 0) {
        temp3 += 0x1FFF;
    }
    m[6] = temp3 >> 13;

    m[7] = -sinX;

    temp4 = cosX * cosZ;
    if (temp4 < 0) {
        temp4 += 0x1FFF;
    }
    m[8] = temp4 >> 13;
}

void createRotationMatrixYX(Transform3D *matrix, u16 angleY, u16 angleX) {
    s32 sinX = approximateSin(angleX);
    s32 cosX = approximateCos(angleX);
    s32 sinY = approximateSin(angleY);
    s32 cosY = approximateCos(angleY);
    s32 negSinY = -sinY;
    s32 negSinX = -sinX;
    s32 temp1 = negSinY * negSinX;
    s32 temp2;
    s32 temp3;
    s32 temp4;

    matrix->m[0][0] = cosY;
    if (temp1 < 0) {
        temp1 += 0x1FFF;
    }

    temp2 = negSinY * cosX;
    matrix->m[0][1] = temp1 >> 13;
    if (temp2 < 0) {
        temp2 += 0x1FFF;
    }

    temp3 = cosY * negSinX;
    matrix->m[0][2] = temp2 >> 13;
    matrix->m[1][0] = 0;
    matrix->m[1][1] = cosX;
    matrix->m[1][2] = sinX;
    matrix->m[2][0] = sinY;
    if (temp3 < 0) {
        temp3 += 0x1FFF;
    }

    temp4 = cosY * cosX;
    matrix->m[2][1] = temp3 >> 13;
    if (temp4 < 0) {
        temp4 += 0x1FFF;
    }

    matrix->m[2][2] = temp4 >> 13;
}

void createRotationMatrixXZ(Transform3D *matrix, u16 angleX, u16 angleZ) {
    s32 sinX = approximateSin(angleX);
    s32 cosX = approximateCos(angleX);
    s32 sinZ = approximateSin(angleZ);
    s32 cosZ = approximateCos(angleZ);
    s32 temp1 = sinX * cosZ;
    s32 temp2;
    s32 temp3;
    s32 temp4;

    matrix->m[0][0] = cosX;
    if (temp1 < 0) {
        temp1 += 0x1FFF;
    }

    temp2 = sinX * sinZ;
    matrix->m[0][1] = temp1 >> 13;
    if (temp2 < 0) {
        temp2 += 0x1FFF;
    }

    temp3 = cosX * cosZ;
    matrix->m[0][2] = temp2 >> 13;
    matrix->m[1][0] = -sinX;
    if (temp3 < 0) {
        temp3 += 0x1FFF;
    }

    temp4 = cosX * sinZ;
    matrix->m[1][1] = temp3 >> 13;
    if (temp4 < 0) {
        temp4 += 0x1FFF;
    }

    matrix->m[1][2] = temp4 >> 13;
    matrix->m[2][0] = 0;
    matrix->m[2][1] = -sinZ;
    matrix->m[2][2] = cosZ;
}

/* createRotationMatrixYZ creates a YZ rotation matrix with the pattern:
 * [ cosY*cosZ,     sinY,         cosY*(-sinZ)  ]
 * [ (-sinY)*cosZ,  cosY,         sinY*sinZ     ]
 * [ cosZ,          0,            cosZ          ]
 */
void createRotationMatrixYZ(s16 *matrix, u16 angleY, u16 angleZ) {
    s32 temp0;
    s32 temp1;
    s32 temp2;
    s32 temp3;
    s32 sinY;
    s32 cosY;
    s32 sinZ;
    s32 cosZ;

    sinY = approximateSin(angleY);
    cosY = approximateCos(angleY);
    sinZ = approximateSin(angleZ);
    cosZ = approximateCos(angleZ);

    temp0 = cosY * cosZ;
    if (temp0 < 0) {
        temp0 += 0x1FFF;
    }
    matrix[0] = temp0 >> 13;
    matrix[1] = sinY;

    temp1 = cosY * -sinZ;
    if (temp1 < 0) {
        temp1 += 0x1FFF;
    }
    matrix[2] = temp1 >> 13;

    temp2 = -sinY * cosZ;
    if (temp2 < 0) {
        temp2 += 0x1FFF;
    }
    matrix[3] = temp2 >> 13;
    matrix[4] = cosY;

    temp3 = -sinY * -sinZ;
    if (temp3 < 0) {
        temp3 += 0x1FFF;
    }
    matrix[5] = temp3 >> 13;

    matrix[6] = sinZ;
    matrix[7] = 0;
    matrix[8] = cosZ;
}

INCLUDE_ASM("asm/nonmatchings/geometry", createRotationMatrixXYZ);

INCLUDE_ASM("asm/nonmatchings/geometry", createRotationMatrixZYX);

void func_8006AFDC_6BBDC(func_8006AFDC_6BBDC_arg *arg0, s32 arg1, s32 arg2, s32 arg3) {
    arg0->unk14 = arg1;
    arg0->unk18 = arg2;
    arg0->unk1C = arg3;
}

void scaleMatrix(Transform3D *matrix, s16 scaleX, s16 scaleY, s16 scaleZ) {
    s32 i;
    s32 temp1;
    s32 temp2;

    for (i = 0; i < 3; i++) {
        temp1 = matrix->m[i][0] * scaleX;
        if (temp1 < 0) {
            temp1 += 0x1FFF;
        }

        temp2 = matrix->m[i][1] * scaleY;
        matrix->m[i][0] = temp1 >> 13;

        if (temp2 < 0) {
            temp2 += 0x1FFF;
        }

        temp1 = matrix->m[i][2] * scaleZ;
        matrix->m[i][1] = temp2 >> 13;

        if (temp1 < 0) {
            temp1 += 0x1FFF;
        }
        matrix->m[i][2] = temp1 >> 13;
    }
}

INCLUDE_ASM("asm/nonmatchings/geometry", func_8006B084_6BC84);

INCLUDE_ASM("asm/nonmatchings/geometry", transformVector);

INCLUDE_ASM("asm/nonmatchings/geometry", transformVector2);

INCLUDE_ASM("asm/nonmatchings/geometry", transformVector3);

INCLUDE_ASM("asm/nonmatchings/geometry", transformVectorRelative);

INCLUDE_ASM("asm/nonmatchings/geometry", rotateVectorY);

void func_8006BDBC_6C9BC(func_8005E800_5F400_arg *arg0, void *arg1, void *arg2) {
    Transform3D *mat1 = arg1;
    Transform3D *mat2 = arg2;
    s32 row;
    s16 *rowPtr;
    s32 i;
    s32 sum;
    s32 j;

    for (i = 0; i < 3; i++) {
        row = i;
        rowPtr = arg0->values + (row * 3);
        for (j = 0; j < 3; j++) {
            sum = rowPtr[0] * mat1->m[0][j] + arg0->values[row * 3 + 1] * mat1->m[1][j] +
                  arg0->values[row * 3 + 2] * mat1->m[2][j];
            if (sum < 0) {
                sum += 0x1FFF;
            }
            mat2->m[i][j] = sum >> 13;
        }
    }
}

void func_8006BE4C_6CA4C(Transform3D *arg0, Transform3D *arg1, Transform3D *arg2) {
    s32 row;
    s16 *rowPtr;
    s32 i;
    s32 sum;
    s32 j;

    for (i = 0; i < 3; i++) {
        row = i;
        rowPtr = arg0->m[row];
        for (j = 0; j < 3; j++) {
            sum = rowPtr[0] * arg1->m[j][0] + arg0->m[row][1] * arg1->m[j][1] + arg0->m[row][2] * arg1->m[j][2];
            if (sum < 0) {
                sum += 0x1FFF;
            }
            arg2->m[i][j] = sum >> 13;
        }
    }
}

extern s32 D_8009A8A4_9B4A4;
extern s32 D_8009A8A8_9B4A8;
extern s32 D_8009A8AC_9B4AC;

typedef void (*CreateXRotS16)(s16 matrix[3][3], s16 angle);

void func_8006BEDC_6CADC(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    Transform3D sp10;
    Transform3D sp30;
    Transform3D sp50;
    s32 *ptr;

    createYRotationMatrix(&sp10, (u16)arg6);
    sp10.translation.x = arg1;
    sp10.translation.y = arg2;
    sp10.translation.z = arg3;
    ((CreateXRotS16)createXRotationMatrix)(sp30.m, (s16)arg5);
    sp30.translation.z = 0;
    sp30.translation.y = 0;
    sp30.translation.x = 0;
    func_8006B084_6BC84(&sp30, &sp10, &sp50);
    ptr = &D_8009A8A4_9B4A4;
    *ptr = 0;
    D_8009A8A8_9B4A8 = 0;
    D_8009A8AC_9B4AC = arg4;
    func_8006B084_6BC84(ptr - 5, &sp50, arg0);
}

void func_8006BFB8_6CBB8(void *srcPtr, void *dstPtr) {
    Transform3D *src = (Transform3D *)srcPtr;
    s32 *mtxWords = (s32 *)dstPtr;

    mtxWords[0] = ((src->m[0][0] * 8) & 0xFFFF0000) + (((s32)((u16)src->m[0][1] << 16) >> 29) & 0xFFFF);
    mtxWords[1] = (src->m[0][2] * 8) & 0xFFFF0000;
    mtxWords[2] = ((src->m[1][0] * 8) & 0xFFFF0000) + (((s32)((u16)src->m[1][1] << 16) >> 29) & 0xFFFF);
    mtxWords[3] = (src->m[1][2] * 8) & 0xFFFF0000;
    mtxWords[4] = ((src->m[2][0] * 8) & 0xFFFF0000) + (((s32)((u16)src->m[2][1] << 16) >> 29) & 0xFFFF);
    mtxWords[5] = (src->m[2][2] * 8) & 0xFFFF0000;
    mtxWords[6] = (src->translation.x & 0xFFFF0000) + ((u16 *)&src->translation.y)[0];
    mtxWords[7] = (src->translation.z & 0xFFFF0000) + 1;
    mtxWords[8] = ((src->m[0][0] << 19) & 0xFFFF0000) + (((s16)src->m[0][1] * 8) & 0xFFFF);
    mtxWords[9] = (src->m[0][2] << 19) & 0xFFFF0000;
    mtxWords[10] = ((src->m[1][0] << 19) & 0xFFFF0000) + (((s16)src->m[1][1] * 8) & 0xFFFF);
    mtxWords[11] = (src->m[1][2] << 19) & 0xFFFF0000;
    mtxWords[12] = ((src->m[2][0] << 19) & 0xFFFF0000) + (((s16)src->m[2][1] * 8) & 0xFFFF);
    mtxWords[13] = (src->m[2][2] << 19) & 0xFFFF0000;
    mtxWords[14] = (src->translation.x << 16) + ((u16 *)&src->translation.y)[1];
    mtxWords[15] = src->translation.z << 16;
}

void func_8006C130_6CD30(Transform3D *arg0, Mtx *arg1) {
    s32 *dst;
    s16 temp16;

    dst = (s32 *)arg1;

    dst[0] = (((s32)arg0->m[0][0] << 1) & 0xFFFF0000) + ((-(s32)((u16)arg0->m[0][1] >> 15)) & 0xFFFF);
    dst[1] = ((s32)arg0->m[0][2] << 1) & 0xFFFF0000;
    dst[2] = (((s32)arg0->m[1][0] << 1) & 0xFFFF0000) + ((-(s32)((u16)arg0->m[1][1] >> 15)) & 0xFFFF);
    dst[3] = ((s32)arg0->m[1][2] << 1) & 0xFFFF0000;
    dst[4] = (((s32)arg0->m[2][0] << 1) & 0xFFFF0000) + ((-(s32)((u16)arg0->m[2][1] >> 15)) & 0xFFFF);
    dst[5] = ((s32)arg0->m[2][2] << 1) & 0xFFFF0000;
    dst[6] = (arg0->translation.x & 0xFFFF0000) + ((u16 *)&arg0->translation.y)[0];
    dst[7] = (arg0->translation.z & 0xFFFF0000) + 1;
    temp16 = arg0->m[0][1];
    dst[8] = (((s32)arg0->m[0][0] << 17) & 0xFFFF0000) + ((((s32)temp16 << 1) & 0xFFFF));
    dst[9] = ((s32)arg0->m[0][2] << 17) & 0xFFFF0000;
    temp16 = arg0->m[1][1];
    dst[10] = (((s32)arg0->m[1][0] << 17) & 0xFFFF0000) + ((((s32)temp16 << 1) & 0xFFFF));
    dst[11] = ((s32)arg0->m[1][2] << 17) & 0xFFFF0000;
    temp16 = arg0->m[2][1];
    dst[12] = (((s32)arg0->m[2][0] << 17) & 0xFFFF0000) + ((((s32)temp16 << 1) & 0xFFFF));
    dst[13] = ((s32)arg0->m[2][2] << 17) & 0xFFFF0000;
    dst[14] = (arg0->translation.x << 16) + ((u16 *)&arg0->translation.y)[1];
    dst[15] = arg0->translation.z << 16;
}

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

s32 distance_2d(s32 x, s32 y) {
    s64 x2 = (s64)x * x;
    s64 y2 = (s64)y * y;

    return isqrt64(x2 + y2);
}

s32 distance_3d(s32 x, s32 y, s32 z) {
    return isqrt64((s64)x * x + (s64)y * y + (s64)z * z);
}

INCLUDE_ASM("asm/nonmatchings/geometry", computeLookAtMatrix);

INCLUDE_ASM("asm/nonmatchings/geometry", matrixToEulerAngles);

INCLUDE_ASM("asm/nonmatchings/geometry", atan2Fixed);

s16 func_8006D21C_6DE1C(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    return atan2Fixed(arg2 - arg0, arg3 - arg1);
}