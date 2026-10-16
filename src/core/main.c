#include "core/main.h"
#include "generated/model_gfx.h"
#include "generated/modelpayload_asset_sizes.h"

#include "assets.h"
#include "common.h"
#include "data/model_display_lists.h"
#include "graphics/displaylist.h"
#include "graphics/graphics.h"
#include "graphics/sprite_table.h"
#include "graphics/tiled_texture.h"
#include "math/geometry.h"
#include "math/rand.h"
#include "system/task_scheduler.h"

typedef struct {
    u8 padding[0x2C];
    void *uncompressedAsset;
    void *compressedAsset;
} RotatingModelTaskState;

typedef struct {
    s32 unk0;
    s32 unk4;
    Vec3i pos;
    s32 unk14;
    s16 unk18;
    s16 unk1A;
    s32 unk1C;
    s32 unk20;
} SubEntryVariant;

typedef struct {
    char padding[44];
    void *uncompressedAsset;
    void *compressedAsset;
} SwingingModelTaskState;

typedef struct {
    char padding[0x2C];
    s32 *unk2C;
    s32 *unk30;
    char padding2[0x10];
    s32 *unk44;
} AnimatedModelTaskCleanupState;

typedef struct {
    void *unk0;
    void *unk4;
    Vec3i pos;
    s16 unk14;
    s16 unk16;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
} ModelEntityTaskConfig;

typedef struct {
    char name[8];
    /* 0x8 */ void *displayListStart;
    /* 0xC */ void *displayListEnd;
    /* 0x10 */ void *compressedDataStart;
    /* 0x14 */ void *compressedDataEnd;
    /* 0x18 */ s32 decompressedSize;
    void *unk1C;
    s32 unk20;
    void *unk24;
    s32 unk28;
    ModelEntityTaskConfig *taskConfigs;
    s16 taskCount;
    u16 unk32;
    u8 diffuseColorR;
    u8 diffuseColorG;
    u8 diffuseColorB;
    u8 unk37;
    u8 ambientColorR;
    u8 ambientColorG;
    u8 ambientColorB;
    u8 unk3B;
} ModelEntityConfig;

typedef struct {
    struct {
        u8 padding[0x14];
        s16 unk14;
        u16 unk16;
    } *ptr;
    u8 padding2[0x80];
    s16 unk84;
    s8 unk86;
    s8 unk87;
} func_80000C2C_182C_arg_unk0;

typedef struct {
    func_80000C2C_182C_arg_unk0 *unk0;
    s8 unk4;
    s8 unk5;
    Transform3D unk8;
    void *unk28;
    void *unk2C;
    void *unk30;
    s32 unk34;
    u8 padding[0xC];
    u16 unk44;
    u16 unk46;
    u16 unk48;
    u16 unk4A;
    s16 unk4C;
    s16 unk4E;
    u16 unk50;
    u16 unk52;
    s32 unk54;
    s32 unk58;
    s32 unk5C;
    s32 unk60;
} func_80000C2C_182C_arg;

typedef struct {
    func_80000C2C_182C_arg_unk0 *unk0;
    s8 unk4;
    s8 unk5;
    u8 pad6[2];
    Transform3D unk8;
    void *unk28;
    void *unk2C;
    void *unk30;
    s32 unk34;
    u8 padding[0xC];
    void *unk44;
    u8 pad48[4];
    s16 unk4C;
    s16 unk4E;
    u16 unk50;
    u16 unk52;
    s16 unk54;
    s16 unk56;
    s16 unk58;
} AnimatedModelTaskState;

typedef struct {
    u8 padding[0x8];
    ScrollingTextureState *unk8;
} func_800014C8_20C8_arg;

typedef struct {
    /* 0x00 */ func_80000C2C_182C_arg_unk0 *owner;
    /* 0x04 */ s8 configIndex;
    /* 0x05 */ s8 animationState;
    /* 0x06 */ u8 _pad6[2];
    /* 0x08 */ SpriteAssetState spriteState;
    /* 0x54 */ union {
        s16 spawnTimer;
        struct {
            Vec3i position;
            s32 scale;
        } spawned;
    } effect;
} SpriteEntityTaskState;

typedef struct {
    u8 padding[0x84];
    s16 unk84;
} StaticModelTaskArg_unk0;

typedef struct {
    StaticModelTaskArg_unk0 *unk0;
    s8 unk4;
    u8 padding[3];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    void *unk28;
    void *unk2C;
    void *unk30;
    s32 unk34;
} StaticModelTaskArg;

typedef struct {
    func_80000C2C_182C_arg_unk0 *unk0;
    s8 unk4;
    s8 unk5;
    DisplayListObject unk8;
    u16 unk44;
    u16 unk46;
    u16 unk48;
    u16 unk4A;
    s16 unk4C;
    s16 unk4E;
    u16 unk50;
    u16 unk52;
    s32 unk54;
    s32 unk58;
    s32 unk5C;
    s32 unk60;
} AnimatedModelTaskUpdateState;

#include "data/shared_model_part_display_lists.c"

