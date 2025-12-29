#include "5AA90.h"
#include "3E160.h"
#include "56910.h"
#include "594E0.h"
#include "5DBC0.h"
#include "common.h"
#include "displaylist.h"
#include "gamestate.h"
#include "geometry.h"
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

s32 func_80059E90_5AA90(void *arg0, void *arg1, u16 arg2, void *arg3) {
    Player *player = (Player *)arg0;
    if (!(player->unkB84 & 0x100)) {
        return func_80060A3C_6163C(arg1, arg2, arg3);
    }
    return player->unkB94;
}

INCLUDE_ASM("asm/nonmatchings/5AA90", func_80059ED0_5AAD0);

INCLUDE_ASM("asm/nonmatchings/5AA90", func_8005A26C_5AE6C);

void func_8005A930_5B530(Player *arg0) {
    Allocation5AA90 *allocation;
    void *sp0;
    void *sp1;
    u16 temp;
    s32 result;

    allocation = getCurrentAllocation();
    sp0 = &allocation->unk30;
    sp1 = (void *)&arg0->worldPos.x;
    temp = func_80059E90_5AA90(arg0, sp0, arg0->unkB94, sp1);
    arg0->unkB94 = temp;
    result = func_8005D020_5DC20(sp0, temp, sp1, 0x200000);
    if (result < arg0->worldPos.y) {
        arg0->worldPos.y = result;
    }
}

s32 func_8005A9A8_5B5A8(Player *arg0) {
    GameState *allocation;
    D_80090F90_91B90_item *item;
    s32 dx, dz;
    s64 distSq;

    allocation = (GameState *)getCurrentAllocation();

    if (allocation->unk63 != 0) {
        return 0;
    }

    if (arg0->unkBC5 == allocation->unk74) {
        return 0;
    }

    if (func_80062254_62E54(&allocation->gameData, arg0->unkB94) != 0) {
        return 0;
    }

    item = func_80055D10_56910(allocation->memoryPoolId);

    dx = arg0->worldPos.x - item->unk0;
    dz = arg0->worldPos.z - item->unk4;

    distSq = (s64)dx * dx + (s64)dz * dz;

    if (0x2FFFFF < isqrt64(distSq)) {
        return 0;
    }

    allocation->unk63 = 3;
    arg0->unkB9C = allocation->unk58;
    allocation->unk58--;

    return 1;
}

s32 func_8005AA9C_5B69C(Player *arg0) {
    GameState *allocation;
    D_80090F90_91B90_item *item;
    s32 dx, dz;
    s64 distSq;

    allocation = (GameState *)getCurrentAllocation();

    if (func_80062254_62E54(&allocation->gameData, arg0->unkB94) == 0) {
        item = func_80055D10_56910(allocation->memoryPoolId);

        dx = arg0->worldPos.x - item->unk0;
        dz = arg0->worldPos.z - item->unk4;

        distSq = (s64)dx * dx + (s64)dz * dz;

        if (!(0x1FFFFF < isqrt64(distSq))) {
            return 1;
        }
    }
    return 0;
}

