#include "20F0.h"
#include "38C90.h"
#include "3E160.h"
#include "4050.h"
#include "6E840.h"
#include "B040.h"
#include "common.h"
#include "graphics.h"
#include "task_scheduler.h"

extern s16 gCurrentBgmId;
extern u8 D_8009DF6C_9EB6C[];

typedef struct {
    s32 value;
    void *ptr;
} ArrayElement_8;

extern ArrayElement_8 D_8008CEA0_8DAA0[];

typedef struct {
    void *ptr;
    s32 count;
} LookupEntry;

extern LookupEntry D_8008CE9C_8DA9C[];

typedef struct {
    u8 unk0;
    u8 unk1;
    s8 pad[5];
    s8 unlockSlotIndex;
    u8 pad2[4];
} GalleryItemEntry;

typedef struct {
    GalleryItemEntry *items;
    s32 count;
} GalleryCategoryData;

extern GalleryCategoryData gGalleryCategories[];

typedef struct {
    s8 menuState;
    s8 selectedOption;
    s8 menuType;
    s8 viewerComplete;
    u8 pad4[0x4];
    void *unk8;
    void *unkC;
    s16 unk10;
    s16 fadeTimer;
    s16 animTimer;
    u8 unk16;
    u8 pad17;
    void *unk18;
    u8 pad1C[0x4];
    Node_70B00 unk20;
    Node_70B00 fadeNode;
    Node_70B00 unk3D0;
    u8 pad5A8[0x1C];
    void *unk5C4;
    u8 pad5C8[0x2C];
    void *unk5F4;
    u8 pad5F8[0x2C];
    SceneModel *menuModel;
} E770_struct;

void playBgmTrack(E770_struct *arg0, s16 bgmId) {
    gCurrentBgmId = bgmId;
    arg0->unk10 = 0x28;
    func_80057514_58114(bgmId, 0x80, 0);
}

void beginMenuFadeOut(E770_struct *arg0) {
    func_8006FDA0_709A0(&arg0->fadeNode, 0xFF, 10);
    arg0->fadeTimer = 10;
}

void beginMenuFadeIn(E770_struct *arg0) {
    func_8006FDA0_709A0(&arg0->fadeNode, 0, 10);
    arg0->fadeTimer = 10;
}

s32 getMenuOptionCount(E770_struct *arg0) {
    s8 temp = arg0->menuType;

    if (temp < 2) {
        return D_8008CEA0_8DAA0[temp].value;
    }
    return 0;
}

void *getMenuOptionEntry(E770_struct *arg0, s32 index) {
    void *result;
    s8 menuType;
    LookupEntry *entry;

    result = NULL;
    menuType = arg0->menuType;

    if (menuType < 2) {
        entry = &D_8008CE9C_8DA9C[menuType];
        if (index < entry->count) {
            result = (u8 *)entry->ptr + index * 8;
        }
    }
    return result;
}

void setMenuAnimation(E770_struct *arg0, s16 animIndex, s16 transitionAnimIndex, s8 loopCount, s16 animTimer) {
    setModelAnimationLooped(arg0->menuModel, animIndex, transitionAnimIndex, loopCount);
    arg0->animTimer = animTimer;
}

INCLUDE_ASM("asm/nonmatchings/E770", func_8000DCD8_E8D8);

void cleanupGalleryMenu(E770_struct *arg0) {
    destroySceneModel(arg0->menuModel);
    arg0->unk18 = freeNodeMemory(arg0->unk18);
    arg0->unkC = freeNodeMemory(arg0->unkC);
    arg0->unk8 = freeNodeMemory(arg0->unk8);
    arg0->unk5C4 = freeNodeMemory(arg0->unk5C4);
    arg0->unk5F4 = freeNodeMemory(arg0->unk5F4);
    unlinkNode(&arg0->unk3D0);
    unlinkNode(&arg0->unk20);
    unlinkNode(&arg0->fadeNode);
}

