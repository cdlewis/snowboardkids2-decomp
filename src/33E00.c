#include "common.h"

extern u8 *allocateTaskMemory(s32 size);
extern void setGameStateHandler(void *handler);
extern void func_80033254_33E54(void);

extern u8 D_800AFCE2_A7052;
extern u8 D_800AFED0_A7240;

void func_80033200_33E00(void) {
    u8 *ptr;
    s32 i;

    ptr = allocateTaskMemory(4);
    i = 3;
    ptr += 3;
    D_800AFCE2_A7052 = 1;
    D_800AFED0_A7240 = 0;
    for (; i >= 0; i--) {
        *ptr-- = 0;
    }
    setGameStateHandler(func_80033254_33E54);
}

INCLUDE_ASM("asm/nonmatchings/33E00", func_80033254_33E54);

extern void func_800697F4_6A3F4(s32 arg0);

void func_800333BC_33FBC(void) {
    func_800697F4_6A3F4(1);
}
