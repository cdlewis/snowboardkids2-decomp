#include "20F0.h"

#include "19E80.h"
#include "5E590.h"
#include "D6F0.h"
#include "assets.h"
#include "audio.h"
#include "displaylist.h"
#include "race_session.h"
#include "rom_loader.h"
#include "task_scheduler.h"

typedef struct {
    u8 padding[0x24];
    /* 0x24 */ u8 *data_ptr;
    /* 0x28 */ TableEntry_19E80 *index_ptr;
    s8 tableField1;
    s8 tableField2;
} AssetSlotTableData;

typedef struct {
    u8 padding[0x3C];
    s8 isDestroyed;
} MarkDestroyedArg;

typedef struct {
    u8 padding[0x3D];
    s8 actionMode;
} getModelActionMode_arg;

extern u8 identityMatrix[32];
extern u8 D_215D70[];

u32 itemMatrix_215360[4] = { 0x0, 0x10000D0, 0x0, 0x0 };

u32 itemMatrix_215440[4] = { 0x0, 0x10000F8, 0x0, 0x0 };

u32 itemMatrix_215550[4] = { 0x0, 0x1000198, 0x0, 0x0 };

u32 itemMatrix_215700[4] = { 0x0, 0x10001A0, 0x0, 0x0 };

u32 itemMatrix_2158B0[4] = { 0x0, 0x10001A0, 0x0, 0x0 };

u32 itemMatrix_215A60[4] = { 0x0, 0x10002F8, 0x0, 0x0 };

ItemAssetEntry itemAssetTable[] = {
    { &_214FC0_ROM_START,                  &_214FC0_ROM_END, &_663440_ROM_START, &_663440_ROM_END, 0x520, (void *)0x80089500        },
    { &_1ED890_ROM_START,                  &_1ED890_ROM_END, &_4A5840_ROM_START, &_4A5840_ROM_END, 0x370, (void *)0x800882F0        },
    { &ITEM_DEBRIS_DISPLAY_LIST_ROM_START,
     &ITEM_DEBRIS_DISPLAY_LIST_ROM_END,
     &ITEM_DEBRIS_COMPRESSED_DATA_ROM_START,
     &ITEM_DEBRIS_COMPRESSED_DATA_ROM_END,
     0x370,                                                                                               (void *)0x80088300        },
    { &_215280_ROM_START,                  &_215280_ROM_END, &_663C90_ROM_START, &_663C90_ROM_END, 0x1D0, (void *)0x80089530        },
    { &_215440_ROM_START,                  &_215440_ROM_END, &_663EE0_ROM_START, &_663EE0_ROM_END, 0x3C0, (void *)itemMatrix_215440 },
    { &BURG_DISPLAY_LIST_ROM_START,
     &BURG_DISPLAY_LIST_ROM_END,
     &BURG_COMPRESSED_DATA_ROM_START,
     &BURG_COMPRESSED_DATA_ROM_END,
     0x620,                                                                                               (void *)0x80088290        },
    { &_1FBE00_ROM_START,                  &_1FBE00_ROM_END, &_4CAD30_ROM_START, &_4CAD30_ROM_END, 0x620, (void *)0x800886A0        },
    { &_215550_ROM_START,                  &_215550_ROM_END, &_664140_ROM_START, &_664140_ROM_END, 0x3B0, (void *)itemMatrix_215550 },
    { &_1FC700_ROM_START,                  &_1FC700_ROM_END, &_4CCA00_ROM_START, &_4CCA00_ROM_END, 0x770, (void *)0x800886E0        },
    { &_215700_ROM_START,                  &_215700_ROM_END, &_664400_ROM_START, &_664400_ROM_END, 0x4A0, (void *)itemMatrix_215700 },
    { &_2158B0_ROM_START,                  &_2158B0_ROM_END, &_664660_ROM_START, &_664660_ROM_END, 0x4A0, (void *)itemMatrix_2158B0 },
    { &CUP_DISPLAY_LIST_ROM_START,
     &CUP_DISPLAY_LIST_ROM_END,
     &CUP_COMPRESSED_DATA_ROM_START,
     &CUP_COMPRESSED_DATA_ROM_END,
     0xA90,                                                                                               (void *)0x80088360        },
    { &_1EE410_ROM_START,                  &_1EE410_ROM_END, &_4A6B60_ROM_START, &_4A6B60_ROM_END, 0xA90, (void *)0x80088350        },
    { &DISH_DISPLAY_LIST_ROM_START,
     &DISH_DISPLAY_LIST_ROM_END,
     &DISH_COMPRESSED_DATA_ROM_START,
     &DISH_COMPRESSED_DATA_ROM_END,
     0x320,                                                                                               (void *)0x80088310        },
    { &ABOXL_DISPLAY_LIST_ROM_START,
     &ABOXL_DISPLAY_LIST_ROM_END,
     &ABOXL_COMPRESSED_DATA_ROM_START,
     &ABOXL_COMPRESSED_DATA_ROM_END,
     0x710,                                                                                               (void *)0x80088700        },
    { &_1EACD0_ROM_START,                  &_1EACD0_ROM_END, &_4A2980_ROM_START, &_4A2980_ROM_END, 0x790, (void *)0x800881E0        },
    { &BOMB2_DISPLAY_LIST_ROM_START,
     &BOMB2_DISPLAY_LIST_ROM_END,
     &BOMB2_COMPRESSED_DATA_ROM_START,
     &BOMB2_COMPRESSED_DATA_ROM_END,
     0x7E0,                                                                                               (void *)0x800881A0        },
    { &_215360_ROM_START,                  &_215360_ROM_END, &_663DB0_ROM_START, &_663DB0_ROM_END, 0x240, (void *)itemMatrix_215360 },
    { &_215A60_ROM_START,                  &_215A60_ROM_END, &_6648C0_ROM_START, &_6648C0_ROM_END, 0x810, (void *)itemMatrix_215A60 },
};

s32 itemAssetCount = ARRAY_SIZE(itemAssetTable);

ItemAssetEntry SLASH_ASSETS[4] = {
    { (void *)&SLASH1_DISPLAY_LIST_ROM_START,
     (void *)&SLASH1_DISPLAY_LIST_ROM_END,
     (void *)&SLASH1_COMPRESSED_DATA_ROM_START,
     (void *)&SLASH1_COMPRESSED_DATA_ROM_END,
     0x3DE00006, (void *)0x80088770 },
    { (void *)&SLASH2_DISPLAY_LIST_ROM_START,
     (void *)&SLASH2_DISPLAY_LIST_ROM_END,
     (void *)&SLASH2_COMPRESSED_DATA_ROM_START,
     (void *)&SLASH2_COMPRESSED_DATA_ROM_END,
     0x3AD00006, (void *)0x800887D0 },
    { (void *)&SLASH3_DISPLAY_LIST_ROM_START,
     (void *)&SLASH3_DISPLAY_LIST_ROM_END,
     (void *)&SLASH3_COMPRESSED_DATA_ROM_START,
     (void *)&SLASH3_COMPRESSED_DATA_ROM_END,
     0x4D900006, (void *)0x80088830 },
    { (void *)&SLASH4_DISPLAY_LIST_ROM_START,
     (void *)&SLASH4_DISPLAY_LIST_ROM_END,
     (void *)&SLASH4_COMPRESSED_DATA_ROM_START,
     (void *)&SLASH4_COMPRESSED_DATA_ROM_END,
     0x3F600006, (void *)0x80088890 },
};

ItemAssetEntry NANCY_ASSETS[4] = {
    { (void *)&_203C30_ROM_START,
     (void *)&_203C30_ROM_END,
     (void *)&_4D45C0_ROM_START,
     (void *)&_4D45C0_ROM_END,
     0x2F200005, (void *)0x800888F0 },
    { (void *)&_204980_ROM_START,
     (void *)&_204980_ROM_END,
     (void *)&_4D5270_ROM_START,
     (void *)&_4D5270_ROM_END,
     0x27F00005, (void *)0x80088940 },
    { (void *)&_205510_ROM_START,
     (void *)&_205510_ROM_END,
     (void *)&_4D5D60_ROM_START,
     (void *)&_4D5D60_ROM_END,
     0x28900005, (void *)0x80088990 },
    { (void *)&_206080_ROM_START,
     (void *)&_206080_ROM_END,
     (void *)&_4D6880_ROM_START,
     (void *)&_4D6880_ROM_END,
     0x2C000005, (void *)0x800889E0 },
};

ItemAssetEntry JAM_ASSETS[4] = {
    { (void *)&JAM1_DISPLAY_LIST_ROM_START,
     (void *)&JAM1_DISPLAY_LIST_ROM_END,
     (void *)&JAM1_COMPRESSED_DATA_ROM_START,
     (void *)&JAM1_COMPRESSED_DATA_ROM_END,
     0x2B000005, (void *)0x80088A30 },
    { (void *)&JAM2_DISPLAY_LIST_ROM_START,
     (void *)&JAM2_DISPLAY_LIST_ROM_END,
     (void *)&JAM2_COMPRESSED_DATA_ROM_START,
     (void *)&JAM2_COMPRESSED_DATA_ROM_END,
     0x2FE00005, (void *)0x80088A80 },
    { (void *)&JAM3_DISPLAY_LIST_ROM_START,
     (void *)&JAM3_DISPLAY_LIST_ROM_END,
     (void *)&JAM3_COMPRESSED_DATA_ROM_START,
     (void *)&JAM3_COMPRESSED_DATA_ROM_END,
     0x2B800005, (void *)0x80088AD0 },
    { (void *)&JAM4_DISPLAY_LIST_ROM_START,
     (void *)&JAM4_DISPLAY_LIST_ROM_END,
     (void *)&JAM4_COMPRESSED_DATA_ROM_START,
     (void *)&JAM4_COMPRESSED_DATA_ROM_END,
     0x40500005, (void *)0x80088B20 },
};

ItemAssetEntry LINDA_ASSETS[4] = {
    { (void *)&JAM5_DISPLAY_LIST_ROM_START,
     (void *)&JAM5_DISPLAY_LIST_ROM_END,
     (void *)&JAM5_COMPRESSED_DATA_ROM_START,
     (void *)&JAM5_COMPRESSED_DATA_ROM_END,
     0x27F00005, (void *)0x80088B70 },
    { (void *)&_20BB50_ROM_START,
     (void *)&_20BB50_ROM_END,
     (void *)&_4DC7A0_ROM_START,
     (void *)&_4DC7A0_ROM_END,
     0x27F00005, (void *)0x80088BC0 },
    { (void *)&_20C5F0_ROM_START,
     (void *)&_20C5F0_ROM_END,
     (void *)&_4DD2C0_ROM_START,
     (void *)&_4DD2C0_ROM_END,
     0x27500005, (void *)0x80088C10 },
    { (void *)&_20D040_ROM_START,
     (void *)&_20D040_ROM_END,
     (void *)&_4DDD90_ROM_START,
     (void *)&_4DDD90_ROM_END,
     0x25C00005, (void *)0x80088C60 },
};

ItemAssetEntry TOMMY_ASSETS[4] = {
    { (void *)&_20DAE0_ROM_START,
     (void *)&_20DAE0_ROM_END,
     (void *)&_4DE820_ROM_START,
     (void *)&_4DE820_ROM_END,
     0x4D000005, (void *)0x80088CB0 },
    { (void *)&_20E850_ROM_START,
     (void *)&_20E850_ROM_END,
     (void *)&_4DFAC0_ROM_START,
     (void *)&_4DFAC0_ROM_END,
     0x50200005, (void *)0x80088D00 },
    { (void *)&_20F730_ROM_START,
     (void *)&_20F730_ROM_END,
     (void *)&_4E0ED0_ROM_START,
     (void *)&_4E0ED0_ROM_END,
     0x4E400005, (void *)0x80088D50 },
    { (void *)&_210390_ROM_START,
     (void *)&_210390_ROM_END,
     (void *)&_4E21A0_ROM_START,
     (void *)&_4E21A0_ROM_END,
     0x48D00005, (void *)0x80088DA0 },
};

