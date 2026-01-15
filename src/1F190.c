#include "202A0.h"
#include "288A0.h"
#include "36B80.h"
#include "56910.h"
#include "6E840.h"
#include "D_800AFE8C_A71FC_type.h"
#include "EepromSaveData_type.h"
#include "common.h"
#include "geometry.h"
#include "graphics.h"
#include "race_session.h"
#include "rom_loader.h"
#include "task_scheduler.h"

USE_ASSET(_458E30);
USE_ASSET(_43A000);
USE_ASSET(_43F050);
USE_ASSET(_459310);
USE_ASSET(_4488E0);

typedef struct {
    Node_70B00 unk0;        // 0x000
    Node_70B00 unk1D8;      // 0x1D8
    Node_70B00 previewNode; // 0x3B0
    Node_70B00 unk588;      // 0x588
    Node_70B00 detailNode;  // 0x760
    Node_70B00 unk938;      // 0x938
    void *unkB10;           // 0xB10
    void *unkB14;           // 0xB14
    void *unkB18;           // 0xB18
    void *unkB1C;           // 0xB1C
    void *unkB20;           // 0xB20
    u8 padB24[0x4];         // 0xB24
    u16 transitionCounter;  // 0xB28
    u8 padB2A[0x2];         // 0xB2A
    s8 selectedIndex;       // 0xB2C
    u8 exitMode;            // 0xB2D
    u8 previewLoadCounter;  // 0xB2E
    u8 padB2F[0x4];         // 0xB2F
    u8 levelIdList[12];     // 0xB33
    u8 padB3F[0x5];         // 0xB3F
    u8 isLoadingPreview;    // 0xB44
    u8 showDetailView;      // 0xB45
    u8 selectedNumber;      // 0xB46
} LevelSelectState_Base;

// typedef struct {
//     Node_70B00 unk0;
//     Node_70B00 unk1D8;
//     Node_70B00 previewNode;
//     Node_70B00 unk588;
//     Node_70B00 detailNode;
//     Node_70B00 unk938;
//     void *unkB10;
//     void *unkB14;
//     void *unkB18;
//     void *unkB1C;
//     void *unkB20;
//     s32 loadStartFrame;
//     u16 transitionCounter;
//     u8 padB2A[0x2];
//     s8 selectedIndex;
//     u8 exitMode;
//     u8 previewLoadCounter;
//     u8 menuState;
//     u8 newLevelId;
//     u8 oldLevelId;
//     u8 scrollDirection;
//     u8 levelIdList[16];
//     u8 menuItemCount;
//     u8 isLoadingPreview;
//     u8 showDetailView;
//     u8 selectedNumber;
//     u8 maxLevelCount;
// } LevelSelectState;

typedef struct {
    /* 0x000 */ Node_70B00 unk0;
    /* 0x1D8 */ Node_70B00 unk1D8;
    /* 0x3B0 */ Node_70B00 previewNode;
    /* 0x588 */ Node_70B00 unk588;
    /* 0x760 */ Node_70B00 detailNode;
    /* 0x938 */ Node_70B00 unk938;
    /* 0xB10 */ void *unkB10;
    /* 0xB14 */ void *unkB14;
    /* 0xB18 */ void *unkB18;
    /* 0xB1C */ void *unkB1C;
    /* 0xB20 */ void *unkB20;
    /* 0xB24 */ s32 loadStartFrame;
    /* 0xB28 */ u16 transitionCounter;
    /* 0xB2A */ u16 padB2A;
    /* 0xB2C */ s8 selectedIndex;
    /* 0xB2D */ u8 exitMode;
    /* 0xB2E */ u8 previewLoadCounter;
    /* 0xB2F */ u8 menuState;
    /* 0xB30 */ u8 unkB30;
    /* 0xB31 */ u8 newLevelId;
    /* 0xB32 */ u8 oldLevelId;
    /* 0xB33 */ u8 levelIdList[12];
    /* 0xB3F */ u8 menuItemCount;
    /* 0xB40 */ u8 padB40[0x3];
    /* 0xB43 */ u8 maxLevelCount;
    /* 0xB44 */ u8 isLoadingPreview;
    /* 0xB45 */ u8 showDetailView;
    /* 0xB46 */ u8 selectedNumber;
    /* 0xB47 */ u8 detailViewMode;
} LevelSelectState;