void enqueueDisplayListIfVisible(func_80000C2C_182C_arg_unk0 *state, void *displayList);
void updateSpawnedSpriteTask(SpriteEntityTaskState *state);
void cleanupSpawnedSpriteTask(SpriteEntityTaskState *state);
void updateRotatingModelTask(func_80000C2C_182C_arg *arg0);
void cleanupRotatingModelTask(RotatingModelTaskState *state);
void updateStaticModelTask(func_80000C2C_182C_arg *arg0);
void cleanupStaticModelTask(SwingingModelTaskState *arg0);
void updateAnimatedModelTask(AnimatedModelTaskUpdateState *arg0);
void cleanupAnimatedModelTask(AnimatedModelTaskCleanupState *arg0);
void cleanupSpriteAnimationTask(SpriteEntityTaskState *state);
void cleanupSpriteSpawnerTask(SpriteEntityTaskState *state);
void updateSpriteSpawnerTask(SpriteEntityTaskState *state);
void initSpawnedSpriteTask(SpriteEntityTaskState *state);
void updateSpriteAnimationTask(SpriteEntityTaskState *state);
void updateSwingingModelTask(func_80000C2C_182C_arg *);
void cleanupSwingingModelTask(SwingingModelTaskState *);
void initRotatingModelTask(func_80000C2C_182C_arg *arg0);
void initSwingingModelTask(func_80000C2C_182C_arg *arg0);
void initSpriteAnimationTask(SpriteEntityTaskState *state);
void initAnimatedModelTask(AnimatedModelTaskState *arg0);
void initSpriteSpawnerTask(SpriteEntityTaskState *state);
void initStaticModelTask(StaticModelTaskArg *arg0);

extern DisplayLists D_80088620[1];
extern DisplayLists D_80088610_89210[1];

ModelEntityTaskConfig D_80088F20_89B20[] = {
    { initRotatingModelTask, &gTownRotatingModel0DisplayLists, { 0xFF8A0000, 0x00390000, 0xFF8A0000 }, -1, -64 },
    { initSwingingModelTask,
     &gTownSwingingModel0DisplayLists,
     { 0xFF2E0000, 0x00410000, 0xFFC80000 },
     0, 0x06AA,
     0x00000020, 0x000001C7 },
    { initSwingingModelTask,
     &gTownSwingingModel1DisplayLists,
     { 0x002E0000, 0x00330000, 0xFF590000 },
     0, 0x1EAB,
     0x00000010, 0x000000E3 },
    { initSwingingModelTask,
     &gTownSwingingModel2DisplayLists,
     { 0x00AD0000, 0x00390000, 0xFFAF0000 },
     0, 0x1956,
     0x00000010, 0x000000E3 },
    { initSwingingModelTask,
     &gTownSwingingModel3DisplayLists,
     { 0x00BE0000, 0x00390000, 0xFFF00000 },
     0, 0x1956,
     -16,
     0x000000E3 },
    { initRotatingModelTask, &gTownRotatingModel1DisplayLists, { 0xFF8A0000, 0x00390000, 0xFF8A0000 }, -1, -64 },
    { initSwingingModelTask,
     &gTownSwingingModel4DisplayLists,
     { 0xFF2E0000, 0x00410000, 0xFFC80000 },
     0, 0x06AA,
     0x00000020, 0x000001C7 },
    { initSwingingModelTask,
     &gTownSwingingModel5DisplayLists,
     { 0x002E0000, 0x00330000, 0xFF590000 },
     0, 0x1EAB,
     0x00000010, 0x000000E3 },
    { initSwingingModelTask,
     &gTownSwingingModel6DisplayLists,
     { 0x00AD0000, 0x00390000, 0xFFAF0000 },
     0, 0x1956,
     0x00000010, 0x000000E3 },
    { initSwingingModelTask,
     &gTownSwingingModel7PartDisplayLists,
     { 0x00BE0000, 0x00390000, 0xFFF00000 },
     0, 0x1956,
     -16,
     0x000000E3 }
};

ModelEntityTaskConfig D_80089088_89C88[] = {
    { initSpriteAnimationTask,
     0x00000000, { 0x00380000, 0x004C0000, 0xFF570000 },
     0, 1,
     0x00000000, 0x00029999,
     0x00000000 }
};

ModelEntityTaskConfig D_800890AC_89CAC[] = {
    { initAnimatedModelTask, &gJungleAnimatedModel1DisplayLists, { 0, 0, 0 },                            0, 4, 0, 0x00000004, 0 },
    { initAnimatedModelTask, &gJungleAnimatedModel0DisplayLists, { 0, 0, 0 },                            0, 4, 0, 0x00000008, 0 },
    { initSpriteSpawnerTask, 0,                                  { 0x00570000, 0x00020000, 0xFF470000 }, 0, 4, 0, 0x00010000, 0 },
    { initStaticModelTask,   &gJungleStaticModelDisplayLists,    { 0, 0, 0 },                            0, 0, 0, 0,          0 }
};

ModelEntityTaskConfig D_8008913C_89D3C[] = {
    { initAnimatedModelTask, &gJungle2AnimatedModel1DisplayLists, { 0, 0, 0 },                            0, 4, 0, 0x00000004, 0 },
    { initAnimatedModelTask, &gJungle2AnimatedModel0DisplayLists, { 0, 0, 0 },                            0, 4, 0, 0x00000008, 0 },
    { initSpriteSpawnerTask, 0,                                   { 0x00570000, 0x00020000, 0xFF470000 }, 0, 4, 0, 0x00010000, 0 }
};

