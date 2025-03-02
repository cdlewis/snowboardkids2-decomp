#include "common.h"

INCLUDE_ASM("asm/nonmatchings/6B140", func_8006A540_6B140);

INCLUDE_ASM("asm/nonmatchings/6B140", func_8006A588_6B188);

INCLUDE_ASM("asm/nonmatchings/6B140", func_8006A5CC_6B1CC);

INCLUDE_ASM("asm/nonmatchings/6B140", func_8006A640_6B240);

INCLUDE_ASM("asm/nonmatchings/6B140", func_8006A6B0_6B2B0);

INCLUDE_ASM("asm/nonmatchings/6B140", func_8006A724_6B324);

INCLUDE_ASM("asm/nonmatchings/6B140", func_8006A838_6B438);

INCLUDE_ASM("asm/nonmatchings/6B140", func_8006A948_6B548);

INCLUDE_ASM("asm/nonmatchings/6B140", func_8006AA5C_6B65C);

INCLUDE_ASM("asm/nonmatchings/6B140", func_8006AB64_6B764);

INCLUDE_ASM("asm/nonmatchings/6B140", func_8006AD9C_6B99C);

INCLUDE_ASM("asm/nonmatchings/6B140", func_8006AFDC_6BBDC);

INCLUDE_ASM("asm/nonmatchings/6B140", func_8006AFEC_6BBEC);

INCLUDE_ASM("asm/nonmatchings/6B140", func_8006B084_6BC84);

INCLUDE_ASM("asm/nonmatchings/6B140", func_8006B35C_6BF5C);

INCLUDE_ASM("asm/nonmatchings/6B140", func_8006B5A8_6C1A8);

INCLUDE_ASM("asm/nonmatchings/6B140", func_8006B7DC_6C3DC);

INCLUDE_ASM("asm/nonmatchings/6B140", func_8006BA10_6C610);

INCLUDE_ASM("asm/nonmatchings/6B140", func_8006BC6C_6C86C);

INCLUDE_ASM("asm/nonmatchings/6B140", func_8006BDBC_6C9BC);

INCLUDE_ASM("asm/nonmatchings/6B140", func_8006BE4C_6CA4C);

INCLUDE_ASM("asm/nonmatchings/6B140", func_8006BEDC_6CADC);

INCLUDE_ASM("asm/nonmatchings/6B140", func_8006BFB8_6CBB8);

INCLUDE_ASM("asm/nonmatchings/6B140", func_8006C130_6CD30);

INCLUDE_ASM("asm/nonmatchings/6B140", func_8006C2A8_6CEA8);

s32 approximate_sqrt(u32 input) {
    u32 remainder = 0;
    s32 quotient = 0;
    s32 divisor_shift = 30;
    u32 dividend = input;

    while (divisor_shift >= 0) {
        remainder <<= 1;
        quotient <<= 1;

        if (remainder < (dividend >> divisor_shift)) {
            remainder += 1;
            dividend -= (remainder << divisor_shift);
            remainder += 1;
            quotient += 1;
        }
        divisor_shift -= 2;
    }
    return quotient;
}

INCLUDE_ASM("asm/nonmatchings/6B140", func_8006C438_6D038);

INCLUDE_ASM("asm/nonmatchings/6B140", func_8006C590_6D190);

INCLUDE_ASM("asm/nonmatchings/6B140", func_8006C5E0_6D1E0);

INCLUDE_ASM("asm/nonmatchings/6B140", func_8006C654_6D254);

INCLUDE_ASM("asm/nonmatchings/6B140", func_8006C8FC_6D4FC);

INCLUDE_ASM("asm/nonmatchings/6B140", func_8006CF7C_6DB7C);

INCLUDE_ASM("asm/nonmatchings/6B140", func_8006D21C_6DE1C);
