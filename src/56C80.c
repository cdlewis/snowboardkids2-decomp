#include "common.h"

void func_80069CC0_6A8C0(void *);
void func_8006A0EC_6ACEC(s32, s32, s32, s32);
void func_800584AC_590AC(s32, s32, s32);
void func_80058414_59014(s32, s32, s32);
extern s32 D_800937E8_943E8[];
extern s32 D_800937EC_943EC[];
extern s32 D_800937F0_943F0[];
extern s32 D_80093974_94574[];
extern s32 D_80093978_94578[];
extern s32 D_8009397C_9457C[];
extern void func_800571D0_57DD0();
typedef struct {
    s32 padding;
    s32 unk4;
    s32 unk8;
    u8 padding2[0x4];
    s16 unk10;
    u16 unk12;
    u8 padding3[0x2];
    s16 unk16;
    s16 unk18;
    s16 unk1A;
    s8 unk1C;
    u8 unk1D;
    s32 unk20;
    u8 padding7[0x60];
    s32 unk84;
    u8 padding4[0x380];
    s32 unk408;
    u8 padding6[0x100];
    s8 unk50C[0x8];
    s32 unk514[8];
    s32 unk534;
    s32 unk538;
} D_800A2990_A3590_type;
extern D_800A2990_A3590_type *D_800A2990_A3590;

