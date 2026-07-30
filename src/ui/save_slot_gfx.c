#include "ui/save_slot_gfx.h"
#include "animation/easing_state.h"
#include "assets.h"
#include "common.h"
#include "data/course_data.h"
#include "data/global_frame_counter.h"
#include "font_encoding.h"
#include "graphics/clip_text_render.h"
#include "graphics/graphics.h"
#include "graphics/sprite_rdp.h"
#include "race/race_session.h"
#include "story/shop_ui.h"
#include "system/task_scheduler.h"
#include "text/font_assets.h"
#include "text/font_render.h"
#include "text/text_layout.h"

u16 gSaveSlotStatSpriteTable[] = {
    0x0000, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0002, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0000, 0x0000,
};

u16 gSaveSlotNameEntryGridTable[] = {
    0x0006, 0x0001, 0x0000, 0x0007, 0x0009, 0x0000, 0x0006, 0x0001, 0x0001, 0x0008, 0x0001, 0x0000, 0x0009,
    0x0009, 0x0000, 0x0008, 0x0001, 0x0001, 0x0008, 0x0001, 0x0000, 0x0009, 0x0009, 0x0000, 0x0008, 0x0001,
    0x0001, 0x0008, 0x0001, 0x0000, 0x0009, 0x0009, 0x0000, 0x0008, 0x0001, 0x0001, 0x0006, 0x0001, 0x0002,
    0x0007, 0x0009, 0x0002, 0x0006, 0x0001, 0x0003, 0xFFFF, 0xFFFF, 0xFFFF, 0xFF88, 0xFF90, 0xFF90, 0x0000,
    0x0000, 0x0000, 0xFF88, 0xFFB0, 0xFF88, 0xFFA8, 0xFF88, 0xFF88, 0xFFA0, 0xFFB0, 0xFFA0, 0x0000,
};

u16 gSaveSlotSelectionParticleXOffsets[] = { 0xFFFC, 0x0003, 0xFFFD, 0x0002, 0x0002, 0xFFFE };

u16 gSaveSlotSelectionParticleYOffsets[] = { 0xFFEC, 0xFFF4, 0x0000, 0x0008, 0x000E, 0x0014 };

s16 gSaveSlotDeleteArrowAnimFrames[] = { 0x000A, 0x000B, 0x000C, 0x000B };

u8 gSaveSlotLoadSavedDataStatusText[] = { _("Load saved data@") };

u8 gSaveSlotFinishedLoadingStatusText[] = { _("Finished loading.") };

u8 gSaveSlotStartingNewGameStatusText[] = { _("Starting new game.") };

u8 gSaveSlotChooseLoadFilePromptText[] = { _("Which file will you load@") };

u8 gSaveSlotChooseSaveFilePromptText[] = { _("Which file will you save in@") };

u8 gSaveSlotQuitGamePromptText[] = { _("Quit game@") };

u8 gSaveSlotFinishedSavingPromptText[] = { _("Finished saving.") };

u8 gSaveSlotSaveAvailablePromptText[] = { _("You can save the data.") };

u8 gSaveSlotUnableToSavePromptText[] = { _("{0xFFFC}{0x0001}Unable to save.") };

u8 gSaveSlotNoDataNameText[] = { _("NO DATA") };

u8 gRumblePakAttachPromptText[] = {
    _("{0xA012}f you are using\nthe Rumble Pak, \nplease set it on\nthe controller."),
};

u8 gRumblePakRemovalWarningText[] = { _("Please do not remove\nthe Rumble Pak\nduring gameplay.") };

u8 gSaveSlotStartGameOptionText[] = { _("Start game.") };

u8 gSaveSlotLoadSavedDataOptionText[] = { _("Load saved data.") };

u8 gSaveSlotTitleScreenOptionText[] = { _("To title screen.") };

u8 gSaveSlotSaveDataOptionText[] = { _("Save Data.") };

u8 gSaveSlotReturnToTownOptionText[] = { _("Return to town.") };

u8 gSaveSlotQuitGameOptionText[] = { _("Quit game.") };

u8 gSaveSlotMenuHeaderText[] = { _("~ MENU ~") };

u8 gSaveSlotBackupMemoryCorruptedStartAbortedText[] = { _("Backup memory\nis corrupted.\nGame start aborted.") };

u8 gSaveSlotBackupMemoryCorruptedSaveLoadAbortedText[] = {
    _("Backup memory is\ncorrupt. Save/load\naborted."),
    PAD2,
    PAD2,
    PAD2,
    PAD2,
    PAD2,
    PAD2,
    PAD2,
    PAD2,
    PAD2,
    PAD2,
    0xFF,
    0xFF,
};

u8 gSaveSlotExpertModeOptionText[] = { _("Expert") };

u8 gSaveSlotNormalModeOptionText[] = { _("Normal") };

u8 gSaveSlotChooseModePromptText[] = { _("Which mode do you want to play@") };

u8 gSaveSlotExpertModeDescriptionText[] = { _("Play in the Expert mode.") };

u8 gSaveSlotNormalModeDescriptionText[] = { _("Play in the Normal mode.") };

void *gSaveSlotMenuOptionTexts[] = {
    gSaveSlotStartGameOptionText,
    gSaveSlotLoadSavedDataOptionText,
    gSaveSlotTitleScreenOptionText,
    gSaveSlotSaveDataOptionText,
    gSaveSlotReturnToTownOptionText,
    gSaveSlotQuitGameOptionText,
    gSaveSlotSaveDataOptionText,
    gSaveSlotQuitGameOptionText,
    gSaveSlotBackupMemoryCorruptedStartAbortedText,
    gSaveSlotBackupMemoryCorruptedSaveLoadAbortedText,
};

