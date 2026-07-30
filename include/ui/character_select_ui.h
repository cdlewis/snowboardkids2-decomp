#pragma once

#include "graphics/graphics.h"
#include "math/geometry.h"

#define CHAR_SELECT_MENU_NAV 0
#define CHAR_SELECT_MENU_ROTATING 1
#define CHAR_SELECT_MENU_CONFIRMING 2
#define CHAR_SELECT_CHAR_ROW_BROWSE 3
#define CHAR_SELECT_CHAR_ROW_SLIDE 4
#define CHAR_SELECT_CHAR_ROW_FLASH 5
#define CHAR_SELECT_CHAR_CONFIRMED 6
#define CHAR_SELECT_CHAR_VARIANT_SLIDE 9
#define CHAR_SELECT_CHAR_VARIANT_BROWSE 10
#define CHAR_SELECT_BOARD_BROWSE 15
#define CHAR_SELECT_BOARD_SLIDE 16
#define CHAR_SELECT_BOARD_FLASH 17
#define CHAR_SELECT_CANCEL_EXIT 20
#define CHAR_SELECT_READY_CONFIRM 25
#define CHAR_SELECT_P2_NAME_REVEAL 26
#define CHAR_SELECT_READY_WAIT 27
#define CHAR_SELECT_P2_CANCEL 30

typedef struct {
    /* 0x0000 */ ViewportNode playerViewports[4];
    /* 0x0750 */ ViewportNode modelViewports[4];
    /* 0x0EA0 */ ViewportNode iconViewports[4];
    /* 0x15F0 */ ViewportNode cameraNode;
    /* 0x17F8 */ Transform3D characterRotations[4];
    /* 0x1878 */ void *menuSpriteAsset;
    /* 0x187C */ void *iconSpriteAsset;
    /* 0x1880 */ u16 carouselAngles[4];
    /* 0x1888 */ s16 previewSpinAngles[4];
    /* 0x1890 */ u8 padding1890[8];
    /* 0x1898 */ u16 menuStates[4];
    /* 0x18A0 */ u16 frameCounters[4];
    /* 0x18A8 */ u8 characterCategories[4];
    /* 0x18AC */ u8 previousCharacterCategories[4];
    /* 0x18B0 */ u8 characterVariants[4];
    /* 0x18B4 */ u8 previousCharacterVariants[4];
    /* 0x18B8 */ u8 boardIds[4];
    /* 0x18BC */ u8 previousBoardIds[4];
    /* 0x18C0 */ u8 scrollDirections[4];
    /* 0x18C4 */ u8 completedSlides[4];
    /* 0x18C8 */ u8 iconDisplayStates[4];
    /* 0x18CC */ u8 maxMenuOption;
    /* 0x18CD */ u8 hasSecretCharacters;
    /* 0x18CE */ s8 unlockedSlotIndices[4];
    /* 0x18D2 */ u8 cursorIndices[4];
    /* 0x18D6 */ u8 previousCursorIndices[4];
    /* 0x18DA */ u8 padding18DA[6];
} CharacterSelectState;

/**
 * Keep the element stride before the allocation base in this expression.
 * KMC otherwise reverses the operands of the resulting `addu`.
 */
#define CHARACTER_SELECT_ROTATION_AT(state, index)                          \
    ((Transform3D *)((index) * sizeof(Transform3D) + (s32)(state) + (s32) & \
                     ((CharacterSelectState *)0)->characterRotations))

typedef struct {
    s16 carouselXPositions[17];
    Vec3s menuPositions[5];
} CharSelectModelPositions;

typedef struct {
    Vec3s positions;
    Vec3s yIncrements[4];
    s16 iconYIncrement;
} CharSelectIconPositions;

typedef struct {
    u8 numStats;
    u8 numColumns;
    u8 itemStats[48];
    s16 iconTargetY[13];
} CharSelectItemData;

typedef struct {
    s16 positions[7];
    s16 D_8008DDE6[3];
} PlayerNumberPositions;

typedef struct {
    s16 iconPositions[11];
    s16 D_8008DE02[11];
} CharSelectStatsPositions;

typedef struct {
    u8 arrowSpriteIndices[2];
    s16 arrowSpriteOffsets[2];
    s8 arrowOffsets[28];
    s16 arrowPositions[13];
} CharSelectArrowData;

typedef struct {
    s16 boardPositions[11];
    u16 boardWidths[17];
} CharSelectBoardData;

typedef struct {
    s16 statSpacing[1];
    s16 statPositions[16];
    s16 namePositions[17];
} CharSelectAnimData;

s32 countUnlockedSlotsInCategory(u8 category);
void initCharacterSelectScreen(void);