ItemAssetEntry WENDY_ASSETS[4] = {
    { (void *)&_2110E0_ROM_START,
     (void *)&_2110E0_ROM_END,
     (void *)&_4E33D0_ROM_START,
     (void *)&_4E33D0_ROM_END,
     0x27A00003, (void *)0x80088DF0 },
    { (void *)&_211920_ROM_START,
     (void *)&_211920_ROM_END,
     (void *)&_4E40A0_ROM_START,
     (void *)&_4E40A0_ROM_END,
     0x1AC00002, (void *)0x80088E20 },
    { (void *)&_211F40_ROM_START,
     (void *)&_211F40_ROM_END,
     (void *)&_4E4AA0_ROM_START,
     (void *)&_4E4AA0_ROM_END,
     0x1AE00002, (void *)0x80088E40 },
    { (void *)&_2125A0_ROM_START,
     (void *)&_2125A0_ROM_END,
     (void *)&_4E54E0_ROM_START,
     (void *)&_4E54E0_ROM_END,
     0x1D000002, (void *)0x80088E60 },
};

ItemAssetEntry BLACK_ASSETS[4] = {
    { (void *)&BLACK_DISPLAY_LIST_ROM_START,
     (void *)&BLACK_DISPLAY_LIST_ROM_END,
     (void *)&BLACK_COMPRESSED_DATA_ROM_START,
     (void *)&BLACK_COMPRESSED_DATA_ROM_END,
     0x3DE00008, (void *)0x80088E80 },
    { (void *)&BLACK_DISPLAY_LIST_ROM_START,
     (void *)&BLACK_DISPLAY_LIST_ROM_END,
     (void *)&BLACK_COMPRESSED_DATA_ROM_START,
     (void *)&BLACK_COMPRESSED_DATA_ROM_END,
     0x3DE00008, (void *)0x80088E80 },
    { (void *)&BLACK_DISPLAY_LIST_ROM_START,
     (void *)&BLACK_DISPLAY_LIST_ROM_END,
     (void *)&BLACK_COMPRESSED_DATA_ROM_START,
     (void *)&BLACK_COMPRESSED_DATA_ROM_END,
     0x3DE00008, (void *)0x80088E80 },
    { (void *)&BLACK_DISPLAY_LIST_ROM_START,
     (void *)&BLACK_DISPLAY_LIST_ROM_END,
     (void *)&BLACK_COMPRESSED_DATA_ROM_START,
     (void *)&BLACK_COMPRESSED_DATA_ROM_END,
     0x3DE00008, (void *)0x80088E80 },
};

ItemAssetEntry PEN_ASSETS[4] = {
    { (void *)&PEN_DISPLAY_LIST_ROM_START,
     (void *)&PEN_DISPLAY_LIST_ROM_END,
     (void *)&PEN_COMPRESSED_DATA_ROM_START,
     (void *)&PEN_COMPRESSED_DATA_ROM_END,
     0x09800001, (void *)0x80088F00 },
    { (void *)&PEN_DISPLAY_LIST_ROM_START,
     (void *)&PEN_DISPLAY_LIST_ROM_END,
     (void *)&PEN_COMPRESSED_DATA_ROM_START,
     (void *)&PEN_COMPRESSED_DATA_ROM_END,
     0x09800001, (void *)0x80088F00 },
    { (void *)&PEN_DISPLAY_LIST_ROM_START,
     (void *)&PEN_DISPLAY_LIST_ROM_END,
     (void *)&PEN_COMPRESSED_DATA_ROM_START,
     (void *)&PEN_COMPRESSED_DATA_ROM_END,
     0x09800001, (void *)0x80088F00 },
    { (void *)&PEN_DISPLAY_LIST_ROM_START,
     (void *)&PEN_DISPLAY_LIST_ROM_END,
     (void *)&PEN_COMPRESSED_DATA_ROM_START,
     (void *)&PEN_COMPRESSED_DATA_ROM_END,
     0x09800001, (void *)0x80088F00 },
};

