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

void *advanceLinearAlloc(s32 size);

void func_8006E02C_6EC2C(void);

void func_8006E000_6EC00(s32 arg0);

typedef struct {
    u8 padding[0xBC];
    s8 unkBC;
    s8 unkBD;
    s8 unkBE;
} func_8006FE28_70A28_arg;

void func_8006FE28_70A28(func_8006FE28_70A28_arg *arg0, s8 arg1, s8 arg2, s8 arg3);

typedef struct {
    u8 padding[0xC8];
    s16 unkC8;
    s16 unkCA;
    u8 padding2[0x104];
    f32 unk1D0;
} func_8006F9BC_705BC_arg;

void func_8006F9BC_705BC(func_8006F9BC_705BC_arg *arg0, f32 arg1, f32 arg2);