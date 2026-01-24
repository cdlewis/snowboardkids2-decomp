#include "1DFAA0.h"
#include "common.h"

typedef struct {
    u8 reserved0;
    u8 reserved1;
    u8 reserved2;
    u8 targetAlpha;
    u16 duration;
} cutsceneChrAlpha_exec_arg;

typedef struct {
    s32 horizontalSpeed;
    s32 verticalVelocity;
    s32 gravity;
    s16 animationId;
    s16 animationLoopCount;
} cutsceneChrHop_exec_arg;

typedef struct {
    s16 targetAngle;
    s16 animationIndex;
    s16 transitionAnimIndex;
    s16 duration;
    s16 rotationStep;
    s16 rotationMultiplier;
} cutsceneChrTurn3_exec_arg;

typedef struct {
    s32 targetX;
    s32 targetY;
    s32 targetZ;
    s16 targetAngle;
    s16 duration;
    s16 animIndex;
    s8 actionMode;
    s8 loopCount;
    s16 transitionAnimIndex;
    s8 animQueued;
} cutsceneChrMove3_exec_arg;

typedef struct {
    s8 reserved0;
    s8 displayEnabled;
} cutsceneChrDisp_exec_arg;

typedef struct {
    s8 pad0;
    s8 partIndex;
    s16 enabled;
} cutsceneChrModelDisp_exec_arg;

typedef struct {
    s32 x;
    s32 y;
    s32 z;
    s16 rotY;
    s16 animIndex;
    s8 actionMode;
    s8 loopCount;
    s16 transitionAnimIndex;
    s8 animQueued;
} cutsceneChrPosition_exec_arg;

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
} cutsceneChrMove_exec_arg;

typedef struct {
    s32 targetX;
    s32 targetY;
    s32 targetZ;
    s16 fallbackRotY;
    s16 duration;
    s16 animIndex;
    s8 actionMode;
    s8 loopCount;
    s16 transitionAnimIndex;
    s8 decelRate;
    s8 animQueued;
} cutsceneChrMove2_exec_arg;

typedef struct {
    s32 targetX;
    s32 targetY;
    s32 targetZ;
    s16 unkC;
    s16 duration;
    s16 animIndex;
    s8 actionMode;
    s8 loopCount;
    s16 transitionAnimIndex;
    s32 verticalVelocity;
    s32 unk1C;
} cutsceneChrMoveSight_exec_arg;

typedef struct {
    s16 animIndex;
    s8 actionMode;
    s8 loopCount;
    s16 transitionAnimIndex;
} cutsceneChrAnime_exec_arg;

typedef struct {
    s16 animIndex;
    s8 actionMode;
    s8 loopCount;
    s16 transitionAnimIndex;
} cutsceneChrAnime2_exec_arg;

typedef struct {
    s32 scalePercent;
    s16 duration;
} cutsceneChrZoom_exec_arg;

typedef struct {
    s32 scalePercentX;
    s32 scalePercentY;
    s32 scalePercentZ;
    s16 duration;
} cutsceneChrZoom2_exec_arg;

typedef struct {
    s16 targetRotY;
    s16 animIndex;
    s16 transitionAnimIndex;
    s16 duration;
    s16 direction;
} cutsceneChrTurn2_exec_arg;

typedef struct {
    s16 rotationSpeed;
    s16 animationId;
    s16 animationLoopCount;
    s16 duration;
    s16 orbitSpeed;
} cutsceneChrRotate_exec_arg;

typedef struct {
    s32 targetX;
    s32 targetY;
    s32 targetZ;
    s16 fallbackRotY;
    s16 duration;
    s16 animationId;
    s8 actionMode;
    s8 animationLoopCount;
    s16 transitionAnimId;
} cutsceneChrMoveToFacing_exec_arg;

typedef struct {
    s32 targetX;
    s32 targetY;
    s32 targetZ;
    s16 facingAngle;
    s16 duration;
    s16 animIndex;
    s8 actionMode;
    s8 loopCount;
    s16 transitionAnimIndex;
    s8 animQueued;
} cutsceneChrUpDown_exec_arg;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s16 unkC;
    s16 unkE;
    s8 unk10;
    s8 unk11;
    s16 unk12;
    s8 unk14;
    s8 pad15;
    s16 unk16;
    s16 unk18;
} cutsceneChrPosition2_exec_arg;

void cutsceneChrPosition_init(void);
s32 cutsceneChrPosition_validate(void);
void cutsceneChrPosition_exec(cutsceneChrPosition_exec_arg *arg0, CutsceneManager *arg1, s8 arg2);
s32 cutsceneChrPosition_isDone(void);
void cutsceneChrMove_init(void);
s32 cutsceneChrMove_validate(void);
void cutsceneChrMove_exec(cutsceneChrMove_exec_arg *arg0, CutsceneManager *arg1, s8 arg2);
s32 cutsceneChrMove_isDone(void);
void cutsceneChrAnime_init(void);
s32 cutsceneChrAnime_validate(void);
void cutsceneChrAnime_exec(cutsceneChrAnime_exec_arg *arg, CutsceneManager *manager, s8 slotIndex);
s32 cutsceneChrAnime_isDone(void);
void cutsceneChrDisp_init(void);
s32 cutsceneChrDisp_validate(void);
void cutsceneChrDisp_exec(cutsceneChrDisp_exec_arg *arg, CutsceneManager *cutsceneManager, s8 slotIndex);
void cutsceneChrItem_init(void);
s32 cutsceneChrItem_validate(void);
void cutsceneChrItem_exec(cutsceneChrDisp_exec_arg *arg, CutsceneManager *cutsceneManager, s8 slotIndex);