AssetGroup gameAssets[] = {
    {
     .name = " SLASH ",
     .numAssets = (s8)0x10,
     .padding = 0x00E7,
     .animationDataStart = (void *)&_4E7AA0_ROM_START,
     .animationDataEnd = (void *)&_4E7AA0_ROM_END,
     .animationDataSize = 0x9F02,
     .anotherAssetIndex = 0xff050000,
     .Assets = (void *)SLASH_ASSETS,
     .count = 4,
     },
    {
     .name = " NANCY ",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x01,
     .padding = 0x0175,
     .animationDataStart = (void *)&_510AD0_ROM_START,
     .animationDataEnd = (void *)&_510AD0_ROM_END,
     .animationDataSize = 0xDE48,
     .anotherAssetIndex = 0xff050000,
     .Assets = (void *)NANCY_ASSETS,
     .count = 4,
     },
    {
     .name = " JAM   ",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x02,
     .padding = 0x012F,
     .animationDataStart = (void *)&JAM_ANIMATION_DATA_ROM_START,
     .animationDataEnd = (void *)&JAM_ANIMATION_DATA_ROM_END,
     .animationDataSize = 0xC848,
     .anotherAssetIndex = 0xff060000,
     .Assets = (void *)JAM_ASSETS,
     .count = 4,
     },
    {
     .name = " LINDA ",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x03,
     .padding = 0x011C,
     .animationDataStart = (void *)&_542AF0_ROM_START,
     .animationDataEnd = (void *)&_542AF0_ROM_END,
     .animationDataSize = 0xE6AE,
     .anotherAssetIndex = 0xff050000,
     .Assets = (void *)LINDA_ASSETS,
     .count = 4,
     },
    {
     .name = " TOMMY ",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x04,
     .padding = 0x0178,
     .animationDataStart = (void *)&_55E550_ROM_START,
     .animationDataEnd = (void *)&_55E550_ROM_END,
     .animationDataSize = 0xB6D0,
     .anotherAssetIndex = 0xff070000,
     .Assets = (void *)TOMMY_ASSETS,
     .count = 4,
     },
    {
     .name = " WENDY ",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x05,
     .padding = 0x0174,
     .animationDataStart = (void *)&PEN_ANIMATION_DATA_ROM_START,
     .animationDataEnd = (void *)&PEN_ANIMATION_DATA_ROM_END,
     .animationDataSize = 0xFE66,
     .anotherAssetIndex = 0xff050000,
     .Assets = (void *)WENDY_ASSETS,
     .count = 4,
     },
    {
     .name = " BLACK ",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x06,
     .padding = 0x0157,
     .animationDataStart = (void *)&BLACK_ANIMATION_DATA_ROM_START,
     .animationDataEnd = (void *)&BLACK_ANIMATION_DATA_ROM_END,
     .animationDataSize = 0xC8A4,
     .anotherAssetIndex = 0xff040000,
     .Assets = (void *)BLACK_ASSETS,
     .count = 4,
     },
    {
     .name = " PEN   ",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x07,
     .padding = 0x0174,
     .animationDataStart = (void *)&PEN_ANIMATION_DATA_ROM_START,
     .animationDataEnd = (void *)&PEN_ANIMATION_DATA_ROM_END,
     .animationDataSize = 0xFE66,
     .anotherAssetIndex = 0xff040000,
     .Assets = (void *)PEN_ASSETS,
     .count = 4,
     },
    {
     .name = " DOG   ",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x08,
     .padding = 0x009F,
     .animationDataStart = (void *)&DOG_ANIMATION_DATA_ROM_START,
     .animationDataEnd = (void *)&DOG_ANIMATION_DATA_ROM_END,
     .animationDataSize = 0x8070,
     .anotherAssetIndex = 0xff080000,
     },
    {
     .name = " DINOS ",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x09,
     .padding = 0x015D,
     .animationDataStart = (void *)&DINOS_ANIMATION_DATA_ROM_START,
     .animationDataEnd = (void *)&DINOS_ANIMATION_DATA_ROM_END,
     .animationDataSize = 0x5788,
     .anotherAssetIndex = 0xff190000,
     },
    {
     .name = " SNOW  ",
     .numAssets = (s8)0x03,
     .assetGroupIndex = (s8)0x0A,
     .anotherAssetIndex = 0xff110000,
     },
    {
     .name = " ROBO  ",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x0B,
     .padding = 0x009D,
     .animationDataStart = (void *)&_5B5590_ROM_START,
     .animationDataEnd = (void *)&_5B5590_ROM_END,
     .animationDataSize = 0x3CDA,
     .anotherAssetIndex = 0xff170000,
     },
    {
     .name = " CHAIR ",
     .displayListStart = (void *)&CHAIR_DISPLAY_LIST_ROM_START,
     .displayListEnd = (void *)&CHAIR_DISPLAY_LIST_ROM_END,
     .compressedDataStart = (void *)&CHAIR_COMPRESSED_DATA_ROM_START,
     .compressedDataEnd = (void *)&CHAIR_COMPRESSED_DATA_ROM_END,
     .decompressedSize = 0x560,
     .unk1C = (void *)0x80088370,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .anotherAssetIndex = 0xff020000,
     },
    {
     .name = " CUP   ",
     .displayListStart = (void *)&CUP_DISPLAY_LIST_ROM_START,
     .displayListEnd = (void *)&CUP_DISPLAY_LIST_ROM_END,
     .compressedDataStart = (void *)&CUP_COMPRESSED_DATA_ROM_START,
     .compressedDataEnd = (void *)&CUP_COMPRESSED_DATA_ROM_END,
     .decompressedSize = 0xA90,
     .unk1C = (void *)0x80088360,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .anotherAssetIndex = 0xff000000,
     },
    {
     .name = " POT   ",
     .displayListStart = (void *)&_1EE410_ROM_START,
     .displayListEnd = (void *)&_1EE410_ROM_END,
     .compressedDataStart = (void *)&_4A6B60_ROM_START,
     .compressedDataEnd = (void *)&_4A6B60_ROM_END,
     .decompressedSize = 0xA90,
     .unk1C = (void *)0x80088350,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .anotherAssetIndex = 0xff000000,
     },
    {
     .name = " SNOW2 ",
     .displayListStart = (void *)&_1EF780_ROM_START,
     .displayListEnd = (void *)&_1EF780_ROM_END,
     .compressedDataStart = (void *)&_4A8180_ROM_START,
     .compressedDataEnd = (void *)&_4A8180_ROM_END,
     .decompressedSize = 0xB20,
     .unk1C = (void *)0x800883D0,
     .numAssets = (s8)0x02,
     .assetGroupIndex = (s8)-0x01,
     .padding = 0x0136,
     .animationDataStart = (void *)&_5B4B50_ROM_START,
     .animationDataEnd = (void *)&_5B4B50_ROM_END,
     .animationDataSize = 0x1CDA,
     .anotherAssetIndex = 0xff110000,
     },
    {
     .name = " BOMB  ",
     .assetGroupIndex = (s8)-0x01,
     .anotherAssetIndex = 0xff000000,
     .soundSequenceDataStart = (void *)&BOMB_SOUND_SEQUENCE_DATA_ROM_START,
     .soundSequenceDataEnd = (void *)&BOMB_SOUND_SEQUENCE_DATA_ROM_END,
     .soundSequenceDataSize = 0x358,
     .initCallback = (void *)0x80006940,
     },
    {
     .name = " BUTTE ",
     .assetGroupIndex = (s8)-0x01,
     .anotherAssetIndex = 0xff000000,
     .initCallback = (void *)0x800063B4,
     },
    {
     .name = " BUS   ",
     .displayListStart = (void *)&BUS_DISPLAY_LIST_ROM_START,
     .displayListEnd = (void *)&BUS_DISPLAY_LIST_ROM_END,
     .compressedDataStart = (void *)&BUS_COMPRESSED_DATA_ROM_START,
     .compressedDataEnd = (void *)&BUS_COMPRESSED_DATA_ROM_END,
     .decompressedSize = 0x1280,
     .unk1C = (void *)0x80088630,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .anotherAssetIndex = 0xff000000,
     .initCallback = (void *)0x80005DB0,
     },
    {
     .name = " UFO   ",
     .displayListStart = (void *)&_1F8D10_ROM_START,
     .displayListEnd = (void *)&_1F8D10_ROM_END,
     .compressedDataStart = (void *)&_4C20A0_ROM_START,
     .compressedDataEnd = (void *)&_4C20A0_ROM_END,
     .decompressedSize = 0x25A0,
     .unk1C = (void *)0x800885C0,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .anotherAssetIndex = 0xff140000,
     .soundSequenceDataStart = (void *)&_4C3660_ROM_START,
     .soundSequenceDataEnd = (void *)&_4C3660_ROM_END,
     .soundSequenceDataSize = 0x838,
     .initCallback = (void *)0x80005808,
     },
    {
     .name = " SLASH2",
     .numAssets = (s8)0x10,
     .padding = 0x007A,
     .animationDataStart = (void *)&_4ED9C0_ROM_START,
     .animationDataEnd = (void *)&_4ED9C0_ROM_END,
     .animationDataSize = 0xC3F2,
     .anotherAssetIndex = 0xff050000,
     .Assets = (void *)SLASH_ASSETS,
     .count = 4,
     },
    {
     .name = " NANCY2",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x01,
     .padding = 0x007A,
     .animationDataStart = (void *)&_518BA0_ROM_START,
     .animationDataEnd = (void *)&_518BA0_ROM_END,
     .animationDataSize = 0x6032,
     .anotherAssetIndex = 0xff050000,
     .Assets = (void *)NANCY_ASSETS,
     .count = 4,
     },
    {
     .name = " JAM2  ",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x02,
     .padding = 0x007A,
     .animationDataStart = (void *)&JAM2_ANIMATION_DATA_ROM_START,
     .animationDataEnd = (void *)&JAM2_ANIMATION_DATA_ROM_END,
     .animationDataSize = 0xB2DA,
     .anotherAssetIndex = 0xff060000,
     .Assets = (void *)JAM_ASSETS,
     .count = 4,
     },
    {
     .name = " LINDA2",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x03,
     .padding = 0x007A,
     .animationDataStart = (void *)&_54B280_ROM_START,
     .animationDataEnd = (void *)&_54B280_ROM_END,
     .animationDataSize = 0x9814,
     .anotherAssetIndex = 0xff050000,
     .Assets = (void *)LINDA_ASSETS,
     .count = 4,
     },
    {
     .name = " TOMMY2",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x04,
     .padding = 0x007A,
     .animationDataStart = (void *)&DOG2_ANIMATION_DATA_ROM_START,
     .animationDataEnd = (void *)&DOG2_ANIMATION_DATA_ROM_END,
     .animationDataSize = 0x8094,
     .anotherAssetIndex = 0xff070000,
     .Assets = (void *)TOMMY_ASSETS,
     .count = 4,
     },
    {
     .name = " WENDY2",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x05,
     .padding = 0x007A,
     .animationDataStart = (void *)&_580AD0_ROM_START,
     .animationDataEnd = (void *)&_580AD0_ROM_END,
     .animationDataSize = 0x6C2A,
     .anotherAssetIndex = 0xff050000,
     .Assets = (void *)WENDY_ASSETS,
     .count = 4,
     },
    {
     .name = " BLACK2",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x06,
     .padding = 0x007A,
     .animationDataStart = (void *)&BLACK2_ANIMATION_DATA_ROM_START,
     .animationDataEnd = (void *)&BLACK2_ANIMATION_DATA_ROM_END,
     .animationDataSize = 0x5A70,
     .anotherAssetIndex = 0xff040000,
     .Assets = (void *)BLACK_ASSETS,
     .count = 4,
     },
    {
     .name = " PEN2  ",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x07,
     .padding = 0x00A0,
     .animationDataStart = (void *)&PEN2_ANIMATION_DATA_ROM_START,
     .animationDataEnd = (void *)&PEN2_ANIMATION_DATA_ROM_END,
     .animationDataSize = 0x40A2,
     .anotherAssetIndex = 0xff040000,
     .Assets = (void *)PEN_ASSETS,
     .count = 4,
     },
    {
     .name = " DOG2  ",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x08,
     .padding = 0x007A,
     .animationDataStart = (void *)&DOG2_ANIMATION_DATA_ROM_START,
     .animationDataEnd = (void *)&DOG2_ANIMATION_DATA_ROM_END,
     .animationDataSize = 0x8094,
     .anotherAssetIndex = 0xff080000,
     },
    {
     .name = " ABOXO ",
     .displayListStart = (void *)&ABOXO_DISPLAY_LIST_ROM_START,
     .displayListEnd = (void *)&ABOXO_DISPLAY_LIST_ROM_END,
     .compressedDataStart = (void *)&ABOXO_COMPRESSED_DATA_ROM_START,
     .compressedDataEnd = (void *)&ABOXO_COMPRESSED_DATA_ROM_END,
     .decompressedSize = 0xD10,
     .unk1C = (void *)0x80088710,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .anotherAssetIndex = 0xff000000,
     .initCallback = (void *)0x80006E60,
     },
    {
     .name = " KEY O ",
     .displayListStart = (void *)&_1FD350_ROM_START,
     .displayListEnd = (void *)&_1FD350_ROM_END,
     .compressedDataStart = (void *)&_4CE2A0_ROM_START,
     .compressedDataEnd = (void *)&_4CE2A0_ROM_END,
     .decompressedSize = 0x4A0,
     .unk1C = (void *)0x80088730,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .padding = 0x0003,
     .animationDataStart = (void *)&_5BD9F0_ROM_START,
     .animationDataEnd = (void *)&_5BD9F0_ROM_END,
     .animationDataSize = 0x108,
     .anotherAssetIndex = 0xff000000,
     },
    {
     .name = " KEY L ",
     .displayListStart = (void *)&_1FD500_ROM_START,
     .displayListEnd = (void *)0x001FD6B0,
     .compressedDataStart = (void *)&_4CE500_ROM_START,
     .compressedDataEnd = (void *)&_4CE500_ROM_END,
     .decompressedSize = 0x4A0,
     .unk1C = (void *)0x80088740,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .padding = 0x0003,
     .animationDataStart = (void *)&_5BD9F0_ROM_START,
     .animationDataEnd = (void *)&_5BD9F0_ROM_END,
     .animationDataSize = 0x108,
     .anotherAssetIndex = 0xff000000,
     },
    {
     .name = " ABOXL ",
     .displayListStart = (void *)&ABOXL_DISPLAY_LIST_ROM_START,
     .displayListEnd = (void *)&ABOXL_DISPLAY_LIST_ROM_END,
     .compressedDataStart = (void *)&ABOXL_COMPRESSED_DATA_ROM_START,
     .compressedDataEnd = (void *)&ABOXL_COMPRESSED_DATA_ROM_END,
     .decompressedSize = 0x710,
     .unk1C = (void *)0x80088700,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .anotherAssetIndex = 0xff000000,
     },
    {
     .name = " KANI  ",
     .displayListStart = (void *)&_1FC700_ROM_START,
     .displayListEnd = (void *)&_1FC700_ROM_END,
     .compressedDataStart = (void *)&_4CCA00_ROM_START,
     .compressedDataEnd = (void *)&_4CCA00_ROM_END,
     .decompressedSize = 0x770,
     .unk1C = (void *)0x800886E0,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .anotherAssetIndex = 0xff040000,
     .initCallback = (void *)0x80007380,
     },
    {
     .name = " NEZU  ",
     .displayListStart = (void *)&_1FD6B0_ROM_START,
     .displayListEnd = (void *)&_1FD6B0_ROM_END,
     .compressedDataStart = (void *)&_4CE760_ROM_START,
     .compressedDataEnd = (void *)&_4CE760_ROM_END,
     .decompressedSize = 0x1080,
     .unk1C = (void *)0x80088750,
     .numAssets = (s8)0x02,
     .assetGroupIndex = (s8)-0x01,
     .padding = 0x017C,
     .animationDataStart = (void *)&_5B4130_ROM_START,
     .animationDataEnd = (void *)&_5B4130_ROM_END,
     .animationDataSize = 0x1FE0,
     .anotherAssetIndex = 0xff050000,
     },
    {
     .name = " CAKEE ",
     .displayListStart = (void *)&CAKEE_DISPLAY_LIST_ROM_START,
     .displayListEnd = (void *)&CAKEE_DISPLAY_LIST_ROM_END,
     .compressedDataStart = (void *)&CAKEE_COMPRESSED_DATA_ROM_START,
     .compressedDataEnd = (void *)&CAKEE_COMPRESSED_DATA_ROM_END,
     .decompressedSize = 0xF80,
     .unk1C = (void *)0x800886F0,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .anotherAssetIndex = 0xff000000,
     },
    {
     .name = " CAKE  ",
     .displayListStart = (void *)&CAKE_DISPLAY_LIST_ROM_START,
     .displayListEnd = (void *)&CAKE_DISPLAY_LIST_ROM_END,
     .compressedDataStart = (void *)&CAKE_COMPRESSED_DATA_ROM_START,
     .compressedDataEnd = (void *)&CAKE_COMPRESSED_DATA_ROM_END,
     .decompressedSize = 0x1620,
     .unk1C = (void *)0x800886D0,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .anotherAssetIndex = 0xff000000,
     .soundSequenceDataStart = (void *)&CAKE_SOUND_SEQUENCE_DATA_ROM_START,
     .soundSequenceDataEnd = (void *)&CAKE_SOUND_SEQUENCE_DATA_ROM_END,
     .soundSequenceDataSize = 0x468,
     .initCallback = (void *)0x80007070,
     },
    {
     .name = " POSP  ",
     .displayListStart = (void *)&_1FC140_ROM_START,
     .displayListEnd = (void *)&_1FC140_ROM_END,
     .compressedDataStart = (void *)&_4CB700_ROM_START,
     .compressedDataEnd = (void *)&_4CB700_ROM_END,
     .decompressedSize = 0x8E0,
     .unk1C = (void *)0x800886C0,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .anotherAssetIndex = 0xff030000,
     },
    {
     .name = " POST  ",
     .displayListStart = (void *)&_1FBF30_ROM_START,
     .displayListEnd = (void *)&_1FBF30_ROM_END,
     .compressedDataStart = (void *)&_4CB1E0_ROM_START,
     .compressedDataEnd = (void *)&_4CB1E0_ROM_END,
     .decompressedSize = 0x7A0,
     .unk1C = (void *)0x800886B0,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .anotherAssetIndex = 0xff030000,
     },
    {
     .name = " NEWS  ",
     .displayListStart = (void *)&_1FBE00_ROM_START,
     .displayListEnd = (void *)&_1FBE00_ROM_END,
     .compressedDataStart = (void *)&_4CAD30_ROM_START,
     .compressedDataEnd = (void *)&_4CAD30_ROM_END,
     .decompressedSize = 0x620,
     .unk1C = (void *)0x800886A0,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .anotherAssetIndex = 0xff000000,
     },
    {
     .name = " SLASH3",
     .numAssets = (s8)0x10,
     .padding = 0x016C,
     .animationDataStart = (void *)&_4F45E0_ROM_START,
     .animationDataEnd = (void *)&_4F45E0_ROM_END,
     .animationDataSize = 0xF51A,
     .anotherAssetIndex = 0xff050000,
     .Assets = (void *)SLASH_ASSETS,
     .count = 4,
     },
    {
     .name = " NANCY3",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x01,
     .padding = 0x015A,
     .animationDataStart = (void *)&_51BCF0_ROM_START,
     .animationDataEnd = (void *)&_51BCF0_ROM_END,
     .animationDataSize = 0xE65E,
     .anotherAssetIndex = 0xff050000,
     .Assets = (void *)NANCY_ASSETS,
     .count = 4,
     },
    {
     .name = " JAM3  ",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x02,
     .padding = 0x0127,
     .animationDataStart = (void *)&JAM3_ANIMATION_DATA_ROM_START,
     .animationDataEnd = (void *)&JAM3_ANIMATION_DATA_ROM_END,
     .animationDataSize = 0xDB44,
     .anotherAssetIndex = 0xff060000,
     .Assets = (void *)JAM_ASSETS,
     .count = 4,
     },
    {
     .name = " LINDA3",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x03,
     .padding = 0x0137,
     .animationDataStart = (void *)&_550BC0_ROM_START,
     .animationDataEnd = (void *)&_550BC0_ROM_END,
     .animationDataSize = 0xDAD4,
     .anotherAssetIndex = 0xff050000,
     .Assets = (void *)LINDA_ASSETS,
     .count = 4,
     },
    {
     .name = " TOMMY3",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x04,
     .padding = 0x0177,
     .animationDataStart = (void *)&_568A70_ROM_START,
     .animationDataEnd = (void *)&_568A70_ROM_END,
     .animationDataSize = 0x127A2,
     .anotherAssetIndex = 0xff070000,
     .Assets = (void *)TOMMY_ASSETS,
     .count = 4,
     },
    {
     .name = " WENDY3",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x05,
     .padding = 0x0176,
     .animationDataStart = (void *)&_584650_ROM_START,
     .animationDataEnd = (void *)&_584650_ROM_END,
     .animationDataSize = 0x12DD2,
     .anotherAssetIndex = 0xff050000,
     .Assets = (void *)WENDY_ASSETS,
     .count = 4,
     },
    {
     .name = " BLACK3",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x06,
     .padding = 0x0132,
     .animationDataStart = (void *)&BLACK3_ANIMATION_DATA_ROM_START,
     .animationDataEnd = (void *)&BLACK3_ANIMATION_DATA_ROM_END,
     .animationDataSize = 0x11794,
     .anotherAssetIndex = 0xff040000,
     .Assets = (void *)BLACK_ASSETS,
     .count = 4,
     },
    {
     .name = " RADIO ",
     .displayListStart = (void *)&_1EE140_ROM_START,
     .displayListEnd = (void *)&_1EE140_ROM_END,
     .compressedDataStart = (void *)&_4A6920_ROM_START,
     .compressedDataEnd = (void *)&_4A6920_ROM_END,
     .decompressedSize = 0x620,
     .unk1C = (void *)0x80088340,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .anotherAssetIndex = 0xff050000,
     .initCallback = (void *)0x80006AB0,
     },
    {
     .name = " TABLE ",
     .displayListStart = (void *)&_1EDEE0_ROM_START,
     .displayListEnd = (void *)&_1EDEE0_ROM_END,
     .compressedDataStart = (void *)&_4A6360_ROM_START,
     .compressedDataEnd = (void *)&_4A6360_ROM_END,
     .decompressedSize = 0xC20,
     .unk1C = (void *)0x80088330,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .anotherAssetIndex = 0xff080000,
     },
    {
     .name = " TUTUM ",
     .displayListStart = (void *)&_1EDCF0_ROM_START,
     .displayListEnd = (void *)&_1EDCF0_ROM_END,
     .compressedDataStart = (void *)&_4A5E10_ROM_START,
     .compressedDataEnd = (void *)&_4A5E10_ROM_END,
     .decompressedSize = 0x930,
     .unk1C = (void *)0x80088320,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .anotherAssetIndex = 0xff000000,
     },
    {
     .name = " SLASHS",
     .numAssets = (s8)0x10,
     .padding = 0x0008,
     .animationDataStart = (void *)&_45B130_ROM_START,
     .animationDataEnd = (void *)&_45B130_ROM_END,
     .animationDataSize = 0x356E,
     .anotherAssetIndex = 0xff050000,
     .Assets = (void *)SLASH_ASSETS,
     .count = 4,
     },
    {
     .name = " NANCYS",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x01,
     .padding = 0x0008,
     .animationDataStart = (void *)&_45D620_ROM_START,
     .animationDataEnd = (void *)&_45D620_ROM_END,
     .animationDataSize = 0x32A4,
     .anotherAssetIndex = 0xff050000,
     .Assets = (void *)NANCY_ASSETS,
     .count = 4,
     },
    {
     .name = " JAMS  ",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x02,
     .padding = 0x0008,
     .animationDataStart = (void *)&JAMS_ANIMATION_DATA_ROM_START,
     .animationDataEnd = (void *)&JAMS_ANIMATION_DATA_ROM_END,
     .animationDataSize = 0x477C,
     .anotherAssetIndex = 0xff060000,
     .Assets = (void *)JAM_ASSETS,
     .count = 4,
     },
    {
     .name = " LINDAS",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x03,
     .padding = 0x0008,
     .animationDataStart = (void *)&_462B50_ROM_START,
     .animationDataEnd = (void *)&_462B50_ROM_END,
     .animationDataSize = 0x3BEC,
     .anotherAssetIndex = 0xff050000,
     .Assets = (void *)LINDA_ASSETS,
     .count = 4,
     },
    {
     .name = " TOMMYS",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x04,
     .padding = 0x0008,
     .animationDataStart = (void *)&_464DD0_ROM_START,
     .animationDataEnd = (void *)&_464DD0_ROM_END,
     .animationDataSize = 0x341E,
     .anotherAssetIndex = 0xff070000,
     .Assets = (void *)TOMMY_ASSETS,
     .count = 4,
     },
    {
     .name = " WENDYS",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x05,
     .padding = 0x000A,
     .animationDataStart = (void *)&_4666C0_ROM_START,
     .animationDataEnd = (void *)&_4666C0_ROM_END,
     .animationDataSize = 0x4E52,
     .anotherAssetIndex = 0xff050000,
     .Assets = (void *)WENDY_ASSETS,
     .count = 4,
     },
    {
     .name = " BLACKS",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x06,
     .padding = 0x000A,
     .animationDataStart = (void *)&BLACKS_ANIMATION_DATA_ROM_START,
     .animationDataEnd = (void *)&BLACKS_ANIMATION_DATA_ROM_END,
     .animationDataSize = 0x58A4,
     .anotherAssetIndex = 0xff040000,
     .Assets = (void *)BLACK_ASSETS,
     .count = 4,
     },
    {
     .name = " PENS  ",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x07,
     .padding = 0x0005,
     .animationDataStart = (void *)&PENS_ANIMATION_DATA_ROM_START,
     .animationDataEnd = (void *)&PENS_ANIMATION_DATA_ROM_END,
     .animationDataSize = 0x35BC,
     .anotherAssetIndex = 0xff040000,
     .Assets = (void *)PEN_ASSETS,
     .count = 4,
     },
    {
     .name = " DOGS  ",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x08,
     .padding = 0x0018,
     .animationDataStart = (void *)&DOGS_ANIMATION_DATA_ROM_START,
     .animationDataEnd = (void *)&DOGS_ANIMATION_DATA_ROM_END,
     .animationDataSize = 0xD8F2,
     .anotherAssetIndex = 0xff080000,
     },
    {
     .name = " DISH  ",
     .displayListStart = (void *)&DISH_DISPLAY_LIST_ROM_START,
     .displayListEnd = (void *)&DISH_DISPLAY_LIST_ROM_END,
     .compressedDataStart = (void *)&DISH_COMPRESSED_DATA_ROM_START,
     .compressedDataEnd = (void *)&DISH_COMPRESSED_DATA_ROM_END,
     .decompressedSize = 0x320,
     .unk1C = (void *)0x80088310,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .anotherAssetIndex = 0xff000000,
     .initCallback = (void *)0x800081C0,
     },
    {
     .name = " SNOW3 ",
     .displayListStart = (void *)&_1EF1C0_ROM_START,
     .displayListEnd = (void *)&_1EF1C0_ROM_END,
     .compressedDataStart = (void *)&_4A79C0_ROM_START,
     .compressedDataEnd = (void *)&_4A79C0_ROM_END,
     .decompressedSize = 0xEB0,
     .unk1C = (void *)0x800883A0,
     .numAssets = (s8)0x03,
     .assetGroupIndex = (s8)-0x01,
     .padding = 0x0136,
     .animationDataStart = (void *)&_5B4B50_ROM_START,
     .animationDataEnd = (void *)&_5B4B50_ROM_END,
     .animationDataSize = 0x1CDA,
     .anotherAssetIndex = 0xff110000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x00000000,
     .count = 0,
     .countPadding = { 0x01, 0x00, 0x00 },
     },
    {
     .name = " SNOW4 ",
     .numAssets = (s8)0x03,
     .assetGroupIndex = (s8)0x0A,
     .padding = 0x0136,
     .animationDataStart = (void *)&_5B4B50_ROM_START,
     .animationDataEnd = (void *)&_5B4B50_ROM_END,
     .animationDataSize = 0x1CDA,
     .anotherAssetIndex = 0xff110000,
     .count = 0,
     },
    {
     .name = " PBOX0 ",
     .displayListStart = (void *)&_1ED5F0_ROM_START,
     .displayListEnd = (void *)&_1ED5F0_ROM_END,
     .compressedDataStart = (void *)&_4A5460_ROM_START,
     .compressedDataEnd = (void *)&_4A5460_ROM_END,
     .decompressedSize = 0x370,
     .unk1C = (void *)0x800882D0,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .anotherAssetIndex = 0xff020000,
     .initCallback = (void *)0x800078C4,
     },
    {
     .name = " PBOX1 ",
     .displayListStart = (void *)&_1ED740_ROM_START,
     .displayListEnd = (void *)&_1ED740_ROM_END,
     .compressedDataStart = (void *)&_4A5650_ROM_START,
     .compressedDataEnd = (void *)&_4A5650_ROM_END,
     .decompressedSize = 0x370,
     .unk1C = (void *)0x800882E0,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .anotherAssetIndex = 0xff020000,
     .initCallback = (void *)0x800078C4,
     },
    {
     .name = " USAGI ",
     .displayListStart = (void *)&_1ED370_ROM_START,
     .displayListEnd = (void *)&_1ED370_ROM_END,
     .compressedDataStart = (void *)&_4A51C0_ROM_START,
     .compressedDataEnd = (void *)&_4A51C0_ROM_END,
     .decompressedSize = 0x5E0,
     .unk1C = (void *)0x800882C0,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .anotherAssetIndex = 0xff040000,
     .initCallback = (void *)0x80007B00,
     },
    {
     .name = " SLASH4",
     .numAssets = (s8)0x10,
     .padding = 0x015D,
     .animationDataStart = (void *)&_4FDE20_ROM_START,
     .animationDataEnd = (void *)&_4FDE20_ROM_END,
     .animationDataSize = 0xAFF4,
     .anotherAssetIndex = 0xff050000,
     .Assets = (void *)SLASH_ASSETS,
     .count = 4,
     },
    {
     .name = " SLASH5",
     .numAssets = (s8)0x10,
     .padding = 0x0138,
     .animationDataStart = (void *)&_504740_ROM_START,
     .animationDataEnd = (void *)&_504740_ROM_END,
     .animationDataSize = 0xC5C0,
     .anotherAssetIndex = 0xff050000,
     .Assets = (void *)SLASH_ASSETS,
     .count = 4,
     },
    {
     .name = " HELI  ",
     .displayListStart = (void *)&HELI_DISPLAY_LIST_ROM_START,
     .displayListEnd = (void *)&HELI_DISPLAY_LIST_ROM_END,
     .compressedDataStart = (void *)&HELI_COMPRESSED_DATA_ROM_START,
     .compressedDataEnd = (void *)&HELI_COMPRESSED_DATA_ROM_END,
     .decompressedSize = 0x1A80,
     .unk1C = (void *)0x800882A0,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .anotherAssetIndex = 0xff160000,
     .initCallback = (void *)0x80007E40,
     },
    {
     .name = " BURG  ",
     .displayListStart = (void *)&BURG_DISPLAY_LIST_ROM_START,
     .displayListEnd = (void *)&BURG_DISPLAY_LIST_ROM_END,
     .compressedDataStart = (void *)&BURG_COMPRESSED_DATA_ROM_START,
     .compressedDataEnd = (void *)&BURG_COMPRESSED_DATA_ROM_END,
     .decompressedSize = 0x620,
     .unk1C = (void *)0x80088290,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .anotherAssetIndex = 0xff010000,
     },
    {
     .name = " MROBO ",
     .displayListStart = (void *)&_1EAEF0_ROM_START,
     .displayListEnd = (void *)&_1EAEF0_ROM_END,
     .compressedDataStart = (void *)&_4A2BC0_ROM_START,
     .compressedDataEnd = (void *)&_4A2BC0_ROM_END,
     .decompressedSize = 0x4070,
     .unk1C = (void *)0x800881F0,
     .numAssets = (s8)0x0A,
     .assetGroupIndex = (s8)-0x01,
     .padding = 0x00A0,
     .animationDataStart = (void *)&_5B7D30_ROM_START,
     .animationDataEnd = (void *)&_5B7D30_ROM_END,
     .animationDataSize = 0x3F40,
     .anotherAssetIndex = 0xff040000,
     },
    {
     .name = " SLASHO",
     .numAssets = (s8)0x10,
     .padding = 0x00A0,
     .animationDataStart = (void *)&_50BFD0_ROM_START,
     .animationDataEnd = (void *)&_50BFD0_ROM_END,
     .animationDataSize = 0x737A,
     .anotherAssetIndex = 0xff050000,
     .Assets = (void *)SLASH_ASSETS,
     .count = 4,
     },
    {
     .name = " NANCYO",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x01,
     .padding = 0x00A0,
     .animationDataStart = (void *)&_5242F0_ROM_START,
     .animationDataEnd = (void *)&_5242F0_ROM_END,
     .animationDataSize = 0x6CEE,
     .anotherAssetIndex = 0xff050000,
     .Assets = (void *)NANCY_ASSETS,
     .count = 4,
     },
    {
     .name = " JAMO  ",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x02,
     .padding = 0x00A0,
     .animationDataStart = (void *)&JAMO_ANIMATION_DATA_ROM_START,
     .animationDataEnd = (void *)&JAMO_ANIMATION_DATA_ROM_END,
     .animationDataSize = 0x66FC,
     .anotherAssetIndex = 0xff060000,
     .Assets = (void *)JAM_ASSETS,
     .count = 4,
     },
    {
     .name = " LINDAO",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x03,
     .padding = 0x00A0,
     .animationDataStart = (void *)&_5596D0_ROM_START,
     .animationDataEnd = (void *)&_5596D0_ROM_END,
     .animationDataSize = 0x8938,
     .anotherAssetIndex = 0xff050000,
     .Assets = (void *)LINDA_ASSETS,
     .count = 4,
     },
    {
     .name = " TOMMYO",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x04,
     .padding = 0x00A0,
     .animationDataStart = (void *)&_573660_ROM_START,
     .animationDataEnd = (void *)&_573660_ROM_END,
     .animationDataSize = 0x5498,
     .anotherAssetIndex = 0xff070000,
     .Assets = (void *)TOMMY_ASSETS,
     .count = 4,
     },
    {
     .name = " WENDYO",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x05,
     .padding = 0x00C8,
     .animationDataStart = (void *)&_590790_ROM_START,
     .animationDataEnd = (void *)&_590790_ROM_END,
     .animationDataSize = 0x5E66,
     .anotherAssetIndex = 0xff050000,
     .Assets = (void *)WENDY_ASSETS,
     .count = 4,
     },
    {
     .name = " BLACKO",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x06,
     .padding = 0x00A0,
     .animationDataStart = (void *)&BLACKO_ANIMATION_DATA_ROM_START,
     .animationDataEnd = (void *)&BLACKO_ANIMATION_DATA_ROM_END,
     .animationDataSize = 0x53D6,
     .anotherAssetIndex = 0xff040000,
     .Assets = (void *)BLACK_ASSETS,
     .count = 4,
     },
    {
     .name = " PENO  ",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x07,
     .padding = 0x0174,
     .animationDataStart = (void *)&PEN_ANIMATION_DATA_ROM_START,
     .animationDataEnd = (void *)&PEN_ANIMATION_DATA_ROM_END,
     .animationDataSize = 0xFE66,
     .anotherAssetIndex = 0xff040000,
     .Assets = (void *)PEN_ASSETS,
     .count = 4,
     },
    {
     .name = " DOGO  ",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x08,
     .padding = 0x0020,
     .animationDataStart = (void *)&DOGO_ANIMATION_DATA_ROM_START,
     .animationDataEnd = (void *)&DOGO_ANIMATION_DATA_ROM_END,
     .animationDataSize = 0xB54,
     .anotherAssetIndex = 0xff080000,
     },
    {
     .name = " BOMB2 ",
     .displayListStart = (void *)&BOMB2_DISPLAY_LIST_ROM_START,
     .displayListEnd = (void *)&BOMB2_DISPLAY_LIST_ROM_END,
     .compressedDataStart = (void *)&BOMB2_COMPRESSED_DATA_ROM_START,
     .compressedDataEnd = (void *)&BOMB2_COMPRESSED_DATA_ROM_END,
     .decompressedSize = 0x7E0,
     .unk1C = (void *)0x800881A0,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .anotherAssetIndex = 0xff020000,
     .soundSequenceDataStart = (void *)&BOMB2_SOUND_SEQUENCE_DATA_ROM_START,
     .soundSequenceDataEnd = (void *)&BOMB2_SOUND_SEQUENCE_DATA_ROM_END,
     .soundSequenceDataSize = 0x358,
     .initCallback = (void *)0x80006940,
     },
    {
     .name = " SLASHN",
     .numAssets = (s8)0x10,
     .padding = 0x0020,
     .animationDataStart = (void *)&_476EB0_ROM_START,
     .animationDataEnd = (void *)&_476EB0_ROM_END,
     .animationDataSize = 0x8274,
     .anotherAssetIndex = 0xff050000,
     .Assets = (void *)SLASH_ASSETS,
     .count = 4,
     },
    {
     .name = " NANCYN",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x01,
     .padding = 0x001A,
     .animationDataStart = (void *)&_47B2D0_ROM_START,
     .animationDataEnd = (void *)&_47B2D0_ROM_END,
     .animationDataSize = 0x6CA4,
     .anotherAssetIndex = 0xff050000,
     .Assets = (void *)NANCY_ASSETS,
     .count = 4,
     },
    {
     .name = " JAMN  ",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x02,
     .padding = 0x001B,
     .animationDataStart = (void *)&JAMN_ANIMATION_DATA_ROM_START,
     .animationDataEnd = (void *)&JAMN_ANIMATION_DATA_ROM_END,
     .animationDataSize = 0x6A3C,
     .anotherAssetIndex = 0xff060000,
     .Assets = (void *)JAM_ASSETS,
     .count = 4,
     },
    {
     .name = " LINDAN",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x03,
     .padding = 0x0017,
     .animationDataStart = (void *)&_482E40_ROM_START,
     .animationDataEnd = (void *)&_482E40_ROM_END,
     .animationDataSize = 0x7D5A,
     .anotherAssetIndex = 0xff050000,
     .Assets = (void *)LINDA_ASSETS,
     .count = 4,
     },
    {
     .name = " TOMMYN",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x04,
     .padding = 0x0024,
     .animationDataStart = (void *)&_4875E0_ROM_START,
     .animationDataEnd = (void *)&_4875E0_ROM_END,
     .animationDataSize = 0xD8B6,
     .anotherAssetIndex = 0xff070000,
     .Assets = (void *)TOMMY_ASSETS,
     .count = 4,
     },
    {
     .name = " WENDYN",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x05,
     .padding = 0x0019,
     .animationDataStart = (void *)&_48E8F0_ROM_START,
     .animationDataEnd = (void *)&_48E8F0_ROM_END,
     .animationDataSize = 0x800E,
     .anotherAssetIndex = 0xff050000,
     .Assets = (void *)WENDY_ASSETS,
     .count = 4,
     },
    {
     .name = " BLACKN",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x06,
     .padding = 0x0017,
     .animationDataStart = (void *)&BLACKN_ANIMATION_DATA_ROM_START,
     .animationDataEnd = (void *)&BLACKN_ANIMATION_DATA_ROM_END,
     .animationDataSize = 0x8F0C,
     .anotherAssetIndex = 0xff040000,
     .Assets = (void *)BLACK_ASSETS,
     .count = 4,
     },
    {
     .name = " PENN  ",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x07,
     .padding = 0x000D,
     .animationDataStart = (void *)&PENN_ANIMATION_DATA_ROM_START,
     .animationDataEnd = (void *)&PENN_ANIMATION_DATA_ROM_END,
     .animationDataSize = 0x252E,
     .anotherAssetIndex = 0xff040000,
     .Assets = (void *)PEN_ASSETS,
     .count = 4,
     },
    {
     .name = " DOGN  ",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x08,
     .padding = 0x000D,
     .animationDataStart = (void *)&DOGN_ANIMATION_DATA_ROM_START,
     .animationDataEnd = (void *)&DOGN_ANIMATION_DATA_ROM_END,
     .animationDataSize = 0x3668,
     .anotherAssetIndex = 0xff080000,
     },
    {
     .name = " DOOR  ",
     .displayListStart = (void *)&DOOR_DISPLAY_LIST_ROM_START,
     .displayListEnd = (void *)&DOOR_DISPLAY_LIST_ROM_END,
     .compressedDataStart = (void *)&DOOR_COMPRESSED_DATA_ROM_START,
     .compressedDataEnd = (void *)&DOOR_COMPRESSED_DATA_ROM_END,
     .decompressedSize = 0xF50,
     .unk1C = (void *)0x80088190,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .anotherAssetIndex = 0xff000000,
     },
    {
     .name = " LTBL0 ",
     .displayListStart = (void *)&_1E9490_ROM_START,
     .displayListEnd = (void *)&_1E9490_ROM_END,
     .compressedDataStart = (void *)&_49F270_ROM_START,
     .compressedDataEnd = (void *)&_49F270_ROM_END,
     .decompressedSize = 0xBC0,
     .unk1C = (void *)0x80088170,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .anotherAssetIndex = 0xff090000,
     },
    {
     .name = " LTBL1 ",
     .displayListStart = (void *)&_1E9300_ROM_START,
     .displayListEnd = (void *)&_1E9300_ROM_END,
     .compressedDataStart = (void *)&_49F0D0_ROM_START,
     .compressedDataEnd = (void *)&_49F0D0_ROM_END,
     .decompressedSize = 0x4A0,
     .unk1C = (void *)0x80088160,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .anotherAssetIndex = 0xff090000,
     },
    {
     .name = " GHOST ",
     .assetGroupIndex = (s8)-0x01,
     .anotherAssetIndex = 0xff000000,
     .soundSequenceDataStart = (void *)&GHOST_SOUND_SEQUENCE_DATA_ROM_START,
     .soundSequenceDataEnd = (void *)&GHOST_SOUND_SEQUENCE_DATA_ROM_END,
     .soundSequenceDataSize = 0xA78,
     .initCallback = (void *)0x80006C50,
     },
    {
     .name = " POST2 ",
     .assetGroupIndex = (s8)-0x01,
     .anotherAssetIndex = 0xff030000,
     .soundSequenceDataStart = (void *)&_663010_ROM_START,
     .soundSequenceDataEnd = (void *)&_663010_ROM_END,
     .soundSequenceDataSize = 0x848,
     .initCallback = (void *)0x80008310,
     },
    {
     .name = " LTBL2 ",
     .displayListStart = (void *)&_1E8A90_ROM_START,
     .displayListEnd = (void *)&_1E8A90_ROM_END,
     .compressedDataStart = (void *)&_49D8F0_ROM_START,
     .compressedDataEnd = (void *)&_49D8F0_ROM_END,
     .decompressedSize = 0x1240,
     .unk1C = (void *)0x80088140,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .anotherAssetIndex = 0xff090000,
     },
    {
     .name = " LTBL3 ",
     .displayListStart = (void *)&_1E8900_ROM_START,
     .displayListEnd = (void *)&_1E8900_ROM_END,
     .compressedDataStart = (void *)&_49D710_ROM_START,
     .compressedDataEnd = (void *)&_49D710_ROM_END,
     .decompressedSize = 0x4A0,
     .unk1C = (void *)0x80088130,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .anotherAssetIndex = 0xff090000,
     },
    {
     .name = " XXXXX ",
     .assetGroupIndex = (s8)-0x01,
     .anotherAssetIndex = 0xff040000,
     },
    {
     .name = " XXXXX ",
     .assetGroupIndex = (s8)-0x01,
     .anotherAssetIndex = 0xff040000,
     },
    {
     .name = " XXXXX ",
     .assetGroupIndex = (s8)-0x01,
     .anotherAssetIndex = 0xff080000,
     },
    {
     .name = " XXXXX ",
     .assetGroupIndex = (s8)-0x01,
     .anotherAssetIndex = 0xff000000,
     },
    {
     .name = " SLASH ",
     .numAssets = (s8)0x10,
     .padding = 0x0011,
     .animationDataStart = (void *)&CHARACTER_SELECT_ANIMATION_DATA_ROM_START,
     .animationDataEnd = (void *)&CHARACTER_SELECT_ANIMATION_DATA_ROM_END,
     .animationDataSize = 0x7E3A,
     .anotherAssetIndex = 0xff050000,
     },
    {
     .name = " NANCY ",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x01,
     .padding = 0x0011,
     .animationDataStart = (void *)&CHARACTER_SELECT_ANIMATION_DATA_ROM_START,
     .animationDataEnd = (void *)&CHARACTER_SELECT_ANIMATION_DATA_ROM_END,
     .animationDataSize = 0x7E3A,
     .anotherAssetIndex = 0xff050000,
     },
    {
     .name = " JAM   ",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x02,
     .padding = 0x0011,
     .animationDataStart = (void *)&CHARACTER_SELECT_ANIMATION_DATA_ROM_START,
     .animationDataEnd = (void *)&CHARACTER_SELECT_ANIMATION_DATA_ROM_END,
     .animationDataSize = 0x7E3A,
     .anotherAssetIndex = 0xff060000,
     },
    {
     .name = " LINDA ",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x03,
     .padding = 0x0011,
     .animationDataStart = (void *)&CHARACTER_SELECT_ANIMATION_DATA_ROM_START,
     .animationDataEnd = (void *)&CHARACTER_SELECT_ANIMATION_DATA_ROM_END,
     .animationDataSize = 0x7E3A,
     .anotherAssetIndex = 0xff050000,
     },
    {
     .name = " TOMMY ",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x04,
     .padding = 0x0011,
     .animationDataStart = (void *)&CHARACTER_SELECT_ANIMATION_DATA_ROM_START,
     .animationDataEnd = (void *)&CHARACTER_SELECT_ANIMATION_DATA_ROM_END,
     .animationDataSize = 0x7E3A,
     .anotherAssetIndex = 0xff070000,
     },
    {
     .name = " WENDY ",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x05,
     .padding = 0x0011,
     .animationDataStart = (void *)&CHARACTER_SELECT_ANIMATION_DATA_ROM_START,
     .animationDataEnd = (void *)&CHARACTER_SELECT_ANIMATION_DATA_ROM_END,
     .animationDataSize = 0x7E3A,
     .anotherAssetIndex = 0xff050000,
     },
    {
     .name = " BLACK ",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x06,
     .padding = 0x0011,
     .animationDataStart = (void *)&CHARACTER_SELECT_ANIMATION_DATA_ROM_START,
     .animationDataEnd = (void *)&CHARACTER_SELECT_ANIMATION_DATA_ROM_END,
     .animationDataSize = 0x7E3A,
     .anotherAssetIndex = 0xff040000,
     },
    {
     .name = " PEN   ",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x07,
     .padding = 0x0011,
     .animationDataStart = (void *)&CHARACTER_SELECT_ANIMATION_DATA_ROM_START,
     .animationDataEnd = (void *)&CHARACTER_SELECT_ANIMATION_DATA_ROM_END,
     .animationDataSize = 0x7E3A,
     .anotherAssetIndex = 0xff040000,
     },
    {
     .name = " DOG   ",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x08,
     .padding = 0x0011,
     .animationDataStart = (void *)&CHARACTER_SELECT_ANIMATION_DATA_ROM_START,
     .animationDataEnd = (void *)&CHARACTER_SELECT_ANIMATION_DATA_ROM_END,
     .animationDataSize = 0x7E3A,
     .anotherAssetIndex = 0xff080000,
     },
    {
     .name = " DINOS ",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x09,
     .padding = 0x0011,
     .animationDataStart = (void *)&CHARACTER_SELECT_ANIMATION_DATA_ROM_START,
     .animationDataEnd = (void *)&CHARACTER_SELECT_ANIMATION_DATA_ROM_END,
     .animationDataSize = 0x7E3A,
     .anotherAssetIndex = 0xff190000,
     },
    {
     .name = " SNOW  ",
     .numAssets = (s8)0x03,
     .assetGroupIndex = (s8)0x0A,
     .padding = 0x0011,
     .animationDataStart = (void *)&CHARACTER_SELECT_ANIMATION_DATA_ROM_START,
     .animationDataEnd = (void *)&CHARACTER_SELECT_ANIMATION_DATA_ROM_END,
     .animationDataSize = 0x7E3A,
     .anotherAssetIndex = 0xff110000,
     },
    {
     .name = " ROBO  ",
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x0B,
     .padding = 0x0011,
     .animationDataStart = (void *)&CHARACTER_SELECT_ANIMATION_DATA_ROM_START,
     .animationDataEnd = (void *)&CHARACTER_SELECT_ANIMATION_DATA_ROM_END,
     .animationDataSize = 0x7E3A,
     .anotherAssetIndex = 0xff170000,
     },
    {
     .name = " MROBO ",
     .displayListStart = (void *)&_1EAEF0_ROM_START,
     .displayListEnd = (void *)&_1EAEF0_ROM_END,
     .compressedDataStart = (void *)&_4A2BC0_ROM_START,
     .compressedDataEnd = (void *)&_4A2BC0_ROM_END,
     .decompressedSize = 0x4070,
     .unk1C = (void *)0x800881F0,
     .numAssets = (s8)0x0A,
     .assetGroupIndex = (s8)-0x01,
     .padding = 0x0011,
     .animationDataStart = (void *)&CHARACTER_SELECT_ANIMATION_DATA_ROM_START,
     .animationDataEnd = (void *)&CHARACTER_SELECT_ANIMATION_DATA_ROM_END,
     .animationDataSize = 0x7E3A,
     .anotherAssetIndex = 0xff040000,
     },
    {
     .name = " USAGI ",
     .displayListStart = (void *)&_1ED370_ROM_START,
     .displayListEnd = (void *)&_1ED370_ROM_END,
     .compressedDataStart = (void *)&_4A51C0_ROM_START,
     .compressedDataEnd = (void *)&_4A51C0_ROM_END,
     .decompressedSize = 0x5E0,
     .unk1C = (void *)0x800882C0,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .anotherAssetIndex = 0xff040000,
     .initCallback = (void *)0x80007B00,
     },
    {
     .name = " BUTTE ",
     .assetGroupIndex = (s8)-0x01,
     .anotherAssetIndex = 0xff000000,
     .initCallback = (void *)0x800063B4,
     },
    {
     .name = " UFO   ",
     .displayListStart = (void *)&_1F8D10_ROM_START,
     .displayListEnd = (void *)&_1F8D10_ROM_END,
     .compressedDataStart = (void *)&_4C20A0_ROM_START,
     .compressedDataEnd = (void *)&_4C20A0_ROM_END,
     .decompressedSize = 0x25A0,
     .unk1C = (void *)0x800885C0,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .anotherAssetIndex = 0xff140000,
     .soundSequenceDataStart = (void *)&_4C3660_ROM_START,
     .soundSequenceDataEnd = (void *)&_4C3660_ROM_END,
     .soundSequenceDataSize = 0x838,
     .initCallback = (void *)0x80005808,
     },
    {
     .name = " KANI  ",
     .displayListStart = (void *)&_1FC700_ROM_START,
     .displayListEnd = (void *)&_1FC700_ROM_END,
     .compressedDataStart = (void *)&_4CCA00_ROM_START,
     .compressedDataEnd = (void *)&_4CCA00_ROM_END,
     .decompressedSize = 0x770,
     .unk1C = (void *)0x800886E0,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .anotherAssetIndex = 0xff040000,
     .initCallback = (void *)0x80007380,
     },
};

