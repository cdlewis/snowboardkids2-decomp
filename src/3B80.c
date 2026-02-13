#include "1DFAA0.h"
#include "33FE0.h"
#include "38C90.h"
#include "assets.h"
#include "common.h"
#include "main.h"
#include "task_scheduler.h"

typedef struct {
    CutsceneManager *cutsceneManager;
    ModelEntityRenderState *renderState;
} TransparentRenderTaskData;

typedef struct {
    void *romStart;
    void *romEnd;
    s32 size;
} AssetEntry;

typedef struct {
    AssetEntry *assetList;
    u8 *tileIndexMap;
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

u8 D_8008BE90[] = { 0x00, 0x01, 0x00, 0x00 };

u8 D_8008BE94[] = {
    0x03, 0x02, 0x06, 0x04, 0x00, 0x03, 0x06, 0x02, 0x06, 0x04, 0x03, 0x04, 0x00, 0x03, 0x02, 0x06, 0x02, 0x02, 0x02,
    0x06, 0x06, 0x02, 0x02, 0x04, 0x00, 0x00, 0x03, 0x04, 0x03, 0x04, 0x03, 0x04, 0x00, 0x03, 0x04, 0x00, 0x00, 0x00,
    0x03, 0x02, 0x02, 0x02, 0x02, 0x02, 0x06, 0x06, 0x02, 0x06, 0x04, 0x01, 0x05, 0x04, 0x00, 0x00, 0x00, 0x03, 0x06,
    0x02, 0x03, 0x02, 0x06, 0x02, 0x06, 0x02, 0x02, 0x02, 0x04, 0x00, 0x00, 0x03, 0x02, 0x06, 0x04, 0x03, 0x06, 0x02,
    0x02, 0x04, 0x03, 0x04, 0x03, 0x04, 0x00, 0x00, 0x00, 0x03, 0x02, 0x02, 0x06, 0x06, 0x02, 0x02, 0x02, 0x02, 0x02,
    0x06, 0x04, 0x03, 0x04, 0x03, 0x04, 0x03, 0x04, 0x03, 0x06, 0x02, 0x04, 0x03, 0x06, 0x02, 0x02, 0x04,
};

AssetEntry D_8008BF04[] = {
    { (void *)&_664C10_ROM_START, (void *)&_664C10_ROM_END, 0x00003280 },
    { (void *)&_667220_ROM_START, (void *)&_667220_ROM_END, 0x00003290 },
};

AssetEntry D_8008BF1C[] = {
    { (void *)&_669380_ROM_START, (void *)&_669380_ROM_END, 0x00002E80 },
    { (void *)&_674970_ROM_START, (void *)&_674970_ROM_END, 0x00004290 },
    { (void *)&_66C040_ROM_START, (void *)&_66C040_ROM_END, 0x00004290 },
    { (void *)&_66A560_ROM_START, (void *)&_66A560_ROM_END, 0x00003690 },
    { (void *)&_66F9D0_ROM_START, (void *)&_66F9D0_ROM_END, 0x00003A90 },
    { (void *)&_677F60_ROM_START, (void *)&_677F60_ROM_END, 0x00004290 },
    { (void *)&_671E90_ROM_START, (void *)&_671E90_ROM_END, 0x00004290 },
};

AssetGroupTableEntry assetGroupTable[] = {
    { D_8008BF04, D_8008BE90, 2, 2    },
    { D_8008BF1C, D_8008BE94, 7, 0x70 },
};

s32 D_8008BF88 = 0x00020000;
s32 D_8008BF8C = 0x00000000;

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
        taskData->loadedAssets[i] = loadCompressedData(assetList[i].romStart, assetList[i].romEnd, assetList[i].size);
    }

    setCallback(updateTiledTextureAssetDisplay);
}

void updateTiledTextureAssetDisplay(TiledTextureTaskData *taskData) {
    AssetGroupTableEntry *entry;
    s32 i;
    s32 scrollOffset;

    entry = &assetGroupTable[taskData->groupIndex];
    if (taskData->initialized == 0) {
        for (i = 0; i < 4; i++) {
            initTiledTextureRenderState(&taskData->elements[i], (s32)taskData->loadedAssets[i % entry->assetCount]);
            taskData->elements[i].width = 0x81;
            taskData->elements[i].height = 0x81;
        }
        taskData->initialized = 1;
        return;
    }

    scrollOffset = ((CutsceneCameraState *)taskData->cutsceneManager->sceneContext)->posXCurrent;
    scrollOffset = (scrollOffset >> 8) * (taskData->scrollSpeedParam >> 8);
    scrollOffset >>= 16;
    taskData->yPos = 0x44;
    taskData->scrollOffset = scrollOffset;

    for (i = 0; i < 4; i++) {
        u32 tableIndex;
        s32 tileIndex;
        s32 assetIndex;
        s32 xPos;

        tableIndex = ((u32)taskData->scrollOffset + (i << 7)) >> 7;
        tileIndex = tableIndex % entry->tableSize;
        tileIndex = entry->tileIndexMap[tileIndex];
        assetIndex = tileIndex % entry->assetCount;
        initTiledTextureRenderState(&taskData->elements[i], (s32)taskData->loadedAssets[assetIndex]);
        taskData->elements[i].width = 0x81;
        taskData->elements[i].height = 0x81;
        xPos = ~taskData->scrollOffset;
        xPos &= 0x7F;
        xPos += -0x80;
        xPos += i << 7;
        taskData->elements[i].xPos = xPos;
        taskData->elements[i].yPos = taskData->yPos;

        if (taskData->cutsceneManager->enableTransparency != 0) {
            if (taskData->cutsceneManager->unk10.unk87 != 0) {
                debugEnqueueCallback(3, 2, renderTiledTexture, &taskData->elements[i]);
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
