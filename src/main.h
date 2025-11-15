#pragma once

#include "6E840.h"
#include "common.h"
#include "gamestate.h"

typedef struct {
    struct {
        s8 padding[0xD5];
        s32 unkDA;
    } *unk0;
    void *unk4;
    void *unk8;
    f32 unkC[8];
    s32 unk2C;
    void *unk30;
    void *unk34;
    void *unk38;
    s8 pad3C[12];
    float unk48[8];
    s32 unk68;
    void *unk6C;
    void *unk70;
    void *unk74;
    s8 pad78[12];
    s16 unk84;
    u8 unk86;
    u8 unk87;
} func_8000056C_116C_arg;

void func_80000460_1060(func_8000056C_116C_arg *arg0, ColorData *arg1, ColorData *arg2);

s32 func_8000056C_116C(func_8000056C_116C_arg *entity, s16 index, void *arg2);

typedef struct {
    struct {
        u8 padding[0x16];
        u16 unk16;
    } *unk0;
    u8 padding[0x8];
    DisplayListObject unkC;
    s32 unk48;
    u8 padding4[0x20];
    s32 unk6C;
    u8 padding5[0x17];
    s8 unk87;
} func_80000760_1360_arg;

void func_80000760_1360(func_80000760_1360_arg *);

typedef struct {
    s32 unk0;
    void *unk4;
    void *unk8;
    u8 padding[0x7A];
    s8 unk86;
} func_80000710_1310_arg;

void func_80000710_1310(func_80000710_1310_arg *);

typedef struct {
    char padding[0x87];
    char unk87;
} setModelRenderMode_arg;

void setModelRenderMode(setModelRenderMode_arg *arg0, s8 arg1);
