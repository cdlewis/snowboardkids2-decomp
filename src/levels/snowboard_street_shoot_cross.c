#include "56910.h"
#include "5AA90.h"
#include "6E840.h"
#include "common.h"
#include "displaylist.h"
#include "gamestate.h"
#include "geometry.h"
#include "overlay.h"
#include "task_scheduler.h"

extern void func_80059BD4_5A7D4(Player *);

USE_ASSET(_4060A0);

typedef struct {
    s8 unk0;
    u8 pad1[3];
    s32 unk4;
    s32 unk8;
    s32 unkC;
} BB504Entry;

typedef struct {
    void *unk0;
    void *unk4;
    BB504Entry *unk8;
    void *unkC;
    void *unk10;
    u8 _pad14[0x2];
    s16 unk16;
} ACD30Struct;

void func_800BB4B8_ACF38(ACD30Struct *arg0);
void func_800BB310_ACD90(void);

typedef struct {
    u8 _pad[0x24];
    ACD30Struct *unk24;
} ACD30AllocationStruct;

typedef struct {
    void *unk0;
    void *unk4;
    void *unk8;
    void *unkC;
    Player *unk10;
    void *unk14;
    u8 pad18[0xC];
    ACD30Struct *unk24;
    u8 pad28[0x32];
    u8 unk5A;
} BB504Allocation;

typedef struct {
    u8 _pad[0x5C];
    u8 unk5C;
} AD510Allocation;

typedef struct {
    u8 _pad[0x14];
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    void *unk20;
    void *unk24;
    void *unk28;
    s32 unk2C;
} AD510Arg;

typedef struct {
    u8 _pad[0x14];
    s16 unk14;
} ACD30Task;

void func_800BB2B0_ACD30(ACD30Struct *arg0) {
    arg0->unkC = loadAsset_34F9A0();
    arg0->unk10 = dmaRequestAndUpdateStateWithSize(&_4060A0_ROM_START, &_4060A0_ROM_END, 0x160);
    arg0->unk0 = NULL;
    setCleanupCallback(func_800BB4B8_ACF38);
    setCallback(func_800BB310_ACD90);
}

INCLUDE_ASM("asm/nonmatchings/levels/snowboard_street_shoot_cross", func_800BB310_ACD90);

void func_800BB690_AD110(void);

void func_800BB428_ACEA8(ACD30Struct *arg0) {
    s32 i;

    for (i = 0; i < arg0->unk16; i++) {
        func_8005BCB8_5C8B8(&arg0->unk8[i].unk4, 0x180000, 0x300000);
    }

    for (i = 0; i < 4; i++) {
        debugEnqueueCallback((u16)i, 4, func_800BB690_AD110, arg0);
    }
}

void func_800BB4B8_ACF38(ACD30Struct *arg0) {
    ACD30AllocationStruct *allocation = (ACD30AllocationStruct *)getCurrentAllocation();
    allocation->unk24 = NULL;
    arg0->unk0 = freeNodeMemory(arg0->unk0);
    arg0->unkC = freeNodeMemory(arg0->unkC);
    arg0->unk10 = freeNodeMemory(arg0->unk10);
}

#define Y_OFFSET 0xFFE80000

s32 func_800BB504(s32 *arg0, s32 arg1) {
    s32 pos[3];
    s32 unused[2];
    BB504Allocation *allocation;
    ACD30Struct *s1;
    s32 range;
    s32 negRange;
    s32 i = 0;
    s32 x, y, z;
    s8 *entry;
    s32 yOffset;
    s32 idx;

    allocation = (BB504Allocation *)getCurrentAllocation();
    s1 = allocation->unk24;

    if (s1) {
        goto check_count;
    }

    return 0;

found:
    *entry = 1;
    allocation->unk5A = allocation->unk5A + 1;
    func_80059BD4_5A7D4(allocation->unk10);
    return 1;

check_count:
    range = arg1 + 0x140000;

    if (s1->unk16 <= 0) {
        return 0;
    }

    yOffset = Y_OFFSET;
    negRange = -range;

    do {
        memcpy(pos, arg0, 12);
        idx = i << 4;
        x = pos[0] - s1->unk8[i].unk4;
        pos[0] = x;
        y = (pos[1] + yOffset) - s1->unk8[i].unk8;
        pos[1] = y;
        z = pos[2] - s1->unk8[i].unkC;
        pos[2] = z;

        if (negRange < x) {
            if (x < range) {
                if (negRange < y) {
                    if (y < range) {
                        if (negRange < z) {
                            if (z < range) {
                                if (distance_3d(x, y, z) < range) {
                                    entry = (s8 *)(idx + (s32)s1->unk8);
                                    if (*entry == 0) {
                                        goto found;
                                    }
                                    return 2;
                                }
                            }
                        }
                    }
                }
            }
        }

        i++;
    } while (i < s1->unk16);

    return 0;
}

INCLUDE_ASM("asm/nonmatchings/levels/snowboard_street_shoot_cross", func_800BB690_AD110);

void func_800BBA50(s32 arg0) {
    ACD30Task *task;

    task = (ACD30Task *)scheduleTask(func_800BB2B0_ACD30, 0, 0, 0x32);
    if (task != NULL) {
        task->unk14 = arg0;
    }
}

typedef struct {
    /* 0x00 */ Mat3x3Padded matrix;
    /* 0x20 */ u8 _pad20[0x1C];
    /* 0x3C */ u16 unk3C;
} AD594Arg;

void func_800BBB14_AD594(AD594Arg *arg0);
void func_800BBB70_AD5F0(AD510Arg *arg0);

void func_800BBA90_AD510(AD510Arg *arg0) {
    AD510Allocation *allocation;
    func_80055E68_56A68_result *result;

    allocation = (AD510Allocation *)getCurrentAllocation();
    result = func_80055E68_56A68(allocation->unk5C);
    arg0->unk20 = (void *)((u32)result + 0x90);
    arg0->unk24 = func_80055DC4_569C4(0xD);
    arg0->unk28 = func_80055DF8_569F8(0xD);
    arg0->unk14 = 0x25990000;
    arg0->unk18 = 0x1A2B0000;
    arg0->unk2C = 0;
    arg0->unk1C = 0xF7A30000;
    setCleanupCallback(func_800BBB70_AD5F0);
    setCallback(func_800BBB14_AD594);
}

void func_800BBB14_AD594(AD594Arg *arg0) {
    s32 i;

    arg0->unk3C -= 0x20;
    createYRotationMatrix(&arg0->matrix, arg0->unk3C);

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
    }
}

void func_800BBB70_AD5F0(AD510Arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}
