#include "20F0.h"
#include "common.h"
#include "task_scheduler.h"

extern s16 D_800BAD0E_1E7DBE;
extern s16 D_800BAA60_1E7B10[];
extern void func_80057ABC_586BC(s32, s32);
extern void func_80058360_58F60(s32, s32, s32, s32);
extern void func_80057DF0_589F0(s32, s32, s32, s32, s32);

s16 func_800B4AF0_1E1BA0(void) {
    return D_800BAD0E_1E7DBE;
}

s16 func_800B4AFC_1E1BAC(s16 arg0) {
    s16 temp_v1;
    s16 temp_v0;
    s16 index;

    temp_v1 = D_800BAD0E_1E7DBE;
    temp_v0 = arg0;

    if (temp_v0 < temp_v1) {
        index = arg0;
    } else {
        index = 0;
    }

    return D_800BAA60_1E7B10[index];
}

void func_800B4B30_1E1BE0(s16 arg0, s16 arg1, s16 arg2, s16 arg3) {
    s16 temp;

    if (arg3 <= 0) {
        temp = func_800B4AFC_1E1BAC(arg0);
        func_80058360_58F60(temp, arg1, arg2 + 0x80, 0);
    } else {
        temp = func_800B4AFC_1E1BAC(arg0);
        func_80057DF0_589F0(temp, arg1, arg2 + 0x80, 0, arg3);
    }
}

INCLUDE_ASM("asm/nonmatchings/1E1BA0", func_800B4BDC_1E1C8C);

extern void func_80058220_58E20(s32, s32);

void func_800B4C80_1E1D30(s16 arg0) {
    s16 temp;

    temp = func_800B4AFC_1E1BAC(arg0);
    func_80058220_58E20(temp, 0);
}

void func_800B4CB0_1E1D60(void) {
    func_80057ABC_586BC(0, 0);
}

INCLUDE_ASM("asm/nonmatchings/1E1BA0", func_800B4CD0_1E1D80);

INCLUDE_ASM("asm/nonmatchings/1E1BA0", func_800B4D74_1E1E24);

extern void func_80069CF8_6A8F8(void);

typedef struct {
    u8 _pad0[0x18];
    s32 unk18;
    u8 _pad1C[0x20];
    u8 unk3C[0x40];
    s32 unk7C;
    u8 _pad80[0x2];
    u16 unk82;
} func_800B4E7C_Struct;

void func_800B4E7C_1E1F2C(func_800B4E7C_Struct *arg0) {
    Mat3x3Padded matrix;
    s32 temp_v0;

    temp_v0 = arg0->unk7C - 0x8000;
    arg0->unk7C = temp_v0;

    if ((s32)0xFFF80000 >= temp_v0) {
        func_80069CF8_6A8F8();
    }

    arg0->unk18 += arg0->unk7C;
    createZRotationMatrix(&matrix, arg0->unk82);

    matrix.unk18 = 0xBB333;
    matrix.unk14 = 0;
    matrix.unk1C = 0xFFEA0000;

    func_8006B084_6BC84(&matrix, (DisplayListObject *)arg0, (DisplayListObject *)&arg0->unk3C);
    enqueueDisplayListObject(0, (DisplayListObject *)arg0);
    enqueueDisplayListObject(0, (DisplayListObject *)&arg0->unk3C);
}

void func_800B4F28_1E1FD8(SceneModel_unk98 *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

typedef struct {
    u8 padding[0x78];
    s32 unk78;
    u8 padding2[4];
    s16 unk80;
    u8 padding3[4];
    s16 unk86;
} func_800B4F60_1E2010_task;

extern void func_800B4CD0_1E1D80(void *);

void func_800B4F60_1E2010(s32 arg0, s16 arg1) {
    func_800B4F60_1E2010_task *task = (func_800B4F60_1E2010_task *)scheduleTask(&func_800B4CD0_1E1D80, 1, 0, 0x64);

    if (task != NULL) {
        task->unk78 = arg0;
        task->unk80 = 0;
        task->unk86 = arg1;
    }
}
