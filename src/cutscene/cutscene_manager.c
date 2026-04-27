#include "cutscene/cutscene_manager.h"
#include "animation/animation_loop.h"
#include "animation/slot_animation.h"
#include "core/main.h"
#include "core/stubs.h"
#include "cutscene/1DD170.h"
#include "effects/transparent_render.h"
#include "graphics/graphics.h"
#include "graphics/model_loader.h"
#include "graphics/sprite_rdp.h"
#include "math/geometry.h"
#include "os_cont.h"
#include "system/controller_io.h"
#include "system/task_scheduler.h"
#include "text/font_assets.h"
#include "ui/level_preview_3d.h"

typedef struct {
    u8 _pad0[0x4];
    s32 unk4;
    s32 unk8;
} SaveDataBuffer;

extern StateEntry *gCutsceneStateTable;
extern s32 gCutsceneStateTableSize;
extern StateEntry *gControllerPakTransferPointer;
extern StateEntry *gControllerPakStateTablePointer;
extern s32 gButtonsPressed[];
extern StateEntry gCutsceneEntryBuffer[];
extern u8 D_800BAF06_1E7FB6;
extern u8 gCutsceneEntryCopyFlag;
extern s16 gCutsceneEntryBufferFrameNumber;
extern s8 gCutsceneEntryCutFlag;
extern s16 gCutsceneEntryBufferSlotIndex;
extern CutsceneFadeAssetNode gCutsceneFadeAssetTable[];
extern char gDebugFrameFormatString[];
extern CutsceneAssetTable gCutsceneAssetTable[];

extern void initializeCutsceneCommand(void *, void *, s32, s32, s32);
extern void func_800BB2F4(s32, s32, s32);

void cleanupCutsceneFadeTask(FadeTaskData *task);
void updateCutsceneFadeTask(FadeTaskData *task);

void enableCutsceneSkip(CutsceneManager *arg0) {
    arg0->skipAnimation = TRUE;
}

void disableCutsceneSkipOnInput(CutsceneManager *arg0) {
    if (arg0->skipAnimation != FALSE && (gButtonsPressed[0] & A_BUTTON)) {
        arg0->skipAnimation = FALSE;
    }
}

void enableSlotUpdate(CutsceneManager *arg0, s16 arg1) {
    arg0->slots[arg1].needsUpdate = 1;
}

void disableSlotUpdate(CutsceneManager *arg0, s16 arg1) {
    arg0->slots[arg1].needsUpdate = 0;
}

CutsceneSlot *getCutsceneSlot(CutsceneManager *manager, s16 slotIndex) {
    return &manager->slots[slotIndex];
}

void initCutsceneManager(CutsceneManager *manager, ViewportNode *sceneNode, void *shadowModel, void *reflectionModel) {
    s32 i;
    s16 modelIndex = getCutsceneInitModelIndex();
    s32 needsVisibility = initModelEntity(&manager->unk10.modelEntity, modelIndex, sceneNode);

    setModelRenderMode(&manager->unk10.renderModeArg, 0);

    for (i = 0; i < (getCutsceneSlotCount() & 0xFF); i++) {
        StateEntryItem *slot = getCurrentStateEntryItem(i);
        manager->slots[i].unk40 = slot->characterId;

        if (slot->characterId != -1) {
            if (isAssetGroupEmpty(slot->characterId)) {
                manager->slots[i].model =
                    createSceneModelEx(slot->characterId, sceneNode, slot->unk6, slot->unk7, slot->unk8, slot->unk9);
            } else {
                manager->slots[i].model = createSceneModel(slot->characterId, sceneNode);
            }

            if (needsVisibility) {
                enableEntityRendering(manager->slots[i].model);
            }
        } else {
            manager->slots[i].model = NULL;
        }

        initSlotData(&manager->slots[i].slotData);
    }

    manager->sceneContext = createAnimationLoopState(sceneNode->id);

    if (needsVisibility) {
        scheduleDualAssetGroupLoad(manager, 0, 0x8000, 1, 0x10000);
        setAnimationLoopMode(manager->sceneContext, 1);
    }

    initSceneRenderNode(&manager->unkFF8, 2, 0xC, sceneNode->id, 0, 2, 0);
    clearAuxRenderEnabled(&manager->unkFF8);

    manager->shadowModel = shadowModel;
    manager->reflectionModel = reflectionModel;
    manager->curtainPosition = 0x10000;
}

void cleanupCutsceneManager(CutsceneManager *manager) {
    s32 i;

    n_alSeqpDelete((ViewportNode *)&manager->unkFF8);

    freeAnimationLoopState(manager->sceneContext);

    for (i = 0; i < (getCutsceneSlotCount() & 0xFF); i++) {
        if (manager->slots[i].model != NULL) {
            manager->slots[i].model = destroySceneModel(manager->slots[i].model);
        }
    }

    freeEffectResources((EffectState *)&manager->unk10.modelEntity);
}

void hideAllSlotModels(CutsceneSlot *slots) {
    s32 i;

    for (i = 0; i < getCutsceneSlotCount(); i++) {
        if (slots[i].slotData.unkA4.ptr != NULL) {
            setModelVisibility(slots[i].slotData.unkA4.ptr, 0);
        }
    }
}

void showAllSlotModels(CutsceneSlot *slots) {
    s32 i;

    for (i = 0; i < getCutsceneSlotCount(); i++) {
        if (slots[i].slotData.unkA4.ptr != NULL) {
            setModelVisibility(slots[i].slotData.unkA4.ptr, 1);
        }
    }
}

void resetAllSlotModels(CutsceneSlot *slots) {
    s32 i;

    for (i = 0; i < getCutsceneSlotCount(); i++) {
        getCurrentStateEntryItem(i);

        if (slots[i].slotData.unkA4.ptr != NULL) {
            applyTransformToModel(slots[i].slotData.unkA4.ptr, &identityMatrix);
            setModelAnimation(slots[i].slotData.unkA4.ptr, 0);
        }
    }
}

