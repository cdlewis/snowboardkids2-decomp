#pragma once

#include "1DFAA0.h"
#include "6E840.h"
#include "common.h"

typedef struct {
    u8 padding[0xFF5];
    u8 unkFF5;
} func_800B2270_1DF320_arg;

typedef struct {
    s8 unk0;
    s8 unk1;
    s8 unk2;
} func_800B228C_1DF33C_arg;

typedef struct {
    s8 unk0;
    s8 unk1;
    u8 unk2;
} func_800B2300_1DF3B0_arg;

void noop_1DF310(void);
s32 returnZero_1DF318(void);
void func_800B2270_1DF320(u8 *arg0, func_800B2270_1DF320_arg *arg1);
void noop_1DF32C(void);
s32 returnZero_1DF334(void);
void func_800B228C_1DF33C(func_800B228C_1DF33C_arg *arg0, Node_70B00 *arg1);
s32 returnZero_1DF398(void);
void noop_1DF3A0(void);
s32 returnZero_1DF3A8(void);
void func_800B2300_1DF3B0(func_800B2300_1DF3B0_arg *, Node_70B00 *);
s32 returnZero_1DF40C(void);
void noop_1DF414(void);
s32 returnZero_1DF41C(void);
void func_800B2374_1DF424(func_800B2A24_1DFAD4_arg_item *arg0, func_800B2A24_1DFAD4_arg *arg1, s8 arg2);
void func_800B2404_1DF4B4(void);
s32 returnZero_1DF590(void);
void noop_1DF598(void);
s32 returnZero_1DF5A0(void);
typedef struct {
    s8 unk0;
} func_800B24F8_1DF5A8_arg;
void func_800B24F8_1DF5A8(func_800B24F8_1DF5A8_arg *arg0);
void noop_1DF5C4(void);
s32 returnZero_1DF5CC(void);
typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
} func_800B2524_1DF5D4_arg0;

typedef struct {
    u8 padding[0xFF8];
    s32 unkFF8;
} func_800B2524_1DF5D4_arg1;
void func_800B2524_1DF5D4(func_800B2524_1DF5D4_arg0 *arg0, func_800B2524_1DF5D4_arg1 *arg1);
void noop_1DF640(void);
s32 returnZero_1DF648(void);
void func_800B25A0_1DF650(u16 *arg0, func_800B29F0_1DFAA0_arg *arg1);
void noop_1DF6BC(void);
s32 returnZero_1DF6C4(void);
void func_800B261C_1DF6CC(void);
void func_800B2700_1DF7B0(func_800B2A24_1DFAD4_arg *arg0, s8 arg1);
s32 returnZero_1DF834(void);
void noop_1DF83C(void);
s32 returnZero_1DF844(void);
typedef struct {
    s8 unk0;
    s8 unk1;
    s8 unk2;
    s8 unk3;
    s8 unk4;
    s8 unk5;
    s8 unk6;
} func_800B279C_1DF84C_arg;
void func_800B279C_1DF84C(func_800B279C_1DF84C_arg *arg0, Node_70B00 *arg1);
s32 returnZero_1DF89C(void);