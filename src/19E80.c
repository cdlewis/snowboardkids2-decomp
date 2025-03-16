#include "common.h"

void func_80019280_19E80(void *arg0, u16 arg1, void *arg2) {
    u32 v0;
    int new_var3;
    u16 temp;
    void *base;
    void *ptr1;
    void *ptr2;
    u16 *new_var;
    void *new_var2;

    ptr2 = arg0 + 8;
    ptr1 = base + ((*((u32 *)(arg0 + 4))) << 4);
    base = ptr2;
    ptr1 = base + ((*((u32 *)(arg0 + 4))) << 4);
    arg1 = (new_var3 = arg1 << 4);
    arg1 = arg1 & 0xFFFF;
    ptr2 = base + new_var3;
    v0 = *((u32 *)ptr2);
    *((void **)arg2) = arg0 + v0;
    new_var = &(*((u16 *)(ptr2 + 6)));
    new_var2 = arg2;
    temp = *((u16 *)(ptr2 + 4));
    *((void **)(4 + arg2)) = ptr1 + (temp << 5);
    *((u16 *)(new_var2 + 8)) = *new_var;
    *((u16 *)(new_var2 + 10)) = *((u16 *)(ptr2 + 8));
}

INCLUDE_ASM("asm/nonmatchings/19E80", func_800192CC_19ECC);