void resetAllSlotTransforms(CutsceneManager *manager) {
    s32 i;
    SceneModel *model;

    for (i = 0; i < getCutsceneSlotCount(); i++) {
        getCurrentStateEntryItem(i);
        model = manager->slots[i].model;

        if (model != NULL) {
            CutsceneSlotData *slotData = &manager->slots[i].slotData;

            interpolateSlotScaleX(slotData, 0x10000, 0);
            interpolateSlotScaleY(slotData, 0x10000, 0);
            interpolateSlotScaleZ(slotData, 0x10000, 0);
            setupSlotTransform(slotData);
            applyTransformToModel(model, &slotData->transform);
        }
    }
}

void prepareCutsceneForPlayback(
    CutsceneManager *manager,
    ViewportNode *uiResource,
    ColorData *lightColors,
    ColorData *ambientColor,
    u16 maxFrame,
    u8 showDebugInfo
) {
    SceneModel *model;
    s32 i;
    u16 modelId;

    manager->uiResource = uiResource;
    manager->lightColors = lightColors;
    manager->ambientColor = ambientColor;
    manager->currentFrame = 0;
    manager->maxFrame = maxFrame;
    manager->endFrame = getCutsceneDefaultEndFrame();

    for (i = 0; i < 0x10; i++) {
        modelId = getCurrentStateEntryItem(i)->characterId;
        model = manager->slots[i].model;
        manager->slots[i].unk42 = 0xFF;
        manager->slots[i].unk43 = 0xFF;
        manager->slots[i].needsUpdate = 0;
        manager->slots[i].padding3[0] = 0;
        manager->slots[i].unk40 = modelId;
        if (model != NULL) {
            model->unk14 = -1;
            manager->slots[i].model->unk16 = -1;
            setModelDisplayEnabled(manager->slots[i].model, 1);
            setItemDisplayEnabled(manager->slots[i].model, 0);
            setAnimationIndex(manager->slots[i].model, -1);
            clearModelAnimationState(manager->slots[i].model);
        }
    }

    manager->textX = -0x90;
    manager->textY = 0x68;
    manager->textPalette = 0;
    manager->textString = (u8 *)&manager->debugText[0];
    manager->showDebugInfo = showDebugInfo;
    manager->enableTransparency = 0;
    manager->skipAnimation = 0;
    showAllSlotModels((CutsceneSlot *)manager);
    resetAllSlotTransforms(manager);
    clearAuxRenderEnabled(&manager->unkFF8);
}

s32 processCutsceneFrame(CutsceneManager *cutsceneManager) {
    s32 slotBitmask;
    SceneModel *currentModel;
    s32 i;
    CutsceneSlotData *slotData;
    s16 cameraOffsetX;
    s32 cameraScaleZ;
    StateEntry *eventEntry;
    s32 shouldInitializeSlot;
    u16 commandType;
    s32 curtainPos;
    u16 eventIndex;
    u8 eventType;

    if (cutsceneManager->showDebugInfo) {
        sprintf((char *)cutsceneManager->debugText, gDebugFrameFormatString, cutsceneManager->currentFrame);
        debugEnqueueCallback(cutsceneManager->uiResource->slot_index, 6, &renderTextPalette, &cutsceneManager->textX);
    }

    while (cutsceneManager->currentFrame <= cutsceneManager->maxFrame && !cutsceneManager->skipAnimation) {
        for (i = 0, slotBitmask = 0; i < getCutsceneSlotCount(); i++) {
            currentModel = cutsceneManager->slots[i].model;
            slotData = &cutsceneManager->slots[i].slotData;

            eventIndex = findEventAtFrame(i, cutsceneManager->currentFrame);
            if ((eventIndex & 0xFFFF) != 0xFFFF) {
                eventEntry = getStateEntry(eventIndex);
                shouldInitializeSlot = 1;

                if (cutsceneManager->currentFrame < cutsceneManager->maxFrame) {
                    eventType = eventEntry->commandCategory - 4;

                    if (eventType < 2) {
                        shouldInitializeSlot = 0;
                    } else {
                        commandType = *(u16 *)&eventEntry->commandCategory;
                        if (commandType == 0x801) {
                            commandType = 0;
                            shouldInitializeSlot = commandType;
                        }
                    }
                }

                if (shouldInitializeSlot) {
                    initializeSlotState(eventEntry, cutsceneManager, slotBitmask >> 24);
                }
            }

            updateSlotData(cutsceneManager, slotBitmask >> 24);
            syncModelFromSlot(slotData, currentModel);

            slotBitmask += 0x1000000;
        }

        cutsceneManager->currentFrame++;
        advanceSceneManager(cutsceneManager->sceneContext);
    }

    finalizeAnimationLoop(cutsceneManager->sceneContext);

    if (!cutsceneManager->skipAnimation) {
        cutsceneManager->maxFrame++;
    }

    for (i = 0; i < getCutsceneSlotCount(); i++) {
        currentModel = cutsceneManager->slots[i].model;
        slotData = &cutsceneManager->slots[i].slotData;

        if (currentModel) {
            setupSlotTransform(slotData);
            applyTransformToModel(currentModel, &slotData->transform);
            if (cutsceneManager->enableTransparency) {
                setModelVisibility(currentModel, 1);
            } else {
                setModelVisibility(currentModel, 0);
            }

            updateModelGeometry(currentModel);

            if (slotData->angle != 0) {
                setModelRotation(currentModel, slotData->angle);
            } else {
                clearModelRotation(currentModel);
            }
        }
    }

    curtainPos = cutsceneManager->curtainPosition;
    cameraOffsetX = -((curtainPos * 120) >> 16);
    cameraScaleZ = (curtainPos * 119) >> 16;

    setModelRenderMode(&cutsceneManager->unk10.renderModeArg, cutsceneManager->enableTransparency);
    setModelCameraTransform(cutsceneManager->uiResource, 0, 0, -0xA0, cameraOffsetX, 0x9F, cameraScaleZ);
    setModelCameraTransform(cutsceneManager->shadowModel, 0, 0, -0xA0, cameraOffsetX, 0x9F, cameraScaleZ);
    setModelCameraTransform(cutsceneManager->reflectionModel, 0, 0, -0xA0, cameraOffsetX, 0x9F, cameraScaleZ);

    return (cutsceneManager->currentFrame <= cutsceneManager->endFrame) ? 1 : 0;
}

