#include "36BE0.h"
#include "5520.h"
#include "common.h"
#include "displaylist.h"
#include "geometry.h"
#include "rom_loader.h"
#include "task_scheduler.h"

USE_ASSET(_215120);
USE_ASSET(_2151D0);
USE_ASSET(_646DF0);
USE_ASSET(_6636F0);
USE_ASSET(_6637B0);
USE_ASSET(_663BE0);

typedef struct {
    s32 reserved;
    void *modelData;
} TrickSpriteEffectCleanupState;

typedef struct {
    Node n;
    u8 padding[0x28];
    u16 unk54;
    s16 unk56;
    s16 unk58;
} func_800B5290_1E2340_task;

typedef struct {
    struct {
        u8 padding[0x2C];
        s32 posX;
        s32 posY;
        s32 posZ;
    } *unk0;
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
        } *unk0;
        u8 padding[0x28];
        s32 posX;
        s32 posY;
        s32 posZ;
    } *unk0;
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
    DisplayListObject unk0;
    void *unk3C;
    s32 unk40;
    s16 unk44;
    s16 unk46;
    s16 unk48;
    s16 unk4A;
    s16 unk4C;
    s16 unk4E;
    s16 unk50;
    s16 unk52;
    s16 unk54;
    s16 unk56;
    s16 unk58;
} func_800B5438_1E24E8_arg0;

void func_800B5438_1E24E8(func_800B5438_1E24E8_arg0 *);
void func_800B54B4_1E2564(ScrollingTextureState *);
void func_800B5318_1E23C8(ScrollingTextureState *);
void updateTrickSpriteEffect(TrickSpriteEffectUpdateState *);
void cleanupTrickSpriteEffectTask(TrickSpriteEffectCleanupState *);
void initTrickSpriteEffectTask(TrickSpriteEffectInitState *);

extern ScrollingTextureDisplayLists D_80089510_8A110;
extern ScrollingTextureDisplayLists D_80089520;

u32 gTrickSpriteEffectVertexData[] = { 0xFFE80018, 0x00000000, 0xFFF0FFF0, 0xFFFFFFFF, 0x00180018, 0x00000000,
                                       0x07F0FFF0, 0xFFFFFFFF, 0x0018FFE8, 0x00000000, 0x07F007F0, 0xFFFFFFFF,
                                       0xFFE8FFE8, 0x00000000, 0xFFF007F0, 0xFFFFFFFF, 0x00040000, 0x00000000,
                                       0x000D0000, 0xFFFC0000, 0x00000000, 0x000D0000 };

s32 gTrickSpriteEffectTransformData[2][3] = {
    { 0xFFFC0000, 0x00000000, 0xFFF30000 },
    { 0x00040000, 0x00000000, 0xFFF30000 }
};

void initTrickSpriteEffectTask(TrickSpriteEffectInitState *arg0) {
    s32 i;

    arg0->modelData = loadCompressedData(&_646DF0_ROM_START, &_646DF0_ROM_END, 0x1188);
    arg0->vertexData = (s32 *)&gTrickSpriteEffectVertexData;
    arg0->alpha = 0xFF;

    arg0->vertexDataCopy = arg0->vertexData;
    arg0->alphaCopy = arg0->alpha;

    for (i = 0; i < 2; i++) {
        // vector is different size (32 vs 16 bit) because that helped
        // the data segment to match.
        transformVector(
            (s16 *)gTrickSpriteEffectTransformData[i],
            &arg0->unk0->unk0->rotationY,
            &arg0->spritePositions[i]
        );
        arg0->spritePositions[i].x -= arg0->unk0->posX;
        arg0->spritePositions[i].y -= arg0->unk0->posY;
        arg0->spritePositions[i].z -= arg0->unk0->posZ;
    }

    arg0->frameIndex = 0;
    arg0->effectParam = arg0->effectParam;

    setCleanupCallback(&cleanupTrickSpriteEffectTask);
    setCallbackWithContinue(&updateTrickSpriteEffect);
}

