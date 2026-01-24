#include "5AA90.h"
#include "56910.h"
#include "594E0.h"
#include "5DBC0.h"
#include "common.h"
#include "displaylist.h"
#include "gamestate.h"
#include "geometry.h"
#include "race_session.h"
#include "task_scheduler.h"

// Struct definitions
typedef struct {
    u8 _pad[0x10];
    /* 0x10 */ void *dataArray;
    u8 _pad2[0x18];
    /* 0x2C */ ListNode_5AA90 *list;
    /* 0x30 */ u8 unk30[0];
} Allocation5AA90;

// Element size is 6 bytes based on *6 pattern in asm
typedef struct {
    /* 0x0 */ s16 unk0;
    /* 0x2 */ s16 unk2;
    /* 0x4 */ s16 unk4;
} Section1Element;

// Element at offset 0x3C has size 0x24 (36 bytes)
typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ u8 pad[0x14];
    /* 0x16 */ u16 unk16;
    /* 0x18 */ u8 pad2[0x4];
    /* 0x1C */ u16 unk1C;
    /* 0x1E */ u8 pad3[0x6];
} Section3Element;

typedef struct {
    u8 padding[0xB88];
    s32 collisionFlags;
    u8 padding2[0x18];
    u16 invincibilityTimer;
} PlayerCollisionData;

// Function implementations
s32 getOrUpdatePlayerSectorIndex(void *entity, void *gameData, u16 currentSectorIndex, void *position) {
    Player *player = (Player *)entity;
    if (!(player->unkB84 & 0x100)) {
        return func_80060A3C_6163C(gameData, currentSectorIndex, position);
    }
    return player->sectorIndex;
}

INCLUDE_ASM("asm/nonmatchings/5AA90", func_80059ED0_5AAD0);

INCLUDE_ASM("asm/nonmatchings/5AA90", func_8005A26C_5AE6C);

// Updates player's sector index and clamps Y position to track height
void func_8005A930_5B530(Player *player) {
    GameState *gameState;
    GameDataLayout *gameData;
    Vec3i *playerPos;
    u16 newSectorIndex;
    s32 trackHeight;

    gameState = (GameState *)getCurrentAllocation();
    gameData = &gameState->gameData;
    playerPos = &player->worldPos;
    newSectorIndex = getOrUpdatePlayerSectorIndex(player, gameData, player->sectorIndex, playerPos);
    player->sectorIndex = newSectorIndex;
    trackHeight = getSectorTrackHeight(gameData, newSectorIndex, playerPos, 0x200000);
    if (trackHeight < player->worldPos.y) {
        player->worldPos.y = trackHeight;
    }
}

/**
 * Checks if player is in shortcut activation zone and activates the shortcut.
 * The shortcut is activated when:
 * - Shortcut gate state is 0 (not yet activated)
 * - Player is not on the final lap
 * - Player is not at the finish zone
 * - Player is within range of the shortcut position
 *
 * @param player The player to check
 * @return 1 if shortcut was activated, 0 otherwise
 */
s32 func_8005A9A8_5B5A8(Player *player) {
    GameState *gameState;
    LevelConfig *levelItem;
    s32 deltaX, deltaZ;
    s64 distSq;

    gameState = (GameState *)getCurrentAllocation();

    // Skip if shortcut already activated
    if (gameState->shortcutGateState != 0) {
        return 0;
    }

    // Skip if player is on final lap
    if (player->currentLap == gameState->finalLapNumber) {
        return 0;
    }

    // Skip if player is at finish zone
    if (getTrackSegmentFinishZoneFlag(&gameState->gameData, player->sectorIndex) != 0) {
        return 0;
    }

    // Get level item containing shortcut position
    levelItem = getLevelConfig(gameState->memoryPoolId);

    // Calculate 2D distance to shortcut position
    deltaX = player->worldPos.x - levelItem->shortcutPosX;
    deltaZ = player->worldPos.z - levelItem->shortcutPosZ;

    distSq = (s64)deltaX * deltaX + (s64)deltaZ * deltaZ;

    // Check if player is within activation range (0x2FFFFF in fixed point)
    if (0x2FFFFF < isqrt64(distSq)) {
        return 0;
    }

    // Activate the shortcut
    gameState->shortcutGateState = 3;
    player->shortcutLapCount = gameState->shortcutActivationCounter;
    gameState->shortcutActivationCounter--;

    return 1;
}

/**
 * Checks if a player is near the shortcut position (but not at finish zone).
 * This is a proximity-only check and does not activate the shortcut.
 *
 * @param player The player to check
 * @return 1 if player is near shortcut, 0 otherwise
 */
