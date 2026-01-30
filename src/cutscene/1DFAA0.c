#include "1DFAA0.h"
#include "10AD0.h"
#include "1DD170.h"
#include "1E2BE0.h"
#include "1E36C0.h"
#include "20F0.h"
#include "3B80.h"
#include "68CF0.h"
#include "90D0.h"
#include "CBA0.h"
#include "controller_io.h"
#include "graphics.h"
#include "main.h"
#include "task_scheduler.h"

typedef struct {
    u8 padding[0x2D8];
    void *unk2D8;
    void *unk2DC;
    s32 unk2E0;
} InitCutsceneManager_slot16;

typedef struct {
    void *romStart;
    void *romEnd;
    s32 decompressedSize;
} CutsceneFrameInfo;

typedef struct {
    CutsceneFrameInfo *introFrames;
    s16 introFrameCount;
    s16 pad06;
    CutsceneFrameInfo *winFrames;
    s16 winFrameCount;
    s16 pad0E;
    CutsceneFrameInfo *loseFrames;
    s16 loseFrameCount;
    s16 pad16;
} CutsceneAssetTable;

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
extern u8 identityMatrix[];
extern CutsceneAssetTable gCutsceneAssetTable[];

extern void *func_800B5B38_1E2BE8(u16);
extern void initializeCutsceneCommand(void *, void *, s32, s32, s32);

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
    s32 needsVisibility = initModelEntity((ModelEntity *)&manager->unk10, modelIndex, sceneNode);

    setModelRenderMode(&manager->unk10, 0);

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

    manager->sceneContext = func_800B5B38_1E2BE8(sceneNode->id);

    if (needsVisibility) {
        scheduleDualAssetGroupLoad(manager, 0, 0x8000, 1, 0x10000);
        setAnimationLoopMode(manager->sceneContext, 1);
    }

    initSceneRenderNode(&manager->unkFF8, 2, 0xC, sceneNode->id, 0, 2, 0);
    clearAuxRenderEnabled(&manager->unkFF8);

    manager->shadowModel = shadowModel;
    manager->reflectionModel = reflectionModel;
    manager->cameraAnimationTimer = 0x10000;
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

    freeEffectResources((EffectState *)&manager->unk10);
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
            applyTransformToModel(slots[i].slotData.unkA4.ptr, (Transform3D *)identityMatrix);
            setModelAnimation(slots[i].slotData.unkA4.ptr, 0);
        }
    }
}

void resetAllSlotTransforms(CutsceneManager *manager) {
    s32 i;
    SceneModel *model;

    i = 0;

    while (i < getCutsceneSlotCount()) {
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

        i++;
    }
}

