#include "common.h"

extern void setCallback(void *);
extern void func_80018D04_19904(void);
extern u8 D_8008D714_8E314[];

void func_80018CB0_198B0(u8 *arg0) {
    u8 temp = arg0[0];
    arg0[1] = 0;
    *(s16 *)(arg0 + 4) = 0;
    *(s16 *)(arg0 + 6) = -0x68;
    *(s16 *)(arg0 + 8) = 0;
    *(u8 **)(arg0 + 0xC) = &D_8008D714_8E314[temp * 20];
    setCallback(func_80018D04_19904);
}

INCLUDE_ASM("asm/nonmatchings/198B0", func_80018D04_19904);