typedef enum {
    MENU_STATE_NAVIGATE = 0,      // Main selection, up/down navigation
    MENU_STATE_SCROLL = 1,        // Not in switch - probably animation transition
    MENU_STATE_CONFIRM = 2,       // Confirm selection, B backs out
    MENU_STATE_NUMBER_SELECT = 3, // Adjust numeric value 1-9
    MENU_STATE_UKNOWN = 4,
    MENU_STATE_PROMPT = 5,       // Awaiting input before detail view
    MENU_STATE_DETAIL_OPEN = 6,  // Opening detail view animation
    MENU_STATE_DETAIL_WAIT = 7,  // Waiting for open animation to finish
    MENU_STATE_DETAIL = 8,       // In detail view, can confirm or back out
    MENU_STATE_DETAIL_CLOSE = 9, // Closing detail view animation
} MenuState;

void func_8001E5EC_1F1EC(void);
void handleLevelSelectInput(void);
void waitForFadeToLevelSelect(void);
void proceedFromLevelSelect(void);
void cancelLevelSelect(void);
void applyLevelSelection(void);
void cleanupLevelSelect(void);
void loadLevelPreview(void);

extern s32 buildUnlockedLevelList(u8 *);

extern s32 gControllerInputs[];
extern s32 gFrameCounter;
extern u8 D_8008D9C0_8E5C0[];
extern u8 identityMatrix[];

void initLevelSelectWithDetail(void) {
    LevelSelectState_Base *allocation = allocateTaskMemory(0xB48);
    allocation->showDetailView = 1;
    setGameStateHandler(func_8001E5EC_1F1EC);
}

void initLevelSelectBasic(void) {
    LevelSelectState_Base *allocation = allocateTaskMemory(0xB48);
    allocation->showDetailView = 0;
    setGameStateHandler(func_8001E5EC_1F1EC);
}