void *gSaveSlotModeOptionTexts[] = {
    gSaveSlotExpertModeOptionText,
    gSaveSlotNormalModeOptionText,
};

void *gSaveSlotMainPromptTexts[] = {
    gSaveSlotLoadSavedDataStatusText,
    gSaveSlotFinishedLoadingStatusText,
    gSaveSlotStartingNewGameStatusText,
};

s32 gSaveSlotNoDataNameTextPtr = (s32)gSaveSlotNoDataNameText;

void *gRumblePakAttachPromptTextPtr = gRumblePakAttachPromptText;

void *gRumblePakRemovalWarningTextPtr = gRumblePakRemovalWarningText;

void *gSaveSlotDialogPromptTexts[] = {
    gSaveSlotChooseSaveFilePromptText,
    gSaveSlotQuitGamePromptText,
    gSaveSlotFinishedSavingPromptText,
    gSaveSlotSaveAvailablePromptText,
    gSaveSlotChooseLoadFilePromptText,
    gSaveSlotChooseModePromptText,
    gSaveSlotExpertModeDescriptionText,
    gSaveSlotNormalModeDescriptionText,
    gSaveSlotUnableToSavePromptText,
    NULL,
    NULL,
};

void updateSaveSlotNumberLabels(SaveSlotNumberLabelsState *arg0);
void cleanupSaveSlotNumberLabels(SaveSlotNumberLabelsState *);
void updateSaveSlotDeleteText(SaveSlotDeleteTextState *);
void updateSaveSlotNameText(SaveSlotNameTextState *arg0);
void cleanupSaveSlotNameText(SaveSlotNameTextState *arg0);
void cleanupSaveSlotGoldDisplay(SaveSlotGoldDisplayState *arg0);
void updateSaveSlotGoldDisplay(SaveSlotGoldDisplayState *);
void updateSaveSlotSelectionParticles(SaveSlotSelectionParticlesState *arg0);
void cleanupSaveSlotSelectionParticles(SaveSlotSelectionParticlesState *arg0);
void updateSaveSlotStatSprites(SaveSlotStatSpritesState *arg0);
void cleanupSaveSlotStatSprites(SaveSlotStatSpritesState *arg0);
void updateSaveSlotItemIcons(SaveSlotItemIconsState *);
void cleanupSaveSlotDeleteText(SaveSlotDeleteTextState *arg0);
void cleanupSaveSlotItemIcons(SaveSlotItemIconsState *arg0);
void renderSaveSlotConfirmationIndicator(void *arg0);
void cleanupSaveSlotConfirmationIndicator(SaveSlotConfirmationIndicatorState *arg0);
void updateSaveSlotDeleteArrow(SaveSlotDeleteArrowState *state);
void cleanupSaveSlotDeleteArrow(SaveSlotDeleteArrowState *arg0);
void cleanupSaveSlotPromptText(SaveSlotPromptTextState *arg0);
void cleanupSaveSlotNameEntryGrid(SaveSlotGridState *arg0);
void updateSaveSlotNameEntryGrid(SaveSlotGridState *arg0);
void updateSaveSlotPromptText(SaveSlotPromptTextState *arg0);

void initSaveSlotStatSprites(SaveSlotStatSpritesState *state) {
    void *spriteSheet;
    s32 i;

    getCurrentAllocation();
    spriteSheet = loadCompressedData(&uiFontSpriteSheet_ROM_START, &uiFontSpriteSheet_ROM_END, 0x2278);
    setCleanupCallback(cleanupSaveSlotStatSprites);

    for (i = 12; i >= 0; i--) {
        state->entries[i].spriteData = spriteSheet;
    }

    setCallback(updateSaveSlotStatSprites);
}

void updateSaveSlotStatSprites(SaveSlotStatSpritesState *arg0) {
    SaveSlotScreenState *allocation;
    s16 x;
    s16 alpha;
    s32 step;
    s32 halfStep;
    s32 gap;
    s32 i;
    u16 *baseTable;
    s32 three;
    u8 isSpecial;
    void *callbackEntry;

    allocation = getCurrentAllocation();
    step = 0xF;

    if (allocation->saveSlotMenuState < 0x32) {
        if (allocation->selectedSaveSlot == arg0->slotIndex || arg0->slotIndex == 3) {
            step = 0x10;
            gap = 0x48;
            x = -0x90;
            alpha = 0xFF;
        } else {
            goto notSelected;
        }
    } else {
    notSelected:
        gap = 0x48;
        x = -0x8A;
        alpha = 0x60;
    }

    i = 0;
    if (allocation->saveSlotMenuState == 0) {
        step = 0x10;
        gap = 0x48;
        x = -0x90;
        alpha = 0xFF;
    }

    halfStep = step / 2;
    three = 3;
    baseTable = gSaveSlotStatSpriteTable;

    do {
        if (i == 7) {
            x += gap;
        } else {
            x += step;
        }

        isSpecial = 0;
        if (i == 6) {
            x -= halfStep;
            arg0->entries[6].frameIndex = baseTable[6] + arg0->slotIndex;
            if (arg0->slotIndex == three) {
                arg0->entries[6].frameIndex = baseTable[6] + allocation->selectedSaveSlot;
            }
            isSpecial = 1;
        } else {
            arg0->entries[i].frameIndex = baseTable[i];
        }

        arg0->entries[i].x = x;
        arg0->entries[i].y = -0x18;
        arg0->entries[i].overridePaletteCount = arg0->slotIndex + 1;
        if (arg0->slotIndex == three) {
            arg0->entries[i].overridePaletteCount = allocation->selectedSaveSlot + 1;
        }
        arg0->entries[i].color.paletteAndAlpha = alpha;
        if (i >= 7) {
            arg0->entries[i].tileMode = 1;
        } else {
            arg0->entries[i].tileMode = 0;
        }

        if (arg0->slotIndex == allocation->selectedSaveSlot && allocation->saveSlotMenuState == 2 &&
            (allocation->selectionAnimState & 1)) {
            arg0->entries[i].overridePaletteCount = 0xFF;
        }
        callbackEntry = &arg0->entries[i];

        i++;
        enqueueCallbackBySlotIndex(arg0->slotIndex + 9, isSpecial, renderTextSprite, callbackEntry);
    } while (i < 13);
}

