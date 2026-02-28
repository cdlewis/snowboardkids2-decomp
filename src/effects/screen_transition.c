#include "36BE0.h"
#include "5520.h"
#include "assets.h"
#include "common.h"
#include "displaylist.h"
#include "geometry.h"
#include "rom_loader.h"
#include "task_scheduler.h"

typedef struct {
    s32 reserved;
    void *modelData;
} TrickSpriteEffectCleanupState;

typedef struct {
    Node n;
    u8 padding[0x28];
    u16 renderLayer;
    s16 colorIndex;
    s16 effectMode;
} ScrollingTextureEffectTask;

typedef struct {
    struct {
        u8 padding[0x2C];
        s32 posX;
        s32 posY;
        s32 posZ;
    } *positionData;
    DataTable_19E80 *modelData;
    loadAssetMetadataByIndex_arg assetMetadata;
    s32 offsetX;
    loadAssetMetadata_arg *sprite2;
    s32 spriteOffsetX;
    s32 spriteOffsetY;
    s32 spriteOffsetZ;
    u8 *dataPtr;
    TableEntry_19E80 *indexPtr;
    u8 alpha;
    u8 animFrame;
    u8 alphaDecay;
    u8 padding43;
    s32 unk44;
    s32 sprite1OffsetX;
    s32 sprite1OffsetY;
    s32 sprite1OffsetZ;
    s32 sprite2OffsetX;
    s32 sprite2OffsetY;
    s32 sprite2OffsetZ;
    s16 frameIndex;
    s16 effectParam;
} TrickSpriteEffectUpdateState;

typedef struct {
    struct {
        struct {
            u8 padding[0x3C0];
            s16 rotationY;
        } *modelPtr;
        u8 padding[0x28];
        s32 posX;
        s32 posY;
        s32 posZ;
    } *positionData;
    void *modelData;
    s32 *vertexData;
    char pad[0x16];
    u8 alpha;
    char pad2[5];
    s32 *vertexDataCopy;
    char pad3[0x16];
    u8 alphaCopy;
    char pad4[5];
    Vec3i spritePositions[2];
    u16 frameIndex;
    u16 effectParam;
} TrickSpriteEffectInitState;

typedef struct {
    DisplayListObject displayListObj;
    void *unk3C;
    s32 unk40;
    s16 scrollOffsetX;
    s16 scrollOffsetY;
    s16 scrollSpeedX;
    s16 scrollSpeedY;
    s16 unk4C;
    s16 unk4E;
    s16 unk50;
    s16 unk52;
    u16 renderLayer;
    s16 framesRemaining;
    s16 effectMode;
} ScrollingTextureEffectUpdateState;

void updateScrollingTextureEffect(ScrollingTextureEffectUpdateState *);
void cleanupScrollingTextureEffectTask(ScrollingTextureState *);
void initScrollingTextureEffectTask(ScrollingTextureState *);
void updateTrickSpriteEffect(TrickSpriteEffectUpdateState *);
void cleanupTrickSpriteEffectTask(TrickSpriteEffectCleanupState *);
void initTrickSpriteEffectTask(TrickSpriteEffectInitState *);

extern DisplayLists D_80089510_8A110;

u32 gTrickSpriteEffectVertexData[] = { 0xFFE80018, 0x00000000, 0xFFF0FFF0, 0xFFFFFFFF, 0x00180018, 0x00000000,
                                       0x07F0FFF0, 0xFFFFFFFF, 0x0018FFE8, 0x00000000, 0x07F007F0, 0xFFFFFFFF,
                                       0xFFE8FFE8, 0x00000000, 0xFFF007F0, 0xFFFFFFFF, 0x00040000, 0x00000000,
                                       0x000D0000, 0xFFFC0000, 0x00000000, 0x000D0000 };

s32 gTrickSpriteEffectTransformData[2][3] = {
    { 0xFFFC0000, 0x00000000, 0xFFF30000 },
    { 0x00040000, 0x00000000, 0xFFF30000 }
};

void initTrickSpriteEffectTask(TrickSpriteEffectInitState *initState) {
    s32 i;

    initState->modelData = loadCompressedData(&_646DF0_ROM_START, &_646DF0_ROM_END, 0x1188);
    initState->vertexData = (s32 *)&gTrickSpriteEffectVertexData;
    initState->alpha = 0xFF;

    initState->vertexDataCopy = initState->vertexData;
    initState->alphaCopy = initState->alpha;

    for (i = 0; i < 2; i++) {
        // Cast is required - transformData is s32 but function expects s16*
        // This size difference helps the data segment match correctly
        transformVector(
            (s16 *)gTrickSpriteEffectTransformData[i],
            &initState->positionData->modelPtr->rotationY,
            &initState->spritePositions[i]
        );
        initState->spritePositions[i].x -= initState->positionData->posX;
        initState->spritePositions[i].y -= initState->positionData->posY;
        initState->spritePositions[i].z -= initState->positionData->posZ;
    }

    initState->frameIndex = 0;
    initState->effectParam = initState->effectParam;

    setCleanupCallback(&cleanupTrickSpriteEffectTask);
    setCallbackWithContinue(&updateTrickSpriteEffect);
}