void func_8001E5EC_1F1EC(void) {
    Transform3D sp20;
    Transform3D *sp20Ptr;
    s32 i;
    LevelSelectState *alloc;
    u8 unlockedLevelCount;
    s8 saveSlot;

    alloc = getCurrentAllocation();
    sp20Ptr = &sp20;

    playMusicTrack(3);
    setupTaskSchedulerNodes(0x44, 0x14, 0, 0, 0, 0, 0, 0);
    memcpy(sp20Ptr, identityMatrix, sizeof(Transform3D));

    alloc->transitionCounter = 0;
    alloc->exitMode = 0;
    alloc->selectedIndex = 0;
    alloc->previewLoadCounter = 0;
    alloc->oldLevelId = 0;
    alloc->padB2A = 0;

    alloc->menuState = 0;
    alloc->newLevelId = 0;
    alloc->unkB30 = 0;
    alloc->isLoadingPreview = 0;
    alloc->loadStartFrame = 0;

    alloc->unkB10 = loadCompressedData(&_43A000_ROM_START, &_43A000_ROM_END, 0xB198);
    alloc->unkB14 = loadCompressedData(&_43F050_ROM_START, &_43F050_ROM_END, 0x14010);
    alloc->unkB18 = loadCompressedData(&_458E30_ROM_START, &_458E30_ROM_END, 0xAE0);
    alloc->unkB1C = loadTextRenderAsset(1);
    alloc->unkB20 = loadAsset_34F7E0();

    initMenuCameraNode(&alloc->unk0, 9, 0x14, 0);
    setModelCameraTransform(&alloc->unk0, 0, -8, -0x98, -0x70, 0x97, 0x6F);
    func_8006BEDC_6CADC(sp20Ptr, 0, 0, 0x800000, 0, 0, 0);
    setViewportTransformById(alloc->unk0.id, sp20Ptr);

    initMenuCameraNode(&alloc->unk1D8, 8, 0xF, 1);
    setModelCameraTransform(&alloc->unk1D8, 0, -8, -0x98, -0x70, 0x97, 0x6F);

    initMenuCameraNode(&alloc->unk938, 0xA, 3, 1);

    initViewportNode(&alloc->unk588, &alloc->unk0, 4, 5, 1);

    initViewportNode(&alloc->previewNode, &alloc->unk588, 0, 0xA, 1);
    func_8006FEF8_70AF8(&alloc->unk588, 0x65);
    func_8006FEF8_70AF8(&alloc->previewNode, 0x64);

    setModelCameraTransform(&alloc->previewNode, 0, 0, -0x5C, -0x22, 0x5C, 0x58);
    setModelCameraTransform(&alloc->unk588, 0, 0, -0x5C, -0x22, 0x5C, 0x58);

    func_8006FA0C_7060C(&alloc->previewNode, 70.0f, 1.4375f, 20.0f, 4000.0f);
    func_8006FA0C_7060C(&alloc->unk588, 70.0f, 1.4375f, 20.0f, 10000.0f);

    setViewportFadeValue(0, 0xFF, 0);
    setViewportFadeValue(&alloc->unk588, 0, 0);
    setViewportFadeValue(&alloc->previewNode, 0xFF, 0);

    scheduleTask(func_80020B44_21744, 1, 0, 0x5A);
    scheduleTask(initUnlockNotification, 1, 0, 0x5A);
    scheduleTask(initLevelPreviewPortraits, 1, 0, 0x5B);
    scheduleTask(initMenuCharacterModel, 1, 0, 0x5B);
    scheduleTask(initConfirmationIndicator, 1, 0, 0x5A);

    if (D_800AFE8C_A71FC->customLapEnabled != 0 && D_800AFE8C_A71FC->unk4 == 1) {
        scheduleTask(initPrizeDisplay, 1, 0, 0x5A);
    }

    unlockedLevelCount = buildUnlockedLevelList(alloc->levelIdList);
    alloc->maxLevelCount = unlockedLevelCount;

    for (i = 0; i < unlockedLevelCount & 0xFF; i++) {
        if (D_800AFE8C_A71FC->saveSlotIndex == alloc->levelIdList[i]) {
            break;
            do { } while (0); }
    }
    alloc->selectedIndex = i;

    saveSlot = D_800AFE8C_A71FC->saveSlotIndex;
    alloc->unkB30 = saveSlot;
    alloc->newLevelId = saveSlot;

    alloc->selectedNumber = D_800AFE8C_A71FC->unk9[0x10];

    if (alloc->showDetailView != 0) {
        alloc->detailViewMode = 0;
        alloc->menuState = 5;
        saveSlot = D_800AFE8C_A71FC->saveSlotIndex;
        alloc->selectedIndex = saveSlot;
        alloc->levelIdList[saveSlot] = D_800AFE8C_A71FC->saveSlotIndex;
        scheduleTask(initMenuBackgroundEffect, 1, 0, 0x64);
        scheduleTask(initCharacterDescriptionText, 1, 0, 0x64);
    }

    setGameStateHandler(initLevelSelectTransition);
}

void initLevelSelectTransition(void) {
    LevelSelectState_Base *allocation = (LevelSelectState_Base *)getCurrentAllocation();

    allocation->transitionCounter++;
    if (allocation->transitionCounter < 3) {
        return;
    }

    allocation->transitionCounter = 2;
    if (getPendingDmaCount() != 0) {
        return;
    }

    allocation->transitionCounter = 0;
    setViewportFadeValue(0, 0, 0xE);
    setGameStateHandler(waitForFadeToLevelSelect);
}

void waitForFadeToLevelSelect(void) {
    if (getViewportFadeMode(0) == 0) {
        setGameStateHandler(handleLevelSelectInput);
    }
}

