#include "56910.h"
#include "common.h"
#include "displaylist.h"
#include "task_scheduler.h"

extern s32 gFrameCounter;

typedef struct {
    u8 _pad[0x80];
    u8 unk80;
    u8 unk81;
} Allocation_AD728;

typedef struct {
    DisplayListObject node1;
    DisplayListObject node2;
} func_800BB368_AD728_arg;

INCLUDE_ASM("asm/nonmatchings/AD670", func_800BB2B0_AD670);

void func_800BB368_AD728(func_800BB368_AD728_arg *arg0) {
    Allocation_AD728 *allocation;
    s32 i;
    func_80055E68_56A68_result *result;

    allocation = (Allocation_AD728 *)getCurrentAllocation();

    if (allocation->unk80 != 0) {
        if (gFrameCounter & 1) {
            result = func_80055E68_56A68(8);
            arg0->node1.unk20 = (DisplayLists *)((u32)result + 0x10);
        } else {
            result = func_80055E68_56A68(8);
            arg0->node1.unk20 = (DisplayLists *)((u32)result + 0x20);
        }
    } else {
        arg0->node1.unk20 = (DisplayLists *)func_80055E68_56A68(8);
    }

    if (allocation->unk81 != 0) {
        if (gFrameCounter & 1) {
            result = func_80055E68_56A68(8);
            arg0->node2.unk20 = (DisplayLists *)((u32)result + 0x10);
        } else {
            result = func_80055E68_56A68(8);
            arg0->node2.unk20 = (DisplayLists *)((u32)result + 0x20);
        }
    } else {
        arg0->node2.unk20 = (DisplayLists *)func_80055E68_56A68(8);
    }

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, &arg0->node1);
        enqueueDisplayListWithFrustumCull(i, &arg0->node2);
    }
}

INCLUDE_ASM("asm/nonmatchings/AD670", func_800BB45C_AD81C);

INCLUDE_ASM("asm/nonmatchings/AD670", func_800BB494_AD854);

INCLUDE_ASM("asm/nonmatchings/AD670", func_800BB51C_AD8DC);

INCLUDE_ASM("asm/nonmatchings/AD670", func_800BB5FC_AD9BC);

INCLUDE_ASM("asm/nonmatchings/AD670", func_800BB664_ADA24);

INCLUDE_ASM("asm/nonmatchings/AD670", func_800BB6C0_ADA80);

INCLUDE_ASM("asm/nonmatchings/AD670", func_800BB718_ADAD8);

INCLUDE_ASM("asm/nonmatchings/AD670", func_800BB75C_ADB1C);

INCLUDE_ASM("asm/nonmatchings/AD670", func_800BBB0C_ADECC);

INCLUDE_ASM("asm/nonmatchings/AD670", func_800BBB38_ADEF8);

INCLUDE_ASM("asm/nonmatchings/AD670", func_800BBB64_ADF24);

INCLUDE_ASM("asm/nonmatchings/AD670", func_800BBB90);

INCLUDE_ASM("asm/nonmatchings/AD670", func_800BBC28_ADFE8);

INCLUDE_ASM("asm/nonmatchings/AD670", func_800BBCFC_AE0BC);

INCLUDE_ASM("asm/nonmatchings/AD670", func_800BBEA0_AE260);

INCLUDE_ASM("asm/nonmatchings/AD670", func_800BBED8);

INCLUDE_ASM("asm/nonmatchings/AD670", func_800BBF28_AE2E8);

INCLUDE_ASM("asm/nonmatchings/AD670", func_800BC084_AE444);

INCLUDE_ASM("asm/nonmatchings/AD670", func_800BC1AC_AE56C);

INCLUDE_ASM("asm/nonmatchings/AD670", func_800BC3D0_AE790);

INCLUDE_ASM("asm/nonmatchings/AD670", func_800BC4F0_AE8B0);

INCLUDE_ASM("asm/nonmatchings/AD670", func_800BC528_AE8E8);

INCLUDE_ASM("asm/nonmatchings/AD670", func_800BC550_AE910);

INCLUDE_ASM("asm/nonmatchings/AD670", func_800BC6C4_AEA84);

INCLUDE_ASM("asm/nonmatchings/AD670", func_800BC768_AEB28);

INCLUDE_ASM("asm/nonmatchings/AD670", func_800BC984_AED44);

INCLUDE_ASM("asm/nonmatchings/AD670", func_800BC9BC);
