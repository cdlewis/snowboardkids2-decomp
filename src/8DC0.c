#include "90F0.h"
#include "common.h"

extern void setCleanupCallback(void *);
extern void setCallback(void *);
extern void func_80009E68_AA68(void *, s32);
extern void func_80009F90_AB90(void *, s32, s32, s32);

void func_800081EC_8DEC(void);
void func_80008308_8F08(void);
void func_80008370_8F70(void);

typedef struct {
    u8 unk0[0x4];
    func_80009F5C_AB5C_arg *unk4;
    u8 unk8[0x48];
    u8 unk50;
} Arg8F10;

void func_800084A8_90A8(Arg8F10 *arg0);

void func_800081C0_8DC0(void) {
    setCleanupCallback(func_80008308_8F08);
    setCallback(func_800081EC_8DEC);
}

INCLUDE_ASM("asm/nonmatchings/8DC0", func_800081EC_8DEC);

void func_80008308_8F08(void) {
}

void func_80008310_8F10(Arg8F10 *arg0) {
    arg0->unk50 = 0;
    func_80009E68_AA68(&arg0->unk4, 9);
    func_80009F90_AB90(&arg0->unk4, 0x10000, 0, -1);
    setCleanupCallback(func_800084A8_90A8);
    setCallback(func_80008370_8F70);
}

INCLUDE_ASM("asm/nonmatchings/8DC0", func_80008370_8F70);

void func_800084A8_90A8(Arg8F10 *arg0) {
    func_80009F5C_AB5C(&arg0->unk4);
}
