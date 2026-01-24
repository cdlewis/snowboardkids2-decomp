#pragma once

#include "common.h"
#include "geometry.h"

// Struct definitions

typedef struct {
    /* 0x00 */ u8 _pad[0x5C];
    /* 0x5C */ u8 memoryPoolId;
} WendysHouseAllocation;

typedef struct {
    /* 0x00 */ s16 matrix[6];
    /* 0x0C */ u8 _pad[0x8];
    /* 0x14 */ s32 posX;
    /* 0x18 */ s32 posY;
    /* 0x1C */ s32 posZ;
    /* 0x20 */ void *displayLists;
    /* 0x24 */ void *uncompressedAsset;
    /* 0x28 */ void *compressedAsset;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ u8 _pad2[0xC];
    /* 0x3C */ s16 oscillationAngle;
    /* 0x3E */ u8 _pad3[0x2];
    /* 0x40 */ s32 localOffsetX;
    /* 0x44 */ s32 localOffsetZ;
    /* 0x48 */ s32 unk48;
} RotatingPlatformTaskState;

typedef struct {
    /* 0x00 */ u8 _pad[0x24];
    /* 0x24 */ void *uncompressedAsset;
    /* 0x28 */ void *compressedAsset;
} TaskAssetState;

typedef struct {
    /* 0x00 */ u8 _pad0[0x14];
    /* 0x14 */ Vec3i position;
    /* 0x20 */ void *displayLists;
    /* 0x24 */ void *uncompressedAsset;
    /* 0x28 */ void *compressedAsset;
    /* 0x2C */ s32 animTimer;
    /* 0x30 */ u8 _pad1[0xC];
    /* 0x3C */ Vec3i velocity;
    /* 0x48 */ u16 rotX;
    /* 0x4A */ u16 rotY;
    /* 0x4C */ s16 targetIndex;
    /* 0x4E */ s16 projectileState;
} WendysHouseProjectileTaskState;

typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 unk6;
} WendysHouseProjectileSpawnerState;

// Function declarations

void initRotatingPlatformTask(RotatingPlatformTaskState *arg0);
void updateRotatingPlatformTask(RotatingPlatformTaskState *arg0);
void cleanupRotatingPlatformTask(TaskAssetState *arg0);

void initWendysHouseProjectileTask(WendysHouseProjectileTaskState *arg0);
void updateWendysHouseProjectileTask(WendysHouseProjectileTaskState *arg0);
void cleanupWendysHouseProjectileTask(TaskAssetState *arg0);

void initWendysHouseProjectileSpawner(WendysHouseProjectileSpawnerState *arg0);
void updateWendysHouseProjectileSpawner(WendysHouseProjectileSpawnerState *arg0);
