#include "levels/starlight_highway.h"
#include "19E80.h"
#include "42170.h"
#include "56910.h"
#include "5AA90.h"
#include "common.h"
#include "displaylist.h"
#include "gamestate.h"
#include "geometry.h"
#include "graphics.h"
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

typedef struct {
    /* 0x00 */ Transform3D node1;
    /* 0x20 */ void *unk20;
    /* 0x24 */ void *unk24;
    /* 0x28 */ void *unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ u8 pad30[0xC];
    /* 0x3C */ Transform3D node2;
    /* 0x5C */ void *unk5C;
    /* 0x60 */ void *unk60;
    /* 0x64 */ void *unk64;
    /* 0x68 */ s32 unk68;
    /* 0x6C */ u8 pad6C[0xC];
    /* 0x78 */ s32 unk78;
    /* 0x7C */ u8 unk7C;
} func_800BC6C4_AEA84_arg;

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
    u8 unk76;
} DebugDisplayListAllocation;

typedef struct {
    /* 0x00 */ Transform3D mat;
    /* 0x20 */ u8 _pad20[0x5C];
} DebugDisplayListPosition;

typedef struct {
    u8 _pad[0x76];
    u8 unk76;
} Allocation_AE790;

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
extern s32 D_800BCB98_AEF58[][3];
extern s32 D_800BCB9C_AEF5C[][3];
extern s32 D_800BCBA0_AEF60[][3];
extern s16 D_800BCBB0_AEF70[];
extern void *gDebugDisplayConfig;

void func_800BC984_AED44(DualSegmentCleanupState *);
void cleanupDebugDisplayListTask(DebugDisplayListCleanupState *);
void updateDebugDisplayListGrowth(DebugDisplayListRenderState *);
void renderStarlightHighwayBuildings(StarlightBuildingRenderData *);
void cleanupStarlightHighwayBuildingTask(DualSegmentCleanupState *);
void loadColorIndexedTexture(void *);
void updateStarlightFireworkSimple(StarlightFireworkTaskState *);
void updateStarlightFireworkComplex(StarlightFireworkTaskState *);
void updateStarlightFirework(StarlightFireworkTaskState *);
void cleanupStarlightFireworkTask(StarlightFireworkCleanupState *);
void func_800BC550_AE910(s16 *arg0);
void func_800BC768_AEB28(func_800BC6C4_AEA84_arg *arg0);

void initStarlightHighwayBuildingTask(StarlightBuildingTaskState *arg0) {
    arg0->unk24 = func_80055DC4_569C4(8);
    arg0->unk28 = func_80055DF8_569F8(8);
    arg0->unk2C = 0;
    createYRotationMatrix(&arg0->node1, 0x98A);
    arg0->node1.translation.x = 0x130F588E;
    arg0->node1.translation.y = 0x03930000;
    arg0->node1.translation.z = 0x0DB72F2C;
    arg0->unk68 = 0;
    arg0->unk60 = arg0->unk24;
    arg0->unk64 = arg0->unk28;
    createYRotationMatrix(&arg0->node2, 0);
    arg0->node2.translation.x = 0;
    arg0->node2.translation.y = 0x30000000;
    arg0->node2.translation.z = 0x30458CB2;
    setCleanupCallback(&cleanupStarlightHighwayBuildingTask);
    setCallback(&renderStarlightHighwayBuildings);
}