s32 isPlayerNearShortcut(Player *player) {
    GameState *gameState;
    LevelConfig *levelItem;
    s32 deltaX, deltaZ;
    s64 distSq;

    gameState = (GameState *)getCurrentAllocation();

    if (getTrackSegmentFinishZoneFlag(&gameState->gameData, player->sectorIndex) == 0) {
        levelItem = getLevelConfig(gameState->memoryPoolId);

        deltaX = player->worldPos.x - levelItem->shortcutPosX;
        deltaZ = player->worldPos.z - levelItem->shortcutPosZ;

        distSq = (s64)deltaX * deltaX + (s64)deltaZ * deltaZ;

        if (!(0x1FFFFF < isqrt64(distSq))) {
            return 1;
        }
    }
    return 0;
}

/**
 * Handles player-to-player collision detection and response.
 * Checks the given player against all other players and pushes them out of collision.
 *
 * @param player The player to check collisions for
 */
void handlePlayerToPlayerCollision(Player *player) {
    Vec3i deltaPos;
    GameState *gameState;
    s32 combinedRadius;
    s32 playerIndex;
    Player *targetPlayer;

    gameState = (GameState *)getCurrentAllocation();

    for (playerIndex = 0; playerIndex < gameState->numPlayers; playerIndex++) {
        targetPlayer = &gameState->players[playerIndex];

        /* Skip collision if players are on the same team */
        if (player->playerIndex == targetPlayer->playerIndex) {
            continue;
        }

        /* Skip if either player has collision disabled (flag 0x10) */
        if (player->unkB88 & 0x10) {
            continue;
        }
        if (targetPlayer->unkB88 & 0x10) {
            continue;
        }

        /* Copy target's collision box local position */
        memcpy(&deltaPos, &targetPlayer->collisionOffset, 0xC);

        /* Convert to world space */
        deltaPos.x += targetPlayer->worldPos.x;
        deltaPos.y += targetPlayer->worldPos.y;
        deltaPos.z += targetPlayer->worldPos.z;

        /* Calculate relative position to player's collision box */
        deltaPos.x -= player->worldPos.x + player->collisionOffset.x;
        deltaPos.y -= player->worldPos.y + player->collisionOffset.y;
        deltaPos.z -= player->worldPos.z + player->collisionOffset.z;

        /* Sum of both collision box radii */
        combinedRadius = targetPlayer->collisionRadius + player->collisionRadius;

        /* Quick AABB check before expensive distance calculation */
        if (-combinedRadius < deltaPos.x && deltaPos.x < combinedRadius && -combinedRadius < deltaPos.y &&
            deltaPos.y < combinedRadius && -combinedRadius < deltaPos.z && deltaPos.z < combinedRadius) {
            s32 dist;

            dist = isqrt64((s64)deltaPos.x * deltaPos.x + (s64)deltaPos.y * deltaPos.y + (s64)deltaPos.z * deltaPos.z);

            if (dist < combinedRadius) {
                /* Avoid divide by zero */
                if (dist == 0) {
                    dist = 1;
                }

                /* Calculate push vector to separate the players */
                deltaPos.x = ((s64)deltaPos.x * combinedRadius / dist) - deltaPos.x;
                deltaPos.y = ((s64)deltaPos.y * combinedRadius / dist) - deltaPos.y;
                deltaPos.z = ((s64)deltaPos.z * combinedRadius / dist) - deltaPos.z;

                /* Skip push if player has flag 0x80 */
                if (player->unkB84 & 0x80) {
                    continue;
                }

                /* If target has flag 0x80, do full push */
                if (targetPlayer->unkB84 & 0x80) {
                    player->worldPos.x -= deltaPos.x;
                    player->worldPos.y -= deltaPos.y;
                    player->worldPos.z -= deltaPos.z;
                } else {
                    /* Half push */
                    player->worldPos.x -= deltaPos.x / 2;
                    player->worldPos.y -= deltaPos.y / 2;
                    player->worldPos.z -= deltaPos.z / 2;
                }
            }
        }
    }
}

/**
 * Checks collision between a player and the target player (player index 1).
 * Handles collision response including pushing the player out and knockback effects.
 * The target player may have multiple collision boxes (controlled by targetPlayer->unkBB4).
 *
 * unkBD9 values:
 * - 1: Attack state that bounces player on boxes 4-5
 * - 2: Attack state that applies knockback on box 0
 * - 3: Attack state that bounces player on boxes 1-2
 */
