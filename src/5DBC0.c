#include "common.h"
#include "gamestate.h"
#include "rand.h"

extern s32 func_80061A64_62664(void *, u16, void *);
extern s32 func_80061D6C_6296C(void *, u16, void *, s32);
extern void func_80062918_63518(void *, u16, void *, void *, void *);

s32 func_8005CFC0_5DBC0(void *arg0, u16 arg1, void *arg2, s32 arg3) {
    return func_80061A64_62664(arg0, arg1, arg2);
}

s32 func_8005CFDC_5DBDC(void *arg0, u16 arg1, void *arg2, s32 arg3, s32 arg4) {
    return func_80061D6C_6296C(arg0, arg1, arg2, arg4);
}

void func_8005CFFC_5DBFC(void *arg0, u16 arg1, void *arg2, void *arg3, void *arg4) {
    func_80062918_63518(arg0, arg1, arg2, arg3, arg4);
}

extern s32 func_80062274_62E74(void *, u16);

s32 func_8005D020_5DC20(void *arg0, u16 arg1) {
    return func_80062274_62E74(arg0, arg1);
}

INCLUDE_ASM("asm/nonmatchings/5DBC0", func_8005D03C_5DC3C);

INCLUDE_ASM("asm/nonmatchings/5DBC0", func_8005D180_5DD80);

INCLUDE_ASM("asm/nonmatchings/5DBC0", func_8005D308_5DF08);

INCLUDE_ASM("asm/nonmatchings/5DBC0", func_8005D48C_5E08C);

INCLUDE_ASM("asm/nonmatchings/5DBC0", func_8005D61C_5E21C);

void func_8005D804_5E404(Player *arg0, u8 arg1, u8 arg2) {
    arg0->unkBE6 = arg1;
    arg0->unkBE7 = arg2;
}

void func_8005D810_5E410(void) {
}

INCLUDE_ASM("asm/nonmatchings/5DBC0", func_8005D818_5E418);

s16 func_8005D8C8_5E4C8(Player *arg0) {
    s32 count;
    u32 temp;
    s8 temp_b7a;
    s8 temp_b7b;
    s16 temp_b7e;

    count = 1;

    if (arg0->unkBDA != 0) {
        temp = randA();
        temp = temp & 3;
        temp = temp < 1;
        count = temp + 1;
        goto end;
    }

    temp_b7a = arg0->unkB7A;
    if (temp_b7a == 7) {
        temp = arg0->unkB80;
        temp = temp ^ 7;
        temp = temp != 0;
        count = temp + 1;
    }

    if (temp_b7a == -7) {
        temp = arg0->unkB80;
        temp = temp ^ temp_b7a;
        temp = temp != 0;
        count += temp;
    }

    temp_b7b = arg0->unkB7B;
    if (temp_b7b == 7) {
        temp = arg0->unkB81;
        temp = temp ^ 7;
        temp = temp != 0;
        count += temp;
    }

    if (temp_b7b == -7) {
        temp = arg0->unkB81;
        temp = temp ^ temp_b7b;
        temp = temp != 0;
        count += temp;
    }

    temp_b7e = arg0->unkB7E;
    temp = temp_b7e & 0xC000;
    temp = temp != 0;
    count += temp;

end:
    return count;
}
