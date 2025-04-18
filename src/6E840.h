#include "common.h"

typedef struct {
    u8 padding[0xBF];
    s8 unkBF;
    s8 unkC0;
    s8 unkC1;
} func_8006FDA0_709A0_arg;

void func_8006FDA0_709A0(func_8006FDA0_709A0_arg *arg0, u8 arg1, u8 arg2);

typedef struct {
    u8 padding[0x1C];
    s32 unk1C;
    u8 padding2[0x10];
    s32 unk30;
} debugEnqueueCallback_arg3;

void debugEnqueueCallback(u16 index, u8 arg1, void *arg2, debugEnqueueCallback_arg3 *arg3);