void activateGalleryMenu(E770_struct *arg0) {
    func_800394BC_3A0BC(&arg0->pad5C8, (s32)arg0->unk5C4);
    func_800394BC_3A0BC(&arg0->pad5F8, (s32)arg0->unk5F4);
    func_8006FDA0_709A0(NULL, 0, 10);
    setMenuAnimation(arg0, 0x90, 0x90, -1, 0);
    arg0->fadeTimer = 10;
    arg0->menuState = 2;
}

void waitForMenuFadeIn(E770_struct *arg0) {
    s16 temp = arg0->fadeTimer;

    if (temp == 0) {
        if (arg0->menuModel->unk16 == 0x15B) {
            setMenuAnimation(arg0, 0x15C, 0x90, 1, -1);
        }
        arg0->menuState = 2;
    } else {
        arg0->fadeTimer = temp - 1;
    }
}

extern s32 gControllerInputs;

void handleGalleryMenuInput(E770_struct *arg0) {
    s32 inputs;
    u8 selection;
    s8 sel_signed;
    s32 sound;
    s32 sel2;

    inputs = gControllerInputs;
    selection = arg0->selectedOption;

    // B button - exit
    if (inputs & B_BUTTON) {
        arg0->menuState = 5;
        return;
    }

    // A button - confirm
    if (inputs & A_BUTTON) {
        beginMenuFadeOut(arg0);
        if (arg0->selectedOption == 5) {
            func_80057564_58164(10);
            func_8006FDA0_709A0(0, 0xFF, 10);
        }
        setMenuAnimation(arg0, 0x15A, 0x15B, 1, 0x14);
        sound = 0xD3;
        arg0->menuState = 3;
        goto play_sound;
    }

    // Left / D-pad left (0x80200)
    if (inputs & 0x80200) {
        sel_signed = (s8)selection;
        if (sel_signed <= 0) {
            goto check_bounds;
        }
        if (sel_signed < 3) {
            arg0->selectedOption = selection - 1;
            goto check_bounds;
        }
        if (sel_signed >= 6) {
            goto check_bounds;
        }
        if (sel_signed < 4) {
            goto check_bounds;
        }
        arg0->selectedOption = selection - 1;
        goto check_bounds;
    }

    // Right / D-pad right (0x40100)
    if (inputs & 0x40100) {
        sel_signed = (s8)selection;
        if (sel_signed < 0) {
            goto set_zero;
        }
        if (sel_signed < 2) {
            arg0->selectedOption = selection + 1;
            goto check_bounds;
        }
        if (sel_signed >= 5) {
            goto check_bounds;
        }
        if (sel_signed < 3) {
            goto check_bounds;
        }
        arg0->selectedOption = selection + 1;
        goto check_bounds;
    }

    // Up / D-pad up (0x10800)
    if (inputs & 0x10800) {
        sel_signed = (s8)selection;
        if (sel_signed >= 6) {
            goto check_bounds;
        }
        if (sel_signed < 3) {
            goto check_bounds;
        }
        arg0->selectedOption = selection - 2;
        if (arg0->menuType == 0) {
            goto check_bounds;
        }
        arg0->selectedOption = selection - 3;
        goto check_bounds;
    }

    // Down / D-pad down (0x20400)
    if (!(inputs & 0x20400)) {
        goto check_bounds;
    }
    sel_signed = (s8)selection;
    if (sel_signed == 0) {
        goto add_three;
    }
    if (sel_signed < 0) {
        goto check_bounds;
    }
    if (sel_signed >= 3) {
        goto check_bounds;
    }
    if (arg0->menuType != 0) {
        goto add_three;
    }
    arg0->selectedOption = selection - 1;

add_three:
    arg0->selectedOption = arg0->selectedOption + 3;

check_bounds:
    if (arg0->selectedOption >= 0) {
        goto check_max;
    }

set_zero:
    arg0->selectedOption = 0;

check_max:
    sel2 = arg0->selectedOption;
    if (getMenuOptionCount(arg0) - 1 < sel2) {
        arg0->selectedOption = getMenuOptionCount(arg0) - 1;
    }

    if (arg0->selectedOption == (s8)selection) {
        return;
    }
    sound = 0x2B;

play_sound:
    func_800585C8_591C8(sound);
}

