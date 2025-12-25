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
    Node_70B00 unk0;   // 0x000
    Node_70B00 unk1D8; // 0x1D8
    Node_70B00 unk3B0; // 0x3B0
    Node_70B00 unk588; // 0x588
    Node_70B00 unk760; // 0x760
    Node_70B00 unk938; // 0x938
    void *unkB10;      // 0xB10
    void *unkB14;      // 0xB14
    void *unkB18;      // 0xB18
    void *unkB1C;      // 0xB1C
    void *unkB20;      // 0xB20
    u8 padB24[0x4];    // 0xB24
    u16 unkB28;        // 0xB28
    u8 padB2A[0x2];    // 0xB2A
    s8 unkB2C;         // 0xB2C
    u8 unkB2D;         // 0xB2D
    u8 unkB2E;         // 0xB2E
    u8 padB2F[0x4];    // 0xB2F
    u8 unkB33[12];     // 0xB33
    u8 padB3F[0x5];    // 0xB3F
    u8 unkB44;         // 0xB44
    u8 unkB45;         // 0xB45
    u8 unkB46;         // 0xB46
} Allocation_1F190;

typedef struct {
    Node_70B00 unk0;
    Node_70B00 unk1D8;
    Node_70B00 unk3B0;
    Node_70B00 unk588;
    Node_70B00 unk760;
    Node_70B00 unk938;
    void *unkB10;
    void *unkB14;
    void *unkB18;
    void *unkB1C;
    void *unkB20;
    s32 unkB24;
    u16 unkB28;
    u8 padB2A[0x2];
    s8 unkB2C;
    u8 unkB2D;
    u8 unkB2E;
    u8 menuState;
    u8 unkB30;
    u8 unkB31;
    u8 unkB32;
    u8 unkB33[16];
    u8 unkB43;
    u8 unkB44;
    u8 unkB45;
    u8 unkB46;
    u8 unkB47;
} Allocation_1F190_Local;

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
void func_8001EB4C_1F74C(void);
void func_8001EB1C_1F71C(void);
void func_8001F33C_1FF3C(void);
void func_8001F358_1FF58(void);
void func_8001F374_1FF74(void);
void func_8001F25C_1FE5C(void);
void func_8001F42C_2002C(void);

extern s32 gControllerInputs[];
extern s32 gFrameCounter;
extern u8 D_8008D9C0_8E5C0[];

void func_8001E590_1F190(void) {
    Allocation_1F190 *allocation = allocateTaskMemory(0xB48);
    allocation->unkB45 = 1;
    setGameStateHandler(func_8001E5EC_1F1EC);
}

void func_8001E5C0_1F1C0(void) {
    Allocation_1F190 *allocation = allocateTaskMemory(0xB48);
    allocation->unkB45 = 0;
    setGameStateHandler(func_8001E5EC_1F1EC);
}

INCLUDE_ASM("asm/nonmatchings/1F190", func_8001E5EC_1F1EC);

void func_8001EAB0_1F6B0(void) {
    Allocation_1F190 *allocation = (Allocation_1F190 *)getCurrentAllocation();

    allocation->unkB28++;
    if (allocation->unkB28 < 3) {
        return;
    }

    allocation->unkB28 = 2;
    if (func_8003BB5C_3C75C() != 0) {
        return;
    }

    allocation->unkB28 = 0;
    func_8006FDA0_709A0(0, 0, 0xE);
    setGameStateHandler(func_8001EB1C_1F71C);
}

void func_8001EB1C_1F71C(void) {
    if (func_8006FE10_70A10(0) == 0) {
        setGameStateHandler(func_8001EB4C_1F74C);
    }
}

