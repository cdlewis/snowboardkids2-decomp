#include "levels/starlight_highway.h"
#include "audio/audio.h"
#include "common.h"
#include "data/asset_metadata.h"
#include "data/course_data.h"
#include "data/data_table.h"
#include "gamestate.h"
#include "gbi.h"
#include "graphics/displaylist.h"
#include "graphics/graphics.h"
#include "math/geometry.h"
#include "math/rand.h"
#include "race/particle_items.h"
#include "race/track_collision.h"
#include "system/task_scheduler.h"

extern Gfx *gDisplayListAllocPtr;

typedef struct {
    s16 rotation[3][3];
    u8 pad2[0xE];
} FireworkInitRotationMatrix;

Vec3i D_800BCA30_AEDF0[] = {
    { 0x3283FD94, 0x0AEC1B32, 0xF69F18F2 },
    { 0x30D13944, 0x0AD1B785, 0xF735E7FA },
    { 0x3246C370, 0x0AC7B8F9, 0xF8E01B6E },
    { 0x2EA90BB0, 0x0A95E21F, 0xF9BAEF6E },
    { 0x30568F30, 0x0AAF6437, 0xF92EF006 },
    { 0x2FC010FC, 0x0A8B78B9, 0xFB2B23CA },
    { 0x3142ADDC, 0x0AA1C4AA, 0xFAB8D082 },
};

Vec3i gStarlightFireworkPositions[] = {
    { 0x238C6C87, 0x1472AC5C, 0xE47D2726 },
    { 0x238C6C87, 0x1472AC5C, 0xE47D2726 },
    { 0x238C6C87, 0x1472AC5C, 0xE47D2726 },
    { 0x30FC2FC7, 0x12259A40, 0xE87CB745 },
    { 0x312EA2DD, 0x12354C23, 0xE83B215E },
    { 0x30FC2FC7, 0x12259A40, 0xE87CB745 },
    { 0x31184770, 0x1286480B, 0xE8735DE5 },
    { 0x30FC2FC7, 0x12259A40, 0xE87CB745 },
    { 0x312EA2DD, 0x12354C23, 0xE83B215E },
};

s16 gStarlightFireworkXRotations[] = {
    0x0180, 0x0000, 0xFE80, 0xFE00, 0x0180, 0xFE00, 0x0180, 0xFE80, 0x0180, 0x0000,
};

Vec3i gStarlightFireworkDirections[] = {
    { 0x00000000, 0x00000000, 0x00100000 },
    { 0x00000000, 0x00000000, 0x00100000 },
    { 0x00000000, 0x00000000, 0xFFF00000 },
    { 0x00000000, 0x00000000, 0xFFF00000 },
    { 0x00000000, 0x00000000, 0x00140000 },
    { 0x00000000, 0x00000000, 0xFFEC0000 },
    { 0x00000000, 0x00000000, 0x00100000 },
    { 0x00000000, 0x00000000, 0xFFE80000 },
    { 0x00000000, 0x00000000, 0x00180000 },
};

s16 gStarlightFireworkRotXSpeeds[] = {
    0x0040, 0x0010, 0xFFA0, 0xFEE0, 0x0100, 0x0100, 0xFEC0, 0x0100, 0xFF00, 0x0000,
};

