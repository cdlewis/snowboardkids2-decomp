#include "common.h"

extern void func_80009F5C_AB5C(s32 *);
extern void func_80069CF8_6A8F8(void);

typedef struct {
    s32 unk0;
    s32 unk4;
} func_80007130_7D30_arg;

typedef struct {
    u8 _pad[0x3C];
    s8 unk3C;
} func_80007100_7D00_inner;

typedef struct {
    func_80007100_7D00_inner *unk0;
} func_80007100_7D00_arg;

INCLUDE_ASM("asm/nonmatchings/7C70", func_80007070_7C70);

void func_80007100_7D00(func_80007100_7D00_arg *arg0) {
    if (arg0->unk0->unk3C == 1) {
        func_80069CF8_6A8F8();
    }
}

void func_80007130_7D30(func_80007130_7D30_arg *arg0) {
    func_80009F5C_AB5C(&arg0->unk4);
}

INCLUDE_ASM("asm/nonmatchings/7C70", func_8000714C_7D4C);

INCLUDE_ASM("asm/nonmatchings/7C70", func_800071E4_7DE4);

void func_80007360_7F60(func_80007130_7D30_arg *arg0) {
    func_80009F5C_AB5C(&arg0->unk4);
}
