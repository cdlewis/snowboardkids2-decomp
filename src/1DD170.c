#include "1DD8C0.h"
#include "1DD970.h"
#include "1DDAF0.h"
#include "1DED40.h"
#include "1DF180.h"
#include "1DF310.h"
#include "1DF8B0.h"
#include "common.h"

typedef struct {
    u8 padding[0x3E];
    u8 unk3E;
    u8 unk3F;
} func_800B06FC_1DD7AC_arg0;

typedef struct {
    u8 padding[0xF0];
    s32 unkF0;
} func_800B06FC_1DD7AC_arg1;

typedef struct {
    /* 0x00 */ s8 identifier[0x7];
    /* 0x07 */ s8 unk7;
    /* 0x08 */ s8 description[0x10];
    /* 0x18 */ s32 (*func1)(func_800B06FC_1DD7AC_arg0 *, func_800B06FC_1DD7AC_arg1 *, s32, s32 arg3, s32 arg4, s8 arg5);
    /* 0x1C */ s32 (*func2)(func_800B06FC_1DD7AC_arg0 *, func_800B06FC_1DD7AC_arg1 *, s32, s32 arg3, s32 arg4, s8 arg5);
    /* 0x20 */ void *func3;
    /* 0x24 */ s32 (*func4)(func_800B06FC_1DD7AC_arg0 *, func_800B06FC_1DD7AC_arg1 *, s8);
    /* 0x28 */ s16 (*func5)(func_800B06FC_1DD7AC_arg0 *, func_800B06FC_1DD7AC_arg1 *, s8);
} CommandEntry;

