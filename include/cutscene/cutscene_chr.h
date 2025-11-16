#include "1DFAA0.h"
#include "common.h"

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s16 unkC;
    s16 unkE;
} cutsceneChrHop_exec_arg;

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
} cutsceneChrTurn3_exec_arg;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s8 unk12;
    s8 unk13;
    s16 unk14;
    s8 unk16;
} cutsceneChrMove3_exec_arg;

typedef struct {
    s8 unk0;
    s8 unk1;
    s16 unk2;
} cutsceneChrModelDisp_exec_arg;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s8 unk12;
    s8 unk13;
    s16 unk14;
    s32 unk18;
    s32 unk1C;
} cutsceneChrMoveSight_exec_arg;

void cutsceneChrPosition_init(void);
s32 cutsceneChrPosition_validate(void);
void cutsceneChrPosition_exec(void);
s32 cutsceneChrPosition_isDone(void);
void cutsceneChrMove_init(void);
s32 cutsceneChrMove_validate(void);
void cutsceneChrMove_exec(void);
s32 cutsceneChrMove_isDone(void);
void cutsceneChrAnime_init(void);
s32 cutsceneChrAnime_validate(void);
void cutsceneChrAnime_exec(void);
s32 cutsceneChrAnime_isDone(void);
void cutsceneChrDisp_init(void);
s32 cutsceneChrDisp_validate(void);
void cutsceneChrDisp_exec(void);
void cutsceneChrItem_init(void);
s32 cutsceneChrItem_validate(void);
void cutsceneChrItem_exec(s8 *a0, CutsceneManager *arg1, s8 arg2);
void cutsceneChrTurn_init(void);
s32 cutsceneChrTurn_validate(void);
void cutsceneChrTurn_exec(void);
void cutsceneChrMove2_init(void);
s32 cutsceneChrMove2_validate(void);
void cutsceneChrMove2_exec(void);
s32 cutsceneChrMove2_isDone(void);
void cutsceneChrZoom_init(void);
s32 cutsceneChrZoom_validate(void);
void cutsceneChrZoom_exec(void);
s32 cutsceneChrZoom_isDone(void);
void cutsceneChrBoardMove_init(void);
s32 cutsceneChrBoardMove_validate(void);
void cutsceneChrBoardMove_exec(void);
s32 cutsceneChrBoardMove_isDone(void);
void cutsceneChrTurn2_init(void);
s32 cutsceneChrTurn2_validate(void);
void cutsceneChrTurn2_exec(void);
void cutsceneChrRotate_init(void);
s32 cutsceneChrRotate_validate(void);
void cutsceneChrRotate_exec(void);
s32 cutsceneChrRotate_isDone(void);
void cutsceneChrHop_init(void);
s32 cutsceneChrHop_validate(void);
void cutsceneChrHop_exec(cutsceneChrHop_exec_arg *a0, CutsceneManager *a1, s8 a2);
void cutsceneChrBack_init(void);
s32 cutsceneChrBack_validate(void);
void cutsceneChrBack_exec(void);
s32 cutsceneChrBack_isDone(void);
void cutsceneChrZoom2_init(void);
s32 cutsceneChrZoom2_validate(void);
void cutsceneChrZoom2_exec(void);
s32 cutsceneChrZoom2_isDone(void);
void cutsceneChrUpDown_init(void);
s32 cutsceneChrUpDown_validate(void);
void cutsceneChrUpDown_exec(void);
s32 cutsceneChrUpDown_isDone(void);
void cutsceneChrFace_init(void);
s32 cutsceneChrFace_validate(void);
void cutsceneChrFace_exec(s8 *arg0, CutsceneManager *arg1, s8 arg2);
void cutsceneChrAnime2_init(void);
s32 cutsceneChrAnime2_validate(void);
void cutsceneChrAnime2_exec(void);
s32 cutsceneChrAnime2_isDone(void);
void cutsceneChrMove3_init(void);
s32 cutsceneChrMove3_validate(void);
void cutsceneChrMove3_exec(cutsceneChrMove3_exec_arg *arg0, CutsceneManager *arg1, s8 arg2);
s32 cutsceneChrMove3_isDone(void);
void cutsceneChrModelDisp_init(void);
s32 cutsceneChrModelDisp_validate(void);
void cutsceneChrModelDisp_exec(cutsceneChrModelDisp_exec_arg *arg0, CutsceneManager *cutsceneManager, s8 index);
void cutsceneChrMoveSight_init(void);
s32 cutsceneChrMoveSight_validate(void);
void cutsceneChrMoveSight_exec(cutsceneChrMoveSight_exec_arg* arg0, CutsceneManager* arg1, s8 arg2);
s32 cutsceneChrMoveSight_isDone(void);
void cutsceneChrAlpha_init(void);
s32 cutsceneChrAlpha_validate(void);
void cutsceneChrAlpha_exec(void);
void cutsceneChrAlpha_update(void);
s32 cutsceneChrAlpha_isDone(void);
void cutsceneChrPosition2_init(void);
s32 cutsceneChrPosition2_validate(void);
void cutsceneChrPosition2_exec(void);
s32 cutsceneChrPosition2_isDone(void);
void cutsceneChrTurn3_init(void);
s32 cutsceneChrTurn3_validate(void);
void cutsceneChrTurn3_exec(cutsceneChrTurn3_exec_arg *arg0, CutsceneManager *arg1, s8 arg2);
s32 cutsceneChrTurn3_isDone(void);
void cutsceneChrKageDisp_init(void);
s32 cutsceneChrKageDisp_validate(void);
typedef struct {
    s16 unk0;
} cutsceneChrKageDisp_exec_arg;

void cutsceneChrKageDisp_exec(cutsceneChrKageDisp_exec_arg *arg0, CutsceneManager *arg1, s8 arg2);