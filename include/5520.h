#pragma once

#include "common.h"
#include "geometry.h"

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
} func_80004FF8_5BF8_arg1_unk20;

typedef struct {
    Mat3x3Padded unk0;
    func_80004FF8_5BF8_arg1_unk20 *unk20;
    void *unk24;
    void *unk28;
    s32 unk2C;
    s32 unk30;
    u8 padding2[0x8];
    void *unk3C;
    s32 unk40;
    s16 unk44;
    s16 unk46;
    s16 unk48;
    s16 unk4A;
    s16 unk4C;
    s16 unk4E;
    s16 unk50;
    s16 unk52;
    s16 unk54;
    s16 unk56;
    s16 unk58;
} func_80004FF8_5BF8_arg1;

void func_80004FF8_5BF8(u16 arg0, func_80004FF8_5BF8_arg1 *arg1);

typedef struct {
    /* 0x00 */ u8 _pad0[0x20];
    /* 0x20 */ struct {
        s32 unk0;
        s32 unk4;
        s32 unk8;
        s32 unkC;
    } *unk20;
    /* 0x24 */ u8 _pad24[0x30-0x24];
    /* 0x34 */ s32 unk30;
    /* 0x38 */ u8 _pad34[0x3B-0x34];
    /* 0x3B */ s8 unk3B;
    /* 0x3C */ u8 _pad3C[8];
    /* 0x44 */ u16 unk44;
    /* 0x46 */ u16 unk46;
    /* 0x48 */ u16 unk48;
    /* 0x4A */ u16 unk4A;
    /* 0x4C */ u8 _pad4C[8];
    /* 0x54 */ u8 unk54[0x20];
    /* 0x74 */ u16 unk74;
    /* 0x76 */ s16 unk76;
    /* 0x78 */ s32 unk78;
    /* 0x7C */ s32 unk7C;
    /* 0x80 */ s32 unk80;
    /* 0x84 */ s8 unk84;
} func_800B5500_Task;

void func_8000576C_636C(s32 arg0, func_800B5500_Task *arg1);
