#include "geometry.h"

#include "common.h"

// 32-byte identity matrix (used in transform3DToMtx)
Transform3D identityMatrix = {
    .m = { { 0x2000, 0x0000, 0x0000 }, { 0x0000, 0x2000, 0x0000 }, { 0x0000, 0x0000, 0x2000 } },
    .translation = { 0,                          0,                          0                          }
};

// Default scale matrix (20-byte packed diagonal matrix with 1.0 values)
u8 gScaleMatrix[20] = {
    0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00,
};

// Temporary position vector used in transform calculations
Vec3i gTempPosition = { 0, 0, 0 };

// 32-byte identity matrix used in rotation calculations
u8 gIdentityMatrix32[32] = {
    0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

// Arctangent lookup table with 341 entries
s16 gAtan2LookupTable[] = {
    0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008, 0x0009, 0x000A, 0x000B, 0x000C, 0x000D,
    0x000E, 0x000F, 0x0010, 0x0011, 0x0012, 0x0013, 0x0014, 0x0015, 0x0016, 0x0017, 0x0018, 0x0019, 0x001A, 0x001B,
    0x001C, 0x001D, 0x001E, 0x001F, 0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027, 0x0028, 0x0029,
    0x002A, 0x002B, 0x002C, 0x002D, 0x002E, 0x002F, 0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037,
    0x0038, 0x0039, 0x003A, 0x003B, 0x003C, 0x003D, 0x003E, 0x003F, 0x0040, 0x0041, 0x0042, 0x0043, 0x0045, 0x0046,
    0x0047, 0x0048, 0x0049, 0x004A, 0x004B, 0x004C, 0x004D, 0x004E, 0x004F, 0x0050, 0x0051, 0x0052, 0x0053, 0x0054,
    0x0055, 0x0056, 0x0057, 0x0058, 0x0059, 0x005A, 0x005B, 0x005C, 0x005D, 0x005E, 0x005F, 0x0060, 0x0061, 0x0062,
    0x0063, 0x0064, 0x0065, 0x0066, 0x0067, 0x0068, 0x0069, 0x006A, 0x006A, 0x006B, 0x006C, 0x006D, 0x006E, 0x006F,
    0x0070, 0x0071, 0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077, 0x0078, 0x0079, 0x007A, 0x007B, 0x007C, 0x007D,
    0x007E, 0x007F, 0x0080, 0x0081, 0x0082, 0x0083, 0x0084, 0x0085, 0x0086, 0x0087, 0x0088, 0x0089, 0x008A, 0x008B,
    0x008C, 0x008D, 0x008E, 0x008F, 0x0090, 0x0091, 0x0092, 0x0093, 0x0094, 0x0095, 0x0096, 0x0097, 0x0098, 0x0098,
    0x0099, 0x009A, 0x009B, 0x009C, 0x009D, 0x009E, 0x009F, 0x00A0, 0x00A1, 0x00A2, 0x00A3, 0x00A4, 0x00A5, 0x00A6,
    0x00A7, 0x00A8, 0x00A9, 0x00AA, 0x00AB, 0x00AC, 0x00AD, 0x00AD, 0x00AE, 0x00AF, 0x00B0, 0x00B1, 0x00B2, 0x00B3,
    0x00B4, 0x00B5, 0x00B6, 0x00B7, 0x00B8, 0x00B9, 0x00BA, 0x00BB, 0x00BC, 0x00BD, 0x00BD, 0x00BE, 0x00BF, 0x00C0,
    0x00C1, 0x00C2, 0x00C3, 0x00C4, 0x00C5, 0x00C6, 0x00C7, 0x00C8, 0x00C9, 0x00CA, 0x00CB, 0x00CB, 0x00CC, 0x00CD,
    0x00CE, 0x00CF, 0x00D0, 0x00D1, 0x00D2, 0x00D3, 0x00D4, 0x00D5, 0x00D6, 0x00D6, 0x00D7, 0x00D8, 0x00D9, 0x00DA,
    0x00DB, 0x00DC, 0x00DD, 0x00DE, 0x00DF, 0x00E0, 0x00E0, 0x00E1, 0x00E2, 0x00E3, 0x00E4, 0x00E5, 0x00E6, 0x00E7,
    0x00E8, 0x00E8, 0x00E9, 0x00EA, 0x00EB, 0x00EC, 0x00ED, 0x00EE, 0x00EF, 0x00F0, 0x00F0, 0x00F1, 0x00F2, 0x00F3,
    0x00F4, 0x00F5, 0x00F6, 0x00F7, 0x00F8, 0x00F8, 0x00F9, 0x00FA, 0x00FB, 0x00FC, 0x00FD, 0x00FE, 0x00FF, 0x00FF,
    0x0100, 0x0101, 0x0102, 0x0103, 0x0104, 0x0105, 0x0105, 0x0106, 0x0107, 0x0108, 0x0109, 0x010A, 0x010B, 0x010B,
    0x010C, 0x010D, 0x010E, 0x010F, 0x0110, 0x0111, 0x0111, 0x0112, 0x0113, 0x0114, 0x0115, 0x0116, 0x0117, 0x0117,
    0x0118, 0x0119, 0x011A, 0x011B, 0x011C, 0x011C, 0x011D, 0x011E, 0x011F, 0x0120, 0x0121, 0x0121, 0x0122, 0x0123,
    0x0124, 0x0125, 0x0126, 0x0126, 0x0127, 0x0128, 0x0129, 0x012A, 0x012A, 0x012B, 0x012C, 0x012D, 0x012E, 0x012F,
    0x012F, 0x0130, 0x0131, 0x0132, 0x0133, 0x0133, 0x0134, 0x0135, 0x0136, 0x0137, 0x0137, 0x0138, 0x0139, 0x013A,
    0x013B, 0x013B, 0x013C, 0x013D, 0x013E, 0x013F, 0x013F, 0x0140, 0x0141, 0x0142, 0x0143, 0x0143, 0x0144, 0x0145,
    0x0146, 0x0147, 0x0147, 0x0148, 0x0149, 0x014A, 0x014A, 0x014B, 0x014C, 0x014D, 0x014E, 0x014E, 0x014F, 0x0150,
    0x0151, 0x0151, 0x0152, 0x0153, 0x0154, 0x0154, 0x0155, 0x0156, 0x0157, 0x0157, 0x0158, 0x0159, 0x015A, 0x015B,
    0x015B, 0x015C, 0x015D, 0x015E, 0x015E, 0x015F, 0x0160, 0x0161, 0x0161, 0x0162, 0x0163, 0x0164, 0x0164, 0x0165,
    0x0166, 0x0166, 0x0167, 0x0168, 0x0169, 0x0169, 0x016A, 0x016B, 0x016C, 0x016C, 0x016D, 0x016E, 0x016F, 0x016F,
    0x0170, 0x0171, 0x0171, 0x0172, 0x0173, 0x0174, 0x0174, 0x0175, 0x0176, 0x0177, 0x0177, 0x0178, 0x0179, 0x0179,
    0x017A, 0x017B, 0x017C, 0x017C, 0x017D, 0x017E, 0x017E, 0x017F, 0x0180, 0x0180, 0x0181, 0x0182, 0x0183, 0x0183,
    0x0184, 0x0185, 0x0185, 0x0186, 0x0187, 0x0187, 0x0188, 0x0189, 0x018A, 0x018A, 0x018B, 0x018C, 0x018C, 0x018D,
    0x018E, 0x018E, 0x018F, 0x0190, 0x0190, 0x0191, 0x0192, 0x0192, 0x0193, 0x0194, 0x0194, 0x0195, 0x0196, 0x0196,
    0x0197, 0x0198, 0x0198, 0x0199, 0x019A, 0x019A, 0x019B, 0x019C, 0x019C, 0x019D, 0x019E, 0x019E, 0x019F, 0x01A0,
    0x01A0, 0x01A1, 0x01A2, 0x01A2, 0x01A3, 0x01A4, 0x01A4, 0x01A5, 0x01A6, 0x01A6, 0x01A7, 0x01A8, 0x01A8, 0x01A9,
    0x01A9, 0x01AA, 0x01AB, 0x01AB, 0x01AC, 0x01AD, 0x01AD, 0x01AE, 0x01AF, 0x01AF, 0x01B0, 0x01B0, 0x01B1, 0x01B2,
    0x01B2, 0x01B3, 0x01B4, 0x01B4, 0x01B5, 0x01B5, 0x01B6, 0x01B7, 0x01B7, 0x01B8, 0x01B9, 0x01B9, 0x01BA, 0x01BA,
    0x01BB, 0x01BC, 0x01BC, 0x01BD, 0x01BE, 0x01BE, 0x01BF, 0x01BF, 0x01C0, 0x01C1, 0x01C1, 0x01C2, 0x01C2, 0x01C3,
    0x01C4, 0x01C4, 0x01C5, 0x01C5, 0x01C6, 0x01C7, 0x01C7, 0x01C8, 0x01C8, 0x01C9, 0x01CA, 0x01CA, 0x01CB, 0x01CB,
    0x01CC, 0x01CD, 0x01CD, 0x01CE, 0x01CE, 0x01CF, 0x01CF, 0x01D0, 0x01D1, 0x01D1, 0x01D2, 0x01D2, 0x01D3, 0x01D4,
    0x01D4, 0x01D5, 0x01D5, 0x01D6, 0x01D6, 0x01D7, 0x01D8, 0x01D8, 0x01D9, 0x01D9, 0x01DA, 0x01DA, 0x01DB, 0x01DC,
    0x01DC, 0x01DD, 0x01DD, 0x01DE, 0x01DE, 0x01DF, 0x01DF, 0x01E0, 0x01E1, 0x01E1, 0x01E2, 0x01E2, 0x01E3, 0x01E3,
    0x01E4, 0x01E4, 0x01E5, 0x01E6, 0x01E6, 0x01E7, 0x01E7, 0x01E8, 0x01E8, 0x01E9, 0x01E9, 0x01EA, 0x01EA, 0x01EB,
    0x01EC, 0x01EC, 0x01ED, 0x01ED, 0x01EE, 0x01EE, 0x01EF, 0x01EF, 0x01F0, 0x01F0, 0x01F1, 0x01F1, 0x01F2, 0x01F3,
    0x01F3, 0x01F4, 0x01F4, 0x01F5, 0x01F5, 0x01F6, 0x01F6, 0x01F7, 0x01F7, 0x01F8, 0x01F8, 0x01F9, 0x01F9, 0x01FA,
    0x01FA, 0x01FB, 0x01FB, 0x01FC, 0x01FC, 0x01FD, 0x01FD, 0x01FE, 0x01FE, 0x01FF, 0x01FF, 0x01FF, 0x01FF, 0x01FF,
    0x01FF, 0x01FF, 0x01FF, 0x01FF,
};

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

INCLUDE_ASM("asm/nonmatchings/geometry", func_8006B084_6BC84);

void transformVector(s16 *inputVec, s16 *transform, void *outputPtr) {
    s32 *mat;
    s16 *vec;
    s32 *out;
    s32 frac0;
    s32 int0;
    s32 frac1;
    s32 int1;
    s32 frac2;
    s32 int2;
    mat = (s32 *)inputVec;
    vec = transform;
    out = (s32 *)outputPtr;
    __asm__("" : "=r"(mat) : "0"(mat));
    __asm__("" : "=r"(vec) : "0"(vec));

    frac0 = (mat[0] & 0xFFFF) * vec[0];
    int0 = (mat[0] >> 16) * (vec[0] << 3);
    if (frac0 < 0) {
        frac0 += 0x1FFF;
    }
    frac1 = (mat[1] & 0xFFFF) * vec[3];
    int1 = (mat[1] >> 16) * (vec[3] << 3);
    frac0 = (int0 + (frac0 >> 13)) + int1;
    if (frac1 < 0) {
        frac1 += 0x1FFF;
    }
    frac2 = (mat[2] & 0xFFFF) * vec[6];
    int2 = (mat[2] >> 16) * (vec[6] << 3);
    int1 = (frac0 + (frac1 >> 13)) + int2;
    if (frac2 < 0) {
        frac2 += 0x1FFF;
    }
    out[0] = int1 + (frac2 >> 13) + ((s32 *)vec)[5];

    frac0 = (mat[0] & 0xFFFF) * vec[1];
    int0 = (mat[0] >> 16) * (vec[1] << 3);
    if (frac0 < 0) {
        frac0 += 0x1FFF;
    }
    frac1 = (mat[1] & 0xFFFF) * vec[4];
    int1 = (mat[1] >> 16) * (vec[4] << 3);
    frac0 = (int0 + (frac0 >> 13)) + int1;
    if (frac1 < 0) {
        frac1 += 0x1FFF;
    }
    frac2 = (mat[2] & 0xFFFF) * vec[7];
    int2 = (mat[2] >> 16) * (vec[7] << 3);
    int1 = (frac0 + (frac1 >> 13)) + int2;
    if (frac2 < 0) {
        frac2 += 0x1FFF;
    }
    out[1] = int1 + (frac2 >> 13) + ((s32 *)vec)[6];

    frac0 = (mat[0] & 0xFFFF) * vec[2];
    int0 = (mat[0] >> 16) * (vec[2] << 3);
    if (frac0 < 0) {
        frac0 += 0x1FFF;
    }
    frac1 = (mat[1] & 0xFFFF) * vec[5];
    int1 = (mat[1] >> 16) * (vec[5] << 3);
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
    out[2] = int1 + (frac2 >> 13) + ((s32 *)vec)[7];
}

void transformVector2(void *matrix, void *vector, Vec3i *arg2) {
    s32 *mat;
    s16 *vec;
    s32 frac0;
    s32 int0;
    s32 frac1;
    s32 int1;
    s32 frac2;
    s32 int2;
    mat = matrix;
    vec = vector;
    __asm__("" : "=r"(mat) : "0"(mat));
    __asm__("" : "=r"(vec) : "0"(vec));
    frac0 = (mat[0] & 0xFFFF) * vec[0];
    int0 = (mat[0] >> 16) * (vec[0] << 3);
    if (frac0 < 0) {
        frac0 += 0x1FFF;
    }
    frac1 = (mat[1] & 0xFFFF) * vec[3];
    int1 = (mat[1] >> 16) * (vec[3] << 3);
    frac0 = (int0 + (frac0 >> 13)) + int1;
    if (frac1 < 0) {
        frac1 += 0x1FFF;
    }
    frac2 = (mat[2] & 0xFFFF) * vec[6];
    int2 = (mat[2] >> 16) * (vec[6] << 3);
    int1 = (frac0 + (frac1 >> 13)) + int2;
    if (frac2 < 0) {
        frac2 += 0x1FFF;
    }
    arg2->x = int1 + (frac2 >> 13);

    frac0 = (mat[0] & 0xFFFF) * vec[1];
    int0 = (mat[0] >> 16) * (vec[1] << 3);
    if (frac0 < 0) {
        frac0 += 0x1FFF;
    }
    frac1 = (mat[1] & 0xFFFF) * vec[4];
    int1 = (mat[1] >> 16) * (vec[4] << 3);
    frac0 = (int0 + (frac0 >> 13)) + int1;
    if (frac1 < 0) {
        frac1 += 0x1FFF;
    }
    frac2 = (mat[2] & 0xFFFF) * vec[7];
    int2 = (mat[2] >> 16) * (vec[7] << 3);
    int1 = (frac0 + (frac1 >> 13)) + int2;
    if (frac2 < 0) {
        frac2 += 0x1FFF;
    }
    arg2->y = int1 + (frac2 >> 13);

    frac0 = (mat[0] & 0xFFFF) * vec[2];
    int0 = (mat[0] >> 16) * (vec[2] << 3);
    if (frac0 < 0) {
        frac0 += 0x1FFF;
    }
    frac1 = (mat[1] & 0xFFFF) * vec[5];
    int1 = (mat[1] >> 16) * (vec[5] << 3);
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
    gTempPosition.x = 0;
    gTempPosition.y = 0;
    gTempPosition.z = tempPosZ;
    tempTransformPtr = (s32 *)&gTempPosition;
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

s32 atan2Fixed(s32 x, s32 y) {
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

s16 computeAngleToPosition(s32 fromX, s32 fromZ, s32 toX, s32 toZ) {
    return atan2Fixed(toX - fromX, toZ - fromZ);
}