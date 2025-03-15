#include "common.h"

void func_80019368_19F68();

extern void func_80069CC0_6A8C0(void*);

char D_8008D7C0_8E3C0[] = "PHONE";

typedef struct {
    s8 unk0;
    s8 unk1;
    char padding[2];
    s16 unk4;
    s16 unk6;
    s16 unk8;
    void* unkC;
} func_80019320_19F20_arg;

void func_80019320_19F20(func_80019320_19F20_arg* arg0) {
    arg0->unk0 = 6;
    arg0->unk6 = -0x68;
    arg0->unk1 = 0;
    arg0->unk4 = 0;
    arg0->unk8 = 0;
    arg0->unkC = &D_8008D7C0_8E3C0;
    func_80069CC0_6A8C0(&func_80019368_19F68);
}

INCLUDE_ASM("asm/nonmatchings/19F20", func_80019368_19F68);