s16 getCutsceneFrameCount(s16 slotIndex, s16 cutsceneType) {
    s16 masked;
    s16 result;

    masked = -1;

    if ((((((((((((cutsceneType & 0xFFFFFFFFu) & 0xFFFFFFFFu) & 0xFFFFFFFFu) & 0xFFFFFFFFu) & 0xFFFFFFFFu) & masked) &
             0xFFFFFFFFu) &
            0xFFFFFFFFu) &
           0xFFFFFFFFu) &
          0xFFFFFFFFu) &
         0xFFFFFFFFu) >= ((u64)3)) {
        return 0;
    }

    if (slotIndex >= 0x10) {
        return 0;
    }

    if (cutsceneType == 0) {
        result = gCutsceneAssetTable[slotIndex].introFrameCount;
    } else if (cutsceneType == 1) {
        result = gCutsceneAssetTable[slotIndex].winFrameCount;
    }

    if (cutsceneType == 2) {
        result = gCutsceneAssetTable[slotIndex].loseFrameCount;
    }

    return result;
}

void *getCutsceneDataMagicPrimary(void) {
    return &gCutsceneStateTable->header[1];
}

void *getCutsceneDataMagicSecondary(void) {
    return &gCutsceneStateTable->header[2];
}

u16 getCutsceneAllocatedEventCount(void) {
    return gCutsceneStateTable->allocatedEventCount;
}

u8 getCutsceneStateEntryItemSize(void) {
    return gCutsceneStateTable->stateEntryItemSize;
}

u8 getCutsceneSlotCount(void) {
    return gCutsceneStateTable->slotCount;
}

u16 getCutsceneFrameMask(void) {
    return gCutsceneStateTable->frameMask;
}

u16 getCutsceneMaxStateEntries(void) {
    return gCutsceneStateTable->maxEntries;
}

s16 getCutsceneInitModelIndex(void) {
    return gCutsceneStateTable->initModelIndex;
}

void setCutsceneInitModelIndex(s16 arg0) {
    gCutsceneStateTable->initModelIndex = arg0;
}

StateEntryItem *getCurrentStateEntryItem(s32 itemIndex) {
    return &((StateTable *)gCutsceneStateTable)->items[itemIndex];
}

s16 getCutsceneDefaultEndFrame(void) {
    return gCutsceneStateTable->defaultEndFrame;
}

void setCutsceneDefaultEndFrame(s16 arg0) {
    gCutsceneStateTable->defaultEndFrame = arg0;
}

u8 getCutsceneConfigByte(void) {
    return gCutsceneStateTable->configByte;
}

void setCutsceneConfigByte(u8 arg0) {
    gCutsceneStateTable->configByte = arg0;
}

s32 saveCutsceneStateTableStub(void) {
    nullsub_s32(gCutsceneStateTable, gCutsceneStateTableSize, 0);
    return 1;
}

s32 loadCutsceneStateTableStub(void) {
    nullsub_s32_2(gCutsceneStateTable, gCutsceneStateTableSize, 0);
    return 1;
}

void *loadCutsceneFrameData(s16 slotIndex, s16 cutsceneType, s16 frameIndex) {
    s16 cutsceneTypeClamped;
    CutsceneFrameInfo *frameInfo;
    CutsceneAssetTable *assetEntry;

    frameInfo = NULL;

    if (cutsceneType >= 3) {
        cutsceneTypeClamped = 2;
    } else {
        cutsceneTypeClamped = cutsceneType & (~cutsceneType >> 31);
    }

    if (slotIndex >= 16) {
        return NULL;
    }

    assetEntry = &gCutsceneAssetTable[slotIndex];

    if (cutsceneTypeClamped == 0) {
        if (frameIndex < assetEntry->introFrameCount) {
            frameInfo = &gCutsceneAssetTable[slotIndex].introFrames[frameIndex];
        }
    } else if (cutsceneTypeClamped == 1) {
        if (frameIndex < assetEntry->winFrameCount) {
            frameInfo = &gCutsceneAssetTable[slotIndex].winFrames[frameIndex];
        }
    }

    if (cutsceneTypeClamped == 2) {
        if (frameIndex < assetEntry->loseFrameCount) {
            frameInfo = &gCutsceneAssetTable[slotIndex].loseFrames[frameIndex];
        }
    }

    if (frameInfo == NULL) {
        return NULL;
    }

    return loadCompressedData(frameInfo->romStart, frameInfo->romEnd, frameInfo->decompressedSize);
}

s32 verifyAndLoadCutsceneState(void *stateBuffer) {
    s32 success;
    s32 *inputState;

    inputState = (s32 *)stateBuffer;

    success = 1;

    if (gCutsceneStateTable->header[1] != inputState[1]) {
        goto skip_copy;
    }

    if (gCutsceneStateTable->header[2] != inputState[2]) {
        goto skip_copy;
    }

    memcpy(gCutsceneStateTable, stateBuffer, 0x78C0);
    goto done;

skip_copy:
    success = 0;

done:
    freeNodeMemory(stateBuffer);
    return success;
}

void saveCutsceneStateTableToControllerPak(void) {
    StateEntry **ptr;

    ptr = &gControllerPakTransferPointer;
    *ptr = (StateEntry *)gCutsceneStateTableSize;
    gControllerPakStateTablePointer = gCutsceneStateTable;
    ((void (*)(s32, StateEntry **))controllerPackWriteAsyncStub)(0, ptr);
    do { } while (controllerPackWritePollStub() == -1); }

