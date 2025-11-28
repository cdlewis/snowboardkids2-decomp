#include "5E590.h"
#include "common.h"
#include "displaylist.h"
#include "task_scheduler.h"

s32 func_80052418_53018(s32, s32);
s32 func_80052A24_53624(s32, s32);
s32 func_80053078_53C78(s32, s32);
s32 func_800537B0_543B0(s32, s32);
s32 func_80053DF0_549F0(s32, s32);
s32 func_80054470_55070(s32, s32);
s32 func_80055820_56420(s32, s32);

typedef struct {
    u8 pad[0xB84];
    u32 unkB84;
} Struct_B548_Result;

extern void *getCurrentAllocation(void);
extern Struct_B548_Result *func_8005B548_5C148(void *arg0, s16 arg1, s32 arg2);
extern void func_80058A68_59668(Player *arg0);
extern s32 func_8005C250_5CE50(void *arg0, s16 arg1, s32 arg2);
extern s32 func_800BB504(void *, s32);

typedef struct {
    void *unk0;
    u8 unk4;
    u8 padding1[0x1B];
    void *unk20;
    u8 padding2[0x1C];
    u16 unk40;
    u16 unk42;
    u8 padding3[0x2];
    u16 unk46;
    u8 padding4[0x4];
    u16 unk4C;
    u8 unk4E;
} Struct_52880;

typedef struct {
    u8 padding[0x44];
    void *unk44;
} Alloc_52880;

INCLUDE_ASM("asm/nonmatchings/52880", func_80051C80_52880);

void func_800523EC_52FEC(Struct_52880 *arg0);
void func_80051E34_52A34(Struct_52880 *arg0);
void func_80051FC4_52BC4(void);

void func_80051DEC_529EC(Struct_52880 *arg0) {
    arg0->unk4C = arg0->unk42;
    arg0->unk20 = load_3ECE40();
    setCleanupCallback(func_800523EC_52FEC);
    setCallbackWithContinue(func_80051E34_52A34);
}

void func_80051E34_52A34(Struct_52880 *arg0) {
    Alloc_52880 *alloc = getCurrentAllocation();
    void *ptr;
    loadAssetMetadata((loadAssetMetadata_arg *)arg0, arg0->unk20, 2);
    ptr = alloc->unk44;
    arg0->unk4E = 0;
    arg0->unk46 = 0;
    arg0->unk0 = ptr;
    setCallbackWithContinue(func_80051FC4_52BC4);
}

INCLUDE_ASM("asm/nonmatchings/52880", func_80051E90_52A90);

INCLUDE_ASM("asm/nonmatchings/52880", func_80051FC4_52BC4);

INCLUDE_ASM("asm/nonmatchings/52880", func_80052128_52D28);

void func_800523EC_52FEC(Struct_52880 *arg0) {
    arg0->unk20 = freeNodeMemory(arg0->unk20);
}

INCLUDE_ASM("asm/nonmatchings/52880", func_80052418_53018);

void func_800524A4_530A4(Struct_52880 *arg0);
extern void func_800525F4_531F4(void);

void func_8005245C_5305C(Struct_52880 *arg0) {
    arg0->unk4C = arg0->unk42;
    arg0->unk20 = load_3ECE40();
    setCleanupCallback(func_800523EC_52FEC);
    setCallbackWithContinue(func_800524A4_530A4);
}

void func_800524A4_530A4(Struct_52880 *arg0) {
    Alloc_52880 *alloc = getCurrentAllocation();
    void *ptr;
    loadAssetMetadata((loadAssetMetadata_arg *)arg0, arg0->unk20, 3);
    ptr = alloc->unk44;
    arg0->unk4E = 0;
    arg0->unk46 = 0;
    arg0->unk0 = ptr;
    setCallbackWithContinue(func_800525F4_531F4);
}

INCLUDE_ASM("asm/nonmatchings/52880", func_80052500_53100);

INCLUDE_ASM("asm/nonmatchings/52880", func_800525F4_531F4);

INCLUDE_ASM("asm/nonmatchings/52880", func_80052758_53358);

INCLUDE_ASM("asm/nonmatchings/52880", func_80052A24_53624);

void func_80052AB0_536B0(Struct_52880 *arg0);

void func_80052A68_53668(Struct_52880 *arg0) {
    arg0->unk4C = arg0->unk42;
    arg0->unk20 = load_3ECE40();
    setCleanupCallback(func_800523EC_52FEC);
    setCallbackWithContinue(func_80052AB0_536B0);
}

extern void func_80052C00_53800(void);

void func_80052AB0_536B0(Struct_52880 *arg0) {
    Alloc_52880 *alloc = getCurrentAllocation();
    void *ptr;
    loadAssetMetadata((loadAssetMetadata_arg *)arg0, arg0->unk20, 4);
    ptr = alloc->unk44;
    arg0->unk4E = 0;
    arg0->unk46 = 0;
    arg0->unk0 = ptr;
    setCallbackWithContinue(func_80052C00_53800);
}

