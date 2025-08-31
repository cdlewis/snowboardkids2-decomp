#include "common.h"

typedef struct {
    char identifier[8];
    char description[16];
    void *func1;
    void *func2;
    void *func3;
    void *func4;
    void *func5;
} CommandEntry;

extern struct {
    char header[16];
    CommandEntry entries[53];
} D_800B9C70_1E6D20;

/*
struct {
    char header[16];
    CommandEntry entries[53];
} D_800B9C70_1E6D20 = {
    "               ",
    { { "INTRK \0\1",
        "INIT TRACK     \0",
        (void *)0x800B20B0,
        (void *)0x800B20B8,
        (void *)0x800B20C0,
        (void *)0,
        (void *)0 },
      { "SYSDP \0\1",
        "SYS DISP       \0",
        (void *)0x800B2260,
        (void *)0x800B2268,
        (void *)0x800B2270,
        (void *)0,
        (void *)0 },
      { "SYSFI \0\1",
        "SYS FADE IN    \0",
        (void *)0x800B227C,
        (void *)0x800B2284,
        (void *)0x800B228C,
        (void *)0,
        (void *)0x800B22E8 },
      { "SYSFO \0\1",
        "SYS FADE OUT   \0",
        (void *)0x800B22F0,
        (void *)0x800B22F8,
        (void *)0x800B2300,
        (void *)0,
        (void *)0x800B235C },
      { "SYSFL \0\1",
        "SYS FLASH      \0",
        (void *)0x800B2364,
        (void *)0x800B236C,
        (void *)0x800B2374,
        (void *)0x800B2404,
        (void *)0x800B24E0 },
      { "SYSIM \0\1",
        "SYS INTMODE    \0",
        (void *)0x800B24E8,
        (void *)0x800B24F0,
        (void *)0x800B24F8,
        (void *)0,
        (void *)0 },
      { "SYSWC \0\1",
        "SYS WIPE COLOR \0",
        (void *)0x800B2514,
        (void *)0x800B251C,
        (void *)0x800B2524,
        (void *)0,
        (void *)0 },
      { "SYSWT \0\1",
        "SYS WAIT       \0",
        (void *)0x800B2590,
        (void *)0x800B2598,
        (void *)0x800B25A0,
        (void *)0,
        (void *)0 },
      { "SYSCU \0\1",
        "SYS CURTAIN    \0",
        (void *)0x800B260C,
        (void *)0x800B2614,
        (void *)0x800B261C,
        (void *)0x800B2700,
        (void *)0x800B2784 },
      { "SYSFL \0\1",
        "SYS FILTER     \0",
        (void *)0x800B278C,
        (void *)0x800B2794,
        (void *)0x800B279C,
        (void *)0,
        (void *)0x800B27EC },
      { "CHRPS \0\1",
        "CHR POSITION   \0",
        (void *)0x800B0A40,
        (void *)0x800B0A48,
        (void *)0x800B0A50,
        (void *)0,
        (void *)0x800B0B08 },
      { "CHRMV \0\1",
        "CHR MOVE       \0",
        (void *)0x800B0B10,
        (void *)0x800B0B18,
        (void *)0x800B0B20,
        (void *)0,
        (void *)0x800B0C38 },
      { "CHRAM \0\1",
        "CHR ANIME      \0",
        (void *)0x800B0C40,
        (void *)0x800B0C48,
        (void *)0x800B0C50,
        (void *)0,
        (void *)0x800B0CCC },
      { "CHRDP \0\1",
        "CHR DISP       \0",
        (void *)0x800B0CD4,
        (void *)0x800B0CDC,
        (void *)0x800B0CE4,
        (void *)0,
        (void *)0 },
      { "CHRIT \0\1",
        "CHR ITEM       \0",
        (void *)0x800B0D24,
        (void *)0x800B0D2C,
        (void *)0x800B0D34,
        (void *)0,
        (void *)0 },
      { "CHRTN \0\1",
        "CHR TURN       \0",
        (void *)0x800B0D74,
        (void *)0x800B0D7C,
        (void *)0x800B0D84,
        (void *)0,
        (void *)0 },
      { "CHRM2 \0\1",
        "CHR MOVE2      \0",
        (void *)0x800B0E5C,
        (void *)0x800B0E64,
        (void *)0x800B0E6C,
        (void *)0,
        (void *)0x800B0F88 },
      { "CHRZM \0\1",
        "CHR ZOOM       \0",
        (void *)0x800B0F90,
        (void *)0x800B0F98,
        (void *)0x800B0FA0,
        (void *)0,
        (void *)0x800B1048 },
      { "CHRBM \0\1",
        "CHR BOARD MOVE \0",
        (void *)0x800B1050,
        (void *)0x800B1058,
        (void *)0x800B1060,
        (void *)0,
        (void *)0x800B111C },
      { "CHRT2 \0\1",
        "CHR TURN2      \0",
        (void *)0x800B1124,
        (void *)0x800B112C,
        (void *)0x800B1134,
        (void *)0,
        (void *)0 },
      { "CHRRT \0\1",
        "CHR ROTATE     \0",
        (void *)0x800B1218,
        (void *)0x800B1220,
        (void *)0x800B1228,
        (void *)0,
        (void *)0x800B129C },
      { "CHRHP \0\1",
        "CHR HOP        \0",
        (void *)0x800B12A4,
        (void *)0x800B12AC,
        (void *)0x800B12B4,
        (void *)0,
        (void *)0 },
      { "CHRBK \0\1",
        "CHR BACK       \0",
        (void *)0x800B1328,
        (void *)0x800B1330,
        (void *)0x800B1338,
        (void *)0,
        (void *)0x800B13C8 },
      { "CHRZ2 \0\1",
        "CHR ZOOM2      \0",
        (void *)0x800B13D0,
        (void *)0x800B13D8,
        (void *)0x800B13E0,
        (void *)0,
        (void *)0x800B14CC },
      { "CHRUD \0\1",
        "CHR UPDOWN     \0",
        (void *)0x800B14D4,
        (void *)0x800B14DC,
        (void *)0x800B14E4,
        (void *)0,
        (void *)0x800B1588 },
      { "CHRFC \0\1",
        "CHR FACE       \0",
        (void *)0x800B1590,
        (void *)0x800B1598,
        (void *)0x800B15A0,
        (void *)0,
        (void *)0 },
      { "CHRA2 \0\1",
        "CHR ANIME2     \0",
        (void *)0x800B15E0,
        (void *)0x800B15E8,
        (void *)0x800B15F0,
        (void *)0,
        (void *)0x800B1670 },
      { "CHRM3 \0\1",
        "CHR MOVE3      \0",
        (void *)0x800B1678,
        (void *)0x800B1680,
        (void *)0x800B1688,
        (void *)0,
        (void *)0x800B1734 },
      { "CHRMD \0\1",
        "CHR MODEL DISP \0",
        (void *)0x800B173C,
        (void *)0x800B1744,
        (void *)0x800B174C,
        (void *)0,
        (void *)0 },
      { "CHRST \0\1",
        "CHR MOVE SIGHT \0",
        (void *)0x800B17B0,
        (void *)0x800B17B8,
        (void *)0x800B17C0,
        (void *)0,
        (void *)0x800B1860 },
      { "CHRAL \0\1",
        "CHR ALPHA      \0",
        (void *)0x800B1868,
        (void *)0x800B1870,
        (void *)0x800B1878,
        (void *)0x800B19B8,
        (void *)0x800B1A88 },
      { "CHRP2 \0\1",
        "CHR POSITION2  \0",
        (void *)0x800B1A90,
        (void *)0x800B1A98,
        (void *)0x800B1AA0,
        (void *)0,
        (void *)0x800B1B68 },
      { "CHRT3 \0\1",
        "CHR TURN3      \0",
        (void *)0x800B1B70,
        (void *)0x800B1B78,
        (void *)0x800B1B80,
        (void *)0,
        (void *)0x800B1C10 },
      { "CHRDP \0\1",
        "CHR KAGEDISP   \0",
        (void *)0x800B1C18,
        (void *)0x800B1C20,
        (void *)0x800B1C28,
        (void *)0,
        (void *)0 },
      { "CAMRS \0\1",
        "CAMERA RESET   \0",
        (void *)0x800B08C0,
        (void *)0x800B08C8,
        (void *)0x800B08D0,
        (void *)0,
        (void *)0 },
      { "CAMMV \0\1",
        "CAMERA MOVE    \0",
        (void *)0x800B0910,
        (void *)0x800B0918,
        (void *)0x800B0920,
        (void *)0,
        (void *)0x800B099C },
      { "CAMJI \0\1",
        "CAMERA JISHIN  \0",
        (void *)0x800B09A4,
        (void *)0x800B09AC,
        (void *)0x800B09B4,
        (void *)0,
        (void *)0 },
      { "CAMRT \0\1",
        "CAMERA ROTATE  \0",
        (void *)0x800B09F4,
        (void *)0x800B09FC,
        (void *)0x800B0A04,
        (void *)0,
        (void *)0x800B0A2C },
      { "SEP   \0\1",
        "SE PLAY        \0",
        (void *)0x800B2118,
        (void *)0x800B2120,
        (void *)0x800B2128,
        (void *)0,
        (void *)0 },
      { "SEP3D \0\1",
        "SE3D PLAY      \0",
        (void *)0x800B218C,
        (void *)0x800B2194,
        (void *)0x800B219C,
        (void *)0,
        (void *)0 },
      { "SEST  \0\1",
        "SE STOP        \0",
        (void *)0x800B2220,
        (void *)0x800B2228,
        (void *)0x800B2230,
        (void *)0,
        (void *)0 },
      { "EFDP  \0\1",
        "EFFECT DISP    \0",
        (void *)0x800B1C90,
        (void *)0x800B1C98,
        (void *)0x800B1CA0,
        (void *)0,
        (void *)0x800B1D44 },
      { "EFFAN \0\1",
        "EFFECT FAN     \0",
        (void *)0x800B1D4C,
        (void *)0x800B1D54,
        (void *)0x800B1D5C,
        (void *)0,
        (void *)0x800B1D9C },
      { "EFRKT \0\1",
        "EFFECT ROCKET  \0",
        (void *)0x800B1DA4,
        (void *)0x800B1DAC,
        (void *)0x800B1DB4,
        (void *)0,
        (void *)0x800B1DF4 },
      { "EFMLT \0\1",
        "EFFECT MLIGHT  \0",
        (void *)0x800B1DFC,
        (void *)0x800B1E04,
        (void *)0x800B1E0C,
        (void *)0,
        (void *)0x800B20A8 },
      { "EFTRK \0\1",
        "EFFECT TRICK   \0",
        (void *)0x800B1F9C,
        (void *)0x800B1FA4,
        (void *)0x800B1FAC,
        (void *)0x800B2010,
        (void *)0x800B20A8 },
      { "BGMP  \0\1",
        "BGM PLAY       \0",
        (void *)0x800B0810,
        (void *)0x800B0818,
        (void *)0x800B0820,
        (void *)0,
        (void *)0 },
      { "BGMFO \0\1",
        "BGM FADE OUT   \0",
        (void *)0x800B0848,
        (void *)0x800B0850,
        (void *)0x800B0858,
        (void *)0,
        (void *)0 },
      { "BGMVL \0\1",
        "BGM VOLUME     \0",
        (void *)0x800B0884,
        (void *)0x800B088C,
        (void *)0x800B0894,
        (void *)0,
        (void *)0 },
      { "EF2PL \0\1", "EFFECT2 DISP   \0", (void *)0, (void *)0, (void *)0, (void *)0, (void *)0 },
      { "S2WP  \0\1",
        "SYS2 WIPE      \0",
        (void *)0x800B2800,
        (void *)0x800B2808,
        (void *)0x800B2810,
        (void *)0,
        (void *)0x800B292C },
      { "S2WT  \0\1",
        "SYS2 WAIT      \0",
        (void *)0x800B2934,
        (void *)0x800B293C,
        (void *)0x800B2944,
        (void *)0,
        (void *)0 } }
};
*/

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
    s32 unk0;
    s16 unk4;
    s8 unk6;
    s8 unk7;
    s32 unk8;
    u8 padding[0x1C];
    s16 (*unk28)(func_800B06FC_1DD7AC_arg0 *, func_800B06FC_1DD7AC_arg1 *, s8);
} D_800BA570_1E7620_item_unk0;

