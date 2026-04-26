#include "race/track_collision.h"
#include "common.h"
#include "data/course_data.h"
#include "gamestate.h"
#include "graphics/displaylist.h"
#include "math/geometry.h"
#include "race/hit_reactions.h"
#include "race/race_session.h"
#include "race/track_height.h"
#include "system/task_scheduler.h"

typedef struct {
    u8 _pad[0x10];
    /* 0x10 */ Player *dataArray;
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
    if (!(player->animFlags & 0x100)) {
        return findTrackSector(gameData, currentSectorIndex, position);
    }
    return player->sectorIndex;
}

typedef struct {
    s32 value;
    u8 pad[12];
} CollisionThresholdEntry;

extern s16 gTrackCollisionSampleOffsets[][8];
extern CollisionThresholdEntry gTrackCollisionThresholds[];
extern s16 trackAlignmentQuadPoints[];
extern s16 trackNormalSamplePoints[];
extern s16 slopeDetectionSamplePoints[];

/**
 * Checks track wall collision at 3 sample points around the player and pushes them out of walls.
 * Returns -1 if a collision occurred, 0 otherwise.
 *
 * When the player is in a stunned state (animFlags & 0x20), the collision threshold
 * increases over time to allow the player to pass through walls while recovering.
 */
s32 handlePlayerTrackWallCollision(Player *player) {
    Vec3i pushOffset;
    Vec3i samplePoints[3];
    Transform3D combinedTransform;
    Transform3D groundTransform;
    GameState *gameState;
    s32 result;
    s32 i;
    s32 collisionResult;
    u8 stunCounter;
    s32 sectorIdx;
    s32 j;

    result = 0;
    gameState = (GameState *)getCurrentAllocation();
    func_8006B084_6BC84(&player->orientationTransform, &player->headingTransform, &groundTransform);
    func_8006B084_6BC84((Transform3D *)&player->tiltTransform, &groundTransform, &combinedTransform);

    i = 0;
    do {
        transformVector(gTrackCollisionSampleOffsets[i], combinedTransform.m[0], &samplePoints[i]);
        i++;
    } while (i < 3);

    i = 0;
    do {
        sectorIdx =
            getOrUpdatePlayerSectorIndex(player, &gameState->gameData, player->sectorIndex, &samplePoints[i]) & 0xFFFF;
        if (i == 0) {
            if (player->animFlags & 0x20) {
                stunCounter = player->stunCollisionCounter;
                if (stunCounter < 10U) {
                    player->stunCollisionCounter = stunCounter + 1;
                }
                collisionResult = func_80060CDC_618DC(
                    &gameState->gameData,
                    sectorIdx,
                    &samplePoints[0],
                    (player->stunCollisionCounter + 6) << 16,
                    &pushOffset
                );
                player->animFlags &= ~0x20;
            } else {
                player->stunCollisionCounter = 0;
                collisionResult = func_80060CDC_618DC(
                    &gameState->gameData,
                    sectorIdx,
                    &samplePoints[0],
                    gTrackCollisionThresholds[0].value,
                    &pushOffset
                );
            }
            for (j = i; j < 3; j++) {
                samplePoints[j].x += pushOffset.x;
                samplePoints[j].z += pushOffset.z;
            }
            player->worldPos.x += pushOffset.x;
            player->worldPos.z += pushOffset.z;
            if (collisionResult == -1) {
                result = -1;
            }
            if (collisionResult == -2) {
                if (samplePoints[i].y < getTrackHeightInSector(
                                            &gameState->gameData,
                                            getOrUpdatePlayerSectorIndex(
                                                player,
                                                &gameState->gameData,
                                                player->sectorIndex,
                                                &samplePoints[i]
                                            ) & 0xFFFF,
                                            &samplePoints[i],
                                            0x100000
                                        ) + 0x100000) {
                    result = -1;
                }
            }
        } else {
            collisionResult = func_80060CDC_618DC(
                &gameState->gameData,
                sectorIdx,
                &samplePoints[i],
                gTrackCollisionThresholds[i].value,
                &pushOffset
            );
            if (collisionResult == -1) {
                result = -1;
                for (j = i; j < 3; j++) {
                    samplePoints[j].x += pushOffset.x;
                    samplePoints[j].z += pushOffset.z;
                }
                player->worldPos.x += pushOffset.x;
                player->worldPos.z += pushOffset.z;
            }
            if (collisionResult == -2) {
                if (samplePoints[i].y < getTrackHeightInSector(
                                            &gameState->gameData,
                                            getOrUpdatePlayerSectorIndex(
                                                player,
                                                &gameState->gameData,
                                                player->sectorIndex,
                                                &samplePoints[i]
                                            ) & 0xFFFF,
                                            &samplePoints[i],
                                            0x100000
                                        ) + 0x100000) {
                    result = -1;
                }
                for (j = i; j < 3; j++) {
                    samplePoints[j].x += pushOffset.x;
                    samplePoints[j].z += pushOffset.z;
                }
                player->worldPos.x += pushOffset.x;
                player->worldPos.z += pushOffset.z;
            }
        }
        i++;
    } while (i < 3);
    return result;
}

