#pragma once

#include "common.h"
#include "graphics/displaylist.h"
#include "math/geometry.h"

typedef struct {
    /* 0x00 */ DisplayListObject renderObject;
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
    void *ghostMatrices;
    void *ghostSpriteAsset;
    u8 textureIndices[8];
} GhostManager;

typedef struct {
    /* 0x00 */ void *spriteAsset;
    /* 0x04 */ loadAssetMetadata_arg sprite;
    /* 0x20 */ u8 padding20[4];
    /* 0x24 */ Vec3i worldPosition;
    /* 0x30 */ Vec3i velocity;
    /* 0x3C */ s16 ghostType;
    /* 0x3E */ s16 trackSector;
    /* 0x40 */ s16 animationTimer;
    /* 0x42 */ s16 lifetime;
    /* 0x44 */ s16 animationFrameIndex;
    /* 0x46 */ u8 fadeDirection;
} AnimatedGhostEntity;

typedef struct {
    /* 0x00 */ void *spriteAsset;
    /* 0x04 */ loadAssetMetadata_arg sprite;
    /* 0x20 */ u8 padding20[4];
    /* 0x24 */ s32 targetYOffset;
    /* 0x28 */ s16 lifetime;
} FloatingBillboard;

typedef struct {
    /* 0x00 */ void *spriteAsset;
    /* 0x04 */ loadAssetMetadata_arg sprite;
    /* 0x20 */ u8 padding20[4];
    /* 0x24 */ s16 variantIndex;
    /* 0x26 */ s16 animationPhase;
} FloatingSpriteEntity;

void scheduleFloatingSpriteEntity(s16 variantIndex);

void initGhostSpawnerTask(GhostSpawnerTask *spawner);

void initSwingingPendulumTrap(SwingingPendulumTrap *trap);

void cleanupSwingingPendulumTrap(SwingingPendulumTrap *trap);

void initFloatingBillboardSpawner(s16 *spawnTimer);

void initGhostManager(GhostManager *ghostManager);

void initLapCounterTask(s16 *lapCounter);

void renderGhosts(GhostManager *ghostManager);