typedef struct {
    D_800BA570_1E7620_item_unk0 *unk0;
    s32 unk4;
} D_800BA570_1E7620_item;
extern D_800BA570_1E7620_item D_800BA570_1E7620[];

D_800BA570_1E7620_item_unk0 *func_800B00C0_1DD170(s32 arg0, s32 arg1) {
    D_800BA570_1E7620_item_unk0 *temp = D_800BA570_1E7620[(u8)arg0].unk0;
    return &temp[(u8)arg1];
}

typedef struct {
    s16 unk0;
    u8 padding[0x6];
} D_800BA574_1E7624_item;

typedef struct {
    s16 unk0;
    u8 padding[0x6];
} D_800BA576_1E7626_item;

extern D_800BA576_1E7626_item D_800BA576_1E7626[];
extern u16 D_800BA5B8_1E7668;
extern D_800BA574_1E7624_item D_800BA574_1E7624[];
extern u8 D_800BA5B9_1E7669;
void func_800BB47C(s32, s32, s32, s32, s32, s32);

void func_800B00F4_1DD1A4(s32 arg0, s32 arg1) {
    func_800B00C0_1DD170(arg0 & 0xFF, arg1 & 0xFF);
}

u8 func_800B0114_1DD1C4(s32 arg0, s32 arg1) {
    return func_800B00C0_1DD170(arg0 & 0xFF, arg1 & 0xFF)->unk7;
}

