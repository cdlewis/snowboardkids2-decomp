#pragma once

#include "common.h"
#include "gamestate.h"

typedef struct {
    /* 0x000 */ u8 _pad0[0xB84];
    /* 0xB84 */ s32 animFlags;
    /* 0xB88 */ u8 _padB88[0x12];
    /* 0xB9A */ s16 boostTimer;
    /* 0xB9C */ u8 _padB9C[0x8];
    /* 0xBA4 */ s16 invincibilityTimer;
    /* 0xBA6 */ s16 featherItemTimer;
    /* 0xBA8 */ u8 _padBA8[0x15];
    /* 0xBBD */ u8 behaviorMode;
    /* 0xBBE */ u8 behaviorPhase;
    /* 0xBBF */ u8 behaviorStep;
    /* 0xBC0 */ u8 behaviorCounter;
} BehaviorState;

typedef struct {
    /* 0x00 */ u8 primaryR;
    /* 0x01 */ u8 primaryG;
    /* 0x02 */ u8 primaryB;
    /* 0x03 */ u8 pad1;
    /* 0x04 */ u8 secondaryR;
    /* 0x05 */ u8 secondaryG;
    /* 0x06 */ u8 secondaryB;
    /* 0x07 */ u8 pad2;
} BossSurfaceColor;

extern BossSurfaceColor gBossSurfaceColors[];

void setPlayerBehaviorMode(Player *player, u8 mode);
void setPlayerBehaviorPhase(Player *player, s32 phase);
void resetPlayerBehaviorToDefault(Player *player);
void initKnockbackBehavior(BehaviorState *arg0);
void applyClampedVelocityToPosition(Player *player);
void renderRacerProjectedShadow(Player *player);
void loadPlayerCharacterAssets(void *arg0);
void freePlayerCharacterAssets(void *arg0);
void renderPlayersByShortcutDistance(void);
void updateAndRenderRaceCharacters(void);
