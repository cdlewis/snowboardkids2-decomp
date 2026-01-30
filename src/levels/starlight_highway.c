#include "levels/starlight_highway.h"
#include "19E80.h"
#include "42170.h"
#include "56910.h"
#include "5AA90.h"
#include "audio.h"
#include "common.h"
#include "displaylist.h"
#include "gamestate.h"
#include "geometry.h"
#include "rand.h"
#include "task_scheduler.h"

extern s32 gFrameCounter;
extern Gfx *gRegionAllocPtr;
extern s16 gGraphicsMode;

typedef struct {
    u8 _pad[0x80];
    u8 animateNode1;
    u8 animateNode2;
} StarlightBuildingAllocation;

typedef struct {
    DisplayListObject node1;
    DisplayListObject node2;
} StarlightBuildingRenderData;

// Starlight Highway Barrier Task
// Each barrier has two gates (left/right) that open when players enter the trigger sector
typedef struct {
    /* 0x00 */ Transform3D leftGateNode;        // Left gate transform
    /* 0x20 */ void *leftGateDisplayListOffset; // Left gate display list offset
    /* 0x24 */ void *displayListMemory1;        // Display list memory pointer 1
    /* 0x28 */ void *displayListMemory2;        // Display list memory pointer 2
    /* 0x2C */ s32 unk2C;                       // Unknown
    /* 0x30 */ u8 pad30[0xC];
    /* 0x3C */ Transform3D rightGateNode;        // Right gate transform
    /* 0x5C */ void *rightGateDisplayListOffset; // Right gate display list offset
    /* 0x60 */ void *unk60;                      // Unknown
    /* 0x64 */ void *unk64;                      // Unknown
    /* 0x68 */ s32 unk68;                        // Unknown
    /* 0x6C */ u8 pad6C[0xC];
    /* 0x78 */ s32 gateOpenAmount; // Gate opening distance (0 = closed, 0x600000 = fully open)
    /* 0x7C */ u8 barrierIndex;    // Barrier index (0 or 1) to select which configuration to use
} StarlightBarrierTask;

typedef struct {
    u8 _pad[0x24];
    void *displayListMemory1;
    void *displayListMemory2;
    u8 _pad2[0x10];
    void *displayListMemory3;
} DebugDisplayListCleanupState;

typedef struct {
    /* 0x00 */ u8 _pad0[0x20];
    /* 0x20 */ void *config;
    /* 0x24 */ void *displayListMemory1;
    /* 0x28 */ void *displayListMemory2;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ u8 _pad30[0xC];
    /* 0x3C */ void *displayListMemory3;
    /* 0x40 */ u8 _pad40[0x4];
    /* 0x44 */ s16 uOffset;
    /* 0x46 */ s16 vOffset;
    /* 0x48 */ s16 uSpeed;
    /* 0x4A */ s16 vSpeed;
    /* 0x4C */ s16 unk4C;
    /* 0x4E */ u8 _pad4E[0x2];
    /* 0x50 */ s16 animationTimer;
} DebugDisplayListTaskState;

typedef struct {
    u8 unk00[0x20];
    struct {
        s32 unk00;
        s32 renderOpaque;
        s32 renderTransparent;
        s32 renderOverlay;
    } *config;
    u8 unk24[0xC];
    s32 unk30;
    u8 unk34[0x10];
    u16 uOffset;
    u16 vOffset;
    u16 uSpeed;
    u16 vSpeed;
    s32 unk4C;
    s16 animationTimer;
    s16 sustainTimer;
} DebugDisplayListRenderState;

typedef struct {
    u8 pad[0x76];
    u8 paused;
} DebugDisplayListAllocation;

typedef struct {
    /* 0x00 */ Transform3D mat;
    /* 0x20 */ u8 _pad20[0x5C];
} DebugDisplayListPosition;

typedef struct {
    u8 _pad[0x76];
    u8 paused;
} TaskAllocationState;

typedef struct {
    /* 0x00 */ u8 _pad0[0x14];
    /* 0x14 */ Vec3i pos;
    /* 0x20 */ void *displayList;
    /* 0x24 */ void *segment1;
    /* 0x28 */ void *segment2;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ u8 _pad30[0xC];
    /* 0x3C */ Vec3i velocity;
    /* 0x48 */ s16 lifetime;
    /* 0x4A */ u16 rotX;
    /* 0x4C */ u16 rotY;
    /* 0x4E */ u8 type;
} StarlightFireworkTaskState;

typedef struct {
    u8 _pad[0x24];
    void *segment1;
    void *segment2;
} StarlightFireworkCleanupState;