void handleCollisionWithTargetPlayer(Player *player) {
    Vec3i deltaPos;
    u8 pad[0x8];
    void *boxBasePtr;
    s32 unused1;
    s32 unused2;
    s32 combinedRadius;
    s32 negRadius;
    s32 dist;
    s32 boxIndex;
    Player *targetPlayer;
    Player *players;

    players = ((GameState *)getCurrentAllocation())->players;
    targetPlayer = players + 1;

    /* Skip collision if players are on the same team */
    if (player->playerIndex == targetPlayer->playerIndex) {
        return;
    }
    /* Skip if either player has collision disabled (flag 0x10) */
    if (player->unkB88 & 0x10) {
        return;
    }
    if (targetPlayer->unkB88 & 0x10) {
        return;
    }

    if ((s8)targetPlayer->unkBB4 > 0) {
        boxIndex = 0;
        boxBasePtr = targetPlayer;
        do {
            /* Copy target's collision box local position (unkAE4 is array of 6 Vec3i at offset 0xAE4)
             * boxBasePtr increments by 0xC (sizeof(Vec3i)) each loop iteration to index into array */
            memcpy(&deltaPos, (u8 *)boxBasePtr + 0xAE4, 0xC);

            /* Convert to world space */
            deltaPos.x += targetPlayer->worldPos.x;
            deltaPos.y += targetPlayer->worldPos.y;
            deltaPos.z += targetPlayer->worldPos.z;

            /* Calculate relative position to player's collision box */
            deltaPos.x -= player->worldPos.x + player->collisionOffset.x;
            deltaPos.y -= player->worldPos.y + player->collisionOffset.y;
            deltaPos.z -= player->worldPos.z + player->collisionOffset.z;

            /* Sum of both collision box radii (unkB2C is treated as array of radii) */
            combinedRadius = (&targetPlayer->unkB2C)[boxIndex] + player->collisionRadius;
            negRadius = -combinedRadius;

            /* Quick AABB check before expensive distance calculation */
            if (negRadius < deltaPos.x && deltaPos.x < combinedRadius && negRadius < deltaPos.y &&
                deltaPos.y < combinedRadius && negRadius < deltaPos.z && deltaPos.z < combinedRadius) {

                dist =
                    isqrt64((s64)deltaPos.x * deltaPos.x + (s64)deltaPos.y * deltaPos.y + (s64)deltaPos.z * deltaPos.z);

                if (dist < combinedRadius) {
                    /* Check for special bounce-back on collision boxes 4-5 when target is in state 1 */
                    if (targetPlayer->unkBD9 == 1) {
                        if ((targetPlayer->unkB84 & 0x40000) && (u32)(boxIndex - 4) < 2U) {
                            setPlayerBouncedBackState(player);
                            goto next;
                        }
                    }
                    /* Check for special bounce-back on collision boxes 1-2 when target is in state 3 */
                    if (targetPlayer->unkBD9 == 3 && (targetPlayer->unkB84 & 0x40000) && (u32)(boxIndex - 1) < 2U) {
                        setPlayerBouncedBackState(player);
                        goto next;
                    }

                    /* Avoid divide by zero */
                    if (dist == 0) {
                        dist = 1;
                    }

                    /* Calculate push vector to separate the players */
                    deltaPos.x = ((s64)deltaPos.x * combinedRadius / dist) - deltaPos.x;
                    deltaPos.y = ((s64)deltaPos.y * combinedRadius / dist) - deltaPos.y;
                    deltaPos.z = ((s64)deltaPos.z * combinedRadius / dist) - deltaPos.z;

                    /* Push player out of collision */
                    player->worldPos.x -= deltaPos.x;
                    player->worldPos.y -= deltaPos.y;
                    player->worldPos.z -= deltaPos.z;

                    /* Apply knockback if target is in state 2 and colliding with main collision box */
                    if ((targetPlayer->unkBD9 == 2) & (boxIndex == 0)) {
                        dist = isqrt64((s64)deltaPos.x * deltaPos.x + (s64)deltaPos.z * deltaPos.z);
                        if (dist > 0x30000) {
                            setPlayerCollisionKnockbackState(
                                player,
                                computeAngleToPosition(
                                    targetPlayer->worldPos.x,
                                    targetPlayer->worldPos.z,
                                    player->worldPos.x,
                                    player->worldPos.z
                                ),
                                dist
                            );
                        }
                    }
                }
            }
        next:
            boxIndex++;
            boxBasePtr = (u8 *)boxBasePtr + 0xC;
        } while (boxIndex < (s8)targetPlayer->unkBB4);
    }
}

/**
 * Finds a player within a given radius of a position.
 * Iterates through all player collision nodes and checks if any player's
 * collision sphere intersects with the search sphere.
 *
 * @param position Center position to search from
 * @param excludePlayerIndex Player index to skip (-1 to include all players)
 * @param searchRadius Radius to search within (added to player's collision radius)
 * @return Player pointer if found, NULL otherwise
 */
