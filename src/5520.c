#include "5520.h"
#include "6E840.h"
#include "common.h"
#include "displaylist.h"
#include "task_scheduler.h"

extern u8 identityMatrix[];
extern void *D_800885D0_891D0;
extern void *D_800885E0_891E0;
extern void *D_800885F0_891F0;

typedef struct {
    /* 0x00 */ void *model;
    /* 0x04 */ u8 opaqueMatrix[0x20];
    /* 0x24 */ void *opaqueSettings;
    /* 0x28 */ void *opaqueDisplayList;
    /* 0x2C */ void *opaqueVertexData;
    /* 0x30 */ s32 opaqueFlag;
    /* 0x34 */ u8 pad34[0xC];
    /* 0x40 */ u8 transparentMatrix[0x20];
    /* 0x60 */ void *transparentSettings;
    /* 0x64 */ void *transparentDisplayList;
    /* 0x68 */ void *transparentVertexData;
    /* 0x6C */ s32 transparentFlag;
    /* 0x70 */ u8 pad70[0xC];
    /* 0x7C */ s32 scale;
    /* 0x80 */ s16 rotationY;
    /* 0x82 */ s8 transparentEnabled;
    /* 0x83 */ s8 overlayAnimState;
    /* 0x84 */ s32 overlayScaleX;
    /* 0x88 */ s32 overlayScaleY;
    /* 0x8C */ s32 oscillationOffset;
    /* 0x90 */ s16 oscillationAngle;
    /* 0x92 */ s8 oscillationEnabled;
    /* 0x93 */ s8 scaleAnimState;
    /* 0x94 */ u8 overlayMatrix[0x20];
    /* 0xB4 */ void *overlaySettings;
    /* 0xB8 */ void *overlayDisplayList;
    /* 0xBC */ void *overlayVertexData;
    /* 0xC0 */ s32 overlayFlag;
    /* 0xC4 */ u8 padC4[0xC];
    /* 0xD0 */ void *soundData;
    /* 0xD4 */ u8 padD4[0x4];
    /* 0xD8 */ s16 scrollU;
    /* 0xDA */ s16 scrollV;
    /* 0xDC */ s16 scrollSpeedU;
    /* 0xDE */ s16 scrollSpeedV;
    /* 0xE0 */ s16 unkE0;
    /* 0xE2 */ s16 unkE2;
    /* 0xE4 */ s16 unkE4;
} RotatingLogoState;

void renderOpaqueTiledTexture(DisplayListObject *arg0);
void renderTransparentTiledTexture(s32 arg0);
void renderOverlayTiledTexture(s32 arg0);
void func_80004920_5520(void);
void func_80005094_5C94(void);
void cleanupRotatingLogo(RotatingLogoState *);
void func_8000595C_655C(void);
void *loadAssetGroupDisplayList(void *);
void *loadAssetGroupVertexData(void *);
void *loadAssetGroupSoundData(void *);

INCLUDE_ASM("asm/nonmatchings/5520", func_80004920_5520);

void renderOpaqueScrollingTexture(DisplayListObject *arg0) {
    func_80004920_5520();
    func_800634E8_640E8(arg0);
}

void renderTransparentScrollingTexture(DisplayListObject *arg0) {
    func_80004920_5520();
    func_80063534_64134(arg0);
}

void renderOverlayScrollingTexture(DisplayListObject *arg0) {
    func_80004920_5520();
    func_80063580_64180(arg0);
}

void enqueueScrollingTextureRender(u16 renderLayer, DisplayListObject *displayListObj) {
    displayListObj->unk30 = 0;

    if (displayListObj->unk20->opaqueDisplayList != NULL) {
        debugEnqueueCallback(renderLayer, 1, &renderOpaqueScrollingTexture, displayListObj);
    }

    if (displayListObj->unk20->transparentDisplayList != NULL) {
        debugEnqueueCallback(renderLayer, 3, &renderTransparentScrollingTexture, displayListObj);
    }

    if (displayListObj->unk20->overlayDisplayList != NULL) {
        debugEnqueueCallback(renderLayer, 5, &renderOverlayScrollingTexture, displayListObj);
    }
}