INCLUDE_ASM("asm/nonmatchings/52880", func_80052B0C_5370C);

INCLUDE_ASM("asm/nonmatchings/52880", func_80052C00_53800);

INCLUDE_ASM("asm/nonmatchings/52880", func_80052DB4_539B4);

INCLUDE_ASM("asm/nonmatchings/52880", func_80053078_53C78);

void func_80053784_54384(Struct_52880 *arg0);
void func_80053104_53D04(Struct_52880 *arg0);
extern void func_80053254_53E54(void);

void func_800530BC_53CBC(Struct_52880 *arg0) {
    arg0->unk4C = arg0->unk42;
    arg0->unk20 = load_3ECE40();
    setCleanupCallback(func_80053784_54384);
    setCallbackWithContinue(func_80053104_53D04);
}

void func_80053104_53D04(Struct_52880 *arg0) {
    Alloc_52880 *alloc = getCurrentAllocation();
    void *ptr;
    loadAssetMetadata((loadAssetMetadata_arg *)arg0, arg0->unk20, 5);
    ptr = alloc->unk44;
    arg0->unk4E = 0;
    arg0->unk46 = 0;
    arg0->unk0 = ptr;
    setCallbackWithContinue(func_80053254_53E54);
}

INCLUDE_ASM("asm/nonmatchings/52880", func_80053160_53D60);

INCLUDE_ASM("asm/nonmatchings/52880", func_80053254_53E54);

INCLUDE_ASM("asm/nonmatchings/52880", func_80053434_54034);

void func_80053784_54384(Struct_52880 *arg0) {
    arg0->unk20 = freeNodeMemory(arg0->unk20);
}

s32 func_800537B0_543B0(s32 arg0, s32 arg1) {
    Struct_52880 *task;

    task = scheduleTask(func_800530BC_53CBC, (arg0 + 4) & 0xFF, 0, 0x6F);
    if (task != NULL) {
        task->unk42 = arg0;
    }
}

void func_8005383C_5443C(Struct_52880 *arg0);

void func_800537F4_543F4(Struct_52880 *arg0) {
    arg0->unk4C = arg0->unk42;
    arg0->unk20 = load_3ECE40();
    setCleanupCallback(func_800523EC_52FEC);
    setCallbackWithContinue(func_8005383C_5443C);
}

extern void func_80053990_54590(void);

void func_8005383C_5443C(Struct_52880 *arg0) {
    Alloc_52880 *alloc = getCurrentAllocation();
    void *ptr;
    loadAssetMetadata((loadAssetMetadata_arg *)arg0, arg0->unk20, 6);
    ptr = alloc->unk44;
    arg0->unk4E = 0;
    arg0->unk46 = 0;
    arg0->unk0 = ptr;
    setCallbackWithContinue(func_80053990_54590);
}

INCLUDE_ASM("asm/nonmatchings/52880", func_80053898_54498);

INCLUDE_ASM("asm/nonmatchings/52880", func_80053990_54590);

INCLUDE_ASM("asm/nonmatchings/52880", func_80053B38_54738);

INCLUDE_ASM("asm/nonmatchings/52880", func_80053DF0_549F0);

void func_80053E90_54A90(Struct_52880 *arg0);

void func_80053E48_54A48(Struct_52880 *arg0) {
    arg0->unk4C = arg0->unk42;
    arg0->unk20 = load_3ECE40();
    setCleanupCallback(func_800523EC_52FEC);
    setCallbackWithContinue(func_80053E90_54A90);
}

extern void func_80053FE0_54BE0(void);

void func_80053E90_54A90(Struct_52880 *arg0) {
    Alloc_52880 *alloc = getCurrentAllocation();
    void *ptr;
    loadAssetMetadata((loadAssetMetadata_arg *)arg0, arg0->unk20, 7);
    ptr = alloc->unk44;
    arg0->unk4E = 0;
    arg0->unk46 = 0;
    arg0->unk0 = ptr;
    setCallbackWithContinue(func_80053FE0_54BE0);
}

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

void func_80054568_55168(Struct_52880 *arg0) {
    getCurrentAllocation();
    if (func_8005C250_5CE50(&arg0->unk4, arg0->unk42, 0xC0000)) {
        arg0->unk4E++;
    }
}

void func_800545F8_551F8(Struct_52880 *arg0);
extern void func_80054658_55258(void);

void func_800545B0_551B0(Struct_52880 *arg0) {
    arg0->unk4C = arg0->unk42;
    arg0->unk20 = load_3ECE40();
    setCleanupCallback(func_800523EC_52FEC);
    setCallbackWithContinue(func_800545F8_551F8);
}

