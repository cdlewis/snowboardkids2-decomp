#include "common.h"

INCLUDE_ASM("asm/nonmatchings/5EA60", func_8005DE60_5EA60);

INCLUDE_ASM("asm/nonmatchings/5EA60", func_8005DE6C_5EA6C);

INCLUDE_ASM("asm/nonmatchings/5EA60", func_8005DE98_5EA98);

INCLUDE_ASM("asm/nonmatchings/5EA60", func_8005DF10_5EB10);

INCLUDE_ASM("asm/nonmatchings/5EA60", func_8005E22C_5EE2C);

INCLUDE_ASM("asm/nonmatchings/5EA60", func_8005E500_5F100);

INCLUDE_ASM("asm/nonmatchings/5EA60", func_8005E800_5F400);

INCLUDE_ASM("asm/nonmatchings/5EA60", func_8005EA44_5F644);

INCLUDE_ASM("asm/nonmatchings/5EA60", func_8005ECB8_5F8B8);

INCLUDE_ASM("asm/nonmatchings/5EA60", func_8005EFC4_5FBC4);

INCLUDE_ASM("asm/nonmatchings/5EA60", func_8005F2FC_5FEFC);

INCLUDE_ASM("asm/nonmatchings/5EA60", func_8005F344_5FF44);

INCLUDE_ASM("asm/nonmatchings/5EA60", func_8005F6DC_602DC);

INCLUDE_ASM("asm/nonmatchings/5EA60", func_8005FAA0_606A0);

INCLUDE_ASM("asm/nonmatchings/5EA60", func_8005FDAC_609AC);

INCLUDE_ASM("asm/nonmatchings/5EA60", func_800600E4_60CE4);

INCLUDE_ASM("asm/nonmatchings/5EA60", func_80060504_61104);

INCLUDE_ASM("asm/nonmatchings/5EA60", func_80060950_61550);

typedef struct {
    u8 padding[0x8];
    u32 unk8;
} func_8006097C_6157C_arg_item;

typedef struct {
    func_8006097C_6157C_arg_item data[0];
} func_8006097C_6157C_arg;

u16 func_8006097C_6157C(func_8006097C_6157C_arg *table, s32 index) {
    s32 adjusted_index = (index << 16) >> 14;
    func_8006097C_6157C_arg_item *entry = (func_8006097C_6157C_arg_item *)((s8 *)table + adjusted_index);
    u16 *data_ptr = (u16 *)((s8 *)table + entry->unk8);

    return *data_ptr;
}