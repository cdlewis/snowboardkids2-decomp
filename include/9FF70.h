#pragma once

#include "common.h"
#include "gamestate.h"

typedef struct {
    u8 _pad0[0xB84];
    s32 unkB84;
    u8 _padB88[0x12]; // 0xB88 to 0xB9A
    s16 boostTimer;
    u8 _padB9C[0x8]; // 0xB9C to 0xBA4
    s16 invincibilityTimer;
    s16 ghostEffectTimer;
    u8 _padBA8[0x15]; // 0xBA8 to 0xBBD
    u8 behaviorMode;
    u8 behaviorPhase;
    u8 behaviorStep;
    u8 behaviorCounter;
} BehaviorState;

void setPlayerBehaviorMode(Player *player, u8 mode);
void setPlayerBehaviorPhase(Player *player, s32 phase);
void resetPlayerBehaviorToDefault(void *arg);
void initKnockbackBehavior(BehaviorState *arg0);
void applyClampedVelocityToPosition(Player *player);
void func_800B9500_A93B0(void *arg0);
void loadPlayerCharacterAssets(void *arg0);
void freePlayerCharacterAssets(void *arg0);
void func_800B0334_A01E4(void);
void updateAndRenderRaceCharacters(void);