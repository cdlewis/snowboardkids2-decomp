#include "ui/options_menu.h"
#include "assets.h"
#include "common.h"
#include "font_encoding.h"
#include "gamestate.h"
#include "graphics/graphics.h"
#include "graphics/sprite_rdp.h"
#include "system/task_scheduler.h"
#include "text/font_render.h"
#include "text/text_layout.h"

u8 optionsMenuTitleTextData[] = { _("   You can change the\nrules for battle mode."), PAD2 };

u8 coinsToggleYesText[] = { _("   Yes"), PAD2 };

u8 coinsToggleNoText[] = { _("    No"), PAD2 };

u8 lapsToggleYesText[] = { _("   Yes"), PAD2 };

u8 lapsToggleNoText[] = { _("    No"), PAD2 };

u8 coinsOnCourseLabelText[] = { _(" Coins on course@") };

u8 shopsOnCourseLabelText[] = { _(" Shops on course@") };

u8 changeLapsLabelText[] = { _(" Change laps@") };

u8 toTitleScreenLabelText[] = { _(" To title screen.") };

void *optionsMenuLabelTextData[] = {
    coinsOnCourseLabelText, shopsOnCourseLabelText, changeLapsLabelText, toTitleScreenLabelText, NULL, NULL, NULL,
    (void *)0x4E534B4A,     (void *)0x45420000,     (void *)0x5C688F56,  (void *)0x4A830000
};

void cleanupOptionsMenuTitle(OptionsMenuTitleState *arg0);
void updateOptionsMenuTitle(OptionsMenuTitleState *arg0);
void cleanupOptionsMenuToggles(OptionsMenuToggleState *arg0);
void updateOptionsMenuToggles(OptionsMenuToggleState *arg0);
void cleanupOptionsMenuLabels(OptionsMenuLabelsState *arg0);
void updateOptionsMenuLabels(OptionsMenuLabelsState *arg0);
void cleanupOptionsMenuCursors(TextRenderArg *arg0);
void updateOptionsMenuCursors(TextRenderArg *arg0);

void initOptionsMenuTitle(OptionsMenuTitleState *arg0) {
    void *textAsset;
    void *spriteAsset;

    textAsset = loadTextRenderAsset(1);
    spriteAsset = loadCompressedData(&optionsMenuSprites_ROM_START, &optionsMenuSprites_ROM_END, 0xBB8);
    setCleanupCallback(cleanupOptionsMenuTitle);

    arg0->titleText.startX = -0x50;
    arg0->titleText.startY = -0x58;
    arg0->titleText.textData = (u16 *)optionsMenuTitleTextData;
    arg0->titleText.fontAsset = textAsset;
    arg0->titleText.shade.value = 0xFF;
    arg0->titleText.textAlpha.value = 0xFF;
    arg0->titleText.paletteIndex = 0;

    arg0->leftIcon.x = -0x90;
    arg0->leftIcon.y = -0x60;
    arg0->leftIcon.spriteData = spriteAsset;
    arg0->leftIcon.frameIndex = 2;
    arg0->leftIcon.color.paletteAndAlpha = 0xFF;
    arg0->leftIcon.tileMode = 1;
    arg0->leftIcon.overridePaletteCount = 0;

    arg0->rightIcon.x = 0x58;
    arg0->rightIcon.y = -0x60;
    arg0->rightIcon.spriteData = spriteAsset;
    arg0->rightIcon.frameIndex = 2;
    arg0->rightIcon.color.paletteAndAlpha = 0xFF;
    arg0->rightIcon.tileMode = 0;
    arg0->rightIcon.overridePaletteCount = 0;

    setCallback(updateOptionsMenuTitle);
}

void updateOptionsMenuTitle(OptionsMenuTitleState *arg0) {
    s32 i;

    enqueueCallbackBySlotIndex(8, 0, renderTextLayout, arg0);

    for (i = 0; i < 2; i++) {
        enqueueCallbackBySlotIndex(8, 0, renderTextSprite, &(&arg0->leftIcon)[i]);
        /* Keep the containing state live so KMC preserves the target base-plus-field-offset addressing. */
        __asm__ volatile("" : : "r"(arg0));
    }
}

void cleanupOptionsMenuTitle(OptionsMenuTitleState *arg0) {
    arg0->titleText.fontAsset = freeNodeMemory(arg0->titleText.fontAsset);
    arg0->leftIcon.spriteData = freeNodeMemory(arg0->leftIcon.spriteData);
}

