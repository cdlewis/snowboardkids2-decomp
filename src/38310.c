#include "38310.h"
#include "10AD0.h"
#include "33FE0.h"
#include "36B80.h"
#include "3E160.h"
#include "D_800AFE8C_A71FC_type.h"
#include "common.h"
#include "overlay.h"
#include "task_scheduler.h"

USE_ASSET(_4196E0);
USE_ASSET(_419C60);
USE_ASSET(_4237C0);
USE_ASSET(_426EF0);

typedef struct {
    s16 x;
    s16 y;
    void *spriteAsset;
    s16 frameIndex;
    s16 highlightValue;
    u8 unkC;
    u8 displayFlags;
    u8 unkE;
    u8 unkF;
} OptionsMenuToggleIconEntry;

typedef union {
    s16 asShort;
    u8 asBytes[2];
} ToggleLabelUnion;

typedef struct {
    s16 x;
    s16 y;
    void *textData;
    void *textAsset;
    ToggleLabelUnion highlight;
    s16 alpha;
    u8 textStyle;
    u8 unk11;
    u8 unk12;
    u8 unk13;
} OptionsMenuToggleLabelEntry;

typedef struct {
    OptionsMenuToggleIconEntry iconEntries[6];
    OptionsMenuToggleLabelEntry labelEntries[6];
    void *textRenderAsset;
} OptionsMenuToggleState;

typedef struct {
    u8 pad[0x8];
    void *textAsset;
    u8 pad2[0xC];
    void *spriteAsset;
} OptionsMenuTitleCleanupArg;

typedef struct {
    u8 pad0[0x4];
    void *unk4;
    u8 pad[0xD0];
    void *unkD8;
} OptionsMenuTogglesCleanupArg;

typedef struct {
    void *unk0;
    void *unk4;
    u8 pad[0x40];
    void *unk48;
} OptionsMenuLabelsCleanupArg;

typedef struct {
    u8 pad0[0x4];
    void *unk4;
} func_80037FB0_38BB0_arg;

typedef struct {
    u8 pad[0x1E0];
    u16 unk1E0;
    u16 unk1E2;
    u8 pad2[0x4];
    u8 unk1E8[4];
    u8 unk1EC;
} func_80037F14_alloc;

extern u8 optionsMenuTitleTextData[];
extern void *D_8008FD8C_9098C;
extern void *D_8008FD9C_9099C;
extern void *D_8008FDAC_909AC;
extern void *D_8008FDBC_909BC;
extern void *optionsMenuLabelTextData[];

void cleanupOptionsMenuLabels(OptionsMenuLabelsCleanupArg *);
void updateOptionsMenuLabels(OptionsMenuLabelsState *);
void cleanupOptionsMenuTitle(OptionsMenuTitleCleanupArg *);
void updateOptionsMenuTitle(u8 *);
void func_80037F14_38B14(OptionsMenuLabelIconEntry *);
void func_80037FB0_38BB0(func_80037FB0_38BB0_arg *arg0);

void initOptionsMenuTitle(OptionsMenuTitleState *arg0) {
    void *textAsset;
    void *spriteAsset;

    textAsset = loadTextRenderAsset(1);
    spriteAsset = loadCompressedData(&_4196E0_ROM_START, &_4196E0_ROM_END, 0xBB8);
    setCleanupCallback(cleanupOptionsMenuTitle);

    arg0->titleText.x = -0x50;
    arg0->titleText.y = -0x58;
    arg0->titleText.textData = optionsMenuTitleTextData;
    arg0->titleText.textAsset = textAsset;
    arg0->titleText.primaryColor = 0xFF;
    arg0->titleText.secondaryColor = 0xFF;
    arg0->titleText.textStyle = 0;

    arg0->leftIcon.x = -0x90;
    arg0->leftIcon.y = -0x60;
    arg0->leftIcon.spriteAsset = spriteAsset;
    arg0->leftIcon.frameIndex = 2;
    arg0->leftIcon.alpha = 0xFF;
    arg0->leftIcon.blinkState = 1;
    arg0->leftIcon.unkD = 0;

    arg0->rightIcon.x = 0x58;
    arg0->rightIcon.y = -0x60;
    arg0->rightIcon.spriteAsset = spriteAsset;
    arg0->rightIcon.frameIndex = 2;
    arg0->rightIcon.alpha = 0xFF;
    arg0->rightIcon.blinkState = 0;
    arg0->rightIcon.unkD = 0;

    setCallback(updateOptionsMenuTitle);
}

void updateOptionsMenuTitle(u8 *arg0) {
    s32 i;

    debugEnqueueCallback(8, 0, func_80035408_36008, arg0);

    for (i = 0; i < 2; i++) {
        debugEnqueueCallback(8, 0, func_80012004_12C04, arg0 + 0x14 + i * 0x10);
    }
}

