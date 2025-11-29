#include "56910.h"
#include "5E590.h"
#include "common.h"
#include "gamestate.h"
#include "graphics.h"
#include "task_scheduler.h"

typedef struct {
    void *unk0;
    void *unk4;
    void *unk8;
    void *unkC;
    void *unk10;
    void *unk14;
    void *unk18;
    void *unk1C;
    void *unk20;
    void *unk24;
    u8 _pad28[0x4];
    void *unk2C;
    u8 _pad30[0xB89];
    u8 unkBB9;
    u8 unkBBA;
    u8 unkBBB;
    u8 unkBBC;
    u8 _padBBD[0xA];
    u8 unkBC7;
    u8 _padBC8[0x1B];
    u8 unkBE3;
    u8 unkBE4;
} func_800B9AE0_arg;

extern s32 func_800B2C18_A2AC8(void *);
extern void func_800B3784_A3634(void *);
extern void func_800B2B3C_A29EC(void *);
extern void *func_8005DE04_5EA04(s16 arg0, s16 arg1);
extern void *func_8005DE30_5EA30(s16 arg0, s16 arg1);
extern void *func_8005DC60_5E860(s32 arg0);
extern void *loadAssetByIndex_5E990(s32 arg0);

typedef void (*func_800B3950_callback)(void *);
extern func_800B3950_callback D_800BAB68_AAA18[];

typedef void (*func_800B468C_callback)(void *);
extern func_800B468C_callback D_800BAB78_AAA28[];

typedef void (*func_800B23AC_callback)(void *);
extern func_800B23AC_callback D_800BAB04_AA9B4[];

typedef void (*func_800B65C8_callback)(void *);
extern func_800B65C8_callback D_800BAC10_AAAC0[];

typedef void (*func_800B6610_callback)(void *);
extern func_800B6610_callback D_800BAC14_AAAC4[];

typedef void (*func_800B1FCC_callback)(void *);
extern func_800B1FCC_callback D_800BAAF4_AA9A4[];

typedef void (*func_800B12C0_callback)(void *);
extern func_800B12C0_callback D_800BAAD4_AA984[];

typedef struct {
    u8 _pad0[0x5C];
    u8 unk5C;
} func_800B99E0_alloc;

typedef struct {
    u8 _pad0[0xB84];
    s32 unkB84;
    u8 _padB88[0x12]; // 0xB88 to 0xB9A
    s16 unkB9A;
    u8 _padB9C[0x8]; // 0xB9C to 0xBA4
    s16 unkBA4;
    s16 unkBA6;
    u8 _padBA8[0x15]; // 0xBA8 to 0xBBD
    u8 unkBBD;
    u8 unkBBE;
    u8 unkBBF;
    u8 unkBC0;
} func_800B00D4_arg;

void func_800B00C0_9FF70(func_800B00D4_arg *arg0, u8 arg1) {
    arg0->unkBBD = arg1;
    arg0->unkBBE = 0;
    arg0->unkBBF = 0;
    arg0->unkBC0 = 0;
}

void func_800B00D4_9FF84(func_800B00D4_arg *arg0, s32 arg1) {
    arg0->unkBBE = arg1;
    arg0->unkBBF = 0;
    arg0->unkBC0 = 0;
}

void func_800B00E4_9FF94(void *arg) {
    func_800B00D4_arg *arg0 = arg;
    arg0->unkBBD = 1;
    arg0->unkBBE = 0;
    arg0->unkBBF = 0;
    arg0->unkBC0 = 0;
}

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B00FC_9FFAC);

typedef struct {
    u8 _pad0[0x434];
    s32 unk434;
    s32 unk438;
    s32 unk43C;
    u8 _pad440[0xC];
    s32 unk44C;
    s32 unk450;
    s32 unk454;
} func_800B0300_arg;

extern void func_8005D03C_5DC3C(void *);

void func_800B02AC_A015C(void *arg) {
    func_800B0300_arg *arg0 = arg;
    func_8005D03C_5DC3C(arg0);
    arg0->unk434 = arg0->unk434 + arg0->unk44C;
    arg0->unk438 = arg0->unk438 + arg0->unk450;
    arg0->unk43C = arg0->unk43C + arg0->unk454;
}

void func_800B0300_A01B0(func_800B0300_arg *arg0) {
    arg0->unk434 = arg0->unk434 + arg0->unk44C;
    arg0->unk438 = arg0->unk438 + arg0->unk450;
    arg0->unk43C = arg0->unk43C + arg0->unk454;
}

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B0334_A01E4);

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B05B8_A0468);

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B0F14_A0DC4);

void func_800B12C0_A1170(func_800B00D4_arg *arg0) {
    D_800BAAD4_AA984[arg0->unkBBE](arg0);
}

typedef struct {
    u8 _pad0[0x44C];
    s32 unk44C;
    s32 unk450;
    s32 unk454;
    u8 _pad458[0x654];
    s32 unkAAC;
    u8 _padAB0[0x8];
    s32 unkAB8;
    u8 _padABC[0xC2];
    u16 unkB7E;
    u8 _padB80[0xC];
    s32 unkB8C;
    u8 _padB90[0x2F];
    u8 unkBBF;
} func_800B12F0_arg;

extern void func_80058BB0_597B0(func_800B12F0_arg *);
extern void func_8005D180_5DD80(void *, s32);

s32 func_800B12F0_A11A0(func_800B12F0_arg *arg0) {
    GameState *alloc = getCurrentAllocation();

    if (alloc->unk79 == 0) {
        arg0->unkAAC = 0;
        if (arg0->unkBBF != 0) {
            if (arg0->unkB8C < 9) {
                func_80058BB0_597B0(arg0);
            }
        }
        func_800B00E4_9FF94(arg0);
        return 1;
    }

    if (alloc->unk7A < 10) {
        if (arg0->unkBBF == 0) {
            if (arg0->unkB7E & 0x4000) {
                arg0->unkB8C = 0;
                arg0->unkBBF = arg0->unkBBF + 1;
            }
        } else {
            arg0->unkB8C = arg0->unkB8C + 1;
        }
    }

    arg0->unk44C = 0;
    arg0->unk454 = 0;
    arg0->unk450 = arg0->unk450 - arg0->unkAB8;
    func_800B02AC_A015C(arg0);
    func_8005D180_5DD80(arg0, 0);
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B13D4_A1284);

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B1544_A13F4);

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B1DA0_A1C50);

