#include "common.h"

typedef struct {
    u8 padding0[16];
    u8 unk10;
    u8 padding1[98];
    u8 unk73;
    char padding[911];
    u8 unk403;
    char padding3[28];
    u8 unk420;
    char padding4[3];
    u8 unk424;
    u8 unk425;
    char padding2[7];
    u8 unk42D;
} func_80036274_36E74_large_struct;

typedef struct {
    s16 unk0;
    s16 unk2;
    s32 unk4;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s8 unk12;
    s8 unk13;
} func_80036328_36F28_arg_item;

typedef struct {
    func_80036328_36F28_arg_item entries[4];
    char __padding[6];
    s32 unk58;
    s16 unk5C;
    s16 unk5E;
    u8 unk60;
    char __padding2[3];
    s16 unk64;
    s16 unk66;
    s16 unk68;
    s16 unk6A;
    s16 unk6C;
    s16 unk6E;
    s16 unk70;
    s8 unk72;
} func_80036328_36F28_arg;

typedef struct {
    char padding[9];
    u8 unk9;
} D_800AFE8C_A71FC_type;

typedef struct {
    char padding[64];
    void* unk40;
} func_800371CC_37DCC_arg;

typedef struct {
    char padding[1012];
    s16 unk3F4;
    s32 unk3F8;
    char padding2[7];
    u8 unk403;
    char padding3[28];
    u8 unk420;
    char padding5[3];
    u8 unk424;
    u8 unk425;
    char padding4[7];
    u8 unk42D;
} func_800698BC_6A4BC_return;

typedef struct {
    char padding;
    s32 unk4;
    char padding2[78];
    s32 unk58;
} func_80036848_37448_arg;

typedef struct {
    u8 unk0;
    u8 unk1;
} func_80036A3C_3763C_arg;

s32 func_80035F80_36B80(int);
void func_80069CE8_6A8E8(void*);
s32 func_80069F94_6AB94(void*, void*, int);
void func_80036274_36E74(u8*);
void func_80036424_37024(void);
void func_80036848_37448(func_80036848_37448_arg*);
void func_80036328_36F28(func_80036328_36F28_arg*);
void func_80036880_37480(void);
void func_80036A68_37668(u8*);

extern void D_45A890;
extern void D_45B130;
extern func_800698BC_6A4BC_return* func_800698BC_6A4BC();
extern func_80036274_36E74_large_struct* func_800699F4_6A5F4(void*, int, int, int);

extern void func_80058220_58E20(u16, void*);
extern void D_8008FAC0_906C0;
extern s16 D_8008FD10_90910[];
extern char D_8008FD1C_9091C[];
extern D_800AFE8C_A71FC_type* D_800AFE8C_A71FC;
extern s32 func_8006A200_6AE00(s32);

void func_80036250_36E50(s8* arg0) {
    *arg0 = 0;
    func_80069CC0_6A8C0(&func_80036274_36E74);
}

void func_80036274_36E74(u8* arg0) {
    func_800698BC_6A4BC_return* temp_v0;
    func_80036274_36E74_large_struct* temp_v0_2;
    func_80036274_36E74_large_struct* temp_v0_3;

    temp_v0 = func_800698BC_6A4BC();
    if (temp_v0->unk424 != 0) {
        if (*arg0 == 0) {
            if ((u8)temp_v0->unk425 < 0xAU) {
                temp_v0_2 = func_800699F4_6A5F4(&func_80036328_36F28, 0, 0, 0x64);
                if (temp_v0_2 != NULL) {
                    temp_v0_2->unk73 = (u8)temp_v0->unk425;
                }
            } else {
                temp_v0_3 = func_800699F4_6A5F4(&func_80036880_37480, 0, 0, 0x64);
                if (temp_v0_3 != NULL) {
                    temp_v0_3->unk10 = (u8)temp_v0->unk425;
                }
            }
            *arg0 = 1;
        }
    } else {
        *arg0 = 0;
    }
}

