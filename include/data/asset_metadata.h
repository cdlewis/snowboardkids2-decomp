#pragma once

#include "graphics/displaylist.h"
#include "system/memory_allocator.h"

typedef struct {
    void *start;
    void *end;
} AssetMeta;

typedef struct {
    void *start;
    void *end;
    s32 uncompressedSize;
} CompressedAssetMeta;

extern DisplayLists gDebugDisplayConfig;
extern DisplayLists gCostumeSlotDisplayLists[18];
extern DisplayLists gCrazyJunglePushZoneDisplayLists[2];
extern DisplayLists gCrazyJungleBossPushZoneDisplayLists[2];
extern DisplayLists gHauntedHousePushZoneDisplayLists[1];
extern DisplayLists gIceLandPushZoneDisplayLists[1];
extern DisplayLists gIceLandBossPushZoneDisplayLists[1];

void *load_3ECE40(void);
void *loadAssetByIndex_94F90(s16 groupIndex, s16 pairIndex);
void *loadAssetByIndex_95200(s16 groupIndex, s16 pairIndex);
DisplayLists *loadAssetByIndex_95380(s16 groupIndex, s16 pairIndex);
DisplayLists *loadAssetByIndex_953B0(s16 groupIndex, s16 pairIndex);
void *loadAssetByIndex_95470(s32 index);
void *loadAssetByIndex_95500(s16 index);
void *loadAssetByIndex_95590(s16 index);
void *loadAssetByIndex_95668(s16 index);
DisplayLists *loadAssetByIndex_95728(s16 index);
void *loadAssetByIndex_953E0(s16 index);

MemoryAllocatorNode *getAssetDataDirect(s16 groupIndex, s16 entityIndex);
