#include "1DD170.h"

#include "1DD8C0.h"
#include "1DD970.h"
#include "1DF180.h"
#include "1DF8B0.h"
#include "1DFAA0.h"
#include "common.h"
#include "cutscene/cutscene_chr.h"
#include "cutscene/cutscene_sys.h"
#include "cutscene/sys_effect.h"

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
        { "INTRK \0", '\1', "INIT TRACK     \0", (void *)&cutsceneInitTrack_init,               (void *)&cutsceneInitTrack_validate,             (void *)&cutsceneInitTrack_exec,               NULL,                               NULL },
        { "SYSDP \0", '\1', "SYS DISP       \0", (void *)&cutsceneSysDisp_init,      (void *)&cutsceneSysDisp_validate,      (void *)&cutsceneSysDisp_exec,      NULL,                               NULL },
        { "SYSFI \0", '\1', "SYS FADE IN    \0", (void *)&cutsceneSysFadeIn_init,    (void *)&cutsceneSysFadeIn_validate,    (void *)&cutsceneSysFadeIn_exec,    NULL,                               (void *)&cutsceneSysFadeIn_isDone },
        { "SYSFO \0", '\1', "SYS FADE OUT   \0", (void *)&cutsceneSysFadeOut_init,   (void *)&cutsceneSysFadeOut_validate,   (void *)&cutsceneSysFadeOut_exec,   NULL,                               (void *)&cutsceneSysFadeOut_isDone },
        { "SYSFL \0", '\1', "SYS FLASH      \0", (void *)&cutsceneSysFlash_init,     (void *)&cutsceneSysFlash_validate,     (void *)&cutsceneSysFlash_setup,    (void *)&cutsceneSysFlash_update,   (void *)&cutsceneSysFlash_isDone },
        { "SYSIM \0", '\1', "SYS INTMODE    \0", (void *)&cutsceneSysIntMode_init,   (void *)&cutsceneSysIntMode_validate,   (void *)&cutsceneSysIntMode_exec,   NULL,                               NULL },
        { "SYSWC \0", '\1', "SYS WIPE COLOR \0", (void *)&cutsceneSysWipeColor_init, (void *)&cutsceneSysWipeColor_validate, (void *)&cutsceneSysWipeColor_exec, NULL,                               NULL },
        { "SYSWT \0", '\1', "SYS WAIT       \0", (void *)&cutsceneSysWait_init,      (void *)&cutsceneSysWait_validate,      (void *)&cutsceneSysWait_exec,      NULL,                               NULL },
        { "SYSCU \0", '\1', "SYS CURTAIN    \0", (void *)&cutsceneSysCurtain_init,   (void *)&cutsceneSysCurtain_validate,   (void *)&cutsceneSysCurtain_exec,   (void *)&cutsceneSysCurtain_update, (void *)&cutsceneSysCurtain_isDone },
        { "SYSFL \0", '\1', "SYS FILTER     \0", (void *)&cutsceneSysFilter_init,    (void *)&cutsceneSysFilter_validate,    (void *)&cutsceneSysFilter_exec,    NULL,                               (void *)&cutsceneSysFilter_isDone },
        { "CHRPS \0", '\1', "CHR POSITION   \0", (void *)&cutsceneChrPosition_init,  (void *)&cutsceneChrPosition_validate,  (void *)&cutsceneChrPosition_exec,  NULL,                               (void *)&cutsceneChrPosition_isDone },
        { "CHRMV \0", '\1', "CHR MOVE       \0", (void *)&cutsceneChrMove_init,      (void *)&cutsceneChrMove_validate,      (void *)&cutsceneChrMove_exec,      NULL,                               (void *)&cutsceneChrMove_isDone },
        { "CHRAM \0", '\1', "CHR ANIME      \0", (void *)&cutsceneChrAnime_init,     (void *)&cutsceneChrAnime_validate,     (void *)&cutsceneChrAnime_exec,     NULL,                               (void *)&cutsceneChrAnime_isDone },
        { "CHRDP \0", '\1', "CHR DISP       \0", (void *)&cutsceneChrDisp_init,      (void *)&cutsceneChrDisp_validate,      (void *)&cutsceneChrDisp_exec,      NULL,                               NULL },
        { "CHRIT \0", '\1', "CHR ITEM       \0", (void *)&cutsceneChrItem_init,      (void *)&cutsceneChrItem_validate,      (void *)&cutsceneChrItem_exec,      NULL,                               NULL },
        { "CHRTN \0", '\1', "CHR TURN       \0", (void *)&cutsceneChrTurn_init,      (void *)&cutsceneChrTurn_validate,      (void *)&cutsceneChrTurn_exec,      NULL,                               NULL },
        { "CHRM2 \0", '\1', "CHR MOVE2      \0", (void *)&cutsceneChrMove2_init,     (void *)&cutsceneChrMove2_validate,     (void *)&cutsceneChrMove2_exec,     NULL,                               (void *)&cutsceneChrMove2_isDone },
        { "CHRZM \0", '\1', "CHR ZOOM       \0", (void *)&cutsceneChrZoom_init,      (void *)&cutsceneChrZoom_validate,      (void *)&cutsceneChrZoom_exec,      NULL,                               (void *)&cutsceneChrZoom_isDone },
        { "CHRBM \0", '\1', "CHR BOARD MOVE \0", (void *)&cutsceneChrBoardMove_init, (void *)&cutsceneChrBoardMove_validate, (void *)&cutsceneChrBoardMove_exec, NULL,                               (void *)&cutsceneChrBoardMove_isDone },
        { "CHRT2 \0", '\1', "CHR TURN2      \0", (void *)&cutsceneChrTurn2_init,     (void *)&cutsceneChrTurn2_validate,     (void *)&cutsceneChrTurn2_exec,     NULL,                               NULL },
        { "CHRRT \0", '\1', "CHR ROTATE     \0", (void *)&cutsceneChrRotate_init,    (void *)&cutsceneChrRotate_validate,    (void *)&cutsceneChrRotate_exec,    NULL,                               (void *)&cutsceneChrRotate_isDone },
        { "CHRHP \0", '\1', "CHR HOP        \0", (void *)&cutsceneChrHop_init,       (void *)&cutsceneChrHop_validate,       (void *)&cutsceneChrHop_exec,       NULL,                               NULL },
        { "CHRBK \0", '\1', "CHR BACK       \0", (void *)&cutsceneChrBack_init,      (void *)&cutsceneChrBack_validate,      (void *)&cutsceneChrBack_exec,      NULL,                               (void *)&cutsceneChrBack_isDone },
        { "CHRZ2 \0", '\1', "CHR ZOOM2      \0", (void *)&cutsceneChrZoom2_init,     (void *)&cutsceneChrZoom2_validate,     (void *)&cutsceneChrZoom2_exec,     NULL,                               (void *)&cutsceneChrZoom2_isDone },
        { "CHRUD \0", '\1', "CHR UPDOWN     \0", (void *)&cutsceneChrUpDown_init,    (void *)&cutsceneChrUpDown_validate,    (void *)&cutsceneChrUpDown_exec,    NULL,                               (void *)&cutsceneChrUpDown_isDone },
        { "CHRFC \0", '\1', "CHR FACE       \0", (void *)&cutsceneChrFace_init,      (void *)&cutsceneChrFace_validate,      (void *)&cutsceneChrFace_exec,      NULL,                               NULL },
        { "CHRA2 \0", '\1', "CHR ANIME2     \0", (void *)&cutsceneChrAnime2_init,    (void *)&cutsceneChrAnime2_validate,    (void *)&cutsceneChrAnime2_exec,    NULL,                               (void *)&cutsceneChrAnime2_isDone },
        { "CHRM3 \0", '\1', "CHR MOVE3      \0", (void *)&cutsceneChrMove3_init,     (void *)&cutsceneChrMove3_validate,     (void *)&cutsceneChrMove3_exec,     NULL,                               (void *)&cutsceneChrMove3_isDone },
        { "CHRMD \0", '\1', "CHR MODEL DISP \0", (void *)&cutsceneChrModelDisp_init, (void *)&cutsceneChrModelDisp_validate, (void *)&cutsceneChrModelDisp_exec, NULL,                               NULL },
        { "CHRST \0", '\1', "CHR MOVE SIGHT \0", (void *)&cutsceneChrMoveSight_init, (void *)&cutsceneChrMoveSight_validate, (void *)&cutsceneChrMoveSight_exec, NULL,                               (void *)&cutsceneChrMoveSight_isDone },
        { "CHRAL \0", '\1', "CHR ALPHA      \0", (void *)&cutsceneChrAlpha_init,     (void *)&cutsceneChrAlpha_validate,     (void *)&cutsceneChrAlpha_exec,     (void *)&cutsceneChrAlpha_update,   (void *)&cutsceneChrAlpha_isDone },
        { "CHRP2 \0", '\1', "CHR POSITION2  \0", (void *)&cutsceneChrPosition2_init, (void *)&cutsceneChrPosition2_validate, (void *)&cutsceneChrPosition2_exec, NULL,                               (void *)&cutsceneChrPosition2_isDone },
        { "CHRT3 \0", '\1', "CHR TURN3      \0", (void *)&cutsceneChrTurn3_init,     (void *)&cutsceneChrTurn3_validate,     (void *)&cutsceneChrTurn3_exec,     NULL,                               (void *)&cutsceneChrTurn3_isDone },
        { "CHRDP \0", '\1', "CHR KAGEDISP   \0", (void *)&cutsceneChrKageDisp_init,  (void *)&cutsceneChrKageDisp_validate,  (void *)&cutsceneChrKageDisp_exec,  NULL,                               NULL },
        { "CAMRS \0", '\1', "CAMERA RESET   \0", (void *)&noop_1DD970,               (void *)&returnZero_1DD978,             (void *)&func_800B08D0_1DD980,      NULL,                               NULL },
        { "CAMMV \0", '\1', "CAMERA MOVE    \0", (void *)&noop_1DD9C0,               (void *)&returnZero_1DD9C8,             (void *)&func_800B0920_1DD9D0,      NULL,                               (void *)&returnZero_1DDA4C },
        { "CAMJI \0", '\1', "CAMERA JISHIN  \0", (void *)&noop_1DDA54,               (void *)&returnZero_1DDA5C,             (void *)&func_800B09B4_1DDA64,      NULL,                               NULL },
        { "CAMRT \0", '\1', "CAMERA ROTATE  \0", (void *)&noop_1DDAA4,               (void *)&returnZero_1DDAAC,             (void *)&func_800B0A04_1DDAB4,      NULL,                               (void *)&returnZero_1DDADC },
        { "SEP   \0", '\1', "SE PLAY        \0", (void *)&noop_1DF1C8,               (void *)&returnZero_1DF1D0,             (void *)&func_800B2128_1DF1D8,      NULL,                               NULL },
        { "SEP3D \0", '\1', "SE3D PLAY      \0", (void *)&noop_1DF23C,               (void *)&returnZero_1DF244,             (void *)&func_800B219C_1DF24C,      NULL,                               NULL },
        { "SEST  \0", '\1', "SE STOP        \0", (void *)&noop_1DF2D0,               (void *)&returnZero_1DF2D8,             (void *)&func_800B2230_1DF2E0,      NULL,                               NULL },
        { "EFDP  \0", '\1', "EFFECT DISP    \0", (void *)&cutsceneEffectDisp_init,               (void *)&cutsceneEffectDisp_validate,             (void *)&cutsceneEffectDisp_exec,      NULL,                               (void *)&cutsceneEffectDisp_isDone },
        { "EFFAN \0", '\1', "EFFECT FAN     \0", (void *)&cutsceneEffectFan_init,               (void *)&cutsceneEffectFan_validate,             (void *)&cutsceneEffectFan_exec,      NULL,                               (void *)&cutsceneEffectFan_isDone },
        { "EFRKT \0", '\1', "EFFECT ROCKET  \0", (void *)&cutsceneEffectRocket_init,               (void *)&cutsceneEffectRocket_validate,             (void *)&cutsceneEffectRocket_exec,      NULL,                               (void *)&cutsceneEffectRocket_isDone },
        { "EFMLT \0", '\1', "EFFECT MLIGHT  \0", (void *)&cutsceneEffectMLight_init,               (void *)&cutsceneEffectMLight_validate,             (void *)&cutsceneEffectMLight_exec,      NULL,                               (void *)&cutsceneEffectTrick_isDone },
        { "EFTRK \0", '\1', "EFFECT TRICK   \0", (void *)&cutsceneEffectTrick_init,               (void *)&cutsceneEffectTrick_validate,             (void *)&cutsceneEffectTrick_exec,      (void *)&cutsceneEffectTrick_update,      (void *)&cutsceneEffectTrick_isDone },
        { "BGMP  \0", '\1', "BGM PLAY       \0", (void *)&noop_1DD8C0,               (void *)&returnZero_1DD8C8,             (void *)&func_800B0820_1DD8D0,      NULL,                               NULL },
        { "BGMFO \0", '\1', "BGM FADE OUT   \0", (void *)&noop_1DD8F8,               (void *)&returnZero_1DD900,             (void *)&func_800B0858_1DD908,      NULL,                               NULL },
        { "BGMVL \0", '\1', "BGM VOLUME     \0", (void *)&noop_1DD934,               (void *)&returnZero_1DD93C,             (void *)&func_800B0894_1DD944,      NULL,                               NULL },
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

CommandEntry *getCommandEntryMasked(u8 arg0, u8 arg1) {
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

s32 executeCommandFunc2(CurrentCommand *arg0, CommandData *arg1, s32 arg2, s32 arg3, s32 arg4, s8 arg5) {
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

INCLUDE_ASM("asm/nonmatchings/cutscene/1DD170", initializeSlotState);

INCLUDE_ASM("asm/nonmatchings/cutscene/1DD170", updateSlotData);

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

s32 getCategorySkipValue(u8 arg0) {
    return commandCategories[arg0].unk6;
}