void *func_800B0138_1DD1E8(u8 a0, u8 a1) {
    if ((a0 == 0xFF) | (a1 == 0xFF)) {
        return &D_800B9C70_1E6D20;
    }
    return &func_800B00C0_1DD170(a0, a1)->unk8;
}

u8 func_800B0184_1DD234(u8 index, s32 value) {
    s32 inc = value + 1;
    s16 lim = D_800BA574_1E7624[index].unk0;
    u8 test = inc;
    s32 res = (test < lim) ? inc : 0;
    return res;
}

s32 func_800B01B4_1DD264(s32 arg0, s32 arg1) {
    s16 temp_v1 = D_800BA574_1E7624[arg0 & 0xFF].unk0;
    u8 var_a1 = arg1 - 1;
    if ((var_a1 & 0xFF) >= temp_v1) {
        var_a1 = temp_v1 - 1;
    }
    return var_a1 & 0xFF;
}

s32 func_800B01F0_1DD2A0(s32 arg0, u8 arg1) {
    s16 temp_v1 = D_800BA574_1E7624[arg0 & 0xFF].unk0;
    u8 var_a1 = arg1;

    if ((var_a1 & 0xFF) >= temp_v1) {
        var_a1 = temp_v1 - 1;
    }
    return var_a1 & 0xFF;
}