typedef struct {
    u8 _pad0[0xB84];
    s32 unkB84;
    u8 _padB88[0x10]; // 0xB88 to 0xB98
    s16 unkB98;
    u8 _padB9A[0x28]; // 0xB9A to 0xBC2
    u8 unkBC2;
    u8 _padBC3[0x17]; // 0xBC3 to 0xBDA
    u8 unkBDA;
} func_800B1F60_arg;

s32 func_800B1F60_A1E10(func_800B1F60_arg *arg0, s32 arg1) {
    if (arg0->unkBDA != 0) {
        goto end;
    }
    if (arg0->unkB98 == 0) {
        goto end;
    }
    if (arg0->unkBC2 != 0) {
        goto end;
    }
    if (0x8000 < arg1) {
        if (arg0->unkB84 & 2) {
            return 1;
        }
    }
    if (arg1 < -0x8000) {
        if (arg0->unkB84 & 2) {
            goto end;
        }
        return 1;
    }
end:
    return 0;
}

void func_800B1FCC_A1E7C(func_800B00D4_arg *arg0) {
    D_800BAAF4_AA9A4[arg0->unkBBF](arg0);
}

typedef struct {
    u8 _pad0[0xB8C];
    s32 unkB8C;
    u8 _padB90[0x2F];
    u8 unkBBF;
} func_800B1FFC_arg;

s32 func_800B1FFC_A1EAC(func_800B1FFC_arg *arg0) {
    arg0->unkB8C = 2;
    arg0->unkBBF = arg0->unkBBF + 1;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B2018_A1EC8);

typedef struct {
    u8 _pad0[0xB84];
    s32 unkB84;
    u8 _padB88[0x37];
    u8 unkBBF;
} func_800B214C_arg;

s32 func_800B214C_A1FFC(func_800B214C_arg *arg0) {
    s32 temp;

    arg0->unkBBF = arg0->unkBBF + 1;
    temp = arg0->unkB84;

    if (temp & 2) {
        arg0->unkB84 = temp & ~2;
    } else {
        arg0->unkB84 = temp | 2;
    }

    return 1;
}

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B217C_A202C);

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B2254_A2104);

void func_800B23AC_A225C(func_800B00D4_arg *arg0) {
    D_800BAB04_AA9B4[arg0->unkBBF](arg0);
}

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B23DC_A228C);

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B243C_A22EC);

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B255C_A240C);

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B26D0_A2580);

typedef struct {
    u8 _pad0[0xBAA];
    s16 unkBAA;
    s16 unkBAC;
    s16 unkBAE;
    u8 _padBB0[0x5];
    u8 unkBB5;
    u8 unkBB6;
    u8 _padBB7[0x1E];
    u8 unkBD5;
} func_800B27FC_arg;

void func_800B27FC_A26AC(func_800B27FC_arg *arg0) {
    arg0->unkBAC = 0;
    arg0->unkBAA = 0;
    arg0->unkBAE = 0;
    arg0->unkBB5 = 0;
    arg0->unkBD5 = 0;
    arg0->unkBB6 = 0;
}

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B2818_A26C8);

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B2950_A2800);

extern void func_80059ADC_5A6DC(void *);
extern void func_800B40D4_A3F84(void *);
extern void func_800B42A8_A4158(void *, s32, s32, s32);
extern void func_800B00FC_9FFAC(void *);
extern s32 func_8005D308_5DF08(void *, s32);
extern void func_80058CFC_598FC(void *);

typedef struct {
    u8 _pad0[0x450];
    s32 unk450;
    u8 _pad454[0x65C];
    s32 unkAB0;
    u8 _padAB4[0x4];
    s32 unkAB8;
    u8 _padABC[0xC8];
    s32 unkB84;
    u8 _padB88[0x37];
    u8 unkBBF;
    u8 unkBC0;
    u8 _padBC1[0xC];
    s8 unkBCD;
} func_800B2A3C_arg;

s32 func_800B2A3C_A28EC(func_800B2A3C_arg *arg0) {
    s32 pad[16];

    if (arg0->unkB84 & 1) {
        if (arg0->unkBC0 < 3) {
            func_800B27FC_A26AC((func_800B27FC_arg *)arg0);
            if (arg0->unkBCD < 0) {
                arg0->unkBC0 = 0;
                arg0->unkBBF = arg0->unkBBF + 1;
                return 1;
            } else {
                arg0->unkBBF = arg0->unkBCD + 6;
                arg0->unkBC0 = 0;
                func_80059ADC_5A6DC(arg0);
                return 1;
            }
        } else {
            func_800B00D4_9FF84((func_800B00D4_arg *)arg0, 1);
            return 1;
        }
    } else {
        arg0->unk450 = arg0->unk450 - arg0->unkAB8;
        func_800B40D4_A3F84(arg0);
        func_800B42A8_A4158(arg0, 0x200, 0x200, arg0->unkAB0);
        func_800B00FC_9FFAC(arg0);
        arg0->unkBC0 = arg0->unkBC0 - 1;
        if (arg0->unkBC0 == 0) {
            func_800B00D4_9FF84((func_800B00D4_arg *)arg0, 0);
        }
        func_8005D308_5DF08(arg0, 3);
        func_80058CFC_598FC(arg0);
        return 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B2B3C_A29EC);

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B2C18_A2AC8);

extern void func_800B419C_A404C(void *);

