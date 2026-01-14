#pragma once

#include "common.h"
#include "geometry.h"

typedef struct {
    /* 0x00 */ u8 pad[0x14];
    /* 0x14 */ Vec3i position;
    /* 0x20 */ void *displayListState;
    /* 0x24 */ void *uncompressedAsset;
    /* 0x28 */ void *compressedAsset;
    /* 0x2C */ s32 animationFrame;
    /* 0x30 */ u8 pad30[0xC];
    /* 0x3C */ s32 pivotX;
    /* 0x40 */ s32 pivotY;
    /* 0x44 */ s32 pivotZ;
    /* 0x48 */ s16 swingPhase;
    /* 0x4A */ s16 swingAngle;
    /* 0x4C */ s16 bobPhase;
    /* 0x4E */ s16 phaseTimer;
    /* 0x50 */ s16 fireProjectileCooldown;
} SwingingPendulumTrap;

typedef struct {
    s16 ghostType;
    s16 spawnTimer;
} GhostSpawnerTask;

typedef struct {
    void *ghostSlotData;
    void *unk4;
} GhostManager;

void func_800BC0FC(s16 arg0);

void initGhostSpawnerTask(GhostSpawnerTask *arg0);

void initSwingingPendulumTrap(SwingingPendulumTrap *arg0);

void initFloatingBillboardSpawner(s16 *spawnTimer);

void func_800BC13C_AFE2C(GhostManager *ghostManager);

void func_800BC72C_B041C(s16 *arg0);