void handleLevelSelectInput(void) {
    LevelSelectState *allocation;
    D_800AFE8C_A71FC_type *tempA0;
    s32 i;
    u8 oldMenuSelection;
    u8 oldB46;

    allocation = (LevelSelectState *)getCurrentAllocation();

    switch (allocation->menuState) {
        case MENU_STATE_NAVIGATE:
            oldMenuSelection = allocation->selectedIndex;
            oldB46 = oldMenuSelection & 0xFF;
            for (i = 0; i < D_800AFE8C_A71FC->numPlayers; i++) {
                if (oldB46 == (allocation->selectedIndex)) {
                    if (gControllerInputs[i] & 0x10800) {
                        allocation->selectedIndex = allocation->selectedIndex - 1;
                        allocation->oldLevelId = 0;
                        if (allocation->selectedIndex < 0) {
                            allocation->selectedIndex = allocation->maxLevelCount - 1;
                        }
                    } else if (gControllerInputs[i] & 0x20400) {
                        allocation->selectedIndex = allocation->selectedIndex + 1;
                        allocation->oldLevelId = 1;
                        if (allocation->selectedIndex > (s32)(allocation->maxLevelCount - 1)) {
                            allocation->selectedIndex = 0;
                        }
                    }
                }
            }

            if (oldB46 != (allocation->selectedIndex)) {
                playSoundEffect(0x2B);
                allocation->menuState = MENU_STATE_SCROLL;
                allocation->transitionCounter = 0;
                allocation->newLevelId = allocation->levelIdList[oldB46];
                allocation->unkB30 = allocation->levelIdList[allocation->selectedIndex];
                if (allocation->previewLoadCounter >= 3) {
                    allocation->previewLoadCounter = 0;
                    terminateTasksByType(0);
                    allocation->isLoadingPreview = 1;
                    allocation->loadStartFrame = gFrameCounter;
                    setViewportFadeValue(&allocation->unk588, 0xFF, 0);
                    setViewportFadeValue(&allocation->previewNode, 0xFF, 0);
                } else {
                    allocation->previewLoadCounter = 0;
                }
            } else {
                loadLevelPreview();
                for (i = 0; i < D_800AFE8C_A71FC->numPlayers; i++) {
                    tempA0 = D_800AFE8C_A71FC;
                    if (allocation->menuState == MENU_STATE_NAVIGATE && allocation->exitMode != 2) {
                        if (gControllerInputs[i] & (A_BUTTON | START_BUTTON)) {
                            if (tempA0->customLapEnabled != 0) {
                                if (tempA0->unk4 == 1) {
                                    allocation->menuState = 3;
                                    allocation->selectedNumber = D_800AFE8C_A71FC->unk9[0x10];
                                } else {
                                    allocation->menuState = 2;
                                }
                            } else {
                                allocation->menuState = 2;
                            }
                        } else if (gControllerInputs[i] & B_BUTTON) {
                            playSoundEffect(0x2E);
                            allocation->exitMode = 2;
                        }
                    }
                }
            }
            break;

        case MENU_STATE_CONFIRM:
            loadLevelPreview();
            for (i = 0; i < D_800AFE8C_A71FC->numPlayers; i++) {
                if (allocation->menuState == MENU_STATE_CONFIRM && allocation->exitMode != 1) {
                    if (gControllerInputs[i] & B_BUTTON) {
                        playSoundEffect(0x2E);
                        allocation->menuState = 0;
                    } else if (gControllerInputs[i] & (A_BUTTON | START_BUTTON)) {
                        allocation->exitMode = 1;
                        D_800AFE8C_A71FC->saveSlotIndex = allocation->levelIdList[allocation->selectedIndex];
                        if (D_800AFE8C_A71FC->unk4 == 0) {
                            D_800AFE8C_A71FC->unk9[0xC] = D_8008D9C0_8E5C0[D_800AFE8C_A71FC->saveSlotIndex];
                        }
                        applyLevelSelection();
                        playSoundEffectOnChannelNoPriority(0x2D, 0);
                    }
                }
            }
            break;

        case MENU_STATE_PROMPT:
            loadLevelPreview();
            if (gControllerInputs[0] & (A_BUTTON | START_BUTTON)) {
                playSoundEffect(0x2C);
                setViewportFadeValue(&allocation->previewNode, 0xFF, 8);
                allocation->menuState = 6;
            } else if (gControllerInputs[0] & B_BUTTON) {
                playSoundEffect(0x2E);
                allocation->exitMode = 2;
            }
            break;

        case MENU_STATE_DETAIL_OPEN:
            if (getViewportFadeMode(&allocation->previewNode) == 0) {
                Node_70B00 *node760;

                terminateTasksByType(0);
                node760 = &allocation->detailNode;
                allocation->isLoadingPreview = 1;
                allocation->loadStartFrame = gFrameCounter;
                allocation->menuState = 7;
                initViewportNode(node760, &allocation->unk0, 0xB, 0x12, 0);
                func_8006FEF8_70AF8(node760, 0xC);
                setModelCameraTransform(node760, 0, 0, -0x5C, -0x22, 0x5C, 0x58);
                setViewportFadeValue(node760, 0xFF, 0);
                setViewportFadeValue(node760, 0, 8);
            }
            break;

        case MENU_STATE_DETAIL_WAIT:
            if (getViewportFadeMode(&allocation->detailNode) == 0) {
                allocation->menuState = 8;
                allocation->detailViewMode = 1;
            }
            break;

        case MENU_STATE_DETAIL_CLOSE: {
            Node_70B00 *node760 = &allocation->detailNode;
            if (getViewportFadeMode(node760) == 0) {
                unlinkNode(node760);
                allocation->menuState = 5;
                allocation->detailViewMode = 2;
            }
        } break;

        case MENU_STATE_DETAIL:
            if (gControllerInputs[0] & B_BUTTON) {
                playSoundEffect(0x2E);
                setViewportFadeValue(&allocation->detailNode, 0xFF, 8);
                allocation->menuState = 9;
                allocation->previewLoadCounter = 0;
            } else if (gControllerInputs[0] & (A_BUTTON | START_BUTTON)) {
                allocation->exitMode = 1;
                D_800AFE8C_A71FC->unk9[0xC] = D_8008D9C0_8E5C0[D_800AFE8C_A71FC->saveSlotIndex];
                playSoundEffect(0x2D);
            }
            break;

        case MENU_STATE_NUMBER_SELECT:
            loadLevelPreview();
            oldB46 = allocation->selectedNumber;
            for (i = 0; i < D_800AFE8C_A71FC->numPlayers; i++) {
                if ((oldB46 & 0xFF) == allocation->selectedNumber) {
                    if (gControllerInputs[i] & 0x40100) {
                        allocation->selectedNumber = allocation->selectedNumber + 1;
                        if ((allocation->selectedNumber & 0xFF) >= 10) {
                            allocation->selectedNumber = 1;
                        }
                    } else if (gControllerInputs[i] & 0x80200) {
                        allocation->selectedNumber = allocation->selectedNumber - 1;
                        if ((allocation->selectedNumber & 0xFF) == 0) {
                            allocation->selectedNumber = 9;
                        }
                    }
                }
            }
            if (oldB46 != allocation->selectedNumber) {
                playSoundEffect(0x2B);
            } else {
                for (i = 0; i < D_800AFE8C_A71FC->numPlayers; i++) {
                    if (allocation->menuState == MENU_STATE_NUMBER_SELECT) {
                        if (gControllerInputs[i] & (A_BUTTON | START_BUTTON)) {
                            allocation->menuState = 2;
                        } else if (gControllerInputs[i] & B_BUTTON) {
                            playSoundEffect(0x2E);
                            allocation->menuState = 0;
                        }
                    }
                }
            }
            break;
    }

    if (allocation->isLoadingPreview != 0) {
        if (getFreeNodeCount(0) == 0x44) {
            s32 frameCheck = gFrameCounter - 5;
            if ((u32)((allocation->loadStartFrame - frameCheck) & 0x0FFFFFFF) > 0x07FFFFFF) {
                allocation->isLoadingPreview = 0;
            }
        }
    }

    if (allocation->exitMode != 0) {
        setMusicFadeOut(0xA);
        if (allocation->exitMode == 2) {
            setViewportFadeValue(NULL, 0xFF, 8);
        } else {
            setViewportFadeValue(NULL, 0xFF, 0x10);
        }
        setGameStateHandler(cleanupLevelSelect);
    }
}