void renderStarlightHighwayBuildings(StarlightBuildingRenderData *arg0) {
    StarlightBuildingAllocation *allocation;
    s32 i;
    func_80055E68_56A68_result *displayListResult;

    allocation = (StarlightBuildingAllocation *)getCurrentAllocation();

    if (allocation->animateNode1 != 0) {
        if (gFrameCounter & 1) {
            displayListResult = func_80055E68_56A68(8);
            arg0->node1.displayLists = (DisplayLists *)((u32)displayListResult + 0x10);
        } else {
            displayListResult = func_80055E68_56A68(8);
            arg0->node1.displayLists = (DisplayLists *)((u32)displayListResult + 0x20);
        }
    } else {
        arg0->node1.displayLists = (DisplayLists *)func_80055E68_56A68(8);
    }

    if (allocation->animateNode2 != 0) {
        if (gFrameCounter & 1) {
            displayListResult = func_80055E68_56A68(8);
            arg0->node2.displayLists = (DisplayLists *)((u32)displayListResult + 0x10);
        } else {
            displayListResult = func_80055E68_56A68(8);
            arg0->node2.displayLists = (DisplayLists *)((u32)displayListResult + 0x20);
        }
    } else {
        arg0->node2.displayLists = (DisplayLists *)func_80055E68_56A68(8);
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
    arg0->displayListMemory1 = func_80055DC4_569C4(8);
    arg0->displayListMemory2 = func_80055DF8_569F8(8);
    arg0->unk2C = 0;
    arg0->displayListMemory3 = func_80055D7C_5697C(8);
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

    if (allocation->unk76 == 0) {
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

    if (allocation->unk76 == 0) {
        arg0->sustainTimer--;
    }

    if (arg0->sustainTimer == 0) {
        setCallback(&updateDebugDisplayListDecay);
    }

    renderDebugDisplayLists(arg0);
}

void updateDebugDisplayListDecay(DebugDisplayListRenderState *arg0) {
    DebugDisplayListAllocation *allocation = getCurrentAllocation();

    if (allocation->unk76 == 0) {
        arg0->animationTimer -= 0x10;
    }

    if (arg0->animationTimer == 0) {
        func_80069CF8_6A8F8();
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
        do { widthShift += 1; if (widthShift < 0x10) { tempWidth >>= 1; goto loop_1; } } while (0);
    }

    tempHeight = tableEntry.field2;
    heightShift = 0;
loop_2:
    if (!(tempHeight & 1)) {
        do { heightShift += 1; if (heightShift < 0x10) { tempHeight >>= 1; goto loop_2; } } while (0);
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

// Keep original name for auto-generated data file references
void func_800BB75C_ADB1C(void *arg) __attribute__((alias("loadColorIndexedTexture")));

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

// Keep original name for auto-generated data file references
void func_800BBB90(s16 arg0) __attribute__((alias("spawnDebugDisplayListTask")));

void initStarlightItemTask(StarlightItemTaskState *arg0) {
    arg0->segment1 = func_80055DC4_569C4(8);
    arg0->segment2 = func_80055DF8_569F8(8);
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
    s32 var_s0;      // playersChecked - counter for players that have been checked
    s32 var_s1;      // playerOffset - byte offset into player array
    s16 temp_v0;     // currentScale - holds the current scale value
    s16 temp_a1;     // scaleForMatrix - scale value to pass to scaleMatrix
    u16 temp_v0_2;   // newRotation - new rotation value after increment
    s32 temp;        // numPlayers - number of players in the game
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
    arg0->displayLists2 = (DisplayLists *)((arg0->displayLists1 = (DisplayLists *)((s32)func_80055E68_56A68(8) + 0xD0)),
                                   (s32)func_80055E68_56A68(8) + 0xE0);
    goto render;

shrinking_animation:
    // Shrinking display lists (collected state)
    arg0->displayLists2 = (DisplayLists *)((arg0->displayLists1 = (DisplayLists *)((s32)func_80055E68_56A68(8) + 0xC0)),
                                   (s32)func_80055E68_56A68(8) + 0xF0);

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

    arg0->segment1 = func_80055DC4_569C4(8);
    arg0->segment2 = func_80055DF8_569F8(8);
    arg0->unk2C = 0;
    arg0->displayList = (void *)((u32)func_80055E68_56A68(8) + 0x90);
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
    Allocation_AE790 *allocation;
    s32 i;
    void *posPtr;

    allocation = getCurrentAllocation();

    if (allocation->unk76 == 0) {
        arg0->pos.x += arg0->velocity.x;
        arg0->pos.y += arg0->velocity.y;
        arg0->pos.z += arg0->velocity.z;
        arg0->lifetime--;

        if (arg0->lifetime == 0) {
            func_80069CF8_6A8F8();
            return;
        }

        posPtr = &arg0->pos;
        if (func_8005C250_5CE50(posPtr, -1, 0x300000) != 0) {
            spawnSparkleEffect(posPtr);
            func_80069CF8_6A8F8();
        }

        arg0->rotX += gStarlightFireworkRotXSpeeds[arg0->type];
        arg0->rotY += gStarlightFireworkRotYSpeeds[arg0->type];
    }

    createCombinedRotationMatrix(arg0, arg0->rotX, arg0->rotY);

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
    }
}

void updateStarlightFireworkComplex(StarlightFireworkTaskState *arg0) {
    Allocation_AE790 *allocation;
    s32 i;
    s16 rotation[3][3];
    s16 pad2[4];
    void (*callback)(StarlightFireworkTaskState *);
    s16 timer;
    u8 state;
    void *posPtr;

    (void)pad2;

    allocation = getCurrentAllocation();

    if (allocation->unk76 == 0) {
        if (arg0->lifetime != 0) {
            arg0->pos.x = arg0->pos.x + arg0->velocity.x;
            arg0->pos.y = arg0->pos.y + arg0->velocity.y;
            arg0->pos.z = arg0->pos.z + arg0->velocity.z;
            arg0->lifetime--;
        } else {
            state = arg0->type;
            switch (state) {
                case 3:
                    createXRotationMatrix(rotation, 0xF300);
                    transformVector2(gStarlightFireworkDirections[arg0->type], rotation, &arg0->velocity);
                    callback = updateStarlightFireworkSimple;
                    timer = 0x78;
                    arg0->lifetime = timer;
                    setCallbackWithContinue(callback);
                    break;
                case 4:
                    createXRotationMatrix(rotation, 0xE00);
                    transformVector2(gStarlightFireworkDirections[arg0->type], rotation, &arg0->velocity);
                    callback = updateStarlightFirework;
                    arg0->lifetime = 0xA;
                    setCallbackWithContinue(callback);
                    break;
                case 5:
                    createXRotationMatrix(rotation, 0xFC00);
                    transformVector2(gStarlightFireworkDirections[arg0->type], rotation, &arg0->velocity);
                    callback = updateStarlightFireworkSimple;
                    timer = 0x78;
                    arg0->lifetime = timer;
                    setCallbackWithContinue(callback);
                    break;
                case 6:
                    createXRotationMatrix(rotation, 0);
                    transformVector2(gStarlightFireworkDirections[arg0->type], rotation, &arg0->velocity);
                    callback = updateStarlightFirework;
                    arg0->lifetime = 0xA;
                    setCallbackWithContinue(callback);
                    break;
                case 7:
                case 8:
                    spawnSparkleEffect(&arg0->pos);
                    func_80069CF8_6A8F8();
                    break;
                default:
                    break;
            }
        }
        posPtr = &arg0->pos;
        func_8005C250_5CE50(posPtr, -1, 0x300000);
        arg0->rotX += gStarlightFireworkRotXSpeeds[arg0->type];
        arg0->rotY += gStarlightFireworkRotYSpeeds[arg0->type];
    }

    createCombinedRotationMatrix(arg0, arg0->rotX, arg0->rotY);

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
    }
}

void updateStarlightFirework(StarlightFireworkTaskState *arg0) {
    Allocation_AE790 *allocation;
    s32 i;
    void *posPtr;

    allocation = getCurrentAllocation();

    if (allocation->unk76 == 0) {
        arg0->pos.x += arg0->velocity.x;
        arg0->pos.y += arg0->velocity.y;
        arg0->pos.z += arg0->velocity.z;
        arg0->lifetime--;

        if (arg0->lifetime == 0) {
            spawnSparkleEffect(&arg0->pos);
            func_80069CF8_6A8F8();
        } else {
            posPtr = &arg0->pos;
            if (func_8005C250_5CE50(posPtr, -1, 0x300000)) {
                spawnSparkleEffect(posPtr);
                func_80069CF8_6A8F8();
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

void func_800BC528_AE8E8(func_800BC528_AE8E8_arg *arg0) {
    arg0->unk0 = 0xF0;
    setCallback(&func_800BC550_AE910);
}

void func_800BC550_AE910(s16 *arg0) {
    Allocation_AE790 *allocation;
    StarlightFireworkTaskState *task;
    s32 new_var;
    s32 new_var2;
    u32 s2;

    allocation = getCurrentAllocation();
    if (allocation->unk76 != 0) {
        return;
    }

    *arg0 -= 1;
    if ((s16)*arg0 == 0x78) {
        task = scheduleTask(initStarlightFireworkTask, 0, 0, 0xC8);
        if (task != NULL) {
            task->type = (u32)(randA() & 0xFF) % 3U;
        }
    }

    if ((s16)(s2 = *arg0) == 0) {
        if (getFreeNodeCount(0) >= 2) {
            s2 = 2;
            s2 = (u8)((u32)(randA() & 0xFF) % 3U) * s2;
            new_var2 = s2 + 3;
            task = scheduleTask(initStarlightFireworkTask, 0, 0, 0xC8);
            if (task != NULL) {
                do {
                    task->type = (new_var = new_var2);
                } while (0);
            }

            task = scheduleTask(initStarlightFireworkTask, 0, 0, 0xC8);
            if (task != NULL) {
                task->type = s2 + 4;
            }
        }

        *arg0 = (randA() & 0x1F) + 0xF0;
    }
}

void func_800BC6C4_AEA84(func_800BC6C4_AEA84_arg *arg0) {
    void *temp;

    arg0->unk24 = func_80055DC4_569C4(8);
    arg0->unk28 = func_80055DF8_569F8(8);
    arg0->unk2C = 0;
    temp = func_80055E68_56A68(8);
    arg0->unk20 = (void *)((u32)temp + 0xA0);
    arg0->unk68 = 0;
    arg0->unk60 = arg0->unk24;
    arg0->unk64 = arg0->unk28;
    temp = func_80055E68_56A68(8);
    arg0->unk5C = (void *)((u32)temp + 0xB0);
    createYRotationMatrix(&arg0->node1, 0x1BEC);
    createYRotationMatrix(&arg0->node2, 0x1BEC);
    arg0->unk78 = 0;
    setCleanupCallback(&func_800BC984_AED44);
    setCallback(&func_800BC768_AEB28);
}

void func_800BC768_AEB28(func_800BC6C4_AEA84_arg *arg0) {
    Vec3i vec;
    s32 pad[2];
    s32 found;
    s32 i;
    u8 temp;
    s32 numPlayers;
    GameState *allocation;
    Player *player;
    s32 unk78Val;
    s32 tempS;

    allocation = getCurrentAllocation();
    temp = allocation->numPlayers;
    found = 0;
    i = 0;
    tempS = temp;

    if (tempS > 0) {
        numPlayers = tempS;
        player = allocation->players;
        do {
            if (player->sectorIndex == D_800BCBB0_AEF70[arg0->unk7C]) {
                found = 1;
            }
            i++;
            player++;
        } while (i < numPlayers);
    }

    if (found != 0) {
        if (allocation->gamePaused == 0) {
            if (arg0->unk78 != 0x600000) {
                arg0->unk78 = arg0->unk78 + 0x100000;
            }
        }
    } else if (allocation->gamePaused == 0) {
        if (arg0->unk78 != 0) {
            arg0->unk78 = arg0->unk78 - 0x100000;
        }
    }

    unk78Val = arg0->unk78;
    vec.y = 0;
    vec.z = 0;
    vec.x = -unk78Val;
    rotateVectorY(&vec, 0x1BEC, &arg0->node1.translation);
    vec.x = arg0->unk78;
    rotateVectorY(&vec, 0x1BEC, &arg0->node2.translation);

    arg0->node1.translation.x = arg0->node1.translation.x + D_800BCB98_AEF58[arg0->unk7C][0];
    arg0->node1.translation.y = arg0->node1.translation.y + D_800BCB9C_AEF5C[arg0->unk7C][0];
    arg0->node1.translation.z = arg0->node1.translation.z + D_800BCBA0_AEF60[arg0->unk7C][0];
    arg0->node2.translation.x = arg0->node2.translation.x + D_800BCB98_AEF58[arg0->unk7C][0];
    arg0->node2.translation.y = arg0->node2.translation.y + D_800BCB9C_AEF5C[arg0->unk7C][0];
    i = 0;
    arg0->node2.translation.z = arg0->node2.translation.z + D_800BCBA0_AEF60[arg0->unk7C][0];

    do {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)&arg0->node1);
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)&arg0->node2);
        i++;
    } while (i < 4);

    (void)pad;
}

void func_800BC984_AED44(DualSegmentCleanupState *arg0) {
    arg0->segment1 = freeNodeMemory(arg0->segment1);
    arg0->segment2 = freeNodeMemory(arg0->segment2);
}

void func_800BC9BC(void) {
    func_800BC6C4_AEA84_arg *task;

    task = (func_800BC6C4_AEA84_arg *)scheduleTask(func_800BC6C4_AEA84, 0, 0, 0xC8);
    if (task != NULL) {
        task->unk7C = 0;
    }

    task = (func_800BC6C4_AEA84_arg *)scheduleTask(func_800BC6C4_AEA84, 0, 0, 0xC8);
    if (task != NULL) {
        task->unk7C = 1;
    }
}
