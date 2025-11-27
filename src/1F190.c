#include "6E840.h"
#include "common.h"
#include "task_scheduler.h"

extern void func_8001E5EC_1F1EC(void);
extern void func_8001EB4C_1F74C(void);
extern void func_8001F33C_1FF3C(void);
extern void func_8001F358_1FF58(void);

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
    u8 padB24[0x9];    // 0xB24
    u8 unkB2D;         // 0xB2D
    u8 padB2E[0x17];   // 0xB2E
    u8 unkB45;         // 0xB45
} Allocation_1F190;

INCLUDE_ASM("asm/nonmatchings/1F190", func_8001E590_1F190);

void func_8001E5C0_1F1C0(void) {
    Allocation_1F190 *allocation = allocateTaskMemory(0xB48);
    allocation->unkB45 = 0;
    setGameStateHandler(func_8001E5EC_1F1EC);
}

INCLUDE_ASM("asm/nonmatchings/1F190", func_8001E5EC_1F1EC);

INCLUDE_ASM("asm/nonmatchings/1F190", func_8001EAB0_1F6B0);

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

INCLUDE_ASM("asm/nonmatchings/1F190", func_8001F33C_1FF3C);

INCLUDE_ASM("asm/nonmatchings/1F190", func_8001F358_1FF58);

INCLUDE_ASM("asm/nonmatchings/1F190", func_8001F374_1FF74);

INCLUDE_ASM("asm/nonmatchings/1F190", func_8001F42C_2002C);

INCLUDE_ASM("asm/nonmatchings/1F190", func_8001F5EC_201EC);