// clang-format off
struct {
    s8 header[16];
    CommandEntry entries[52];
} D_800B9C70_1E6D20 = {
    "               ",
    {
        { "INTRK \0", '\1', "INIT TRACK     \0", (void *)&noop_1DF160, (void *)&returnZero_1DF168, (void *)&noop_1DF170, NULL, NULL },
        { "SYSDP \0", '\1', "SYS DISP       \0", (void *)&noop_1DF310, (void *)&returnZero_1DF318, (void *)&func_800B2270_1DF320, NULL, NULL },
        { "SYSFI \0", '\1', "SYS FADE IN    \0", (void *)&noop_1DF32C, (void *)&func_800B2284_1DF334, (void *)&func_800B228C_1DF33C, NULL, (void *)&func_800B22E8_1DF398 },
        { "SYSFO \0", '\1', "SYS FADE OUT   \0", (void *)&noop_1DF3A0, (void *)&func_800B22F8_1DF3A8, (void *)&func_800B2300_1DF3B0, NULL, (void *)&func_800B235C_1DF40C },
        { "SYSFL \0", '\1', "SYS FLASH      \0", (void *)&noop_1DF414, (void *)&func_800B236C_1DF41C, (void *)&func_800B2374_1DF424, (void *)&func_800B2404_1DF4B4, (void *)&func_800B24E0_1DF590 },
        { "SYSIM \0", '\1', "SYS INTMODE    \0", (void *)&noop_1DF598, (void *)&func_800B24F0_1DF5A0, (void *)&func_800B24F8_1DF5A8, NULL, NULL },
        { "SYSWC \0", '\1', "SYS WIPE COLOR \0", (void *)&noop_1DF5C4, (void *)&func_800B251C_1DF5CC, (void *)&func_800B2524_1DF5D4, NULL, NULL },
        { "SYSWT \0", '\1', "SYS WAIT       \0", (void *)&noop_1DF640, (void *)&func_800B2598_1DF648, (void *)&func_800B25A0_1DF650, NULL, NULL },
        { "SYSCU \0", '\1', "SYS CURTAIN    \0", (void *)&noop_1DF6BC, (void *)&func_800B2614_1DF6C4, (void *)&func_800B261C_1DF6CC, (void *)&func_800B2700_1DF7B0, (void *)&func_800B2784_1DF834 },
        { "SYSFL \0", '\1', "SYS FILTER     \0", (void *)&noop_1DF83C, (void *)&func_800B2794_1DF844, (void *)&func_800B279C_1DF84C, NULL, (void *)&func_800B27EC_1DF89C },
        { "CHRPS \0", '\1', "CHR POSITION   \0", (void *)&noop_1DDAF0, (void *)&func_800B0A48_1DDAF8, (void *)&func_800B0A50_1DDB00, NULL, (void *)&func_800B0B08_1DDBB8 },
        { "CHRMV \0", '\1', "CHR MOVE       \0", (void *)&noop_1DDBC0, (void *)&func_800B0B18_1DDBC8, (void *)&func_800B0B20_1DDBD0, NULL, (void *)&func_800B0C38_1DDCE8 },
        { "CHRAM \0", '\1', "CHR ANIME      \0", (void *)&noop_1DDCF0, (void *)&func_800B0C48_1DDCF8, (void *)&func_800B0C50_1DDD00, NULL, (void *)&func_800B0CCC_1DDD7C },
        { "CHRDP \0", '\1', "CHR DISP       \0", (void *)&noop_1DDD84, (void *)&func_800B0CDC_1DDD8C, (void *)&func_800B0CE4_1DDD94, NULL, NULL },
        { "CHRIT \0", '\1', "CHR ITEM       \0", (void *)&noop_1DDDD4, (void *)&func_800B0D2C_1DDDDC, (void *)&func_800B0D34_1DDDE4, NULL, NULL },
        { "CHRTN \0", '\1', "CHR TURN       \0", (void *)&noop_1DDE24, (void *)&func_800B0D7C_1DDE2C, (void *)&func_800B0D84_1DDE34, NULL, NULL },
        { "CHRM2 \0", '\1', "CHR MOVE2      \0", (void *)&noop_1DDF0C, (void *)&func_800B0E64_1DDF14, (void *)&func_800B0E6C_1DDF1C, NULL, (void *)&func_800B0F88_1DE038 },
        { "CHRZM \0", '\1', "CHR ZOOM       \0", (void *)&noop_1DE040, (void *)&func_800B0F98_1DE048, (void *)&func_800B0FA0_1DE050, NULL, (void *)&func_800B1048_1DE0F8 },
        { "CHRBM \0", '\1', "CHR BOARD MOVE \0", (void *)&noop_1DE100, (void *)&func_800B1058_1DE108, (void *)&func_800B1060_1DE110, NULL, (void *)&func_800B111C_1DE1CC },
        { "CHRT2 \0", '\1', "CHR TURN2      \0", (void *)&noop_1DE1D4, (void *)&func_800B112C_1DE1DC, (void *)&func_800B1134_1DE1E4, NULL, NULL },
        { "CHRRT \0", '\1', "CHR ROTATE     \0", (void *)&noop_1DE2C8, (void *)&func_800B1220_1DE2D0, (void *)&func_800B1228_1DE2D8, NULL, (void *)&func_800B129C_1DE34C },
        { "CHRHP \0", '\1', "CHR HOP        \0", (void *)&noop_1DE354, (void *)&func_800B12AC_1DE35C, (void *)&func_800B12B4_1DE364, NULL, NULL },
        { "CHRBK \0", '\1', "CHR BACK       \0", (void *)&noop_1DE3D8, (void *)&func_800B1330_1DE3E0, (void *)&func_800B1338_1DE3E8, NULL, (void *)&func_800B13C8_1DE478 },
        { "CHRZ2 \0", '\1', "CHR ZOOM2      \0", (void *)&noop_1DE480, (void *)&func_800B13D8_1DE488, (void *)&func_800B13E0_1DE490, NULL, (void *)&func_800B14CC_1DE57C },
        { "CHRUD \0", '\1', "CHR UPDOWN     \0", (void *)&noop_1DE584, (void *)&func_800B14DC_1DE58C, (void *)&func_800B14E4_1DE594, NULL, (void *)&func_800B1588_1DE638 },
        { "CHRFC \0", '\1', "CHR FACE       \0", (void *)&noop_1DE640, (void *)&func_800B1598_1DE648, (void *)&func_800B15A0_1DE650, NULL, NULL },
        { "CHRA2 \0", '\1', "CHR ANIME2     \0", (void *)&noop_1DE690, (void *)&func_800B15E8_1DE698, (void *)&func_800B15F0_1DE6A0, NULL, (void *)&func_800B1670_1DE720 },
        { "CHRM3 \0", '\1', "CHR MOVE3      \0", (void *)&noop_1DE728, (void *)&func_800B1680_1DE730, (void *)&func_800B1688_1DE738, NULL, (void *)&func_800B1734_1DE7E4 },
        { "CHRMD \0", '\1', "CHR MODEL DISP \0", (void *)&noop_1DE7EC, (void *)&func_800B1744_1DE7F4, (void *)&func_800B174C_1DE7FC, NULL, NULL },
        { "CHRST \0", '\1', "CHR MOVE SIGHT \0", (void *)&noop_1DE860, (void *)&func_800B17B8_1DE868, (void *)&func_800B17C0_1DE870, NULL, (void *)&func_800B1860_1DE910 },
        { "CHRAL \0", '\1', "CHR ALPHA      \0", (void *)&noop_1DE918, (void *)&func_800B1870_1DE920, (void *)&func_800B1878_1DE928, (void *)&func_800B19B8_1DEA68, (void *)&func_800B1A88_1DEB38 },
        { "CHRP2 \0", '\1', "CHR POSITION2  \0", (void *)&noop_1DEB40, (void *)&func_800B1A98_1DEB48, (void *)&func_800B1AA0_1DEB50, NULL, (void *)&func_800B1B68_1DEC18 },
        { "CHRT3 \0", '\1', "CHR TURN3      \0", (void *)&noop_1DEC20, (void *)&func_800B1B78_1DEC28, (void *)&func_800B1B80_1DEC30, NULL, (void *)&func_800B1C10_1DECC0 },
        { "CHRDP \0", '\1', "CHR KAGEDISP   \0", (void *)&noop_1DECC8, (void *)&func_800B1C20_1DECD0, (void *)&func_800B1C28_1DECD8, NULL, NULL },
        { "CAMRS \0", '\1', "CAMERA RESET   \0", (void *)&noop_1DD970, (void *)&func_800B08C8_1DD978, (void *)&func_800B08D0_1DD980, NULL, NULL },
        { "CAMMV \0", '\1', "CAMERA MOVE    \0", (void *)&noop_1DD9C0, (void *)&func_800B0918_1DD9C8, (void *)&func_800B0920_1DD9D0, NULL, (void *)&func_800B099C_1DDA4C },
        { "CAMJI \0", '\1', "CAMERA JISHIN  \0", (void *)&noop_1DDA54, (void *)&func_800B09AC_1DDA5C, (void *)&func_800B09B4_1DDA64, NULL, NULL },
        { "CAMRT \0", '\1', "CAMERA ROTATE  \0", (void *)&noop_1DDAA4, (void *)&func_800B09FC_1DDAAC, (void *)&func_800B0A04_1DDAB4, NULL, (void *)&func_800B0A2C_1DDADC },
        { "SEP   \0", '\1', "SE PLAY        \0", (void *)&noop_1DF1C8, (void *)&func_800B2120_1DF1D0, (void *)&func_800B2128_1DF1D8, NULL, NULL },
        { "SEP3D \0", '\1', "SE3D PLAY      \0", (void *)&noop_1DF23C, (void *)&func_800B2194_1DF244, (void *)&func_800B219C_1DF24C, NULL, NULL },
        { "SEST  \0", '\1', "SE STOP        \0", (void *)&noop_1DF2D0, (void *)&func_800B2228_1DF2D8, (void *)&func_800B2230_1DF2E0, NULL, NULL },
        { "EFDP  \0", '\1', "EFFECT DISP    \0", (void *)&noop_1DED40, (void *)&func_800B1C98_1DED48, (void *)&func_800B1CA0_1DED50, NULL, (void *)&func_800B1D44_1DEDF4 },
        { "EFFAN \0", '\1', "EFFECT FAN     \0", (void *)&noop_1DEDFC, (void *)&func_800B1D54_1DEE04, (void *)&func_800B1D5C_1DEE0C, NULL, (void *)&func_800B1D9C_1DEE4C },
        { "EFRKT \0", '\1', "EFFECT ROCKET  \0", (void *)&noop_1DEE54, (void *)&func_800B1DAC_1DEE5C, (void *)&func_800B1DB4_1DEE64, NULL, (void *)&func_800B1DF4_1DEEA4 },
        { "EFMLT \0", '\1', "EFFECT MLIGHT  \0", (void *)&noop_1DEEAC, (void *)&func_800B1E04_1DEEB4, (void *)&func_800B1E0C_1DEEBC, NULL, (void *)&func_800B20A8_1DF158 },
        { "EFTRK \0", '\1', "EFFECT TRICK   \0", (void *)&noop_1DF04C, (void *)&func_800B1FA4_1DF054, (void *)&func_800B1FAC_1DF05C, (void *)&func_800B2010_1DF0C0, (void *)&func_800B20A8_1DF158 },
        { "BGMP  \0", '\1', "BGM PLAY       \0", (void *)&noop_1DD8C0, (void *)&func_800B0818_1DD8C8, (void *)&func_800B0820_1DD8D0, NULL, NULL },
        { "BGMFO \0", '\1', "BGM FADE OUT   \0", (void *)&noop_1DD8F8, (void *)&func_800B0850_1DD900, (void *)&func_800B0858_1DD908, NULL, NULL },
        { "BGMVL \0", '\1', "BGM VOLUME     \0", (void *)&noop_1DD934, (void *)&func_800B088C_1DD93C, (void *)&func_800B0894_1DD944, NULL, NULL },
        { "EF2PL \0", '\1', "EFFECT2 DISP   \0", NULL, NULL, NULL, NULL, NULL },
        { "S2WP  \0", '\1', "SYS2 WIPE      \0", (void *)&noop_1DF8B, (void *)&func_800B2808_1DF8B8, (void *)&func_800B2810_1DF8C0, NULL, (void *)&func_800B292C_1DF9DC },
        { "S2WT  \0", '\1', "SYS2 WAIT      \0", (void *)&noop_1DF9E4, (void *)&func_800B293C_1DF9EC, (void *)&func_800B2944_1DF9F4, NULL, NULL }
    }
};
// clang-format on