void cleanupSaveSlotStatSprites(SaveSlotStatSpritesState *arg0) {
    arg0->entries[0].spriteData = freeNodeMemory(arg0->entries[0].spriteData);
}

void initSaveSlotNameEntryGrid(SaveSlotGridState *state) {
    SaveSlotScreenState *allocation;
    void *spriteSheet;
    s32 i;
    s32 entryIndex;
    s32 j;
    s32 spriteIndex;
    s32 k;

    allocation = getCurrentAllocation();
    spriteSheet = loadCompressedData(&uiFontSpriteSheet_ROM_START, &font_main_ROM_START, 0x2278);
    setCleanupCallback(cleanupSaveSlotNameEntryGrid);
    state->entries = allocateNodeMemory(0x370);

    for (i = 0; i < 0x37; i++) {
        state->entries[i].spriteData = spriteSheet;
        state->entries[i].color.paletteAndAlpha = 0xFF;
        state->entries[i].overridePaletteCount = 0;
    }

    entryIndex = 0;
    for (k = 0; gSaveSlotNameEntryGridTable[k] != 0xFFFF; k += 3) {
        spriteIndex = gSaveSlotNameEntryGridTable[k];
        for (j = 0; j < gSaveSlotNameEntryGridTable[k + 1]; j++) {
            state->entries[(u16)entryIndex].frameIndex = spriteIndex;
            state->entries[(u16)entryIndex].tileMode = gSaveSlotNameEntryGridTable[k + 2];
            entryIndex++;
        }
    }

    state->cursorSprite.x = allocation->nameEntryGridX + 0x98;
    state->cursorSprite.y = allocation->nameEntryCursorY + 0x38;
    state->cursorSprite.spriteData = spriteSheet;
    state->cursorSprite.frameIndex = 0x13;
    setCallback(updateSaveSlotNameEntryGrid);
}

void updateSaveSlotNameEntryGrid(SaveSlotGridState *arg0) {
    SaveSlotScreenState *allocation;
    s32 row;
    s32 rowIndex;
    s32 baseIndex;
    s32 col;
    s32 colIndex;
    s32 rowStartIndex;
    s32 entryIndex;
    u16 screenState;
    u16 animFrame;

    allocation = getCurrentAllocation();

    row = 0;
    rowIndex = 0;
    baseIndex = 0;

    do {
        for (col = 0, colIndex = 0, rowStartIndex = baseIndex; colIndex < 0xB; colIndex++, col += 0x10) {
            entryIndex = rowStartIndex + colIndex;
            arg0->entries[entryIndex].x = allocation->nameEntryGridX + col;
            arg0->entries[entryIndex].y = allocation->nameEntryCursorY + row;
            enqueueCallbackBySlotIndex(8U, 0U, renderTextSprite, &arg0->entries[entryIndex]);
        }

        row += 0x10;
        rowIndex += 1;
        baseIndex += 0xB;
    } while (rowIndex < 5);

    screenState = allocation->saveSlotMenuState;
    if ((screenState == 8) | (screenState == 0xB)) {
        arg0->cursorSprite.y = allocation->nameEntryCursorBaseY + 0x38;
    } else {
        arg0->cursorSprite.y = allocation->nameEntryCursorY + 0x38;
    }

    if (!(gGlobalFrameCounter & 7)) {
        animFrame = arg0->cursorSprite.frameIndex + 1;
        arg0->cursorSprite.frameIndex = animFrame;
        if ((u32)(animFrame & 0xFFFF) >= 0x15U) {
            arg0->cursorSprite.frameIndex = 0x13U;
        }
    }

    enqueueCallbackBySlotIndex(8U, 1U, renderSpriteFrame, &arg0->cursorSprite);
}

void cleanupSaveSlotNameEntryGrid(SaveSlotGridState *arg0) {
    arg0->cursorSprite.spriteData = freeNodeMemory(arg0->cursorSprite.spriteData);
    arg0->entries = freeNodeMemory(arg0->entries);
}

void initSaveSlotItemIcons(SaveSlotItemIconsState *arg0) {
    SaveSlotScreenState *allocation;
    s32 i;
    s16 yPos;

    allocation = getCurrentAllocation();
    arg0->icons = (TextRenderArg *)allocateNodeMemory(0xF0);
    arg0->spriteSheet = loadCompressedData(&uiFontSpriteSheet_ROM_START, &uiFontSpriteSheet_ROM_END, 0x2278);
    arg0->slotHasData = allocation->slotStatus.displaySlotActive[arg0->slotIndex];
    setCleanupCallback(cleanupSaveSlotItemIcons);

    for (i = 0; i < 15; i++) {
        yPos = ((-(allocation->slotData[arg0->slotIndex].save_slot_status[i] != 0)) & (-8)) | 0xFF70;
        arg0->icons[i].x = -0x78 + i * 0x10;
        arg0->icons[i].y = yPos;
        arg0->icons[i].spriteData = arg0->spriteSheet;
        arg0->icons[i].frameIndex = 5;
        arg0->icons[i].color.paletteAndAlpha = 0xFF;
        arg0->icons[i].overridePaletteCount = 0;
        arg0->icons[i].tileMode = 0;
    }

    arg0->animFrame = 0;
    setCallback(updateSaveSlotItemIcons);
}

