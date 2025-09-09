#include "1DD8C0.h"
#include "1DD970.h"
#include "1DDAF0.h"
#include "1DED40.h"
#include "1DF180.h"
#include "1DF310.h"
#include "1DF8B0.h"
#include "1DFAA0.h"
#include "common.h"

#define COMMAND_INIT_TRACK 0
#define COMMAND_SYS_DISP 1
#define COMMAND_SYS_FADE_IN 2
#define COMMAND_SYS_FADE_OUT 3
#define COMMAND_SYS_FLASH 4
#define COMMAND_SYS_INTMODE 5
#define COMMAND_SYS_WIPE_COLOR 6
#define COMMAND_SYS_WAIT 7
#define COMMAND_SYS_CURTAIN 8
#define COMMAND_SYS_FILTER 9
#define COMMAND_CHR_POSITION 10
#define COMMAND_CHR_MOVE 11
#define COMMAND_CHR_ANIME 12
#define COMMAND_CHR_DISP 13
#define COMMAND_CHR_ITEM 14
#define COMMAND_CHR_TURN 15
#define COMMAND_CHR_MOVE2 16
#define COMMAND_CHR_ZOOM 17
#define COMMAND_CHR_BOARD_MOVE 18
#define COMMAND_CHR_TURN2 19
#define COMMAND_CHR_ROTATE 20
#define COMMAND_CHR_HOP 21
#define COMMAND_CHR_BACK 22
#define COMMAND_CHR_ZOOM2 23
#define COMMAND_CHR_UPDOWN 24
#define COMMAND_CHR_FACE 25
#define COMMAND_CHR_ANIME2 26
#define COMMAND_CHR_MOVE3 27
#define COMMAND_CHR_MODEL_DISP 28
#define COMMAND_CHR_MOVE_SIGHT 29
#define COMMAND_CHR_ALPHA 30
#define COMMAND_CHR_POSITION2 31
#define COMMAND_CHR_TURN3 32
#define COMMAND_CHRKAGEDISP 33
#define COMMAND_CAMERA_RESET 34
#define COMMAND_CAMERA_MOVE 35
#define COMMAND_CAMERA_JISHIN 36
#define COMMAND_CAMERA_ROTATE 37
#define COMMAND_SE_PLAY 38
#define COMMAND_SE3D_PLAY 39
#define COMMAND_SE_STOP 40
#define COMMAND_EFFECT_DISP 41
#define COMMAND_EFFECT_FAN 42
#define COMMAND_EFFECT_ROCKET 43
#define COMMAND_EFFECT_MLIGHT 44
#define COMMAND_EFFECT_TRICK 45
#define COMMAND_BGM_PLAY 46
#define COMMAND_BGM_FADE_OUT 47
#define COMMAND_BGM_VOLUME 48
#define COMMAND_EFFECT2_DISP 49
#define COMMAND_SYS2_WIPE 50
#define COMMAND_SYS2_WAIT 51

typedef struct {
    u8 scriptState[0x3E];
    u8 commandCategory;
    u8 commandIndex;
} CurrentCommand;

typedef struct {
    u8 padding[0xF0];
    s32 isActive;
} CommandData;

typedef struct {
    /* 0x00 */ s8 identifier[0x7];
    /* 0x07 */ s8 unk7;
    /* 0x08 */ s8 description[0x10];
    /* 0x18 */ void (*func1)(CurrentCommand *, CommandData *, s8);
    /* 0x1C */ s32 (*func2)(CurrentCommand *, CommandData *, s32, s32 arg3, s32 arg4, s8 arg5);
    /* 0x20 */ void *func3;
    /* 0x24 */ s32 (*func4)(CurrentCommand *, CommandData *, s8);
    /* 0x28 */ s16 (*func5)(CurrentCommand *, CommandData *, s8);
} CommandEntry;