void alignPlayerToTrackSurface(Player *player) {
    s32 heights[6];
    Vec3i points[6];
    Vec3i normals[6];
    Transform3D spC0;
    Transform3D spE0;
    Allocation5AA90 *allocation;
    s32 i;
    s32 contactMask;
    s32 pushUpOffset;
    s32 magnitude;
    s32 sign;
    s32 lowBits;
    s16 angle;
    s16 prevPitchAngle;
    s32 adjustedY;
    s32 dx;
    s32 dz;

    allocation = getCurrentAllocation();
    func_8006B084_6BC84(&player->orientationTransform, &player->headingTransform, &spE0);
    func_8006B084_6BC84((Transform3D *)&player->tiltTransform, &spE0, &spC0);

    if (player->animFlags & 0x40) {
        player->animFlags &= ~0x40;
        for (i = 0; i < 2; i++) {
            transformVector(&slopeDetectionSamplePoints[i * 6], player->headingTransform.m[0], &points[i]);
        }
        for (i = 0; i < 2; i++) {
            heights[i] = getTrackHeightInSector(
                &allocation->unk30,
                getOrUpdatePlayerSectorIndex(player, &allocation->unk30, player->sectorIndex, &points[i]) & 0xFFFF,
                &points[i],
                0x100000
            );
        }
        player->rollAngle = atan2Fixed(heights[1] - heights[0], -0xC0000) & 0x1FFF;
        if (player->rollAngle > 0x1000) {
            player->rollAngle -= 0x2000;
        }
        createZRotationMatrix((Transform3D *)&player->tiltTransform, (u16)player->rollAngle);
        func_8006B084_6BC84((Transform3D *)&player->tiltTransform, &spE0, &spC0);
    }

    pushUpOffset = 0;
    for (pushUpOffset = 0; pushUpOffset < 3; pushUpOffset++) {
        prevPitchAngle = player->pitchAngle;
        contactMask = 0;
        for (i = 0; i < 4; i++) {
            transformVector(&trackAlignmentQuadPoints[i * 6], spC0.m[0], &points[i]);
            heights[i] = getTrackHeightInSector(
                &allocation->unk30,
                getOrUpdatePlayerSectorIndex(player, &allocation->unk30, player->sectorIndex, &points[i]) & 0xFFFF,
                &points[i],
                0x100000
            );
        }

        if (points[0].y < heights[0]) {
            contactMask = 1;
            points[1].y += heights[0] - points[0].y;
            points[0].y = heights[0];
        }
        if (points[1].y < heights[1]) {
            contactMask = 1;
            points[0].y += heights[1] - points[1].y;
        }
        if (points[2].y < heights[2]) {
            contactMask = 1;
            points[3].y += heights[2] - points[2].y;
            points[2].y = heights[2];
        }
        if (points[3].y < heights[3]) {
            contactMask = 1;
            points[2].y += heights[3] - points[3].y;
        }

        if (contactMask << 16) {
            dx = points[2].x - points[0].x;
            dz = points[2].z - points[0].z;

            player->pitchAngle = atan2Fixed(points[0].y - points[2].y, -isqrt64((s64)dx * dx + (s64)dz * dz));
            createCombinedRotationMatrix(&player->orientationTransform, player->pitchAngle, player->steeringAngle);
            func_8006B084_6BC84(&player->orientationTransform, &player->headingTransform, &spE0);
            func_8006B084_6BC84((Transform3D *)&player->tiltTransform, &spE0, &spC0);
            if (player->pitchAngle == prevPitchAngle) {
                break;
            }
        } else {
            break;
        }
    }

    pushUpOffset = 0;
    for (i = 0; i < 4; i++) {
        transformVector(&trackAlignmentQuadPoints[i * 6], spC0.m[0], &points[i]);
        heights[i] = getTrackHeightInSectorWithOffset(
            &allocation->unk30,
            getOrUpdatePlayerSectorIndex(player, &allocation->unk30, player->sectorIndex, &points[i]),
            &points[i],
            0x100000,
            (s32)&normals[i]
        );
        adjustedY = points[i].y + pushUpOffset;
        if (adjustedY < heights[i]) {
            pushUpOffset += heights[i] - adjustedY;
        }
    }

    dx = 0;
    for (i = 4; i < 6; i++) {
        transformVector(&trackNormalSamplePoints[dx * 6], spC0.m[0], &points[i]);
        heights[i] = getTrackHeightInSectorWithOffset(
            &allocation->unk30,
            getOrUpdatePlayerSectorIndex(player, &allocation->unk30, player->sectorIndex, &points[i]),
            &points[i],
            0x100000,
            (s32)&normals[i]
        );
        dx++;
    }

    contactMask = 0;
    player->surfaceNormalX = 0;
    player->surfaceNormalY = 0;
    player->surfaceNormalZ = 0;
    for (i = 0; i < 6; i++) {
        adjustedY = points[i].y;
        adjustedY -= heights[i];
        lowBits = (adjustedY & 0xFFFF) * spE0.m[1][1];
        adjustedY = (adjustedY >> 16) * (spE0.m[1][1] << 3);
        if (lowBits < 0) {
            lowBits += (1 << 13) - 1;
        }
        adjustedY += lowBits >> 13;
        if (adjustedY < 0x24000) {
            player->surfaceNormalX += normals[i].x;
            player->surfaceNormalY += normals[i].y;
            contactMask |= 1 << i;
            player->surfaceNormalZ += normals[i].z;
        }
    }

    player->animFlags |= 1;
    if ((contactMask & 3) && (contactMask & 0x3C) || (contactMask & 0x30) && (contactMask & 0xC)) {
        player->animFlags &= ~1;
        if (pushUpOffset > 0) {
            player->worldPos.y += pushUpOffset;
        }

        magnitude = isqrt64(
            (s64)player->surfaceNormalX * player->surfaceNormalX +
            (s64)player->surfaceNormalY * player->surfaceNormalY + (s64)player->surfaceNormalZ * player->surfaceNormalZ
        );
        sign = magnitude >> 31;
        player->surfaceNormalX = (s64)player->surfaceNormalX * 0x2000 / (s64)magnitude;
        player->surfaceNormalY = (s64)player->surfaceNormalY * 0x2000 / (s64)magnitude;
        player->surfaceNormalZ = (s64)player->surfaceNormalZ * 0x2000 / (s64)magnitude;
    } else {
        player->surfaceNormalX = 0;
        player->surfaceNormalY = 0x2000;
        player->surfaceNormalZ = 0;
    }
}