Player *findPlayerNearPosition(Vec3i *position, s32 excludePlayerIndex, s32 searchRadius) {
    Vec3i deltaPos;
    s32 combinedRadius;
    Allocation5AA90 *allocation;
    ListNode_5AA90 *node;

    allocation = getCurrentAllocation();
    node = allocation->list;

    for (; node != NULL; node = node->next) {
        if (excludePlayerIndex == node->id) {
            continue;
        }

        memcpy(&deltaPos, &node->localPos, 0xC);

        deltaPos.x += node->posPtr->x;
        deltaPos.y += node->posPtr->y;
        deltaPos.z += node->posPtr->z;

        deltaPos.x -= position->x;
        deltaPos.y -= position->y;
        deltaPos.z -= position->z;

        combinedRadius = node->radius + searchRadius;

        if (-combinedRadius < deltaPos.x && deltaPos.x < combinedRadius && -combinedRadius < deltaPos.y &&
            deltaPos.y < combinedRadius && -combinedRadius < deltaPos.z && deltaPos.z < combinedRadius) {
            s32 dist;

            dist = isqrt64((s64)deltaPos.x * deltaPos.x + (s64)deltaPos.y * deltaPos.y + (s64)deltaPos.z * deltaPos.z);

            if (dist < combinedRadius) {
                u8 playerIndex = node->id;
                return (Player *)((u8 *)allocation->dataArray + ((playerIndex * 3 * 128) - (playerIndex * 3)) * 8);
            }
        }
    }

    return NULL;
}

s32 isPointInPlayerCollisionSphere(Player *player, Vec3i *point, s32 extraRadius) {
    Vec3i delta;
    s32 combinedRadius;
    s32 negRadius;
    s64 distSq;

    memcpy(&delta, &player->collisionOffset, 0xC);

    delta.x += player->worldPos.x;
    delta.y += player->worldPos.y;
    delta.z += player->worldPos.z;

    delta.x -= point->x;
    delta.y -= point->y;
    delta.z -= point->z;

    combinedRadius = player->collisionRadius + extraRadius;
    negRadius = -combinedRadius;

    if (negRadius < delta.x && delta.x < combinedRadius && negRadius < delta.y && delta.y < combinedRadius &&
        negRadius < delta.z && delta.z < combinedRadius) {
        distSq = (s64)delta.x * delta.x + (s64)delta.y * delta.y + (s64)delta.z * delta.z;
        if (isqrt64(distSq) < combinedRadius) {
            return 1;
        }
    }

    return 0;
}

/**
 * Finds a vulnerable (non-invincible, collision-enabled) player within a given radius of a position.
 * Similar to findPlayerNearPosition, but additionally filters out:
 * - Players with collision disabled (unkB88 & 0x10)
 * - Players with active invincibility (invincibilityTimer != 0)
 *
 * @param position Center position to search from (pointer to 3 consecutive s32 values: x, y, z)
 * @param excludePlayerIndex Player index to skip (-1 to include all players)
 * @param searchRadius Radius to search within (added to player's collision radius)
 * @return Player pointer if found, NULL otherwise
 */
Player *findVulnerablePlayerNearPosition(void *position, s32 excludePlayerIndex, s32 searchRadius) {
    Vec3i deltaPos;
    s32 combinedRadius;
    Allocation5AA90 *allocation;
    ListNode_5AA90 *node;
    Player *playerData;
    void *dataArray;
    u8 playerId;

    allocation = getCurrentAllocation();
    node = allocation->list;

    for (; node != NULL; node = node->next) {
        playerId = node->id;
        if (excludePlayerIndex == playerId) {
            continue;
        }

        dataArray = allocation->dataArray;
        playerData = (Player *)(playerId * 0xBE8 + (s32)dataArray);

        /* Skip players with collision disabled */
        if (playerData->unkB88 & 0x10) {
            continue;
        }

        /* Skip invincible players */
        if (playerData->invincibilityTimer != 0) {
            continue;
        }

        memcpy(&deltaPos, &node->localPos, 0xC);

        deltaPos.x += node->posPtr->x;
        deltaPos.y += node->posPtr->y;
        deltaPos.z += node->posPtr->z;

        deltaPos.x -= ((Vec3i *)position)->x;
        deltaPos.y -= ((Vec3i *)position)->y;
        deltaPos.z -= ((Vec3i *)position)->z;

        combinedRadius = node->radius + searchRadius;

        if (-combinedRadius < deltaPos.x && deltaPos.x < combinedRadius && -combinedRadius < deltaPos.y &&
            deltaPos.y < combinedRadius && -combinedRadius < deltaPos.z && deltaPos.z < combinedRadius) {
            s32 dist;

            dist = isqrt64((s64)deltaPos.x * deltaPos.x + (s64)deltaPos.y * deltaPos.y + (s64)deltaPos.z * deltaPos.z);

            if (dist < combinedRadius) {
                u8 index = node->id;
                return (Player *)((u8 *)allocation->dataArray + ((index * 3 * 128) - (index * 3)) * 8);
            }
        }
    }

    return NULL;
}

