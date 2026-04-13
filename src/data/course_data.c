#include "data/course_data.h"
#include "assets.h"
#include "common.h"
#include "data/asset_metadata.h"
#include "system/rom_loader.h"
#include "system/task_scheduler.h"

USE_ASSET(_B9C40);
USE_ASSET(_218F00);

LevelConfig gLevelConfigs[] = {
    { .shortcutPosX = 0x19654B2B,
     .shortcutPosZ = 0x03439FEC,
     .yawOffset = 0x0000,
     .padding = 0,
     .spawnPos = { .x = 0xFFFF538C, .y = 0x2FFFEC47, .z = 0x304EFAA0 },
     .lightColors = { .r = 0xFF, .g = 0xFF, .b = 0xFF, .pad = 0x00, .r2 = 0x00, .g2 = 0x50, .b2 = 0x50, .pad2 = 0x00 },
     .fogColors = { .r = 0x8A, .g = 0x70, .b = 0xEA, .pad = 0x00, .r2 = 0x50, .g2 = 0x70, .b2 = 0xF0, .pad2 = 0x00 },
     .musicTrack = 0x000C,
     .padding2 = { 0 } },
    { .shortcutPosX = 0xD3C19C2E,
     .shortcutPosZ = 0xCCDD0443,
     .yawOffset = 0x0000,
     .padding = 0,
     .spawnPos = { .x = 0xFFB3CA5D, .y = 0x2FFFDB4F, .z = 0x2FFEA951 },
     .lightColors = { .r = 0xFF, .g = 0xFF, .b = 0xFF, .pad = 0x00, .r2 = 0x00, .g2 = 0x50, .b2 = 0x50, .pad2 = 0x00 },
     .fogColors = { .r = 0xC0, .g = 0xC0, .b = 0xC0, .pad = 0x00, .r2 = 0x20, .g2 = 0x60, .b2 = 0xAA, .pad2 = 0x00 },
     .musicTrack = 0x0013,
     .padding2 = { 0 } },
    { .shortcutPosX = 0xC509752E,
     .shortcutPosZ = 0xD846D91D,
     .yawOffset = 0xF800,
     .padding = 0,
     .spawnPos = { .x = 0x0005E526, .y = 0x3000B20C, .z = 0x304DA3E1 },
     .lightColors = { .r = 0xFF, .g = 0xFF, .b = 0xFF, .pad = 0x00, .r2 = 0x00, .g2 = 0x50, .b2 = 0x50, .pad2 = 0x00 },
     .fogColors = { .r = 0x60, .g = 0x70, .b = 0x80, .pad = 0x00, .r2 = 0x00, .g2 = 0x80, .b2 = 0xFF, .pad2 = 0x00 },
     .musicTrack = 0x0011,
     .padding2 = { 0 } },
    { .shortcutPosX = 0xC509752E,
     .shortcutPosZ = 0xD846D91D,
     .yawOffset = 0xF800,
     .padding = 0,
     .spawnPos = { .x = 0x0005E526, .y = 0x3000B20C, .z = 0x304DA3E1 },
     .lightColors = { .r = 0xFF, .g = 0xFF, .b = 0x80, .pad = 0x00, .r2 = 0x00, .g2 = 0x50, .b2 = 0x50, .pad2 = 0x00 },
     .fogColors = { .r = 0x40, .g = 0x60, .b = 0xA0, .pad = 0x00, .r2 = 0x00, .g2 = 0x00, .b2 = 0x40, .pad2 = 0x00 },
     .musicTrack = 0x001F,
     .padding2 = { 0 } },
    { .shortcutPosX = 0xED1B13F8,
     .shortcutPosZ = 0xCADE7A1A,
     .yawOffset = 0xF800,
     .padding = 0,
     .spawnPos = { .x = 0x004D695A, .y = 0x300018E6, .z = 0x304430A2 },
     .lightColors = { .r = 0xFF, .g = 0xFF, .b = 0xFF, .pad = 0x00, .r2 = 0x00, .g2 = 0x50, .b2 = 0x50, .pad2 = 0x00 },
     .fogColors = { .r = 0xC0, .g = 0x80, .b = 0x60, .pad = 0x00, .r2 = 0xA0, .g2 = 0x70, .b2 = 0x40, .pad2 = 0x00 },
     .musicTrack = 0x000E,
     .padding2 = { 0 } },
    { .shortcutPosX = 0x3D070980,
     .shortcutPosZ = 0x00025DA4,
     .yawOffset = 0x0000,
     .padding = 0,
     .spawnPos = { .x = 0xFFC990D9, .y = 0x300FA11C, .z = 0x30551EA4 },
     .lightColors = { .r = 0xFF, .g = 0xFF, .b = 0xFF, .pad = 0x00, .r2 = 0x00, .g2 = 0x50, .b2 = 0x50, .pad2 = 0x00 },
     .fogColors = { .r = 0xC0, .g = 0xC0, .b = 0xC0, .pad = 0x00, .r2 = 0x80, .g2 = 0xC8, .b2 = 0xFF, .pad2 = 0x00 },
     .musicTrack = 0x000F,
     .padding2 = { 0 } },
    { .shortcutPosX = 0xC53CE61C,
     .shortcutPosZ = 0xD80C3DED,
     .yawOffset = 0xF800,
     .padding = 0,
     .spawnPos = { .x = 0xFFF1E7C2, .y = 0x300146B4, .z = 0x30451B0B },
     .lightColors = { .r = 0xF0, .g = 0xF0, .b = 0xA0, .pad = 0x00, .r2 = 0x00, .g2 = 0x50, .b2 = 0x50, .pad2 = 0x00 },
     .fogColors = { .r = 0x9F, .g = 0x9F, .b = 0x20, .pad = 0x00, .r2 = 0x60, .g2 = 0x80, .b2 = 0x40, .pad2 = 0x00 },
     .musicTrack = 0x0010,
     .padding2 = { 0 } },
    { .shortcutPosX = 0xC53CE61C,
     .shortcutPosZ = 0xD80C3DED,
     .yawOffset = 0xF800,
     .padding = 0,
     .spawnPos = { .x = 0xFFF1E7C2, .y = 0x300146B4, .z = 0x30451B0B },
     .lightColors = { .r = 0xFA, .g = 0xFA, .b = 0xA7, .pad = 0x00, .r2 = 0x00, .g2 = 0x50, .b2 = 0x50, .pad2 = 0x00 },
     .fogColors = { .r = 0x4A, .g = 0x40, .b = 0xBF, .pad = 0x00, .r2 = 0x20, .g2 = 0x1A, .b2 = 0x45, .pad2 = 0x00 },
     .musicTrack = 0x001F,
     .padding2 = { 0 } },
    { .shortcutPosX = 0x13A2B2B2,
     .shortcutPosZ = 0x0D88B1D8,
     .yawOffset = 0x0000,
     .padding = 0,
     .spawnPos = { .x = 0x00000000, .y = 0x30000000, .z = 0x30458CB2 },
     .lightColors = { .r = 0xC0, .g = 0xD0, .b = 0xFF, .pad = 0x00, .r2 = 0x00, .g2 = 0x50, .b2 = 0x50, .pad2 = 0x00 },
     .fogColors = { .r = 0x40, .g = 0x30, .b = 0x20, .pad = 0x00, .r2 = 0x07, .g2 = 0x00, .b2 = 0x20, .pad2 = 0x00 },
     .musicTrack = 0x0012,
     .padding2 = { 0 } },
    { .shortcutPosX = 0xF81F16C7,
     .shortcutPosZ = 0xF7874E40,
     .yawOffset = 0x0800,
     .padding = 0,
     .spawnPos = { .x = 0x000932B6, .y = 0x2FFF0788, .z = 0x305235B0 },
     .lightColors = { .r = 0xC0, .g = 0xC0, .b = 0xC0, .pad = 0x00, .r2 = 0x00, .g2 = 0x50, .b2 = 0x50, .pad2 = 0x00 },
     .fogColors = { .r = 0xA0, .g = 0x90, .b = 0xD0, .pad = 0x00, .r2 = 0x00, .g2 = 0x10, .b2 = 0x20, .pad2 = 0x00 },
     .musicTrack = 0x0015,
     .padding2 = { 0 } },
    { .shortcutPosX = 0x3EC583F5,
     .shortcutPosZ = 0x32FA3735,
     .yawOffset = 0x0000,
     .padding = 0,
     .spawnPos = { .x = 0xFFFB5440, .y = 0x30053ADE, .z = 0x3040B139 },
     .lightColors = { .r = 0xFF, .g = 0xDF, .b = 0x0F, .pad = 0x00, .r2 = 0x00, .g2 = 0x50, .b2 = 0x50, .pad2 = 0x00 },
     .fogColors = { .r = 0x60, .g = 0x50, .b = 0x80, .pad = 0x00, .r2 = 0x80, .g2 = 0x70, .b2 = 0xA0, .pad2 = 0x00 },
     .musicTrack = 0x0016,
     .padding2 = { 0 } },
    { .shortcutPosX = 0x3EC583F5,
     .shortcutPosZ = 0x32FA3735,
     .yawOffset = 0x0000,
     .padding = 0,
     .spawnPos = { .x = 0xFFFB5440, .y = 0x30053ADE, .z = 0x3040B139 },
     .lightColors = { .r = 0xDA, .g = 0xDA, .b = 0xEF, .pad = 0x00, .r2 = 0x00, .g2 = 0x50, .b2 = 0x50, .pad2 = 0x00 },
     .fogColors = { .r = 0x70, .g = 0x4A, .b = 0x8A, .pad = 0x00, .r2 = 0x60, .g2 = 0x60, .b2 = 0x80, .pad2 = 0x00 },
     .musicTrack = 0x0017,
     .padding2 = { 0 } },
    { .shortcutPosX = 0x276F3DFA,
     .shortcutPosZ = 0x033D7D8E,
     .yawOffset = 0xF800,
     .padding = 0,
     .spawnPos = { .x = 0xFFFCCAD4, .y = 0x2FFF0000, .z = 0x304E27C5 },
     .lightColors = { .r = 0xFF, .g = 0xFF, .b = 0xFF, .pad = 0x00, .r2 = 0x00, .g2 = 0x50, .b2 = 0x50, .pad2 = 0x00 },
     .fogColors = { .r = 0xC0, .g = 0xC0, .b = 0xC0, .pad = 0x00, .r2 = 0x80, .g2 = 0xC8, .b2 = 0xFF, .pad2 = 0x00 },
     .musicTrack = 0x000D,
     .padding2 = { 0 } },
    { .shortcutPosX = 0x276F3DFA,
     .shortcutPosZ = 0x033D7D8E,
     .yawOffset = 0xF800,
     .padding = 0,
     .spawnPos = { .x = 0xFFFCCAD4, .y = 0x2FFF0000, .z = 0x304E27C5 },
     .lightColors = { .r = 0xFF, .g = 0xFF, .b = 0xC0, .pad = 0x00, .r2 = 0x00, .g2 = 0x50, .b2 = 0x50, .pad2 = 0x00 },
     .fogColors = { .r = 0x80, .g = 0xA0, .b = 0xC0, .pad = 0x00, .r2 = 0xFF, .g2 = 0xFF, .b2 = 0xC0, .pad2 = 0x00 },
     .musicTrack = 0x000C,
     .padding2 = { 0 } },
    { .shortcutPosX = 0xC7679672,
     .shortcutPosZ = 0x02AFA0C2,
     .yawOffset = 0x0800,
     .padding = 0,
     .spawnPos = { .x = 0xFFF49A8A, .y = 0x30000000, .z = 0x30514975 },
     .lightColors = { .r = 0xFF, .g = 0xFF, .b = 0xFF, .pad = 0x00, .r2 = 0x00, .g2 = 0x50, .b2 = 0x50, .pad2 = 0x00 },
     .fogColors = { .r = 0xC0, .g = 0xC0, .b = 0xC0, .pad = 0x00, .r2 = 0x20, .g2 = 0x4B, .b2 = 0xFF, .pad2 = 0x00 },
     .musicTrack = 0x0014,
     .padding2 = { 0 } },
    { .shortcutPosX = 0xC7679672,
     .shortcutPosZ = 0x02AFA0C2,
     .yawOffset = 0x0800,
     .padding = 0,
     .spawnPos = { .x = 0xFFF49A8A, .y = 0x30000000, .z = 0x30514975 },
     .lightColors = { .r = 0xFF, .g = 0xFF, .b = 0xFF, .pad = 0x00, .r2 = 0x00, .g2 = 0x50, .b2 = 0x50, .pad2 = 0x00 },
     .fogColors = { .r = 0xC0, .g = 0xC0, .b = 0xC0, .pad = 0x00, .r2 = 0x20, .g2 = 0x4B, .b2 = 0xFF, .pad2 = 0x00 },
     .musicTrack = 0x0014,
     .padding2 = { 0 } },
};