ModelEntityConfig modelEntityConfigs[14] = {
    { .name = "TOWN   ",
     .displayListStart = (void *)&TOWN_DISPLAY_LIST_ROM_START,
     .displayListEnd = (void *)&TOWN_DISPLAY_LIST_ROM_END,
     .compressedDataStart = (void *)&TOWN_COMPRESSED_DATA_ROM_START,
     .compressedDataEnd = (void *)&TOWN_COMPRESSED_DATA_ROM_END,
     .decompressedSize = TOWN_COMPRESSED_DATA_DECOMPRESSED_SIZE,
     .unk1C = &gTownPrimaryDisplayLists,
     .unk20 = 0x00010000,
     .unk24 = &gTownSecondaryPartDisplayLists,
     .unk28 = 0x00010000,
     .taskConfigs = D_80088F20_89B20,
     .taskCount = 5,
     .unk32 = 0xFFFF,
     .diffuseColorR = 0xFF,
     .diffuseColorG = 0xFF,
     .diffuseColorB = 0xC0,
     .unk37 = 0xFF,
     .ambientColorR = 0x60,
     .ambientColorG = 0x70,
     .ambientColorB = 0x80,
     .unk3B = 0xFF },
    { .name = "TOWN   ",
     .displayListStart = (void *)&TOWN_DISPLAY_LIST_ROM_START,
     .displayListEnd = (void *)&TOWN_DISPLAY_LIST_ROM_END,
     .compressedDataStart = (void *)&TOWN_COMPRESSED_DATA_ROM_START,
     .compressedDataEnd = (void *)&TOWN_COMPRESSED_DATA_ROM_END,
     .decompressedSize = TOWN_COMPRESSED_DATA_DECOMPRESSED_SIZE,
     .unk1C = &gTownPrimaryDisplayLists,
     .unk20 = 0x00010000,
     .unk24 = &gTownSecondaryPartDisplayLists,
     .unk28 = 0x00010000,
     .taskConfigs = D_80088F20_89B20,
     .taskCount = 5,
     .unk32 = 0xFFFF,
     .diffuseColorR = 0xFF,
     .diffuseColorG = 0xFF,
     .diffuseColorB = 0xFF,
     .unk37 = 0xFF,
     .ambientColorR = 0xD0,
     .ambientColorG = 0xD0,
     .ambientColorB = 0xD0,
     .unk3B = 0xFF },
    { .name = "YAMA   ",
     .displayListStart = (void *)&YAMA_DISPLAY_LIST_ROM_START,
     .displayListEnd = (void *)&YAMA_DISPLAY_LIST_ROM_END,
     .compressedDataStart = (void *)&YAMA_COMPRESSED_DATA_ROM_START,
     .compressedDataEnd = (void *)&YAMA_COMPRESSED_DATA_ROM_END,
     .decompressedSize = YAMA_COMPRESSED_DATA_DECOMPRESSED_SIZE,
     .unk1C = &gYamaPrimaryDisplayLists,
     .unk20 = 0x00010000,
     .unk24 = &gYamaSecondaryDisplayLists,
     .unk28 = 0x00010000,
     .taskConfigs = D_80089088_89C88,
     .taskCount = 1,
     .unk32 = 0xFFFF,
     .diffuseColorR = 0xFF,
     .diffuseColorG = 0xFF,
     .diffuseColorB = 0xC0,
     .unk37 = 0xFF,
     .ambientColorR = 0x80,
     .ambientColorG = 0x80,
     .ambientColorB = 0xA0,
     .unk3B = 0xFF },
    { .name = "YAMA_Y ",
     .displayListStart = (void *)&YAMA_Y_DISPLAY_LIST_ROM_START,
     .displayListEnd = (void *)&YAMA_Y_DISPLAY_LIST_ROM_END,
     .compressedDataStart = (void *)&YAMA_Y_COMPRESSED_DATA_ROM_START,
     .compressedDataEnd = (void *)&YAMA_Y_COMPRESSED_DATA_ROM_END,
     .decompressedSize = YAMA_Y_COMPRESSED_DATA_DECOMPRESSED_SIZE,
     .unk1C = &gYamaYPrimaryDisplayLists,
     .unk20 = 0x00010000,
     .unk24 = &gYamaYSecondaryDisplayLists,
     .unk28 = 0x00010000,
     .taskConfigs = D_80089088_89C88,
     .taskCount = 1,
     .unk32 = 0xFFFF,
     .diffuseColorR = 0xFF,
     .diffuseColorG = 0xC0,
     .diffuseColorB = 0x00,
     .unk37 = 0xFF,
     .ambientColorR = 0x60,
     .ambientColorG = 0x50,
     .ambientColorB = 0x40,
     .unk3B = 0xFF },
    { .name = "LINDA P",
     .displayListStart = (void *)&LINDA_P_DISPLAY_LIST_ROM_START,
     .displayListEnd = (void *)&LINDA_P_DISPLAY_LIST_ROM_END,
     .compressedDataStart = (void *)&LINDA_P_COMPRESSED_DATA_ROM_START,
     .compressedDataEnd = (void *)&LINDA_P_COMPRESSED_DATA_ROM_END,
     .decompressedSize = LINDA_P_COMPRESSED_DATA_DECOMPRESSED_SIZE,
     .unk1C = &gLindaPDisplayLists,
     .unk20 = 0x00010000,
     .taskConfigs = NULL,
     .unk32 = 0xFFFF,
     .diffuseColorR = 0xFF,
     .diffuseColorG = 0xFF,
     .diffuseColorB = 0xFF,
     .unk37 = 0xFF,
     .ambientColorR = 0x80,
     .ambientColorG = 0x70,
     .ambientColorB = 0x60,
     .unk3B = 0xFF },
    { .name = "LINDA B",
     .displayListStart = (void *)&LINDA_B_DISPLAY_LIST_ROM_START,
     .displayListEnd = (void *)&LINDA_B_DISPLAY_LIST_ROM_END,
     .compressedDataStart = (void *)&LINDA_B_COMPRESSED_DATA_ROM_START,
     .compressedDataEnd = (void *)&LINDA_B_COMPRESSED_DATA_ROM_END,
     .decompressedSize = LINDA_B_COMPRESSED_DATA_DECOMPRESSED_SIZE,
     .unk1C = &gLindaBDisplayLists,
     .unk20 = 0x00010000,
     .unk24 = 0,
     .unk28 = 0,
     .taskConfigs = NULL,
     .taskCount = 0,
     .unk32 = 0xFFFF,
     .diffuseColorR = 0xFF,
     .diffuseColorG = 0xFF,
     .diffuseColorB = 0xFF,
     .unk37 = 0xFF,
     .ambientColorR = 0x80,
     .ambientColorG = 0x70,
     .ambientColorB = 0x60,
     .unk3B = 0xFF },
    { .name = "WENDY H",
     .displayListStart = (void *)&WENDY_H_DISPLAY_LIST_ROM_START,
     .displayListEnd = (void *)&WENDY_H_DISPLAY_LIST_ROM_END,
     .compressedDataStart = (void *)&WENDY_H_COMPRESSED_DATA_ROM_START,
     .compressedDataEnd = (void *)&WENDY_H_COMPRESSED_DATA_ROM_END,
     .decompressedSize = WENDY_H_COMPRESSED_DATA_DECOMPRESSED_SIZE,
     .unk1C = &gWendyHousePrimaryDisplayLists,
     .unk20 = 0x00010000,
     .unk24 = 0,
     .unk28 = 0,
     .taskConfigs = NULL,
     .taskCount = 0,
     .unk32 = 0xFFFF,
     .diffuseColorR = 0xFF,
     .diffuseColorG = 0xFF,
     .diffuseColorB = 0xFF,
     .unk37 = 0xFF,
     .ambientColorR = 0x80,
     .ambientColorG = 0x80,
     .ambientColorB = 0xA0,
     .unk3B = 0xFF },
    { .name = "BEACH  ",
     .displayListStart = (void *)&TOWN_DISPLAY_LIST_ROM_START,
     .displayListEnd = (void *)&TOWN_DISPLAY_LIST_ROM_END,
     .compressedDataStart = (void *)&TOWN_COMPRESSED_DATA_ROM_START,
     .compressedDataEnd = (void *)&TOWN_COMPRESSED_DATA_ROM_END,
     .decompressedSize = TOWN_COMPRESSED_DATA_DECOMPRESSED_SIZE,
     .unk1C = &gTownPrimaryDisplayLists,
     .unk20 = 0x00010000,
     .unk24 = &gTownSecondaryPartDisplayLists,
     .unk28 = 0x00010000,
     .taskConfigs = NULL,
     .taskCount = 0,
     .unk32 = 0xFFFF,
     .diffuseColorR = 0xFF,
     .diffuseColorG = 0xFF,
     .diffuseColorB = 0xFF,
     .unk37 = 0xFF,
     .ambientColorR = 0xD0,
     .ambientColorG = 0xD0,
     .ambientColorB = 0xD0,
     .unk3B = 0xFF },
    { .name = "JUNGLE ",
     .displayListStart = (void *)&JUNGLE_DISPLAY_LIST_ROM_START,
     .displayListEnd = (void *)&JUNGLE_DISPLAY_LIST_ROM_END,
     .compressedDataStart = (void *)&JUNGLE_COMPRESSED_DATA_ROM_START,
     .compressedDataEnd = (void *)&JUNGLE_COMPRESSED_DATA_ROM_END,
     .decompressedSize = JUNGLE_COMPRESSED_DATA_DECOMPRESSED_SIZE,
     .unk1C = &gJunglePrimaryDisplayLists,
     .unk20 = 0x00010000,
     .unk24 = &gJungleSecondaryDisplayLists,
     .unk28 = 0x00010000,
     .taskConfigs = D_800890AC_89CAC,
     .taskCount = 4,
     .unk32 = 0xFFFF,
     .diffuseColorR = 0xFF,
     .diffuseColorG = 0xFF,
     .diffuseColorB = 0xFF,
     .unk37 = 0xFF,
     .ambientColorR = 0x60,
     .ambientColorG = 0x60,
     .ambientColorB = 0x60,
     .unk3B = 0xFF },
    { .name = "SPACE  ",
     .displayListStart = (void *)&SPACE_DISPLAY_LIST_ROM_START,
     .displayListEnd = (void *)&SPACE_DISPLAY_LIST_ROM_END,
     .compressedDataStart = (void *)&SPACE_COMPRESSED_DATA_ROM_START,
     .compressedDataEnd = (void *)&SPACE_COMPRESSED_DATA_ROM_END,
     .decompressedSize = SPACE_COMPRESSED_DATA_DECOMPRESSED_SIZE,
     .unk1C = &gSpacePrimaryDisplayLists,
     .unk20 = 0x00010000,
     .unk24 = &gSpaceSecondaryDisplayLists,
     .unk28 = 0x00010000,
     .taskConfigs = NULL,
     .taskCount = 0,
     .unk32 = 0xFFFF,
     .diffuseColorR = 0xFF,
     .diffuseColorG = 0xFF,
     .diffuseColorB = 0x80,
     .unk37 = 0xFF,
     .ambientColorR = 0x00,
     .ambientColorG = 0x20,
     .ambientColorB = 0x40,
     .unk3B = 0xFF },
    { .name = "ISLAND ",
     .displayListStart = (void *)&ISLAND_DISPLAY_LIST_ROM_START,
     .displayListEnd = (void *)&ISLAND_DISPLAY_LIST_ROM_END,
     .compressedDataStart = (void *)&ISLAND_COMPRESSED_DATA_ROM_START,
     .compressedDataEnd = (void *)&ISLAND_COMPRESSED_DATA_ROM_END,
     .decompressedSize = ISLAND_COMPRESSED_DATA_DECOMPRESSED_SIZE,
     .unk1C = &D_80088610_89210,
     .unk20 = 0x00010000,
     .unk24 = &D_80088620,
     .unk28 = 0x00010000,
     .taskConfigs = NULL,
     .unk32 = 0xFFFF,
     .diffuseColorR = 0xFF,
     .diffuseColorG = 0xFF,
     .diffuseColorB = 0xFF,
     .unk37 = 0xFF,
     .ambientColorR = 0x60,
     .ambientColorG = 0x60,
     .ambientColorB = 0x60,
     .unk3B = 0xFF },
    { .name = "HAUNTED",
     .displayListStart = (void *)&OTHER_HAUNTED_DISPLAY_LIST_ROM_START,
     .displayListEnd = (void *)&OTHER_HAUNTED_DISPLAY_LIST_ROM_END,
     .compressedDataStart = (void *)&OTHER_HAUNTED_COMPRESSED_DATA_ROM_START,
     .compressedDataEnd = (void *)&OTHER_HAUNTED_COMPRESSED_DATA_ROM_END,
     .decompressedSize = OTHER_HAUNTED_COMPRESSED_DATA_DECOMPRESSED_SIZE,
     .unk1C = &gOtherHauntedPrimaryDisplayLists,
     .unk20 = 0x00010000,
     .unk24 = &gOtherHauntedSecondaryPartDisplayLists,
     .unk28 = 0x00010000,
     .taskConfigs = NULL,
     .taskCount = 0,
     .unk32 = 0xFFFF,
     .diffuseColorR = 0xFF,
     .diffuseColorG = 0x80,
     .diffuseColorB = 0x00,
     .unk37 = 0xFF,
     .ambientColorR = 0x00,
     .ambientColorG = 0x20,
     .ambientColorB = 0x80,
     .unk3B = 0xFF },
    { .name = "JUNGLE2",
     .displayListStart = (void *)&JUNGLE2_DISPLAY_LIST_ROM_START,
     .displayListEnd = (void *)&JUNGLE2_DISPLAY_LIST_ROM_END,
     .compressedDataStart = (void *)&JUNGLE2_COMPRESSED_DATA_ROM_START,
     .compressedDataEnd = (void *)&JUNGLE2_COMPRESSED_DATA_ROM_END,
     .decompressedSize = JUNGLE2_COMPRESSED_DATA_DECOMPRESSED_SIZE,
     .unk1C = &gJungle2PrimaryDisplayLists,
     .unk20 = 0x00010000,
     .unk24 = &gJungle2SecondaryDisplayLists,
     .unk28 = 0x00010000,
     .taskConfigs = D_8008913C_89D3C,
     .taskCount = 3,
     .unk32 = 0xFFFF,
     .diffuseColorR = 0xFA,
     .diffuseColorG = 0xFA,
     .diffuseColorB = 0xA7,
     .unk37 = 0xFF,
     .ambientColorR = 0x4A,
     .ambientColorG = 0x40,
     .ambientColorB = 0xBF,
     .unk3B = 0xFF },
    { .name = "OPENING",
     .diffuseColorR = 0xFF,
     .diffuseColorG = 0xFF,
     .diffuseColorB = 0xFF,
     .unk37 = 0xFF,
     .ambientColorR = 0xD0,
     .ambientColorG = 0xD0,
     .ambientColorB = 0xD0,
     .unk3B = 0xFF }
};

