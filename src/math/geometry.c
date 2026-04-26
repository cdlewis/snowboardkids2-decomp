#include "math/geometry.h"

#include "common.h"
#include "gbi.h"
#include "gu.h"

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

// Expected memory layout for arg1 of transformVectorRelative:
// - 3x3 s16 matrix at offset 0 (18 bytes)
// - 2 bytes padding at offset 18
// - Vec3i position at offset 20 (0x14)
typedef struct {
    s16 m[3][3];
    s16 _pad;
    Vec3i position;
} Matrix3x3AndPosition;

typedef void (*CreateXRotS16)(s16 matrix[3][3], s16 angle);

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

void createRotationMatrixXYZ(s16 *m, u16 angleX, u16 angleY, u16 angleZ) {
    s32 sinX;
    s32 cosX;
    s32 sinY;
    s32 cosY;
    s32 sinZ;
    s32 cosZ;
    s32 temp1;
    s32 temp2;
    s32 temp3;
    s32 temp4;
    s32 sinXsinY;
    s32 cosXsinY;
    s32 negSinZ;
    s32 negSinX;

    sinX = approximateSin(angleX);
    cosX = approximateCos(angleX);
    sinY = approximateSin(angleY);
    cosY = approximateCos(angleY);
    sinZ = approximateSin(angleZ);
    cosZ = approximateCos(angleZ);

    temp1 = cosY * cosZ;
    if (temp1 < 0) {
        temp1 += 0x1FFF;
    }

    temp2 = cosY * sinZ;
    m[0] = temp1 >> 13;
    if (temp2 < 0) {
        temp2 += 0x1FFF;
    }

    temp3 = sinX * sinY;
    m[1] = temp2 >> 13;
    m[2] = -sinY;

    temp4 = temp3;
    if (temp3 < 0) {
        temp4 = temp3 + 0x1FFF;
    }
    sinXsinY = temp4 >> 13;
    __asm__("" : : "r"(temp3));

    negSinZ = -sinZ;

    temp1 = sinXsinY * cosZ + cosX * negSinZ;
    if (temp1 < 0) {
        temp1 += 0x1FFF;
    }
    m[3] = temp1 >> 13;

    __asm__ __volatile__("" ::: "memory");

    temp1 = sinXsinY * sinZ + cosX * cosZ;
    if (temp1 < 0) {
        temp1 += 0x1FFF;
    }

    temp2 = sinX * cosY;
    m[4] = temp1 >> 13;
    if (temp2 < 0) {
        temp2 += 0x1FFF;
    }

    temp3 = cosX * sinY;
    m[5] = temp2 >> 13;

    temp4 = temp3;
    if (temp3 < 0) {
        temp4 = temp3 + 0x1FFF;
    }
    cosXsinY = temp4 >> 13;
    __asm__("" : : "r"(temp3));
    negSinX = -sinX;

    temp1 = cosXsinY * cosZ + negSinX * negSinZ;
    if (temp1 < 0) {
        temp1 += 0x1FFF;
    }
    m[6] = temp1 >> 13;

    __asm__ __volatile__("" ::: "memory");

    temp1 = cosXsinY * sinZ + negSinX * cosZ;
    if (temp1 < 0) {
        temp1 += 0x1FFF;
    }

    temp2 = cosX * cosY;
    m[7] = temp1 >> 13;
    if (temp2 < 0) {
        temp2 += 0x1FFF;
    }

    m[8] = temp2 >> 13;
}

void createRotationMatrixZYX(s16 *matrix, s16 angleX, s16 angleZ, s16 angleY) {
    s32 sinZ, cosZ;
    s32 sinY, cosY;
    s32 sinX, cosX;

    sinZ = approximateSin(angleZ);
    cosZ = approximateCos(angleZ);
    sinY = approximateSin(angleY);
    cosY = approximateCos(angleY);
    sinX = approximateSin(angleX);
    cosX = approximateCos(angleX);

    matrix[0] = (cosX * cosY + ((sinX * sinZ) / 0x2000) * sinY) / 0x2000;
    matrix[1] = (sinX * cosZ) / 0x2000;
    matrix[2] = (cosX * -sinY + ((sinX * sinZ) / 0x2000) * cosY) / 0x2000;
    matrix[3] = (-sinX * cosY + ((cosX * sinZ) / 0x2000) * sinY) / 0x2000;
    matrix[4] = (cosX * cosZ) / 0x2000;
    matrix[5] = (-sinX * -sinY + ((cosX * sinZ) / 0x2000) * cosY) / 0x2000;
    matrix[6] = (cosZ * sinY) / 0x2000;
    matrix[7] = -sinZ;
    matrix[8] = (cosZ * cosY) / 0x2000;
}

