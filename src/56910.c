#include "56910.h"
#include "common.h"
#include "rom_loader.h"
#include "task_scheduler.h"

USE_ASSET(_216290);
USE_ASSET(_34F7E0);
USE_ASSET(_34CB50);
USE_ASSET(_34F9A0);
USE_ASSET(_350140);
USE_ASSET(_3505F0);
USE_ASSET(_B7E70);

extern s32 gSkyDisplayLists2[];
extern s32 gSkyDisplayLists1[];
extern Asset_56910 gCourseDataAssets[];
extern Asset_56910 gSpriteAssets[];
extern Asset_56910 gCompressedSegment2Assets[];
extern AssetWithoutSize gUncompressedAssets[];
extern LevelDisplayLists *gSkyDisplayLists3[];
extern LevelConfig gLevelConfigs[];

LevelConfig *getLevelConfig(s32 index) {
    return &gLevelConfigs[index];
}

void *loadCourseDataByIndex(s32 index) {
    return loadCompressedData(
        gCourseDataAssets[index].start,
        gCourseDataAssets[index].end,
        gCourseDataAssets[index].size
    );
}

void *loadSpriteAssetByIndex(s32 index) {
    return loadCompressedData(gSpriteAssets[index].start, gSpriteAssets[index].end, gSpriteAssets[index].size);
}

void *loadUncompressedAssetByIndex(s32 index) {
    return loadUncompressedData(gUncompressedAssets[index].start, gUncompressedAssets[index].end);
}

void *loadCompressedSegment2AssetByIndex(s32 index) {
    return loadCompressedData(
        gCompressedSegment2Assets[index].start,
        gCompressedSegment2Assets[index].end,
        gCompressedSegment2Assets[index].size
    );
}

s32 getSkyDisplayLists1ByIndex(s32 arg0) {
    return gSkyDisplayLists1[arg0];
}

s32 getSkyDisplayLists2ByIndex(s32 arg0) {
    return gSkyDisplayLists2[arg0];
}

LevelDisplayLists *getSkyDisplayLists3ByIndex(s32 index) {
    return gSkyDisplayLists3[index];
}

void *loadAsset_B7E70(void) {
    return loadUncompressedData(&_B7E70_ROM_START, &_B7E70_ROM_END);
}

void *loadAsset_216290(void) {
    return loadCompressedData(&_216290_ROM_START, &_216290_ROM_END, 0x5740);
}

void *loadAsset_34CB50(void) {
    return loadCompressedData(&_34CB50_ROM_START, &_34CB50_ROM_END, 0x5E28);
}

void *loadAsset_34F9A0(void) {
    return loadCompressedData(&_34F9A0_ROM_START, &_34F9A0_ROM_END, 0xE08);
}

void *loadAsset_350140(void) {
    return loadCompressedData(&_350140_ROM_START, &_350140_ROM_END, 0x868);
}

void *loadAsset_3505F0(void) {
    return loadCompressedData(&_3505F0_ROM_START, &_3505F0_ROM_END, 0xA88);
}

void *loadAsset_34F7E0(void) {
    return loadCompressedData(&_34F7E0_ROM_START, &_34F7E0_ROM_END, 0x438);
}