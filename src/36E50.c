#include "common.h"

typedef struct {
    u8 padding0[16];
    u8 unk10;
    u8 padding1[98];
    u8 unk73;
    u8 padding2[944];
    u8 unk424;
    u8 unk425;
} func_80036274_36E74_large_struct;

extern void func_80069CC0_6A8C0(void*); /* extern */
extern void func_80036274_36E74;

extern func_80036274_36E74_large_struct* func_800698BC_6A4BC();                     /* extern */
extern func_80036274_36E74_large_struct* func_800699F4_6A5F4(void*, int, int, int); /* extern */

void func_80036250_36E50(s8* arg0) {
    *arg0 = 0;
    func_80069CC0_6A8C0(&func_80036274_36E74);
}

void func_80036274_36E74(u8* arg0) {
    func_80036274_36E74_large_struct* temp_v0;
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

INCLUDE_ASM("asm/nonmatchings/36E50", func_80036328_36F28);

INCLUDE_ASM("asm/nonmatchings/36E50", func_80036424_37024);

INCLUDE_ASM("asm/nonmatchings/36E50", func_80036848_37448);

INCLUDE_ASM("asm/nonmatchings/36E50", func_80036880_37480);

INCLUDE_ASM("asm/nonmatchings/36E50", func_80036920_37520);

INCLUDE_ASM("asm/nonmatchings/36E50", func_80036A10_37610);

INCLUDE_ASM("asm/nonmatchings/36E50", func_80036A3C_3763C);

INCLUDE_ASM("asm/nonmatchings/36E50", func_80036A68_37668);

INCLUDE_ASM("asm/nonmatchings/36E50", func_80036AF8_376F8);

INCLUDE_ASM("asm/nonmatchings/36E50", func_80036C20_37820);

INCLUDE_ASM("asm/nonmatchings/36E50", func_80036D54_37954);

INCLUDE_ASM("asm/nonmatchings/36E50", func_800371CC_37DCC);
