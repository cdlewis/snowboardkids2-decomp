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
    /* 0x00 */ void *unk0;
    /* 0x04 */ u8 unk4[0x20];
    /* 0x24 */ void *unk24;
    /* 0x28 */ void *unk28;
    /* 0x2C */ void *unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ u8 pad34[0xC];
    /* 0x40 */ u8 unk40[0x20];
    /* 0x60 */ void *unk60;
    /* 0x64 */ void *unk64;
    /* 0x68 */ void *unk68;
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ u8 pad70[0xC];
    /* 0x7C */ s32 unk7C;
    /* 0x80 */ s16 unk80;
    /* 0x82 */ s8 unk82;
    /* 0x83 */ s8 unk83;
    /* 0x84 */ s32 unk84;
    /* 0x88 */ s32 unk88;
    /* 0x8C */ s32 unk8C;
    /* 0x90 */ s16 unk90;
    /* 0x92 */ s8 unk92;
    /* 0x93 */ s8 unk93;
    /* 0x94 */ u8 unk94[0x20];
    /* 0xB4 */ void *unkB4;
    /* 0xB8 */ void *unkB8;
    /* 0xBC */ void *unkBC;
    /* 0xC0 */ s32 unkC0;
    /* 0xC4 */ u8 padC4[0xC];
    /* 0xD0 */ void *unkD0;
    /* 0xD4 */ u8 padD4[0x4];
    /* 0xD8 */ s16 unkD8;
    /* 0xDA */ s16 unkDA;
    /* 0xDC */ s16 unkDC;
    /* 0xDE */ s16 unkDE;
    /* 0xE0 */ s16 unkE0;
    /* 0xE2 */ s16 unkE2;
    /* 0xE4 */ s16 unkE4;
} func_80005808_6408_arg;

void func_800056E8_62E8(DisplayListObject *arg0);
void func_80005714_6314(s32 arg0);
void func_80005740_6340(s32 arg0);
void func_80004920_5520(void);
void func_80005094_5C94(void);
void func_80005D30_6930(func_80005808_6408_arg *);
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

void func_80004FF8_5BF8(u16 arg0, DisplayListObject *arg1) {
    arg1->unk30 = 0;

    if (arg1->unk20->opaqueDisplayList != NULL) {
        debugEnqueueCallback(arg0, 1, &renderOpaqueScrollingTexture, arg1);
    }

    if (arg1->unk20->transparentDisplayList != NULL) {
        debugEnqueueCallback(arg0, 3, &renderTransparentScrollingTexture, arg1);
    }

    if (arg1->unk20->overlayDisplayList != NULL) {
        debugEnqueueCallback(arg0, 5, &renderOverlayScrollingTexture, arg1);
    }
}

INCLUDE_ASM("asm/nonmatchings/5520", func_80005094_5C94);

void func_800056E8_62E8(DisplayListObject *arg0) {
    func_80005094_5C94();
    buildDisplayListSegment(arg0);
}

void func_80005714_6314(s32 arg0) {
    func_80005094_5C94();
    func_800638C0_644C0((DisplayListObject *)arg0);
}

void func_80005740_6340(s32 arg0) {
    func_80005094_5C94();
    func_8006395C_6455C((DisplayListObject *)arg0);
}

void func_8000576C_636C(s32 arg0, func_800B5500_Task *arg1) {
    arg1->unk30 = 0;
    if (arg1->unk20->unk4 != 0) {
        debugEnqueueCallback(arg0 & 0xFFFF, 1, &func_800056E8_62E8, arg1);
    }
    if (arg1->unk20->unk8 != 0) {
        debugEnqueueCallback(arg0 & 0xFFFF, 3, &func_80005714_6314, arg1);
    }
    if (arg1->unk20->unkC != 0) {
        debugEnqueueCallback(arg0 & 0xFFFF, 5, &func_80005740_6340, arg1);
    }
}

void func_80005808_6408(func_80005808_6408_arg *arg0) {
    memcpy(&arg0->unk4, identityMatrix, 0x20);
    arg0->unk28 = loadAssetGroupDisplayList(arg0->unk0);
    arg0->unk2C = loadAssetGroupVertexData(arg0->unk0);
    arg0->unk30 = 0;
    arg0->unk24 = &D_800885D0_891D0;
    memcpy(&arg0->unk40, identityMatrix, 0x20);
    arg0->unk64 = loadAssetGroupDisplayList(arg0->unk0);
    arg0->unk68 = loadAssetGroupVertexData(arg0->unk0);
    arg0->unk6C = 0;
    arg0->unk60 = &D_800885E0_891E0;
    memcpy(&arg0->unk94, identityMatrix, 0x20);
    arg0->unkB8 = loadAssetGroupDisplayList(arg0->unk0);
    arg0->unkBC = loadAssetGroupVertexData(arg0->unk0);
    arg0->unkB4 = &D_800885F0_891F0;
    arg0->unkC0 = 0;
    arg0->unk7C = 0x2000;
    arg0->unk80 = 0;
    arg0->unk82 = 0;
    arg0->unk83 = 0;
    arg0->unk84 = 0;
    arg0->unk88 = 0;
    arg0->unkD0 = loadAssetGroupSoundData(arg0->unk0);
    arg0->unkD8 = 0;
    arg0->unkDA = 0;
    arg0->unkDC = 0;
    arg0->unkDE = -0x10;
    arg0->unkE0 = 0;
    arg0->unkE2 = 0;
    arg0->unkE4 = 0;
    arg0->unk92 = 0;
    arg0->unk90 = 0;
    arg0->unk8C = 0;
    arg0->unk93 = 0;
    setCleanupCallback(&func_80005D30_6930);
    setCallback(&func_8000595C_655C);
}

INCLUDE_ASM("asm/nonmatchings/5520", func_8000595C_655C);

void func_80005D30_6930(func_80005808_6408_arg *arg0) {
    arg0->unkD0 = freeNodeMemory(arg0->unkD0);
    arg0->unkB8 = freeNodeMemory(arg0->unkB8);
    arg0->unkBC = freeNodeMemory(arg0->unkBC);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
    arg0->unk2C = freeNodeMemory(arg0->unk2C);
    arg0->unk64 = freeNodeMemory(arg0->unk64);
    arg0->unk68 = freeNodeMemory(arg0->unk68);
}
