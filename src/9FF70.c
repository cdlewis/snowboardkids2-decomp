#include "9FF70.h"
#include "3E160.h"
#include "4CD70.h"
#include "51060.h"
#include "56910.h"
#include "5AA90.h"
#include "5E590.h"
#include "6E840.h"
#include "A9A40.h"
#include "common.h"
#include "gamestate.h"
#include "geometry.h"
#include "graphics.h"
#include "rand.h"
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
    void *unk28;
    void *unk2C;
    u8 padding[0x404];
    void *unk434;
    s32 unk438;
    s32 unk43C;
    s32 unk440;
    s32 unk444;
    s32 unk448;
    s32 unk44C;
    s32 unk450;
    s32 unk454;
    s32 unk458;
    s32 unk45C;
    s32 unk460;
    u8 padding1[0x4];
    s32 unk468;
    s32 unk46C;
    void *unk470;
    s32 unk474;
    s32 unk478;
    s32 unk47C;
    s32 unk480;
    u8 padding2[0x4EC];
    Mat3x3Padded unk970;
    u8 padding25[0x14];
    s32 unk9A4;
    u8 padding3[0xD4];
    s32 unkA7C;
    s32 _padA80;
    s32 unkA84;
    u8 _padA88[0x4];
    u16 unkA8C;
    s16 unkA8E;
    s16 unkA90;
    s16 unkA92;
    s16 unkA94;
    s16 unkA96;
    s16 unkA98;
    s16 unkA9A;
    u8 padding4[0xC];
    s32 unkAA8;
    s32 unkAAC;
    s32 unkAB0;
    s32 unkAB4;
    s32 unkAB8;
    s32 unkABC;
    s32 unkAC0;
    u16 unkAC4;
    u8 _padAC6[0x2];
    s32 unkAC8;
    s32 unkACC;
    s32 unkAD0;
    u8 padding45[0xC];
    s32 unkAE0;
    u8 padding5[0x5C];
    s32 unkB40;
    void *unkB44;
    s32 unkB48;
    s32 unkB4C;
    u8 padding6[0x20];
    s32 unkB70;
    u16 unkB74;
    u8 _padB76[2];
    s16 unkB78;
    s8 unkB7A;
    s8 unkB7B;
    s16 unkB7C;
    u16 unkB7E;
    s32 unkB80;
    s32 unkB84;
    s32 unkB88;
    s32 unkB8C;
    s32 unkB90;
    s16 unkB94;
    u8 _padB96[0x2];
    s16 unkB98;
    u8 _padB9A[0x2];
    s16 unkB9C;
    s16 unkB9E;
    u16 unkBA0;
    s16 unkBA2;
    u8 padding65[0x6];
    s16 unkBAA;
    s16 unkBAC;
    s16 unkBAE;
    u16 unkBB0;
    u16 unkBB2;
    u8 unkBB4;
    s8 unkBB5;
    u8 unkBB6;
    u8 unkBB7;
    u8 unkBB8;
    u8 unkBB9;
    u8 unkBBA;
    u8 unkBBB;
    u8 unkBBC;
    u8 unkBBD;
    u8 unkBBE;
    u8 unkBBF;
    u8 unkBC0;
    u8 unkBC1;
    u8 unkBC2;
    u8 unkBC3;
    u8 unkBC4;
    u8 unkBC5;
    u8 unkBC6;
    u8 unkBC7;
    u8 unkBC8;
    u8 unkBC9;
    u8 unkBCA;
    u8 unkBCB;
    u8 unkBCC;
    s8 unkBCD;
    u8 unkBCE;
    u8 padding10[0x6];
    u8 unkBD5;
    u8 unkBD6;
    u8 unkBD7;
    u8 padding11[0x2];
    u8 unkBDA;
    u8 unkBDB;
    u8 unkBDC;
    u8 unkBDD;
    u8 unkBDE;
    u8 unkBDF;
    u8 unkBE0;
    u8 unkBE1;
    u8 unkBE2;
    u8 unkBE3;
    u8 unkBE4;
} func_800B30B0_arg;

extern s32 func_8005D308_5DF08(void *, s32);

extern u16 D_800BAB48_AA9F8[];
extern u16 D_800BAB58_AAA08[];

extern s32 func_80059E90_5AA90(void *arg0, void *arg1, u16 arg2, void *arg3);
extern s32 func_8005CFC0_5DBC0(void *, s32, void *, s32);
extern s32 D_800BA348_AA1F8;
extern s32 D_800BA350_AA200;
extern s32 D_800BAB40_AA9F0;
extern s32 D_800BAB44_AA9F4;
extern s32 D_800BAB3C_AA9EC;

s32 func_800B2C18_A2AC8(func_800B30B0_arg *);
void func_800B2B3C_A29EC(func_800B30B0_arg *);
void func_800B3784_A3634(func_800B30B0_arg *);
void func_800B4058_A3F08(func_800B30B0_arg *);
void func_800B419C_A404C(func_800B30B0_arg *);
void func_800B00FC_9FFAC(func_800B30B0_arg *);
void func_800B40D4_A3F84(func_800B30B0_arg *);
void func_800B27FC_A26AC(func_800B30B0_arg *);
void func_8005D48C_5E08C(func_800B30B0_arg *, s32, s32);
void func_80058CFC_598FC(void *);

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

void func_800B46BC_A456C(func_800B00D4_arg *);

void func_800B00C0_9FF70(func_800B00D4_arg *arg0, u8 arg1) {
    arg0->unkBBD = arg1;
    arg0->unkBBE = 0;
    arg0->unkBBF = 0;
    arg0->unkBC0 = 0;
}

