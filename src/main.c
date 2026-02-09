#include "main.h"

#include "4050.h"
#include "5520.h"
#include "90F0.h"
#include "common.h"
#include "displaylist.h"
#include "geometry.h"
#include "rand.h"
#include "task_scheduler.h"

typedef struct {
    u8 padding[0x2C];
    void *uncompressedAsset;
    void *compressedAsset;
} RotatingModelTaskState;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
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
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
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
    s32 unk1C;
    s32 unk20;
    s32 unk24;
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
    s32 unk28;
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
    s32 unk28;
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
    u8 padding[0x8];
    void *spriteAsset;
} SpriteAnimationTaskState;

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
    s32 unk28;
    void *unk2C;
    void *unk30;
    s32 unk34;
} StaticModelTaskArg;

void cleanupSpriteAnimationTask(SpriteAnimationTaskState *state);
void cleanupSpriteSpawnerTask(func_80000C2C_182C_arg *arg0);
void updateSpriteSpawnerTask(func_80000C2C_182C_arg *);
void initSpawnedSpriteTask(func_80000C2C_182C_arg *arg0);
void updateSpriteAnimationTask(func_80000C2C_182C_arg *);
void updateSwingingModelTask(func_80000C2C_182C_arg *);
void cleanupSwingingModelTask(SwingingModelTaskState *);

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

void enqueueDisplayListIfVisible(func_80000C2C_182C_arg_unk0 *state, void *displayList);
void updateSpawnedSpriteTask(func_80000C2C_182C_arg *arg0);
void cleanupSpawnedSpriteTask(func_800014C8_20C8_arg *arg0);
void updateRotatingModelTask(func_80000C2C_182C_arg *arg0);
void cleanupRotatingModelTask(RotatingModelTaskState *state);
void updateStaticModelTask(func_80000C2C_182C_arg *arg0);
void cleanupStaticModelTask(SwingingModelTaskState *arg0);
void updateAnimatedModelTask(AnimatedModelTaskUpdateState *arg0);
void cleanupAnimatedModelTask(AnimatedModelTaskCleanupState *arg0);

extern s32 identityMatrix[];
extern u32 D_80088610_89210;

void initRotatingModelTask(func_80000C2C_182C_arg *arg0);
void initSwingingModelTask(func_80000C2C_182C_arg *arg0);
void initSpriteAnimationTask(func_80000C2C_182C_arg *arg0);
void initAnimatedModelTask(AnimatedModelTaskState *arg0);
void initSpriteSpawnerTask(func_80000C2C_182C_arg *arg0);
void initStaticModelTask(StaticModelTaskArg *arg0);

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
} D_80088F00_89B00_entry;

D_80088F00_89B00_entry D_80088F00_89B00[] = {
    { 0x00000000, 0x01000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00010000 }
};