typedef struct {
    u8 _pad0[0x44C];
    s32 unk44C;
    s32 unk450;
    s32 unk454;
    u8 _pad458[0x660];
    s32 unkAB8;
} func_800B2DDC_arg;

void func_800B2DDC_A2C8C(void *arg) {
    func_800B2DDC_arg *arg0 = arg;
    arg0->unk44C = arg0->unk44C - (arg0->unk44C >> 7);
    arg0->unk450 = arg0->unk450 - arg0->unkAB8;
    arg0->unk454 = arg0->unk454 - (arg0->unk454 >> 7);
    func_800B419C_A404C(arg0);
    func_800B00FC_9FFAC(arg0);
}

s32 func_800B2E38_A2CE8(void *arg0) {
    if (func_800B2C18_A2AC8(arg0) != 0) {
        return 1;
    }
    func_800B2DDC_A2C8C(arg0);
    func_800B3784_A3634(arg0);
    func_800B2B3C_A29EC(arg0);
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B2E80_A2D30);

typedef struct {
    u8 _pad0[0xB7C];
    s16 unkB7C;
    u8 _padB7E[0xC];
    s32 unkB8C;
    u8 _padB90[0x30];
    u8 unkBC0;
    u8 _padBC1[0xC];
    s8 unkBCD;
    u8 _padBCE[0xC];
    u8 unkBDA;
} func_800B2EE4_arg;

extern void func_800B2E80_A2D30(func_800B2EE4_arg *arg0);
extern void func_800B2818_A26C8(func_800B2EE4_arg *arg0, s8 arg1);
extern s8 func_8005D818_5E418(func_800B2EE4_arg *arg0);

void func_800B2EE4_A2D94(func_800B2EE4_arg *arg0, s8 arg1) {
    switch (arg0->unkBC0) {
        case 0:
            func_800B2E80_A2D30(arg0);
            func_800B2818_A26C8(arg0, arg1);
            break;
        case 1:
            if (arg0->unkBDA != 0 || (arg0->unkB7C & 0x8000)) {
                arg0->unkBC0 = arg0->unkBC0 + 1;
            }
            break;
        case 2:
            if (arg0->unkBDA != 0 || (!(arg0->unkB7C & 0x8000) && arg0->unkB8C == 0x200)) {
                arg0->unkBCD = func_8005D818_5E418(arg0);
            } else if (arg0->unkB7C & 0x8000) {
                func_8005D818_5E418(arg0);
            }
            break;
    }
}

extern void func_800B4058_A3F08(void *);

typedef struct {
    u8 _pad0[0xA96];
    s16 unkA96;
    s16 unkA98;
    u8 _padA9A[0xE8];
    s32 unkB84;
    u8 _padB88[0x4];
    s32 unkB8C;
    u8 _padB90[0x2F];
    u8 unkBBF;
    u8 unkBC0;
    u8 _padBC1[0xC];
    s8 unkBCD;
} func_800B30B0_arg;

s32 func_800B2FD0_A2E80(func_800B30B0_arg *arg0) {
    if (func_800B2C18_A2AC8(arg0) != 0) {
        return 1;
    }

    func_800B2EE4_A2D94((func_800B2EE4_arg *)arg0, 0);
    func_800B2DDC_A2C8C(arg0);

    if (arg0->unkB8C != 0) {
        if (arg0->unkA96 >= 0x1001) {
            if (arg0->unkBCD == -1) {
                arg0->unkB8C = arg0->unkB8C - 0x22;
            }
        }

        arg0->unkA96 = arg0->unkA96 + arg0->unkB8C;

        if (arg0->unkA96 >= 0x2000) {
            arg0->unkB8C = 0;
            if (arg0->unkBCD < 0) {
                arg0->unkB84 = arg0->unkB84 & ~0x4000;
            } else {
                arg0->unkBBF = arg0->unkBCD + 6;
                arg0->unkBC0 = 0;
            }
        } else {
            func_800B4058_A3F08(arg0);
        }
    }

    func_800B3784_A3634(arg0);
    func_800B2B3C_A29EC(arg0);
    return 0;
}

s32 func_800B30B0_A2F60(func_800B30B0_arg *arg0) {
    if (func_800B2C18_A2AC8(arg0) != 0) {
        return 1;
    }

    func_800B2EE4_A2D94((func_800B2EE4_arg *)arg0, 1);
    func_800B2DDC_A2C8C(arg0);

    if (arg0->unkB8C != 0) {
        if (arg0->unkA96 < -0x1000) {
            if (arg0->unkBCD == -1) {
                arg0->unkB8C = arg0->unkB8C - 0x22;
            }
        }

        arg0->unkA96 = arg0->unkA96 - arg0->unkB8C;

        if (arg0->unkA96 < -0x1FFF) {
            arg0->unkB8C = 0;
            if (arg0->unkBCD < 0) {
                arg0->unkB84 = arg0->unkB84 & ~0x4000;
            } else {
                arg0->unkBBF = arg0->unkBCD + 6;
                arg0->unkBC0 = 0;
            }
        } else {
            func_800B4058_A3F08(arg0);
        }
    }

    func_800B3784_A3634(arg0);
    func_800B2B3C_A29EC(arg0);
    return 0;
}

s32 func_800B3190_A3040(func_800B30B0_arg *arg0) {
    if (func_800B2C18_A2AC8(arg0) != 0) {
        return 1;
    }

    func_800B2EE4_A2D94((func_800B2EE4_arg *)arg0, 2);
    func_800B2DDC_A2C8C(arg0);

    if (arg0->unkB8C != 0) {
        if (arg0->unkA98 < -0x1000) {
            if (arg0->unkBCD == -1) {
                arg0->unkB8C = arg0->unkB8C - 0x22;
            }
        }

        arg0->unkA98 = arg0->unkA98 - arg0->unkB8C;

        if (arg0->unkA98 < -0x1FFF) {
            arg0->unkB8C = 0;
            if (arg0->unkBCD < 0) {
                arg0->unkB84 = arg0->unkB84 & ~0x4000;
            } else {
                arg0->unkBBF = arg0->unkBCD + 6;
                arg0->unkBC0 = 0;
            }
        } else {
            func_800B4058_A3F08(arg0);
        }
    }

    func_800B3784_A3634(arg0);
    func_800B2B3C_A29EC(arg0);
    return 0;
}

