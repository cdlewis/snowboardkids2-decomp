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

INCLUDE_ASM("asm/nonmatchings/DA20", func_8000CFCC_DBCC);