s32 spawnedSpriteScales[4] = { 0x00010000, 0x00011999, 0x0000E666, 0x00013333 };

DisplayLists D_80089500[1] = {
    { 0x00000000, ITEM_0_DISPLAY_LIST_part_0148, NULL, NULL }
};

void setModelEntityVisibility(ModelEntity *entity, s8 isVisible) {
    entity->isVisible = isVisible;
}

s32 osVoiceCheckWord(u8 *data) {
    return 0xE;
}

void setupModelEntityLighting(ModelEntity *entity, DirectionalLightData *lightColors, AmbientLightData *ambientLight) {
    s32 temp_v0;
    s32 quarterAverage;
    int colorSum;
    s32 halfAverage;
    s32 average;
    u8 blueValue;
    s32 new_var;
    ModelEntityConfig *config;

    config = &modelEntityConfigs[entity->configIndex];
    colorSum = config->diffuseColorR + config->diffuseColorG;
    temp_v0 = colorSum;
    blueValue = config->diffuseColorB;
    new_var = (temp_v0 + blueValue) / 3;
    average = new_var;
    temp_v0 = ((u32)new_var) >> 0x1F;
    temp_v0 = average + ((s8)temp_v0);
    new_var = temp_v0 >> 0x1;
    halfAverage = new_var;
    if (average < 0) {
        average += 3;
    }
    lightColors[0].directionX = 0;
    lightColors[0].directionY = 0x7F;
    lightColors[0].directionZ = 0x7F;
    lightColors[0].r = config->diffuseColorR;
    lightColors[0].g = config->diffuseColorG;
    quarterAverage = average >> 2;
    lightColors[0].b = config->diffuseColorB;
    lightColors[1].r = quarterAverage;
    lightColors[1].g = quarterAverage;
    lightColors[1].b = quarterAverage;
    lightColors[1].directionX = 0x7F;
    lightColors[1].directionY = 0x7F;
    lightColors[1].directionZ = 0;
    lightColors[2].directionX = -0x7F;
    lightColors[2].directionY = 0x7F;
    lightColors[2].directionZ = 0;
    lightColors[2].r = halfAverage;
    lightColors[2].g = halfAverage;
    lightColors[2].b = halfAverage;
    ambientLight->color.r = config->ambientColorR;
    ambientLight->color.g = config->ambientColorG;
    ambientLight->color.b = config->ambientColorB;
    setViewportLightColors(entity->viewport->viewportId, 3, lightColors, &ambientLight->color);
}

