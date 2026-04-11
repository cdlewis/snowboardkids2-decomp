#include "assets.h"
#include "audio/audio.h"
#include "common.h"
#include "data/course_data.h"
#include "gamestate.h"
#include "os_cont.h"
#include "race/race_session.h"
#include "system/task_scheduler.h"
#include "text/font_render.h"

extern void renderTiledSprite3x3(void *, s16, s16, s16, s16, u8, u8, u8, u8, u8);
extern s32 gControllerInputs;
/* Data segment - forward declarations */
extern u32 D_800BBA88_B6368[];
extern u32 D_800BBB38_B6418[];
extern u32 D_800BBBD4_B64B4[];
extern u32 D_800BBC98_B6578[];
extern u32 D_800BBD50_B6630[];
extern u32 D_800BBE04_B66E4[];
extern u32 D_800BBEC0_B67A0[];
extern u32 D_800BBF90_B6870[];
extern u32 D_800BC088_B6968[];
extern u32 D_800BC0D4_B69B4[];
extern u32 D_800BC1C0_B6AA0[];
extern u32 D_800BC270_B6B50[];
extern u32 D_800BC368_B6C48[];
extern u32 D_800BC47C_B6D5C[];
extern u32 D_800BC530_B6E10[];
extern u32 D_800BC5FC_B6EDC[];
extern u32 D_800BC6C8_B6FA8[];
extern u32 D_800BC780_B7060[];
extern u32 D_800BC884_B7164[];
extern u32 D_800BC974_B7254[];
extern u32 D_800BCA9C_B737C[];
extern s8 D_800BCB60_B7440[];
extern s8 D_800BCB64_B7444[];
extern s8 D_800BCB68_B7448[];
extern s8 D_800BCB6C_B744C[];
extern s8 D_800BCB70_B7450[];
extern s8 D_800BCB74_B7454[];
extern s8 D_800BCB78_B7458[];
extern s8 D_800BCB7C_B745C[];
extern s8 D_800BCB80_B7460[];
extern s8 D_800BCB84_B7464[];
extern s8 D_800BCB88_B7468[];
extern s8 D_800BCB8C_B746C[];
extern s8 D_800BCB90_B7470[];

typedef struct {
    void *uiAsset;
    void *textRenderContext;
    void *messageData;
    s16 panelIndex;
} TrainingInstructionState;

typedef struct {
    void *uiAsset;
    void *textRenderContext;
    s32 *messageData;
    s16 panelIndex;
    s16 messageIndex;
    s16 panelWidth;
    s16 panelHeight;
    s16 alphaColor;
    u8 scale;
    u8 colorIndex;
} TrainingInstructionRuntimeState;

#define GET_ALPHA_COLOR_HIGH_BYTE(arg0) (((u8 *)&(arg0)->alphaColor)[1])

/* Data segment - pointer table */
u32 *D_800BBA30_B6310[] = {
    (u32 *)D_800BBA88_B6368, (u32 *)D_800BBA88_B6368, (u32 *)D_800BBB38_B6418, (u32 *)D_800BBBD4_B64B4,
    (u32 *)D_800BBC98_B6578, (u32 *)D_800BBD50_B6630, (u32 *)D_800BBE04_B66E4, (u32 *)D_800BBEC0_B67A0,
    (u32 *)D_800BBF90_B6870, (u32 *)D_800BC088_B6968, (u32 *)D_800BC0D4_B69B4, (u32 *)D_800BC1C0_B6AA0,
    (u32 *)D_800BC270_B6B50, (u32 *)D_800BC368_B6C48, (u32 *)D_800BC47C_B6D5C, (u32 *)D_800BC530_B6E10,
    (u32 *)D_800BC5FC_B6EDC, (u32 *)D_800BC6C8_B6FA8, (u32 *)D_800BC780_B7060, (u32 *)D_800BC884_B7164,
    (u32 *)D_800BC974_B7254, (u32 *)D_800BCA9C_B737C,
};

/* Data segment - panel data arrays */
u32 D_800BBA88_B6368[] = {
    0xFFFC0005, 0xFFFBFFFB, 0xFFFBFFFB, 0xFFFBFFFB, 0xFFFBFFFB, 0xFFFBFFFB, 0x8049001C, 0x8041802E, 0x803F8041,
    0xFFFB805B, 0xFFFB0013, 0x8042803A, 0x803D8049, 0xFFFD000C, 0x8035802E, 0x80408032, 0xFFFB000C, 0x803C802E,
    0x80308035, 0x8048FFFD, 0x00138042, 0x803A803D, 0xFFFB802F, 0x8046FFFB, 0x803D803F, 0x80328040, 0x80408036,
    0x803B8034, 0xFFFB8041, 0x80358032, 0xFFFD000A, 0xFFFB000B, 0x80428041, 0x8041803C, 0x803BFFFB, 0x8041803C,
    0xFFFB803A, 0x803C8043, 0x8032FFFB, 0x8033803C, 0x803F8044, 0x802E803F, 0x80318050, 0xFFFF0000,
};