void cleanupOptionsMenuTitle(OptionsMenuTitleCleanupArg *arg0) {
    arg0->textAsset = freeNodeMemory(arg0->textAsset);
    arg0->spriteAsset = freeNodeMemory(arg0->spriteAsset);
}

void cleanupOptionsMenuToggles(OptionsMenuTogglesCleanupArg *);
void updateOptionsMenuToggles(OptionsMenuToggleState *);

#define ARG0 ((OptionsMenuToggleState *)arg0)

void initOptionsMenuToggles(void *arg0) {
    void *spriteAsset;
    s32 i;
    s32 column;
    u8 optionValue;

    getCurrentAllocation();
    spriteAsset = loadCompressedData(&_4196E0_ROM_START, &_419C60_ROM_START, 0xBB8);
    ARG0->textRenderAsset = loadTextRenderAsset(1);
    setCleanupCallback(cleanupOptionsMenuToggles);

    for (i = 0; i < 6; i++) {
        column = i & 1;
        ARG0->iconEntries[i].x = (column * 0x38) + 0x18;
        ARG0->iconEntries[i].y = ((i / 2) << 5) - 0x20;
        ARG0->iconEntries[i].spriteAsset = spriteAsset;
        ARG0->iconEntries[i].frameIndex = 1;
        ARG0->iconEntries[i].highlightValue = 0;
        ARG0->iconEntries[i].unkC = 0;

        if (i < 2) {
            optionValue = D_800AFE8C_A71FC->unk1F;
        } else if (i < 4) {
            optionValue = D_800AFE8C_A71FC->unk20;
        } else {
            optionValue = (D_800AFE8C_A71FC->customLapEnabled + 1) & 1;
        }
        column = i & 1;
        ARG0->iconEntries[i].displayFlags = ((optionValue + column) & 1) | 2;

        ARG0->labelEntries[i].x = (column * 0x3A) + 0x16;
        ARG0->labelEntries[i].y = ARG0->iconEntries[i].y + 2;
        ARG0->labelEntries[i].textAsset = ARG0->textRenderAsset;
        ARG0->labelEntries[i].highlight.asShort = 0;
        ARG0->labelEntries[i].alpha = 0xFF;
        ARG0->labelEntries[i].textStyle = 5;

        if (i < 4) {
            if (column != 0) {
                ARG0->labelEntries[i].textData = &D_8008FD9C_9099C;
            } else {
                ARG0->labelEntries[i].textData = &D_8008FD8C_9098C;
            }
        } else {
            if (column != 0) {
                ARG0->labelEntries[i].textData = &D_8008FDBC_909BC;
            } else {
                ARG0->labelEntries[i].textData = &D_8008FDAC_909AC;
            }
        }
    }

    setCallback(updateOptionsMenuToggles);
}

#undef ARG0

void updateOptionsMenuToggles(OptionsMenuToggleState *arg0) {
    func_80037F14_alloc *alloc;
    s32 i;
    u8 optionValue;

    alloc = getCurrentAllocation();

    for (i = 0; i < 6; i++) {
        if (i < 2) {
            optionValue = D_800AFE8C_A71FC->unk1F;
        } else if (i < 4) {
            optionValue = D_800AFE8C_A71FC->unk20;
        } else {
            optionValue = (D_800AFE8C_A71FC->customLapEnabled + 1) & 1;
        }

        arg0->iconEntries[i].displayFlags = ((optionValue + (i & 1)) & 1) | 2;

        if (alloc->unk1E2 == 0) {
            u8 idx;
            s32 value;
            idx = alloc->unk1EC;
            value = optionValue + (idx << 1);
            if (value == i) {
                arg0->iconEntries[i].highlightValue = alloc->unk1E8[idx];
                arg0->labelEntries[i].highlight.asShort = alloc->unk1E8[alloc->unk1EC];
            } else {
                arg0->iconEntries[i].highlightValue = 0;
                arg0->labelEntries[i].highlight.asShort = 0;
            }
        } else {
            arg0->iconEntries[i].highlightValue = 0;
            arg0->labelEntries[i].highlight.asShort = 0;
        }

        debugEnqueueCallback(8, 0, func_80012FA8_13BA8, &arg0->iconEntries[i]);

        func_80035DE0_369E0(
            arg0->textRenderAsset,
            arg0->labelEntries[i].textData,
            arg0->labelEntries[i].x,
            arg0->labelEntries[i].y,
            arg0->labelEntries[i].highlight.asBytes[1],
            arg0->labelEntries[i].textStyle,
            (void *)8,
            1
        );
    }
}

