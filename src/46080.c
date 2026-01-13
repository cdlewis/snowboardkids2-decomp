#include "19E80.h"
#include "413E0.h"
#include "51060.h"
#include "56910.h"
#include "594E0.h"
#include "5AA90.h"
#include "5E590.h"
#include "6E840.h"
#include "common.h"
#include "course.h"
#include "displaylist.h"
#include "gamestate.h"
#include "gbi.h"
#include "geometry.h"
#include "graphics.h"
#include "levels/haunted_house.h"
#include "levels/ice_land.h"
#include "levels/jingle_town.h"
#include "levels/lindas_castle.h"
#include "levels/snowboard_street_shoot_cross.h"
#include "levels/starlight_highway.h"
#include "levels/sunny_mountain.h"
#include "levels/wendys_house.h"
#include "race_session.h"
#include "rand.h"
#include "rom_loader.h"
#include "task_scheduler.h"

USE_ASSET(_3F6670);
USE_ASSET(_3F6950);
USE_ASSET(_3F3EF0);

typedef struct {
    void *assetTable;
    loadAssetMetadata_arg metadata;
    u8 _pad20[0x4];
    s32 velX;
    s32 velY;
    s32 velZ;
    Player *player;
    u16 sectorIndex;
    u8 _pad36[0x2];
    s16 playerIndex;
    s16 initFlag;
} PanelProjectileInitArg;

typedef struct {
    u8 _pad0[0x4];
    u8 metadataPad[0x4];
    Vec3i position;
    u8 _pad14[0x10];
    s32 velX;
    s32 velY;
    s32 velZ;
    u8 _pad30[0x4];
    u16 sectorIndex;
    s16 timer;
    s16 playerIndex;
    s16 initFlag;
} PanelProjectileUpdateArg;

typedef struct {
    u8 _pad0[0x4];
    func_80066444_67044_arg1 metadata;
} PanelProjectileImpactArg;

typedef struct {
    u8 padding[0x5B];
    u8 availableHomingProjectileSlots;
    u8 padding2[0x1A];
    u8 unk76;
} allocation_46080;

typedef struct {
    void *assetData;
    void *metadataPtr;
    s32 posX;
    s32 posY;
    s32 posZ;
    u8 _pad14[0x10];
    s32 velX;
    s32 velY;
    s32 velZ;
    u16 sectorIndex;
    u8 _pad32[0x2];
    s16 impactTimer;
    s16 targetPlayerIndex;
    s16 targetVariant;
} ItemHomingProjectileMoveArg;

typedef struct {
    void *assetData;
    void *metadataPtr;
    u8 _pad[0x28];
    s16 sectorIndex;
    u8 _pad2[0x2];
    s16 impactTimer;
    s16 targetPlayerIndex;
    s16 targetVariant;
} ItemHomingProjectileImpactArg;

typedef struct {
    void *projectileAsset;
} BossHomingProjectileTask;

typedef struct {
    void *projectileAsset;
} BossHomingProjectileCleanupArg;

typedef struct {
    u8 _pad0[0x4];
    u8 metadata[0x4];
    Vec3i position;
    u8 _pad14[0x14];
    s32 velocityX;
    s32 velocityY;
    s32 velocityZ;
    u16 sectorIndex;
    s16 bounceTimer;
    s16 playerIndex;
} BossHomingProjectile;

typedef struct {
    u8 _pad0[0x4];
    func_80066444_67044_arg1 displayListState;
} BossHomingProjectileBounceArg;

typedef struct {
    u8 pad[0x24];
    void *player;
} BossHomingProjectileSpawnTask;

typedef struct {
    void *unk0;
} func_8004BC20_4C820_arg;

typedef struct {
    u8 pad[0x24];
    Player *unk24;
} func_8004BC5C_4C85C_Task;

typedef struct {
    u8 _pad0[0x4];
    u8 unk4[0x4];
    Vec3i unk8;
    u8 _pad14[0x14];
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    u16 unk34;
    s16 unk36;
    s16 unk38;
} func_8004BE40_4CA40_arg;

typedef struct {
    u8 _pad0[0x4];
    func_80066444_67044_arg1 unk4;
    s16 unk38;
} func_8004BFBC_4CBBC_arg;

typedef struct {
    void *assetData;
    loadAssetMetadata_arg metadata;
    u8 _pad[0x4];
    u16 animationCounter;
    u8 _pad2[0x2];
    s32 velocity;
} PlayerSparkleTask;

typedef struct {
    u8 padding0[0x8];
    Vec3i pos;         /* 0x8 - Position (x, y, z) - 12 bytes */
    u8 padding2[0x10]; /* 0x14 - Padding (16 bytes) to reach 0x24 */
    s16 playerIndex;   /* 0x24 - Player index (unkB94) - 2 bytes */
    s16 padding26;     /* 0x26 - Padding (2 bytes) */
    s32 timer;         /* 0x28 - Impact timer (4 bytes) */
    Vec3i vel;         /* 0x2C - Velocity (x, y, z) - 12 bytes */
} HomingProjectileTask;

typedef struct {
    u8 _pad[0x3C];
    s32 playerIndex;
} PlayerRenderTask;

typedef struct {
    void *assetData;
    void *metadataPtr;
    u8 _pad[0x1C];
    s32 velOffsetX;
    s32 velOffsetY;
    s32 velOffsetZ;
    u8 _pad2[0x2];
    s16 animFrame;
    u8 _pad3[0x2];
    s16 targetPlayerIndex;
    s16 targetVariant;
} ItemHomingProjectileInitArg;

typedef struct {
    u8 _pad0[0x4C];
    s32 unk4C;
    s32 unk50;
    s32 unk54;
    u8 _pad58[0x3F4];
    s32 velocity;
    s32 unk450;
    s32 unk454;
    u8 _pad458[0x63C];
    u16 unkA94;
    u8 _padA96[0xFE];
    u16 unkB94;
    u8 _padB96[0x22];
    u8 unkBB8;
} BossEntity;

typedef struct {
    void *projectileAsset;
    void *unk4;
    Vec3i position;
    u8 _pad14[0x10];
    BossEntity *boss;
    Vec3i velocity;
    s16 sectorIndex;
    s16 bounceTimer;
    s16 playerIndex;
} BossHomingProjectileSpawnArg;

typedef struct {
    s16 unk0;
    u8 _pad2[0x4];
    u16 unk6;
    u8 unk8[0xC];
} SceneAnimationEntryNew;

typedef struct {
    void *transformBuffer;
    void *headerData;
    SceneAnimationEntryNew *entries;
    void *assetData;
    void *loadedData;
    s16 assetIndex;
    s16 dataIndex;
    s16 entryCount;
    s16 unk1A;
    s32 frameCounter;
} SceneAnimationTaskNew;

typedef struct {
    u8 _pad0[0x4];
    func_80066444_67044_arg1 unk4;
    s16 unk38;
} func_8004BB0C_4C70C_arg;

typedef struct {
    s32 unk0;
    func_80066444_67044_arg1 unk4;
    s16 unk38;
} func_8004B990_4C590_arg;

typedef struct {
    void *unk0;
} func_8004C0D0_4CCD0_arg;

typedef struct {
    void *assetData; /* 0x0 - Asset data pointer */
    void *metadata;  /* 0x4 - Asset metadata pointer */
    s32 posX;        /* 0x8 - X position */
    s32 posY;        /* 0xC - Y position */
    s32 posZ;        /* 0x10 - Z position */
    u8 _pad14[0x10];
    u16 sectorIndex; /* 0x24 - Course sector index */
    u16 animFrame;   /* 0x26 - Animation frame */
    s16 timer;       /* 0x28 - Impact timer */
    u8 _pad2A[0x2];
    s32 velX; /* 0x2C - X velocity */
    s32 velY; /* 0x30 - Y velocity */
    s32 velZ; /* 0x34 - Z velocity */
} HomingProjectileUpdateArg;

typedef struct {
    DataTable_19E80 *assetTable;
    OutputStruct_19E80 textureEntry;
    Transform3D transform;
    Mtx *transformMatrix;
    GameStateUnk44Unk2C0 *renderEntry;
    s32 frameIndex;
    s32 isSecondaryBox;
} ItemBoxBurstEffectState;

typedef struct {
    void *unk0;
} func_8004B264_4BE64_arg;

typedef struct {
    u8 _pad[0x24];
    void *unk24;
    void *unk28;
} SkyRenderTaskCleanupArg;

typedef struct {
    u8 _pad[0xB4];
    s16 skyType;
} ScheduledTask;

typedef struct {
    u8 _pad0[0x20];
    s32 unk20;
    void *unk24;
    void *unk28;
    s32 unk2C;
    u8 _pad30[0xC];
    u8 unk3C[0x20];
    s32 unk5C;
    void *unk60;
    void *unk64;
    s32 unk68;
    u8 _pad6C[0xC];
    u8 unk78[0x20];
    void *unk98;
    void *unk9C;
    void *unkA0;
    s32 unkA4;
    u8 _padA8[0xC];
    s16 skyType;
} SkyRenderTaskState;

typedef struct {
    u8 _pad[0x30];
    u8 unk30[0x2C];
    u8 unk5C;
    u8 _pad5D[0x19];
    u8 unk76;
} GameState_46080;

typedef struct {
    u8 _pad[0x14];
    Vec3i position;
    void *unk20;
    void *unk24;
    void *unk28;
    void *unk2C;
    u8 _pad3[0x10];
    s32 unk40;
} PlayerRenderTaskState;

typedef struct {
    u8 _pad[0x24];
    void *unk24;
    void *unk28;
} PlayerRenderTaskCleanupArg;

typedef struct {
    u8 _pad[0x20];
    DisplayLists *displayLists;
    u8 _pad2[0x3C - 0x24];
    s32 playerIndex;
    s32 pendingSyncFrames;
} PlayerDisplayListState;

typedef struct {
    s16 unk0;
    u8 _pad2[0x6];
    u8 unk8[0xC];
} SceneAnimationEntry;

typedef struct {
    void *transformBuffer;
    void *headerData;
    SceneAnimationEntry *volatile entries;
    void *assetData;
    void *loadedData;
    s16 assetIndex;
    s16 dataIndex;
    s16 entryCount;
} SceneAnimationTask;

typedef union {
    s16 halfword;
    struct {
        u8 high;
        u8 low;
    } bytes;
} HalfwordBytes;

typedef struct {
    void *unk0;
    void *opaqueDisplayList;
    void *transparentDisplayList;
    void *overlayDisplayList;
} SceneryDisplayLists;

typedef struct {
    u8 _pad[0x20];
    SceneryDisplayLists *displayLists;
    u8 _pad2[0xC];
    s32 unk30;
    u8 _pad3[0x10];
    HalfwordBytes scrollX;
    HalfwordBytes scrollY;
    s16 unk48;
    s16 scrollSpeedX;
    s16 scrollSpeedY;
} ScrollingSceneryTaskState;

typedef struct {
    u8 _pad[0x24];
    void *unk24;
    void *unk28;
    void *unk2C;
    u8 _pad2[0xC];
    void *unk3C;
    u8 _pad3[0x4];
    HalfwordBytes scrollX;
    HalfwordBytes scrollY;
    s16 assetPoolIndex;
} ScrollingSceneryCleanupState;

typedef struct {
    u8 _pad[0x76];
    u8 unk76;
    u8 _pad2[2];
    u8 unk79;
} Allocation_47D1C;

typedef struct {
    DisplayListObject displayListObject;
    s32 frameCounter;
    s32 rotationAngle;
} FlyingSceneryState;

typedef struct {
    u8 unk0[0x14];
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    void *unk20;
    void *unk24;
    void *unk28;
    void *unk2C;
    u8 _pad2[0xC];
    u8 unk3C[0x14];
    s32 unk50;
    s32 unk54;
    s32 unk58;
    void *unk5C;
    u8 _pad3[0x18];
    u8 unk78[0x18];
    s32 unk90;
    u8 _pad4[4];
    void *unk98;
} CourseSceneryTaskState;

typedef struct {
    u8 _pad[0x3C];
    u8 unk3C[0x3C];
    u8 unk78[0x3C];
    s16 rotationAngle;
} CourseSceneryUpdateState;

typedef struct {
    u8 _pad[0x20];
    void *displayLists;
    u8 _pad2[0x24];
    s16 assetPoolIndex;
    s16 scrollSpeedX;
    s16 scrollSpeedY;
    s16 unk4E;
    s16 unk50;
} ScrollingSceneryTask;

typedef struct {
    u8 _pad[0x24];
    void *unk24;
    void *unk28;
} CourseSceneryCleanupArg;

typedef struct {
    s8 visible;
    u8 _pad1[0x3];
    u8 positionData[0xC];
} GoldCoinEntry;

typedef struct {
    void *matrixBuffer;
    void *displayData;
    GoldCoinEntry *entries;
    u8 _padC[0x4];
    s32 *compressedData;
    u8 _pad14[0x2];
    s16 coinCount;
    s16 animationFrame;
} GoldCoinSetupState;

typedef struct {
    void *unk0;
    u8 _pad[0x8];
    void *unkC;
    void *unk10;
    s16 courseIndex;
} GoldCoinsTaskState;

typedef struct {
    u8 _pad0[0x76];
    u8 unk76;
} AllocationData;

typedef struct {
    void *assetData;
} PlayerAuraCleanupState;

typedef struct {
    void *assetData;
} PlayerAuraTask;

typedef struct {
    u8 _pad0[0x44];
    s32 unk44;
} AllocationType_46080;

typedef struct {
    loadAssetMetadata_arg asset;
    u8 _pad1C[0x4];
} AssetMetadata_46080;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 posX;
    s32 posY;
    s32 posZ;
    u8 _pad14[0xC];
} PlayerAuraElement;

typedef struct {
    void *assetData;
    AssetMetadata_46080 element0;
    AssetMetadata_46080 element1;
    AssetMetadata_46080 element2;
    Player *player;
    s16 holdTimer;
    s16 animationAngle;
} PlayerAuraState;

typedef struct {
    s8 visible;
    s8 processed;
    s16 respawnTimer;
    Vec3i position;
} GoldCoinRenderEntry;

typedef struct {
    void *matrixBuffer;
    void *displayData;
    GoldCoinRenderEntry *entries;
    DataTable_19E80 *textureTable;
    s32 unk10;
    s16 unk14;
    s16 coinCount;
    u16 animationFrame;
} GoldCoinRenderState;

typedef struct {
    Node n;
    s32 unk2C;
    s32 unk30;
    s32 padding[2];
    s32 unk3C;
} NodeWithPayload;

typedef struct {
    u8 _pad[0x14];
    s16 courseId;
} ItemBoxSystemTaskParams;

typedef struct {
    Transform3D matrix;
    u8 padding[0x1C];
    DisplayListObject displayList;
    s32 baseY;
    s16 rotationAngle;
    s16 stateTimer;
    s8 state;
    s8 isSecondaryItemBox;
    u8 padding4[0x2];
} ItemBox;

typedef struct {
    u8 _pad[0x8];
    ItemBox *itemBoxes;
    u8 _pad2[0xA];
    s16 itemBoxCount;
} ItemBoxController;

typedef struct {
    u8 padding[0x18];
    u8 unk18;
    u8 _pad[0x7];
    u8 unk20;
} SecondaryItemDropElement;

typedef struct {
    s16 unk0;
    u8 _pad[14];
} ItemBoxPositionEntry;

typedef struct {
    s32 offset;
} ItemBoxPositionBlock;

typedef struct {
    void *asset1;
    void *asset2;
    void *itemBoxMemory;
    ItemBoxPositionBlock *positionData;
    ItemBoxPositionEntry *positionEntries;
    s16 courseId;
    s16 itemBoxCount;
    u8 secondaryDropMode;
    u8 _pad3[0x3];
    u8 primaryDropMode;
    u8 _pad4[0x3];
    u8 secondaryDropSeed;
    u8 _pad5[0x3];
    u8 primaryDropSeed;
} ItemBoxSystemState;

typedef struct {
    u8 padding[0xA4];
    Player *player;
} PlayerHaloTaskParams;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 posX;
    s32 posY;
    s32 posZ;
    u8 _pad14[0xC];
} PlayerHaloElement;

typedef struct {
    void *assetData;
} PlayerHaloTask;

typedef struct {
    u8 padding[0x64];
    void *player;
} PlayerAuraTaskParams;

typedef struct {
    u8 _pad0[0x44];
    s32 displayListBase;
} GoldCoinAllocation;

typedef struct {
    u8 _pad[0xC];
    s32 unkC;
    s32 unk10;
    s32 unk14;
    u8 _pad2[0x44];
    u8 unk5C;
} AllocationStruct;

typedef struct {
    void *assetData;
    AssetMetadata_46080 elements[5];
    Player *player;
    s16 frameTimer;
    u16 animationIndex;
    s16 animationAngle;
} PlayerHaloState;

typedef struct {
    void *assetData;                /* 0x0 - Asset data pointer */
    loadAssetMetadata_arg metadata; /* 0x4 - Asset metadata (28 bytes) */
    u8 _pad20[0x6];                 /* 0x20 - Padding to reach 0x26 */
    s16 animFrame;                  /* 0x26 - Animation frame */
    s16 timer;                      /* 0x28 - Impact timer */
} HomingProjectileImpactArg;

