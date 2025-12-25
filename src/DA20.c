#include "common.h"
#include "displaylist.h"
#include "overlay.h"
#include "rand.h"
#include "task_scheduler.h"

USE_ASSET(_647F90);

extern void *D_8008CDD0_8D9D0;

void func_8000CFA0_DBA0(void **);

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
} DA20_PositionNode;

typedef struct {
    void *unk0;
    loadAssetMetadata_arg unk4;
    u8 padding[4];
    loadAssetMetadata_arg unk24;
    u8 padding2[4];
    s32 unk44;
    s32 unk48;
    s32 unk4C;
    s16 unk50;
} func_8000CEA4_DAA4_arg;

void func_8000CEA4_DAA4(func_8000CEA4_DAA4_arg *);

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
    arg0->unk0 = loadCompressedData(&_647F90_ROM_START, &_647F90_ROM_END, 0xF18);
    arg0->unk4 = &D_8008CDD0_8D9D0;
    arg0->unk1E = (randA() & 0x1F) + 0x70;
    arg0->unk50 = 0;
    arg0->unk24 = arg0->unk4;
    arg0->unk3E = arg0->unk1E;
    setCleanupCallback(&func_8000CFA0_DBA0);
    setCallbackWithContinue(&func_8000CEA4_DAA4);
}

void func_8000CEA4_DAA4(func_8000CEA4_DAA4_arg *arg0) {
    s32 i;
    loadAssetMetadata_arg *unk4_ptr = &arg0->unk4;

    loadAssetMetadata(unk4_ptr, arg0->unk0, arg0->unk50 / 4);

    arg0->unk24.data_ptr = arg0->unk4.data_ptr;
    arg0->unk24.index_ptr = arg0->unk4.index_ptr;
    arg0->unk24.unk18 = arg0->unk4.unk18;
    arg0->unk24.unk19 = arg0->unk4.unk19;

    func_80067EDC_68ADC(0, unk4_ptr);
    func_80067EDC_68ADC(0, &arg0->unk24);

    if (arg0->unk50 != 0) {
        DA20_PositionNode *node = (DA20_PositionNode *)arg0;
        for (i = 0; i < 2; i++) {
            node[i].unk8 += arg0->unk44;
            node[i].unkC += arg0->unk48;
            node[i].unk10 += arg0->unk4C;
        }
    }

    arg0->unk50++;
    if (arg0->unk50 == 20) {
        func_80069CF8_6A8F8();
    }
}

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
