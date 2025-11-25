#include "90F0.h"
#include "common.h"
#include "rand.h"

typedef struct {
    u8 _pad[0xC];
    s16 unkC;
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
extern void setCleanupCallback(void *);
extern void setCallback(void *);
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

INCLUDE_ASM("asm/nonmatchings/6DD0", func_80006940_7540);

INCLUDE_ASM("asm/nonmatchings/6DD0", func_800069B4_75B4);

void func_80006A88_7688(func_80006398_6F98_arg *arg0) {
    func_80009F5C_AB5C(&arg0->unk4);
}
