#pragma once

#include "common.h"
#include "displaylist.h"
#include "geometry.h"

/* Global data arrays for train positioning */
extern s32 gJingleTownTrainInitialHeights[];
extern s32 gJingleTownTrainWaypointsX[];
extern s32 gJingleTownTrainWaypointsZ[];
extern s32 gJingleTownTrainWaypointsX2[];
extern s32 gJingleTownTrainWaypointsZ2[];
extern Vec3i gJingleTownTrainForwardVector;
extern Vec3i gJingleTownTrainForwardVector2;

typedef struct {
    u8 _pad0[0x24];
    void *unk24;
    void *unk28;
} B4240Struct;

typedef struct {
    u8 _pad0[0x30];
    void *unk30;
    u8 _pad34[0x28];
    u8 unk5C;
    u8 _pad5D[0x19];
    u8 unk76;
} B4240AllocationStruct;

typedef struct {
    DisplayListObject node;
    s32 posX;
    s32 height;
    s32 posZ;
    s32 yOffset;
    s32 yVelocity;
    s16 trainIndex;
    s16 waypointIndex;
    u16 rotation;
    s16 unk56;
    s16 waitTimer;
    s16 loopCount;
} JingleTownTrain;

void spawnJingleTownTrains(void);
void renderJingleTownTrain(JingleTownTrain *train);
void pullPlayersToTrain(JingleTownTrain *train);
void initJingleTownTrain(JingleTownTrain *train);
void handleTrainIdleState(JingleTownTrain *train);
void handleTrainHopBehavior(JingleTownTrain *train);
void handleTrainJumpBehavior(JingleTownTrain *train);
void cleanupJingleTownTrain(B4240Struct *arg0);
