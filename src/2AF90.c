#include "common.h"

INCLUDE_ASM("asm/nonmatchings/2AF90", func_8002A390_2AF90);

INCLUDE_ASM("asm/nonmatchings/2AF90", func_8002A4AC_2B0AC);

INCLUDE_ASM("asm/nonmatchings/2AF90", func_8002A7CC_2B3CC);

INCLUDE_ASM("asm/nonmatchings/2AF90", func_8002ACFC_2B8FC);

INCLUDE_ASM("asm/nonmatchings/2AF90", func_8002ADB4_2B9B4);

INCLUDE_ASM("asm/nonmatchings/2AF90", func_8002AE80_2BA80);

INCLUDE_ASM("asm/nonmatchings/2AF90", func_8002B248_2BE48);

s16 func_8002B4B8_2C0B8(u16 arg0, u16 arg1) {
    s16 diff;

    arg0 &= 0x1FFF;
    arg1 &= 0x1FFF;
    diff = (arg1 - arg0) & 0x1FFF;

    if (diff >= 0x1001) {
        diff |= 0xE000;
    }

    return diff;
}

INCLUDE_ASM("asm/nonmatchings/2AF90", func_8002B4E4_2C0E4);

INCLUDE_ASM("asm/nonmatchings/2AF90", func_8002B598_2C198);

INCLUDE_ASM("asm/nonmatchings/2AF90", func_8002B760_2C360);

INCLUDE_ASM("asm/nonmatchings/2AF90", func_8002B94C_2C54C);

INCLUDE_ASM("asm/nonmatchings/2AF90", func_8002BAEC_2C6EC);
