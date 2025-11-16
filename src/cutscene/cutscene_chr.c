#include "cutscene/cutscene_chr.h"
#include "1DFAA0.h"
#include "20F0.h"

void func_800021E8_2DE8(SceneModel *, s16, s16, s32);
void func_800B788C_1E493C(CutsceneSlotData *, s32, s32, s32);

void cutsceneChrPosition_init(void) {
}

s32 cutsceneChrPosition_validate(void) {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/cutscene/cutscene_chr", cutsceneChrPosition_exec);

s32 cutsceneChrPosition_isDone(void) {
    return 0;
}

void cutsceneChrMove_init(void) {
}

s32 cutsceneChrMove_validate(void) {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/cutscene/cutscene_chr", cutsceneChrMove_exec);

s32 cutsceneChrMove_isDone(void) {
    return 0;
}

void cutsceneChrAnime_init(void) {
}

s32 cutsceneChrAnime_validate(void) {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/cutscene/cutscene_chr", cutsceneChrAnime_exec);

s32 cutsceneChrAnime_isDone(void) {
    return 0;
}

void cutsceneChrDisp_init(void) {
}

s32 cutsceneChrDisp_validate(void) {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/cutscene/cutscene_chr", cutsceneChrDisp_exec);

void cutsceneChrItem_init(void) {
}

s32 cutsceneChrItem_validate(void) {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/cutscene/cutscene_chr", cutsceneChrItem_exec);

void cutsceneChrTurn_init(void) {
}

s32 cutsceneChrTurn_validate(void) {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/cutscene/cutscene_chr", cutsceneChrTurn_exec);

void cutsceneChrMove2_init(void) {
}

s32 cutsceneChrMove2_validate(void) {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/cutscene/cutscene_chr", cutsceneChrMove2_exec);

s32 cutsceneChrMove2_isDone(void) {
    return 0;
}

void cutsceneChrZoom_init(void) {
}

s32 cutsceneChrZoom_validate(void) {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/cutscene/cutscene_chr", cutsceneChrZoom_exec);

s32 cutsceneChrZoom_isDone(void) {
    return 0;
}

void cutsceneChrBoardMove_init(void) {
}

s32 cutsceneChrBoardMove_validate(void) {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/cutscene/cutscene_chr", cutsceneChrBoardMove_exec);

s32 cutsceneChrBoardMove_isDone(void) {
    return 0;
}

void cutsceneChrTurn2_init(void) {
}

s32 cutsceneChrTurn2_validate(void) {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/cutscene/cutscene_chr", cutsceneChrTurn2_exec);

void cutsceneChrRotate_init(void) {
}

s32 cutsceneChrRotate_validate(void) {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/cutscene/cutscene_chr", cutsceneChrRotate_exec);

s32 cutsceneChrRotate_isDone(void) {
    return 0;
}

void cutsceneChrHop_init(void) {
}

s32 cutsceneChrHop_validate(void) {
    return 0;
}

void cutsceneChrHop_exec(cutsceneChrHop_exec_arg *a0, CutsceneManager *a1, s8 a2) {
    CutsceneSlot *s1 = &a1->slots[a2];

    func_800B788C_1E493C(&s1->slotData, a0->unk0, a0->unk4, a0->unk8);
    func_800021E8_2DE8(s1->model, a0->unkC, a0->unkE, 1);
}

void cutsceneChrBack_init(void) {
}

s32 cutsceneChrBack_validate(void) {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/cutscene/cutscene_chr", cutsceneChrBack_exec);

s32 cutsceneChrBack_isDone(void) {
    return 0;
}

void cutsceneChrZoom2_init(void) {
}

s32 cutsceneChrZoom2_validate(void) {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/cutscene/cutscene_chr", cutsceneChrZoom2_exec);

s32 cutsceneChrZoom2_isDone(void) {
    return 0;
}

void cutsceneChrUpDown_init(void) {
}

s32 cutsceneChrUpDown_validate(void) {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/cutscene/cutscene_chr", cutsceneChrUpDown_exec);

s32 cutsceneChrUpDown_isDone(void) {
    return 0;
}

void cutsceneChrFace_init(void) {
}

s32 cutsceneChrFace_validate(void) {
    return 0;
}

void cutsceneChrFace_exec(s8 *arg0, CutsceneManager *arg1, s8 arg2) {
    CutsceneSlot *slot = &arg1->slots[arg2];
    do {
        func_80001688_2288(slot->model, *arg0);
    } while (0);
}

void cutsceneChrAnime2_init(void) {
}

s32 cutsceneChrAnime2_validate(void) {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/cutscene/cutscene_chr", cutsceneChrAnime2_exec);

s32 cutsceneChrAnime2_isDone(void) {
    return 0;
}

void cutsceneChrMove3_init(void) {
}

s32 cutsceneChrMove3_validate(void) {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/cutscene/cutscene_chr", cutsceneChrMove3_exec);

s32 cutsceneChrMove3_isDone(void) {
    return 0;
}

void cutsceneChrModelDisp_init(void) {
}

s32 cutsceneChrModelDisp_validate(void) {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/cutscene/cutscene_chr", cutsceneChrModelDisp_exec);

void cutsceneChrMoveSight_init(void) {
}

s32 cutsceneChrMoveSight_validate(void) {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/cutscene/cutscene_chr", cutsceneChrMoveSight_exec);

s32 cutsceneChrMoveSight_isDone(void) {
    return 0;
}

void cutsceneChrAlpha_init(void) {
}

s32 cutsceneChrAlpha_validate(void) {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/cutscene/cutscene_chr", cutsceneChrAlpha_exec);

INCLUDE_ASM("asm/nonmatchings/cutscene/cutscene_chr", cutsceneChrAlpha_update);

s32 cutsceneChrAlpha_isDone(void) {
    return 0;
}

void cutsceneChrPosition2_init(void) {
}

s32 cutsceneChrPosition2_validate(void) {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/cutscene/cutscene_chr", cutsceneChrPosition2_exec);

s32 cutsceneChrPosition2_isDone(void) {
    return 0;
}

void cutsceneChrTurn3_init(void) {
}

s32 cutsceneChrTurn3_validate(void) {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/cutscene/cutscene_chr", cutsceneChrTurn3_exec);

s32 cutsceneChrTurn3_isDone(void) {
    return 0;
}

void cutsceneChrKageDisp_init(void) {
}

s32 cutsceneChrKageDisp_validate(void) {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/cutscene/cutscene_chr", cutsceneChrKageDisp_exec);