s16 gStarlightFireworkRotYSpeeds[] = {
    0x0010, 0x0030, 0xFFF0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

Vec3i gStarlightBarrierPositions[] = {
    { 0x0B1CAE7B, 0x1EC852A1, 0xF7713B01 },
    { 0x0D503C28, 0x1E6981DD, 0xF54EDDEF },
};

s16 gStarlightBarrierTriggerSectors[] = {
    0x003E, 0x003F, 0, 0, 0, 0, 0, 0,
};

void cleanupStarlightBarrierTask(DisplayListObject *);
void cleanupDebugDisplayListTask(StarlightDebugDisplayTaskState *);
void updateDebugDisplayListGrowth(StarlightDebugDisplayTaskState *);
void renderStarlightHighwayBuildings(StarlightBuildingTaskState *);
void cleanupStarlightHighwayBuildingTask(DisplayListObject *);
void loadColorIndexedTexture(void *);
void updateStarlightFireworkSimple(StarlightFireworkTaskState *);
void updateStarlightFireworkComplex(StarlightFireworkTaskState *);
void updateStarlightFirework(StarlightFireworkTaskState *);
void cleanupStarlightFireworkTask(DisplayListObject *);
void updateStarlightBarrier(StarlightBarrierTaskState *arg0);
void renderColorIndexedOpaqueDisplayList(void *);
void renderColorIndexedTransparentDisplayList(void *);
void renderColorIndexedOverlayDisplayList(void *);
void renderDebugDisplayLists(StarlightDebugDisplayTaskState *arg0);
void updateDebugDisplayListSustain(StarlightDebugDisplayTaskState *arg0);
void updateDebugDisplayListDecay(StarlightDebugDisplayTaskState *arg0);

void initStarlightHighwayBuildingTask(StarlightBuildingTaskState *task) {
    task->building1.segment1 = loadUncompressedAssetByIndex(8);
    task->building1.segment2 = loadCompressedSegment2AssetByIndex(8);
    task->building1.segment3 = 0;
    createYRotationMatrix(&task->building1.transform, 0x98A);
    task->building1.transform.translation.x = 0x130F588E;
    task->building1.transform.translation.y = 0x03930000;
    task->building1.transform.translation.z = 0x0DB72F2C;
    task->building2.segment3 = 0;
    task->building2.segment1 = task->building1.segment1;
    task->building2.segment2 = task->building1.segment2;
    createYRotationMatrix(&task->building2.transform, 0);
    task->building2.transform.translation.x = 0;
    task->building2.transform.translation.y = 0x30000000;
    task->building2.transform.translation.z = 0x30458CB2;
    setCleanupCallback(&cleanupStarlightHighwayBuildingTask);
    setCallback(&renderStarlightHighwayBuildings);
}

void renderStarlightHighwayBuildings(StarlightBuildingTaskState *arg0) {
    GameState *gameState;
    s32 i;
    LevelDisplayLists *displayListResult;

    gameState = getCurrentAllocation();

    if (gameState->fadeInPlayerCount != 0) {
        if (gFrameCounter & 1) {
            displayListResult = getSkyDisplayLists3ByIndex(8);
            arg0->building1.displayLists = (DisplayLists *)((u32)displayListResult + 0x10);
        } else {
            displayListResult = getSkyDisplayLists3ByIndex(8);
            arg0->building1.displayLists = (DisplayLists *)((u32)displayListResult + 0x20);
        }
    } else {
        arg0->building1.displayLists = (DisplayLists *)getSkyDisplayLists3ByIndex(8);
    }

    if (gameState->shortcutWarpPlayerCount != 0) {
        if (gFrameCounter & 1) {
            displayListResult = getSkyDisplayLists3ByIndex(8);
            arg0->building2.displayLists = (DisplayLists *)((u32)displayListResult + 0x10);
        } else {
            displayListResult = getSkyDisplayLists3ByIndex(8);
            arg0->building2.displayLists = (DisplayLists *)((u32)displayListResult + 0x20);
        }
    } else {
        arg0->building2.displayLists = (DisplayLists *)getSkyDisplayLists3ByIndex(8);
    }

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, &arg0->building1);
        enqueueDisplayListWithFrustumCull(i, &arg0->building2);
    }
}

void cleanupStarlightHighwayBuildingTask(DisplayListObject *arg0) {
    arg0->segment1 = freeNodeMemory(arg0->segment1);
    arg0->segment2 = freeNodeMemory(arg0->segment2);
}

void initDebugDisplayListTask(StarlightDebugDisplayTaskState *arg0) {
    arg0->displayObject.segment1 = loadUncompressedAssetByIndex(8);
    arg0->displayObject.segment2 = loadCompressedSegment2AssetByIndex(8);
    arg0->displayObject.segment3 = 0;
    arg0->textureTable = loadSpriteAssetByIndex(8);
    arg0->displayObject.displayLists = &gDebugDisplayConfig;
    arg0->textureIndex = 0;
    arg0->textureUSpeed = 8;
    arg0->textureVSpeed = 0;
    arg0->textureUOffset = 0;
    arg0->textureVOffset = 0;
    arg0->fade.timer = 0;
    setCleanupCallback(&cleanupDebugDisplayListTask);
    setCallback(&updateDebugDisplayListGrowth);
}

