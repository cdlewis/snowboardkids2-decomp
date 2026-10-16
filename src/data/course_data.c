#include "data/course_data.h"
#include "assets.h"
#include "common.h"
#include "data/asset_metadata.h"
#include "generated/course_asset_sizes.h"
#include "generated/modelpayload_asset_sizes.h"
#include "system/rom_loader.h"
#include "system/task_scheduler.h"

USE_ASSET(SUNNY_MOUNTAIN_COURSE_DISPLAY_LISTS);
USE_ASSET(SUNNY_MOUNTAIN_COURSE_MODEL_RESOURCES);

LevelConfig gLevelConfigs[] = {
#include "generated/course_definitions/level_configs.inc"
};

CompressedAssetMeta gCourseDataAssets[] = {
#include "generated/course_definitions/track_mesh_assets.inc"
};

CompressedAssetMeta gSpriteAssets[] = {
#include "generated/course_definitions/texture_table_assets.inc"
};

AssetMeta gUncompressedAssets[] = {
#include "generated/course_definitions/display_list_assets.inc"
};

CompressedAssetMeta gCompressedSegment2Assets[] = {
#include "generated/course_definitions/model_resource_assets.inc"
};

DisplayLists *gSkyDisplayListsByCourse[] = {
#include "generated/course_definitions/course_sky_display_lists.inc"
};

DisplayLists *gFogDisplayListsByCourse[] = {
#include "generated/course_definitions/course_fog_display_lists.inc"
};

LevelDisplayLists *gDisplayListTablesByCourse[] = {
#include "generated/course_definitions/course_display_list_tables.inc"
};

LevelConfig *getLevelConfig(s32 index) {
    return &gLevelConfigs[index];
}

void *loadCourseDataByIndex(s32 index) {
    return loadCompressedData(
        gCourseDataAssets[index].start,
        gCourseDataAssets[index].end,
        gCourseDataAssets[index].uncompressedSize
    );
}

void *loadSpriteAssetByIndex(s32 index) {
    return loadCompressedData(
        gSpriteAssets[index].start,
        gSpriteAssets[index].end,
        gSpriteAssets[index].uncompressedSize
    );
}

void *loadUncompressedAssetByIndex(s32 index) {
    return loadUncompressedData(gUncompressedAssets[index].start, gUncompressedAssets[index].end);
}

void *loadCompressedSegment2AssetByIndex(s32 index) {
    return loadCompressedData(
        gCompressedSegment2Assets[index].start,
        gCompressedSegment2Assets[index].end,
        gCompressedSegment2Assets[index].uncompressedSize
    );
}

DisplayLists *getSkyDisplayListsForCourse(s32 courseId) {
    return gSkyDisplayListsByCourse[courseId];
}

DisplayLists *getFogDisplayListsForCourse(s32 courseId) {
    return gFogDisplayListsByCourse[courseId];
}

LevelDisplayLists *getDisplayListTableForCourse(s32 courseId) {
    return gDisplayListTablesByCourse[courseId];
}

void *loadAsset_B7E70(void) {
    return loadUncompressedData(
        &PARTICLE_EFFECT_UNCOMPRESSED_GRAPHICS_ROM_START,
        &PARTICLE_EFFECT_UNCOMPRESSED_GRAPHICS_ROM_END
    );
}

void *loadAsset_216290(void) {
    return loadCompressedData(
        &PARTICLE_EFFECT_COMPRESSED_GRAPHICS_ROM_START,
        &PARTICLE_EFFECT_COMPRESSED_GRAPHICS_ROM_END,
        PARTICLE_EFFECT_COMPRESSED_GRAPHICS_DECOMPRESSED_SIZE
    );
}

void *loadAsset_34CB50(void) {
    return loadCompressedData(&hudSpriteAsset_ROM_START, &hudSpriteAsset_ROM_END, 0x5E28);
}

void *loadShootCrossSprites(void) {
    return loadCompressedData(&shootCrossSprites_ROM_START, &shootCrossSprites_ROM_END, 0xE08);
}

void *loadSpeedCrossSprites(void) {
    return loadCompressedData(&speedCrossSprites_ROM_START, &speedCrossSprites_ROM_END, 0x868);
}

void *loadTrickCrossSprites(void) {
    return loadCompressedData(&trickCrossSprites_ROM_START, &trickCrossSprites_ROM_END, 0xA88);
}

void *loadAsset_34F7E0(void) {
    return loadCompressedData(&UI_MENUS_GRAPHICS_ROM_START, &UI_MENUS_GRAPHICS_ROM_END, 0x438);
}