typedef struct FD98_struct {
    s8 viewerState;
    s8 navigationMode;
    s8 cursorIndex;
    u8 pageUpCursorDest;
    u8 pageDownCursorDest;
    u8 pad5[0x3];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
} FD98_struct;

extern void initGalleryViewer(FD98_struct *);

void processGalleryMenuSelection(E770_struct *arg0) {
    s16 temp = arg0->fadeTimer;

    if (temp == 0) {
        if (arg0->selectedOption == 5) {
            cleanupGalleryMenu(arg0);
            setCutsceneSelection(0, 2);
            createTaskQueue(loadCutsceneOverlay, 0x64);
            arg0->unk16 = 1;
            arg0->menuState = 7;
        } else {
            scheduleTask(initGalleryViewer, 0, 0, 0);
            arg0->viewerComplete = 0;
            arg0->menuState = 4;
        }
    } else {
        arg0->fadeTimer = temp - 1;
    }
}

void waitForGalleryViewerExit(E770_struct *arg0) {
    if (arg0->viewerComplete != 0) {
        terminateTasksByType(1);
        beginMenuFadeIn(arg0);
        arg0->menuState = 1;
    }
}

void beginGalleryMenuExit(E770_struct *arg0) {
    func_800585C8_591C8(0xDB);
    func_8006FDA0_709A0(NULL, 0xFF, 10);
    setMenuAnimation(arg0, 0x15A, 0x15B, 1, -1);
    arg0->fadeTimer = 10;
    arg0->menuState = 6;
}

extern void onGalleryMenuExit(void);

s32 completeGalleryMenuExit(E770_struct *arg0) {
    s16 temp;

    if (arg0->fadeTimer == 8) {
        func_80057564_58164(8);
    }
    temp = arg0->fadeTimer;
    if (temp == 0) {
        cleanupGalleryMenu(arg0);
        terminateSchedulerWithCallback(onGalleryMenuExit);
        return 1;
    }
    arg0->fadeTimer = temp - 1;
    return 0;
}

extern void func_8000E6E0_F2E0(void);

void initGalleryMenu(void) {
    allocateTaskMemory(0xCC0);
    gCurrentBgmId = 5;
    setupTaskSchedulerNodes(0x40, 4, 0, 0, 0, 0, 0, 0);
    func_800585C8_591C8(0xD0);
    setGameStateHandler(func_8000E6E0_F2E0);
}

INCLUDE_ASM("asm/nonmatchings/E770", func_8000E6E0_F2E0);

extern void func_8000DCD8_E8D8(E770_struct *);

void updateGalleryMenu(void) {
    E770_struct *s0;

    s0 = getCurrentAllocation();
    switch (s0->menuState) {
        case 0:
            activateGalleryMenu(s0);
            break;
        case 1:
            waitForMenuFadeIn(s0);
            break;
        case 2:
            handleGalleryMenuInput(s0);
            break;
        case 3:
            processGalleryMenuSelection(s0);
            break;
        case 4:
            waitForGalleryViewerExit(s0);
            break;
        case 5:
        default:
            beginGalleryMenuExit(s0);
            break;
        case 6:
            if (completeGalleryMenuExit(s0)) {
                return;
            }
            break;
        case 7:
            if (func_80069810_6A410()) {
                setGameStateHandler(func_8000E6E0_F2E0);
            }
            return;
    }
    func_8000DCD8_E8D8(s0);
}