void renderDebugDisplayLists(StarlightDebugDisplayTaskState *arg0) {
    s32 i;

    arg0->textureUOffset += arg0->textureUSpeed;
    arg0->textureVOffset = arg0->textureVOffset + arg0->textureVSpeed;
    arg0->displayObject.transformMatrix = 0;
    arg0->textureUOffset = arg0->textureUOffset & 0xFF;
    arg0->textureVOffset = arg0->textureVOffset & 0xFF;
    ;

    for (i = 0; i < 4; i++) {
        if (arg0->displayObject.displayLists->opaqueDisplayList != NULL) {
            enqueueCallbackBySlotIndex(i & 0xFFFF, 1, renderColorIndexedOpaqueDisplayList, arg0);
        }

        if (arg0->displayObject.displayLists->transparentDisplayList != NULL) {
            enqueueCallbackBySlotIndex(i & 0xFFFF, 3, renderColorIndexedTransparentDisplayList, arg0);
        }

        if (arg0->displayObject.displayLists->overlayDisplayList == NULL) {
            continue;
        }

        enqueueCallbackBySlotIndex(i & 0xFFFF, 5, renderColorIndexedOverlayDisplayList, arg0);
    }
}

void updateDebugDisplayListGrowth(StarlightDebugDisplayTaskState *arg0) {
    GameState *gameState = getCurrentAllocation();

    if (gameState->gamePaused == 0) {
        arg0->fade.timer += 0x10;
    }

    if (arg0->fade.timer == 0xC0) {
        setCallback(&updateDebugDisplayListSustain);
        arg0->sustainTimer = 0x40;
    }

    renderDebugDisplayLists(arg0);
}

void updateDebugDisplayListSustain(StarlightDebugDisplayTaskState *arg0) {
    GameState *gameState = getCurrentAllocation();

    if (gameState->gamePaused == 0) {
        arg0->sustainTimer--;
    }

    if (arg0->sustainTimer == 0) {
        setCallback(&updateDebugDisplayListDecay);
    }

    renderDebugDisplayLists(arg0);
}

void updateDebugDisplayListDecay(StarlightDebugDisplayTaskState *arg0) {
    GameState *gameState = getCurrentAllocation();

    if (gameState->gamePaused == 0) {
        arg0->fade.timer -= 0x10;
    }

    if (arg0->fade.timer == 0) {
        terminateCurrentTask();
    }

    renderDebugDisplayLists(arg0);
}

void cleanupDebugDisplayListTask(StarlightDebugDisplayTaskState *arg0) {
    arg0->displayObject.segment1 = freeNodeMemory(arg0->displayObject.segment1);
    arg0->displayObject.segment2 = freeNodeMemory(arg0->displayObject.segment2);
    arg0->textureTable = freeNodeMemory(arg0->textureTable);
}