void func_8001EB4C_1F74C(void) {
    Allocation_1F190_Local *allocation;
    D_800AFE8C_A71FC_type *tempA0;
    s32 i;
    u8 oldMenuSelection;
    u8 oldB46;

    allocation = (Allocation_1F190_Local *)getCurrentAllocation();

    switch (allocation->menuState) {
        case MENU_STATE_NAVIGATE:
            oldMenuSelection = allocation->unkB2C;
            oldB46 = oldMenuSelection & 0xFF;
            for (i = 0; i < D_800AFE8C_A71FC->unk8; i++) {
                if (oldB46 == ((s8)allocation->unkB2C)) {
                    if (gControllerInputs[i] & 0x10800) {
                        allocation->unkB2C = (s8)allocation->unkB2C - 1;
                        allocation->unkB32 = 0;
                        if ((s8)allocation->unkB2C < 0) {
                            allocation->unkB2C = allocation->unkB43 - 1;
                        }
                    } else if (gControllerInputs[i] & 0x20400) {
                        allocation->unkB2C = (s8)allocation->unkB2C + 1;
                        allocation->unkB32 = 1;
                        if ((s8)allocation->unkB2C > (s32)(allocation->unkB43 - 1)) {
                            allocation->unkB2C = 0;
                        }
                    }
                }
            }

            if (oldB46 != ((s8)allocation->unkB2C)) {
                func_800585C8_591C8(0x2B);
                allocation->menuState = MENU_STATE_SCROLL;
                allocation->unkB28 = 0;
                allocation->unkB31 = allocation->unkB33[oldB46];
                allocation->unkB30 = allocation->unkB33[(s8)allocation->unkB2C];
                if (allocation->unkB2E >= 3) {
                    allocation->unkB2E = 0;
                    terminateTasksByType(0);
                    allocation->unkB44 = 1;
                    allocation->unkB24 = gFrameCounter;
                    func_8006FDA0_709A0(&allocation->unk588, 0xFF, 0);
                    func_8006FDA0_709A0(&allocation->unk3B0, 0xFF, 0);
                } else {
                    allocation->unkB2E = 0;
                }
            } else {
                func_8001F42C_2002C();
                for (i = 0; i < D_800AFE8C_A71FC->unk8; i++) {
                    tempA0 = D_800AFE8C_A71FC;
                    if (allocation->menuState == MENU_STATE_NAVIGATE && allocation->unkB2D != 2) {
                        if (gControllerInputs[i] & (A_BUTTON | START_BUTTON)) {
                            if (tempA0->unk21 != 0) {
                                if (tempA0->unk4 == 1) {
                                    allocation->menuState = 3;
                                    allocation->unkB46 = D_800AFE8C_A71FC->unk9[0x10];
                                } else {
                                    allocation->menuState = 2;
                                }
                            } else {
                                allocation->menuState = 2;
                            }
                        } else if (gControllerInputs[i] & B_BUTTON) {
                            func_800585C8_591C8(0x2E);
                            allocation->unkB2D = 2;
                        }
                    }
                }
            }
            break;

        case MENU_STATE_CONFIRM:
            func_8001F42C_2002C();
            for (i = 0; i < D_800AFE8C_A71FC->unk8; i++) {
                if (allocation->menuState == MENU_STATE_CONFIRM && allocation->unkB2D != 1) {
                    if (gControllerInputs[i] & B_BUTTON) {
                        func_800585C8_591C8(0x2E);
                        allocation->menuState = 0;
                    } else if (gControllerInputs[i] & (A_BUTTON | START_BUTTON)) {
                        allocation->unkB2D = 1;
                        D_800AFE8C_A71FC->saveSlotIndex = allocation->unkB33[(s8)allocation->unkB2C];
                        if (D_800AFE8C_A71FC->unk4 == 0) {
                            D_800AFE8C_A71FC->unk9[0xC] = D_8008D9C0_8E5C0[D_800AFE8C_A71FC->saveSlotIndex];
                        }
                        func_8001F374_1FF74();
                        func_80058220_58E20(0x2D, 0);
                    }
                }
            }
            break;

        case MENU_STATE_PROMPT:
            func_8001F42C_2002C();
            if (gControllerInputs[0] & (A_BUTTON | START_BUTTON)) {
                func_800585C8_591C8(0x2C);
                func_8006FDA0_709A0(&allocation->unk3B0, 0xFF, 8);
                allocation->menuState = 6;
            } else if (gControllerInputs[0] & B_BUTTON) {
                func_800585C8_591C8(0x2E);
                allocation->unkB2D = 2;
            }
            break;

        case MENU_STATE_DETAIL_OPEN:
            if (func_8006FE10_70A10(&allocation->unk3B0) == 0) {
                Node_70B00 *node760;

                terminateTasksByType(0);
                node760 = &allocation->unk760;
                allocation->unkB44 = 1;
                allocation->unkB24 = gFrameCounter;
                allocation->menuState = 7;
                func_8006FAA4_706A4(node760, &allocation->unk0, 0xB, 0x12, 0);
                func_8006FEF8_70AF8(node760, 0xC);
                setModelCameraTransform(node760, 0, 0, -0x5C, -0x22, 0x5C, 0x58);
                func_8006FDA0_709A0(node760, 0xFF, 0);
                func_8006FDA0_709A0(node760, 0, 8);
            }
            break;

        case MENU_STATE_DETAIL_WAIT:
            if (func_8006FE10_70A10(&allocation->unk760) == 0) {
                allocation->menuState = 8;
                allocation->unkB47 = 1;
            }
            break;

        case MENU_STATE_DETAIL_CLOSE: {
            Node_70B00 *node760 = &allocation->unk760;
            if (func_8006FE10_70A10(node760) == 0) {
                unlinkNode(node760);
                allocation->menuState = 5;
                allocation->unkB47 = 2;
            }
        } break;

        case MENU_STATE_DETAIL:
            if (gControllerInputs[0] & B_BUTTON) {
                func_800585C8_591C8(0x2E);
                func_8006FDA0_709A0(&allocation->unk760, 0xFF, 8);
                allocation->menuState = 9;
                allocation->unkB2E = 0;
            } else if (gControllerInputs[0] & (A_BUTTON | START_BUTTON)) {
                allocation->unkB2D = 1;
                D_800AFE8C_A71FC->unk9[0xC] = D_8008D9C0_8E5C0[D_800AFE8C_A71FC->saveSlotIndex];
                func_800585C8_591C8(0x2D);
            }
            break;

        case MENU_STATE_NUMBER_SELECT:
            func_8001F42C_2002C();
            oldB46 = allocation->unkB46;
            for (i = 0; i < D_800AFE8C_A71FC->unk8; i++) {
                if ((oldB46 & 0xFF) == allocation->unkB46) {
                    if (gControllerInputs[i] & 0x40100) {
                        allocation->unkB46 = allocation->unkB46 + 1;
                        if ((allocation->unkB46 & 0xFF) >= 10) {
                            allocation->unkB46 = 1;
                        }
                    } else if (gControllerInputs[i] & 0x80200) {
                        allocation->unkB46 = allocation->unkB46 - 1;
                        if ((allocation->unkB46 & 0xFF) == 0) {
                            allocation->unkB46 = 9;
                        }
                    }
                }
            }
            if (oldB46 != allocation->unkB46) {
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

    if (allocation->unkB44 != 0) {
        if (getFreeNodeCount(0) == 0x44) {
            s32 frameCheck = gFrameCounter - 5;
            if ((u32)((allocation->unkB24 - frameCheck) & 0x0FFFFFFF) > 0x07FFFFFF) {
                allocation->unkB44 = 0;
            }
        }
    }

    if (allocation->unkB2D != 0) {
        func_80057564_58164(0xA);
        if (allocation->unkB2D == 2) {
            func_8006FDA0_709A0(NULL, 0xFF, 8);
        } else {
            func_8006FDA0_709A0(NULL, 0xFF, 0x10);
        }
        setGameStateHandler(func_8001F25C_1FE5C);
    }
}

void func_8001F25C_1FE5C(void) {
    Allocation_1F190 *allocation = (Allocation_1F190 *)getCurrentAllocation();

    if (func_8006FE10_70A10(NULL) != 0) {
        return;
    }

    unlinkNode(&allocation->unk0);
    unlinkNode(&allocation->unk3B0);
    unlinkNode(&allocation->unk588);
    unlinkNode(&allocation->unk1D8);
    unlinkNode(&allocation->unk938);

    allocation->unkB10 = freeNodeMemory(allocation->unkB10);
    allocation->unkB14 = freeNodeMemory(allocation->unkB14);
    allocation->unkB18 = freeNodeMemory(allocation->unkB18);
    allocation->unkB1C = freeNodeMemory(allocation->unkB1C);
    allocation->unkB20 = freeNodeMemory(allocation->unkB20);

    if (allocation->unkB2D == 1) {
        terminateSchedulerWithCallback(func_8001F33C_1FF3C);
        if (allocation->unkB45 != 0) {
            unlinkNode(&allocation->unk760);
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

void func_8001F374_1FF74(void) {
    Allocation_1F190 *allocation;
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
        ptr->unk9[0x10] = allocation->unkB46;
        D_800AFE8C_A71FC->unk22 = allocation->unkB46;
    }
}

#define ASPECT_RATIO (4.0f / 3.0f)

void func_8001F42C_2002C(void) {
    Allocation_1F190 *allocation;
    D_80090F90_91B90_item *item;
    u8 counter;
    u8 poolIdRaw;
    u8 poolId;
    Node_70B00 *node;

    allocation = (Allocation_1F190 *)getCurrentAllocation();

    if (allocation->unkB44 != 0) {
        return;
    }

    counter = allocation->unkB2E;
    if (counter < 3) {
        allocation->unkB2E = counter + 1;
        if ((u8)(counter + 1) != 3) {
            return;
        }

        poolIdRaw = allocation->unkB33[allocation->unkB2C];
        poolId = poolIdRaw;
        item = func_80055D10_56910(poolId);

        func_8006FC70_70870(100, 1, &item->unk18, &item->unk20);

        if (poolId == 11) {
            node = &allocation->unk3B0;
            func_8006FA0C_7060C(node, 50.0f, ASPECT_RATIO, 10.0f, 1000.0f);
            func_8006FE48_70A48(100, 900, 999, item->unk20.r2, item->unk20.g2, item->unk20.b2);
            func_80041518_42118(node);
        } else {
            func_8006FA0C_7060C(&allocation->unk3B0, 50.0f, ASPECT_RATIO, 10.0f, 4000.0f);
            func_8006FE48_70A48(100, 995, 999, item->unk20.r2, item->unk20.g2, item->unk20.b2);
        }

        scheduleTask(&func_8001F6A0_202A0, 0, 0, 0x5B);
        func_800497FC_4A3FC(poolIdRaw);
        allocation->unkB2E = allocation->unkB2E + 1;
    } else if (counter < 6) {
        allocation->unkB2E = counter + 1;
    } else {
        if (func_8003BB5C_3C75C() == 0) {
            func_8006FDA0_709A0(&allocation->unk588, 0, 0);
            func_8006FDA0_709A0(&allocation->unk3B0, 0, 0x14);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/1F190", func_8001F5EC_201EC);
