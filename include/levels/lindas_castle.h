#pragma once

#include "common.h"
#include "graphics/displaylist.h"

typedef struct {
    s32 x;
    s32 z;
} FlyingEnemyWaypoint;

typedef struct {
    s32 surfaceType;
    FlyingEnemyWaypoint waypoints[2];
} FlyingEnemyPath;

typedef struct {
    /* 0x00 */ DisplayListObject displayObject;
    /* 0x3C */ Vec3i trackPosition;
    /* 0x48 */ s32 heightOffset;
    /* 0x4C */ s32 verticalVelocity;
    /* 0x50 */ s16 pathIndex;
    /* 0x52 */ s16 targetWaypointIndex;
    /* 0x54 */ u16 yawAngle;
    /* 0x56 */ s16 surfaceType;
} FlyingEnemyTask;

typedef struct {
    u16 pathIndex;
    s16 spawnTimer;
} FlyingEnemySpawnerState;

void renderFlyingEnemy(FlyingEnemyTask *task);
void initCrazyJungleHazards(void);
void initFlyingEnemySpawner(FlyingEnemySpawnerState *spawner);
void updateFlyingEnemySpawner(FlyingEnemySpawnerState *spawner);
void initLindasCastleLapCounter(s16 *lapCounter);
