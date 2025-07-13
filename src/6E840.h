#include "common.h"

typedef struct {
    u8 padding[0xBF];
    s8 unkBF;
    s8 unkC0;
    s8 unkC1;
} func_8006FDA0_709A0_arg;

void func_8006FDA0_709A0(func_8006FDA0_709A0_arg *arg0, u8 arg1, u8 arg2);

void debugEnqueueCallback(u16 index, u8 arg1, void *arg2, void *arg3);

void *arenaAlloc16(s32 size);

void *advanceLinearAlloc(s32 arg0);