/**
 * Updates the player's sector index based on their current position and
 * clamps their Y position to the track height.
 *
 * @param player The player to update
 */
void updatePlayerSectorAndClampYToTrack(Player *player) {
    GameState *gameState;
    TrackGeometryFaceData *gameData;
    Vec3i *playerPos;
    u16 newSectorIndex;
    s32 trackHeight;

    gameState = (GameState *)getCurrentAllocation();
    gameData = (TrackGeometryFaceData *)&gameState->gameData;
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
s32 tryActivateShortcut(Player *player) {
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
        if (player->behaviorFlags & 0x10) {
            continue;
        }
        if (targetPlayer->behaviorFlags & 0x10) {
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
                if (player->animFlags & 0x80) {
                    continue;
                }

                /* If target has flag 0x80, do full push */
                if (targetPlayer->animFlags & 0x80) {
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
 * Checks collision between `player` and the boss player at slot 1.
 *
 * The boss has up to (s8)targetPlayer->unkBB4 collision boxes. Box 0 is the player's
 * own collisionOffset/collisionRadius; boxes 1+ live in the extraCollisionOffsets /
 * extraCollisionRadii arrays. The loop walks `boxBasePtr` along the offset array
 * by sizeof(Vec3i) each iteration so that boxBasePtr + 0xAE4 yields
 * &extraCollisionOffsets[boxIndex].
 *
 * flyingAttackState selects an "active hit" region on the boss:
 *   - 1: boxes 4-5 bounce the player back
 *   - 2: box 0 applies a knockback
 *   - 3: boxes 1-2 bounce the player back
 * Other collisions just push `player` out of the box.
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

    /* Don't collide a player against itself. */
    if (player->playerIndex == targetPlayer->playerIndex) {
        return;
    }
    /* Either party can disable collisions via behaviorFlags & 0x10. */
    if (player->behaviorFlags & 0x10) {
        return;
    }
    if (targetPlayer->behaviorFlags & 0x10) {
        return;
    }

    if ((s8)targetPlayer->unkBB4 > 0) {
        boxIndex = 0;
        boxBasePtr = targetPlayer;
        do {
            /* &targetPlayer->extraCollisionOffsets[boxIndex] (boxBasePtr advances by sizeof(Vec3i)). */
            memcpy(&deltaPos, (u8 *)boxBasePtr + 0xAE4, sizeof(Vec3i));

            deltaPos.x += targetPlayer->worldPos.x;
            deltaPos.y += targetPlayer->worldPos.y;
            deltaPos.z += targetPlayer->worldPos.z;

            deltaPos.x -= player->worldPos.x + player->collisionOffset.x;
            deltaPos.y -= player->worldPos.y + player->collisionOffset.y;
            deltaPos.z -= player->worldPos.z + player->collisionOffset.z;

            combinedRadius = (&targetPlayer->extraCollisionRadii)[boxIndex] + player->collisionRadius;
            negRadius = -combinedRadius;

            if (negRadius < deltaPos.x && deltaPos.x < combinedRadius && negRadius < deltaPos.y &&
                deltaPos.y < combinedRadius && negRadius < deltaPos.z && deltaPos.z < combinedRadius) {

                dist =
                    isqrt64((s64)deltaPos.x * deltaPos.x + (s64)deltaPos.y * deltaPos.y + (s64)deltaPos.z * deltaPos.z);

                if (dist < combinedRadius) {
                    /* State 1: boxes 4-5 are an active hit zone. */
                    if (targetPlayer->flyingAttackState == 1) {
                        if ((targetPlayer->animFlags & 0x40000) && (u32)(boxIndex - 4) < 2U) {
                            setPlayerBouncedBackState(player);
                            goto next;
                        }
                    }
                    /* State 3: boxes 1-2 are an active hit zone. */
                    if (targetPlayer->flyingAttackState == 3 && (targetPlayer->animFlags & 0x40000) &&
                        (u32)(boxIndex - 1) < 2U) {
                        setPlayerBouncedBackState(player);
                        goto next;
                    }

                    if (dist == 0) {
                        dist = 1;
                    }

                    deltaPos.x = ((s64)deltaPos.x * combinedRadius / dist) - deltaPos.x;
                    deltaPos.y = ((s64)deltaPos.y * combinedRadius / dist) - deltaPos.y;
                    deltaPos.z = ((s64)deltaPos.z * combinedRadius / dist) - deltaPos.z;

                    player->worldPos.x -= deltaPos.x;
                    player->worldPos.y -= deltaPos.y;
                    player->worldPos.z -= deltaPos.z;

                    /* State 2: main box hit -> apply knockback if push was significant. */
                    if ((targetPlayer->flyingAttackState == 2) & (boxIndex == 0)) {
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
            boxBasePtr = (u8 *)boxBasePtr + sizeof(Vec3i);
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
 * - Players with collision disabled (behaviorFlags & 0x10)
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
        if (playerData->behaviorFlags & 0x10) {
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

        /* Calculate delta position from origin to player's collision sphere */
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

        /* Rotate delta position by inverse angle to align with oriented area */
        rotateVectorY(&deltaPos, negAngle, &rotatedPos);

        /* Compute 2D distance in XY plane */
        dist = isqrt64((s64)rotatedPos.x * rotatedPos.x + (s64)rotatedPos.y * rotatedPos.y);

        if (dist >= radialThreshold) {
            continue;
        }

        /* Check z direction and apply push/pull */
        if (rotatedPos.z >= 0) {
            /* Forward collision - knockback */
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
            /* Backward collision - pull */
            if (-(depthExtent + player->collisionRadius) >= rotatedPos.z) {
                continue;
            }

            deltaPos.y = 0;
            deltaPos.x = 0;
            deltaPos.z = -(depthExtent + player->collisionListNode.radius) - rotatedPos.z;

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
        if (targetPlayer->animFlags & 0x80) {
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

            if (targetPlayer->animFlags & 0x80) {
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

/**
 * Finds a homing target for projectiles and returns the angle offset to turn toward them.
 *
 * Searches for vulnerable (non-invincible) players within a forward-facing cone,
 * marks them with targeting flags on pathFlags, and returns the angle adjustment
 * needed to aim at the closest target.
 *
 * @param pos Center position to search from
 * @param facingAngle Current facing angle of the projectile
 * @param excludePlayerIdx Player index to exclude from targeting (typically the shooter)
 * @param searchRadius Maximum distance to search for targets
 * @param closeRangeThreshold Distance threshold for marking target as "close range"
 * @return Angle offset to turn toward closest target, or 0 if no target found
 */
s16 getHomingAngleToTarget(
    Vec3i *pos,
    s16 facingAngle,
    s32 excludePlayerIdx,
    s32 searchRadius,
    s32 closeRangeThreshold
) {
    Vec3i deltaPos;
    s16 closestAngle;
    s32 closestDist;
    s16 closestPlayerId;
    s16 angleDiff;
    Allocation5AA90 *allocation;
    ListNode_5AA90 *node;

    closestAngle = 0;
    allocation = (Allocation5AA90 *)getCurrentAllocation();
    closestDist = searchRadius + 10;
    closestPlayerId = -1;

    for (node = allocation->list; node != NULL; node = node->next) {
        if (excludePlayerIdx != node->id && allocation->dataArray[node->id].invincibilityTimer == 0) {
            memcpy(&deltaPos, &node->localPos, 0xC);

            deltaPos.x += node->posPtr->x;
            deltaPos.y += node->posPtr->y;
            deltaPos.z += node->posPtr->z;

            deltaPos.x -= pos->x;
            deltaPos.y -= pos->y;
            deltaPos.z -= pos->z;

            if (deltaPos.x < searchRadius && -searchRadius < deltaPos.x && deltaPos.y < searchRadius &&
                -searchRadius < deltaPos.y && deltaPos.z < searchRadius && -searchRadius < deltaPos.z) {
                angleDiff = atan2Fixed(deltaPos.x, deltaPos.z);
                angleDiff = (angleDiff - facingAngle) & 0x1FFF;

                if ((angleDiff - 0x800) >= 0x1001U) {
                    s32 dist = isqrt64(
                        (s64)deltaPos.x * deltaPos.x + (s64)deltaPos.y * deltaPos.y + (s64)deltaPos.z * deltaPos.z
                    );

                    if (dist < searchRadius) {
                        if (dist <= 0x13FFFFF) {
                            allocation->dataArray[node->id].pathFlags |= 2;
                        } else {
                            allocation->dataArray[node->id].pathFlags |= 1;
                        }

                        if (dist < closestDist) {
                            closestAngle = angleDiff;
                            closestDist = dist;
                            closestPlayerId = node->id;
                        }
                    }
                }
            }
        }
    }

    if (closestPlayerId >= 0 && closestDist < closeRangeThreshold * 4 + closeRangeThreshold / 2) {
        allocation->dataArray[closestPlayerId].pathFlags |= 8;
    }

    return closestAngle;
}

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

                if (playerData->behaviorFlags & 0x10) {
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

typedef struct {
    u8 padding[0xB88];
    s32 collisionFlags;
    u8 padding2[0x18];
    u16 invincibilityTimer;
} PlayerCollisionData;

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

extern s16 terrainSamplePattern_State1[3][6];
extern s16 terrainSamplePattern_State2[3][6];
extern s16 terrainSamplePattern_State3[3][6];

void computePlayerTerrainAlignment(Player *player) {
    Vec3i normal;
    Vec3i rotatedNormal;
    Vec3i samplePoints[3];
    Transform3D combinedMatrix;
    Transform3D headingOrientationMatrix;
    GameState *gameState;
    s32 groundHeight;
    s16(*samplePattern)[6];
    s32 i;
    s32 sampledHeight;
    u8 attackState;
    s64 sampleDistSq;
    s32 length;
    s16 pitchAngle, rollAngle;

    player->animFlags |= 1;

    gameState = (GameState *)getCurrentAllocation();

    groundHeight = getTrackHeightInSector(&gameState->gameData, player->sectorIndex, &player->worldPos, 0x100000);

    if (!(groundHeight < player->worldPos.y)) {
        player->worldPos.y = groundHeight;
        player->animFlags &= ~1;
    }

    memcpy(&player->headingTransform.translation, &player->worldPos, sizeof(Vec3i));

    createYRotationMatrix(&player->headingTransform, player->rotY);

    func_8006B084_6BC84(&player->orientationTransform, &player->headingTransform, &headingOrientationMatrix);
    func_8006B084_6BC84((Transform3D *)&player->tiltTransform, &headingOrientationMatrix, &combinedMatrix);

    attackState = player->flyingAttackState;

    switch (attackState) {
        case 1:
            samplePattern = terrainSamplePattern_State1;
            break;
        case 2:
            samplePattern = terrainSamplePattern_State2;
            break;
        case 3:
            samplePattern = terrainSamplePattern_State3;
            break;
    }

    for (i = 0; i < 3; i++) {
        transformVector(samplePattern[i], combinedMatrix.m[0], &samplePoints[i]);
    }

    for (i = 0; i < 3; i++) {
        u16 sectorIndex =
            getOrUpdatePlayerSectorIndex(player, &gameState->gameData, player->sectorIndex, &samplePoints[i]);
        sampledHeight = getTrackHeightInSector(&gameState->gameData, sectorIndex, &samplePoints[i], 0x100000);

        if (!(samplePoints[i].y >= sampledHeight)) {
            samplePoints[i].y = sampledHeight;
        }
    }

    samplePoints[1].x -= samplePoints[0].x;
    samplePoints[1].y -= samplePoints[0].y;
    samplePoints[1].z -= samplePoints[0].z;

    sampleDistSq = (s64)samplePoints[1].x * samplePoints[1].x + (s64)samplePoints[1].y * samplePoints[1].y +
                   (s64)samplePoints[1].z * samplePoints[1].z;
    length = isqrt64(sampleDistSq);

    samplePoints[1].x = (s32)(((s64)samplePoints[1].x * 0x2000) / length);
    samplePoints[1].y = (s32)(((s64)samplePoints[1].y * 0x2000) / length);
    samplePoints[1].z = (s32)(((s64)samplePoints[1].z * 0x2000) / length);

    samplePoints[2].x -= samplePoints[0].x;
    samplePoints[2].y -= samplePoints[0].y;
    samplePoints[2].z -= samplePoints[0].z;

    length = isqrt64(
        (s64)samplePoints[2].x * samplePoints[2].x + (s64)samplePoints[2].y * samplePoints[2].y +
        (s64)samplePoints[2].z * samplePoints[2].z
    );

    samplePoints[2].x = (s32)(((s64)samplePoints[2].x * 0x2000) / length);
    samplePoints[2].y = (s32)(((s64)samplePoints[2].y * 0x2000) / length);
    samplePoints[2].z = (s32)(((s64)samplePoints[2].z * 0x2000) / length);

    normal.x = samplePoints[2].y * samplePoints[1].z - samplePoints[2].z * samplePoints[1].y;
    normal.y = samplePoints[2].z * samplePoints[1].x - samplePoints[2].x * samplePoints[1].z;
    normal.z = samplePoints[2].x * samplePoints[1].y - samplePoints[2].y * samplePoints[1].x;

    length = isqrt64((s64)normal.x * normal.x + (s64)normal.y * normal.y + (s64)normal.z * normal.z);

    normal.x = (s32)(((s64)normal.x * 0x2000) / length);
    normal.y = (s32)(((s64)normal.y * 0x2000) / length);
    normal.z = (s32)(((s64)normal.z * 0x2000) / length);

    transformVector3(&normal, &player->headingTransform, &rotatedNormal);

    pitchAngle = atan2Fixed(rotatedNormal.z, -rotatedNormal.y);
    player->pitchAngle = (-pitchAngle) & 0x1FFF;

    length = isqrt64((s64)rotatedNormal.y * rotatedNormal.y + (s64)rotatedNormal.z * rotatedNormal.z);

    rollAngle = atan2Fixed(-rotatedNormal.x, -length);
    player->rollAngle = (-rollAngle) & 0x1FFF;

    createXRotationMatrix(player->orientationTransform.m, player->pitchAngle);
}

s16 getPlayerTargetTrackAngle(Player *player) {
    GameState *gameState;
    Section3Element *sectorEntry;
    s32 targetAngle;

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
            if (player->animFlags & 2) {
                targetAngle += 0x1000;
            }
        }
    } else {
        Section1Element *waypointData = (Section1Element *)gameState->gameData.section1Data;
        Section1Element *waypoint1 = (Section1Element *)(sectorEntry->unk16 * 6 + (u32)waypointData);
        Section1Element *waypoint2 = (Section1Element *)(sectorEntry->unk1C * 6 + (u32)waypointData);
        targetAngle = computeAngleToPosition(waypoint1->unk0, waypoint1->unk4, waypoint2->unk0, waypoint2->unk4);
        if (player->animFlags & 2) {
            targetAngle += 0x1000;
        }
    }
    return targetAngle;
}