typedef struct {
    s16 rotation[3][3];
    u8 pad2[0xE];
} FireworkInitRotationMatrix;

extern Vec3i D_800BCA30_AEDF0[];
extern s32 gStarlightFireworkPositions[][3];
extern s16 gStarlightFireworkXRotations[];
extern s32 gStarlightFireworkDirections[][3];
extern s16 gStarlightFireworkRotXSpeeds[];
extern s16 gStarlightFireworkRotYSpeeds[];
extern s32 gStarlightBarrierPositionsX[][3];
extern s32 gStarlightBarrierPositionsY[][3];
extern s32 gStarlightBarrierPositionsZ[][3];
extern s16 gStarlightBarrierTriggerSectors[];
extern void *gDebugDisplayConfig;

void cleanupStarlightBarrierTask(DualSegmentCleanupState *);
void cleanupDebugDisplayListTask(DebugDisplayListCleanupState *);
void updateDebugDisplayListGrowth(DebugDisplayListRenderState *);
void renderStarlightHighwayBuildings(StarlightBuildingRenderData *);
void cleanupStarlightHighwayBuildingTask(DualSegmentCleanupState *);
void loadColorIndexedTexture(void *);
void updateStarlightFireworkSimple(StarlightFireworkTaskState *);
void updateStarlightFireworkComplex(StarlightFireworkTaskState *);
void updateStarlightFirework(StarlightFireworkTaskState *);
void cleanupStarlightFireworkTask(StarlightFireworkCleanupState *);
void updateFireworkShowTimer(FireworkShowTimerState *arg0);
void updateStarlightBarrier(StarlightBarrierTask *arg0);

void initStarlightHighwayBuildingTask(StarlightBuildingTaskState *task) {
    task->segment1 = loadUncompressedAssetByIndex(8);
    task->segment2 = loadCompressedSegment2AssetByIndex(8);
    task->unk2C = 0;
    createYRotationMatrix(&task->node1, 0x98A);
    task->node1.translation.x = 0x130F588E;
    task->node1.translation.y = 0x03930000;
    task->node1.translation.z = 0x0DB72F2C;
    task->unk68 = 0;
    task->cleanupSegment1 = task->segment1;
    task->cleanupSegment2 = task->segment2;
    createYRotationMatrix(&task->node2, 0);
    task->node2.translation.x = 0;
    task->node2.translation.y = 0x30000000;
    task->node2.translation.z = 0x30458CB2;
    setCleanupCallback(&cleanupStarlightHighwayBuildingTask);
    setCallback(&renderStarlightHighwayBuildings);
}

void renderStarlightHighwayBuildings(StarlightBuildingRenderData *arg0) {
    StarlightBuildingAllocation *allocation;
    s32 i;
    LevelDisplayLists *displayListResult;

    allocation = (StarlightBuildingAllocation *)getCurrentAllocation();

    if (allocation->animateNode1 != 0) {
        if (gFrameCounter & 1) {
            displayListResult = getSkyDisplayLists3ByIndex(8);
            arg0->node1.displayLists = (DisplayLists *)((u32)displayListResult + 0x10);
        } else {
            displayListResult = getSkyDisplayLists3ByIndex(8);
            arg0->node1.displayLists = (DisplayLists *)((u32)displayListResult + 0x20);
        }
    } else {
        arg0->node1.displayLists = (DisplayLists *)getSkyDisplayLists3ByIndex(8);
    }

    if (allocation->animateNode2 != 0) {
        if (gFrameCounter & 1) {
            displayListResult = getSkyDisplayLists3ByIndex(8);
            arg0->node2.displayLists = (DisplayLists *)((u32)displayListResult + 0x10);
        } else {
            displayListResult = getSkyDisplayLists3ByIndex(8);
            arg0->node2.displayLists = (DisplayLists *)((u32)displayListResult + 0x20);
        }
    } else {
        arg0->node2.displayLists = (DisplayLists *)getSkyDisplayLists3ByIndex(8);
    }

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, &arg0->node1);
        enqueueDisplayListWithFrustumCull(i, &arg0->node2);
    }
}

void cleanupStarlightHighwayBuildingTask(DualSegmentCleanupState *arg0) {
    arg0->segment1 = freeNodeMemory(arg0->segment1);
    arg0->segment2 = freeNodeMemory(arg0->segment2);
}