u32 D_800BBB38_B6418[] = {
    0xFFFC0005, 0xFFFBFFFB, 0xFFFBFFFB, 0xFFFBFFFB, 0xFFFBFFFB, 0xFFFBFFFB, 0xFFFBFFFB, 0xFFFB8049,
    0x000C803C, 0x803F803B, 0x8032803F, 0x8036803B, 0x80348049, 0xFFFD001D, 0x80368039, 0x8041FFFB,
    0x80418035, 0x8032FFFB, 0x000C803C, 0x803B8041, 0x803F803C, 0x8039FFFB, 0x001C8041, 0x80368030,
    0x8038FFFD, 0x8041803C, 0xFFFB8041, 0x80358032, 0xFFFB8039, 0x80328033, 0x80418051, 0x803F8036,
    0x80348035, 0x8041FFFB, 0x8041803C, 0xFFFB8041, 0x8042803F, 0x803B8050, 0xFFFF0000,
};

u32 D_800BBBD4_B64B4[] = {
    0xFFFC0005, 0xFFFBFFFB, 0xFFFBFFFB, 0xFFFBFFFB, 0xFFFBFFFB, 0xFFFBFFFB, 0xFFFBFFFB, 0xFFFBFFFB, 0xFFFBFFFB,
    0xFFFBFFFB, 0x8049000C, 0x803C8036, 0x803B8049, 0xFFFD000A, 0xFFFB8030, 0x803C8036, 0x803BFFFB, 0x80368040,
    0xFFFB8044, 0x803C803F, 0x80418035, 0xFFFB8025, 0x80248024, 0x00108050, 0xFFFD000E, 0x803B8041, 0x8032803F,
    0xFFFB802E, 0xFFFB8040, 0x8035803C, 0x803DFFFB, 0x80448036, 0x80418035, 0xFFFB8025, 0x80248024, 0x0010FFFD,
    0x8041803C, 0xFFFB802F, 0x80428046, 0xFFFB8044, 0x8032802E, 0x803D803C, 0x803B8040, 0xFFFB802E, 0x803B8031,
    0xFFFB8036, 0x80418032, 0x803A8040, 0x8050FFFF,
};

u32 D_800BBC98_B6578[] = {
    0xFFFC0005, 0xFFFBFFFB, 0xFFFBFFFB, 0xFFFBFFFB, 0xFFFBFFFB, 0xFFFBFFFB, 0xFFFBFFFB, 0xFFFBFFFB,
    0xFFFBFFFB, 0x80490020, 0x8032802E, 0x803D803C, 0x803B8040, 0x8049FFFD, 0x000E803B, 0x80418032,
    0x803FFFFB, 0x80418035, 0x8032FFFB, 0x001B8032, 0x8031FFFB, 0x001C8035, 0x803C803D, 0xFFFB8041,
    0x803CFFFB, 0x802F8042, 0x8046FFFD, 0x80448032, 0x802E803D, 0x803C803B, 0x80408050, 0xFFFD0019,
    0x803F8032, 0x80408040, 0xFFFB8041, 0x80358032, 0xFFFB0023, 0xFFFB000B, 0x80428041, 0x8041803C,
    0x803BFFFB, 0x8041803C, 0xFFFD8040, 0x8035803C, 0x803C8041, 0x8048FFFF,
};

u32 D_800BBD50_B6630[] = {
    0xFFFC0005, 0x001C803D, 0x80328030, 0x8036802E, 0x8039FFFB, 0x001D8032, 0x80308035, 0x803B8036, 0x803E8042,
    0x80328048, 0xFFFD0011, 0x803C8039, 0x8031FFFB, 0x8031803C, 0x8044803B, 0xFFFB8041, 0x80358032, 0xFFFB000C,
    0x803C803B, 0x8041803F, 0x803C8039, 0xFFFD001C, 0x80418036, 0x80308038, 0xFFFB8044, 0x80358032, 0x803BFFFB,
    0x80428040, 0x8036803B, 0x8034FFFB, 0x80418035, 0x8032FFFD, 0x000B803C, 0x803A802F, 0xFFFB8041, 0x803CFFFB,
    0x80408035, 0x803C803C, 0x8041FFFB, 0x802F8032, 0x80358036, 0x803B8031, 0xFFFB8046, 0x803C8042, 0x8048FFFF,
};

u32 D_800BBE04_B66E4[] = {
    0xFFFC0005, 0xFFFBFFFB, 0xFFFBFFFB, 0xFFFBFFFB, 0xFFFBFFFB, 0xFFFBFFFB, 0xFFFBFFFB, 0xFFFBFFFB,
    0xFFFBFFFB, 0xFFFBFFFB, 0x8049A012, 0x80418032, 0x803A8049, 0xFFFD000E, 0x803B8041, 0x8032803F,
    0xFFFB8041, 0x80358032, 0xFFFB000B, 0x80398042, 0x8032FFFB, 0x001C8035, 0x803C803D, 0xFFFB8041,
    0x803CFFFB, 0x802F8042, 0x8046FFFD, 0x80368041, 0x8032803A, 0x80408048, 0xFFFD0019, 0x803F8032,
    0x80408040, 0xFFFB8041, 0x80358032, 0xFFFB000B, 0xFFFB000B, 0x80428041, 0x8041803C, 0x803BFFFB,
    0x8041803C, 0xFFFB8042, 0x80408032, 0xFFFD8036, 0x80418032, 0x803A8040, 0x8048FFFF,
};