void onGalleryMenuExit(void) {
    gCurrentBgmId = 5;
    func_800697F4_6A3F4(1);
}

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEB0_9EAB0);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEB4_9EAB4);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEB8_9EAB8);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEBC_9EABC);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEC0_9EAC0);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEC4_9EAC4);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEC8_9EAC8);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DECC_9EACC);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DED0_9EAD0);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DED4_9EAD4);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DED8_9EAD8);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEDC_9EADC);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEE0_9EAE0);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEE4_9EAE4);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEE8_9EAE8);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEEC_9EAEC);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEF0_9EAF0);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEF4_9EAF4);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEF8_9EAF8);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEFC_9EAFC);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF00_9EB00);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF04_9EB04);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF08_9EB08);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF0C_9EB0C);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF10_9EB10);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF14_9EB14);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF18_9EB18);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF1C_9EB1C);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF20_9EB20);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF24_9EB24);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF28_9EB28);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF2C_9EB2C);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF30_9EB30);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF34_9EB34);

u8 isGalleryItemUnlocked(u8 itemIndex) {
    E770_struct *menu;
    s8 categoryIndex;
    GalleryItemEntry *item;
    s8 unlockSlot;
    u8 itemIndexMasked;

    menu = getCurrentAllocation();
    categoryIndex = menu->selectedOption;
    itemIndexMasked = itemIndex;
    item = &gGalleryCategories[categoryIndex].items[itemIndexMasked];

    switch (categoryIndex) {
        case 1:
            return func_80038058_38C58(itemIndex) & 0xFF;
        case 2:
            return func_80038000_38C00(itemIndex) & 0xFF;
        case 3:
            unlockSlot = item->unlockSlotIndex;
            if (unlockSlot == -1) {
                return 1;
            }
            if (unlockSlot == -2) {
                return func_80038070_38C70() & 0xFF;
            }
            return isSaveSlotUnlocked(unlockSlot & 0xFF) & 0xFF;
        case 0:
        case 4:
            return isSaveSlotUnlocked(itemIndex) & 0xFF;
        case 5:
            return func_80038070_38C70() & 0xFF;
    }
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/E770", func_8000EE88_FA88);

void startViewerFadeIn(E770_struct *arg0) {
    void *alloc = getCurrentAllocation();
    arg0->menuState = 1;
    beginMenuFadeIn(alloc);
}

void waitForViewerFadeIn(E770_struct *arg0) {
    E770_struct *alloc = getCurrentAllocation();
    s16 temp;

    temp = alloc->fadeTimer;
    if (temp == 0) {
        if (alloc->menuModel->unk16 == 0x15B) {
            setMenuAnimation(alloc, 0x15C, 0x90, 1, -1);
        }
        arg0->menuState = 2;
    } else {
        alloc->fadeTimer = temp - 1;
    }
}

void handleViewerGridNavigation_TwoPage(FD98_struct *arg0) {
    s32 inputs;
    s8 temp;
    u8 val;

    inputs = gControllerInputs;

    if (inputs & 0x80200) {
        if (arg0->cursorIndex == 0) {
            arg0->cursorIndex = 14;
        } else {
            arg0->cursorIndex = arg0->cursorIndex - 1;
        }
        func_800585C8_591C8(0x2B);
        return;
    }

    if (inputs & 0x40100) {
        if (arg0->cursorIndex == 14) {
            arg0->cursorIndex = 0;
        } else {
            arg0->cursorIndex = arg0->cursorIndex + 1;
        }
        func_800585C8_591C8(0x2B);
        return;
    }

    if (inputs & 0x10800) {
        val = arg0->cursorIndex;
        if ((u8)(val - 8) < 7u) {
            arg0->cursorIndex = val - 7;
        } else {
            if (val == 0) {
                arg0->pageUpCursorDest = val + 8;
                arg0->pageDownCursorDest = arg0->cursorIndex;
            } else {
                arg0->pageUpCursorDest = val + 7;
                arg0->pageDownCursorDest = arg0->cursorIndex;
            }
            arg0->navigationMode = 1;
        }
        func_800585C8_591C8(0x2B);
        return;
    }

    if (inputs & 0x20400) {
        temp = arg0->cursorIndex;
        if (temp == 0) {
            arg0->cursorIndex = 8;
        } else if ((u8)(temp - 1) < 7u) {
            arg0->cursorIndex = temp + 7;
        } else {
            arg0->pageUpCursorDest = temp;
            arg0->navigationMode = 1;
            arg0->pageDownCursorDest = arg0->cursorIndex - 7;
        }
        func_800585C8_591C8(0x2B);
    }
}

void handleViewerGridNavigation_TwoColumn(FD98_struct *arg0) {
    s32 inputs;
    s8 temp2;
    s8 newVal;

    inputs = gControllerInputs;

    if (inputs & 0x80200) {
        s8 temp = arg0->cursorIndex;
        if (!temp) {
            newVal = 8;
        } else if (temp == 9) {
            newVal = 17;
        } else {
            newVal = temp - 1;
        }
        arg0->cursorIndex = newVal;
        func_800585C8_591C8(0x2B);
        return;
    }

    if (inputs & 0x40100) {
        s8 temp = arg0->cursorIndex;
        if (temp == 8) {
            arg0->cursorIndex = 0;
        } else if (temp == 17) {
            arg0->cursorIndex = 9;
        } else {
            arg0->cursorIndex = temp + 1;
        }
        func_800585C8_591C8(0x2B);
        return;
    }

    if (inputs & 0x10800) {
        u8 val = arg0->cursorIndex;
        temp2 = val - 9;
        if ((u8)temp2 < 9u) {
            arg0->cursorIndex = temp2;
        } else {
            arg0->pageUpCursorDest = val + 9;
            arg0->navigationMode = 1;
            arg0->pageDownCursorDest = arg0->cursorIndex;
        }
        func_800585C8_591C8(0x2B);
        return;
    }

    if (inputs & 0x20400) {
        u8 val = arg0->cursorIndex;
        if (val < 9u) {
            arg0->cursorIndex = val + 9;
        } else {
            arg0->pageUpCursorDest = val;
            arg0->navigationMode = 1;
            arg0->pageDownCursorDest = arg0->cursorIndex - 9;
        }
        func_800585C8_591C8(0x2B);
    }
}

void handleViewerGridNavigation_SingleRow(FD98_struct *arg0) {
    s32 inputs;

    inputs = gControllerInputs;

    if (inputs & 0x80200) {
        if (arg0->cursorIndex == 0) {
            arg0->cursorIndex = 8;
        } else {
            arg0->cursorIndex = arg0->cursorIndex - 1;
        }
        func_800585C8_591C8(0x2B);
        return;
    }

    if (inputs & 0x40100) {
        if (arg0->cursorIndex == 8) {
            arg0->cursorIndex = 0;
        } else {
            arg0->cursorIndex = arg0->cursorIndex + 1;
        }
        func_800585C8_591C8(0x2B);
        return;
    }

    if (inputs & 0x10800) {
        goto set_values;
    }
    if (inputs & 0x20400) {
    set_values:
        arg0->pageUpCursorDest = arg0->cursorIndex;
        arg0->pageDownCursorDest = arg0->cursorIndex;
        arg0->navigationMode = 1;
        func_800585C8_591C8(0x2B);
    }
}

void handleViewerGridNavigation_ThreeRow(FD98_struct *arg0) {
    u8 sp30[0xC];
    E770_struct *alloc;
    GalleryItemEntry *entry;
    u8 *sp30_ptr;

    alloc = getCurrentAllocation();
    sp30_ptr = sp30;
    entry = &gGalleryCategories[alloc->selectedOption].items[arg0->cursorIndex];
    memcpy(sp30_ptr, D_8009DF6C_9EB6C, 0xC);

    if (gControllerInputs & 0x8000) {
        if (isGalleryItemUnlocked(arg0->cursorIndex)) {
            playBgmTrack(alloc, entry->pad[4]);
            if (alloc->menuModel->unk16 != 0x92) {
                spawnSpriteEffectInternal(
                    alloc->menuModel,
                    0,
                    9,
                    -1,
                    (cutsceneEffectDisp_exec_arg *)sp30_ptr,
                    0xCCCC,
                    0,
                    1,
                    0,
                    0,
                    1
                );
            }
            setMenuAnimation(alloc, 0x92, 0x92, -1, 0);
            return;
        }
        func_800585C8_591C8(9);
        return;
    }

    if (gControllerInputs & 0x80200) {
        s8 temp_v0 = arg0->cursorIndex;
        s8 temp_v1 = temp_v0;
        if (temp_v0 == 0) {
            arg0->cursorIndex = 0x1A;
        } else {
            arg0->cursorIndex = temp_v1 - 1;
        }
        func_800585C8_591C8(0x2B);
        return;
    }

    if (gControllerInputs & 0x40100) {
        s8 temp = arg0->cursorIndex;
        if (temp == 0x1A) {
            arg0->cursorIndex = 0;
        } else {
            arg0->cursorIndex = temp + 1;
        }
        func_800585C8_591C8(0x2B);
        return;
    }

    if (gControllerInputs & 0x10800) {
        u8 val = arg0->cursorIndex;
        s8 temp = val - 9;
        if ((u8)temp < 0x12u) {
            arg0->cursorIndex = temp;
        } else {
            arg0->pageUpCursorDest = val + 0x12;
            arg0->navigationMode = 1;
            arg0->pageDownCursorDest = arg0->cursorIndex;
        }
        func_800585C8_591C8(0x2B);
        return;
    }

    if (gControllerInputs & 0x20400) {
        u8 val = arg0->cursorIndex;
        if (val < 0x12u) {
            arg0->cursorIndex = val + 9;
        } else {
            arg0->pageUpCursorDest = val;
            arg0->navigationMode = 1;
            arg0->pageDownCursorDest = arg0->cursorIndex - 0x12;
        }
        func_800585C8_591C8(0x2B);
    }
}

void handleViewerGridNavigation_Prize(FD98_struct *arg0) {
    s32 inputs;
    s8 temp;
    u8 val;

    inputs = gControllerInputs;

    if (inputs & 0x80200) {
        if (arg0->cursorIndex == 0) {
            arg0->cursorIndex = 14;
        } else {
            arg0->cursorIndex = arg0->cursorIndex - 1;
        }
        func_800585C8_591C8(0x2B);
        return;
    }

    if (inputs & 0x40100) {
        if (arg0->cursorIndex == 14) {
            arg0->cursorIndex = 0;
        } else {
            arg0->cursorIndex = arg0->cursorIndex + 1;
        }
        func_800585C8_591C8(0x2B);
        return;
    }

    if (inputs & 0x10800) {
        val = arg0->cursorIndex;
        if ((u8)(val - 8) < 7u) {
            arg0->cursorIndex = val - 7;
        } else {
            if (val == 0) {
                arg0->pageUpCursorDest = val + 8;
                arg0->pageDownCursorDest = arg0->cursorIndex;
            } else {
                arg0->pageUpCursorDest = val + 7;
                arg0->pageDownCursorDest = arg0->cursorIndex;
            }
            arg0->navigationMode = 1;
        }
        func_800585C8_591C8(0x2B);
        return;
    }

    if (inputs & 0x20400) {
        temp = arg0->cursorIndex;
        if (temp == 0) {
            arg0->cursorIndex = 8;
        } else if ((u8)(temp - 1) < 7u) {
            arg0->cursorIndex = temp + 7;
        } else {
            arg0->pageUpCursorDest = temp;
            arg0->navigationMode = 1;
            arg0->pageDownCursorDest = arg0->cursorIndex - 7;
        }
        func_800585C8_591C8(0x2B);
    }
}

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF6C_9EB6C);

