#include "1DFAA0.h"
#include "10AD0.h"
#include "1DD170.h"
#include "1E2BE0.h"
#include "1E36C0.h"
#include "20F0.h"
#include "3B80.h"
#include "68CF0.h"
#include "6E840.h"
#include "90D0.h"
#include "CBA0.h"
#include "controller_io.h"
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
extern s8 D_800BAE00_1E7EB0[];
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

void initCutsceneManager(CutsceneManager *manager, Node_70B00 *sceneNode, void *shadowModel, void *reflectionModel) {
    s32 i;
    s16 modelIndex = getCutsceneInitModelIndex();
    s32 needsVisibility = initModelEntity((ModelEntity *)&manager->unk10, modelIndex, sceneNode);

    setModelRenderMode(&manager->unk10, 0);

    for (i = 0; i < (getCutsceneSlotCount() & 0xFF); i++) {
        StateEntryItem *slot = getCurrentStateEntryItem(i);
        manager->slots[i].unk40 = slot->unk4;

        if (slot->unk4 != -1) {
            if (isAssetGroupEmpty(slot->unk4)) {
                manager->slots[i].model =
                    createSceneModelEx(slot->unk4, sceneNode, slot->unk6, slot->unk7, slot->unk8, slot->unk9);
            } else {
                manager->slots[i].model = createSceneModel(slot->unk4, sceneNode);
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

    n_alSeqpDelete((Node_70B00 *)&manager->unkFF8);

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
    CutsceneManager *iterPtr;
    s32 byteOffset;
    SceneModel *model;
    s32 slotOffset;
    CutsceneSlot *slot;
    CutsceneSlotData *slotData;

    i = 0;
    iterPtr = manager;
    byteOffset = 0;

    while (i < (getCutsceneSlotCount() & 0xFF)) {
        getCurrentStateEntryItem(i);
        model = ((CutsceneSlot *)((u8 *)iterPtr + 0xA8))->model;

        if (model != NULL) {
            slotOffset = byteOffset + 0xA8;
            slot = (CutsceneSlot *)((u8 *)manager + slotOffset);
            slotData = &slot->slotData;

            interpolateSlotScaleX(slotData, 0x10000, 0);
            interpolateSlotScaleY(slotData, 0x10000, 0);
            interpolateSlotScaleZ(slotData, 0x10000, 0);
            setupSlotTransform(slotData);
            applyTransformToModel(model, &slotData->unk04);
        }

        iterPtr = (CutsceneManager *)((u8 *)iterPtr + 0xF4);
        byteOffset += 0xF4;
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
        modelId = getCurrentStateEntryItem(i)->unk4;
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
        sprintf((char *)cutsceneManager->debugText, (const char *)D_800BAE00_1E7EB0, cutsceneManager->currentFrame);
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
            applyTransformToModel(currentModel, &slotData->unk04);
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
    return gCutsceneStateTable->unk12;
}

u8 getCutsceneSlotCount(void) {
    return gCutsceneStateTable->unk13;
}

u16 getCutsceneFrameMask(void) {
    return gCutsceneStateTable->unk14;
}

u16 getCutsceneMaxStateEntries(void) {
    return gCutsceneStateTable->unk16;
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

void initializeCutsceneSystem(void *arg0) {
    u8 *base;
    s32 i;
    s32 next;
    s32 j;
    u16 index;
    s32 neg1;
    u16 ffff;

    gCutsceneStateTableSize = 0x78E0;
    gCutsceneStateTable = allocateNodeMemory(0x78E0);
    i = 0;
    do {
        StateEntry *ptr;
        s32 shift;
        s32 prev;

        ptr = gCutsceneStateTable;
        shift = i << 6;
        next = i + 1;
        prev = i - 1;
        base = (u8 *)ptr + shift;
        *(s16 *)(base + 0xF8) = next;
        *(s16 *)(base + 0xFA) = prev;
        initializeStateEntry(i);
        i = next;
    } while (next < 0x1E0);

    {
        StateEntry *v1ptr = gCutsceneStateTable;
        StateEntry *a0ptr;
        v1ptr->scriptData[4] = 0x45;
        a0ptr = gCutsceneStateTable;
        *(u16 *)((u8 *)v1ptr + 0xFA) = 0xFFFF;
        *(u16 *)((u8 *)v1ptr + 0x78B8) = 0xFFFF;
        *(s32 *)v1ptr = 0;
        a0ptr->scriptData[5] = 0x44;
    }
    gCutsceneStateTable->scriptData[6] = 0x41;
    gCutsceneStateTable->scriptData[7] = 0x54;
    gCutsceneStateTable->scriptData[8] = 0x30;
    gCutsceneStateTable->scriptData[9] = 0x30;
    gCutsceneStateTable->scriptData[0xA] = 0x30;
    i = 0;
    gCutsceneStateTable->scriptData[0xB] = 0x31;
    {
        StateEntry *v1ptr2 = gCutsceneStateTable;
        StateEntry *a0ptr2;
        neg1 = -1;
        v1ptr2->unk12 = 0x38;
        a0ptr2 = gCutsceneStateTable;
        ffff = 0xFFFF;
        *(s16 *)((u8 *)v1ptr2 + 0xE) = 0x1DF;
        v1ptr2->current_index = 0;
        v1ptr2->allocatedEventCount = 0;
        a0ptr2->unk13 = 0x10;
    }
    {
        StateEntry *v1Reg = gCutsceneStateTable;
        j = 0;
        v1Reg->unk14 = 0x8000;
        v1Reg->unk16 = 0x1E0;
        v1Reg->initModelIndex = 0;
        v1Reg->defaultEndFrame = 0x64;
        v1Reg->configByte = 0;
    }
    do {
        u8 *itemBase;
        u8 *itemBase2;
        u8 *entryBase;
        s32 a1;
        s32 shift2;

        gCutsceneStateTable->allocatedEventCount += 1;
        index = allocateStateEntry();
        itemBase = (u8 *)gCutsceneStateTable;
        a1 = i << 24;
        itemBase += j;
        itemBase[0x26] = 0;
        itemBase2 = (u8 *)gCutsceneStateTable + j;
        i++;
        a1 >>= 24;
        *(s16 *)(itemBase + 0x20) = index;
        *(s16 *)(itemBase + 0x24) = neg1;
        itemBase2[0x27] = neg1;
        ((u8 *)gCutsceneStateTable + j)[0x28] = 0;
        shift2 = (index & 0xFFFF) << 6;
        ((u8 *)gCutsceneStateTable + j)[0x29] = neg1;
        {
            u8 *ptr = (u8 *)gCutsceneStateTable;
            s32 cmdOffset = shift2 + 0xC0;
            j += 0xA;
            entryBase = ptr + shift2;
            *(u16 *)(entryBase + 0xF8) = ffff;
            *(u16 *)(entryBase + 0xFA) = ffff;
            initializeCutsceneCommand(ptr + cmdOffset, arg0, 0, 0, a1);
        }
    } while (i < 0x10);

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
            if (targetFrame < (u16)entry->frameNumber) {
                break;
            }
            current = next;
            entry = getStateEntry(nextIdx);
            next = entry->next_index;
        } while (next != 0xFFFF);
    }

    if ((mode & 0xFF) == 0) {
        entry = getStateEntry(current);
        if ((u16)entry->frameNumber != frameNumber) {
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
        if (a1 < (u16)temp->frameNumber) {
            return 0xFFFF;
        }

        temp = getStateEntry(current);
        if ((u16)temp->frameNumber == a1) {
            return current;
        }

        temp = getStateEntry(current);
        current = temp->next_index;
    }

    return 0xFFFF;
}

s32 insertCutsceneEvent(u8 slotIndex, u16 frameNumber) {
    StateEntry *eventTable;
    u16 insertAfterIndex;
    u16 entryIndex;
    u16 allocatedIndex;
    u16 originalNextIndex;
    StateEntry *entry;
    StateEntry *entryPtr;

    if (gCutsceneStateTable->allocatedEventCount >= 0x1E0) {
        goto ret_ffff;
    }

    insertAfterIndex = findStateEntryIndex(slotIndex & 0xFF, frameNumber, 1);
    entryIndex = insertAfterIndex;

    if (entryIndex == 0xFFFF) {
        return 0xFFFF;
    }

    entry = getStateEntry(entryIndex);
    if ((u16)entry->frameNumber != frameNumber) {
        goto do_work;
    }

ret_ffff:
    return 0xFFFF;

do_work:
    allocatedIndex = allocateStateEntry();
    eventTable = gCutsceneStateTable;

    /* Read next_index from entry at entryIndex */
    originalNextIndex = *(u16 *)((u8 *)eventTable + (u32)(entryIndex << 6) + 0xF8);
    /* Set next_index of entry at entryIndex to newly allocated entry */
    *(u16 *)((u8 *)eventTable + (u32)(entryIndex << 6) + 0xF8) = allocatedIndex;

    if (originalNextIndex != 0xFFFF) {
        /* Set prev_index of original next entry to point to newly allocated entry */
        *(u16 *)((u8 *)eventTable + (u32)(originalNextIndex << 6) + 0xFA) = allocatedIndex;
    }

    entryPtr = gCutsceneStateTable;
    entryIndex = allocatedIndex;
    entryPtr = (StateEntry *)((u8 *)entryPtr + (u32)(entryIndex << 6));
    /* Set prev_index of new entry to point to insert-after entry */
    *(u16 *)((u8 *)entryPtr + 0xFA) = insertAfterIndex;
    /* Set next_index of new entry to point to original next entry */
    *(u16 *)((u8 *)entryPtr + 0xF8) = originalNextIndex;

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

void reorderCutsceneEvent(u16 eventIndex, u16 oldPreviousIndex, u16 newPreviousIndex) {
    u8 *entryPtr;
    u32 oldPreviousOffset;
    u16 newPreviousIndexMasked;
    u16 eventIndexMasked;
    u16 oldPreviousIndexMasked;
    u32 eventOffset;
    u16 originalNextIndex;
    u16 eventPreviousIndex;
    u8 *eventTable;
    u8 *tablePtr;
    u32 newPreviousOffset;

    eventIndexMasked = eventIndex;
    oldPreviousIndexMasked = oldPreviousIndex;
    newPreviousIndexMasked = newPreviousIndex & 0xFFFF;
    getStateEntry(newPreviousIndexMasked);

    eventOffset = eventIndexMasked & 0xFFFF;
    getStateEntry(eventOffset);

    oldPreviousOffset = oldPreviousIndexMasked & 0xFFFF;
    getStateEntry(oldPreviousOffset);

    eventTable = (u8 *)gCutsceneStateTable;

    oldPreviousOffset = oldPreviousOffset << 6;
    eventOffset = eventOffset << 6;

    /* Read next_index from old prev entry and prev_index from entry being moved */
    originalNextIndex = *(u16 *)(eventTable + oldPreviousOffset + 0xF8);
    entryPtr = eventTable + eventOffset;
    eventPreviousIndex = *(u16 *)(entryPtr + 0xFA);

    /* Unlink entry from current position: update next entry's prev_index */
    if ((originalNextIndex & 0xFFFF) != 0xFFFF) {
        *(u16 *)(eventTable + ((originalNextIndex & 0xFFFF) << 6) + 0xFA) = eventPreviousIndex;
    }

    /* Unlink entry from current position: update prev entry's next_index */
    if ((eventPreviousIndex & 0xFFFF) != 0xFFFF) {
        u8 *temp = (u8 *)gCutsceneStateTable;
        *(u16 *)(temp + ((eventPreviousIndex & 0xFFFF) << 6) + 0xF8) = originalNextIndex;
    }

    /* Insert entry after newPreviousIndex */
    tablePtr = (u8 *)gCutsceneStateTable;
    newPreviousOffset = newPreviousIndexMasked << 6;
    originalNextIndex = *(u16 *)(tablePtr + newPreviousOffset + 0xF8);
    *(u16 *)(tablePtr + newPreviousOffset + 0xF8) = eventIndexMasked;
    *(u16 *)(tablePtr + oldPreviousOffset + 0xF8) = originalNextIndex;

    /* Update the next entry's prev_index to point to the old prev */
    if ((originalNextIndex & 0xFFFF) != 0xFFFF) {
        *(u16 *)(tablePtr + ((originalNextIndex & 0xFFFF) << 6) + 0xFA) = oldPreviousIndexMasked;
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

    count = gCutsceneStateTable->unk13;
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
        if (i < gCutsceneStateTable->unk13) {
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

    searchResult = findStateEntryIndex(slotIndex, frameNumber & 0xFFFF, 0);
    entryIndex = searchResult & 0xFFFF;

    if (entryIndex == 0xFFFF) {
        return;
    }

    if ((u16)getStateEntry(entryIndex)->frameNumber == 0) {
        return;
    }

    memcpy(gCutsceneEntryBuffer, getStateEntry(entryIndex), 0x40);

    base = gCutsceneStateTable;
    freeListHead = *(u16 *)((u8 *)base + 0xE);
    entryNextIndex = *(u16 *)((u8 *)base + (entryIndex << 6) + 0xF8);
    entryPrevIndex = *(u16 *)((u8 *)base + (entryIndex << 6) + 0xFA);

    *(u16 *)((u8 *)base + (freeListHead << 6) + 0xF8) = searchResult;
    *(u16 *)((u8 *)base + 0xE) = searchResult;

    if ((entryPrevIndex & 0xFFFF) != 0xFFFF) {
        *(u16 *)((u8 *)base + (entryPrevIndex << 6) + 0xF8) = entryNextIndex;
    }

    if ((entryNextIndex & 0xFFFF) != 0xFFFF) {
        *(u16 *)((u8 *)gCutsceneStateTable + (entryNextIndex << 6) + 0xFA) = entryPrevIndex;
    }

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
    categorySkip = (-((~getCurrentStateEntryItem(slotIndex)->unk4) != 0)) | 1;

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
                func_80069CF8_6A8F8();
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