void initDebugDisplayListTask(DebugDisplayListTaskState *arg0) {
    arg0->displayListMemory1 = loadUncompressedAssetByIndex(8);
    arg0->displayListMemory2 = loadCompressedSegment2AssetByIndex(8);
    arg0->unk2C = 0;
    arg0->displayListMemory3 = loadSpriteAssetByIndex(8);
    arg0->config = &gDebugDisplayConfig;
    arg0->unk4C = 0;
    arg0->uSpeed = 8;
    arg0->vSpeed = 0;
    arg0->uOffset = 0;
    arg0->vOffset = 0;
    arg0->animationTimer = 0;
    setCleanupCallback(&cleanupDebugDisplayListTask);
    setCallback(&updateDebugDisplayListGrowth);
}

void renderColorIndexedOpaqueDisplayList(void *);
void renderColorIndexedTransparentDisplayList(void *);
void renderColorIndexedOverlayDisplayList(void *);
void renderDebugDisplayLists(DebugDisplayListRenderState *arg0);
void updateDebugDisplayListSustain(DebugDisplayListRenderState *arg0);
void updateDebugDisplayListDecay(DebugDisplayListRenderState *arg0);

void renderDebugDisplayLists(DebugDisplayListRenderState *arg0) {
    s32 i;

    arg0->uOffset += arg0->uSpeed;
    arg0->vOffset = arg0->vOffset + arg0->vSpeed;
    arg0->unk30 = 0;
    arg0->uOffset = arg0->uOffset & 0xFF;
    arg0->vOffset = arg0->vOffset & 0xFF;
    ;

    for (i = 0; i < 4; i++) {
        if (arg0->config->renderOpaque != 0) {
            debugEnqueueCallback(i & 0xFFFF, 1, renderColorIndexedOpaqueDisplayList, arg0);
        }

        if (arg0->config->renderTransparent != 0) {
            debugEnqueueCallback(i & 0xFFFF, 3, renderColorIndexedTransparentDisplayList, arg0);
        }

        if (arg0->config->renderOverlay == 0) {
            continue;
        }

        debugEnqueueCallback(i & 0xFFFF, 5, renderColorIndexedOverlayDisplayList, arg0);
    }
}

void updateDebugDisplayListGrowth(DebugDisplayListRenderState *arg0) {
    DebugDisplayListAllocation *allocation = getCurrentAllocation();

    if (allocation->paused == 0) {
        arg0->animationTimer += 0x10;
    }

    if (arg0->animationTimer == 0xC0) {
        setCallback(&updateDebugDisplayListSustain);
        arg0->sustainTimer = 0x40;
    }

    renderDebugDisplayLists(arg0);
}

void updateDebugDisplayListSustain(DebugDisplayListRenderState *arg0) {
    DebugDisplayListAllocation *allocation = getCurrentAllocation();

    if (allocation->paused == 0) {
        arg0->sustainTimer--;
    }

    if (arg0->sustainTimer == 0) {
        setCallback(&updateDebugDisplayListDecay);
    }

    renderDebugDisplayLists(arg0);
}

void updateDebugDisplayListDecay(DebugDisplayListRenderState *arg0) {
    DebugDisplayListAllocation *allocation = getCurrentAllocation();

    if (allocation->paused == 0) {
        arg0->animationTimer -= 0x10;
    }

    if (arg0->animationTimer == 0) {
        terminateCurrentTask();
    }

    renderDebugDisplayLists(arg0);
}

void cleanupDebugDisplayListTask(DebugDisplayListCleanupState *arg0) {
    arg0->displayListMemory1 = freeNodeMemory(arg0->displayListMemory1);
    arg0->displayListMemory2 = freeNodeMemory(arg0->displayListMemory2);
    arg0->displayListMemory3 = freeNodeMemory(arg0->displayListMemory3);
}

typedef struct {
    /* 0x00 */ u8 _pad0[0x3C];
    /* 0x3C */ DataTable_19E80 *textureTable;
    /* 0x40 */ u8 _pad40[4];
    /* 0x44 */ u16 tileULow;
    /* 0x46 */ u16 tileVLow;
    /* 0x48 */ u8 _pad48[4];
    /* 0x4C */ u16 textureIndex;
    /* 0x4E */ u8 _pad4E[3];
    /* 0x51 */ u8 alpha;
} TextureRenderState;