void func_8005AB58_5B758(Player *player) {
    Vec3i deltaPos;
    GameState *allocation;
    s32 unused1;
    s32 combinedRadius;
    s32 playerIndex;
    Player *targetPlayer;
    s32 numPlayers;

    allocation = (GameState *)getCurrentAllocation();
    numPlayers = allocation->numPlayers;

    for (playerIndex = 0; playerIndex < allocation->numPlayers; playerIndex++) {
        targetPlayer = &allocation->players[playerIndex];

        /* Skip collision if players are on the same team */
        if (player->unkBB8 == targetPlayer->unkBB8) {
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
        memcpy(&deltaPos, &targetPlayer->unkAD4, 0xC);

        /* Convert to world space */
        deltaPos.x += targetPlayer->worldPos.x;
        deltaPos.y += targetPlayer->worldPos.y;
        deltaPos.z += targetPlayer->worldPos.z;

        /* Calculate relative position to player's collision box */
        deltaPos.x -= player->worldPos.x + player->unkAD4[0];
        deltaPos.y -= player->worldPos.y + player->unkAD4[1];
        deltaPos.z -= player->worldPos.z + player->unkAD4[2];

        /* Sum of both collision box radii */
        combinedRadius = targetPlayer->unkAE0 + player->unkAE0;

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
 */
void func_8005AE8C_5BA8C(Player *player) {
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
    if (player->unkBB8 == targetPlayer->unkBB8) {
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
            deltaPos.x -= player->worldPos.x + player->unkAD4[0];
            deltaPos.y -= player->worldPos.y + player->unkAD4[1];
            deltaPos.z -= player->worldPos.z + player->unkAD4[2];

            /* Sum of both collision box radii */
            combinedRadius = (&targetPlayer->unkB2C)[boxIndex] + player->unkAE0;
            negRadius = -combinedRadius;

            /* Quick AABB check before expensive distance calculation */
            if (negRadius < deltaPos.x && deltaPos.x < combinedRadius && negRadius < deltaPos.y &&
                deltaPos.y < combinedRadius && negRadius < deltaPos.z && deltaPos.z < combinedRadius) {

                dist =
                    isqrt64((s64)deltaPos.x * deltaPos.x + (s64)deltaPos.y * deltaPos.y + (s64)deltaPos.z * deltaPos.z);

                if (dist < combinedRadius) {
                    /* Check for special knockback on collision boxes 4-5 when target is in state 1 */
                    if (targetPlayer->unkBD9 == 1) {
                        if ((targetPlayer->unkB84 & 0x40000) && (u32)(boxIndex - 4) < 2U) {
                            func_80058B30_59730(player);
                            goto next;
                        }
                    }
                    /* Check for special knockback on collision boxes 1-2 when target is in state 3 */
                    if (targetPlayer->unkBD9 == 3 && (targetPlayer->unkB84 & 0x40000) && (u32)(boxIndex - 1) < 2U) {
                        func_80058B30_59730(player);
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
                            func_80058950_59550(
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

void *func_8005B24C_5BE4C(Vec3i *arg0, s32 arg1, s32 arg2) {
    Vec3i pos;
    s32 combinedRadius;
    Allocation5AA90 *allocation;
    ListNode_5AA90 *node;

    allocation = getCurrentAllocation();
    node = allocation->list;

    for (; node != NULL; node = node->next) {
        if (arg1 == node->id) {
            continue;
        }

        memcpy(&pos, &node->localPos, 0xC);

        pos.x += node->posPtr->x;
        pos.y += node->posPtr->y;
        pos.z += node->posPtr->z;

        pos.x -= arg0->x;
        pos.y -= arg0->y;
        pos.z -= arg0->z;

        combinedRadius = node->radius + arg2;

        if (-combinedRadius < pos.x && pos.x < combinedRadius && -combinedRadius < pos.y && pos.y < combinedRadius &&
            -combinedRadius < pos.z && pos.z < combinedRadius) {
            s32 dist;

            dist = isqrt64((s64)pos.x * pos.x + (s64)pos.y * pos.y + (s64)pos.z * pos.z);

            if (dist < combinedRadius) {
                u8 index = node->id;
                return (u8 *)allocation->dataArray + ((index * 3 * 128) - (index * 3)) * 8;
            }
        }
    }

    return NULL;
}

s32 func_8005B400_5C000(Player *arg0, Vec3i *arg1, s32 arg2) {
    Vec3i localVec;
    s32 radius;
    s32 negRadius;
    s64 distSq;

    memcpy(&localVec, &arg0->unkAD4, 0xC);

    localVec.x = localVec.x + arg0->worldPos.x;
    localVec.y = localVec.y + arg0->worldPos.y;
    localVec.z = localVec.z + arg0->worldPos.z;

    localVec.x = localVec.x - arg1->x;
    localVec.y = localVec.y - arg1->y;
    localVec.z = localVec.z - arg1->z;

    radius = arg0->unkAE0 + arg2;
    negRadius = -radius;

    if (negRadius < localVec.x && localVec.x < radius && negRadius < localVec.y && localVec.y < radius &&
        negRadius < localVec.z && localVec.z < radius) {
        distSq = (s64)localVec.x * localVec.x + (s64)localVec.y * localVec.y + (s64)localVec.z * localVec.z;
        if (isqrt64(distSq) < radius) {
            return 1;
        }
    }

    return 0;
}

Player *func_8005B548_5C148(Vec3i *arg0, s32 arg1, s32 arg2) {
    Vec3i pos;
    s32 combinedRadius;
    Allocation5AA90 *allocation;
    ListNode_5AA90 *node;
    Player *playerData;
    void *dataArray;
    u8 id;

    allocation = getCurrentAllocation();
    node = allocation->list;

    for (; node != NULL; node = node->next) {
        id = node->id;
        if (arg1 == id) {
            continue;
        }

        dataArray = allocation->dataArray;
        playerData = (Player *)(id * 0xBE8 + (s32)dataArray);

        if (playerData->unkB88 & 0x10) {
            continue;
        }

        if (playerData->unkBA4 != 0) {
            continue;
        }

        memcpy(&pos, &node->localPos, 0xC);

        pos.x += node->posPtr->x;
        pos.y += node->posPtr->y;
        pos.z += node->posPtr->z;

        pos.x -= arg0->x;
        pos.y -= arg0->y;
        pos.z -= arg0->z;

        combinedRadius = node->radius + arg2;

        if (-combinedRadius < pos.x && pos.x < combinedRadius && -combinedRadius < pos.y && pos.y < combinedRadius &&
            -combinedRadius < pos.z && pos.z < combinedRadius) {
            s32 dist;

            dist = isqrt64((s64)pos.x * pos.x + (s64)pos.y * pos.y + (s64)pos.z * pos.z);

            if (dist < combinedRadius) {
                u8 index = node->id;
                return (Player *)((u8 *)allocation->dataArray + ((index * 3 * 128) - (index * 3)) * 8);
            }
        }
    }

    return NULL;
}

INCLUDE_ASM("asm/nonmatchings/5AA90", func_8005B730_5C330);

INCLUDE_ASM("asm/nonmatchings/5AA90", func_8005B9E4_5C5E4);

INCLUDE_ASM("asm/nonmatchings/5AA90", func_8005BCB8_5C8B8);

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
                } else if (playerData->unkBA4 != 0) {
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
                            func_80058AEC_596EC(
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
    u16 unkBA4;
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
        if (((PlayerData *)(((u8 *)allocation->dataArray) + (index * 0xBE8)))->unkBA4 != 0) {
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

    localVec.x = localVec.x - (arg2->worldPos.x + arg2->unkAD4[0]);
    localVec.y = localVec.y - (arg2->worldPos.y + arg2->unkAD4[1]);
    localVec.z = localVec.z - (arg2->worldPos.z + arg2->unkAD4[2]);

    combinedRadius = arg1 + arg2->unkAE0;
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
    elem = (Section3Element *)(arg0->unkB94 * 0x24 + (u32)allocation->gameData.section3Data);

    if (elem->unk0 < 0) {
        if (allocation->unk74 == arg0->unkBC5) {
            Section1Element *section1Data = (Section1Element *)allocation->gameData.section1Data;
            Section1Element *v1 = (Section1Element *)(elem->unk1C * 6 + (u32)section1Data);
            Section1Element *v2 = (Section1Element *)(elem->unk16 * 6 + (u32)section1Data);
            result = func_8006D21C_6DE1C(v1->unk0, v1->unk4, v2->unk0, v2->unk4) + 0x800;
        } else {
            D_80090F90_91B90_item *item = func_80055D10_56910(allocation->memoryPoolId);
            result = func_8006D21C_6DE1C(item->unk0, item->unk4, arg0->worldPos.x, arg0->worldPos.z);
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
