#pragma once

#include "common.h"
#include "math/geometry.h"

typedef struct {
    /* 0x00 */ s16 viewMatrix[9];
    /* 0x12 */ u8 pad12[0x2];
    /* 0x14 */ Vec3i viewTranslation;
    /* 0x20 */ s16 orientMatrix[9];
    /* 0x32 */ u8 pad32[0x2];
    /* 0x34 */ s32 cameraX;
    /* 0x38 */ s32 cameraY;
    /* 0x3C */ s32 cameraZ;
    /* 0x40 */ u8 pad40[0x4];
    /* 0x44 */ s16 viewAngle;
    /* 0x46 */ s16 targetAngle;
    /* 0x48 */ s16 orbitAngle;
    /* 0x4A */ u8 pad4A[0x2];
    /* 0x4C */ s32 orbitRadius;
    /* 0x50 */ s16 unk50;
    /* 0x52 */ u8 pad52[0x2];
    /* 0x54 */ s32 travelDistance;
    /* 0x58 */ u8 pad58[0x2];
    /* 0x5A */ s8 horizontalSpeed;
    /* 0x5B */ s8 verticalSpeed;
} StoryMapCameraState;

s32 __abs(s32 n);