// clang-format off
struct {
    CommandEntry *unk0;
    s16 unk4;
    s16 unk6;
} D_800BA570_1E7620[9] = {
    { &D_800B9C70_1E6D20.entries[0],  0x0001, 0x0000 },
    { &D_800B9C70_1E6D20.entries[1],  0x0009, 0x0000 },
    { &D_800B9C70_1E6D20.entries[10], 0x0018, 0x0001 },
    { &D_800B9C70_1E6D20.entries[34], 0x0004, 0x0000 },
    { &D_800B9C70_1E6D20.entries[38], 0x0003, 0x0000 },
    { &D_800B9C70_1E6D20.entries[41], 0x0005, 0x0001 },
    { &D_800B9C70_1E6D20.entries[46], 0x0003, 0x0000 },
    { &D_800B9C70_1E6D20.entries[49], 0x0001, 0x0000 },
    { &D_800B9C70_1E6D20.entries[50], 0x0002, 0x0001 } 
};
// clang-format on

u8 D_800BA5B8_1E7668 = 0;
u8 D_800BA5B9_1E7669 = 9;

void func_800BB47C(s32, s32, s32, s32, s32, s32);

CommandEntry *func_800B00C0_1DD170(s32 arg0, s32 arg1) {
    CommandEntry *temp = D_800BA570_1E7620[(u8)arg0].unk0;
    return &temp[(u8)arg1];
}

