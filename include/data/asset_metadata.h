#pragma once

#include "system/memory_allocator.h"

typedef struct {
    void *asset1;
    void *asset2;
    void *asset3;
    void *asset4;
} UnknownAssetStruct;

extern s32 D_80094CC0_958C0[4];
extern s32 D_80094CD0_958D0[4];
extern s32 D_80094CE0_958E0[48];
extern s32 D_80094DB0_959B0[4];
extern s32 D_80094DC0_959C0[4];
extern s32 D_80094E20_95A20[44];
extern s32 D_80094ED0_95AD0[4];
extern s32 D_80094F00_95B00[4];
extern s32 D_80094F10_95B10[40];
extern s32 D_80094FB0_95BB0[4];
extern s32 D_80094FE0_95BE0[4];
extern s32 D_80094FF0_95BF0[40];
extern s32 D_80095090_95C90[4];
extern s32 D_800950A0_95CA0[4];
extern s32 D_800950C0_95CC0[52];
extern s32 D_80095190_95D90[4];
extern s32 D_800951A0_95DA0[4];
extern s32 D_800951B0_95DB0[40];
extern s32 D_80095250_95E50[4];
extern s32 D_80095260_95E60[4];
extern s32 D_80095270_95E70[52];
extern s32 D_80095390_95F90[4];
extern s32 D_800953A0_95FA0[4];
extern s32 D_800953B0_95FB0[36];
extern s32 D_80095490_96090[4];
extern s32 D_800954A0_960A0[4];
extern s32 D_800954B0_960B0[64];
extern s32 D_800955F0_961F0[4];
extern s32 D_80095600_96200[4];
extern s32 D_80095610_96210[40];
extern s32 D_800956C0_962C0[4];
extern s32 D_800956D0_962D0[40];
extern s32 D_80095780_96380[4];
extern s32 D_80095790_96390[4];
extern s32 D_800957A0_963A0[36];
extern s32 D_80095840_96440[4];
extern s32 D_80095850_96450[4];
extern s32 D_80095870_96470[4];
extern s32 D_80095880_96480[40];
extern s32 D_80095920_96520[4];
extern s32 D_80095940_96540[4];
extern s32 D_80095950_96550[40];
extern s32 D_800959F0_965F0[4];
extern s32 D_80095A00_96600[4];
extern s32 D_80095A10_96610[4];

void *load_3ECE40(void);
void *loadAssetByIndex_94F90(s16 groupIndex, s16 pairIndex);
void *loadAssetByIndex_95200(s16 groupIndex, s16 pairIndex);
UnknownAssetStruct *loadAssetByIndex_95380(s16 groupIndex, s16 pairIndex);
s32 loadAssetByIndex_953B0(s16 groupIndex, s16 pairIndex);
void *loadAssetByIndex_95470(s32 index);
void *loadAssetByIndex_95500(s16 index);
void *loadAssetByIndex_95590(s16 index);
void *loadAssetByIndex_95668(s16 index);
void *loadAssetByIndex_95728(s16 index);
void *loadAssetByIndex_953E0(s16 index);

MemoryAllocatorNode *getAssetDataDirect(s16 groupIndex, s16 entityIndex);
