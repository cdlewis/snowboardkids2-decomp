#include "common.h"

typedef struct {
    u8 padding[0xFF5];
    s8 unkFF5;
} func_80002FD4_3BD4_inner;

typedef struct {
    func_80002FD4_3BD4_inner *unk0;
    void *unk4;
} func_80002FD4_3BD4_arg;

extern void func_80000760_1360(void *);

INCLUDE_ASM("asm/nonmatchings/3B80", func_80002F80_3B80);

void func_80002FD4_3BD4(func_80002FD4_3BD4_arg *arg0) {
    if (arg0->unk0->unkFF5 != 0) {
        func_80000760_1360(arg0->unk4);
    }
}

INCLUDE_ASM("asm/nonmatchings/3B80", func_80003000_3C00);

INCLUDE_ASM("asm/nonmatchings/3B80", func_800030B4_3CB4);

INCLUDE_ASM("asm/nonmatchings/3B80", func_80003184_3D84);

INCLUDE_ASM("asm/nonmatchings/3B80", func_800033AC_3FAC);