void updateTrickSpriteEffect(TrickSpriteEffectUpdateState *state) {
    loadAssetMetadataByIndex(&state->assetMetadata, state->modelData, state->frameIndex, state->effectParam);
    state->dataPtr = state->assetMetadata.data_ptr;
    state->indexPtr = state->assetMetadata.index_ptr;
    state->alpha = state->assetMetadata.unk18;
    state->animFrame = state->assetMetadata.unk19;
    state->assetMetadata.unk4 = state->sprite1OffsetX + state->positionData->posX;
    state->assetMetadata.unk8 = state->sprite1OffsetY + state->positionData->posY;
    state->assetMetadata.unkC = state->sprite1OffsetZ + state->positionData->posZ;
    state->spriteOffsetX = state->sprite2OffsetX + state->positionData->posX;
    state->spriteOffsetY = state->sprite2OffsetY + state->positionData->posY;
    state->spriteOffsetZ = state->sprite2OffsetZ + state->positionData->posZ;

    enqueueAlphaSprite(0, (loadAssetMetadata_arg *)&state->assetMetadata);
    enqueueAlphaSprite(0, (loadAssetMetadata_arg *)&state->sprite2);

    state->assetMetadata.alpha -= 0x14;
    state->alphaDecay -= 0x14;

    if (++state->frameIndex == 8) {
        terminateCurrentTask();
    }
}

void cleanupTrickSpriteEffectTask(TrickSpriteEffectCleanupState *state) {
    state->modelData = freeNodeMemory(state->modelData);
}

typedef struct {
    Node n;
    u8 padding[0x36];
    s16 effectParam;
} TrickSpriteEffectTask;
void spawnTrickSpriteEffect(void *model, s16 effectParam) {
    TrickSpriteEffectTask *task = (TrickSpriteEffectTask *)scheduleTask(&initTrickSpriteEffectTask, 1, 0, 0x64);
    if (task != NULL) {
        task->n.prev = model;
        task->effectParam = effectParam;
    }
}

void spawnScrollingTextureEffect(u16 renderLayer, void *transformMatrix, s16 colorIndex, s16 effectMode) {
    ScrollingTextureEffectTask *task =
        (ScrollingTextureEffectTask *)scheduleTask(&initScrollingTextureEffectTask, 1, 0, 0);
    if (task != NULL) {
        task->renderLayer = renderLayer;
        memcpy(task, transformMatrix, 0x20);
        task->colorIndex = colorIndex;
        task->effectMode = effectMode;
    }
}

void initScrollingTextureEffectTask(ScrollingTextureState *arg0) {
    setCleanupCallback(&cleanupScrollingTextureEffectTask);

    if (arg0->unk58 == 0) {
        arg0->base.segment1 = loadUncompressedData(&_215120_ROM_START, &_215120_ROM_END);
        arg0->base.segment2 = loadCompressedData(&_6636F0_ROM_START, &_6636F0_ROM_END, 0x200);
        arg0->base.segment3 = 0;
        arg0->base.displayLists = &D_80089510_8A110;
        arg0->paletteIndex = 0;
    } else if (arg0->unk58 != 1) {
        arg0->base.segment1 = loadUncompressedData(&_215120_ROM_START, &_215120_ROM_END);
        arg0->base.segment2 = loadCompressedData(&_6636F0_ROM_START, &_6636F0_ROM_END, 0x200);
        arg0->base.segment3 = 0;
        arg0->base.displayLists = &D_80089510_8A110;
        arg0->paletteIndex = 0;
    } else {
        arg0->base.segment1 = loadUncompressedData(&_2151D0_ROM_START, &_2151D0_ROM_END);
        arg0->base.segment2 = loadCompressedData(&_663BE0_ROM_START, &_663BE0_ROM_END, 0x200);
        arg0->base.displayLists = &D_80089520;
        arg0->base.segment3 = 0;
        arg0->paletteIndex = 1;
    }

    arg0->textureTable = loadCompressedData(&_6637B0_ROM_START, &_6637B0_ROM_END, 0x858);
    arg0->tileScrollU = 0;
    arg0->tileScrollV = 0;
    arg0->unk48 = 0;
    arg0->unk4A = 0x10;
    arg0->textureIndex = 0;
    arg0->paletteMode = 0;

    setCallback(&updateScrollingTextureEffect);
}

void updateScrollingTextureEffect(ScrollingTextureEffectUpdateState *arg0) {
    if (arg0->framesRemaining == 0) {
        terminateCurrentTask();
    }

    arg0->scrollOffsetX += arg0->scrollSpeedX;
    arg0->scrollOffsetY += arg0->scrollSpeedY;

    // mask off the first 8 bits to stop overflow?
    arg0->scrollOffsetX = 0xFF & arg0->scrollOffsetX;
    arg0->scrollOffsetY = 0xFF & arg0->scrollOffsetY;

    enqueueScrollingTextureRender(arg0->renderLayer, &arg0->displayListObj);

    arg0->framesRemaining--;
}

void cleanupScrollingTextureEffectTask(ScrollingTextureState *arg0) {
    arg0->textureTable = freeNodeMemory(arg0->textureTable);
    arg0->base.segment2 = freeNodeMemory(arg0->base.segment2);
    arg0->base.segment1 = freeNodeMemory(arg0->base.segment1);
}