OSThread *__osActiveQueue_0 = (OSThread *)0x75;

char emptyAssetGroup[12] = "       \0\0\0";

u32 gDefaultEntityData8C938[18] = { 0xFFE00008, 0x0,        0x7F0FFF0,  0xFFFFFFFF, 0x200008,   0x0,
                                    0x7F007F0,  0xFFFFFFFF, 0x20FFF8,   0x0,        0xFFF007F0, 0xFFFFFFFF,
                                    0xFFE0FFF8, 0x0,        0xFFF0FFF0, 0xFFFFFFFF, 0x0,        0x0 };

void markEntityDestroyed(MarkDestroyedArg *arg0) {
    arg0->isDestroyed = 1;
}

void setModelHeight(SceneModel *arg0, s32 height) {
    arg0->height = height;
}

void disableEntityRendering(GameEntity *arg0) {
    arg0->renderEnabled = 0;
}

void enableEntityRendering(SceneModel *arg0) {
    arg0->renderEnabled = 1;
}

void enableModelShadow(SceneModel *arg0) {
    arg0->shadowEnabled = 1;
}

void disableModelShadow(SceneModel *arg0) {
    arg0->shadowEnabled = 0;
}

void setModelAlpha(SceneModel *arg0, u8 arg1) {
    arg0->alpha = arg1;
}