typedef struct {
    s16 targetAngle;
    s16 animIndex;
    s16 transitionAnimIndex;
} cutsceneChrTurn_exec_arg;

void cutsceneChrTurn_init(void);
s32 cutsceneChrTurn_validate(void);
void cutsceneChrTurn_exec(cutsceneChrTurn_exec_arg *arg0, CutsceneManager *arg1, s8 arg2);
void cutsceneChrMove2_init(void);
s32 cutsceneChrMove2_validate(void);
void cutsceneChrMove2_exec(cutsceneChrMove2_exec_arg *arg, CutsceneManager *cutsceneManager, s8 slotIndex);
s32 cutsceneChrMove2_isDone(void);
void cutsceneChrZoom_init(void);
s32 cutsceneChrZoom_validate(void);
void cutsceneChrZoom_exec(cutsceneChrZoom_exec_arg *arg0, CutsceneManager *arg1, s8 arg2);
s32 cutsceneChrZoom_isDone(void);
void cutsceneChrBoardMove_init(void);
s32 cutsceneChrBoardMove_validate(void);
void cutsceneChrBoardMove_exec(cutsceneChrMove2_exec_arg *arg, CutsceneManager *cutsceneManager, s8 slotIndex);
s32 cutsceneChrBoardMove_isDone(void);
void cutsceneChrTurn2_init(void);
s32 cutsceneChrTurn2_validate(void);
void cutsceneChrTurn2_exec(cutsceneChrTurn2_exec_arg *arg0, CutsceneManager *arg1, s8 arg2);
void cutsceneChrRotate_init(void);
s32 cutsceneChrRotate_validate(void);
void cutsceneChrRotate_exec(cutsceneChrRotate_exec_arg *arg, CutsceneManager *manager, s8 slotIndex);
s32 cutsceneChrRotate_isDone(void);
void cutsceneChrHop_init(void);
s32 cutsceneChrHop_validate(void);
void cutsceneChrHop_exec(cutsceneChrHop_exec_arg *arg, CutsceneManager *manager, s8 slotIndex);
void cutsceneChrMoveToFacing_init(void);
s32 cutsceneChrMoveToFacing_validate(void);
void cutsceneChrMoveToFacing_exec(cutsceneChrMoveToFacing_exec_arg *arg, CutsceneManager *manager, s8 slotIndex);
s32 cutsceneChrMoveToFacing_isDone(void);
void cutsceneChrZoom2_init(void);
s32 cutsceneChrZoom2_validate(void);
void cutsceneChrZoom2_exec(cutsceneChrZoom2_exec_arg *arg0, CutsceneManager *arg1, s8 arg2);
s32 cutsceneChrZoom2_isDone(void);
void cutsceneChrUpDown_init(void);
s32 cutsceneChrUpDown_validate(void);
void cutsceneChrUpDown_exec(cutsceneChrUpDown_exec_arg *arg0, CutsceneManager *arg1, s8 arg2);
s32 cutsceneChrUpDown_isDone(void);
void cutsceneChrFace_init(void);
s32 cutsceneChrFace_validate(void);
void cutsceneChrFace_exec(s8 *animIndex, CutsceneManager *manager, s8 slotIndex);
void cutsceneChrAnime2_init(void);
s32 cutsceneChrAnime2_validate(void);
void cutsceneChrAnime2_exec(cutsceneChrAnime2_exec_arg *arg0, CutsceneManager *arg1, s8 arg2);
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
void cutsceneChrMoveSight_exec(cutsceneChrMoveSight_exec_arg *arg0, CutsceneManager *cutsceneManager, s8 slotIndex);
s32 cutsceneChrMoveSight_isDone(void);
void cutsceneChrAlpha_init(void);
s32 cutsceneChrAlpha_validate(void);
void cutsceneChrAlpha_exec(cutsceneChrAlpha_exec_arg *arg0, CutsceneManager *arg1, s8 arg2);
void cutsceneChrAlpha_update(CutsceneManager *arg0, s8 arg1);
s32 cutsceneChrAlpha_isDone(void);
void cutsceneChrPosition2_init(void);
s32 cutsceneChrPosition2_validate(void);
void cutsceneChrPosition2_exec(cutsceneChrPosition2_exec_arg *arg0, CutsceneManager *arg1, s8 arg2);
s32 cutsceneChrPosition2_isDone(void);
void cutsceneChrTurn3_init(void);
s32 cutsceneChrTurn3_validate(void);
void cutsceneChrTurn3_exec(cutsceneChrTurn3_exec_arg *arg0, CutsceneManager *arg1, s8 arg2);
s32 cutsceneChrTurn3_isDone(void);
void cutsceneChrKageDisp_init(void);
s32 cutsceneChrKageDisp_validate(void);
typedef struct {
    s16 hideShadow;
} cutsceneChrKageDisp_exec_arg;

void cutsceneChrKageDisp_exec(cutsceneChrKageDisp_exec_arg *arg0, CutsceneManager *arg1, s8 arg2);