#include "5E590.h"
#include "common.h"
#include "task_scheduler.h"

s32 func_80052418_53018(s32, s32);
s32 func_80052A24_53624(s32, s32);
s32 func_80053078_53C78(s32, s32);
s32 func_800537B0_543B0(s32, s32);
s32 func_80053DF0_549F0(s32, s32);
s32 func_80054470_55070(s32, s32);
s32 func_80055820_56420(s32, s32);

typedef struct {
    u8 padding[0x20];
    void *unk20;
    u8 padding2[0x1E];
    u16 unk42;
    u8 padding3[0x8];
    u16 unk4C;
} Struct_52880;

INCLUDE_ASM("asm/nonmatchings/52880", func_80051C80_52880);

INCLUDE_ASM("asm/nonmatchings/52880", func_80051DEC_529EC);

INCLUDE_ASM("asm/nonmatchings/52880", func_80051E34_52A34);

INCLUDE_ASM("asm/nonmatchings/52880", func_80051E90_52A90);

INCLUDE_ASM("asm/nonmatchings/52880", func_80051FC4_52BC4);

INCLUDE_ASM("asm/nonmatchings/52880", func_80052128_52D28);

INCLUDE_ASM("asm/nonmatchings/52880", func_800523EC_52FEC);

INCLUDE_ASM("asm/nonmatchings/52880", func_80052418_53018);

INCLUDE_ASM("asm/nonmatchings/52880", func_8005245C_5305C);

INCLUDE_ASM("asm/nonmatchings/52880", func_800524A4_530A4);

INCLUDE_ASM("asm/nonmatchings/52880", func_80052500_53100);

INCLUDE_ASM("asm/nonmatchings/52880", func_800525F4_531F4);

INCLUDE_ASM("asm/nonmatchings/52880", func_80052758_53358);

INCLUDE_ASM("asm/nonmatchings/52880", func_80052A24_53624);

void func_800523EC_52FEC(void);
void func_80052AB0_536B0(void);

void func_80052A68_53668(Struct_52880 *arg0) {
    arg0->unk4C = arg0->unk42;
    arg0->unk20 = load_3ECE40();
    setCleanupCallback(func_800523EC_52FEC);
    setCallbackWithContinue(func_80052AB0_536B0);
}

INCLUDE_ASM("asm/nonmatchings/52880", func_80052AB0_536B0);

INCLUDE_ASM("asm/nonmatchings/52880", func_80052B0C_5370C);

INCLUDE_ASM("asm/nonmatchings/52880", func_80052C00_53800);

INCLUDE_ASM("asm/nonmatchings/52880", func_80052DB4_539B4);

INCLUDE_ASM("asm/nonmatchings/52880", func_80053078_53C78);

INCLUDE_ASM("asm/nonmatchings/52880", func_800530BC_53CBC);

INCLUDE_ASM("asm/nonmatchings/52880", func_80053104_53D04);

INCLUDE_ASM("asm/nonmatchings/52880", func_80053160_53D60);

INCLUDE_ASM("asm/nonmatchings/52880", func_80053254_53E54);

INCLUDE_ASM("asm/nonmatchings/52880", func_80053434_54034);

INCLUDE_ASM("asm/nonmatchings/52880", func_80053784_54384);

INCLUDE_ASM("asm/nonmatchings/52880", func_800537B0_543B0);

INCLUDE_ASM("asm/nonmatchings/52880", func_800537F4_543F4);

INCLUDE_ASM("asm/nonmatchings/52880", func_8005383C_5443C);

INCLUDE_ASM("asm/nonmatchings/52880", func_80053898_54498);

INCLUDE_ASM("asm/nonmatchings/52880", func_80053990_54590);

INCLUDE_ASM("asm/nonmatchings/52880", func_80053B38_54738);

INCLUDE_ASM("asm/nonmatchings/52880", func_80053DF0_549F0);

INCLUDE_ASM("asm/nonmatchings/52880", func_80053E48_54A48);

INCLUDE_ASM("asm/nonmatchings/52880", func_80053E90_54A90);

INCLUDE_ASM("asm/nonmatchings/52880", func_80053EEC_54AEC);

INCLUDE_ASM("asm/nonmatchings/52880", func_80053FE0_54BE0);

INCLUDE_ASM("asm/nonmatchings/52880", func_80054144_54D44);