u32 D_800BBEC0_B67A0[] = {
    0xFFFC0005, 0xFFFBFFFB, 0xFFFBFFFB, 0xFFFBFFFB, 0xFFFBFFFB, 0xFFFBFFFB, 0xFFFBFFFB, 0x8049000C, 0x803C803F,
    0x803B8032, 0x803F8036, 0x803B8034, 0xFFFB8026, 0x8049FFFD, 0x001D803C, 0xFFFB803A, 0x802E8038, 0x8032FFFB,
    0x80408035, 0x802E803F, 0x803DFFFB, 0x80418042, 0x803F803B, 0x8040804E, 0xFFFD8035, 0x803C8039, 0x8031FFFB,
    0x8031803C, 0x8044803B, 0xFFFB8041, 0x80358032, 0xFFFB000C, 0x803C803B, 0x8041803F, 0x803C8039, 0xFFFD001C,
    0x80418036, 0x80308038, 0xFFFB8041, 0x803CFFFB, 0x80418035, 0x8032FFFB, 0x802F803C, 0x80418041, 0x803C803A,
    0xFFFD8039, 0x80328033, 0x80418051, 0x803F8036, 0x80348035, 0x80418048, 0xFFFF0000,
};

u32 D_800BBF90_B6870[] = {
    0xFFFC0005, 0xFFFBFFFB, 0xFFFBFFFB, 0xFFFBFFFB, 0xFFFBFFFB, 0xFFFBFFFB, 0xFFFBFFFB, 0x80490010, 0x803F802E,
    0x802FFFFB, 0x8041803F, 0x80368030, 0x80388049, 0xFFFD0019, 0x803F8032, 0x80408040, 0xFFFB802E, 0x803B8046,
    0xFFFB000C, 0xFFFB000B, 0x80428041, 0x8041803C, 0x803BFFFB, 0x80448035, 0x80368039, 0x8032FFFD, 0x8036803B,
    0xFFFB8041, 0x80358032, 0xFFFB802E, 0x8036803F, 0xFFFB8041, 0x803CFFFB, 0x8031803C, 0xFFFB8041, 0x80358036,
    0x8040FFFB, 0x8041803F, 0x80368030, 0x80388048, 0xFFFD001D, 0x802E803D, 0xFFFB803C, 0x80418035, 0x8032803F,
    0xFFFB000C, 0xFFFB000B, 0x80428041, 0x8041803C, 0x803B8040, 0xFFFB8041, 0x803CFFFD, 0x8031803C, 0xFFFB803A,
    0x80428039, 0x80418036, 0x803D8039, 0x8032FFFB, 0x8034803F, 0x802E802F, 0x80408048, 0xFFFF0000,
};

u32 D_800BC088_B6968[] = {
    0xFFFC0005, 0x000D803C, 0xFFFB8034, 0x803F802E, 0x802FFFFB, 0x8041803F, 0x80368030,
    0x80388040, 0xFFFB8041, 0x803CFFFB, 0x8032802E, 0x803F803B, 0xFFFB8040, 0x803C803A,
    0x8032FFFD, 0x803A803C, 0x803B8032, 0x80468050, 0xFFFF0000,
};

u32 D_800BC0D4_B69B4[] = {
    0xFFFC0005, 0xFFFBFFFB, 0xFFFBFFFB, 0xFFFBFFFB, 0xFFFBFFFB, 0x8049001C, 0x803D8036, 0x803B803B, 0x8036803B,
    0x8034FFFB, 0x8041803F, 0x80368030, 0x80388049, 0xFFFD0019, 0x803F8032, 0x80408040, 0xFFFB802E, 0x803B8031,
    0xFFFB8035, 0x803C8039, 0x8031FFFB, 0x8031803C, 0x8044803B, 0xFFFD8041, 0x80358032, 0xFFFB000A, 0xFFFB000B,
    0x80428041, 0x8041803C, 0x803B804E, 0xFFFB8041, 0x80358032, 0x803BFFFB, 0x8035803C, 0x80398031, 0xFFFD8031,
    0x803C8044, 0x803BFFFB, 0x80418035, 0x8032FFFB, 0x000C803C, 0x803B8041, 0x803F803C, 0x8039FFFB, 0x001C8041,
    0x80368030, 0x80388050, 0xFFFD001B, 0x80328039, 0x8032802E, 0x80408032, 0xFFFB8041, 0x80358032, 0xFFFB000A,
    0xFFFB000B, 0x80428041, 0x8041803C, 0x803B8050, 0xFFFF0000,
};

