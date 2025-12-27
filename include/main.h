#pragma once

#include "6E840.h"
#include "common.h"
#include "gamestate.h"

typedef struct {
    struct {
        s8 padding[0xD5];
        s32 unkDA;
    } *parent;
    void *modelData;
    void *textureData;
    f32 primaryMatrix[8];
    s32 displayConfig;
    void *activeModel;
    void *activeTexture;
    void *animState;
    s8 pad3C[12];
    float secondaryMatrix[8];
    s32 secondaryConfig;
    void *secondaryModel;
    void *secondaryTexture;
    void *unk74;
    s8 pad78[12];
    s16 configIndex;
    u8 isDisposed;
    u8 isVisible;
} ModelEntity;

void func_80000460_1060(ModelEntity *arg0, ColorData *arg1, ColorData *arg2);

s32 initModelEntity(ModelEntity *entity, s16 index, void *arg2);

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
    s8 isDisposed;
} EffectState;

void freeEffectResources(EffectState *);

typedef struct {
    u8 padding[0x6];
    u16 unk6;
    s8 padding2[0x7F];
    s8 unk87;
} setModelRenderMode_arg;

void setModelRenderMode(setModelRenderMode_arg *arg0, s8 arg1);