void loadColorIndexedTexture(void *arg) {
    StarlightDebugDisplayTaskState *state = arg;
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

    gDPPipeSync(gDisplayListAllocPtr++);
    gDPSetTextureLUT(gDisplayListAllocPtr++, G_TT_RGBA16);
    gGraphicsMode = -1;

    getTableEntryByU16Index(state->textureTable, state->textureIndex, &tableEntry);

    tempWidth = tableEntry.width;
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

    tempHeight = tableEntry.height;
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

    gDPSetTextureImage(gDisplayListAllocPtr++, G_IM_FMT_CI, G_IM_SIZ_16b, 1, tableEntry.data_ptr);

    gDPSetTile(
        gDisplayListAllocPtr++,
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

    gDPLoadSync(gDisplayListAllocPtr++);

    loadBlockCmd = gDisplayListAllocPtr++;
    loadBlockCmd->words.w0 = 0xF3000000;
    widthDiv16 = tableEntry.width >> 4;
    dxtBase = 0x800;
    if (widthDiv16 != 0) {
        dxtBase = widthDiv16 + 0x7FF;
    }
    lrs = (((s32)((tableEntry.width * tableEntry.height) + 3)) >> 2) - 1;
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

    gDPPipeSync(gDisplayListAllocPtr++);

    tileLine = (((tableEntry.width >> 1) + 7) >> 3) & 0x1FF;
    gDPSetTile(
        gDisplayListAllocPtr++,
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
        gDisplayListAllocPtr++,
        G_TX_RENDERTILE,
        0,
        0,
        (tableEntry.width - 1) << 2,
        (tableEntry.height - 1) << 2
    );

    gDPSetTileSize(
        gDisplayListAllocPtr++,
        G_TX_RENDERTILE,
        state->textureUOffset & 0xFFF,
        state->textureVOffset & 0xFFF,
        ((tableEntry.width + (s16)state->textureUOffset - 1) << 2) & 0xFFF,
        ((tableEntry.height + (s16)state->textureVOffset - 1) << 2) & 0xFFF
    );

    gDPSetTextureImage(gDisplayListAllocPtr++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, tableEntry.index_ptr);

    gDPTileSync(gDisplayListAllocPtr++);

    gDPSetTile(
        gDisplayListAllocPtr++,
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

    gDPLoadSync(gDisplayListAllocPtr++);

    gDPLoadTLUTCmd(gDisplayListAllocPtr++, G_TX_LOADTILE, loadTileParams);

    gDPPipeSync(gDisplayListAllocPtr++);

    gDPSetEnvColor(gDisplayListAllocPtr++, 0xFF, 0xFF, 0xFF, state->fade.color.alpha);
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

void spawnDebugDisplayListTask(s16 arg0) {
    StarlightDebugDisplayTaskState *task;

    task = scheduleTask(initDebugDisplayListTask, 0, 0, 0xD2);
    if (task != NULL) {
        if (arg0 == 0) {
            createYRotationMatrix(&task->displayObject.transform, 0x98A);
            task->displayObject.transform.translation.x = 0x130F588E;
            task->displayObject.transform.translation.y = 0x03930000;
            task->displayObject.transform.translation.z = 0x0DB72F2C;
        } else {
            createYRotationMatrix(&task->displayObject.transform, 0);
            task->displayObject.transform.translation.x = 0;
            task->displayObject.transform.translation.y = 0x30000000;
            task->displayObject.transform.translation.z = 0x30458CB2;
        }
    }
}

void initStarlightItemTask(StarlightItemTaskState *arg0) {
    arg0->item.segment1 = loadUncompressedAssetByIndex(8);
    arg0->item.segment2 = loadCompressedSegment2AssetByIndex(8);
    arg0->item.segment3 = 0;
    memcpy(&arg0->item.transform.translation, &D_800BCA30_AEDF0[arg0->positionIndex], sizeof(Vec3i));
    arg0->shadow.segment3 = 0;
    arg0->item.transform.translation.y += 0x100000;
    arg0->shadow.segment1 = arg0->item.segment1;
    arg0->shadow.segment2 = arg0->item.segment2;
    createYRotationMatrix(&arg0->shadow.transform, 0);
    arg0->scale = 0x2000;
    arg0->shadow.transform.translation.x = arg0->item.transform.translation.x;
    arg0->shadow.transform.translation.y = arg0->item.transform.translation.y + 0x180000;
    arg0->shadow.transform.translation.z = arg0->item.transform.translation.z;
    setCleanupCallback(&cleanupStarlightItemTask);
    setCallback(&updateStarlightItemTask);
}

// Update and render the starlight item (power-up) on Starlight Highway
// Checks for player proximity, handles collection animation, and renders the item with shadow
void updateStarlightItemTask(StarlightItemTaskState *arg0) {
    GameState *allocation;
    s32 playersChecked;
    s32 playerByteOffset;
    s16 temp_v0;   // currentScale - holds the current scale value
    s16 temp_a1;   // scaleForMatrix - scale value to pass to scaleMatrix
    u16 temp_v0_2; // newRotation - new rotation value after increment
    s32 temp;      // numPlayers - number of players in the game
    u8 pad[0x10];

    (void)pad;

    allocation = (GameState *)getCurrentAllocation();
    playersChecked = 0;
    temp = allocation->numPlayers;
    if (temp > 0) {
        playerByteOffset = 0;
    // Check each player to see if any are in range (0x200000 = close range)
    check_player_range:
        if (isPlayerInRangeAndPull(
                &arg0->item.transform.translation,
                0x200000,
                (Player *)((u8 *)allocation->players + playerByteOffset)
            ) == 0) {
            playersChecked += 1;
            playerByteOffset += sizeof(Player);
            if (playersChecked < (s32)allocation->numPlayers) {
                goto check_player_range;
            }
        }
    }

    // If a player is in range and item is at base scale, trigger collection animation
    if ((playersChecked != allocation->numPlayers) && (arg0->scale == 0x2000)) {
        playersChecked = 0;
        if (allocation->numPlayers != 0) {
            playerByteOffset = 0;
            do {
                // Pull players closer (0x500000 = attraction range)
                isPlayerInRangeAndPull(
                    &arg0->item.transform.translation,
                    0x500000,
                    (Player *)((u8 *)allocation->players + playerByteOffset)
                );
                playersChecked += 1;
                playerByteOffset += sizeof(Player);
            } while (playersChecked < (s32)allocation->numPlayers);
        }
        arg0->scale = 0x4000;
        queueSoundAtPosition(&arg0->item.transform.translation, 0x4E);
    }

    // Rotate the item
    temp_v0_2 = arg0->rotation + 0x100;
    arg0->rotation = temp_v0_2;
    createYRotationMatrix(&arg0->item.transform, temp_v0_2 & 0xFFFF);

    // Handle shrinking animation if item was collected
    temp_v0 = arg0->scale;
    if (temp_v0 != 0x2000) {
        if (allocation->gamePaused == 0) {
            arg0->scale = (s16)(temp_v0 - 0x800);
        }
        temp_a1 = arg0->scale;
        scaleMatrix(&arg0->item.transform, temp_a1, 0x2000, temp_a1);
        if (arg0->scale != 0x2000) {
            goto shrinking_animation;
        }
    }
    // Full-size display lists (normal state)
    arg0->shadow.displayLists =
        (DisplayLists *)((arg0->item.displayLists = (DisplayLists *)((s32)getSkyDisplayLists3ByIndex(8) + 0xD0)),
                         (s32)getSkyDisplayLists3ByIndex(8) + 0xE0);
    goto render;

shrinking_animation:
    // Shrinking display lists (collected state)
    arg0->shadow.displayLists =
        (DisplayLists *)((arg0->item.displayLists = (DisplayLists *)((s32)getSkyDisplayLists3ByIndex(8) + 0xC0)),
                         (s32)getSkyDisplayLists3ByIndex(8) + 0xF0);

render:
    playersChecked = 0;
    do {
        enqueueDisplayListWithFrustumCull(playersChecked, &arg0->item);
        enqueueDisplayListWithFrustumCull(playersChecked, &arg0->shadow);
        playersChecked += 1;
    } while (playersChecked < 4);
}

void cleanupStarlightItemTask(DisplayListObject *arg0) {
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

    arg0->displayObject.segment1 = loadUncompressedAssetByIndex(8);
    arg0->displayObject.segment2 = loadCompressedSegment2AssetByIndex(8);
    arg0->displayObject.segment3 = 0;
    arg0->displayObject.displayLists = (DisplayLists *)((u32)getSkyDisplayLists3ByIndex(8) + 0x90);
    memcpy(&arg0->displayObject.transform.translation, &gStarlightFireworkPositions[arg0->type], sizeof(Vec3i));
    rotPtr = stack.rotation;
    createXRotationMatrix(rotPtr, gStarlightFireworkXRotations[arg0->type]);
    transformVector2(&gStarlightFireworkDirections[arg0->type], rotPtr, &arg0->velocity);
    arg0->displayObject.transform.translation.x -= arg0->velocity.x * 0x78;
    arg0->displayObject.transform.translation.y -= arg0->velocity.y * 0x78;
    arg0->displayObject.transform.translation.z -= arg0->velocity.z * 0x78;
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
    GameState *gameState;
    s32 i;
    void *posPtr;

    gameState = getCurrentAllocation();

    if (gameState->gamePaused == 0) {
        arg0->displayObject.transform.translation.x += arg0->velocity.x;
        arg0->displayObject.transform.translation.y += arg0->velocity.y;
        arg0->displayObject.transform.translation.z += arg0->velocity.z;
        arg0->lifetime--;

        if (arg0->lifetime == 0) {
            terminateCurrentTask();
            return;
        }

        posPtr = &arg0->displayObject.transform.translation;
        if (checkStarHitCollisionWithVulnerablePlayers(posPtr, -1, 0x300000) != 0) {
            spawnSparkleEffect(posPtr);
            terminateCurrentTask();
        }

        arg0->rotationX += gStarlightFireworkRotXSpeeds[arg0->type];
        arg0->rotationY += gStarlightFireworkRotYSpeeds[arg0->type];
    }

    createCombinedRotationMatrix(&arg0->displayObject.transform, arg0->rotationX, arg0->rotationY);

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, &arg0->displayObject);
    }
}

void updateStarlightFireworkComplex(StarlightFireworkTaskState *firework) {
    GameState *gameState;
    s32 i;
    s16 rotation[3][3];
    s16 pad2[4];
    u8 fireworkType;

    (void)pad2;

    gameState = getCurrentAllocation();

    if (gameState->gamePaused == 0) {
        if (firework->lifetime != 0) {
            firework->displayObject.transform.translation.x += firework->velocity.x;
            firework->displayObject.transform.translation.y += firework->velocity.y;
            firework->displayObject.transform.translation.z += firework->velocity.z;
            firework->lifetime--;
        } else {
            fireworkType = firework->type;
            switch (fireworkType) {
                case 3:
                    createXRotationMatrix(rotation, 0xF300);
                    transformVector2(&gStarlightFireworkDirections[firework->type], rotation, &firework->velocity);
                    firework->lifetime = 0x78;
                    setCallbackWithContinue(updateStarlightFireworkSimple);
                    break;
                case 4:
                    createXRotationMatrix(rotation, 0xE00);
                    transformVector2(&gStarlightFireworkDirections[firework->type], rotation, &firework->velocity);
                    firework->lifetime = 0xA;
                    setCallbackWithContinue(updateStarlightFirework);
                    break;
                case 5:
                    createXRotationMatrix(rotation, 0xFC00);
                    transformVector2(&gStarlightFireworkDirections[firework->type], rotation, &firework->velocity);
                    firework->lifetime = 0x78;
                    setCallbackWithContinue(updateStarlightFireworkSimple);
                    break;
                case 6:
                    createXRotationMatrix(rotation, 0);
                    transformVector2(&gStarlightFireworkDirections[firework->type], rotation, &firework->velocity);
                    firework->lifetime = 0xA;
                    setCallbackWithContinue(updateStarlightFirework);
                    break;
                case 7:
                case 8:
                    spawnSparkleEffect(&firework->displayObject.transform.translation);
                    terminateCurrentTask();
                    break;
                default:
                    break;
            }
        }
        checkStarHitCollisionWithVulnerablePlayers(&firework->displayObject.transform.translation, -1, 0x300000);
        firework->rotationX += gStarlightFireworkRotXSpeeds[firework->type];
        firework->rotationY += gStarlightFireworkRotYSpeeds[firework->type];
    }

    createCombinedRotationMatrix(&firework->displayObject.transform, firework->rotationX, firework->rotationY);

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, &firework->displayObject);
    }
}

