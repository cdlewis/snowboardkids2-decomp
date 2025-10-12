#include "common.h"

#include "D_800AFE8C_A71FC_type.h"

INCLUDE_ASM("asm/nonmatchings/2DD40", func_8002D140_2DD40);

INCLUDE_ASM("asm/nonmatchings/2DD40", func_8002D46C_2E06C);

INCLUDE_ASM("asm/nonmatchings/2DD40", func_8002D668_2E268);

INCLUDE_ASM("asm/nonmatchings/2DD40", func_8002D814_2E414);

INCLUDE_ASM("asm/nonmatchings/2DD40", func_8002DA54_2E654);

INCLUDE_ASM("asm/nonmatchings/2DD40", func_8002DE44_2EA44);

INCLUDE_ASM("asm/nonmatchings/2DD40", func_8002E024_2EC24);

INCLUDE_ASM("asm/nonmatchings/2DD40", func_8002E170_2ED70);

INCLUDE_ASM("asm/nonmatchings/2DD40", func_8002E314_2EF14);

INCLUDE_ASM("asm/nonmatchings/2DD40", func_8002E680_2F280);

INCLUDE_ASM("asm/nonmatchings/2DD40", func_8002E8B4_2F4B4);

INCLUDE_ASM("asm/nonmatchings/2DD40", func_8002EBB0_2F7B0);

INCLUDE_ASM("asm/nonmatchings/2DD40", func_8002ED30_2F930);

void func_8002ED40_2F940(s32 a0) {
    s32 new_var = 0x98967F;

    D_800AFE8C_A71FC->padding += a0;

    if (D_800AFE8C_A71FC->padding > new_var) {
        D_800AFE8C_A71FC->padding = new_var;
    }

    if (D_800AFE8C_A71FC->padding < 0) {
        D_800AFE8C_A71FC->padding = 0;
    }
}