/* Note: HomingProjectileUpdateArg uses individual fields to maintain exact memory layout */

typedef struct {
    void *assetData;   /* 0x0 - Asset data pointer */
    void *metadataPtr; /* 0x4 - Metadata pointer */
    u8 _pad[0x1E];
    s16 animFrame; /* 0x26 - Animation frame */
    u8 _pad2[0x4];
    s32 velX; /* 0x2C - X velocity */
    s32 velY; /* 0x30 - Y velocity */
    s32 velZ; /* 0x34 - Z velocity */
} HomingProjectileInitArg;

typedef struct {
    void *matrixBuffer;
    void *displayData;
    GoldCoinRenderEntry *entries;
    u8 _padC[0x4];
    s32 *compressedData;
    u8 _pad14[0x2];
    s16 coinCount;
    u16 animationFrame;
} GoldCoinUpdateState;

typedef struct {
    void *romStart;
    void *romEnd;
    s32 decompressedSize;
} CompressedAsset;

extern s8 D_80090C94_91894[];
extern u8 D_80090C95_91895[];
extern void *D_80094DA0_959A0;
extern void *D_80094DE0_959E0;
extern void *D_80094DF0_959F0;
extern void *D_80094E00_95A00;
extern void *D_80094E10_95A10;
extern void *D_80094EE0_95AE0;
extern void *D_80094EF0_95AF0;
extern void *D_80094FC0_95BC0;
extern void *D_80094FD0_95BD0;
extern void *D_800950B0_95CB0;
extern void *D_80095360_95F60;
extern void *D_80095370_95F70;
extern void *D_80095380_95F80;
extern void *D_80095460_96060;
extern void *D_80095470_96070;
extern void *D_80095480_96080;
extern void *D_800955C0_961C0;
extern void *D_800955D0_961D0;
extern void *D_800955E0_961E0;
extern void *D_80095860_96460;
extern void *D_80095930_96530;
extern Gfx D_8009A780_9B380[];
extern s32 D_800A8B14_9FE84;
extern CompressedAsset D_80090BD4_917D4[];
extern s32 gFlyingSceneryInitOffset[3];
extern CompressedAsset D_80090AF0_916F0[];
extern s32 D_8009A8A4_9B4A4;
extern CompressedAsset D_80090CEC_918EC[];
extern u8 D_80090CA8_918A8[][5];
extern u8 D_80090CE0_918E0[];
extern u8 D_80090CBC_918BC[][9];
extern s32 itemHomingProjectileBaseVector;
extern s32 D_80090E40_91A40;
extern s32 D_80090E4C_91A4C;
extern s16 D_80090E50_91A50;
extern s32 D_80090BC8_917C8[3];
extern s32 D_80090BBC_917BC[3];
extern s32 D_80090B98_91798[];
extern s32 D_80090BB0_917B0;
extern s32 gCourseSceneryOffset;
extern s32 D_80090BA4_917A4[3];
extern s32 gFrameCounter;
extern u16 D_8009ADE0_9B9E0;
extern Gfx *gRegionAllocPtr;
extern s16 gGraphicsMode;
extern Gfx D_80090DB0_919B0[];
extern s32 identityMatrix[];
extern void *D_80094DD0_959D0;
extern s32 bossHomingProjectileBaseVector;

void updateAllItemBoxes(ItemBoxController *arg0);
void initHomingProjectileMovement(HomingProjectileInitArg *arg0);
void updateHomingProjectileImpact(HomingProjectileImpactArg *arg0);
void cleanupGoldCoinsTask(GoldCoinsTaskState *arg0);
void setupGoldCoinEntries(GoldCoinSetupState *arg0);
void enqueuePlayerDisplayList(PlayerDisplayListState *arg0);
void initItemHomingProjectileMovement(ItemHomingProjectileInitArg *);
void func_8004B990_4C590(func_8004B990_4C590_arg *arg0);
void renderSkyDisplayListsWithCourseFog(SkyRenderTaskState *);
void updateFlyingSceneryAscendingStep(FlyingSceneryState *state);
void updateFlyingSceneryGlidingStep(FlyingSceneryState *state);
void updateFlyingSceneryTurningStep(FlyingSceneryState *state);
void cleanupPlayerSparkleTask(PlayerSparkleTask *arg0);
void loadPlayerSparkleData(PlayerSparkleTask *);
void updateCourseSceneryTask(CourseSceneryUpdateState *);
void cleanupCourseSceneryTask(CourseSceneryCleanupArg *);
void resetFlyingSceneryPosition(FlyingSceneryState *);
void updateFlyingSceneryReturnGlideStep(FlyingSceneryState *state);
void updateFlyingSceneryDescendingStep(FlyingSceneryState *state);
void func_8004C0D0_4CCD0(func_8004C0D0_4CCD0_arg *arg0);
void cleanupPanelProjectileTask(s32 **);
void cleanupHomingProjectileTask(s32 **);
void updateHomingProjectileMovement(HomingProjectileUpdateArg *arg0);
void setupItemBoxBurstTexture(ItemBoxBurstEffectState *arg0);
void updateItemBoxBurstFrame(ItemBoxBurstEffectState *state);
void cleanupItemBoxBurstEffect(ItemBoxBurstEffectState *arg0);
void cleanupItemHomingProjectileTask(func_8004B264_4BE64_arg *arg0);
void cleanupSkyRenderTask(SkyRenderTaskCleanupArg *);
void dispatchSkyRenderCallback(ScheduledTask *);
void updateItemHomingProjectileMovement(ItemHomingProjectileMoveArg *);
void func_80045CC8_468C8(void);
void cleanupBossHomingProjectileTask(BossHomingProjectileCleanupArg *);
void spawnBossHomingProjectileTask(void *arg0);
void updateItemHomingProjectileImpact(ItemHomingProjectileImpactArg *);
void updatePanelProjectileImpact(PanelProjectileImpactArg *arg0);
void updatePanelProjectileMovement(PanelProjectileUpdateArg *arg0);
void initPanelProjectileMovement(PanelProjectileInitArg *arg0);
void func_8004BC20_4C820(func_8004BC20_4C820_arg *arg0);
void func_8004B834_4C434(BossHomingProjectileSpawnArg *);
void func_8004BB0C_4C70C(func_8004BB0C_4C70C_arg *);
void func_8004BCFC_4C8FC(BossHomingProjectileSpawnArg *arg0);
void func_8004BE40_4CA40(func_8004BE40_4CA40_arg *arg0);
void func_8004BFBC_4CBBC(func_8004BFBC_4CBBC_arg *arg0);
void spawnBossHomingProjectile(BossHomingProjectileSpawnArg *arg0);
void bounceBossHomingProjectile(BossHomingProjectileBounceArg *arg0);
void updateBossHomingProjectile(BossHomingProjectile *arg0);
void renderSkyDisplayLists(SkyRenderTaskState *arg0);
void updatePlayerRenderCounter(void);
void cleanupPlayerRenderTask(PlayerRenderTaskCleanupArg *);
void cleanupSceneAnimationTask(SceneAnimationTask *);
void setupSceneAnimationTask(SceneAnimationTask *);
void updateSceneAnimationTask(SceneAnimationTaskNew *);
void updateScrollingSceneryTask(ScrollingSceneryTaskState *);
void cleanupScrollingSceneryTask(ScrollingSceneryCleanupState *arg0);
void updatePlayerSparkleWithStateCheck(PlayerSparkleTask *);
void updatePlayerSparkle(PlayerSparkleTask *);
void updatePlayerSparkleMovement(PlayerSparkleTask *);
void func_8004674C_4734C(DisplayListObject *);
void renderScrollingSceneryOpaque(DisplayListObject *);
void renderScrollingSceneryTransparent(DisplayListObject *);
void renderScrollingSceneryOverlay(DisplayListObject *);
void cleanupFlyingSceneryTask(CourseSceneryCleanupArg *);
void updateFlyingSceneryHorizontalStep(FlyingSceneryState *);
void updateFlyingSceneryVerticalStep(FlyingSceneryState *state);
void updatePlayerAuraHolding(PlayerAuraState *);
void loadPlayerAuraData(PlayerAuraState *);
void updatePlayerAuraRising(PlayerAuraState *);
void updatePlayerAuraDescending(PlayerAuraState *);
void cleanupPlayerAuraTask(PlayerAuraCleanupState *);
void renderGoldCoins(GoldCoinRenderState *state);
void cleanupPlayerHaloTask(PlayerHaloTask *arg0);
void loadPlayerHaloData(PlayerHaloState *arg0);
void updatePlayerHaloRising(PlayerHaloState *arg0);
void updatePlayerHaloDescending(PlayerHaloState *arg0);
void updatePlayerHaloAnimating(PlayerHaloState *arg0);
void spawnItemBoxBurstEffect(void *displayList, s32 isSecondaryBox);
void func_80048F0C_49B0C(ItemBoxSystemState *arg0, s32 arg1);
void initItemBoxPositions(ItemBoxSystemState *arg0);
void cleanupItemBoxSystem(ItemBoxSystemState *);
void renderItemBoxBurstEffect(ItemBoxBurstEffectState *state);
void updateGoldCoinsTask(GoldCoinUpdateState *arg0);

void initSkyRenderTask(SkyRenderTaskState *state) {
    void *identity = identityMatrix;

    memcpy(state, identity, 0x20);

    state->unk20 = func_80055E40_56A40(state->skyType);
    state->unk24 = func_80055DC4_569C4(state->skyType);
    state->unk28 = func_80055DF8_569F8(state->skyType);
    state->unk2C = 0;

    memcpy(state->unk3C, identity, 0x20);

    state->unk5C = func_80055E54_56A54(state->skyType);
    state->unk68 = 0;
    state->unk60 = state->unk24;
    state->unk64 = state->unk28;

    memcpy(state->unk78, identity, 0x20);

    state->unk98 = &D_80094DD0_959D0;
    state->unkA4 = 0;
    state->unk9C = state->unk24;
    state->unkA0 = state->unk28;

    setCleanupCallback(&cleanupSkyRenderTask);
    setCallback(&dispatchSkyRenderCallback);
}

void dispatchSkyRenderCallback(ScheduledTask *task) {
    if (task->skyType == 1) {
        if ((func_800698DC_6A4DC() & 0xFF) == 0x37) {
            setCallbackWithContinue(renderSkyDisplayListsWithCourseFog);
            return;
        }
    }
    setCallbackWithContinue(renderSkyDisplayLists);
}

void renderSkyDisplayLists(SkyRenderTaskState *arg0) {
    s32 i;

    for (i = 0; i < 4; i++) {
        enqueueDisplayListObjectWithFullRenderState(i, arg0);
    }

    if (arg0->unk5C != 0) {
        for (i = 0; i < 4; i++) {
            func_80065DA8_669A8(i + 4, (DisplayListObject *)&arg0->unk3C);
        }
    }
}

void renderSkyDisplayListsWithCourseFog(SkyRenderTaskState *arg0) {
    s32 nodeId;
    s32 viewportId;
    GameState *state;
    s32 playerOffset;
    s32 i;
    D_80090F90_91B90_item *levelData;
    u8 fogR;
    u16 fogNodeId;
    s32 fogG;
    s32 fogB;
    Player *player;

    state = (GameState *)getCurrentAllocation();

    i = 0;
    if (i < state->unk5F) {
    loop1:
        enqueueDisplayListObjectWithFullRenderState(i, arg0);
        i++;
        if (i < state->unk5F) {
            goto loop1;
        }
    }

    i = 0;
    if (i < state->unk5F) {
        fogG = 0x10;
        fogB = 0x30;
        nodeId = 0x64;
        viewportId = 4;
        playerOffset = 0;
    loop2:
        player = (Player *)(playerOffset + (s32)state->players);
        if (player->unkB94 < 0x39) {
            func_80065DA8_669A8(viewportId, (DisplayListObject *)&arg0->unk3C);
            levelData = func_80055D10_56910(state->memoryPoolId);
            fogR = levelData->unk20.r2;
            fogNodeId = nodeId;
            func_8006FE48_70A48(fogNodeId, 0x3E3, 0x3E7, fogR, levelData->unk20.g2, levelData->unk20.b2);
        } else {
            func_80065DA8_669A8(viewportId, (DisplayListObject *)&arg0->unk78);
            fogNodeId = nodeId;
            func_8006FE48_70A48(fogNodeId, 0x3E3, 0x3E7, 0x10, fogG, fogB);
        }
        nodeId++;
        viewportId++;
        i++;
        playerOffset += 0xBE8;
        if (i < state->unk5F) {
            goto loop2;
        }
    }
}

void cleanupSkyRenderTask(SkyRenderTaskCleanupArg *state) {
    state->unk24 = freeNodeMemory(state->unk24);
    state->unk28 = freeNodeMemory(state->unk28);
}

void scheduleSkyRenderTask(s32 skyType) {
    ScheduledTask *task = scheduleTask(initSkyRenderTask, 0, 0, 0xD2);
    if (task != NULL) {
        task->skyType = skyType;
    }
}

void initPlayerRenderTask(PlayerRenderTaskState *state) {
    u16 rotation;
    GameState_46080 *allocation;

    allocation = (GameState_46080 *)getCurrentAllocation();
    rotation = getTrackEndInfo(&allocation->unk30, &state->position);
    createYRotationMatrix((Transform3D *)state, rotation);
    state->unk20 = (void *)((u32)func_80055E68_56A68(allocation->unk5C) + 0x30);
    state->unk24 = func_80055DC4_569C4(allocation->unk5C);
    state->unk28 = func_80055DF8_569F8(allocation->unk5C);
    state->unk2C = NULL;
    state->unk40 = 1;
    setCleanupCallback(cleanupPlayerRenderTask);
    setCallback(updatePlayerRenderCounter);
}

void updatePlayerRenderCounter(void) {
    GameState *gameState = (GameState *)getCurrentAllocation();
    gameState->pendingPlayerRenderTasks -= 1;
    setCallbackWithContinue(&enqueuePlayerDisplayList);
}

void enqueuePlayerDisplayList(PlayerDisplayListState *state) {
    GameState *gameState;
    s32 pendingFrames;
    s32 playerIndex;
    Player *players;
    u8 playerSyncValue;
    u8 expectedSyncValue;
    func_80055E68_56A68_result *result;

    gameState = (GameState *)getCurrentAllocation();
    pendingFrames = state->pendingSyncFrames;

    if (pendingFrames != 0) {
        playerIndex = state->playerIndex;
        players = gameState->players;

        playerSyncValue = players[playerIndex].unkBC5;
        expectedSyncValue = gameState->unk74;

        if (playerSyncValue == expectedSyncValue) {
            state->pendingSyncFrames = pendingFrames - 1;
            result = func_80055E68_56A68(gameState->memoryPoolId);
            state->displayLists = &result->unk40;
        }
    }

    enqueueDisplayListObject(state->playerIndex, (DisplayListObject *)state);
}

void cleanupPlayerRenderTask(PlayerRenderTaskCleanupArg *state) {
    state->unk24 = freeNodeMemory(state->unk24);
    state->unk28 = freeNodeMemory(state->unk28);
}

void schedulePlayerRenderTask(s32 playerIndex) {
    PlayerRenderTask *task = (PlayerRenderTask *)scheduleTask(&initPlayerRenderTask, 0, 0, 0xD3);
    if (task != NULL) {
        task->playerIndex = playerIndex;
    }
}

void initSceneAnimationTask(SceneAnimationTask *arg0) {
    s16 courseID;

    arg0->assetData = func_80055D7C_5697C(arg0->assetIndex);
    courseID = arg0->dataIndex;
    arg0->loadedData = loadCompressedData(
        D_80090AF0_916F0[courseID].romStart,
        D_80090AF0_916F0[courseID].romEnd,
        D_80090AF0_916F0[courseID].decompressedSize
    );
    arg0->transformBuffer = 0;
    setCleanupCallback(cleanupSceneAnimationTask);
    setCallback(setupSceneAnimationTask);
}

void setupSceneAnimationTask(SceneAnimationTask *arg0) {
    s32 i;
    s32 offset;
    s32 *ptr;
    SceneAnimationEntry *entryList;
    SceneAnimationEntry *loop_ent;
    void *header;
    s32 sp10[2];

    header = arg0->loadedData;
    arg0->headerData = (void *)((s32)header + *(s32 *)header);
    arg0->entries = (SceneAnimationEntry *)((s32)arg0->loadedData + *(s32 *)((s32)arg0->loadedData + 4));

    entryList = arg0->entries;
    arg0->entryCount = 0;

    if (entryList->unk0 >= 0) {
        loop_ent = entryList;
        do {
            arg0->entryCount++;
        } while (loop_ent[arg0->entryCount].unk0 >= 0);
    }

    i = 0;
    arg0->transformBuffer = allocateNodeMemory(arg0->entryCount << 6);

    if (arg0->entryCount > 0) {
        ptr = &D_8009A8A4_9B4A4;
        offset = 0;
        do {
            memcpy(ptr, (u8 *)(offset + (s32)arg0->entries) + 8, 0xC);
            func_8006BFB8_6CBB8(ptr - 5, (u8 *)arg0->transformBuffer + (i << 6));
            i++;
            offset += 0x14;
        } while (i < arg0->entryCount);
    }

    setCallback(&updateSceneAnimationTask);
}