s32 loadCutsceneStateTableFromControllerPak(void) {
    SaveDataBuffer *buffer;
    s32 result;
    SaveDataBuffer *compareData;
    StateEntry **ptr;

    buffer = allocateNodeMemory(gCutsceneStateTableSize);
    ptr = &gControllerPakTransferPointer;
    gControllerPakStateTablePointer = (StateEntry *)buffer;
    *ptr = (StateEntry *)gCutsceneStateTableSize;
    ((void (*)(s32, StateEntry **))controllerPackReadAsyncStub)(0, ptr);

    do {
        result = controllerPackReadPollStub();
    } while (result == -1);

    if (result == 0) {
        compareData = (SaveDataBuffer *)gCutsceneStateTable;
        if (compareData->unk4 == buffer->unk4 && compareData->unk8 == buffer->unk8) {
            memcpy(gCutsceneStateTable, buffer, 0x78C0);
        }
    }

    freeNodeMemory(buffer);
    return result;
}

u16 allocateStateEntry(void) {
    u16 index = gCutsceneStateTable->current_index;
    StateEntry *currentEntry = &gCutsceneStateTable[index];

    gCutsceneStateTable->current_index = currentEntry[3].next_index;

    return index;
}

void resetScriptState(u8 *arg0) {
    s32 i;

    for (i = 0x37; i >= 0; i--) {
        arg0[i] = 0;
    }
}

void initializeStateEntry(s32 entryIndex) {
    StateEntry *entry;

    resetScriptState((u8 *)(ScriptData *)&gCutsceneStateTable[entryIndex + 3]);

    entry = gCutsceneStateTable + entryIndex + 3;
    entry->frameNumber = 0;

    entry = gCutsceneStateTable + entryIndex + 3;
    entry->commandCategory = 0;

    entry = gCutsceneStateTable + entryIndex + 3;
    entry->commandType = 0;
}

/*
 * Initializes the cutscene state table from ROM asset data.
 *
 * The state table is a 0x78E0-byte block structured as:
 *   [0x000 - 0x0BF] Header area: metadata + 16 slot items (StateTable)
 *   [0x0C0 - 0x78BF] 480 (0x1E0) event entries, each 0x40 bytes (StateEntry)
 *
 * Event entry n (0-based) is at offset (n+3)*0x40 from the table base.
 * Its next_index field is at table offset n*0x40 + 0xF8 (= (n+3)*0x40 + 0x38).
 * Its prev_index field is at table offset n*0x40 + 0xFA (= (n+3)*0x40 + 0x3A).
 */
void initializeCutsceneSystem(void *romAssetAddr) {
    u8 *entryPtr;
    s32 slotIdx;
    s32 nextIdx;
    s32 itemByteOffset;
    u16 entryIndex;
    s32 negOne;     /* -1 constant; kept in a callee-saved register for the slot init loop */
    u16 invalidIdx; /* 0xFFFF sentinel; kept in a callee-saved register for the slot init loop */

    gCutsceneStateTableSize = 0x78E0;
    gCutsceneStateTable = allocateNodeMemory(0x78E0);

    /* Build doubly-linked free list for all 480 event entries.
     * The list pointers use raw table indexing: entry n's next_index is
     * at table base + n*0x40 + 0xF8 (bypassing the 3-entry header offset). */
    slotIdx = 0;
    do {
        StateEntry *rawTable;
        s32 entryByteOffset;
        s32 prevIdx;

        rawTable = gCutsceneStateTable;
        entryByteOffset = slotIdx << 6; /* slotIdx * sizeof(StateEntry) */
        nextIdx = slotIdx + 1;
        prevIdx = slotIdx - 1;
        entryPtr = (u8 *)rawTable + entryByteOffset;
        *(s16 *)(entryPtr + 0xF8) = nextIdx; /* entry[slotIdx].next_index */
        *(s16 *)(entryPtr + 0xFA) = prevIdx; /* entry[slotIdx].prev_index */
        initializeStateEntry(slotIdx);
        slotIdx = nextIdx;
    } while (nextIdx < 0x1E0);

    /* Terminate free list boundaries and write "EDAT0001" magic signature.
     * The magic is stored in header[1] ("EDAT") and header[2] ("0001") and
     * is used by verifyAndLoadCutsceneState to validate save data. */
    {
        StateEntry *firstEntry = gCutsceneStateTable;
        StateEntry *firstEntry2;
        ((u8 *)firstEntry->header)[4] = 'E';
        firstEntry2 = gCutsceneStateTable;
        *(u16 *)((u8 *)firstEntry + 0xFA) = 0xFFFF;   /* entry[0].prev_index = none */
        *(u16 *)((u8 *)firstEntry + 0x78B8) = 0xFFFF; /* entry[0x1DF].next_index = none */
        firstEntry->header[0] = 0;
        ((u8 *)firstEntry2->header)[5] = 'D';
    }
    ((u8 *)gCutsceneStateTable->header)[6] = 'A';
    ((u8 *)gCutsceneStateTable->header)[7] = 'T';
    ((u8 *)gCutsceneStateTable->header)[8] = '0';
    ((u8 *)gCutsceneStateTable->header)[9] = '0';
    ((u8 *)gCutsceneStateTable->header)[10] = '0';
    slotIdx = 0;
    ((u8 *)gCutsceneStateTable->header)[11] = '1';

    /* Initialize table metadata */
    {
        StateEntry *table = gCutsceneStateTable;
        StateEntry *table2;
        negOne = -1;
        table->stateEntryItemSize = 0x38; /* size of script data per entry */
        table2 = gCutsceneStateTable;
        invalidIdx = 0xFFFF;
        table->freeListTailIndex = 0x1DF; /* last entry in free list */
        table->current_index = 0;
        table->allocatedEventCount = 0;
        table2->slotCount = 0x10; /* 16 character slots */
    }

    {
        StateEntry *table = gCutsceneStateTable;
        itemByteOffset = 0;
        table->frameMask = 0x8000; /* high bit marks the sentinel end-of-list frame */
        table->maxEntries = 0x1E0; /* 480 total event entries */
        table->initModelIndex = 0;
        table->defaultEndFrame = 100;
        table->configByte = 0;
    }

    /* Allocate one event entry per slot as each slot's head node.
     * Item fields use raw byte offsets from the table base (itemByteOffset
     * tracks slot n's position as n * sizeof(StateEntryItem)):
     *   items[n].headIndex    at itemByteOffset + 0x20
     *   items[n].characterId  at itemByteOffset + 0x24  (= -1: no character assigned)
     *   items[n].unk6         at itemByteOffset + 0x26
     *   items[n].unk7         at itemByteOffset + 0x27
     *   items[n].unk8         at itemByteOffset + 0x28
     *   items[n].unk9         at itemByteOffset + 0x29
     * The allocated entry's linked list pointers are both set to 0xFFFF (no neighbours),
     * and initializeCutsceneCommand sets up its initial command from the ROM asset. */
    do {
        u8 *itemBytes;
        u8 *itemBytes2;
        u8 *allocatedEntryBytes;
        s32 slotIdxByte;
        s32 entryByteOffset;

        gCutsceneStateTable->allocatedEventCount++;
        entryIndex = allocateStateEntry();
        itemBytes = (u8 *)gCutsceneStateTable;
        slotIdxByte = slotIdx << 24;
        itemBytes += itemByteOffset;
        itemBytes[0x26] = 0; /* item->unk6 = 0 */
        itemBytes2 = (u8 *)gCutsceneStateTable + itemByteOffset;
        slotIdx++;
        slotIdxByte >>= 24;                                     /* sign-extend to s8 slot index */
        *(s16 *)(itemBytes + 0x20) = entryIndex;                /* item->headIndex = entryIndex */
        *(s16 *)(itemBytes + 0x24) = negOne;                    /* item->characterId = -1 */
        itemBytes2[0x27] = negOne;                              /* item->unk7 = -1 */
        ((u8 *)gCutsceneStateTable + itemByteOffset)[0x28] = 0; /* item->unk8 = 0 */
        entryByteOffset = (entryIndex & 0xFFFF) << 6;
        ((u8 *)gCutsceneStateTable + itemByteOffset)[0x29] = negOne; /* item->unk9 = -1 */
        {
            u8 *tableBase = (u8 *)gCutsceneStateTable;
            s32 cmdOffset = entryByteOffset + 0xC0; /* offset to entry start (skip 3-entry header) */
            itemByteOffset += sizeof(StateEntryItem);
            allocatedEntryBytes = tableBase + entryByteOffset;
            *(u16 *)(allocatedEntryBytes + 0xF8) = invalidIdx; /* entry->next_index = 0xFFFF */
            *(u16 *)(allocatedEntryBytes + 0xFA) = invalidIdx; /* entry->prev_index = 0xFFFF */
            initializeCutsceneCommand(tableBase + cmdOffset, romAssetAddr, 0, 0, slotIdxByte);
        }
    } while (slotIdx < 0x10);

    gCutsceneEntryCopyFlag = 0;
    gCutsceneEntryBufferSlotIndex = -1;
    gCutsceneEntryBufferFrameNumber = -1;
    gCutsceneEntryCutFlag = 0;
}