void func_800570BC_57CBC();
extern OSMesgQueue D_800A2CD0_A38D0;
extern OSMesgQueue D_800A2CF0_A38F0;
extern s32 D_800A2D1C_A391C;
extern s32 D_800A2D20_A3920;
extern s32 D_800A2D24_A3924;
extern s32 D_800A2D28_A3928;
extern s32 D_800A2D38_A3938;
extern u8 D_80093B84_94784[];
extern s32 D_800A2D2C_A392C;
void func_80057F48_58B48(s32 arg0, s32 arg1, s32 arg2, s32 arg3); /* extern */
extern void func_80057214_57E14();
void func_800579E8_585E8(s32, s32);
extern s32 D_80093B00_94700;
void func_80057CE4_588E4(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
void func_8005854C_5914C(s32, s32);
void func_80058064_58C64(s32, s32, s32, s32);
extern OSMesgQueue D_800A2CD0_A38D0;
extern OSMesgQueue D_800A2CF0_A38F0;
extern s32 D_800A2D30_A3930;
void func_80057E18_58A18(s32, s32, s32, s32, s32);
void func_80057124_57D24(void);
void *func_80058638_59238();
void func_80069CD0_6A8D0(void *);
extern u8 D_80093BA5_947A5;
extern u8 D_80093BA6_947A6;

INCLUDE_ASM("asm/nonmatchings/56C80", func_80056080_56C80);

INCLUDE_ASM("asm/nonmatchings/56C80", func_8005610C_56D0C);

INCLUDE_ASM("asm/nonmatchings/56C80", func_8005628C_56E8C);

INCLUDE_ASM("asm/nonmatchings/56C80", func_80056914_57514);

void func_80056990_57590(s32 arg0, s32 arg1) {
    D_800A2990_A3590->unk538 = arg1;
    D_800A2990_A3590->unk534 = arg0;
}

INCLUDE_ASM("asm/nonmatchings/56C80", func_800569A4_575A4);

void func_80056A88_57688(void *arg0, u8 arg1, s32 arg2) {
    s32 i;
    s8 id;
    void *dst;
    volatile u8 padding[0x2];

    i = 0;
    id = arg2;
    if (D_800A2990_A3590->unk408 > 0) {
        do {
            if (D_800A2990_A3590->unk50C[i] == id) {
                dst = (void *)((i << 5) + (s32)D_800A2990_A3590 + 0x40C);
                memcpy(dst, arg0, 0x20);
                return;
            }
            i++;
        } while (i < D_800A2990_A3590->unk408);
    }
    if (D_800A2990_A3590->unk408 < 8) {
        D_800A2990_A3590->unk50C[D_800A2990_A3590->unk408] = id;
        dst = (void *)(((D_800A2990_A3590->unk408) << 5) + (s32)D_800A2990_A3590 + 0x40C);
        memcpy(dst, arg0, 0x20);
        D_800A2990_A3590->unk514[D_800A2990_A3590->unk408] = arg1;
        D_800A2990_A3590->unk408++;
    }
}

INCLUDE_ASM("asm/nonmatchings/56C80", func_80056B7C_5777C);

INCLUDE_ASM("asm/nonmatchings/56C80", func_80056C6C_5786C);

INCLUDE_ASM("asm/nonmatchings/56C80", func_80056D64_57964);

INCLUDE_ASM("asm/nonmatchings/56C80", func_80056E64_57A64);

INCLUDE_ASM("asm/nonmatchings/56C80", func_80056F8C_57B8C);

void func_800570BC_57CBC() {
    D_800A2990_A3590->unk20 = (s32)((D_800A2990_A3590->unk20 + 1) & 0xFFFFFF);
}

void func_800570E0_57CE0(void) {
    func_80069CE8_6A8E8(func_800570E0_57CE0);
    if (D_800A2990_A3590->unk1D != 0) {
        func_80069CC0_6A8C0(&func_80057124_57D24);
    }
}

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

void func_800571D0_57DD0() {
    func_800579E8_585E8(D_800A2990_A3590->unk4, *(&D_80093B00_94700 + (D_800A2990_A3590->unk12)));
    func_80069CC0_6A8C0(&func_80057214_57E14);
}

INCLUDE_ASM("asm/nonmatchings/56C80", func_80057214_57E14);

INCLUDE_ASM("asm/nonmatchings/56C80", func_800572B0_57EB0);

INCLUDE_ASM("asm/nonmatchings/56C80", func_800573F8_57FF8);

extern void func_800570E0_57CE0();

void func_80057470_58070(void) {
    if (func_80058638_59238() == NULL) {
        func_80069CD0_6A8D0(&func_800570E0_57CE0);
    }
}

INCLUDE_ASM("asm/nonmatchings/56C80", func_800574A0_580A0);

void func_800574E0_580E0(s16 arg0, s8 arg1) {
    D_800A2990_A3590->unk1D = 2;
    D_800A2990_A3590->unk10 = arg0;
    D_800A2990_A3590->unk16 = 0x80;
    D_800A2990_A3590->unk18 = 0x80;
    D_800A2990_A3590->unk1A = 0;
    D_800A2990_A3590->unk1C = arg1;
}

void func_80057514_58114(u32 arg0, s16 arg1, s16 arg2) {
    D_800A2990_A3590_type *new_var;
    D_800A2990_A3590->unk1D = 2;
    D_800A2990_A3590->unk10 = arg0;
    D_800A2990_A3590->unk16 = 0;
    D_800A2990_A3590->unk18 = arg1;
    D_800A2990_A3590->unk1A = arg2;
    D_800A2990_A3590->unk1C = D_80093B84_94784[arg0];
}

void func_80057550_58150(s16 arg0, s16 arg1) {
    D_800A2990_A3590->unk18 = arg0;
    D_800A2990_A3590->unk1A = arg1;
}

INCLUDE_ASM("asm/nonmatchings/56C80", func_80057564_58164);

INCLUDE_ASM("asm/nonmatchings/56C80", func_8005758C_5818C);

INCLUDE_ASM("asm/nonmatchings/56C80", func_80057614_58214);

void func_80057870_58470(s32 arg0) {
    void *message;

    if ((D_80093BA5_947A5 != NULL) && (D_80093BA6_947A6 == NULL)) {
        D_80093BA6_947A6++;
        D_800A2D30_A3930 = arg0;
        osSendMesg(&D_800A2CD0_A38D0, (OSMesg *)0xD, OS_MESG_BLOCK);
        osRecvMesg(&D_800A2CF0_A38F0, &message, OS_MESG_BLOCK);
    }
}

void func_800578DC_584DC(s32 arg0, s32 arg1) {
    void *message;

    D_800A2D2C_A392C = arg0;
    D_800A2D30_A3930 = arg1;
    osSendMesg(&D_800A2CD0_A38D0, (OSMesg *)7, OS_MESG_BLOCK);
    osRecvMesg(&D_800A2CF0_A38F0, &message, OS_MESG_BLOCK);
}

void func_80057928_58528(s32 arg0, s32 arg1) {
    void *message;

    D_800A2D2C_A392C = arg0;
    D_800A2D24_A3924 = arg1;
    osSendMesg(&D_800A2CD0_A38D0, (OSMesg *)0xC, OS_MESG_BLOCK);
    osRecvMesg(&D_800A2CF0_A38F0, &message, OS_MESG_BLOCK);
}

INCLUDE_ASM("asm/nonmatchings/56C80", func_80057974_58574);

INCLUDE_ASM("asm/nonmatchings/56C80", func_800579CC_585CC);

INCLUDE_ASM("asm/nonmatchings/56C80", func_800579E8_585E8);

void *func_80057A34_58634(s32 arg0) {
    void *message;

    D_800A2D2C_A392C = arg0;
    osSendMesg(&D_800A2CD0_A38D0, (OSMesg *)9, OS_MESG_BLOCK);
    osRecvMesg(&D_800A2CF0_A38F0, &message, OS_MESG_BLOCK);
    return message;
}

void *func_80057A7C_5867C(void) {
    void *message;

    osSendMesg(&D_800A2CD0_A38D0, (OSMesg *)8, OS_MESG_BLOCK);
    osRecvMesg(&D_800A2CF0_A38F0, &message, OS_MESG_BLOCK);
    return message;
}

INCLUDE_ASM("asm/nonmatchings/56C80", func_80057ABC_586BC);

void func_80057B1C_5871C(s32 arg0) {
    void *message;

    D_800A2D30_A3930 = arg0;
    osSendMesg(&D_800A2CD0_A38D0, (OSMesg *)6, OS_MESG_BLOCK);
    osRecvMesg(&D_800A2CF0_A38F0, &message, OS_MESG_BLOCK);
    D_800A2990_A3590->unk84 = 0;
    D_800A2990_A3590->unk408 = 0;
}

INCLUDE_ASM("asm/nonmatchings/56C80", func_80057B70_58770);

INCLUDE_ASM("asm/nonmatchings/56C80", func_80057CB4_588B4);

INCLUDE_ASM("asm/nonmatchings/56C80", func_80057CE4_588E4);

void func_80057DF0_589F0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    func_80057CE4_588E4(arg0, arg1, arg2, arg3, arg4, 0xC);
}