// clang-format off
struct {
    s8 header[16];
    CommandEntry entries[52];
} commandTable = {
    "               ",
    {
        { "INTRK \0", '\1', "INIT TRACK     \0", (void *)&noop_1DF160, (void *)&returnZero_1DF168, (void *)&noop_1DF170, NULL, NULL },
        { "SYSDP \0", '\1', "SYS DISP       \0", (void *)&noop_1DF310, (void *)&returnZero_1DF318, (void *)&func_800B2270_1DF320, NULL, NULL },
        { "SYSFI \0", '\1', "SYS FADE IN    \0", (void *)&noop_1DF32C, (void *)&returnZero_1DF334, (void *)&func_800B228C_1DF33C, NULL, (void *)&returnZero_1DF398 },
        { "SYSFO \0", '\1', "SYS FADE OUT   \0", (void *)&noop_1DF3A0, (void *)&returnZero_1DF3A8, (void *)&func_800B2300_1DF3B0, NULL, (void *)&returnZero_1DF40C },
        { "SYSFL \0", '\1', "SYS FLASH      \0", (void *)&noop_1DF414, (void *)&returnZero_1DF41C, (void *)&func_800B2374_1DF424, (void *)&func_800B2404_1DF4B4, (void *)&returnZero_1DF590 },
        { "SYSIM \0", '\1', "SYS INTMODE    \0", (void *)&noop_1DF598, (void *)&returnZero_1DF5A0, (void *)&func_800B24F8_1DF5A8, NULL, NULL },
        { "SYSWC \0", '\1', "SYS WIPE COLOR \0", (void *)&noop_1DF5C4, (void *)&returnZero_1DF5CC, (void *)&func_800B2524_1DF5D4, NULL, NULL },
        { "SYSWT \0", '\1', "SYS WAIT       \0", (void *)&noop_1DF640, (void *)&returnZero_1DF648, (void *)&func_800B25A0_1DF650, NULL, NULL },
        { "SYSCU \0", '\1', "SYS CURTAIN    \0", (void *)&noop_1DF6BC, (void *)&returnZero_1DF6C4, (void *)&func_800B261C_1DF6CC, (void *)&func_800B2700_1DF7B0, (void *)&returnZero_1DF834 },
        { "SYSFL \0", '\1', "SYS FILTER     \0", (void *)&noop_1DF83C, (void *)&returnZero_1DF844, (void *)&func_800B279C_1DF84C, NULL, (void *)&returnZero_1DF89C },
        { "CHRPS \0", '\1', "CHR POSITION   \0", (void *)&noop_1DDAF0, (void *)&returnZero_1DDAF8, (void *)&func_800B0A50_1DDB00, NULL, (void *)&returnZero_1DDBB8 },
        { "CHRMV \0", '\1', "CHR MOVE       \0", (void *)&noop_1DDBC0, (void *)&returnZero_1DDBC8, (void *)&func_800B0B20_1DDBD0, NULL, (void *)&returnZero_1DDCE8 },
        { "CHRAM \0", '\1', "CHR ANIME      \0", (void *)&noop_1DDCF0, (void *)&returnZero_1DDCF8, (void *)&func_800B0C50_1DDD00, NULL, (void *)&returnZero_1DDD7C },
        { "CHRDP \0", '\1', "CHR DISP       \0", (void *)&noop_1DDD84, (void *)&returnZero_1DDD8C, (void *)&func_800B0CE4_1DDD94, NULL, NULL },
        { "CHRIT \0", '\1', "CHR ITEM       \0", (void *)&noop_1DDDD4, (void *)&returnZero_1DDDDC, (void *)&func_800B0D34_1DDDE4, NULL, NULL },
        { "CHRTN \0", '\1', "CHR TURN       \0", (void *)&noop_1DDE24, (void *)&returnZero_1DDE2C, (void *)&func_800B0D84_1DDE34, NULL, NULL },
        { "CHRM2 \0", '\1', "CHR MOVE2      \0", (void *)&noop_1DDF0C, (void *)&returnZero_1DDF14, (void *)&func_800B0E6C_1DDF1C, NULL, (void *)&returnZero_1DE038 },
        { "CHRZM \0", '\1', "CHR ZOOM       \0", (void *)&noop_1DE040, (void *)&returnZero_1DE048, (void *)&func_800B0FA0_1DE050, NULL, (void *)&returnZero_1DE0F8 },
        { "CHRBM \0", '\1', "CHR BOARD MOVE \0", (void *)&noop_1DE100, (void *)&returnZero_1DE108, (void *)&func_800B1060_1DE110, NULL, (void *)&returnZero_1DE1CC },
        { "CHRT2 \0", '\1', "CHR TURN2      \0", (void *)&noop_1DE1D4, (void *)&returnZero_1DE1DC, (void *)&func_800B1134_1DE1E4, NULL, NULL },
        { "CHRRT \0", '\1', "CHR ROTATE     \0", (void *)&noop_1DE2C8, (void *)&returnZero_1DE2D0, (void *)&func_800B1228_1DE2D8, NULL, (void *)&returnZero_1DE34C },
        { "CHRHP \0", '\1', "CHR HOP        \0", (void *)&noop_1DE354, (void *)&returnZero_1DE35C, (void *)&func_800B12B4_1DE364, NULL, NULL },
        { "CHRBK \0", '\1', "CHR BACK       \0", (void *)&noop_1DE3D8, (void *)&returnZero_1DE3E0, (void *)&func_800B1338_1DE3E8, NULL, (void *)&returnZero_1DE478 },
        { "CHRZ2 \0", '\1', "CHR ZOOM2      \0", (void *)&noop_1DE480, (void *)&returnZero_1DE488, (void *)&func_800B13E0_1DE490, NULL, (void *)&returnZero_1DE57C },
        { "CHRUD \0", '\1', "CHR UPDOWN     \0", (void *)&noop_1DE584, (void *)&returnZero_1DE58C, (void *)&func_800B14E4_1DE594, NULL, (void *)&returnZero_1DE638 },
        { "CHRFC \0", '\1', "CHR FACE       \0", (void *)&noop_1DE640, (void *)&returnZero_1DE648, (void *)&func_800B15A0_1DE650, NULL, NULL },
        { "CHRA2 \0", '\1', "CHR ANIME2     \0", (void *)&noop_1DE690, (void *)&returnZero_1DE698, (void *)&func_800B15F0_1DE6A0, NULL, (void *)&returnZero_1DE720 },
        { "CHRM3 \0", '\1', "CHR MOVE3      \0", (void *)&noop_1DE728, (void *)&returnZero_1DE730, (void *)&func_800B1688_1DE738, NULL, (void *)&returnZero_1DE7E4 },
        { "CHRMD \0", '\1', "CHR MODEL DISP \0", (void *)&noop_1DE7EC, (void *)&returnZero_1DE7F4, (void *)&func_800B174C_1DE7FC, NULL, NULL },
        { "CHRST \0", '\1', "CHR MOVE SIGHT \0", (void *)&noop_1DE860, (void *)&returnZero_1DE868, (void *)&func_800B17C0_1DE870, NULL, (void *)&returnZero_1DE910 },
        { "CHRAL \0", '\1', "CHR ALPHA      \0", (void *)&noop_1DE918, (void *)&returnZero_1DE920, (void *)&func_800B1878_1DE928, (void *)&func_800B19B8_1DEA68, (void *)&returnZero_1DEB38 },
        { "CHRP2 \0", '\1', "CHR POSITION2  \0", (void *)&noop_1DEB40, (void *)&returnZero_1DEB48, (void *)&func_800B1AA0_1DEB50, NULL, (void *)&returnZero_1DEC18 },
        { "CHRT3 \0", '\1', "CHR TURN3      \0", (void *)&noop_1DEC20, (void *)&returnZero_1DEC28, (void *)&func_800B1B80_1DEC30, NULL, (void *)&returnZero_1DECC0 },
        { "CHRDP \0", '\1', "CHR KAGEDISP   \0", (void *)&noop_1DECC8, (void *)&returnZero_1DECD0, (void *)&func_800B1C28_1DECD8, NULL, NULL },
        { "CAMRS \0", '\1', "CAMERA RESET   \0", (void *)&noop_1DD970, (void *)&returnZero_1DD978, (void *)&func_800B08D0_1DD980, NULL, NULL },
        { "CAMMV \0", '\1', "CAMERA MOVE    \0", (void *)&noop_1DD9C0, (void *)&returnZero_1DD9C8, (void *)&func_800B0920_1DD9D0, NULL, (void *)&returnZero_1DDA4C },
        { "CAMJI \0", '\1', "CAMERA JISHIN  \0", (void *)&noop_1DDA54, (void *)&returnZero_1DDA5C, (void *)&func_800B09B4_1DDA64, NULL, NULL },
        { "CAMRT \0", '\1', "CAMERA ROTATE  \0", (void *)&noop_1DDAA4, (void *)&returnZero_1DDAAC, (void *)&func_800B0A04_1DDAB4, NULL, (void *)&returnZero_1DDADC },
        { "SEP   \0", '\1', "SE PLAY        \0", (void *)&noop_1DF1C8, (void *)&returnZero_1DF1D0, (void *)&func_800B2128_1DF1D8, NULL, NULL },
        { "SEP3D \0", '\1', "SE3D PLAY      \0", (void *)&noop_1DF23C, (void *)&returnZero_1DF244, (void *)&func_800B219C_1DF24C, NULL, NULL },
        { "SEST  \0", '\1', "SE STOP        \0", (void *)&noop_1DF2D0, (void *)&returnZero_1DF2D8, (void *)&func_800B2230_1DF2E0, NULL, NULL },
        { "EFDP  \0", '\1', "EFFECT DISP    \0", (void *)&noop_1DED40, (void *)&returnZero_1DED48, (void *)&func_800B1CA0_1DED50, NULL, (void *)&returnZero_1DEDF4 },
        { "EFFAN \0", '\1', "EFFECT FAN     \0", (void *)&noop_1DEDFC, (void *)&returnZero_1DEE04, (void *)&func_800B1D5C_1DEE0C, NULL, (void *)&returnZero_1DEE4C },
        { "EFRKT \0", '\1', "EFFECT ROCKET  \0", (void *)&noop_1DEE54, (void *)&returnZero_1DEE5C, (void *)&func_800B1DB4_1DEE64, NULL, (void *)&returnZero_1DEEA4 },
        { "EFMLT \0", '\1', "EFFECT MLIGHT  \0", (void *)&noop_1DEEAC, (void *)&returnZero_1DEEB4, (void *)&func_800B1E0C_1DEEBC, NULL, (void *)&returnZero_1DF158 },
        { "EFTRK \0", '\1', "EFFECT TRICK   \0", (void *)&noop_1DF04C, (void *)&returnZero_1DF054, (void *)&func_800B1FAC_1DF05C, (void *)&func_800B2010_1DF0C0, (void *)&returnZero_1DF158 },
        { "BGMP  \0", '\1', "BGM PLAY       \0", (void *)&noop_1DD8C0, (void *)&returnZero_1DD8C8, (void *)&func_800B0820_1DD8D0, NULL, NULL },
        { "BGMFO \0", '\1', "BGM FADE OUT   \0", (void *)&noop_1DD8F8, (void *)&returnZero_1DD900, (void *)&func_800B0858_1DD908, NULL, NULL },
        { "BGMVL \0", '\1', "BGM VOLUME     \0", (void *)&noop_1DD934, (void *)&returnZero_1DD93C, (void *)&func_800B0894_1DD944, NULL, NULL },
        { "EF2PL \0", '\1', "EFFECT2 DISP   \0", NULL, NULL, NULL, NULL, NULL },
        { "S2WP  \0", '\1', "SYS2 WIPE      \0", (void *)&noop_1DF8B, (void *)&returnZero_1DF8B8, (void *)&func_800B2810_1DF8C0, NULL, (void *)&returnZero_1DF9DC },
        { "S2WT  \0", '\1', "SYS2 WAIT      \0", (void *)&noop_1DF9E4, (void *)&returnZero_1DF9EC, (void *)&func_800B2944_1DF9F4, NULL, NULL }
    }
};
// clang-format on

