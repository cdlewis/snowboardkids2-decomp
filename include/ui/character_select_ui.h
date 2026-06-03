#ifndef _232F0_H
#define _232F0_H

#include "math/geometry.h"

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

#endif
