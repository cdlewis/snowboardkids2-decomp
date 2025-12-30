#include "10AD0.h"
#include "38C90.h"
#include "3E160.h"
#include "6E840.h"
#include "common.h"
#include "overlay.h"
#include "task_scheduler.h"

USE_ASSET(_67AB10);
USE_ASSET(_67BEB0);
USE_ASSET(_67DB80);

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 pad6;
} E090_DataEntry;

extern E090_DataEntry D_8008CE10_8DA10[];

void transitionFromLogoScreen(void);

typedef struct {
    s16 unk0;
    s16 unk2;
    s32 unk4;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s8 unk12;
    s8 unk13;
    s8 unk14;
    u8 pad15[3];
} E090_InnerStruct;

typedef struct {
    u8 pad0[0x10];
    s16 unk10;
    s16 unk12;
    u8 pad14[0x18];
} E090_SubStruct;

typedef struct {
    s8 state;
    s8 visibleLetterCount;
    s16 frameTimer;
    u8 pad4[0x4];
    Node_70B00 unk8;
    void *unk1E0;
    void *unk1E4;
    void *unk1E8;
    E090_SubStruct unk1EC;
    E090_SubStruct unk218;
    E090_InnerStruct unk244[6];
    s32 unk2D4[6];
} E090_struct;

void initLogoScreenElements(E090_struct *arg0);
void initLogoDisplaySequence(E090_struct *arg0);
void updateLogoDisplayFade(E090_struct *arg0);
void initTitleTextSequence(E090_struct *arg0);
void updateTitleTextDelay(E090_struct *arg0);
void updateTitleLetterReveal(E090_struct *arg0);
void updateTitleFinalFadeIn(E090_struct *arg0);
void cleanupLogoScreen(E090_struct *arg0);

void initLogoNodeFadeIn(E090_struct *arg0) {
    func_8006FE28_70A28(&arg0->unk8, 0, 0, 0);
    func_8006FDA0_709A0(&arg0->unk8, 0xFF, 0xF);
}

void initLogoNodeFadeOut(E090_struct *arg0) {
    func_8006FE28_70A28(&arg0->unk8, 0, 0, 0);
    func_8006FDA0_709A0(&arg0->unk8, 0, 0xF);
}

void initLogoScreenElements(E090_struct *arg0) {
    s32 i;

    func_800394BC_3A0BC(&arg0->unk1EC, (s32)arg0->unk1E0);
    func_800394BC_3A0BC(&arg0->unk218, (s32)arg0->unk1E4);

    arg0->unk1EC.unk10 = 0;
    arg0->unk1EC.unk12 = 0;
    arg0->unk218.unk10 = 0;
    arg0->unk218.unk12 = 0;

    for (i = 0; i < 6; i++) {
        arg0->unk244[i].unk0 = D_8008CE10_8DA10[i].unk0;
        arg0->unk244[i].unk2 = D_8008CE10_8DA10[i].unk2;
        arg0->unk244[i].unk4 = (s32)arg0->unk1E8;
        arg0->unk244[i].unk8 = D_8008CE10_8DA10[i].unk4;
        arg0->unk244[i].unkA = 0x400;
        arg0->unk244[i].unkC = 0x400;
        arg0->unk244[i].unkE = 0;
        arg0->unk244[i].unk10 = 0xFF;
        arg0->unk244[i].unk12 = 0;
        arg0->unk244[i].unk13 = 0;
        arg0->unk244[i].unk14 = 0;
        arg0->unk2D4[i] = 0;
    }

    arg0->state = 1;
}

void initLogoDisplaySequence(E090_struct *arg0) {
    initLogoNodeFadeOut(arg0);
    arg0->frameTimer = 100;
    arg0->state = 2;
}

void updateLogoDisplayFade(E090_struct *arg0) {
    arg0->frameTimer--;
    if (arg0->frameTimer == 15) {
        initLogoNodeFadeIn(arg0);
    } else if (arg0->frameTimer == 0) {
        initLogoNodeFadeOut(arg0);
        arg0->state = 3;
    }
}

void initTitleTextSequence(E090_struct *arg0) {
    initLogoNodeFadeOut(arg0);
    arg0->frameTimer = 100;
    arg0->state = 4;
}

void updateTitleTextDelay(E090_struct *arg0) {
    arg0->frameTimer--;
    if (arg0->frameTimer == 0x50) {
        arg0->state = 5;
        arg0->visibleLetterCount++;
    }
}