u32 D_800BC1C0_B6AA0[] = {
    0xFFFC0005, 0x0022803C, 0x8042804B, 0x80398039, 0xFFFB8033, 0x802E8039, 0x8039FFFB, 0x80368033, 0xFFFB8046,
    0x803C8042, 0xFFFB8041, 0x803F8046, 0xFFFB8041, 0x803CFFFD, 0x8039802E, 0x803B8031, 0xFFFB8044, 0x80358036,
    0x80398032, 0xFFFB8040, 0x803D8036, 0x803B803B, 0x8036803B, 0x80348050, 0xFFFD000D, 0x803CFFFB, 0x8040803D,
    0x8036803B, 0x803B8036, 0x803B8034, 0xFFFB8041, 0x803F8036, 0x80308038, 0x8040FFFB, 0x8041803C, 0xFFFB8032,
    0x802E803F, 0x803BFFFD, 0x803A803C, 0x803F8032, 0xFFFB803A, 0x803C803B, 0x80328046, 0x8048FFFF,
};

u32 D_800BC270_B6B50[] = {
    0xFFFC0005, 0xFFFBFFFB, 0xFFFBFFFB, 0xFFFB8049, 0x000C803C, 0x803A802F, 0x8036803B, 0x802E8041, 0x8036803C,
    0x803BFFFB, 0x8041803F, 0x80368030, 0x80388040, 0x8049FFFD, 0x0019803F, 0x80328040, 0x8040FFFB, 0x80418035,
    0x8032FFFB, 0x000CFFFB, 0x000B8042, 0x80418041, 0x803C803B, 0xFFFD8044, 0x80358036, 0x80398032, 0xFFFB8031,
    0x803C8036, 0x803B8034, 0xFFFB8040, 0x803D8036, 0x803B803B, 0x8036803B, 0x8034FFFB, 0x8041803F, 0x80368030,
    0x80388040, 0xFFFD8041, 0x803CFFFB, 0x803D8032, 0x803F8033, 0x803C803F, 0x803AFFFB, 0x8030803C, 0x803A802F,
    0x803CFFFB, 0x8041803F, 0x80368030, 0x80388040, 0x8050FFFD, 0x0022803C, 0x8042804B, 0x80398039, 0xFFFB8032,
    0x802E803F, 0x803BFFFB, 0x803A803C, 0x803F8032, 0xFFFB803A, 0x803C803B, 0x80328046, 0x8050FFFF,
};

u32 D_800BC368_B6C48[] = {
    0xFFFC0005, 0xFFFBFFFB, 0xFFFB8049, 0x000C803C, 0x803B8041, 0x8036803B, 0x8042803C, 0x80428040, 0xFFFB001C,
    0x803D8036, 0x803BFFFB, 0x8041803F, 0x80368030, 0x80388049, 0xFFFD0019, 0x803F8032, 0x80408040, 0xFFFB8041,
    0x80358032, 0xFFFB000A, 0xFFFB000B, 0x80428041, 0x8041803C, 0x803BFFFB, 0x802E803B, 0x8031FFFD, 0x8035803C,
    0x80398031, 0xFFFB8031, 0x803C8044, 0x803BFFFB, 0x80418035, 0x8032FFFB, 0x000C803C, 0x803B8041, 0x803F803C,
    0x8039FFFB, 0x001C8041, 0x80368030, 0x80388050, 0xFFFD001B, 0x80328039, 0x8032802E, 0x80408032, 0xFFFB8041,
    0x80358032, 0xFFFB000A, 0xFFFB000B, 0x80428041, 0x8041803C, 0x803B804E, 0xFFFB8041, 0x80358032, 0x803BFFFD,
    0x8041802E, 0x803DFFFB, 0x80418035, 0x8032FFFB, 0x000AFFFB, 0x000B8042, 0x80418041, 0x803C803B, 0xFFFB803F,
    0x8032803D, 0x8032802E, 0x80418032, 0x80318039, 0x80468050, 0xFFFF0000,
};

u32 D_800BC47C_B6D5C[] = {
    0xFFFC0005, 0x0022803C, 0x8042FFFB, 0x80448036, 0x80398039, 0xFFFB8040, 0x803D8036, 0x803BFFFB, 0x802E8040,
    0xFFFB803A, 0x802E803B, 0x8046FFFB, 0x80418036, 0x803A8032, 0x8040FFFD, 0x802E8040, 0xFFFB8046, 0x803C8042,
    0xFFFB8041, 0x802E803D, 0xFFFB8041, 0x80358032, 0xFFFB000A, 0xFFFB000B, 0x80428041, 0x8041803C, 0x803B8048,
    0xFFFD000E, 0x802E803F, 0x803BFFFB, 0x803A803C, 0x803F8032, 0xFFFB803A, 0x803C803B, 0x80328046, 0xFFFB802F,
    0x8046FFFB, 0x8040803D, 0x8036803B, 0x803B8036, 0x803B8034, 0xFFFD803A, 0x803C803F, 0x80328048, 0xFFFF0000,
};