u8 getModelAlpha(SceneModel *arg0) {
    return arg0->alpha;
}

void applyTransformToModel(SceneModel *arg0, Transform3D *arg1) {
    if (arg0 != NULL) {
        memcpy(&arg0->unk18, arg1, sizeof(Transform3D));
    }
}

OSThread *__osGetActiveQueue(void) {
    return __osActiveQueue_0;
}

s32 getModelTransformPtr(s32 model) {
    return model + 0x18;
}

s16 getAnimationCount(s16 arg0) {
    return gameAssets[arg0].padding;
}

s8 getAssetCount(s16 arg0) {
    return gameAssets[arg0].numAssets;
}

void setModelActionMode(SceneModel *model, s8 actionMode) {
    model->actionMode = actionMode;
}

s8 getModelActionMode(getModelActionMode_arg *arg0) {
    return arg0->actionMode;
}

s32 getWalkAnimationId(s16 arg0) {
    return 0x98;
}

s32 getRunAnimationId(s16 arg0) {
    return 0x9B;
}

s32 func_800015EC_21EC(void) {
    return 6;
}

s32 getPlayerCount(void) {
    return 6;
}

void setModelVisibility(SceneModel *arg0, s8 arg1) {
    arg0->unk88 = arg1;
}

void setModelDisplayEnabled(SceneModel *arg0, s8 arg1) {
    arg0->displayEnabled = arg1;
}