INCLUDE_ASM("asm/nonmatchings/5520", func_80005094_5C94);

void renderOpaqueTiledTexture(DisplayListObject *arg0) {
    func_80005094_5C94();
    buildDisplayListSegment(arg0);
}

void renderTransparentTiledTexture(s32 arg0) {
    func_80005094_5C94();
    func_800638C0_644C0((DisplayListObject *)arg0);
}

void renderOverlayTiledTexture(s32 arg0) {
    func_80005094_5C94();
    func_8006395C_6455C((DisplayListObject *)arg0);
}

void enqueueTiledTextureRender(s32 arg0, func_800B5500_Task *arg1) {
    arg1->unk30 = 0;
    if (arg1->unk20->unk4 != 0) {
        debugEnqueueCallback(arg0 & 0xFFFF, 1, &renderOpaqueTiledTexture, arg1);
    }
    if (arg1->unk20->unk8 != 0) {
        debugEnqueueCallback(arg0 & 0xFFFF, 3, &renderTransparentTiledTexture, arg1);
    }
    if (arg1->unk20->unkC != 0) {
        debugEnqueueCallback(arg0 & 0xFFFF, 5, &renderOverlayTiledTexture, arg1);
    }
}

void initializeRotatingLogo(RotatingLogoState *state) {
    memcpy(&state->opaqueMatrix, identityMatrix, 0x20);
    state->opaqueDisplayList = loadAssetGroupDisplayList(state->model);
    state->opaqueVertexData = loadAssetGroupVertexData(state->model);
    state->opaqueFlag = 0;
    state->opaqueSettings = &D_800885D0_891D0;
    memcpy(&state->transparentMatrix, identityMatrix, 0x20);
    state->transparentDisplayList = loadAssetGroupDisplayList(state->model);
    state->transparentVertexData = loadAssetGroupVertexData(state->model);
    state->transparentFlag = 0;
    state->transparentSettings = &D_800885E0_891E0;
    memcpy(&state->overlayMatrix, identityMatrix, 0x20);
    state->overlayDisplayList = loadAssetGroupDisplayList(state->model);
    state->overlayVertexData = loadAssetGroupVertexData(state->model);
    state->overlaySettings = &D_800885F0_891F0;
    state->overlayFlag = 0;
    state->scale = 0x2000;
    state->rotationY = 0;
    state->transparentEnabled = 0;
    state->overlayAnimState = 0;
    state->overlayScaleX = 0;
    state->overlayScaleY = 0;
    state->soundData = loadAssetGroupSoundData(state->model);
    state->scrollU = 0;
    state->scrollV = 0;
    state->scrollSpeedU = 0;
    state->scrollSpeedV = -0x10;
    state->unkE0 = 0;
    state->unkE2 = 0;
    state->unkE4 = 0;
    state->oscillationEnabled = 0;
    state->oscillationAngle = 0;
    state->oscillationOffset = 0;
    state->scaleAnimState = 0;
    setCleanupCallback(&cleanupRotatingLogo);
    setCallback(&func_8000595C_655C);
}

INCLUDE_ASM("asm/nonmatchings/5520", func_8000595C_655C);

void cleanupRotatingLogo(RotatingLogoState *state) {
    state->soundData = freeNodeMemory(state->soundData);
    state->overlayDisplayList = freeNodeMemory(state->overlayDisplayList);
    state->overlayVertexData = freeNodeMemory(state->overlayVertexData);
    state->opaqueDisplayList = freeNodeMemory(state->opaqueDisplayList);
    state->opaqueVertexData = freeNodeMemory(state->opaqueVertexData);
    state->transparentDisplayList = freeNodeMemory(state->transparentDisplayList);
    state->transparentVertexData = freeNodeMemory(state->transparentVertexData);
}
