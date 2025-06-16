#include "5E590.h"
#include "69EF0.h"
#include "common.h"
#include "gamestate.h"

extern void func_8004083C_4143C();
extern void func_80040948_41548();

typedef struct {
    void* unk0;
    void* unk4;
} func_800407E0_413E0_arg;

void func_800407E0_413E0(func_800407E0_413E0_arg* arg0) {
    GameState* temp_s0 = GameStateGet();
    arg0->unk0 = load_3ECE40();
    arg0->unk4 = (u8*)(temp_s0->unk44 + 0x1380);
    setCleanupCallback(&func_80040948_41548);
    setCallbackWithContinue(&func_8004083C_4143C);
}

INCLUDE_ASM("asm/nonmatchings/413E0", func_8004083C_4143C);

INCLUDE_ASM("asm/nonmatchings/413E0", func_80040870_41470);

INCLUDE_ASM("asm/nonmatchings/413E0", func_80040948_41548);

void func_80040974_41574(void* arg0) {
    Node* temp_v0 = scheduleTask(&func_800407E0_413E0, 0U, 0U, 0xF0U);
    if (temp_v0 != NULL) {
        temp_v0->cleanupCallback = arg0;
    }
}

INCLUDE_ASM("asm/nonmatchings/413E0", func_800409B4_415B4);

INCLUDE_ASM("asm/nonmatchings/413E0", func_80040B4C_4174C);

INCLUDE_ASM("asm/nonmatchings/413E0", func_80040D48_41948);

INCLUDE_ASM("asm/nonmatchings/413E0", func_80040D80_41980);

INCLUDE_ASM("asm/nonmatchings/413E0", func_80040E00_41A00);

INCLUDE_ASM("asm/nonmatchings/413E0", func_80040E4C_41A4C);

INCLUDE_ASM("asm/nonmatchings/413E0", func_80040F34_41B34);

INCLUDE_ASM("asm/nonmatchings/413E0", func_80040F6C_41B6C);

INCLUDE_ASM("asm/nonmatchings/413E0", func_80041010_41C10);

INCLUDE_ASM("asm/nonmatchings/413E0", func_8004106C_41C6C);

INCLUDE_ASM("asm/nonmatchings/413E0", func_8004119C_41D9C);

INCLUDE_ASM("asm/nonmatchings/413E0", func_800413E0_41FE0);

INCLUDE_ASM("asm/nonmatchings/413E0", func_80041418_42018);

INCLUDE_ASM("asm/nonmatchings/413E0", func_80041518_42118);
