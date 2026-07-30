#pragma once

#include "common.h"
#include "data/data_table.h"
#include "gbi.h"
#include "levels/snowboard_street.h"
#include "math/geometry.h"

typedef struct {
    /* 0x00 */ s8 state;
    /* 0x01 */ u8 padding[3];
    /* 0x04 */ Vec3i position;
} ShootCrossTarget;

typedef struct ShootCrossTargets {
    /* 0x00 */ Mtx *transformMatrices;
    /* 0x04 */ Vtx *quadVertices;
    /* 0x08 */ ShootCrossTarget *targets;
    /* 0x0C */ DataTable_19E80 *spriteTable;
    /* 0x10 */ s32 *positionData;
    /* 0x14 */ u8 padding[2];
    /* 0x16 */ s16 targetCount;
} ShootCrossTargets;

void initRotatingSky(SnowboardStreetRotatingSky *rotatingSky);
s32 checkProjectileTargetHit(Vec3i *projectilePos, s32 hitRange);
void scheduleShootCrossTargetsTask(s32 courseId);
