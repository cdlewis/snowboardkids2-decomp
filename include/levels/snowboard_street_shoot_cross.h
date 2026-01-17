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

void initRotatingSky(RotatingSkyArg *arg0);
s32 checkProjectileTargetHit(Vec3i *arg0, s32 arg1);
void scheduleShootCrossTargetsTask(s32 courseId);
