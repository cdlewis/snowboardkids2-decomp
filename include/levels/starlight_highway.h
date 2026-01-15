#pragma once

#include "common.h"
#include "displaylist.h"
#include "geometry.h"

typedef struct {
    s16 timer;
} FireworkShowTimerState;

typedef struct {
    /* 0x00 */ Transform3D node1;
    /* 0x20 */ u8 _pad20[4];
    /* 0x24 */ void *unk24;
    /* 0x28 */ void *unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ u8 _pad30[0xC];
    /* 0x3C */ Transform3D node2;
    /* 0x5C */ u8 _pad5C[4];
    /* 0x60 */ void *unk60;
    /* 0x64 */ void *unk64;
    /* 0x68 */ s32 unk68;
} StarlightBuildingTaskState;

typedef struct {
    /* 0x00 */ Transform3D mat1;
    /* 0x20 */ DisplayLists *displayLists1;
    /* 0x24 */ void *segment1;
    /* 0x28 */ void *segment2;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ u8 _pad30[0xC];
    /* 0x3C */ Transform3D mat2;
    /* 0x5C */ DisplayLists *displayLists2;
    /* 0x60 */ void *unk60;
    /* 0x64 */ void *unk64;
    /* 0x68 */ s32 unk68;
    /* 0x6C */ u8 _pad6C[0xC];
    /* 0x78 */ s16 scale;
    /* 0x7A */ u16 rotation;
    /* 0x7C */ u8 positionIndex;
} StarlightItemTaskState;

typedef struct {
    u8 _pad[0x24];
    void *segment1;
    void *segment2;
} DualSegmentCleanupState;

s32 func_800BB504(Vec3i *arg0, s32 arg1);
void spawnDebugDisplayListTask(s16 arg0);
void spawnStarlightItems(void);
void func_800BC9BC(void);
void initFireworkShowTimer(FireworkShowTimerState *arg0);
void updateFireworkShowTimer(FireworkShowTimerState *arg0);
void initStarlightHighwayBuildingTask(StarlightBuildingTaskState *arg0);
void initStarlightItemTask(StarlightItemTaskState *);
void cleanupStarlightItemTask(DualSegmentCleanupState *);
void updateStarlightItemTask(StarlightItemTaskState *);
