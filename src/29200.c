#include "29200.h"
#include "20F0.h"
#include "common.h"
#include "rand.h"
#include "task_scheduler.h"

extern u16 D_8008E19C_8ED9C[];
extern u8 D_8008E030_8EC30[][16];
extern s16 D_8008E0C0_8ECC0[];

extern void func_800288D4_294D4(void *);

void func_800288A0_294A0(void);

void func_80028B44_29744(Func80028600Arg *arg0);
void func_80028B80_29780(Func80028600Arg *arg0);

typedef struct {
    u8 pad0[0x418];
    s16 unk418;
    u8 pad41A[7];
    u8 unk421;
    u8 unk422;
    u8 pad423[0x6];
    u8 unk429;
    u8 pad42A[0x3];
    u8 unk42D;
    u8 unk42E;
} AllocationData29200;

void func_80028600_29200(Func80028600Arg *arg0) {
    AllocationData29200 *alloc;
    u8 temp;
    u32 itemType;
    s16 arg5;

    alloc = getCurrentAllocation();
    alloc->unk418 = D_8008E19C_8ED9C[arg0->unk5C];
    temp = D_8008E030_8EC30[arg0->unk5C][randB() & 0xF];
    arg0->unk5D = temp;
    alloc->unk42D = temp;
    itemType = arg0->unk5D;
    arg5 = 5;

    if (itemType == 5 || itemType == 0xB) {
        if (itemType == 5) {
            arg0->model = func_800019B8_25B8(arg0->unk5C + 0x50, alloc, 0, -1, 0, itemType);
            alloc->unk421 = 0;
        } else {
            arg0->model = func_800019B8_25B8(arg0->unk5C + 0x50, alloc, 0, 0, 0, arg5);
            alloc->unk421 = 1;
        }
    } else if (itemType < 0xBU || itemType == 0x10) {
        arg0->model = func_8000198C_258C(arg0->unk5C + 0x50, alloc);
        alloc->unk421 = 0;
    } else {
        arg0->model = func_800019B8_25B8(arg0->unk5C + 0x50, alloc, 0, 0, 0, -1);
        alloc->unk421 = 1;
    }

    setCleanupCallback(func_80028B80_29780);
    setCallback(func_800288A0_294A0);
}

void func_80028744_29344(Func80028600Arg *arg0) {
    AllocationData29200 *alloc;
    u8 temp;
    u32 itemType;
    s16 arg5;

    alloc = getCurrentAllocation();

    do {
        temp = D_8008E030_8EC30[arg0->unk5C][randB() & 0xF];
        arg0->unk5D = temp;
    } while (D_8008E0C0_8ECC0[temp] == 4);

    alloc->unk42D = temp;
    itemType = arg0->unk5D;
    arg5 = 5;

    if (itemType == 5 || itemType == 0xB) {
        if (itemType == 5) {
            arg0->model = func_800019B8_25B8(arg0->unk5C + 0x50, alloc, 0, -1, 0, itemType);
            alloc->unk421 = 0;
        } else {
            arg0->model = func_800019B8_25B8(arg0->unk5C + 0x50, alloc, 0, 0, 0, arg5);
            alloc->unk421 = 1;
        }
    } else if (itemType < 0xBU || itemType == 0x10) {
        arg0->model = func_8000198C_258C(arg0->unk5C + 0x50, alloc);
        alloc->unk421 = 0;
    } else {
        arg0->model = func_800019B8_25B8(arg0->unk5C + 0x50, alloc, 0, 0, 0, -1);
        alloc->unk421 = 1;
    }

    setCallback(func_800288A0_294A0);
}

void func_800288A0_294A0(void) {
    AllocationData29200 *alloc = getCurrentAllocation();

    if (alloc->unk429 == 0) {
        setCallback(func_800288D4_294D4);
    }
}

INCLUDE_ASM("asm/nonmatchings/29200", func_800288D4_294D4);

void func_80028AEC_296EC(Func80028600Arg *arg0) {
    AllocationData29200 *alloc = getCurrentAllocation();

    alloc->unk422 = 1;
    alloc->unk42E = 1;
    arg0->model = func_80002014_2C14(arg0->model);
    arg0->unk5A = (randB() & 0x1F) + 0xF;
    setCallback(func_80028B44_29744);
}

void func_80028B44_29744(Func80028600Arg *arg0) {
    arg0->unk5A--;
    if (arg0->unk5A == 0) {
        arg0->unk5A = 0;
        setCallback(func_80028744_29344);
    }
}

void func_80028B80_29780(Func80028600Arg *arg0) {
    arg0->model = func_80002014_2C14(arg0->model);
}