s32 func_800B3270_A3120(func_800B30B0_arg *arg0) {
    if (func_800B2C18_A2AC8(arg0) != 0) {
        return 1;
    }

    func_800B2EE4_A2D94((func_800B2EE4_arg *)arg0, 3);
    func_800B2DDC_A2C8C(arg0);

    if (arg0->unkB8C != 0) {
        if (arg0->unkA98 >= 0x1001) {
            if (arg0->unkBCD == -1) {
                arg0->unkB8C = arg0->unkB8C - 0x22;
            }
        }

        arg0->unkA98 = arg0->unkA98 + arg0->unkB8C;

        if (arg0->unkA98 >= 0x2000) {
            arg0->unkB8C = 0;
            if (arg0->unkBCD < 0) {
                arg0->unkB84 = arg0->unkB84 & ~0x4000;
            } else {
                arg0->unkBBF = arg0->unkBCD + 6;
                arg0->unkBC0 = 0;
            }
        } else {
            func_800B4058_A3F08(arg0);
        }
    }

    func_800B3784_A3634(arg0);
    func_800B2B3C_A29EC(arg0);
    return 0;
}

s32 func_800B3350_A3200(func_800B30B0_arg *arg0) {
    if (func_800B2C18_A2AC8(arg0) != 0) {
        return 1;
    }

    func_800B2EE4_A2D94((func_800B2EE4_arg *)arg0, 4);
    func_800B2DDC_A2C8C(arg0);

    if (arg0->unkB8C != 0) {
        if (arg0->unkA96 >= 0x1001) {
            if (arg0->unkBCD == -1) {
                arg0->unkB8C = arg0->unkB8C - 0x22;
            }
        }

        arg0->unkA96 = arg0->unkA96 + arg0->unkB8C;
        arg0->unkA98 = arg0->unkA98 - arg0->unkB8C;

        if (arg0->unkA96 >= 0x2000) {
            arg0->unkB8C = 0;
            if (arg0->unkBCD < 0) {
                arg0->unkB84 = arg0->unkB84 & ~0x4000;
            } else {
                arg0->unkBBF = arg0->unkBCD + 6;
                arg0->unkBC0 = 0;
            }
        } else {
            func_800B4058_A3F08(arg0);
        }
    }

    func_800B3784_A3634(arg0);
    func_800B2B3C_A29EC(arg0);
    return 0;
}

s32 func_800B3438_A32E8(func_800B30B0_arg *arg0) {
    if (func_800B2C18_A2AC8(arg0) != 0) {
        return 1;
    }

    func_800B2EE4_A2D94((func_800B2EE4_arg *)arg0, 5);
    func_800B2DDC_A2C8C(arg0);

    if (arg0->unkB8C != 0) {
        if (arg0->unkA96 >= 0x1001) {
            if (arg0->unkBCD == -1) {
                arg0->unkB8C = arg0->unkB8C - 0x22;
            }
        }

        arg0->unkA96 = arg0->unkA96 + arg0->unkB8C;
        arg0->unkA98 = arg0->unkA98 + arg0->unkB8C;

        if (arg0->unkA96 >= 0x2000) {
            arg0->unkB8C = 0;
            if (arg0->unkBCD < 0) {
                arg0->unkB84 = arg0->unkB84 & ~0x4000;
            } else {
                arg0->unkBBF = arg0->unkBCD + 6;
                arg0->unkBC0 = 0;
            }
        } else {
            func_800B4058_A3F08(arg0);
        }
    }

    func_800B3784_A3634(arg0);
    func_800B2B3C_A29EC(arg0);
    return 0;
}

s32 func_800B3520_A33D0(func_800B30B0_arg *arg0) {
    if (func_800B2C18_A2AC8(arg0) != 0) {
        return 1;
    }

    func_800B2EE4_A2D94((func_800B2EE4_arg *)arg0, 6);
    func_800B2DDC_A2C8C(arg0);

    if (arg0->unkB8C != 0) {
        if (arg0->unkA96 < -0x1000) {
            if (arg0->unkBCD == -1) {
                arg0->unkB8C = arg0->unkB8C - 0x22;
            }
        }

        arg0->unkA96 = arg0->unkA96 - arg0->unkB8C;
        arg0->unkA98 = arg0->unkA98 - arg0->unkB8C;

        if (arg0->unkA96 < -0x1FFF) {
            arg0->unkB8C = 0;
            if (arg0->unkBCD < 0) {
                arg0->unkB84 = arg0->unkB84 & ~0x4000;
            } else {
                arg0->unkBBF = arg0->unkBCD + 6;
                arg0->unkBC0 = 0;
            }
        } else {
            func_800B4058_A3F08(arg0);
        }
    }

    func_800B3784_A3634(arg0);
    func_800B2B3C_A29EC(arg0);
    return 0;
}

s32 func_800B3608_A34B8(func_800B30B0_arg *arg0) {
    if (func_800B2C18_A2AC8(arg0) != 0) {
        return 1;
    }

    func_800B2EE4_A2D94((func_800B2EE4_arg *)arg0, 7);
    func_800B2DDC_A2C8C(arg0);

    if (arg0->unkB8C != 0) {
        if (arg0->unkA96 < -0x1000) {
            if (arg0->unkBCD == -1) {
                arg0->unkB8C = arg0->unkB8C - 0x22;
            }
        }

        arg0->unkA96 = arg0->unkA96 - arg0->unkB8C;
        arg0->unkA98 = arg0->unkA98 + arg0->unkB8C;

        if (arg0->unkA96 < -0x1FFF) {
            arg0->unkB8C = 0;
            if (arg0->unkBCD < 0) {
                arg0->unkB84 = arg0->unkB84 & ~0x4000;
            } else {
                arg0->unkBBF = arg0->unkBCD + 6;
                arg0->unkBC0 = 0;
            }
        } else {
            func_800B4058_A3F08(arg0);
        }
    }

    func_800B3784_A3634(arg0);
    func_800B2B3C_A29EC(arg0);
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B36F0_A35A0);

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B3784_A3634);

