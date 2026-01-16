#pragma once

#include "common.h"
#include "geometry.h"

typedef struct {
    /* 0x00 */ Transform3D transform;       /* Transformation matrix */
    /* 0x20 */ void *displayLists;           /* Display lists pointer (offset +0xB0 from asset data) */
    /* 0x24 */ void *uncompressedAssetData;  /* Uncompressed asset data */
    /* 0x28 */ void *compressedAssetData;    /* Compressed segment2 asset data */
    /* 0x2C */ s32 segment3Ptr;              /* Always set to 0 */
    /* 0x30 */ void *segment2;               /* Display list segment 2 pointer */
    /* 0x34 */ Mtx *transformMatrix;         /* Transform matrix pointer */
    /* 0x38 */ u8 light1R;
    /* 0x39 */ u8 light1G;
    /* 0x3A */ u8 light1B;
    /* 0x3B */ u8 numParts;
    /* 0x3C */ s32 yVelocity;                /* Y velocity (downward speed) */
    /* 0x40 */ s32 xVelocity;                /* X velocity */
    /* 0x44 */ s32 zVelocity;                /* Z velocity */
    /* 0x48 */ s32 distance;                 /* Distance for atan2 calculation */
    /* 0x4C */ s16 delayTimer;               /* Delay timer before starting next flight */
    /* 0x4E */ u16 rotationAngle;            /* Y-axis rotation angle */
} SunnyMountainFlyingBirdTask;

void initSunnyMountainFlyingBirdTask(SunnyMountainFlyingBirdTask *arg0);