void func_800B00D4_9FF84(func_800B30B0_arg *arg0, s32 arg1) {
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

extern void func_8005D03C_5DC3C(func_800B30B0_arg *);

void func_800B02AC_A015C(func_800B30B0_arg *arg0) {
    func_8005D03C_5DC3C(arg0);
    arg0->unk434 = arg0->unk434 + arg0->unk44C;
    arg0->unk438 = arg0->unk438 + arg0->unk450;
    arg0->unk43C = arg0->unk43C + arg0->unk454;
}

void func_800B0300_A01B0(func_800B30B0_arg *arg0) {
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

extern void func_8005D180_5DD80(func_800B30B0_arg *, s32);

s32 func_800B12F0_A11A0(func_800B30B0_arg *arg0) {
    GameState *alloc = getCurrentAllocation();

    if (alloc->unk79 == 0) {
        arg0->unkAAC = 0;
        if (arg0->unkBBF != 0) {
            if (arg0->unkB8C < 9) {
                func_80058BB0_597B0((Player *)arg0);
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

extern void func_8005D804_5E404(void *, u8, u8);
extern void func_80059BD4_5A7D4(void *);

s32 func_800B13D4_A1284(func_800B30B0_arg *arg0) {
    s32 sp10[3];

    if (!(arg0->unkB84 & 0x20000)) {
        func_800B00E4_9FF94(arg0);
        return 1;
    }

    if (arg0->unkBBF == 0) {
        arg0->unkBBF++;
        if (arg0->unk450 > 0) {
            arg0->unk450 = 0;
        }
        arg0->unkB8C = 0;
    }

    if (arg0->unkB8C < 4) {
        arg0->unkB8C++;
    } else if (arg0->unkB8C == 4) {
        func_80059BD4_5A7D4(arg0);
        arg0->unkB8C++;
    }

    arg0->unkA90 = 0;
    arg0->unkA92 = 0;
    arg0->unk9A4 = 0;

    if (arg0->unkB84 & 2) {
        arg0->unkA8E = arg0->unkBB0;
        arg0->unkA94 = arg0->unkBB2;
    } else {
        arg0->unkA8E = -arg0->unkBB0;
        arg0->unkA94 = arg0->unkBB2 + 0x1000;
    }

    rotateVectorY(&arg0->unk44C, (s16)(-arg0->unkBB2), sp10);
    sp10[0] = 0;
    rotateVectorY(sp10, (s16)(arg0->unkBB2), &arg0->unk44C);

    arg0->unk450 = arg0->unk450 - arg0->unkAB8;
    func_800B02AC_A015C(arg0);

    if (arg0->unkBC0 == 0) {
        if (func_8005D308_5DF08(arg0, 0x21) != 0) {
            arg0->unkBC0++;
        }
    } else {
        func_8005D180_5DD80(arg0, 0x22);
    }

    func_8005D804_5E404(arg0, 1, 0);

    return 0;
}

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B1544_A13F4);

s32 func_800B1DA0_A1C50(func_800B30B0_arg *arg0) {
    s16 temp_v1;
    s32 var_a3;
    s32 temp_v1_3;
    s32 temp_a0;
    u8 temp;

    if (arg0->unkB7C & 0xF) {
        func_800B00D4_9FF84(arg0, 4);
        arg0->unkBBF = 5;
        func_800B27FC_A26AC(arg0);
        return 1;
    }
    if (!(arg0->unkB84 & 1)) {
        if (arg0->unkBC0 >= 0xBU) {
            func_800B00D4_9FF84(arg0, 3);
        } else {
            func_800B00D4_9FF84(arg0, 0);
        }
        return 1;
    }
    temp = arg0->unkBC0;
    if (temp < 0x64U) {
        arg0->unkBC0 = temp + 1;
    }
    if (arg0->unkBDA != 0) {
        ((void (*)(void *))func_800B9B90_A9A40)(arg0);
        var_a3 = (s16)func_8006D21C_6DE1C(arg0->unkA7C, arg0->unkA84, (s32)arg0->unk434, arg0->unk43C);
        temp_v1 = arg0->unkA94;
        var_a3 = (var_a3 - temp_v1) & 0x1FFF;
        if (var_a3 >= 0x1001) {
            var_a3 -= 0x2000;
        }
        if (var_a3 >= 0x41) {
            var_a3 = 0x40;
        }
        if (var_a3 < -0x40) {
            var_a3 = -0x40;
        }
        arg0->unkA94 = temp_v1 + var_a3;
    } else {
        arg0->unkA94 = (u16)arg0->unkA94 - (arg0->unkB7A * 4);
    }
    temp_v1_3 = arg0->unk44C;
    temp_a0 = arg0->unk454;
    arg0->unk450 -= arg0->unkAB8;
    arg0->unk44C = temp_v1_3 - (temp_v1_3 >> 7);
    arg0->unk454 = temp_a0 - (temp_a0 >> 7);
    if (arg0->unkBC0 >= 0xBU) {
        func_800B419C_A404C(arg0);
    }
    func_800B00FC_9FFAC(arg0);
    var_a3 = arg0->unkA92;
    if (var_a3 >= 0) {
        if (var_a3 >= 0x401) {
            var_a3 = 0x400;
        }
        func_8005D48C_5E08C(arg0, 2, var_a3 / 2);
    } else {
        var_a3 = -var_a3;
        if (var_a3 >= 0x401) {
            var_a3 = 0x400;
        }
        func_8005D48C_5E08C(arg0, 1, var_a3 / 2);
    }
    func_80058CFC_598FC(arg0);
    return 0;
}

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

extern s32 func_8005A9A8_5B5A8(func_800B30B0_arg *);
extern void func_800B65F8_A64A8(func_800B00D4_arg *);
extern s32 func_800B42A8_A4158(func_800B30B0_arg *, s32, s32, s32);

s32 func_800B2018_A1EC8(func_800B30B0_arg *arg0) {
    s16 temp_v0;
    s16 v1;
    s32 temp;

    if (arg0->unkB84 & 0x20000) {
        func_800B00D4_9FF84(arg0, 6);
        return 1;
    }

    if (func_8005A9A8_5B5A8(arg0) != 0) {
        func_800B65F8_A64A8((func_800B00D4_arg *)arg0);
        return 1;
    }

    if (arg0->unkB84 & 1) {
        func_800B00D4_9FF84(arg0, 1);
        return 1;
    }

    arg0->unk450 = arg0->unk450 - arg0->unkAB8;
    func_800B40D4_A3F84(arg0);
    func_800B42A8_A4158(arg0, 0x200, 0x200, arg0->unkAB0);
    func_800B00FC_9FFAC(arg0);

    temp_v0 = arg0->unkA92;
    v1 = temp_v0;
    if (temp_v0 >= 0) {
        if (temp_v0 >= 0x401) {
            v1 = 0x400;
        }
        func_8005D48C_5E08C(arg0, 2, v1 / 2);
    } else {
        v1 = -v1;
        if (v1 >= 0x401) {
            v1 = 0x400;
        }
        func_8005D48C_5E08C(arg0, 1, v1 / 2);
    }

    temp = arg0->unkB8C - 1;
    arg0->unkB8C = temp;
    if (temp == 0) {
        arg0->unkBBF = arg0->unkBBF + 1;
    }

    func_80058CFC_598FC(arg0);
    return 0;
}

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

extern s32 func_8005A9A8_5B5A8(func_800B30B0_arg *);
extern void func_800B65F8_A64A8(func_800B00D4_arg *);
extern s8 func_8005D818_5E418(func_800B30B0_arg *);
s32 func_800B42A8_A4158(func_800B30B0_arg *, s32, s32, s32);

s32 func_800B243C_A22EC(func_800B30B0_arg *arg0) {
    s32 temp_s1;

    if (arg0->unkB84 & 0x20000) {
        func_800B00D4_9FF84(arg0, 6);
        return 1;
    }

    if (func_8005A9A8_5B5A8(arg0) != 0) {
        func_800B65F8_A64A8((func_800B00D4_arg *)arg0);
        return 1;
    }

    if (arg0->unkB84 & 1) {
        func_800B00D4_9FF84(arg0, 1);
        return 1;
    }

    arg0->unk450 -= arg0->unkAB8;
    func_800B40D4_A3F84(arg0);
    temp_s1 = func_800B42A8_A4158(arg0, 0x200, 0x200, arg0->unkAB0);
    func_800B00FC_9FFAC(arg0);

    if (!(arg0->unkB7C & 0x8000)) {
        arg0->unkBC0 = 1;
    }

    func_8005D818_5E418(arg0);
    func_8005D308_5DF08(arg0, 3);

    arg0->unkB8C -= 1;
    if (arg0->unkB8C == 0) {
        arg0->unkBBF += 1;
    }

    if (func_800B1F60_A1E10((func_800B1F60_arg *)arg0, temp_s1) != 0) {
        func_800B00D4_9FF84(arg0, 2);
    }

    func_80058CFC_598FC(arg0);
    return 0;
}

s32 func_800B255C_A240C(func_800B30B0_arg *arg0) {
    s32 temp_s1;

    if (arg0->unkB84 & 0x20000) {
        func_800B00D4_9FF84(arg0, 6);
        return 1;
    }

    if (func_8005A9A8_5B5A8(arg0) != 0) {
        func_800B65F8_A64A8((func_800B00D4_arg *)arg0);
        return 1;
    }

    if (arg0->unkB84 & 1) {
        func_800B00D4_9FF84(arg0, 1);
        return 1;
    }

    arg0->unk450 -= arg0->unkAB8;
    func_800B40D4_A3F84(arg0);
    temp_s1 = func_800B42A8_A4158(arg0, 0x200, 0x200, arg0->unkAB0);
    func_800B00FC_9FFAC(arg0);

    if (!(arg0->unkB7C & 0x8000)) {
        if (arg0->unkBDA != 0 && arg0->unkBDC != 0) {
            arg0->unkB8C = 0x10000;
        }
        arg0->unkBC0 = 1;
        if (arg0->unkB8C > 0xFFFF) {
            arg0->unkBCD = func_8005D818_5E418(arg0);
        }
    } else {
        func_8005D818_5E418(arg0);
        if (arg0->unkB8C <= 0xFFFF) {
            arg0->unkB8C += 0x4000;
        }
    }

    if (arg0->unkBC0 != 0) {
        arg0->unkBBF += 1;
    }

    func_8005D308_5DF08(arg0, 3);

    if (func_800B1F60_A1E10((func_800B1F60_arg *)arg0, temp_s1) != 0) {
        func_800B00D4_9FF84(arg0, 2);
    }

    func_80058CFC_598FC(arg0);
    return 0;
}

s32 func_800B26D0_A2580(func_800B30B0_arg *arg0) {
    s32 sp10[8];
    s32 sp30[8];
    s32 sp50[3];
    s32 *ptr;

    arg0->unkBC0 = 3;
    arg0->unkBBF++;
    D_800BAB40_AA9F0 = arg0->unkB8C + arg0->unkABC;

    if (*(volatile s32 *)&arg0->unkB84 & 2) {
        D_800BAB44_AA9F4 = 0xFFFE0000;
    } else {
        D_800BAB44_AA9F4 = 0x20000;
    }

    if (arg0->unkBDA != 0) {
        if (arg0->unkBDC == 0) {
            if (arg0->unkB98 != 0) {
                ptr = &D_800BAB44_AA9F4;
                *ptr = *ptr * 2;
            }
        }
    } else {
        if (*(u16 *)&arg0->unkB7A == 7) {
            ptr = &D_800BAB44_AA9F4;
            *ptr = *ptr * 2;
        }
    }

    func_8006BDBC_6C9BC((func_8005E800_5F400_arg *)((u8 *)arg0 + 0x990), &arg0->unk970, sp10);
    func_8006BDBC_6C9BC((func_8005E800_5F400_arg *)((u8 *)arg0 + 0x9B0), sp10, sp30);
    transformVector2(&D_800BAB3C_AA9EC, sp30, sp50);

    arg0->unk44C += sp50[0];
    arg0->unk450 += sp50[1] + arg0->unkAB8;
    arg0->unk454 += sp50[2];

    return 1;
}

void func_800B27FC_A26AC(func_800B30B0_arg *arg0) {
    arg0->unkBAC = 0;
    arg0->unkBAA = 0;
    arg0->unkBAE = 0;
    arg0->unkBB5 = 0;
    arg0->unkBD5 = 0;
    arg0->unkBB6 = 0;
}

void func_800B2818_A26C8(func_800B30B0_arg *arg0, s8 arg1) {
    arg0->unkBAC = arg0->unkBAC + D_800BAB48_AA9F8[arg0->unkBB5];
    arg0->unkBAA = arg0->unkBAA + D_800BAB58_AAA08[arg0->unkBB5];

    if (arg0->unkBAE == 0) {
        arg0->unkBAE = arg0->unkBAE | (1 << arg1);
    } else {
        if (!((arg0->unkBAE >> arg1) & 1)) {
            arg0->unkBAC += 100;
            arg0->unkBAA += 10;
        }

        arg0->unkBAE |= (1 << arg1);

        if (arg0->unkBAE == 0xFF) {
            arg0->unkBAC += 1300;
            arg0->unkBAA += 565;
            arg0->unkBAE |= 0x100;
        }
    }

    arg0->unkBB5++;
    if (arg0->unkBB5 >= 8) {
        arg0->unkBB5 = 7;
    }

    if (arg0->unkBAC >= 10000) {
        arg0->unkBAC = 9999;
    }

    if (arg0->unkBAA >= 1000) {
        arg0->unkBAA = 999;
    }
}

void func_800B2950_A2800(func_800B30B0_arg *arg0, s32 arg1) {
    s8 temp_a0;
    s8 temp_v1;

    arg0->unkBAC += 0x1E;
    arg0->unkBAA += 1;
    temp_a0 = arg0->unkBB6;
    temp_v1 = temp_a0;

    if (temp_a0 == 0) {
        arg0->unkBB6 = temp_v1 | (1 << arg1);
    } else {
        if (!((temp_a0 >> (s8)arg1) & 1)) {
            arg0->unkBAC += 0x14;
            arg0->unkBAA += 1;
        }

        arg0->unkBB6 = arg0->unkBB6 | (1 << (s8)arg1);

        if ((s8)arg0->unkBB6 == 0xF) {
            arg0->unkBAC += 0x14;
            arg0->unkBAA += 3;
            arg0->unkBB6 |= 0x10;
        }
    }

    if ((s16)arg0->unkBAC >= 10000) {
        arg0->unkBAC = 9999;
    }

    if ((s16)arg0->unkBAA >= 1000) {
        arg0->unkBAA = 999;
    }
}

extern void func_80059ADC_5A6DC(void *);
s32 func_800B42A8_A4158(func_800B30B0_arg *, s32, s32, s32);

s32 func_800B2A3C_A28EC(func_800B30B0_arg *arg0) {
    s32 pad[16];

    if (arg0->unkB84 & 1) {
        if (arg0->unkBC0 < 3) {
            func_800B27FC_A26AC(arg0);
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
            func_800B00D4_9FF84(arg0, 1);
            return 1;
        }
    } else {
        arg0->unk450 = arg0->unk450 - arg0->unkAB8;
        func_800B40D4_A3F84(arg0);
        func_800B42A8_A4158(arg0, 0x200, 0x200, arg0->unkAB0);
        func_800B00FC_9FFAC(arg0);
        arg0->unkBC0 = arg0->unkBC0 - 1;
        if (arg0->unkBC0 == 0) {
            func_800B00D4_9FF84(arg0, 0);
        }
        func_8005D308_5DF08(arg0, 3);
        func_80058CFC_598FC(arg0);
        return 0;
    }
}

void func_800B2B3C_A29EC(func_800B30B0_arg *arg0) {
    s32 temp_v1;
    u16 temp_a1;
    s16 var_v1;

    temp_v1 = arg0->unkB84;
    if (!(temp_v1 & 0x1000)) {
        if (arg0->unkBDA != 0) {
            func_800B9B90_A9A40();
            var_v1 = func_8006D21C_6DE1C(arg0->unkA7C, arg0->unkA84, (s32)arg0->unk434, arg0->unk43C);
            temp_a1 = (u16)arg0->unkA94;
            var_v1 = (var_v1 - temp_a1) & 0x1FFF;
            if (var_v1 >= 0x1001) {
                var_v1 = var_v1 | 0xE000;
            }
            if (var_v1 >= 0x41) {
                var_v1 = 0x40;
            }
            if (var_v1 < -0x40) {
                var_v1 = -0x40;
            }
            arg0->unkA94 = temp_a1 + var_v1;
        } else {
            arg0->unkA94 = (u16)arg0->unkA94 - (arg0->unkB7A * 4);
        }
        func_80058CFC_598FC(arg0);
        return;
    }
    if (temp_v1 & 0x4000) {
        func_800516B4_522B4(arg0);
    }
}

s32 func_800B2C18_A2AC8(func_800B30B0_arg *arg0) {
    GameState *state;
    s32 flags;
    u8 stateUnk7A;
    s16 var_a0;
    s16 var_v1;
    s32 temp_v0;

    state = getCurrentAllocation();
    flags = arg0->unkB84;

    if (flags & 1) {
        return 0;
    }

    if (flags & 0x1000) {
        func_800B46BC_A456C((func_800B00D4_arg *)arg0);
        return 1;
    }

    if (flags & 0x80000) {
        goto skip_to_end;
    }

    stateUnk7A = state->unk7A;
    if (stateUnk7A != 0 && stateUnk7A != 8) {
        if (!(stateUnk7A == 9 || stateUnk7A == 10)) {
            arg0->unkBAC = 0;
        }
    }

    if (arg0->unkBAC != 0) {
        func_80059A48_5A648((Player *)arg0, arg0->unkBAC);
        if (arg0->unkBC7 == 0) {
            func_8004D890_4E490(arg0->unkBB8, (void *)arg0->unkBAC);

            var_v1 = arg0->unkBAC;
            if (var_v1 < 0x96) {
                var_a0 = 0x11C;
            } else {
                var_a0 = 0x11D;
            }

            if (var_v1 >= 0x12C) {
                var_a0 = 0x11E;
            }
            if (var_v1 >= 0x190) {
                var_a0 = 0x11F;
            }
            if (var_v1 >= 0x1F4) {
                var_a0 = 0x120;
            }
            if (var_v1 >= 0xBB8) {
                var_a0 = 0x121;
            }

            func_80058530_59130(var_a0, 6);
        }
    }

    if (state->unk7A == 6) {
        if (arg0->unkBAA != 0) {
            func_8004FCF0_508F0(arg0->unkBAA);
            func_80059A88_5A688((Player *)arg0, arg0->unkBAA);

            var_v1 = arg0->unkBAA;
            if (var_v1 < 0xF) {
                var_a0 = 0x11C;
            } else {
                var_a0 = 0x11D;
            }

            if (var_v1 >= 0x1E) {
                var_a0 = 0x11E;
            }
            if (var_v1 >= 0x37) {
                var_a0 = 0x11F;
            }
            if (var_v1 >= 0x5F) {
                var_a0 = 0x120;
            }
            if (var_v1 >= 0x3E7) {
                var_a0 = 0x121;
            }

            func_80058530_59130(var_a0, 6);
        }
    }

skip_to_end:
    if (arg0->unkBAE != 0) {
        func_80059BD4_5A7D4(arg0);
        func_8005D804_5E404(arg0, 1, 0xF);
    }

    func_800B00D4_9FF84(arg0, 3);
    return 1;
}

void func_800B2DDC_A2C8C(func_800B30B0_arg *arg0) {
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

extern void func_8005D810_5E410(func_800B30B0_arg *);

void func_800B2E80_A2D30(func_800B30B0_arg *arg0) {
    arg0->unkB8C = 0x200;
    arg0->unkA96 = 0;
    arg0->unkA98 = 0;
    arg0->unkA9A = 0;
    arg0->unkBCD = -1;
    arg0->unkBC0++;
    arg0->unkB84 |= 0x4000;
    func_8005D810_5E410(arg0);
    func_80056B7C_5777C(&arg0->unk434, 0xB);
}

extern s8 func_8005D818_5E418(func_800B30B0_arg *arg0);

void func_800B2EE4_A2D94(func_800B30B0_arg *arg0, s8 arg1) {
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

s32 func_800B2FD0_A2E80(func_800B30B0_arg *arg0) {
    if (func_800B2C18_A2AC8(arg0) != 0) {
        return 1;
    }

    func_800B2EE4_A2D94(arg0, 0);
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

    func_800B2EE4_A2D94(arg0, 1);
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

    func_800B2EE4_A2D94(arg0, 2);
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

    func_800B2EE4_A2D94(arg0, 3);
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

    func_800B2EE4_A2D94(arg0, 4);
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

    func_800B2EE4_A2D94(arg0, 5);
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

    func_800B2EE4_A2D94(arg0, 6);
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

    func_800B2EE4_A2D94(arg0, 7);
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

s32 func_800B36F0_A35A0(func_800B30B0_arg *arg0) {
    u16 temp_a1 = arg0->unkB7E;
    s32 var_v1;

    if (temp_a1 & 0x8) {
        var_v1 = 0x17;
    } else {
        var_v1 = 0;
    }

    if (temp_a1 & 0x4) {
        var_v1 = 0x19;
    }

    if (temp_a1 & 0x1) {
        var_v1 = 0x15;
        if (arg0->unkB84 & 0x2) {
            var_v1 = 0x13;
        }
    }

    if ((arg0->unkB7E & 0x2)) {
        var_v1 = 0x13;
        if (arg0->unkB84 & 0x2) {
            var_v1 = 0x15;
        }
    }

    if (arg0->unkBDA != 0) {
        u8 temp_a1_2 = arg0->unkBDC;
        if (temp_a1_2 & 0x8) {
            arg0->unkBDC = temp_a1_2 & 0xF7;
            var_v1 = 0x15;
        }
    }

    return var_v1;
}

void func_800B3784_A3634(func_800B30B0_arg *arg0) {
    s32 result;
    u8 temp;

    switch ((u8)arg0->unkBD5) {
        default:
            break;

        case 0:
            arg0->unkBD5++;
            arg0->unkBD6 = 4;
        case 1:
            result = func_800B36F0_A35A0(arg0);
            if (result != 0) {
                arg0->unkBD6 = result;
                arg0->unkBD7 = 0;
                arg0->unkBD5 = arg0->unkBD5 + 1;
                arg0->unkB84 |= 0x8000;
            }
            func_8005D308_5DF08(arg0, arg0->unkBD6);
            break;

        case 2:
            result = func_8005D308_5DF08(arg0, arg0->unkBD6);
            if (result != 0) {
                if (arg0->unkBD6 == 0x17) {
                    func_800B2950_A2800(arg0, 0);
                }

                if (arg0->unkBD6 == 0x19) {
                    func_800B2950_A2800(arg0, 1);
                }

                if (arg0->unkBD6 == 0x15) {
                    func_800B2950_A2800(arg0, 2);
                }

                if (arg0->unkBD6 == 0x13) {
                    func_800B2950_A2800(arg0, 3);
                }

                arg0->unkBD5++;
                arg0->unkBD6++;
            }

            result = func_800B36F0_A35A0(arg0);
            if (result != 0) {
                arg0->unkBD7 = result;
            }

            break;

        case 3:
            result = func_8005D308_5DF08(arg0, arg0->unkBD6);
            if (result != 0) {
                if (arg0->unkBD7 != 0) {
                    arg0->unkBD5 = 2;
                    arg0->unkBD6 = arg0->unkBD7;
                    arg0->unkBD7 = 0;
                    break;
                } else {
                    arg0->unkB84 &= 0xFFFF7FFF;
                    arg0->unkBD5 = 1;
                }
            }

            result = func_800B36F0_A35A0(arg0);
            if (result != 0) {
                arg0->unkBD7 = result;
            }
            break;
    }

    if (arg0->unkB84 & 0xC000) {
        arg0->unkB84 |= 0x1000;
    } else {
        arg0->unkB84 &= ~0x1000;
    }
}

void func_800B3950_A3800(func_800B00D4_arg *arg0) {
    D_800BAB68_AAA18[arg0->unkBBF](arg0);
}

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B3980_A3830);

typedef struct {
    u8 pad[0x176C];
    s32 unk176C;
} GameStateUnk10;

typedef struct {
    u8 pad[0x10];
    GameStateUnk10 *unk10;
    u8 pad2[0x44];
    u8 unk58;
    u8 unk59;
    u8 unk5A;
    u8 pad3[0x1F];
    u8 unk7A;
    u8 unk7B;
    u8 unk7C;
    u8 unk7D;
} GameStateLocal;

s32 func_800B3CBC_A3B6C(func_800B30B0_arg *arg0) {
    s32 var_s2 = 0;
    GameStateLocal *temp_s0;

    temp_s0 = getCurrentAllocation();
    arg0->unk44C = 0;
    arg0->unk454 = 0;
    arg0->unk450 -= 0x6000;
    func_800B40D4_A3F84(arg0);
    func_800B02AC_A015C(arg0);
    func_8005D180_5DD80(arg0, 0);
    if (temp_s0->unk7B != 0) {
        switch (temp_s0->unk7A) {
            case 0:
            case 1:
            case 8:
            case 9:
                if (arg0->unkBC4 == 0) {
                    var_s2 = 1;
                }
                break;
            case 2:
            case 3:
                if (temp_s0->unk10->unk176C & 0x100000) {
                    var_s2 = 1;
                }
                break;
            case 4:
                if (temp_s0->unk7D == 0) {
                    var_s2 = 1;
                }
                break;
            case 5:
                if ((temp_s0->unk7D == 0) && (temp_s0->unk5A == 0x14)) {
                    var_s2 = 1;
                }
                break;
            case 6:
                if ((temp_s0->unk7D == 0) && (arg0->unkB70 >= 0x12C)) {
                    var_s2 = 1;
                }
                break;
        }
        if (var_s2 != 0) {
            arg0->unkBBF = 2;
        } else {
            arg0->unkBBF = 3;
        }
        arg0->unkBC0 = 0;
    }
    return 0;
}

extern void func_80059CE0_5A8E0(void *);
extern void func_8005D804_5E404(void *, u8, u8);

s32 func_800B3DF0_A3CA0(func_800B30B0_arg *arg0) {
    arg0->unk44C = 0;
    arg0->unk454 = 0;
    arg0->unk450 = arg0->unk450 - 0x6000;
    func_800B40D4_A3F84(arg0);
    func_800B02AC_A015C(arg0);

    if (arg0->unkAE0 <= 0xDFFFF) {
        arg0->unkAE0 = arg0->unkAE0 + 0x8000;
    }

    switch (arg0->unkBC0) {
        case 0:
            if (func_8005D308_5DF08(arg0, 0x1B) != 0) {
                arg0->unkBC0 = arg0->unkBC0 + 1;
                func_80059CE0_5A8E0(arg0);
            }
            break;
        case 1:
            if (func_8005D308_5DF08(arg0, 0x1D) != 0) {
                arg0->unkB8C = 0x14;
                arg0->unkBC0 = arg0->unkBC0 + 1;
            }
            break;
        case 2:
            if (arg0->unkB8C != 0) {
                arg0->unkB8C = arg0->unkB8C - 1;
            } else {
                arg0->unkBC0 = arg0->unkBC0 + 1;
            }
            break;
        case 3:
            if (func_8005D308_5DF08(arg0, 0x1C) == 0) {
                goto end;
            }
            arg0->unkBC0 = arg0->unkBC0 + 1;
            break;
        case 4:
            if (func_8005D308_5DF08(arg0, 0x1D) != 0) {
                arg0->unkBC0 = 2;
                arg0->unkB8C = 0x14;
            }
            break;
    }

end:
    func_8005D804_5E404(arg0, 1, 0);
    return 0;
}

s32 func_800B3F2C_A3DDC(func_800B30B0_arg *arg0) {
    u32 temp_v0;
    s32 temp_v1;
    s32 temp_v0_2;
    u8 state;
    s32 temp;

    temp_v0 = arg0->unkB84;
    temp_v1 = arg0->unk450;

    arg0->unk44C = 0;
    arg0->unk454 = 0;

    temp_v0 |= 0x60;
    temp_v1 += -0x6000;

    arg0->unkB84 = temp_v0;
    arg0->unk450 = temp_v1;

    func_800B40D4_A3F84(arg0);
    func_800B02AC_A015C(arg0);

    temp = arg0->unkAE0;
    if (!(0xDFFFF < temp)) {
        arg0->unkAE0 = temp + 0x8000;
    }

    state = arg0->unkBC0;

    switch (state) {
        case 0:
            if (func_8005D308_5DF08(arg0, 0x1E)) {
                temp_v0_2 = arg0->unkBC0;
                temp_v0_2++;
                arg0->unkBC0 = temp_v0_2;
            }
            break;

        case 1:
            if (func_8005D308_5DF08(arg0, 0x20)) {
                temp_v1 = 0x14;
                temp_v0_2 = arg0->unkBC0;
                arg0->unkB8C = temp_v1;
                temp_v0_2++;
                arg0->unkBC0 = temp_v0_2;
            }
            break;

        case 2:
            temp_v0_2 = arg0->unkB8C;
            if (temp_v0_2 != 0) {
                temp_v0_2--;
                arg0->unkB8C = temp_v0_2;
            } else {
                temp_v0_2 = arg0->unkBC0;
                temp_v0_2++;
                arg0->unkBC0 = temp_v0_2;
            }
            break;

        case 3:
            if (func_8005D308_5DF08(arg0, 0x1F)) {
                temp_v0_2 = arg0->unkBC0;
                temp_v0_2++;
                arg0->unkBC0 = temp_v0_2;
            }
            break;

        case 4:
            if (func_8005D308_5DF08(arg0, 0x20)) {
                arg0->unkBC0 = 2;
                arg0->unkB8C = 0x14;
            }
            break;
    }

    func_8005D804_5E404(arg0, 4, 0);

    return 0;
}

extern Mat3x3Padded D_8009A890_9B490;
extern s32 D_8009A8A4_9B4A4;
extern s32 D_8009A8A8_9B4A8;
extern s32 D_8009A8AC_9B4AC;

void func_800B4058_A3F08(func_800B30B0_arg *arg0) {
    Mat3x3Padded sp10;
    s16 *a98;
    s16 *a96;

    D_8009A8A4_9B4A4 = 0;
    D_8009A8A8_9B4A8 = 0xFFF00000;
    D_8009A8AC_9B4AC = 0;
    a98 = &arg0->unkA98;
    a96 = &arg0->unkA96;
    createRotationMatrixYX(&sp10, *a98, *a96);
    sp10.unk14 = 0;
    sp10.unk18 = 0x100000;
    sp10.unk1C = 0;
    func_8006B084_6BC84(&D_8009A890_9B490, &sp10, &arg0->padding3[0x28]);
    arg0->unkB84 |= 0x800;
}

void func_800B40D4_A3F84(func_800B30B0_arg *arg0) {
    s32 temp;
    s32 delta;
    s32 value;
    s32 value2;
    s32 temp_a2;
    s32 var_v1;
    s32 sp[4];

    temp = (u16)arg0->unkA92 & 0x1FFF;
    arg0->unkA92 = temp;
    if (temp >= 0x1001) {
        arg0->unkA92 = temp - 0x2000;
    }
    value = arg0->unkA92;
    delta = -value, value2 = value;
    if (delta >= 0x81) {
        delta = 0x80;
    }
    if (delta < -0x80) {
        delta = -0x80;
    }
    arg0->unkA92 = value2 + delta;

    temp = (u16)arg0->unkA90 & 0x1FFF;
    arg0->unkA90 = temp;
    if (temp >= 0x1001) {
        arg0->unkA90 = temp - 0x2000;
    }
    value = arg0->unkA90;
    delta = -value, value2 = value;
    if (delta >= 0x81) {
        delta = 0x80;
    }
    if (delta < -0x80) {
        delta = -0x80;
    }
    temp_a2 = arg0->unk9A4;
    arg0->unkA90 = value2 + delta;
    var_v1 = -temp_a2;
    if (var_v1 > 0x8000) {
        var_v1 = 0x8000;
    }
    if (var_v1 < -0x8000) {
        var_v1 = -0x8000;
    }
    arg0->unk9A4 = temp_a2 + var_v1;
}

void func_800B419C_A404C(func_800B30B0_arg *arg0) {
    s16 temp;
    s16 value;
    s32 delta, value2;
    s32 temp2, delta2, value3;
    s16 result;
    s32 pad[5];

    // Handle unkA92
    temp = arg0->unkA92 & 0x1FFF;
    arg0->unkA92 = temp;
    if (!(temp < 0x1001)) {
        arg0->unkA92 = temp - 0x2000;
    }

    value = arg0->unkA92;
    delta = -value, value2 = value;
    if (!(delta < 0x29)) {
        delta = 0x28;
    }
    if (delta < -0x28) {
        delta = -0x28;
    }
    arg0->unkA92 = value2 + delta;

    // Handle unkA8E
    temp = arg0->unkA8E & 0x1FFF;
    arg0->unkA8E = temp;
    if (!(temp < 0x1001)) {
        arg0->unkA8E = temp - 0x2000;
    }

    value = arg0->unkA8E;
    delta = -value, value2 = value;
    if (!(delta < 0x29)) {
        delta = 0x28;
    }
    if (delta < -0x28) {
        delta = -0x28;
    }
    arg0->unkA8E = value2 + delta;

    // Handle unkA90
    temp = arg0->unkA90 & 0x1FFF;
    arg0->unkA90 = temp;
    if (!(temp < 0x1001)) {
        arg0->unkA90 = temp - 0x2000;
    }

    value = arg0->unkA90;
    delta = -value, value2 = value;
    if (!(delta < 0x29)) {
        delta = 0x28;
    }
    if (delta < -0x28) {
        delta = -0x28;
    }
    result = value2 + delta;

    // Handle unk9A4
    temp2 = arg0->unk9A4;
    arg0->unkA90 = result;
    delta2 = -temp2, value3 = temp2;
    if (!(delta2 < 0x6001)) {
        delta2 = 0x6000;
    }
    if (delta2 < -0x6000) {
        delta2 = -0x6000;
    }
    arg0->unk9A4 = value3 + delta2;
}

extern s16 identityMatrix[];

s32 func_800B42A8_A4158(func_800B30B0_arg *arg0, s32 arg1, s32 arg2, s32 arg3) {
    Vec3s32 sp10;
    Vec3s32 sp20;
    Vec3s32 sp30;
    Mat3x3Padded sp40;
    Mat3x3Padded sp60;
    Mat3x3Padded sp80;
    Mat3x3Padded spA0;
    s32 temp;
    s32 temp2;
    s32 resultY;

    createYRotationMatrix(&sp40, arg0->unkA90 + arg0->unkA94);

    sp30.unk0 = sp40.m[2][0];
    sp30.unk4 = sp40.m[2][1];
    sp30.unk8 = sp40.m[2][2];

    memcpy(&sp60, identityMatrix, 0x20);
    memcpy(&sp80, identityMatrix, 0x20);

    temp2 = approximate_sqrt(arg0->unk458 * arg0->unk458 + arg0->unk460 * arg0->unk460);
    temp = temp2 & 0xFFFF;

    if (temp != 0) {
        sp80.m[0][0] = (arg0->unk460 << 13) / temp;
        sp80.m[0][2] = (arg0->unk458 << 13) / temp;
        sp80.m[2][0] = (-arg0->unk458 << 13) / temp;
        sp80.m[2][2] = (arg0->unk460 << 13) / temp;
    }

    sp60.m[1][1] = arg0->unk45C;
    sp60.m[1][2] = -temp;
    sp60.m[2][1] = temp;
    sp60.m[2][2] = arg0->unk45C;

    func_8006BDBC_6C9BC((void *)&sp80, &sp60, &spA0);
    transformVector2(&arg0->unk44C, &spA0, &sp10.unk0);

    if (sp10.unk4 < 0) {
        sp10.unk4 = 0;
    }

    transformVector2(&sp30.unk0, &spA0, &sp20.unk0);
    memcpy(&sp80, identityMatrix, 0x20);

    temp2 = approximate_sqrt(sp20.unk0 * sp20.unk0 + sp20.unk8 * sp20.unk8);
    temp = temp2 & 0xFFFF;

    if (temp != 0) {
        sp80.m[0][0] = (sp20.unk8 << 13) / temp;
        sp80.m[2][2] = (sp20.unk8 << 13) / temp;
        sp80.m[0][2] = (sp20.unk0 << 13) / temp;
        sp80.m[2][0] = (-sp20.unk0 << 13) / temp;
    }

    transformVector2(&sp10.unk0, &sp80, &sp20.unk0);

    if (sp20.unk0 > 0) {
        if (arg3 < sp20.unk0) {
            sp20.unk0 = sp20.unk0 - arg3;
        } else {
            sp20.unk0 = 0;
        }
    } else {
        if (sp20.unk0 < -arg3) {
            sp20.unk0 = sp20.unk0 + arg3;
        } else {
            sp20.unk0 = 0;
        }
    }

    if (sp20.unk8 > 0) {
        if (arg1 < sp20.unk8) {
            sp20.unk8 = sp20.unk8 - arg1;
        } else {
            sp20.unk8 = 0;
        }
    } else {
        if (sp20.unk8 < -arg2) {
            sp20.unk8 = sp20.unk8 + arg2;
        } else {
            sp20.unk8 = 0;
        }
    }

    resultY = sp20.unk8;
    transformVector3(&sp20.unk0, &sp80, &sp10.unk0);
    transformVector3(&sp10.unk0, &spA0, &sp20.unk0);

    arg0->unk44C = sp20.unk0;
    arg0->unk454 = sp20.unk8;

    return resultY;
}

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

extern s16 func_8005CE98_5DA98(func_800B30B0_arg *);

s32 func_800B470C_A45BC(func_800B30B0_arg *arg0) {
    s32 stackVec[3];
    GameState *allocation;
    s16 angle;
    s16 delta;
    s32 vel1;
    s32 vel2;
    s16 rotAngle;

    allocation = getCurrentAllocation();

    if (arg0->unkBBF == 0) {
        arg0->unkA8C = 0xFFFF;
        arg0->unkB88 = 0;
        arg0->unk44C = arg0->unkAC8;
        arg0->unk454 = arg0->unkAD0;
        arg0->unkBBF++;
        rotAngle = atan2Fixed(-arg0->unkAC8, -arg0->unkAD0);
        rotateVectorY((u8 *)allocation->unk48 + 0xE4, rotAngle, stackVec);
        stackVec[0] += (s32)arg0->unk434;
        stackVec[2] += arg0->unk43C;
        stackVec[1] = arg0->unk438 + 0x100000;
        func_80056B7C_5777C(&arg0->unk434, 0xD);
        func_80050ECC_51ACC(stackVec);
    }

    angle = (func_8005CE98_5DA98(arg0) - arg0->unkA94) & 0x1FFF;
    delta = angle;
    if (angle >= 0x1001) {
        delta = angle | 0xE000;
    }
    if (delta >= 0x101) {
        delta = 0x100;
    }
    if (delta < -0x100) {
        delta = -0x100;
    }
    arg0->unkA94 = arg0->unkA94 + delta;
    arg0->unk450 -= 0x6000;
    vel1 = arg0->unk44C;
    arg0->unk44C = vel1 - (vel1 >> 4);
    vel2 = arg0->unk454;
    arg0->unk454 = vel2 - (vel2 >> 4);
    func_800B419C_A404C(arg0);
    func_800B02AC_A015C(arg0);
    if (func_8005D308_5DF08(arg0, 8) != 0) {
        arg0->unkAA8 = arg0->unkAA8 / 2;
        func_800B00E4_9FF94(arg0);
    }
    func_8005D804_5E404(arg0, 3, 0);
    return 0;
}

s32 func_800B48AC_A475C(func_800B30B0_arg *arg0) {
    s32 stackVec[3];
    GameState *allocation;
    s16 angle;
    s16 delta;
    s32 vel1;
    s32 vel2;
    s16 rotAngle;

    allocation = getCurrentAllocation();

    if (arg0->unkBBF == 0) {
        arg0->unkA8C = 0xFFFF;
        arg0->unkB88 = 0;
        arg0->unk44C = arg0->unkAC8;
        arg0->unk454 = arg0->unkAD0;
        arg0->unkBBF++;
        rotAngle = atan2Fixed(-arg0->unkAC8, -arg0->unkAD0);
        rotateVectorY((u8 *)allocation->unk48 + 0xE4, rotAngle, stackVec);
        stackVec[0] += (s32)arg0->unk434;
        stackVec[2] += arg0->unk43C;
        stackVec[1] = arg0->unk438 + 0x100000;
        func_80056B7C_5777C(&arg0->unk434, 0xD);
        func_80050ECC_51ACC(stackVec);
    }

    angle = (func_8005CE98_5DA98(arg0) - arg0->unkA94) & 0x1FFF;
    delta = angle;
    if (angle >= 0x1001) {
        delta = angle | 0xE000;
    }
    if (delta >= 0x101) {
        delta = 0x100;
    }
    if (delta < -0x100) {
        delta = -0x100;
    }
    arg0->unkA94 = arg0->unkA94 + delta;
    arg0->unk450 -= 0x6000;
    vel1 = arg0->unk44C;
    arg0->unk44C = vel1 - (vel1 >> 4);
    vel2 = arg0->unk454;
    arg0->unk454 = vel2 - (vel2 >> 4);
    func_800B419C_A404C(arg0);
    func_800B02AC_A015C(arg0);
    if (func_8005D308_5DF08(arg0, 7) != 0) {
        arg0->unkAA8 = arg0->unkAA8 / 2;
        func_800B00E4_9FF94(arg0);
    }
    func_8005D804_5E404(arg0, 3, 0);
    return 0;
}

s32 func_800B4A4C_A48FC(func_800B30B0_arg *arg0) {
    arg0->unkB88 = 1;
    arg0->unkB84 = arg0->unkB84 | 0x20;
    arg0->unk450 = arg0->unk450 - 0x6000;
    arg0->unk44C = arg0->unk44C - (arg0->unk44C >> 6);
    arg0->unk454 = arg0->unk454 - (arg0->unk454 >> 6);
    func_800B40D4_A3F84(arg0);
    func_800B02AC_A015C(arg0);

    if (func_8005D308_5DF08(arg0, 9) != 0) {
        if ((arg0->unkB84 & 1) == 0) {
            func_800B00D4_9FF84(arg0, 3);
        }
    }

    arg0->unkB84 = arg0->unkB84 | 0x10000;
    func_8005D804_5E404(arg0, 3, 0);
    return 0;
}

extern void func_80051C08_52808(void *, s32);

typedef struct {
    u8 _pad0[0xBDF];
    u8 unkBDF;
    u8 unkBE0;
    u8 unkBE1;
} func_800B7B64_arg;

extern void func_800B7B44_A79F4(func_800B7B64_arg *, s32);

s32 func_800B4B08_A49B8(func_800B30B0_arg *arg0) {
    s64 distSq;
    s32 dist;

    if (arg0->unkBBF == 0) {
        arg0->unkB8C = 0x14;
        arg0->unkBBF++;
    }

    arg0->unkB84 |= 0x60;
    arg0->unk450 -= 0x6000;
    arg0->unkB88 = 1;

    if (arg0->unkB84 & 1) {
        arg0->unkA8C = -1;
        arg0->unkB8C = 0xA;
        arg0->unkBC0 = 0;
        arg0->unk44C -= (arg0->unk44C >> 5);
        arg0->unk454 -= (arg0->unk454 >> 5);
    } else {
        if (arg0->unkBC0 == 0) {
            arg0->unkBC0++;
            func_800B7B44_A79F4((func_800B7B64_arg *)arg0, 0);
            func_80056B7C_5777C(&arg0->unk434, 0x25);
            func_80051C08_52808(arg0, arg0->unkBCC & 0xF);
        }
        func_800B42A8_A4158(arg0, 0x8000, 0x8000, 0x8000);
    }

    func_800B40D4_A3F84(arg0);
    func_800B02AC_A015C(arg0);

    if (func_8005D308_5DF08(arg0, 0xA) != 0) {
        if ((arg0->unkB84 & 1) == 0) {
            if (arg0->unkB8C == 0) {
                distSq = (s64)arg0->unk44C * arg0->unk44C + (s64)arg0->unk454 * arg0->unk454;
                dist = isqrt64(distSq);

                if (dist <= 0x1FFFF) {
                    arg0->unkB88 = 0;
                    func_800B00D4_9FF84(arg0, 4);
                }
            } else {
                arg0->unkB8C--;
            }
        }
    }

    func_8005D804_5E404(arg0, 3, 0);
    return 0;
}

s32 func_800B4CB8_A4B68(func_800B30B0_arg *arg0) {
    s16 angle;
    s16 diff;
    s16 clampedDiff;

    if (arg0->unkBBF == 0) {
        arg0->unkB88 = 0;
        arg0->unkB8C = 0xF;
        arg0->unkBBF++;
        if (arg0->unk450 > 0) {
            arg0->unk450 = 0;
        }
    }

    if (arg0->unkB8C != 0) {
        arg0->unkB8C--;
        arg0->unkB84 |= 0x40;
    }

    angle = func_8005CE98_5DA98(arg0);
    diff = angle - arg0->unkA94;
    clampedDiff = diff & 0x1FFF;

    if (clampedDiff >= 0x1001) {
        clampedDiff |= 0xE000;
    }

    if (clampedDiff > 0x100) {
        clampedDiff = 0x100;
    }

    if (clampedDiff < -0x100) {
        clampedDiff = -0x100;
    }

    arg0->unkA94 += clampedDiff;
    arg0->unk44C = 0;
    arg0->unk454 = 0;
    arg0->unk450 -= 0x6000;

    func_800B40D4_A3F84(arg0);
    func_800B02AC_A015C(arg0);

    if (func_8005D308_5DF08(arg0, 0xB) != 0) {
        func_800B00E4_9FF94(arg0);
    }

    return 0;
}

extern s32 D_800BABD4_AAA84[3];
extern void func_80050C80_51880(void *, s32);
extern s32 func_8005D8C8_5E4C8(void *);

s32 func_800B4DB8_A4C68(func_800B30B0_arg *arg0) {
    s32 dist;
    s32 temp;

    if (arg0->unkBBF == 0) {
        arg0->unkB8C = 0x1E;
        arg0->unkBBF++;
    }

    arg0->unkB84 |= 0x60;
    arg0->unk450 -= 0x6000;
    arg0->unkB88 = 3;

    if (arg0->unkB84 & 1) {
        arg0->unk44C -= arg0->unk44C >> 5;
        arg0->unk454 -= arg0->unk454 >> 5;
    } else {
        func_800B42A8_A4158(arg0, 0x100, 0x100, 0x100);
        func_80050C80_51880(arg0, arg0->unkBCC & 0xF);
    }

    dist = distance_3d(arg0->unk44C, arg0->unk450, arg0->unk454);
    if (dist <= 0xFFFF) {
        if (arg0->unkB84 & 2) {
            rotateVectorY(D_800BABD4_AAA84, (s16)(arg0->unkA94 + 0x1000), &arg0->unk44C);
        } else {
            rotateVectorY(D_800BABD4_AAA84, arg0->unkA94, &arg0->unk44C);
        }
    }

    arg0->unkA94 = atan2Fixed(-arg0->unk44C, -arg0->unk454);
    if (arg0->unkB84 & 2) {
        arg0->unkA94 += 0x1000;
    }

    func_800B40D4_A3F84(arg0);
    func_800B02AC_A015C(arg0);
    func_8005D180_5DD80(arg0, 0xC);

    if (arg0->unkB8C == 0) {
        func_800B00D4_9FF84(arg0, 2);
    } else {
        temp = func_8005D8C8_5E4C8(arg0);
        arg0->unkB8C -= temp;
        if (arg0->unkB8C < 0) {
            arg0->unkB8C = 0;
        }
    }

    func_8005D804_5E404(arg0, 3, 0);

    return 0;
}

s32 func_800B4F5C_A4E0C(func_800B30B0_arg *arg0) {
    s16 angle;

    if (arg0->unkBBF != 0) {
        arg0->unkB8C = 0;
        arg0->unkBBF++;
    }

    arg0->unkB88 = 7;
    arg0->unkB84 |= 0x60;

    if (arg0->unk450 > 0) {
        arg0->unk450 = 0;
    }

    arg0->unk450 -= 0x6000;

    if (arg0->unkB84 & 1) {
        arg0->unk44C = arg0->unk44C - (arg0->unk44C >> 5);
        arg0->unk454 = arg0->unk454 - (arg0->unk454 >> 5);
    } else {
        func_800B42A8_A4158(arg0, 0x100, 0x100, 0x100);
        func_80050C80_51880(arg0, arg0->unkBCC & 0xF);
    }

    angle = atan2Fixed(-arg0->unk44C, -arg0->unk454);
    arg0->unkA94 = angle;

    if (arg0->unkB84 & 2) {
        arg0->unkA94 = angle + 0x1000;
    }

    func_800B40D4_A3F84(arg0);
    func_800B02AC_A015C(arg0);
    func_8005D180_5DD80(arg0, 0xC);

    if (arg0->unkBC9 != 1) {
        if (!(arg0->unkB84 & 1)) {
            arg0->unkB8C++;
            if (arg0->unkB8C >= 0xB) {
                func_800B00D4_9FF84(arg0, 5);
                goto skip;
            }
        }
    } else {
        arg0->unkB8C = 0;
    }
skip:
    func_8005D804_5E404(arg0, 3, 0);
    return 0;
}

s32 func_800B50C0_A4F70(func_800B30B0_arg *arg0) {
    s32 stackVec[3];
    void **allocation;
    u16 angle;
    s32 vel1;
    s32 vel2;

    allocation = getCurrentAllocation();

    if (arg0->unkBBF == 0) {
        arg0->unkBBF++;
        angle = arg0->unkAC4;
        arg0->unkA94 = angle;
        rotateVectorY(&arg0->unkAC8, (s16)angle, &arg0->unk44C);

        if (arg0->unkB84 & 2) {
            arg0->unkA94 += 0x1000;
        }

        rotateVectorY((u8 *)allocation[0x48 / sizeof(void *)] + 0xE4, arg0->unkA94, stackVec);

        stackVec[0] += (s32)arg0->unk434;
        stackVec[2] += arg0->unk43C;
        stackVec[1] = arg0->unk438 + 0x140000;

        func_80056B7C_5777C(&arg0->unk434, 0xD);
        func_80050ECC_51ACC(stackVec);
    }

    arg0->unkB88 = 1;
    arg0->unkB84 |= 0x20;
    arg0->unk450 -= 0x6000;

    vel1 = arg0->unk44C;
    arg0->unk44C = vel1 - (vel1 >> 6);

    vel2 = arg0->unk454;
    arg0->unk454 = vel2 - (vel2 >> 6);

    func_800B40D4_A3F84(arg0);
    func_800B02AC_A015C(arg0);

    if (func_8005D308_5DF08(arg0, 0xE) != 0) {
        if ((arg0->unkB84 & 1) == 0) {
            func_800B00D4_9FF84(arg0, 8);
        }
    }

    arg0->unkB84 |= 0x10000;
    func_8005D804_5E404(arg0, 3, 0);

    return 0;
}

s32 func_800B5234_A50E4(func_800B30B0_arg *arg0) {
    if (arg0->unkBBF == 0) {
        arg0->unk450 = 0;
        arg0->unkB8C = 0x14;
        arg0->unkBBF = arg0->unkBBF + 1;
    }

    arg0->unkB84 = arg0->unkB84 | 0x60;
    arg0->unkB88 = 1;
    arg0->unk450 = arg0->unk450 - 0x6000;

    if (arg0->unkB84 & 1) {
        arg0->unkA8C = 0xFFFF;
        arg0->unkB8C = 0xA;
        arg0->unkBC0 = 0;
        arg0->unk44C = arg0->unk44C - (arg0->unk44C >> 5);
        arg0->unk454 = arg0->unk454 - (arg0->unk454 >> 5);
    } else {
        if (arg0->unkBC0 == 0) {
            arg0->unkBC0++;
            func_800B7B44_A79F4((func_800B7B64_arg *)arg0, 0);
            func_80056B7C_5777C(&arg0->unk434, 0x25);
            func_80051C08_52808(arg0, arg0->unkBCC & 0xF);
        }
        func_800B42A8_A4158(arg0, 0x8000, 0x8000, 0x8000);
    }

    func_800B40D4_A3F84(arg0);
    func_800B02AC_A015C(arg0);

    if (func_8005D308_5DF08(arg0, 0xF) != 0) {
        if (!(arg0->unkB84 & 1)) {
            if (arg0->unkB8C == 0) {
                arg0->unkB88 = 0;
                func_800B00D4_9FF84(arg0, 0xD);
            } else {
                arg0->unkB8C = arg0->unkB8C - 1;
            }
        }
    }

    func_8005D804_5E404(arg0, 3, 0);
    return 0;
}

s32 func_800B5394_A5244(func_800B30B0_arg *arg0) {
    if (arg0->unkBBF == 0) {
        arg0->unkBBF = arg0->unkBBF + 1;
        func_80056B7C_5777C(&arg0->unk434, 0xD);
    }

    arg0->unkB88 = 8;
    arg0->unkB84 = arg0->unkB84 | 0x20;

    if (arg0->unk450 > 0) {
        arg0->unk450 = 0;
    }

    arg0->unk44C = arg0->unk44C / 2;
    arg0->unk454 = arg0->unk454 / 2;
    arg0->unk450 = arg0->unk450 - 0x6000;

    func_800B42A8_A4158(arg0, 0x8000, 0x8000, 0x8000);
    func_800B40D4_A3F84(arg0);
    func_800B02AC_A015C(arg0);

    if (func_8005D308_5DF08(arg0, 0xE) != 0) {
        func_800B00D4_9FF84(arg0, 0xA);
    }

    func_8005D804_5E404(arg0, 3, 0);
    return 0;
}

s32 func_800B5478_A5328(func_800B30B0_arg *arg0) {
    if (arg0->unkBBF == 0) {
        arg0->unk450 = 0;
        arg0->unkB8C = 0x14;
        arg0->unkBBF = arg0->unkBBF + 1;
        if ((arg0->unkB84 & 1) == 0) {
            func_80056B7C_5777C(&arg0->unk434, 0x25);
            func_80051C08_52808(arg0, arg0->unkBCC & 0xF);
        }
    }

    arg0->unkB88 = 8;
    arg0->unkB84 = arg0->unkB84 | 0x60;

    if (arg0->unk450 > 0) {
        arg0->unk450 = 0;
    }

    arg0->unk44C = arg0->unk44C / 2;
    arg0->unk450 = arg0->unk450 - 0x6000;
    arg0->unk454 = arg0->unk454 / 2;

    func_800B42A8_A4158(arg0, 0x8000, 0x8000, 0x8000);
    func_800B40D4_A3F84(arg0);
    func_800B02AC_A015C(arg0);

    if (func_8005D308_5DF08(arg0, 0xF) != 0) {
        if ((arg0->unkB84 & 1) == 0) {
            if (arg0->unkB8C == 0) {
                func_800B00D4_9FF84(arg0, 0xB);
            } else {
                arg0->unkB8C = arg0->unkB8C - 1;
            }
        }
    }

    func_8005D804_5E404(arg0, 3, 0);
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B55B4_A5464);

s32 func_800B58B8_A5768(func_800B30B0_arg *arg0) {
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

s32 func_800B5938_A57E8(func_800B30B0_arg *arg0) {
    s16 angle;
    s16 angleDelta;

    if (arg0->unkBBF == 0) {
        arg0->unkB88 = 0;
        arg0->unkB8C = 0xF;
        arg0->unkBBF++;
        if (arg0->unk450 > 0) {
            arg0->unk450 = 0;
        }
        arg0->unkA8C = 0xFFFF;
    }

    if (arg0->unkB8C != 0) {
        arg0->unkB8C--;
        arg0->unkB84 |= 0x40;
    }

    angle = func_8005CE98_5DA98(arg0);
    angleDelta = (angle - arg0->unkA94) & 0x1FFF;

    if (angleDelta >= 0x1001) {
        angleDelta |= 0xE000;
    }

    if (angleDelta >= 0x101) {
        angleDelta = 0x100;
    }

    if (angleDelta < -0x100) {
        angleDelta = -0x100;
    }

    arg0->unkA94 += angleDelta;
    arg0->unk44C = 0;
    arg0->unk454 = 0;
    arg0->unk450 -= 0x6000;

    func_800B40D4_A3F84(arg0);
    func_800B02AC_A015C(arg0);

    if (func_8005D308_5DF08(arg0, 0x10) != 0) {
        func_800B00E4_9FF94(arg0);
    }

    return 0;
}

extern s32 func_8004175C_4235C(void *);

s32 func_800B5A40_A58F0(func_800B30B0_arg *arg0) {
    s32 temp;

    if (!arg0->unkBBF) {
        temp = arg0->unkB84;
        arg0->unkBBF++;
        arg0->unk450 = 0xA0000;
        arg0->unkB88 = 0x20;
        arg0->unkB8C = 5;
        arg0->unkA8C = 0xFFFF;
        arg0->unkB84 = temp | 0x20;
    }

    if (arg0->unk450 > 0) {
        arg0->unk450 = arg0->unk450 - 0x8000;
    } else {
        if (arg0->unkBBF == 1) {
            if (func_8004175C_4235C(arg0) != 0) {
                arg0->unkBBF++;
            }
        }

        arg0->unk450 -= 0x800;

        if (func_8005D8C8_5E4C8(arg0) != 1) {
            arg0->unk450 += 0x100;
        }
    }

    arg0->unk44C = arg0->unk44C - (arg0->unk44C >> 6);
    arg0->unk454 = arg0->unk454 - (arg0->unk454 >> 6);

    func_800B419C_A404C(arg0);

    arg0->unkA94 += 0x40;

    func_800B0300_A01B0(arg0);
    func_8005D308_5DF08(arg0, 9);

    if (arg0->unkB8C != 0) {
        arg0->unkB8C--;
        if (arg0->unkB8C != 0) {
            goto skip;
        }
    }

    if ((arg0->unkB84 & 1) == 0) {
        func_800B00D4_9FF84(arg0, 4);
    }

skip:
    func_8005D804_5E404(arg0, 3, 0);

    return 0;
}

extern void func_800419E4_425E4(void *);

s32 func_800B5B90_A5A40(func_800B30B0_arg *arg0) {
    s32 temp_v1;
    u8 temp_v0;

    temp_v0 = arg0->unkBBF;
    if (temp_v0 == 0) {
        arg0->unkBBF = temp_v0 + 1;
        if (arg0->unk450 > 0) {
            arg0->unk450 = 0;
        }
        arg0->unkB88 = 0x40;
        arg0->unkB8C = 0x1E;
        arg0->unkA8C = 0xFFFF;
        arg0->unkB84 |= 0x20;
        arg0->unkBCE &= 0xFE;
        func_8005D308_5DF08(arg0, 0xE);
    }
    if (arg0->unkBBF == 1) {
        func_800419E4_425E4(arg0);
        arg0->unkBBF += 1;
    }
    arg0->unk44C = 0;
    arg0->unk454 = 0;
    arg0->unk450 -= 0x6000;
    func_800B40D4_A3F84(arg0);
    func_800B0300_A01B0(arg0);
    if (arg0->unkBCE & 1) {
        arg0->unkB8C = 0;
    }
    if (arg0->unkB8C == 0) {
        func_800B7B44_A79F4((func_800B7B64_arg *)arg0, 4);
        func_800B00D4_9FF84(arg0, 0x10);
    } else {
        temp_v1 = arg0->unkB8C - func_8005D8C8_5E4C8(arg0);
        arg0->unkB8C = temp_v1;
        if (temp_v1 < 0) {
            arg0->unkB8C = 0;
        }
    }
    func_8005D804_5E404(arg0, 3, 0);
    return 0;
}

s32 func_800B5CB8_A5B68(func_800B30B0_arg *arg0) {
    arg0->unkB88 = 1;
    arg0->unkB84 = arg0->unkB84 | 0x20;
    arg0->unk450 = arg0->unk450 - 0x6000;
    arg0->unk44C = arg0->unk44C - (arg0->unk44C >> 6);
    arg0->unk454 = arg0->unk454 - (arg0->unk454 >> 6);
    func_800B40D4_A3F84(arg0);
    func_800B0300_A01B0(arg0);

    if (func_8005D308_5DF08(arg0, 0xE) != 0) {
        if ((arg0->unkB84 & 1) == 0) {
            func_800B00D4_9FF84(arg0, 8);
        }
    }

    func_8005D804_5E404(arg0, 3, 0);
    return 0;
}

extern s32 func_800420A8_42CA8(void *);
extern s32 func_8005D8C8_5E4C8(void *);

s32 func_800B5D68_A5C18(func_800B30B0_arg *arg0) {
    if (arg0->unkBBF == 0) {
        arg0->unkB8C = 0x78;
        arg0->unkBBF = arg0->unkBBF + 1;
    }

    if (arg0->unkBBF == 1) {
        if (func_800420A8_42CA8(arg0) != 0) {
            arg0->unkBBF = arg0->unkBBF + 1;
        }
    }

    arg0->unkB88 = 0x80;
    arg0->unk450 = arg0->unk450 - arg0->unkAB8;
    func_800B40D4_A3F84(arg0);

    if (arg0->unkB84 & 1) {
        arg0->unk44C = arg0->unk44C - (arg0->unk44C >> 7);
        arg0->unk454 = arg0->unk454 - (arg0->unk454 >> 7);
    } else {
        func_800B42A8_A4158(arg0, 0x200, 0x200, arg0->unkAB0);
    }

    func_800B02AC_A015C(arg0);
    func_8005D308_5DF08(arg0, 0);

    if (arg0->unkB8C == 0) {
        arg0->unkB88 = 0;
        func_800B7B44_A79F4((func_800B7B64_arg *)arg0, 4);
        func_800B00E4_9FF94(arg0);
        return 0;
    } else {
        arg0->unkB8C = arg0->unkB8C - func_8005D8C8_5E4C8(arg0);
        if (arg0->unkB8C < 0) {
            arg0->unkB8C = 0;
        }
    }

    return 0;
}

extern void func_80050C00_51800(void *);
extern s32 func_8005D8C8_5E4C8(void *);

s32 func_800B5E90_A5D40(func_800B30B0_arg *arg0) {
    s16 angle;

    if (arg0->unkBBF == 0) {
        arg0->unkB8C = 0x1E;
        arg0->unkBBF = arg0->unkBBF + 1;
    }

    arg0->unkB84 = arg0->unkB84 | 0x60;
    arg0->unkB88 = 0x102;
    arg0->unk450 = arg0->unk450 - 0x6000;

    if (arg0->unkB84 & 1) {
        arg0->unk44C = arg0->unk44C - (arg0->unk44C >> 5);
        arg0->unk454 = arg0->unk454 - (arg0->unk454 >> 5);
    } else {
        func_800B42A8_A4158(arg0, 0x100, 0x100, 0x100);
    }

    angle = atan2Fixed(-arg0->unk44C, -arg0->unk454);
    arg0->unkA94 = angle;

    if (arg0->unkB84 & 2) {
        arg0->unkA94 = angle + 0x1000;
    }

    func_800B40D4_A3F84(arg0);
    func_800B02AC_A015C(arg0);
    func_8005D180_5DD80(arg0, 0xC);

    if (arg0->unkB8C == 0) {
        func_800B00D4_9FF84(arg0, 2);
    } else {
        arg0->unkB8C = arg0->unkB8C - func_8005D8C8_5E4C8(arg0);
        if (arg0->unkB8C < 0) {
            arg0->unkB8C = 0;
        }
    }

    func_8005D804_5E404(arg0, 3, 0);
    return 0;
}

s32 func_800B5FC4_A5E74(func_800B30B0_arg *arg0) {
    u8 temp;
    s32 velZ;
    s32 dist;

    temp = arg0->unkBBF;
    if (temp == 0) {
        arg0->unk44C = arg0->unkAC8;
        velZ = arg0->unkAD0;
        arg0->unkBBF = temp + 1;
        arg0->unk454 = velZ;
        dist = isqrt64((s64)arg0->unk44C * arg0->unk44C + (s64)velZ * velZ);
        if (dist > 0x60000) {
            arg0->unk44C = (s64)arg0->unk44C * 0x60000 / dist;
            arg0->unk454 = (s64)arg0->unk454 * 0x60000 / dist;
        }
        arg0->unk450 = 0x90000;
        arg0->unkA8C = 0xFFFF;
    }

    arg0->unkB88 = 0x200;
    arg0->unkB84 |= 0x20;
    arg0->unk450 += 0xFFFF7000;
    arg0->unk44C -= arg0->unk44C >> 6;
    arg0->unk454 -= arg0->unk454 >> 6;

    func_800B40D4_A3F84(arg0);
    func_800B0300_A01B0(arg0);

    if (func_8005D308_5DF08(arg0, 9) != 0) {
        if ((arg0->unkB84 & 1) == 0) {
            func_800B00D4_9FF84(arg0, 0x14);
        }
    }

    if (randA() & 1) {
        func_80050C00_51800(arg0);
    }

    func_8005D804_5E404(arg0, 3, 0);
    return 0;
}

s32 func_800B6194_A6044(func_800B30B0_arg *arg0) {
    s16 angle;

    if (arg0->unkBBF == 0) {
        arg0->unkB8C = 0x1E;
        arg0->unkBBF = arg0->unkBBF + 1;
    }

    arg0->unkB84 = arg0->unkB84 | 0x60;
    arg0->unk450 = arg0->unk450 - 0x6000;
    arg0->unkB88 = 0x202;

    if (arg0->unkB84 & 1) {
        arg0->unk44C = arg0->unk44C - (arg0->unk44C >> 5);
        arg0->unk454 = arg0->unk454 - (arg0->unk454 >> 5);
    } else {
        func_800B42A8_A4158(arg0, 0x100, 0x100, 0x100);
    }

    angle = atan2Fixed(-arg0->unk44C, -arg0->unk454);
    arg0->unkA94 = angle;

    if (arg0->unkB84 & 2) {
        arg0->unkA94 = angle + 0x1000;
    }

    func_800B40D4_A3F84(arg0);
    func_800B02AC_A015C(arg0);
    func_8005D180_5DD80(arg0, 0xC);

    if (arg0->unkB8C == 0) {
        func_800B00D4_9FF84(arg0, 2);
    } else {
        arg0->unkB8C = arg0->unkB8C - func_8005D8C8_5E4C8(arg0);
        if (arg0->unkB8C < 0) {
            arg0->unkB8C = 0;
        }
    }

    if (randA() & 1) {
        func_80050C00_51800(arg0);
    }

    func_8005D804_5E404(arg0, 3, 0);
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/9FF70", func_800B62E4_A6194);

extern s16 D_800BABEC_AAA9C;
extern u16 D_800BABEE_AAA9E;

s32 func_800B6488_A6338(func_800B30B0_arg *arg0) {
    u8 temp_v0;

    temp_v0 = arg0->unkBBF;
    if (temp_v0 == 0) {
        arg0->unkBBF = temp_v0 + 1;
        arg0->unkB8C = 0;
        arg0->unk450 = 0;
        arg0->unkA8C = 0xFFFF;
        func_8005D308_5DF08(arg0, 0xB);
        func_80056B7C_5777C(&arg0->unk434, 0x34);
        if (!(arg0->unkB84 & 1)) {
            func_80051C08_52808(arg0, arg0->unkBCC & 0xF);
        }
    }

    if (arg0->unkB8C == 0) {
        arg0->unkB8C = *(&D_800BABEC_AAA9C + (arg0->unkBC0 * 2));
        arg0->unkB9E = *(&D_800BABEE_AAA9E + (arg0->unkBC0 * 2));
        arg0->unkBC0 += 1;
    } else {
        s32 temp_v1;
        temp_v1 = arg0->unkB8C - func_8005D8C8_5E4C8(arg0);
        arg0->unkB8C = temp_v1;
        if (temp_v1 < 0) {
            arg0->unkB8C = 0;
        }
    }

    arg0->unkB88 = 0x10;
    arg0->unk44C = 0;
    arg0->unk454 = 0;
    arg0->unkB84 |= 0x40;
    arg0->unk450 -= 0x6000;
    func_800B40D4_A3F84(arg0);
    func_800B0300_A01B0(arg0);

    if (arg0->unkB8C == -1) {
        arg0->unkB88 = 0;
        func_800B00D4_9FF84(arg0, 4);
    }

    func_8005D804_5E404(arg0, 3, 0);

    return 0;
}

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

s32 func_800B67E4_A6694(func_800B30B0_arg *arg0) {
    D_80090F90_91B90_item *item;
    GameState *gameState = (GameState *)getCurrentAllocation();

    item = func_80055D10_56910(gameState->memoryPoolId);

    arg0->unk44C = 0;
    arg0->unk454 = 0;
    arg0->unk450 = arg0->unk450 - 0x6000;
    arg0->unk434 = arg0->unk434 + ((item->unk0 - (s32)arg0->unk434) >> 2);
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

s32 func_800B6D14_A6BC4(func_800B30B0_arg *arg0) {
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

extern s32 D_800BAC8C_AAB3C;
extern s32 D_800BAC98_AAB48;
extern void func_8006FDC8_709C8(u16 arg0, u8 arg1, u8 arg2);
extern void func_8004D364_4DF64(u8 arg0);

s32 func_800B6DB8_A6C68(func_800B30B0_arg *arg0) {
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

s32 func_800B6E5C_A6D0C(func_800B30B0_arg *arg0) {
    s32 sp10[3];
    s32 pad[8];
    GameState *gameState;
    D_80090F90_91B90_item *item;

    gameState = getCurrentAllocation();
    item = func_80055D10_56910(gameState->memoryPoolId);

    if (arg0->unkBC0 == 0) {
        arg0->unkA94 = 0x1000;
        arg0->unkB9C = 0;
        arg0->unkB94 = 0;
        arg0->unkB84 = arg0->unkB84 | 0x200;
        arg0->unkBC0++;
        arg0->unkBC5++;

        createYRotationMatrix(&arg0->unk970, 0x1000);
        transformVector2(&D_800BAC98_AAB48, &arg0->unk970, sp10);

        arg0->unk434 = (void *)(item->unkC + sp10[0]);
        arg0->unk438 = item->unk10 + sp10[1];
        arg0->unk43C = item->unk14 + sp10[2];

        memcpy(&arg0->unk440, &arg0->unk434, 0xC);

        arg0->unkBC3 = 1;
        arg0->unkB8C = 0x32;

        func_8006FDC8_709C8(arg0->unkBB8, 0, 0x10);

        if (arg0->unkBC7 == 0) {
            if (arg0->unkBC5 == gameState->unk74) {
                func_8004D364_4DF64(arg0->unkBB8);
            }
        }
    }

    func_800B40D4_A3F84(arg0);
    transformVector2(&D_800BAC8C_AAB3C, &arg0->unk970, &arg0->unk44C);
    func_800B02AC_A015C(arg0);

    if (arg0->unkB8C != 0) {
        arg0->unkB8C--;
    } else {
        arg0->unkB8C = 0x2C;
        arg0->unkBBF++;
    }

    func_8005D308_5DF08(arg0, 0xD);
    return 0;
}

s32 func_800B6FE8_A6E98(func_800B30B0_arg *arg0) {
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

s32 func_800B7078_A6F28(func_800B30B0_arg *arg0) {
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
        memcpy(&arg0->unk470, &arg0->unk434, 0xC);
        arg0->unk480 = 0;
        arg0->unk468 = 0x30000;
        arg0->unkB8C = 0;
    }

    return 0;
}

s32 func_800B7108_A6FB8(func_800B30B0_arg *arg0) {
    s32 var_v0;
    s32 temp_a1;
    s32 new_unk474;
    s32 new_unkB8C;

    var_v0 = arg0->unkB48;
    temp_a1 = arg0->unk474;
    var_v0 = (var_v0 - temp_a1) * arg0->unkB8C;
    if (var_v0 < 0) {
        var_v0 += 0xF;
    }
    arg0->unk474 = (var_v0 >> 4) + temp_a1;
    arg0->unk480 = arg0->unk480 + arg0->unk468;
    new_unk474 = arg0->unk474;
    new_unkB8C = arg0->unkB8C + 1;
    arg0->unkB8C = new_unkB8C;
    arg0->unk468 = arg0->unk468 - 0x6000;
    arg0->unk438 = new_unk474 + arg0->unk480;
    func_800B40D4_A3F84(arg0);
    func_8005D308_5DF08(arg0, 4);
    if (arg0->unkB8C == 16) {
        s32 tempB84 = arg0->unkB84;
        u8 tempBBF = arg0->unkBBF;
        arg0->unkBC0 = 0;
        tempBBF = tempBBF + 1;
        tempB84 = tempB84 | 0x2000;
        arg0->unkB84 = tempB84;
        arg0->unkBBF = tempBBF;
        func_80056B7C_5777C(&arg0->unk434, 0x25);
    }
    return 0;
}

s32 func_800B71E4_A7094(func_800B30B0_arg *arg0) {
    arg0->unk434 = arg0->unkB44;
    arg0->unk438 = arg0->unkB48;
    arg0->unk43C = arg0->unkB4C;
    arg0->unkA8E = arg0->unkB74;

    if (func_8005D308_5DF08(arg0, 5) != 0) {
        arg0->unkBC0 = 0;
        arg0->unkBBF = arg0->unkBBF + 1;
    }

    return 0;
}

s32 func_800B724C_A70FC(func_800B30B0_arg *arg0) {
    GameState *alloc;
    u8 flags;

    alloc = (GameState *)getCurrentAllocation();
    arg0->unk434 = arg0->unkB44;
    arg0->unk438 = arg0->unkB48;
    arg0->unk43C = arg0->unkB4C;
    arg0->unkA8E = arg0->unkB74;
    flags = arg0->unkBCE;

    if (flags & 8) {
        arg0->unkBCE = flags & 0xF7;
        func_8006FDC8_709C8(arg0->unkBB8, 0xFF, 0x10);
    }

    if (arg0->unkBCE & 2) {
        arg0->unkA94 = 0x1000;
        arg0->unkB9C = 0;
        arg0->unkB94 = 0;
        arg0->unkBBF++;
        arg0->unkB84 |= 0x200;
        arg0->unkBC5++;
        memcpy(&arg0->unk440, &arg0->unk434, 0xC);
        arg0->unkBC3 = 1;
        func_8006FDC8_709C8(arg0->unkBB8, 0, 0x10);
        if (arg0->unkBC7 == 0 && arg0->unkBC5 == alloc->unk74) {
            func_8004D364_4DF64(arg0->unkBB8);
        }
    }

    func_8005D308_5DF08(arg0, 0);

    return 0;
}

s32 func_800B735C_A720C(func_800B30B0_arg *arg0) {
    arg0->unk434 = arg0->unkB44;
    arg0->unk438 = arg0->unkB48;
    arg0->unk43C = arg0->unkB4C;
    arg0->unkA8E = arg0->unkB74;

    func_8005D308_5DF08(arg0, 0);

    if (arg0->unkBCE & 0x4) {
        arg0->unkB8C = 6;
        arg0->unkBBF = arg0->unkBBF + 1;
    }

    return 0;
}

s32 func_800B73CC_A727C(func_800B30B0_arg *arg0) {
    arg0->unk434 = arg0->unkB44;
    arg0->unk438 = arg0->unkB48;
    arg0->unk43C = arg0->unkB4C;
    arg0->unkA8E = arg0->unkB74;

    func_8005D308_5DF08(arg0, 3);

    arg0->unkB8C = arg0->unkB8C - 1;
    if (arg0->unkB8C == 0) {
        arg0->unkB8C = 0xE;
        arg0->unkBC0 = 0;
        arg0->unkBBF = arg0->unkBBF + 1;
    }

    return 0;
}

s32 func_800B7444_A72F4(func_800B30B0_arg *arg0) {
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

s32 func_800B74E4_A7394(func_800B30B0_arg *arg0) {
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

s32 func_800B756C_A741C(func_800B30B0_arg *arg0) {
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

typedef struct {
    u8 _pad0[0x63];
    u8 unk63; // 0x63
} func_800B75F4_alloc;

s32 func_800B75F4_A74A4(func_800B30B0_arg *arg0) {
    func_800B75F4_alloc *alloc = (func_800B75F4_alloc *)getCurrentAllocation();
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
        u8 temp2 = arg0->unkBBF;
        s32 temp3 = arg0->unkB84;
        arg0->unkBA0 = 0x2000;
        arg0->unkBA2 = 0x2000;
        arg0->unkB8C = 10;
        temp2 = temp2 + 1;
        temp3 = temp3 | 0x800000;
        arg0->unkBBF = temp2;
        arg0->unkB84 = temp3;
        alloc->unk63 = alloc->unk63 & 2;
        func_80056B7C_5777C(&arg0->unk434, 0x4E);
    }

    return 0;
}

typedef struct {
    u8 _pad0[0x81];
    u8 unk81; // 0x81
} func_800B76BC_alloc;

s32 func_800B76BC_A756C(func_800B30B0_arg *arg0) {
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

s32 func_800B7784_A7634(func_800B30B0_arg *arg0) {
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

s32 func_800B781C_A76CC(func_800B30B0_arg *arg0) {
    GameState *gameState;
    D_80090F90_91B90_item *item;
    s32 pad[12];

    gameState = getCurrentAllocation();
    item = func_80055D10_56910(gameState->memoryPoolId);

    if (arg0->unkBC0 == 0) {
        arg0->unkA94 = 0xE00;
        arg0->unkB9C = 0;
        arg0->unkB94 = 0;
        arg0->unkB84 |= 0x200;
        arg0->unkBC0++;
        arg0->unkBC5++;

        createYRotationMatrix(&arg0->unk970, 0xE00);

        arg0->unk434 = (void *)item->unkC;
        arg0->unk438 = item->unk10;
        arg0->unk43C = item->unk14;

        memcpy(&arg0->unk440, &arg0->unk434, 0xC);

        arg0->unkBC3 = 2;
        func_8006FDC8_709C8(arg0->unkBB8, 0, 0x10);

        if (arg0->unkBC7 == 0) {
            if (arg0->unkBC5 == gameState->unk74) {
                func_8004D364_4DF64(arg0->unkBB8);
            }
        }

        func_80056B7C_5777C(&arg0->unk434, 0x26);
    }

    func_800B40D4_A3F84(arg0);
    func_8005D180_5DD80(arg0, 0);

    arg0->unkBA0 += 0x400;
    arg0->unkB8C = 0x400;
    arg0->unkA94 = arg0->unkA94 + 0x400;
    arg0->unkBA2 = arg0->unkBA2 - 0x400;

    if (arg0->unkBA0 == 0x2000) {
        arg0->unkB84 &= 0xFF7FFFFF;
        arg0->unkBBF++;
    }

    arg0->unkB84 |= 0x10000;
    return 0;
}

s32 func_800B7998_A7848(func_800B30B0_arg *arg0) {
    func_800B76BC_alloc *alloc = (func_800B76BC_alloc *)getCurrentAllocation();
    s32 temp;

    func_800B40D4_A3F84(arg0);
    func_8005D180_5DD80(arg0, 0);

    arg0->unkA94 = arg0->unkA94 + arg0->unkB8C;
    temp = arg0->unkB8C - 0x10;
    arg0->unkB8C = temp;

    if (temp == 0) {
        arg0->unkB8C = 6;
        arg0->unkBBF = arg0->unkBBF + 1;
        alloc->unk81 = alloc->unk81 - 1;
    }

    arg0->unkB84 = arg0->unkB84 | 0x10000;
    return 0;
}

s32 func_800B7A30_A78E0(func_800B30B0_arg *arg0) {
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

s32 func_800B7A94_A7944(func_800B30B0_arg *arg0) {
    if (arg0->unkBC0 == 0) {
        arg0->unk454 = 0xFFF80000;
        arg0->unk44C = 0;
        arg0->unk450 = 0x30000;
        arg0->unkBC0 = arg0->unkBC0 + 1;
    }

    arg0->unkB84 = arg0->unkB84 | 0x10000;
    arg0->unk450 = arg0->unk450 - 0x6000;
    func_800B02AC_A015C(arg0);
    func_800B40D4_A3F84(arg0);

    if (arg0->unkB8C != 0) {
        arg0->unkB8C = arg0->unkB8C - 1;
    } else {
        arg0->unkB84 = arg0->unkB84 & 0xFFFFDC7F;
        func_800B00E4_9FF94(arg0);
    }

    func_8005D308_5DF08(arg0, 4);
    return 0;
}

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

void func_800B98CC_A977C(func_800B30B0_arg *arg0) {
    void *alloc;
    void *allocPlus30;
    s32 i;
    s32 offset;
    u8 *ptr;
    void *addrPtr;
    s32 temp;

    alloc = getCurrentAllocation();
    i = 0;
    allocPlus30 = (void *)((u8 *)alloc + 0x30);
    offset = 0xA10;
    ptr = (u8 *)arg0;

    do {
        addrPtr = (u8 *)arg0 + offset;
        *(volatile s32 *)(ptr + 0xA10) = arg0->unk970.unk14 + *(s32 *)((u8 *)&D_800BA348_AA1F8 + offset);
        *(volatile s32 *)(ptr + 0xA18) = arg0->unk970.unk1C + *(s32 *)((u8 *)&D_800BA350_AA200 + offset);

        temp = func_80059E90_5AA90((void *)arg0, allocPlus30, arg0->unkB94, addrPtr);
        *(volatile s32 *)(ptr + 0xA14) = func_8005CFC0_5DBC0(allocPlus30, temp, addrPtr, 0x100000);

        i++;
        offset += 0xC;
        ptr += 0xC;
    } while (i < 9);

    arg0->unkBC1 = 1;

    if (!(arg0->unkB84 & 0x800000)) {
        for (i = 0; i < 4; i++) {
            debugEnqueueCallback(i, 1, func_800B9500_A93B0, (void *)arg0);
        }
    }
}

void func_800B99E0(void *varg0) {
    func_800B30B0_arg *arg0 = (func_800B30B0_arg *)varg0;
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

void func_800B9AE0(void *varg0) {
    func_800B30B0_arg *arg0 = (func_800B30B0_arg *)varg0;
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