ModelEntityTaskConfig D_80088F20_89B20[] = {
    { .unk0 = initRotatingModelTask,
     .unk4 = 0x80088410,
     .unk8 = 0xFF8A0000,
     .unkC = 0x00390000,
     .unk10 = 0xFF8A0000,
     .unk14 = -1,
     .unk16 = -64 },
    { .unk0 = initSwingingModelTask,
     .unk4 = 0x80088400,
     .unk8 = 0xFF2E0000,
     .unkC = 0x00410000,
     .unk10 = 0xFFC80000,
     .unk14 = 0,
     .unk16 = 0x06AA,
     .unk18 = 0x00000020,
     .unk1C = 0x000001C7 },
    { .unk0 = initSwingingModelTask,
     .unk4 = 0x80088420,
     .unk8 = 0x002E0000,
     .unkC = 0x00330000,
     .unk10 = 0xFF590000,
     .unk14 = 0,
     .unk16 = 0x1EAB,
     .unk18 = 0x00000010,
     .unk1C = 0x000000E3 },
    { .unk0 = initSwingingModelTask,
     .unk4 = 0x80088430,
     .unk8 = 0x00AD0000,
     .unkC = 0x00390000,
     .unk10 = 0xFFAF0000,
     .unk14 = 0,
     .unk16 = 0x1956,
     .unk18 = 0x00000010,
     .unk1C = 0x000000E3 },
    { .unk0 = initSwingingModelTask,
     .unk4 = 0x80088440,
     .unk8 = 0x00BE0000,
     .unkC = 0x00390000,
     .unk10 = 0xFFF00000,
     .unk14 = 0,
     .unk16 = 0x1956,
     .unk18 = -16,
     .unk1C = 0x000000E3 },
    { .unk0 = initRotatingModelTask,
     .unk4 = 0x80088480,
     .unk8 = 0xFF8A0000,
     .unkC = 0x00390000,
     .unk10 = 0xFF8A0000,
     .unk14 = -1,
     .unk16 = -64 },
    { .unk0 = initSwingingModelTask,
     .unk4 = 0x80088470,
     .unk8 = 0xFF2E0000,
     .unkC = 0x00410000,
     .unk10 = 0xFFC80000,
     .unk14 = 0,
     .unk16 = 0x06AA,
     .unk18 = 0x00000020,
     .unk1C = 0x000001C7 },
    { .unk0 = initSwingingModelTask,
     .unk4 = 0x80088490,
     .unk8 = 0x002E0000,
     .unkC = 0x00330000,
     .unk10 = 0xFF590000,
     .unk14 = 0,
     .unk16 = 0x1EAB,
     .unk18 = 0x00000010,
     .unk1C = 0x000000E3 },
    { .unk0 = initSwingingModelTask,
     .unk4 = 0x800884A0,
     .unk8 = 0x00AD0000,
     .unkC = 0x00390000,
     .unk10 = 0xFFAF0000,
     .unk14 = 0,
     .unk16 = 0x1956,
     .unk18 = 0x00000010,
     .unk1C = 0x000000E3 },
    { .unk0 = initSwingingModelTask,
     .unk4 = 0x800884B0,
     .unk8 = 0x00BE0000,
     .unkC = 0x00390000,
     .unk10 = 0xFFF00000,
     .unk14 = 0,
     .unk16 = 0x1956,
     .unk18 = -16,
     .unk1C = 0x000000E3 }
};

ModelEntityTaskConfig D_80089088_89C88[] = {
    { .unk0 = initSpriteAnimationTask,
     .unk4 = 0x00000000,
     .unk8 = 0x00380000,
     .unkC = 0x004C0000,
     .unk10 = 0xFF570000,
     .unk14 = 0,
     .unk16 = 1,
     .unk18 = 0x00000000,
     .unk1C = 0x00029999,
     .unk20 = 0x00000000 }
};

ModelEntityTaskConfig D_800890AC_89CAC[] = {
    { .unk0 = initAnimatedModelTask, .unk4 = 0x80088500, .unk14 = 0, .unk16 = 4, .unk1C = 0x00000004 },
    { .unk0 = initAnimatedModelTask, .unk4 = 0x800884F0, .unk14 = 0, .unk16 = 4, .unk1C = 0x00000008 },
    { .unk0 = initSpriteSpawnerTask,
     .unk8 = 0x00570000,
     .unkC = 0x00020000,
     .unk10 = 0xFF470000,
     .unk16 = 4,
     .unk1C = 0x00010000 },
    { .unk0 = initStaticModelTask, .unk4 = 0x80088510 }
};

ModelEntityTaskConfig D_8008913C_89D3C[] = {
    { .unk0 = initAnimatedModelTask, .unk4 = 0x80088550, .unk16 = 4, .unk1C = 0x00000004 },
    { .unk0 = initAnimatedModelTask, .unk4 = 0x80088540, .unk16 = 4, .unk1C = 0x00000008 },
    { .unk0 = initSpriteSpawnerTask,
     .unk8 = 0x00570000,
     .unkC = 0x00020000,
     .unk10 = 0xFF470000,
     .unk16 = 4,
     .unk1C = 0x00010000 }
};