void cleanupCutsceneSystem(void) {
    if (gCutsceneStateTable != NULL) {
        freeNodeMemory(gCutsceneStateTable);
    }
}

/**
 * Find a state entry for a given slot and frame number.
 *
 * This function searches through the linked list of state entries for a given slot.
 * The entries are sorted by frame number.
 *
 * @param slotIndex The slot index to search (0-15)
 * @param frameNumber The frame number to search for
 * @param findInsertionPoint If non-zero, finds where to insert a new entry.
 *                           If zero, requires an exact match.
 * @return The index of the found entry, or 0xFFFF if not found
 */
u16 findStateEntryIndex(u8 slotIndex, u16 frameNumber, s32 findInsertionPoint) {
    StateEntry *entry;
    u16 current;
    u16 next;
    u16 nextIdx;
    u16 targetFrame;
    s32 mode;

    current = gCutsceneStateTable->items[slotIndex].headIndex;
    mode = findInsertionPoint;

    entry = getStateEntry(current);
    next = entry->next_index;

    if (next != 0xFFFF) {
        targetFrame = frameNumber;
        do {
            entry = getStateEntry(current);
            nextIdx = next;
            if (entry->frameNumber == targetFrame) {
                if ((mode & 0xFF) != 0) {
                    return 0xFFFF;
                }
            }

            entry = getStateEntry(nextIdx);
            if (targetFrame < entry->frameNumber) {
                break;
            }
            current = next;
            entry = getStateEntry(nextIdx);
            next = entry->next_index;
        } while (next != 0xFFFF);
    }

    if ((mode & 0xFF) == 0) {
        entry = getStateEntry(current);
        if (entry->frameNumber != frameNumber) {
            return 0xFFFF;
        }
    }

    return current;
}

s32 findEventAtFrame(u8 a0, u16 a1) {
    StateEntry *base;
    StateEntry *temp;
    u16 current;

    base = gCutsceneStateTable;
    current = base->items[a0].headIndex;

    while (current != 0xFFFF) {
        temp = getStateEntry(current);
        if (a1 < temp->frameNumber) {
            return 0xFFFF;
        }

        temp = getStateEntry(current);
        if (temp->frameNumber == a1) {
            return current;
        }

        temp = getStateEntry(current);
        current = temp->next_index;
    }

    return 0xFFFF;
}

/**
 * Insert a new cutscene event into the linked list for a given slot.
 * Events are stored in a doubly-linked list sorted by frame number.
 *
 * This function inserts a new event after the event with the largest
 * frame number less than the target frame. If an event with the target
 * frame number already exists, the insertion fails.
 *
 * @param slotIndex The slot index (0-15) to insert into
 * @param frameNumber The frame number for the new event
 * @return The index of the newly inserted event, or 0xFFFF on failure
 */