void cleanupLevelSelect(void) {
    LevelSelectState_Base *allocation = (LevelSelectState_Base *)getCurrentAllocation();

    if (getViewportFadeMode(NULL) != 0) {
        return;
    }

    unlinkNode(&allocation->unk0);
    unlinkNode(&allocation->previewNode);
    unlinkNode(&allocation->unk588);
    unlinkNode(&allocation->unk1D8);
    unlinkNode(&allocation->unk938);

    allocation->unkB10 = freeNodeMemory(allocation->unkB10);
    allocation->unkB14 = freeNodeMemory(allocation->unkB14);
    allocation->unkB18 = freeNodeMemory(allocation->unkB18);
    allocation->unkB1C = freeNodeMemory(allocation->unkB1C);
    allocation->unkB20 = freeNodeMemory(allocation->unkB20);

    if (allocation->exitMode == 1) {
        terminateSchedulerWithCallback(proceedFromLevelSelect);
        if (allocation->showDetailView != 0) {
            unlinkNode(&allocation->detailNode);
        }
    } else {
        terminateSchedulerWithCallback(cancelLevelSelect);
    }
}

void proceedFromLevelSelect(void) {
    func_800697F4_6A3F4(1);
}

void cancelLevelSelect(void) {
    func_800697F4_6A3F4(0xFF);
}

