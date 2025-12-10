#include "5E590.h"
#include "common.h"
#include "displaylist.h"
#include "task_scheduler.h"

typedef void (*FuncPtr)(void *);

typedef struct {
    /* 0x0 */ void *ptr;
    u8 _pad[0x38]
} Element0x3C; // Size: 0x3C

typedef struct {
    u8 _pad0[0x38];
    u8 unk38[0x6C - 0x38];
    u8 unk6C;
    u8 unk6D;
    u8 unk6E;
    u8 _pad6F[1];
    u8 unk70;
    u8 unk71;
    u8 unk72;
    u8 _pad73[0x328 - 0x73];
    // Element0x3C elements[12] overlaps at offset 0x58
    u8 _pad328[0x434 - 0x328];
    s32 unk434;
    s32 unk438;
    s32 unk43C;
    u8 _pad440[0x44C - 0x440];
    s32 unk44C;
    s32 unk450;
    s32 unk454;
    u8 _pad458[0x984 - 0x458];
    u8 unk984[0xC];
    u8 _pad990[0xAE0 - 0x990];
    s32 unkAE0;
    u8 _padAE4[0xB2C - 0xAE4];
    s32 unkB2C;
    u8 _padB30[0xB84 - 0xB30];
    s32 unkB84;
    s32 unkB88;
    u8 _padB8C[0xB94 - 0xB8C];
    u16 unkB94;
    u8 _padB96[0xBB4 - 0xB96];
    u8 unkBB4;
    u8 _padBB5[0xBB7 - 0xBB5];
    u8 unkBB7;
    u8 _padBB8[0xBB9 - 0xBB8];
    u8 unkBB9;
    u8 unkBBA;
    u8 _padBBB[0xBBD - 0xBBB];
    u8 unkBBD;
    u8 unkBBE;
    u8 unkBBF;
    u8 unkBC0;
    u8 _padBC1[0xBC9 - 0xBC1];
    u8 unkBC9;
    u8 unkBCA;
    u8 _padBCB[1];
    u8 unkBCC;
    u8 _padBCD[0xBDB - 0xBCD];
    u8 unkBDB;
} func_800BC4AC_arg;

extern FuncPtr D_800BCA44_B1F34[];
extern FuncPtr D_800BCA5C_B1F4C[];

extern u16 D_8009ADE0_9B9E0;
extern u8 D_800BACC8_AAB78;
extern u8 D_800BACC9_AAB79;
extern u8 D_800BACCA_AAB7A;
extern u8 D_800BACCC_AAB7C;
extern u8 D_800BACCD_AAB7D;
extern u8 D_800BACCE_AAB7E;

extern void func_800B00D4_9FF84(func_800BC4AC_arg *, s32);
extern void func_800B02AC_A015C(func_800BC4AC_arg *);
extern s32 func_8005D308_5DF08(func_800BC4AC_arg *, s32);
extern void func_8005D180_5DD80(func_800BC4AC_arg *, s32);
extern u16 func_80059E90_5AA90(void *arg0, void *arg1, u16 arg2, void *arg3);
extern void func_8005C868_5D468(void *arg0);
extern void func_8005CFFC_5DBFC(void *arg0, u16 arg1, void *arg2, void *arg3, void *arg4);
extern void func_80064808_65408(s32, void *, u8);
extern void enqueueMultiPartDisplayList(s32, void *, u8);
extern void func_800BC61C_B1B0C(func_800BC4AC_arg *);
extern void func_8004C10C_4CD0C(func_800BC4AC_arg *);

INCLUDE_ASM("asm/nonmatchings/levels/ice_land_boss", func_800BB2B0_B07A0);

INCLUDE_ASM("asm/nonmatchings/levels/ice_land_boss", func_800BB66C_B0B5C);

void func_800BB8E0_B0DD0(func_800BC4AC_arg *arg0) {
    D_800BCA44_B1F34[arg0->unkBBE](arg0);
}

void func_800BB910_B0E00(func_800BC4AC_arg *arg0) {
    s32 i;
    Element0x3C *elements = (Element0x3C *)arg0;

    for (i = 0; i < 12; i++) {
        *(void **)((u8 *)elements + i * 0x3C + 0x58) =
            (void *)(loadAssetByIndex_953B0(arg0->unkBB9, arg0->unkBBA) + i * 16 + 0xC0);
    }

    arg0->unkAE0 = 0x100000;
    arg0->unkB2C = 0x100000;
    arg0->unkBB4 = 1;
    arg0->unkB84 = arg0->unkB84 | 0x400000;
}

s32 func_800BB998_B0E88(func_800BC4AC_arg *arg0) {
    s32 pad[3];
    GameState *gameState = getCurrentAllocation();

    if (gameState->unk79 == 0) {
        func_800B00D4_9FF84(arg0, 1);
        return 1;
    }

    arg0->unk44C -= arg0->unk44C / 8;
    arg0->unk454 -= arg0->unk454 / 8;
    arg0->unk450 += -0x8000;
    func_800B02AC_A015C(arg0);

    if (arg0->unkB84 & 0x400000) {
        func_8005D180_5DD80(arg0, 2);
    } else {
        func_8005D180_5DD80(arg0, 0);
    }

    return 0;
}