void handleViewerInput(FD98_struct *arg0) {
    FD98_struct *alloc;
    s32 inputs;
    s8 temp;
    u8 newVal;
    s32 sound;

    alloc = getCurrentAllocation();
    inputs = gControllerInputs;

    if (inputs & B_BUTTON) {
        setMenuAnimation((E770_struct *)alloc, 0x90, 0x90, -1, -1);
        sound = 0x2E;
        arg0->viewerState = 3;
        goto play_sound;
    }

    temp = arg0->navigationMode;
    if (temp == 0) {
        goto do_switch;
    }
    if (temp == 1) {
        goto button_check;
    }
    return;

do_switch:
    switch (alloc->navigationMode) {
        case 0:
            handleViewerGridNavigation_TwoPage(arg0);
            break;
        case 1:
            handleViewerGridNavigation_TwoColumn(arg0);
            break;
        case 2:
            handleViewerGridNavigation_SingleRow(arg0);
            break;
        case 3:
            handleViewerGridNavigation_ThreeRow(arg0);
            break;
        case 4:
            handleViewerGridNavigation_Prize(arg0);
            break;
    }
    return;

button_check:
    if (inputs & A_BUTTON) {
        arg0->viewerState = 3;
        setMenuAnimation((E770_struct *)alloc, 0x90, 0x90, -1, -1);
        sound = 0x2E;
        goto play_sound;
    }
    if (inputs & 0x10800) {
        newVal = arg0->pageUpCursorDest;
        goto set_2b;
    }
    if (inputs & 0x20400) {
        newVal = arg0->pageDownCursorDest;
    set_2b:
        sound = 0x2B;
        arg0->navigationMode = 0;
        arg0->cursorIndex = newVal;
    play_sound:
        func_800585C8_591C8(sound);
    }
}