void setBonePosition(BoneAnimationState *state, s32 x, s32 y, s32 z) {
    state->position[0] = x;
    state->position[1] = y;
    state->position[2] = z;
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

/**
 * Multiplies two Transform3D structures: result = left * right
 *
 * This function combines two 3D transforms by:
 * 1. Multiplying the 3x3 rotation matrices (fixed-point s2.13 format)
 * 2. Transforming the left translation by the right rotation matrix
 * 3. Adding the right translation to the result
 *
 * Used for composing hierarchical transforms in skeletal animation,
 * camera systems, and object positioning.
 */
void func_8006B084_6BC84(Transform3D *arg0, Transform3D *arg1, Transform3D *arg2) {
    s32 i, j;
    s32 sum;
    s16 *dest;
    s32 frac0;
    s32 int0;
    s32 frac1;
    s32 int1;
    s32 frac2;
    s32 int2;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            dest = &arg2->m[i][j];
            sum = arg0->m[i][0] * arg1->m[0][j] + arg0->m[i][1] * arg1->m[1][j] + arg0->m[i][2] * arg1->m[2][j];
            if (sum < 0) {
                sum += 0x1FFF;
            }
            *dest = sum >> 13;
        }
    }

    frac0 = (arg0->translation.x & 0xFFFF) * arg1->m[0][0];
    int0 = (arg0->translation.x >> 16) * (arg1->m[0][0] << 3);
    if (frac0 < 0) {
        frac0 += 0x1FFF;
    }

    frac1 = (arg0->translation.y & 0xFFFF) * arg1->m[1][0];
    int1 = (arg0->translation.y >> 16) * (arg1->m[1][0] << 3);
    frac0 = int0 + (frac0 >> 13) + int1;
    if (frac1 < 0) {
        frac1 += 0x1FFF;
    }

    frac2 = (arg0->translation.z & 0xFFFF) * arg1->m[2][0];
    int2 = (arg0->translation.z >> 16) * (arg1->m[2][0] << 3);
    int1 = frac0 + (frac1 >> 13) + int2;
    if (frac2 < 0) {
        frac2 += 0x1FFF;
    }

    arg2->translation.x = int1 + (frac2 >> 13) + arg1->translation.x;
    frac0 = (arg0->translation.x & 0xFFFF) * arg1->m[0][1];
    int0 = (arg0->translation.x >> 16) * (arg1->m[0][1] << 3);
    if (frac0 < 0) {
        frac0 += 0x1FFF;
    }

    frac1 = (arg0->translation.y & 0xFFFF) * arg1->m[1][1];
    int1 = (arg0->translation.y >> 16) * (arg1->m[1][1] << 3);
    frac0 = int0 + (frac0 >> 13) + int1;
    if (frac1 < 0) {
        frac1 += 0x1FFF;
    }

    frac2 = (arg0->translation.z & 0xFFFF) * arg1->m[2][1];
    int2 = (arg0->translation.z >> 16) * (arg1->m[2][1] << 3);
    int1 = frac0 + (frac1 >> 13) + int2;
    if (frac2 < 0) {
        frac2 += 0x1FFF;
    }

    arg2->translation.y = int1 + (frac2 >> 13) + arg1->translation.y;

    frac0 = (arg0->translation.x & 0xFFFF) * arg1->m[0][2];
    int0 = (arg0->translation.x >> 16) * (arg1->m[0][2] << 3);
    if (frac0 < 0) {
        frac0 += 0x1FFF;
    }

    frac1 = (arg0->translation.y & 0xFFFF) * arg1->m[1][2];
    int1 = (arg0->translation.y >> 16) * (arg1->m[1][2] << 3);
    frac0 = int0 + (frac0 >> 13) + int1;
    if (frac1 < 0) {
        frac1 += 0x1FFF;
    }

    frac2 = (arg0->translation.z & 0xFFFF) * arg1->m[2][2];
    int2 = (arg0->translation.z >> 16) * (arg1->m[2][2] << 3);
    int1 = frac0 + (frac1 >> 13) + int2;
    if (frac2 < 0) {
        frac2 += 0x1FFF;
    }

    arg2->translation.z = int1 + (frac2 >> 13) + arg1->translation.z;
}

