#pragma once

#include "common.h"

typedef struct {
    s16 x;
    s16 y;
    s16 palette;
    s16 unk06;
    void *text;
} StatTextLabel;

typedef struct {
    void *progressBarAsset;
    s16 priceLabelX;
    s16 priceLabelY;
    void *spriteAsset;
    s16 priceSpriteIndex;
    s16 unk0E;
    StatTextLabel statLabels[3];
    s16 progressBarX;
    s16 progressBarY;
    char statBuffer1[3];
    char statBuffer2[3];
    char statBuffer3[3];
} ItemStatsDisplay;

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
    s8 unkA;
    s8 unkB;
} func_800308FC_314FC_arg;

typedef struct {
    func_800308FC_314FC_arg items[4];
    s16 unk30;
    s16 unk32;
    void *unk34;
    void *unk38;
    s16 unk3C;
    s16 unk3E;
    s8 unk40;
} func_80030A00_31600_arg;

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
    s16 unkA;
    s8 unkC;
    u8 unkD;
    s8 unkE;
    s8 unkF;
} func_8002FA9C_3069C_item;

typedef struct {
    func_8002FA9C_3069C_item items[2];
    s8 unk20;
} func_8002FA9C_3069C_arg;

void func_8002FA9C_3069C(func_8002FA9C_3069C_arg *arg0);
void func_800304B8_310B8(func_800308FC_314FC_arg *arg0);
void func_80030694_31294(ItemStatsDisplay *arg0);
void func_800308C4_314C4(ItemStatsDisplay *arg0);
void func_800308FC_314FC(func_800308FC_314FC_arg *arg0);
void func_80030A00_31600(func_80030A00_31600_arg *arg0);