void setItemDisplayEnabled(SceneModel *arg0, s8 arg1) {
    if (arg0->unk4C != -1) {
        arg0->unk4E = arg1;
    }
}

s32 getItemAssetCount(void) {
    return itemAssetCount;
}

void setItemFlags(SceneModel *arg0, s8 arg1) {
    arg0->unk4F = arg1;
}

s16 getAnimationFrameCount(SceneModel *model) {
    return model->animFrameCount;
}

void setModelPendingDestroy(SceneModel *arg0) {
    arg0->unk89 |= 1;
}

void clearModelPendingDestroy(SceneModel *arg0) {
    arg0->unk89 &= 0xFE;
}

s8 getAnimationIndex(SceneModel *arg0) {
    return arg0->animationIndex;
}

s8 getModelAnimationCount(SceneModel *arg0) {
    ModelAnimationData *animData;

    animData = arg0->unk9C;
    if (animData != NULL) {
        return animData->animationCount;
    }
    return 0;
}

void setAnimationIndex(SceneModel *arg0, s8 arg1) {
    if (arg1 < getModelAnimationCount(arg0)) {
        arg0->animationIndex = arg1;
    }
}

void setModelPartDisplayFlag(SceneModel *model, s32 partIndex) {
    model->partDisplayFlags = (s32)(model->partDisplayFlags | (1 << partIndex));
}

