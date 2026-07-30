#pragma once

#include "common.h"
#include "graphics/displaylist.h"

typedef struct {
    s32 x;
    s32 z;
} JingleTownTrainWaypoint;

typedef struct {
    s32 initialTrackSector;
    JingleTownTrainWaypoint waypoints[2];
} JingleTownTrainPath;

typedef struct {
    /* 0x00 */ DisplayListObject displayObject;
    /* 0x3C */ Vec3i trackPosition;
    /* 0x48 */ s32 heightOffset;
    /* 0x4C */ s32 verticalVelocity;
    /* 0x50 */ s16 pathIndex;
    /* 0x52 */ s16 targetWaypointIndex;
    /* 0x54 */ u16 yawAngle;
    /* 0x56 */ s16 trackSector;
    /* 0x58 */ s16 idleTimer;
    /* 0x5A */ s16 remainingJumps;
} JingleTownTrain;

void spawnJingleTownTrains(void);