Asset_56910 gCourseDataAssets[] = {
    { (void *)&_224420_ROM_START, (void *)&_224420_ROM_END, 0x000024A8 },
    { (void *)&_23CB80_ROM_START, (void *)&_23CB80_ROM_END, 0x00003468 },
    { (void *)&_24D6C0_ROM_START, (void *)&_24D6C0_ROM_END, 0x0000342A },
    { (void *)&_261530_ROM_START, (void *)&_261530_ROM_END, 0x0000342A },
    { (void *)&_274E20_ROM_START, (void *)&_274E20_ROM_END, 0x00004A9A },
    { (void *)&_28BA90_ROM_START, (void *)&_28BA90_ROM_END, 0x00003F92 },
    { (void *)&_29DF80_ROM_START, (void *)&_29DF80_ROM_END, 0x00002E50 },
    { (void *)&_2B0500_ROM_START, (void *)&_2B0500_ROM_END, 0x00002E50 },
    { (void *)&_2C8310_ROM_START, (void *)&_2C8310_ROM_END, 0x00004742 },
    { (void *)&_2DFA30_ROM_START, (void *)&_2DFA30_ROM_END, 0x000059B6 },
    { (void *)&_2F6620_ROM_START, (void *)&_2F6620_ROM_END, 0x00003BE6 },
    { (void *)&_30B250_ROM_START, (void *)&_30B250_ROM_END, 0x00003BE6 },
    { (void *)&_322130_ROM_START, (void *)&_322130_ROM_END, 0x00002C9E },
    { (void *)&_3375E0_ROM_START, (void *)&_3375E0_ROM_END, 0x00002C9E },
    { (void *)&_3471A0_ROM_START, (void *)&_3471A0_ROM_END, 0x00002E2C },
    { (void *)&_3471A0_ROM_START, (void *)&_3471A0_ROM_END, 0x00002E2C },
};