void clearModelPartDisplayFlag(SceneModel *model, s32 partIndex) {
    model->partDisplayFlags = (s32)(model->partDisplayFlags & ~(1 << partIndex));
}

void *loadAssetDataByMode(s16 groupIndex, s16 entityIndex, s16 mode) {
    AssetGroup *group;
    Asset *entity;

    group = &gameAssets[groupIndex];
    if (group->Assets == NULL) {
        return NULL;
    }

    if (entityIndex >= group->count) {
        return NULL;
    }

    entity = &group->Assets[entityIndex];
    if (entity == NULL) {
        return NULL;
    }

    switch (mode) {
        case MODE_DMA:
            return loadUncompressedData(entity->displayListStart, entity->displayListEnd);

        case MODE_QUEUED_DMA:
            return loadCompressedData(entity->compressedDataStart, entity->compressedDataEnd, entity->decompressedSize);

        case MODE_DIRECT_FETCH:
            return entity->directDataPtr;

        default:
            return NULL;
    }
}

s32 isModelVisible(func_80002B50_3750_arg *model) {
    s32 new_var;
    int new_var2 = 0;
    s32 isVisible = 0;

    if ((model != 0) && ((new_var = model->unk88) != new_var2)) {
        isVisible = model->displayEnabled != 0;
    }

    new_var = isVisible;
    return new_var;
}

void *loadAssetGroupSoundData(SceneModel *arg0) {
    s16 idx = arg0->index;
    AssetGroup *entry = &gameAssets[idx];
    void *allocatedSpaceStart = NULL;

    if (entry->soundSequenceDataStart != NULL) {
        allocatedSpaceStart = loadCompressedData(
            entry->soundSequenceDataStart,
            entry->soundSequenceDataEnd,
            entry->soundSequenceDataSize
        );
    }

    return allocatedSpaceStart;
}

void *loadAssetGroupDisplayList(SceneModel *arg0) {
    AssetGroup *entity = &gameAssets[arg0->index];
    return loadUncompressedData(entity->displayListStart, entity->displayListEnd);
}

void *loadAssetGroupCompressedData(SceneModel *arg0) {
    AssetGroup *entity = &gameAssets[arg0->index];
    return loadCompressedData(entity->compressedDataStart, entity->compressedDataEnd, entity->decompressedSize);
}

s32 hasModelGraphicsData(SceneModel *model) {
    return model->unk0->graphicsData != 0;
}

s32 isAssetGroupEmpty(s16 assetIndex) {
    return gameAssets[assetIndex].displayListStart == NULL;
}

AssetGroup *getAssetGroupOrDefault(s32 assetIndex) {
    if (assetIndex != -1) {
        return &gameAssets[assetIndex];
    }

    return (AssetGroup *)&emptyAssetGroup;
}

void clearModelAnimationState(SceneModel *model) {
    model->unk14 = -1;
    model->unk16 = -1;
    model->unk3A = -1;
    model->unk38 = -1;
    model->unk8E = -1;
}

void *createSceneModel(s32 assetGroupIndex, void *allocation) {
    return createSceneModelEx(assetGroupIndex, allocation, 0, -1, -1, -1);
}

void *
createSceneModelEx(s32 assetGroupIndex, void *allocation, s8 assetPairIndex, s8 param4, s8 param5, s16 assetIndex) {
    GameState *obj = allocateNodeMemory(0x160);
    if (obj != NULL) {
        initializeGameEntity(obj, assetGroupIndex, allocation, assetPairIndex, param4, param5, assetIndex);
    }
    return obj;
}

#ifdef NON_MATCHING
void initializeGameEntity(
    void *entity,
    s32 assetGroupIndex,
    void *param3,
    s8 assetPairIndex,
    s8 param5,
    s8 param6,
    s16 yetAnotherAssetIndex
) {
    GameEntity *ent = (GameEntity *)entity;
    AssetGroup *assetEntry;
    s32 i;
    AssetSlot *slot;
    void *asset1;
    void *asset2;
    ItemAssetEntry *node8A1A0;
    Node *task;

    assetEntry = &gameAssets[assetGroupIndex];

    ent->unk00 = allocateNodeMemory(0x780);
    ent->unk0C = assetGroupIndex;
    ent->unk14 = -1;
    ent->unk16 = -1;
    ent->unk0E = (s8)assetEntry->numAssets;
    ent->unk3A = -1;
    ent->unk38 = -1;
    ent->unk8E = -1;
    ent->unk89 = 0;
    ent->unk40 = 0;
    ent->unk44 = NULL;
    ent->unk48 = 0;
    ent->unk95 = 0;

    memcpy(&ent->asset2TransformationMatrix, identityMatrix, 0x20);

    ent->partDisplayFlags = -1;
    ent->alpha = 0xFF;
    ent->unk4C = yetAnotherAssetIndex;
    ent->shadowEnabled = 0;

    for (i = 0; i < 0x20; i++) {
        ent->unk00[i].unk20 = ent->unk00[i].asset1 = ent->unk00[i].asset2 = ent->unk00[i].asset3 = NULL;
    }

    if (assetEntry->displayListStart != NULL) {
        asset1 = loadUncompressedData(assetEntry->displayListStart, assetEntry->displayListEnd);
        asset2 = loadCompressedData(
            assetEntry->compressedDataStart,
            assetEntry->compressedDataEnd,
            assetEntry->decompressedSize
        );

        for (i = 0; i < assetEntry->numAssets; i++) {
            ent->unk00[i].unk20 = &assetEntry->unk1C[i];
            ent->unk00[i].asset1 = asset1;
            ent->unk00[i].asset2 = asset2;
            memcpy(ent->unk00[i].transformationMatrix, identityMatrix, 0x20);
        }
    } else if (assetEntry->assetGroupIndex != -1) {
        asset1 = loadAssetByIndex_94F90(assetEntry->assetGroupIndex, assetPairIndex);
        asset2 = loadAssetByIndex_95200(assetEntry->assetGroupIndex, assetPairIndex);

        for (i = 0; i < assetEntry->numAssets; i++) {
            ent->unk00[i].unk20 = &loadAssetByIndex_95380(assetEntry->assetGroupIndex, assetPairIndex)[i];
            ent->unk00[i].asset1 = asset1;
            ent->unk00[i].asset2 = asset2;
            memcpy(ent->unk00[i].transformationMatrix, identityMatrix, 0x20);
        }
    }

    if (assetEntry->padding) {
        if (assetEntry->anotherAssetIndex == -1) {
            ent->unk08 = loadCompressedData(
                assetEntry->animationDataStart,
                assetEntry->animationDataEnd,
                assetEntry->animationDataSize
            );
            ent->unk04 = allocateNodeMemory(0x980);
        } else {
            playSoundEffect(0);
        }
    } else {
        if (assetEntry->anotherAssetIndex != -1) {
            ent->unk08 = loadAssetByIndex_953E0(assetEntry->anotherAssetIndex);
            ent->unk04 = allocateNodeMemory(0x980);
        } else {
            ent->unk08 = NULL;
            ent->unk04 = NULL;
        }
    }

    if (param5 != -1) {
        ent->unk00[16].unk20 = loadAssetByIndex_95728(param5);
        ent->unk00[16].asset1 = loadAssetByIndex_95500(param5);
        ent->unk00[16].asset2 = loadAssetByIndex_95590(param5);
        ent->unk00[16].asset3 = loadAssetByIndex_95668(param6);

        memcpy(&ent->unk00[16].transformationMatrix, identityMatrix, 0x20);
    }

    if (yetAnotherAssetIndex != -1 && yetAnotherAssetIndex < itemAssetCount) {
        node8A1A0 = &itemAssetTable[yetAnotherAssetIndex];

        // ent->unk00[17].unk20 = node8A1A0->unk14;
        ent->unk00[17].asset1 = loadUncompressedData(node8A1A0->assetStart, node8A1A0->assetEnd);
        ent->unk00[17].asset2 = loadCompressedData(node8A1A0->asset2Start, node8A1A0->asset2End, node8A1A0->asset2Size);
        ent->unk00[17].asset3 = NULL;

        memcpy(&ent->unk00[17].transformationMatrix, identityMatrix, 0x20);
    }

    if (assetEntry->initCallback != NULL) {
        task = scheduleTask(assetEntry->initCallback, 0, 0, 0);
        if (task != NULL) {
            task->prev = (Node *)ent;
        }
    }

    ent->unk98 = allocateNodeMemory(0x3C);
    slot = ent->unk98;

    ent->unkA0 = NULL;

    slot->asset3 = NULL;
    slot->asset2 = NULL;
    slot->asset1 = NULL;

    ent->animationIndex = -1;
    ent->unk9C = NULL;

    if (assetEntry->Assets != NULL && assetPairIndex < assetEntry->count) {
        ent->unk9C = &assetEntry->Assets[assetPairIndex];
    }

    ent->unk98->asset1 = loadAssetDataByMode(assetGroupIndex, assetPairIndex, 0);
    ent->unk98->asset2 = loadAssetDataByMode(assetGroupIndex, assetPairIndex, 1);
    ent->unkA0 = loadAssetDataByMode(assetGroupIndex, assetPairIndex, 2);
    ent->unk98->unk20 = loadAssetDataByMode(assetGroupIndex, assetPairIndex, 2);

    ent->unk10 = param3;

    memcpy(&ent->transformationMatrix, identityMatrix, 0x20);

    ent->isDestroyed = 0;
    ent->displayEnabled = 1;
    ent->unk88 = 1;
    ent->unk4E = 0;
    ent->unk4F = (assetEntry->anotherAssetIndex >> 8) & 0xFF;
    ent->actionMode = 0;

    loadSpriteAsset(&ent->unkA4, 0);
    ent->unk114 = loadSpriteEffectModelData();
    ent->unk118 = loadSpriteEffectTextureData();
    ent->soundData = loadAssetGroupSoundData((SceneModel *)ent);
    ent->renderEnabled = 0;
    ent->height = 0;

    ent->unk120 = loadCompressedData(_646850_ROM_START, _646850_ROM_END, 0x238);
    ent->unk124 = &gDefaultEntityData8C938;
    ent->unk13A = 0x50;
    ent->unk154 = 0;
    ent->unk156 = 0;
    ent->unk138 = 0;
    ent->unk13B = 0;
    ent->unk134 = ent->unk120;
}
#else
INCLUDE_ASM("asm/nonmatchings/20F0", initializeGameEntity);
#endif

