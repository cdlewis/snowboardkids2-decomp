#include "90F0.h"
#include "common.h"
#include "rand.h"
#include "task_scheduler.h"

extern s32 func_8000A030_AC30(void *, s32);
extern void func_8000A13C_AD3C(void *, u16, s32, s32, s32, s32, s32, s32, s32);

typedef struct {
    u8 _pad[0xC];
    s16 unkC;
    u8 _pad2[0x2];
    struct {
        u8 padding[0x16];
        u16 unk16;
    } *unk10;
    u8 _pad3[0x18];
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    u8 _pad4[0x4];
    s8 unk3C;
    s8 unk3D;
    s8 unk3E;
    s8 unk3F;
    u8 _pad5[0x48];
    s8 unk88;
} func_80006940_inner;

typedef struct {
    func_80006940_inner *unk0;
    func_80009F5C_AB5C_arg *unk4;
    u8 _pad8[0x50 - 0x8];
    s32 unk50;
    s32 unk54;
    s32 unk58;
    s32 unk5C;
    s32 unk60;
    s32 unk64;
    s16 unk68;
    s16 unk6A;
    s32 unk6C;
    s32 unk70;
    s32 unk74;
    s32 unk78;
    s32 unk7C;
    s32 unk80;
    s16 unk84;
    s16 unk86;
    s16 unk88;
    s16 unk8A;
    s8 unk8C;
} func_80006398_6F98_arg;

extern void func_80009E68_AA68(void *, s32);
extern void func_80009F90_AB90(void *, s32, s32, s32);
void func_80006A88_7688(func_80006398_6F98_arg *);
void func_800069B4_75B4(func_80006398_6F98_arg *);
void func_80006398_6F98(func_80006398_6F98_arg *);
void func_8000629C_6E9C(func_80006398_6F98_arg *);
void func_80006924_7524(func_80006398_6F98_arg *);
void func_80006458_7058(func_80006398_6F98_arg *);

void func_800061D0_6DD0(func_80006398_6F98_arg *arg0) {
    s32 rand1;
    s32 rand2;
    u8 rand3;

    func_80009E68_AA68(&arg0->unk4, 2);
    func_80009F90_AB90(&arg0->unk4, 0x10000, 1, -1);

    rand1 = randA();
    rand1 = rand1 & 0xFF;
    rand1 = rand1 << 11;
    rand2 = randA();

    arg0->unk5C = rand1 + 0xFFFC0000;
    arg0->unk60 = 0;
    arg0->unk64 = (((u8)rand2) << 11) + 0xFFFC0000;

    rand3 = randA();
    arg0->unk6A = (rand3 % 3) + 2;

    setCleanupCallback(func_80006398_6F98);
    setCallback(func_8000629C_6E9C);
}

INCLUDE_ASM("asm/nonmatchings/6DD0", func_8000629C_6E9C);

void func_80006398_6F98(func_80006398_6F98_arg *arg0) {
    func_80009F5C_AB5C(&arg0->unk4);
}

void func_800063B4_6FB4(func_80006398_6F98_arg *arg0) {
    func_80009E68_AA68(&arg0->unk4, 2);
    func_80009F90_AB90(&arg0->unk4, 0x10000, 0, -1);

    arg0->unk50 = 0x10000;
    arg0->unk5C = 0;
    arg0->unk58 = 0;
    arg0->unk54 = 0;
    arg0->unk74 = 0;
    arg0->unk70 = 0;
    arg0->unk6C = 0;
    arg0->unk80 = 0;
    arg0->unk7C = 0;
    arg0->unk78 = 0;
    arg0->unk88 = 0;
    arg0->unk86 = 0;
    arg0->unk84 = 0;
    arg0->unk8A = 0;
    arg0->unk8C = 0;

    setCleanupCallback(func_80006924_7524);
    setCallback(func_80006458_7058);
}

INCLUDE_ASM("asm/nonmatchings/6DD0", func_80006458_7058);

void func_80006924_7524(func_80006398_6F98_arg *arg0) {
    func_80009F5C_AB5C(&arg0->unk4);
}

void func_80006940_7540(func_80006398_6F98_arg *arg0) {
    func_80006940_inner *inner = arg0->unk0;

    if (inner->unkC == 0x4F) {
        func_80009E68_AA68(&arg0->unk4, 6);
    } else {
        func_80009E68_AA68(&arg0->unk4, 3);
    }
    func_80009F90_AB90(&arg0->unk4, 0x10000, 0, -1);
    setCleanupCallback(func_80006A88_7688);
    setCallback(func_800069B4_75B4);
}

void func_800069B4_75B4(func_80006398_6F98_arg *arg0) {
    s8 unused[2];
    func_80006940_inner *inner;
    func_80006940_inner *a0_inner;
    s32 s3;
    s32 s4;
    s32 s2;

    do {
        unused[0] = 1;
    } while (0);

    do {
        unused[1] = -1;
    } while (0);

    inner = arg0->unk0;

    if (inner->unk3C == 1) {
        func_80069CF8_6A8F8();
        return;
    }

    s3 = inner->unk2C;
    s4 = inner->unk30;
    s2 = inner->unk34;

    func_8000A030_AC30(&arg0->unk4, 0x10000);

    a0_inner = arg0->unk0;
    if (a0_inner->unk88 != 0) {
        if (a0_inner->unk3F != 0) {
            func_8000A13C_AD3C(&arg0->unk4, a0_inner->unk10->unk16, s3, s4, s2, 0x10000, 0x10000, 0, 0);
        }
    }
}

void func_80006A88_7688(func_80006398_6F98_arg *arg0) {
    func_80009F5C_AB5C(&arg0->unk4);
}
