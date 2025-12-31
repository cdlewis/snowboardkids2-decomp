#pragma once

#include "common.h"
#include "geometry.h"

typedef struct {
    /* 0x00 */ u8 pad[0x14];
    /* 0x14 */ Vec3i unk14;
    /* 0x20 */ void *unk20;
    /* 0x24 */ void *unk24;
    /* 0x28 */ void *unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ u8 pad30[0xC];
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ s32 unk40;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ s16 unk48;
    /* 0x4A */ s16 unk4A;
    /* 0x4C */ s16 unk4C;
    /* 0x4E */ s16 unk4E;
    /* 0x50 */ s16 unk50;
} func_800BB8E8_AF5D8_arg;

typedef struct {
    s16 unk0;
    s16 unk2;
} func_800BB74C_AF43C_arg;

typedef struct {
    void *ghostSlotData;
    void *unk4;
} GhostManager;

void func_800BC0FC(s16 arg0);

void func_800BB74C_AF43C(func_800BB74C_AF43C_arg *arg0);

void func_800BB8E8_AF5D8(func_800BB8E8_AF5D8_arg *arg0);

void func_800BBE84_AFB74(s16 *arg0);

void func_800BC13C_AFE2C(GhostManager *ghostManager);

void func_800BC72C_B041C(s16 *arg0);
