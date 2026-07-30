#pragma once

#include "EepromSaveData_type.h"
#include "common.h"
#include "graphics/graphics.h"
#include "graphics/sprite_rdp.h"
#include "text/font_assets.h"
#include "text/text_layout.h"

typedef struct {
    /* 0x000 */ ViewportNode mainViewport;
    /* 0x1D8 */ ViewportNode slotModels[4];
    /* 0x938 */ EepromSaveData_type slotData[4];
    /* 0xAA8 */ void *uiFontSpriteSheet;
    /* 0xAAC */ void *confirmationSpriteSheet;
    /* 0xAB0 */ void *snowflakeSpriteSheet;
    /* 0xAB4 */ void *textRenderAsset;
    /* 0xAB8 */ void *goldIconSpriteSheet;
    /* 0xABC */ s16 slideOffset;
    /* 0xABE */ s16 nameEntryGridX;
    /* 0xAC0 */ s16 nameEntryCursorY;
    /* 0xAC2 */ u16 nameEntryCursorBaseY;
    /* 0xAC4 */ u16 selectionAnimState;
    /* 0xAC6 */ u16 saveSlotMenuState;
    /* 0xAC8 */ u8 selectedSaveSlot;
    /* 0xAC9 */ u8 hasCurrentSaveData;
    /* 0xACA */ u8 eepromOperationStatus;
    /* 0xACB */ u8 mainPromptIndex;
    /* 0xACC */ u8 numValidSlots;
    /* 0xACD */ u8 eepromErrorStatus;
    /* 0xACE */ union {
        struct {
            u8 slotHasData[3];
            u8 originalSlotDataFlag;
        } selection;
        u8 displaySlotActive[4];
    } slotStatus;
    /* 0xAD2 */ u8 writeSlotIndexPlusOne;
    /* 0xAD3 */ u8 menuAnimOffsetX;
    /* 0xAD4 */ u8 menuChoiceIndex;
    /* 0xAD5 */ u8 saveSlotDialogType;
    /* 0xAD6 */ u8 saveSlotDialogSelection;
    /* 0xAD7 */ u8 savedSlotIndex;
    /* 0xAD8 */ u8 animDelayCounter;
} SaveSlotScreenState;

typedef SpriteRenderArg SaveSlotConfirmationIndicatorState;

typedef struct SaveSlotDeleteArrowState {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ void *spriteAsset;
    /* 0x08 */ s16 frameIndex;
    /* 0x0A */ s16 alpha;
    /* 0x0C */ u8 tileMode;
    /* 0x0D */ u8 blinkAlpha;
    /* 0x0E */ u8 padE[2];
    /* 0x10 */ u8 animIndex;
    /* 0x11 */ u8 animDelay;
} SaveSlotDeleteArrowState;

typedef struct SaveSlotDeleteTextState {
    /* 0x00 */ void *spriteAsset;
    /* 0x04 */ void *textAsset;
    /* 0x08 */ s16 offsetX;
    /* 0x0A */ s16 offsetY;
} SaveSlotDeleteTextState;

typedef struct SaveSlotGoldDisplayState {
    /* 0x00 */ ScaledSpriteArg icons[4];
    /* 0x50 */ ColoredTextRenderArg text[4];
    /* 0x80 */ u8 textBuffers[4][10];
    /* 0xA8 */ u8 animFrames[4];
} SaveSlotGoldDisplayState;

typedef struct SaveSlotGridState {
    /* 0x00 */ TextRenderArg *entries;
    /* 0x04 */ SpriteRenderArg cursorSprite;
} SaveSlotGridState;

typedef struct SaveSlotItemIconsState {
    /* 0x00 */ void *spriteSheet;
    /* 0x04 */ TextRenderArg *icons;
    /* 0x08 */ u8 pad8[0x14];
    /* 0x1C */ u8 slotIndex;
    /* 0x1D */ u8 animFrame;
    /* 0x1E */ u8 slotHasData;
} SaveSlotItemIconsState;

typedef struct SaveSlotNameTextState {
    /* 0x00 */ void *textAsset;
    /* 0x04 */ u8 pad4[0x4];
    /* 0x08 */ TextLayoutArg label;
    /* 0x1C */ u8 slotIndex;
    /* 0x1D */ u8 isAnimating;
    /* 0x1E */ u8 slotHasData;
} SaveSlotNameTextState;

typedef struct SaveSlotNumberLabelsState {
    /* 0x00 */ TextRenderArg sprites[6];
    /* 0x60 */ ColoredTextRenderArg texts[9];
    /* 0xCC */ char textBuffers[9][4];
    /* 0xF0 */ u8 slotIndex;
} SaveSlotNumberLabelsState;

typedef struct SaveSlotPromptTextState {
    /* 0x00 */ TextLayoutArg mainText;
    /* 0x14 */ TextLayoutArg hintText;
} SaveSlotPromptTextState;

typedef struct SaveSlotSelectionParticlesState {
    /* 0x00 */ SpriteRenderArg entries[4];
    /* 0x30 */ u8 pad30[0x18];
    /* 0x48 */ u8 isRightSide;
    /* 0x49 */ u8 animToggle;
} SaveSlotSelectionParticlesState;

typedef struct SaveSlotStatSpritesState {
    /* 0x00 */ TextRenderArg entries[13];
    /* 0xD0 */ u8 slotIndex;
} SaveSlotStatSpritesState;

void initSaveSlotConfirmationIndicator(SaveSlotConfirmationIndicatorState *state);
void initSaveSlotDeleteArrow(SaveSlotDeleteArrowState *state);
void initSaveSlotDeleteText(SaveSlotDeleteTextState *state);
void initSaveSlotGoldDisplay(SaveSlotGoldDisplayState *state);
void initSaveSlotItemIcons(SaveSlotItemIconsState *state);
void initSaveSlotItemLabels(SaveSlotNumberLabelsState *state);
void initSaveSlotNameEntryGrid(SaveSlotGridState *state);
void initSaveSlotNameText(SaveSlotNameTextState *state);
void initSaveSlotPromptText(SaveSlotPromptTextState *state);
void initSaveSlotSelectionParticles(SaveSlotSelectionParticlesState *state);
void initSaveSlotStatSprites(SaveSlotStatSpritesState *state);
