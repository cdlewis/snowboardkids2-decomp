#pragma once

#include "common.h"
#include "displaylist.h"
#include "geometry.h"

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
} ScrollingTextureDisplayLists;

typedef struct {
    Transform3D unk0;
    ScrollingTextureDisplayLists *unk20;
    void *unk24;
    void *unk28;
    s32 unk2C;
    s32 unk30;
    u8 padding2[0x8];
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

typedef struct {
    /* 0x00 */ u8 _pad0[0x20];
    /* 0x20 */ ScrollingTextureDisplayLists *unk20;
    /* 0x24 */ void *unk24;
    /* 0x28 */ void *unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ u8 _pad34[0x3B - 0x34];
    /* 0x3B */ u8 unk3B;
    /* 0x3C */ void *unk3C;
    /* 0x40 */ s32 unk40;
    /* 0x44 */ union {
        u16 unk44;
        u8 unk44_bytes[2];
    } unk44_union;
    /* 0x46 */ union {
        u16 unk46;
        u8 unk46_bytes[2];
    } unk46_union;
    /* 0x48 */ u16 unk48;
    /* 0x4A */ u16 unk4A;
    /* 0x4C */ s16 unk4C;
    /* 0x4E */ s16 unk4E;
    /* 0x50 */ s16 unk50;
    /* 0x52 */ s16 unk52;
    /* 0x54 */ u8 unk54[0x20];
    /* 0x74 */ u16 unk74;
    /* 0x76 */ s16 unk76;
    /* 0x78 */ union {
        s32 unk78;
        s16 unk78_shorts[2];
    } unk78_union;
    /* 0x7C */ s32 unk7C;
    /* 0x80 */ s32 unk80;
    /* 0x84 */ s8 unk84;
} func_800B5500_Task;

void enqueueTiledTextureRender(s32 arg0, func_800B5500_Task *arg1);