void func_800545F8_551F8(Struct_52880 *arg0) {
    Alloc_52880 *alloc;
    void *ptr;
    s32 pad[4];

    alloc = getCurrentAllocation();
    loadAssetMetadata((loadAssetMetadata_arg *)arg0, arg0->unk20, 6);
    ptr = alloc->unk44;
    arg0->unk40 = 0x65;
    arg0->unk4E = 0;
    arg0->unk0 = ptr;
    setCallbackWithContinue(func_80054658_55258);
}

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

void func_80054C8C_5588C(s16 arg0) {
    Struct_52880 *task;
    s32 pad[4];

    task = scheduleTask(func_80054880_55480, 0, 0, 0x6F);
    if (task != NULL) {
        task->unk42 = arg0;
    }
}

typedef struct {
    u8 padding[0x20];
    void *unk20;
} func_80054CCC_558CC_arg;

void func_800553A8_55FA8(func_80054CCC_558CC_arg *arg0);
void func_80054D0C_5590C(Struct_52880 *arg0);
extern void func_80054F44_55B44(void);

void func_80054CCC_558CC(func_80054CCC_558CC_arg *arg0) {
    arg0->unk20 = load_3ECE40();
    setCleanupCallback(func_800553A8_55FA8);
    setCallbackWithContinue(func_80054D0C_5590C);
}

void func_80054D0C_5590C(Struct_52880 *arg0) {
    Alloc_52880 *alloc = getCurrentAllocation();
    void *ptr;
    loadAssetMetadata((loadAssetMetadata_arg *)arg0, arg0->unk20, 0x69);
    ptr = alloc->unk44;
    arg0->unk4E = 0;
    arg0->unk46 = 0;
    arg0->unk0 = ptr;
    arg0->unk4C = arg0->unk42;
    setCallbackWithContinue(func_80054F44_55B44);
}

INCLUDE_ASM("asm/nonmatchings/52880", func_80054D70_55970);

INCLUDE_ASM("asm/nonmatchings/52880", func_80054F44_55B44);

INCLUDE_ASM("asm/nonmatchings/52880", func_800550B4_55CB4);

void func_800553A8_55FA8(func_80054CCC_558CC_arg *arg0) {
    arg0->unk20 = freeNodeMemory(arg0->unk20);
}

INCLUDE_ASM("asm/nonmatchings/52880", func_800553D4_55FD4);

void func_80055460_56060(Struct_52880 *arg0);

void func_80055418_56018(Struct_52880 *arg0) {
    arg0->unk4C = arg0->unk42;
    arg0->unk20 = load_3ECE40();
    setCleanupCallback(func_800523EC_52FEC);
    setCallbackWithContinue(func_80055460_56060);
}

extern void func_800554FC_560FC(void);

void func_80055460_56060(Struct_52880 *arg0) {
    Alloc_52880 *alloc = getCurrentAllocation();
    void *ptr;
    loadAssetMetadata((loadAssetMetadata_arg *)arg0, arg0->unk20, 0x6E);
    ptr = alloc->unk44;
    arg0->unk4E = 0;
    arg0->unk46 = 0;
    arg0->unk0 = ptr;
    setCallbackWithContinue(func_800554FC_560FC);
}

void func_800554BC_560BC(Struct_52880 *arg0) {
    if (func_800BB504(&arg0->unk4, 0x80000)) {
        arg0->unk4E++;
    }
}

INCLUDE_ASM("asm/nonmatchings/52880", func_800554FC_560FC);

INCLUDE_ASM("asm/nonmatchings/52880", func_80055650_56250);

INCLUDE_ASM("asm/nonmatchings/52880", func_80055820_56420);

typedef struct {
    u8 padding[0x20];
    void *unk20;
} func_80055864_56464_arg;

void func_800558A4_564A4(Struct_52880 *arg0);
extern void func_80055964_56564(void);

void func_80055864_56464(func_80055864_56464_arg *arg0) {
    arg0->unk20 = load_3ECE40();
    setCleanupCallback(func_800523EC_52FEC);
    setCallbackWithContinue(func_800558A4_564A4);
}

void func_800558A4_564A4(Struct_52880 *arg0) {
    Alloc_52880 *alloc = getCurrentAllocation();
    void *ptr;
    loadAssetMetadata((loadAssetMetadata_arg *)arg0, arg0->unk20, 4);
    ptr = alloc->unk44;
    arg0->unk4E = 0;
    arg0->unk46 = 0;
    arg0->unk0 = ptr;
    setCallbackWithContinue(func_80055964_56564);
}

void func_80055900_56500(Struct_52880 *arg0) {
    Struct_B548_Result *result;

    getCurrentAllocation();
    result = func_8005B548_5C148(&arg0->unk4, arg0->unk42, 0x80000);
    if (result != NULL) {
        if ((result->unkB84 & 0x1000) == 0) {
            func_80058A68_59668((Player *)result);
            arg0->unk4E++;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/52880", func_80055964_56564);

INCLUDE_ASM("asm/nonmatchings/52880", func_80055A84_56684);

INCLUDE_ASM("asm/nonmatchings/52880", func_80055C80_56880);