void initOptionsMenuToggles(OptionsMenuToggleState *arg0) {
    void *spriteAsset;
    s32 i;
    s32 column;
    u8 optionValue;

    getCurrentAllocation();
    spriteAsset = loadCompressedData(&optionsMenuSprites_ROM_START, &uiCornerSprites_ROM_START, 0xBB8);
    arg0->textRenderAsset = loadTextRenderAsset(1);
    setCleanupCallback(cleanupOptionsMenuToggles);

    for (i = 0; i < 6; i++) {
        column = i & 1;
        arg0->toggleIcons[i].x = (column * 0x38) + 0x18;
        arg0->toggleIcons[i].y = ((i / 2) << 5) - 0x20;
        arg0->toggleIcons[i].spriteData = spriteAsset;
        arg0->toggleIcons[i].frameIndex = 1;
        arg0->toggleIcons[i].color.paletteAndAlpha = 0;
        arg0->toggleIcons[i].tileMode = 0;

        if (i < 2) {
            optionValue = gGameSessionContext->battleTimeLimit;
        } else if (i < 4) {
            optionValue = gGameSessionContext->battleScoreLimit;
        } else {
            optionValue = (gGameSessionContext->customLapEnabled + 1) & 1;
        }
        column = i & 1;
        arg0->toggleIcons[i].overridePaletteCount = ((optionValue + column) & 1) | 2;

        arg0->toggleLabels[i].startX = (column * 0x3A) + 0x16;
        arg0->toggleLabels[i].startY = arg0->toggleIcons[i].y + 2;
        arg0->toggleLabels[i].fontAsset = arg0->textRenderAsset;
        arg0->toggleLabels[i].shade.value = 0;
        arg0->toggleLabels[i].textAlpha.value = 0xFF;
        arg0->toggleLabels[i].paletteIndex = 5;

        if (i < 4) {
            if (column != 0) {
                arg0->toggleLabels[i].textData = (u16 *)&coinsToggleNoText;
            } else {
                arg0->toggleLabels[i].textData = (u16 *)&coinsToggleYesText;
            }
        } else {
            if (column != 0) {
                arg0->toggleLabels[i].textData = (u16 *)&lapsToggleNoText;
            } else {
                arg0->toggleLabels[i].textData = (u16 *)&lapsToggleYesText;
            }
        }
    }

    setCallback(updateOptionsMenuToggles);
}

void updateOptionsMenuToggles(OptionsMenuToggleState *arg0) {
    OptionsMenuState *alloc;
    s32 i;
    u8 optionValue;

    alloc = getCurrentAllocation();

    for (i = 0; i < 6; i++) {
        if (i < 2) {
            optionValue = gGameSessionContext->battleTimeLimit;
        } else if (i < 4) {
            optionValue = gGameSessionContext->battleScoreLimit;
        } else {
            optionValue = (gGameSessionContext->customLapEnabled + 1) & 1;
        }

        arg0->toggleIcons[i].overridePaletteCount = ((optionValue + (i & 1)) & 1) | 2;

        if (alloc->phase == OPTIONS_MENU_SELECTING) {
            u8 idx;
            s32 value;
            idx = alloc->selectedOption;
            value = optionValue + (idx << 1);
            if (value == i) {
                arg0->toggleIcons[i].color.paletteAndAlpha = alloc->highlightAlphas[idx];
                arg0->toggleLabels[i].shade.value = alloc->highlightAlphas[alloc->selectedOption];
            } else {
                arg0->toggleIcons[i].color.paletteAndAlpha = 0;
                arg0->toggleLabels[i].shade.value = 0;
            }
        } else {
            arg0->toggleIcons[i].color.paletteAndAlpha = 0;
            arg0->toggleLabels[i].shade.value = 0;
        }

        enqueueCallbackBySlotIndex(8, 0, renderAlphaBlendedTextSprite, &arg0->toggleIcons[i]);

        enqueueTextLayoutAlphaBlended(
            arg0->textRenderAsset,
            arg0->toggleLabels[i].textData,
            arg0->toggleLabels[i].startX,
            arg0->toggleLabels[i].startY,
            arg0->toggleLabels[i].shade.bytes.low,
            arg0->toggleLabels[i].paletteIndex,
            8,
            1
        );
    }
}

void cleanupOptionsMenuToggles(OptionsMenuToggleState *arg0) {
    arg0->textRenderAsset = freeNodeMemory(arg0->textRenderAsset);
    arg0->toggleIcons[0].spriteData = freeNodeMemory(arg0->toggleIcons[0].spriteData);
}