void loadColorIndexedTexture(void *arg) {
    TextureRenderState *state = (TextureRenderState *)arg;
    OutputStruct_19E80 tableEntry;
    s32 dxtBase;
    s32 lrs;
    u16 widthDiv16;
    Gfx *loadBlockCmd;
    long loadBlockWord;
    s32 loadTileParams;
    s32 widthShift;
    s32 heightShift;
    s32 tempWidth;
    s32 tempHeight;
    u32 tileLine;

    gDPPipeSync(gRegionAllocPtr++);
    gDPSetTextureLUT(gRegionAllocPtr++, G_TT_RGBA16);
    gGraphicsMode = -1;

    getTableEntryByU16Index(state->textureTable, state->textureIndex, &tableEntry);

    tempWidth = tableEntry.field1;
    widthShift = 0;
loop_1:
    if (!(tempWidth & 1)) {
        do {
            widthShift += 1;
            if (widthShift < 0x10) {
                tempWidth >>= 1;
                goto loop_1;
            }
        } while (0);
    }

    tempHeight = tableEntry.field2;
    heightShift = 0;
loop_2:
    if (!(tempHeight & 1)) {
        do {
            heightShift += 1;
            if (heightShift < 0x10) {
                tempHeight >>= 1;
                goto loop_2;
            }
        } while (0);
    }

    gDPSetTextureImage(gRegionAllocPtr++, G_IM_FMT_CI, G_IM_SIZ_16b, 1, tableEntry.data_ptr);

    gDPSetTile(
        gRegionAllocPtr++,
        G_IM_FMT_CI,
        G_IM_SIZ_16b,
        0,
        0x0000,
        G_TX_LOADTILE,
        0,
        0,
        heightShift,
        0,
        0,
        widthShift,
        0
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
    if (lrs >= 0x800) {
        lrs = 0x7FF;
    }
    loadTileParams = ((lrs & 0xFFF) << 12) | 0x07000000;
    loadBlockWord = loadTileParams;
    if (widthDiv16 != 0) {
        loadBlockWord |= (dxtBase / widthDiv16) & 0xFFF;
    } else {
        loadBlockWord |= dxtBase & 0xFFF;
    }
    loadBlockCmd->words.w1 = loadBlockWord;

    gDPPipeSync(gRegionAllocPtr++);

    tileLine = (((tableEntry.field1 >> 1) + 7) >> 3) & 0x1FF;
    gDPSetTile(
        gRegionAllocPtr++,
        G_IM_FMT_CI,
        G_IM_SIZ_4b,
        tileLine,
        0,
        G_TX_RENDERTILE,
        0,
        0,
        heightShift,
        0,
        0,
        widthShift,
        0
    );

    loadTileParams = 15;

    gDPSetTileSize(
        gRegionAllocPtr++,
        G_TX_RENDERTILE,
        0,
        0,
        (tableEntry.field1 - 1) << 2,
        (tableEntry.field2 - 1) << 2
    );

    gDPSetTileSize(
        gRegionAllocPtr++,
        G_TX_RENDERTILE,
        state->tileULow & 0xFFF,
        state->tileVLow & 0xFFF,
        ((tableEntry.field1 + (s16)state->tileULow - 1) << 2) & 0xFFF,
        ((tableEntry.field2 + (s16)state->tileVLow - 1) << 2) & 0xFFF
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

    gDPLoadTLUTCmd(gRegionAllocPtr++, G_TX_LOADTILE, loadTileParams);

    gDPPipeSync(gRegionAllocPtr++);

    gDPSetEnvColor(gRegionAllocPtr++, 0xFF, 0xFF, 0xFF, state->alpha);
}

void renderColorIndexedOpaqueDisplayList(void *arg0) {
    loadColorIndexedTexture(arg0);
    renderOpaqueDisplayList(arg0);
}

void renderColorIndexedTransparentDisplayList(void *arg0) {
    loadColorIndexedTexture(arg0);
    renderTransparentDisplayList(arg0);
}

void renderColorIndexedOverlayDisplayList(void *arg0) {
    loadColorIndexedTexture(arg0);
    renderOverlayDisplayList(arg0);
}

// This is probably wrong but stops the build failing.
void func_800BBB90(s16 arg0) __attribute__((alias("spawnDebugDisplayListTask")));

void spawnDebugDisplayListTask(s16 arg0) {
    DebugDisplayListPosition *task;

    task = scheduleTask(initDebugDisplayListTask, 0, 0, 0xD2);
    if (task != NULL) {
        if (arg0 == 0) {
            createYRotationMatrix(&task->mat, 0x98A);
            task->mat.translation.x = 0x130F588E;
            task->mat.translation.y = 0x03930000;
            task->mat.translation.z = 0x0DB72F2C;
        } else {
            createYRotationMatrix(&task->mat, 0);
            task->mat.translation.x = 0;
            task->mat.translation.y = 0x30000000;
            task->mat.translation.z = 0x30458CB2;
        }
    }
}

void initStarlightItemTask(StarlightItemTaskState *arg0) {
    arg0->segment1 = loadUncompressedAssetByIndex(8);
    arg0->segment2 = loadCompressedSegment2AssetByIndex(8);
    arg0->unk2C = 0;
    memcpy(&arg0->mat1.translation, &D_800BCA30_AEDF0[arg0->positionIndex], sizeof(Vec3i));
    arg0->unk68 = 0;
    arg0->mat1.translation.y += 0x100000;
    arg0->unk60 = arg0->segment1;
    arg0->unk64 = arg0->segment2;
    createYRotationMatrix(&arg0->mat2, 0);
    arg0->scale = 0x2000;
    arg0->mat2.translation.x = arg0->mat1.translation.x;
    arg0->mat2.translation.y = arg0->mat1.translation.y + 0x180000;
    arg0->mat2.translation.z = arg0->mat1.translation.z;
    setCleanupCallback(&cleanupStarlightItemTask);
    setCallback(&updateStarlightItemTask);
}

// Update and render the starlight item (power-up) on Starlight Highway
// Checks for player proximity, handles collection animation, and renders the item with shadow
void updateStarlightItemTask(StarlightItemTaskState *arg0) {
    GameState *allocation;
    s32 var_s0;    // playersChecked - counter for players that have been checked
    s32 var_s1;    // playerOffset - byte offset into player array
    s16 temp_v0;   // currentScale - holds the current scale value
    s16 temp_a1;   // scaleForMatrix - scale value to pass to scaleMatrix
    u16 temp_v0_2; // newRotation - new rotation value after increment
    s32 temp;      // numPlayers - number of players in the game
    u8 pad[0x10];

    (void)pad;

    allocation = (GameState *)getCurrentAllocation();
    var_s0 = 0;
    temp = allocation->numPlayers;
    if (temp > 0) {
        var_s1 = 0;
    // Check each player to see if any are in range (0x200000 = close range)
    check_player_range:
        if (isPlayerInRangeAndPull(
                (Vec3i *)&arg0->mat1.translation.x,
                0x200000,
                (Player *)((u8 *)allocation->players + var_s1)
            ) == 0) {
            var_s0 += 1;
            var_s1 += 0xBE8;
            if (var_s0 < (s32)allocation->numPlayers) {
                goto check_player_range;
            }
        }
    }

    // If a player is in range and item is at base scale, trigger collection animation
    if ((var_s0 != allocation->numPlayers) && (arg0->scale == 0x2000)) {
        var_s0 = 0;
        if (allocation->numPlayers != 0) {
            var_s1 = 0;
            do {
                // Pull players closer (0x500000 = attraction range)
                isPlayerInRangeAndPull(
                    (Vec3i *)&arg0->mat1.translation.x,
                    0x500000,
                    (Player *)((u8 *)allocation->players + var_s1)
                );
                var_s0 += 1;
                var_s1 += 0xBE8;
            } while (var_s0 < (s32)allocation->numPlayers);
        }
        arg0->scale = 0x4000;
        queueSoundAtPosition(&arg0->mat1.translation, 0x4E);
    }

    // Rotate the item
    temp_v0_2 = arg0->rotation + 0x100;
    arg0->rotation = temp_v0_2;
    createYRotationMatrix(&arg0->mat1, temp_v0_2 & 0xFFFF);

    // Handle shrinking animation if item was collected
    temp_v0 = arg0->scale;
    if (temp_v0 != 0x2000) {
        if (allocation->gamePaused == 0) {
            arg0->scale = (s16)(temp_v0 - 0x800);
        }
        temp_a1 = arg0->scale;
        scaleMatrix(&arg0->mat1, temp_a1, 0x2000, temp_a1);
        if (arg0->scale != 0x2000) {
            goto shrinking_animation;
        }
    }
    // Full-size display lists (normal state)
    arg0->displayLists2 =
        (DisplayLists *)((arg0->displayLists1 = (DisplayLists *)((s32)getSkyDisplayLists3ByIndex(8) + 0xD0)),
                         (s32)getSkyDisplayLists3ByIndex(8) + 0xE0);
    goto render;

shrinking_animation:
    // Shrinking display lists (collected state)
    arg0->displayLists2 =
        (DisplayLists *)((arg0->displayLists1 = (DisplayLists *)((s32)getSkyDisplayLists3ByIndex(8) + 0xC0)),
                         (s32)getSkyDisplayLists3ByIndex(8) + 0xF0);

render:
    var_s0 = 0;
    do {
        enqueueDisplayListWithFrustumCull(var_s0, (DisplayListObject *)&arg0->mat1);
        enqueueDisplayListWithFrustumCull(var_s0, (DisplayListObject *)&arg0->mat2);
        var_s0 += 1;
    } while (var_s0 < 4);
}

void cleanupStarlightItemTask(DualSegmentCleanupState *arg0) {
    arg0->segment1 = freeNodeMemory(arg0->segment1);
    arg0->segment2 = freeNodeMemory(arg0->segment2);
}

void spawnStarlightItems(void) {
    s32 i;

    for (i = 0; i < 7; i++) {
        StarlightItemTaskState *task = scheduleTask(initStarlightItemTask, 0, 0, 0x32);
        if (task != NULL) {
            task->positionIndex = i;
        }
    }
}

void initStarlightFireworkTask(StarlightFireworkTaskState *arg0) {
    u8 pad[0x20];
    FireworkInitRotationMatrix stack;
    s16(*rotPtr)[3];
    void (*callback)(StarlightFireworkTaskState *);

    (void)pad;

    arg0->segment1 = loadUncompressedAssetByIndex(8);
    arg0->segment2 = loadCompressedSegment2AssetByIndex(8);
    arg0->unk2C = 0;
    arg0->displayList = (void *)((u32)getSkyDisplayLists3ByIndex(8) + 0x90);
    memcpy(&arg0->pos, gStarlightFireworkPositions[arg0->type], 0xC);
    rotPtr = stack.rotation;
    createXRotationMatrix(rotPtr, gStarlightFireworkXRotations[arg0->type]);
    transformVector2(gStarlightFireworkDirections[arg0->type], rotPtr, &arg0->velocity);
    arg0->pos.x = arg0->pos.x - arg0->velocity.x * 0x78;
    arg0->pos.y = arg0->pos.y - arg0->velocity.y * 0x78;
    arg0->pos.z = arg0->pos.z - arg0->velocity.z * 0x78;
    setCleanupCallback(cleanupStarlightFireworkTask);
    if (arg0->type < 3) {
        callback = updateStarlightFireworkSimple;
        arg0->lifetime = 0xF0;
    } else {
        arg0->lifetime = 0x78;
        callback = updateStarlightFireworkComplex;
    }
    setCallback(callback);
}

void updateStarlightFireworkSimple(StarlightFireworkTaskState *arg0) {
    TaskAllocationState *allocation;
    s32 i;
    void *posPtr;

    allocation = getCurrentAllocation();

    if (allocation->paused == 0) {
        arg0->pos.x += arg0->velocity.x;
        arg0->pos.y += arg0->velocity.y;
        arg0->pos.z += arg0->velocity.z;
        arg0->lifetime--;

        if (arg0->lifetime == 0) {
            terminateCurrentTask();
            return;
        }

        posPtr = &arg0->pos;
        if (checkStarHitCollisionWithVulnerablePlayers(posPtr, -1, 0x300000) != 0) {
            spawnSparkleEffect(posPtr);
            terminateCurrentTask();
        }

        arg0->rotX += gStarlightFireworkRotXSpeeds[arg0->type];
        arg0->rotY += gStarlightFireworkRotYSpeeds[arg0->type];
    }

    createCombinedRotationMatrix(arg0, arg0->rotX, arg0->rotY);

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
    }
}

void updateStarlightFireworkComplex(StarlightFireworkTaskState *firework) {
    TaskAllocationState *allocation;
    s32 i;
    s16 rotation[3][3];
    s16 pad2[4];
    u8 fireworkType;

    (void)pad2;

    allocation = getCurrentAllocation();

    if (allocation->paused == 0) {
        if (firework->lifetime != 0) {
            firework->pos.x = firework->pos.x + firework->velocity.x;
            firework->pos.y = firework->pos.y + firework->velocity.y;
            firework->pos.z = firework->pos.z + firework->velocity.z;
            firework->lifetime--;
        } else {
            fireworkType = firework->type;
            switch (fireworkType) {
                case 3:
                    createXRotationMatrix(rotation, 0xF300);
                    transformVector2(gStarlightFireworkDirections[firework->type], rotation, &firework->velocity);
                    firework->lifetime = 0x78;
                    setCallbackWithContinue(updateStarlightFireworkSimple);
                    break;
                case 4:
                    createXRotationMatrix(rotation, 0xE00);
                    transformVector2(gStarlightFireworkDirections[firework->type], rotation, &firework->velocity);
                    firework->lifetime = 0xA;
                    setCallbackWithContinue(updateStarlightFirework);
                    break;
                case 5:
                    createXRotationMatrix(rotation, 0xFC00);
                    transformVector2(gStarlightFireworkDirections[firework->type], rotation, &firework->velocity);
                    firework->lifetime = 0x78;
                    setCallbackWithContinue(updateStarlightFireworkSimple);
                    break;
                case 6:
                    createXRotationMatrix(rotation, 0);
                    transformVector2(gStarlightFireworkDirections[firework->type], rotation, &firework->velocity);
                    firework->lifetime = 0xA;
                    setCallbackWithContinue(updateStarlightFirework);
                    break;
                case 7:
                case 8:
                    spawnSparkleEffect(&firework->pos);
                    terminateCurrentTask();
                    break;
                default:
                    break;
            }
        }
        checkStarHitCollisionWithVulnerablePlayers(&firework->pos, -1, 0x300000);
        firework->rotX += gStarlightFireworkRotXSpeeds[firework->type];
        firework->rotY += gStarlightFireworkRotYSpeeds[firework->type];
    }

    createCombinedRotationMatrix(firework, firework->rotX, firework->rotY);

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)firework);
    }
}