typedef struct {
    void *unk0;
    u8 unk4;
    u8 unk5;
} initModelEntity_task;

s32 initModelEntity(ModelEntity *entity, s16 index, ViewportNode *viewport) {
    ModelEntityConfig *entry;
    s32 i;

    entity->configIndex = index;
    entity->isVisible = 1;
    entry = &modelEntityConfigs[index];
    entity->viewport = viewport;
    entity->isDisposed = 0;

    if (entry->unk32 == 0xFFFF) {
        entity->displayListData = loadUncompressedData(entry->displayListStart, entry->displayListEnd);
        entity->textureData =
            loadCompressedData(entry->compressedDataStart, entry->compressedDataEnd, entry->decompressedSize);

        memcpy(&entity->primaryDisplayList.transform, &identityMatrix, sizeof(Transform3D));

        entity->primaryDisplayList.segment3 = 0;
        entity->primaryDisplayList.segment1 = entity->displayListData;
        entity->primaryDisplayList.segment2 = entity->textureData;
        entity->primaryDisplayList.displayLists = entry->unk1C;

        if (entry->unk24 != 0) {
            memcpy(&entity->secondaryDisplayList.transform, &identityMatrix, sizeof(Transform3D));
            entity->secondaryDisplayList.segment1 = entity->displayListData;
            entity->secondaryDisplayList.segment2 = entity->textureData;
            entity->secondaryDisplayList.displayLists = entry->unk24;
        } else {
            entity->secondaryDisplayList.displayLists = 0;
            entity->secondaryDisplayList.segment2 = 0;
            entity->secondaryDisplayList.segment1 = 0;
        }

        entity->secondaryDisplayList.segment3 = 0;

        for (i = 0; i < entry->taskCount; i++) {
            initModelEntity_task *task = scheduleTask((entry->taskConfigs + i)->unk0, 0, 0, 0xC8);
            if (task != NULL) {
                task->unk0 = entity;
                task->unk4 = i;
                task->unk5 = 0;
            }
        }

    } else {
        entity->displayListData = 0;
        entity->textureData = 0;
        entity->primaryDisplayList.segment1 = 0;
        entity->primaryDisplayList.segment2 = 0;
        entity->primaryDisplayList.segment3 = 0;
        entity->primaryDisplayList.displayLists = 0;
        entity->secondaryDisplayList.segment1 = 0;
        entity->secondaryDisplayList.segment2 = 0;
        entity->secondaryDisplayList.displayLists = 0;
        entity->secondaryDisplayList.segment3 = 0;

        return 1;
    }

    return 0;
}

