#include "common.h"

extern void *freeNodeMemory(void *);

INCLUDE_ASM("asm/nonmatchings/DA20", func_8000CE20_DA20);

INCLUDE_ASM("asm/nonmatchings/DA20", func_8000CEA4_DAA4);

void func_8000CFA0_DBA0(void **arg0) {
    *arg0 = freeNodeMemory(*arg0);
}

INCLUDE_ASM("asm/nonmatchings/DA20", func_8000CFCC_DBCC);
