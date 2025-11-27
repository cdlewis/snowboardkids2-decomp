#include "common.h"

extern void setCleanupCallback(void *);
extern void setCallback(void *);
extern void *loadAsset_34CB50(void);
extern void func_8004D3E4_4DFE4(void *);
extern void func_8004D464_4E064(void *);
extern u8 randA(void);
extern void func_8004E2D8_4EED8(void);
extern void func_8004E410_4F010(void);
extern void func_8004E614_4F214(void);
extern void *freeNodeMemory(void *);

typedef struct {
    u8 pad0[0x4];
    void *unk4;
    u8 pad8[0x8];
    void *unk10;
} Struct_func_8004DCC4;

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    u16 unk8;
    u16 unkA;
    u32 unkC;
} Struct_func_8004D3A4;

typedef struct {
    void *unk0;
    u8 pad4[0x4];
    void *unk8;
} Struct_func_8004FF28;

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004C170_4CD70);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004C254_4CE54);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004C294_4CE94);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004C2C0_4CEC0);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004C46C_4D06C);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004C5A8_4D1A8);

typedef struct {
    u8 pad0[0x4];
    void *unk4;
    u8 pad8[0x14];
    void *unk1C;
} Struct_func_8004C6F0;

void func_8004C6F0_4D2F0(Struct_func_8004C6F0 *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
    arg0->unk1C = freeNodeMemory(arg0->unk1C);
}

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004C728_4D328);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004C928_4D528);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004C9E8_4D5E8);

void func_8004CA58_4D658(Struct_func_8004DCC4 *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
    arg0->unk10 = freeNodeMemory(arg0->unk10);
}

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004CA90_4D690);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004CBF0_4D7F0);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004CCC4_4D8C4);

typedef struct {
    void *unk0;
    u8 pad4[0x8];
    void *unkC;
} Struct_func_8004CD88;

void func_8004CD88_4D988(Struct_func_8004CD88 *arg0) {
    arg0->unk0 = freeNodeMemory(arg0->unk0);
    arg0->unkC = freeNodeMemory(arg0->unkC);
}

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004CDC0_4D9C0);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004CEC4_4DAC4);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004D0D0_4DCD0);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004D134_4DD34);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004D19C_4DD9C);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004D23C_4DE3C);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004D298_4DE98);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004D338_4DF38);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004D364_4DF64);

void func_8004D3A4_4DFA4(Struct_func_8004D3A4 *arg0) {
    arg0->unk4 = loadAsset_34CB50();
    setCleanupCallback(func_8004D464_4E064);
    setCallback(func_8004D3E4_4DFE4);
}

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004D3E4_4DFE4);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004D464_4E064);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004D490_4E090);

INCLUDE_RODATA("asm/nonmatchings/4CD70", D_8009E884_9F484);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004D544_4E144);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004D63C_4E23C);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004D6FC_4E2FC);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004D784_4E384);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004D7D0_4E3D0);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004D858_4E458);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004D890_4E490);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004D8E4_4E4E4);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004D954_4E554);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004D98C_4E58C);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004D9B8_4E5B8);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004D9D0_4E5D0);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004DB98_4E798);

void func_8004DCC4_4E8C4(Struct_func_8004DCC4 *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
    arg0->unk10 = freeNodeMemory(arg0->unk10);
}

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004DCFC_4E8FC);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004DDD0_4E9D0);

void func_8004DEC0_4EAC0(Struct_func_8004DCC4 *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
    arg0->unk10 = freeNodeMemory(arg0->unk10);
}

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004DEF8_4EAF8);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004DFA0_4EBA0);

void func_8004E0BC_4ECBC(Struct_func_8004DCC4 *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
    arg0->unk10 = freeNodeMemory(arg0->unk10);
}

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004E0F4_4ECF4);

typedef struct {
    u8 pad0[0x4];
    void *unk4;
    u8 pad8[0x2];
    u8 unkA;
    u8 padB[0x7];
    s16 unk12;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
} Struct_func_8004E134;

void func_8004E134_4ED34(Struct_func_8004E134 *arg0) {
    u8 temp;

    arg0->unk4 = loadAsset_34CB50();
    temp = randA();

    switch (temp & 1) {
        case 0:
            arg0->unk12 = ((randA() & 0xFF) - 0x80) << 4;
            arg0->unk14 = -0x780;
            arg0->unk16 = randA() & 0xF;
            arg0->unk1C = (randA() & 0xFF) << 4;
            if (randA() & 1) {
                arg0->unk1A = (randA() & 7) | 0x40;
            } else {
                arg0->unk1A = (randA() & 7) | 0x20;
            }
            setCallback(func_8004E410_4F010);
            break;
        case 1:
            arg0->unk12 = ((randA() & 0xFF) - 0x80) << 4;
            arg0->unk14 = -0x780;
            if (randA() & 1) {
                arg0->unk1A = (randA() & 7) + 0x3C;
            } else {
                arg0->unk1A = (randA() & 7) + 0x22;
            }
            arg0->unk1C = (randA() & 1) - 1;
            arg0->unk18 = (-((randA() & 0xFF) << 3)) & 0xF;
            arg0->unk1E = randA() & 0xF;
            __asm__ volatile("" ::: "memory");
            setCallback(func_8004E2D8_4EED8);
            break;
    }

    arg0->unkA = (randA() & 0xFF) % 5 + 12;
    setCleanupCallback(func_8004E614_4F214);
}

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004E2D8_4EED8);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004E410_4F010);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004E4EC_4F0EC);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004E580_4F180);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004E614_4F214);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004E640_4F240);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004E6A4_4F2A4);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004E6F8_4F2F8);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004E794_4F394);

