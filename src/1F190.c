#include "202A0.h"
#include "3A1F0.h"
#include "3E160.h"
#include "56910.h"
#include "6E840.h"
#include "D_800AFE8C_A71FC_type.h"
#include "common.h"
#include "graphics.h"
#include "task_scheduler.h"

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

typedef struct {
    Node_70B00 unk0;
    Node_70B00 unk1D8;
    Node_70B00 previewNode;
    Node_70B00 unk588;
    Node_70B00 detailNode;
    Node_70B00 unk938;
    void *unkB10;
    void *unkB14;
    void *unkB18;
    void *unkB1C;
    void *unkB20;
    s32 loadStartFrame;
    u16 transitionCounter;
    u8 padB2A[0x2];
    s8 selectedIndex;
    u8 exitMode;
    u8 previewLoadCounter;
    u8 menuState;
    u8 newLevelId;
    u8 oldLevelId;
    u8 scrollDirection;
    u8 levelIdList[16];
    u8 menuItemCount;
    u8 isLoadingPreview;
    u8 showDetailView;
    u8 selectedNumber;
    u8 detailViewState;
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
void func_8001F33C_1FF3C(void);
void func_8001F358_1FF58(void);
void applyLevelSelection(void);
void cleanupLevelSelect(void);
void loadLevelPreview(void);

extern s32 gControllerInputs[];
extern s32 gFrameCounter;
extern u8 D_8008D9C0_8E5C0[];

void func_8001E590_1F190(void) {
    LevelSelectState_Base *allocation = allocateTaskMemory(0xB48);
    allocation->showDetailView = 1;
    setGameStateHandler(func_8001E5EC_1F1EC);
}

void func_8001E5C0_1F1C0(void) {
    LevelSelectState_Base *allocation = allocateTaskMemory(0xB48);
    allocation->showDetailView = 0;
    setGameStateHandler(func_8001E5EC_1F1EC);
}

INCLUDE_ASM("asm/nonmatchings/1F190", func_8001E5EC_1F1EC);

void initLevelSelectTransition(void) {
    LevelSelectState_Base *allocation = (LevelSelectState_Base *)getCurrentAllocation();

    allocation->transitionCounter++;
    if (allocation->transitionCounter < 3) {
        return;
    }

    allocation->transitionCounter = 2;
    if (func_8003BB5C_3C75C() != 0) {
        return;
    }

    allocation->transitionCounter = 0;
    func_8006FDA0_709A0(0, 0, 0xE);
    setGameStateHandler(waitForFadeToLevelSelect);
}

void waitForFadeToLevelSelect(void) {
    if (func_8006FE10_70A10(0) == 0) {
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
            for (i = 0; i < D_800AFE8C_A71FC->unk8; i++) {
                if (oldB46 == (allocation->selectedIndex)) {
                    if (gControllerInputs[i] & 0x10800) {
                        allocation->selectedIndex = allocation->selectedIndex - 1;
                        allocation->scrollDirection = 0;
                        if (allocation->selectedIndex < 0) {
                            allocation->selectedIndex = allocation->menuItemCount - 1;
                        }
                    } else if (gControllerInputs[i] & 0x20400) {
                        allocation->selectedIndex = allocation->selectedIndex + 1;
                        allocation->scrollDirection = 1;
                        if (allocation->selectedIndex > (s32)(allocation->menuItemCount - 1)) {
                            allocation->selectedIndex = 0;
                        }
                    }
                }
            }

            if (oldB46 != (allocation->selectedIndex)) {
                func_800585C8_591C8(0x2B);
                allocation->menuState = MENU_STATE_SCROLL;
                allocation->transitionCounter = 0;
                allocation->oldLevelId = allocation->levelIdList[oldB46];
                allocation->newLevelId = allocation->levelIdList[allocation->selectedIndex];
                if (allocation->previewLoadCounter >= 3) {
                    allocation->previewLoadCounter = 0;
                    terminateTasksByType(0);
                    allocation->isLoadingPreview = 1;
                    allocation->loadStartFrame = gFrameCounter;
                    func_8006FDA0_709A0(&allocation->unk588, 0xFF, 0);
                    func_8006FDA0_709A0(&allocation->previewNode, 0xFF, 0);
                } else {
                    allocation->previewLoadCounter = 0;
                }
            } else {
                loadLevelPreview();
                for (i = 0; i < D_800AFE8C_A71FC->unk8; i++) {
                    tempA0 = D_800AFE8C_A71FC;
                    if (allocation->menuState == MENU_STATE_NAVIGATE && allocation->exitMode != 2) {
                        if (gControllerInputs[i] & (A_BUTTON | START_BUTTON)) {
                            if (tempA0->unk21 != 0) {
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
                            func_800585C8_591C8(0x2E);
                            allocation->exitMode = 2;
                        }
                    }
                }
            }
            break;

        case MENU_STATE_CONFIRM:
            loadLevelPreview();
            for (i = 0; i < D_800AFE8C_A71FC->unk8; i++) {
                if (allocation->menuState == MENU_STATE_CONFIRM && allocation->exitMode != 1) {
                    if (gControllerInputs[i] & B_BUTTON) {
                        func_800585C8_591C8(0x2E);
                        allocation->menuState = 0;
                    } else if (gControllerInputs[i] & (A_BUTTON | START_BUTTON)) {
                        allocation->exitMode = 1;
                        D_800AFE8C_A71FC->saveSlotIndex = allocation->levelIdList[allocation->selectedIndex];
                        if (D_800AFE8C_A71FC->unk4 == 0) {
                            D_800AFE8C_A71FC->unk9[0xC] = D_8008D9C0_8E5C0[D_800AFE8C_A71FC->saveSlotIndex];
                        }
                        applyLevelSelection();
                        func_80058220_58E20(0x2D, 0);
                    }
                }
            }
            break;

        case MENU_STATE_PROMPT:
            loadLevelPreview();
            if (gControllerInputs[0] & (A_BUTTON | START_BUTTON)) {
                func_800585C8_591C8(0x2C);
                func_8006FDA0_709A0(&allocation->previewNode, 0xFF, 8);
                allocation->menuState = 6;
            } else if (gControllerInputs[0] & B_BUTTON) {
                func_800585C8_591C8(0x2E);
                allocation->exitMode = 2;
            }
            break;

        case MENU_STATE_DETAIL_OPEN:
            if (func_8006FE10_70A10(&allocation->previewNode) == 0) {
                Node_70B00 *node760;

                terminateTasksByType(0);
                node760 = &allocation->detailNode;
                allocation->isLoadingPreview = 1;
                allocation->loadStartFrame = gFrameCounter;
                allocation->menuState = 7;
                func_8006FAA4_706A4(node760, &allocation->unk0, 0xB, 0x12, 0);
                func_8006FEF8_70AF8(node760, 0xC);
                setModelCameraTransform(node760, 0, 0, -0x5C, -0x22, 0x5C, 0x58);
                func_8006FDA0_709A0(node760, 0xFF, 0);
                func_8006FDA0_709A0(node760, 0, 8);
            }
            break;

        case MENU_STATE_DETAIL_WAIT:
            if (func_8006FE10_70A10(&allocation->detailNode) == 0) {
                allocation->menuState = 8;
                allocation->detailViewState = 1;
            }
            break;

        case MENU_STATE_DETAIL_CLOSE: {
            Node_70B00 *node760 = &allocation->detailNode;
            if (func_8006FE10_70A10(node760) == 0) {
                unlinkNode(node760);
                allocation->menuState = 5;
                allocation->detailViewState = 2;
            }
        } break;

        case MENU_STATE_DETAIL:
            if (gControllerInputs[0] & B_BUTTON) {
                func_800585C8_591C8(0x2E);
                func_8006FDA0_709A0(&allocation->detailNode, 0xFF, 8);
                allocation->menuState = 9;
                allocation->previewLoadCounter = 0;
            } else if (gControllerInputs[0] & (A_BUTTON | START_BUTTON)) {
                allocation->exitMode = 1;
                D_800AFE8C_A71FC->unk9[0xC] = D_8008D9C0_8E5C0[D_800AFE8C_A71FC->saveSlotIndex];
                func_800585C8_591C8(0x2D);
            }
            break;

        case MENU_STATE_NUMBER_SELECT:
            loadLevelPreview();
            oldB46 = allocation->selectedNumber;
            for (i = 0; i < D_800AFE8C_A71FC->unk8; i++) {
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
                func_800585C8_591C8(0x2B);
            } else {
                for (i = 0; i < D_800AFE8C_A71FC->unk8; i++) {
                    if (allocation->menuState == MENU_STATE_NUMBER_SELECT) {
                        if (gControllerInputs[i] & (A_BUTTON | START_BUTTON)) {
                            allocation->menuState = 2;
                        } else if (gControllerInputs[i] & B_BUTTON) {
                            func_800585C8_591C8(0x2E);
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
        func_80057564_58164(0xA);
        if (allocation->exitMode == 2) {
            func_8006FDA0_709A0(NULL, 0xFF, 8);
        } else {
            func_8006FDA0_709A0(NULL, 0xFF, 0x10);
        }
        setGameStateHandler(cleanupLevelSelect);
    }
}

void cleanupLevelSelect(void) {
    LevelSelectState_Base *allocation = (LevelSelectState_Base *)getCurrentAllocation();

    if (func_8006FE10_70A10(NULL) != 0) {
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
        terminateSchedulerWithCallback(func_8001F33C_1FF3C);
        if (allocation->showDetailView != 0) {
            unlinkNode(&allocation->detailNode);
        }
    } else {
        terminateSchedulerWithCallback(func_8001F358_1FF58);
    }
}

void func_8001F33C_1FF3C(void) {
    func_800697F4_6A3F4(1);
}

void func_8001F358_1FF58(void) {
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
            D_800AFE8C_A71FC->unk8 = 1;
        } else {
            ptr->unk9[0x10] = 3;
            D_800AFE8C_A71FC->unk8 = 1;
        }
    } else {
        ptr->unk9[0x10] = allocation->selectedNumber;
        D_800AFE8C_A71FC->unk22 = allocation->selectedNumber;
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

        func_8006FC70_70870(100, 1, &item->unk18, &item->unk20);

        if (poolId == 11) {
            node = &allocation->previewNode;
            func_8006FA0C_7060C(node, 50.0f, ASPECT_RATIO, 10.0f, 1000.0f);
            func_8006FE48_70A48(100, 900, 999, item->unk20.r2, item->unk20.g2, item->unk20.b2);
            func_80041518_42118(node);
        } else {
            func_8006FA0C_7060C(&allocation->previewNode, 50.0f, ASPECT_RATIO, 10.0f, 4000.0f);
            func_8006FE48_70A48(100, 995, 999, item->unk20.r2, item->unk20.g2, item->unk20.b2);
        }

        scheduleTask(&func_8001F6A0_202A0, 0, 0, 0x5B);
        func_800497FC_4A3FC(poolIdRaw);
        allocation->previewLoadCounter = allocation->previewLoadCounter + 1;
    } else if (counter < 6) {
        allocation->previewLoadCounter = counter + 1;
    } else {
        if (func_8003BB5C_3C75C() == 0) {
            func_8006FDA0_709A0(&allocation->unk588, 0, 0);
            func_8006FDA0_709A0(&allocation->previewNode, 0, 0x14);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/1F190", func_8001F5EC_201EC);