INCLUDE_ASM("asm/nonmatchings/levels/ice_land_boss", func_800BBA54_B0F44);

s32 func_800BC008_B14F8(func_800BC4AC_arg *arg0) {
    s32 pad[3];

    getCurrentAllocation();

    arg0->unk44C -= arg0->unk44C / 8;
    arg0->unk454 -= arg0->unk454 / 8;
    arg0->unk450 += -0x8000;
    func_800B02AC_A015C(arg0);

    if (arg0->unkB84 & 0x400000) {
        func_8005D180_5DD80(arg0, 2);
    } else {
        func_8005D180_5DD80(arg0, 0);
    }

    return 0;
}

void func_800BC0A8_B1598(func_800BC4AC_arg *arg0) {
    D_800BCA5C_B1F4C[arg0->unkBBE](arg0);
}

INCLUDE_ASM("asm/nonmatchings/levels/ice_land_boss", func_800BC0D8_B15C8);

s32 func_800BC3B8_B18A8(func_800BC4AC_arg *arg0) {
    u8 temp;

    temp = arg0->unkBBF;
    if (temp == 0) {
        arg0->unkBBF = temp + 1;
        arg0->unk450 = 0x80000;
        if (!(arg0->unkB84 & 0x80000)) {
            if (arg0->unkBDB != 0) {
                arg0->unkBDB = arg0->unkBDB - 1;
            }
        }
    }

    arg0->unkB88 = 0x200;
    arg0->unk44C = 0;
    arg0->unk454 = 0;
    arg0->unk450 = arg0->unk450 + (-0x8000);

    func_800B02AC_A015C(arg0);
    func_8005D180_5DD80(arg0, 5);

    if ((D_8009ADE0_9B9E0 & 0xF) == 0) {
        func_8004C10C_4CD0C(arg0);
    }

    if (arg0->unk450 < 0) {
        if (!(arg0->unkB84 & 0x1)) {
            temp = arg0->unkBDB;
            arg0->unkB88 = 0;
            arg0->unkBBD = 1;
            arg0->unkBBE = 1;
            arg0->unkBBF = 0;
            arg0->unkBC0 = 0;

            if (temp == 0) {
                arg0->unkB84 = arg0->unkB84 | 0x100000;
            }

            return 0;
        }
    }

    return 0;
}

s32 func_800BC4AC_B199C(func_800BC4AC_arg *arg0) {
    s32 pad[3];

    getCurrentAllocation();

    arg0->unk450 += -0x8000;
    func_800B02AC_A015C(arg0);

    if (arg0->unkBBF == 0) {
        if (func_8005D308_5DF08(arg0, 6) != 0) {
            arg0->unkBBF = arg0->unkBBF + 1;
        }
        return 0;
    }

    func_8005D180_5DD80(arg0, 7);

    return 0;
}

void func_800BC520_B1A10(func_800BC4AC_arg *arg0) {
    s32 pad[8];
    s32 sp38[3];
    s32 pad2[8];
    void *alloc;
    void *allocPlus30;
    u16 temp;

    alloc = getCurrentAllocation();
    memcpy(arg0->unk984, &arg0->unk434, 0xC);
    allocPlus30 = (u8 *)alloc + 0x30;
    temp = func_80059E90_5AA90(arg0, allocPlus30, arg0->unkB94, &arg0->unk434);
    arg0->unkB94 = temp;
    func_80060CDC_618DC(allocPlus30, temp, &arg0->unk434, 0x187000, sp38);
    arg0->unk434 = arg0->unk434 + sp38[0];
    arg0->unk43C = arg0->unk43C + sp38[2];
    func_8005C868_5D468(arg0);

    if (arg0->unkB84 & 0x10000) {
        arg0->unkBC9 = 0;
    } else {
        func_8005CFFC_5DBFC(allocPlus30, arg0->unkB94, &arg0->unk434, &arg0->unkBC9, &arg0->unkBCC);
        arg0->unkBCA = arg0->unkBC9 >> 4;
        arg0->unkBC9 = arg0->unkBC9 & 0xF;
    }
}

INCLUDE_ASM("asm/nonmatchings/levels/ice_land_boss", func_800BC61C_B1B0C);

void func_800BC7A8_B1C98(func_800BC4AC_arg *arg0) {
    s32 pad[36];
    s32 i;
    s32 index;

    getCurrentAllocation();
    func_800BC61C_B1B0C(arg0);

    index = arg0->unkBCC >> 4;

    if (index == 0) {
        for (i = 0; i < 4; i++) {
            func_80064808_65408(i, &arg0->unk38, arg0->unkBB7);
        }
    } else {
        arg0->unk6C = *(&D_800BACC8_AAB78 + index * 8);
        arg0->unk6D = *(&D_800BACC9_AAB79 + index * 8);
        arg0->unk6E = *(&D_800BACCA_AAB7A + index * 8);
        arg0->unk70 = *(&D_800BACCC_AAB7C + index * 8);
        arg0->unk71 = *(&D_800BACCD_AAB7D + index * 8);
        arg0->unk72 = *(&D_800BACCE_AAB7E + index * 8);

        for (i = 0; i < 4; i++) {
            enqueueMultiPartDisplayList(i, &arg0->unk38, arg0->unkBB7);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/levels/ice_land_boss", func_800BC89C_B1D8C);