void updateSaveSlotItemIcons(SaveSlotItemIconsState *arg0) {
    SaveSlotScreenState *allocation;
    u16 alpha;
    s32 alphaCheck;
    s32 i;
    u16 screenState;
    u8 slotIndex;
    u8 animFrame;

    allocation = getCurrentAllocation();
    screenState = allocation->saveSlotMenuState;

    if (screenState >= 0x32) {
        arg0->animFrame = 0;
        alpha = 0x60;
    } else if (screenState == 0) {
        alpha = 0xFF;
    } else {
        if (allocation->selectedSaveSlot == arg0->slotIndex) {
            if (screenState < 2) {
                alpha = 0xFE;
            } else {
                alpha = 0xFF;
                arg0->animFrame = 0;
            }
        } else {
            arg0->animFrame = 0;
            alpha = ((arg0->slotIndex == 3) ? 0xFE : 0) | 0x60;
        }
    }

    i = 0;

    do {
        alphaCheck = alpha & 0xFFFF;
        if (alphaCheck == 0xFE) {
            slotIndex = arg0->slotIndex;
            if (slotIndex != 3) {
                if (allocation->slotData[slotIndex].save_slot_status[i] == 1) {
                    animFrame = arg0->animFrame;
                    if (animFrame < 0x10) {
                        arg0->icons[i].color.paletteAndAlpha = alphaCheck - ((animFrame + 1) * 8);
                    } else if (animFrame != 0x1F) {
                        arg0->icons[i].color.paletteAndAlpha = (animFrame * 8) | 6;
                    } else {
                        arg0->icons[i].color.paletteAndAlpha = alphaCheck;
                    }
                } else {
                    arg0->icons[i].color.paletteAndAlpha = 0x60;
                }
            } else {
                if (allocation->slotData[3].save_slot_status[i] == 1) {
                    arg0->icons[i].color.paletteAndAlpha = alphaCheck;
                } else {
                    arg0->icons[i].color.paletteAndAlpha = 0x60;
                }
            }
        } else {
            arg0->icons[i].color.paletteAndAlpha = alpha;
            if (alphaCheck == 0xFF && allocation->saveSlotMenuState != 2) {
                if (allocation->slotData[arg0->slotIndex].save_slot_status[i] == 1) {
                    arg0->icons[i].color.paletteAndAlpha = 0xFE;
                } else {
                    arg0->icons[i].color.paletteAndAlpha = 0x60;
                }
            } else if (alphaCheck == 0x60) {
                if (allocation->slotData[arg0->slotIndex].save_slot_status[i] != 1) {
                    arg0->icons[i].color.paletteAndAlpha = 0x30;
                }
            }
        }

        if (alphaCheck != 0x60) {
            arg0->icons[i].x = (i * 16) - 0x78;
        } else {
            arg0->icons[i].x = (i * 15) - 0x72;
        }

        arg0->icons[i].overridePaletteCount = 0;

        if (allocation->saveSlotMenuState == 2 && arg0->slotIndex == allocation->selectedSaveSlot &&
            (allocation->selectionAnimState & 1)) {
            arg0->icons[i].overridePaletteCount = 0xFF;
        }

        enqueueCallbackBySlotIndex(arg0->slotIndex + 9, 0, renderTextSprite, &arg0->icons[i]);
        i++;
    } while (i < 15);

    if ((alpha & 0xFFFF) == 0xFE && arg0->slotIndex != 3) {
        arg0->animFrame = (arg0->animFrame + 1) & 0x1F;
    } else {
        arg0->animFrame = 0;
    }
}

void cleanupSaveSlotItemIcons(SaveSlotItemIconsState *arg0) {
    arg0->spriteSheet = freeNodeMemory(arg0->spriteSheet);
    arg0->icons = freeNodeMemory(arg0->icons);
}

void initSaveSlotNameText(SaveSlotNameTextState *arg0) {
    SaveSlotScreenState *allocation;
    s32 temp_c;
    void *temp_10;

    allocation = getCurrentAllocation();
    arg0->textAsset = loadTextRenderAsset(1);
    arg0->slotHasData = allocation->slotStatus.displaySlotActive[arg0->slotIndex];
    setCleanupCallback(cleanupSaveSlotNameText);

    *(volatile s16 *)&arg0->label.startX = -0x26;
    *(volatile s16 *)&arg0->label.startY = -4;

    temp_c = gSaveSlotNoDataNameTextPtr;
    temp_10 = arg0->textAsset;

    arg0->label.shade.value = 0xFF;
    arg0->label.paletteIndex = 0;
    arg0->label.textAlpha.value = 0xFF;
    arg0->isAnimating = 0;

    arg0->label.textData = (u16 *)temp_c;
    arg0->label.fontAsset = temp_10;

    setCallback(updateSaveSlotNameText);
}

void updateSaveSlotNameText(SaveSlotNameTextState *arg0) {
    SaveSlotScreenState *allocation;
    u16 temp;

    allocation = getCurrentAllocation();

    if (arg0->slotIndex == allocation->selectedSaveSlot) {
        goto check_ac6;
    }
    if (arg0->slotIndex != 3) {
        arg0->label.shade.value = 0x60;
        goto end;
    }
check_ac6:
    temp = allocation->saveSlotMenuState;
    if (temp < 0x32) {
        arg0->label.shade.value = 0xFF;
        goto end;
    }
    arg0->label.shade.value = 0x60;

end:
    enqueueCallbackBySlotIndex(arg0->slotIndex + 9, 0, renderTextLayout, &arg0->label);
}

