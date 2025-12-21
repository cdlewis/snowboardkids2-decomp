#include "288A0.h"
#include "3E160.h"
#include "6E840.h"
#include "rand.h"
#include "task_scheduler.h"

extern ColorData D_8008DEF0_8EAF0;
extern ColorData D_8008DEF8_8EAF8;

extern u8 D_8008DF00_8EB00[][10];
extern u8 D_8008DF3C_8EB3C[][8];
extern u16 D_8009ADE0_9B9E0;

typedef struct {
    u8 pad0[0x9];
    u8 unk9;
} GameConfigLocal_288A0;

extern GameConfigLocal_288A0 *D_800AFE8C_A71FC;
extern void func_80028600_29200(void *);
extern void func_8002BCF0_2C8F0(void *);

void func_800285EC_291EC(void);

typedef struct {
    u8 pad0[0x41C];
    s32 unk41C;
    u8 unk420;
    u8 pad421[0xB];
    u8 unk42C;
} AllocationLocal_288A0;

typedef struct {
    u8 pad0[0x5C];
    u8 unk5C;
} Task5C_288A0;

typedef struct {
    u8 pad0[0xD4];
    u8 unkD4;
} TaskD4_288A0;

void func_80027CA0_288A0(Node_70B00 *arg0, s32 arg1, s32 arg2, s32 arg3) {
    s8 sp20[0x20];
    s32 temp_a3;

    temp_a3 = arg3 & 0xFF;
    if (temp_a3 == 0) {
        func_8006FAA4_706A4(arg0, 0, arg1 & 0xFF, arg2 & 0xFF, 1);
    } else {
        func_8006FAA4_706A4(arg0, 0, arg1 & 0xFF, arg2 & 0xFF, 0);
    }

    func_8006F9BC_705BC(arg0, 1.0f, 1.0f);
    func_8006FEF8_70AF8(arg0, ((arg1 & 0xFF) + 1));
    setModelCameraTransform(arg0, 0, 0, -0x98, -0x70, 0x97, 0x6F);
    func_8006FA0C_7060C(arg0, 50.0f, 1.3333334f, 10.0f, 4000.0f);
    func_8006FE48_70A48(arg0->id, 0x3D4, 0x3E6, 0x64, 0x64, 0x64);
    func_8006BEDC_6CADC(&sp20, 0, 0, 0x200000, 0, 0, 0);
    func_8006FD3C_7093C(arg0->id, &sp20);
    func_8006FE28_70A28(arg0, 0, 0, 0);
    func_8006FDA0_709A0(arg0, 0, 0);
    func_8006FC70_70870(arg0->id, 1, &D_8008DEF0_8EAF0, &D_8008DEF8_8EAF8);
}

INCLUDE_ASM("asm/nonmatchings/288A0", func_80027E04_28A04);

INCLUDE_ASM("asm/nonmatchings/288A0", func_80028074_28C74);

void func_80028480_29080(u8 *arg0) {
    AllocationLocal_288A0 *alloc;
    u8 tableValue;
    u8 randVal;
    Task5C_288A0 *task;
    TaskD4_288A0 *task2;

    alloc = getCurrentAllocation();
    alloc->unk41C = 0;
    alloc->unk420 = 0xFF;

    if ((randA() & 7) == 7) {
        randVal = randB();
        *arg0 = D_8008DF00_8EB00[D_800AFE8C_A71FC->unk9][randVal % 10] - 1;
        task2 = scheduleTask(func_8002BCF0_2C8F0, 0, 0, 0x5B);
        task2->unkD4 = *arg0;
        alloc->unk42C = *arg0;
        goto check_s2;
    }

    tableValue = D_8008DF3C_8EB3C[D_800AFE8C_A71FC->unk9][D_8009ADE0_9B9E0 & 7];
    if (tableValue != 0) {
        task = scheduleTask(func_80028600_29200, 0, 0, 0x5B);
        task->unk5C = tableValue - 1;
        alloc->unk420 = tableValue - 1;
    }

check_s2:
    if (tableValue != 0) {
        alloc->unk41C = 1;
        setCallback(func_800285EC_291EC);
    } else {
        func_80069CF8_6A8F8();
    }
}

void func_800285EC_291EC(void) {
}
