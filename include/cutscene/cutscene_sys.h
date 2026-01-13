#pragma once

#include "1DFAA0.h"
#include "6E840.h"
#include "common.h"

typedef struct {
    u8 padding[0xFF5];
    /* 0xFF5 */ u8 enableTransparency;
} cutsceneSysDisp_exec_arg;

typedef struct {
    s8 unused;
    s8 fadeMode;
    s8 brightnessIndex;
} cutsceneSysFadeIn_exec_arg;

typedef struct {
    s8 unused;
    s8 fadeMode;
    u8 brightnessIndex;
} cutsceneSysFadeOut_exec_arg;

void cutsceneSysDisp_init(void);
s32 cutsceneSysDisp_validate(void);
void cutsceneSysDisp_exec(u8 *enableTransparencyValue, cutsceneSysDisp_exec_arg *displayArgs);
void cutsceneSysFadeIn_init(void);
s32 cutsceneSysFadeIn_validate(void);
void cutsceneSysFadeIn_exec(cutsceneSysFadeIn_exec_arg *arg0, Node_70B00 *arg1);
s32 cutsceneSysFadeIn_isDone(void);
void cutsceneSysFadeOut_init(void);
s32 cutsceneSysFadeOut_validate(void);
void cutsceneSysFadeOut_exec(cutsceneSysFadeOut_exec_arg *fadeArgs, Node_70B00 *node);
s32 cutsceneSysFadeOut_isDone(void);
void cutsceneSysFlash_init(void);
s32 cutsceneSysFlash_validate(void);
void cutsceneSysFlash_setup(func_800B2A24_1DFAD4_arg_item *srcItem, CutsceneManager *cutsceneManager, s8 slotIndex);
void cutsceneSysFlash_update(CutsceneManager *cutsceneManager, s8 slotIndex);
s32 cutsceneSysFlash_isDone(void);
void cutsceneSysIntMode_init(void);
s32 cutsceneSysIntMode_validate(void);
typedef struct {
    s8 vStartExtension;
} cutsceneSysIntMode_exec_arg;
void cutsceneSysIntMode_exec(cutsceneSysIntMode_exec_arg *arg0);
void cutsceneSysWipeColor_init(void);
s32 cutsceneSysWipeColor_validate(void);
typedef struct {
    u8 r;
    u8 g;
    u8 b;
} cutsceneSysWipeColor_exec_arg0;

typedef struct {
    u8 padding[0xFF8];
    s32 wipeColorNode;
} cutsceneSysWipeColor_exec_arg1;
void cutsceneSysWipeColor_exec(cutsceneSysWipeColor_exec_arg0 *arg0, cutsceneSysWipeColor_exec_arg1 *arg1);
void cutsceneSysWait_init(void);
s32 cutsceneSysWait_validate(void);
void cutsceneSysWait_exec(u16 *waitFrames, CutsceneManager *cutsceneManager);
void cutsceneSysCurtain_init(void);
s32 cutsceneSysCurtain_validate(void);
typedef struct {
    s32 targetPercent;
    u16 duration;
} CurtainParams;
void cutsceneSysCurtain_exec(CurtainParams *params, CutsceneManager *cutsceneManager, s8 idx);
void cutsceneSysCurtain_update(CutsceneManager *cutsceneManager, s8 slotIndex);
s32 cutsceneSysCurtain_isDone(void);
void cutsceneSysFilter_init(void);
s32 cutsceneSysFilter_validate(void);
typedef struct {
    s8 unused0;
    s8 cameraFilterEnable;
    s8 unused2;
    s8 cameraFilterParam;
    s8 envColorRed;
    s8 envColorGreen;
    s8 envColorBlue;
} cutsceneSysFilter_exec_arg;
void cutsceneSysFilter_exec(cutsceneSysFilter_exec_arg *arg0, Node_70B00 *arg1);
s32 cutsceneSysFilter_isDone(void);