/**
 * Handles collision between an oriented cylindrical area and all players.
 * The area is defined by an origin point, a radial threshold (XY plane after rotation),
 * and a depth extent (Z axis). Players in front (positive Z) get knocked back,
 * players behind (negative Z) get pulled toward the origin.
 *
 * @param origin Center position of the oriented area
 * @param radialThreshold Radius threshold in the XY plane (after rotation)
 * @param depthExtent Forward/backward extent on the Z axis
 * @param rotationAngle Rotation angle for the oriented area
 */
void handleOrientedAreaCollision(Vec3i *origin, s32 radialThreshold, s32 depthExtent, s16 rotationAngle) {
    Vec3i deltaPos;
    Vec3i rotatedPos;
    s32 negAngle;
    s32 pad1;
    s32 pad2;
    s32 sp40;
    s32 sp44;
    GameState *allocation;
    s32 combinedRadius;
    s32 playerIndex;
    Player *player;
    s32 dist;

    allocation = (GameState *)getCurrentAllocation();
    for (playerIndex = 0; playerIndex < allocation->numPlayers; playerIndex++) {
        negAngle = (s16)-rotationAngle;

        player = &allocation->players[playerIndex];

        /* Use max of radialThreshold, depthExtent for combined radius */
        if (radialThreshold < depthExtent) {
            combinedRadius = depthExtent;
        } else {
            combinedRadius = radialThreshold;
        }
        combinedRadius += player->collisionRadius;

        /* Calculate delta position */
        deltaPos.x = (player->collisionOffset.x + player->worldPos.x) - origin->x;
        deltaPos.y = (player->collisionOffset.y + player->worldPos.y) - origin->y;
        deltaPos.z = (player->collisionOffset.z + player->worldPos.z) - origin->z;

        /* AABB check */
        if (deltaPos.x >= combinedRadius) {
            continue;
        }
        if (-combinedRadius >= deltaPos.x) {
            continue;
        }
        if (deltaPos.y >= combinedRadius) {
            continue;
        }
        if (-combinedRadius >= deltaPos.y) {
            continue;
        }
        if (deltaPos.z >= combinedRadius) {
            continue;
        }
        if (-combinedRadius >= deltaPos.z) {
            continue;
        }

        /* Rotate delta position by negative angle */
        rotateVectorY(&deltaPos, negAngle, &rotatedPos);

        /* Compute 2D distance in XY plane */
        dist = isqrt64((s64)rotatedPos.x * rotatedPos.x + (s64)rotatedPos.y * rotatedPos.y);

        if (dist >= radialThreshold) {
            continue;
        }

        /* Check z direction and apply push */
        if (rotatedPos.z >= 0) {
            /* Forward collision */
            if (rotatedPos.z >= depthExtent + player->collisionRadius) {
                continue;
            }

            deltaPos.y = 0;
            deltaPos.x = 0;
            deltaPos.z = (depthExtent + player->collisionRadius) - rotatedPos.z;

            rotateVectorY(&deltaPos, rotationAngle, &rotatedPos);

            player->worldPos.x += rotatedPos.x;
            player->worldPos.y += rotatedPos.y;
            player->worldPos.z += rotatedPos.z;

            setPlayerCollisionKnockbackState(player, negAngle, deltaPos.z);
        } else {
            /* Backward collision */
            if (-(depthExtent + player->collisionRadius) >= rotatedPos.z) {
                continue;
            }

            deltaPos.y = 0;
            deltaPos.x = 0;
            /* Pull uses player's rear collision radius at offset 0xB64 */
            deltaPos.z = -(depthExtent + *(s32 *)((u8 *)player + 0xB64)) - rotatedPos.z;

            rotateVectorY(&deltaPos, rotationAngle, &rotatedPos);

            player->worldPos.x += rotatedPos.x;
            player->worldPos.y += rotatedPos.y;
            player->worldPos.z += rotatedPos.z;

            setPlayerPullState(player, &rotatedPos);
        }
    }
}

/**
 * Checks 2D collision (xz-plane) between a point and all players.
 * Pushes players out of collision and may apply knockback.
 *
 * @param pos Position to check collision against
 * @param extraRadius Radius to add to player's collision radius
 * @param maxHeight Maximum height difference for collision
 * @param excludePlayerIdx Player index to skip (team check)
 * @return 1 if knockback was applied, 0 otherwise
 */