void func_800B00F4_1DD1A4(s32 arg0, s32 arg1) {
    func_800B00C0_1DD170(arg0 & 0xFF, arg1 & 0xFF);
}

u8 func_800B0114_1DD1C4(s32 arg0, s32 arg1) {
    return func_800B00C0_1DD170(arg0 & 0xFF, arg1 & 0xFF)->unk7;
}

void *func_800B0138_1DD1E8(u8 a0, u8 a1) {
    if ((a0 == 0xFF) | (a1 == 0xFF)) {
        return &D_800B9C70_1E6D20.header;
    }

    return &func_800B00C0_1DD170(a0, a1)->description;
}

u8 func_800B0184_1DD234(u8 index, s32 value) {
    s32 inc = value + 1;
    s16 lim = D_800BA570_1E7620[index].unk4;
    u8 test = inc;
    s32 res = (test < lim) ? inc : 0;
    return res;
}

s32 func_800B01B4_1DD264(s32 arg0, s32 arg1) {
    s16 temp_v1 = D_800BA570_1E7620[arg0 & 0xFF].unk4;
    u8 var_a1 = arg1 - 1;

    if ((var_a1 & 0xFF) >= temp_v1) {
        var_a1 = temp_v1 - 1;
    }

    return var_a1 & 0xFF;
}

