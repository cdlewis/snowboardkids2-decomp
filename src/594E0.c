#include "3E160.h"
#include "5DBC0.h"
#include "common.h"
#include "gamestate.h"
#include "graphics.h"
#include "rand.h"

extern s32 func_80044050_44C50(void);
extern u16 D_80093F90_94B90[];
extern u16 D_80093FA4_94BA4[];
extern u16 D_80093FB8_94BB8[];
extern u16 D_80093FCC_94BCC[];
extern u16 D_80093FE0_94BE0[];
extern u16 D_80093FF4_94BF4[];
extern u16 D_80094008_94C08[];
extern u16 D_8009401C_94C1C[];
extern u16 D_80094030_94C30[];
extern u16 D_80094044_94C44[];
extern u16 D_80094058_94C58[];

void func_800588E0_594E0(Player *arg0, s16 arg1, void *arg2) {
    if (arg0->unkAC2 != 0) {
        return;
    }
    if (arg0->unkB88 & ~0x80) {
        return;
    }
    arg0->unkAC2 = arg1;
    memcpy(&arg0->unkAC8, arg2, 0xC);
}

void func_80058924_59524(Player *arg0) {
    if (arg0->unkAC2 < 3) {
        if (!(arg0->unkB88 & 0x77F)) {
            arg0->unkAC2 = 3;
        }
    }
}

void func_80058950_59550(Player *arg0, s16 arg1, s32 arg2) {
    if (arg0->unkAC2 < 4) {
        if ((arg0->unkB88 & 0x77F) == 0) {
            arg0->unkAC2 = 4;
            arg0->unkAC4 = arg1;
            arg0->unkAC8 = 0;
            arg0->unkACC = 0;
            if (arg2 > 0x60000) {
                arg2 = 0x60000;
            }
            arg0->unkAD0 = -arg2;
        }
    }
}

void func_800589A0_595A0(Player *arg0) {
    if (arg0->unkAC2 < 0x32) {
        if (!(arg0->unkB88 & 0x71F)) {
            arg0->unkAC2 = 0x32;
        }
    }
}

void func_800589CC_595CC(Player *arg0, void *arg1) {
    if (arg0->unkAC2 < 0x31) {
        if (!(arg0->unkB88 & 0x71F)) {
            arg0->unkAC2 = 0x31;
            memcpy(&arg0->unkAC8, arg1, 0xC);
        }
    }
}

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

void func_80058AEC_596EC(Player *arg0, void *arg1) {
    if (arg0->unkAC2 < 0x3C) {
        if (!(arg0->unkB88 & 0x218)) {
            arg0->unkAC2 = 0x3D;
            memcpy(&arg0->unkAC8, arg1, 0xC);
        }
    }
}

void func_80058B30_59730(Player *arg0) {
    if (arg0->unkAC2 < 0x3C) {
        if (arg0->unkBA4 == 0) {
            if (!(arg0->unkB88 & 0x18)) {
                arg0->unkAC2 = 0x3E;
            }
        }
    }
}

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

s32 func_80058BB0_597B0(Player *arg0) {
    if (arg0->unkBD0 == 0) {
        if (func_800436B4_442B4(arg0) != 0) {
            if (arg0->unkB84 & 2) {
                arg0->unkBD0 = 4;
            } else {
                arg0->unkBD0 = 3;
            }
            arg0->unkB9A = 0x5A;
            return 1;
        }
    }
    return 0;
}

s32 func_80058C10_59810(Player *arg0) {
    if (arg0->unkBD1 != 0) {
        return 0;
    }
    if (func_80044050_44C50() == 0) {
        return 0;
    }
    arg0->unkBD1 = 1;
    arg0->unkBA6 = 300;
    return 1;
}

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

void func_80059A48_5A648(Player *arg0, s32 arg1) {
    if (arg0->unkBC6 != 0) {
        return;
    }

    arg0->unkB6C += arg1;

    if (arg0->unkB6C > 99999) {
        arg0->unkB6C = 99999;
    }

    if (arg0->unkB6C < 0) {
        arg0->unkB6C = 0;
    }
}