void func_800B3950_A3800(func_800B00D4_arg *arg0) {
    D_800BAB68_AAA18[arg0->unkBBF](arg0);
}

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B3980_A3830);

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B3CBC_A3B6C);

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B3DF0_A3CA0);

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B3F2C_A3DDC);

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B4058_A3F08);

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B40D4_A3F84);

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B419C_A404C);

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B42A8_A4158);

void func_800B468C_A453C(func_800B00D4_arg *arg0) {
    D_800BAB78_AAA28[arg0->unkBBE](arg0);
}

extern void func_80059D30_5A930(void *);

void func_800B46BC_A456C(func_800B00D4_arg *arg0) {
    arg0->unkBBD = 2;
    arg0->unkBBE = 5;
    arg0->unkBBF = 0;
    arg0->unkBC0 = 0;
    arg0->unkB9A = 0;
    arg0->unkBA6 = 0;
    arg0->unkBA4 = 0;
    arg0->unkB84 = arg0->unkB84 & 0xFFFF2FFF;
    func_80059D30_5A930(arg0);
}

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B470C_A45BC);

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B48AC_A475C);

extern void func_8005D804_5E404(void *, u8, u8);

typedef struct {
    u8 _pad0[0x44C];
    s32 unk44C; // 0x44C
    s32 unk450; // 0x450
    s32 unk454; // 0x454
    u8 _pad458[0xB84 - 0x458];
    s32 unkB84; // 0xB84
    s32 unkB88; // 0xB88
} func_800B4A4C_arg;

s32 func_800B4A4C_A48FC(func_800B4A4C_arg *arg0) {
    arg0->unkB88 = 1;
    arg0->unkB84 = arg0->unkB84 | 0x20;
    arg0->unk450 = arg0->unk450 - 0x6000;
    arg0->unk44C = arg0->unk44C - (arg0->unk44C >> 6);
    arg0->unk454 = arg0->unk454 - (arg0->unk454 >> 6);
    func_800B40D4_A3F84(arg0);
    func_800B02AC_A015C(arg0);

    if (func_8005D308_5DF08(arg0, 9) != 0) {
        if ((arg0->unkB84 & 1) == 0) {
            func_800B00D4_9FF84((func_800B00D4_arg *)arg0, 3);
        }
    }

    arg0->unkB84 = arg0->unkB84 | 0x10000;
    func_8005D804_5E404(arg0, 3, 0);
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B4B08_A49B8);

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B4CB8_A4B68);

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B4DB8_A4C68);

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B4F5C_A4E0C);

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B50C0_A4F70);

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B5234_A50E4);

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B5394_A5244);

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B5478_A5328);

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B55B4_A5464);

typedef struct {
    u8 _pad0[0x438];
    s32 unk438;
    u8 _pad43C[0xB88 - 0x43C];
    s32 unkB88;
    s32 unkB8C;
} func_800B58B8_arg;

s32 func_800B58B8_A5768(func_800B58B8_arg *arg0) {
    arg0->unkB8C -= 1;

    if (arg0->unkB8C == 0) {
        arg0->unkB88 = 0;
        func_800B00E4_9FF94(arg0);
        return 1;
    }

    arg0->unkB88 = 8;
    arg0->unk438 = arg0->unk438 + 0xFFFF0000;
    func_800B40D4_A3F84(arg0);
    func_8005D180_5DD80(arg0, 0);
    func_8005D804_5E404(arg0, 3, 0);
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B5938_A57E8);

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B5A40_A58F0);

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B5B90_A5A40);

typedef struct {
    u8 _pad0[0x44C];
    s32 unk44C; // 0x44C
    s32 unk450; // 0x450
    s32 unk454; // 0x454
    u8 _pad458[0xB84 - 0x458];
    s32 unkB84; // 0xB84
    s32 unkB88; // 0xB88
} func_800B5CB8_arg;

s32 func_800B5CB8_A5B68(func_800B5CB8_arg *arg0) {
    arg0->unkB88 = 1;
    arg0->unkB84 = arg0->unkB84 | 0x20;
    arg0->unk450 = arg0->unk450 - 0x6000;
    arg0->unk44C = arg0->unk44C - (arg0->unk44C >> 6);
    arg0->unk454 = arg0->unk454 - (arg0->unk454 >> 6);
    func_800B40D4_A3F84(arg0);
    func_800B0300_A01B0((func_800B0300_arg *)arg0);

    if (func_8005D308_5DF08(arg0, 0xE) != 0) {
        if ((arg0->unkB84 & 1) == 0) {
            func_800B00D4_9FF84((func_800B00D4_arg *)arg0, 8);
        }
    }

    func_8005D804_5E404(arg0, 3, 0);
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B5D68_A5C18);

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B5E90_A5D40);

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B5FC4_A5E74);

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B6194_A6044);

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B62E4_A6194);

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B6488_A6338);

void func_800B65C8_A6478(func_800B00D4_arg *arg0) {
    D_800BAC10_AAAC0[arg0->unkBBE](arg0);
}

void func_800B65F8_A64A8(func_800B00D4_arg *arg0) {
    arg0->unkBBD = 3;
    arg0->unkBBE = 0;
    arg0->unkBBF = 0;
    arg0->unkBC0 = 0;
}

void func_800B6610_A64C0(func_800B00D4_arg *arg0) {
    D_800BAC14_AAAC4[arg0->unkBBF](arg0);
}