void cleanupSaveSlotNameText(SaveSlotNameTextState *arg0) {
    arg0->textAsset = freeNodeMemory(arg0->textAsset);
}

void initSaveSlotItemLabels(SaveSlotNumberLabelsState *arg0) {
    SaveSlotScreenState *saveData;
    s16 itemY;
    void *spriteSheet;
    s32 i;
    char *numberBuffer;

    saveData = getCurrentAllocation();
    spriteSheet = loadCompressedData(&uiFontSpriteSheet_ROM_START, &uiFontSpriteSheet_ROM_END, 0x2278);
    setCleanupCallback(cleanupSaveSlotNumberLabels);

    for (i = 0; i < 15; i++) {
        itemY = (saveData->slotData[arg0->slotIndex].save_slot_status[i] == 0) ? -0x90 : 0;
        numberBuffer = arg0->textBuffers[i];

        if (i < 9) {
            sprintf(numberBuffer, gIntegerFormatString, i + 1);
            arg0->texts[i].x = -0x70 + i * 0x10;
            arg0->texts[i].y = itemY;
            arg0->texts[i].string = (u8 *)numberBuffer;
            arg0->texts[i].palette = 0;
            *(s16 *)&arg0->texts[i].pad = 0xFF;
        } else {
            arg0->sprites[i - 9].x = -0x78 + i * 0x10;
            arg0->sprites[i - 9].y = itemY;
            arg0->sprites[i - 9].spriteData = spriteSheet;
            arg0->sprites[i - 9].frameIndex = i + 4;
            arg0->sprites[i - 9].color.paletteAndAlpha = 0xFF;
            arg0->sprites[i - 9].overridePaletteCount = 0;
            arg0->sprites[i - 9].tileMode = 0;
        }
    }

    setCallback(updateSaveSlotNumberLabels);
}

void updateSaveSlotNumberLabels(SaveSlotNumberLabelsState *arg0) {
    SaveSlotScreenState *allocation;
    u16 alpha;
    u16 alphaCheck;
    s32 i;
    u16 screenState;

    allocation = getCurrentAllocation();
    screenState = allocation->saveSlotMenuState;

    alpha = 0x60;
    if (screenState < 0x32) {
        alpha = 0xFF;
        if (screenState != 0) {
            if (allocation->selectedSaveSlot == arg0->slotIndex) {
                do {
                    alpha = ((-((screenState < 2) ^ 1)) & 0xFF) | 0xFE;
                } while (0);
            } else {
                alpha = ((-(arg0->slotIndex == 3)) & 0xFE) | 0x60;
            }
        }
    }

    i = 0;
    alphaCheck = alpha;
    do {
        if (i < 9) {
            *(s16 *)&arg0->texts[i].pad = alpha;
        } else {
            arg0->sprites[i - 9].color.paletteAndAlpha = alpha;
        }

        if ((alphaCheck == 0xFF) && (allocation->saveSlotMenuState != 2)) {
            if (allocation->slotData[arg0->slotIndex].save_slot_status[i] == 1) {
                if (i < 9) {
                    *(s16 *)&arg0->texts[i].pad = 0xFE;
                } else {
                    arg0->sprites[i - 9].color.paletteAndAlpha = 0xFE;
                }
            } else if (i < 9) {
                *(s16 *)&arg0->texts[i].pad = 0x60;
            } else {
                arg0->sprites[i - 9].color.paletteAndAlpha = 0x60;
            }
        } else if (alphaCheck == 0x60) {
            if (allocation->slotData[arg0->slotIndex].save_slot_status[i] != 1) {
                if (i < 9) {
                    *(s16 *)&arg0->texts[i].pad = 0x30;
                } else {
                    arg0->sprites[i - 9].color.paletteAndAlpha = 0x30;
                }
            }
        }

        if (alphaCheck != 0x60) {
            if (i < 9) {
                arg0->texts[i].x = (s16)((i * 16) - 0x70);
            } else {
                arg0->sprites[i - 9].x = (i * 16) - 0x78;
            }
        } else if (i < 9) {
            arg0->texts[i].x = (s16)((i * 15) - 0x6A);
        } else {
            arg0->sprites[i - 9].x = (i * 15) - 0x72;
        }

        if (i < 9) {
            arg0->texts[i].palette = 0;
        } else {
            arg0->sprites[i - 9].overridePaletteCount = 0;
        }

        if (((allocation->saveSlotMenuState == 2) && (arg0->slotIndex == allocation->selectedSaveSlot)) &&
            (allocation->selectionAnimState & 1)) {
            if (i < 9) {
                arg0->texts[i].palette = 0xFF;
            } else {
                arg0->sprites[i - 9].overridePaletteCount = 0xFF;
            }
        }

        if (i < 9) {
            enqueueCallbackBySlotIndex(arg0->slotIndex + 9, 1, renderTextColored, &arg0->texts[i]);
        } else {
            enqueueCallbackBySlotIndex(arg0->slotIndex + 9, 1, renderTextSprite, &arg0->sprites[i - 9]);
        }

        i++;
    } while (i < 15);
}

void cleanupSaveSlotNumberLabels(SaveSlotNumberLabelsState *arg0) {
    arg0->sprites[0].spriteData = freeNodeMemory(arg0->sprites[0].spriteData);
}