void updateStarlightFirework(StarlightFireworkTaskState *arg0) {
    TaskAllocationState *allocation;
    s32 i;
    void *posPtr;

    allocation = getCurrentAllocation();

    if (allocation->paused == 0) {
        arg0->pos.x += arg0->velocity.x;
        arg0->pos.y += arg0->velocity.y;
        arg0->pos.z += arg0->velocity.z;
        arg0->lifetime--;

        if (arg0->lifetime == 0) {
            spawnSparkleEffect(&arg0->pos);
            terminateCurrentTask();
        } else {
            posPtr = &arg0->pos;
            if (checkStarHitCollisionWithVulnerablePlayers(posPtr, -1, 0x300000)) {
                spawnSparkleEffect(posPtr);
                terminateCurrentTask();
            }
        }

        arg0->rotX += gStarlightFireworkRotXSpeeds[arg0->type];
        arg0->rotY += gStarlightFireworkRotYSpeeds[arg0->type];
    }

    createCombinedRotationMatrix(arg0, arg0->rotX, arg0->rotY);

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
    }
}

void cleanupStarlightFireworkTask(StarlightFireworkCleanupState *arg0) {
    arg0->segment1 = freeNodeMemory(arg0->segment1);
    arg0->segment2 = freeNodeMemory(arg0->segment2);
}

