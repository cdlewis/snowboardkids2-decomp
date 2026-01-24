#pragma once

#include "common.h"
#include "geometry.h"

typedef struct {
    u8 _pad[0x14];
    s32 posX;
    s32 posY;
    s32 posZ;
    void *displayLists;
    void *uncompressedAsset;
    void *compressedAsset;
    s32 unk2C;
} RotatingSkyArg;

typedef struct {
    /* 0x00 */ Transform3D matrix;
    /* 0x20 */ u8 _pad20[0x1C];
    /* 0x3C */ u16 rotationAngle;
} RotatingSkyRenderArg;

void initSpeedCrossRotatingSky(RotatingSkyArg *arg0);
