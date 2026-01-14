#pragma once

#include "1DFAA0.h"
#include "common.h"

typedef struct {
    u16 unk0;
    u16 unk2;
} cutsceneEffectTrick_exec_arg;

typedef struct {
    s16 displayDuration;
} cutsceneEffectFan_exec_arg;

typedef struct {
    s16 displayDuration;
} cutsceneEffectRocket_exec_arg;

typedef struct {
    s16 colorIndex;     // color/palette index for the light effect
    s16 duration;       // display duration in frames
    s32 positionVec;    // position vector (cast to s32[3] for x,y,z access)
    s32 unk8;           // unused parameter
    s32 unkC;           // unused parameter
    s16 effectMode;     // 0 = direct positioning, 1 = with rotation
} cutsceneEffectMLight_exec_arg;

typedef struct {
    s32 animDuration1;
    s32 animDuration2;
    s32 animDuration3;
    s16 posX;
    s16 posY;
    s16 scale;
    u8 pad12;
    s8 effectType;
    s16 param1;
    s16 param2;
} cutsceneEffectDisp_exec_arg;

void cutsceneEffectDisp_init(void);
s32 cutsceneEffectDisp_validate(void);
void cutsceneEffectDisp_exec(cutsceneEffectDisp_exec_arg *arg0, CutsceneManager *arg1, s8 arg2);
s32 cutsceneEffectDisp_isDone(void);
void cutsceneEffectFan_init(void);
s32 cutsceneEffectFan_validate(void);
void cutsceneEffectFan_exec(cutsceneEffectFan_exec_arg *arg0, CutsceneSlot *arg1, s8 arg2);
s32 cutsceneEffectFan_isDone(void);
void cutsceneEffectRocket_init(void);
s32 cutsceneEffectRocket_validate(void);
void cutsceneEffectRocket_exec(cutsceneEffectRocket_exec_arg *arg0, CutsceneSlot *arg1, s8 arg2);
s32 cutsceneEffectRocket_isDone(void);
void cutsceneEffectMLight_init(void);
s32 cutsceneEffectMLight_validate(void);
void cutsceneEffectMLight_exec(cutsceneEffectMLight_exec_arg *arg0, CutsceneManager *arg1, s8 arg2);
s32 cutsceneEffectMLight_isDone(void);
void cutsceneEffectTrick_init(void);
s32 cutsceneEffectTrick_validate(void);
void cutsceneEffectTrick_exec(cutsceneEffectTrick_exec_arg *arg0, CutsceneManager *arg1, s8 arg2);
void cutsceneEffectTrick_update(CutsceneManager *arg0, s8 arg1);
s32 cutsceneEffectTrick_isDone(void);
void cutsceneInitTrack_init(void);
s32 cutsceneInitTrack_validate(void);
void cutsceneInitTrack_exec(void);