u32 D_800BC530_B6E10[] = {
    0xFFFC0005, 0x0022803C, 0x8042FFFB, 0x8030802E, 0x803BFFFB, 0x80308035, 0x802E803B, 0x80348032, 0xFFFB8041,
    0x80358032, 0xFFFD8031, 0x8036803F, 0x80328030, 0x80418036, 0x803C803B, 0xFFFB803C, 0x8033FFFB, 0x80418035,
    0x8032FFFB, 0x8040803D, 0x8036803B, 0xFFFB8036, 0x8033FFFD, 0x8046803C, 0x8042FFFB, 0x8035803C, 0x80398031,
    0xFFFB8031, 0x803C8044, 0x803BFFFB, 0x80418035, 0x8032FFFB, 0x000C803C, 0x803B8041, 0x803F803C, 0x8039FFFD,
    0x001C8041, 0x80368030, 0x8038FFFB, 0x8036803B, 0xFFFB8031, 0x80368033, 0x80338032, 0x803F8032, 0x803B8041,
    0xFFFD803D, 0x803C8040, 0x80368041, 0x8036803C, 0x803B8040, 0x8048FFFF,
};

u32 D_800BC5FC_B6EDC[] = {
    0xFFFC0005, 0xFFFBFFFB, 0xFFFBFFFB, 0xFFFBFFFB, 0xFFFB8049, 0xFFFC0008, 0xF05DFFFC, 0x0005FFFB, 0x0020802E,
    0x803F803B, 0x8036803B, 0x8034FFFB, 0x001C8036, 0x8034803B, 0x8049FFFD, 0x00208035, 0x8032803B, 0xFFFB802E,
    0xFFFB803F, 0x80368043, 0x802E8039, 0x804B8040, 0xFFFB8040, 0x8035803C, 0x8041FFFD, 0x802E803D, 0x803D803F,
    0x803C802E, 0x80308035, 0x80328040, 0xFFFB8046, 0x803C8042, 0x804E8046, 0x803C8042, 0x804B8039, 0x8039FFFB,
    0x80408032, 0x8032FFFD, 0xFFFC0008, 0xF05DFFFC, 0x0005FFFB, 0x80408036, 0x8034803B, 0xFFFB803C, 0x803BFFFB,
    0x80418035, 0x8032FFFB, 0x80408030, 0x803F8032, 0x8032803B, 0x8050FFFF,
};

u32 D_800BC6C8_B6FA8[] = {
    0xFFFC0005, 0x00208035, 0x8032803B, 0xFFFB8041, 0x80358032, 0xFFFBFFFC, 0x0008F05D, 0xFFFC0005,
    0xFFFB802F, 0x80398036, 0x803B8038, 0x8040804E, 0xFFFB8037, 0x8042803A, 0x803D8048, 0xFFFDA012,
    0x8033FFFB, 0x80418035, 0x8032FFFB, 0x80418036, 0x803A8036, 0x803B8034, 0xFFFB8036, 0x8040FFFB,
    0x803F8036, 0x80348035, 0x8041804E, 0xFFFD8046, 0x803C8042, 0xFFFB8030, 0x802E803B, 0xFFFB8031,
    0x803C8031, 0x80348032, 0xFFFB8041, 0x80358032, 0xFFFB802E, 0x80418041, 0x802E8030, 0x8038FFFD,
    0x802F8046, 0xFFFB8037, 0x8042803A, 0x803D8036, 0x803B8034, 0x8048FFFF,
};

u32 D_800BC780_B7060[] = {
    0xFFFC0005, 0xFFFBFFFB, 0xFFFBFFFB, 0xFFFBFFFB, 0xFFFBFFFB, 0xFFFBFFFB, 0xFFFBFFFB, 0xFFFBFFFB, 0xFFFBFFFB,
    0xFFFBFFFB, 0x80490015, 0x80368033, 0x80418049, 0xFFFD000A, 0x803D803D, 0x803F803C, 0x802E8030, 0x8035FFFB,
    0x80418035, 0x8032FFFB, 0x80398036, 0x80338041, 0xFFFB8044, 0x80358032, 0x803BFFFD, 0x8046803C, 0x8042FFFB,
    0x8030803C, 0x803A803D, 0x80398032, 0x80418032, 0xFFFB802E, 0xFFFB8039, 0x802E803D, 0x8050FFFD, 0x001D8035,
    0x8032FFFB, 0x80398036, 0x80338041, 0xFFFB8033, 0x803C803F, 0xFFFB8041, 0x80358036, 0x8040FFFB, 0x8030803C,
    0x8042803F, 0x80408032, 0xFFFD8036, 0x8040FFFB, 0x8039803C, 0x8030802E, 0x80418032, 0x8031FFFB, 0x803C803B,
    0xFFFB8041, 0x80358032, 0xFFFB8039, 0x80328033, 0x8041804F, 0x8035802E, 0x803B8031, 0xFFFD8040, 0x80368031,
    0x80328050, 0xFFFF0000,
};