void updateSceneAnimationTask(SceneAnimationTaskNew *arg0) {
    s32 i;

    arg0->frameCounter = (arg0->frameCounter + 1) & 0x7FFFFFFF;

    for (i = 0; i < arg0->entryCount; i++) {
        if (arg0->entries[i].unk6 == 0) {
            break;
        }

        if (arg0->entries[i].unk6 == 1) {
            func_8005BCB8_5C8B8(arg0->entries[i].unk8, 0x100000, 0xC00000);
        }

        if (arg0->entries[i].unk6 == 2) {
            func_8005BCB8_5C8B8(arg0->entries[i].unk8, 0x100000, 0x680000);
        }

        if (arg0->entries[i].unk6 == 3) {
            func_8005BCB8_5C8B8(arg0->entries[i].unk8, 0x140000, 0x300000);
        }

        if (arg0->entries[i].unk6 == 4) {
            func_8005BCB8_5C8B8(arg0->entries[i].unk8, 0x120000, 0xC00000);
        }
    }

    for (i = 0; i < 4; i++) {
        debugEnqueueCallback((u16)i, 4, func_80045CC8_468C8, arg0);
    }
}

void cleanupSceneAnimationTask(SceneAnimationTask *arg0) {
    arg0->transformBuffer = freeNodeMemory(arg0->transformBuffer);
    arg0->assetData = freeNodeMemory(arg0->assetData);
    arg0->loadedData = freeNodeMemory(arg0->loadedData);
}

INCLUDE_ASM("asm/nonmatchings/46080", func_80045CC8_468C8);

void scheduleSceneAnimationTask(s32 arg0, s16 arg1) {
    SceneAnimationTask *task = (SceneAnimationTask *)scheduleTask(initSceneAnimationTask, 0, 0, 0xD3);
    if (task != NULL) {
        task->assetIndex = arg0;
        task->dataIndex = arg1;
    }
}

void initPlayerSparkleTask(PlayerSparkleTask *task) {
    task->assetData = loadAsset_34CB50();
    setCleanupCallback(cleanupPlayerSparkleTask);
    setCallback(loadPlayerSparkleData);
}

void loadPlayerSparkleData(PlayerSparkleTask *task) {
    GameState *state = (GameState *)getCurrentAllocation();

    loadAssetMetadata(&task->metadata, task->assetData, 0);
    task->metadata.unk0 = (loadAssetMetadata_arg *)((u8 *)state->unk44 + 0x40);

    switch (state->memoryPoolId) {
        case 0:
            memcpy(&task->metadata.position, state->unk48 + 0x24, sizeof(Vec3i));
            setCallback(updatePlayerSparkleWithStateCheck);
            break;
        case 6:
            memcpy(&task->metadata.position, state->unk48 + 0x1C8, sizeof(Vec3i));
            setCallback(updatePlayerSparkle);
            break;
        case 1:
            memcpy(&task->metadata.position, state->unk48 + 0x1EC, sizeof(Vec3i));
            setCallback(updatePlayerSparkle);
            break;
        case 2:
            memcpy(&task->metadata.position, state->unk48 + 0x1F8, sizeof(Vec3i));
            setCallback(updatePlayerSparkle);
            break;
        case 5:
            memcpy(&task->metadata.position, state->unk48 + 0x204, sizeof(Vec3i));
            setCallback(updatePlayerSparkle);
            break;
        case 8:
            memcpy(&task->metadata.position, state->unk48 + 0x234, sizeof(Vec3i));
            setCallback(updatePlayerSparkle);
            break;
        case 9:
            memcpy(&task->metadata.position, state->unk48 + 0x240, sizeof(Vec3i));
            setCallback(updatePlayerSparkle);
            break;
    }
}

void updatePlayerSparkle(PlayerSparkleTask *task) {
    s32 i;
    u16 counter;

    counter = task->animationCounter + 1;
    task->animationCounter = counter & 0xF;

    if ((counter & 0x7) != 0) {
        loadAssetMetadata(&task->metadata, task->assetData, (counter & 0xF) >> 3);
    }

    for (i = 0; i < 4; i++) {
        func_80066444_67044(i, (func_80066444_67044_arg1 *)&task->metadata);
    }
}

void updatePlayerSparkleWithStateCheck(PlayerSparkleTask *task) {
    GameState *state;
    s32 i;

    state = (GameState *)getCurrentAllocation();
    updatePlayerSparkle(task);

    for (i = 0; i < state->unk5F; i++) {
        if ((u32)(state->players[i].unkB94 - 6) < 5U) {
            task->velocity = 0xC0000;
            setCallback(updatePlayerSparkleMovement);
            return;
        }
    }
}

void updatePlayerSparkleMovement(PlayerSparkleTask *task) {
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_a0;
    s32 temp_a1;
    s32 temp_a2;

    temp_v0 = task->metadata.position.x + 0x27D76;
    temp_v1 = task->metadata.position.z + 0x44DDE;
    temp_a0 = task->velocity + 0xFFFE8000;
    temp_a1 = task->metadata.position.y + temp_a0;
    temp_a2 = 0x2C150614 < temp_a1;

    task->metadata.position.x = temp_v0;
    task->metadata.position.z = temp_v1;
    task->velocity = temp_a0;
    task->metadata.position.y = temp_a1;

    if (temp_a2 == 0) {
        func_80069CF8_6A8F8();
    }

    updatePlayerSparkle(task);
}

void cleanupPlayerSparkleTask(PlayerSparkleTask *task) {
    task->assetData = freeNodeMemory(task->assetData);
}

void initScrollingSceneryTask(ScrollingSceneryCleanupState *arg0) {
    arg0->unk24 = func_80055DC4_569C4(arg0->assetPoolIndex);
    arg0->unk28 = func_80055DF8_569F8(arg0->assetPoolIndex);
    arg0->unk2C = NULL;
    memcpy(arg0, identityMatrix, 0x20);
    arg0->unk3C = func_80055D7C_5697C(arg0->assetPoolIndex);
    arg0->scrollX.halfword = 0;
    arg0->scrollY.halfword = 0;
    setCleanupCallback(&cleanupScrollingSceneryTask);
    setCallback(&updateScrollingSceneryTask);
}

void updateScrollingSceneryTask(ScrollingSceneryTaskState *arg0) {
    u8 scrollXLow;
    u8 scrollYLow;
    s32 i;

    arg0->scrollX.halfword = arg0->scrollX.halfword + arg0->scrollSpeedX;
    scrollXLow = arg0->scrollX.bytes.low;

    arg0->scrollY.halfword = arg0->scrollY.halfword + arg0->scrollSpeedY;
    scrollYLow = arg0->scrollY.bytes.low;

    arg0->unk30 = 0;
    arg0->scrollX.halfword = scrollXLow;
    arg0->scrollY.halfword = scrollYLow;

    for (i = 0; i < 4; i++) {
        if (arg0->displayLists->opaqueDisplayList != NULL) {
            debugEnqueueCallback(i, 1, &renderScrollingSceneryOpaque, arg0);
        }

        if (arg0->displayLists->transparentDisplayList != NULL) {
            debugEnqueueCallback(i, 3, &renderScrollingSceneryTransparent, arg0);
        }

        if (arg0->displayLists->overlayDisplayList != NULL) {
            debugEnqueueCallback(i, 5, &renderScrollingSceneryOverlay, arg0);
        }
    }
}

void cleanupScrollingSceneryTask(ScrollingSceneryCleanupState *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
    arg0->unk3C = freeNodeMemory(arg0->unk3C);
}

INCLUDE_ASM("asm/nonmatchings/46080", func_8004674C_4734C);

void renderScrollingSceneryOpaque(DisplayListObject *arg0) {
    func_8004674C_4734C(arg0);
    renderOpaqueDisplayList(arg0);
}

void renderScrollingSceneryTransparent(DisplayListObject *arg0) {
    func_8004674C_4734C(arg0);
    renderTransparentDisplayList(arg0);
}

void renderScrollingSceneryOverlay(DisplayListObject *arg0) {
    func_8004674C_4734C(arg0);
    renderOverlayDisplayList(arg0);
}

void scheduleScrollingSceneryTask(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    ScrollingSceneryTask *task = (ScrollingSceneryTask *)scheduleTask(&initScrollingSceneryTask, 0, 0, 0xD2);
    if (task != NULL) {
        task->displayLists = arg0;
        task->assetPoolIndex = arg1;
        task->scrollSpeedX = arg2;
        task->scrollSpeedY = arg3;
        task->unk4E = arg4;
        task->unk50 = arg5;
    }
}

void initCourseSceneryTask(CourseSceneryTaskState *arg0) {
    Vec3i sp10;
    GameState_46080 *allocation;
    D_80090F90_91B90_item *levelData;
    u16 rotation;
    func_80055E68_56A68_result *displayLists;

    allocation = (GameState_46080 *)getCurrentAllocation();
    rotation = getTrackEndInfo(&allocation->unk30, &sp10) + 0x800;
    levelData = func_80055D10_56910(allocation->unk5C);

    arg0->unk20 = func_80055E68_56A68(allocation->unk5C);
    arg0->unk24 = func_80055DC4_569C4(allocation->unk5C);
    arg0->unk28 = func_80055DF8_569F8(allocation->unk5C);
    arg0->unk2C = NULL;

    createYRotationMatrix((Transform3D *)arg0, (u16)(rotation + levelData->unk8));

    arg0->unk18 = sp10.y;
    transformVector2(&gCourseSceneryOffset, arg0, &sp10);

    arg0->unk14 = levelData->unk0 + sp10.x;
    arg0->unk1C = levelData->unk4 + sp10.z;

    memcpy(arg0->unk3C, arg0, 0x3C);

    displayLists = func_80055E68_56A68(allocation->unk5C);
    arg0->unk5C = (void *)((u32)displayLists + 0x20);
    createYRotationMatrix((Transform3D *)arg0->unk3C, 0x1000);

    arg0->unk50 = levelData->unkC.x;
    arg0->unk54 = levelData->unkC.y;
    arg0->unk58 = levelData->unkC.z;

    if (allocation->unk5C == 4) {
        memcpy(arg0->unk78, arg0, 0x3C);
        displayLists = func_80055E68_56A68(allocation->unk5C);
        arg0->unk98 = (void *)((u32)displayLists + 0x90);
        arg0->unk90 = arg0->unk90 + 0xAB0000;
    }

    setCleanupCallback(cleanupCourseSceneryTask);
    setCallback(updateCourseSceneryTask);
}

void updateCourseSceneryTask(CourseSceneryUpdateState *arg0) {
    GameState_46080 *allocation;
    s32 i;
    s16 rotation;

    allocation = (GameState_46080 *)getCurrentAllocation();

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)&arg0->unk3C);
    }

    if (allocation->unk5C == 4) {
        rotation = arg0->rotationAngle + 0x80;
        arg0->rotationAngle = rotation;
        createYRotationMatrix((Transform3D *)&arg0->unk78, rotation);

        for (i = 0; i < 4; i++) {
            enqueueDisplayListObject(i, (DisplayListObject *)&arg0->unk78);
        }
    }
}

void cleanupCourseSceneryTask(CourseSceneryCleanupArg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

void initFlyingSceneryTask(FlyingSceneryState *arg0) {
    Vec3i position;
    GameState_46080 *allocation;
    u16 rotation;
    D_80090F90_91B90_item *levelData;

    allocation = (GameState_46080 *)getCurrentAllocation();
    rotation = getTrackEndInfo(&allocation->unk30, &position) + 0x800;
    levelData = func_80055D10_56910(allocation->unk5C);

    arg0->displayListObject.displayLists = (void *)((u32)func_80055E68_56A68(allocation->unk5C) + 0x10);
    arg0->displayListObject.segment1 = func_80055DC4_569C4(allocation->unk5C);
    arg0->displayListObject.segment2 = func_80055DF8_569F8(allocation->unk5C);
    arg0->displayListObject.segment3 = 0;

    createYRotationMatrix((Transform3D *)arg0, rotation + levelData->unk8);
    arg0->displayListObject.transform.translation.y = position.y;

    transformVector2(&gFlyingSceneryInitOffset, arg0, &position);

    arg0->displayListObject.transform.translation.x = levelData->unk0 + position.x;
    arg0->displayListObject.transform.translation.z = levelData->unk4 + position.z;
    arg0->frameCounter = 0x30;

    setCleanupCallback(&cleanupFlyingSceneryTask);
    setCallbackWithContinue(&updateFlyingSceneryHorizontalStep);
}

void updateFlyingSceneryHorizontalStep(FlyingSceneryState *state) {
    Vec3i movement;
    s32 i;
    Allocation_47D1C *allocation;

    allocation = (Allocation_47D1C *)getCurrentAllocation();

    if (allocation->unk76 == 0) {
        transformVector2(D_80090B98_91798, state, &movement);

        state->displayListObject.transform.translation.x += movement.x;
        state->displayListObject.transform.translation.z += movement.z;

        if (state->frameCounter != 0) {
            state->frameCounter--;
        } else {
            state->frameCounter = 0xB4;
            setCallback(updateFlyingSceneryVerticalStep);
        }
    }

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)state);
    }
}

void updateFlyingSceneryVerticalStep(FlyingSceneryState *state) {
    Allocation_47D1C *allocation;
    Vec3i movement;
    s32 i;

    allocation = (Allocation_47D1C *)getCurrentAllocation();

    if (allocation->unk76 == 0) {
        transformVector2(D_80090BA4_917A4, state, &movement);

        state->displayListObject.transform.translation.x += movement.x;
        state->displayListObject.transform.translation.y += movement.y;
        state->displayListObject.transform.translation.z += movement.z;

        if (state->frameCounter != 0) {
            state->frameCounter -= 1;
        } else {
            setCallback(&resetFlyingSceneryPosition);
        }
    }

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)state);
    }
}

void resetFlyingSceneryPosition(FlyingSceneryState *state) {
    Vec3i vec;
    AllocationStruct *alloc;
    D_80090F90_91B90_item *matrix;

    alloc = (AllocationStruct *)getCurrentAllocation();
    createYRotationMatrix(&state->displayListObject.transform, 0x1000);
    matrix = func_80055D10_56910(alloc->unk5C);
    transformVector2(D_80090BC8_917C8, &state->displayListObject.transform, &vec);

    state->displayListObject.transform.translation.x = matrix->unkC.x + vec.x;
    state->displayListObject.transform.translation.y = matrix->unkC.y + vec.y;
    state->displayListObject.transform.translation.z = matrix->unkC.z + vec.z;
    state->frameCounter = 0x32;

    setCallbackWithContinue(updateFlyingSceneryAscendingStep);
}

void updateFlyingSceneryAscendingStep(FlyingSceneryState *state) {
    AllocationData *allocation = getCurrentAllocation();
    s32 i;
    Vec3i tempVec;

    if (allocation->unk76 == 0) {
        transformVector2(D_80090BA4_917A4, &state->displayListObject.transform, &tempVec);
        state->displayListObject.transform.translation.x += tempVec.x;
        state->displayListObject.transform.translation.y += tempVec.y;
        state->displayListObject.transform.translation.z += tempVec.z;

        if (state->frameCounter != 0) {
            state->frameCounter--;
        } else {
            state->frameCounter = 0x2C;
            setCallback(&updateFlyingSceneryGlidingStep);
        }
    }

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, &state->displayListObject);
    }
}

void updateFlyingSceneryGlidingStep(FlyingSceneryState *state) {
    Vec3i vec;
    s32 i;
    AllocationData *allocation;

    allocation = getCurrentAllocation();
    i = 0;

    if (allocation->unk76 == 0) {
        transformVector2(D_80090B98_91798, state, &vec);

        state->displayListObject.transform.translation.x += vec.x;
        state->displayListObject.transform.translation.y += vec.y;
        state->displayListObject.transform.translation.z += vec.z;

        if (state->frameCounter != 0) {
            state->frameCounter--;
        } else {
            state->rotationAngle = 0;
            setCallback(updateFlyingSceneryTurningStep);
        }
        i = 0;
    }

    do {
        enqueueDisplayListWithFrustumCull(i, &state->displayListObject);
        i++;
    } while (i < 4);
}

void updateFlyingSceneryTurningStep(FlyingSceneryState *state) {
    AllocationData *allocation;
    Vec3i vector;
    s32 i;
    s32 rotation;
    u16 angle;

    allocation = getCurrentAllocation();

    if (allocation->unk76 == 0) {
        rotation = state->rotationAngle;
        angle = rotation + 0x1080;
        state->rotationAngle = rotation + 0x80;
        createYRotationMatrix(&state->displayListObject.transform, angle);
        transformVector2(&D_80090BB0_917B0, state, &vector);

        state->displayListObject.transform.translation.x += vector.x;
        state->displayListObject.transform.translation.y += vector.y;
        state->displayListObject.transform.translation.z += vector.z;

        if (state->rotationAngle == 0x1080) {
            state->frameCounter = 0x2C;
            setCallback(&updateFlyingSceneryReturnGlideStep);
        }
    }

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, &state->displayListObject);
    }
}

