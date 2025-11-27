#include "56910.h"
#include "common.h"
#include "graphics.h"
#include "task_scheduler.h"

extern void *D_8009A6C0_9B2C0;
extern void func_800419AC_425AC(void *);
extern void func_80041810_42410(void *);

typedef struct {
    u8 pad0[0x20];
    void *unk20;
    void *unk24;
    void *unk28;
    s32 unk2C;
    u8 pad30[0xC];
    void *unk3C;
    s32 unk40;
} Func4179CArg;

INCLUDE_ASM("asm/nonmatchings/42170", func_80041570_42170);

INCLUDE_ASM("asm/nonmatchings/42170", func_800415E8_421E8);

INCLUDE_ASM("asm/nonmatchings/42170", func_8004168C_4228C);

INCLUDE_ASM("asm/nonmatchings/42170", func_80041724_42324);

INCLUDE_ASM("asm/nonmatchings/42170", func_8004175C_4235C);

void func_8004179C_4239C(Func4179CArg *arg0) {
    getCurrentAllocation();
    arg0->unk20 = &D_8009A6C0_9B2C0;
    arg0->unk24 = loadAsset_B7E70();
    arg0->unk28 = loadAsset_216290();
    arg0->unk2C = 0;
    arg0->unk40 = 0x400;
    func_80056B7C_5777C((u8 *)arg0->unk3C + 0x434, 0x13);
    setCleanupCallback(func_800419AC_425AC);
    setCallbackWithContinue(func_80041810_42410);
}

INCLUDE_ASM("asm/nonmatchings/42170", func_80041810_42410);

INCLUDE_ASM("asm/nonmatchings/42170", func_800419AC_425AC);

INCLUDE_ASM("asm/nonmatchings/42170", func_800419E4_425E4);

INCLUDE_ASM("asm/nonmatchings/42170", func_80041A24_42624);

INCLUDE_ASM("asm/nonmatchings/42170", func_80041A60_42660);

INCLUDE_ASM("asm/nonmatchings/42170", func_80041A9C_4269C);

INCLUDE_ASM("asm/nonmatchings/42170", func_80041AD8_426D8);

INCLUDE_ASM("asm/nonmatchings/42170", func_80041B18_42718);

INCLUDE_ASM("asm/nonmatchings/42170", func_80041C28_42828);

INCLUDE_ASM("asm/nonmatchings/42170", func_80041DE4_429E4);

INCLUDE_ASM("asm/nonmatchings/42170", func_80041E10_42A10);

INCLUDE_ASM("asm/nonmatchings/42170", func_80041EA4_42AA4);

INCLUDE_ASM("asm/nonmatchings/42170", func_80041F38_42B38);

INCLUDE_ASM("asm/nonmatchings/42170", func_80041FB4_42BB4);

INCLUDE_ASM("asm/nonmatchings/42170", func_80042070_42C70);

INCLUDE_ASM("asm/nonmatchings/42170", func_800420A8_42CA8);

INCLUDE_ASM("asm/nonmatchings/42170", func_800420E8_42CE8);

INCLUDE_ASM("asm/nonmatchings/42170", func_80042160_42D60);

INCLUDE_ASM("asm/nonmatchings/42170", func_80042254_42E54);

INCLUDE_ASM("asm/nonmatchings/42170", func_80042308_42F08);

INCLUDE_ASM("asm/nonmatchings/42170", func_80042340_42F40);

INCLUDE_ASM("asm/nonmatchings/42170", func_800423A4_42FA4);

INCLUDE_ASM("asm/nonmatchings/42170", func_80042410_43010);

INCLUDE_ASM("asm/nonmatchings/42170", func_8004247C_4307C);

INCLUDE_ASM("asm/nonmatchings/42170", func_8004256C_4316C);

INCLUDE_ASM("asm/nonmatchings/42170", func_80042638_43238);

INCLUDE_ASM("asm/nonmatchings/42170", func_80042670_43270);

INCLUDE_ASM("asm/nonmatchings/42170", func_800426B0_432B0);

INCLUDE_ASM("asm/nonmatchings/42170", func_8004273C_4333C);

INCLUDE_ASM("asm/nonmatchings/42170", func_80042820_43420);

INCLUDE_ASM("asm/nonmatchings/42170", func_8004290C_4350C);

INCLUDE_ASM("asm/nonmatchings/42170", func_800429C4_435C4);

INCLUDE_ASM("asm/nonmatchings/42170", func_800429FC_435FC);

INCLUDE_ASM("asm/nonmatchings/42170", func_80042A6C_4366C);

INCLUDE_ASM("asm/nonmatchings/42170", func_80042B64_43764);

INCLUDE_ASM("asm/nonmatchings/42170", func_80042BA4_437A4);