// clang-format off
struct {
    CommandEntry *unk0;
    s16 unk4;
    s16 unk6;
} commandCategories[9] = {
    { &commandTable.entries[COMMAND_INIT_TRACK],   0x01, 0 },
    { &commandTable.entries[COMMAND_SYS_DISP],     0x09, 0 },
    { &commandTable.entries[COMMAND_CHR_POSITION], 0x18, 1 },
    { &commandTable.entries[COMMAND_CAMERA_RESET], 0x04, 0 },
    { &commandTable.entries[COMMAND_SE_PLAY],      0x03, 0 },
    { &commandTable.entries[COMMAND_EFFECT_DISP],  0x05, 1 },
    { &commandTable.entries[COMMAND_BGM_PLAY],     0x03, 0 },
    { &commandTable.entries[COMMAND_EFFECT2_DISP], 0x01, 0 },
    { &commandTable.entries[COMMAND_SYS2_WIPE],    0x02, 1 } 
};
// clang-format on

u8 D_800BA5B8_1E7668 = 0;
u8 D_800BA5B9_1E7669 = 9;

void func_800BB47C(s32, s32, s32, s32, s32, s32);

CommandEntry *getCommandEntry(s32 arg0, s32 arg1) {
    CommandEntry *temp = commandCategories[(u8)arg0].unk0;
    return &temp[(u8)arg1];
}

