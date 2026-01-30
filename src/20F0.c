#include "20F0.h"

#include "19E80.h"
#include "5E590.h"
#include "D6F0.h"
#include "audio.h"
#include "displaylist.h"
#include "race_session.h"
#include "rom_loader.h"
#include "task_scheduler.h"

USE_ASSET(_1EA200);
USE_ASSET(_1EACD0);
USE_ASSET(_1EC940);
USE_ASSET(_1ED890);
USE_ASSET(_1ED890);
USE_ASSET(_1ED9E0);
USE_ASSET(_1EDB30);
USE_ASSET(_1EE410);
USE_ASSET(_1EE7D0);
USE_ASSET(_1FBE00);
USE_ASSET(_1FC700);
USE_ASSET(_1FCBA0);
USE_ASSET(_214FC0);
USE_ASSET(_215280);
USE_ASSET(_215360);
USE_ASSET(_215440);
USE_ASSET(_215550);
USE_ASSET(_215700);
USE_ASSET(_2158B0);
USE_ASSET(_215A60);
USE_ASSET(_4A0E80);
USE_ASSET(_4A2980);
USE_ASSET(_4A3E80);
USE_ASSET(_4A5840);
USE_ASSET(_4A5840);
USE_ASSET(_4A5A30);
USE_ASSET(_4A5C20);
USE_ASSET(_4A6B60);
USE_ASSET(_4A7070);
USE_ASSET(_4CAD30);
USE_ASSET(_4CCA00);
USE_ASSET(_4CD980);
USE_ASSET(_646850);
USE_ASSET(_663440);
USE_ASSET(_663440);
USE_ASSET(_663C90);
USE_ASSET(_663DB0);
USE_ASSET(_663EE0);
USE_ASSET(_664140);
USE_ASSET(_664400);
USE_ASSET(_664660);
USE_ASSET(_6648C0);

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

typedef struct {
    void *assetStart;
    void *assetEnd;
    void *asset2Start;
    void *asset2End;
    s32 asset2Size;
    s32 unk14;
} ItemAssetEntry;

extern u8 identityMatrix[32];
extern u8 D_215D70[];

u32 D_80089540_8A140[4] = { 0x0, 0x10000D0, 0x0, 0x0 };

u32 D_80089550_8A150[4] = { 0x0, 0x10000F8, 0x0, 0x0 };

u32 D_80089560_8A160[4] = { 0x0, 0x1000198, 0x0, 0x0 };

u32 D_80089570_8A170[4] = { 0x0, 0x10001A0, 0x0, 0x0 };

u32 D_80089580_8A180[4] = { 0x0, 0x10001A0, 0x0, 0x0 };

u32 D_80089590_8A190[4] = { 0x0, 0x10002F8, 0x0, 0x0 };

ItemAssetEntry itemAssetTable[] = {
    { &_214FC0_ROM_START, &_214FC0_ROM_END, &_663440_ROM_START, &_663440_ROM_END, 0x520, 0x80089500 },
    { &_1ED890_ROM_START, &_1ED890_ROM_END, &_4A5840_ROM_START, &_4A5840_ROM_END, 0x370, 0x800882F0 },
    { &_1ED9E0_ROM_START, &_1ED9E0_ROM_END, &_4A5A30_ROM_START, &_4A5A30_ROM_END, 0x370, 0x80088300 },
    { &_215280_ROM_START, &_215280_ROM_END, &_663C90_ROM_START, &_663C90_ROM_END, 0x1D0, 0x80089530 },
    { &_215440_ROM_START, &_215440_ROM_END, &_663EE0_ROM_START, &_663EE0_ROM_END, 0x3C0, 0x80089550 },
    { &_1EC940_ROM_START, &_1EC940_ROM_END, &_4A3E80_ROM_START, &_4A3E80_ROM_END, 0x620, 0x80088290 },
    { &_1FBE00_ROM_START, &_1FBE00_ROM_END, &_4CAD30_ROM_START, &_4CAD30_ROM_END, 0x620, 0x800886A0 },
    { &_215550_ROM_START, &_215550_ROM_END, &_664140_ROM_START, &_664140_ROM_END, 0x3B0, 0x80089560 },
    { &_1FC700_ROM_START, &_1FC700_ROM_END, &_4CCA00_ROM_START, &_4CCA00_ROM_END, 0x770, 0x800886E0 },
    { &_215700_ROM_START, &_215700_ROM_END, &_664400_ROM_START, &_664400_ROM_END, 0x4A0, 0x80089570 },
    { &_2158B0_ROM_START, &_2158B0_ROM_END, &_664660_ROM_START, &_664660_ROM_END, 0x4A0, 0x80089580 },
    { &_1EE7D0_ROM_START, &_1EE7D0_ROM_END, &_4A7070_ROM_START, &_4A7070_ROM_END, 0xA90, 0x80088360 },
    { &_1EE410_ROM_START, &_1EE410_ROM_END, &_4A6B60_ROM_START, &_4A6B60_ROM_END, 0xA90, 0x80088350 },
    { &_1EDB30_ROM_START, &_1EDB30_ROM_END, &_4A5C20_ROM_START, &_4A5C20_ROM_END, 0x320, 0x80088310 },
    { &_1FCBA0_ROM_START, &_1FCBA0_ROM_END, &_4CD980_ROM_START, &_4CD980_ROM_END, 0x710, 0x80088700 },
    { &_1EACD0_ROM_START, &_1EACD0_ROM_END, &_4A2980_ROM_START, &_4A2980_ROM_END, 0x790, 0x800881E0 },
    { &_1EA200_ROM_START, &_1EA200_ROM_END, &_4A0E80_ROM_START, &_4A0E80_ROM_END, 0x7E0, 0x800881A0 },
    { &_215360_ROM_START, &_215360_ROM_END, &_663DB0_ROM_START, &_663DB0_ROM_END, 0x240, 0x80089540 },
    { &_215A60_ROM_START, &_215A60_ROM_END, &_6648C0_ROM_START, &_6648C0_ROM_END, 0x810, 0x80089590 },
};

s32 itemAssetCount = 0x13;

u32 D_8008976C_8A36C[24] = { 0x1FDA30, 0x1FEEE0, 0x4CEFF0, 0x4D0470, 0x3DE00006, 0x80088770,
                             0x1FEEE0, 0x200580, 0x4D0470, 0x4D1870, 0x3AD00006, 0x800887D0,
                             0x200580, 0x202550, 0x4D1870, 0x4D2FD0, 0x4D900006, 0x80088830,
                             0x202550, 0x203C30, 0x4D2FD0, 0x4D45C0, 0x3F600006, 0x80088890 };

u32 D_800897CC_8A3CC[24] = { 0x203C30, 0x204980, 0x4D45C0, 0x4D5270, 0x2F200005, 0x800888F0,
                             0x204980, 0x205510, 0x4D5270, 0x4D5D60, 0x27F00005, 0x80088940,
                             0x205510, 0x206080, 0x4D5D60, 0x4D6880, 0x28900005, 0x80088990,
                             0x206080, 0x206C10, 0x4D6880, 0x4D7470, 0x2C000005, 0x800889E0 };

u32 D_8008982C_8A42C[24] = { 0x206C10, 0x207A30, 0x4D7470, 0x4D84B0, 0x2B000005, 0x80088A30,
                             0x207A30, 0x208AC0, 0x4D84B0, 0x4D9560, 0x2FE00005, 0x80088A80,
                             0x208AC0, 0x209A40, 0x4D9560, 0x4DA6E0, 0x2B800005, 0x80088AD0,
                             0x209A40, 0x20B0B0, 0x4DA6E0, 0x4DBC80, 0x40500005, 0x80088B20 };

u32 D_8008988C_8A48C[24] = { 0x20B0B0, 0x20BB50, 0x4DBC80, 0x4DC7A0, 0x27F00005, 0x80088B70,
                             0x20BB50, 0x20C5F0, 0x4DC7A0, 0x4DD2C0, 0x27F00005, 0x80088BC0,
                             0x20C5F0, 0x20D040, 0x4DD2C0, 0x4DDD90, 0x27500005, 0x80088C10,
                             0x20D040, 0x20DAE0, 0x4DDD90, 0x4DE820, 0x25C00005, 0x80088C60 };

u32 D_800898EC_8A4EC[24] = { 0x20DAE0, 0x20E850, 0x4DE820, 0x4DFAC0, 0x4D000005, 0x80088CB0,
                             0x20E850, 0x20F730, 0x4DFAC0, 0x4E0ED0, 0x50200005, 0x80088D00,
                             0x20F730, 0x210390, 0x4E0ED0, 0x4E21A0, 0x4E400005, 0x80088D50,
                             0x210390, 0x2110E0, 0x4E21A0, 0x4E33D0, 0x48D00005, 0x80088DA0 };

u32 D_8008994C_8A54C[24] = { 0x2110E0, 0x211920, 0x4E33D0, 0x4E40A0, 0x27A00003, 0x80088DF0,
                             0x211920, 0x211F40, 0x4E40A0, 0x4E4AA0, 0x1AC00002, 0x80088E20,
                             0x211F40, 0x2125A0, 0x4E4AA0, 0x4E54E0, 0x1AE00002, 0x80088E40,
                             0x2125A0, 0x212CF0, 0x4E54E0, 0x4E5FC0, 0x1D000002, 0x80088E60 };

u32 D_800899AC_8A5AC[24] = { 0x212CF0, 0x214B70, 0x4E5FC0, 0x4E7640, 0x3DE00008, 0x80088E80,
                             0x212CF0, 0x214B70, 0x4E5FC0, 0x4E7640, 0x3DE00008, 0x80088E80,
                             0x212CF0, 0x214B70, 0x4E5FC0, 0x4E7640, 0x3DE00008, 0x80088E80,
                             0x212CF0, 0x214B70, 0x4E5FC0, 0x4E7640, 0x3DE00008, 0x80088E80 };