s32 checkPositionPlayerCollisionWithKnockback(Vec3i *pos, s32 extraRadius, s32 maxHeight, s16 excludePlayerIdx) {
    Vec3i deltaPos;
    GameState *allocation;
    s32 combinedRadius;
    s32 playerIndex;
    Player *targetPlayer;
    s32 dist;
    s32 negRadius;
    s32 deltaX, deltaZ;

    allocation = (GameState *)getCurrentAllocation();

    for (playerIndex = 0; playerIndex < allocation->numPlayers; playerIndex++) {
        targetPlayer = &allocation->players[playerIndex];

        /* Skip collision if players are on the same team */
        if (excludePlayerIdx == targetPlayer->playerIndex) {
            continue;
        }

        /* Copy target's collision box local position */
        memcpy(&deltaPos, &targetPlayer->collisionOffset, 0xC);

        /* Convert to world space */
        deltaPos.x += targetPlayer->worldPos.x;
        deltaPos.y += targetPlayer->worldPos.y;
        deltaPos.z += targetPlayer->worldPos.z;

        /* Calculate relative position */
        deltaPos.x -= pos->x;
        deltaPos.y -= pos->y;
        deltaPos.z -= pos->z;

        /* Height constraint check - must be above ground and within range */
        if (deltaPos.y <= 0) {
            continue;
        }
        if (deltaPos.y >= maxHeight) {
            continue;
        }

        /* Sum of both collision radii */
        combinedRadius = targetPlayer->collisionRadius + extraRadius;
        negRadius = -combinedRadius;

        /* Quick AABB check on xz plane */
        if (negRadius >= deltaPos.x || deltaPos.x >= combinedRadius) {
            continue;
        }
        if (negRadius >= deltaPos.z || deltaPos.z >= combinedRadius) {
            continue;
        }

        /* 2D distance check (xz plane only) */
        dist = isqrt64((s64)deltaPos.x * deltaPos.x + (s64)deltaPos.z * deltaPos.z);

        if (dist >= combinedRadius) {
            continue;
        }

        /* Avoid divide by zero */
        if (dist == 0) {
            dist = 1;
        }

        /* Calculate push vector (xz only) */
        deltaX = deltaPos.x;
        deltaZ = deltaPos.z;
        deltaPos.x = ((s64)deltaX * combinedRadius / dist) - deltaX;
        deltaPos.z = ((s64)deltaZ * combinedRadius / dist) - deltaZ;

        /* Skip push if target has flag 0x80 */
        if (targetPlayer->unkB84 & 0x80) {
            continue;
        }

        /* Apply push to target player */
        targetPlayer->worldPos.x += deltaPos.x;
        targetPlayer->worldPos.z += deltaPos.z;

        /* Check if push was significant enough for knockback */
        dist = isqrt64((s64)deltaPos.x * deltaPos.x + (s64)deltaPos.z * deltaPos.z);

        if (dist > 0x30000) {
            setPlayerCollisionKnockbackState(
                targetPlayer,
                computeAngleToPosition(pos->x, pos->z, targetPlayer->worldPos.x, targetPlayer->worldPos.z),
                dist
            );
            return 1;
        }
    }

    return 0;
}

/**
 * Checks 2D collision (xz-plane) between a point and all players.
 * Pushes players out of collision and may call setPlayerPullState.
 *
 * @param pos Position to check collision against (Vec3i*)
 * @param extraRadius Radius to add to player's collision radius
 * @param maxHeight Maximum height difference for collision
 * @return Always returns 0
 */
s32 checkPositionPlayerCollisionWithPull(void *pos, s32 extraRadius, s32 maxHeight) {
    Vec3i deltaPos;
    s64 unused;
    GameState *allocation;
    Vec3i *deltaPosPtr;
    s32 combinedRadius;
    s32 playerIndex;
    Player *targetPlayer;
    int new_var;
    s32 dist;
    s32 negRadius;
    s32 deltaX;
    s32 deltaZ;

    allocation = (GameState *)getCurrentAllocation();
    playerIndex = 0;
    new_var = 0;

    if ((s32)allocation->numPlayers > new_var) {
        deltaPosPtr = &deltaPos;
        do {
            targetPlayer = &allocation->players[playerIndex];

            memcpy(deltaPosPtr, &targetPlayer->collisionOffset, 0xC);

            deltaPos.x += targetPlayer->worldPos.x;
            deltaPos.y += targetPlayer->worldPos.y;
            deltaPos.z += targetPlayer->worldPos.z;

            deltaPos.x -= ((Vec3i *)pos)->x;
            deltaPos.y -= ((Vec3i *)pos)->y;
            deltaPos.z -= ((Vec3i *)pos)->z;

            if (deltaPos.y <= 0) {
                goto next;
            }
            if (deltaPos.y >= maxHeight) {
                goto next;
            }

            combinedRadius = targetPlayer->collisionRadius + extraRadius;
            negRadius = -combinedRadius;

            if ((negRadius >= deltaPos.x) || (deltaPos.x >= combinedRadius)) {
                goto next;
            }
            if ((negRadius >= deltaPos.z) || (deltaPos.z >= combinedRadius)) {
                goto next;
            }

            dist = isqrt64(((s64)deltaPos.x * deltaPos.x) + ((s64)deltaPos.z * deltaPos.z));

            if (dist >= combinedRadius) {
                goto next;
            }

            if (dist == 0) {
                dist = 1;
            }

            deltaX = deltaPos.x;
            deltaZ = deltaPos.z;
            deltaPos.x = (((s64)deltaX * combinedRadius) / dist) - deltaX;
            deltaPos.z = (((s64)deltaZ * combinedRadius) / dist) - deltaZ;

            if (targetPlayer->unkB84 & 0x80) {
                goto next;
            }

            targetPlayer->worldPos.x += deltaPos.x;
            targetPlayer->worldPos.z += deltaPos.z;

            dist = isqrt64(((s64)deltaPos.x * deltaPos.x) + ((s64)deltaPos.z * deltaPos.z));

            if (dist > 0x40000) {
                deltaPos.y = 0;
                setPlayerPullState(targetPlayer, deltaPosPtr);
            }
        next:
            playerIndex++;
        } while (playerIndex < allocation->numPlayers);
    }

    return 0;
}