void cleanupModelEntity(ModelEntity *entity) {
    if (entity->displayListData != 0) {
        entity->textureData = freeNodeMemory(entity->textureData);
        entity->displayListData = freeNodeMemory(entity->displayListData);
    }
    entity->isDisposed = 1;
}

void renderModelEntity(ModelEntity *entity) {
    if (entity->isVisible != 0) {
        if (entity->primaryDisplayList.segment1 != 0) {
            enqueueDisplayListObject(entity->viewport->callbackSlotIndex, &entity->primaryDisplayList);
        }
        if (entity->secondaryDisplayList.segment1 != 0) {
            enqueueDisplayListObjectWithFullRenderState(
                entity->viewport->callbackSlotIndex,
                &entity->secondaryDisplayList
            );
        }
    }
}

void enqueueDisplayListIfVisible(func_80000C2C_182C_arg_unk0 *state, void *displayList) {
    if (state->unk87 != 0) {
        enqueueDisplayListObject(state->ptr->unk16, displayList);
    }
}

void initRotatingModelTask(func_80000C2C_182C_arg *arg0) {
    ModelEntityConfig *config = &modelEntityConfigs[arg0->unk0->unk84];
    ModelEntityTaskConfig *taskConfig = &config->taskConfigs[arg0->unk4];

    setCleanupCallback(&cleanupRotatingModelTask);
    memcpy(&arg0->unk8, &identityMatrix, sizeof(Transform3D));

    arg0->unk2C = loadUncompressedData(config->displayListStart, config->displayListEnd);
    arg0->unk30 = loadCompressedData(config->compressedDataStart, config->compressedDataEnd, config->decompressedSize);
    arg0->unk34 = 0;
    arg0->unk28 = taskConfig->unk4;
    arg0->unk8.translation.x = taskConfig->pos.x;
    arg0->unk8.translation.y = taskConfig->pos.y;
    arg0->unk8.translation.z = taskConfig->pos.z;
    arg0->unk44 = 0;

    setCallback(&updateRotatingModelTask);
}

void updateRotatingModelTask(func_80000C2C_182C_arg *arg0) {
    u16 rotationAngle;
    ModelEntityConfig *config = &modelEntityConfigs[arg0->unk0->unk84];
    ModelEntityTaskConfig *subEntry = &config->taskConfigs[arg0->unk4];

    if (arg0->unk0->unk86 != 0) {
        terminateCurrentTask();
    }

    rotationAngle = ((SubEntryVariant *)subEntry)->unk14;
    rotationAngle = arg0->unk44 + rotationAngle;
    arg0->unk44 = rotationAngle;

    createYRotationMatrix(&arg0->unk8, rotationAngle);
    enqueueDisplayListIfVisible(arg0->unk0, &arg0->unk8);
}

void cleanupRotatingModelTask(RotatingModelTaskState *state) {
    state->compressedAsset = freeNodeMemory(state->compressedAsset);
    state->uncompressedAsset = freeNodeMemory(state->uncompressedAsset);
}

void initSwingingModelTask(func_80000C2C_182C_arg *arg0) {
    ModelEntityConfig *temp_s0;
    ModelEntityTaskConfig *temp_s2;

    temp_s0 = &modelEntityConfigs[arg0->unk0->unk84];
    temp_s2 = &temp_s0->taskConfigs[arg0->unk4];

    setCleanupCallback(&cleanupSwingingModelTask);

    memcpy(&arg0->unk8, &identityMatrix, sizeof(Transform3D));

    arg0->unk2C = loadUncompressedData(temp_s0->displayListStart, temp_s0->displayListEnd);
    arg0->unk30 =
        loadCompressedData(temp_s0->compressedDataStart, temp_s0->compressedDataEnd, temp_s0->decompressedSize);
    arg0->unk34 = 0;
    arg0->unk28 = temp_s2->unk4;
    arg0->unk44 = 0;

    setCallback(&updateSwingingModelTask);
}