s32 func_800B6640_A64F0(func_800B30B0_arg *arg0) {
    arg0->unkB8C = 10;
    arg0->unkB84 = arg0->unkB84 | 0x180;

    if (arg0->unkB84 & 2) {
        arg0->unkBC0 = 1;
        arg0->unkB84 = arg0->unkB84 & ~2;
    }

    arg0->unkBBF = arg0->unkBBF + 1;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B6688_A6538);

typedef struct {
    u8 _pad0[0x434];
    s32 unk434;
    u8 _pad438[0x4];
    s32 unk43C;
    u8 _pad440[0xC];
    s32 unk44C;
    s32 unk450;
    s32 unk454;
    u8 _pad458[0x767];
    u8 unkBBF;
    u8 unkBC0;
} func_800B67E4_arg;

s32 func_800B67E4_A6694(func_800B67E4_arg *arg0) {
    D_80090F90_91B90_item *item;
    GameState *gameState = (GameState *)getCurrentAllocation();

    item = func_80055D10_56910(gameState->memoryPoolId);

    arg0->unk44C = 0;
    arg0->unk454 = 0;
    arg0->unk450 = arg0->unk450 - 0x6000;
    arg0->unk434 = arg0->unk434 + ((item->unk0 - arg0->unk434) >> 2);
    arg0->unk43C = arg0->unk43C + ((item->unk4 - arg0->unk43C) >> 2);
    func_800B02AC_A015C(arg0);
    func_800B40D4_A3F84(arg0);

    if (func_8005D308_5DF08(arg0, 3) != 0) {
        arg0->unkBC0 = 0;
        arg0->unkBBF = arg0->unkBBF + 1;
    }

    return 0;
}

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B6890_A6740);

extern s32 D_800BAC80_AAB30;
extern void transformVector2(void *matrix, void *vector, s32 *output);

typedef struct {
    u8 _pad0[0x44C];
    s32 unk44C;
    u8 _pad450[0x520];
    s32 unk970;
    u8 _pad974[0x210];
    s32 unkB84;
    u8 _padB88[0x4];
    s32 unkB8C;
    u8 _padB90[0x2F];
    u8 unkBBF;
} func_800B6D14_arg;

s32 func_800B6D14_A6BC4(func_800B6D14_arg *arg0) {
    GameState *gameState = getCurrentAllocation();

    func_80055D10_56910(gameState->memoryPoolId);
    func_800B40D4_A3F84(arg0);

    if (arg0->unkB8C != 0) {
        arg0->unkB8C = arg0->unkB8C - 1;
    } else {
        arg0->unkB8C = 0xB4;
        arg0->unkBBF = arg0->unkBBF + 1;
    }

    if (arg0->unkB8C < 0x12) {
        arg0->unkB84 = arg0->unkB84 | 0x2000;
        transformVector2(&D_800BAC80_AAB30, &arg0->unk970, &arg0->unk44C);
    }

    func_800B02AC_A015C(arg0);
    func_8005D308_5DF08(arg0, 0xD);

    return 0;
}

typedef struct {
    u8 _pad0[0x44C];
    s32 unk44C;
    u8 _pad450[0x520];
    s32 unk970;
    u8 _pad974[0x218];
    s32 unkB8C;
    u8 _padB90[0x28];
    u8 unkBB8;
    u8 _padBB9[0x5];
    u8 unkBBE;
    u8 unkBBF;
    u8 unkBC0;
} func_800B6DB8_arg;

extern s32 D_800BAC8C_AAB3C;
extern void func_8006FDC8_709C8(u16 arg0, u8 arg1, u8 arg2);

s32 func_800B6DB8_A6C68(func_800B6DB8_arg *arg0) {
    s32 pad[12];
    GameState *gameState = getCurrentAllocation();

    func_80055D10_56910(gameState->memoryPoolId);
    func_800B40D4_A3F84(arg0);
    transformVector2(&D_800BAC8C_AAB3C, &arg0->unk970, &arg0->unk44C);
    func_800B02AC_A015C(arg0);

    if (arg0->unkB8C != 0) {
        arg0->unkB8C = arg0->unkB8C - 1;
        if (arg0->unkB8C == 0x11) {
            func_8006FDC8_709C8(arg0->unkBB8, 0xFF, 0x10);
        }
    } else {
        arg0->unkBBF = arg0->unkBBF + 1;
        arg0->unkBC0 = 0;
    }

    func_8005D308_5DF08(arg0, 0xD);
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B6E5C_A6D0C);

typedef struct {
    u8 _pad0[0x44C];
    s32 unk44C;
    u8 _pad450[0x520];
    s32 unk970;
    u8 _pad974[0x210];
    s32 unkB84;
    u8 _padB88[0x4];
    s32 unkB8C;
} func_800B6FE8_arg;

s32 func_800B6FE8_A6E98(func_800B6FE8_arg *arg0) {
    GameState *gameState = getCurrentAllocation();

    func_80055D10_56910(gameState->memoryPoolId);
    transformVector2(&D_800BAC80_AAB30, &arg0->unk970, &arg0->unk44C);
    func_800B02AC_A015C(arg0);
    func_800B40D4_A3F84(arg0);

    if (arg0->unkB8C != 0) {
        arg0->unkB8C = arg0->unkB8C - 1;
    } else {
        arg0->unkB84 = arg0->unkB84 & ~0x2380;
        func_800B00E4_9FF94(arg0);
    }

    func_8005D308_5DF08(arg0, 0xD);
    return 0;
}