void initOptionsMenuLabels(OptionsMenuLabelsState *arg0) {
    void *spriteAsset;
    s32 i;
    s32 minusE0;
    s16 temp;
    s16 iconX;
    s16 initialPaletteOverride;
    s16 textX;
    s16 textAlpha;
    u8 paletteIndex;

    getCurrentAllocation();
    spriteAsset = loadCompressedData(&optionsMenuSprites_ROM_START, &optionsMenuSprites_ROM_END, 0xBB8);
    arg0->textRenderAsset = loadTextRenderAsset(1);
    setCleanupCallback(cleanupOptionsMenuLabels);

    i = 0;
    iconX = -0x80;
    initialPaletteOverride = 1;
    textX = -0x7C;
    textAlpha = 0xFF;
    paletteIndex = 5;
    for (i = 0; i < 4; i++) {
        arg0->optionIcons[i].x = iconX;
        arg0->optionIcons[i].y = -0x20 + 0x20 * i;
        arg0->optionIcons[i].spriteData = spriteAsset;
        arg0->optionIcons[i].frameIndex = 0;
        arg0->optionIcons[i].color.paletteAndAlpha = 0;
        arg0->optionIcons[i].tileMode = 0;
        arg0->optionIcons[i].overridePaletteCount = initialPaletteOverride;

        arg0->optionLabels[i].startX = textX;
        arg0->optionLabels[i].startY = arg0->optionIcons[i].y;
        arg0->optionLabels[i].textData = optionsMenuLabelTextData[i];
        arg0->optionLabels[i].fontAsset = arg0->textRenderAsset;
        arg0->optionLabels[i].shade.value = 0;
        arg0->optionLabels[i].textAlpha.value = textAlpha;
        arg0->optionLabels[i].paletteIndex = paletteIndex;
    }

    arg0->optionIcons[3].x = -0x44;
    arg0->optionIcons[3].y = 0x48;
    arg0->optionLabels[3].startX = -0x40;
    arg0->optionLabels[3].startY = arg0->optionIcons[3].y + 2;

    setCallback(updateOptionsMenuLabels);
}

void updateOptionsMenuLabels(OptionsMenuLabelsState *arg0) {
    OptionsMenuState *alloc;
    s32 i;
    do {
        alloc = (OptionsMenuState *)getCurrentAllocation();

        for (i = 0; i < 4; i++) {
            if (alloc->phase == OPTIONS_MENU_SELECTING) {
                if (alloc->selectedOption == i) {
                    arg0->optionIcons[i].color.paletteAndAlpha = alloc->highlightAlphas[i];
                    arg0->optionLabels[i].shade.value = alloc->highlightAlphas[i];
                } else {
                    arg0->optionIcons[i].color.paletteAndAlpha = 0;
                    arg0->optionLabels[i].shade.value = 0;
                }
            } else {
                if (alloc->phase == OPTIONS_MENU_EXIT_DELAY && alloc->selectedOption == i &&
                    (alloc->exitBlinkTimer & 1)) {
                    arg0->optionIcons[i].overridePaletteCount = 0xFF;
                } else {
                    arg0->optionIcons[i].overridePaletteCount = 1;
                }
                arg0->optionIcons[i].color.paletteAndAlpha = 0;
                arg0->optionLabels[i].shade.value = 0;
            }

            enqueueCallbackBySlotIndex(8, 0, renderAlphaBlendedTextSprite, &arg0->optionIcons[i]);

            enqueueTextLayoutAlphaBlended(
                arg0->textRenderAsset,
                arg0->optionLabels[i].textData,
                arg0->optionLabels[i].startX,
                arg0->optionLabels[i].startY,
                (u8)arg0->optionLabels[i].shade.value,
                arg0->optionLabels[i].paletteIndex,
                8,
                1
            );
        }
    } while (0);
}

void cleanupOptionsMenuLabels(OptionsMenuLabelsState *arg0) {
    arg0->optionLabels[0].fontAsset = freeNodeMemory(arg0->optionLabels[0].fontAsset);
    arg0->optionIcons[0].spriteData = freeNodeMemory(arg0->optionIcons[0].spriteData);
}

void initOptionsMenuCursors(TextRenderArg *arg0) {
    void *cursorSpriteAsset;
    s32 i;
    s32 initialY;
    s32 xPos;

    getCurrentAllocation();
    cursorSpriteAsset = loadCompressedData(&menuUiSprites_ROM_START, &playerCountSelectSprites_ROM_START, 0x8A08);
    setCleanupCallback(cleanupOptionsMenuCursors);

    i = 0;
    initialY = -32;
    xPos = 8;
    do {
        arg0[i].x = xPos;
        arg0[i].y = initialY;
        arg0[i].spriteData = cursorSpriteAsset;
        arg0[i].frameIndex = i;
        arg0[i].color.paletteAndAlpha = 0;
        arg0[i].overridePaletteCount = 0;
        arg0[i].tileMode = 0;
        i++;
        xPos += 0x78;
    } while (i < 2);

    setCallback(updateOptionsMenuCursors);
}

void updateOptionsMenuCursors(TextRenderArg *arg0) {
    OptionsMenuState *alloc = getCurrentAllocation();
    s32 i;

    for (i = 0; i < 2; i++) {
        if (alloc->phase != OPTIONS_MENU_SELECTING) {
            continue;
        }
        if (alloc->selectedOption >= 3) {
            continue;
        }
        arg0[i].y = (alloc->selectedOption * 32) - 32;
        arg0[i].color.paletteAndAlpha = alloc->highlightAlphas[alloc->selectedOption];
        enqueueCallbackBySlotIndex(8, 0, renderAlphaBlendedTextSprite, &arg0[i]);
    }
}

void cleanupOptionsMenuCursors(TextRenderArg *arg0) {
    arg0->spriteData = freeNodeMemory(arg0->spriteData);
}