void startViewerFadeOut(E770_struct *arg0) {
    void *alloc = getCurrentAllocation();
    beginMenuFadeOut(alloc);
    arg0->menuState = 4;
}

s32 updateViewerFadeOut(E770_struct *arg0) {
    E770_struct *alloc = getCurrentAllocation();
    s16 temp = alloc->fadeTimer;

    if (temp == 0) {
        func_80069CF8_6A8F8();
        return 1;
    }
    alloc->fadeTimer = temp - 1;
    return 0;
}

extern void onGalleryViewerCleanup(void);
extern void updateGalleryViewer(E770_struct *);

void initGalleryViewer(FD98_struct *arg0) {
    getCurrentAllocation();
    setCleanupCallback(onGalleryViewerCleanup);
    arg0->unk10 = 0x780000;
    arg0->unk14 = 0x0F0000;
    arg0->unk8 = 0xFF0000;
    arg0->viewerState = 0;
    arg0->navigationMode = 0;
    arg0->cursorIndex = 0;
    arg0->unkC = 0xFFF10000;
    setCallback(updateGalleryViewer);
}

extern void func_8000EE88_FA88(E770_struct *);

void updateGalleryViewer(E770_struct *arg0) {
    getCurrentAllocation();
    switch (arg0->menuState) {
        case 0:
            startViewerFadeIn(arg0);
            break;
        case 1:
            waitForViewerFadeIn(arg0);
            break;
        case 2:
            handleViewerInput((FD98_struct *)arg0);
            break;
        case 3:
            startViewerFadeOut(arg0);
            break;
        case 4:
            if (updateViewerFadeOut(arg0)) {
                return;
            }
            break;
    }
    func_8000EE88_FA88(arg0);
}

void onGalleryViewerCleanup(void) {
    E770_struct *alloc = getCurrentAllocation();
    alloc->viewerComplete = 1;
}