CommandEntry *getCommandEntryMasked(s32 arg0, s32 arg1) {
    CommandEntry *temp = getCommandEntry(arg0 & 0xFF, arg1 & 0xFF);
    return temp;
}

u8 getCommandUnk7Field(s32 arg0, s32 arg1) {
    return getCommandEntry(arg0 & 0xFF, arg1 & 0xFF)->unk7;
}

void *getCommandDescription(u8 a0, u8 a1) {
    if (a0 == 0xFF || a1 == 0xFF) {
        return &commandTable.header;
    }

    return &getCommandEntry(a0, a1)->description;
}

u8 incrementCommandIndexWithWrap(u8 index, s32 value) {
    s32 inc = value + 1;
    s16 lim = commandCategories[index].unk4;
    u8 test = inc;
    s32 res = (test < lim) ? inc : 0;
    return res;
}

s32 decrementCommandIndexClamped(s32 arg0, s32 arg1) {
    s16 temp_v1 = commandCategories[arg0 & 0xFF].unk4;
    u8 var_a1 = arg1 - 1;

    if ((var_a1 & 0xFF) >= temp_v1) {
        var_a1 = temp_v1 - 1;
    }

    return var_a1 & 0xFF;
}

s32 clampCommandIndex(s32 arg0, u8 arg1) {
    s16 temp_v1 = commandCategories[arg0 & 0xFF].unk4;
    u8 var_a1 = arg1;

    if (arg1 >= temp_v1) {
        var_a1 = temp_v1 - 1;
    }

    return var_a1;
}