Asset_56910 gSpriteAssets[] = {
    { (void *)&_226BA0_ROM_START, (void *)&_226BA0_ROM_END, 0x00002208 },
    { (void *)&_240520_ROM_START, (void *)&_240520_ROM_END, 0x00001BA8 },
    { (void *)&_251060_ROM_START, (void *)&_251060_ROM_END, 0x00002A58 },
    { (void *)&_264EC0_ROM_START, (void *)&_264EC0_ROM_END, 0x00002A58 },
    { (void *)&_27A020_ROM_START, (void *)&_27A020_ROM_END, 0x00000918 },
    { (void *)&_28FF50_ROM_START, (void *)&_28FF50_ROM_END, 0x00001528 },
    { (void *)&_2A1080_ROM_START, (void *)&_2A1080_ROM_END, 0x00001D68 },
    { (void *)&_2B35E0_ROM_START, (void *)&_2B35E0_ROM_END, 0x00001298 },
    { (void *)&_2CD0D0_ROM_START, (void *)&_2CD0D0_ROM_END, 0x00000BC8 },
    { (void *)&_2E59C0_ROM_START, (void *)&_2E59C0_ROM_END, 0x000015E8 },
    { (void *)&_2FA740_ROM_START, (void *)&_2FA740_ROM_END, 0x00002C28 },
    { (void *)&_30F370_ROM_START, (void *)&_30F370_ROM_END, 0x00000938 },
    { (void *)&_325120_ROM_START, (void *)&_325120_ROM_END, 0x00000438 },
    { (void *)&_33A5D0_ROM_START, (void *)&_33A5D0_ROM_END, 0x00000438 },
    { (void *)&_34A2C0_ROM_START, (void *)&_34A2C0_ROM_END, 0x00000438 },
    { (void *)&_34A2C0_ROM_START, (void *)&_34A2C0_ROM_END, 0x00000438 },
};

