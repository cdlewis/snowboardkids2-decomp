#pragma once

#include "6E840.h"
#include "graphics.h"
#include "main.h"

typedef struct {
    /* 0x00 */ ViewportNode node1;
    /* 0x1C8 */ ViewportNode node2;
    /* 0x390 */ u8 padding2[0x20];
    /* 0x3B0 */ s32 unk3D0;
    /* 0x3B4 */ s32 unk3D4;
    /* 0x3B8 */ s32 unk3D8;
    /* 0x3BC */ void *unk3DC;
    /* 0x3C0 */ void *unk3E0;
    /* 0x3C4 */ void *unk3E4;
    /* 0x3C8 */ s16 unk3E8;
    /* 0x3CA */ u8 padding3[0x14];
    /* 0x3DE */ s16 unk3FE;
    /* 0x3E0 */ u8 padding4[0x3];
    /* 0x3E3 */ u8 unk403;
    /* 0x3E4 */ u8 padding5[0x20];
    /* 0x404 */ u8 locationDiscovered;
    /* 0x405 */ s8 discoveredLocationId;
    /* 0x406 */ u8 unk426;
    /* 0x407 */ u8 unk427;
    /* 0x408 */ u8 unk428;
    /* 0x409 */ u8 unk429;
    /* 0x40A */ u8 unk42A;
    /* 0x40B */ u8 padding6[0x3];
    /* 0x40E */ s8 unk42E;
} task_memory_430;

void cleanupTransitionEffect(EffectState *arg0);
void gameStateCleanupHandler(void);
void gameStateCountdownHandler(void);
void initializeGameState(void);
void invokeTransitionEffect(ModelEntityRenderState *state);
void setShortCountdownAndContinue(void);
void setupGameStateTransition(ModelEntity *arg0);
void transitionToMainMenu(void);
void transitionToNextGameMode(void);
