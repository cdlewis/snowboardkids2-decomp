#include "5AA90.h"
#include "3E160.h"
#include "56910.h"
#include "common.h"
#include "displaylist.h"
#include "gamestate.h"
#include "geometry.h"
#include "task_scheduler.h"
extern s32 func_8005D020_5DC20(void *arg0, u16 arg1, void *arg2, s32 arg3);

typedef struct ListNode_5AA90 {
    /* 0x00 */ struct ListNode_5AA90 *next;
    /* 0x04 */ Vec3s32 *posPtr;
    /* 0x08 */ Vec3s32 localPos;
    /* 0x14 */ s32 radius;
    /* 0x18 */ u8 id;
} ListNode_5AA90;

typedef struct {
    u8 _pad[0x10];
    /* 0x10 */ void *dataArray;
    u8 _pad2[0x18];
    /* 0x2C */ ListNode_5AA90 *list;
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
    void *allocation;
    void *sp0;
    void *sp1;
    u16 temp;
    s32 result;

    allocation = getCurrentAllocation();
    sp0 = (void *)((u8 *)allocation + 0x30);
    sp1 = (void *)&arg0->worldPosX;
    temp = func_80059E90_5AA90(arg0, sp0, arg0->unkB94, sp1);
    arg0->unkB94 = temp;
    result = func_8005D020_5DC20(sp0, temp, sp1, 0x200000);
    if (result < arg0->worldPosY) {
        arg0->worldPosY = result;
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

    dx = arg0->worldPosX - item->unk0;
    dz = arg0->worldPosZ - item->unk4;

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

        dx = arg0->worldPosX - item->unk0;
        dz = arg0->worldPosZ - item->unk4;

        distSq = (s64)dx * dx + (s64)dz * dz;

        if (!(0x1FFFFF < isqrt64(distSq))) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/5AA90", func_8005AB58_5B758);

INCLUDE_ASM("asm/nonmatchings/5AA90", func_8005AE8C_5BA8C);

void *func_8005B24C_5BE4C(Vec3s32 *arg0, s32 arg1, s32 arg2) {
    Vec3s32 pos;
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

        pos.unk0 += node->posPtr->unk0;
        pos.unk4 += node->posPtr->unk4;
        pos.unk8 += node->posPtr->unk8;

        pos.unk0 -= arg0->unk0;
        pos.unk4 -= arg0->unk4;
        pos.unk8 -= arg0->unk8;

        combinedRadius = node->radius + arg2;

        if (-combinedRadius < pos.unk0 && pos.unk0 < combinedRadius && -combinedRadius < pos.unk4 &&
            pos.unk4 < combinedRadius && -combinedRadius < pos.unk8 && pos.unk8 < combinedRadius) {
            s32 dist;

            dist = isqrt64((s64)pos.unk0 * pos.unk0 + (s64)pos.unk4 * pos.unk4 + (s64)pos.unk8 * pos.unk8);

            if (dist < combinedRadius) {
                u8 index = node->id;
                return (u8 *)allocation->dataArray + ((index * 3 * 128) - (index * 3)) * 8;
            }
        }
    }

    return NULL;
}

s32 func_8005B400_5C000(Player *arg0, Vec3s32 *arg1, s32 arg2) {
    Vec3s32 localVec;
    s32 radius;
    s32 negRadius;
    s64 distSq;

    memcpy(&localVec, &arg0->unkAD4, 0xC);

    localVec.unk0 = localVec.unk0 + arg0->worldPosX;
    localVec.unk4 = localVec.unk4 + arg0->worldPosY;
    localVec.unk8 = localVec.unk8 + arg0->worldPosZ;

    localVec.unk0 = localVec.unk0 - arg1->unk0;
    localVec.unk4 = localVec.unk4 - arg1->unk4;
    localVec.unk8 = localVec.unk8 - arg1->unk8;

    radius = arg0->unkAE0 + arg2;
    negRadius = -radius;

    if (negRadius < localVec.unk0 && localVec.unk0 < radius && negRadius < localVec.unk4 && localVec.unk4 < radius &&
        negRadius < localVec.unk8 && localVec.unk8 < radius) {
        distSq = (s64)localVec.unk0 * localVec.unk0 + (s64)localVec.unk4 * localVec.unk4 +
                 (s64)localVec.unk8 * localVec.unk8;
        if (isqrt64(distSq) < radius) {
            return 1;
        }
    }

    return 0;
}

Player *func_8005B548_5C148(Vec3s32 *arg0, s32 arg1, s32 arg2) {
    Vec3s32 pos;
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

        pos.unk0 += node->posPtr->unk0;
        pos.unk4 += node->posPtr->unk4;
        pos.unk8 += node->posPtr->unk8;

        pos.unk0 -= arg0->unk0;
        pos.unk4 -= arg0->unk4;
        pos.unk8 -= arg0->unk8;

        combinedRadius = node->radius + arg2;

        if (-combinedRadius < pos.unk0 && pos.unk0 < combinedRadius && -combinedRadius < pos.unk4 &&
            pos.unk4 < combinedRadius && -combinedRadius < pos.unk8 && pos.unk8 < combinedRadius) {
            s32 dist;

            dist = isqrt64((s64)pos.unk0 * pos.unk0 + (s64)pos.unk4 * pos.unk4 + (s64)pos.unk8 * pos.unk8);

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

s32 func_8005C250_5CE50(Vec3s32 *arg0, s32 arg1, s32 arg2) {
    Vec3s32 pos;
    s32 combinedRadius;
    Allocation5AA90 *allocation;
    ListNode_5AA90 *node;
    Player *playerData;
    void *dataArray;
    s32 result;
    Vec3s32 *posPtr;

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

                    pos.unk0 += node->posPtr->unk0;
                    pos.unk4 += node->posPtr->unk4;
                    pos.unk8 += node->posPtr->unk8;

                    pos.unk0 -= arg0->unk0;
                    pos.unk4 -= arg0->unk4;
                    pos.unk8 -= arg0->unk8;

                    combinedRadius = node->radius + arg2;

                    if (-combinedRadius < pos.unk0 && pos.unk0 < combinedRadius && -combinedRadius < pos.unk4 &&
                        pos.unk4 < combinedRadius && -combinedRadius < pos.unk8 && pos.unk8 < combinedRadius) {
                        s32 dist;

                        dist = isqrt64((s64)pos.unk0 * pos.unk0 + (s64)pos.unk4 * pos.unk4 + (s64)pos.unk8 * pos.unk8);

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

INCLUDE_ASM("asm/nonmatchings/5AA90", func_8005C454_5D054);

s32 isPlayerInRangeAndPull(Vec3s32 *arg0, s32 arg1, Player *arg2) {
    Vec3s32 localVec;
    s32 unused1;
    s32 unused2;
    s32 combinedRadius;
    s32 negRadius;
    s32 dist;

    memcpy(&localVec, arg0, 0xC);

    localVec.unk0 = localVec.unk0 - (arg2->worldPosX + arg2->unkAD4[0]);
    localVec.unk4 = localVec.unk4 - (arg2->worldPosY + arg2->unkAD4[1]);
    localVec.unk8 = localVec.unk8 - (arg2->worldPosZ + arg2->unkAD4[2]);

    combinedRadius = arg1 + arg2->unkAE0;
    negRadius = -combinedRadius;

    if (negRadius < localVec.unk0 && localVec.unk0 < combinedRadius && negRadius < localVec.unk4 &&
        localVec.unk4 < combinedRadius && negRadius < localVec.unk8 && localVec.unk8 < combinedRadius) {

        dist = isqrt64(
            (s64)localVec.unk0 * localVec.unk0 + (s64)localVec.unk4 * localVec.unk4 + (s64)localVec.unk8 * localVec.unk8
        );

        if (dist < combinedRadius) {
            if (dist == 0) {
                dist = 1;
            }

            localVec.unk0 = ((s64)localVec.unk0 * combinedRadius / dist) - localVec.unk0;
            localVec.unk4 = ((s64)localVec.unk4 * combinedRadius / dist) - localVec.unk4;
            localVec.unk8 = ((s64)localVec.unk8 * combinedRadius / dist) - localVec.unk8;

            arg2->worldPosX -= localVec.unk0;
            arg2->worldPosY -= localVec.unk4;
            arg2->worldPosZ -= localVec.unk8;

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
            result = func_8006D21C_6DE1C(item->unk0, item->unk4, arg0->worldPosX, arg0->worldPosZ);
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