SceneModel *destroySceneModel(SceneModel *arg0) {
    cleanupSceneModel(arg0);
    return (SceneModel *)freeNodeMemory(arg0);
}

void *cleanupSceneModel(SceneModel *model) {
    SceneModel_unk98 *slotData;
    SceneModel_unk0 *modelData;
    void *freedSlot;
    SceneModel_unk0 *modelData2;

    if (model == 0) {
        return 0;
    }

    model->isDestroyed = 1;
    model->unk120 = freeNodeMemory(model->unk120);
    model->unk11C = freeNodeMemory(model->unk11C);
    model->unk118 = freeSpriteEffectTextureData(model->unk118);
    model->unk114 = freeSpriteEffectModelData(model->unk114);
    releaseNodeMemoryRef((void **)&model->unkA4);

    slotData = model->unk98;
    slotData->unk24 = freeNodeMemory(slotData->unk24);
    slotData->unk28 = freeNodeMemory(slotData->unk28);
    freedSlot = freeNodeMemory(model->unk98);

    modelData = model->unk0;
    model->unk98 = freedSlot;
    modelData->unk420 = freeNodeMemory(modelData->unk420);
    modelData->unk424 = freeNodeMemory(modelData->unk424);

    modelData2 = model->unk0;
    modelData2->unk3E4 = freeNodeMemory(modelData2->unk3E4);
    modelData2->unk3E8 = freeNodeMemory(modelData2->unk3E8);
    modelData2->unk3EC = freeNodeMemory(modelData2->unk3EC);

    model->unk4 = freeNodeMemory(model->unk4);
    model->unk8 = freeNodeMemory(model->unk8);
    model->unk0->unk24 = freeNodeMemory(model->unk0->unk24);
    model->unk0->unk28 = freeNodeMemory(model->unk0->unk28);
    model->unk0 = freeNodeMemory(model->unk0);

    return model;
}

void enqueueModelDisplayListByIndex(func_80002B50_3750_arg *model, s16 index) {
    if (index < model->unkE) {
        enqueueDisplayListObject(model->unk10->unk16, (void *)model->unk0 + (index * 0x3C));
    }
}

void setModelAnimation(SceneModel *model, s16 animationIndex) {
    setModelAnimationQueued(model, animationIndex, -1, -1, -1);
}

void setModelAnimationLooped(SceneModel *model, s16 animIndex, s16 transitionAnimIndex, s8 loopCount) {
    setModelAnimationQueued(model, animIndex, transitionAnimIndex, loopCount, -1);
}

void setModelAnimationQueued(SceneModel *arg0, s16 arg1, s16 arg2, s8 arg3, s16 arg4) {
    setModelAnimationEx(arg0, arg1, arg2, arg3, (s32)arg4, 1);
}

void setModelAnimationEx(SceneModel *arg0, s16 arg1, s16 arg2, s8 arg3, short arg4, s8 arg5) {
    int new_var;
    s16 var_t0;
    s16 var_a1;
    AssetGroup *entity;

    s16 local_arg2 = arg2;
    s16 local_arg1 = arg1;
    s16 local_arg4 = arg4;

    entity = &gameAssets[arg0->index];

    arg0->unk89 &= 0xFD;
    if (arg5) {
        arg0->unk89 |= 2;
    }

    if (arg0->unk8) {
        s16 temp_v0 = (local_arg1 < entity->padding) ? -1 : 0;
        var_t0 = local_arg1 & temp_v0;
        temp_v0 = var_t0;

        if (local_arg2 >= entity->padding) {
            local_arg2 = -1;
        }

        if ((local_arg4) >= entity->padding) {
            local_arg4 = -1;
        }

        new_var = -1;
        if (arg0->unk3A != new_var) {
            local_arg1 = local_arg4;
        }

        if ((arg0->unk16 != local_arg1) | ((~(local_arg4)) != 0)) {
            arg0->unk95 = 0;
            arg0->unk8C = 0;
            temp_v0 = arg0->unk16;
            arg0->unk16 = var_t0;
            arg0->unk14 = temp_v0;

            if (arg3 == new_var) {
                if (var_t0 != local_arg2) {
                    arg0->unk38 = local_arg2;
                    arg0->unk8E = local_arg2;
                } else {
                    arg0->unk38 = -1;
                    arg0->unk8E = new_var;
                }
            } else {
                arg0->unk38 = local_arg2;
                arg0->unk8E = local_arg2;
            }

            arg0->unk3A = local_arg4;
            arg0->unk3E = arg3;
            arg0->unk90 = 0;

            if (arg0->unk3A == -1) {
                var_a1 = var_t0;
            } else {
                var_a1 = local_arg4;
                if (arg0->unk3E != -1) {
                    arg0->unk3E++;
                }
            }

            {
                s32 i;
                s32 count = getAnimationBoneCount(arg0->unk8, var_a1);
                for (i = 0; i < count; i++) {
                    initBoneAnimationState(arg0->unk8, var_a1, (s16)i, arg0->unk4 + i);
                }
            }

            clearModelRotation(arg0);
        }
    }
}

s32 clearModelRotation(SceneModel *arg0) {
    return setModelRotation(arg0, -1);
}

s32 setModelRotation(SceneModel *arg0, s16 arg1) {
    s8 local_buffer[0x20];
    s32 s3;
    s32 i;
    s32 s4;
    s32 s6;
    s32 s7;
    s16 s5 = arg1;
    s16 temp;

    memcpy(&local_buffer, &identityMatrix, sizeof(local_buffer));

    s7 = 0;
    s4 = 0;
    s6 = 0;

    temp = arg0->unk16;

    if (arg0->unk8 == 0) {
        return 0;
    }

    if (temp == (-1)) {
        return 0;
    }

    arg0->unk8C++;
    if (arg0->unk3E != 0) {
        temp = (arg0->unk3A != -1) ? arg0->unk3A : arg0->unk16;
        s3 = getAnimationBoneCount(arg0->unk8, temp);
        if (s3 >= 32) {
            s3 = 31;
        }

        if (arg1 == -1) {
            for (i = 0; i < s3; i++) {
                s4 |= advanceBoneAnimation(arg0->unk8, temp, (s16)i, &arg0->unk4[i]);
                s6 |= 1 << i;
                arg0->unk90 |= s4 << i;
            }
        } else {
            for (i = 0; i < s3; i++) {
                interpolateBoneAnimation(&arg0->unk4[i], arg1 & 0xFFFF);
            }
        }
    } else if (arg0->unk38 != -1) {
        arg0->unk14 = arg0->unk16;
        arg0->unk16 = arg0->unk38;
        arg0->unk3E = -1;
        arg0->unk38 = -1;

        s3 = getAnimationBoneCount(arg0->unk8, arg0->unk16);
        for (i = 0; i < s3; i++) {
            initBoneAnimationState(arg0->unk8, arg0->unk16, (s16)i, &arg0->unk4[i]);
        }
    }

    if ((arg0->unk90 & s6) == s6) {
        s7 = 1;
        arg0->unk90 = 0;
    }

    if (s7 != 0) {
        arg0->unk95++;
        s7 = arg0->unk95;
        if (arg0->unk3E > 0) {
            arg0->unk3E--;
        }

        arg0->unk3A = -1;
        if (arg0->unk16 != arg0->unk8E) {
            if (arg0->unk16 != arg0->unk14) {
                arg0->animFrameCount = arg0->unk8C;
            }
            if (arg0->unk16 != arg0->unk8E) {
                arg0->unk8C = 0;
                return s7;
            }
        }

        if (arg0->unk16 == arg0->unk14) {
            if (arg0->animFrameCount != arg0->unk8C) {
                arg0->animFrameCount = arg0->unk8C;
            }
        }

        arg0->unk8C = 0;
    }

    return s7;
}

INCLUDE_ASM("asm/nonmatchings/20F0", updateModelGeometry);

void enqueueModelDisplayList(func_80002B50_3750_arg *arg0, DisplayListObject *arg1) {
    if ((arg0 != NULL) && (arg0->unk88 != 0) && (arg0->displayEnabled != 0)) {
        enqueueDisplayListObject(arg0->unk10->unk16, arg1);
    }
}

s32 getEmptyModelDisplayOffset(SceneModel *arg0) {
    s32 new_var;
    if (isAssetGroupEmpty(arg0->index) == 0) {
        return 0;
    }
    new_var = hasModelGraphicsData(arg0);
    if (new_var != 0) {
        return 0;
    }
    new_var = 0x90;
    return new_var;
}

s32 getModelDisplayDataOffset(SceneModel *model) {
    u32 assetGroupIndex = model->index;
    if (isAssetGroupEmpty(assetGroupIndex) == 0) {
        return 8;
    }

    if (hasModelGraphicsData(model) != 0) {
        assetGroupIndex = 8;
        return assetGroupIndex;
    }

    return 0x90;
}

void loadAssetSlotTableEntry(AssetSlotTableData *slot, DataTable_19E80 *table, s32 entryIndex) {
    OutputStruct_19E80 result;

    getTableEntryByU16Index(table, (s16)entryIndex, &result);
    slot->data_ptr = result.data_ptr;
    slot->index_ptr = result.index_ptr;
    slot->tableField1 = result.field1;
    slot->tableField2 = result.field2;
}