s32 func_800B0228_1DD2D8(u8 nextIndex, s16 skipValue) {
    nextIndex++;
    if ((nextIndex & 0xFF) >= D_800BA5B8_1E7668) {
        nextIndex = 0;
    }

    if (skipValue != -1) {
        while (D_800BA576_1E7626[nextIndex & 0xFF].unk0 == skipValue) {
            nextIndex++;
            if ((nextIndex & 0xFF) >= D_800BA5B8_1E7668) {
                nextIndex = 0;
            }
        }
    }

    return nextIndex & 0xFF;
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
    if (temp_v1 != -1 && D_800BA576_1E7626[var_a0 & 0xFF].unk0 == temp_v1) {
        var_a0 -= 1;
        if ((var_a0 & 0xFF) == 0xFF) {
            var_a0 = D_800BA5B9_1E7669 - 1;
        }
    }

    return var_a0 & 0xFF;
}

INCLUDE_ASM("asm/nonmatchings/1DD170", func_800B0328_1DD3D8);

INCLUDE_ASM("asm/nonmatchings/1DD170", func_800B043C_1DD4EC);

INCLUDE_ASM("asm/nonmatchings/1DD170", func_800B0534_1DD5E4);

INCLUDE_ASM("asm/nonmatchings/1DD170", func_800B0628_1DD6D8);

s16 func_800B06FC_1DD7AC(func_800B06FC_1DD7AC_arg0 *arg0, func_800B06FC_1DD7AC_arg1 *arg1, s8 arg2) {
    s32 var_s2 = 1;
    s16 (*temp_v1)(func_800B06FC_1DD7AC_arg0 *, func_800B06FC_1DD7AC_arg1 *, s8) =
        func_800B00C0_1DD170(arg0->unk3E, arg0->unk3F)->unk28;

    if (temp_v1 != 0) {
        func_800B06FC_1DD7AC_arg1 *temp = &arg1[arg2];
        if (temp->unkF0 == 0) {
            var_s2 = (D_800BA576_1E7626[arg0->unk3E].unk0 != 1);
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
    return D_800BA576_1E7626[arg0 & 0xFF].unk0;
}