void updateSwingingModelTask(func_80000C2C_182C_arg *arg0) {
    Transform3D yRotMatrix;
    Transform3D zRotMatrix;
    ModelEntityConfig *config;
    ModelEntityTaskConfig *subEntry;

    config = &modelEntityConfigs[arg0->unk0->unk84];
    subEntry = &config->taskConfigs[arg0->unk4];
    if (arg0->unk0->unk86 != 0) {
        terminateCurrentTask();
    }

    switch (arg0->unk5) {
        case 0:
            arg0->unk44 += subEntry->unk18;
            if (subEntry->unk18 > 0) {
                if ((s16)arg0->unk44 > subEntry->unk1C) {
                    arg0->unk5 = 1;
                }
            } else {
                if ((s16)arg0->unk44 < -subEntry->unk1C) {
                    arg0->unk5 = 1;
                }
            }
            break;

        case 1:
            arg0->unk44 -= subEntry->unk18;
            if (subEntry->unk18 > 0) {
                if ((s16)arg0->unk44 < -subEntry->unk1C) {
                    arg0->unk5 = 0;
                }
            } else {
                if ((s16)arg0->unk44 > subEntry->unk1C) {
                    arg0->unk5 = 0;
                }
            }
            break;
    }

    createYRotationMatrix(&yRotMatrix, subEntry->unk16);
    createZRotationMatrix(&zRotMatrix, arg0->unk44);
    composeTransform3D(&zRotMatrix, &yRotMatrix, &arg0->unk8);

    arg0->unk8.translation.x = subEntry->pos.x;
    arg0->unk8.translation.y = subEntry->pos.y;
    arg0->unk8.translation.z = subEntry->pos.z;

    enqueueDisplayListIfVisible(arg0->unk0, &arg0->unk8);
}

void cleanupSwingingModelTask(SwingingModelTaskState *state) {
    state->compressedAsset = freeNodeMemory(state->compressedAsset);
    state->uncompressedAsset = freeNodeMemory(state->uncompressedAsset);
}

void initSpriteAnimationTask(SpriteEntityTaskState *state) {
    ModelEntityConfig *config = &modelEntityConfigs[state->owner->unk84];
    ModelEntityTaskConfig *subEntry = &config->taskConfigs[state->configIndex];
    setCleanupCallback(&cleanupSpriteAnimationTask);
    loadSpriteAsset(&state->spriteState, subEntry->unk16);
    setCallback(&updateSpriteAnimationTask);
}

void updateSpriteAnimationTask(SpriteEntityTaskState *state) {
    ModelEntityConfig *config = &modelEntityConfigs[state->owner->unk84];
    SubEntryVariant *subEntry = (SubEntryVariant *)&config->taskConfigs[state->configIndex];

    if (state->owner->unk86 != 0) {
        terminateCurrentTask();
    }

    switch (state->animationState) {
        case 0:
            setSpriteAnimation(&state->spriteState, 0x10000, subEntry->unk1A, -1);
            state->animationState = 1;
            break;
        case 1:
            updateSpriteAnimation(&state->spriteState, 0x10000);
            break;
    }

    if (state->owner->unk87 != 0) {
        renderOpaqueSprite(
            &state->spriteState,
            state->owner->ptr->unk16,
            subEntry->pos.x,
            subEntry->pos.y,
            subEntry->pos.z,
            subEntry->unk1C,
            subEntry->unk1C,
            0,
            0
        );
    }
}

void cleanupSpriteAnimationTask(SpriteEntityTaskState *state) {
    releaseNodeMemoryRef((void **)&state->spriteState.assetData);
}

void initStaticModelTask(StaticModelTaskArg *arg0) {
    ModelEntityConfig *var_s0;
    ModelEntityTaskConfig *var_s2;
    s16 temp;
    s8 temp2;

    temp = arg0->unk0->unk84;
    temp2 = arg0->unk4;
    var_s0 = &modelEntityConfigs[temp];
    var_s2 = &var_s0->taskConfigs[temp2];

    setCleanupCallback(cleanupStaticModelTask);

    memcpy(&arg0->unk8, &identityMatrix, sizeof(Transform3D));

    arg0->unk2C = loadUncompressedData(var_s0->displayListStart, var_s0->displayListEnd);
    arg0->unk30 = loadCompressedData(var_s0->compressedDataStart, var_s0->compressedDataEnd, var_s0->decompressedSize);
    arg0->unk34 = 0;
    arg0->unk28 = var_s2->unk4;

    setCallback(updateStaticModelTask);
}

void updateStaticModelTask(func_80000C2C_182C_arg *arg0) {
    ModelEntityConfig *config = &modelEntityConfigs[arg0->unk0->unk84];
    ModelEntityTaskConfig *subEntry = &config->taskConfigs[arg0->unk4];

    if (arg0->unk0->unk86 != 0) {
        terminateCurrentTask();
    }

    arg0->unk8.translation.x = subEntry->pos.x;
    arg0->unk8.translation.y = subEntry->pos.y;
    arg0->unk8.translation.z = subEntry->pos.z;
    enqueueDisplayListIfVisible(arg0->unk0, &arg0->unk8);
}

void cleanupStaticModelTask(SwingingModelTaskState *state) {
    state->compressedAsset = freeNodeMemory(state->compressedAsset);
    state->uncompressedAsset = freeNodeMemory(state->uncompressedAsset);
}

