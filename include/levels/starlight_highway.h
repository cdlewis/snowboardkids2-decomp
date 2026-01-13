#pragma once

#include "common.h"
#include "geometry.h"

typedef struct {
    s16 unk0;
} func_800BC528_AE8E8_arg;

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

s32 func_800BB504(Vec3i *arg0, s32 arg1);
void spawnDebugDisplayListTask(s16 arg0);
void func_800BBB90(s16 arg0);
void func_800BBED8(void);
void func_800BC9BC(void);
void func_800BC528_AE8E8(func_800BC528_AE8E8_arg *arg0);
void initStarlightHighwayBuildingTask(StarlightBuildingTaskState *arg0);