void updateTitleLetterReveal(E090_struct *arg0) {
    s16 temp = arg0->frameTimer - 0x29;
    arg0->frameTimer--;
    switch (temp) {
        case 11:
        case 17:
        case 23:
        case 29:
        case 35:
            arg0->visibleLetterCount++;
            break;
        case 0:
            arg0->state = 6;
            break;
    }
}

void updateTitleFinalFadeIn(E090_struct *arg0) {
    arg0->frameTimer--;
    if (arg0->frameTimer == 15) {
        initLogoNodeFadeIn(arg0);
    } else if (arg0->frameTimer == 0) {
        arg0->state = 7;
    }
}

void cleanupLogoScreen(E090_struct *arg0) {
    arg0->unk1E0 = freeNodeMemory(arg0->unk1E0);
    arg0->unk1E4 = freeNodeMemory(arg0->unk1E4);
    arg0->unk1E8 = freeNodeMemory(arg0->unk1E8);
    unlinkNode(&arg0->unk8);
    terminateSchedulerWithCallback(transitionFromLogoScreen);
}

void loadLogoScreenAssets(void);
void updateLogoScreen(void);

void initLogoScreen(void) {
    setGameStateHandler(loadLogoScreenAssets);
}

void loadLogoScreenAssets(void) {
    E090_struct *logoScreen;
    Node_70B00 *node;
    u8 nodeParams[0x20];

    logoScreen = (E090_struct *)allocateTaskMemory(0x2F0);
    logoScreen->state = 0;
    logoScreen->frameTimer = 0;
    logoScreen->visibleLetterCount = 0;
    logoScreen->unk1E0 = loadCompressedData(&_67AB10_ROM_START, &_67AB10_ROM_END, 0x6350);
    logoScreen->unk1E4 = loadCompressedData(&_67BEB0_ROM_START, &_67BEB0_ROM_END, 0x4320);
    logoScreen->unk1E8 = loadCompressedData(&_67DB80_ROM_START, &_67DB80_ROM_END, 0x1A68);

    node = &logoScreen->unk8;
    func_8006FAA4_706A4(node, 0, 0, 10, 0);
    func_8006F9BC_705BC(node, 1.0f, 1.0f);
    setModelCameraTransform(node, 0, 0, -0xA0, -0x78, 0x9F, 0x77);
    func_8006FEF8_70AF8(node, 1);
    func_8006BEDC_6CADC(&nodeParams, 0, 0, 0x01000000, 0, 0, 0);
    func_8006FD3C_7093C(logoScreen->unk8.id, &nodeParams);
    func_8006FE28_70A28(node, 0, 0, 0);
    func_8006FDA0_709A0(node, 0xFF, 0);
    setGameStateHandler(updateLogoScreen);
}

void updateLogoScreen(void) {
    E090_struct *logoScreen = getCurrentAllocation();
    s8 state;
    s32 i;

    state = logoScreen->state;
    switch (state) {
        case 0:
            initLogoScreenElements(logoScreen);
            break;
        case 1:
            initLogoDisplaySequence(logoScreen);
            break;
        case 2:
            updateLogoDisplayFade(logoScreen);
            break;
        case 3:
            initTitleTextSequence(logoScreen);
            break;
        case 4:
            updateTitleTextDelay(logoScreen);
            break;
        case 5:
            updateTitleLetterReveal(logoScreen);
            break;
        case 6:
            updateTitleFinalFadeIn(logoScreen);
            break;
        case 7:
        default:
            cleanupLogoScreen(logoScreen);
            return;
    }

    state = logoScreen->state;
    if (state == 2) {
        debugEnqueueCallback(0, 4, func_80038420_39020, &logoScreen->unk1EC);
    } else if (state >= 2) {
        if (state < 7) {
            if (state >= 4) {
                debugEnqueueCallback(0, 4, func_80038420_39020, &logoScreen->unk218);

                for (i = 0; i < logoScreen->visibleLetterCount; i++) {
                    logoScreen->unk2D4[i] += 0x330000;
                    if (logoScreen->unk2D4[i] > 0xFFFFFF) {
                        logoScreen->unk2D4[i] = 0xFF0000;
                    }
                    logoScreen->unk244[i].unk14 = (s8)(logoScreen->unk2D4[i] >> 16);
                    debugEnqueueCallback(0, 3, func_80011924_12524, &logoScreen->unk244[i]);
                }
            }
        }
    }
}

void transitionFromLogoScreen(void) {
    func_800697F4_6A3F4(1);
}