void transformVector(s16 *inputVec, s16 *transformPtr, void *outputPtr) {
    Vec3i *input = (Vec3i *)inputVec;
    Transform3D *transform = (Transform3D *)transformPtr;
    Vec3i *output = (Vec3i *)outputPtr;
    s32 frac0;
    s32 int0;
    s32 frac1;
    s32 int1;
    s32 frac2;
    s32 int2;

    __asm__("" : "=r"(input) : "0"(input));
    __asm__("" : "=r"(transform) : "0"(transform));

    frac0 = (input->x & 0xFFFF) * transform->m[0][0];
    int0 = (input->x >> 16) * (transform->m[0][0] << 3);
    if (frac0 < 0) {
        frac0 += 0x1FFF;
    }
    frac1 = (input->y & 0xFFFF) * transform->m[1][0];
    int1 = (input->y >> 16) * (transform->m[1][0] << 3);
    frac0 = (int0 + (frac0 >> 13)) + int1;
    if (frac1 < 0) {
        frac1 += 0x1FFF;
    }
    frac2 = (input->z & 0xFFFF) * transform->m[2][0];
    int2 = (input->z >> 16) * (transform->m[2][0] << 3);
    int1 = (frac0 + (frac1 >> 13)) + int2;
    if (frac2 < 0) {
        frac2 += 0x1FFF;
    }
    output->x = int1 + (frac2 >> 13) + transform->translation.x;

    frac0 = (input->x & 0xFFFF) * transform->m[0][1];
    int0 = (input->x >> 16) * (transform->m[0][1] << 3);
    if (frac0 < 0) {
        frac0 += 0x1FFF;
    }
    frac1 = (input->y & 0xFFFF) * transform->m[1][1];
    int1 = (input->y >> 16) * (transform->m[1][1] << 3);
    frac0 = (int0 + (frac0 >> 13)) + int1;
    if (frac1 < 0) {
        frac1 += 0x1FFF;
    }
    frac2 = (input->z & 0xFFFF) * transform->m[2][1];
    int2 = (input->z >> 16) * (transform->m[2][1] << 3);
    int1 = (frac0 + (frac1 >> 13)) + int2;
    if (frac2 < 0) {
        frac2 += 0x1FFF;
    }
    output->y = int1 + (frac2 >> 13) + transform->translation.y;

    frac0 = (input->x & 0xFFFF) * transform->m[0][2];
    int0 = (input->x >> 16) * (transform->m[0][2] << 3);
    if (frac0 < 0) {
        frac0 += 0x1FFF;
    }
    frac1 = (input->y & 0xFFFF) * transform->m[1][2];
    int1 = (input->y >> 16) * (transform->m[1][2] << 3);
    frac0 = (int0 + (frac0 >> 13)) + int1;
    if (frac1 < 0) {
        frac1 += 0x1FFF;
    }
    frac2 = (input->z & 0xFFFF) * transform->m[2][2];
    int2 = (input->z >> 16) * (transform->m[2][2] << 3);
    int1 = (frac0 + (frac1 >> 13)) + int2;
    if (frac2 < 0) {
        frac2 += 0x1FFF;
    }
    output->z = int1 + (frac2 >> 13) + transform->translation.z;
}

// Transform a vector by a 3x3 matrix (without translation)
// input: Vec3i input vector (passed as s32* for fixed-point access)
// matrix: 3x3 s16 rotation matrix (9 elements)
// output: Vec3i result vector
void transformVector2(void *input, void *matrix, Vec3i *output) {
    s32 *vec;
    s16 *mat;
    s32 frac0;
    s32 int0;
    s32 frac1;
    s32 int1;
    s32 frac2;
    s32 int2;

    vec = input;
    mat = matrix;

    __asm__("" : "=r"(vec) : "0"(vec));
    __asm__("" : "=r"(mat) : "0"(mat));

    // Transform X component
    frac0 = (vec[0] & 0xFFFF) * mat[0];
    int0 = (vec[0] >> 16) * (mat[0] << 3);
    if (frac0 < 0) {
        frac0 += 0x1FFF;
    }
    frac1 = (vec[1] & 0xFFFF) * mat[3];
    int1 = (vec[1] >> 16) * (mat[3] << 3);
    frac0 = (int0 + (frac0 >> 13)) + int1;
    if (frac1 < 0) {
        frac1 += 0x1FFF;
    }
    frac2 = (vec[2] & 0xFFFF) * mat[6];
    int2 = (vec[2] >> 16) * (mat[6] << 3);
    int1 = (frac0 + (frac1 >> 13)) + int2;
    if (frac2 < 0) {
        frac2 += 0x1FFF;
    }
    output->x = int1 + (frac2 >> 13);

    // Transform Y component
    frac0 = (vec[0] & 0xFFFF) * mat[1];
    int0 = (vec[0] >> 16) * (mat[1] << 3);
    if (frac0 < 0) {
        frac0 += 0x1FFF;
    }
    frac1 = (vec[1] & 0xFFFF) * mat[4];
    int1 = (vec[1] >> 16) * (mat[4] << 3);
    frac0 = (int0 + (frac0 >> 13)) + int1;
    if (frac1 < 0) {
        frac1 += 0x1FFF;
    }
    frac2 = (vec[2] & 0xFFFF) * mat[7];
    int2 = (vec[2] >> 16) * (mat[7] << 3);
    int1 = (frac0 + (frac1 >> 13)) + int2;
    if (frac2 < 0) {
        frac2 += 0x1FFF;
    }
    output->y = int1 + (frac2 >> 13);

    // Transform Z component
    frac0 = (vec[0] & 0xFFFF) * mat[2];
    int0 = (vec[0] >> 16) * (mat[2] << 3);
    if (frac0 < 0) {
        frac0 += 0x1FFF;
    }
    frac1 = (vec[1] & 0xFFFF) * mat[5];
    int1 = (vec[1] >> 16) * (mat[5] << 3);
    frac0 = (int0 + (frac0 >> 13)) + int1;
    if (frac1 < 0) {
        frac1 += 0x1FFF;
    }
    frac2 = (vec[2] & 0xFFFF) * mat[8];
    int2 = (vec[2] >> 16) * (mat[8] << 3);
    int1 = (frac0 + (frac1 >> 13)) + int2;
    if (frac2 < 0) {
        frac2 += 0x1FFF;
    }
    output->z = int1 + (frac2 >> 13);
}

