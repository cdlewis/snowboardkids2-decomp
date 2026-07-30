#pragma once

#include "common.h"
#include "levels/snowboard_street.h"
#include "math/geometry.h"

void initRotatingSky(SnowboardStreetRotatingSky *rotatingSky);
s32 checkProjectileTargetHit(Vec3i *projectilePos, s32 hitRange);
void scheduleShootCrossTargetsTask(s32 courseId);
