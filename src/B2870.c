#include "common.h"
#include "gamestate.h"
#include "task_scheduler.h"

typedef void (*FuncPtr)(void *);

typedef struct {
    u8 pad[0x44C];
    s32 unk44C;
    s32 unk450;
    s32 unk454;
    u8 pad2[0x766];
    u8 unkBBE;
} Arg0Struct;

extern FuncPtr D_800BCB5C_B411C[];
extern FuncPtr D_800BCB74_B4134[];
extern void func_800B00D4_9FF84(Arg0Struct *, s32);
extern void func_800B02AC(Arg0Struct *);

INCLUDE_ASM("asm/nonmatchings/B2870", func_800BB2B0_B2870);

INCLUDE_ASM("asm/nonmatchings/B2870", func_800BB66C_B2C2C);

void func_800BB86C_B2E2C(Arg0Struct *arg0) {
    D_800BCB5C_B411C[arg0->unkBBE](arg0);
}

s32 func_800BB89C_B2E5C(Arg0Struct *arg0) {
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

    return 0;
}

INCLUDE_ASM("asm/nonmatchings/B2870", func_800BB930_B2EF0);

INCLUDE_ASM("asm/nonmatchings/B2870", func_800BBE68_B3428);

INCLUDE_ASM("asm/nonmatchings/B2870", func_800BBEBC_B347C);

void func_800BBF40_B3500(Arg0Struct *arg0) {
    D_800BCB74_B4134[arg0->unkBBE](arg0);
}

INCLUDE_ASM("asm/nonmatchings/B2870", func_800BBF70_B3530);

INCLUDE_ASM("asm/nonmatchings/B2870", func_800BC094_B3654);

INCLUDE_ASM("asm/nonmatchings/B2870", func_800BC1C0_B3780);

INCLUDE_ASM("asm/nonmatchings/B2870", func_800BC378_B3938);

INCLUDE_ASM("asm/nonmatchings/B2870", func_800BC474_B3A34);

INCLUDE_ASM("asm/nonmatchings/B2870", func_800BC5A8_B3B68);

INCLUDE_ASM("asm/nonmatchings/B2870", func_800BCA10_B3FD0);