void transformVector3(Vec3i *arg0, Transform3D *arg1, Vec3i *arg2) {
    s32 *mat;
    s16 *vec;
    s32 frac0;
    s32 int0;
    s32 frac1;
    s32 int1;
    s32 frac2;
    s32 int2;
    mat = (s32 *)arg0;
    vec = (s16 *)arg1;
    __asm__("" : "=r"(mat) : "0"(mat));
    __asm__("" : "=r"(vec) : "0"(vec));
    frac0 = (mat[0] & 0xFFFF) * vec[0];
    int0 = (mat[0] >> 16) * (vec[0] << 3);
    if (frac0 < 0) {
        frac0 += 0x1FFF;
    }
    frac1 = (mat[1] & 0xFFFF) * vec[1];
    int1 = (mat[1] >> 16) * (vec[1] << 3);
    frac0 = (int0 + (frac0 >> 13)) + int1;
    if (frac1 < 0) {
        frac1 += 0x1FFF;
    }
    frac2 = (mat[2] & 0xFFFF) * vec[2];
    int2 = (mat[2] >> 16) * (vec[2] << 3);
    int1 = (frac0 + (frac1 >> 13)) + int2;
    if (frac2 < 0) {
        frac2 += 0x1FFF;
    }
    arg2->x = int1 + (frac2 >> 13);

    frac0 = (mat[0] & 0xFFFF) * vec[3];
    int0 = (mat[0] >> 16) * (vec[3] << 3);
    if (frac0 < 0) {
        frac0 += 0x1FFF;
    }
    frac1 = (mat[1] & 0xFFFF) * vec[4];
    int1 = (mat[1] >> 16) * (vec[4] << 3);
    frac0 = (int0 + (frac0 >> 13)) + int1;
    if (frac1 < 0) {
        frac1 += 0x1FFF;
    }
    frac2 = (mat[2] & 0xFFFF) * vec[5];
    int2 = (mat[2] >> 16) * (vec[5] << 3);
    int1 = (frac0 + (frac1 >> 13)) + int2;
    if (frac2 < 0) {
        frac2 += 0x1FFF;
    }
    arg2->y = int1 + (frac2 >> 13);

    frac0 = (mat[0] & 0xFFFF) * vec[6];
    int0 = (mat[0] >> 16) * (vec[6] << 3);
    if (frac0 < 0) {
        frac0 += 0x1FFF;
    }
    frac1 = (mat[1] & 0xFFFF) * vec[7];
    int1 = (mat[1] >> 16) * (vec[7] << 3);
    frac0 = (int0 + (frac0 >> 13)) + int1;
    if (frac1 < 0) {
        frac1 += 0x1FFF;
    }
    frac2 = (mat[2] & 0xFFFF) * vec[8];
    int2 = (mat[2] >> 16) * (vec[8] << 3);
    int1 = (frac0 + (frac1 >> 13)) + int2;
    if (frac2 < 0) {
        frac2 += 0x1FFF;
    }
    arg2->z = int1 + (frac2 >> 13);
}

