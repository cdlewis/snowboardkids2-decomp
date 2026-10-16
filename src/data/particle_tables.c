#include "assets.h"
#include "common.h"
#include "data/asset_metadata.h"
#include "race/particle_items.h"

typedef struct {
    void *romStart;
    void *romEnd;
    s32 decompressedSize;
} CompressedAssetEntry;

u32 D_800908A0_914A0[] = {
    0xFFE80018, 0x00000000, 0xFFF0FFF0, 0xFFFFFFFF, 0x00180018, 0x00000000, 0x07F0FFF0, 0xFFFFFFFF,
    0x0018FFE8, 0x00000000, 0x07F007F0, 0xFFFFFFFF, 0xFFE8FFE8, 0x00000000, 0xFFF007F0, 0xFFFFFFFF,
};

u32 D_800908E0_914E0[] = {
    0x00002000, 0x00020000, 0x00020000, 0xFFFFC800, 0x00040000, 0x0001C000, 0xFFFFD000, 0x00052000, 0xFFFDF800,
    0xFFFF0000, 0x0000E000, 0xFFFFE000, 0x00021000, 0x00030100, 0xFFFFD000, 0x00020000, 0x0003F000, 0x00003800,
};

s8 starAnimFrameDurations[20] = {
    0x0F, 0x3A, 0x0F, 0x3B, 0x0F, 0x3A, 0x0F, 0x3B, 0x1E, 0x3C,
    0x0F, 0x3D, 0x0F, 0x3E, 0x0F, 0x3D, 0x0F, 0x3E, 0x00, 0x00,
};

u32 D_8009093C_9153C[] = {
    0x024D034E, 0x034F0350, 0x02510452, 0x02530254, 0x03550000,
};

u32 D_80090950_91550[] = { 0x01560157, 0x00000000 };

u32 D_80090958_91558[] = { 0x01590000 };

u32 D_8009095C_9155C[] = { 0x025A025B, 0x00000000 };

u32 D_80090964_91564[] = { 0x025C025D, 0x025C025D, 0x045E055F, 0x00000000 };

u32 D_80090974_91574[] = { 0x024D024E, 0x024F0250, 0x02510000 };

PushZoneDataEntry gPushZoneData[] = {
    { gCrazyJunglePushZoneDisplayLists,
     0, { 0x1013507C, 0x13E86349, (s32)0xF1905137 },
     0xFFC7, 0x0712,
     -8,
     8, -3,
     2, -424,
     392 },
    { gCrazyJunglePushZoneDisplayLists,
     1, { 0x0A2084E6, 0x1352462B, (s32)0xEE384AE5 },
     0xFFA7, 0x03DF,
     -8,
     8, -3,
     2, -480,
     480 },
    { gCrazyJungleBossPushZoneDisplayLists,
     0, { 0x1013507C, 0x13E86349, (s32)0xF1905137 },
     0xFFC7, 0x0712,
     -8,
     8, -3,
     2, -424,
     392 },
    { gCrazyJungleBossPushZoneDisplayLists,
     1, { 0x0A2084E6, 0x1352462B, (s32)0xEE384AE5 },
     0xFFA7, 0x03DF,
     -8,
     8, -3,
     2, -480,
     480 },
    { gIceLandPushZoneDisplayLists,
     0, { (s32)0xFF83AC8D, 0x19290E26, 0x19D8FFFA },
     0xFEB9, 0xFA37,
     -8,
     8, -3,
     2, -565,
     565 },
    { gIceLandBossPushZoneDisplayLists,
     0, { (s32)0xFF83AC8D, 0x19290E26, 0x19D8FFFA },
     0xFEB9, 0xFA37,
     -8,
     8, -3,
     2, -565,
     565 },
    { gHauntedHousePushZoneDisplayLists,
     0, { 0x0D4039B0, 0x2B2BB65E, 0x1EF0660B },
     0xFF44, 0xFD45,
     -8,
     8, -3,
     2, -237,
     240 },
    { gHauntedHousePushZoneDisplayLists,
     0, { 0x0F3EF9B0, 0x2A8B365E, 0x1B1E660B },
     0xFF3E, 0xFD45,
     -8,
     8, -3,
     2, -237,
     240 },
};

u32 D_80090AA0_916A0[] = { 0x009A8000, 0x000D8000, 0xFF880000 };

u32 D_80090AAC_916AC[] = { 0x00000000, 0x00000000, 0x00040000 };

CompressedAssetEntry D_80090AB8_916B8[] = {
    { &ITEM_TRIGGER_DATA_00_ROM_START, &ITEM_TRIGGER_DATA_00_ROM_END, 0x00000130 },
    { &ITEM_TRIGGER_DATA_01_ROM_START, &ITEM_TRIGGER_DATA_01_ROM_END, 0x000002A0 },
    { &ITEM_TRIGGER_DATA_02_ROM_START, &ITEM_TRIGGER_DATA_02_ROM_END, 0x00000200 },
    { &ITEM_TRIGGER_DATA_03_ROM_START, &ITEM_TRIGGER_DATA_03_ROM_END, 0x00000200 },
};

static u32 D_80090AE8_padding[2] = { 0, 0 };