void updateTrickSpriteEffect(TrickSpriteEffectUpdateState *state) {
    loadAssetMetadataByIndex(&state->assetMetadata, state->modelData, state->frameIndex, state->effectParam);
    state->dataPtr = state->assetMetadata.data_ptr;
    state->indexPtr = state->assetMetadata.index_ptr;
    state->alpha = state->assetMetadata.unk18;
    state->animFrame = state->assetMetadata.unk19;
    state->assetMetadata.unk4 = state->sprite1OffsetX + state->unk0->posX;
    state->assetMetadata.unk8 = state->sprite1OffsetY + state->unk0->posY;
    state->assetMetadata.unkC = state->sprite1OffsetZ + state->unk0->posZ;
    state->spriteOffsetX = state->sprite2OffsetX + state->unk0->posX;
    state->spriteOffsetY = state->sprite2OffsetY + state->unk0->posY;
    state->spriteOffsetZ = state->sprite2OffsetZ + state->unk0->posZ;

    enqueueAlphaSprite(0, (loadAssetMetadata_arg *)&state->assetMetadata);
    enqueueAlphaSprite(0, (loadAssetMetadata_arg *)&state->sprite2);

    state->assetMetadata.unk1A -= 0x14;
    state->alphaDecay -= 0x14;

    if (++state->frameIndex == 8) {
        func_80069CF8_6A8F8();
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
void spawnTrickSpriteEffect(void *arg0, s16 arg1) {
    TrickSpriteEffectTask *task = (TrickSpriteEffectTask *)scheduleTask(&initTrickSpriteEffectTask, 1, 0, 0x64);
    if (task != NULL) {
        task->n.prev = arg0;
        task->effectParam = arg1;
    }
}

void func_800B5290_1E2340(u16 arg0, void *arg1, s16 arg2, s16 arg3) {
    func_800B5290_1E2340_task *task = (func_800B5290_1E2340_task *)scheduleTask(&func_800B5318_1E23C8, 1, 0, 0);
    if (task != NULL) {
        task->unk54 = arg0;
        memcpy(task, arg1, 0x20);
        task->unk56 = arg2;
        task->unk58 = arg3;
    }
}

void func_800B5318_1E23C8(ScrollingTextureState *arg0) {
    setCleanupCallback(&func_800B54B4_1E2564);

    if (arg0->unk58 == 0) {
        arg0->unk24 = loadUncompressedData(&_215120_ROM_START, &_215120_ROM_END);
        arg0->unk28 = loadCompressedData(&_6636F0_ROM_START, &_6636F0_ROM_END, 0x200);
        arg0->unk2C = 0;
        arg0->unk20 = &D_80089510_8A110;
        arg0->unk50 = 0;
    } else if (arg0->unk58 != 1) {
        arg0->unk24 = loadUncompressedData(&_215120_ROM_START, &_215120_ROM_END);
        arg0->unk28 = loadCompressedData(&_6636F0_ROM_START, &_6636F0_ROM_END, 0x200);
        arg0->unk2C = 0;
        arg0->unk20 = &D_80089510_8A110;
        arg0->unk50 = 0;
    } else {
        arg0->unk24 = loadUncompressedData(&_2151D0_ROM_START, &_2151D0_ROM_END);
        arg0->unk28 = loadCompressedData(&_663BE0_ROM_START, &_663BE0_ROM_END, 0x200);
        arg0->unk20 = &D_80089520;
        arg0->unk2C = 0;
        arg0->unk50 = 1;
    }

    arg0->unk3C = loadCompressedData(&_6637B0_ROM_START, &_6637B0_ROM_END, 0x858);
    arg0->unk44 = 0;
    arg0->unk46 = 0;
    arg0->unk48 = 0;
    arg0->unk4A = 0x10;
    arg0->unk4C = 0;
    arg0->unk4E = 0;

    setCallback(&func_800B5438_1E24E8);
}

void func_800B5438_1E24E8(func_800B5438_1E24E8_arg0 *arg0) {
    if (arg0->unk56 == 0) {
        func_80069CF8_6A8F8();
    }

    arg0->unk44 += arg0->unk48;
    arg0->unk46 += arg0->unk4A;

    // mask off the first 8 bits to stop overflow?
    arg0->unk44 = 0xFF & arg0->unk44;
    arg0->unk46 = 0xFF & arg0->unk46;

    enqueueScrollingTextureRender(arg0->unk54, &arg0->unk0);

    arg0->unk56--;
}

void func_800B54B4_1E2564(ScrollingTextureState *arg0) {
    arg0->unk3C = freeNodeMemory(arg0->unk3C);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
    arg0->unk24 = freeNodeMemory(arg0->unk24);
}
