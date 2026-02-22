#ifndef _232F0_H
#define _232F0_H

#include "geometry.h"

typedef struct {
    s16 unk0[17];
    Vec3s unk22[5];
} CharSelectModelPositions;

typedef struct {
    Vec3s positions;
    Vec3s yIncrements[4];
    s16 unk1E;
} CharSelectIconPositions;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2[48];
    s16 unkBE[13];
} CharSelectItemData;

typedef struct {
    s16 positions[7];
    s16 D_8008DDE6[3];
} PlayerNumberPositions;

typedef struct {
    s16 unk0[11];
    s16 D_8008DE02[11];
} CharSelectStatsPositions;

typedef struct {
    u8 unk0[2];
    s16 unk2[2];
    s8 unk6[28];
    s16 arrowPositions[13];
} CharSelectArrowData;

typedef struct {
    s16 unk0[11];
    u16 unk16[17];
} CharSelectBoardData;

typedef struct {
    s16 unk0[1];
    s16 unk2[16];
    s16 unk22[17];
} CharSelectAnimData;

#endif
