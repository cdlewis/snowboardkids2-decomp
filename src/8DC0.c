#include "common.h"

extern void setCleanupCallback(void *);
extern void setCallback(void *);

void func_800081EC_8DEC(void);
void func_80008308_8F08(void);

void func_800081C0_8DC0(void) {
    setCleanupCallback(func_80008308_8F08);
    setCallback(func_800081EC_8DEC);
}

INCLUDE_ASM("asm/nonmatchings/8DC0", func_800081EC_8DEC);

void func_80008308_8F08(void) {
}

INCLUDE_ASM("asm/nonmatchings/8DC0", func_80008310_8F10);

INCLUDE_ASM("asm/nonmatchings/8DC0", func_80008370_8F70);

INCLUDE_ASM("asm/nonmatchings/8DC0", func_800084A8_90A8);
