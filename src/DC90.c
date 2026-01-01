#include "DC90.h"
#include "36B80.h"
#include "3E160.h"
#include "56910.h"
#include "B040.h"
#include "common.h"
#include "graphics.h"
#include "task_scheduler.h"

extern s32 gControllerInputs;

extern void func_8006D7B0_6E3B0(s32, s16, s16, s16, s16, u8, u8, u8, u8, u8);
extern void *handleMenuSelection(DC90TaskStruct *arg0);

void updateCutsceneWaitMenu(DC90TaskStruct *arg0);
void cleanupCutsceneWaitMenu(DC90TaskStruct *arg0);

void initMenuFadeIn(DC90TaskStruct *arg0) {
    void *temp = arg0->baseSpriteData;

    arg0->unk20.asS16 = 0xFF;
    arg0->unk22.asS16 = 0xFF;
    arg0->renderPosX = 0;
    arg0->renderPosY = 0;
    arg0->unk24 = 0;
    arg0->state = 1;
    arg0->currentSprite = temp;
}

void animateMenuExpandHorizontal(DC90TaskStruct *arg0) {
    arg0->animFrameX++;
    if (arg0->animFrameX >= 13) {
        arg0->state = 2;
    }
    if (arg0->unk3C < 65) {
        arg0->unk3C = 64;
    } else {
        arg0->unk3C -= 16;
    }
}

void animateMenuExpandVertical(DC90TaskStruct *arg0) {
    s16 temp;
    s16 temp2;

    if (++arg0->animFrameY >= 6) {
        arg0->state = 3;
    }

    temp2 = temp = arg0->unk3C;
    arg0->unk3C = (temp < 0x41) ? 0x40 : temp2 - 0x10;
}

void *handleMenuSelection(DC90TaskStruct *arg0) {
    void *temp_v0 = getTable2DEntry((Table_B934 *)arg0->unk8, arg0->unkE, arg0->unk10);
    unsigned long new_var;

    new_var = temp_v0 == 0;
    if (new_var) {
        arg0->state = 6;
        arg0->unk18 = temp_v0;
    } else {
        arg0->unk18 = temp_v0;
    }
    if (gControllerInputs & A_BUTTON) {
        func_800585C8_591C8(45);
        arg0->state = 4;
    }
    return temp_v0;
}

void animateMenuContractVertical(DC90TaskStruct *arg0) {
    arg0->animFrameY--;
    if (arg0->animFrameY < 2) {
        arg0->state = 5;
    }
    if (arg0->unk3E > 0) {
        arg0->unk3E -= 16;
    } else {
        arg0->unk3E = 0;
    }
}

void animateMenuContractHorizontal(DC90TaskStruct *arg0) {
    arg0->animFrameX--;
    if (arg0->animFrameX < 2) {
        arg0->state = 6;
    }
    if (arg0->unk3E > 0) {
        arg0->unk3E -= 16;
    } else {
        arg0->unk3E = 0;
    }
}

void initCutsceneWaitMenu(DC90TaskStruct *arg0) {
    arg0->state = 0;
    arg0->unk10 = 0;
    arg0->unk4 = loadAsset_34F7E0();
    arg0->baseSpriteData = loadTextRenderAsset(1);
    arg0->unk8 = loadDmaAsset(2);
    arg0->animFrameX = 1;
    arg0->animFrameY = 1;
    arg0->unk3C = 0xFF;
    arg0->unk3E = 0xC0;
    func_800585C8_591C8(0x2C);
    setCleanupCallback(&cleanupCutsceneWaitMenu);
    setCallback(&updateCutsceneWaitMenu);
}

void updateCutsceneWaitMenu(DC90TaskStruct *arg0) {
    void *result = 0;
    s32 flag = 0;

    switch (arg0->state) {
        case 0:
            initMenuFadeIn(arg0);
            break;
        case 1:
            animateMenuExpandHorizontal(arg0);
            break;
        case 2:
            animateMenuExpandVertical(arg0);
            break;
        case 3:
            result = handleMenuSelection(arg0);
            flag = 1;
            break;
        case 4:
            animateMenuContractVertical(arg0);
            break;
        case 5:
            animateMenuContractHorizontal(arg0);
            break;
        case 6:
            arg0->unk0[0xFF7] = 0;
            func_80069CF8_6A8F8();
            return;
    }

    arg0->unk2C = -(arg0->animFrameX << 3);
    arg0->unk30 = -(arg0->animFrameY << 3);

    if (result != 0) {
        s16 temp16;
        arg0->renderPosX = arg0->unk2C;
        temp16 = arg0->unk30;
        arg0->renderPosY = temp16;
        func_80035260_35E60(
            arg0->currentSprite,
            arg0->unk18,
            arg0->renderPosX,
            temp16,
            arg0->unk20.asBytes.low,
            arg0->unk22.asBytes.low,
            arg0->unk24,
            1,
            0
        );
    }

    func_8006D7B0_6E3B0(
        (s32)arg0->unk4,
        (s16)(arg0->unk2C),
        (s16)(arg0->unk30),
        (s16)(arg0->animFrameX),
        (s16)(arg0->animFrameY),
        flag,
        (u8)arg0->unk3C,
        (u8)arg0->unk3E,
        1,
        0
    );
}

void cleanupCutsceneWaitMenu(DC90TaskStruct *arg0) {
    arg0->baseSpriteData = freeNodeMemory(arg0->baseSpriteData);
    arg0->unk4 = freeNodeMemory(arg0->unk4);
    arg0->unk8 = freeNodeMemory(arg0->unk8);
}