typedef struct {
    u8 _pad0[0x434];
    u8 unk434[0xC];    // 0x434 - source for memcpy (12 bytes)
    u8 _pad440[0xC];   // 0x440 to 0x44C
    s32 unk44C;        // 0x44C
    s32 unk450;        // 0x450
    s32 unk454;        // 0x454
    u8 _pad458[0x10];  // 0x458 to 0x468
    s32 unk468;        // 0x468
    u8 _pad46C[0x4];   // 0x46C to 0x470
    u8 unk470[0xC];    // 0x470 - destination for memcpy (12 bytes)
    u8 _pad47C[0x4];   // 0x47C to 0x480
    s32 unk480;        // 0x480
    u8 _pad484[0x708]; // 0x484 to 0xB8C
    s32 unkB8C;        // 0xB8C
    u8 _padB90[0x2F];  // 0xB90 to 0xBBF
    u8 unkBBF;         // 0xBBF
    u8 unkBC0;         // 0xBC0
} func_800B7078_arg;

s32 func_800B7078_A6F28(func_800B7078_arg *arg0) {
    arg0->unk44C = 0;
    arg0->unk454 = 0;
    arg0->unk450 = arg0->unk450 - 0x6000;
    func_800B02AC_A015C(arg0);
    func_800B40D4_A3F84(arg0);
    func_8005D308_5DF08(arg0, 3);

    arg0->unkB8C = arg0->unkB8C - 1;
    if (arg0->unkB8C == 0) {
        arg0->unkBC0 = 0;
        arg0->unkBBF = arg0->unkBBF + 1;
        memcpy(arg0->unk470, arg0->unk434, 0xC);
        arg0->unk480 = 0;
        arg0->unk468 = 0x30000;
        arg0->unkB8C = 0;
    }

    return 0;
}

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B7108_A6FB8);

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B71E4_A7094);

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B724C_A70FC);

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B735C_A720C);

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B73CC_A727C);

typedef struct {
    u8 _pad0[0x44C];
    s32 unk44C; // 0x44C
    s32 unk450; // 0x450
    s32 unk454; // 0x454
    u8 _pad458[0xB84 - 0x458];
    s32 unkB84;       // 0xB84
    u8 _padB88[4];    // 0xB88
    s32 unkB8C;       // 0xB8C
    u8 _padB90[0x30]; // 0xB90 to 0xBC0
    u8 unkBC0;        // 0xBC0
} func_800B7444_arg;

s32 func_800B7444_A72F4(func_800B7444_arg *arg0) {
    if (arg0->unkBC0 == 0) {
        arg0->unk454 = 0xFFF80000;
        arg0->unk44C = 0;
        arg0->unk450 = 0x30000;
        arg0->unkBC0 = arg0->unkBC0 + 1;
    }

    arg0->unk450 = arg0->unk450 - 0x6000;
    func_800B02AC_A015C(arg0);
    func_800B40D4_A3F84(arg0);

    if (arg0->unkB8C != 0) {
        arg0->unkB8C = arg0->unkB8C - 1;
    } else {
        arg0->unkB84 = arg0->unkB84 & ~0x2380;
        func_800B00E4_9FF94(arg0);
    }

    func_8005D308_5DF08(arg0, 4);
    return 0;
}

extern void func_800BBB90(s16);

typedef struct {
    u8 _pad0[0x434];
    u8 unk434;
    u8 _pad435[0x17];
    s32 unk44C;
    s32 unk450;
    s32 unk454;
    u8 _pad458[0x734];
    s32 unkB8C;
    u8 _padB90[0x2F];
    u8 unkBBF;
    u8 unkBC0;
} func_800B74E4_arg;

s32 func_800B74E4_A7394(func_800B74E4_arg *arg0) {
    arg0->unk44C = 0;
    arg0->unk454 = 0;
    arg0->unk450 = arg0->unk450 - 0x6000;
    func_800B02AC_A015C(arg0);
    func_800B40D4_A3F84(arg0);
    func_8005D180_5DD80(arg0, 0);

    arg0->unkB8C = arg0->unkB8C - 1;
    if (arg0->unkB8C == 0) {
        arg0->unkBC0 = 0;
        arg0->unkB8C = 0;
        arg0->unkBBF = arg0->unkBBF + 1;
        func_800BBB90(0);
        func_80056B7C_5777C(&arg0->unk434, 0x26);
    }

    return 0;
}

typedef struct {
    u8 _pad0[0x44C];
    s32 unk44C; // 0x44C
    s32 unk450; // 0x450
    s32 unk454; // 0x454
    u8 _pad458[0xA94 - 0x458];
    u16 unkA94; // 0xA94
    u8 _padA96[0xB8C - 0xA96];
    s32 unkB8C; // 0xB8C
    u8 _padB90[0xBA0 - 0xB90];
    u16 unkBA0; // 0xBA0
    u16 unkBA2; // 0xBA2
    u8 _padBA4[0xBB8 - 0xBA4];
    u8 unkBB8; // 0xBB8
    u8 _padBB9[0xBBF - 0xBB9];
    u8 unkBBF; // 0xBBF
} func_800B7784_arg;

s32 func_800B756C_A741C(func_800B7784_arg *arg0) {
    arg0->unk44C = 0;
    arg0->unk454 = 0;
    arg0->unk450 = arg0->unk450 - 0x6000;
    func_800B02AC_A015C(arg0);
    func_800B40D4_A3F84(arg0);
    func_8005D180_5DD80(arg0, 0);
    arg0->unkA94 = arg0->unkA94 + arg0->unkB8C;
    arg0->unkB8C = arg0->unkB8C + 0x10;

    if (arg0->unkB8C == 0x400) {
        arg0->unkB8C = 10;
        arg0->unkBBF = arg0->unkBBF + 1;
    }

    return 0;
}

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B75F4_A74A4);

typedef struct {
    u8 _pad0[0x81];
    u8 unk81; // 0x81
} func_800B76BC_alloc;