void initFireworkShowTimer(FireworkShowTimerState *arg0) {
    arg0->timer = 0xF0;
    setCallback(&updateFireworkShowTimer);
}

void updateFireworkShowTimer(FireworkShowTimerState *arg0) {
    TaskAllocationState *allocation;
    StarlightFireworkTaskState *task;
    s32 fireworkTypeBase;
    s32 firstFireworkType;
    u32 timerValue;

    allocation = getCurrentAllocation();
    if (allocation->paused != 0) {
        return;
    }

    arg0->timer -= 1;
    if (arg0->timer == 0x78) {
        task = scheduleTask(initStarlightFireworkTask, 0, 0, 0xC8);
        if (task != NULL) {
            task->type = (u32)(randA() & 0xFF) % 3U;
        }
    }

    if ((timerValue = arg0->timer) == 0) {
        if (getFreeNodeCount(0) >= 2) {
            fireworkTypeBase = 2;
            fireworkTypeBase = (u8)((u32)(randA() & 0xFF) % 3U) * fireworkTypeBase;
            firstFireworkType = fireworkTypeBase + 3;
            task = scheduleTask(initStarlightFireworkTask, 0, 0, 0xC8);
            if (task != NULL) {
                task->type = firstFireworkType;
            }

            task = scheduleTask(initStarlightFireworkTask, 0, 0, 0xC8);
            if (task != NULL) {
                task->type = fireworkTypeBase + 4;
            }
        }

        arg0->timer = (randA() & 0x1F) + 0xF0;
    }
}

