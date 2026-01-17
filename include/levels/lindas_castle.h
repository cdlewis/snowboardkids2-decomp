#pragma once

#include "common.h"
#include "geometry.h"

typedef struct {
    /* 0x00 */ Transform3D matrix;
    /* 0x20 */ u8 _pad20[0x1C];
    /* 0x3C */ s32 targetPosition[3];
    /* 0x48 */ s32 velocityY;
    /* 0x4C */ s32 gravity;
    /* 0x50 */ s16 waypointIndex;
    /* 0x52 */ s16 targetWaypointIndex;
    /* 0x54 */ u16 rotationAngle;
    /* 0x56 */ s16 surfaceType;
} FlyingEnemyTaskArg;

typedef struct {
    u16 waypointIndex;
    s16 spawnTimer;
} FlyingEnemySpawnerState;

void func_800BB2B0(FlyingEnemyTaskArg *arg0);
void func_800BBA50(s32 arg0);
void initCrazyJungleHazards(void);
void initFlyingEnemySpawner(FlyingEnemySpawnerState *arg0);
void updateFlyingEnemySpawner(FlyingEnemySpawnerState *arg0);
void func_800BBAF8_AB9A8(s16 *arg0);