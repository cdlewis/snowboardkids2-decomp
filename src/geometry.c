#include "geometry.h"

#include "common.h"

typedef struct {
    s16 m00_int;
    u16 m00_frac;
    s16 m10_int;
    s16 m20_int;
    u16 m20_frac;
    s16 m01_int;
    s16 m11_int;
    u16 m11_frac;
    s16 m21_int;
    s32 posX;
    u16 posY_frac;
    u16 posZ_frac;
    s32 scale;
    s32 _pad0;
    s32 _pad1;
    s32 _pad2;
    s32 _pad3;
    s32 _pad4;
    s32 _pad5;
    s32 _pad6;
    s32 _pad7;
} PackedBoneTransform;

typedef struct {
    s32 m00_int;
    s32 m10_int;
    s32 m20_int;
    s32 m01_int;
    s32 m11_int;
    s32 m21_int;
    s32 posX_int;
    s32 scale_int;
    s32 m00_frac;
    s32 m10_frac;
    s32 m20_frac;
    s32 m01_frac;
    s32 m11_frac;
    s32 m21_frac;
    s32 posX_frac;
    s32 scale_frac;
} N64MatrixParts;

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

void setBonePosition(BoneAnimationState *arg0, s32 arg1, s32 arg2, s32 arg3) {
    arg0->position[0] = arg1;
    arg0->position[1] = arg2;
    arg0->position[2] = arg3;
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

void func_8006BDBC_6C9BC(BoneAnimationState *arg0, void *arg1, void *arg2) {
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

// Multiplies a 3x3 rotation matrix by the transpose of another 3x3 rotation matrix.
// Result = arg0 * transpose(arg1), where * is matrix multiplication.
void func_mulMatrix3x3T(Transform3D *arg0, Transform3D *arg1, Transform3D *arg2) {
    Transform3D *mat1 = arg1;
    Transform3D *mat2 = arg2;
    s32 row;
    s16 *rowPtr;
    s32 i;
    s32 sum;
    s32 j;

    for (i = 0; i < 3; i++) {
        row = i;
        rowPtr = arg0->m[row];
        for (j = 0; j < 3; j++) {
            sum = rowPtr[0] * mat1->m[j][0] + arg0->m[row][1] * mat1->m[j][1] + arg0->m[row][2] * mat1->m[j][2];
            if (sum < 0) {
                sum += 0x1FFF;
            }
            mat2->m[i][j] = sum >> 13;
        }
    }
}

extern s32 D_8009A8A4_9B4A4;
extern s32 D_8009A8A8_9B4A8;
extern s32 D_8009A8AC_9B4AC;

typedef void (*CreateXRotS16)(s16 matrix[3][3], s16 angle);

void func_8006BEDC_6CADC(void *output, s32 posX, s32 posY, s32 posZ, s32 tempPosZ, s32 pitchAngle, s32 yawAngle) {
    Transform3D yRotation;
    Transform3D xRotation;
    Transform3D combined;
    s32 *tempTransformPtr;

    createYRotationMatrix(&yRotation, (u16)yawAngle);
    yRotation.translation.x = posX;
    yRotation.translation.y = posY;
    yRotation.translation.z = posZ;
    ((CreateXRotS16)createXRotationMatrix)(xRotation.m, (s16)pitchAngle);
    xRotation.translation.z = 0;
    xRotation.translation.y = 0;
    xRotation.translation.x = 0;
    func_8006B084_6BC84(&xRotation, &yRotation, &combined);
    tempTransformPtr = &D_8009A8A4_9B4A4;
    *tempTransformPtr = 0;
    D_8009A8A8_9B4A8 = 0;
    D_8009A8AC_9B4AC = tempPosZ;
    func_8006B084_6BC84(tempTransformPtr - 5, &combined, output);
}

/**
 * Converts a Transform3D (3x3 rotation matrix + translation) to an N64 RSP Mtx.
 *
 * Similar to transform3DToN64Mtx but uses different scaling for rotation values.
 * This function scales rotation values by 8 instead of 2.
 *
 * Matrix layout (each word packs two 16-bit values):
 *   dst[0-5]:   Integer parts of 3x3 rotation matrix (rows 0-2)
 *   dst[6-7]:   Integer parts of translation vector + w=1
 *   dst[8-13]:  Fractional parts of 3x3 rotation matrix
 *   dst[14-15]: Fractional parts of translation vector
 */
void transform3DToMtx(void *srcPtr, void *dstPtr) {
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
    mtxWords[8] = ((src->m[0][0] << 19) & 0xFFFF0000) + ((src->m[0][1] * 8) & 0xFFFF);
    mtxWords[9] = (src->m[0][2] << 19) & 0xFFFF0000;
    mtxWords[10] = ((src->m[1][0] << 19) & 0xFFFF0000) + ((src->m[1][1] * 8) & 0xFFFF);
    mtxWords[11] = (src->m[1][2] << 19) & 0xFFFF0000;
    mtxWords[12] = ((src->m[2][0] << 19) & 0xFFFF0000) + ((src->m[2][1] * 8) & 0xFFFF);
    mtxWords[13] = (src->m[2][2] << 19) & 0xFFFF0000;
    mtxWords[14] = (src->translation.x << 16) + ((u16 *)&src->translation.y)[1];
    mtxWords[15] = src->translation.z << 16;
}

/**
 * Converts a Transform3D (3x3 rotation matrix + translation) to an N64 RSP Mtx.
 *
 * The Transform3D uses s2.13 fixed-point for rotation values.
 * The Mtx uses s15.16 fixed-point split into integer and fractional halves.
 *
 * Similar to transform3DToMtx but uses different scaling (2x instead of 8x)
 * for the rotation matrix conversion.
 *
 * Matrix layout (each word packs two 16-bit values):
 *   mtx[0-5]:   Integer parts of 3x3 rotation matrix (rows 0-2)
 *   mtx[6-7]:   Integer parts of translation vector + w=1
 *   mtx[8-13]:  Fractional parts of 3x3 rotation matrix
 *   mtx[14-15]: Fractional parts of translation vector
 */
void transform3DToN64Mtx(Transform3D *transform, Mtx *mtx) {
    s32 *mtxWords;
    s16 m01;

    mtxWords = (s32 *)mtx;

    /* Row 0: m[0][0] and m[0][1] integer parts */
    mtxWords[0] = (((s32)transform->m[0][0] << 1) & 0xFFFF0000) + ((-(s32)((u16)transform->m[0][1] >> 15)) & 0xFFFF);
    mtxWords[1] = ((s32)transform->m[0][2] << 1) & 0xFFFF0000;

    /* Row 1: m[1][0] and m[1][1] integer parts */
    mtxWords[2] = (((s32)transform->m[1][0] << 1) & 0xFFFF0000) + ((-(s32)((u16)transform->m[1][1] >> 15)) & 0xFFFF);
    mtxWords[3] = ((s32)transform->m[1][2] << 1) & 0xFFFF0000;

    /* Row 2: m[2][0] and m[2][1] integer parts */
    mtxWords[4] = (((s32)transform->m[2][0] << 1) & 0xFFFF0000) + ((-(s32)((u16)transform->m[2][1] >> 15)) & 0xFFFF);
    mtxWords[5] = ((s32)transform->m[2][2] << 1) & 0xFFFF0000;

    /* Row 3: translation integer parts (x high, y high, z high, w=1) */
    mtxWords[6] = (transform->translation.x & 0xFFFF0000) + ((u16 *)&transform->translation.y)[0];
    mtxWords[7] = (transform->translation.z & 0xFFFF0000) + 1;

    /* Row 0: m[0][0] and m[0][1] fractional parts */
    m01 = transform->m[0][1];
    mtxWords[8] = (((s32)transform->m[0][0] << 17) & 0xFFFF0000) + ((((s32)m01 << 1) & 0xFFFF));
    mtxWords[9] = ((s32)transform->m[0][2] << 17) & 0xFFFF0000;

    /* Row 1: m[1][0] and m[1][1] fractional parts */
    m01 = transform->m[1][1];
    mtxWords[10] = (((s32)transform->m[1][0] << 17) & 0xFFFF0000) + ((((s32)m01 << 1) & 0xFFFF));
    mtxWords[11] = ((s32)transform->m[1][2] << 17) & 0xFFFF0000;

    /* Row 2: m[2][0] and m[2][1] fractional parts */
    m01 = transform->m[2][1];
    mtxWords[12] = (((s32)transform->m[2][0] << 17) & 0xFFFF0000) + ((((s32)m01 << 1) & 0xFFFF));
    mtxWords[13] = ((s32)transform->m[2][2] << 17) & 0xFFFF0000;

    /* Row 3: translation fractional parts (x low, y low, z low, w=0) */
    mtxWords[14] = (transform->translation.x << 16) + ((u16 *)&transform->translation.y)[1];
    mtxWords[15] = transform->translation.z << 16;
}

void convertPackedBoneTransformToN64Matrix(PackedBoneTransform *src, N64MatrixParts *dst) {
    dst->m00_int = (s32)((src->m00_int & 0xFFFF0000) + (((s32)(src->m00_frac << 0x10) >> 0x1F) & 0xFFFF));
    dst->m10_int = (s32)(src->m10_int & 0xFFFF0000);
    dst->m20_int = (s32)((src->m20_int & 0xFFFF0000) + (((s32)(src->m20_frac << 0x10) >> 0x1F) & 0xFFFF));
    dst->m01_int = (s32)(src->m01_int & 0xFFFF0000);
    dst->m11_int = (s32)((src->m11_int & 0xFFFF0000) + (((s32)(src->m11_frac << 0x10) >> 0x1F) & 0xFFFF));
    dst->m21_int = (s32)(src->m21_int & 0xFFFF0000);
    dst->posX_int = (s32)((src->posX & 0xFFFF0000) + src->posY_frac);
    dst->scale_int = (s32)((src->scale & 0xFFFF0000) + 1);
    dst->m00_frac = (s32)(((src->m00_int << 0x10) & 0xFFFF0000) + src->m00_frac);
    dst->m10_frac = (s32)((src->m10_int << 0x10) & 0xFFFF0000);
    dst->m20_frac = (s32)(((src->m20_int << 0x10) & 0xFFFF0000) + src->m20_frac);
    dst->m01_frac = (s32)((src->m01_int << 0x10) & 0xFFFF0000);
    dst->m11_frac = (s32)(((src->m11_int << 0x10) & 0xFFFF0000) + src->m11_frac);
    dst->m21_frac = (s32)((src->m21_int << 0x10) & 0xFFFF0000);
    dst->posX_frac = (s32)((src->posX << 0x10) + src->posZ_frac);
    dst->scale_frac = (s32)(src->scale << 0x10);
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

s16 computeAngleToPosition(s32 fromX, s32 fromZ, s32 toX, s32 toZ) {
    return atan2Fixed(toX - fromX, toZ - fromZ);
}