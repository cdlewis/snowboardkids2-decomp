#pragma once

#include "common.h"
#include "gamestate.h"

void setPlayerProjectileHitState(Player *player);
void setPlayerState50(Player *player);
void setPlayerPullState(Player *player, void *pullTarget);
void setPlayerCollisionKnockbackState(Player *player, s16 knockbackAngle, s32 knockbackStrength);
void func_80058CFC_598FC(void *arg0);

void addPlayerRaceGold(Player *player, s32 amount);
void playTrickLandingVoice(Player *player);
void playTrickSuccessVoice(Player *player);
void playRaceWinVoice(Player *player);
void playCharacterVoice1(Player *player);
void playCharacterVoice2(Player *player);
void playCharacterVoice3(Player *player);
