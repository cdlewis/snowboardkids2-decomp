#include "common.h"
#include "gamestate.h"
#include "graphics.h"

INCLUDE_ASM("asm/nonmatchings/594E0", func_800588E0_594E0);

void func_80058924_59524(Player *arg0) {
    if (arg0->unkAC2 < 3) {
        if (!(arg0->unkB88 & 0x77F)) {
            arg0->unkAC2 = 3;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/594E0", func_80058950_59550);

void func_800589A0_595A0(Player *arg0) {
    if (arg0->unkAC2 < 0x32) {
        if (!(arg0->unkB88 & 0x71F)) {
            arg0->unkAC2 = 0x32;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/594E0", func_800589CC_595CC);

void func_80058A10_59610(Player *arg0) {
    if (arg0->unkAC2 < 0x33) {
        if (!(arg0->unkB88 & 0x59F)) {
            arg0->unkAC2 = 0x33;
        }
    }
}

void func_80058A3C_5963C(Player *arg0) {
    if (arg0->unkAC2 < 0x34) {
        if (!(arg0->unkB88 & 0x4BC)) {
            arg0->unkAC2 = 0x34;
        }
    }
}

void func_80058A68_59668(Player *arg0) {
    if (arg0->unkAC2 < 0x35) {
        if (!(arg0->unkB88 & 0xD8)) {
            arg0->unkAC2 = 0x35;
        }
    }
}

void func_80058A94_59694(Player *arg0) {
    if (arg0->unkAC2 < 0x36) {
        if (!(arg0->unkB88 & 0x49C)) {
            arg0->unkAC2 = 0x36;
        }
    }
}

void func_80058AC0_596C0(Player *arg0) {
    if (arg0->unkAC2 < 0x3C) {
        if (!(arg0->unkB88 & 0x9C)) {
            arg0->unkAC2 = 0x3C;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/594E0", func_80058AEC_596EC);

INCLUDE_ASM("asm/nonmatchings/594E0", func_80058B30_59730);

void func_80058B68_59768(Player *arg0) {
    if (arg0->unkAC2 < 0x50) {
        if (!(arg0->unkB88 & 0x1C)) {
            arg0->unkAC2 = 0x50;
        }
    }
}

void func_80058B94_59794(Player *arg0) {
    if (!(arg0->unkB88 & 8)) {
        arg0->unkAC2 = 100;
    }
}

INCLUDE_ASM("asm/nonmatchings/594E0", func_80058BB0_597B0);

INCLUDE_ASM("asm/nonmatchings/594E0", func_80058C10_59810);

INCLUDE_ASM("asm/nonmatchings/594E0", func_80058C64_59864);

s32 func_80058CC4_598C4(Player *arg0) {
    if (arg0->unkBA4 == 0) {
        arg0->unkBA4 = 0x96;
        func_80056B7C_5777C(&arg0->worldPosX, 0x48);
        return 1;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/594E0", func_80058CFC_598FC);

INCLUDE_ASM("asm/nonmatchings/594E0", func_80059394_59F94);

INCLUDE_ASM("asm/nonmatchings/594E0", func_800597C0_5A3C0);

INCLUDE_ASM("asm/nonmatchings/594E0", func_80059A48_5A648);

INCLUDE_ASM("asm/nonmatchings/594E0", func_80059A88_5A688);

s32 func_80059AC4_5A6C4(Player *arg0) {
    if (!arg0->unkBC6) {
        return arg0->unkB6C;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/594E0", func_80059ADC_5A6DC);

INCLUDE_ASM("asm/nonmatchings/594E0", func_80059BD4_5A7D4);

INCLUDE_ASM("asm/nonmatchings/594E0", func_80059C24_5A824);

INCLUDE_ASM("asm/nonmatchings/594E0", func_80059C90_5A890);

INCLUDE_ASM("asm/nonmatchings/594E0", func_80059CE0_5A8E0);

INCLUDE_ASM("asm/nonmatchings/594E0", func_80059D30_5A930);

INCLUDE_ASM("asm/nonmatchings/594E0", func_80059DB8_5A9B8);

INCLUDE_ASM("asm/nonmatchings/594E0", func_80059DFC_5A9FC);

INCLUDE_ASM("asm/nonmatchings/594E0", func_80059E40_5AA40);