INCLUDE_ASM("asm/nonmatchings/52880", func_80054470_55070);

s32 func_800544B4_550B4(s32 arg0, s32 arg1, s32 arg2) {
    switch (arg0) {
        case 0:
            return func_80052418_53018(arg1, arg2);
        case 1:
            return func_80052A24_53624(arg1, arg2);
        case 2:
            return func_80053078_53C78(arg1, arg2);
        case 3:
            return func_800537B0_543B0(arg1, arg2);
        case 4:
            return func_80053DF0_549F0(arg1, arg2);
        case 5:
            return func_80054470_55070(arg1, arg2);
        case 6:
            return func_80055820_56420(arg1, arg2);
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/52880", func_80054568_55168);

INCLUDE_ASM("asm/nonmatchings/52880", func_800545B0_551B0);

INCLUDE_ASM("asm/nonmatchings/52880", func_800545F8_551F8);

INCLUDE_ASM("asm/nonmatchings/52880", func_80054658_55258);

INCLUDE_ASM("asm/nonmatchings/52880", func_800547E0_553E0);

extern void func_800548C8_554C8(void);

void func_80054880_55480(Struct_52880 *arg0) {
    arg0->unk4C = arg0->unk42;
    arg0->unk20 = load_3ECE40();
    setCleanupCallback(func_800523EC_52FEC);
    setCallbackWithContinue(func_800548C8_554C8);
}

INCLUDE_ASM("asm/nonmatchings/52880", func_800548C8_554C8);

INCLUDE_ASM("asm/nonmatchings/52880", func_80054AE4_556E4);

INCLUDE_ASM("asm/nonmatchings/52880", func_80054C8C_5588C);

typedef struct {
    u8 padding[0x20];
    void *unk20;
} func_80054CCC_558CC_arg;

extern void func_800553A8_55FA8(void);
extern void func_80054D0C_5590C(void);

void func_80054CCC_558CC(func_80054CCC_558CC_arg *arg0) {
    arg0->unk20 = load_3ECE40();
    setCleanupCallback(func_800553A8_55FA8);
    setCallbackWithContinue(func_80054D0C_5590C);
}

INCLUDE_ASM("asm/nonmatchings/52880", func_80054D0C_5590C);

INCLUDE_ASM("asm/nonmatchings/52880", func_80054D70_55970);

INCLUDE_ASM("asm/nonmatchings/52880", func_80054F44_55B44);

INCLUDE_ASM("asm/nonmatchings/52880", func_800550B4_55CB4);

INCLUDE_ASM("asm/nonmatchings/52880", func_800553A8_55FA8);

INCLUDE_ASM("asm/nonmatchings/52880", func_800553D4_55FD4);

extern void func_80055460_56060(void);

void func_80055418_56018(Struct_52880 *arg0) {
    arg0->unk4C = arg0->unk42;
    arg0->unk20 = load_3ECE40();
    setCleanupCallback(func_800523EC_52FEC);
    setCallbackWithContinue(func_80055460_56060);
}

INCLUDE_ASM("asm/nonmatchings/52880", func_80055460_56060);

INCLUDE_ASM("asm/nonmatchings/52880", func_800554BC_560BC);

INCLUDE_ASM("asm/nonmatchings/52880", func_800554FC_560FC);

INCLUDE_ASM("asm/nonmatchings/52880", func_80055650_56250);

INCLUDE_ASM("asm/nonmatchings/52880", func_80055820_56420);

typedef struct {
    u8 padding[0x20];
    void *unk20;
} func_80055864_56464_arg;

extern void func_800558A4_564A4(void);

void func_80055864_56464(func_80055864_56464_arg *arg0) {
    arg0->unk20 = load_3ECE40();
    setCleanupCallback(func_800523EC_52FEC);
    setCallbackWithContinue(func_800558A4_564A4);
}

INCLUDE_ASM("asm/nonmatchings/52880", func_800558A4_564A4);

INCLUDE_ASM("asm/nonmatchings/52880", func_80055900_56500);

INCLUDE_ASM("asm/nonmatchings/52880", func_80055964_56564);

INCLUDE_ASM("asm/nonmatchings/52880", func_80055A84_56684);

INCLUDE_ASM("asm/nonmatchings/52880", func_80055C80_56880);