INCLUDE_ASM("asm/nonmatchings/5AA90", func_8005BF50_5CB50);

/**
 * Checks collision between a position and vulnerable players (non-invincible, collision-enabled).
 * When a collision is detected, applies the star hit state to that player.
 *
 * @param pos Position to check collision against
 * @param excludePlayerIdx Player index to skip (-1 to include all players)
 * @param extraRadius Radius to add to player's collision radius
 * @return 1 if a collision was found and star hit state was set, 0 otherwise
 */
s32 checkStarHitCollisionWithVulnerablePlayers(Vec3i *pos, s32 excludePlayerIdx, s32 extraRadius) {
    Vec3i deltaPos;
    s32 combinedRadius;
    Allocation5AA90 *allocation;
    ListNode_5AA90 *node;
    Player *playerData;
    void *dataArray;
    s32 result;
    Vec3i *deltaPosPtr;

    allocation = getCurrentAllocation();
    node = allocation->list;
    result = 0;
    if (node != NULL) {
        deltaPosPtr = &deltaPos;
        do {
            u8 id = node->id;
            if (excludePlayerIdx == id) {
                ;
            } else {
                dataArray = allocation->dataArray;
                playerData = (Player *)(id * 0xBE8 + (s32)dataArray);

                if (playerData->unkB88 & 0x10) {
                    ;
                } else if (playerData->invincibilityTimer != 0) {
                    ;
                } else {
                    memcpy(deltaPosPtr, &node->localPos, 0xC);

                    deltaPos.x += node->posPtr->x;
                    deltaPos.y += node->posPtr->y;
                    deltaPos.z += node->posPtr->z;

                    deltaPos.x -= pos->x;
                    deltaPos.y -= pos->y;
                    deltaPos.z -= pos->z;

                    combinedRadius = node->radius + extraRadius;

                    if (-combinedRadius < deltaPos.x && deltaPos.x < combinedRadius && -combinedRadius < deltaPos.y &&
                        deltaPos.y < combinedRadius && -combinedRadius < deltaPos.z && deltaPos.z < combinedRadius) {
                        s32 dist;

                        dist = isqrt64(
                            (s64)deltaPos.x * deltaPos.x + (s64)deltaPos.y * deltaPos.y + (s64)deltaPos.z * deltaPos.z
                        );

                        if (dist < combinedRadius) {
                            u8 index = node->id;
                            result = 1;
                            setPlayerStarHitState(
                                (Player *)((u8 *)allocation->dataArray + ((index * 3 * 128) - (index * 3)) * 8),
                                deltaPosPtr
                            );
                        }
                    }
                }
            }
            node = node->next;
        } while (node != NULL);
    }

    return result;
}

/**
 * Finds a vulnerable player (non-invincible, collision-enabled) near a position.
 * Similar to findVulnerablePlayerNearPosition, but also returns the delta vector.
 *
 * @param position Position to search from
 * @param excludePlayerIdx Player index to skip
 * @param searchRadius Radius to add to player's collision radius
 * @param outDelta Output parameter for the delta vector (from position to player's collision sphere)
 * @return Player pointer if found, NULL otherwise
 */
