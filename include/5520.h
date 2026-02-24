#pragma once

#include "common.h"
#include "displaylist.h"
#include "geometry.h"
#include "20F0.h"

typedef struct {
    DisplayListObject base;
    void *textureTable;
    s32 unk40;
    s16 tileScrollU;
    s16 tileScrollV;
    s16 unk48;
    s16 unk4A;
    s16 textureIndex;
    s16 paletteMode;
    s16 paletteIndex;
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

typedef struct {
    /* 0x00 */ void *model;
    /* 0x04 */ u8 opaqueMatrix[0x20];
    /* 0x24 */ void *opaqueSettings;
    /* 0x28 */ void *opaqueDisplayList;
    /* 0x2C */ void *opaqueVertexData;
    /* 0x30 */ s32 opaqueFlag;
    /* 0x34 */ u8 pad34[0xC];
    /* 0x40 */ u8 transparentMatrix[0x20];
    /* 0x60 */ void *transparentSettings;
    /* 0x64 */ void *transparentDisplayList;
    /* 0x68 */ void *transparentVertexData;
    /* 0x6C */ s32 transparentFlag;
    /* 0x70 */ u8 pad70[0xC];
    /* 0x7C */ s32 scale;
    /* 0x80 */ s16 rotationY;
    /* 0x82 */ s8 transparentEnabled;
    /* 0x83 */ s8 overlayAnimState;
    /* 0x84 */ s32 overlayScaleX;
    /* 0x88 */ s32 overlayScaleY;
    /* 0x8C */ s32 oscillationOffset;
    /* 0x90 */ s16 oscillationAngle;
    /* 0x92 */ s8 oscillationEnabled;
    /* 0x93 */ s8 scaleAnimState;
    /* 0x94 */ u8 overlayMatrix[0x20];
    /* 0xB4 */ void *overlaySettings;
    /* 0xB8 */ void *overlayDisplayList;
    /* 0xBC */ void *overlayVertexData;
    /* 0xC0 */ s32 overlayFlag;
    /* 0xC4 */ u8 padC4[0xC];
    /* 0xD0 */ void *soundData;
    /* 0xD4 */ u8 padD4[0x4];
    /* 0xD8 */ s16 scrollU;
    /* 0xDA */ s16 scrollV;
    /* 0xDC */ s16 scrollSpeedU;
    /* 0xDE */ s16 scrollSpeedV;
    /* 0xE0 */ s16 unkE0;
    /* 0xE2 */ s16 unkE2;
    /* 0xE4 */ s16 unkE4;
} RotatingLogoState;

void initializeRotatingLogo(RotatingLogoState *state);