s32 getNextCategorySkipping(u8 nextIndex, s16 skipValue) {
    u16 limit = *(u16 *)&D_800BA5B8_1E7668;

    nextIndex++;
    if (nextIndex >= limit) {
        nextIndex = 0;
    }

    if (skipValue != -1) {
        while (commandCategories[nextIndex].unk6 == skipValue) {
            nextIndex++;
            if (nextIndex >= limit) {
                nextIndex = 0;
            }
        }
    }

    return nextIndex;
}

s32 getPrevCategorySkipping(s32 arg0, s32 arg1) {
    s16 temp_v1;
    s32 var_a0;
    s32 var_v0;

    var_a0 = arg0 - 1;
    if ((var_a0 & 0xFF) == 0xFF) {
        var_a0 = D_800BA5B9_1E7669 - 1;
        var_v0 = arg1 << 0x10;
    }

    temp_v1 = (s16)arg1;
    if (temp_v1 != -1 && commandCategories[var_a0 & 0xFF].unk6 == temp_v1) {
        var_a0 -= 1;
        if ((var_a0 & 0xFF) == 0xFF) {
            var_a0 = D_800BA5B9_1E7669 - 1;
        }
    }

    return var_a0 & 0xFF;
}

void initializeCutsceneCommand(
    CurrentCommand *currentCommand,
    CommandData *commandData,
    u8 commandCategory,
    u8 commandIndex,
    u8 frameIndex
) {
    s32 shouldRun;
    s32 temp;
    void (*handler)(CurrentCommand *, CommandData *, s8);
    s32 *temp2;

    shouldRun = TRUE;

    if ((commandCategory != 0 || commandIndex != 0) == 0 || currentCommand->commandCategory != commandCategory ||
        currentCommand->commandIndex != commandIndex) {
        resetScriptState(currentCommand->scriptState);
        currentCommand->commandCategory = commandCategory;
        currentCommand->commandIndex = commandIndex;

        handler = getCommandEntry(commandCategory, commandIndex & 0xFF)->func1;
        if (handler) {
            // basically: !arg1[arg4].isActive
            if ((*(temp2 = &commandData[(s8)frameIndex].isActive)) == FALSE) {
                shouldRun = shouldRun & (-(commandCategories[commandCategory].unk6 != 1));
            }

            if (shouldRun) {
                handler(currentCommand, commandData, frameIndex);
            }
        }
    }
}

s32 executeCommandFunc2(
    CurrentCommand *arg0,
    CommandData *arg1,
    s32 arg2,
    s32 arg3,
    s32 arg4,
    s8 arg5
) {
    s32 check;
    CommandEntry *temp_v0;
    CommandData *ptr;

    check = 1;
    temp_v0 = getCommandEntry(arg0->commandCategory, arg0->commandIndex);

    if (temp_v0->func2) {
        ptr = &arg1[arg5];
        if (ptr->isActive == 0) {
            check = commandCategories[arg0->commandCategory].unk6 != 1;
        }

        if (check) {
            return temp_v0->func2(arg0, arg1, arg2, arg3, arg4, arg5);
        }
    }

    return 0;
}

INCLUDE_ASM("asm/nonmatchings/1DD170", func_800B0534_1DD5E4);

INCLUDE_ASM("asm/nonmatchings/1DD170", func_800B0628_1DD6D8);

s16 executeCommandFunc5(CurrentCommand *arg0, CommandData *arg1, s8 arg2) {
    s32 var_s2 = 1;
    s16 (*temp_v1)(CurrentCommand *, CommandData *, s8) =
        getCommandEntry(arg0->commandCategory, arg0->commandIndex)->func5;

    if (temp_v1 != 0) {
        CommandData *temp = &arg1[arg2];
        if (temp->isActive == 0) {
            var_s2 = (commandCategories[arg0->commandCategory].unk6 != 1);
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

s16 getCategorySkipValue(s32 arg0) {
    return commandCategories[arg0 & 0xFF].unk6;
}