void applyLevelSelection(void) {
    LevelSelectState_Base *allocation;
    D_800AFE8C_A71FC_type *ptr;
    u8 unk4;
    u8 saveSlotIndex;

    allocation = getCurrentAllocation();
    ptr = D_800AFE8C_A71FC;
    unk4 = ptr->unk4;
    saveSlotIndex = ptr->saveSlotIndex;

    if (unk4 == 0) {
        if (saveSlotIndex == 3 || saveSlotIndex == 7 || saveSlotIndex == 11 || saveSlotIndex >= 12) {
            ptr->unk9[0x10] = 1;
            D_800AFE8C_A71FC->numPlayers = 1;
        } else {
            ptr->unk9[0x10] = 3;
            D_800AFE8C_A71FC->numPlayers = 1;
        }
    } else {
        ptr->unk9[0x10] = allocation->selectedNumber;
        D_800AFE8C_A71FC->customLapCount = allocation->selectedNumber;
    }
}

#define ASPECT_RATIO (4.0f / 3.0f)

void loadLevelPreview(void) {
    LevelSelectState_Base *allocation;
    D_80090F90_91B90_item *item;
    u8 counter;
    u8 poolIdRaw;
    u8 poolId;
    Node_70B00 *node;

    allocation = (LevelSelectState_Base *)getCurrentAllocation();

    if (allocation->isLoadingPreview != 0) {
        return;
    }

    counter = allocation->previewLoadCounter;
    if (counter < 3) {
        allocation->previewLoadCounter = counter + 1;
        if ((u8)(counter + 1) != 3) {
            return;
        }

        poolIdRaw = allocation->levelIdList[allocation->selectedIndex];
        poolId = poolIdRaw;
        item = func_80055D10_56910(poolId);

        setViewportLightColors(100, 1, &item->unk18, &item->unk20);

        if (poolId == 11) {
            node = &allocation->previewNode;
            func_8006FA0C_7060C(node, 50.0f, ASPECT_RATIO, 10.0f, 1000.0f);
            setViewportFogById(100, 900, 999, item->unk20.r2, item->unk20.g2, item->unk20.b2);
            spawnConfettiEffect(node);
        } else {
            func_8006FA0C_7060C(&allocation->previewNode, 50.0f, ASPECT_RATIO, 10.0f, 4000.0f);
            setViewportFogById(100, 995, 999, item->unk20.r2, item->unk20.g2, item->unk20.b2);
        }

        scheduleTask(&initLevelPreviewCharacter, 0, 0, 0x5B);
        scheduleLevelEnvironmentTasks(poolIdRaw);
        allocation->previewLoadCounter = allocation->previewLoadCounter + 1;
    } else if (counter < 6) {
        allocation->previewLoadCounter = counter + 1;
    } else {
        if (getPendingDmaCount() == 0) {
            setViewportFadeValue(&allocation->unk588, 0, 0);
            setViewportFadeValue(&allocation->previewNode, 0, 0x14);
        }
    }
}

s32 buildUnlockedLevelList(u8 *levelIdList) {
    s32 count;
    s32 maxSlots;
    s32 i;

    count = 0;
    maxSlots = D_800AFE8C_A71FC->unk4 ? 15 : 12;

    for (i = 0; i < maxSlots; i++) {
        if (D_800AFE8C_A71FC->unk4 == 0) {
            do {
                if (EepromSaveData->save_slot_status[i] != 0) {
                    levelIdList[(u8)(count++)] = i;
                } else {
                    levelIdList[i] = 0;
                }
            } while (0);
        } else if (EepromSaveData->save_slot_data[i] != 0) {
            levelIdList[(u8)(count++)] = i;
        } else {
            levelIdList[i] = 0;
        }
    }

    i = (u8)count;
    return i;
}