void transformVectorRelative(void *arg0, void *arg1, void *arg2) {
    Vec3i diff;
    Vec3i *input = arg0;
    Matrix3x3AndPosition *transform = arg1;
    Vec3i *output = arg2;
    s32 frac0;
    s32 int0a;
    s32 frac1a;
    s32 int1a;
    s32 frac2a;
    s32 int2a;
    s32 int0b;
    s32 frac1b;
    s32 int1b;
    s32 frac2b;
    s32 int2b;
    s32 int0c;
    s32 frac1c;
    s32 int1c;
    s32 frac2c;
    s32 int2c;

    diff.x = input->x - transform->position.x;
    diff.y = input->y - transform->position.y;
    diff.z = input->z - transform->position.z;

    /* Section 1: use int0a as accumulator */
    frac0 = (diff.x & 0xFFFF) * transform->m[0][0];
    int0a = (diff.x >> 16) * (transform->m[0][0] << 3);
    if (frac0 < 0) {
        frac0 += 0x1FFF;
    }
    frac1a = (diff.y & 0xFFFF) * transform->m[0][1];
    int1a = (diff.y >> 16) * (transform->m[0][1] << 3);
    int0a = (int0a + (frac0 >> 13)) + int1a;
    if (frac1a < 0) {
        frac1a += 0x1FFF;
    }
    frac2a = (diff.z & 0xFFFF) * transform->m[0][2];
    int2a = (diff.z >> 16) * (transform->m[0][2] << 3);
    int1a = (int0a + (frac1a >> 13)) + int2a;
    if (frac2a < 0) {
        frac2a += 0x1FFF;
    }
    output->x = int1a + (frac2a >> 13);

    /* Section 2: use frac0 as accumulator */
    frac0 = (diff.x & 0xFFFF) * transform->m[1][0];
    int0b = (diff.x >> 16) * (transform->m[1][0] << 3);
    if (frac0 < 0) {
        frac0 += 0x1FFF;
    }
    frac1b = (diff.y & 0xFFFF) * transform->m[1][1];
    int1b = (diff.y >> 16) * (transform->m[1][1] << 3);
    frac0 = (int0b + (frac0 >> 13)) + int1b;
    if (frac1b < 0) {
        frac1b += 0x1FFF;
    }
    frac2b = (diff.z & 0xFFFF) * transform->m[1][2];
    int2b = (diff.z >> 16) * (transform->m[1][2] << 3);
    int1b = (frac0 + (frac1b >> 13)) + int2b;
    if (frac2b < 0) {
        frac2b += 0x1FFF;
    }
    output->y = int1b + (frac2b >> 13);

    /* Section 3: use frac0 as accumulator */
    frac0 = (diff.x & 0xFFFF) * transform->m[2][0];
    int0c = (diff.x >> 16) * (transform->m[2][0] << 3);
    if (frac0 < 0) {
        frac0 += 0x1FFF;
    }
    frac1c = (diff.y & 0xFFFF) * transform->m[2][1];
    int1c = (diff.y >> 16) * (transform->m[2][1] << 3);
    frac0 = (int0c + (frac0 >> 13)) + int1c;
    if (frac1c < 0) {
        frac1c += 0x1FFF;
    }
    frac2c = (diff.z & 0xFFFF) * transform->m[2][2];
    int2c = (diff.z >> 16) * (transform->m[2][2] << 3);
    int1c = (frac0 + (frac1c >> 13)) + int2c;
    if (frac2c < 0) {
        frac2c += 0x1FFF;
    }
    output->z = int1c + (frac2c >> 13);
}

void rotateVectorY(void *arg0, s16 angle, Vec3i *output) {
    s32 cosVal;
    s32 sinVal;
    s32 cosShifted;
    s32 sinCast;
    s32 inputX;
    s32 *input;
    s32 frac;
    s32 intPart;
    s32 frac2;
    s32 intPart2;
    s32 accumulated;
    s32 zInt;
    s32 zFrac;
    s32 zAccum;

    input = (s32 *)arg0;

    cosVal = approximateCos(angle);
    sinVal = approximateSin(angle);

    inputX = input[0];
    frac = (inputX & 0xFFFF) * (s16)cosVal;
    cosShifted = (s16)cosVal << 3;
    inputX >>= 16;
    intPart = inputX * cosShifted;
    if (frac < 0) {
        frac += 0x1FFF;
    }
    sinCast = (s16)sinVal;
    intPart2 = (input[2] >> 16) * (sinCast << 3);
    frac2 = (input[2] & 0xFFFF) * sinCast;
    accumulated = intPart + (frac >> 13) + intPart2;
    if (frac2 < 0) {
        frac2 += 0x1FFF;
    }
    output->x = accumulated + (frac2 >> 13);

    output->y = input[1];

    inputX = input[0];
    frac = (inputX & 0xFFFF) * -sinCast;
    inputX >>= 16;
    intPart2 = inputX * (-sinCast << 3);
    if (frac < 0) {
        frac += 0x1FFF;
    }
    zInt = (input[2] >> 16) * cosShifted;
    zFrac = (input[2] & 0xFFFF) * (s16)cosVal;
    zAccum = intPart2 + (frac >> 13) + zInt;
    if (zFrac < 0) {
        zFrac += 0x1FFF;
    }
    output->z = zAccum + (zFrac >> 13);
}

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

/**
 * Multiplies a 3x3 rotation matrix by the transpose of another 3x3 rotation matrix.
 * result = left * transpose(right)
 *
 * Computes the product of a matrix and the transpose of another matrix in one step,
 * avoiding the need to explicitly transpose the right matrix first.
 * Used for coordinate system transformations in skeletal animation.
 */
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

