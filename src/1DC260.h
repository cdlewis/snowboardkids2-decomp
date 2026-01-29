#pragma once

#include "10AD0.h"
#include "6E840.h"
#include "common.h"

typedef struct {
    s16 x;
    s16 y;
    void *asset;
    s16 spriteIndex;
    s16 alpha;
    s8 unkC;
    s8 unkD;
    u8 pad[2];
} SpriteElement16;

typedef struct {
    s8 unk0;
    u8 padding[1];
    s16 unk2;
    s32 unk4;
    ViewportNode unk8;
    ViewportNode unk1E0;
    ViewportNode unk3B8;
    ViewportNode unk590;
    ViewportNode unk768;
    s16 unk940;
    s16 unk942;
    s16 unk944;
    void *unk948;
    s8 unk94C;
    u8 padding8[0xF];
    void *unk95C;
    void *unk960;
    void *unk964;
    void *unk968;
    s16 unk96C;
    s16 unk96E;
    u8 padding9A[0x4];
    void *unk974;
    s16 unk978;
    s16 unk97A;
    u8 unk97C;
    u8 padding9B[0x3];
    s16 unk980;
    s16 unk982;
    void *unk984;
    void *unk988;
    s16 unk98C;
    s16 unk98E;
    u8 unk990;
    u8 padding9C[0x3];
    void *unk994;
    void *unk998;
    void *unk99C;
    s16 unk9A0;
    u8 padding9D[0x2];
    s16 unk9A4;
    s16 unk9A6;
    u8 padding9E[0x4];
    s16 unk9AC;
    s16 unk9AE;
    s16 unk9B0;
    s16 unk9B2;
    u8 padding9F[0x4];
    void *unk9B8[6];
    u8 padding11[0x41C];
    void *volatile cornerDecorationAsset;
    s32 leftCornerAlpha;
    s32 leftCornerFadeSpeed;
    s32 rightCornerAlpha;
    s32 rightCornerFadeSpeed;
    SpriteElement16 leftTopCornerSprite;
    SpriteElement16 leftBottomCornerSprite;
    SpriteElement16 rightTopCornerSprite;
    SpriteElement16 rightBottomCornerSprite;
    s16 unkE40;
    s16 unkE42;
    ColorData unkE44[3];
    u8 paddingE5C[4];
    s16 unkE60;
} func_80003EE0_4AE0_task_memory;

void initCreditsCornerDecorationSprites(void *arg0);
void updateCreditsCornerDecorationSprites(void *arg0);
