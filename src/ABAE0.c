#include "common.h"
#include "task_scheduler.h"

typedef void (*FuncPtr)(void *);

extern FuncPtr D_800BC454_ACC84[];
extern FuncPtr D_800BC460_ACC90[];

typedef struct {
    u8 pad[0x434];
    u8 unk434;
    u8 pad435[0x17];
    s32 unk44C;
    s32 unk450;
    s32 unk454;
    u8 pad458[0xA8C - 0x458];
    u16 unkA8C;
    u8 padA8E[0xB84 - 0xA8E];
    s32 unkB84;
    s32 unkB88;
    u8 padB8C[0xBBD - 0xB8C];
    u8 unkBBD;
    u8 unkBBE;
    u8 unkBBF;
    u8 unkBC0;
} Arg0Struct;

extern void func_800B00D4_9FF84(Arg0Struct *, s32);
extern void func_800B02AC_A015C(Arg0Struct *);
extern void func_8005D180_5DD80(Arg0Struct *, s32);
extern void func_80056B7C_5777C(void *, s32);

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

INCLUDE_ASM("asm/nonmatchings/ABAE0", func_800BBFEC_AC81C);

INCLUDE_ASM("asm/nonmatchings/ABAE0", func_800BC0E8_AC918);

INCLUDE_ASM("asm/nonmatchings/ABAE0", func_800BC23C_ACA6C);

INCLUDE_ASM("asm/nonmatchings/ABAE0", func_800BC330_ACB60);