void initSaveSlotPromptText(SaveSlotPromptTextState *arg0) {
    SaveSlotScreenState *allocation;
    void *textAsset;
    s16 hintX;
    s16 hintY;

    allocation = getCurrentAllocation();
    textAsset = loadTextRenderAsset(1);
    setCleanupCallback(cleanupSaveSlotPromptText);

    arg0->mainText.startX = -0x78;
    arg0->mainText.startY = -0x60;
    arg0->mainText.fontAsset = textAsset;
    arg0->mainText.shade.value = 0xFF;
    arg0->mainText.paletteIndex = 0;
    arg0->mainText.textAlpha.value = 0xFF;
    hintX = allocation->nameEntryGridX + 0xA;
    arg0->hintText.startX = hintX;
    hintY = allocation->nameEntryCursorY + 0xA;
    arg0->hintText.fontAsset = textAsset;
    arg0->hintText.shade.value = 0xFF;
    arg0->hintText.paletteIndex = 4;
    arg0->hintText.textAlpha.value = 0xFF;
    arg0->hintText.startY = hintY;

    setCallback(updateSaveSlotPromptText);
}

void updateSaveSlotPromptText(SaveSlotPromptTextState *arg0) {
    SaveSlotScreenState *alloc;
    void *promptText;
    s16 centeredX;
    u16 screenState;
    u16 screenState2;
    s16 hintY;

    alloc = getCurrentAllocation();

    if ((alloc->saveSlotMenuState < 0x3D) || (alloc->hasCurrentSaveData != 0)) {
        promptText = gSaveSlotMainPromptTexts[alloc->mainPromptIndex];
        arg0->mainText.textData = promptText;
        centeredX = ((0x120 - getMaxLinePixelWidth(promptText)) / 2) - 0x90;
        arg0->mainText.startX = centeredX;
        enqueueTextLayout(
            arg0->mainText.fontAsset,
            arg0->mainText.textData,
            centeredX,
            arg0->mainText.startY,
            arg0->mainText.shade.value,
            arg0->mainText.textAlpha.value,
            arg0->mainText.paletteIndex,
            8,
            0
        );

        screenState = alloc->saveSlotMenuState;
        if (screenState >= 6) {
            if (screenState >= 0xA) {
                arg0->hintText.textData = gRumblePakRemovalWarningTextPtr;
            } else {
                arg0->hintText.textData = gRumblePakAttachPromptTextPtr;
            }

            screenState2 = alloc->saveSlotMenuState;
            if (screenState2 == 8 || screenState2 == 0xB) {
                hintY = alloc->nameEntryCursorBaseY + 0xA;
            } else {
                hintY = alloc->nameEntryCursorY + 0xA;
            }
            arg0->hintText.startY = hintY;

            if ((u32)(alloc->saveSlotMenuState - 9) < 2U) {
                if (alloc->selectionAnimState & 1) {
                    arg0->hintText.paletteIndex = 0xFF;
                } else {
                    arg0->hintText.paletteIndex = 4;
                }
            } else {
                arg0->hintText.paletteIndex = 4;
            }

            enqueueTextLayout(
                arg0->hintText.fontAsset,
                arg0->hintText.textData,
                arg0->hintText.startX,
                arg0->hintText.startY,
                arg0->hintText.shade.value,
                arg0->hintText.textAlpha.value,
                arg0->hintText.paletteIndex,
                8,
                0
            );
        }
    }
}

void cleanupSaveSlotPromptText(SaveSlotPromptTextState *arg0) {
    arg0->mainText.fontAsset = freeNodeMemory(arg0->mainText.fontAsset);
}

void initSaveSlotGoldDisplay(SaveSlotGoldDisplayState *arg0) {
    void *goldIconAsset;
    s32 i;

    goldIconAsset = loadCompressedData(&goldIconSprite_ROM_START, &goldIconSprite_ROM_END, 0x388);
    setCleanupCallback(cleanupSaveSlotGoldDisplay);

    for (i = 0; i < 4; i++) {
        arg0->text[i].x = 0x18;
        arg0->text[i].y = 0xC;
        arg0->text[i].palette = 0;
        *(s16 *)&arg0->text[i].pad = 0xFF;
        arg0->text[i].string = arg0->textBuffers[i];

        arg0->icons[i].x = 0x5C;
        arg0->icons[i].y = 0x10;
        arg0->icons[i].spriteData = goldIconAsset;
        arg0->icons[i].frameIndex = 0;
        arg0->icons[i].renderWidth = 0x555;
        arg0->icons[i].renderHeight = 0x555;
        *(s16 *)&arg0->icons[i].pad0E = 0;
        *(s16 *)&arg0->icons[i].pad0E[2] = 0xFF;
        arg0->icons[i].overridePaletteCount = 0;
        arg0->icons[i].tileMode = 0;

        arg0->animFrames[i] = 0;
    }

    setCallback(updateSaveSlotGoldDisplay);
}