void initAnimatedModelTask(AnimatedModelTaskState *arg0) {
    ModelEntityConfig *temp_s0;
    ModelEntityTaskConfig *temp_s2;

    temp_s0 = &modelEntityConfigs[arg0->unk0->unk84];
    temp_s2 = &temp_s0->taskConfigs[arg0->unk4];

    setCleanupCallback(&cleanupAnimatedModelTask);
    memcpy(&arg0->unk8, &identityMatrix, sizeof(Transform3D));

    arg0->unk2C = loadUncompressedData(temp_s0->displayListStart, temp_s0->displayListEnd);
    arg0->unk30 =
        loadCompressedData(temp_s0->compressedDataStart, temp_s0->compressedDataEnd, temp_s0->decompressedSize);
    arg0->unk34 = 0;
    arg0->unk28 = temp_s2->unk4;
    arg0->unk44 = loadSpriteAssetData(temp_s2->unk16);
    arg0->unk4C = 0;
    arg0->unk4E = 0;
    arg0->unk50 = temp_s2->unk18;
    arg0->unk52 = temp_s2->unk1C;
    arg0->unk54 = 0;
    arg0->unk56 = 0;
    arg0->unk58 = 0;

    setCallback(&updateAnimatedModelTask);
}

void updateAnimatedModelTask(AnimatedModelTaskUpdateState *state) {
    ModelEntityConfig *config;
    ModelEntityTaskConfig *subEntry;

    config = &modelEntityConfigs[state->unk0->unk84];
    subEntry = &config->taskConfigs[state->unk4];

    if (state->unk0->unk86 != 0) {
        terminateCurrentTask();
    }

    state->unk4C = state->unk4C + state->unk50;
    state->unk4E = state->unk4E + state->unk52;
    state->unk4C = (u8)state->unk4C;
    state->unk4E = (u8)state->unk4E;
    state->unk8.transform.translation.x = subEntry->pos.x;
    state->unk8.transform.translation.y = subEntry->pos.y;
    state->unk8.transform.translation.z = subEntry->pos.z;

    if (state->unk0->unk87 != 0) {
        enqueueScrollingTextureRender(state->unk0->ptr->unk16, &state->unk8);
    }
}

void cleanupAnimatedModelTask(AnimatedModelTaskCleanupState *state) {
    state->unk44 = freeNodeMemory(state->unk44);
    state->unk30 = freeNodeMemory(state->unk30);
    state->unk2C = freeNodeMemory(state->unk2C);
}

void initSpriteSpawnerTask(SpriteEntityTaskState *state) {
    ModelEntityConfig *config = &modelEntityConfigs[state->owner->unk84];
    ModelEntityTaskConfig *subEntry = &config->taskConfigs[state->configIndex];
    setCleanupCallback(&cleanupSpriteSpawnerTask);
    loadSpriteAsset(&state->spriteState, subEntry->unk16);
    state->effect.spawnTimer = 0;
    setCallback(&updateSpriteSpawnerTask);
}

void updateSpriteSpawnerTask(SpriteEntityTaskState *state) {
    s16 temp;
    SpriteEntityTaskState *task;
    u8 tempUnk4;

    if (state->owner->unk86 != 0) {
        terminateCurrentTask();
    }

    temp = state->effect.spawnTimer;
    if (temp <= 0) {
        task = scheduleTask(initSpawnedSpriteTask, 0, 0, 0xC8);
        if (task != NULL) {
            task->owner = state->owner;
            tempUnk4 = state->configIndex;
            task->animationState = 0;
            task->configIndex = tempUnk4;
        } else {
            state->effect.spawnTimer = 0;
        }
    } else {
        state->effect.spawnTimer = temp - 1;
    }
}

void cleanupSpriteSpawnerTask(SpriteEntityTaskState *state) {
    releaseNodeMemoryRef((void **)&state->spriteState.assetData);
}

void initSpawnedSpriteTask(SpriteEntityTaskState *state) {
    volatile s32 sp10;
    volatile s32 sp14;
    volatile s32 sp18;
    volatile s32 sp1C;
    Transform3D sp20;
    ModelEntityConfig *config;
    ModelEntityTaskConfig *subEntry;

    config = &modelEntityConfigs[state->owner->unk84];
    subEntry = &config->taskConfigs[state->configIndex];

    memcpy(&sp20, &identityMatrix, sizeof(Transform3D));
    setCleanupCallback(&cleanupSpawnedSpriteTask);
    loadSpriteAsset(&state->spriteState, subEntry->unk16);

    sp10 = ((randA() & 0x1F) - 0x10) << 0x10;
    sp14 = ((randA() & 1) - 4) << 0x10;
    sp18 = 0;

    createYRotationMatrix(&sp20, 0x1D83);
    transformVector((s16 *)&sp10, (s16 *)&sp20, &state->effect.spawned.position);

    state->effect.spawned.position.x += subEntry->pos.x;
    state->effect.spawned.position.y += subEntry->pos.y;
    state->effect.spawned.position.z += subEntry->pos.z;
    state->effect.spawned.scale = spawnedSpriteScales[randA() & 3];

    setCallback(&updateSpawnedSpriteTask);
}

void updateSpawnedSpriteTask(SpriteEntityTaskState *state) {
    ModelEntityConfig *config;
    SubEntryVariant *subEntry;

    config = &modelEntityConfigs[state->owner->unk84];
    subEntry = (SubEntryVariant *)&config->taskConfigs[state->configIndex];

    if (state->owner->unk86 != 0) {
        terminateCurrentTask();
    }

    switch (state->animationState) {
        case 0:
            setSpriteAnimation(&state->spriteState, 0x10000, subEntry->unk1A, -1);
            state->animationState = 1;
            break;
        case 1:
            if (updateSpriteAnimation(&state->spriteState, 0x10000) != 0) {
                terminateCurrentTask();
                return;
            }
            break;
    }

    if (state->owner->unk87 != 0) {
        renderOpaqueSprite(
            &state->spriteState,
            state->owner->ptr->unk16,
            state->effect.spawned.position.x,
            state->effect.spawned.position.y,
            state->effect.spawned.position.z,
            state->effect.spawned.scale,
            state->effect.spawned.scale,
            0,
            0
        );
    }
}

void cleanupSpawnedSpriteTask(SpriteEntityTaskState *state) {
    releaseNodeMemoryRef((void **)&state->spriteState.assetData);
}
