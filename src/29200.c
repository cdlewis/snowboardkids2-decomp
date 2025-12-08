#include "20F0.h"
#include "common.h"
#include "rand.h"
#include "task_scheduler.h"
extern void func_800288D4_294D4(void *);
extern void func_80028744_29344(void *);

typedef struct {
    void *model;
    u8 pad4[0x56];
    s16 unk5A;
} Func80028AECArg;

void func_80028B44_29744(Func80028AECArg *arg0);

typedef struct {
    u8 pad0[0x422];
    u8 unk422;
    u8 pad423[0x6];
    u8 unk429;
    u8 pad42A[0x4];
    u8 unk42E;
} AllocationData29200;

INCLUDE_ASM("asm/nonmatchings/29200", func_80028600_29200);

INCLUDE_ASM("asm/nonmatchings/29200", func_80028744_29344);

void func_800288A0_294A0(void) {
    AllocationData29200 *alloc = getCurrentAllocation();

    if (alloc->unk429 == 0) {
        setCallback(func_800288D4_294D4);
    }
}

INCLUDE_ASM("asm/nonmatchings/29200", func_800288D4_294D4);

void func_80028AEC_296EC(Func80028AECArg *arg0) {
    AllocationData29200 *alloc = getCurrentAllocation();

    alloc->unk422 = 1;
    alloc->unk42E = 1;
    arg0->model = func_80002014_2C14(arg0->model);
    arg0->unk5A = (randB() & 0x1F) + 0xF;
    setCallback(func_80028B44_29744);
}

void func_80028B44_29744(Func80028AECArg *arg0) {
    arg0->unk5A--;
    if (arg0->unk5A == 0) {
        arg0->unk5A = 0;
        setCallback(func_80028744_29344);
    }
}

void func_80028B80_29780(Func80028AECArg *arg0) {
    arg0->model = func_80002014_2C14(arg0->model);
}
