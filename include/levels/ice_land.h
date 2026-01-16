#pragma once

#include "common.h"

typedef struct {
    u8 pad[0x24];
    void *segment1;
    void *segment2;
} IceLandMovingPlatformCleanupArgs;

typedef struct {
    s16 timer;
    s16 pathIndex;
    u8 pad4[0x20];
    void *segment1;
    void *segment2;
} IceLandMovingPlatformSchedulerTask;

void func_800BB688_B2408(IceLandMovingPlatformSchedulerTask *arg0);