void updateSaveSlotGoldDisplay(SaveSlotGoldDisplayState *state) {
    SaveSlotScreenState *allocation;
    s32 i;

    allocation = getCurrentAllocation();

    for (i = 0; i < 4; i++) {
        if (allocation->slotStatus.displaySlotActive[i] == 0) {
            continue;
        }

        if (allocation->saveSlotMenuState >= 0x32) {
            state->text[i].x = 0x1D;
            *(s16 *)&state->text[i].pad = 0x60;
            state->icons[i].frameIndex = 0;
            *(s16 *)&state->icons[i].pad0E[2] = 0x60;
        } else if (i == allocation->selectedSaveSlot) {
            state->text[i].x = 0x18;
            *(s16 *)&state->text[i].pad = 0xFF;
            *(s16 *)&state->icons[i].pad0E[2] = 0xFF;

            if (allocation->saveSlotMenuState < 2) {
                state->animFrames[i]++;
                if (state->animFrames[i] == 2) {
                    state->animFrames[i] = 0;
                    state->icons[i].frameIndex++;
                    if ((u16)state->icons[i].frameIndex >= 6) {
                        state->icons[i].frameIndex = 0;
                    }
                }
            } else {
                state->icons[i].frameIndex = 0;
                state->icons[i].overridePaletteCount = 0;
                state->text[i].palette = 0;
                if (allocation->saveSlotMenuState == 2 && (allocation->selectionAnimState & 1)) {
                    state->icons[i].overridePaletteCount = 0xFF;
                    state->text[i].palette = 0xFF;
                }
            }
        } else {
            if (i < 3) {
                state->text[i].x = 0x1D;
                *(s16 *)&state->text[i].pad = 0x60;
                state->icons[i].frameIndex = 0;
                *(s16 *)&state->icons[i].pad0E[2] = 0x60;
            } else {
                state->text[i].x = 0x17;
                *(s16 *)&state->text[i].pad = 0xFF;
                state->icons[i].frameIndex = 0;
                *(s16 *)&state->icons[i].pad0E[2] = 0xFF;
            }
            state->animFrames[i] = 0;
        }

        sprintf((char *)&state->textBuffers[i], gGoldFormatString7d, allocation->slotData[i].slotGold);

        if (allocation->saveSlotMenuState != 0x18 || allocation->selectedSaveSlot != i) {
            enqueueCallbackBySlotIndex(i + 9, 7, renderTextColored, &state->text[i]);
        }
        enqueueCallbackBySlotIndex(i + 9, 0, renderScaledShadedSpriteFrame, &state->icons[i]);
    }
}

void cleanupSaveSlotGoldDisplay(SaveSlotGoldDisplayState *arg0) {
    arg0->icons[0].spriteData = freeNodeMemory(arg0->icons[0].spriteData);
}

void initSaveSlotConfirmationIndicator(SaveSlotConfirmationIndicatorState *state) {
    void *asset = loadCompressedData(&okPromptSprites_ROM_START, &okPromptSprites_ROM_END, 0x1B48);

    setCleanupCallback(&cleanupSaveSlotConfirmationIndicator);

    state->x = -0x2C;
    state->y = -0x14;
    state->frameIndex = 0xD;
    state->spriteData = asset;

    setCallback(&renderSaveSlotConfirmationIndicator);
}

void renderSaveSlotConfirmationIndicator(void *arg0) {
    SaveSlotScreenState *allocation;
    u16 val;

    allocation = getCurrentAllocation();
    val = allocation->saveSlotMenuState;

    if (val == 3 || val == 0x3C || val == 0x17) {
        enqueueCallbackBySlotIndex(8, 7, renderSpriteFrame, arg0);
    }
}

void cleanupSaveSlotConfirmationIndicator(SaveSlotConfirmationIndicatorState *arg0) {
    arg0->spriteData = freeNodeMemory(arg0->spriteData);
}

void initSaveSlotSelectionParticles(SaveSlotSelectionParticlesState *state) {
    SaveSlotScreenState *allocation;
    void *snowflakeAsset;
    s32 i;
    s16 baseX;
    s16 xOffset;
    s16 yOffset;

    allocation = getCurrentAllocation();
    snowflakeAsset = loadCompressedData(&snowflakeSprite_ROM_START, &snowflakeSprite_ROM_END, 0x9488);
    setCleanupCallback(cleanupSaveSlotSelectionParticles);

    state->animToggle = 0;

    baseX = allocation->slideOffset - 6;
    xOffset = baseX;
    yOffset = (allocation->selectedSaveSlot * 0x38) - 0x3C;

    if (state->isRightSide == 0) {
        xOffset = -(s16)((baseX << 16) >> 16);
    }

    for (i = 0; i < 4; i++) {
        state->entries[i].x = xOffset + gSaveSlotSelectionParticleXOffsets[i];
        state->entries[i].y = yOffset + gSaveSlotSelectionParticleYOffsets[i];
        state->entries[i].frameIndex = (i % 8) + 0x10;
        state->entries[i].paletteIndex = 0x12;
        state->entries[i].spriteData = snowflakeAsset;
    }

    setCallback(updateSaveSlotSelectionParticles);
}

void updateSaveSlotSelectionParticles(SaveSlotSelectionParticlesState *state) {
    SaveSlotScreenState *allocation;
    unsigned int new_var;
    s16 baseX;
    s32 i;

    allocation = getCurrentAllocation();

    if (allocation->slideOffset != 0) {
        baseX = allocation->slideOffset;
        if (state->isRightSide == 0) {
            new_var = baseX;
            baseX = -new_var;
        }

        state->animToggle = (u8)((state->animToggle + 1) & 1);

        for (i = 0; i < 4; i++) {
            state->entries[i].x = baseX + gSaveSlotSelectionParticleXOffsets[i];

            if (state->animToggle == 0) {
                state->entries[i].frameIndex++;
                if (state->entries[i].frameIndex > 0x17) {
                    state->entries[i].frameIndex = 0x10;
                }
            }

            enqueueCallbackBySlotIndex(8, 0, renderSpriteFrameWithPalette, (void *)&state->entries[i]);
        }
    } else {
        terminateCurrentTask();
    }
}

void cleanupSaveSlotSelectionParticles(SaveSlotSelectionParticlesState *arg0) {
    arg0->entries[0].spriteData = freeNodeMemory(arg0->entries[0].spriteData);
}

void initSaveSlotDeleteText(SaveSlotDeleteTextState *state) {
    SaveSlotDeleteTextState *temp_s0;

    temp_s0 = state;
    getCurrentAllocation();
    temp_s0->spriteAsset = loadAsset_34F7E0();
    temp_s0->textAsset = loadTextRenderAsset(1);
    setCleanupCallback(cleanupSaveSlotDeleteText);
    temp_s0->offsetX = 0;
    temp_s0->offsetY = 0;
    setCallback(updateSaveSlotDeleteText);
}