s32 func_800B01F0_1DD2A0(s32 arg0, u8 arg1) {
    s16 temp_v1 = D_800BA570_1E7620[arg0 & 0xFF].unk4;
    u8 var_a1 = arg1;

    if (arg1 >= temp_v1) {
        var_a1 = temp_v1 - 1;
    }

    return var_a1;
}

s32 func_800B0228_1DD2D8(u8 nextIndex, s16 skipValue) {
    u16 limit = *(u16 *)&D_800BA5B8_1E7668;

    nextIndex++;
    if (nextIndex >= limit) {
        nextIndex = 0;
    }

    if (skipValue != -1) {
        while (D_800BA570_1E7620[nextIndex].unk6 == skipValue) {
            nextIndex++;
            if (nextIndex >= limit) {
                nextIndex = 0;
            }
        }
    }

    return nextIndex;
}

s32 func_800B02B4_1DD364(s32 arg0, s32 arg1) {
    s16 temp_v1;
    s32 var_a0;
    s32 var_v0;

    var_a0 = arg0 - 1;
    if ((var_a0 & 0xFF) == 0xFF) {
        var_a0 = D_800BA5B9_1E7669 - 1;
        var_v0 = arg1 << 0x10;
    }

    temp_v1 = (s16)arg1;
    if (temp_v1 != -1 && D_800BA570_1E7620[var_a0 & 0xFF].unk6 == temp_v1) {
        var_a0 -= 1;
        if ((var_a0 & 0xFF) == 0xFF) {
            var_a0 = D_800BA5B9_1E7669 - 1;
        }
    }

    return var_a0 & 0xFF;
}

INCLUDE_ASM("asm/nonmatchings/1DD170", func_800B0328_1DD3D8);

s32 func_800B043C_1DD4EC(
    func_800B06FC_1DD7AC_arg0 *arg0,
    func_800B06FC_1DD7AC_arg1 *arg1,
    s32 arg2,
    s32 arg3,
    s32 arg4,
    s8 arg5
) {
    s32 check;
    CommandEntry *temp_v0;
    func_800B06FC_1DD7AC_arg1 *ptr;

    check = 1;
    temp_v0 = func_800B00C0_1DD170(arg0->unk3E, arg0->unk3F);

    if (temp_v0->func2) {
        ptr = &arg1[arg5];
        if (ptr->unkF0 == 0) {
            check = D_800BA570_1E7620[arg0->unk3E].unk6 != 1;
        }

        if (check) {
            return temp_v0->func2(arg0, arg1, arg2, arg3, arg4, arg5);
        }
    }

    return 0;
}

INCLUDE_ASM("asm/nonmatchings/1DD170", func_800B0534_1DD5E4);

INCLUDE_ASM("asm/nonmatchings/1DD170", func_800B0628_1DD6D8);

s16 func_800B06FC_1DD7AC(func_800B06FC_1DD7AC_arg0 *arg0, func_800B06FC_1DD7AC_arg1 *arg1, s8 arg2) {
    s32 var_s2 = 1;
    s16 (*temp_v1)(func_800B06FC_1DD7AC_arg0 *, func_800B06FC_1DD7AC_arg1 *, s8) =
        func_800B00C0_1DD170(arg0->unk3E, arg0->unk3F)->func5;

    if (temp_v1 != 0) {
        func_800B06FC_1DD7AC_arg1 *temp = &arg1[arg2];
        if (temp->unkF0 == 0) {
            var_s2 = (D_800BA570_1E7620[arg0->unk3E].unk6 != 1);
        }

        if (var_s2 != 0) {
            return temp_v1(arg0, arg1, arg2);
        }
    }

    return 0;
}

void func_800B07BC_1DD86C(s32 arg0) {
    func_800BB47C(arg0, 2, 0x14, 0x11, 8, 0x20);
}

s16 func_800B07F0_1DD8A0(s32 arg0) {
    return D_800BA570_1E7620[arg0 & 0xFF].unk6;
}