void func_80059A88_5A688(Player *arg0, s32 arg1) {
    if (arg0->unkBC6 != 0) {
        return;
    }
    arg0->unkB70 += arg1;
    if (arg0->unkB70 >= 10000) {
        arg0->unkB70 = 9999;
    }
    if (arg0->unkB70 < 0) {
        arg0->unkB70 = 0;
    }
}

s32 func_80059AC4_5A6C4(Player *arg0) {
    if (!arg0->unkBC6) {
        return arg0->unkB6C;
    }
    return 0;
}

void func_80059ADC_5A6DC(Player *arg0) {
    if (arg0->unkBB9 < 9) {
        switch (randA() % 3) {
            case 0:
                func_80056D64_57964(&arg0->worldPosX, D_80093F90_94B90[arg0->unkBB9], 5, arg0->unkBB8 + 4);
                break;
            case 1:
                func_80056D64_57964(&arg0->worldPosX, D_80093FA4_94BA4[arg0->unkBB9], 5, arg0->unkBB8 + 4);
                break;
            case 2:
                func_80056D64_57964(&arg0->worldPosX, D_80093FB8_94BB8[arg0->unkBB9], 5, arg0->unkBB8 + 4);
                break;
        }
    }
}

void func_80059BD4_5A7D4(Player *arg0) {
    u8 index = arg0->unkBB9;
    if (index < 9) {
        if (arg0->unkB88 == 0) {
            func_80056D64_57964(&arg0->worldPosX, D_80093FCC_94BCC[index], 5, arg0->unkBB8 + 4);
        }
    }
}

void func_80059C24_5A824(Player *arg0) {
    u8 index = arg0->unkBB9;
    if (index < 9) {
        if (arg0->unkB88 == 0) {
            func_80056D64_57964(&arg0->worldPosX, D_80093FE0_94BE0[index], 5, arg0->unkBB8 + 4);
            func_8005D804_5E404(arg0, 1, 0xF);
        }
    }
}

void func_80059C90_5A890(Player *arg0) {
    u8 index = arg0->unkBB9;
    if (index < 9) {
        if (arg0->unkB88 == 0) {
            func_80056D64_57964(&arg0->worldPosX, D_80093FF4_94BF4[index], 5, arg0->unkBB8 + 4);
        }
    }
}

void func_80059CE0_5A8E0(Player *arg0) {
    u8 index = arg0->unkBB9;
    if (index < 9) {
        if (arg0->unkB88 == 0) {
            func_80056D64_57964(&arg0->worldPosX, D_80094008_94C08[index], 5, arg0->unkBB8 + 4);
        }
    }
}

void func_80059D30_5A930(Player *arg0) {
    if (arg0->unkBB9 < 9) {
        if (randA() & 1) {
            func_80056D64_57964(&arg0->worldPosX, D_8009401C_94C1C[arg0->unkBB9], 5, arg0->unkBB8 + 4);
        } else {
            func_80056D64_57964(&arg0->worldPosX, D_80094030_94C30[arg0->unkBB9], 5, arg0->unkBB8 + 4);
        }
    }
}

void func_80059DB8_5A9B8(Player *arg0) {
    u8 index = arg0->unkBB9;
    if (index < 9) {
        func_80056D64_57964(&arg0->worldPosX, D_80094044_94C44[index], 5, arg0->unkBB8 + 4);
    }
}

void func_80059DFC_5A9FC(Player *arg0) {
    u8 index = arg0->unkBB9;
    if (index < 9) {
        func_80056D64_57964(&arg0->worldPosX, D_80094058_94C58[index], 5, arg0->unkBB8 + 4);
    }
}

extern u16 D_8009406C_94C6C[];

void func_80059E40_5AA40(Player *arg0) {
    u8 index = arg0->unkBB9;
    if (index < 9) {
        func_80056D64_57964(&arg0->worldPosX, D_8009406C_94C6C[index], 5, arg0->unkBB8 + 4);
    }
}
