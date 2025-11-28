#include "common.h"
#include "task_scheduler.h"

extern u8 randA(void);

typedef struct {
    u8 _pad0[0x24];
    void *unk24;
    void *unk28;
} B4240Struct;

typedef struct {
    u8 _pad0[0x76];
    u8 unk76;
} B4240AllocationStruct;

typedef struct {
    u8 _pad0[0x48];
    s32 unk48;
    s32 unk4C;
    u8 _pad2[0x8];
    s16 unk58;
    s16 unk5A;
} B4240FuncArg;

void func_800BB2B0_B4240(B4240FuncArg *);
void func_800BB320_B42B0(B4240FuncArg *);
void func_800BB63C_B45CC(B4240FuncArg *);
void func_800BB89C_B482C(B4240FuncArg *);

INCLUDE_ASM("asm/nonmatchings/B4240", func_800BB2B0_B4240);

INCLUDE_ASM("asm/nonmatchings/B4240", func_800BB320_B42B0);

INCLUDE_ASM("asm/nonmatchings/B4240", func_800BB454_B43E4);

void func_800BB598_B4528(B4240FuncArg *arg0) {
    B4240AllocationStruct *alloc;
    void (*callback)(B4240FuncArg *);
    s16 val;

    alloc = getCurrentAllocation();

    if (arg0->unk58 == 0) {
        if (randA() & 1) {
            callback = func_800BB63C_B45CC;
            arg0->unk4C = 0x30000;
            val = 3;
        } else {
            callback = func_800BB89C_B482C;
            arg0->unk4C = 0x20000;
            val = 6;
        }
        arg0->unk48 = 0;
        arg0->unk5A = val;
        setCallback(callback);
    }

    if (alloc->unk76 == 0) {
        arg0->unk58--;
        func_800BB320_B42B0(arg0);
    }

    func_800BB2B0_B4240(arg0);
}

INCLUDE_ASM("asm/nonmatchings/B4240", func_800BB63C_B45CC);

INCLUDE_ASM("asm/nonmatchings/B4240", func_800BB89C_B482C);

void func_800BBAFC_B4A8C(B4240Struct *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

INCLUDE_ASM("asm/nonmatchings/B4240", func_800BBB34);