AssetWithoutSize gUncompressedAssets[] = {
    { &_B9C40_ROM_START,  &_B9C40_ROM_END  },
    { &_C2E50_ROM_START,  &_C2E50_ROM_END  },
    { &_CE7A0_ROM_START,  &_CE7A0_ROM_END  },
    { &_DA9D0_ROM_START,  &_DA9D0_ROM_END  },
    { &_E6880_ROM_START,  &_E6880_ROM_END  },
    { &_F2B50_ROM_START,  &_F2B50_ROM_END  },
    { &_1004E0_ROM_START, &_1004E0_ROM_END },
    { &_10B010_ROM_START, &_10B010_ROM_END },
    { &_116860_ROM_START, &_116860_ROM_END },
    { &_124DD0_ROM_START, &_124DD0_ROM_END },
    { &_132BB0_ROM_START, &_132BB0_ROM_END },
    { &_13D660_ROM_START, &_13D660_ROM_END },
    { &_147B70_ROM_START, &_147B70_ROM_END },
    { &_156C00_ROM_START, &_156C00_ROM_END },
    { &_165720_ROM_START, &_165720_ROM_END },
    { &_165720_ROM_START, &_165720_ROM_END },
};

Asset_56910 gCompressedSegment2Assets[] = {
    { &_218F00_ROM_START, &_218F00_ROM_END, 0x00014CB0 },
    { &_228400_ROM_START, &_228400_ROM_END, 0x0001D560 },
    { &_2415B0_ROM_START, &_2415B0_ROM_END, 0x000182B0 },
    { &_252E80_ROM_START, &_252E80_ROM_END, 0x000181D0 },
    { &_266C50_ROM_START, &_266C50_ROM_END, 0x0001C6C0 },
    { &_27A780_ROM_START, &_27A780_ROM_END, 0x00021480 },
    { &_290D70_ROM_START, &_290D70_ROM_END, 0x000176D0 },
    { &_2A2810_ROM_START, &_2A2810_ROM_END, 0x00019040 },
    { &_2B45F0_ROM_START, &_2B45F0_ROM_END, 0x00027BE0 },
    { &_2CD790_ROM_START, &_2CD790_ROM_END, 0x00025EA0 },
    { &_2E6CC0_ROM_START, &_2E6CC0_ROM_END, 0x0001C050 },
    { &_2FC440_ROM_START, &_2FC440_ROM_END, 0x0001ACD0 },
    { &_30F9D0_ROM_START, &_30F9D0_ROM_END, 0x00020640 },
    { &_325260_ROM_START, &_325260_ROM_END, 0x00020210 },
    { &_33A710_ROM_START, &_33A710_ROM_END, 0x00016160 },
    { &_33A710_ROM_START, &_33A710_ROM_END, 0x00016160 },
};

