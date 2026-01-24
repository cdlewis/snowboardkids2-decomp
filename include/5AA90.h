#pragma once

#include "common.h"
#include "gamestate.h"
#include "geometry.h"

typedef struct ListNode_5AA90 {
    /* 0x00 */ struct ListNode_5AA90 *next;
    /* 0x04 */ Vec3i *posPtr;
    /* 0x08 */ Vec3i localPos;
    /* 0x14 */ s32 radius;
    /* 0x18 */ u8 id;
} ListNode_5AA90;

void func_8005A930_5B530(Player *player);
s32 isPlayerNearShortcut(Player *player);
void handlePlayerToPlayerCollision(Player *player);
void handleCollisionWithTargetPlayer(Player *player);
void addCollisionSectorNodeToList(ListNode_5AA90 *arg0);
Player *findPlayerNearPosition(Vec3i *position, s32 excludePlayerIndex, s32 searchRadius);
Player *findVulnerablePlayerNearPosition(void *position, s32 excludePlayerIndex, s32 searchRadius);
s32 func_8005A9A8_5B5A8(Player *player);
s32 isPointInPlayerCollisionSphere(Player *player, Vec3i *point, s32 extraRadius);
s32 checkPositionPlayerCollisionWithKnockback(Vec3i *pos, s32 extraRadius, s32 maxHeight, s16 excludePlayerIdx);
s32 checkPositionPlayerCollisionWithPull(void *pos, s32 extraRadius, s32 maxHeight);
s32 checkStarHitCollisionWithVulnerablePlayers(Vec3i *pos, s32 excludePlayerIdx, s32 extraRadius);
void func_8005C868_5D468(void *arg0);
s32 isPlayerInRangeAndPull(Vec3i *arg0, s32 arg1, Player *arg2);
void handleOrientedAreaCollision(Vec3i *origin, s32 radialThreshold, s32 depthExtent, s16 rotationAngle);
s32 getOrUpdatePlayerSectorIndex(void *entity, void *gameData, u16 currentSectorIndex, void *position);
Player *
findVulnerablePlayerNearPositionWithDelta(Vec3i *position, s32 excludePlayerIdx, s32 searchRadius, Vec3i *outDelta);
s16 func_8005BF50_5CB50(Vec3i *arg0, s16 arg1, s16 arg2, s32 arg3, s32 arg4);
void func_8005A26C_5AE6C(Player *);
s32 func_80059ED0_5AAD0(Player *);