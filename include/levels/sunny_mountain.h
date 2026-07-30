#pragma once

#include "common.h"
#include "graphics/displaylist.h"
#include "math/geometry.h"

typedef struct {
    /* 0x00 */ Vec3i startPosition;
    /* 0x0C */ Vec3i endPosition;
    /* 0x18 */ DisplayListObject liftDisplayObject;
    /* 0x54 */ DisplayListObject *chairDisplayObjects;
    /* 0x58 */ Transform3D chairTransforms[4];
    /* 0xD8 */ s16 waypointIndex;
    /* 0xDA */ u16 chairRotationAngle;
} SunnyMountainChairLiftTask;

typedef struct {
    /* 0x00 */ DisplayListObject displayObject;
    /* 0x3C */ s32 yVelocity;
    /* 0x40 */ s32 xVelocity;
    /* 0x44 */ s32 zVelocity;
    /* 0x48 */ s32 horizontalSpeed;
    /* 0x4C */ s16 delayTimer;
    /* 0x4E */ u16 flightAngle;
} SunnyMountainFlyingBirdTask;

void initSunnyMountainChairLiftTask(SunnyMountainChairLiftTask *taskState);
void initSunnyMountainFlyingBirdTask(SunnyMountainFlyingBirdTask *arg0);