void func_80036328_36F28(func_80036328_36F28_arg* arg0) {
    s32 resource_id;
    s32 temp_value;
    s32 i;
    s16 var_a0;

    func_80036328_36F28_arg_item* var_v1;

    resource_id = func_80069F94_6AB94(&D_45A890, &D_45B130, 0x3108);
    temp_value = func_80035F80_36B80(1);

    func_80069CE8_6A8E8(&func_80036848_37448);

    // Initialize main struct fields
    arg0->unk6E = 0x2CC;
    arg0->unk70 = 0x2CC;
    arg0->unk6A = 0x4000;
    arg0->unk6C = 0x4000;
    arg0->unk72 = 0;
    arg0->unk64 = 0;
    arg0->unk66 = 0;
    arg0->unk68 = 0xFF;

    for (i = 0; i < 4; i++) {
        var_v1 = &arg0->entries[i];
        var_v1->unk0 = 0;
        var_v1->unk2 = 0;
        var_v1->unk8 = i;
        var_v1->unk13 = 0;
        var_v1->unk12 = 0;
        var_v1->unkE = 0;
        var_v1->unk10 = 0xFF;
        var_v1->unkA = 0x400;
        var_v1->unkC = 0x400;
        var_v1->unk4 = resource_id;
    }

    arg0->unk58 = temp_value;
    arg0->unk5C = 0xFF;
    arg0->unk60 = 5;
    arg0->unk5E = 0xFF;

    func_80069CC0_6A8C0(&func_80036424_37024);
}

INCLUDE_ASM("asm/nonmatchings/36E50", func_80036424_37024);

void func_80036848_37448(func_80036848_37448_arg* arg0) {
    arg0->unk4 = func_8006A200_6AE00(arg0->unk4);
    arg0->unk58 = func_8006A200_6AE00(arg0->unk58);
}

INCLUDE_ASM("asm/nonmatchings/36E50", func_80036880_37480);

INCLUDE_ASM("asm/nonmatchings/36E50", func_80036920_37520);

INCLUDE_ASM("asm/nonmatchings/36E50", func_80036A10_37610);

void func_80036A3C_3763C(func_80036A3C_3763C_arg *arg0) {
    arg0->unk0 = 0xA;
    arg0->unk1 = 0;
    func_80069CC0_6A8C0(&func_80036A68_37668);
}

void func_80036A68_37668(u8* arg0) {
    s16 temp_v0_2;
    s16 var_v1;
    func_800698BC_6A4BC_return* temp_v0 = func_800698BC_6A4BC();

    if (temp_v0->unk3F8 > 0x400000) {
        temp_v0_2 = temp_v0->unk3F4;
        var_v1 = temp_v0_2;
        if (temp_v0_2 >= 0x1001) {
            var_v1 -= 0x2000;
        }
        if (var_v1 >= 0x821) {
            temp_v0->unk424 = 1;
            temp_v0->unk425 = (u8)*arg0;
            return;
        }
        if (var_v1 < -0x844) {
            temp_v0->unk424 = 1;
            temp_v0->unk425 = (u8)(*arg0 + 1);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/36E50", func_80036AF8_376F8);

INCLUDE_ASM("asm/nonmatchings/36E50", func_80036C20_37820);
// 93% match
// #include "func_80036C20_37820"

INCLUDE_ASM("asm/nonmatchings/36E50", func_80036D54_37954);

void func_800371CC_37DCC(func_800371CC_37DCC_arg* arg0) {
    func_800698BC_6A4BC_return* temp_v0 = func_800698BC_6A4BC();

    if (temp_v0->unk420 == 3 && temp_v0->unk42D == 8) {
        arg0->unk40 = &D_8008FAC0_906C0;
        func_80058220_58E20(D_8008FD10_90910[D_800AFE8C_A71FC->unk9], 0);
        temp_v0->unk403 = D_8008FD1C_9091C[D_800AFE8C_A71FC->unk9];
    }
}
