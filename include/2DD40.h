#pragma once

#include "common.h"
#include "geometry.h"
#include "20F0.h"
#include "297B0.h"

typedef struct Func2E024Arg Func2E024Arg;
struct Func2E024Arg {
    /* 0x00 */ Func297D8Arg elements[2];          /* 2 * 0x64 = 0xC8 */
    /* 0xC8 */ void (*callback)(Func2E024Arg *);
    /* 0xCC */ u16 unkCC[2];
    /* 0xD0 */ u8 ctrl[2];
    /* 0xD2 */ u8 unkD2;
    /* 0xD3 */ u8 unkD3;
    /* 0xD4 */ u8 unkD4;
    /* 0xD5 */ u8 unkD5;
};

void func_8002D140_2DD40(Func2E024Arg *);
void func_8002EBB0_2F7B0(void *);
s32 func_8002ED30_2F930(void);
void func_8002ED40_2F940(s32 a0);