#pragma once

#include "common.h"
#include "geometry.h"

typedef struct {
    /* 0x00 */ void *unk0;
    /* 0x04 */ Transform3D unk4;
    /* 0x24 */ u8 pad24[0xC];
    /* 0x30 */ u16 unk30;
    /* 0x32 */ u8 pad32[0x96];
    /* 0xC8 */ void *unkC8;
    /* 0xCC */ u8 padCC[0x7];
    /* 0xD3 */ u8 unkD3;
} Func8002BEF4Arg;

void func_8002BCF0_2C8F0(void *arg0);

void func_8002BEF4_2CAF4(Func8002BEF4Arg *arg0);