void updateStarlightFirework(StarlightFireworkTaskState *arg0) {
    GameState *gameState;
    s32 i;
    void *posPtr;

    gameState = getCurrentAllocation();

    if (gameState->gamePaused == 0) {
        arg0->displayObject.transform.translation.x += arg0->velocity.x;
        arg0->displayObject.transform.translation.y += arg0->velocity.y;
        arg0->displayObject.transform.translation.z += arg0->velocity.z;
        arg0->lifetime--;

        if (arg0->lifetime == 0) {
            spawnSparkleEffect(&arg0->displayObject.transform.translation);
            terminateCurrentTask();
        } else {
            posPtr = &arg0->displayObject.transform.translation;
            if (checkStarHitCollisionWithVulnerablePlayers(posPtr, -1, 0x300000)) {
                spawnSparkleEffect(posPtr);
                terminateCurrentTask();
            }
        }

        arg0->rotationX += gStarlightFireworkRotXSpeeds[arg0->type];
        arg0->rotationY += gStarlightFireworkRotYSpeeds[arg0->type];
    }

    createCombinedRotationMatrix(&arg0->displayObject.transform, arg0->rotationX, arg0->rotationY);

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, &arg0->displayObject);
    }
}

void cleanupStarlightFireworkTask(DisplayListObject *arg0) {
    arg0->segment1 = freeNodeMemory(arg0->segment1);
    arg0->segment2 = freeNodeMemory(arg0->segment2);
}

