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
    trackHeight = func_8005D020_5DC20(gameData, newSectorIndex, playerPos, 0x200000);
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
    D_80090F90_91B90_item *levelItem;
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
    levelItem = func_80055D10_56910(gameState->memoryPoolId);

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
    D_80090F90_91B90_item *levelItem;
    s32 deltaX, deltaZ;
    s64 distSq;

    gameState = (GameState *)getCurrentAllocation();

    if (getTrackSegmentFinishZoneFlag(&gameState->gameData, player->sectorIndex) == 0) {
        levelItem = func_80055D10_56910(gameState->memoryPoolId);

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
 */
void handleCollisionWithTargetPlayer(Player *player) {
    Vec3i deltaPos;
    u8 pad[0x8];
    void *collisionBoxPtr;
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
        collisionBoxPtr = targetPlayer;
        do {
            /* Copy target's collision box local position (offset 0xAE4 is unkAE4 array) */
            memcpy(&deltaPos, (u8 *)collisionBoxPtr + 0xAE4, 0xC);

            /* Convert to world space */
            deltaPos.x += targetPlayer->worldPos.x;
            deltaPos.y += targetPlayer->worldPos.y;
            deltaPos.z += targetPlayer->worldPos.z;

            /* Calculate relative position to player's collision box */
            deltaPos.x -= player->worldPos.x + player->collisionOffset.x;
            deltaPos.y -= player->worldPos.y + player->collisionOffset.y;
            deltaPos.z -= player->worldPos.z + player->collisionOffset.z;

            /* Sum of both collision box radii */
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
                                func_8006D21C_6DE1C(
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
            collisionBoxPtr = (u8 *)collisionBoxPtr + 0xC;
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

void func_8005B730_5C330(Vec3i *arg0, s32 arg1, s32 arg2, s16 arg3) {
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
    s32 numPlayersToCheck;

    allocation = (GameState *)getCurrentAllocation();
    for (playerIndex = 0; playerIndex < allocation->numPlayers; playerIndex++) {
        negAngle = (s16)-arg3;

        player = &allocation->players[playerIndex];

        /* Use max of arg1, arg2 for combined radius */
        if (arg1 < arg2) {
            combinedRadius = arg2;
        } else {
            combinedRadius = arg1;
        }
        combinedRadius += player->collisionRadius;

        /* Calculate delta position */
        deltaPos.x = (player->collisionOffset.x + player->worldPos.x) - arg0->x;
        deltaPos.y = (player->collisionOffset.y + player->worldPos.y) - arg0->y;
        deltaPos.z = (player->collisionOffset.z + player->worldPos.z) - arg0->z;

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

        if (dist >= arg1) {
            continue;
        }

        /* Check z direction and apply push */
        if (rotatedPos.z >= 0) {
            /* Forward collision */
            if (rotatedPos.z >= arg2 + player->collisionRadius) {
                continue;
            }

            deltaPos.y = 0;
            deltaPos.x = 0;
            deltaPos.z = (arg2 + player->collisionRadius) - rotatedPos.z;

            rotateVectorY(&deltaPos, arg3, &rotatedPos);

            player->worldPos.x += rotatedPos.x;
            player->worldPos.y += rotatedPos.y;
            player->worldPos.z += rotatedPos.z;

            setPlayerCollisionKnockbackState(player, negAngle, deltaPos.z);
        } else {
            /* Backward collision - condition uses collisionRadius */
            if (-(arg2 + player->collisionRadius) >= rotatedPos.z) {
                continue;
            }

            deltaPos.y = 0;
            deltaPos.x = 0;
            /* Push calculation uses field at offset 0xB64 */
            deltaPos.z = -(arg2 + *(s32 *)((u8 *)player + 0xB64)) - rotatedPos.z;

            rotateVectorY(&deltaPos, arg3, &rotatedPos);

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
 * @param arg0 Position to check collision against
 * @param arg1 Radius to add to player's collision radius
 * @param arg2 Maximum height difference for collision
 * @param arg3 Player index to skip (team check)
 * @return 1 if knockback was applied, 0 otherwise
 */
s32 func_8005B9E4_5C5E4(Vec3i *arg0, s32 arg1, s32 arg2, s16 arg3) {
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
        if (arg3 == targetPlayer->playerIndex) {
            continue;
        }

        /* Copy target's collision box local position */
        memcpy(&deltaPos, &targetPlayer->collisionOffset, 0xC);

        /* Convert to world space */
        deltaPos.x += targetPlayer->worldPos.x;
        deltaPos.y += targetPlayer->worldPos.y;
        deltaPos.z += targetPlayer->worldPos.z;

        /* Calculate relative position */
        deltaPos.x -= arg0->x;
        deltaPos.y -= arg0->y;
        deltaPos.z -= arg0->z;

        /* Height constraint check - must be above ground and within range */
        if (deltaPos.y <= 0) {
            continue;
        }
        if (deltaPos.y >= arg2) {
            continue;
        }

        /* Sum of both collision radii */
        combinedRadius = targetPlayer->collisionRadius + arg1;
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
                func_8006D21C_6DE1C(arg0->x, arg0->z, targetPlayer->worldPos.x, targetPlayer->worldPos.z),
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
 * @param arg0 Position to check collision against (Vec3i*)
 * @param arg1 Radius to add to player's collision radius
 * @param arg2 Maximum height difference for collision
 * @return Always returns 0
 */
s32 func_8005BCB8_5C8B8(void *arg0, s32 arg1, s32 arg2) {
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

            deltaPos.x -= ((Vec3i *)arg0)->x;
            deltaPos.y -= ((Vec3i *)arg0)->y;
            deltaPos.z -= ((Vec3i *)arg0)->z;

            if (deltaPos.y <= 0) {
                goto next;
            }
            if (deltaPos.y >= arg2) {
                goto next;
            }

            combinedRadius = targetPlayer->collisionRadius + arg1;
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

s32 func_8005C250_5CE50(Vec3i *arg0, s32 arg1, s32 arg2) {
    Vec3i pos;
    s32 combinedRadius;
    Allocation5AA90 *allocation;
    ListNode_5AA90 *node;
    Player *playerData;
    void *dataArray;
    s32 result;
    Vec3i *posPtr;

    allocation = getCurrentAllocation();
    node = allocation->list;
    result = 0;
    if (node != NULL) {
        posPtr = &pos;
        do {
            u8 id = node->id;
            if (arg1 == id) {
                ;
            } else {
                dataArray = allocation->dataArray;
                playerData = (Player *)(id * 0xBE8 + (s32)dataArray);

                if (playerData->unkB88 & 0x10) {
                    ;
                } else if (playerData->invincibilityTimer != 0) {
                    ;
                } else {
                    memcpy(posPtr, &node->localPos, 0xC);

                    pos.x += node->posPtr->x;
                    pos.y += node->posPtr->y;
                    pos.z += node->posPtr->z;

                    pos.x -= arg0->x;
                    pos.y -= arg0->y;
                    pos.z -= arg0->z;

                    combinedRadius = node->radius + arg2;

                    if (-combinedRadius < pos.x && pos.x < combinedRadius && -combinedRadius < pos.y &&
                        pos.y < combinedRadius && -combinedRadius < pos.z && pos.z < combinedRadius) {
                        s32 dist;

                        dist = isqrt64((s64)pos.x * pos.x + (s64)pos.y * pos.y + (s64)pos.z * pos.z);

                        if (dist < combinedRadius) {
                            u8 index = node->id;
                            result = 1;
                            setPlayerStarHitState(
                                (Player *)((u8 *)allocation->dataArray + ((index * 3 * 128) - (index * 3)) * 8),
                                posPtr
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

typedef struct {
    u8 padding[0xB88];
    s32 unkB88;
    u8 padding3[0x18];
    u16 invincibilityTimer;
} PlayerData;

void *func_8005C454_5D054(Vec3i *arg0, s32 arg1, s32 arg2, Vec3i *arg3) {
    s32 combinedRadius;
    s32 negRadius;
    Allocation5AA90 *allocation;
    ListNode_5AA90 *node;
    PlayerData *playerData;
    u8 index;
    int new_var;
    allocation = (Allocation5AA90 *)getCurrentAllocation();
    for (node = allocation->list; node != 0; node = node->next) {
        index = node->id;
        if (arg1 == index) {
            continue;
        };
        if (((PlayerData *)(((u8 *)allocation->dataArray) + (index * 0xBE8)))->unkB88 & 0x10) {
            continue;
        }
        new_var = 0xC;
        if (((PlayerData *)(((u8 *)allocation->dataArray) + (index * 0xBE8)))->invincibilityTimer != 0) {
            continue;
        }
        memcpy(arg3, &node->localPos, new_var);
        arg3->x += node->posPtr->x;
        arg3->y += node->posPtr->y;
        arg3->z += node->posPtr->z;
        arg3->x -= arg0->x;
        arg3->y -= arg0->y;
        arg3->z -= arg0->z;
        combinedRadius = node->radius + arg2;
        negRadius = -combinedRadius;
        if ((((((negRadius < arg3->x) && (arg3->x < combinedRadius)) && (negRadius < arg3->y)) &&
              (arg3->y < combinedRadius)) &&
             (negRadius < arg3->z)) &&
            (arg3->z < combinedRadius)) {
            if (distance_3d(arg3->x, arg3->y, arg3->z) < combinedRadius) {
                return ((u8 *)allocation->dataArray) + (node->id * 0xBE8);
            }
        }
    }

    return 0;
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

void func_8005C838_5D438(ListNode_5AA90 *arg0) {
    Allocation5AA90 *alloc = (Allocation5AA90 *)getCurrentAllocation();
    arg0->next = alloc->list;
    alloc->list = arg0;
}

INCLUDE_ASM("asm/nonmatchings/5AA90", func_8005C868_5D468);

s16 func_8005CE98_5DA98(Player *arg0) {
    GameState *allocation;
    Section3Element *elem;
    s16 result;

    allocation = (GameState *)getCurrentAllocation();
    elem = (Section3Element *)(arg0->sectorIndex * 0x24 + (u32)allocation->gameData.section3Data);

    if (elem->unk0 < 0) {
        if (allocation->finalLapNumber == arg0->currentLap) {
            Section1Element *section1Data = (Section1Element *)allocation->gameData.section1Data;
            Section1Element *v1 = (Section1Element *)(elem->unk1C * 6 + (u32)section1Data);
            Section1Element *v2 = (Section1Element *)(elem->unk16 * 6 + (u32)section1Data);
            result = func_8006D21C_6DE1C(v1->unk0, v1->unk4, v2->unk0, v2->unk4) + 0x800;
        } else {
            D_80090F90_91B90_item *item = func_80055D10_56910(allocation->memoryPoolId);
            result = func_8006D21C_6DE1C(item->shortcutPosX, item->shortcutPosZ, arg0->worldPos.x, arg0->worldPos.z);
            if (arg0->unkB84 & 2) {
                result += 0x1000;
            }
        }
    } else {
        Section1Element *section1Data = (Section1Element *)allocation->gameData.section1Data;
        Section1Element *v1 = (Section1Element *)(elem->unk16 * 6 + (u32)section1Data);
        Section1Element *v2 = (Section1Element *)(elem->unk1C * 6 + (u32)section1Data);
        result = func_8006D21C_6DE1C(v1->unk0, v1->unk4, v2->unk0, v2->unk4);
        if (arg0->unkB84 & 2) {
            result += 0x1000;
        }
    }
    return result;
}