INCLUDE_ASM("asm/nonmatchings/56C80", func_80057E18_58A18);

void func_80057F28_58B28(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    func_80057E18_58A18(arg0, arg1, arg2, arg3, 0xC);
}

INCLUDE_ASM("asm/nonmatchings/56C80", func_80057F48_58B48);

void func_80058048_58C48(s32 arg0, s32 arg1, s32 arg2) {
    func_80057F48_58B48(arg0, arg1, arg2, 0xC);
}

INCLUDE_ASM("asm/nonmatchings/56C80", func_80058064_58C64);

void func_80058138_58D38(s32 arg0, s32 arg1, s32 arg2) {
    func_80058064_58C64(arg0, arg1, arg2, 0xC);
}

INCLUDE_ASM("asm/nonmatchings/56C80", func_80058154_58D54);

INCLUDE_ASM("asm/nonmatchings/56C80", func_80058220_58E20);

INCLUDE_ASM("asm/nonmatchings/56C80", func_8005823C_58E3C);

INCLUDE_ASM("asm/nonmatchings/56C80", func_800582C0_58EC0);

INCLUDE_ASM("asm/nonmatchings/56C80", func_800582DC_58EDC);

INCLUDE_ASM("asm/nonmatchings/56C80", func_80058360_58F60);

void func_80058380_58F80(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 *new_var;
    void *message;
    s32 new_var2;
    D_800A2D1C_A391C = arg0;
    D_800A2D24_A3924 = arg1;
    D_800A2D28_A3928 = arg2;
    new_var = &D_800A2990_A3590->unk20;
    new_var2 = *new_var;
    D_800A2D38_A3938 = arg3;
    D_800A2D20_A3920 = new_var2;
    osSendMesg(&D_800A2CD0_A38D0, (OSMesg *)1, OS_MESG_BLOCK);
    osRecvMesg(&D_800A2CF0_A38F0, &message, OS_MESG_BLOCK);
    func_800570BC_57CBC();
}

INCLUDE_ASM("asm/nonmatchings/56C80", func_800583F8_58FF8);

void func_80058414_59014(s32 arg0, s32 arg1, s32 arg2) {
    void *sp10;
    s32 *new_var;
    s32 new_var2;
    D_800A2D1C_A391C = arg0;
    D_800A2D24_A3924 = arg1;
    D_800A2D28_A3928 = 0x80;
    new_var = &D_800A2990_A3590->unk20;
    new_var2 = *new_var;
    D_800A2D38_A3938 = arg2;
    D_800A2D20_A3920 = new_var2;
    osSendMesg(&D_800A2CD0_A38D0, (OSMesg *)1, OS_MESG_BLOCK);
    osRecvMesg(&D_800A2CF0_A38F0, (OSMesg *)(&sp10), OS_MESG_BLOCK);
    func_800570BC_57CBC();
}

void func_80058490_59090(s32 arg0, s32 arg1) {
    func_80058414_59014(arg0, arg1, 0xC);
}

INCLUDE_ASM("asm/nonmatchings/56C80", func_800584AC_590AC);

void func_80058530_59130(s32 arg0, s32 arg1) {
    func_800584AC_590AC(arg0, arg1, 0xC);
}

INCLUDE_ASM("asm/nonmatchings/56C80", func_8005854C_5914C);

void func_800585C8_591C8(s32 arg0) {
    func_8005854C_5914C(arg0, 0xC);
}

INCLUDE_ASM("asm/nonmatchings/56C80", func_800585E4_591E4);

void *func_80058638_59238() {
    void *message;

    if (D_80093BA5_947A5 != NULL) {
        osSendMesg(&D_800A2CD0_A38D0, (OSMesg *)0xE, OS_MESG_BLOCK);
        osRecvMesg(&D_800A2CF0_A38F0, &message, OS_MESG_BLOCK);

        return message;
    }

    return NULL;
}
