#include "90F0.h"
#include "common.h"
#include "task_scheduler.h"

typedef struct {
    u8 _pad0[0x6];
    s16 unk6;
    u8 _pad8[0x18];
    func_80009F5C_AB5C_arg *unk20;
    u8 _pad24[0x48];
    s32 unk6C;
    s16 unk70;
} func_8000B510_C110_arg;

typedef struct {
    void *romStart;
    void *romEnd;
    s32 size;
} DmaEntry;

extern void func_80009E68_AA68(void *, s32);
extern void func_80009F90_AB90(void *, s32, s32, s32);

INCLUDE_ASM("asm/nonmatchings/B040", func_8000A440_B040);

INCLUDE_ASM("asm/nonmatchings/B040", func_8000A49C_B09C);

INCLUDE_ASM("asm/nonmatchings/B040", func_8000A510_B110);

INCLUDE_ASM("asm/nonmatchings/B040", func_8000A834_B434);

void func_8000A8B8_B4B8(void);
void func_8000A988_B588(func_8000B510_C110_arg *);

void func_8000A854_B454(func_8000B510_C110_arg *arg0) {
    func_80009E68_AA68(&arg0->unk20, 0);
    func_80009F90_AB90(&arg0->unk20, 0x10000, arg0->unk6, -1);
    setCleanupCallback(func_8000A988_B588);
    setCallback(func_8000A8B8_B4B8);
}

INCLUDE_ASM("asm/nonmatchings/B040", func_8000A8B8_B4B8);

void func_8000A988_B588(func_8000B510_C110_arg *arg0) {
    func_80009F5C_AB5C(&arg0->unk20);
}

void func_8000AA08_B608(void);
void func_8000AD08_B908(func_8000B510_C110_arg *);

void func_8000A9A4_B5A4(func_8000B510_C110_arg *arg0) {
    func_80009E68_AA68(&arg0->unk20, 0);
    func_80009F90_AB90(&arg0->unk20, 0x10000, arg0->unk6, -1);
    setCleanupCallback(func_8000AD08_B908);
    setCallback(func_8000AA08_B608);
}

INCLUDE_ASM("asm/nonmatchings/B040", func_8000AA08_B608);

void func_8000AD08_B908(func_8000B510_C110_arg *arg0) {
    func_80009F5C_AB5C(&arg0->unk20);
}

void func_8000AD88_B988(void);
void func_8000B028_BC28(func_8000B510_C110_arg *);

void func_8000AD24_B924(func_8000B510_C110_arg *arg0) {
    func_80009E68_AA68(&arg0->unk20, 0);
    func_80009F90_AB90(&arg0->unk20, 0x10000, arg0->unk6, -1);
    setCleanupCallback(func_8000B028_BC28);
    setCallback(func_8000AD88_B988);
}

INCLUDE_ASM("asm/nonmatchings/B040", func_8000AD88_B988);

void func_8000B028_BC28(func_8000B510_C110_arg *arg0) {
    func_80009F5C_AB5C(&arg0->unk20);
}

void func_8000B0A8_BCA8(void);
void func_8000B1B0_BDB0(func_8000B510_C110_arg *);

void func_8000B044_BC44(func_8000B510_C110_arg *arg0) {
    func_80009E68_AA68(&arg0->unk20, 0);
    func_80009F90_AB90(&arg0->unk20, 0x10000, arg0->unk6, -1);
    setCleanupCallback(func_8000B1B0_BDB0);
    setCallback(func_8000B0A8_BCA8);
}

INCLUDE_ASM("asm/nonmatchings/B040", func_8000B0A8_BCA8);

void func_8000B1B0_BDB0(func_8000B510_C110_arg *arg0) {
    func_80009F5C_AB5C(&arg0->unk20);
}

void func_8000B230_BE30(void);
void func_8000B370_BF70(func_8000B510_C110_arg *);

void func_8000B1CC_BDCC(func_8000B510_C110_arg *arg0) {
    func_80009E68_AA68(&arg0->unk20, 0);
    func_80009F90_AB90(&arg0->unk20, 0x10000, arg0->unk6, -1);
    setCleanupCallback(func_8000B370_BF70);
    setCallback(func_8000B230_BE30);
}

INCLUDE_ASM("asm/nonmatchings/B040", func_8000B230_BE30);

void func_8000B370_BF70(func_8000B510_C110_arg *arg0) {
    func_80009F5C_AB5C(&arg0->unk20);
}

INCLUDE_ASM("asm/nonmatchings/B040", func_8000B38C_BF8C);

INCLUDE_ASM("asm/nonmatchings/B040", func_8000B400_C000);

void func_8000B510_C110(func_8000B510_C110_arg *arg0) {
    func_80009F5C_AB5C(&arg0->unk20);
}

void func_8000B598_C198(void);
void func_8000B684_C284(func_8000B510_C110_arg *);

void func_8000B52C_C12C(func_8000B510_C110_arg *arg0) {
    setCleanupCallback(func_8000B684_C284);
    func_80009E68_AA68(&arg0->unk20, 0);
    func_80009F90_AB90(&arg0->unk20, 0x10000, arg0->unk6, -1);
    arg0->unk6C = 0;
    arg0->unk70 = 0;
    setCallback(func_8000B598_C198);
}

INCLUDE_ASM("asm/nonmatchings/B040", func_8000B598_C198);

void func_8000B684_C284(func_8000B510_C110_arg *arg0) {
    func_80009F5C_AB5C(&arg0->unk20);
}

extern DmaEntry *D_8008CC2C_8D82C;
extern s32 D_8008CC30_8D830;

DmaEntry *func_8000B6A0_C2A0(void) {
    return D_8008CC2C_8D82C;
}

s32 func_8000B6AC_C2AC(void) {
    return D_8008CC30_8D830;
}

void *func_8000B6B8_C2B8(s16 arg0) {
    DmaEntry *entry;

    if (arg0 >= func_8000B6AC_C2AC()) {
        return 0;
    }
    entry = &func_8000B6A0_C2A0()[arg0];
    return dmaRequestAndUpdateStateWithSize(entry->romStart, entry->romEnd, entry->size);
}

INCLUDE_ASM("asm/nonmatchings/B040", func_8000B714_C314);

INCLUDE_ASM("asm/nonmatchings/B040", func_8000B770_C370);

INCLUDE_ASM("asm/nonmatchings/B040", func_8000B7FC_C3FC);

INCLUDE_ASM("asm/nonmatchings/B040", func_8000B8D8_C4D8);

INCLUDE_ASM("asm/nonmatchings/B040", func_8000B934_C534);