u32 D_80089A0C_8A60C[24] = { 0x00214B70, 0x00214E70, 0x004E7640, 0x004E7AA0, 0x09800001, 0x80088F00,
                             0x00214B70, 0x00214E70, 0x004E7640, 0x004E7AA0, 0x09800001, 0x80088F00,
                             0x00214B70, 0x00214E70, 0x004E7640, 0x004E7AA0, 0x09800001, 0x80088F00,
                             0x00214B70, 0x00214E70, 0x004E7640, 0x004E7AA0, 0x09800001, 0x80088F00 };

AssetGroup gameAssets[] = {
    /* Entry 0:  SLASH \0 */
    {
     .name = { 0x20, 0x53, 0x4C, 0x41, 0x53, 0x48, 0x20, 0x00 }, /*  SLASH \0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x00,
     .padding = 0x00E7,
     .asset3Start = (void *)0x004E7AA0,
     .asset3End = (void *)0x004ED9C0,
     .asset3Size = 0x9F02,
     .anotherAssetIndex = 0xff050000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x8008976C,
     .count = 4,
     },
    /* Entry 1:  NANCY \0 */
    {
     .name = { 0x20, 0x4E, 0x41, 0x4E, 0x43, 0x59, 0x20, 0x00 }, /*  NANCY \0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x01,
     .padding = 0x0175,
     .asset3Start = (void *)0x00510AD0,
     .asset3End = (void *)0x00518BA0,
     .asset3Size = 0xDE48,
     .anotherAssetIndex = 0xff050000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x800897CC,
     .count = 4,
     },
    /* Entry 2:  JAM   \0 */
    {
     .name = { 0x20, 0x4A, 0x41, 0x4D, 0x20, 0x20, 0x20, 0x00 }, /*  JAM   \0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x02,
     .padding = 0x012F,
     .asset3Start = (void *)0x00527EE0,
     .asset3End = (void *)0x0052FC30,
     .asset3Size = 0xC848,
     .anotherAssetIndex = 0xff060000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x8008982C,
     .count = 4,
     },
    /* Entry 3:  LINDA \0 */
    {
     .name = { 0x20, 0x4C, 0x49, 0x4E, 0x44, 0x41, 0x20, 0x00 }, /*  LINDA \0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x03,
     .padding = 0x011C,
     .asset3Start = (void *)0x00542AF0,
     .asset3End = (void *)0x0054B280,
     .asset3Size = 0xE6AE,
     .anotherAssetIndex = 0xff050000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x8008988C,
     .count = 4,
     },
    /* Entry 4:  TOMMY \0 */
    {
     .name = { 0x20, 0x54, 0x4F, 0x4D, 0x4D, 0x59, 0x20, 0x00 }, /*  TOMMY \0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x04,
     .padding = 0x0178,
     .asset3Start = (void *)0x0055E550,
     .asset3End = (void *)0x00564920,
     .asset3Size = 0xB6D0,
     .anotherAssetIndex = 0xff070000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x800898EC,
     .count = 4,
     },
    /* Entry 5:  WENDY \0 */
    {
     .name = { 0x20, 0x57, 0x45, 0x4E, 0x44, 0x59, 0x20, 0x00 }, /*  WENDY \0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x05,
     .padding = 0x0174,
     .asset3Start = (void *)0x00576600,
     .asset3End = (void *)0x00580AD0,
     .asset3Size = 0xFE66,
     .anotherAssetIndex = 0xff050000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x8008994C,
     .count = 4,
     },
    /* Entry 6:  BLACK \0 */
    {
     .name = { 0x20, 0x42, 0x4C, 0x41, 0x43, 0x4B, 0x20, 0x00 }, /*  BLACK \0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x06,
     .padding = 0x0157,
     .asset3Start = (void *)0x005944E0,
     .asset3End = (void *)0x0059C0A0,
     .asset3Size = 0xC8A4,
     .anotherAssetIndex = 0xff040000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x800899AC,
     .count = 4,
     },
    /* Entry 7:  PEN   \0 */
    {
     .name = { 0x20, 0x50, 0x45, 0x4E, 0x20, 0x20, 0x20, 0x00 }, /*  PEN   \0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x07,
     .padding = 0x0174,
     .asset3Start = (void *)0x00576600,
     .asset3End = (void *)0x00580AD0,
     .asset3Size = 0xFE66,
     .anotherAssetIndex = 0xff040000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x80089A0C,
     .count = 4,
     },
    /* Entry 8:  DOG   \0 */
    {
     .name = { 0x20, 0x44, 0x4F, 0x47, 0x20, 0x20, 0x20, 0x00 }, /*  DOG   \0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x08,
     .padding = 0x009F,
     .asset3Start = (void *)0x005B9AC0,
     .asset3End = (void *)0x005BD9F0,
     .asset3Size = 0x8070,
     .anotherAssetIndex = 0xff080000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 9:  DINOS \0 */
    {
     .name = { 0x20, 0x44, 0x49, 0x4E, 0x4F, 0x53, 0x20, 0x00 }, /*  DINOS \0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x09,
     .padding = 0x015D,
     .asset3Start = (void *)0x005B05A0,
     .asset3End = (void *)0x005B4130,
     .asset3Size = 0x5788,
     .anotherAssetIndex = 0xff190000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 10:  SNOW  \0 */
    {
     .name = { 0x20, 0x53, 0x4E, 0x4F, 0x57, 0x20, 0x20, 0x00 }, /*  SNOW  \0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x03,
     .assetGroupIndex = (s8)0x0A,
     .padding = 0x0000,
     .asset3Start = (void *)0x00000000,
     .asset3End = (void *)0x00000000,
     .asset3Size = 0x0,
     .anotherAssetIndex = 0xff110000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 11:  ROBO  \0 */
    {
     .name = { 0x20, 0x52, 0x4F, 0x42, 0x4F, 0x20, 0x20, 0x00 }, /*  ROBO  \0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x0B,
     .padding = 0x009D,
     .asset3Start = (void *)0x005B5590,
     .asset3End = (void *)0x005B7D30,
     .asset3Size = 0x3CDA,
     .anotherAssetIndex = 0xff170000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 12:  CHAIR \0 */
    {
     .name = { 0x20, 0x43, 0x48, 0x41, 0x49, 0x52, 0x20, 0x00 }, /*  CHAIR \0 */
        .displayListStart = (void *)0x001EEC10,
     .displayListEnd = (void *)0x001EEDF0,
     .vertexDataStart = (void *)0x004A73F0,
     .vertexDataEnd = (void *)0x004A7600,
     .size = 0x560,
     .unk1C = (void *)0x80088370,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .padding = 0x0000,
     .asset3Start = (void *)0x00000000,
     .asset3End = (void *)0x00000000,
     .asset3Size = 0x0,
     .anotherAssetIndex = 0xff020000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 13:  CUP   \0 */
    {
     .name = { 0x20, 0x43, 0x55, 0x50, 0x20, 0x20, 0x20, 0x00 }, /*  CUP   \0 */
        .displayListStart = (void *)0x001EE7D0,
     .displayListEnd = (void *)0x001EEC10,
     .vertexDataStart = (void *)0x004A7070,
     .vertexDataEnd = (void *)0x004A73F0,
     .size = 0xA90,
     .unk1C = (void *)0x80088360,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .padding = 0x0000,
     .asset3Start = (void *)0x00000000,
     .asset3End = (void *)0x00000000,
     .asset3Size = 0x0,
     .anotherAssetIndex = 0xff000000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 14:  POT   \0 */
    {
     .name = { 0x20, 0x50, 0x4F, 0x54, 0x20, 0x20, 0x20, 0x00 }, /*  POT   \0 */
        .displayListStart = (void *)0x001EE410,
     .displayListEnd = (void *)0x001EE7D0,
     .vertexDataStart = (void *)0x004A6B60,
     .vertexDataEnd = (void *)0x004A7070,
     .size = 0xA90,
     .unk1C = (void *)0x80088350,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .padding = 0x0000,
     .asset3Start = (void *)0x00000000,
     .asset3End = (void *)0x00000000,
     .asset3Size = 0x0,
     .anotherAssetIndex = 0xff000000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 15:  SNOW2 \0 */
    {
     .name = { 0x20, 0x53, 0x4E, 0x4F, 0x57, 0x32, 0x20, 0x00 }, /*  SNOW2 \0 */
        .displayListStart = (void *)0x001EF780,
     .displayListEnd = (void *)0x001EFB50,
     .vertexDataStart = (void *)0x004A8180,
     .vertexDataEnd = (void *)0x004A8650,
     .size = 0xB20,
     .unk1C = (void *)0x800883D0,
     .numAssets = (s8)0x02,
     .assetGroupIndex = (s8)-0x01,
     .padding = 0x0136,
     .asset3Start = (void *)0x005B4B50,
     .asset3End = (void *)0x005B5590,
     .asset3Size = 0x1CDA,
     .anotherAssetIndex = 0xff110000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 16:  BOMB  \0 */
    {
     .name = { 0x20, 0x42, 0x4F, 0x4D, 0x42, 0x20, 0x20, 0x00 }, /*  BOMB  \0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x00,
     .assetGroupIndex = (s8)-0x01,
     .padding = 0x0000,
     .asset3Start = (void *)0x00000000,
     .asset3End = (void *)0x00000000,
     .asset3Size = 0x0,
     .anotherAssetIndex = 0xff000000,
     .soundSequenceDataStart = (void *)0x006489F0,
     .soundSequenceDataEnd = (void *)0x00648B50,
     .soundSequenceDataSize = 0x358,
     .initCallback = (void *)0x80006940,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 17:  BUTTE \0 */
    {
     .name = { 0x20, 0x42, 0x55, 0x54, 0x54, 0x45, 0x20, 0x00 }, /*  BUTTE \0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x00,
     .assetGroupIndex = (s8)-0x01,
     .padding = 0x0000,
     .asset3Start = (void *)0x00000000,
     .asset3End = (void *)0x00000000,
     .asset3Size = 0x0,
     .anotherAssetIndex = 0xff000000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x800063B4,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 18:  BUS   \0 */
    {
     .name = { 0x20, 0x42, 0x55, 0x53, 0x20, 0x20, 0x20, 0x00 }, /*  BUS   \0 */
        .displayListStart = (void *)0x001FB040,
     .displayListEnd = (void *)0x001FB4E0,
     .vertexDataStart = (void *)0x004C9310,
     .vertexDataEnd = (void *)0x004C9E70,
     .size = 0x1280,
     .unk1C = (void *)0x80088630,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .padding = 0x0000,
     .asset3Start = (void *)0x00000000,
     .asset3End = (void *)0x00000000,
     .asset3Size = 0x0,
     .anotherAssetIndex = 0xff000000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x80005DB0,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 19:  UFO   \0 */
    {
     .name = { 0x20, 0x55, 0x46, 0x4F, 0x20, 0x20, 0x20, 0x00 }, /*  UFO   \0 */
        .displayListStart = (void *)0x001F8D10,
     .displayListEnd = (void *)0x001F94F0,
     .vertexDataStart = (void *)0x004C20A0,
     .vertexDataEnd = (void *)0x004C3660,
     .size = 0x25A0,
     .unk1C = (void *)0x800885C0,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .padding = 0x0000,
     .asset3Start = (void *)0x00000000,
     .asset3End = (void *)0x00000000,
     .asset3Size = 0x0,
     .anotherAssetIndex = 0xff140000,
     .soundSequenceDataStart = (void *)0x004C3660,
     .soundSequenceDataEnd = (void *)0x004C3A60,
     .soundSequenceDataSize = 0x838,
     .initCallback = (void *)0x80005808,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 20:  SLASH2\0 */
    {
     .name = { 0x20, 0x53, 0x4C, 0x41, 0x53, 0x48, 0x32, 0x00 }, /*  SLASH2\0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x00,
     .padding = 0x007A,
     .asset3Start = (void *)0x004ED9C0,
     .asset3End = (void *)0x004F45E0,
     .asset3Size = 0xC3F2,
     .anotherAssetIndex = 0xff050000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x8008976C,
     .count = 4,
     },
    /* Entry 21:  NANCY2\0 */
    {
     .name = { 0x20, 0x4E, 0x41, 0x4E, 0x43, 0x59, 0x32, 0x00 }, /*  NANCY2\0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x01,
     .padding = 0x007A,
     .asset3Start = (void *)0x00518BA0,
     .asset3End = (void *)0x0051BCF0,
     .asset3Size = 0x6032,
     .anotherAssetIndex = 0xff050000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x800897CC,
     .count = 4,
     },
    /* Entry 22:  JAM2  \0 */
    {
     .name = { 0x20, 0x4A, 0x41, 0x4D, 0x32, 0x20, 0x20, 0x00 }, /*  JAM2  \0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x02,
     .padding = 0x007A,
     .asset3Start = (void *)0x0052FC30,
     .asset3End = (void *)0x00535E80,
     .asset3Size = 0xB2DA,
     .anotherAssetIndex = 0xff060000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x8008982C,
     .count = 4,
     },
    /* Entry 23:  LINDA2\0 */
    {
     .name = { 0x20, 0x4C, 0x49, 0x4E, 0x44, 0x41, 0x32, 0x00 }, /*  LINDA2\0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x03,
     .padding = 0x007A,
     .asset3Start = (void *)0x0054B280,
     .asset3End = (void *)0x00550BC0,
     .asset3Size = 0x9814,
     .anotherAssetIndex = 0xff050000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x8008988C,
     .count = 4,
     },
    /* Entry 24:  TOMMY2\0 */
    {
     .name = { 0x20, 0x54, 0x4F, 0x4D, 0x4D, 0x59, 0x32, 0x00 }, /*  TOMMY2\0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x04,
     .padding = 0x007A,
     .asset3Start = (void *)0x00564920,
     .asset3End = (void *)0x00568A70,
     .asset3Size = 0x8094,
     .anotherAssetIndex = 0xff070000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x800898EC,
     .count = 4,
     },
    /* Entry 25:  WENDY2\0 */
    {
     .name = { 0x20, 0x57, 0x45, 0x4E, 0x44, 0x59, 0x32, 0x00 }, /*  WENDY2\0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x05,
     .padding = 0x007A,
     .asset3Start = (void *)0x00580AD0,
     .asset3End = (void *)0x00584650,
     .asset3Size = 0x6C2A,
     .anotherAssetIndex = 0xff050000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x8008994C,
     .count = 4,
     },
    /* Entry 26:  BLACK2\0 */
    {
     .name = { 0x20, 0x42, 0x4C, 0x41, 0x43, 0x4B, 0x32, 0x00 }, /*  BLACK2\0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x06,
     .padding = 0x007A,
     .asset3Start = (void *)0x0059C0A0,
     .asset3End = (void *)0x0059EF90,
     .asset3Size = 0x5A70,
     .anotherAssetIndex = 0xff040000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x800899AC,
     .count = 4,
     },
    /* Entry 27:  PEN2  \0 */
    {
     .name = { 0x20, 0x50, 0x45, 0x4E, 0x32, 0x20, 0x20, 0x00 }, /*  PEN2  \0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x07,
     .padding = 0x00A0,
     .asset3Start = (void *)0x005AD910,
     .asset3End = (void *)0x005B0030,
     .asset3Size = 0x40A2,
     .anotherAssetIndex = 0xff040000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x80089A0C,
     .count = 4,
     },
    /* Entry 28:  DOG2  \0 */
    {
     .name = { 0x20, 0x44, 0x4F, 0x47, 0x32, 0x20, 0x20, 0x00 }, /*  DOG2  \0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x08,
     .padding = 0x007A,
     .asset3Start = (void *)0x00564920,
     .asset3End = (void *)0x00568A70,
     .asset3Size = 0x8094,
     .anotherAssetIndex = 0xff080000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 29:  ABOXO \0 */
    {
     .name = { 0x20, 0x41, 0x42, 0x4F, 0x58, 0x4F, 0x20, 0x00 }, /*  ABOXO \0 */
        .displayListStart = (void *)0x001FCE20,
     .displayListEnd = (void *)0x001FD350,
     .vertexDataStart = (void *)0x004CDCB0,
     .vertexDataEnd = (void *)0x004CE2A0,
     .size = 0xD10,
     .unk1C = (void *)0x80088710,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .padding = 0x0000,
     .asset3Start = (void *)0x00000000,
     .asset3End = (void *)0x00000000,
     .asset3Size = 0x0,
     .anotherAssetIndex = 0xff000000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x80006E60,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 30:  KEY O \0 */
    {
     .name = { 0x20, 0x4B, 0x45, 0x59, 0x20, 0x4F, 0x20, 0x00 }, /*  KEY O \0 */
        .displayListStart = (void *)0x001FD350,
     .displayListEnd = (void *)0x001FD500,
     .vertexDataStart = (void *)0x004CE2A0,
     .vertexDataEnd = (void *)0x004CE500,
     .size = 0x4A0,
     .unk1C = (void *)0x80088730,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .padding = 0x0003,
     .asset3Start = (void *)0x005BD9F0,
     .asset3End = (void *)0x005BDB20,
     .asset3Size = 0x108,
     .anotherAssetIndex = 0xff000000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 31:  KEY L \0 */
    {
     .name = { 0x20, 0x4B, 0x45, 0x59, 0x20, 0x4C, 0x20, 0x00 }, /*  KEY L \0 */
        .displayListStart = (void *)0x001FD500,
     .displayListEnd = (void *)0x001FD6B0,
     .vertexDataStart = (void *)0x004CE500,
     .vertexDataEnd = (void *)0x004CE760,
     .size = 0x4A0,
     .unk1C = (void *)0x80088740,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .padding = 0x0003,
     .asset3Start = (void *)0x005BD9F0,
     .asset3End = (void *)0x005BDB20,
     .asset3Size = 0x108,
     .anotherAssetIndex = 0xff000000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 32:  ABOXL \0 */
    {
     .name = { 0x20, 0x41, 0x42, 0x4F, 0x58, 0x4C, 0x20, 0x00 }, /*  ABOXL \0 */
        .displayListStart = (void *)0x001FCBA0,
     .displayListEnd = (void *)0x001FCE20,
     .vertexDataStart = (void *)0x004CD980,
     .vertexDataEnd = (void *)0x004CDCB0,
     .size = 0x710,
     .unk1C = (void *)0x80088700,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .padding = 0x0000,
     .asset3Start = (void *)0x00000000,
     .asset3End = (void *)0x00000000,
     .asset3Size = 0x0,
     .anotherAssetIndex = 0xff000000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 33:  KANI  \0 */
    {
     .name = { 0x20, 0x4B, 0x41, 0x4E, 0x49, 0x20, 0x20, 0x00 }, /*  KANI  \0 */
        .displayListStart = (void *)0x001FC700,
     .displayListEnd = (void *)0x001FC9A0,
     .vertexDataStart = (void *)0x004CCA00,
     .vertexDataEnd = (void *)0x004CCE20,
     .size = 0x770,
     .unk1C = (void *)0x800886E0,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .padding = 0x0000,
     .asset3Start = (void *)0x00000000,
     .asset3End = (void *)0x00000000,
     .asset3Size = 0x0,
     .anotherAssetIndex = 0xff040000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x80007380,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 34:  NEZU  \0 */
    {
     .name = { 0x20, 0x4E, 0x45, 0x5A, 0x55, 0x20, 0x20, 0x00 }, /*  NEZU  \0 */
        .displayListStart = (void *)0x001FD6B0,
     .displayListEnd = (void *)0x001FDA30,
     .vertexDataStart = (void *)0x004CE760,
     .vertexDataEnd = (void *)0x004CEFF0,
     .size = 0x1080,
     .unk1C = (void *)0x80088750,
     .numAssets = (s8)0x02,
     .assetGroupIndex = (s8)-0x01,
     .padding = 0x017C,
     .asset3Start = (void *)0x005B4130,
     .asset3End = (void *)0x005B4B50,
     .asset3Size = 0x1FE0,
     .anotherAssetIndex = 0xff050000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 35:  CAKEE \0 */
    {
     .name = { 0x20, 0x43, 0x41, 0x4B, 0x45, 0x45, 0x20, 0x00 }, /*  CAKEE \0 */
        .displayListStart = (void *)0x001FC9A0,
     .displayListEnd = (void *)0x001FCBA0,
     .vertexDataStart = (void *)0x004CCE20,
     .vertexDataEnd = (void *)0x004CD980,
     .size = 0xF80,
     .unk1C = (void *)0x800886F0,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .padding = 0x0000,
     .asset3Start = (void *)0x00000000,
     .asset3End = (void *)0x00000000,
     .asset3Size = 0x0,
     .anotherAssetIndex = 0xff000000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 36:  CAKE  \0 */
    {
     .name = { 0x20, 0x43, 0x41, 0x4B, 0x45, 0x20, 0x20, 0x00 }, /*  CAKE  \0 */
        .displayListStart = (void *)0x001FC370,
     .displayListEnd = (void *)0x001FC700,
     .vertexDataStart = (void *)0x004CBCF0,
     .vertexDataEnd = (void *)0x004CCA00,
     .size = 0x1620,
     .unk1C = (void *)0x800886D0,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .padding = 0x0000,
     .asset3Start = (void *)0x00000000,
     .asset3End = (void *)0x00000000,
     .asset3Size = 0x0,
     .anotherAssetIndex = 0xff000000,
     .soundSequenceDataStart = (void *)0x00647740,
     .soundSequenceDataEnd = (void *)0x00647930,
     .soundSequenceDataSize = 0x468,
     .initCallback = (void *)0x80007070,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 37:  POSP  \0 */
    {
     .name = { 0x20, 0x50, 0x4F, 0x53, 0x50, 0x20, 0x20, 0x00 }, /*  POSP  \0 */
        .displayListStart = (void *)0x001FC140,
     .displayListEnd = (void *)0x001FC370,
     .vertexDataStart = (void *)0x004CB700,
     .vertexDataEnd = (void *)0x004CBCF0,
     .size = 0x8E0,
     .unk1C = (void *)0x800886C0,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .padding = 0x0000,
     .asset3Start = (void *)0x00000000,
     .asset3End = (void *)0x00000000,
     .asset3Size = 0x0,
     .anotherAssetIndex = 0xff030000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 38:  POST  \0 */
    {
     .name = { 0x20, 0x50, 0x4F, 0x53, 0x54, 0x20, 0x20, 0x00 }, /*  POST  \0 */
        .displayListStart = (void *)0x001FBF30,
     .displayListEnd = (void *)0x001FC140,
     .vertexDataStart = (void *)0x004CB1E0,
     .vertexDataEnd = (void *)0x004CB700,
     .size = 0x7A0,
     .unk1C = (void *)0x800886B0,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .padding = 0x0000,
     .asset3Start = (void *)0x00000000,
     .asset3End = (void *)0x00000000,
     .asset3Size = 0x0,
     .anotherAssetIndex = 0xff030000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 39:  NEWS  \0 */
    {
     .name = { 0x20, 0x4E, 0x45, 0x57, 0x53, 0x20, 0x20, 0x00 }, /*  NEWS  \0 */
        .displayListStart = (void *)0x001FBE00,
     .displayListEnd = (void *)0x001FBF30,
     .vertexDataStart = (void *)0x004CAD30,
     .vertexDataEnd = (void *)0x004CB1E0,
     .size = 0x620,
     .unk1C = (void *)0x800886A0,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .padding = 0x0000,
     .asset3Start = (void *)0x00000000,
     .asset3End = (void *)0x00000000,
     .asset3Size = 0x0,
     .anotherAssetIndex = 0xff000000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 40:  SLASH3\0 */
    {
     .name = { 0x20, 0x53, 0x4C, 0x41, 0x53, 0x48, 0x33, 0x00 }, /*  SLASH3\0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x00,
     .padding = 0x016C,
     .asset3Start = (void *)0x004F45E0,
     .asset3End = (void *)0x004FDE20,
     .asset3Size = 0xF51A,
     .anotherAssetIndex = 0xff050000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x8008976C,
     .count = 4,
     },
    /* Entry 41:  NANCY3\0 */
    {
     .name = { 0x20, 0x4E, 0x41, 0x4E, 0x43, 0x59, 0x33, 0x00 }, /*  NANCY3\0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x01,
     .padding = 0x015A,
     .asset3Start = (void *)0x0051BCF0,
     .asset3End = (void *)0x005242F0,
     .asset3Size = 0xE65E,
     .anotherAssetIndex = 0xff050000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x800897CC,
     .count = 4,
     },
    /* Entry 42:  JAM3  \0 */
    {
     .name = { 0x20, 0x4A, 0x41, 0x4D, 0x33, 0x20, 0x20, 0x00 }, /*  JAM3  \0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x02,
     .padding = 0x0127,
     .asset3Start = (void *)0x00535E80,
     .asset3End = (void *)0x0053E160,
     .asset3Size = 0xDB44,
     .anotherAssetIndex = 0xff060000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x8008982C,
     .count = 4,
     },
    /* Entry 43:  LINDA3\0 */
    {
     .name = { 0x20, 0x4C, 0x49, 0x4E, 0x44, 0x41, 0x33, 0x00 }, /*  LINDA3\0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x03,
     .padding = 0x0137,
     .asset3Start = (void *)0x00550BC0,
     .asset3End = (void *)0x005596D0,
     .asset3Size = 0xDAD4,
     .anotherAssetIndex = 0xff050000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x8008988C,
     .count = 4,
     },
    /* Entry 44:  TOMMY3\0 */
    {
     .name = { 0x20, 0x54, 0x4F, 0x4D, 0x4D, 0x59, 0x33, 0x00 }, /*  TOMMY3\0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x04,
     .padding = 0x0177,
     .asset3Start = (void *)0x00568A70,
     .asset3End = (void *)0x00573660,
     .asset3Size = 0x127A2,
     .anotherAssetIndex = 0xff070000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x800898EC,
     .count = 4,
     },
    /* Entry 45:  WENDY3\0 */
    {
     .name = { 0x20, 0x57, 0x45, 0x4E, 0x44, 0x59, 0x33, 0x00 }, /*  WENDY3\0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x05,
     .padding = 0x0176,
     .asset3Start = (void *)0x00584650,
     .asset3End = (void *)0x00590790,
     .asset3Size = 0x12DD2,
     .anotherAssetIndex = 0xff050000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x8008994C,
     .count = 4,
     },
    /* Entry 46:  BLACK3\0 */
    {
     .name = { 0x20, 0x42, 0x4C, 0x41, 0x43, 0x4B, 0x33, 0x00 }, /*  BLACK3\0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x06,
     .padding = 0x0132,
     .asset3Start = (void *)0x0059EF90,
     .asset3End = (void *)0x005A9D70,
     .asset3Size = 0x11794,
     .anotherAssetIndex = 0xff040000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x800899AC,
     .count = 4,
     },
    /* Entry 47:  RADIO \0 */
    {
     .name = { 0x20, 0x52, 0x41, 0x44, 0x49, 0x4F, 0x20, 0x00 }, /*  RADIO \0 */
        .displayListStart = (void *)0x001EE140,
     .displayListEnd = (void *)0x001EE410,
     .vertexDataStart = (void *)0x004A6920,
     .vertexDataEnd = (void *)0x004A6B60,
     .size = 0x620,
     .unk1C = (void *)0x80088340,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .padding = 0x0000,
     .asset3Start = (void *)0x00000000,
     .asset3End = (void *)0x00000000,
     .asset3Size = 0x0,
     .anotherAssetIndex = 0xff050000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x80006AB0,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 48:  TABLE \0 */
    {
     .name = { 0x20, 0x54, 0x41, 0x42, 0x4C, 0x45, 0x20, 0x00 }, /*  TABLE \0 */
        .displayListStart = (void *)0x001EDEE0,
     .displayListEnd = (void *)0x001EE140,
     .vertexDataStart = (void *)0x004A6360,
     .vertexDataEnd = (void *)0x004A6920,
     .size = 0xC20,
     .unk1C = (void *)0x80088330,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .padding = 0x0000,
     .asset3Start = (void *)0x00000000,
     .asset3End = (void *)0x00000000,
     .asset3Size = 0x0,
     .anotherAssetIndex = 0xff080000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 49:  TUTUM \0 */
    {
     .name = { 0x20, 0x54, 0x55, 0x54, 0x55, 0x4D, 0x20, 0x00 }, /*  TUTUM \0 */
        .displayListStart = (void *)0x001EDCF0,
     .displayListEnd = (void *)0x001EDEE0,
     .vertexDataStart = (void *)0x004A5E10,
     .vertexDataEnd = (void *)0x004A6360,
     .size = 0x930,
     .unk1C = (void *)0x80088320,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .padding = 0x0000,
     .asset3Start = (void *)0x00000000,
     .asset3End = (void *)0x00000000,
     .asset3Size = 0x0,
     .anotherAssetIndex = 0xff000000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 50:  SLASHS\0 */
    {
     .name = { 0x20, 0x53, 0x4C, 0x41, 0x53, 0x48, 0x53, 0x00 }, /*  SLASHS\0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x00,
     .padding = 0x0008,
     .asset3Start = (void *)0x0045B130,
     .asset3End = (void *)0x0045D620,
     .asset3Size = 0x356E,
     .anotherAssetIndex = 0xff050000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x8008976C,
     .count = 4,
     },
    /* Entry 51:  NANCYS\0 */
    {
     .name = { 0x20, 0x4E, 0x41, 0x4E, 0x43, 0x59, 0x53, 0x00 }, /*  NANCYS\0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x01,
     .padding = 0x0008,
     .asset3Start = (void *)0x0045D620,
     .asset3End = (void *)0x0045F6D0,
     .asset3Size = 0x32A4,
     .anotherAssetIndex = 0xff050000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x800897CC,
     .count = 4,
     },
    /* Entry 52:  JAMS  \0 */
    {
     .name = { 0x20, 0x4A, 0x41, 0x4D, 0x53, 0x20, 0x20, 0x00 }, /*  JAMS  \0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x02,
     .padding = 0x0008,
     .asset3Start = (void *)0x0045F6D0,
     .asset3End = (void *)0x00462B50,
     .asset3Size = 0x477C,
     .anotherAssetIndex = 0xff060000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x8008982C,
     .count = 4,
     },
    /* Entry 53:  LINDAS\0 */
    {
     .name = { 0x20, 0x4C, 0x49, 0x4E, 0x44, 0x41, 0x53, 0x00 }, /*  LINDAS\0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x03,
     .padding = 0x0008,
     .asset3Start = (void *)0x00462B50,
     .asset3End = (void *)0x00464DD0,
     .asset3Size = 0x3BEC,
     .anotherAssetIndex = 0xff050000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x8008988C,
     .count = 4,
     },
    /* Entry 54:  TOMMYS\0 */
    {
     .name = { 0x20, 0x54, 0x4F, 0x4D, 0x4D, 0x59, 0x53, 0x00 }, /*  TOMMYS\0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x04,
     .padding = 0x0008,
     .asset3Start = (void *)0x00464DD0,
     .asset3End = (void *)0x004666C0,
     .asset3Size = 0x341E,
     .anotherAssetIndex = 0xff070000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x800898EC,
     .count = 4,
     },
    /* Entry 55:  WENDYS\0 */
    {
     .name = { 0x20, 0x57, 0x45, 0x4E, 0x44, 0x59, 0x53, 0x00 }, /*  WENDYS\0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x05,
     .padding = 0x000A,
     .asset3Start = (void *)0x004666C0,
     .asset3End = (void *)0x0046A0E0,
     .asset3Size = 0x4E52,
     .anotherAssetIndex = 0xff050000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x8008994C,
     .count = 4,
     },
    /* Entry 56:  BLACKS\0 */
    {
     .name = { 0x20, 0x42, 0x4C, 0x41, 0x43, 0x4B, 0x53, 0x00 }, /*  BLACKS\0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x06,
     .padding = 0x000A,
     .asset3Start = (void *)0x0046A0E0,
     .asset3End = (void *)0x0046D570,
     .asset3Size = 0x58A4,
     .anotherAssetIndex = 0xff040000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x800899AC,
     .count = 4,
     },
    /* Entry 57:  PENS  \0 */
    {
     .name = { 0x20, 0x50, 0x45, 0x4E, 0x53, 0x20, 0x20, 0x00 }, /*  PENS  \0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x07,
     .padding = 0x0005,
     .asset3Start = (void *)0x0046D570,
     .asset3End = (void *)0x0046F0C0,
     .asset3Size = 0x35BC,
     .anotherAssetIndex = 0xff040000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x80089A0C,
     .count = 4,
     },
    /* Entry 58:  DOGS  \0 */
    {
     .name = { 0x20, 0x44, 0x4F, 0x47, 0x53, 0x20, 0x20, 0x00 }, /*  DOGS  \0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x08,
     .padding = 0x0018,
     .asset3Start = (void *)0x0046F0C0,
     .asset3End = (void *)0x00476EB0,
     .asset3Size = 0xD8F2,
     .anotherAssetIndex = 0xff080000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 59:  DISH  \0 */
    {
     .name = { 0x20, 0x44, 0x49, 0x53, 0x48, 0x20, 0x20, 0x00 }, /*  DISH  \0 */
        .displayListStart = (void *)0x001EDB30,
     .displayListEnd = (void *)0x001EDCF0,
     .vertexDataStart = (void *)0x004A5C20,
     .vertexDataEnd = (void *)0x004A5E10,
     .size = 0x320,
     .unk1C = (void *)0x80088310,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .padding = 0x0000,
     .asset3Start = (void *)0x00000000,
     .asset3End = (void *)0x00000000,
     .asset3Size = 0x0,
     .anotherAssetIndex = 0xff000000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x800081C0,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 60:  SNOW3 \0 */
    {
     .name = { 0x20, 0x53, 0x4E, 0x4F, 0x57, 0x33, 0x20, 0x00 }, /*  SNOW3 \0 */
        .displayListStart = (void *)0x001EF1C0,
     .displayListEnd = (void *)0x001EF780,
     .vertexDataStart = (void *)0x004A79C0,
     .vertexDataEnd = (void *)0x004A8180,
     .size = 0xEB0,
     .unk1C = (void *)0x800883A0,
     .numAssets = (s8)0x03,
     .assetGroupIndex = (s8)-0x01,
     .padding = 0x0136,
     .asset3Start = (void *)0x005B4B50,
     .asset3End = (void *)0x005B5590,
     .asset3Size = 0x1CDA,
     .anotherAssetIndex = 0xff110000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x00000000,
     .count = 0,
     .countPadding = { 0x01, 0x00, 0x00 },
     },
    /* Entry 61:  SNOW4 \0 */
    {
     .name = { 0x20, 0x53, 0x4E, 0x4F, 0x57, 0x34, 0x20, 0x00 }, /*  SNOW4 \0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x03,
     .assetGroupIndex = (s8)0x0A,
     .padding = 0x0136,
     .asset3Start = (void *)0x005B4B50,
     .asset3End = (void *)0x005B5590,
     .asset3Size = 0x1CDA,
     .anotherAssetIndex = 0xff110000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 62:  PBOX0 \0 */
    {
     .name = { 0x20, 0x50, 0x42, 0x4F, 0x58, 0x30, 0x20, 0x00 }, /*  PBOX0 \0 */
        .displayListStart = (void *)0x001ED5F0,
     .displayListEnd = (void *)0x001ED740,
     .vertexDataStart = (void *)0x004A5460,
     .vertexDataEnd = (void *)0x004A5650,
     .size = 0x370,
     .unk1C = (void *)0x800882D0,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .padding = 0x0000,
     .asset3Start = (void *)0x00000000,
     .asset3End = (void *)0x00000000,
     .asset3Size = 0x0,
     .anotherAssetIndex = 0xff020000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x800078C4,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 63:  PBOX1 \0 */
    {
     .name = { 0x20, 0x50, 0x42, 0x4F, 0x58, 0x31, 0x20, 0x00 }, /*  PBOX1 \0 */
        .displayListStart = (void *)0x001ED740,
     .displayListEnd = (void *)0x001ED890,
     .vertexDataStart = (void *)0x004A5650,
     .vertexDataEnd = (void *)0x004A5840,
     .size = 0x370,
     .unk1C = (void *)0x800882E0,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .padding = 0x0000,
     .asset3Start = (void *)0x00000000,
     .asset3End = (void *)0x00000000,
     .asset3Size = 0x0,
     .anotherAssetIndex = 0xff020000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x800078C4,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 64:  USAGI \0 */
    {
     .name = { 0x20, 0x55, 0x53, 0x41, 0x47, 0x49, 0x20, 0x00 }, /*  USAGI \0 */
        .displayListStart = (void *)0x001ED370,
     .displayListEnd = (void *)0x001ED5F0,
     .vertexDataStart = (void *)0x004A51C0,
     .vertexDataEnd = (void *)0x004A5460,
     .size = 0x5E0,
     .unk1C = (void *)0x800882C0,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .padding = 0x0000,
     .asset3Start = (void *)0x00000000,
     .asset3End = (void *)0x00000000,
     .asset3Size = 0x0,
     .anotherAssetIndex = 0xff040000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x80007B00,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 65:  SLASH4\0 */
    {
     .name = { 0x20, 0x53, 0x4C, 0x41, 0x53, 0x48, 0x34, 0x00 }, /*  SLASH4\0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x00,
     .padding = 0x015D,
     .asset3Start = (void *)0x004FDE20,
     .asset3End = (void *)0x00504740,
     .asset3Size = 0xAFF4,
     .anotherAssetIndex = 0xff050000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x8008976C,
     .count = 4,
     },
    /* Entry 66:  SLASH5\0 */
    {
     .name = { 0x20, 0x53, 0x4C, 0x41, 0x53, 0x48, 0x35, 0x00 }, /*  SLASH5\0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x00,
     .padding = 0x0138,
     .asset3Start = (void *)0x00504740,
     .asset3End = (void *)0x0050BFD0,
     .asset3Size = 0xC5C0,
     .anotherAssetIndex = 0xff050000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x8008976C,
     .count = 4,
     },
    /* Entry 67:  HELI  \0 */
    {
     .name = { 0x20, 0x48, 0x45, 0x4C, 0x49, 0x20, 0x20, 0x00 }, /*  HELI  \0 */
        .displayListStart = (void *)0x001ECAD0,
     .displayListEnd = (void *)0x001ED370,
     .vertexDataStart = (void *)0x004A4260,
     .vertexDataEnd = (void *)0x004A51C0,
     .size = 0x1A80,
     .unk1C = (void *)0x800882A0,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .padding = 0x0000,
     .asset3Start = (void *)0x00000000,
     .asset3End = (void *)0x00000000,
     .asset3Size = 0x0,
     .anotherAssetIndex = 0xff160000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x80007E40,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 68:  BURG  \0 */
    {
     .name = { 0x20, 0x42, 0x55, 0x52, 0x47, 0x20, 0x20, 0x00 }, /*  BURG  \0 */
        .displayListStart = (void *)0x001EC940,
     .displayListEnd = (void *)0x001ECAD0,
     .vertexDataStart = (void *)0x004A3E80,
     .vertexDataEnd = (void *)0x004A4260,
     .size = 0x620,
     .unk1C = (void *)0x80088290,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .padding = 0x0000,
     .asset3Start = (void *)0x00000000,
     .asset3End = (void *)0x00000000,
     .asset3Size = 0x0,
     .anotherAssetIndex = 0xff010000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 69:  MROBO \0 */
    {
     .name = { 0x20, 0x4D, 0x52, 0x4F, 0x42, 0x4F, 0x20, 0x00 }, /*  MROBO \0 */
        .displayListStart = (void *)0x001EAEF0,
     .displayListEnd = (void *)0x001EC940,
     .vertexDataStart = (void *)0x004A2BC0,
     .vertexDataEnd = (void *)0x004A3E80,
     .size = 0x4070,
     .unk1C = (void *)0x800881F0,
     .numAssets = (s8)0x0A,
     .assetGroupIndex = (s8)-0x01,
     .padding = 0x00A0,
     .asset3Start = (void *)0x005B7D30,
     .asset3End = (void *)0x005B9AC0,
     .asset3Size = 0x3F40,
     .anotherAssetIndex = 0xff040000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 70:  SLASHO\0 */
    {
     .name = { 0x20, 0x53, 0x4C, 0x41, 0x53, 0x48, 0x4F, 0x00 }, /*  SLASHO\0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x00,
     .padding = 0x00A0,
     .asset3Start = (void *)0x0050BFD0,
     .asset3End = (void *)0x00510AD0,
     .asset3Size = 0x737A,
     .anotherAssetIndex = 0xff050000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x8008976C,
     .count = 4,
     },
    /* Entry 71:  NANCYO\0 */
    {
     .name = { 0x20, 0x4E, 0x41, 0x4E, 0x43, 0x59, 0x4F, 0x00 }, /*  NANCYO\0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x01,
     .padding = 0x00A0,
     .asset3Start = (void *)0x005242F0,
     .asset3End = (void *)0x00527EE0,
     .asset3Size = 0x6CEE,
     .anotherAssetIndex = 0xff050000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x800897CC,
     .count = 4,
     },
    /* Entry 72:  JAMO  \0 */
    {
     .name = { 0x20, 0x4A, 0x41, 0x4D, 0x4F, 0x20, 0x20, 0x00 }, /*  JAMO  \0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x02,
     .padding = 0x00A0,
     .asset3Start = (void *)0x0053E160,
     .asset3End = (void *)0x00542AF0,
     .asset3Size = 0x66FC,
     .anotherAssetIndex = 0xff060000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x8008982C,
     .count = 4,
     },
    /* Entry 73:  LINDAO\0 */
    {
     .name = { 0x20, 0x4C, 0x49, 0x4E, 0x44, 0x41, 0x4F, 0x00 }, /*  LINDAO\0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x03,
     .padding = 0x00A0,
     .asset3Start = (void *)0x005596D0,
     .asset3End = (void *)0x0055E550,
     .asset3Size = 0x8938,
     .anotherAssetIndex = 0xff050000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x8008988C,
     .count = 4,
     },
    /* Entry 74:  TOMMYO\0 */
    {
     .name = { 0x20, 0x54, 0x4F, 0x4D, 0x4D, 0x59, 0x4F, 0x00 }, /*  TOMMYO\0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x04,
     .padding = 0x00A0,
     .asset3Start = (void *)0x00573660,
     .asset3End = (void *)0x00576600,
     .asset3Size = 0x5498,
     .anotherAssetIndex = 0xff070000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x800898EC,
     .count = 4,
     },
    /* Entry 75:  WENDYO\0 */
    {
     .name = { 0x20, 0x57, 0x45, 0x4E, 0x44, 0x59, 0x4F, 0x00 }, /*  WENDYO\0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x05,
     .padding = 0x00C8,
     .asset3Start = (void *)0x00590790,
     .asset3End = (void *)0x005944E0,
     .asset3Size = 0x5E66,
     .anotherAssetIndex = 0xff050000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x8008994C,
     .count = 4,
     },
    /* Entry 76:  BLACKO\0 */
    {
     .name = { 0x20, 0x42, 0x4C, 0x41, 0x43, 0x4B, 0x4F, 0x00 }, /*  BLACKO\0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x06,
     .padding = 0x00A0,
     .asset3Start = (void *)0x005A9D70,
     .asset3End = (void *)0x005AD1C0,
     .asset3Size = 0x53D6,
     .anotherAssetIndex = 0xff040000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x800899AC,
     .count = 4,
     },
    /* Entry 77:  PENO  \0 */
    {
     .name = { 0x20, 0x50, 0x45, 0x4E, 0x4F, 0x20, 0x20, 0x00 }, /*  PENO  \0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x07,
     .padding = 0x0174,
     .asset3Start = (void *)0x00576600,
     .asset3End = (void *)0x00580AD0,
     .asset3Size = 0xFE66,
     .anotherAssetIndex = 0xff040000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x80089A0C,
     .count = 4,
     },
    /* Entry 78:  DOGO  \0 */
    {
     .name = { 0x20, 0x44, 0x4F, 0x47, 0x4F, 0x20, 0x20, 0x00 }, /*  DOGO  \0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x08,
     .padding = 0x0020,
     .asset3Start = (void *)0x005B0030,
     .asset3End = (void *)0x005B05A0,
     .asset3Size = 0xB54,
     .anotherAssetIndex = 0xff080000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 79:  BOMB2 \0 */
    {
     .name = { 0x20, 0x42, 0x4F, 0x4D, 0x42, 0x32, 0x20, 0x00 }, /*  BOMB2 \0 */
        .displayListStart = (void *)0x001EA200,
     .displayListEnd = (void *)0x001EA480,
     .vertexDataStart = (void *)0x004A0E80,
     .vertexDataEnd = (void *)0x004A1160,
     .size = 0x7E0,
     .unk1C = (void *)0x800881A0,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .padding = 0x0000,
     .asset3Start = (void *)0x00000000,
     .asset3End = (void *)0x00000000,
     .asset3Size = 0x0,
     .anotherAssetIndex = 0xff020000,
     .soundSequenceDataStart = (void *)0x00648B50,
     .soundSequenceDataEnd = (void *)0x00648C60,
     .soundSequenceDataSize = 0x358,
     .initCallback = (void *)0x80006940,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 80:  SLASHN\0 */
    {
     .name = { 0x20, 0x53, 0x4C, 0x41, 0x53, 0x48, 0x4E, 0x00 }, /*  SLASHN\0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x00,
     .padding = 0x0020,
     .asset3Start = (void *)0x00476EB0,
     .asset3End = (void *)0x0047B2D0,
     .asset3Size = 0x8274,
     .anotherAssetIndex = 0xff050000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x8008976C,
     .count = 4,
     },
    /* Entry 81:  NANCYN\0 */
    {
     .name = { 0x20, 0x4E, 0x41, 0x4E, 0x43, 0x59, 0x4E, 0x00 }, /*  NANCYN\0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x01,
     .padding = 0x001A,
     .asset3Start = (void *)0x0047B2D0,
     .asset3End = (void *)0x0047EFC0,
     .asset3Size = 0x6CA4,
     .anotherAssetIndex = 0xff050000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x800897CC,
     .count = 4,
     },
    /* Entry 82:  JAMN  \0 */
    {
     .name = { 0x20, 0x4A, 0x41, 0x4D, 0x4E, 0x20, 0x20, 0x00 }, /*  JAMN  \0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x02,
     .padding = 0x001B,
     .asset3Start = (void *)0x0047EFC0,
     .asset3End = (void *)0x00482E40,
     .asset3Size = 0x6A3C,
     .anotherAssetIndex = 0xff060000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x8008982C,
     .count = 4,
     },
    /* Entry 83:  LINDAN\0 */
    {
     .name = { 0x20, 0x4C, 0x49, 0x4E, 0x44, 0x41, 0x4E, 0x00 }, /*  LINDAN\0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x03,
     .padding = 0x0017,
     .asset3Start = (void *)0x00482E40,
     .asset3End = (void *)0x004875E0,
     .asset3Size = 0x7D5A,
     .anotherAssetIndex = 0xff050000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x8008988C,
     .count = 4,
     },
    /* Entry 84:  TOMMYN\0 */
    {
     .name = { 0x20, 0x54, 0x4F, 0x4D, 0x4D, 0x59, 0x4E, 0x00 }, /*  TOMMYN\0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x04,
     .padding = 0x0024,
     .asset3Start = (void *)0x004875E0,
     .asset3End = (void *)0x0048E8F0,
     .asset3Size = 0xD8B6,
     .anotherAssetIndex = 0xff070000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x800898EC,
     .count = 4,
     },
    /* Entry 85:  WENDYN\0 */
    {
     .name = { 0x20, 0x57, 0x45, 0x4E, 0x44, 0x59, 0x4E, 0x00 }, /*  WENDYN\0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x05,
     .padding = 0x0019,
     .asset3Start = (void *)0x0048E8F0,
     .asset3End = (void *)0x004937A0,
     .asset3Size = 0x800E,
     .anotherAssetIndex = 0xff050000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x8008994C,
     .count = 4,
     },
    /* Entry 86:  BLACKN\0 */
    {
     .name = { 0x20, 0x42, 0x4C, 0x41, 0x43, 0x4B, 0x4E, 0x00 }, /*  BLACKN\0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x06,
     .padding = 0x0017,
     .asset3Start = (void *)0x004937A0,
     .asset3End = (void *)0x00498680,
     .asset3Size = 0x8F0C,
     .anotherAssetIndex = 0xff040000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x800899AC,
     .count = 4,
     },
    /* Entry 87:  PENN  \0 */
    {
     .name = { 0x20, 0x50, 0x45, 0x4E, 0x4E, 0x20, 0x20, 0x00 }, /*  PENN  \0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x07,
     .padding = 0x000D,
     .asset3Start = (void *)0x00498680,
     .asset3End = (void *)0x004998E0,
     .asset3Size = 0x252E,
     .anotherAssetIndex = 0xff040000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x80089A0C,
     .count = 4,
     },
    /* Entry 88:  DOGN  \0 */
    {
     .name = { 0x20, 0x44, 0x4F, 0x47, 0x4E, 0x20, 0x20, 0x00 }, /*  DOGN  \0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x08,
     .padding = 0x000D,
     .asset3Start = (void *)0x004998E0,
     .asset3End = (void *)0x0049B420,
     .asset3Size = 0x3668,
     .anotherAssetIndex = 0xff080000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 89:  DOOR  \0 */
    {
     .name = { 0x20, 0x44, 0x4F, 0x4F, 0x52, 0x20, 0x20, 0x00 }, /*  DOOR  \0 */
        .displayListStart = (void *)0x001E9C40,
     .displayListEnd = (void *)0x001EA200,
     .vertexDataStart = (void *)0x004A0670,
     .vertexDataEnd = (void *)0x004A0E80,
     .size = 0xF50,
     .unk1C = (void *)0x80088190,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .padding = 0x0000,
     .asset3Start = (void *)0x00000000,
     .asset3End = (void *)0x00000000,
     .asset3Size = 0x0,
     .anotherAssetIndex = 0xff000000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 90:  LTBL0 \0 */
    {
     .name = { 0x20, 0x4C, 0x54, 0x42, 0x4C, 0x30, 0x20, 0x00 }, /*  LTBL0 \0 */
        .displayListStart = (void *)0x001E9490,
     .displayListEnd = (void *)0x001E9790,
     .vertexDataStart = (void *)0x0049F270,
     .vertexDataEnd = (void *)0x0049FA20,
     .size = 0xBC0,
     .unk1C = (void *)0x80088170,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .padding = 0x0000,
     .asset3Start = (void *)0x00000000,
     .asset3End = (void *)0x00000000,
     .asset3Size = 0x0,
     .anotherAssetIndex = 0xff090000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 91:  LTBL1 \0 */
    {
     .name = { 0x20, 0x4C, 0x54, 0x42, 0x4C, 0x31, 0x20, 0x00 }, /*  LTBL1 \0 */
        .displayListStart = (void *)0x001E9300,
     .displayListEnd = (void *)0x001E9490,
     .vertexDataStart = (void *)0x0049F0D0,
     .vertexDataEnd = (void *)0x0049F270,
     .size = 0x4A0,
     .unk1C = (void *)0x80088160,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .padding = 0x0000,
     .asset3Start = (void *)0x00000000,
     .asset3End = (void *)0x00000000,
     .asset3Size = 0x0,
     .anotherAssetIndex = 0xff090000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 92:  GHOST \0 */
    {
     .name = { 0x20, 0x47, 0x48, 0x4F, 0x53, 0x54, 0x20, 0x00 }, /*  GHOST \0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x00,
     .assetGroupIndex = (s8)-0x01,
     .padding = 0x0000,
     .asset3Start = (void *)0x00000000,
     .asset3End = (void *)0x00000000,
     .asset3Size = 0x0,
     .anotherAssetIndex = 0xff000000,
     .soundSequenceDataStart = (void *)0x006462A0,
     .soundSequenceDataEnd = (void *)0x00646850,
     .soundSequenceDataSize = 0xA78,
     .initCallback = (void *)0x80006C50,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 93:  POST2 \0 */
    {
     .name = { 0x20, 0x50, 0x4F, 0x53, 0x54, 0x32, 0x20, 0x00 }, /*  POST2 \0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x00,
     .assetGroupIndex = (s8)-0x01,
     .padding = 0x0000,
     .asset3Start = (void *)0x00000000,
     .asset3End = (void *)0x00000000,
     .asset3Size = 0x0,
     .anotherAssetIndex = 0xff030000,
     .soundSequenceDataStart = (void *)0x00663010,
     .soundSequenceDataEnd = (void *)0x00663330,
     .soundSequenceDataSize = 0x848,
     .initCallback = (void *)0x80008310,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 94:  LTBL2 \0 */
    {
     .name = { 0x20, 0x4C, 0x54, 0x42, 0x4C, 0x32, 0x20, 0x00 }, /*  LTBL2 \0 */
        .displayListStart = (void *)0x001E8A90,
     .displayListEnd = (void *)0x001E8E60,
     .vertexDataStart = (void *)0x0049D8F0,
     .vertexDataEnd = (void *)0x0049E2E0,
     .size = 0x1240,
     .unk1C = (void *)0x80088140,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .padding = 0x0000,
     .asset3Start = (void *)0x00000000,
     .asset3End = (void *)0x00000000,
     .asset3Size = 0x0,
     .anotherAssetIndex = 0xff090000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 95:  LTBL3 \0 */
    {
     .name = { 0x20, 0x4C, 0x54, 0x42, 0x4C, 0x33, 0x20, 0x00 }, /*  LTBL3 \0 */
        .displayListStart = (void *)0x001E8900,
     .displayListEnd = (void *)0x001E8A90,
     .vertexDataStart = (void *)0x0049D710,
     .vertexDataEnd = (void *)0x0049D8F0,
     .size = 0x4A0,
     .unk1C = (void *)0x80088130,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .padding = 0x0000,
     .asset3Start = (void *)0x00000000,
     .asset3End = (void *)0x00000000,
     .asset3Size = 0x0,
     .anotherAssetIndex = 0xff090000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 96:  XXXXX \0 */
    {
     .name = { 0x20, 0x58, 0x58, 0x58, 0x58, 0x58, 0x20, 0x00 }, /*  XXXXX \0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x00,
     .assetGroupIndex = (s8)-0x01,
     .padding = 0x0000,
     .asset3Start = (void *)0x00000000,
     .asset3End = (void *)0x00000000,
     .asset3Size = 0x0,
     .anotherAssetIndex = 0xff040000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 97:  XXXXX \0 */
    {
     .name = { 0x20, 0x58, 0x58, 0x58, 0x58, 0x58, 0x20, 0x00 }, /*  XXXXX \0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x00,
     .assetGroupIndex = (s8)-0x01,
     .padding = 0x0000,
     .asset3Start = (void *)0x00000000,
     .asset3End = (void *)0x00000000,
     .asset3Size = 0x0,
     .anotherAssetIndex = 0xff040000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 98:  XXXXX \0 */
    {
     .name = { 0x20, 0x58, 0x58, 0x58, 0x58, 0x58, 0x20, 0x00 }, /*  XXXXX \0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x00,
     .assetGroupIndex = (s8)-0x01,
     .padding = 0x0000,
     .asset3Start = (void *)0x00000000,
     .asset3End = (void *)0x00000000,
     .asset3Size = 0x0,
     .anotherAssetIndex = 0xff080000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 99:  XXXXX \0 */
    {
     .name = { 0x20, 0x58, 0x58, 0x58, 0x58, 0x58, 0x20, 0x00 }, /*  XXXXX \0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x00,
     .assetGroupIndex = (s8)-0x01,
     .padding = 0x0000,
     .asset3Start = (void *)0x00000000,
     .asset3End = (void *)0x00000000,
     .asset3Size = 0x0,
     .anotherAssetIndex = 0xff000000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 100:  SLASH \0 */
    {
     .name = { 0x20, 0x53, 0x4C, 0x41, 0x53, 0x48, 0x20, 0x00 }, /*  SLASH \0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x00,
     .padding = 0x0011,
     .asset3Start = (void *)0x005BDB20,
     .asset3End = (void *)0x005C4040,
     .asset3Size = 0x7E3A,
     .anotherAssetIndex = 0xff050000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 101:  NANCY \0 */
    {
     .name = { 0x20, 0x4E, 0x41, 0x4E, 0x43, 0x59, 0x20, 0x00 }, /*  NANCY \0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x01,
     .padding = 0x0011,
     .asset3Start = (void *)0x005BDB20,
     .asset3End = (void *)0x005C4040,
     .asset3Size = 0x7E3A,
     .anotherAssetIndex = 0xff050000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 102:  JAM   \0 */
    {
     .name = { 0x20, 0x4A, 0x41, 0x4D, 0x20, 0x20, 0x20, 0x00 }, /*  JAM   \0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x02,
     .padding = 0x0011,
     .asset3Start = (void *)0x005BDB20,
     .asset3End = (void *)0x005C4040,
     .asset3Size = 0x7E3A,
     .anotherAssetIndex = 0xff060000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 103:  LINDA \0 */
    {
     .name = { 0x20, 0x4C, 0x49, 0x4E, 0x44, 0x41, 0x20, 0x00 }, /*  LINDA \0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x03,
     .padding = 0x0011,
     .asset3Start = (void *)0x005BDB20,
     .asset3End = (void *)0x005C4040,
     .asset3Size = 0x7E3A,
     .anotherAssetIndex = 0xff050000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 104:  TOMMY \0 */
    {
     .name = { 0x20, 0x54, 0x4F, 0x4D, 0x4D, 0x59, 0x20, 0x00 }, /*  TOMMY \0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x04,
     .padding = 0x0011,
     .asset3Start = (void *)0x005BDB20,
     .asset3End = (void *)0x005C4040,
     .asset3Size = 0x7E3A,
     .anotherAssetIndex = 0xff070000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 105:  WENDY \0 */
    {
     .name = { 0x20, 0x57, 0x45, 0x4E, 0x44, 0x59, 0x20, 0x00 }, /*  WENDY \0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x05,
     .padding = 0x0011,
     .asset3Start = (void *)0x005BDB20,
     .asset3End = (void *)0x005C4040,
     .asset3Size = 0x7E3A,
     .anotherAssetIndex = 0xff050000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 106:  BLACK \0 */
    {
     .name = { 0x20, 0x42, 0x4C, 0x41, 0x43, 0x4B, 0x20, 0x00 }, /*  BLACK \0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x06,
     .padding = 0x0011,
     .asset3Start = (void *)0x005BDB20,
     .asset3End = (void *)0x005C4040,
     .asset3Size = 0x7E3A,
     .anotherAssetIndex = 0xff040000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 107:  PEN   \0 */
    {
     .name = { 0x20, 0x50, 0x45, 0x4E, 0x20, 0x20, 0x20, 0x00 }, /*  PEN   \0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x07,
     .padding = 0x0011,
     .asset3Start = (void *)0x005BDB20,
     .asset3End = (void *)0x005C4040,
     .asset3Size = 0x7E3A,
     .anotherAssetIndex = 0xff040000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 108:  DOG   \0 */
    {
     .name = { 0x20, 0x44, 0x4F, 0x47, 0x20, 0x20, 0x20, 0x00 }, /*  DOG   \0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x08,
     .padding = 0x0011,
     .asset3Start = (void *)0x005BDB20,
     .asset3End = (void *)0x005C4040,
     .asset3Size = 0x7E3A,
     .anotherAssetIndex = 0xff080000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 109:  DINOS \0 */
    {
     .name = { 0x20, 0x44, 0x49, 0x4E, 0x4F, 0x53, 0x20, 0x00 }, /*  DINOS \0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x09,
     .padding = 0x0011,
     .asset3Start = (void *)0x005BDB20,
     .asset3End = (void *)0x005C4040,
     .asset3Size = 0x7E3A,
     .anotherAssetIndex = 0xff190000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 110:  SNOW  \0 */
    {
     .name = { 0x20, 0x53, 0x4E, 0x4F, 0x57, 0x20, 0x20, 0x00 }, /*  SNOW  \0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x03,
     .assetGroupIndex = (s8)0x0A,
     .padding = 0x0011,
     .asset3Start = (void *)0x005BDB20,
     .asset3End = (void *)0x005C4040,
     .asset3Size = 0x7E3A,
     .anotherAssetIndex = 0xff110000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 111:  ROBO  \0 */
    {
     .name = { 0x20, 0x52, 0x4F, 0x42, 0x4F, 0x20, 0x20, 0x00 }, /*  ROBO  \0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x10,
     .assetGroupIndex = (s8)0x0B,
     .padding = 0x0011,
     .asset3Start = (void *)0x005BDB20,
     .asset3End = (void *)0x005C4040,
     .asset3Size = 0x7E3A,
     .anotherAssetIndex = 0xff170000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 112:  MROBO \0 */
    {
     .name = { 0x20, 0x4D, 0x52, 0x4F, 0x42, 0x4F, 0x20, 0x00 }, /*  MROBO \0 */
        .displayListStart = (void *)0x001EAEF0,
     .displayListEnd = (void *)0x001EC940,
     .vertexDataStart = (void *)0x004A2BC0,
     .vertexDataEnd = (void *)0x004A3E80,
     .size = 0x4070,
     .unk1C = (void *)0x800881F0,
     .numAssets = (s8)0x0A,
     .assetGroupIndex = (s8)-0x01,
     .padding = 0x0011,
     .asset3Start = (void *)0x005BDB20,
     .asset3End = (void *)0x005C4040,
     .asset3Size = 0x7E3A,
     .anotherAssetIndex = 0xff040000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x00000000,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 113:  USAGI \0 */
    {
     .name = { 0x20, 0x55, 0x53, 0x41, 0x47, 0x49, 0x20, 0x00 }, /*  USAGI \0 */
        .displayListStart = (void *)0x001ED370,
     .displayListEnd = (void *)0x001ED5F0,
     .vertexDataStart = (void *)0x004A51C0,
     .vertexDataEnd = (void *)0x004A5460,
     .size = 0x5E0,
     .unk1C = (void *)0x800882C0,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .padding = 0x0000,
     .asset3Start = (void *)0x00000000,
     .asset3End = (void *)0x00000000,
     .asset3Size = 0x0,
     .anotherAssetIndex = 0xff040000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x80007B00,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 114:  BUTTE \0 */
    {
     .name = { 0x20, 0x42, 0x55, 0x54, 0x54, 0x45, 0x20, 0x00 }, /*  BUTTE \0 */
        .displayListStart = (void *)0x00000000,
     .displayListEnd = (void *)0x00000000,
     .vertexDataStart = (void *)0x00000000,
     .vertexDataEnd = (void *)0x00000000,
     .size = 0x0,
     .unk1C = (void *)0x00000000,
     .numAssets = (s8)0x00,
     .assetGroupIndex = (s8)-0x01,
     .padding = 0x0000,
     .asset3Start = (void *)0x00000000,
     .asset3End = (void *)0x00000000,
     .asset3Size = 0x0,
     .anotherAssetIndex = 0xff000000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x800063B4,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 115:  UFO   \0 */
    {
     .name = { 0x20, 0x55, 0x46, 0x4F, 0x20, 0x20, 0x20, 0x00 }, /*  UFO   \0 */
        .displayListStart = (void *)0x001F8D10,
     .displayListEnd = (void *)0x001F94F0,
     .vertexDataStart = (void *)0x004C20A0,
     .vertexDataEnd = (void *)0x004C3660,
     .size = 0x25A0,
     .unk1C = (void *)0x800885C0,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .padding = 0x0000,
     .asset3Start = (void *)0x00000000,
     .asset3End = (void *)0x00000000,
     .asset3Size = 0x0,
     .anotherAssetIndex = 0xff140000,
     .soundSequenceDataStart = (void *)0x004C3660,
     .soundSequenceDataEnd = (void *)0x004C3A60,
     .soundSequenceDataSize = 0x838,
     .initCallback = (void *)0x80005808,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
    /* Entry 116:  KANI  \0 */
    {
     .name = { 0x20, 0x4B, 0x41, 0x4E, 0x49, 0x20, 0x20, 0x00 }, /*  KANI  \0 */
        .displayListStart = (void *)0x001FC700,
     .displayListEnd = (void *)0x001FC9A0,
     .vertexDataStart = (void *)0x004CCA00,
     .vertexDataEnd = (void *)0x004CCE20,
     .size = 0x770,
     .unk1C = (void *)0x800886E0,
     .numAssets = (s8)0x01,
     .assetGroupIndex = (s8)-0x01,
     .padding = 0x0000,
     .asset3Start = (void *)0x00000000,
     .asset3End = (void *)0x00000000,
     .asset3Size = 0x0,
     .anotherAssetIndex = 0xff040000,
     .soundSequenceDataStart = (void *)0x00000000,
     .soundSequenceDataEnd = (void *)0x00000000,
     .soundSequenceDataSize = 0x0,
     .initCallback = (void *)0x80007380,
     .Assets = (void *)0x00000000,
     .count = 0,
     },
};

OSThread *__osActiveQueue_0 = (OSThread *)0x75;

char emptyAssetGroup[12] = "       \0\0\0";

u32 D_8008BD38_8C938[18] = { 0xFFE00008, 0x0,        0x7F0FFF0,  0xFFFFFFFF, 0x200008,   0x0,
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
            return loadCompressedData(entity->vertexDataStart, entity->vertexDataEnd, entity->romBSize);

        case MODE_DIRECT_FETCH:
            return entity->unk14;

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

void *loadAssetGroupVertexData(SceneModel *arg0) {
    AssetGroup *entity = &gameAssets[arg0->index];
    return loadCompressedData(entity->vertexDataStart, entity->vertexDataEnd, entity->size);
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
        asset2 = loadCompressedData(assetEntry->vertexDataStart, assetEntry->vertexDataEnd, assetEntry->size);

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
            ent->unk08 = loadCompressedData(assetEntry->asset3Start, assetEntry->asset3End, assetEntry->asset3Size);
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
    ent->unk124 = &D_8008BD38_8C938;
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