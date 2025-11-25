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

INCLUDE_ASM("asm/nonmatchings/3B80", func_800030B4_3CB4);

INCLUDE_ASM("asm/nonmatchings/3B80", func_80003184_3D84);

typedef struct {
    void *unk0;
    u8 _pad4[0x4];
    u8 unk8;
    u8 _pad9[0x3];
} func_800033AC_3FAC_TableEntry; // size 0xC

typedef struct {
    void *unk0;
    void **unk4;
    void *unk8;
    u8 _padC[0xB8];
    u8 unkC4;
    u8 unkC5;
} func_800033AC_3FAC_arg;

extern func_800033AC_3FAC_TableEntry D_8008BF70_8CB70[];

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