INCLUDE_ASM("asm/nonmatchings/42170", func_80042C98_43898);

INCLUDE_ASM("asm/nonmatchings/42170", func_80042D54_43954);

INCLUDE_ASM("asm/nonmatchings/42170", func_80042E40_43A40);

INCLUDE_ASM("asm/nonmatchings/42170", func_80042F2C_43B2C);

INCLUDE_ASM("asm/nonmatchings/42170", func_80042F58_43B58);

INCLUDE_ASM("asm/nonmatchings/42170", func_80042FC0_43BC0);

INCLUDE_ASM("asm/nonmatchings/42170", func_80043018_43C18);

INCLUDE_ASM("asm/nonmatchings/42170", func_8004309C_43C9C);

INCLUDE_ASM("asm/nonmatchings/42170", func_8004320C_43E0C);

INCLUDE_ASM("asm/nonmatchings/42170", func_800432D8_43ED8);

INCLUDE_ASM("asm/nonmatchings/42170", func_80043310_43F10);

INCLUDE_ASM("asm/nonmatchings/42170", func_80043374_43F74);

INCLUDE_ASM("asm/nonmatchings/42170", func_800433EC_43FEC);

INCLUDE_ASM("asm/nonmatchings/42170", func_800435F0_441F0);

INCLUDE_ASM("asm/nonmatchings/42170", func_8004367C_4427C);

INCLUDE_ASM("asm/nonmatchings/42170", func_800436B4_442B4);

INCLUDE_ASM("asm/nonmatchings/42170", func_80043718_44318);

INCLUDE_ASM("asm/nonmatchings/42170", func_800437C4_443C4);

INCLUDE_ASM("asm/nonmatchings/42170", func_80043860_44460);

INCLUDE_ASM("asm/nonmatchings/42170", func_800438A0_444A0);

INCLUDE_ASM("asm/nonmatchings/42170", func_8004393C_4453C);

INCLUDE_ASM("asm/nonmatchings/42170", func_800439F4_445F4);

INCLUDE_ASM("asm/nonmatchings/42170", func_80043AB4_446B4);

INCLUDE_ASM("asm/nonmatchings/42170", func_80043C00_44800);

INCLUDE_ASM("asm/nonmatchings/42170", func_80043CA4_448A4);

INCLUDE_ASM("asm/nonmatchings/42170", func_80043D30_44930);

INCLUDE_ASM("asm/nonmatchings/42170", func_80043D5C_4495C);

INCLUDE_ASM("asm/nonmatchings/42170", func_80043DC0_449C0);

INCLUDE_ASM("asm/nonmatchings/42170", func_80043E24_44A24);

INCLUDE_ASM("asm/nonmatchings/42170", func_80043F8C_44B8C);

INCLUDE_ASM("asm/nonmatchings/42170", func_80044018_44C18);

INCLUDE_ASM("asm/nonmatchings/42170", func_80044050_44C50);

INCLUDE_ASM("asm/nonmatchings/42170", func_800440B4_44CB4);

INCLUDE_ASM("asm/nonmatchings/42170", func_800441A4_44DA4);

INCLUDE_ASM("asm/nonmatchings/42170", func_80044500_45100);

INCLUDE_ASM("asm/nonmatchings/42170", func_80044538_45138);

INCLUDE_ASM("asm/nonmatchings/42170", func_80044578_45178);

INCLUDE_ASM("asm/nonmatchings/42170", func_80044684_45284);

INCLUDE_ASM("asm/nonmatchings/42170", func_800447D4_453D4);

INCLUDE_ASM("asm/nonmatchings/42170", func_80044888_45488);

INCLUDE_ASM("asm/nonmatchings/42170", func_80044990_45590);

INCLUDE_ASM("asm/nonmatchings/42170", func_80044AB8_456B8);

INCLUDE_ASM("asm/nonmatchings/42170", func_80044BBC_457BC);

INCLUDE_ASM("asm/nonmatchings/42170", func_80044C38_45838);

INCLUDE_ASM("asm/nonmatchings/42170", func_80044CA4_458A4);

INCLUDE_ASM("asm/nonmatchings/42170", func_80044CDC_458DC);

INCLUDE_ASM("asm/nonmatchings/42170", func_80044D1C_4591C);

INCLUDE_ASM("asm/nonmatchings/42170", func_80044DB0_459B0);

INCLUDE_ASM("asm/nonmatchings/42170", func_80044EC4_45AC4);

INCLUDE_ASM("asm/nonmatchings/42170", func_80045010_45C10);

INCLUDE_ASM("asm/nonmatchings/42170", func_80045054_45C54);

INCLUDE_ASM("asm/nonmatchings/42170", func_80045434_46034);