s32 func_800B76BC_A756C(func_800B7784_arg *arg0) {
    func_800B76BC_alloc *alloc = (func_800B76BC_alloc *)getCurrentAllocation();
    u16 temp1;
    u16 temp2;

    arg0->unk44C = 0;
    arg0->unk454 = 0;
    arg0->unk450 = arg0->unk450 - 0x6000;
    func_800B02AC_A015C(arg0);
    func_800B40D4_A3F84(arg0);
    func_8005D180_5DD80(arg0, 0);

    temp1 = arg0->unkBA0;
    temp2 = arg0->unkBA2;
    temp1 -= 0x400;
    arg0->unkBA0 = temp1;
    arg0->unkA94 = arg0->unkA94 + 0x400;
    temp2 += 0x400;
    arg0->unkBA2 = temp2;

    if (arg0->unkBA0 == 0) {
        arg0->unkB8C = 0x11;
        arg0->unkBBF = arg0->unkBBF + 1;
        func_8006FDC8_709C8(arg0->unkBB8, 0xFF, 0x10);
        alloc->unk81 = alloc->unk81 + 1;
        func_800BBB90(1);
    }

    return 0;
}

typedef struct {
    u8 _pad0[0x80];
    u8 unk80; // 0x80
} func_800B7784_alloc;

s32 func_800B7784_A7634(func_800B7784_arg *arg0) {
    func_800B7784_alloc *alloc = (func_800B7784_alloc *)getCurrentAllocation();
    s32 temp;

    arg0->unk44C = 0;
    arg0->unk454 = 0;
    arg0->unk450 = arg0->unk450 - 0x6000;
    func_800B02AC_A015C(arg0);
    func_800B40D4_A3F84(arg0);
    func_8005D180_5DD80(arg0, 0);
    arg0->unkA94 = arg0->unkA94 + 0x400;

    temp = arg0->unkB8C - 1;
    arg0->unkB8C = temp;
    if (temp == 0) {
        arg0->unkBBF = arg0->unkBBF + 1;
        alloc->unk80 = alloc->unk80 - 1;
    }

    return 0;
}

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B781C_A76CC);

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B7998_A7848);

typedef struct {
    u8 _pad0[0xB84];
    s32 unkB84;       // 0xB84
    u8 _padB88[4];    // 0xB88
    s32 unkB8C;       // 0xB8C
    u8 _padB90[0x2F]; // 0xB90 to 0xBBF
    u8 unkBBF;        // 0xBBF
    u8 unkBC0;        // 0xBC0
} func_800B7A30_arg;

s32 func_800B7A30_A78E0(func_800B7A30_arg *arg0) {
    func_8005D308_5DF08(arg0, 3);

    arg0->unkB8C = arg0->unkB8C - 1;
    if (arg0->unkB8C == 0) {
        arg0->unkB8C = 0xE;
        arg0->unkBC0 = 0;
        arg0->unkBBF = arg0->unkBBF + 1;
    }

    arg0->unkB84 = arg0->unkB84 | 0x10000;
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B7A94_A7944);

typedef struct {
    u8 _pad0[0xBDF];
    u8 unkBDF;
    u8 unkBE0;
    u8 unkBE1;
} func_800B7B64_arg;

extern u8 D_800BACBC_AAB6C[];

void func_800B7B44_A79F4(func_800B7B64_arg *arg0, s32 arg1) {
    arg0->unkBDF = arg1;
    *(s8 *)&arg0->unkBE0 = -1;
    arg0->unkBE1 = D_800BACBC_AAB6C[arg1];
}

u8 func_800B7B64_A7A14(func_800B7B64_arg *arg0, s32 arg1) {
    arg0->unkBDF = arg1;
    arg0->unkBE1 = D_800BACBC_AAB6C[arg1];
    return arg0->unkBE1;
}

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B7B7C_A7A2C);

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B82D8_A8188);

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B8894_A8744);

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B9500_A93B0);

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B98CC_A977C);

void func_800B99E0(func_800B9AE0_arg *arg0) {
    func_800B99E0_alloc *alloc;
    u8 v0;
    u8 v1;

    alloc = getCurrentAllocation();
    arg0->unk4 = loadAssetByIndex_94F90(arg0->unkBB9, arg0->unkBBA);
    arg0->unk8 = loadAssetByIndex_95200(arg0->unkBB9, arg0->unkBBA);
    arg0->unk0 = loadAssetByIndex_953E0(arg0->unkBB9);

    v0 = arg0->unkBB9;
    if (v0 < 6) {
        arg0->unk20 = func_8005DE04_5EA04(arg0->unkBB9, arg0->unkBBA);
        arg0->unk24 = func_8005DE30_5EA30(arg0->unkBB9, arg0->unkBBA);
    }

    arg0->unkC = loadAssetByIndex_95500(arg0->unkBBB);
    arg0->unk10 = loadAssetByIndex_95590(arg0->unkBBB);

    v1 = arg0->unkBBB;
    if (v1 < 9) {
        arg0->unk14 = loadAssetByIndex_95668(arg0->unkBBC);
    } else {
        arg0->unk14 = NULL;
    }

    arg0->unk18 = load_3ECE40();

    if (arg0->unkBC7 != 0) {
        arg0->unk1C = func_8005DC60_5E860(alloc->unk5C);
    }

    if (arg0->unkBE3 != 0) {
        arg0->unk2C = loadAssetByIndex_5E990(arg0->unkBE4);
    }
}

void func_800B9AE0(func_800B9AE0_arg *arg0) {
    s32 pad[8];

    arg0->unk4 = freeNodeMemory(arg0->unk4);
    arg0->unk8 = freeNodeMemory(arg0->unk8);
    arg0->unk0 = freeNodeMemory(arg0->unk0);
    arg0->unk20 = freeNodeMemory(arg0->unk20);
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unkC = freeNodeMemory(arg0->unkC);
    arg0->unk10 = freeNodeMemory(arg0->unk10);
    arg0->unk14 = freeNodeMemory(arg0->unk14);
    arg0->unk18 = freeNodeMemory(arg0->unk18);
    arg0->unk1C = freeNodeMemory(arg0->unk1C);
    arg0->unk2C = freeNodeMemory(arg0->unk2C);
}