u32 D_800BC884_B7164[] = {
    0xFFFC0005, 0xFFFB8049, 0x000A8030, 0x80308032, 0x80398032, 0x803F802E, 0x80418036, 0x803C803B, 0xFFFB001D,
    0x80328030, 0x8035803B, 0x8036803E, 0x80428032, 0x8049FFFD, 0x00208035, 0x8032803B, 0xFFFB8046, 0x803C8042,
    0xFFFB8035, 0x80368041, 0xFFFB8041, 0x80358032, 0xFFFB8044, 0x802E8039, 0x8039FFFB, 0x802E803B, 0x8031FFFD,
    0x80408039, 0x803C8044, 0xFFFB8031, 0x803C8044, 0x803B804E, 0xFFFD8035, 0x803C8039, 0x8031FFFB, 0x80418035,
    0x8032FFFB, 0x80408041, 0x80368030, 0x8038FFFB, 0x8042803D, 0x8044802E, 0x803F8031, 0xFFFB802E, 0x803B8031,
    0xFFFD8037, 0x8042803A, 0x803DFFFB, 0x8041803C, 0xFFFB802E, 0x80308030, 0x80328039, 0x8032803F, 0x802E8041,
    0x8032FFFB, 0x8033802E, 0x80408041, 0x8032803F, 0x8050FFFD, 0xFFFF0000,
};

u32 D_800BC974_B7254[] = {
    0xFFFC0005, 0x000B8042, 0x8041FFFB, 0x80368033, 0xFFFB8046, 0x803C8042, 0xFFFB8037, 0x8042803A, 0x803DFFFB,
    0x8041803C, 0x803CFFFB, 0x80358036, 0x80348035, 0x804EFFFD, 0x802EFFFB, 0x8041803F, 0x80368030, 0x8038FFFB,
    0x80448036, 0x80398039, 0xFFFB802F, 0x8032FFFB, 0x80328045, 0x80328030, 0x80428041, 0x80328031, 0x804EFFFD,
    0x8040803C, 0xFFFB802F, 0x8032FFFB, 0x8030802E, 0x803F8032, 0x80338042, 0x80398050, 0xFFFD000A, 0x80398040,
    0x803C804E, 0xFFFB8046, 0x803C8042, 0xFFFB8030, 0x802E803B, 0x804B8041, 0xFFFB8034, 0x803CFFFB, 0x802F8032,
    0x8046803C, 0x803B8031, 0xFFFD8041, 0x80358032, 0xFFFB803A, 0x802E8045, 0x8050FFFB, 0x8040803D, 0x80328032,
    0x8031FFFB, 0x80328043, 0x8032803B, 0xFFFB8044, 0x80368041, 0x8035FFFB, 0x80418035, 0x8032FFFD, 0x000C803C,
    0x803B8041, 0x803F803C, 0x8039FFFB, 0x001C8041, 0x80368030, 0x8038FFFB, 0x80358032, 0x80398031, 0xFFFB8042,
    0x803D8050, 0xFFFF0000,
};

u32 D_800BCA9C_B737C[] = {
    0xFFFC0005, 0x001D8035, 0x80368040, 0xFFFB8041, 0x803F802E, 0x8036803B, 0x8036803B, 0x8034FFFB, 0x80408032,
    0x80408040, 0x8036803C, 0x803BFFFD, 0x80368040, 0xFFFB803B, 0x803C8044, 0xFFFB803C, 0x80438032, 0x803F8050,
    0xFFFD000F, 0x803C803F, 0xFFFB8041, 0x80358032, 0xFFFB8040, 0x80328030, 0x803C803B, 0x8031FFFB, 0x8039802E,
    0x803D804E, 0xFFFD803F, 0x8032803A, 0x8032803A, 0x802F8032, 0x803FFFFB, 0x80448035, 0x802E8041, 0xFFFB8046,
    0x803C8042, 0xFFFB8039, 0x8032802E, 0x803F803B, 0x80328031, 0xFFFD802E, 0x803B8031, 0xFFFB8040, 0x803B803C,
    0x8044802F, 0x803C802E, 0x803F8031, 0x8048FFFF,
};

/* Data segment - message index tables */
s8 D_800BCB60_B7440[] = { 1, -1, 0, 0 };
s8 D_800BCB64_B7444[] = { 2, -1, 0, 0 };
s8 D_800BCB68_B7448[] = { 3, -1, 0, 0 };
s8 D_800BCB6C_B744C[] = { 4, 5, -1, 0 };
s8 D_800BCB70_B7450[] = { 6, -1, 0, 0 };
s8 D_800BCB74_B7454[] = { 7, -1, 0, 0 };
s8 D_800BCB78_B7458[] = { 8, 9, -1, 0 };
s8 D_800BCB7C_B745C[] = { 10, 11, -1, 0 };
s8 D_800BCB80_B7460[] = { 12, -1, 0, 0 };
s8 D_800BCB84_B7464[] = { 13, 14, 15, -1 };
s8 D_800BCB88_B7468[] = { 16, 17, -1, 0 };
s8 D_800BCB8C_B746C[] = { 18, 19, 20, -1 };
s8 D_800BCB90_B7470[] = { 21, -1, 0, 0 };

