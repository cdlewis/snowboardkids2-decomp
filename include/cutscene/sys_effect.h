#include "1DFAA0.h"
#include "common.h"

typedef struct {
    u16 unk0;
    u16 unk2;
} cutsceneEffectTrick_exec_arg;

typedef struct {
    s16 unk0;
} cutsceneEffectFan_exec_arg;

typedef struct {
    s16 unk0;
} cutsceneEffectRocket_exec_arg;

typedef struct {
    u8 padding[0xC];
    s16 unkC;
    s16 unkE;
    s16 unk10;
    u8 unk12;
    s8 unk13;
    s16 unk14;
    s16 unk16;
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
void cutsceneEffectMLight_exec(void);
void cutsceneEffectTrick_init(void);
s32 cutsceneEffectTrick_validate(void);
void cutsceneEffectTrick_exec(cutsceneEffectTrick_exec_arg *arg0, CutsceneManager *arg1, s8 arg2);
void cutsceneEffectTrick_update(CutsceneManager *arg0, s8 arg1);
s32 cutsceneEffectTrick_isDone(void);
void cutsceneInitTrack_init(void);
s32 cutsceneInitTrack_validate(void);
void cutsceneInitTrack_exec(void);