#pragma once

#include "common.h"
#include "gamestate.h"

void setPlayerProjectileHitState(Player *player);
void setPlayerState50(Player *player);
void setPlayerPullState(Player *player, void *pullTarget);
void setPlayerCollisionKnockbackState(Player *player, s16 knockbackAngle, s32 knockbackStrength);
void func_80058CFC_598FC(void *arg0);

void func_80059A48_5A648(Player *arg0, s32 arg1);
void func_80059ADC_5A6DC(Player *arg0);
void func_80059BD4_5A7D4(Player *arg0);
void func_80059CE0_5A8E0(Player *arg0);