void updateFlyingSceneryReturnGlideStep(FlyingSceneryState *state) {
    Allocation_47D1C *allocation;
    Vec3i vec;
    s32 i;

    allocation = (Allocation_47D1C *)getCurrentAllocation();

    if (allocation->unk76 == 0) {
        createYRotationMatrix(&state->displayListObject.transform, 0);
        transformVector2(D_80090B98_91798, state, &vec);

        state->displayListObject.transform.translation.x += vec.x;
        state->displayListObject.transform.translation.y += vec.y;
        state->displayListObject.transform.translation.z += vec.z;

        if (state->frameCounter != 0) {
            state->frameCounter -= 1;
        } else {
            state->frameCounter = 0x32;
            setCallback(&updateFlyingSceneryDescendingStep);
        }
    }

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, &state->displayListObject);
    }
}

void updateFlyingSceneryDescendingStep(FlyingSceneryState *state) {
    GameState *alloc;
    Vec3i stackBuffer;
    s32 i;

    alloc = (GameState *)getCurrentAllocation();

    if (alloc->gamePaused == 0) {
        transformVector2(D_80090BBC_917BC, state, &stackBuffer);

        state->displayListObject.transform.translation.x += stackBuffer.x;
        state->displayListObject.transform.translation.y += stackBuffer.y;
        state->displayListObject.transform.translation.z += stackBuffer.z;

        if (state->frameCounter != 0) {
            state->frameCounter--;
        } else {
            func_80069CF8_6A8F8();
        }

        i = 0;
    }

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, &state->displayListObject);
    }
}