s32 insertCutsceneEvent(u8 slotIndex, u16 frameNumber) {
    u16 insertAfterIndex;
    u16 entryIndex;
    u16 allocatedIndex;
    u16 nextIndex;
    StateEntry *entry;
    StateEntry *newEntry;

    if (gCutsceneStateTable->allocatedEventCount >= 0x1E0) {
        goto return_error;
    }

    insertAfterIndex = findStateEntryIndex(slotIndex, frameNumber, 1);
    entryIndex = insertAfterIndex;

    if (entryIndex == 0xFFFF) {
        return 0xFFFF;
    }

    entry = getStateEntry(entryIndex);
    if (entry->frameNumber != frameNumber) {
        goto insert_entry;
    }

return_error:
    return 0xFFFF;

insert_entry:
    allocatedIndex = allocateStateEntry();

    entry = gCutsceneStateTable;
    nextIndex = *(u16 *)((u8 *)entry + (u32)(entryIndex << 6) + 0xF8);
    *(u16 *)((u8 *)entry + (u32)(entryIndex << 6) + 0xF8) = allocatedIndex;

    if (nextIndex != 0xFFFF) {
        *(u16 *)((u8 *)entry + (u32)(nextIndex << 6) + 0xFA) = allocatedIndex;
    }

    newEntry = gCutsceneStateTable;
    entryIndex = allocatedIndex;
    newEntry = &newEntry[entryIndex];
    newEntry[3].prev_index = insertAfterIndex;
    newEntry[3].next_index = nextIndex;

    initializeStateEntry(entryIndex);

    gCutsceneStateTable->allocatedEventCount++;

    if (entryIndex != 0xFFFF) {
        entry = getStateEntry(entryIndex);
        entry->frameNumber = frameNumber;

        entry = getStateEntry(entryIndex);
        entry->commandCategory = 0;

        entry = getStateEntry(entryIndex);
        entry->commandType = 0;
    }

    return entryIndex;
}

/**
 * Reorder a cutscene event by moving it from after one entry to after another.
 * This function manipulates a doubly-linked list of cutscene events.
 *
 * The function moves the event at `eventIndex` from its current position
 * (after `oldPreviousIndex`) to a new position (after `newPreviousIndex`).
 *
 * @param eventIndex The index of the event to move
 * @param oldPreviousIndex The index of the entry that was before the event (before the move)
 * @param newPreviousIndex The index of the entry that should be before the event (after the move)
 */
void reorderCutsceneEvent(u16 eventIndex, u16 oldPreviousIndex, u16 newPreviousIndex) {
    u8 *eventPtr;
    u32 oldPrevOffset;
    u16 newPrevIdx;
    u32 eventOffset;
    u16 nextIdx;
    u16 prevIdx;
    u8 *table;
    u8 *table2;
    u32 newPrevOffset;

    newPrevIdx = newPreviousIndex & 0xFFFF;
    getStateEntry(newPrevIdx);

    eventOffset = eventIndex & 0xFFFF;
    getStateEntry(eventOffset);

    oldPrevOffset = oldPreviousIndex & 0xFFFF;
    getStateEntry(oldPrevOffset);

    table = (u8 *)gCutsceneStateTable;

    oldPrevOffset = oldPrevOffset << 6;
    eventOffset = eventOffset << 6;

    nextIdx = *(u16 *)(table + oldPrevOffset + 0xF8);
    eventPtr = table + eventOffset;
    prevIdx = *(u16 *)(eventPtr + 0xFA);

    if (nextIdx != 0xFFFF) {
        *(u16 *)(table + (nextIdx << 6) + 0xFA) = prevIdx;
    }

    if (prevIdx != 0xFFFF) {
        u8 *t = (u8 *)gCutsceneStateTable;
        *(u16 *)(t + (prevIdx << 6) + 0xF8) = nextIdx;
    }

    table2 = (u8 *)gCutsceneStateTable;
    newPrevOffset = newPrevIdx << 6;
    nextIdx = *(u16 *)(table2 + newPrevOffset + 0xF8);
    *(u16 *)(table2 + newPrevOffset + 0xF8) = eventIndex;
    *(u16 *)(table2 + oldPrevOffset + 0xF8) = nextIdx;

    if (nextIdx != 0xFFFF) {
        *(u16 *)(table2 + (nextIdx << 6) + 0xFA) = oldPreviousIndex;
    }
}

StateEntry *getStateEntry(u16 arg0) {
    return &gCutsceneStateTable[arg0 + 3];
}

/**
 * Renders a vertical menu of cutscene slots with one item highlighted.
 *
 * @param arg0 UI resource identifier
 * @param arg1 Index of the slot to highlight (0-15)
 */
