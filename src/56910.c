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

extern s32 D_B7E70;
extern s32 D_B9C40;
extern s32 D_80091550_92150[];
extern s32 D_80091510_92110[];
extern Asset_56910 gCourseDataAssets[];
extern Asset_56910 gSpriteAssets[];
extern Asset_56910 D_80091450_92050[];
extern AssetWithoutSize gUncompressedAssets[];
extern func_80055E68_56A68_result *D_80091590_92190[];
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

void *func_80055DF8_569F8(s32 index) {
    return loadCompressedData(D_80091450_92050[index].start, D_80091450_92050[index].end, D_80091450_92050[index].size);
}

s32 func_80055E40_56A40(s32 arg0) {
    return D_80091510_92110[arg0];
}

s32 func_80055E54_56A54(s32 arg0) {
    return D_80091550_92150[arg0];
}

func_80055E68_56A68_result *func_80055E68_56A68(s32 index) {
    return D_80091590_92190[index];
}

void *loadAsset_B7E70(void) {
    return loadUncompressedData(&D_B7E70, &D_B9C40);
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