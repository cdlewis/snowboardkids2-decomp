#include "common.h"
#include "task_scheduler.h"

extern void *D_8008EBF0_8F7F0[];

typedef struct {
    u8 pad[0xD4];
    u8 unkD4;
} func_8002BE44_arg;

INCLUDE_ASM("asm/nonmatchings/2C8F0", func_8002BCF0_2C8F0);

void func_8002BE44_2CA44(func_8002BE44_arg *arg0) {
    GameState *allocation = (GameState *)getCurrentAllocation();

    if (allocation->unk429 == 0) {
        setCallback(D_8008EBF0_8F7F0[arg0->unkD4]);
    }
}

void func_8002BE8C_2CA8C(void) {
}

INCLUDE_ASM("asm/nonmatchings/2C8F0", func_8002BE94_2CA94);

INCLUDE_ASM("asm/nonmatchings/2C8F0", func_8002BEF4_2CAF4);

INCLUDE_ASM("asm/nonmatchings/2C8F0", func_8002BFEC_2CBEC);

INCLUDE_ASM("asm/nonmatchings/2C8F0", func_8002C570_2D170);

INCLUDE_ASM("asm/nonmatchings/2C8F0", func_8002C798_2D398);