s32 gSkyDisplayLists1[] = {
    (s32)D_80094CC0_958C0, (s32)D_80094DB0_959B0, (s32)D_80094ED0_95AD0, (s32)D_80094FB0_95BB0,
    (s32)D_80095090_95C90, (s32)D_80095190_95D90, (s32)D_80095250_95E50, (s32)D_80095390_95F90,
    (s32)D_80095490_96090, (s32)D_800955F0_961F0, (s32)D_800956C0_962C0, (s32)D_80095790_96390,
    (s32)D_80095850_96450, (s32)D_80095920_96520, (s32)D_800959F0_965F0, (s32)D_800959F0_965F0,
};

s32 gSkyDisplayLists2[] = {
    (s32)D_80094CD0_958D0, (s32)D_80094DC0_959C0, (s32)D_80094F00_95B00, (s32)D_80094FE0_95BE0,
    (s32)D_800950A0_95CA0, (s32)D_800951A0_95DA0, (s32)D_80095260_95E60, (s32)D_800953A0_95FA0,
    (s32)D_800954A0_960A0, (s32)D_80095600_96200, (s32)D_80095780_96380, (s32)D_80095840_96440,
    (s32)D_80095870_96470, (s32)D_80095940_96540, (s32)D_80095A00_96600, (s32)D_80095A00_96600,
};

LevelDisplayLists *gSkyDisplayLists3[] = {
    (LevelDisplayLists *)D_80094CE0_958E0, (LevelDisplayLists *)D_80094E20_95A20, (LevelDisplayLists *)D_80094F10_95B10,
    (LevelDisplayLists *)D_80094FF0_95BF0, (LevelDisplayLists *)D_800950C0_95CC0, (LevelDisplayLists *)D_800951B0_95DB0,
    (LevelDisplayLists *)D_80095270_95E70, (LevelDisplayLists *)D_800953B0_95FB0, (LevelDisplayLists *)D_800954B0_960B0,
    (LevelDisplayLists *)D_80095610_96210, (LevelDisplayLists *)D_800956D0_962D0, (LevelDisplayLists *)D_800957A0_963A0,
    (LevelDisplayLists *)D_80095880_96480, (LevelDisplayLists *)D_80095950_96550, (LevelDisplayLists *)D_80095A10_96610,
    (LevelDisplayLists *)D_80095A10_96610,
};

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
