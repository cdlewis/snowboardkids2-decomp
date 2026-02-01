#include "1DFAA0.h"
#include "33FE0.h"
#include "38C90.h"
#include "common.h"
#include "main.h"
#include "task_scheduler.h"

typedef struct {
    CutsceneManager *cutsceneManager;
    ModelEntityRenderState *renderState;
} TransparentRenderTaskData;

typedef struct {
    void *context;
    void **loadedAssets;
    s32 param;
    s32 unkC;
    s32 unk10;
    struct {
        u8 _pad00[0x10];
        s16 unk24;
        s16 unk26;
        s16 unk28;
        s16 unk2A;
        u8 _pad2C[0x14];
    } elements[4];
    u8 groupIndex;
    s8 unkC5;
} TiledTextureTaskData;

typedef struct {
    void *unk0;
    void *unk4;
    s32 unk8;
} AssetEntry;

typedef struct {
    AssetEntry *assetList;
    u8 *unk4;
    u8 assetCount;
    u8 tableSize;
    u8 _padA[0x2];
} AssetGroupTableEntry;

typedef struct {
    void *context;
    void **loadedAssets;
    s32 param;
    void *unkC;
    void *unk10;
    u8 _pad14[0xB0];
    u8 groupIndex;
    u8 unkC5;
} AssetGroupTaskData;

extern AssetGroupTableEntry assetGroupTable[];

void updateTiledTextureAssetDisplay(TiledTextureTaskData *taskData);
void freeAssetGroupResources(AssetGroupTaskData *taskData);
void renderModelIfTransparent(TransparentRenderTaskData *taskData);

void scheduleTransparentModelRender(CutsceneManager *cutsceneManager, ModelEntityRenderState *renderState) {
    TransparentRenderTaskData *task;

    task = scheduleTask(renderModelIfTransparent, 2, 0, 0xF0);
    if (task != NULL) {
        task->cutsceneManager = cutsceneManager;
        task->renderState = renderState;
    }
}

void renderModelIfTransparent(TransparentRenderTaskData *taskData) {
    if (taskData->cutsceneManager->enableTransparency != 0) {
        renderModelEntity(taskData->renderState);
    }
}

void loadAssetGroupResources(AssetGroupTaskData *taskData);

void scheduleDualAssetGroupLoad(void *context, u8 groupIndex1, s32 param1, u8 groupIndex2, s32 param2) {
    AssetGroupTaskData *task;

    task = scheduleTask(loadAssetGroupResources, 3, 0, 0);
    if (task != NULL) {
        task->context = context;
        task->groupIndex = groupIndex2;
        task->param = param2;
        task->unkC5 = 0;
    }

    task = scheduleTask(loadAssetGroupResources, 3, 0, 0);
    if (task != NULL) {
        task->context = context;
        task->groupIndex = groupIndex1;
        task->param = param1;
        task->unkC5 = 0;
    }
}

void loadAssetGroupResources(AssetGroupTaskData *taskData) {
    AssetGroupTableEntry *entry;
    AssetEntry *assetList;
    s32 i;

    entry = &assetGroupTable[taskData->groupIndex];
    assetList = entry->assetList;

    setCleanupCallback(freeAssetGroupResources);
    i = 0;

    taskData->unk10 = NULL;
    taskData->unkC = NULL;

    taskData->loadedAssets = allocateNodeMemory(entry->assetCount * 4);

    for (i = 0; i < entry->assetCount; i++) {
        taskData->loadedAssets[i] = loadCompressedData(assetList[i].unk0, assetList[i].unk4, assetList[i].unk8);
    }

    setCallback(updateTiledTextureAssetDisplay);
}

void updateTiledTextureAssetDisplay(TiledTextureTaskData *arg0) {
    AssetGroupTableEntry *entry;
    s32 i;
    s32 temp;

    entry = &assetGroupTable[arg0->groupIndex];
    if (arg0->unkC5 == 0) {
        for (i = 0; i < 4; i++) {
            initTiledTextureRenderState(&arg0->elements[i], (s32)arg0->loadedAssets[i % entry->assetCount]);
            arg0->elements[i].unk28 = 0x81;
            arg0->elements[i].unk2A = 0x81;
        }
        arg0->unkC5 = 1;
        return;
    }

    temp = *(s32 *)((u8 *)((CutsceneManager *)arg0->context)->sceneContext + 0x34);
    temp = (temp >> 8) * (arg0->param >> 8);
    temp >>= 16;
    arg0->unk10 = 0x44;
    arg0->unkC = temp;

    for (i = 0; i < 4; i++) {
        u32 tableIndex;
        s32 tileIndex;
        s32 assetIndex;
        s32 scrollValue;

        tableIndex = ((u32)arg0->unkC + (i << 7)) >> 7;
        tileIndex = tableIndex % entry->tableSize;
        tileIndex = entry->unk4[tileIndex];
        assetIndex = tileIndex % entry->assetCount;
        initTiledTextureRenderState(&arg0->elements[i], (s32)arg0->loadedAssets[assetIndex]);
        arg0->elements[i].unk28 = 0x81;
        arg0->elements[i].unk2A = 0x81;
        scrollValue = ~arg0->unkC;
        scrollValue &= 0x7F;
        scrollValue += -0x80;
        scrollValue += i << 7;
        arg0->elements[i].unk24 = scrollValue;
        arg0->elements[i].unk26 = arg0->unk10;

        if (((CutsceneManager *)arg0->context)->enableTransparency != 0) {
            if (((CutsceneManager *)arg0->context)->unk10.unk87 != 0) {
                debugEnqueueCallback(3, 2, renderTiledTexture, &arg0->elements[i]);
            }
        }
    }
}

void freeAssetGroupResources(AssetGroupTaskData *taskData) {
    AssetGroupTableEntry *entry;
    s32 i;
    u8 index;

    index = taskData->groupIndex;
    entry = &assetGroupTable[index];

    for (i = 0; i < entry->assetCount; i++) {
        taskData->loadedAssets[i] = freeNodeMemory(taskData->loadedAssets[i]);
    }

    taskData->loadedAssets = freeNodeMemory(taskData->loadedAssets);
}
