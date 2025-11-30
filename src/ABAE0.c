#include "common.h"
#include "task_scheduler.h"

typedef void (*FuncPtr)(void *);

extern FuncPtr D_800BC454_ACC84[];
extern FuncPtr D_800BC460_ACC90[];
extern u8 D_800BACC8_AAB78;
extern u8 D_800BACC9_AAB79;
extern u8 D_800BACCA_AAB7A;
extern u8 D_800BACCC_AAB7C;
extern u8 D_800BACCD_AAB7D;
extern u8 D_800BACCE_AAB7E;

typedef struct {
    u8 pad[0x38];
    u8 unk38[0x6C - 0x38];
    u8 unk6C;
    u8 unk6D;
    u8 unk6E;
    u8 pad6F[1];
    u8 unk70;
    u8 unk71;
    u8 unk72;
    u8 pad73[0x434 - 0x73];
    s32 unk434;
    s32 unk438;
    s32 unk43C;
    u8 pad440[0xC];
    s32 unk44C;
    s32 unk450;
    s32 unk454;
    u8 pad458[0x10];
    s32 unk468;
    u8 pad46C[0x8];
    s32 unk474;
    u8 pad478[0x984 - 0x478];
    u8 unk984[0xC];
    u8 pad990[0xA8C - 0x990];
    u16 unkA8C;
    u8 padA8E[0xA9C - 0xA8E];
    u16 unkA9C;
    u16 unkA9E;
    u8 padAA0[0xB84 - 0xAA0];
    s32 unkB84;
    s32 unkB88;
    s32 unkB8C;
    u8 padB90[0xB94 - 0xB90];
    u16 unkB94;
    u8 padB96[0xBB7 - 0xB96];
    u8 unkBB7;
    u8 unkBB8;
    u8 padBB9[0xBBD - 0xBB9];
    u8 unkBBD;
    u8 unkBBE;
    u8 unkBBF;
    u8 unkBC0;
    u8 padBC1[0x8];
    u8 unkBC9;
    u8 unkBCA;
    u8 padBCB[1];
    u8 unkBCC;
    u8 padBCD[0xBDB - 0xBCD];
    u8 unkBDB;
} Arg0Struct;

extern void func_800B00D4_9FF84(Arg0Struct *, s32);
extern void func_800B02AC_A015C(Arg0Struct *);
extern void func_8005D180_5DD80(Arg0Struct *, s32);
extern void func_80056B7C_5777C(void *, s32);
extern u16 func_80059E90_5AA90(void *arg0, void *arg1, u16 arg2, void *arg3);
extern void func_80060CDC_618DC(void *arg0, u16 arg1, void *arg2, s32 arg3, s32 *arg4);
extern void func_8005C868_5D468(void *arg0);
extern void func_8005CFFC_5DBFC(void *arg0, u16 arg1, void *arg2, void *arg3, void *arg4);
extern void func_80064808_65408(s32, void *, u8);
extern void enqueueMultiPartDisplayList(s32, void *, u8);
extern void func_800BC0E8_AC918(Arg0Struct *);

INCLUDE_ASM("asm/nonmatchings/ABAE0", func_800BB2B0_ABAE0);

INCLUDE_ASM("asm/nonmatchings/ABAE0", func_800BB754_ABF84);

void func_800BB9E8_AC218(Arg0Struct *arg0) {
    D_800BC454_ACC84[arg0->unkBBE](arg0);
}

s32 func_800BBA18_AC248(Arg0Struct *arg0) {
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
    func_8005D180_5DD80(arg0, 0);

    return 0;
}

INCLUDE_ASM("asm/nonmatchings/ABAE0", func_800BBAB8_AC2E8);

s32 func_800BBD98_AC5C8(Arg0Struct *arg0) {
    s32 pad[3];
    getCurrentAllocation();

    arg0->unk44C -= arg0->unk44C / 8;
    arg0->unk454 -= arg0->unk454 / 8;
    arg0->unk450 += -0x8000;
    func_800B02AC_A015C(arg0);
    func_8005D180_5DD80(arg0, 0);

    return 0;
}

void func_800BBE1C_AC64C(Arg0Struct *arg0) {
    D_800BC460_ACC90[arg0->unkBBE](arg0);
}

INCLUDE_ASM("asm/nonmatchings/ABAE0", func_800BBE4C_AC67C);

s32 func_800BBF3C_AC76C(Arg0Struct *arg0) {
    if (arg0->unkBBF == 0) {
        arg0->unkA8C = 0xFFFF;
        arg0->unk450 = 0x80000;
        arg0->unkBBF = arg0->unkBBF + 1;
        func_80056B7C_5777C(&arg0->unk434, 0x21);
    }

    arg0->unkB88 = 0x200;
    arg0->unk44C = 0;
    arg0->unk454 = 0;
    arg0->unk450 = arg0->unk450 + -0x8000;
    func_800B02AC_A015C(arg0);
    func_8005D180_5DD80(arg0, 2);

    if (arg0->unkB84 & 1) {
    } else {
        arg0->unkB88 = 0;
        arg0->unkBBD = 1;
        arg0->unkBBE = 1;
        arg0->unkBBF = 0;
        arg0->unkBC0 = 0;
    }
    return 0;
}

void func_800BBFEC_AC81C(Arg0Struct *arg0) {
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

INCLUDE_ASM("asm/nonmatchings/ABAE0", func_800BC0E8_AC918);

void func_800BC23C_ACA6C(Arg0Struct *arg0) {
    s32 i;
    s32 index;
    s32 pad[36];

    getCurrentAllocation();
    func_800BC0E8_AC918(arg0);

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

INCLUDE_ASM("asm/nonmatchings/ABAE0", func_800BC330_ACB60);