void cleanupFlyingSceneryTask(CourseSceneryCleanupArg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

void initGoldCoinsTask(GoldCoinsTaskState *arg0) {
    s16 courseIndex;

    arg0->unkC = loadCompressedData(&_3F6670_ROM_START, &_3F6950_ROM_START, 0x388);

    courseIndex = arg0->courseIndex;

    arg0->unk10 = loadCompressedData(
        D_80090BD4_917D4[courseIndex].romStart,
        D_80090BD4_917D4[courseIndex].romEnd,
        D_80090BD4_917D4[courseIndex].decompressedSize
    );

    arg0->unk0 = 0;
    setCleanupCallback(cleanupGoldCoinsTask);
    setCallback(setupGoldCoinEntries);
}

void setupGoldCoinEntries(GoldCoinSetupState *arg0) {
    GoldCoinAllocation *alloc;
    s32 *ptr;
    s32 i;
    s32 *globalBuf;
    s8 one;
    s32 sp10[2];
    s32 offset;

    (void)sp10;

    alloc = (GoldCoinAllocation *)getCurrentAllocation();
    arg0->displayData = (void *)(alloc->displayListBase + 0x80);

    ptr = arg0->compressedData;
    arg0->entries = (GoldCoinEntry *)((u8 *)ptr + *ptr);

    arg0->animationFrame = 0;
    arg0->coinCount = 0;

    if (arg0->entries[0].visible >= 0) {
        do {
            arg0->coinCount++;
        } while (arg0->entries[arg0->coinCount].visible >= 0);
    }

    i = 0;
    arg0->matrixBuffer = allocateNodeMemory(arg0->coinCount << 6);

    if (arg0->coinCount > 0) {
        one = 1;
        globalBuf = &D_8009A8A4_9B4A4;
        do {
            offset = i * 16;
            *(s8 *)(offset + (s32)arg0->entries) = one;
            memcpy(globalBuf, (u8 *)(offset + (s32)arg0->entries) + 4, 0xC);
            func_8006BFB8_6CBB8(globalBuf - 5, (u8 *)arg0->matrixBuffer + (i << 6));
            i++;
        } while (i < arg0->coinCount);
    }

    setCallback(updateGoldCoinsTask);
}

void updateGoldCoinsTask(GoldCoinUpdateState *arg0) {
    s32 i;
    Player *player;
    s16 timer;

    if (D_8009ADE0_9B9E0 & 1) {
        arg0->animationFrame = arg0->animationFrame + 1;
    }

    if ((s16)arg0->animationFrame >= 6) {
        arg0->animationFrame = 0;
    }

    timer = 0;
    for (i = 0; i < arg0->coinCount; i++) {
        timer = arg0->entries[i].visible;
        if (timer == 0) {
            timer = arg0->entries[i].respawnTimer;
            if (timer != 0) {
                arg0->entries[i].respawnTimer = timer - 1;
            } else if (arg0->entries[i].processed == 0) {
                arg0->entries[i].visible = 1;
            }
        } else {
            player = (Player *)func_8005B24C_5BE4C(&arg0->entries[i].position, -1, 0x100000);
            if (player != NULL) {
                queueSoundAtPosition(&arg0->entries[i].position, 7);
                arg0->entries[i].visible = 0;
                arg0->entries[i].respawnTimer = 0x1E;
                func_80059A48_5A648(player, 0x64);
            }
        }
        arg0->entries[i].processed = 2;
    }

    for (i = 0; i < 4; i++) {
        debugEnqueueCallback((u16)i, 4, renderGoldCoins, arg0);
    }
}

void cleanupGoldCoinsTask(GoldCoinsTaskState *arg0) {
    arg0->unk0 = freeNodeMemory(arg0->unk0);
    arg0->unkC = freeNodeMemory(arg0->unkC);
    arg0->unk10 = freeNodeMemory(arg0->unk10);
}

void renderGoldCoins(GoldCoinRenderState *state) {
    OutputStruct_19E80 tableEntry;
    s32 dxtBase;
    s32 new_var;
    u32 line;
    s32 lrs;
    u16 widthDiv16;
    Gfx *loadBlockCmd;
    long loadBlockWord;
    s32 i;
    u32 tileLine;

    gSPDisplayList(gRegionAllocPtr++, D_8009A780_9B380);
    gGraphicsMode = -1;

    getTableEntryByU16Index(state->textureTable, state->animationFrame, &tableEntry);

    gDPSetTextureImage(gRegionAllocPtr++, G_IM_FMT_CI, G_IM_SIZ_16b, 1, tableEntry.data_ptr);

    gDPSetTile(
        gRegionAllocPtr++,
        G_IM_FMT_CI,
        G_IM_SIZ_16b,
        0,
        0x0000,
        G_TX_LOADTILE,
        0,
        G_TX_CLAMP,
        G_TX_NOMASK,
        G_TX_NOLOD,
        G_TX_CLAMP,
        G_TX_NOMASK,
        G_TX_NOLOD
    );

    gDPLoadSync(gRegionAllocPtr++);

    loadBlockCmd = gRegionAllocPtr++;
    loadBlockCmd->words.w0 = 0xF3000000;
    widthDiv16 = tableEntry.field1 >> 4;
    dxtBase = 0x800;
    if (widthDiv16 != 0) {
        dxtBase = widthDiv16 + 0x7FF;
    }
    lrs = (((s32)((tableEntry.field1 * tableEntry.field2) + 3)) >> 2) - 1;
    if (lrs < 0x800) {
    } else {
        lrs = 0x7FF;
    }
    line = lrs & 0xFFF;
    new_var = (line << 12) | 0x07000000;
    loadBlockWord = new_var;
    if (widthDiv16 != 0) {
        loadBlockWord |= (dxtBase / widthDiv16) & 0xFFF;
    } else {
        loadBlockWord |= dxtBase & 0xFFF;
    }
    loadBlockCmd->words.w1 = loadBlockWord;

    gDPPipeSync(gRegionAllocPtr++);

    tileLine = (((tableEntry.field1 >> 1) + 7) >> 3) & 0x1FF;
    new_var = G_TX_NOMIRROR;
    gDPSetTile(
        gRegionAllocPtr++,
        G_IM_FMT_CI,
        G_IM_SIZ_4b,
        tileLine,
        0,
        G_TX_RENDERTILE,
        0,
        G_TX_CLAMP,
        G_TX_NOMASK,
        G_TX_NOLOD,
        G_TX_CLAMP,
        G_TX_NOMASK,
        G_TX_NOLOD
    );

    gDPSetTileSize(
        gRegionAllocPtr++,
        G_TX_RENDERTILE,
        0,
        0,
        (tableEntry.field1 - 1) << 2,
        (tableEntry.field2 - 1) << 2
    );

    gDPSetTextureImage(gRegionAllocPtr++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, tableEntry.index_ptr);

    gDPTileSync(gRegionAllocPtr++);

    gDPSetTile(
        gRegionAllocPtr++,
        G_IM_FMT_RGBA,
        G_IM_SIZ_4b,
        0,
        0x0100,
        G_TX_LOADTILE,
        0,
        G_TX_NOMIRROR | G_TX_WRAP,
        G_TX_NOMASK,
        G_TX_NOLOD,
        G_TX_NOMIRROR | G_TX_WRAP,
        G_TX_NOMASK,
        G_TX_NOLOD
    );

    gDPLoadSync(gRegionAllocPtr++);

    gDPLoadTLUTCmd(gRegionAllocPtr++, G_TX_LOADTILE, 15);

    gDPPipeSync(gRegionAllocPtr++);

    for (i = 0; i < state->coinCount; i++) {
        if (isObjectCulled(&state->entries[i].position) == 0) {
            state->entries[i].processed = 1;
            if (state->entries[i].visible != 0) {
                gSPMatrix(
                    gRegionAllocPtr++,
                    (u8 *)state->matrixBuffer + (i << 6),
                    G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW
                );
                gSPMatrix(gRegionAllocPtr++, D_800A8B14_9FE84, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
                gSPVertex(gRegionAllocPtr++, state->displayData, 4, 0);
                gSP2Triangles(gRegionAllocPtr++, 0, 3, 2, 0, 2, 1, 0, 0);
            }
        } else {
            state->entries[i].processed &= 1;
        }
    }
}

void scheduleGoldCoinsTask(s16 courseIndex) {
    GoldCoinsTaskState *result = (GoldCoinsTaskState *)scheduleTask(initGoldCoinsTask, 0, 0, 0xD3);
    if (result != NULL) {
        result->courseIndex = courseIndex;
    }
}

void initPlayerAuraTask(PlayerAuraTask *task) {
    task->assetData = loadAsset_34CB50();
    setCleanupCallback(&cleanupPlayerAuraTask);
    setCallbackWithContinue(&loadPlayerAuraData);
}

void loadPlayerAuraData(PlayerAuraState *state) {
    AllocationType_46080 *alloc;

    alloc = (AllocationType_46080 *)getCurrentAllocation();
    loadAssetMetadata(&state->element0.asset, state->assetData, 2);
    loadAssetMetadata(&state->element1.asset, state->assetData, 3);
    loadAssetMetadata(&state->element2.asset, state->assetData, 4);

    state->element0.asset.unk0 = (loadAssetMetadata_arg *)(alloc->unk44 + 0xC0);
    state->element1.asset.unk0 = (loadAssetMetadata_arg *)(alloc->unk44 + 0x100);
    state->element2.asset.unk0 = (loadAssetMetadata_arg *)(alloc->unk44 + 0x140);
    state->animationAngle = 0;
    setCallbackWithContinue(&updatePlayerAuraRising);
}

void updatePlayerAuraRising(PlayerAuraState *state) {
    GameState *allocation;
    s32 heightOffset;
    s32 sinVal;
    s32 i;
    s32 j;
    s16 temp;
    s32 offset;
    PlayerAuraElement *elem;

    allocation = (GameState *)getCurrentAllocation();

    if (allocation->gamePaused == 0) {
        temp = state->animationAngle + 0x40;
        state->animationAngle = temp;
        if (temp == 0x800) {
            state->holdTimer = 0x1E;
            loadAssetMetadata(&state->element2.asset, state->assetData, 5);
            setCallback(&updatePlayerAuraHolding);
        }
    }

    sinVal = approximateSin(state->animationAngle);
    heightOffset = ((0x2000 - sinVal) << 10) + 0x190000;
    i = 0;
    elem = (PlayerAuraElement *)state;

loop:
    elem->posX = state->player->worldPos.x;
    elem->posY = state->player->worldPos.y + heightOffset;
    elem->posZ = state->player->worldPos.z;
    i += 1;
    elem += 1;
    if (i < 3)
        goto loop;

    i = 0;
    j = 0;
    do {
        offset = 4;
        do {
            func_80066444_67044(i, (func_80066444_67044_arg1 *)((s32)state + offset));
            j++;
            offset += 0x20;
        } while (j < 3);
        i++;
        j = 0;
    } while (i < 4);
}

void updatePlayerAuraHolding(PlayerAuraState *state) {
    GameState *allocation;
    s32 heightOffset;
    s32 i;
    s32 j;
    s32 offset;
    PlayerAuraElement *elem;

    allocation = (GameState *)getCurrentAllocation();

    if (allocation->gamePaused == 0) {
        state->holdTimer -= 1;
        if (state->holdTimer == 0) {
            loadAssetMetadata(&state->element2.asset, state->assetData, 4);
            setCallback(updatePlayerAuraDescending);
        }
    }

    i = 0;
    heightOffset = 0x190000;
    elem = (PlayerAuraElement *)state;

loop:
    elem->posX = state->player->worldPos.x;
    elem->posY = state->player->worldPos.y + heightOffset;
    elem->posZ = state->player->worldPos.z;
    i += 1;
    elem += 1;
    if (i < 3)
        goto loop;

    i = 0;
    j = 0;
    do {
        offset = 4;
        do {
            func_80066444_67044(i, (func_80066444_67044_arg1 *)((s32)state + offset));
            j++;
            offset += 0x20;
        } while (j < 3);
        i++;
        j = 0;
    } while (i < 4);
}

void updatePlayerAuraDescending(PlayerAuraState *state) {
    GameState *allocation;
    s32 heightOffset;
    s32 i;
    s32 j;
    s32 offset;
    PlayerAuraElement *elem;

    allocation = (GameState *)getCurrentAllocation();

    if (allocation->gamePaused == 0) {
        state->animationAngle -= 0x40;
        if (state->animationAngle == 0) {
            func_80069CF8_6A8F8();
        }
    }

    heightOffset = ((0x2000 - approximateSin(state->animationAngle)) * 5 * 256) + 0x190000;
    i = 0;
    elem = (PlayerAuraElement *)state;

loop:
    elem->posX = state->player->worldPos.x;
    elem->posY = state->player->worldPos.y + heightOffset;
    elem->posZ = state->player->worldPos.z;
    i += 1;
    elem += 1;
    if (i < 3)
        goto loop;

    i = 0;
    j = 0;
    do {
        offset = 4;
        do {
            func_80066444_67044(i, (void *)((s32)state + offset));
            j++;
            offset += 0x20;
        } while (j < 3);
        i++;
        j = 0;
    } while (i < 4);
}

void cleanupPlayerAuraTask(PlayerAuraCleanupState *state) {
    state->assetData = freeNodeMemory(state->assetData);
}

void schedulePlayerAuraTask(void *player) {
    PlayerAuraTaskParams *task;

    task = (PlayerAuraTaskParams *)scheduleTask(initPlayerAuraTask, 0, 0, 0xD4);
    if (task != NULL) {
        task->player = player;
    }
}

void initPlayerHaloTask(PlayerHaloTask *task) {
    task->assetData = loadAsset_34CB50();
    setCleanupCallback(&cleanupPlayerHaloTask);
    setCallbackWithContinue(&loadPlayerHaloData);
}

void loadPlayerHaloData(PlayerHaloState *state) {
    AllocationType_46080 *allocation;
    s32 i;
    s32 offset;

    allocation = (AllocationType_46080 *)getCurrentAllocation();

    loadAssetMetadata(&state->elements[0].asset, state->assetData, 2);
    loadAssetMetadata(&state->elements[1].asset, state->assetData, 3);

    for (i = 2; i < 5; i++) {
        loadAssetMetadata(&state->elements[i].asset, state->assetData, 6);
    }

    state->animationIndex = 0;

    i = 0;
    offset = 0x180;
    for (; i < 5; i++) {
        state->elements[i].asset.unk0 = (loadAssetMetadata_arg *)(allocation->unk44 + offset);
        offset += 0x40;
    }

    state->animationAngle = 0;
    setCallbackWithContinue(&updatePlayerHaloRising);
}

void updatePlayerHaloRising(PlayerHaloState *state) {
    Allocation_47D1C *allocation;
    s32 heightOffset;
    s32 sinVal;
    s32 j;
    s32 i;
    PlayerHaloElement *elem;

    allocation = (Allocation_47D1C *)getCurrentAllocation();

    if (allocation->unk76 == 0) {
        state->animationAngle += 0x40;
        if (state->animationAngle == 0x800) {
            state->frameTimer = 0;
            setCallback(&updatePlayerHaloAnimating);
        }
    }

    sinVal = approximateSin(state->animationAngle);
    heightOffset = ((0x2000 - sinVal) << 10) + 0x200000;
    j = 0;
    elem = (PlayerHaloElement *)state;

loop:
    elem->posX = state->player->worldPos.x;
    elem->posY = state->player->worldPos.y + heightOffset;
    elem->posZ = state->player->worldPos.z;
    j += 1;
    elem += 1;
    if (j < 5)
        goto loop;

    for (i = 0; i < 2; i++) {
        func_80066444_67044(state->player->unkBB8, (func_80066444_67044_arg1 *)&state->elements[i]);
    }

    for (i = 2; i < 5; i++) {
        func_800670A4_67CA4(state->player->unkBB8, (func_80066444_67044_arg1 *)&state->elements[i]);
    }
}

void updatePlayerHaloAnimating(PlayerHaloState *state) {
    Allocation_47D1C *allocation;
    s32 i;
    s32 offset;
    PlayerHaloElement *elem;
    s32 j;
    s16 frameTimer;
    u16 index;
    s32 tableIdx;
    s32 heightOffset;

    frameTimer = state->frameTimer;
    if (frameTimer == 0) {
        index = state->animationIndex + 1;
        state->animationIndex = index;
        state->frameTimer = D_80090C94_91894[(s16)index * 2];

        i = 2;
        offset = 0x44;
    loop1:
        loadAssetMetadata(
            (loadAssetMetadata_arg *)((u8 *)state + offset),
            state->assetData,
            D_80090C95_91895[(s16)state->animationIndex * 2]
        );
        offset += 0x20;
        i += 1;
        if (i < 5)
            goto loop1;

        if (state->frameTimer == -1) {
            allocation = (Allocation_47D1C *)getCurrentAllocation();
            allocation->unk79 = allocation->unk79 - 1;
            setCallback(updatePlayerHaloDescending);
        }

        if (state->player->unkBB8 != 0) {
            goto position_loop;
        }
        tableIdx = (s16)state->animationIndex * 2;
        if (D_80090C95_91895[tableIdx] == 7) {
            func_80058530_59130(0x116, 6);
        }
        tableIdx = (s16)state->animationIndex * 2;
        if (D_80090C95_91895[tableIdx] == 0xB) {
            func_80058530_59130(0x117, 6);
        }
        i = 0;
        goto position_loop;
    }
    state->frameTimer = frameTimer - 1;

position_loop:
    i = 0;
    heightOffset = 0x200000;
    elem = (PlayerHaloElement *)state;
loop2:
    elem->posX = state->player->worldPos.x;
    elem->posY = state->player->worldPos.y + heightOffset;
    elem->posZ = state->player->worldPos.z;
    i += 1;
    elem += 1;
    if (i < 5)
        goto loop2;

    for (i = 0; i < 2; i++) {
        func_80066444_67044(state->player->unkBB8, (func_80066444_67044_arg1 *)&state->elements[i]);
    }

    for (i = 2; i < 5; i++) {
        func_800670A4_67CA4(state->player->unkBB8, (func_80066444_67044_arg1 *)&state->elements[i]);
    }
}

void updatePlayerHaloDescending(PlayerHaloState *state) {
    Allocation_47D1C *allocation;
    s32 heightOffset;
    s32 sinVal;
    s32 j;
    s32 i;
    PlayerHaloElement *elem;

    allocation = (Allocation_47D1C *)getCurrentAllocation();

    if (allocation->unk76 == 0) {
        state->animationAngle -= 0x40;
        if (state->animationAngle == 0) {
            func_80069CF8_6A8F8();
        }
    }

    sinVal = approximateSin(state->animationAngle);
    heightOffset = ((0x2000 - sinVal) << 10) + 0x200000;
    j = 0;
    elem = (PlayerHaloElement *)state;

loop:
    elem->posX = state->player->worldPos.x;
    elem->posY = state->player->worldPos.y + heightOffset;
    elem->posZ = state->player->worldPos.z;
    j += 1;
    elem += 1;
    if (j < 5)
        goto loop;

    for (i = 0; i < 2; i++) {
        func_80066444_67044(state->player->unkBB8, (func_80066444_67044_arg1 *)&state->elements[i]);
    }

    for (i = 2; i < 5; i++) {
        func_800670A4_67CA4(state->player->unkBB8, (func_80066444_67044_arg1 *)&state->elements[i]);
    }
}

void cleanupPlayerHaloTask(PlayerHaloTask *task) {
    task->assetData = freeNodeMemory(task->assetData);
}

void schedulePlayerHaloTask(Player *player) {
    PlayerHaloTaskParams *task;

    task = (PlayerHaloTaskParams *)scheduleTask(&initPlayerHaloTask, 0, 0, 0xD4);
    if (task != NULL) {
        task->player = player;
    }
}

s32 rollPrimaryItemDrop(Player *arg0, u8 *arg1) {
    s32 i;
    s32 randVal;
    u8 index;
    ItemBoxSystemState *element;

    randVal = getRand(*(arg1 + arg0->finishPosition + 0x24)) & 0xFF;
    index = arg0->finishPosition;

    for (i = 0; i < 5; i++) {
        if (D_80090CA8_918A8[index][i] >= randVal) {
            break;
        }
    }

    element = (ItemBoxSystemState *)(arg1 + index);

    switch (element->primaryDropMode) {
        case 0:
            element->primaryDropSeed = element->primaryDropSeed + 1;
            break;
        case 1:
            element->primaryDropSeed = element->primaryDropSeed - 1;
            break;
        case 2:
            element->primaryDropSeed = element->primaryDropSeed + 3;
            break;
        case 3:
            element->primaryDropSeed = element->primaryDropSeed - 3;
            break;
        default:
            return i + 1;
    }

    return i + 1;
}

s32 rollSecondaryItemDrop(Player *arg0, u8 *arg1) {
    s32 i;
    s32 randVal;
    u8 index;
    SecondaryItemDropElement *element;

    if (arg0->unkB84 & 0x1000000) {
        randVal = randB() & 0xFF;
        for (i = 0; i < 9; i++) {
            if (D_80090CE0_918E0[i] >= randVal) {
                break;
            }
        }
        return i + 1;
    }

    randVal = getRand(*(arg1 + arg0->finishPosition + 0x20)) & 0xFF;
    index = arg0->finishPosition;

    for (i = 0; i < 9; i++) {
        if (D_80090CBC_918BC[index][i] >= randVal) {
            break;
        }
    }

    element = (SecondaryItemDropElement *)(arg1 + index);

    switch (element->unk18) {
        case 0:
            element->unk20 = element->unk20 + 1;
            break;
        case 1:
            element->unk20 = element->unk20 - 1;
            break;
        case 2:
            element->unk20 = element->unk20 + 3;
            break;
        case 3:
            element->unk20 = element->unk20 - 3;
            break;
        default:
            return i + 1;
    }

    return i + 1;
}

void updateItemBox(ItemBox *itemBox, ItemBoxController *controller) {
    Vec3i collisionPos;
    Vec3i *collisionPosPtr;
    GameState *gameState;
    Player *player;
    s32 i;
    s8 state;
    s32 sinResult;

    gameState = (GameState *)getCurrentAllocation();
    state = itemBox->state;

    switch (state) {
        case 0:
            if (gameState->gamePaused == 0) {
                collisionPosPtr = &collisionPos;
                memcpy(collisionPosPtr, &itemBox->displayList.transform.translation, 0xC);
                collisionPos.y += 0x100000;
                player = (Player *)func_8005B24C_5BE4C(collisionPosPtr, -1, 0x100000);
                if (player != NULL) {
                    itemBox->state = itemBox->state + 1;
                    spawnItemBoxBurstEffect(&itemBox->displayList, itemBox->isSecondaryItemBox);
                    if ((player->unkB6C >= 100) || (player->unkBDA != 0)) {
                        func_80059A48_5A648(player, -100);
                        if (itemBox->isSecondaryItemBox != 0) {
                            if (player->unkBD4 == 7) {
                                gameState->availableHomingProjectileSlots =
                                    gameState->availableHomingProjectileSlots + 1;
                            }
                            player->unkBD4 = rollSecondaryItemDrop(player, (u8 *)controller);
                            if (gameState->availableHomingProjectileSlots < 6) {
                                if ((player->unkBD4 & 0xFF) == 7) {
                                    player->unkBD4 = 10;
                                }
                            }
                            if (player->unkBD4 == 7) {
                                gameState->availableHomingProjectileSlots =
                                    gameState->availableHomingProjectileSlots - 1;
                            }
                            player->unkBD8 |= 2;
                        } else {
                            player->unkBD2 = rollPrimaryItemDrop(player, (u8 *)controller);
                            player->unkBD3 = 3;
                            player->unkBD8 |= 1;
                        }
                        queueSoundAtPosition(&itemBox->displayList.transform.translation, 8);
                    } else {
                        queueSoundAtPosition(&itemBox->displayList.transform.translation, 9);
                    }
                } else if (gameState->unk79 == 0) {
                    randB();
                }
            }
            break;
        case 1:
            if (itemBox->rotationAngle != 0x1000) {
                if (gameState->gamePaused == 0) {
                    itemBox->rotationAngle = (itemBox->rotationAngle + 0x100) & 0x1FFF;
                }
                createYRotationMatrix(&itemBox->matrix, itemBox->rotationAngle);
            } else {
                itemBox->state = state + 1;
                itemBox->stateTimer = 4;
            }
            break;
        case 2:
            if (gameState->gamePaused == 0) {
                itemBox->stateTimer = itemBox->stateTimer - 1;
            }
            if (itemBox->stateTimer == 0) {
                itemBox->state = itemBox->state + 1;
            }
            break;
        case 3:
            if (itemBox->rotationAngle != 0) {
                if (gameState->gamePaused == 0) {
                    itemBox->rotationAngle = (itemBox->rotationAngle + 0x100) & 0x1FFF;
                }
                createYRotationMatrix(&itemBox->matrix, itemBox->rotationAngle);
            } else {
                itemBox->state = 0;
                itemBox->displayList.transform.translation.y = itemBox->baseY + 0x300000;
            }
            break;
    }

    if (itemBox->state == 0) {
        if (gameState->gamePaused != 0) {
            i = 0;
        } else if (itemBox->displayList.transform.translation.y > itemBox->baseY) {
            itemBox->displayList.transform.translation.y =
                itemBox->displayList.transform.translation.y + (s32)0xFFFA0000;
            i = 0;
        } else {
            i = 0;
        }
        do {
            enqueueDisplayListWithFrustumCull(i, &itemBox->displayList);
            i++;
        } while (i < 4);
    }

    itemBox->matrix.translation.y = itemBox->baseY;
    sinResult = approximateSin((D_8009ADE0_9B9E0 << 8) & 0x1F00);
    i = 0;
    itemBox->matrix.translation.y += 0x200000 + (sinResult << 5);

    do {
        enqueueBillboardedDisplayListObject(i, (DisplayListObject *)itemBox);
        i++;
    } while (i < 4);
}

void initItemBoxSystem(ItemBoxSystemState *state) {
    s32 i;
    s32 courseID;
    ItemBoxSystemState *ptr;

    state->asset1 = loadAsset_B7E70();
    state->asset2 = loadAsset_216290();

    courseID = state->courseId;

    state->positionData = loadCompressedData(
        D_80090CEC_918EC[courseID].romStart,
        D_80090CEC_918EC[courseID].romEnd,
        D_80090CEC_918EC[courseID].decompressedSize
    );
    state->itemBoxMemory = NULL;

    setCleanupCallback(&cleanupItemBoxSystem);

    i = 0;
loop:
    ptr = (ItemBoxSystemState *)((u8 *)state + i);
    ptr->secondaryDropMode = randA() & 3;
    ptr->primaryDropMode = randA() & 3;
    ptr->secondaryDropSeed = randA();
    ptr->primaryDropSeed = randA();
    i++;
    if (i < 4) {
        goto loop;
    }

    setCallback(&initItemBoxPositions);
}

INCLUDE_ASM("asm/nonmatchings/46080", func_80048F0C_49B0C);

void initItemBoxPositions(ItemBoxSystemState *state) {
    ItemBoxPositionEntry *ptr;
    ItemBoxPositionEntry *base;
    s32 count;
    s32 i;

    state->positionEntries = (ItemBoxPositionEntry *)((s32)state->positionData + state->positionData->offset);
    state->itemBoxCount = 0;
    ptr = state->positionEntries;

    if (ptr->unk0 != -1) {
        base = ptr;
        do {
            state->itemBoxCount++;
        } while (base[state->itemBoxCount].unk0 != -1);
    }

    count = state->itemBoxCount;
    state->itemBoxMemory = allocateNodeMemory(count * 132);

    for (i = 0; i < state->itemBoxCount; i++) {
        func_80048F0C_49B0C(state, i);
    }

    setCallback(&updateAllItemBoxes);
}

void updateAllItemBoxes(ItemBoxController *controller) {
    s32 i;

    for (i = 0; i < controller->itemBoxCount; i++) {
        updateItemBox(&controller->itemBoxes[i], controller);
    }
}

void cleanupItemBoxSystem(ItemBoxSystemState *state) {
    state->asset1 = freeNodeMemory(state->asset1);
    state->asset2 = freeNodeMemory(state->asset2);
    state->itemBoxMemory = freeNodeMemory(state->itemBoxMemory);
    state->positionData = freeNodeMemory(state->positionData);
}

void scheduleItemBoxSystemTask(s32 courseId) {
    ItemBoxSystemTaskParams *task = (ItemBoxSystemTaskParams *)scheduleTask(&initItemBoxSystem, 0, 0, 0xEA);
    if (task != NULL) {
        task->courseId = courseId;
    }
}

void initItemBoxBurstEffect(void **arg0) {
    *arg0 = load_3ECE40();
    setCleanupCallback(&cleanupItemBoxBurstEffect);
    setCallbackWithContinue(&setupItemBoxBurstTexture);
}

void setupItemBoxBurstTexture(ItemBoxBurstEffectState *arg0) {
    u16 index;

    if (arg0->isSecondaryBox == 0) {
        index = 0x33;
    } else {
        index = 0x34;
    }

    getTableEntryByU16Index(arg0->assetTable, index, &arg0->textureEntry);
    arg0->frameIndex = 0;
    setCallbackWithContinue(&updateItemBoxBurstFrame);
}

void updateItemBoxBurstFrame(ItemBoxBurstEffectState *state) {
    GameState *gameState = (GameState *)getCurrentAllocation();
    GameStateUnk44Unk2C0 *frameData;
    s32 i;

    if (state->frameIndex == 8) {
        func_80069CF8_6A8F8();
        return;
    }

    frameData = &gameState->unk44->unk2C0[state->frameIndex];
    state->transformMatrix = 0;
    state->renderEntry = frameData;

    for (i = 0; i < 4; i++) {
        debugEnqueueCallback(i, 1, &renderItemBoxBurstEffect, state);
    }

    if (gameState->gamePaused == 0) {
        state->frameIndex++;
    }
}

void cleanupItemBoxBurstEffect(ItemBoxBurstEffectState *state) {
    state->assetTable = freeNodeMemory(state->assetTable);
}

void renderItemBoxBurstEffect(ItemBoxBurstEffectState *state) {
    s32 dxtBase;
    s32 new_var;
    u32 line;
    s32 lrs;
    u16 dxt;
    u16 widthDiv16;
    Gfx *loadBlockCmd;
    long loadBlockWord;
    volatile u8 padding[0x10];

    gDPPipeSync(gRegionAllocPtr++);

    gDPSetTextureLUT(gRegionAllocPtr++, G_TT_RGBA16);

    gDPSetTextureImage(gRegionAllocPtr++, G_IM_FMT_CI, G_IM_SIZ_16b, 1, state->textureEntry.data_ptr);

    gDPSetTile(
        gRegionAllocPtr++,
        G_IM_FMT_CI,
        G_IM_SIZ_16b,
        0,
        0x0000,
        G_TX_LOADTILE,
        0,
        G_TX_NOMIRROR | G_TX_WRAP,
        G_TX_NOMASK,
        G_TX_NOLOD,
        G_TX_NOMIRROR | G_TX_WRAP,
        G_TX_NOMASK,
        G_TX_NOLOD
    );

    gDPLoadSync(gRegionAllocPtr++);

    loadBlockCmd = gRegionAllocPtr++;
    loadBlockCmd->words.w0 = 0xF3000000;
    gGraphicsMode = -1;
    widthDiv16 = state->textureEntry.field1 >> 4;
    dxtBase = 0x800;
    if (widthDiv16 != 0) {
        dxtBase = widthDiv16 + 0x7FF;
    }
    lrs = (((s32)((state->textureEntry.field1 * state->textureEntry.field2) + 3)) >> 2) - 1;
    if (lrs < 0x800) {
    } else {
        lrs = 0x7FF;
    }
    line = lrs & 0xFFF;
    new_var = (line << 12) | 0x07000000;
    loadBlockWord = new_var;
    if (widthDiv16 != 0) {
        loadBlockWord |= (dxtBase / widthDiv16) & 0xFFF;
    } else {
        loadBlockWord |= dxtBase & 0xFFF;
    }
    loadBlockCmd->words.w1 = loadBlockWord;

    gDPPipeSync(gRegionAllocPtr++);

    line = (((state->textureEntry.field1 >> 1) + 7) >> 3) & 0x1FF;
    new_var = G_TX_NOMIRROR;
    gDPSetTile(gRegionAllocPtr++, G_IM_FMT_CI, G_IM_SIZ_4b, line, 0, G_TX_RENDERTILE, 0, 0, 0, 0, 0, 0, 0);

    gDPSetTileSize(
        gRegionAllocPtr++,
        G_TX_RENDERTILE,
        0,
        0,
        (state->textureEntry.field1 - 1) << 2,
        (state->textureEntry.field2 - 1) << 2
    );

    gDPSetTextureImage(gRegionAllocPtr++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, state->textureEntry.index_ptr);

    gDPTileSync(gRegionAllocPtr++);

    gDPSetTile(
        gRegionAllocPtr++,
        G_IM_FMT_RGBA,
        G_IM_SIZ_4b,
        0,
        0x0100,
        G_TX_LOADTILE,
        0,
        G_TX_NOMIRROR | G_TX_WRAP,
        G_TX_NOMASK,
        G_TX_NOLOD,
        G_TX_NOMIRROR | G_TX_WRAP,
        G_TX_NOMASK,
        G_TX_NOLOD
    );

    gDPLoadSync(gRegionAllocPtr++);

    gDPLoadTLUTCmd(gRegionAllocPtr++, G_TX_LOADTILE, 15);

    gDPPipeSync(gRegionAllocPtr++);

    if (state->transformMatrix == NULL) {
        state->transformMatrix = arenaAlloc16(0x40);
        if (state->transformMatrix == NULL) {
            return;
        }
        func_8006C130_6CD30(&state->transform, state->transformMatrix);
    }

    gDPPipeSync(gRegionAllocPtr++);

    gDPSetTexturePersp(gRegionAllocPtr++, G_TP_PERSP);

    gSPSegment(gRegionAllocPtr++, 0x02, state->renderEntry);

    gSPMatrix(gRegionAllocPtr++, state->transformMatrix, (G_MTX_NOPUSH | G_MTX_LOAD) | G_MTX_MODELVIEW);

    gSPDisplayList(gRegionAllocPtr++, D_80090DB0_919B0);
}

void spawnItemBoxBurstEffect(void *displayList, s32 isSecondaryBox) {
    NodeWithPayload *task = (NodeWithPayload *)scheduleTask(&initItemBoxBurstEffect, 0, 0, 0xEB);
    if (task != NULL) {
        memcpy((void *)((s32)task + 0x10), displayList, 0x20);
        task->unk3C = isSecondaryBox;
    }
}

void scheduleLevelEnvironmentTasks(s32 poolId) {
    s32 temp;

    switch (poolId) {
        case SUNNY_MOUNTAIN:
            scheduleSkyRenderTask(poolId);
            scheduleScrollingSceneryTask(&D_80094DA0_959A0, poolId, 0, 4, 0x12, 0);
            scheduleSceneAnimationTask(poolId, 0);
            break;
        case TURTLE_ISLAND:
            scheduleSkyRenderTask(poolId);
            scheduleScrollingSceneryTask(&D_80094DE0_959E0, poolId, 4, 0, 0, 0);
            temp = 1;
            scheduleScrollingSceneryTask(&D_80094DF0_959F0, poolId, 4, 0, temp, 0);
            scheduleScrollingSceneryTask(&D_80094E00_95A00, poolId, 4, 0, 0, 0);
            scheduleScrollingSceneryTask(&D_80094E10_95A10, poolId, 0, 1, 2, temp);
            break;
        case JINGLE_TOWN:
            scheduleSkyRenderTask(poolId);
            scheduleScrollingSceneryTask(&D_80094EE0_95AE0, poolId, 0, 4, 0, 0);
            scheduleScrollingSceneryTask(&D_80094EF0_95AF0, poolId, 1, 0, 2, 0);
            break;
        case JINGLE_TOWN_BOSS:
            scheduleSkyRenderTask(poolId);
            scheduleScrollingSceneryTask(&D_80094FC0_95BC0, poolId, 0, 4, 0, 0);
            scheduleScrollingSceneryTask(&D_80094FD0_95BD0, poolId, 1, 0, 2, 0);
            scheduleSceneAnimationTask(poolId, 5);
            break;
        case WENDYS_HOUSE:
            scheduleSkyRenderTask(poolId);
            scheduleScrollingSceneryTask(&D_800950B0_95CB0, poolId, -4, 0, 0, 0);
            scheduleSceneAnimationTask(poolId, 0xA);
            break;
        case LINDAS_CASTLE:
            scheduleSkyRenderTask(poolId);
            break;
        case CRAZY_JUNGLE:
            scheduleScrollingSceneryTask(&D_80095360_95F60, poolId, 0, 4, 0, 0);
            scheduleScrollingSceneryTask(&D_80095370_95F70, poolId, 0, 4, 1, 0);
            scheduleScrollingSceneryTask(&D_80095380_95F80, poolId, 4, 0, 2, 0);
            scheduleSkyRenderTask(poolId);
            scheduleSceneAnimationTask(poolId, 1);
            break;
        case CRAZY_JUNGLE_BOSS:
            scheduleScrollingSceneryTask(&D_80095460_96060, poolId, 0, 4, 0, 0);
            scheduleScrollingSceneryTask(&D_80095470_96070, poolId, 0, 4, 1, 0);
            scheduleScrollingSceneryTask(&D_80095480_96080, poolId, 4, 0, 2, 0);
            scheduleSkyRenderTask(poolId);
            scheduleSceneAnimationTask(poolId, 2);
            break;
        case STARLIGHT_HIGHWAY:
            scheduleSkyRenderTask(poolId);
            scheduleScrollingSceneryTask(&D_800955C0_961C0, poolId, 4, 0, 1, 0);
            scheduleScrollingSceneryTask(&D_800955D0_961D0, poolId, 0, 4, 2, 0);
            scheduleScrollingSceneryTask(&D_800955E0_961E0, poolId, 4, 0, 3, 0);
            break;
        case HAUNTED_HOUSE:
            scheduleSkyRenderTask(poolId);
            scheduleSceneAnimationTask(poolId, 0xB);
            break;
        case ICE_LAND:
            scheduleSkyRenderTask(poolId);
            scheduleSceneAnimationTask(poolId, 8);
            break;
        case ICE_LAND_BOSS:
            scheduleSkyRenderTask(poolId);
            scheduleSceneAnimationTask(poolId, 9);
            break;
        case SNOWBOARD_STREET_SPEED_CROSS:
            scheduleSkyRenderTask(poolId);
            scheduleScrollingSceneryTask(&D_80095860_96460, poolId, 0, -4, 0, 0);
            break;
        case SNOWBOARD_STREET_SHOT_CROSS:
            scheduleSkyRenderTask(poolId);
            scheduleScrollingSceneryTask(&D_80095930_96530, poolId, 0, -4, 0, 0);
            break;
        case X_CROSS:
        case TRAINING:
            scheduleSkyRenderTask(poolId);
            scheduleSceneAnimationTask(poolId, 7);
            break;
    }
}

void schedulePlayerSparkleTask(void) {
    GameState *allocation = (GameState *)getCurrentAllocation();
    if (allocation->unk7A == 0) {
        scheduleTask(&initPlayerSparkleTask, 0, 0, 0xD3);
    }
}

void scheduleGoldCoinsIfEnabled(s32 courseIndex) {
    void (*func_ptr)(s32) = (void (*)(s32))scheduleGoldCoinsTask;
    GameState *gameState;
    u8 goldCoinsDisabled;

    gameState = (GameState *)getCurrentAllocation();
    goldCoinsDisabled = gameState->unk83;
    if (goldCoinsDisabled == 0) {
        func_ptr(courseIndex);
    }
}

void scheduleItemBoxSystemTaskIfEnabled(s32 courseIndex) {
    GameState *gameState;
    u8 itemBoxesDisabled;

    gameState = (GameState *)getCurrentAllocation();
    itemBoxesDisabled = gameState->itemBoxesDisabled;
    if (itemBoxesDisabled != 0) {
        return;
    }
    scheduleItemBoxSystemTask(courseIndex);
}

void scheduleCourseTasks(s32 courseId, s32 playerCount) {
    GameState *gs;
    s32 i;

    gs = (GameState *)getCurrentAllocation();

    switch (courseId) {
        case SUNNY_MOUNTAIN:
            for (i = 0; i < playerCount; i++) {
                gs->pendingPlayerRenderTasks = gs->pendingPlayerRenderTasks + 1;
                schedulePlayerRenderTask(i);
            }
            schedulePlayerSparkleTask();
            scheduleTask(&func_800BB2B0, 0, 0, 0xD3);
            scheduleTask(&func_800BB814_B5114, 0, 0, 0xD3);
            scheduleTask(initCourseSceneryTask, 0, 0, 0xD3);
            scheduleTask(&initStartGate, 0, 0, 0xD3);
            scheduleGoldCoinsIfEnabled(courseId);
            scheduleItemBoxSystemTaskIfEnabled(courseId);
            break;

        case TURTLE_ISLAND:
            for (i = 0; i < playerCount; i++) {
                gs->pendingPlayerRenderTasks = gs->pendingPlayerRenderTasks + 1;
                schedulePlayerRenderTask(i);
            }
            scheduleGoldCoinsIfEnabled(courseId);
            scheduleItemBoxSystemTaskIfEnabled(courseId);
            scheduleTask(initCourseSceneryTask, 0, 0, 0xD3);
            scheduleTask(&initStartGate, 0, 0, 0xD3);
            scheduleSceneAnimationTask(courseId, 4);
            schedulePlayerSparkleTask();
            break;

        case JINGLE_TOWN:
            for (i = 0; i < playerCount; i++) {
                gs->pendingPlayerRenderTasks = gs->pendingPlayerRenderTasks + 1;
                schedulePlayerRenderTask(i);
            }
            if (gs->unk7A != 0xB) {
                scheduleGoldCoinsIfEnabled(courseId);
                scheduleItemBoxSystemTaskIfEnabled(courseId);
                func_800BBB34();
                schedulePlayerSparkleTask();
            }
            scheduleSceneAnimationTask(courseId, 3);
            scheduleTask(initCourseSceneryTask, 0, 0, 0xD3);
            scheduleTask(&initStartGate, 0, 0, 0xD3);
            break;

        case JINGLE_TOWN_BOSS:
            for (i = 0; i < playerCount; i++) {
                gs->pendingPlayerRenderTasks = gs->pendingPlayerRenderTasks + 1;
                schedulePlayerRenderTask(i);
            }
            scheduleTask(initCourseSceneryTask, 0, 0, 0xD3);
            if (gs->unk7A == 8) {
                scheduleGoldCoinsIfEnabled(courseId);
                scheduleItemBoxSystemTaskIfEnabled(courseId);
            } else {
                func_80045434_46034(1);
            }
            scheduleTask(&initStartGate, 0, 0, 0xD3);
            break;

        case WENDYS_HOUSE:
            for (i = 0; i < playerCount; i++) {
                gs->pendingPlayerRenderTasks = gs->pendingPlayerRenderTasks + 1;
                schedulePlayerRenderTask(i);
            }
            scheduleTask(&func_800BB2B0, 0, 0, 0x32);
            scheduleTask(initCourseSceneryTask, 0, 0, 0xD3);
            scheduleTask(&func_800BB7F0_B5A00, 0, 0, 0x5E);
            scheduleGoldCoinsIfEnabled(courseId);
            scheduleItemBoxSystemTaskIfEnabled(courseId);
            scheduleTask(&initStartGate, 0, 0, 0xD3);
            break;

        case LINDAS_CASTLE:
            for (i = 0; i < playerCount; i++) {
                gs->pendingPlayerRenderTasks = gs->pendingPlayerRenderTasks + 1;
                schedulePlayerRenderTask(i);
            }
            scheduleGoldCoinsIfEnabled(courseId);
            scheduleItemBoxSystemTaskIfEnabled(courseId);
            scheduleTask(initCourseSceneryTask, 0, 0, 0xD3);
            scheduleSceneAnimationTask(courseId, 6);
            scheduleTask(&initStartGate, 0, 0, 0xD3);
            scheduleTask(&func_800BBA28_AB8D8, 0, 0, 0x31);
            scheduleTask(&func_800BBAF8_AB9A8, 0, 0, 0xF0);
            schedulePlayerSparkleTask();
            break;

        case CRAZY_JUNGLE:
            for (i = 0; i < playerCount; i++) {
                gs->pendingPlayerRenderTasks = gs->pendingPlayerRenderTasks + 1;
                schedulePlayerRenderTask(i);
            }
            spawnPushZone(0);
            spawnPushZone(1);
            scheduleGoldCoinsIfEnabled(courseId);
            scheduleItemBoxSystemTaskIfEnabled(courseId);
            scheduleTask(initCourseSceneryTask, 0, 0, 0xD3);
            func_800BBA98();
            schedulePlayerSparkleTask();
            scheduleTask(&initStartGate, 0, 0, 0xD3);
            break;

        case CRAZY_JUNGLE_BOSS:
            spawnPushZone(2);
            spawnPushZone(3);
            if (gs->unk7A != 8) {
                func_80045434_46034(0);
            } else {
                scheduleGoldCoinsIfEnabled(courseId);
                scheduleItemBoxSystemTaskIfEnabled(courseId);
            }
            scheduleTask(initCourseSceneryTask, 0, 0, 0xD3);
            scheduleTask(&initStartGate, 0, 0, 0xD3);
            break;

        case STARLIGHT_HIGHWAY:
            for (i = 0; i < playerCount; i++) {
                gs->pendingPlayerRenderTasks = gs->pendingPlayerRenderTasks + 1;
                schedulePlayerRenderTask(i);
            }
            scheduleGoldCoinsIfEnabled(courseId);
            scheduleItemBoxSystemTaskIfEnabled(courseId);
            scheduleTask(&initStartGate, 0, 0, 0xD3);
            scheduleTask(&func_800BB2B0, 0, 0, 0xD3);
            func_800BBED8();
            scheduleTask(&func_800BC528_AE8E8, 0, 0, 0xC7);
            func_800BC9BC();
            schedulePlayerSparkleTask();
            break;

        case HAUNTED_HOUSE:
            for (i = 0; i < playerCount; i++) {
                gs->pendingPlayerRenderTasks = gs->pendingPlayerRenderTasks + 1;
                schedulePlayerRenderTask(i);
            }
            scheduleGoldCoinsIfEnabled(courseId);
            scheduleItemBoxSystemTaskIfEnabled(courseId);
            scheduleTask(&func_800BB74C_AF43C, 0, 0, 0xC8);
            scheduleTask(initCourseSceneryTask, 0, 0, 0xD3);
            scheduleTask(&initStartGate, 0, 0, 0xD3);
            scheduleTask(&func_800BB8E8_AF5D8, 0, 0, 0x32);
            scheduleTask(&func_800BBE84_AFB74, 0, 0, 0x32);
            spawnPushZone(6);
            spawnPushZone(7);
            func_800BC0FC(0);
            func_800BC0FC(1);
            func_800BC0FC(2);
            func_800BC0FC(3);
            func_800BC0FC(4);
            scheduleTask(&func_800BC13C_AFE2C, 0, 0, 0xD3);
            scheduleTask(&func_800BC72C_B041C, 0, 0, 0xD3);
            schedulePlayerSparkleTask();
            break;

        case ICE_LAND:
            for (i = 0; i < playerCount; i++) {
                gs->pendingPlayerRenderTasks = gs->pendingPlayerRenderTasks + 1;
                schedulePlayerRenderTask(i);
            }
            spawnPushZone(4);
            scheduleTask(initCourseSceneryTask, 0, 0, 0xD3);
            scheduleTask(&initStartGate, 0, 0, 0xD3);
            scheduleTask(&func_800BB688_B2408, 0, 0, 0xD3);
            scheduleGoldCoinsIfEnabled(courseId);
            scheduleItemBoxSystemTaskIfEnabled(courseId);
            break;

        case ICE_LAND_BOSS:
            for (i = 0; i < playerCount; i++) {
                gs->pendingPlayerRenderTasks = gs->pendingPlayerRenderTasks + 1;
                schedulePlayerRenderTask(i);
            }
            if (gs->unk7A == 8) {
                scheduleGoldCoinsIfEnabled(courseId);
                scheduleItemBoxSystemTaskIfEnabled(courseId);
            } else {
                func_80045434_46034(2);
            }
            scheduleTask(initCourseSceneryTask, 0, 0, 0xD3);
            scheduleTask(&initStartGate, 0, 0, 0xD3);
            func_80041418_42018();
            spawnPushZone(5);
            break;

        case SNOWBOARD_STREET_SPEED_CROSS:
            for (i = 0; i < playerCount; i++) {
                gs->pendingPlayerRenderTasks = gs->pendingPlayerRenderTasks + 1;
                schedulePlayerRenderTask(i);
            }
            if (gs->unk7A == 8) {
                scheduleGoldCoinsIfEnabled(courseId);
                scheduleItemBoxSystemTaskIfEnabled(courseId);
            } else {
                func_80045434_46034(3);
            }
            scheduleTask(initCourseSceneryTask, 0, 0, 0xD3);
            scheduleTask(&initStartGate, 0, 0, 0xD3);
            scheduleTask(&func_800BB2B0, 0, 0, 0xD3);
            break;

        case SNOWBOARD_STREET_SHOT_CROSS:
            for (i = 0; i < playerCount; i++) {
                gs->pendingPlayerRenderTasks = gs->pendingPlayerRenderTasks + 1;
                schedulePlayerRenderTask(i);
            }
            if (gs->unk7A == 8) {
                scheduleGoldCoinsIfEnabled(courseId);
                scheduleItemBoxSystemTaskIfEnabled(courseId);
            } else {
                func_800BBA50(courseId);
            }
            scheduleTask(initCourseSceneryTask, 0, 0, 0xD3);
            scheduleTask(&initStartGate, 0, 0, 0xD3);
            scheduleTask(&func_800BBA90_AD510, 0, 0, 0xD3);
            break;

        case X_CROSS:
            for (i = 0; i < playerCount; i++) {
                gs->pendingPlayerRenderTasks = gs->pendingPlayerRenderTasks + 1;
                schedulePlayerRenderTask(i);
            }
            if (gs->unk7A == 8) {
                scheduleGoldCoinsIfEnabled(courseId);
                scheduleItemBoxSystemTaskIfEnabled(courseId);
            }
            scheduleTask(initCourseSceneryTask, 0, 0, 0xD3);
            scheduleTask(&initStartGate, 0, 0, 0xD3);
            break;

        case TRAINING:
            for (i = 0; i < playerCount; i++) {
                gs->pendingPlayerRenderTasks = gs->pendingPlayerRenderTasks + 1;
                schedulePlayerRenderTask(i);
            }
            scheduleGoldCoinsIfEnabled(courseId);
            scheduleItemBoxSystemTaskIfEnabled(courseId);
            scheduleTask(initCourseSceneryTask, 0, 0, 0xD3);
            scheduleTask(&initStartGate, 0, 0, 0xD3);
            break;
    }
}

void initHomingProjectileTask(void **arg0) {
    *arg0 = loadCompressedData(&_3F6670_ROM_START, &_3F6670_ROM_END, 0x388);
    setCleanupCallback(&cleanupHomingProjectileTask);
    setCallbackWithContinue(&initHomingProjectileMovement);
}

void initHomingProjectileMovement(HomingProjectileInitArg *arg0) {
    Vec3i rotatedVector;
    GameState *allocation;
    void *temp;
    u8 randomValue;

    allocation = (GameState *)getCurrentAllocation();
    arg0->animFrame = 0;

    temp = (void *)((s32)allocation->unk44 + 0x80);
    arg0->metadataPtr = temp;

    loadAssetMetadata((void *)((s32)arg0 + 4), arg0->assetData, arg0->animFrame);

    randomValue = randA();
    rotateVectorY(&itemHomingProjectileBaseVector, (randomValue & 0xFF) << 5, &rotatedVector);

    arg0->velX += rotatedVector.x;
    arg0->velY += rotatedVector.y;
    arg0->velZ += rotatedVector.z;

    setCallbackWithContinue(updateHomingProjectileMovement);
}

void updateHomingProjectileMovement(HomingProjectileUpdateArg *arg0) {
    Vec3i sp;
    GameState_46080 *s0;
    void *temp_s0;
    void *s2;
    s32 i;

    s0 = (GameState_46080 *)getCurrentAllocation();
    if (s0->unk76 == 0) {
        arg0->posX = arg0->posX + arg0->velX;
        arg0->posY = arg0->posY + arg0->velY;
        arg0->posZ = arg0->posZ + arg0->velZ;
        arg0->velY = arg0->velY + 0xFFFF0000;

        temp_s0 = &s0->unk30;
        s2 = &arg0->posX;

        arg0->sectorIndex = func_80060A3C_6163C(temp_s0, arg0->sectorIndex, s2);

        func_80060CDC_618DC(temp_s0, arg0->sectorIndex, s2, 0x100000, &sp);

        arg0->posX = arg0->posX + sp.x;
        arg0->posZ = arg0->posZ + sp.z;

        sp.y = func_80061A64_62664(temp_s0, arg0->sectorIndex, s2);

        if (arg0->posY < sp.y) {
            arg0->posY = sp.y;
            arg0->timer = 0x5A;
            setCallback(&updateHomingProjectileImpact);
        }

        if (D_8009ADE0_9B9E0 & 1) {
            arg0->animFrame = arg0->animFrame + 1;
            if ((s16)arg0->animFrame >= 6) {
                arg0->animFrame = 0;
            }
            loadAssetMetadata((loadAssetMetadata_arg *)&arg0->metadata, arg0->assetData, (s16)arg0->animFrame);
        }
    }
    for (i = 0; i < 4; i++) {
        func_80066444_67044(i, (func_80066444_67044_arg1 *)&arg0->metadata);
    }
}

void updateHomingProjectileImpact(HomingProjectileImpactArg *arg0) {
    GameState *allocation;
    void *s2;
    void *s0;
    s32 i;

    allocation = (GameState *)getCurrentAllocation();
    if (allocation->gamePaused == 0) {
        arg0->timer--;
    }

    if (arg0->timer == 0) {
        func_80069CF8_6A8F8();
    }

    s2 = (void *)((s32)arg0 + 8);
    s0 = func_8005B24C_5BE4C(s2, -1, 0x100000);

    if (s0 != NULL) {
        queueSoundAtPosition(s2, 7);
        func_80059A48_5A648(s0, 100);
        func_80069CF8_6A8F8();
    }

    if (D_8009ADE0_9B9E0 & 1) {
        arg0->animFrame++;
        if (arg0->animFrame >= 6) {
            arg0->animFrame = 0;
        }
        loadAssetMetadata(&arg0->metadata, arg0->assetData, arg0->animFrame);
    }

    if (arg0->timer >= 0x1F || (gFrameCounter & 1)) {
        for (i = 0; i < 4; i++) {
            func_80066444_67044(i, (func_80066444_67044_arg1 *)&arg0->metadata);
        }
    }
}

void cleanupHomingProjectileTask(s32 **arg0) {
    GameState *temp_v0;

    temp_v0 = (GameState *)getCurrentAllocation();
    temp_v0->availableHomingProjectileSlots += 1;
    *arg0 = freeNodeMemory(*arg0);
}

void spawnHomingProjectile(void *arg0, s32 arg1, void *arg2) {
    GameState *gs;
    HomingProjectileTask *node;

    gs = (GameState *)getCurrentAllocation();
    if (gs->availableHomingProjectileSlots != 0) {
        node = (HomingProjectileTask *)scheduleTask(&initHomingProjectileTask, 3U, 0U, 0xEFU);
        if (node != NULL) {
            gs->availableHomingProjectileSlots -= 1;
            memcpy(&node->pos, arg0, 0xC);
            memcpy(&node->vel, arg2, 0xC);
            node->playerIndex = arg1;
        }
    }
}

void initPanelProjectileTask(s32 *arg0) {
    *arg0 = (s32)load_3ECE40();
    setCleanupCallback(cleanupPanelProjectileTask);
    setCallbackWithContinue(&initPanelProjectileMovement);
}

void initPanelProjectileMovement(PanelProjectileInitArg *arg0) {
    GameState *gs;
    Player *player;
    Vec3i *temp_s1;
    s32 pad[4];

    gs = (GameState *)getCurrentAllocation();
    arg0->metadata.unk0 = (loadAssetMetadata_arg *)((u8 *)gs->unk44 + 0xEC0);
    loadAssetMetadata(&arg0->metadata, arg0->assetTable, 0x3F);
    player = arg0->player;
    temp_s1 = &arg0->metadata.position;
    memcpy(temp_s1, &player->unk31C, sizeof(Vec3i));
    arg0->sectorIndex = arg0->player->unkB94;
    arg0->playerIndex = (s16)arg0->player->unkBB8;
    memcpy(&arg0->velX, &arg0->player->velocity, sizeof(Vec3i));
    arg0->initFlag = 1;
    arg0->velY = arg0->velY + 0x28000;
    queueSoundAtPosition(temp_s1, 0x17);
    setCallbackWithContinue(updatePanelProjectileMovement);
}

void updatePanelProjectileMovement(PanelProjectileUpdateArg *arg0) {
    Vec3i sp;
    GameState_46080 *s0;
    void *temp_s0;
    Vec3i *s2;
    s32 temp_a0;
    s32 temp_a1;
    s32 temp_v1;
    s32 var_v0;
    s32 var_v0_2;
    void *temp_v0_3;
    s32 i;

    s0 = (GameState_46080 *)getCurrentAllocation();
    if (s0->unk76 == 0) {
        if (arg0->initFlag != 0) {
            arg0->initFlag = 0;
        } else {
            temp_a0 = arg0->velX;
            var_v0 = temp_a0;
            if (temp_a0 < 0) {
                var_v0 = temp_a0 + 7;
            }
            temp_v1 = arg0->velY;
            temp_a1 = arg0->velZ;
            arg0->velX = temp_a0 - (var_v0 >> 3);
            var_v0_2 = temp_a1;
            arg0->velY = temp_v1 - 0x3000;
            if (temp_a1 < 0) {
                var_v0_2 = temp_a1 + 7;
            }
            arg0->velZ = temp_a1 - (var_v0_2 >> 3);
            arg0->position.x = arg0->position.x + arg0->velX;
            arg0->position.y = arg0->position.y + arg0->velY;
            arg0->position.z = arg0->position.z + arg0->velZ;
        }
        temp_s0 = &s0->unk30;
        s2 = &arg0->position;
        arg0->sectorIndex = func_80060A3C_6163C(temp_s0, arg0->sectorIndex, s2);
        func_80060CDC_618DC(temp_s0, arg0->sectorIndex, s2, 0x100000, &sp);
        arg0->position.x = arg0->position.x + sp.x;
        arg0->position.z = arg0->position.z + sp.z;
        sp.y = func_80061A64_62664(temp_s0, arg0->sectorIndex, s2);
        temp_v0_3 = func_8005B24C_5BE4C(s2, arg0->playerIndex, 0xA0000);
        if (temp_v0_3 != NULL) {
            func_80050ECC_51ACC(s2);
            queueSoundAtPosition(s2, 0xD);
            setPlayerProjectileHitState(temp_v0_3);
            func_80069CF8_6A8F8();
            return;
        }
        if (arg0->position.y < sp.y) {
            arg0->position.y = sp.y;
            arg0->timer = 0x1518;
            setCallback(updatePanelProjectileImpact);
        }
    }
    for (i = 0; i < 4; i++) {
        func_80066444_67044(i, (func_80066444_67044_arg1 *)&arg0->metadataPad);
    }
}

void updatePanelProjectileImpact(PanelProjectileImpactArg *arg0) {
    Vec3i sp10;
    Vec3i *s0;
    Vec3i *s2;
    void *s3;
    s32 i;
    allocation_46080 *alloc;

    alloc = (allocation_46080 *)getCurrentAllocation();

    if (alloc->unk76 == 0) {
        if (alloc->availableHomingProjectileSlots < 10) {
            if (arg0->metadata.unk36 < 0x1194) {
                if (arg0->metadata.unk36 >= 0x1F) {
                    arg0->metadata.unk36 = 0x1E;
                }
            }
        }
        arg0->metadata.unk36--;
    }

    s2 = &arg0->metadata.unk4;

    if (arg0->metadata.unk36 == 0) {
        func_80069CF8_6A8F8();
    }

    s3 = func_8005B24C_5BE4C(s2, -1, 0xA0000);
    s0 = &sp10;

    if (s3 != NULL) {
        memcpy(s0, s2, 0xC);
        sp10.y = sp10.y + 0xA0000;
        func_80050ECC_51ACC(s0);
        queueSoundAtPosition(s0, 0xD);
        setPlayerProjectileHitState(s3);
        func_80069CF8_6A8F8();
    } else {
        i = 0;
        if (arg0->metadata.unk36 < 0x1F) {
            if ((gFrameCounter & 1) == 0) {
                return;
            }
        }
        for (i = 0; i < 4; i++) {
            func_80066444_67044(i, &arg0->metadata);
        }
    }
}

void cleanupPanelProjectileTask(s32 **arg0) {
    GameState *temp_v0;

    temp_v0 = (GameState *)getCurrentAllocation();
    temp_v0->availableHomingProjectileSlots += 1;
    *arg0 = freeNodeMemory(*arg0);
}

void spawnPanelProjectile(s32 arg0) {
    NodeWithPayload *temp_v0;

    getCurrentAllocation();
    temp_v0 = (NodeWithPayload *)scheduleTask(&initPanelProjectileTask, 3, 0, 0xEFU);
    if (temp_v0 != NULL) {
        temp_v0->unk30 = arg0;
    }
}

void initItemHomingProjectileTask(void **arg0) {
    *arg0 = loadCompressedData(&_3F3EF0_ROM_START, &_3F3EF0_ROM_END, 0x2608);
    setCleanupCallback(&cleanupItemHomingProjectileTask);
    setCallbackWithContinue(&initItemHomingProjectileMovement);
}

void initItemHomingProjectileMovement(ItemHomingProjectileInitArg *arg0) {
    GameState *allocation;
    Vec3i rotatedVector;
    u8 randomValue;

    allocation = (GameState *)getCurrentAllocation();

    if (arg0->targetVariant == 0) {
        arg0->animFrame = arg0->targetPlayerIndex;
    } else {
        arg0->animFrame = arg0->targetPlayerIndex + 7;
    }

    arg0->metadataPtr = (void *)((s32)allocation->unk44 + 0xF80);
    loadAssetMetadata((void *)((s32)arg0 + 4), arg0->assetData, arg0->animFrame);

    randomValue = randA();
    rotateVectorY(&itemHomingProjectileBaseVector, (randomValue & 0xFF) << 5, &rotatedVector);

    arg0->velOffsetX += rotatedVector.x;
    arg0->velOffsetY += rotatedVector.y;
    arg0->velOffsetZ += rotatedVector.z;

    queueSoundAtPosition((Vec3i *)((s32)arg0 + 8), 0x17);
    setCallbackWithContinue(&updateItemHomingProjectileMovement);
}

void updateItemHomingProjectileMovement(ItemHomingProjectileMoveArg *arg0) {
    GameState_46080 *allocation;
    void *allocPlus30;
    s32 *argPlus8;
    Vec3i sp18;
    u16 tempU16;
    s32 tempS32;
    s32 i;

    allocation = (GameState_46080 *)getCurrentAllocation();
    i = 0;

    if (allocation->unk76 == 0) {
        allocPlus30 = &allocation->unk30;

        arg0->posX = arg0->posX + arg0->velX;
        arg0->posY = arg0->posY + arg0->velY;
        arg0->posZ = arg0->posZ + arg0->velZ;
        arg0->velY = arg0->velY + (s32)0xFFFF0000;

        argPlus8 = &arg0->posX;

        tempU16 = func_80060A3C_6163C(allocPlus30, arg0->sectorIndex, argPlus8);
        arg0->sectorIndex = tempU16;

        func_80060CDC_618DC(allocPlus30, tempU16, argPlus8, 0x100000, &sp18);

        arg0->posX = arg0->posX + sp18.x;
        arg0->posZ = arg0->posZ + sp18.z;

        tempS32 = func_80061A64_62664(allocPlus30, arg0->sectorIndex, argPlus8);
        sp18.y = tempS32;

        if (arg0->posY < tempS32) {
            arg0->posY = tempS32;
            arg0->impactTimer = 0x5A;
            setCallback(updateItemHomingProjectileImpact);
        }
        i = 0;
    }

    for (i = 0; i < 4; i++) {
        func_80066444_67044(i, (func_80066444_67044_arg1 *)&arg0->metadataPtr);
    }
}

void updateItemHomingProjectileImpact(ItemHomingProjectileImpactArg *arg0) {
    allocation_46080 *allocation;
    Player *player;
    s32 i;

    allocation = (allocation_46080 *)getCurrentAllocation();

    if (allocation->unk76 == 0) {
        arg0->impactTimer--;
    }

    if (arg0->impactTimer == 0) {
        if (arg0->targetVariant == 1) {
            if (arg0->targetPlayerIndex == 7) {
                allocation->availableHomingProjectileSlots--;
            }
        }
        func_80069CF8_6A8F8();
        return;
    }

    player = (Player *)func_8005B24C_5BE4C((void *)&arg0->metadataPtr + 4, -1, 0x100000);

    if (player != NULL) {
        if (arg0->targetVariant == 0) {
            u16 val = (u16)arg0->targetPlayerIndex;
            u8 tmp = player->unkBD8 | 1;
            player->unkBD3 = 3;
            player->unkBD2 = (u8)val;
            player->unkBD8 = tmp;
        } else {
            player->unkBD4 = (u8)arg0->targetPlayerIndex;
            player->unkBD8 |= 2;
        }
        queueSoundAtPosition((void *)&arg0->metadataPtr + 4, 8);
        func_80069CF8_6A8F8();
    }

    if (arg0->impactTimer < 0x1F) {
        i = 0;
        if ((gFrameCounter & 1) == 0) {
            return;
        }
    }

    for (i = 0; i < 4; i++) {
        func_80066444_67044(i, (func_80066444_67044_arg1 *)&arg0->metadataPtr);
    }
}

void cleanupItemHomingProjectileTask(func_8004B264_4BE64_arg *arg0) {
    GameState *allocation;

    allocation = (GameState *)getCurrentAllocation();
    allocation->availableHomingProjectileSlots++;
    arg0->unk0 = freeNodeMemory(arg0->unk0);
}

void *spawnItemHomingProjectile(void *arg0, u32 arg1, void *arg2, s16 arg3, s32 arg4) {
    GameState *allocation;
    void *task;

    allocation = (GameState *)getCurrentAllocation();

    if (allocation->availableHomingProjectileSlots == 0) {
        goto end;
    }

    task = scheduleTask(initItemHomingProjectileTask, 3, 0, 0xEF);

    if (task == NULL) {
        goto end;
    }

    allocation->availableHomingProjectileSlots--;

    memcpy((u8 *)task + 8, arg0, 0xC);
    memcpy((u8 *)task + 0x24, arg2, 0xC);

    *(s16 *)((u8 *)task + 0x30) = (s16)arg1;
    *(s16 *)((u8 *)task + 0x38) = arg3;
    *(s16 *)((u8 *)task + 0x36) = arg4;

end:
    return task;
}

void initBossHomingProjectileTask(BossHomingProjectileTask *arg0) {
    arg0->projectileAsset = loadAssetByIndex_95470(9);
    setCleanupCallback(&cleanupBossHomingProjectileTask);
    setCallbackWithContinue(&spawnBossHomingProjectile);
}

void spawnBossHomingProjectile(BossHomingProjectileSpawnArg *arg0) {
    GameState *gameState;
    s32 pad[4];
    s32 rotationAngle;
    s16 randomOffset;
    Vec3i *position;
    BossEntity *boss;

    gameState = (GameState *)getCurrentAllocation();
    arg0->unk4 = (void *)((s32)gameState->unk44 + 0x1300);

    loadAssetMetadata((loadAssetMetadata_arg *)&arg0->unk4, arg0->projectileAsset, 3);

    boss = arg0->boss;
    position = &arg0->position;
    memcpy(position, (u8 *)boss + 0x358, sizeof(Vec3i));

    arg0->position.y = arg0->position.y + (s32)0xFFF10000;

    arg0->sectorIndex = arg0->boss->unkB94;
    arg0->playerIndex = arg0->boss->unkBB8;

    randomOffset = randA() & 0xFF;
    rotationAngle = ((randomOffset - 0x80) * 6) - -0x1000;

    rotateVectorY(&bossHomingProjectileBaseVector, arg0->boss->unkA94 + rotationAngle, &arg0->velocity);

    arg0->velocity.x = arg0->velocity.x + arg0->boss->velocity;
    arg0->velocity.y = arg0->velocity.y + arg0->boss->unk450;
    arg0->velocity.z = arg0->velocity.z + arg0->boss->unk454;

    queueSoundAtPosition(position, 0x1F);

    setCallbackWithContinue(&updateBossHomingProjectile);
}

void updateBossHomingProjectile(BossHomingProjectile *projectile) {
    Vec3i groundOffset;
    GameState_46080 *gameState;
    Vec3i *position;
    s32 i;
    Player *hitPlayer;

    gameState = getCurrentAllocation();

    if (gameState->unk76 != 0) {
        gameState = NULL;
        goto exit_loop;
    }

    position = &projectile->position;

    projectile->velocityY -= 0x4000;
    projectile->position.x += projectile->velocityX;
    projectile->position.y += projectile->velocityY;
    projectile->position.z += projectile->velocityZ;

    projectile->sectorIndex = func_80060A3C_6163C(&gameState->unk30, projectile->sectorIndex, position);

    func_80060CDC_618DC(&gameState->unk30, projectile->sectorIndex, position, 0x100000, &groundOffset);

    projectile->position.x += groundOffset.x;
    projectile->position.z += groundOffset.z;

    groundOffset.y = func_80061A64_62664(&gameState->unk30, projectile->sectorIndex, position);

    hitPlayer = func_8005B24C_5BE4C(position, projectile->playerIndex, 0xA0000);

    if (hitPlayer != NULL) {
        func_80050ECC_51ACC(position);
        queueSoundAtPosition(position, 0xD);
        setPlayerProjectileHitState(hitPlayer);
        func_80069CF8_6A8F8();
        return;
    }

    if (projectile->position.y < groundOffset.y) {
        projectile->position.y = groundOffset.y;
        projectile->bounceTimer = 0x1E0;
        setCallback(&bounceBossHomingProjectile);
    }

    gameState = NULL;

exit_loop:
    for (i = 0; i < 4; i++) {
        func_80066444_67044(i, (func_80066444_67044_arg1 *)&projectile->metadata);
    }
}

void bounceBossHomingProjectile(BossHomingProjectileBounceArg *arg0) {
    Vec3i bouncePosition;
    Vec3i *copyPtr;
    Vec3i *position;
    Player *hitPlayer;
    s32 i;

    if (((GameState *)getCurrentAllocation())->gamePaused == 0) {
        arg0->displayListState.unk36--;
    }

    position = &arg0->displayListState.unk4;

    if (arg0->displayListState.unk36 == 0) {
        func_80069CF8_6A8F8();
    }

    hitPlayer = func_8005B24C_5BE4C(position, -1, 0xA0000);
    copyPtr = &bouncePosition;

    if (hitPlayer != NULL) {
        memcpy(copyPtr, position, sizeof(Vec3i));
        bouncePosition.y = bouncePosition.y + 0xA0000;
        func_80050ECC_51ACC(copyPtr);
        queueSoundAtPosition(copyPtr, 0xD);
        setPlayerProjectileHitState(hitPlayer);
        func_80069CF8_6A8F8();
    } else {
        if (arg0->displayListState.unk36 < 0x1F) {
            i = 0;
            if ((gFrameCounter & 1) == 0) {
                return;
            }
        }
        for (i = 0; i < 4; i++) {
            func_80066444_67044(i, &arg0->displayListState);
        }
    }
}

void cleanupBossHomingProjectileTask(BossHomingProjectileCleanupArg *arg0) {
    GameState *gameState;

    gameState = (GameState *)getCurrentAllocation();
    gameState->availableHomingProjectileSlots++;
    arg0->projectileAsset = freeNodeMemory(arg0->projectileAsset);
}

void spawnBossHomingProjectileTask(void *arg0) {
    GameState *allocation;
    BossHomingProjectileSpawnTask *task;

    allocation = getCurrentAllocation();
    task = (BossHomingProjectileSpawnTask *)scheduleTask(&initBossHomingProjectileTask, 3, 0, 0xEF);

    if (task != NULL) {
        task->player = arg0;
        allocation->availableHomingProjectileSlots -= 1;
    }
}

void func_8004B7F4_4C3F4(MemoryAllocatorNode **arg0) {
    *arg0 = load_3ECE40();
    setCleanupCallback(&func_8004BC20_4C820);
    setCallbackWithContinue(&func_8004B834_4C434);
}

void func_8004B834_4C434(BossHomingProjectileSpawnArg *arg0) {
    GameState *allocation;
    u8 randomValue;
    s32 rotationAngle;
    u8 randomValue3;
    u8 randomValue4;
    s32 *addr;
    s32 pad[4];

    allocation = (GameState *)getCurrentAllocation();
    arg0->unk4 = (void *)((s32)allocation->unk44 + 0x1400);

    randomValue = randA();
    loadAssetMetadata((loadAssetMetadata_arg *)&arg0->unk4, arg0->projectileAsset, (randomValue % 3) + 0x6B);

    memcpy(&arg0->position.x, (void *)((s32)arg0->boss + 0x4C), 0xC);

    arg0->position.y = arg0->position.y + (s32)0xFFF10000;

    arg0->sectorIndex = arg0->boss->unkB94;
    arg0->playerIndex = arg0->boss->unkBB8;

    randomValue = randA();
    rotationAngle = ((randomValue & 0xFF) << 5) + arg0->boss->unkA94;

    randomValue3 = randA();
    addr = &D_80090E40_91A40;
    *addr = (randomValue3 & 0xFF) * 7 * 256;
    rotateVectorY(addr - 2, rotationAngle, &arg0->velocity);

    arg0->velocity.x = arg0->velocity.x + arg0->boss->velocity;

    randomValue4 = randA();
    arg0->velocity.y = arg0->velocity.y + (arg0->boss->unk450 + (((randomValue4 & 0xFF) * 5) << 9));
    arg0->velocity.z = arg0->velocity.z + arg0->boss->unk454;

    setCallbackWithContinue(&func_8004B990_4C590);
}

void func_8004B990_4C590(func_8004B990_4C590_arg *arg0) {
    Vec3i sp;
    GameState_46080 *alloc;
    void *s0;
    Vec3i *s2;
    void *temp_s0;
    s32 i;

    alloc = (GameState_46080 *)getCurrentAllocation();

    if (alloc->unk76 != 0) {
        s0 = NULL;
    } else {
        s0 = &alloc->unk30;
        s2 = &arg0->unk4.unk4;

        arg0->unk4.unk28 -= 0x6000;
        arg0->unk4.unk4.x += arg0->unk4.unk24;
        arg0->unk4.unk4.y += arg0->unk4.unk28;
        arg0->unk4.unk4.z += arg0->unk4.unk2C;

        arg0->unk4.unk30 = func_80060A3C_6163C(s0, arg0->unk4.unk30, s2);

        func_80060CDC_618DC(s0, arg0->unk4.unk30, s2, 0x100000, &sp);

        arg0->unk4.unk4.x += sp.x;
        arg0->unk4.unk4.z += sp.z;

        sp.y = func_80061A64_62664(s0, arg0->unk4.unk30, s2);

        temp_s0 = func_8005B24C_5BE4C(s2, arg0->unk38, 0xA0000);

        if (temp_s0 != NULL) {
            func_80050ECC_51ACC(s2);
            queueSoundAtPosition(s2, 0xD);
            setPlayerProjectileHitState(temp_s0);
            func_80069CF8_6A8F8();
            return;
        }

        if (arg0->unk4.unk4.y < sp.y) {
            arg0->unk4.unk4.y = sp.y;
            arg0->unk4.unk36 = 0x96;
            setCallback(&func_8004BB0C_4C70C);
        }

        s0 = NULL;
    }

    for (i = 0; i < 4; i++) {
        func_80066444_67044(i, &arg0->unk4);
    }
}

void func_8004BB0C_4C70C(func_8004BB0C_4C70C_arg *arg0) {
    Vec3i sp10;
    Vec3i *s0;
    Vec3i *s2;
    void *s3;
    s32 i;

    if (((GameState *)getCurrentAllocation())->gamePaused == 0) {
        arg0->unk4.unk36--;
    }

    if (arg0->unk4.unk36 == 0) {
        func_80069CF8_6A8F8();
    }

    s2 = &arg0->unk4.unk4;
    s3 = func_8005B24C_5BE4C(s2, arg0->unk38, 0xA0000);
    s0 = &sp10;

    if (s3 != NULL) {
        memcpy(s0, s2, 0xC);
        sp10.y = sp10.y + 0xA0000;
        func_80050ECC_51ACC(s0);
        queueSoundAtPosition(s0, 0xD);
        setPlayerProjectileHitState(s3);
        func_80069CF8_6A8F8();
    } else {
        if (arg0->unk4.unk36 < 0x1F) {
            i = 0;
            if ((gFrameCounter & 1) == 0) {
                return;
            }
        }
        for (i = 0; i < 4; i++) {
            func_80066444_67044(i, &arg0->unk4);
        }
    }
}

void func_8004BC20_4C820(func_8004BC20_4C820_arg *arg0) {
    GameState *allocation = (GameState *)getCurrentAllocation();
    allocation->availableHomingProjectileSlots++;
    arg0->unk0 = freeNodeMemory(arg0->unk0);
}

void func_8004BC5C_4C85C(Player *arg0) {
    GameState *allocation;
    func_8004BC5C_4C85C_Task *task;

    allocation = (GameState *)getCurrentAllocation();
    task = (func_8004BC5C_4C85C_Task *)scheduleTask(func_8004B7F4_4C3F4, 3, 0, 0xEF);

    if (task != NULL) {
        task->unk24 = arg0;
        allocation->availableHomingProjectileSlots -= 1;
    }
}

void func_8004BCBC_4C8BC(MemoryAllocatorNode **arg0) {
    *arg0 = load_3ECE40();
    setCleanupCallback(&func_8004C0D0_4CCD0);
    setCallbackWithContinue(&func_8004BCFC_4C8FC);
}

void func_8004BCFC_4C8FC(BossHomingProjectileSpawnArg *arg0) {
    GameState *allocation;
    Vec3i *temp_s2;
    s32 rotationAngle;
    s32 *addr;
    s32 pad[4];

    allocation = (GameState *)getCurrentAllocation();
    arg0->unk4 = (void *)((s32)allocation->unk44 + 0xEC0);
    loadAssetMetadata((loadAssetMetadata_arg *)&arg0->unk4, arg0->projectileAsset, 0x3F);
    arg0->sectorIndex = arg0->boss->unkB94;
    temp_s2 = (Vec3i *)((s32)arg0 + 8);
    transformVector(&D_80090E50_91A50, (s16 *)((s32)arg0->boss + 0x164), temp_s2);
    arg0->playerIndex = arg0->boss->unkBB8;
    rotationAngle = ((randA() & 0xFF) << 5) + arg0->boss->unkA94;
    addr = &D_80090E4C_91A4C;
    *addr = (randA() & 0xFF) * 0x580;
    rotateVectorY(addr - 2, rotationAngle, (Vec3i *)((s32)arg0 + 0x28));
    arg0->velocity.x = arg0->velocity.x + arg0->boss->velocity;
    arg0->velocity.y = arg0->velocity.y + (arg0->boss->unk450 + ((randA() & 0xFF) * 0x600));
    arg0->velocity.z = arg0->velocity.z + arg0->boss->unk454;
    queueSoundAtPosition(temp_s2, 0x17);
    setCallbackWithContinue(&func_8004BE40_4CA40);
}

void func_8004BE40_4CA40(func_8004BE40_4CA40_arg *arg0) {
    Vec3i sp;
    GameState_46080 *s0;
    Vec3i *s2;
    s32 i;
    void *temp_s0;

    s0 = getCurrentAllocation();

    if (s0->unk76 != 0) {
        i = 0;
        goto exit_loop;
    }

    s2 = &arg0->unk8;

    arg0->unk2C -= 0x4000;
    arg0->unk8.x += arg0->unk28;
    arg0->unk8.y += arg0->unk2C;
    arg0->unk8.z += arg0->unk30;

    arg0->unk34 = func_80060A3C_6163C(s0->unk30, arg0->unk34, s2);

    func_80060CDC_618DC(s0->unk30, arg0->unk34, s2, 0x100000, &sp);

    arg0->unk8.x += sp.x;
    arg0->unk8.z += sp.z;

    sp.y = func_80061A64_62664(s0->unk30, arg0->unk34, s2);

    temp_s0 = func_8005B24C_5BE4C(s2, arg0->unk38, 0xA0000);

    if (temp_s0 != NULL) {
        func_80050ECC_51ACC(s2);
        queueSoundAtPosition(s2, 0xD);
        setPlayerProjectileHitState(temp_s0);
        func_80069CF8_6A8F8();
        return;
    }

    if (arg0->unk8.y < sp.y) {
        arg0->unk8.y = sp.y;
        arg0->unk36 = 0xF0;
        setCallback(&func_8004BFBC_4CBBC);
    }

    i = 0;

exit_loop:
    do {
        func_80066444_67044(i, (func_80066444_67044_arg1 *)&arg0->unk4);
        i++;
    } while (i < 4);
}

void func_8004BFBC_4CBBC(func_8004BFBC_4CBBC_arg *arg0) {
    Vec3i sp10;
    Vec3i *s0;
    Vec3i *s2;
    void *s3;
    s32 i;

    if (((GameState *)getCurrentAllocation())->gamePaused == 0) {
        arg0->unk4.unk36--;
    }

    if (arg0->unk4.unk36 == 0) {
        func_80069CF8_6A8F8();
    }

    s2 = &arg0->unk4.unk4;
    s3 = func_8005B24C_5BE4C(s2, arg0->unk38, 0xA0000);
    s0 = &sp10;

    if (s3 != NULL) {
        memcpy(s0, s2, sizeof(Vec3i));
        sp10.y = sp10.y + 0xA0000;
        func_80050ECC_51ACC(s0);
        queueSoundAtPosition(s0, 0xD);
        setPlayerProjectileHitState(s3);
        func_80069CF8_6A8F8();
    } else {
        if (arg0->unk4.unk36 < 0x1F) {
            i = 0;
            if ((gFrameCounter & 1) == 0) {
                return;
            }
        }
        for (i = 0; i < 4; i++) {
            func_80066444_67044(i, &arg0->unk4);
        }
    }
}

void func_8004C0D0_4CCD0(func_8004C0D0_4CCD0_arg *arg0) {
    GameState *allocation = (GameState *)getCurrentAllocation();
    allocation->availableHomingProjectileSlots++;
    arg0->unk0 = freeNodeMemory(arg0->unk0);
}

void func_8004C10C_4CD0C(Player *arg0) {
    GameState *allocation;
    Node *task;

    allocation = (GameState *)getCurrentAllocation();
    task = (Node *)scheduleTask((void *)func_8004BCBC_4C8BC, 3, 0, 0xEF);

    if (task != NULL) {
        task->cleanupCallback = (void (*)(void *))arg0;
        allocation->availableHomingProjectileSlots--;
    }
}

INCLUDE_RODATA("asm/nonmatchings/46080", D_8009E868_9F468);

INCLUDE_RODATA("asm/nonmatchings/46080", sGoldFormatShort);

INCLUDE_RODATA("asm/nonmatchings/46080", sGoldFormatLong);