Player *
findVulnerablePlayerNearPositionWithDelta(Vec3i *position, s32 excludePlayerIdx, s32 searchRadius, Vec3i *outDelta) {
    s32 combinedRadius;
    s32 negRadius;
    Allocation5AA90 *allocation;
    ListNode_5AA90 *node;
    u8 index;
    int new_var;

    allocation = (Allocation5AA90 *)getCurrentAllocation();

    for (node = allocation->list; node != 0; node = node->next) {
        index = node->id;
        if (excludePlayerIdx == index) {
            continue;
        };
        if (((PlayerCollisionData *)(((u8 *)allocation->dataArray) + (index * 0xBE8)))->collisionFlags & 0x10) {
            continue;
        }
        new_var = 0xC;
        if (((PlayerCollisionData *)(((u8 *)allocation->dataArray) + (index * 0xBE8)))->invincibilityTimer != 0) {
            continue;
        }
        memcpy(outDelta, &node->localPos, new_var);
        outDelta->x += node->posPtr->x;
        outDelta->y += node->posPtr->y;
        outDelta->z += node->posPtr->z;
        outDelta->x -= position->x;
        outDelta->y -= position->y;
        outDelta->z -= position->z;
        combinedRadius = node->radius + searchRadius;
        negRadius = -combinedRadius;
        if ((((((negRadius < outDelta->x) && (outDelta->x < combinedRadius)) && (negRadius < outDelta->y)) &&
              (outDelta->y < combinedRadius)) &&
             (negRadius < outDelta->z)) &&
            (outDelta->z < combinedRadius)) {
            if (distance_3d(outDelta->x, outDelta->y, outDelta->z) < combinedRadius) {
                return (Player *)(((u8 *)allocation->dataArray) + (node->id * 0xBE8));
            }
        }
    }

    return NULL;
}

s32 isPlayerInRangeAndPull(Vec3i *arg0, s32 arg1, Player *arg2) {
    Vec3i localVec;
    s32 unused1;
    s32 unused2;
    s32 combinedRadius;
    s32 negRadius;
    s32 dist;

    memcpy(&localVec, arg0, 0xC);

    localVec.x = localVec.x - (arg2->worldPos.x + arg2->collisionOffset.x);
    localVec.y = localVec.y - (arg2->worldPos.y + arg2->collisionOffset.y);
    localVec.z = localVec.z - (arg2->worldPos.z + arg2->collisionOffset.z);

    combinedRadius = arg1 + arg2->collisionRadius;
    negRadius = -combinedRadius;

    if (negRadius < localVec.x && localVec.x < combinedRadius && negRadius < localVec.y &&
        localVec.y < combinedRadius && negRadius < localVec.z && localVec.z < combinedRadius) {

        dist = isqrt64((s64)localVec.x * localVec.x + (s64)localVec.y * localVec.y + (s64)localVec.z * localVec.z);

        if (dist < combinedRadius) {
            if (dist == 0) {
                dist = 1;
            }

            localVec.x = ((s64)localVec.x * combinedRadius / dist) - localVec.x;
            localVec.y = ((s64)localVec.y * combinedRadius / dist) - localVec.y;
            localVec.z = ((s64)localVec.z * combinedRadius / dist) - localVec.z;

            arg2->worldPos.x -= localVec.x;
            arg2->worldPos.y -= localVec.y;
            arg2->worldPos.z -= localVec.z;

            return 1;
        }
    }

    return 0;
}

void addCollisionSectorNodeToList(ListNode_5AA90 *arg0) {
    Allocation5AA90 *alloc = (Allocation5AA90 *)getCurrentAllocation();
    arg0->next = alloc->list;
    alloc->list = arg0;
}

INCLUDE_ASM("asm/nonmatchings/5AA90", func_8005C868_5D468);

s16 getPlayerTargetTrackAngle(Player *player) {
    GameState *gameState;
    Section3Element *sectorEntry;
    s16 targetAngle;

    gameState = (GameState *)getCurrentAllocation();
    sectorEntry = (Section3Element *)(player->sectorIndex * 0x24 + (u32)gameState->gameData.section3Data);

    if (sectorEntry->unk0 < 0) {
        if (gameState->finalLapNumber == player->currentLap) {
            Section1Element *waypointData = (Section1Element *)gameState->gameData.section1Data;
            Section1Element *waypoint1 = (Section1Element *)(sectorEntry->unk1C * 6 + (u32)waypointData);
            Section1Element *waypoint2 = (Section1Element *)(sectorEntry->unk16 * 6 + (u32)waypointData);
            targetAngle =
                computeAngleToPosition(waypoint1->unk0, waypoint1->unk4, waypoint2->unk0, waypoint2->unk4) + 0x800;
        } else {
            LevelConfig *levelConfig = getLevelConfig(gameState->memoryPoolId);
            targetAngle = computeAngleToPosition(
                levelConfig->shortcutPosX,
                levelConfig->shortcutPosZ,
                player->worldPos.x,
                player->worldPos.z
            );
            if (player->unkB84 & 2) {
                targetAngle += 0x1000;
            }
        }
    } else {
        Section1Element *waypointData = (Section1Element *)gameState->gameData.section1Data;
        Section1Element *waypoint1 = (Section1Element *)(sectorEntry->unk16 * 6 + (u32)waypointData);
        Section1Element *waypoint2 = (Section1Element *)(sectorEntry->unk1C * 6 + (u32)waypointData);
        targetAngle = computeAngleToPosition(waypoint1->unk0, waypoint1->unk4, waypoint2->unk0, waypoint2->unk4);
        if (player->unkB84 & 2) {
            targetAngle += 0x1000;
        }
    }
    return targetAngle;
}