void cleanupOptionsMenuToggles(OptionsMenuTogglesCleanupArg *arg0) {
    arg0->unkD8 = freeNodeMemory(arg0->unkD8);
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void initOptionsMenuLabels(OptionsMenuLabelsState *arg0) {
    void *spriteAsset;
    s32 i;
    s32 minusE0;
    s16 temp;
    s16 iconX;
    s16 initialAlpha;
    s16 textX;
    s16 textAlpha;
    u8 textStyle;

    getCurrentAllocation();
    spriteAsset = loadCompressedData(&_4196E0_ROM_START, &_4196E0_ROM_END, 0xBB8);
    arg0->textRenderAsset = loadTextRenderAsset(1);
    setCleanupCallback(cleanupOptionsMenuLabels);

    i = 0;
    iconX = -0x80;
    initialAlpha = 1;
    textX = -0x7C;
    textAlpha = 0xFF;
    textStyle = 5;
    for (i = 0; i < 4; i++) {
        arg0->iconEntries[i].x = iconX;
        arg0->iconEntries[i].y = -0x20 + 0x20 * i;
        arg0->iconEntries[i].spriteAsset = spriteAsset;
        arg0->iconEntries[i].frameIndex = 0;
        arg0->iconEntries[i].highlightValue = 0;
        arg0->iconEntries[i].unkC = 0;
        arg0->iconEntries[i].alpha = initialAlpha;

        arg0->textEntries[i].x = textX;
        arg0->textEntries[i].y = arg0->iconEntries[i].y;
        arg0->textEntries[i].textData = optionsMenuLabelTextData[i];
        arg0->textEntries[i].textAsset = arg0->textRenderAsset;
        arg0->textEntries[i].highlight = 0;
        arg0->textEntries[i].alpha = textAlpha;
        arg0->textEntries[i].textStyle = textStyle;
    }

    arg0->iconEntries[3].x = -0x44;
    arg0->iconEntries[3].y = 0x48;
    arg0->textEntries[3].x = -0x40;
    arg0->textEntries[3].y = arg0->iconEntries[3].y + 2;

    setCallback(updateOptionsMenuLabels);
}

void updateOptionsMenuLabels(OptionsMenuLabelsState *arg0) {
    func_80037F14_alloc *alloc;
    s32 i;
    do {
        alloc = (func_80037F14_alloc *)getCurrentAllocation();

        for (i = 0; i < 4; i++) {
            if (alloc->unk1E2 == 0) {
                if (alloc->unk1EC == i) {
                    arg0->iconEntries[i].highlightValue = alloc->unk1E8[i];
                    arg0->textEntries[i].highlight = alloc->unk1E8[i];
                } else {
                    arg0->iconEntries[i].highlightValue = 0;
                    arg0->textEntries[i].highlight = 0;
                }
            } else {
                if (alloc->unk1E2 == 1 && alloc->unk1EC == i && (alloc->unk1E0 & 1)) {
                    arg0->iconEntries[i].alpha = 0xFF;
                } else {
                    arg0->iconEntries[i].alpha = 1;
                }
                arg0->iconEntries[i].highlightValue = 0;
                arg0->textEntries[i].highlight = 0;
            }

            debugEnqueueCallback(8, 0, func_80012FA8_13BA8, &arg0->iconEntries[i]);

            func_80035DE0_369E0(
                arg0->textRenderAsset,
                arg0->textEntries[i].textData,
                arg0->textEntries[i].x,
                arg0->textEntries[i].y,
                (u8)arg0->textEntries[i].highlight,
                arg0->textEntries[i].textStyle,
                (void *)8,
                1
            );
        }
    } while (0);
}

void cleanupOptionsMenuLabels(OptionsMenuLabelsCleanupArg *arg0) {
    arg0->unk48 = freeNodeMemory(arg0->unk48);
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_80037E78_38A78(OptionsMenuLabelIconEntry *arg0) {
    void *allocation;
    s32 i;
    s32 minus32;
    s32 val;

    getCurrentAllocation();
    allocation = loadCompressedData(&_4237C0_ROM_START, &_426EF0_ROM_START, 0x8A08);
    setCleanupCallback(func_80037FB0_38BB0);

    i = 0;
    minus32 = -32;
    val = 8;
    do {
        arg0[i].x = val;
        arg0[i].y = minus32;
        arg0[i].spriteAsset = allocation;
        arg0[i].frameIndex = i;
        arg0[i].highlightValue = 0;
        arg0[i].alpha = 0;
        arg0[i].unkC = 0;
        i++;
        val += 0x78;
    } while (i < 2);

    setCallback(func_80037F14_38B14);
}

void func_80037F14_38B14(OptionsMenuLabelIconEntry *arg0) {
    func_80037F14_alloc *alloc = getCurrentAllocation();
    s32 i;

    for (i = 0; i < 2; i++) {
        if (alloc->unk1E2 != 0) {
            continue;
        }
        if (alloc->unk1EC >= 3) {
            continue;
        }
        arg0[i].y = (alloc->unk1EC * 32) - 32;
        arg0[i].highlightValue = alloc->unk1E8[alloc->unk1EC];
        debugEnqueueCallback(8, 0, func_80012FA8_13BA8, &arg0[i]);
    }
}

void func_80037FB0_38BB0(func_80037FB0_38BB0_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}
