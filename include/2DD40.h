#pragma once

#include "common.h"
#include "geometry.h"
#include "20F0.h"

typedef struct Func2E024Arg Func2E024Arg;
struct Func2E024Arg {
    /* 0x00 */ SceneModel *model;
    /* 0x04 */ Transform3D unk4;
    /* 0x24 */ u8 pad24[0x6];
    /* 0x2A */ u16 unk2A;
    /* 0x2C */ u16 unk2C;
    /* 0x2E */ u8 pad2E[0x12];
    /* 0x40 */ s32 unk40;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ s32 unk48;
    /* 0x4C */ u8 pad4C[0x4];
    /* 0x50 */ s16 unk50;
    /* 0x52 */ u8 pad52[0x8];
    /* 0x5A */ u16 unk5A;
    /* 0x5C */ u8 pad5C[0x2];
    /* 0x5E */ u8 unk5E;
    /* 0x5F */ u8 pad5F[0x3];
    /* 0x62 */ s8 unk62;
    /* 0x63 */ u8 pad63[0x1];
    /* 0x64 */ SceneModel *unk64;
    /* 0x68 */ u8 pad68[0x14];
    /* 0x7C */ s32 unk7C;
    /* 0x80 */ u8 pad80[0x4];
    /* 0x84 */ s32 unk84;
    /* 0x88 */ u8 pad88[0x1C];
    /* 0xA4 */ s32 unkA4;
    /* 0xA8 */ s32 unkA8;
    /* 0xAC */ s32 unkAC;
    /* 0xB0 */ u8 padB0[0x4];
    /* 0xB4 */ s16 unkB4;
    /* 0xB6 */ u8 padB6[0x8];
    /* 0xBE */ s16 unkBE;
    /* 0xC0 */ u8 padC0[0x2];
    /* 0xC2 */ u8 unkC2;
    /* 0xC3 */ u8 padC3[0x3];
    /* 0xC6 */ s8 unkC6;
    /* 0xC7 */ u8 padC7[0x1];
    /* 0xC8 */ void (*unkC8)(Func2E024Arg *);
    /* 0xCC */ s16 unkCC[2];
    /* 0xD0 */ u8 padD0[0x2];
    /* 0xD2 */ u8 unkD2;
    /* 0xD3 */ u8 padD3[0x2];
    /* 0xD5 */ u8 unkD5;
};

void func_8002D140_2DD40(Func2E024Arg *);
void func_8002EBB0_2F7B0(void *);
s32 func_8002ED30_2F930(void);
void func_8002ED40_2F940(s32 a0);