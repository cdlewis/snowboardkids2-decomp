#include "common.h"
#include "task_scheduler.h"

typedef void (*FuncPtr)(void *);

extern FuncPtr D_800BC454_ACC84[];
extern FuncPtr D_800BC460_ACC90[];

typedef struct {
    u8 pad[0x44C];
    s32 unk44C;
    s32 unk450;
    s32 unk454;
    u8 pad2[0x766];
    u8 unkBBE;
} Arg0Struct;

extern void func_800B00D4_9FF84(Arg0Struct *, s32);
extern void func_800B02AC(Arg0Struct *);
extern void func_8005D180_5DD80(Arg0Struct *, s32);

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
    func_800B02AC(arg0);
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
    func_800B02AC(arg0);
    func_8005D180_5DD80(arg0, 0);

    return 0;
}

void func_800BBE1C_AC64C(Arg0Struct *arg0) {
    D_800BC460_ACC90[arg0->unkBBE](arg0);
}

INCLUDE_ASM("asm/nonmatchings/ABAE0", func_800BBE4C_AC67C);

INCLUDE_ASM("asm/nonmatchings/ABAE0", func_800BBF3C_AC76C);

INCLUDE_ASM("asm/nonmatchings/ABAE0", func_800BBFEC_AC81C);

INCLUDE_ASM("asm/nonmatchings/ABAE0", func_800BC0E8_AC918);

INCLUDE_ASM("asm/nonmatchings/ABAE0", func_800BC23C_ACA6C);

INCLUDE_ASM("asm/nonmatchings/ABAE0", func_800BC330_ACB60);
