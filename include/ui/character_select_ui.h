#pragma once

#include "math/geometry.h"
#include "ui/character_select_gfx.h"
#include "ui/level_preview_3d.h"

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
