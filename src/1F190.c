#include "202A0.h"
#include "3A1F0.h"
#include "3E160.h"
#include "56910.h"
#include "6E840.h"
#include "D_800AFE8C_A71FC_type.h"
#include "common.h"
#include "task_scheduler.h"

extern void func_8001E5EC_1F1EC(void);
extern void func_8001EB4C_1F74C(void);
void func_8001EB1C_1F71C(void);
void func_8001F33C_1FF3C(void);
void func_8001F358_1FF58(void);

typedef struct {
    Node_70B00 unk0;   // 0x000
    Node_70B00 unk1D8; // 0x1D8
    Node_70B00 unk3B0; // 0x3B0
    Node_70B00 unk588; // 0x588
    Node_70B00 unk760; // 0x760
    Node_70B00 unk938; // 0x938
    void *unkB10;      // 0xB10
    void *unkB14;      // 0xB14
    void *unkB18;      // 0xB18
    void *unkB1C;      // 0xB1C
    void *unkB20;      // 0xB20
    u8 padB24[0x4];    // 0xB24
    u16 unkB28;        // 0xB28
    u8 padB2A[0x2];    // 0xB2A
    s8 unkB2C;         // 0xB2C
    u8 unkB2D;         // 0xB2D
    u8 unkB2E;         // 0xB2E
    u8 padB2F[0x4];    // 0xB2F
    u8 unkB33[12];     // 0xB33
    u8 padB3F[0x5];    // 0xB3F
    u8 unkB44;         // 0xB44
    u8 unkB45;         // 0xB45
    u8 unkB46;         // 0xB46
} Allocation_1F190;

void func_8001E590_1F190(void) {
    Allocation_1F190 *allocation = allocateTaskMemory(0xB48);
    allocation->unkB45 = 1;
    setGameStateHandler(func_8001E5EC_1F1EC);
}

void func_8001E5C0_1F1C0(void) {
    Allocation_1F190 *allocation = allocateTaskMemory(0xB48);
    allocation->unkB45 = 0;
    setGameStateHandler(func_8001E5EC_1F1EC);
}

INCLUDE_ASM("asm/nonmatchings/1F190", func_8001E5EC_1F1EC);

void func_8001EAB0_1F6B0(void) {
    Allocation_1F190 *allocation = (Allocation_1F190 *)getCurrentAllocation();

    allocation->unkB28++;
    if (allocation->unkB28 < 3) {
        return;
    }

    allocation->unkB28 = 2;
    if (func_8003BB5C_3C75C() != 0) {
        return;
    }

    allocation->unkB28 = 0;
    func_8006FDA0_709A0(0, 0, 0xE);
    setGameStateHandler(func_8001EB1C_1F71C);
}

void func_8001EB1C_1F71C(void) {
    if (func_8006FE10_70A10(0) == 0) {
        setGameStateHandler(func_8001EB4C_1F74C);
    }
}

INCLUDE_ASM("asm/nonmatchings/1F190", func_8001EB4C_1F74C);

void func_8001F25C_1FE5C(void) {
    Allocation_1F190 *allocation = (Allocation_1F190 *)getCurrentAllocation();

    if (func_8006FE10_70A10(NULL) != 0) {
        return;
    }

    unlinkNode(&allocation->unk0);
    unlinkNode(&allocation->unk3B0);
    unlinkNode(&allocation->unk588);
    unlinkNode(&allocation->unk1D8);
    unlinkNode(&allocation->unk938);

    allocation->unkB10 = freeNodeMemory(allocation->unkB10);
    allocation->unkB14 = freeNodeMemory(allocation->unkB14);
    allocation->unkB18 = freeNodeMemory(allocation->unkB18);
    allocation->unkB1C = freeNodeMemory(allocation->unkB1C);
    allocation->unkB20 = freeNodeMemory(allocation->unkB20);

    if (allocation->unkB2D == 1) {
        terminateSchedulerWithCallback(func_8001F33C_1FF3C);
        if (allocation->unkB45 != 0) {
            unlinkNode(&allocation->unk760);
        }
    } else {
        terminateSchedulerWithCallback(func_8001F358_1FF58);
    }
}

void func_8001F33C_1FF3C(void) {
    func_800697F4_6A3F4(1);
}

void func_8001F358_1FF58(void) {
    func_800697F4_6A3F4(0xFF);
}

void func_8001F374_1FF74(void) {
    Allocation_1F190 *allocation;
    D_800AFE8C_A71FC_type *ptr;
    u8 unk4;
    u8 saveSlotIndex;

    allocation = getCurrentAllocation();
    ptr = D_800AFE8C_A71FC;
    unk4 = ptr->unk4;
    saveSlotIndex = ptr->saveSlotIndex;

    if (unk4 == 0) {
        if (saveSlotIndex == 3 || saveSlotIndex == 7 || saveSlotIndex == 11 || saveSlotIndex >= 12) {
            ptr->unk9[0x10] = 1;
            D_800AFE8C_A71FC->unk8 = 1;
        } else {
            ptr->unk9[0x10] = 3;
            D_800AFE8C_A71FC->unk8 = 1;
        }
    } else {
        ptr->unk9[0x10] = allocation->unkB46;
        D_800AFE8C_A71FC->unk22 = allocation->unkB46;
    }
}

#define ASPECT_RATIO (4.0f / 3.0f)

void func_8001F42C_2002C(void) {
    Allocation_1F190 *allocation;
    D_80090F90_91B90_item *item;
    u8 counter;
    u8 poolIdRaw;
    u8 poolId;
    Node_70B00 *node;

    allocation = (Allocation_1F190 *)getCurrentAllocation();

    if (allocation->unkB44 != 0) {
        return;
    }

    counter = allocation->unkB2E;
    if (counter < 3) {
        allocation->unkB2E = counter + 1;
        if ((u8)(counter + 1) != 3) {
            return;
        }

        poolIdRaw = allocation->unkB33[allocation->unkB2C];
        poolId = poolIdRaw;
        item = func_80055D10_56910(poolId);

        func_8006FC70_70870(100, 1, &item->unk18, &item->unk20);

        if (poolId == 11) {
            node = &allocation->unk3B0;
            func_8006FA0C_7060C(node, 50.0f, ASPECT_RATIO, 10.0f, 1000.0f);
            func_8006FE48_70A48(100, 900, 999, item->unk20.r2, item->unk20.g2, item->unk20.b2);
            func_80041518_42118(node);
        } else {
            func_8006FA0C_7060C(&allocation->unk3B0, 50.0f, ASPECT_RATIO, 10.0f, 4000.0f);
            func_8006FE48_70A48(100, 995, 999, item->unk20.r2, item->unk20.g2, item->unk20.b2);
        }

        scheduleTask(&func_8001F6A0_202A0, 0, 0, 0x5B);
        func_800497FC_4A3FC(poolIdRaw);
        allocation->unkB2E = allocation->unkB2E + 1;
    } else if (counter < 6) {
        allocation->unkB2E = counter + 1;
    } else {
        if (func_8003BB5C_3C75C() == 0) {
            func_8006FDA0_709A0(&allocation->unk588, 0, 0);
            func_8006FDA0_709A0(&allocation->unk3B0, 0, 0x14);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/1F190", func_8001F5EC_201EC);
