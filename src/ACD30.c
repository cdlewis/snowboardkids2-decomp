#include "common.h"

extern void *getCurrentAllocation(void);
extern void *freeNodeMemory(void *);

typedef struct {
    void *unk0;
    u8 _pad4[0x8];
    void *unkC;
    void *unk10;
} ACD30Struct;

typedef struct {
    u8 _pad[0x24];
    s32 unk24;
} ACD30AllocationStruct;

INCLUDE_ASM("asm/nonmatchings/ACD30", func_800BB2B0_ACD30);

INCLUDE_ASM("asm/nonmatchings/ACD30", func_800BB310_ACD90);

INCLUDE_ASM("asm/nonmatchings/ACD30", func_800BB428_ACEA8);

void func_800BB4B8_ACF38(ACD30Struct *arg0) {
    ACD30AllocationStruct *allocation = (ACD30AllocationStruct *)getCurrentAllocation();
    allocation->unk24 = 0;
    arg0->unk0 = freeNodeMemory(arg0->unk0);
    arg0->unkC = freeNodeMemory(arg0->unkC);
    arg0->unk10 = freeNodeMemory(arg0->unk10);
}

INCLUDE_ASM("asm/nonmatchings/ACD30", func_800BB504);

INCLUDE_ASM("asm/nonmatchings/ACD30", func_800BB690_AD110);

INCLUDE_ASM("asm/nonmatchings/ACD30", func_800BBA50);

INCLUDE_ASM("asm/nonmatchings/ACD30", func_800BBA90_AD510);

INCLUDE_ASM("asm/nonmatchings/ACD30", func_800BBB14_AD594);

INCLUDE_ASM("asm/nonmatchings/ACD30", func_800BBB70_AD5F0);
