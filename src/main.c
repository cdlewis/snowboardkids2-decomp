#include "main.h"

#include "6E840.h"
#include "common.h"
#include "task_scheduler.h"

void *func_80009F5C_AB5C(s32);

typedef struct {
    char padding[44];
    s32 *unk2C;
    s32 *unk30;
} func_80000BF4_17F4_arg;

typedef struct {
    char padding[0x2C];
    s32 *unk2C;
    s32 *unk30;
    char padding2[0x10];
    s32 *unk44;
} func_80001114_1D14_arg;

typedef struct {
    u8 unk0;
    char padding[0x33];
    u8 unk34;
    u8 unk35;
    u8 unk36;
    u8 unk37;
    u8 unk38;
    u8 unk39;
    u8 unk3A;
} D_800891A8_89DA8_entry;
extern D_800891A8_89DA8_entry *D_800891A8_89DA8[];

typedef struct {
    char padding[0x87];
    char unk87;
} func_80000450_1050_arg;

void func_80000450_1050(func_80000450_1050_arg *arg0, s8 arg1) {
    arg0->unk87 = arg1;
}

s32 osVoiceCheckWord(u8 *data) {
    return 0xE;
}

void func_80000460_1060(func_80000460_1060_arg0 *arg0, s8 *arg1, s8 *arg2) {
    s32 temp_v0;
    s32 temp_v0_2;
    int new_var3;
    s32 temp_t1;
    s32 var_a1;
    u8 temp_a0;
    s32 new_var;
    D_800891A8_89DA8_entry *temp_a2;
    s16 index;
    temp_a2 = (D_800891A8_89DA8_entry *)((s32)D_800891A8_89DA8 + arg0->unk84 * 0x3C);
    new_var3 = temp_a2->unk34 + temp_a2->unk35;
    temp_v0 = new_var3;
    temp_a0 = temp_a2->unk36;
    new_var = (temp_v0 + temp_a0) / 3;
    var_a1 = new_var;
    temp_v0 = ((u32)new_var) >> 0x1F;
    temp_v0 = var_a1 + ((s8)temp_v0);
    new_var = temp_v0 >> 0x1;
    temp_t1 = new_var;
    if (var_a1 < 0) {
        var_a1 += 3;
    }
    arg1[0x4] = 0;
    arg1[0x5] = 0x7F;
    arg1[0x6] = 0x7F;
    arg1[0x0] = temp_a2->unk34;
    arg1[0x1] = temp_a2->unk35;
    temp_v0_2 = var_a1 >> 2;
    arg1[0x2] = temp_a2->unk36;
    arg1[0x8] = temp_v0_2;
    arg1[0x9] = temp_v0_2;
    arg1[0xA] = temp_v0_2;
    arg1[0xC] = 0x7F;
    arg1[0xD] = 0x7F;
    arg1[0xE] = 0;
    arg1[0x14] = -0x7F;
    arg1[0x15] = 0x7F;
    arg1[0x16] = 0;
    arg1[0x10] = temp_t1;
    arg1[0x11] = temp_t1;
    arg1[0x12] = temp_t1;
    arg2[0x0] = temp_a2->unk38;
    arg2[0x1] = temp_a2->unk39;
    arg2[0x2] = temp_a2->unk3A;
    func_8006FC70_70870(arg0->unk0->unkDA, 3, arg1, arg2);
}

INCLUDE_ASM("asm/nonmatchings/main", func_8000056C_116C);

INCLUDE_ASM("asm/nonmatchings/main", func_80000710_1310);

INCLUDE_ASM("asm/nonmatchings/main", func_80000760_1360);

INCLUDE_ASM("asm/nonmatchings/main", func_800007C4_13C4);

INCLUDE_ASM("asm/nonmatchings/main", func_800007F0_13F0);

INCLUDE_ASM("asm/nonmatchings/main", func_800008D0_14D0);

INCLUDE_ASM("asm/nonmatchings/main", func_80000968_1568);

INCLUDE_ASM("asm/nonmatchings/main", func_800009A0_15A0);

// connected to __rmonPanic via L80000AE0_16E0
INCLUDE_ASM("asm/nonmatchings/main", func_80000A68_1668);

INCLUDE_ASM("asm/nonmatchings/main", __rmonPanic);

INCLUDE_ASM("asm/nonmatchings/main", rmonmisc_text_010C);

void func_80000BF4_17F4(func_80000BF4_17F4_arg *arg0) {
    arg0->unk30 = (s32 *)freeGameStateMemory(arg0->unk30);
    arg0->unk2C = (s32 *)freeGameStateMemory(arg0->unk2C);
}

INCLUDE_ASM("asm/nonmatchings/main", func_80000C2C_182C);

INCLUDE_ASM("asm/nonmatchings/main", func_80000CAC_18AC);

void func_80000DA4_19A4(s32 arg0) {
    func_80009F5C_AB5C(arg0 + 8);
}

INCLUDE_ASM("asm/nonmatchings/main", func_80000DC0_19C0);

INCLUDE_ASM("asm/nonmatchings/main", func_80000E84_1A84);

void func_80000F14_1B14(func_80000BF4_17F4_arg *arg0) {
    arg0->unk30 = (s32 *)freeGameStateMemory(arg0->unk30);
    arg0->unk2C = (s32 *)freeGameStateMemory(arg0->unk2C);
}

INCLUDE_ASM("asm/nonmatchings/main", func_80000F4C_1B4C);

INCLUDE_ASM("asm/nonmatchings/main", func_80001040_1C40);

void func_80001114_1D14(func_80001114_1D14_arg *arg0) {
    arg0->unk44 = (s32 *)freeGameStateMemory(arg0->unk44);
    arg0->unk30 = (s32 *)freeGameStateMemory(arg0->unk30);
    arg0->unk2C = (s32 *)freeGameStateMemory(arg0->unk2C);
}

INCLUDE_ASM("asm/nonmatchings/main", func_80001158_1D58);

INCLUDE_ASM("asm/nonmatchings/main", func_800011DC_1DDC);

void func_80001264_1E64(s32 arg0) {
    func_80009F5C_AB5C(arg0 + 8);
}

INCLUDE_ASM("asm/nonmatchings/main", func_80001280_1E80);

INCLUDE_ASM("asm/nonmatchings/main", func_800013B8_1FB8);

void func_800014C8_20C8(s32 arg0) {
    func_80009F5C_AB5C(arg0 + 8);
}