/* Data segment - panel message table pointers */
s8 *s_trainingPanelMessageTables[] = {
    (s8 *)D_800BCB60_B7440,
    (s8 *)D_800BCB64_B7444,
    (s8 *)D_800BCB68_B7448,
    (s8 *)D_800BCB6C_B744C,
    (s8 *)D_800BCB70_B7450,
    (s8 *)D_800BCB74_B7454,
    (s8 *)D_800BCB78_B7458,
    (s8 *)D_800BCB7C_B745C,
    (s8 *)D_800BCB80_B7460,
    (s8 *)D_800BCB84_B7464,
    (s8 *)D_800BCB88_B7468,
    (s8 *)D_800BCB8C_B746C,
    (s8 *)D_800BCB90_B7470,
    NULL,
    NULL,
};

void shrinkTrainingInstructionPanelWidth(TrainingInstructionRuntimeState *arg0);

void cleanupTrainingInstructionTask(TrainingInstructionState *arg0);

void checkTrainingInstructionCheckpoint(TrainingInstructionRuntimeState *arg0);

void initTrainingInstructionTask(TrainingInstructionState *arg0) {
    getCurrentAllocation();
    arg0->textRenderContext = loadTextRenderAsset(1);
    arg0->uiAsset = loadAsset_34F7E0();
    arg0->messageData = loadCompressedData(&_40E1C0_ROM_START, &_40E1C0_ROM_END, 0x1130);
    arg0->panelIndex = 0;
    setCleanupCallback(cleanupTrainingInstructionTask);
    setCallback(checkTrainingInstructionCheckpoint);
}

void expandTrainingInstructionPanelWidth(TrainingInstructionRuntimeState *arg0);

void checkTrainingInstructionCheckpoint(TrainingInstructionRuntimeState *arg0) {
    GameState *state = getCurrentAllocation();

    if (state->gamePaused == 0) {
        switch (arg0->panelIndex) {
            case 0:
                if (state->raceIntroState != 0) {
                    break;
                }
                goto do_action;
            case 1:
                if ((state->players->sectorIndex < 3) || (state->players->animFlags & 1)) {
                    break;
                }
                goto do_action;
            case 2:
                if (state->players->sectorIndex < 0xE) {
                    break;
                }
                goto do_action;
            case 3:
                if ((state->players->sectorIndex < 0x12) || (state->players->animFlags & 1)) {
                    break;
                }
                goto do_action;
            case 4:
                if (state->players->sectorIndex < 0x14) {
                    break;
                }
                goto do_action;
            case 5:
                if (state->players->sectorIndex < 0x1A) {
                    break;
                }
                goto do_action;
            case 6:
                if (state->players->sectorIndex < 0x28) {
                    break;
                }
                goto do_action;
            case 7:
                if (state->players->sectorIndex < 0x2D) {
                    break;
                }
                goto do_action;
            case 8:
                if (state->players->sectorIndex < 0x30) {
                    break;
                }
                goto do_action;
            case 9:
                if (state->players->sectorIndex < 0x32) {
                    break;
                }
                goto do_action;
            case 10:
                if (state->players->sectorIndex < 0x35) {
                    break;
                }
                goto do_action;
            case 11:
                if (state->players->sectorIndex < 0x38) {
                    break;
                }
                goto do_action;
            case 12:
                if ((state->players->currentLap == 0) || (state->players->sectorIndex == 0)) {
                    break;
                }
                goto do_action;
            case 13:
                break;
            default:
            do_action:
                state->trainingPanelState = 1;
                playSoundEffect(0x2C);
                arg0->panelWidth = 1;
                arg0->panelHeight = 1;
                arg0->alphaColor = 0xF0;
                arg0->messageIndex = 0;
                *(s16 *)&arg0->scale = 0xC0;
                setCallback(expandTrainingInstructionPanelWidth);
                break;
        }
    }
}

void expandTrainingInstructionPanelHeight(TrainingInstructionRuntimeState *arg0);
void displayTrainingInstructionAndWaitForInput(TrainingInstructionRuntimeState *arg0);
void shrinkTrainingInstructionPanelForNextInstruction(TrainingInstructionRuntimeState *arg0);

void expandTrainingInstructionPanelWidth(TrainingInstructionRuntimeState *arg0) {
    s16 width;
    s16 height;

    arg0->panelWidth += 2;

    if (arg0->alphaColor != 0x40) {
        arg0->alphaColor -= 0x10;
    }

    if (arg0->panelWidth == 0xD) {
        setCallback(expandTrainingInstructionPanelHeight);
    }

    width = arg0->panelWidth;
    height = arg0->panelHeight;

    renderTiledSprite3x3(
        arg0->uiAsset,
        ((-width) << 19) >> 16,
        ((-height) << 19) >> 16,
        width,
        height,
        0,
        GET_ALPHA_COLOR_HIGH_BYTE(arg0),
        arg0->colorIndex,
        0xC,
        0x6
    );
}

void shrinkTrainingInstructionPanelForNextMessage(TrainingInstructionRuntimeState *arg0) {
    s16 width;
    s16 height;

    arg0->panelHeight--;
    arg0->alphaColor += 0x10;

    if (arg0->panelHeight == 1) {
        setCallback(expandTrainingInstructionPanelHeight);
    }

    width = arg0->panelWidth;
    height = arg0->panelHeight;

    renderTiledSprite3x3(
        arg0->uiAsset,
        ((-width) << 19) >> 16,
        ((-height) << 19) >> 16,
        width,
        height,
        0,
        (u8)arg0->alphaColor,
        arg0->colorIndex,
        0xC,
        0x6
    );
}

