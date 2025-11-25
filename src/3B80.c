#include "common.h"
#include "task_scheduler.h"

typedef struct {
    u8 padding[0xFF5];
    s8 unkFF5;
} func_80002FD4_3BD4_inner;

typedef struct {
    func_80002FD4_3BD4_inner *unk0;
    void *unk4;
} func_80002FD4_3BD4_arg;

extern void func_80000760_1360(void *);
void func_80002FD4_3BD4(func_80002FD4_3BD4_arg *arg0);

void func_80002F80_3B80(void *arg0, void *arg1) {
    func_80002FD4_3BD4_arg *task;

    task = scheduleTask(func_80002FD4_3BD4, 2, 0, 0xF0);
    if (task != NULL) {
        task->unk0 = arg0;
        task->unk4 = arg1;
    }
}

void func_80002FD4_3BD4(func_80002FD4_3BD4_arg *arg0) {
    if (arg0->unk0->unkFF5 != 0) {
        func_80000760_1360(arg0->unk4);
    }
}

INCLUDE_ASM("asm/nonmatchings/3B80", func_80003000_3C00);

typedef struct {
    void *unk0;
    void *unk4;
    s32 unk8;
} AssetEntry;

typedef struct {
    AssetEntry *unk0;
    u8 _pad4[0x4];
    u8 unk8;
    u8 _pad9[0x3];
} func_800033AC_3FAC_TableEntry;

typedef struct {
    void *unk0;
    void **unk4;
    void *unk8;
    void *unkC;
    void *unk10;
    u8 _pad14[0xB0];
    u8 unkC4;
    u8 unkC5;
} func_800033AC_3FAC_arg;

extern func_800033AC_3FAC_TableEntry D_8008BF70_8CB70[];
extern void func_80003184_3D84(void *);
void func_800033AC_3FAC(func_800033AC_3FAC_arg *arg0);

void func_800030B4_3CB4(func_800033AC_3FAC_arg *arg0) {
    func_800033AC_3FAC_TableEntry *entry;
    AssetEntry *assetList;
    s32 i;

    entry = &D_8008BF70_8CB70[arg0->unkC4];
    assetList = entry->unk0;

    setCleanupCallback(func_800033AC_3FAC);
    i = 0;

    arg0->unk10 = NULL;
    arg0->unkC = NULL;

    arg0->unk4 = allocateNodeMemory(entry->unk8 * 4);

    for (i = 0; i < entry->unk8; i++) {
        arg0->unk4[i] = dmaRequestAndUpdateStateWithSize(assetList[i].unk0, assetList[i].unk4, assetList[i].unk8);
    }

    setCallback(func_80003184_3D84);
}

INCLUDE_ASM("asm/nonmatchings/3B80", func_80003184_3D84);

void func_800033AC_3FAC(func_800033AC_3FAC_arg *arg0) {
    func_800033AC_3FAC_TableEntry *entry;
    s32 i;
    u8 index;

    index = arg0->unkC4;
    entry = &D_8008BF70_8CB70[index];

    for (i = 0; i < entry->unk8; i++) {
        arg0->unk4[i] = freeNodeMemory(arg0->unk4[i]);
    }

    arg0->unk4 = freeNodeMemory(arg0->unk4);
}