typedef struct {
    u8 pad0[0x4];
    void *unk4;
    u8 pad8[0x1C];
    void *unk24;
} Struct_func_8004E884;

void func_8004E884_4F484(Struct_func_8004E884 *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
    arg0->unk24 = freeNodeMemory(arg0->unk24);
}

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004E8BC_4F4BC);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004E940_4F540);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004EA28_4F628);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004EA54_4F654);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004EAF0_4F6F0);

typedef struct {
    u8 pad0[0x4];
    void *unk4;
    u8 pad8[0x10];
    void *unk18;
} Struct_func_8004EB90;

void func_8004EB90_4F790(Struct_func_8004EB90 *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
    arg0->unk18 = freeNodeMemory(arg0->unk18);
}

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004EBC8_4F7C8);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004EC08_4F808);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004ECA4_4F8A4);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004ED94_4F994);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004EDCC_4F9CC);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004EE24_4FA24);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004EEB4_4FAB4);

typedef struct {
    u8 pad0[0x4];
    void *unk4;
    u8 pad8[0x4];
    void *unkC;
} Struct_func_8004F04C;

void func_8004F04C_4FC4C(Struct_func_8004F04C *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
    arg0->unkC = freeNodeMemory(arg0->unkC);
}

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004F084_4FC84);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004F104_4FD04);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004F168_4FD68);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004F194_4FD94);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004F1D4_4FDD4);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004F27C_4FE7C);

void func_8004F3EC_4FFEC(Struct_func_8004DCC4 *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
    arg0->unk10 = freeNodeMemory(arg0->unk10);
}

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004F424_50024);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004F4A8_500A8);

void func_8004F69C_5029C(Struct_func_8004F04C *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
    arg0->unkC = freeNodeMemory(arg0->unkC);
}

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004F6D4_502D4);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004F760_50360);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004F7F4_503F4);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004F820_50420);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004F898_50498);

void func_8004F9E8_505E8(Struct_func_8004F04C *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
    arg0->unkC = freeNodeMemory(arg0->unkC);
}

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004FA20_50620);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004FAB4_506B4);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004FB64_50764);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004FBE8_507E8);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004FC34_50834);

void func_8004FCB8_508B8(Struct_func_8004F04C *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
    arg0->unkC = freeNodeMemory(arg0->unkC);
}

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004FCF0_508F0);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004FD30_50930);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004FDD0_509D0);

void func_8004FF28_50B28(Struct_func_8004FF28 *arg0) {
    arg0->unk8 = freeNodeMemory(arg0->unk8);
    arg0->unk0 = freeNodeMemory(arg0->unk0);
}

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004FF60_50B60);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004FFB8_50BB8);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_80050098_50C98);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_800500F0_50CF0);

INCLUDE_RODATA("asm/nonmatchings/4CD70", D_8009E894_9F494);

INCLUDE_RODATA("asm/nonmatchings/4CD70", D_8009E89C_9F49C);

INCLUDE_RODATA("asm/nonmatchings/4CD70", D_8009E8A0_9F4A0);

INCLUDE_RODATA("asm/nonmatchings/4CD70", D_8009E8A8_9F4A8);

INCLUDE_RODATA("asm/nonmatchings/4CD70", D_8009E8AC_9F4AC);

INCLUDE_RODATA("asm/nonmatchings/4CD70", D_8009E8C0_9F4C0);

INCLUDE_RODATA("asm/nonmatchings/4CD70", D_8009E8D4_9F4D4);

INCLUDE_RODATA("asm/nonmatchings/4CD70", D_8009E8E0_9F4E0);

INCLUDE_RODATA("asm/nonmatchings/4CD70", D_8009E8EC_9F4EC);

INCLUDE_RODATA("asm/nonmatchings/4CD70", D_8009E8F8_9F4F8);

INCLUDE_RODATA("asm/nonmatchings/4CD70", D_8009E904_9F504);

INCLUDE_RODATA("asm/nonmatchings/4CD70", D_8009E914_9F514);

INCLUDE_RODATA("asm/nonmatchings/4CD70", D_8009E924_9F524);

INCLUDE_RODATA("asm/nonmatchings/4CD70", D_8009E928_9F528);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8005011C_50D1C);
