#pragma once

#include "common.h"
#include "gamestate.h"

void setPlayerProjectileHitState(Player *player);
void setPlayerState50(Player *player);
void setPlayerPullState(Player *player, void *pullTarget);
void setPlayerCollisionKnockbackState(Player *player, s16 knockbackAngle, s32 knockbackStrength);
void setPlayerHomingProjectileRetaliationState(Player *player);
void setPlayerParachuteState(Player *player);
void setPlayerShrinkState(Player *player);
void setPlayerPanelHitState(Player *player);
void setPlayerFrozenState(Player *player);
void setPlayerStarHitState(Player *player, Vec3i *hitPosition);
void setPlayerBouncedBackState(Player *player);
void func_80058CFC_598FC(void *arg0);
void addPlayerRaceGold(Player *player, s32 amount);
void playTrickLandingVoice(Player *player);
void playTrickSuccessVoice(Player *player);
void playRaceWinVoice(Player *player);
void playCharacterVoice1(Player *player);
void playCharacterVoice2(Player *player);
void playCharacterVoice3(Player *player);
void setPlayerState80(Player *player);
void setPlayerState100(Player *player);
void setPlayerHitStunState(Player *player, s16 hitState, void *knockbackData);