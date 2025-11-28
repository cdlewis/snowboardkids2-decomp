#include "common.h"

typedef void (*FuncPtr)(void *);

typedef struct {
    u8 pad[0xBBE];
    u8 unkBBE;
} Arg0Struct;

extern FuncPtr D_800BCB5C_B411C[];
extern FuncPtr D_800BCB74_B4134[];

INCLUDE_ASM("asm/nonmatchings/B2870", func_800BB2B0_B2870);

INCLUDE_ASM("asm/nonmatchings/B2870", func_800BB66C_B2C2C);

void func_800BB86C_B2E2C(Arg0Struct *arg0) {
    D_800BCB5C_B411C[arg0->unkBBE](arg0);
}

INCLUDE_ASM("asm/nonmatchings/B2870", func_800BB89C_B2E5C);

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
