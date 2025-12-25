#pragma once

#include "20F0.h"
#include "common.h"
#include "main.h"

typedef struct {
    u8 padding[0x2];
    s16 unk2;
    u8 padding2[0x34];
    void *unk38;
} Struct163F8;

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
    u8 unkA;
    u8 unkB;
    u8 unkC;
    u8 unkD;
    u8 unkE;
} Struct16728;

typedef struct {
    SceneModel *unk0;   // 0x00
    Mat3x3Padded unk4; // 0x04 - 0x20 bytes of transform data
    s32 unk24;          // 0x24
    u16 *unk28;         // 0x28 - pointer to u16 animation data
    u16 unk2C;          // 0x2C - current animation state
    u8 unk2E;           // 0x2E
    u8 unk2F;           // 0x2F - index into transform array
    u8 _pad30;          // 0x30
    u8 unk31;           // 0x31
    u8 unk32;           // 0x32
} Struct16B68;

void func_80016434_17034(Struct163F8 *arg0);
void func_80016488_17088(void);
void func_80016728_17328(Struct16728 *arg0);
void func_80016860_17460(func_8000056C_116C_arg *arg0);
void func_800168F4_174F4(Struct16B68 *arg0);