void renderCutsceneSlotMenu(s32 arg0, s16 arg1) {
    s32 count;
    s32 i;
    s32 yPos;
    s32 target;
    volatile s32 pad;

    count = gCutsceneStateTable->slotCount;
    i = 0;
    if (count > 0) {
        target = arg1;
        yPos = 0x30000;
    loop:
        if (i == target) {
            func_800BB2F4(arg0, yPos >> 16, 7);
        } else {
            func_800BB2F4(arg0, yPos >> 16, 3);
        }
        yPos += 0x10000;
        i++;
        if (i < gCutsceneStateTable->slotCount) {
            goto loop;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/cutscene/cutscene_manager", func_800B3FFC_1E10AC);

u16 getMaxCutsceneFrameNumber(void) {
    u16 maxFrameNumber;
    u16 slotIndex;
    u16 entryIndex;
    StateEntry *entry;
    u16 currentFrame;

    maxFrameNumber = 0;
    slotIndex = 0;

    while (slotIndex < getCutsceneSlotCount()) {
        entryIndex = findStateEntryIndex(slotIndex, 0x8000, 1);
        entry = getStateEntry(entryIndex);
        currentFrame = entry->frameNumber;
        if (maxFrameNumber < currentFrame) {
            maxFrameNumber = currentFrame;
        }
        slotIndex++;
    }

    return maxFrameNumber;
}

u16 getSlotLastFrameNumber(u8 slotIndex) {
    u16 entryIndex;
    StateEntry *entry;

    entryIndex = findStateEntryIndex(slotIndex, 0x8000, 1);
    entry = getStateEntry(entryIndex);
    return entry->frameNumber;
}

StateEntry *getCutsceneStateTable(void) {
    return gCutsceneStateTable;
}

u8 *getCutsceneStateTableLastBytePtr(void) {
    return (u8 *)gCutsceneStateTable + gCutsceneStateTableSize - 1;
}

u16 getCutEntryBufferSlotIndex(u16 defaultSlotIndex) {
    if (gCutsceneEntryCutFlag != 0) {
        s16 storedSlotIndex = gCutsceneEntryBufferSlotIndex;
        if (storedSlotIndex != -1) {
            return storedSlotIndex;
        }
    }
    return defaultSlotIndex;
}

u16 getCutEntryBufferFrameNumber(u16 defaultFrameNumber) {
    if (gCutsceneEntryCutFlag == 0) {
        return defaultFrameNumber;
    }
    if (gCutsceneEntryBufferFrameNumber == -1) {
        return defaultFrameNumber;
    }
    return gCutsceneEntryBufferFrameNumber;
}

void pasteCutsceneEntry(void) {
    s16 storedSlotIndex;
    s16 storedFrameNumber;
    u8 slotIndex;
    u16 frameNumber;

    if (gCutsceneEntryCutFlag != 0) {
        storedSlotIndex = gCutsceneEntryBufferSlotIndex;
        if (storedSlotIndex != -1) {
            storedFrameNumber = gCutsceneEntryBufferFrameNumber;
            if (storedFrameNumber != -1) {
                slotIndex = storedSlotIndex;
                frameNumber = storedFrameNumber;
                pasteCutsceneEntryToSlot(slotIndex, frameNumber);
            }
        }
    }
    gCutsceneEntryCutFlag = 0;
}

void cutCutsceneEntry(u8 slotIndex, s16 frameNumber) {
    u16 searchResult;
    u16 entryIndex;
    u16 nextIndex;
    u16 prevIndex;
    u16 freeListTail;
    StateEntry *table;

    searchResult = findStateEntryIndex(slotIndex, frameNumber & 0xFFFF, 0);
    entryIndex = searchResult & 0xFFFF;

    if (entryIndex == 0xFFFF) {
        return;
    }

    if (getStateEntry(entryIndex)->frameNumber == 0) {
        return;
    }

    memcpy(gCutsceneEntryBuffer, getStateEntry(entryIndex), sizeof(StateEntry));

    table = gCutsceneStateTable;
    freeListTail = table->freeListTailIndex;
    nextIndex = *(u16 *)((u8 *)table + (entryIndex << 6) + 0xF8);
    prevIndex = *(u16 *)((u8 *)table + (entryIndex << 6) + 0xFA);

    *(u16 *)((u8 *)table + (freeListTail << 6) + 0xF8) = searchResult;
    table->freeListTailIndex = searchResult;

    if ((prevIndex & 0xFFFF) != 0xFFFF) {
        *(u16 *)((u8 *)table + (prevIndex << 6) + 0xF8) = nextIndex;
    }

    if ((nextIndex & 0xFFFF) != 0xFFFF) {
        *(u16 *)((u8 *)gCutsceneStateTable + (nextIndex << 6) + 0xFA) = prevIndex;
    }

    gCutsceneEntryCopyFlag = 1;
    gCutsceneEntryBufferSlotIndex = slotIndex;
    gCutsceneEntryBufferFrameNumber = frameNumber;
    gCutsceneEntryCutFlag = 1;
    gCutsceneStateTable->allocatedEventCount -= 1;
}

/**
 * Copy a cutscene entry to the global buffer for later pasting.
 *
 * Unlike cutCutsceneEntry, this leaves the original entry in place.
 *
 * @param slotIndex The slot index (0-15) containing the entry
 * @param frameNumber The frame number of the entry to copy
 */
void copyCutsceneEntry(u8 slotIndex, u16 frameNumber) {
    u16 eventId;
    StateEntry *src;
    StateEntry *dst;
    u8 *srcBytes;
    u8 *dstBytes;
    s32 i;
    u8 temp;

    eventId = findEventAtFrame(slotIndex, frameNumber);

    // Copy the current entry to gCutsceneEntryBuffer
    // We copy 0x38 bytes which is everything before next_index/prev_index linked list pointers,
    // then separately copy commandCategory and commandType
    if (eventId != 0xFFFF) {
        src = getStateEntry(eventId);
        dst = &gCutsceneEntryBuffer[0];
        srcBytes = (u8 *)src;
        dstBytes = (u8 *)dst;

        // Copy the data portion (everything except linked list pointers)
        for (i = 0; i < 0x38; i++) {
            dstBytes[i] = srcBytes[i];
        }

        dst->commandCategory = src->commandCategory;
        temp = src->commandType;
        gCutsceneEntryCopyFlag = 1;
        gCutsceneEntryCutFlag = 0;
        dst->commandType = temp;
    }
}

/**
 * Paste a previously copied/cut cutscene entry to a slot and frame.
 *
 * If an entry doesn't exist at the target location, a new one is created.
 * The entry data from the global buffer is copied to the target entry.
 *
 * @param slotIndex The slot index (0-15) to paste into
 * @param frameNumber The frame number where the entry should be pasted
 */
void pasteCutsceneEntryToSlot(u8 slotIndex, u16 frameNumber) {
    s32 categorySkip;
    StateEntry *dest;
    StateEntry *srcEntry;
    s32 i;

    srcEntry = gCutsceneEntryBuffer;
    categorySkip = (-((~getCurrentStateEntryItem(slotIndex)->characterId) != 0)) | 1;

    if (gCutsceneEntryCopyFlag != 0 && getCategorySkipValue(D_800BAF06_1E7FB6) != categorySkip) {
        s32 eventIndex = findEventAtFrame(slotIndex, frameNumber);
        if ((eventIndex & 0xFFFF) == 0xFFFF) {
            eventIndex = insertCutsceneEvent(slotIndex, frameNumber);
        }

        dest = getStateEntry(eventIndex);
        // Copy the data portion (0x38 bytes = everything before linked list pointers)
        for (i = 0; i < 0x38; i++) {
            ((u8 *)(ScriptData *)dest)[i] = ((u8 *)(ScriptData *)srcEntry)[i];
        }

        dest->commandCategory = srcEntry->commandCategory;
        dest->commandType = srcEntry->commandType;

        gCutsceneEntryCutFlag = 0;
    }
}

void shiftCutsceneEntryFrame(u8 slotIndex, u16 frameNumber, s32 frameDelta) {
    u16 result;
    StateEntry *entry;

    result = findStateEntryIndex(slotIndex, frameNumber, 0);

    if (result != 0xFFFF) {
        entry = getStateEntry(result);
        entry->frameNumber += frameDelta;
    }
}

void *loadCutsceneSlotAsset(s8 slotIndex) {
    CutsceneFadeAssetNode *node;

    if (slotIndex < 0x10) {
        node = &gCutsceneFadeAssetTable[slotIndex];
        if (gCutsceneFadeAssetTable[slotIndex].start != NULL) {
            return loadCompressedData(node->start, node->end, node->size);
        }
    }

    return NULL;
}

void startCutsceneFadeEffect(s32 arg0, s8 slotIndex, s16 duration) {
    FadeTaskData *task;

    if (duration == 0) {
        return;
    }

    if (slotIndex >= 0x10) {
        return;
    }

    if (gCutsceneFadeAssetTable[slotIndex].start == NULL) {
        return;
    }

    task = (FadeTaskData *)scheduleTask(&initCutsceneFadeTask, 1, 0, 0x64);
    if (task != NULL) {
        task->state = 0;
        task->slotIndex = slotIndex;
        task->duration = duration;
        task->unk04.unk4 = arg0;
        task->fadeAlpha = 0xFF;
    }
}

void initCutsceneFadeTask(FadeTaskData *task) {
    CutsceneFadeAssetNode *node;
    s32 i;

    node = &gCutsceneFadeAssetTable[task->slotIndex];
    task->assetData = loadCompressedData(node->start, node->end, node->size);

    for (i = 0; i < 6; i++) {
        task->sprites[i].flags = 0;
        task->sprites[i].y = -0x50 + (i * 0x10);
        task->sprites[i].assetData = task->assetData;
        task->sprites[i].slotIndex = i;
        task->sprites[i].scaleX = 0x400;
        task->sprites[i].scaleY = 0x400;
        task->sprites[i].rotation = 0;
        task->sprites[i].alpha = 0xFF;
        task->sprites[i].r = 0;
        task->sprites[i].g = 0;
        task->sprites[i].b = 0;
    }

    switch (node->fadeType) {
        case 0:
            task->centerSprite.flags = 0;
            task->centerSprite.y = -0x20;
            task->centerSprite.slotIndex = 6;
            task->centerSprite.scaleX = 0x400;
            task->centerSprite.scaleY = 0x400;
            task->centerSprite.rotation = 0;
            task->centerSprite.alpha = 0xFF;
            task->centerSprite.r = 0;
            task->centerSprite.g = 0;
            task->centerSprite.b = 0;
            task->centerSprite.assetData = task->assetData;
            break;
        case 1:
            task->centerSprite.flags = 0;
            task->centerSprite.y = -0x28;
            task->centerSprite.slotIndex = 6;
            task->centerSprite.scaleX = 0x400;
            task->centerSprite.scaleY = 0x400;
            task->centerSprite.rotation = 0;
            task->centerSprite.alpha = 0xFF;
            task->centerSprite.r = 0;
            task->centerSprite.g = 0;
            task->centerSprite.b = 0;
            task->centerSprite.assetData = task->assetData;

            task->bottomSprite.flags = 0;
            task->bottomSprite.y = -0x16;
            task->bottomSprite.slotIndex = 7;
            task->bottomSprite.scaleX = 0x400;
            task->bottomSprite.scaleY = 0x400;
            task->bottomSprite.rotation = 0;
            task->bottomSprite.alpha = 0xFF;
            task->bottomSprite.r = 0;
            task->bottomSprite.g = 0;
            task->bottomSprite.b = 0;
            task->bottomSprite.assetData = task->assetData;
            break;
    }

    task->fadeAlpha = 0;
    setCleanupCallback((void (*)(void *))cleanupCutsceneFadeTask);
    setCallback((void (*)(void *))updateCutsceneFadeTask);
}

void updateCutsceneFadeTask(FadeTaskData *task) {
    CutsceneFadeAssetNode *node;
    s32 i;

    node = &gCutsceneFadeAssetTable[task->slotIndex];

    switch (task->state) {
        case 0:
            task->fadeAlpha += 10;
            if (task->fadeAlpha >= 0xFF) {
                task->fadeAlpha = 0xFF;
                task->state = 1;
            }
            break;
        case 1:
            task->duration--;
            if (task->duration < 0) {
                task->state = 2;
            }
            break;
        case 2:
            task->fadeAlpha -= 10;
            if (task->fadeAlpha <= 0) {
                terminateCurrentTask();
                return;
            }
            break;
    }

    switch (node->fadeType) {
        case 0:
            task->centerSprite.b = (u8)task->fadeAlpha;
            debugEnqueueCallback(task->unk04.split.unk6, 0, &renderScaledAlphaSpriteFrame, &task->centerSprite);
            break;
        case 1:
            task->centerSprite.b = (u8)task->fadeAlpha;
            debugEnqueueCallback(task->unk04.split.unk6, 0, &renderScaledAlphaSpriteFrame, &task->centerSprite);
            task->bottomSprite.b = (u8)task->fadeAlpha;
            debugEnqueueCallback(task->unk04.split.unk6, 0, &renderScaledAlphaSpriteFrame, &task->bottomSprite);
            break;
    }

    for (i = 0; i < 6; i++) {
        task->sprites[i].b = (u8)task->fadeAlpha;
        debugEnqueueCallback(task->unk04.split.unk6, 0, &renderScaledAlphaSpriteFrame, &task->sprites[i]);
    }
}

void cleanupCutsceneFadeTask(FadeTaskData *task) {
    freeNodeMemory(task->assetData);
}
