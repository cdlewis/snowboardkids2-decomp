#pragma once

#include "common.h"
#include "displaylist.h"
#include "geometry.h"

typedef struct {
    DisplayListObject base;
    void *unk3C;
    s32 unk40;
    s16 unk44;
    s16 unk46;
    s16 unk48;
    s16 unk4A;
    s16 unk4C;
    s16 unk4E;
    s16 unk50;
    s16 unk52;
    s16 unk54;
    s16 unk56;
    s16 unk58;
} ScrollingTextureState;

void enqueueScrollingTextureRender(u16 renderLayer, DisplayListObject *displayListObj);

extern DisplayLists D_80089520;

typedef struct {
    /* 0x00 */ DisplayListObject base;
    /* 0x3C */ void *vertexData;
    /* 0x40 */ s32 unk40;
    /* 0x44 */ union {
        u16 scrollU;
        u8 scrollU_bytes[2];
    } scrollUUnion;
    /* 0x46 */ union {
        u16 scrollV;
        u8 scrollV_bytes[2];
    } scrollVUnion;
    /* 0x48 */ u16 scrollUSpeed;
    /* 0x4A */ u16 scrollVSpeed;
    /* 0x4C */ s16 unk4C;
    /* 0x4E */ s16 unk4E;
    /* 0x50 */ s16 unk50;
    /* 0x52 */ s16 unk52;
    /* 0x54 */ u8 initialMatrix[0x20];
    /* 0x74 */ u16 renderLayer;
    /* 0x76 */ s16 colorIndex;
    /* 0x78 */ union {
        s32 currentScale;
        s16 currentScale_shorts[2];
    } currentScale_union;
    /* 0x7C */ s32 targetScale;
    /* 0x80 */ s32 alpha;
    /* 0x84 */ s8 animState;
} TrickBurstEffectTask;

void enqueueTiledTextureRender(s32 arg0, TrickBurstEffectTask *arg1);