// Starlight Highway Barrier System
// Creates two gates that open when players are in the trigger sector
// and close when no players are nearby. Each barrier has two nodes (left/right)
// that move apart when opening.

void initStarlightBarrierTask(StarlightBarrierTask *arg0) {
    void *temp;

    arg0->displayListMemory1 = loadUncompressedAssetByIndex(8);
    arg0->displayListMemory2 = loadCompressedSegment2AssetByIndex(8);
    arg0->unk2C = 0;
    temp = getSkyDisplayLists3ByIndex(8);
    arg0->leftGateDisplayListOffset = (void *)((u32)temp + 0xA0);
    arg0->unk68 = 0;
    arg0->unk60 = arg0->displayListMemory1;
    arg0->unk64 = arg0->displayListMemory2;
    temp = getSkyDisplayLists3ByIndex(8);
    arg0->rightGateDisplayListOffset = (void *)((u32)temp + 0xB0);
    createYRotationMatrix(&arg0->leftGateNode, 0x1BEC);
    createYRotationMatrix(&arg0->rightGateNode, 0x1BEC);
    arg0->gateOpenAmount = 0;
    setCleanupCallback(&cleanupStarlightBarrierTask);
    setCallback(&updateStarlightBarrier);
}

// Update barrier animation: open/close based on player proximity to trigger sector
void updateStarlightBarrier(StarlightBarrierTask *arg0) {
    Vec3i vec;
    s32 pad[2];
    s32 found; // 1 if any player is in the trigger sector
    s32 i;
    u8 temp;
    s32 numPlayers;
    GameState *allocation;
    Player *player;
    s32 gateOpenAmount; // Copy of gate open amount
    s32 tempS;

    allocation = getCurrentAllocation();
    temp = allocation->numPlayers;
    found = 0;
    i = 0;
    tempS = temp;

    // Check if any player is in the trigger sector for this barrier
    if (tempS > 0) {
        numPlayers = tempS;
        player = allocation->players;
        do {
            if (player->sectorIndex == gStarlightBarrierTriggerSectors[arg0->barrierIndex]) {
                found = 1;
            }
            i++;
            player++;
        } while (i < numPlayers);
    }

    // Open the gate if player is near, close if not
    if (found != 0) {
        if (allocation->gamePaused == 0) {
            if (arg0->gateOpenAmount != 0x600000) {
                arg0->gateOpenAmount = arg0->gateOpenAmount + 0x100000;
            }
        }
    } else if (allocation->gamePaused == 0) {
        if (arg0->gateOpenAmount != 0) {
            arg0->gateOpenAmount = arg0->gateOpenAmount - 0x100000;
        }
    }

    // Apply the open amount to both gate positions
    gateOpenAmount = arg0->gateOpenAmount;
    vec.y = 0;
    vec.z = 0;
    vec.x = -gateOpenAmount;
    rotateVectorY(&vec, 0x1BEC, &arg0->leftGateNode.translation);
    vec.x = arg0->gateOpenAmount;
    rotateVectorY(&vec, 0x1BEC, &arg0->rightGateNode.translation);

    // Add base position offset for the barrier
    arg0->leftGateNode.translation.x =
        arg0->leftGateNode.translation.x + gStarlightBarrierPositionsX[arg0->barrierIndex][0];
    arg0->leftGateNode.translation.y =
        arg0->leftGateNode.translation.y + gStarlightBarrierPositionsY[arg0->barrierIndex][0];
    arg0->leftGateNode.translation.z =
        arg0->leftGateNode.translation.z + gStarlightBarrierPositionsZ[arg0->barrierIndex][0];
    arg0->rightGateNode.translation.x =
        arg0->rightGateNode.translation.x + gStarlightBarrierPositionsX[arg0->barrierIndex][0];
    arg0->rightGateNode.translation.y =
        arg0->rightGateNode.translation.y + gStarlightBarrierPositionsY[arg0->barrierIndex][0];
    i = 0;
    arg0->rightGateNode.translation.z =
        arg0->rightGateNode.translation.z + gStarlightBarrierPositionsZ[arg0->barrierIndex][0];

    do {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)&arg0->leftGateNode);
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)&arg0->rightGateNode);
        i++;
    } while (i < 4);

    (void)pad;
}

void cleanupStarlightBarrierTask(DualSegmentCleanupState *arg0) {
    arg0->segment1 = freeNodeMemory(arg0->segment1);
    arg0->segment2 = freeNodeMemory(arg0->segment2);
}

// Spawn the two Starlight Highway barriers
// Creates two barrier tasks with indices 0 and 1
void spawnStarlightBarriers(void) {
    StarlightBarrierTask *task;

    task = (StarlightBarrierTask *)scheduleTask(initStarlightBarrierTask, 0, 0, 0xC8);
    if (task != NULL) {
        task->barrierIndex = 0;
    }

    task = (StarlightBarrierTask *)scheduleTask(initStarlightBarrierTask, 0, 0, 0xC8);
    if (task != NULL) {
        task->barrierIndex = 1;
    }
}
