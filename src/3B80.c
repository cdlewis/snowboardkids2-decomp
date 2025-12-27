#include "1DFAA0.h"
#include "33FE0.h"
#include "common.h"
#include "main.h"
#include "task_scheduler.h"

typedef struct {
    CutsceneManager *cutsceneManager;
    ModelEntityRenderState *renderState;
} TransparentRenderTaskData;

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

typedef struct {
    void *unk0;
    void *unk4;
    s32 unk8;
} AssetEntry;

typedef struct {
    AssetEntry *assetList;
    u8 _pad4[0x4];
    u8 assetCount;
    u8 _pad9[0x3];
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

extern AssetGroupTableEntry assetGroupTable[];
extern void func_80003184_3D84(void *);
void freeAssetGroupResources(AssetGroupTaskData *taskData);

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

    setCallback(func_80003184_3D84);
}

INCLUDE_ASM("asm/nonmatchings/3B80", func_80003184_3D84);

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
