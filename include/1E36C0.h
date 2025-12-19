#pragma once

#include "common.h"
#include "cutscene/cutscene_sys2.h"
#include "1DFAA0.h"

typedef struct {
    u8 pad[0x54];
    s32 unk54;
    s32 unk58;
    s32 unk5C;
    s32 unk60;
    s32 unk64;
    s32 unk68;
} unk_func_800B68F4_1E39A4;

s16 func_800B6610_1E36C0(cutsceneSys2Wait_exec_asset *arg0);
void func_800B66B4_1E3764(CutsceneSlotData *arg0);
s32 syncModelFromSlot(void *, void *);
void func_800B68F4_1E39A4(unk_func_800B68F4_1E39A4 *arg0, s32 arg1, s32 arg2, s32 arg3);
void func_800B6AB8_1E3B68(CutsceneSlotData *arg0, s16 arg1);
void func_800B6B6C_1E3C1C(CutsceneSlotData *arg0);
void func_800B6BDC_1E3C8C(CutsceneSlotData *arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4);
void func_800B6C04_1E3CB4(CutsceneSlotData *arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5, s16 arg6);
s32 func_800B6C8C_1E3D3C(CutsceneSlotData *arg0, SceneModel *arg1, s32 arg2, s32 arg3, s32 arg4, s16 arg5, s16 arg6);
s32 func_800B6CD8_1E3D88(CutsceneSlotData *arg0, SceneModel *arg1, s32 arg2, s32 arg3, s32 arg4, s16 arg5, s16 arg6, s32 arg7, s32 arg8);
s32 func_800B75C4_1E4674(CutsceneSlotData *arg0, SceneModel *arg1, s16 arg2, s32 arg3, s32 arg4, s32 arg5);
void func_800B7760_1E4810(CutsceneSlotData *arg0, s32 arg1, s16 arg2);
void func_800B77C4_1E4874(CutsceneSlotData *arg0, s32 arg1, s16 arg2);
void func_800B7828_1E48D8(CutsceneSlotData *arg0, s32 arg1, s16 arg2);
void func_800B788C_1E493C(CutsceneSlotData *arg0, s32 arg1, s32 arg2, s32 arg3);
void func_800B7A60_1E4B10(CutsceneSlotData *arg0, SceneModel *unused, s32 targetX, s32 targetY, s32 targetZ, s16 duration);
void func_800B7B70_1E4C20(CutsceneSlotData *arg0, s32 *arg1, s16 arg2, s32 arg3, s32 arg4);
s32 func_800B83B8_1E5468(CutsceneSlotData *arg0, StateEntry *arg1);
void func_800B8C3C_1E5CEC(CutsceneSlotData *arg0);
s32 func_800B8D34_1E5DE4(CutsceneSlotData *arg0);
s32 func_800B8DA8_1E5E58(CutsceneSlotData *arg0);
