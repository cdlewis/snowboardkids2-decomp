#pragma once

#include "common.h"
#include "gamestate.h"
#include "math/geometry.h"

void updatePlayerSectorAndClampYToTrack(Player *player);
s32 isPlayerNearShortcut(Player *player);
void handlePlayerToPlayerCollision(Player *player);
void handleCollisionWithTargetPlayer(Player *player);
void addCollisionSectorNodeToList(ListNode_5AA90 *arg0);
Player *findPlayerNearPosition(Vec3i *position, s32 excludePlayerIndex, s32 searchRadius);
Player *findVulnerablePlayerNearPosition(void *position, s32 excludePlayerIndex, s32 searchRadius);
s32 tryActivateShortcut(Player *player);
s32 isPointInPlayerCollisionSphere(Player *player, Vec3i *point, s32 extraRadius);
s32 checkPositionPlayerCollisionWithKnockback(Vec3i *pos, s32 extraRadius, s32 maxHeight, s16 excludePlayerIdx);
s32 checkPositionPlayerCollisionWithPull(void *pos, s32 extraRadius, s32 maxHeight);
s32 checkStarHitCollisionWithVulnerablePlayers(Vec3i *pos, s32 excludePlayerIdx, s32 extraRadius);
void computePlayerTerrainAlignment(Player *player);
s32 isPlayerInRangeAndPull(Vec3i *arg0, s32 arg1, Player *arg2);
void handleOrientedAreaCollision(Vec3i *origin, s32 radialThreshold, s32 depthExtent, s16 rotationAngle);
s32 getOrUpdatePlayerSectorIndex(void *entity, void *gameData, u16 currentSectorIndex, void *position);
Player *
findVulnerablePlayerNearPositionWithDelta(Vec3i *position, s32 excludePlayerIdx, s32 searchRadius, Vec3i *outDelta);
s16 getHomingAngleToTarget(Vec3i *pos, s16 facingAngle, s32 excludePlayerIdx, s32 searchRadius, s32 closeRangeThreshold);
void func_8005A26C_5AE6C(Player *);
s32 handlePlayerTrackWallCollision(Player *player);