void expandTrainingInstructionPanelHeight(TrainingInstructionRuntimeState *arg0) {
    s16 width;
    s16 height;

    arg0->panelHeight++;

    if (arg0->alphaColor != 0x40) {
        arg0->alphaColor -= 0x10;
    }

    if (arg0->panelHeight == 6) {
        setCallback(displayTrainingInstructionAndWaitForInput);
    }

    width = arg0->panelWidth;
    height = arg0->panelHeight;

    renderTiledSprite3x3(
        arg0->uiAsset,
        ((-width) << 19) >> 16,
        ((-height) << 19) >> 16,
        width,
        height,
        0,
        GET_ALPHA_COLOR_HIGH_BYTE(arg0),
        arg0->colorIndex,
        0xC,
        0x6
    );
}

void displayTrainingInstructionAndWaitForInput(TrainingInstructionRuntimeState *arg0) {
    s8 *table_ptr;
    s32 temp_v1_2;
    s16 temp_v0;
    s8 temp_v1_3;
    s32 s0_var = 6;
    s32 s1_var = 0xC;

    table_ptr = s_trainingPanelMessageTables[arg0->panelIndex];
    temp_v1_2 = arg0->messageData[table_ptr[arg0->messageIndex]];
    func_80035260_35E60(
        arg0->textRenderContext,
        (void *)arg0->messageData + temp_v1_2,
        -0x68,
        -0x30,
        0xFF,
        0xFF,
        0,
        s1_var,
        s0_var
    );

    renderTiledSprite3x3(
        arg0->uiAsset,
        -0x68,
        -0x30,
        0xD,
        s0_var,
        1,
        GET_ALPHA_COLOR_HIGH_BYTE(arg0),
        arg0->colorIndex,
        s1_var,
        s0_var
    );

    if (gControllerInputs & A_BUTTON) {
        temp_v0 = arg0->messageIndex + 1;
        arg0->messageIndex = temp_v0;
        table_ptr = s_trainingPanelMessageTables[arg0->panelIndex];
        temp_v1_3 = table_ptr[temp_v0];
        if (temp_v1_3 == -1) {
            playSoundEffect(0x2D);
            arg0->panelIndex = arg0->panelIndex + 1;
            setCallback(shrinkTrainingInstructionPanelForNextInstruction);
        } else {
            playSoundEffect(0x2B);
            setCallback(shrinkTrainingInstructionPanelForNextMessage);
        }
    }
}

#define GET_SCALE_AS_S16(arg0) (*(s16 *)&(arg0)->scale)
#define SET_SCALE_AS_S16(arg0, val) (*(s16 *)&(arg0)->scale = (val))

void shrinkTrainingInstructionPanelForNextInstruction(TrainingInstructionRuntimeState *arg0) {
    s16 scale;
    s16 width;
    s16 height;

    arg0->panelHeight--;

    scale = GET_SCALE_AS_S16(arg0);
    if (scale != 0) {
        SET_SCALE_AS_S16(arg0, scale - 0x10);
    }

    if (arg0->panelHeight == 1) {
        setCallback(shrinkTrainingInstructionPanelWidth);
    }

    width = arg0->panelWidth;
    height = arg0->panelHeight;

    renderTiledSprite3x3(
        arg0->uiAsset,
        ((-width) << 19) >> 16,
        ((-height) << 19) >> 16,
        width,
        height,
        0,
        (u8)arg0->alphaColor,
        arg0->colorIndex,
        0xC,
        0x6
    );
}

void shrinkTrainingInstructionPanelWidth(TrainingInstructionRuntimeState *arg0) {
    s16 scale;
    s16 width;
    s16 height;
    GameState *state;

    state = getCurrentAllocation();
    arg0->panelWidth--;

    scale = GET_SCALE_AS_S16(arg0);
    if (scale != 0) {
        SET_SCALE_AS_S16(arg0, scale - 0x10);
    }

    if (arg0->panelWidth == 1) {
        state->trainingPanelState = -1;
        setCallback(checkTrainingInstructionCheckpoint);
    }

    width = arg0->panelWidth;
    height = arg0->panelHeight;

    renderTiledSprite3x3(
        arg0->uiAsset,
        ((-width) << 19) >> 16,
        ((-height) << 19) >> 16,
        width,
        height,
        0,
        GET_ALPHA_COLOR_HIGH_BYTE(arg0),
        arg0->colorIndex,
        0xC,
        0x6
    );
}

void cleanupTrainingInstructionTask(TrainingInstructionState *arg0) {
    arg0->uiAsset = freeNodeMemory(arg0->uiAsset);
    arg0->textRenderContext = freeNodeMemory(arg0->textRenderContext);
    arg0->messageData = freeNodeMemory(arg0->messageData);
}
