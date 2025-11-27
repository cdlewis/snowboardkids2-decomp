#include "common.h"
#include "overlay.h"
#include "rand.h"
#include "task_scheduler.h"

USE_ASSET(_647F90);

extern void *D_8008CDD0_8D9D0;

void func_8000CEA4_DAA4(void *);
void func_8000CFA0_DBA0(void **);

typedef struct {
    void *unk0;
    void *unk4;
    u8 padding[0x16];
    u8 unk1E;
    u8 padding2[0x5];
    void *unk24;
    u8 padding3[0x16];
    u8 unk3E;
    u8 padding4[0x11];
    s16 unk50;
} func_8000CE20_DA20_arg;

typedef struct {
    u8 padding[0x8];
    u8 unk8[0xC];
    u8 padding2[0x14];
    u8 unk28[0xC];
    u8 padding3[0x10];
    s32 unk44;
    s32 unk48;
    s32 unk4C;
    u8 padding4[0x4];
    s16 unk54;
} func_8000CE20_DA20_task_memory;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} func_8000CFCC_DBCC_arg2;

void func_8000CE20_DA20(func_8000CE20_DA20_arg *arg0) {
    arg0->unk0 = dmaRequestAndUpdateStateWithSize(&_647F90_ROM_START, &_647F90_ROM_END, 0xF18);
    arg0->unk4 = &D_8008CDD0_8D9D0;
    arg0->unk1E = (randA() & 0x1F) + 0x70;
    arg0->unk50 = 0;
    arg0->unk24 = arg0->unk4;
    arg0->unk3E = arg0->unk1E;
    setCleanupCallback(&func_8000CFA0_DBA0);
    setCallbackWithContinue(&func_8000CEA4_DAA4);
}

INCLUDE_ASM("asm/nonmatchings/DA20", func_8000CEA4_DAA4);

void func_8000CFA0_DBA0(void **arg0) {
    *arg0 = freeNodeMemory(*arg0);
}

void func_8000CFCC_DBCC(void *arg0, void *arg1, func_8000CFCC_DBCC_arg2 *arg2) {
    func_8000CE20_DA20_task_memory *task = (func_8000CE20_DA20_task_memory *)scheduleTask(&func_8000CE20_DA20, 0, 0, 0);
    if (task != NULL) {
        memcpy(&task->unk8, arg0, 0xC);
        memcpy(&task->unk28, arg1, 0xC);
        task->unk54 = 0;
        task->unk44 = arg2->unk0 / 2;
        task->unk48 = arg2->unk4 / 2;
        task->unk4C = arg2->unk8 / 2;
    }
}