void updateSaveSlotDeleteText(SaveSlotDeleteTextState *state) {
    SaveSlotScreenState *allocation;
    s16 offsetX;
    s16 offsetY;
    s32 loopCount;
    s32 i;
    s32 alphaArg;
    s32 flagArg;
    s32 temp;

    allocation = getCurrentAllocation();

    if (allocation->saveSlotMenuState < 0x32) {
        return;
    }

    if (allocation->saveSlotDialogType == 0xA) {
        state->offsetX = -(allocation->menuChoiceIndex * 24);
    } else {
        state->offsetX = -(allocation->menuChoiceIndex * 16);
    }

    offsetY = -(allocation->menuChoiceIndex * 8);
    state->offsetY = offsetY;

    renderTiledSprite3x3(
        state->spriteAsset,
        state->offsetX,
        offsetY,
        allocation->menuAnimOffsetX,
        allocation->menuChoiceIndex,
        0,
        0x60,
        0xB0,
        8,
        0
    );

    if ((u32)(allocation->saveSlotMenuState - 0x33) >= 4) {
        return;
    }

    if (allocation->saveSlotDialogType < 8) {
        enqueueTextLayout(state->textAsset, &gSaveSlotMenuHeaderText, -0x24, -0x30, 0xFF, 0xFF, 5, 8, 7);

        loopCount = (-(allocation->saveSlotDialogType != 2) & 3) | 2;
        if (loopCount == 0) {
            return;
        }

        temp = 0xFFF00000;
        for (i = 0; i < loopCount; i++) {
            flagArg = 5;
            if (allocation->saveSlotDialogSelection == i) {
                alphaArg = 0xFF;
                if (allocation->saveSlotMenuState == 0x34) {
                    flagArg = (-(allocation->selectionAnimState & 1) & 0xFF) | 5;
                }
            } else {
                alphaArg = 0x60;
            }

            enqueueTextLayout(
                state->textAsset,
                gSaveSlotMenuOptionTexts[(allocation->saveSlotDialogType * 3) + i],
                -0x40,
                (s16)((temp + i * 0x180000) >> 16),
                alphaArg,
                0xFF,
                flagArg,
                8,
                7
            );
        }
    } else if (allocation->saveSlotDialogType == 0xA) {
        for (i = 0; i < 2; i++) {
            flagArg = 5;
            if (allocation->saveSlotDialogSelection == i) {
                alphaArg = 0xFF;
                if (allocation->saveSlotMenuState == 0x34) {
                    flagArg = (-(allocation->selectionAnimState & 1) & 0xFF) | 5;
                }
            } else {
                alphaArg = 0x60;
            }

            enqueueTextLayout(
                state->textAsset,
                gSaveSlotModeOptionTexts[i],
                -0x40,
                (s16)((0xFFF00000 + i * 0x180000) >> 16),
                alphaArg,
                0xFF,
                flagArg,
                8,
                7
            );
        }
    } else {
        s16 xOffset;
        if (allocation->eepromErrorStatus != 0x62) {
            xOffset = -0x58 | -0x60;
        } else {
            xOffset = -0x60;
        }

        enqueueTextLayout(
            state->textAsset,
            gSaveSlotMenuOptionTexts[allocation->saveSlotDialogType],
            xOffset,
            -0x20,
            0xFF,
            0xFF,
            5,
            8,
            7
        );
    }
}

void cleanupSaveSlotDeleteText(SaveSlotDeleteTextState *state) {
    state->spriteAsset = freeNodeMemory(state->spriteAsset);
    state->textAsset = freeNodeMemory(state->textAsset);
}

void initSaveSlotDeleteArrow(SaveSlotDeleteArrowState *state) {
    void *spriteAsset;

    spriteAsset = loadCompressedData(&uiFontSpriteSheet_ROM_START, &uiFontSpriteSheet_ROM_END, 0x2278);
    setCleanupCallback(cleanupSaveSlotDeleteArrow);

    state->x = -0x52;
    state->y = -0x14;
    state->frameIndex = 0xA;
    state->animIndex = 0;
    state->animDelay = 0;
    state->spriteAsset = spriteAsset;
    state->alpha = 0xFF;
    state->blinkAlpha = 0;
    state->tileMode = 0;

    setCallback(updateSaveSlotDeleteArrow);
}

void updateSaveSlotDeleteArrow(SaveSlotDeleteArrowState *state) {
    SaveSlotScreenState *allocation;
    u32 temp;

    allocation = getCurrentAllocation();

    temp = allocation->saveSlotMenuState;
    if ((u32)(temp - 0x33) < 2U) {
        state->y = (allocation->saveSlotDialogSelection * 3 * 8) - 0x14;

        if (allocation->saveSlotMenuState == 0x34) {
            if (allocation->selectionAnimState & 1) {
                state->blinkAlpha = 0xFF;
            } else {
                state->blinkAlpha = 0;
            }
        } else {
            state->blinkAlpha = 0;
        }

        enqueueCallbackBySlotIndex(8, 1, renderTextSprite, state);

        if (allocation->saveSlotMenuState == 0x33) {
            state->animDelay++;
            state->animDelay &= 3;
            if (state->animDelay == 0) {
                state->animIndex++;
                state->animIndex &= 3;
                state->frameIndex = gSaveSlotDeleteArrowAnimFrames[state->animIndex];
            }
        } else {
            state->animDelay = 0;
            state->animIndex = 0;
            state->frameIndex = 0xA;
        }
    } else {
        state->animDelay = 0;
        state->animIndex = 0;
        state->frameIndex = 0xA;
    }
}

void cleanupSaveSlotDeleteArrow(SaveSlotDeleteArrowState *arg0) {
    arg0->spriteAsset = freeNodeMemory(arg0->spriteAsset);
}