ModelEntityConfig modelEntityConfigs[14] = {
    /* [0] TOWN */
    { .name = "TOWN   ",
     .displayListStart = (void *)0x001EFB50,
     .displayListEnd = (void *)0x001F1DB0,
     .compressedDataStart = (void *)0x004A8650,
     .compressedDataEnd = (void *)0x004B04A0,
     .decompressedSize = 0x0000E180,
     .unk1C = 0x800883F0,
     .unk20 = 0x00010000,
     .unk24 = 0x80088450,
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
    /* [1] TOWN */
    { .name = "TOWN   ",
     .displayListStart = (void *)0x001EFB50,
     .displayListEnd = (void *)0x001F1DB0,
     .compressedDataStart = (void *)0x004A8650,
     .compressedDataEnd = (void *)0x004B04A0,
     .decompressedSize = 0x0000E180,
     .unk1C = 0x800883F0,
     .unk20 = 0x00010000,
     .unk24 = 0x80088450,
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
    /* [2] YAMA */
    { .name = "YAMA   ",
     .displayListStart = (void *)0x001F6D00,
     .displayListEnd = (void *)0x001F7DB0,
     .compressedDataStart = (void *)0x004BCFB0,
     .compressedDataEnd = (void *)0x004BF950,
     .decompressedSize = 0x00003FE0,
     .unk1C = 0x80088580,
     .unk20 = 0x00010000,
     .unk24 = 0x80088590,
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
    /* [3] YAMA_Y */
    { .name = "YAMA_Y ",
     .displayListStart = (void *)0x001F7DB0,
     .displayListEnd = (void *)0x001F8D10,
     .compressedDataStart = (void *)0x004BF950,
     .compressedDataEnd = (void *)0x004C20A0,
     .decompressedSize = 0x00003B30,
     .unk1C = 0x800885A0,
     .unk20 = 0x00010000,
     .unk24 = 0x800885B0,
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
    /* [4] LINDA P */
    { .name = "LINDA P",
     .displayListStart = (void *)0x001E9790,
     .displayListEnd = (void *)0x001E9C40,
     .compressedDataStart = (void *)0x0049FA20,
     .compressedDataEnd = (void *)0x004A0670,
     .decompressedSize = 0x000014A0,
     .unk1C = 0x80088180,
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
    /* [5] LINDA B */
    { .name = "LINDA B",
     .displayListStart = (void *)0x001E8E60,
     .displayListEnd = (void *)0x001E9300,
     .compressedDataStart = (void *)0x0049E2E0,
     .compressedDataEnd = (void *)0x0049F0D0,
     .decompressedSize = 0x00001680,
     .unk1C = 0x80088150,
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
    /* [6] WENDY H */
    { .name = "WENDY H",
     .displayListStart = (void *)0x001F94F0,
     .displayListEnd = (void *)0x001FA080,
     .compressedDataStart = (void *)0x004C3A60,
     .compressedDataEnd = (void *)0x004C5840,
     .decompressedSize = 0x00004120,
     .unk1C = 0x80088600,
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
    /* [7] BEACH */
    { .name = "BEACH  ",
     .displayListStart = (void *)0x001EFB50,
     .displayListEnd = (void *)0x001F1DB0,
     .compressedDataStart = (void *)0x004A8650,
     .compressedDataEnd = (void *)0x004B04A0,
     .decompressedSize = 0x0000E180,
     .unk1C = 0x800883F0,
     .unk20 = 0x00010000,
     .unk24 = 0x80088450,
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
    /* [8] JUNGLE */
    { .name = "JUNGLE ",
     .displayListStart = (void *)0x001F4730,
     .displayListEnd = (void *)0x001F5270,
     .compressedDataStart = (void *)0x004B73C0,
     .compressedDataEnd = (void *)0x004B8D10,
     .decompressedSize = 0x000021D0,
     .unk1C = 0x800884D0,
     .unk20 = 0x00010000,
     .unk24 = 0x800884E0,
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
    /* [9] SPACE */
    { .name = "SPACE  ",
     .displayListStart = (void *)0x001F5FD0,
     .displayListEnd = (void *)0x001F6D00,
     .compressedDataStart = (void *)0x004BAF60,
     .compressedDataEnd = (void *)0x004BCFB0,
     .decompressedSize = 0x000036D0,
     .unk1C = 0x80088560,
     .unk20 = 0x00010000,
     .unk24 = 0x80088570,
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
    /* [10] ISLAND */
    { .name = "ISLAND ",
     .displayListStart = (void *)0x001FA080,
     .displayListEnd = (void *)0x001FB040,
     .compressedDataStart = (void *)0x004C5840,
     .compressedDataEnd = (void *)0x004C9310,
     .decompressedSize = 0x00005AD0,
     .unk1C = (s32)&D_80088610_89210,
     .unk20 = 0x00010000,
     .unk24 = 0x80088620,
     .unk28 = 0x00010000,
     .taskConfigs = NULL,
     .taskCount = 0,
     .unk32 = 0xFFFF,
     .diffuseColorR = 0xFF,
     .diffuseColorG = 0xFF,
     .diffuseColorB = 0xFF,
     .unk37 = 0xFF,
     .ambientColorR = 0x60,
     .ambientColorG = 0x60,
     .ambientColorB = 0x60,
     .unk3B = 0xFF },
    /* [11] HAUNTED */
    { .name = "HAUNTED",
     .displayListStart = (void *)0x001EA480,
     .displayListEnd = (void *)0x001EACD0,
     .compressedDataStart = (void *)0x004A1160,
     .compressedDataEnd = (void *)0x004A2980,
     .decompressedSize = 0x000024B0,
     .unk1C = 0x800881B0,
     .unk20 = 0x00010000,
     .unk24 = 0x800881C0,
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
    /* [12] JUNGLE2 */
    { .name = "JUNGLE2",
     .displayListStart = (void *)0x001F5270,
     .displayListEnd = (void *)0x001F5FD0,
     .compressedDataStart = (void *)0x004B8D10,
     .compressedDataEnd = (void *)0x004BAF60,
     .decompressedSize = 0x00002EC0,
     .unk1C = 0x80088520,
     .unk20 = 0x00010000,
     .unk24 = 0x80088530,
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
    /* [13] OPENING */
    { .name = "OPENING",
     .displayListStart = NULL,
     .displayListEnd = NULL,
     .compressedDataStart = NULL,
     .compressedDataEnd = NULL,
     .decompressedSize = 0,
     .unk1C = 0,
     .unk20 = 0,
     .unk24 = 0,
     .unk28 = 0,
     .taskConfigs = NULL,
     .taskCount = 0,
     .unk32 = 0,
     .diffuseColorR = 0xFF,
     .diffuseColorG = 0xFF,
     .diffuseColorB = 0xFF,
     .unk37 = 0xFF,
     .ambientColorR = 0xD0,
     .ambientColorG = 0xD0,
     .ambientColorB = 0xD0,
     .unk3B = 0xFF }
};

s32 spawnedSpriteScales[8] = { 0x00010000, 0x00011999, 0x0000E666, 0x00013333,
                               0x00000000, 0x01000148, 0x00000000, 0x00000000 };

void setModelRenderMode(setModelRenderMode_arg *arg0, s8 arg1) {
    arg0->unk87 = arg1;
}

s32 osVoiceCheckWord(u8 *data) {
    return 0xE;
}

void setupModelEntityLighting(ModelEntity *entity, ColorData *lightColors, ColorData *ambientColor) {
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
    lightColors[0].r2 = 0;
    lightColors[0].g2 = 0x7F;
    lightColors[0].b2 = 0x7F;
    lightColors[0].r = config->diffuseColorR;
    lightColors[0].g = config->diffuseColorG;
    quarterAverage = average >> 2;
    lightColors[0].b = config->diffuseColorB;
    lightColors[1].r = quarterAverage;
    lightColors[1].g = quarterAverage;
    lightColors[1].b = quarterAverage;
    lightColors[1].r2 = 0x7F;
    lightColors[1].g2 = 0x7F;
    lightColors[1].b2 = 0;
    lightColors[2].r2 = -0x7F;
    lightColors[2].g2 = 0x7F;
    lightColors[2].b2 = 0;
    lightColors[2].r = halfAverage;
    lightColors[2].g = halfAverage;
    lightColors[2].b = halfAverage;
    ambientColor[0].r = config->ambientColorR;
    ambientColor[0].g = config->ambientColorG;
    ambientColor[0].b = config->ambientColorB;
    setViewportLightColors(entity->parent->unkDA, 3, lightColors, ambientColor);
}

typedef struct {
    void *unk0;
    u8 unk4;
    u8 unk5;
} initModelEntity_task;

s32 initModelEntity(ModelEntity *entity, s16 index, void *arg2) {
    ModelEntityConfig *entry;
    s32 i;

    entity->configIndex = index;
    entity->isVisible = 1;
    entry = &modelEntityConfigs[index];
    entity->parent = arg2;
    entity->isDisposed = 0;

    if (entry->unk32 == 0xFFFF) {
        entity->modelData = loadUncompressedData(entry->displayListStart, entry->displayListEnd);
        entity->textureData =
            loadCompressedData(entry->compressedDataStart, entry->compressedDataEnd, entry->decompressedSize);

        memcpy(&entity->primaryMatrix, &identityMatrix, 0x20);

        entity->animState = 0;
        entity->activeModel = entity->modelData;
        entity->activeTexture = entity->textureData;
        entity->displayConfig = entry->unk1C;

        if (entry->unk24 != 0) {
            memcpy(&entity->secondaryMatrix, identityMatrix, 0x20);
            entity->secondaryModel = entity->modelData;
            entity->secondaryTexture = entity->textureData;
            entity->secondaryConfig = entry->unk24;
        } else {
            entity->secondaryConfig = 0;
            entity->secondaryTexture = 0;
            entity->secondaryModel = 0;
        }

        entity->unk74 = 0;

        for (i = 0; i < entry->taskCount; i++) {
            initModelEntity_task *task = scheduleTask((entry->taskConfigs + i)->unk0, 0, 0, 0xC8);
            if (task != NULL) {
                task->unk0 = entity;
                task->unk4 = i;
                task->unk5 = 0;
            }
        }

    } else {
        entity->modelData = 0;
        entity->textureData = 0;
        entity->activeModel = 0;
        entity->activeTexture = 0;
        entity->animState = 0;
        entity->displayConfig = 0;
        entity->secondaryModel = 0;
        entity->secondaryTexture = 0;
        entity->secondaryConfig = 0;
        entity->unk74 = 0;

        return 1;
    }

    return 0;
}

void freeEffectResources(EffectState *state) {
    if (state->unk4 != 0) {
        state->unk8 = freeNodeMemory(state->unk8);
        state->unk4 = freeNodeMemory(state->unk4);
    }
    state->isDisposed = 1;
}

void renderModelEntity(ModelEntityRenderState *state) {
    if (state->isVisible != 0) {
        if (state->primaryDisplayList.segment1 != 0) {
            enqueueDisplayListObject(state->parent->slotIndex, &state->primaryDisplayList);
        }
        if (state->hasSecondaryDisplayList != 0) {
            enqueueDisplayListObjectWithFullRenderState(state->parent->slotIndex, &state->secondaryDisplayList);
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
    arg0->unk8.translation.x = taskConfig->unk8;
    arg0->unk8.translation.y = taskConfig->unkC;
    arg0->unk8.translation.z = taskConfig->unk10;
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

    memcpy(&arg0->unk8, &identityMatrix, 0x20);

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
    func_8006B084_6BC84(&zRotMatrix, &yRotMatrix, &arg0->unk8);

    arg0->unk8.translation.x = subEntry->unk8;
    arg0->unk8.translation.y = subEntry->unkC;
    arg0->unk8.translation.z = subEntry->unk10;

    enqueueDisplayListIfVisible(arg0->unk0, &arg0->unk8);
}

void cleanupSwingingModelTask(SwingingModelTaskState *state) {
    state->compressedAsset = freeNodeMemory(state->compressedAsset);
    state->uncompressedAsset = freeNodeMemory(state->uncompressedAsset);
}

void initSpriteAnimationTask(func_80000C2C_182C_arg *arg0) {
    ModelEntityConfig *config = &modelEntityConfigs[arg0->unk0->unk84];
    ModelEntityTaskConfig *subEntry = &config->taskConfigs[arg0->unk4];
    setCleanupCallback(&cleanupSpriteAnimationTask);
    loadSpriteAsset((SpriteAssetState *)&arg0->unk8, subEntry->unk16);
    setCallback(&updateSpriteAnimationTask);
}

void updateSpriteAnimationTask(func_80000C2C_182C_arg *arg0) {
    ModelEntityConfig *config = &modelEntityConfigs[arg0->unk0->unk84];
    SubEntryVariant *subEntry = (SubEntryVariant *)&config->taskConfigs[arg0->unk4];

    if (arg0->unk0->unk86 != 0) {
        terminateCurrentTask();
    }

    switch (arg0->unk5) {
        case 0:
            setSpriteAnimation(&arg0->unk8, 0x10000, subEntry->unk1A, -1);
            arg0->unk5 = 1;
            break;
        case 1:
            updateSpriteAnimation(&arg0->unk8, 0x10000);
            break;
    }

    if (arg0->unk0->unk87 != 0) {
        renderOpaqueSprite(
            &arg0->unk8,
            arg0->unk0->ptr->unk16,
            subEntry->unk8,
            subEntry->unkC,
            subEntry->unk10,
            subEntry->unk1C,
            subEntry->unk1C,
            0,
            0
        );
    }
}

void cleanupSpriteAnimationTask(SpriteAnimationTaskState *state) {
    releaseNodeMemoryRef((&state->spriteAsset));
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

    memcpy(&arg0->unk8, &identityMatrix, 0x20);

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

    arg0->unk8.translation.x = subEntry->unk8;
    arg0->unk8.translation.y = subEntry->unkC;
    arg0->unk8.translation.z = subEntry->unk10;
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
    memcpy(&arg0->unk8, &identityMatrix, 0x20);

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
    state->unk8.transform.translation.x = subEntry->unk8;
    state->unk8.transform.translation.y = subEntry->unkC;
    state->unk8.transform.translation.z = subEntry->unk10;

    if (state->unk0->unk87 != 0) {
        enqueueScrollingTextureRender(state->unk0->ptr->unk16, &state->unk8);
    }
}

void cleanupAnimatedModelTask(AnimatedModelTaskCleanupState *state) {
    state->unk44 = freeNodeMemory(state->unk44);
    state->unk30 = freeNodeMemory(state->unk30);
    state->unk2C = freeNodeMemory(state->unk2C);
}

void initSpriteSpawnerTask(func_80000C2C_182C_arg *arg0) {
    ModelEntityConfig *config = &modelEntityConfigs[arg0->unk0->unk84];
    ModelEntityTaskConfig *subEntry = &config->taskConfigs[arg0->unk4];
    setCleanupCallback(&cleanupSpriteSpawnerTask);
    loadSpriteAsset((SpriteAssetState *)&arg0->unk8, subEntry->unk16);
    *(s16 *)&arg0->unk54 = 0;
    setCallback(&updateSpriteSpawnerTask);
}

void updateSpriteSpawnerTask(func_80000C2C_182C_arg *arg0) {
    s16 temp;
    func_80000C2C_182C_arg *task;
    u8 tempUnk4;

    if (arg0->unk0->unk86 != 0) {
        terminateCurrentTask();
    }

    temp = *(s16 *)&arg0->unk54;
    if (temp <= 0) {
        task = scheduleTask(initSpawnedSpriteTask, 0, 0, 0xC8);
        if (task != NULL) {
            task->unk0 = arg0->unk0;
            tempUnk4 = arg0->unk4;
            task->unk5 = 0;
            task->unk4 = tempUnk4;
        } else {
            *(s16 *)&arg0->unk54 = 0;
        }
    } else {
        *(s16 *)&arg0->unk54 = temp - 1;
    }
}

void cleanupSpriteSpawnerTask(func_80000C2C_182C_arg *arg0) {
    releaseNodeMemoryRef((void **)&arg0->unk8);
}

void initSpawnedSpriteTask(func_80000C2C_182C_arg *arg0) {
    volatile s32 sp10;
    volatile s32 sp14;
    volatile s32 sp18;
    volatile s32 sp1C;
    Transform3D sp20;
    ModelEntityConfig *config;
    ModelEntityTaskConfig *subEntry;

    config = &modelEntityConfigs[arg0->unk0->unk84];
    subEntry = &config->taskConfigs[arg0->unk4];

    memcpy(&sp20, &identityMatrix, sizeof(Transform3D));
    setCleanupCallback(&cleanupSpawnedSpriteTask);
    loadSpriteAsset((SpriteAssetState *)&arg0->unk8, subEntry->unk16);

    sp10 = ((randA() & 0x1F) - 0x10) << 0x10;
    sp14 = ((randA() & 1) - 4) << 0x10;
    sp18 = 0;

    createYRotationMatrix(&sp20, 0x1D83);
    transformVector((s16 *)&sp10, (s16 *)&sp20, &arg0->unk54);

    arg0->unk54 = arg0->unk54 + subEntry->unk8;
    arg0->unk58 = arg0->unk58 + subEntry->unkC;
    arg0->unk5C = arg0->unk5C + subEntry->unk10;
    arg0->unk60 = spawnedSpriteScales[randA() & 3];

    setCallback(&updateSpawnedSpriteTask);
}

void updateSpawnedSpriteTask(func_80000C2C_182C_arg *arg0) {
    ModelEntityConfig *config;
    SubEntryVariant *subEntry;

    config = &modelEntityConfigs[arg0->unk0->unk84];
    subEntry = (SubEntryVariant *)&config->taskConfigs[arg0->unk4];

    if (arg0->unk0->unk86 != 0) {
        terminateCurrentTask();
    }

    switch (arg0->unk5) {
        case 0:
            setSpriteAnimation(&arg0->unk8, 0x10000, subEntry->unk1A, -1);
            arg0->unk5 = 1;
            break;
        case 1:
            if (updateSpriteAnimation(&arg0->unk8, 0x10000) != 0) {
                terminateCurrentTask();
                return;
            }
            break;
    }

    if (arg0->unk0->unk87 != 0) {
        renderOpaqueSprite(
            &arg0->unk8,
            arg0->unk0->ptr->unk16,
            arg0->unk54,
            arg0->unk58,
            arg0->unk5C,
            arg0->unk60,
            arg0->unk60,
            0,
            0
        );
    }
}

void cleanupSpawnedSpriteTask(func_800014C8_20C8_arg *arg0) {
    releaseNodeMemoryRef((void **)&arg0->unk8);
}
