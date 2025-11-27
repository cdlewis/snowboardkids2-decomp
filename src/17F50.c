#include "common.h"
#include "task_scheduler.h"

void func_80017384_17F84(void);

void func_80017350_17F50(void) {
    GameState *state = (GameState *)getCurrentAllocation();

    if (state->unk429 == 0) {
        setCallback(&func_80017384_17F84);
    }
}

INCLUDE_ASM("asm/nonmatchings/17F50", func_80017384_17F84);

INCLUDE_ASM("asm/nonmatchings/17F50", func_800175E0_181E0);

INCLUDE_ASM("asm/nonmatchings/17F50", func_80017FE8_18BE8);

INCLUDE_ASM("asm/nonmatchings/17F50", func_80018148_18D48);

extern u8 D_800A8CC8_A0038;

void func_800182F4_18EF4(void);

void func_8001829C_18E9C(void) {
    GameState *state = (GameState *)getCurrentAllocation();

    if (func_8006FE10_70A10(0) == 0) {
        D_800A8CC8_A0038 = state->unk425;
        state->unk427 = state->unk425 + 1;
        setCallback(&func_800182F4_18EF4);
    }
}

void func_800182F4_18EF4(void) {
}

INCLUDE_ASM("asm/nonmatchings/17F50", func_800182FC_18EFC);

INCLUDE_ASM("asm/nonmatchings/17F50", func_80018474_19074);

INCLUDE_ASM("asm/nonmatchings/17F50", func_80018580_19180);

void func_800187DC_193DC(void);

void func_80018798_19398(void) {
    GameState *state = (GameState *)getCurrentAllocation();

    if (func_8006FE10_70A10(0) == 0) {
        state->unk427 = 0xFF;
        setCallbackWithContinue(&func_800187DC_193DC);
    }
}

void func_800187DC_193DC(void) {
}

INCLUDE_ASM("asm/nonmatchings/17F50", abs);