void createViewportTransform(
    void *output,
    s32 posX,
    s32 posY,
    s32 posZ,
    s32 depthOffset,
    s32 pitchAngle,
    s32 yawAngle
) {
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
    gScaleMatrix.translation.x = 0;
    gScaleMatrix.translation.y = 0;
    gScaleMatrix.translation.z = depthOffset;
    tempTransformPtr = (s32 *)&gScaleMatrix.translation;
    func_8006B084_6BC84((Transform3D *)(tempTransformPtr - 5), &combined, output);
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
    u16 *yWords = (u16 *)&src->translation.y;

    mtxWords[0] = ((src->m[0][0] * 8) & 0xFFFF0000) + (((s32)((u16)src->m[0][1] << 16) >> 29) & 0xFFFF);
    mtxWords[1] = (src->m[0][2] * 8) & 0xFFFF0000;
    mtxWords[2] = ((src->m[1][0] * 8) & 0xFFFF0000) + (((s32)((u16)src->m[1][1] << 16) >> 29) & 0xFFFF);
    mtxWords[3] = (src->m[1][2] * 8) & 0xFFFF0000;
    mtxWords[4] = ((src->m[2][0] * 8) & 0xFFFF0000) + (((s32)((u16)src->m[2][1] << 16) >> 29) & 0xFFFF);
    mtxWords[5] = (src->m[2][2] * 8) & 0xFFFF0000;
    mtxWords[6] = (src->translation.x & 0xFFFF0000) + yWords[0];
    mtxWords[7] = (src->translation.z & 0xFFFF0000) + 1;
    mtxWords[8] = ((src->m[0][0] << 19) & 0xFFFF0000) + ((src->m[0][1] * 8) & 0xFFFF);
    mtxWords[9] = (src->m[0][2] << 19) & 0xFFFF0000;
    mtxWords[10] = ((src->m[1][0] << 19) & 0xFFFF0000) + ((src->m[1][1] * 8) & 0xFFFF);
    mtxWords[11] = (src->m[1][2] << 19) & 0xFFFF0000;
    mtxWords[12] = ((src->m[2][0] << 19) & 0xFFFF0000) + ((src->m[2][1] * 8) & 0xFFFF);
    mtxWords[13] = (src->m[2][2] << 19) & 0xFFFF0000;
    mtxWords[14] = (src->translation.x << 16) + yWords[1];
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
    s32 *dst = (s32 *)mtx;
    u16 *y_halves = (u16 *)&transform->translation.y;

    // Integer parts of rotation matrix (s2.13 -> s15.16, scaled by 2)
    dst[0] = (((s32)transform->m[0][0] << 1) & 0xFFFF0000) + ((-(s32)((u16)transform->m[0][1] >> 15)) & 0xFFFF);
    dst[1] = ((s32)transform->m[0][2] << 1) & 0xFFFF0000;
    dst[2] = (((s32)transform->m[1][0] << 1) & 0xFFFF0000) + ((-(s32)((u16)transform->m[1][1] >> 15)) & 0xFFFF);
    dst[3] = ((s32)transform->m[1][2] << 1) & 0xFFFF0000;
    dst[4] = (((s32)transform->m[2][0] << 1) & 0xFFFF0000) + ((-(s32)((u16)transform->m[2][1] >> 15)) & 0xFFFF);
    dst[5] = ((s32)transform->m[2][2] << 1) & 0xFFFF0000;

    // Translation integer parts (x high, y high, z high, w=1)
    dst[6] = (transform->translation.x & 0xFFFF0000) + y_halves[0];
    dst[7] = (transform->translation.z & 0xFFFF0000) + 1;

    // Fractional parts of rotation matrix
    dst[8] = (((s32)transform->m[0][0] << 17) & 0xFFFF0000) + ((((s32)transform->m[0][1] << 1) & 0xFFFF));
    dst[9] = ((s32)transform->m[0][2] << 17) & 0xFFFF0000;
    dst[10] = (((s32)transform->m[1][0] << 17) & 0xFFFF0000) + ((((s32)transform->m[1][1] << 1) & 0xFFFF));
    dst[11] = ((s32)transform->m[1][2] << 17) & 0xFFFF0000;
    dst[12] = (((s32)transform->m[2][0] << 17) & 0xFFFF0000) + ((((s32)transform->m[2][1] << 1) & 0xFFFF));
    dst[13] = ((s32)transform->m[2][2] << 17) & 0xFFFF0000;

    // Translation fractional parts (x low, y low, z low, w=0)
    dst[14] = (transform->translation.x << 16) + y_halves[1];
    dst[15] = transform->translation.z << 16;
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

s32 isqrt64(s64 val) {
    u64 remainder;
    u64 mask;
    s32 quotient;
    s32 shift;
    s32 count;

    remainder = 0;
    mask = 0xFFFFFFFF00000000ULL;
    quotient = 0;
    shift = 30;

    for (count = 0; count < 16; count++) {
        if (((u64)val & mask) == 0) {
            break;
        }
        mask <<= 2;
        shift += 2;
    }

    if (count == 0) {
        goto approx_path;
    }

    count = shift;

    while (count >= 0) {
        remainder <<= 1;
        quotient <<= 1;

        if (remainder < ((u64)val >> count)) {
            remainder += 1;
            val -= (remainder << count);
            remainder += 1;
            quotient += 1;
        }
        count -= 2;
    }

    return quotient;

approx_path:
    return approximate_sqrt((u32)val);
}

s32 distance_2d(s32 x, s32 y) {
    s64 x2 = (s64)x * x;
    s64 y2 = (s64)y * y;

    return isqrt64(x2 + y2);
}

s32 distance_3d(s32 x, s32 y, s32 z) {
    return isqrt64((s64)x * x + (s64)y * y + (s64)z * z);
}

void computeLookAtMatrix(Vec3i *from, Vec3i *to, Transform3D *out) {
    s32 diff[3];
    s32 horzDist;
    s32 sinPitch;
    s32 cosPitch;
    s32 sinYaw;
    s32 cosYaw;
    s32 totalDist;
    s32 temp;
    s32 temp2;

    diff[0] = to->x - from->x;
    diff[1] = to->y - from->y;
    diff[2] = to->z - from->z;

    horzDist = isqrt64((s64)diff[0] * diff[0] + (s64)diff[2] * diff[2]);

    sinPitch = 0;
    cosPitch = 0x2000;
    sinYaw = 0;
    cosYaw = 0x2000;

    totalDist = isqrt64((s64)diff[1] * diff[1] + (s64)horzDist * horzDist);

    if (totalDist != 0) {
        sinPitch = (s64)diff[1] * 0x2000 / totalDist;
        cosPitch = (s64)horzDist * 0x2000 / totalDist;
    }

    if (horzDist != 0) {
        sinYaw = -(s64)diff[0] * 0x2000 / horzDist;
        cosYaw = -(s64)diff[2] * 0x2000 / horzDist;
    }

    temp = sinPitch * sinYaw;
    out->m[0][0] = cosYaw;
    out->m[0][1] = 0;
    out->m[0][2] = -sinYaw;
    if (temp < 0) {
        temp += 0x1FFF;
    }

    temp2 = sinPitch * cosYaw;
    out->m[1][0] = temp >> 13;
    out->m[1][1] = cosPitch;
    if (temp2 < 0) {
        temp2 += 0x1FFF;
    }

    temp = cosPitch * sinYaw;
    out->m[1][2] = temp2 >> 13;
    if (temp < 0) {
        temp += 0x1FFF;
    }

    temp2 = cosPitch * cosYaw;
    out->m[2][0] = temp >> 13;
    out->m[2][1] = -sinPitch;
    if (temp2 < 0) {
        temp2 += 0x1FFF;
    }

    out->m[2][2] = temp2 >> 13;
    memcpy(&out->translation, from, sizeof(Vec3i));
}

void matrixToEulerAngles(
    Transform3D *cameraMatrix,
    Transform3D *objectTransform,
    f32 *lookAtX,
    f32 *lookAtY,
    f32 *lookAtZ,
    f32 *upX,
    f32 *upY,
    f32 *upZ
) {
    Transform3D pitchMatrix;
    Transform3D yawMatrix;
    Transform3D tempMatrix;
    Vec3i delta;
    Vec3i rotated;
    s32 rotatedX;
    u32 xzMag;
    u32 totalMag;
    s32 cosVal;
    Transform3D *resultMatrix;
    Transform3D *identity;

    delta.x = cameraMatrix->translation.x - objectTransform->translation.x;
    delta.y = cameraMatrix->translation.y - objectTransform->translation.y;
    delta.z = cameraMatrix->translation.z - objectTransform->translation.z;

    if ((u32)(delta.x + 0x10000000) > 0x20000000U || (u32)(delta.y + 0x10000000) > 0x20000000U ||
        (u32)(delta.z + 0x10000000) > 0x20000000U) {
        *upX = 0.0f;
        *upY = 256.0f;
        *upZ = 0.0f;
        *lookAtX = 0.0f;
        *lookAtY = 0.0f;
        *lookAtZ = -256.0f;
        return;
    }

    rotated.x = (s32)((s64)(s16)((u16 *)cameraMatrix)[0] * delta.x / 0x2000) +
                (s32)((s64)(s16)((u16 *)cameraMatrix)[1] * delta.y / 0x2000) +
                (s32)((s64)(s16)((u16 *)cameraMatrix)[2] * delta.z / 0x2000);
    rotatedX = rotated.x;

    rotated.y = (s32)((s64)(s16)((u16 *)cameraMatrix)[3] * delta.x / 0x2000) +
                (s32)((s64)(s16)((u16 *)cameraMatrix)[4] * delta.y / 0x2000) +
                (s32)((s64)(s16)((u16 *)cameraMatrix)[5] * delta.z / 0x2000);

    rotated.z = (s32)((s64)(s16)((u16 *)cameraMatrix)[6] * delta.x / 0x2000) +
                (s32)((s64)(s16)((u16 *)cameraMatrix)[7] * delta.y / 0x2000) +
                (s32)((s64)(s16)((u16 *)cameraMatrix)[8] * delta.z / 0x2000);

    xzMag = isqrt64((s64)rotatedX * rotatedX + (s64)rotated.z * rotated.z);
    totalMag = isqrt64((u64)xzMag * xzMag + (s64)rotated.y * rotated.y);

    resultMatrix = &pitchMatrix;
    identity = &identityMatrix;
    memcpy(resultMatrix, identity, sizeof(Transform3D));
    if (totalMag != 0) {
        pitchMatrix.m[2][1] = (s64)rotated.y * 0x2000 / totalMag;
        pitchMatrix.m[1][2] = -pitchMatrix.m[2][1];
        cosVal = (s64)xzMag * 0x2000 / totalMag;
        pitchMatrix.m[2][2] = cosVal;
        pitchMatrix.m[1][1] = cosVal;
    }

    memcpy(&yawMatrix, identity, sizeof(Transform3D));
    if (xzMag != 0) {
        yawMatrix.m[2][0] = (s64)rotated.x * 0x2000 / xzMag;
        yawMatrix.m[0][2] = -yawMatrix.m[2][0];
        cosVal = (s64)rotated.z * 0x2000 / xzMag;
        yawMatrix.m[2][2] = cosVal;
        yawMatrix.m[0][0] = cosVal;
    }

    func_8006BDBC_6C9BC((BoneAnimationState *)resultMatrix, &yawMatrix, &tempMatrix);
    func_8006BDBC_6C9BC((BoneAnimationState *)cameraMatrix, &tempMatrix, resultMatrix);

    *lookAtX = (f32)pitchMatrix.m[2][0];
    *lookAtY = (f32)pitchMatrix.m[2][1];
    *lookAtZ = (f32)pitchMatrix.m[2][2];
    *upX = (f32)pitchMatrix.m[1][0];
    *upY = (f32)pitchMatrix.m[1][1];
    *upZ = (f32)pitchMatrix.m[1][2];
}

s16 atan2Fixed(s32 x, s32 y) {
    s16 ratio;
    s16 tableVal;
    s16 result;

    y = -y;

    if (x == 0 && y == 0) {
        return 0;
    }

    if (x >= 0 && y >= 0) {
        if (x < y) {
            ratio = (s64)x * 640 / y;
            tableVal = gAtan2LookupTable[ratio];
            return (-tableVal * 2) & 0x1FFE;
        } else {
            ratio = (s64)y * 640 / x;
            tableVal = gAtan2LookupTable[ratio];
            result = 0x400 - tableVal;
            return (-(result * 2)) & 0x1FFF;
        }
    }

    if (x >= 0 && y < 0) {
        y = -y;
        if (y < x) {
            ratio = (s64)y * 640 / x;
            result = gAtan2LookupTable[ratio] + 0x400;
            return (-(result * 2)) & 0x1FFF;
        } else {
            ratio = (s64)x * 640 / y;
            tableVal = gAtan2LookupTable[ratio];
            result = 0x800 - tableVal;
            return (-(result * 2)) & 0x1FFF;
        }
    }

    if (x < 0 && y < 0) {
        x = -x;
        y = -y;
        if (x < y) {
            ratio = (s64)x * 640 / y;
            result = gAtan2LookupTable[ratio] + 0x800;
            return (-(result * 2)) & 0x1FFF;
        } else {
            ratio = (s64)y * 640 / x;
            tableVal = gAtan2LookupTable[ratio];
            result = 0xC00 - tableVal;
            return (-(result * 2)) & 0x1FFF;
        }
    }

    if (x < 0 && y >= 0) {
        x = -x;
        if (y < x) {
            ratio = (s64)y * 640 / x;
            result = gAtan2LookupTable[ratio] + 0xC00;
            return (-(result * 2)) & 0x1FFF;
        } else {
            ratio = (s64)x * 640 / y;
            tableVal = gAtan2LookupTable[ratio];
            result = 0x1000 - tableVal;
            return (-(result * 2)) & 0x1FFF;
        }
    }

    return 0;
}

s32 computeAngleToPosition(s32 fromX, s32 fromZ, s32 toX, s32 toZ) {
    return atan2Fixed(toX - fromX, toZ - fromZ);
}