void prepareCutsceneForPlayback(
    CutsceneManager *manager,
    s32 uiResource,
    s32 pad4_0,
    s32 pad4_4,
    u16 maxFrame,
    u8 showDebugInfo
) {
    SceneModel *model;
    s32 i;
    u16 modelId;

    i = 0;
    *(s32 *)&manager->uiResource = uiResource;
    *(s32 *)&manager->pad4[0] = pad4_0;
    *(s32 *)&manager->pad4[4] = pad4_4;
    manager->currentFrame = 0;
    manager->maxFrame = maxFrame;
    manager->endFrame = getCutsceneDefaultEndFrame();
    do {
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
        i += 1;
    } while (i < 0x10);
    *(s16 *)&manager->textRenderer = -0x90;
    *((s16 *)&manager->textRenderer + 1) = 0x68;
    *(s16 *)&manager->padFEC[0] = 0;
    *(s32 *)&manager->padFEC[4] = (s32)&manager->debugText[0];
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
    s32 cameraAnimTimer;
    u16 eventIndex;
    u8 eventType;

    if (cutsceneManager->showDebugInfo) {
        sprintf((char *)cutsceneManager->debugText, gDebugFrameFormatString, cutsceneManager->currentFrame);
        debugEnqueueCallback(
            cutsceneManager->uiResource->slot_index,
            6,
            &renderTextPalette,
            &cutsceneManager->textRenderer
        );
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

    cameraAnimTimer = cutsceneManager->cameraAnimationTimer;
    cameraOffsetX = -((cameraAnimTimer * 120) >> 16);
    cameraScaleZ = (cameraAnimTimer * 119) >> 16;

    setModelRenderMode(&cutsceneManager->unk10, cutsceneManager->enableTransparency);
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
    return &gCutsceneStateTable->scriptData[4];
}

void *getCutsceneDataMagicSecondary(void) {
    return &gCutsceneStateTable->scriptData[8];
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
    return &gCutsceneStateTable->items[itemIndex];
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
    s32 tableOffset;

    frameInfo = NULL;

    if (cutsceneType >= 3) {
        cutsceneTypeClamped = 2;
    } else {
        cutsceneTypeClamped = cutsceneType & (~cutsceneType >> 31);
    }

    if (slotIndex >= 16) {
        return NULL;
    }

    tableOffset = slotIndex * 0x18;
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
    s32 *globalState;
    s32 *inputState;

    inputState = (s32 *)stateBuffer;
    globalState = (s32 *)gCutsceneStateTable;

    success = 1;

    // Verify magic values at offsets 0x4 and 0x8 before loading state
    // These are part of the "EDDAT001" signature (0x45='E' at offset 4, 0x30='0' at offset 8)
    if (globalState[1] != inputState[1]) {
        goto skip_copy;
    }

    if (globalState[2] != inputState[2]) {
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

typedef struct {
    u8 _pad0[0x4];
    s32 unk4;
    s32 unk8;
} SaveDataBuffer;

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

void initializeStateEntry(s32 arg0) {
    StateEntry *temp;

    resetScriptState(gCutsceneStateTable[arg0 + 3].scriptData);

    temp = (gCutsceneStateTable + arg0 + 3);
    temp->frameNumber = 0;

    temp = gCutsceneStateTable + arg0 + 3;
    temp->commandCategory = 0;

    temp = gCutsceneStateTable + arg0 + 3;
    temp->commandType = 0;
}

void initializeCutsceneSystem(void *romAssetAddr) {
    u8 *base;
    s32 slotIdx;
    s32 next;
    s32 itemByteOffset;
    u16 entryIndex;
    s32 negOne;
    u16 invalidIdx;

    // Allocate cutscene state table (480 StateEntry structs × 64 bytes = 0x7800, plus 0xE0 byte header = 0x78E0 total)
    gCutsceneStateTableSize = 0x78E0;
    gCutsceneStateTable = allocateNodeMemory(0x78E0);

    // Initialize free list: link all StateEntry structs via next/prev indices
    // Each entry points to the next (idx+1) and previous (idx-1) in the array
    slotIdx = 0;
    do {
        StateEntry *entry;
        s32 entryByteOffset;
        s32 prev;

        entry = gCutsceneStateTable;
        entryByteOffset = slotIdx << 6; // Each StateEntry is 64 bytes
        next = slotIdx + 1;
        prev = slotIdx - 1;
        base = (u8 *)entry + entryByteOffset;
        *(s16 *)(base + 0xF8) = next; // next_index
        *(s16 *)(base + 0xFA) = prev; // prev_index
        initializeStateEntry(slotIdx);
        slotIdx = next;
    } while (next < 0x1E0); // Initialize 480 entries (0x1E0)

    // Set up magic signature "EDDAT001" in scriptData (used for save data validation)
    {
        StateEntry *firstEntry = gCutsceneStateTable;
        StateEntry *firstEntry2;
        firstEntry->scriptData[4] = 0x45; // 'E'
        firstEntry2 = gCutsceneStateTable;
        *(u16 *)((u8 *)firstEntry + 0xFA) = 0xFFFF;   // entry[0].prev_index = invalid (list head)
        *(u16 *)((u8 *)firstEntry + 0x78B8) = 0xFFFF; // entry[479].prev_index = invalid (list tail)
        *(s32 *)firstEntry = 0;                       // Clear first 4 bytes (part of magic)
        firstEntry2->scriptData[5] = 0x44;            // 'D'
    }
    gCutsceneStateTable->scriptData[6] = 0x41;  // 'A'
    gCutsceneStateTable->scriptData[7] = 0x54;  // 'T'
    gCutsceneStateTable->scriptData[8] = 0x30;  // '0'
    gCutsceneStateTable->scriptData[9] = 0x30;  // '0'
    gCutsceneStateTable->scriptData[10] = 0x30; // '0'
    slotIdx = 0;
    gCutsceneStateTable->scriptData[11] = 0x31; // '1'

    // Initialize state table header fields
    {
        StateEntry *table = gCutsceneStateTable;
        StateEntry *table2;
        negOne = -1;
        table->stateEntryItemSize = 0x38;
        table2 = gCutsceneStateTable;
        invalidIdx = 0xFFFF;                 // Invalid index marker
        *(s16 *)((u8 *)table + 0xE) = 0x1DF; // Free list tail index
        table->current_index = 0;
        table->allocatedEventCount = 0;
        table2->slotCount = 0x10; // 16 cutscene slots
    }

    // Initialize remaining header fields
    {
        StateEntry *table = gCutsceneStateTable;
        itemByteOffset = 0;
        table->frameMask = 0x8000;
        table->maxEntries = 0x1E0; // Max 480 state entries
        table->initModelIndex = 0;
        table->defaultEndFrame = 100; // Default animation end frame
        table->configByte = 0;
    }

    // Initialize 16 slot items (one for each cutscene slot: characters, cameras, etc.)
    do {
        u8 *itemBytes;
        u8 *itemBytes2;
        u8 *entryBytes;
        s32 slotIdxByte;
        s32 entryByteOffset;

        gCutsceneStateTable->allocatedEventCount++;
        entryIndex = allocateStateEntry();
        itemBytes = (u8 *)gCutsceneStateTable;
        slotIdxByte = slotIdx << 24; // Isolate low byte via shift
        itemBytes += itemByteOffset;
        itemBytes[0x26] = 0; // StateEntryItem unk26 field
        itemBytes2 = (u8 *)gCutsceneStateTable + itemByteOffset;
        slotIdx++;
        slotIdxByte >>= 24;
        *(s16 *)(itemBytes + 0x20) = entryIndex;                // headIndex: first event in linked list
        *(s16 *)(itemBytes + 0x24) = negOne;                    // unk4: model asset index (-1 = none)
        itemBytes2[0x27] = negOne;                              // unk7
        ((u8 *)gCutsceneStateTable + itemByteOffset)[0x28] = 0; // unk8
        entryByteOffset = (entryIndex & 0xFFFF) << 6;
        ((u8 *)gCutsceneStateTable + itemByteOffset)[0x29] = negOne; // unk9
        {
            u8 *tableBase = (u8 *)gCutsceneStateTable;
            s32 cmdOffset = entryByteOffset + 0xC0; // Offset to scriptData in entry
            itemByteOffset += 0xA;                  // Each StateEntryItem is 10 bytes
            entryBytes = tableBase + entryByteOffset;
            *(u16 *)(entryBytes + 0xF8) = invalidIdx; // next_index
            *(u16 *)(entryBytes + 0xFA) = invalidIdx; // prev_index
            initializeCutsceneCommand(tableBase + cmdOffset, romAssetAddr, 0, 0, slotIdxByte);
        }
    } while (slotIdx < 0x10);

    // Initialize global cutscene entry buffer flags (for copy/paste operations)
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

    // Get the next_index from the entry we're inserting after
    nextIndex = *(u16 *)((u8 *)entry + (u32)(entryIndex << 6) + 0xF8);
    // Update the entry's next_index to point to the new entry
    *(u16 *)((u8 *)entry + (u32)(entryIndex << 6) + 0xF8) = allocatedIndex;

    if (nextIndex != 0xFFFF) {
        // Update the original next entry's prev_index to point to the new entry
        *(u16 *)((u8 *)entry + (u32)(nextIndex << 6) + 0xFA) = allocatedIndex;
    }

    newEntry = gCutsceneStateTable;
    entryIndex = allocatedIndex;
    newEntry = (StateEntry *)((u8 *)newEntry + (u32)(entryIndex << 6));
    // Set the new entry's prev_index to point to the entry we inserted after
    *(u16 *)((u8 *)newEntry + 0xFA) = insertAfterIndex;
    // Set the new entry's next_index to point to the original next entry
    *(u16 *)((u8 *)newEntry + 0xF8) = nextIndex;

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
    u16 newPrevMasked;
    u16 eventIdxMasked;
    u16 oldPrevMasked;
    u32 eventOffset;
    u16 nextIdx;
    u16 prevIdx;
    u8 *tableBase;
    u8 *tablePtr;
    u32 newPrevOffset;

    // Mask all indices to u16 and call getStateEntry (for validation)
    eventIdxMasked = eventIndex;
    oldPrevMasked = oldPreviousIndex;
    newPrevMasked = newPreviousIndex & 0xFFFF;
    getStateEntry(newPrevMasked);

    eventOffset = eventIdxMasked & 0xFFFF;
    getStateEntry(eventOffset);

    oldPrevOffset = oldPrevMasked & 0xFFFF;
    getStateEntry(oldPrevOffset);

    tableBase = (u8 *)gCutsceneStateTable;

    // Convert indices to byte offsets (each entry is 64 bytes)
    oldPrevOffset = oldPrevOffset << 6;
    eventOffset = eventOffset << 6;

    // Read the next_index from old previous entry and prev_index from event entry
    // Offset 0xF8 = 3 * 64 (reserved entries) + 0x38 (next_index offset in StateEntry)
    nextIdx = *(u16 *)(tableBase + oldPrevOffset + 0xF8);
    eventPtr = tableBase + eventOffset;
    // Offset 0xFA = 3 * 64 (reserved entries) + 0x3A (prev_index offset in StateEntry)
    prevIdx = *(u16 *)(eventPtr + 0xFA);

    // Unlink entry from current position: update next entry's prev_index
    if (nextIdx != 0xFFFF) {
        *(u16 *)(tableBase + (nextIdx << 6) + 0xFA) = prevIdx;
    }

    // Unlink entry from current position: update prev entry's next_index
    if (prevIdx != 0xFFFF) {
        u8 *temp = (u8 *)gCutsceneStateTable;
        *(u16 *)(temp + (prevIdx << 6) + 0xF8) = nextIdx;
    }

    // Insert entry after newPreviousIndex
    tablePtr = (u8 *)gCutsceneStateTable;
    newPrevOffset = newPrevMasked << 6;
    nextIdx = *(u16 *)(tablePtr + newPrevOffset + 0xF8);
    *(u16 *)(tablePtr + newPrevOffset + 0xF8) = eventIdxMasked;
    *(u16 *)(tablePtr + oldPrevOffset + 0xF8) = nextIdx;

    // Update the next entry's prev_index to point to the old prev
    if (nextIdx != 0xFFFF) {
        *(u16 *)(tablePtr + (nextIdx << 6) + 0xFA) = oldPrevMasked;
    }
}

StateEntry *getStateEntry(u16 arg0) {
    return &gCutsceneStateTable[arg0 + 3];
}

extern void func_800BB2F4(s32, s32, s32);

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

INCLUDE_ASM("asm/nonmatchings/cutscene/1DFAA0", func_800B3FFC_1E10AC);

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
    return &gCutsceneStateTable->scriptData[gCutsceneStateTableSize] - 1;
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
    u16 entryNextIndex;
    u16 entryPrevIndex;
    u16 freeListHead;
    StateEntry *base;

    // Find the state entry for the given slot and frame
    searchResult = findStateEntryIndex(slotIndex, frameNumber & 0xFFFF, 0);
    entryIndex = searchResult & 0xFFFF;

    if (entryIndex == 0xFFFF) {
        return;
    }

    // Don't allow cutting the frame 0 entry
    if (getStateEntry(entryIndex)->frameNumber == 0) {
        return;
    }

    // Copy the entry to the global buffer for paste operations
    memcpy(gCutsceneEntryBuffer, getStateEntry(entryIndex), 0x40);

    // Get the state table and linked list pointers
    base = gCutsceneStateTable;
    freeListHead = *(u16 *)((u8 *)base + 0xE);                        // Free list head is at offset 0xE
    entryNextIndex = *(u16 *)((u8 *)base + (entryIndex << 6) + 0xF8); // next_index
    entryPrevIndex = *(u16 *)((u8 *)base + (entryIndex << 6) + 0xFA); // prev_index

    // Add the cut entry to the free list (insert at head)
    *(u16 *)((u8 *)base + (freeListHead << 6) + 0xF8) = searchResult;
    *(u16 *)((u8 *)base + 0xE) = searchResult;

    // Update the linked list: unlink the entry from its current position
    if ((entryPrevIndex & 0xFFFF) != 0xFFFF) {
        *(u16 *)((u8 *)base + (entryPrevIndex << 6) + 0xF8) = entryNextIndex;
    }

    if ((entryNextIndex & 0xFFFF) != 0xFFFF) {
        *(u16 *)((u8 *)gCutsceneStateTable + (entryNextIndex << 6) + 0xFA) = entryPrevIndex;
    }

    // Set global flags to indicate a cut operation
    gCutsceneEntryCopyFlag = 1;
    gCutsceneEntryBufferSlotIndex = slotIndex;
    gCutsceneEntryBufferFrameNumber = frameNumber;
    gCutsceneEntryCutFlag = 1;
    gCutsceneStateTable->allocatedEventCount -= 1;
}

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
    if (eventId != 0xFFFF) {
        src = getStateEntry(eventId);
        dst = &gCutsceneEntryBuffer[0];
        srcBytes = (u8 *)src;
        dstBytes = (u8 *)dst;

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
        for (i = 0; i < 0x38; i++) {
            dest->scriptData[i] = srcEntry->scriptData[i];
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
    TaskData *task;

    if (duration == 0) {
        return;
    }

    if (slotIndex >= 0x10) {
        return;
    }

    if (gCutsceneFadeAssetTable[slotIndex].start == NULL) {
        return;
    }

    task = (TaskData *)scheduleTask(&initCutsceneFadeTask, 1, 0, 0x64);
    if (task != NULL) {
        task->state = 0;
        task->slotIndex = slotIndex;
        task->duration = duration;
        task->unk4 = arg0;
        task->fadeAlpha = 0xFF;
    }
}

typedef struct {
    void *start;
    void *end;
    u32 size;
    s8 fadeType;
} CutsceneFadeAsset;

typedef struct {
    s16 flags;
    s16 y;
    void *assetData;
    s16 slotIndex;
    s16 scaleY;
    s16 scaleX;
    s16 rotation;
    s16 alpha;
    u8 r;
    u8 g;
    u8 b;
} FadeSprite;

typedef struct {
    s8 state;
    s8 slotIndex;
    s16 duration;
    s32 unk4;
    void *assetData;
    FadeSprite sprites[6];
    FadeSprite centerSprite;
    FadeSprite bottomSprite;
    u8 unkCC[0x18];
    s16 fadeAlpha;
} FadeTaskData2;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-non-prototype"
#pragma clang diagnostic ignored "-Wstrict-prototypes"
extern void updateCutsceneFadeTask();
extern void cleanupCutsceneFadeTask();
#pragma clang diagnostic pop

void initCutsceneFadeTask(void *varg0) {
    FadeTaskData2 *task = (FadeTaskData2 *)varg0;
    CutsceneFadeAsset *node;
    s32 i;

    node = (CutsceneFadeAsset *)&gCutsceneFadeAssetTable[task->slotIndex];
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

typedef struct {
    s8 state;
    s8 slotIndex;
    s16 duration;
    s16 unk4;
    u16 unk6;
    void *unk8;
    u8 sprites[0x90];
    u8 centerSprite[0x18];
    u8 bottomSprite[0x18];
    u8 unkCC[0x18];
    s16 fadeAlpha;
} FadeTaskData;

void updateCutsceneFadeTask(FadeTaskData *task) {
    CutsceneFadeAsset *node;
    s32 offset;
    u8 *ptr;
    s32 i;

    node = (CutsceneFadeAsset *)&gCutsceneFadeAssetTable[task->slotIndex];

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
            task->centerSprite[0x14] = (u8)task->fadeAlpha;
            debugEnqueueCallback(task->unk6, 0, &func_80011924_12524, &task->centerSprite);
            break;
        case 1:
            task->centerSprite[0x14] = (u8)task->fadeAlpha;
            debugEnqueueCallback(task->unk6, 0, &func_80011924_12524, &task->centerSprite);
            task->bottomSprite[0x14] = (u8)task->fadeAlpha;
            debugEnqueueCallback(task->unk6, 0, &func_80011924_12524, &task->bottomSprite);
            break;
    }

    i = 0;
    offset = 0xC;
    ptr = (u8 *)task;
    do {
        ptr[0x20] = (u8)task->fadeAlpha;
        debugEnqueueCallback(task->unk6, 0, &func_80011924_12524, (u8 *)task + offset);
        offset += 0x18;
        ptr += 0x18;
        i++;
    } while (i < 6);
}

typedef struct {
    u8 padding[0x8];
    void *assetData;
} CutsceneFadeCleanupArgs;
void cleanupCutsceneFadeTask(CutsceneFadeCleanupArgs *args) {
    freeNodeMemory(args->assetData);
}
