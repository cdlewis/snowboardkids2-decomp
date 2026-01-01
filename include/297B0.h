#pragma once

#include "20F0.h"

typedef struct {
    /* 0x00 */ SceneModel *model;
    /* 0x04 */ Transform3D matrix;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ u8 pad28[0x4];
    /* 0x2C */ u16 rotation;
    /* 0x2E */ u16 unk2E;
    /* 0x30 */ u16 unk30;
    /* 0x32 */ u8 pad2E[0x37-0x32];
    /* 0x37 */ u8 unk37;
    /* 0x38 */ u8 padding_38[0x3A-0x38];
    /* 0x3A */ u8 unk3A;
    /* 0x3B */ u8 pad3B[0x1];
    /* 0x3C */ void *callback;
    /* 0x40 */ s32 unk40;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ s32 unk48;
    /* 0x4C */ u8 pad4C[0x4];
    /* 0x50 */ s16 unk50;
    /* 0x52 */ s16 unk52;
    /* 0x54 */ u8 pad52[0x2];
    /* 0x56 */ u16 unk56;
    /* 0x58 */ u16 unk58;
    /* 0x5A */ s16 unk5A;
    /* 0x5C */ u8 unk5C;
    /* 0x5D */ u8 unk5D;
    /* 0x5E */ u8 unk5E;
    /* 0x5F */ u8 unk5F;
    /* 0x60 */ u8 pad60[0x1];
    /* 0x61 */ u8 unk61;
    /* 0x62 */ s8 unk62;
    /* 0x63 */ u8 pad63;
} Func297D8Arg; /* size = 0x64 */

typedef struct {
    /* 0x00 */ SceneModel *unk0;
    /* 0x04 */ applyTransformToModel_arg1 unk4;
    /* 0x28 */ u8 pad28[0x28];
    /* 0x50 */ s16 unk50;
} func_8002A290_2AE90_arg;

void func_8002A2D0_2AED0(Func297D8Arg *);
void func_8002A290_2AE90(Func297D8Arg *);
void updateStoryMapNpcJump(Func297D8Arg *);
void updateStoryMapNpcLookAround(Func297D8Arg *);
void updateStoryMapNpcNod(Func297D8Arg *);
void updateStoryMapNpcWave(Func297D8Arg *);
void updateStoryMapNpcFloatEffect(Func297D8Arg *);
void updateStoryMapNpcThinkEffect(Func297D8Arg *);
void updateStoryMapNpcStretch(Func297D8Arg *);
void updateStoryMapNpcTalk(Func297D8Arg *);
void func_8002A0DC_2ACDC(Func297D8Arg *);
void updateStoryMapNpcIdle(Func297D8Arg *);
void func_8002A044_2AC44(Func297D8Arg *);