void initFireworkShowTimer(FireworkShowTimerState *arg0) {
    arg0->timer = 0xF0;
    setCallback(&updateFireworkShowTimer);
}

void updateFireworkShowTimer(FireworkShowTimerState *arg0) {
    GameState *gameState;
    StarlightFireworkTaskState *task;
    s32 fireworkTypeBase;
    s32 firstFireworkType;
    u32 timerValue;

    gameState = getCurrentAllocation();
    if (gameState->gamePaused != 0) {
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

void initStarlightBarrierTask(StarlightBarrierTaskState *arg0) {
    void *temp;

    arg0->leftGate.segment1 = loadUncompressedAssetByIndex(8);
    arg0->leftGate.segment2 = loadCompressedSegment2AssetByIndex(8);
    arg0->leftGate.segment3 = 0;
    temp = getSkyDisplayLists3ByIndex(8);
    arg0->leftGate.displayLists = (DisplayLists *)((u32)temp + 0xA0);
    arg0->rightGate.segment3 = 0;
    arg0->rightGate.segment1 = arg0->leftGate.segment1;
    arg0->rightGate.segment2 = arg0->leftGate.segment2;
    temp = getSkyDisplayLists3ByIndex(8);
    arg0->rightGate.displayLists = (DisplayLists *)((u32)temp + 0xB0);
    createYRotationMatrix(&arg0->leftGate.transform, 0x1BEC);
    createYRotationMatrix(&arg0->rightGate.transform, 0x1BEC);
    arg0->openAmount = 0;
    setCleanupCallback(&cleanupStarlightBarrierTask);
    setCallback(&updateStarlightBarrier);
}

// Update barrier animation: open/close based on player proximity to trigger sector
void updateStarlightBarrier(StarlightBarrierTaskState *arg0) {
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
            if (arg0->openAmount != 0x600000) {
                arg0->openAmount += 0x100000;
            }
        }
    } else if (allocation->gamePaused == 0) {
        if (arg0->openAmount != 0) {
            arg0->openAmount -= 0x100000;
        }
    }

    // Apply the open amount to both gate positions
    gateOpenAmount = arg0->openAmount;
    vec.y = 0;
    vec.z = 0;
    vec.x = -gateOpenAmount;
    rotateVectorY(&vec, 0x1BEC, &arg0->leftGate.transform.translation);
    vec.x = arg0->openAmount;
    rotateVectorY(&vec, 0x1BEC, &arg0->rightGate.transform.translation);

    // Add base position offset for the barrier
    arg0->leftGate.transform.translation.x += gStarlightBarrierPositions[arg0->barrierIndex].x;
    arg0->leftGate.transform.translation.y += gStarlightBarrierPositions[arg0->barrierIndex].y;
    arg0->leftGate.transform.translation.z += gStarlightBarrierPositions[arg0->barrierIndex].z;
    arg0->rightGate.transform.translation.x += gStarlightBarrierPositions[arg0->barrierIndex].x;
    arg0->rightGate.transform.translation.y += gStarlightBarrierPositions[arg0->barrierIndex].y;
    i = 0;
    arg0->rightGate.transform.translation.z += gStarlightBarrierPositions[arg0->barrierIndex].z;

    do {
        enqueueDisplayListWithFrustumCull(i, &arg0->leftGate);
        enqueueDisplayListWithFrustumCull(i, &arg0->rightGate);
        i++;
    } while (i < 4);

    (void)pad;
}

void cleanupStarlightBarrierTask(DisplayListObject *arg0) {
    arg0->segment1 = freeNodeMemory(arg0->segment1);
    arg0->segment2 = freeNodeMemory(arg0->segment2);
}

// Spawn the two Starlight Highway barriers
// Creates two barrier tasks with indices 0 and 1
void spawnStarlightBarriers(void) {
    StarlightBarrierTaskState *task;

    task = scheduleTask(initStarlightBarrierTask, 0, 0, 0xC8);
    if (task != NULL) {
        task->barrierIndex = 0;
    }

    task = scheduleTask(initStarlightBarrierTask, 0, 0, 0xC8);
    if (task != NULL) {
        task->barrierIndex = 1;
    }
}
