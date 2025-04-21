#include "common.h"

void func_80069CC0_6A8C0(void *);
void func_8006A0EC_6ACEC(s32, s32, s32, s32);
extern s32 D_800937E8_943E8[];
extern s32 D_800937EC_943EC[];
extern s32 D_800937F0_943F0[];
extern s32 D_80093974_94574[];
extern s32 D_80093978_94578[];
extern s32 D_8009397C_9457C[];
extern s32 func_800571D0_57DD0;
typedef struct
{
    s32 padding;
    s32 unk4;
    s32 unk8;
    u8 padding2[0x4];
    s16 unk10;
    u16 unk12;
} D_800A2990_A3590_type;
extern D_800A2990_A3590_type *D_800A2990_A3590;

INCLUDE_ASM("asm/nonmatchings/56C80", func_80056080_56C80);

INCLUDE_ASM("asm/nonmatchings/56C80", func_8005610C_56D0C);

INCLUDE_ASM("asm/nonmatchings/56C80", func_8005628C_56E8C);

INCLUDE_ASM("asm/nonmatchings/56C80", func_80056914_57514);

INCLUDE_ASM("asm/nonmatchings/56C80", func_80056990_57590);

INCLUDE_ASM("asm/nonmatchings/56C80", func_800569A4_575A4);

INCLUDE_ASM("asm/nonmatchings/56C80", func_80056A88_57688);

INCLUDE_ASM("asm/nonmatchings/56C80", func_80056B7C_5777C);

INCLUDE_ASM("asm/nonmatchings/56C80", func_80056C6C_5786C);

INCLUDE_ASM("asm/nonmatchings/56C80", func_80056D64_57964);

INCLUDE_ASM("asm/nonmatchings/56C80", func_80056E64_57A64);

INCLUDE_ASM("asm/nonmatchings/56C80", func_80056F8C_57B8C);

INCLUDE_ASM("asm/nonmatchings/56C80", func_800570BC_57CBC);

INCLUDE_ASM("asm/nonmatchings/56C80", func_800570E0_57CE0);

void func_80057124_57D24(void) {
    s32 temp_v0;
    s32 temp_v0_2;
    u16 temp_v1;

    temp_v1 = D_800A2990_A3590->unk10;

    D_800A2990_A3590->unk12 = temp_v1;
    temp_v0 = ((temp_v1 << 1) + temp_v1);
    func_8006A0EC_6ACEC(
        D_800937E8_943E8[temp_v0],
        D_800937EC_943EC[temp_v0],
        D_800937F0_943F0[temp_v0],
        D_800A2990_A3590->unk8);

    temp_v0_2 = ((D_800A2990_A3590->unk12 << 1) + D_800A2990_A3590->unk12);
    func_8006A0EC_6ACEC(
        D_80093974_94574[temp_v0_2],
        D_80093978_94578[temp_v0_2],
        D_8009397C_9457C[temp_v0_2],
        D_800A2990_A3590->unk4);

    func_80069CC0_6A8C0(&func_800571D0_57DD0);
}

INCLUDE_ASM("asm/nonmatchings/56C80", func_800571D0_57DD0);

INCLUDE_ASM("asm/nonmatchings/56C80", func_80057214_57E14);

INCLUDE_ASM("asm/nonmatchings/56C80", func_800572B0_57EB0);

INCLUDE_ASM("asm/nonmatchings/56C80", func_800573F8_57FF8);

INCLUDE_ASM("asm/nonmatchings/56C80", func_80057470_58070);

INCLUDE_ASM("asm/nonmatchings/56C80", func_800574A0_580A0);

INCLUDE_ASM("asm/nonmatchings/56C80", func_800574E0_580E0);

INCLUDE_ASM("asm/nonmatchings/56C80", func_80057514_58114);

INCLUDE_ASM("asm/nonmatchings/56C80", func_80057550_58150);

INCLUDE_ASM("asm/nonmatchings/56C80", func_80057564_58164);

INCLUDE_ASM("asm/nonmatchings/56C80", func_8005758C_5818C);

INCLUDE_ASM("asm/nonmatchings/56C80", func_80057614_58214);

INCLUDE_ASM("asm/nonmatchings/56C80", func_80057870_58470);

INCLUDE_ASM("asm/nonmatchings/56C80", func_800578DC_584DC);

INCLUDE_ASM("asm/nonmatchings/56C80", func_80057928_58528);

INCLUDE_ASM("asm/nonmatchings/56C80", func_80057974_58574);

INCLUDE_ASM("asm/nonmatchings/56C80", func_800579CC_585CC);

INCLUDE_ASM("asm/nonmatchings/56C80", func_800579E8_585E8);

INCLUDE_ASM("asm/nonmatchings/56C80", func_80057A34_58634);

INCLUDE_ASM("asm/nonmatchings/56C80", func_80057A7C_5867C);

INCLUDE_ASM("asm/nonmatchings/56C80", func_80057ABC_586BC);

INCLUDE_ASM("asm/nonmatchings/56C80", func_80057B1C_5871C);

INCLUDE_ASM("asm/nonmatchings/56C80", func_80057B70_58770);

INCLUDE_ASM("asm/nonmatchings/56C80", func_80057CB4_588B4);

INCLUDE_ASM("asm/nonmatchings/56C80", func_80057CE4_588E4);

INCLUDE_ASM("asm/nonmatchings/56C80", func_80057DF0_589F0);

INCLUDE_ASM("asm/nonmatchings/56C80", func_80057E18_58A18);

INCLUDE_ASM("asm/nonmatchings/56C80", func_80057F28_58B28);

INCLUDE_ASM("asm/nonmatchings/56C80", func_80057F48_58B48);

INCLUDE_ASM("asm/nonmatchings/56C80", func_80058048_58C48);

INCLUDE_ASM("asm/nonmatchings/56C80", func_80058064_58C64);

INCLUDE_ASM("asm/nonmatchings/56C80", func_80058138_58D38);

INCLUDE_ASM("asm/nonmatchings/56C80", func_80058154_58D54);

INCLUDE_ASM("asm/nonmatchings/56C80", func_80058220_58E20);

INCLUDE_ASM("asm/nonmatchings/56C80", func_8005823C_58E3C);

INCLUDE_ASM("asm/nonmatchings/56C80", func_800582C0_58EC0);

INCLUDE_ASM("asm/nonmatchings/56C80", func_800582DC_58EDC);

INCLUDE_ASM("asm/nonmatchings/56C80", func_80058360_58F60);

INCLUDE_ASM("asm/nonmatchings/56C80", func_80058380_58F80);

INCLUDE_ASM("asm/nonmatchings/56C80", func_800583F8_58FF8);

INCLUDE_ASM("asm/nonmatchings/56C80", func_80058414_59014);

INCLUDE_ASM("asm/nonmatchings/56C80", func_80058490_59090);

INCLUDE_ASM("asm/nonmatchings/56C80", func_800584AC_590AC);

INCLUDE_ASM("asm/nonmatchings/56C80", func_80058530_59130);

INCLUDE_ASM("asm/nonmatchings/56C80", func_8005854C_5914C);

INCLUDE_ASM("asm/nonmatchings/56C80", func_800585C8_591C8);

INCLUDE_ASM("asm/nonmatchings/56C80", func_800585E4_591E4);

INCLUDE_ASM("asm/nonmatchings/56C80", func_80058638_59238);
