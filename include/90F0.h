#pragma once

#include "5520.h"
#include "common.h"
#include "19E80.h"

typedef struct {
    /* 0x00 */ DataTable_19E80 *table;
    u8 _pad[0xC];
    /* 0x10 */ u16 index;
} func_8000A410_B010_arg;

typedef struct {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ u16 unk4;
    /* 0x06 */ u16 unk6;
} AnimationEntry;

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ s16 unk4;
} AnimationHeader;

typedef struct {
    /* 0x00 */ u8 pad0[0x8];
    /* 0x08 */ AnimationHeader *unk8;
    /* 0x0C */ AnimationEntry *unkC;
    /* 0x10 */ u16 unk10;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
} AnimationState;

typedef struct {
    /* 0x00 */ void *unk0;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ u8 unk6;
    /* 0x07 */ u8 unk7;
    /* 0x08 */ void *unk8;
    /* 0x0C */ void *unkC;
    /* 0x10 */ s16 unk10;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ u8 pad18[0x10];
    /* 0x28 */ void *unk28;
    /* 0x2C */ u8 pad2C[0x20];
} SpriteAssetState;

typedef struct {
} func_80009F5C_AB5C_arg;

typedef struct {
    /* 0x00 */ s8 unk0;
    /* 0x01 */ s8 unk1;
    /* 0x02 */ u8 pad2[2];
    /* 0x04 */ u16 unk4;
    /* 0x06 */ u8 pad6[2];
} SpriteEntry;

typedef struct {
    /* 0x00 */ u8 pad0[0x0C];
    /* 0x0C */ SpriteEntry *unkC;
    /* 0x10 */ u8 pad10[4];
    /* 0x14 */ s16 unk14;
    /* 0x16 */ u8 pad16[2];
    /* 0x18 */ void *unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ DataTable_19E80 *unk28;
    /* 0x2C */ u16 unk2C;
    /* 0x2E */ u8 unk2E;
    /* 0x2F */ u8 unk2F;
    /* 0x30 */ u8 pad30[0x10];
    /* 0x40 */ s32 unk40;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ u16 unk48;
    /* 0x4A */ u16 unk4A;
} SpriteState;

s32 func_80009E68_AA68(SpriteAssetState *arg0, s16 arg1);
void *func_80009EF8_AAF8(s16 arg0);
void func_80009F5C_AB5C(func_80009F5C_AB5C_arg **arg0);
void func_80009F90_AB90(void *arg0, s32 arg1, s32 arg2, s32 arg3);
s32 func_8000A030_AC30(void *arg0, s32 arg1);
void func_8000A13C_AD3C(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s16 arg7, u8 arg8);