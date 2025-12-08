#include "5AA90.h"
#include "56910.h"
#include "common.h"
#include "displaylist.h"
#include "gamestate.h"
#include "task_scheduler.h"
extern s32 func_8005D020_5DC20(void *arg0, u16 arg1, void *arg2, s32 arg3);
extern s16 func_80062254_62E54(void *arg0, u16 arg1);
extern s32 isqrt64(s64 val);

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

u16 func_80059E90_5AA90(Player *arg0, void *arg1, u16 arg2, void *arg3) {
    if (!(arg0->unkB84 & 0x100)) {
        return func_80060A3C_6163C(arg1, arg2, arg3);
    }
    return arg0->unkB94;
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

INCLUDE_ASM("asm/nonmatchings/5AA90", func_8005B548_5C148);

INCLUDE_ASM("asm/nonmatchings/5AA90", func_8005B730_5C330);

INCLUDE_ASM("asm/nonmatchings/5AA90", func_8005B9E4_5C5E4);

INCLUDE_ASM("asm/nonmatchings/5AA90", func_8005BCB8_5C8B8);

INCLUDE_ASM("asm/nonmatchings/5AA90", func_8005BF50_5CB50);

INCLUDE_ASM("asm/nonmatchings/5AA90", func_8005C250_5CE50);

INCLUDE_ASM("asm/nonmatchings/5AA90", func_8005C454_5D054);

INCLUDE_ASM("asm/nonmatchings/5AA90", isPlayerInRangeAndPull);

void func_8005C838_5D438(ListNode_5AA90 *arg0) {
    Allocation5AA90 *alloc = (Allocation5AA90 *)getCurrentAllocation();
    arg0->next = alloc->list;
    alloc->list = arg0;
}

INCLUDE_ASM("asm/nonmatchings/5AA90", func_8005C868_5D468);

INCLUDE_ASM("asm/nonmatchings/5AA90", func_8005CE98_5DA98);
