#include "20F0.h"
#include "56910.h"
#include "68CF0.h"
#include "6E840.h"
#include "common.h"
#include "displaylist.h"
#include "gamestate.h"
#include "geometry.h"
#include "overlay.h"
#include "task_scheduler.h"

USE_ASSET(_419C60);
USE_ASSET(_42F1D0);
USE_ASSET(_41A1D0);
USE_ASSET(_4237C0);
USE_ASSET(_3F6670)
USE_ASSET(_3F6950)

typedef struct {
    s16 unk00;
    s16 unk02;
    s16 unk04;
    s16 unk06;
    void *unk08;
} SubStruct;

typedef struct {
    void *unk00;
    s16 unk04;
    s16 unk06;
    void *unk08;
    s16 unk0C;
    s16 unk0E;
    SubStruct substruct[3];
    s16 unk34;
    s16 unk36;
    s8 unk38;
    s8 unk39;
    s8 unk3A;
} Struct_80030694;

typedef struct {
    u8 padding[0x2C];
    s32 *unk2C;
} func_8002FA70_30670_arg;

typedef struct {
    u8 padding[0x20];
    s32 unk20;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    u8 padding2[0x30];
    s8 unk60;
} func_8002F658_30258_arg;

typedef struct {
    SceneModel *unk0;
    s32 unk4;
    u8 padding[0x1B];
    s16 unk24;
} func_8002EFD8_2FBD8_arg;

typedef struct {
    u8 padding[0x2C];
    s32 unk2C;
} func_8002FA1C_3061C_arg;
typedef struct {
    u8 padding[0x8];
    char *cameraYString;
    u8 padding2[0x8];
    char *cameraYRotationString;
    s32 cameraY;
    s16 cameraRotation;
} cameraState;

typedef struct {
    u8 padding[0x3C];
    s32 unk3C;
    u8 padding2[0x10];
    u32 unk50;
    u8 padding3[0xC];
    u8 unk60;
} S0;

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
} func_800308FC_314FC_arg;

typedef struct {
    void *unk0;
    Mat3x3Padded unk4;
    s16 unk24;
    s8 unk26;
} func_8002EF3C_2FB3C_arg;

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    void *unk8;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    u8 padding[0x2];
    void *unk14;
    s32 unk18;
    s16 unk1C;
    s16 unk1E;
    u8 padding2[0x1D];
    s16 unk3E;
} func_8002ED90_2F990_arg;

typedef struct {
    u8 padding[0x50];
    s32 unk50;
    u8 padding2[0xC];
    u8 unk60;
} func_8002F518_30118_arg;

typedef struct {
    func_800308FC_314FC_arg items[7];
    s16 unk54;
    s16 unk56;
    void *unk58;
    s16 unk5C;
    s8 unk5E;
} func_800302AC_30EAC_arg;

typedef struct {
    u8 _pad[0x4];
    void* unk4;
} func_8002FF28_30B28_arg;

extern void func_8000FED0_10AD0(void);
extern void* freeNodeMemory(void*);
extern s32 gButtonsPressed[];
extern s32 gControllerInputs[4];
extern u8 identityMatrix[];

void func_80030378_30F78(void);
void func_80030480_31080(void);
void func_8002EFD8_2FBD8(void *);
void func_8002F024_2FC24(void);
void func_8002F110_2FD10(func_8002EFD8_2FBD8_arg *);
void func_8002F290_2FE90(void);
void func_8002F5C8_301C8(void *);
void func_8002F72C_3032C(void);
void func_8002F980_30580(void);
void func_8002FA1C_3061C(func_8002FA1C_3061C_arg *);
void func_8002FA44_30644(void *);
void func_8002FA70_30670(func_8002FA70_30670_arg *);
void func_8002FB40_30740(void);
void func_8002FCA8_308A8(func_8002FF28_30B28_arg* arg0);
void func_80030238_30E38(void);
void func_80030280_30E80(void);
void func_80030764_31364(void);
void func_800308C4_314C4(void);
void func_80030974_31574(void *);
void func_800309D4_315D4(func_800308FC_314FC_arg *);
void func_800394BC_3A0BC(func_8002FA1C_3061C_arg *, s32);
void func_80038420_39020(void);
void func_8006FF90_70B90(s32, s32, void *, void *);
void updateDebugCameraYState(cameraState *arg0);

void initDebugCameraController(func_8002ED90_2F990_arg *arg0) {
    arg0->unk18 = 0;
    arg0->unk1C = 0;
    arg0->unk0 = -0x80;
    arg0->unk2 = -0x58;
    arg0->unk8 = &arg0->unk3E;
    arg0->unkE = -0x50;
    arg0->unk4 = 0;
    arg0->unkC = -0x80;
    arg0->unk10 = 0;
    arg0->unk14 = &arg0->unk1E;

    setCallback(&updateDebugCameraYState);
}

void updateDebugCameraYState(cameraState *arg0) {
    s32 sp20[8];
    u64 pad;
    char *cameraYRotation;
    char *cameraYString;
    GameState *temp_s0 = (GameState *)getCurrentAllocation();

    if (gButtonsPressed[1] & R_TRIG) {
        arg0->cameraRotation++;
    } else if (gButtonsPressed[1] & L_TRIG) {
        arg0->cameraRotation--;
    }
    do {
        arg0->cameraRotation = arg0->cameraRotation & 0x1FFF;
    } while (0);
    if (gControllerInputs[1] & U_JPAD) {
        arg0->cameraY--;
    } else if (gControllerInputs[1] & D_JPAD) {
        arg0->cameraY++;
    }
    func_8006BEDC_6CADC(&sp20, 0, arg0->cameraY << 0x10, 0x600000, 0, (s32)((s16)arg0->cameraRotation), 0);
    func_8006FD3C_7093C(temp_s0->unkDA, &sp20);

    cameraYString = ((char *)((s32)arg0)) + 0x3E;
    sprintf(cameraYString, "CAMERA Y = %d", arg0->cameraY);
    cameraYRotation = ((char *)((s32)arg0)) + 0x1E;
    sprintf(cameraYRotation, "CAMERA RX = %X", (s16)arg0->cameraRotation);
    arg0->cameraYString = cameraYString;
    arg0->cameraYRotationString = cameraYRotation;

    debugEnqueueCallback(8, 7, &renderTextPalette, arg0);
    debugEnqueueCallback(8, 7, &renderTextPalette, ((void *)((s32)arg0)) + 0xC);
}

void func_8002EF3C_2FB3C(func_8002EF3C_2FB3C_arg *arg0) {
    GameState *temp = (GameState *)getCurrentAllocation();
    arg0->unk0 = func_8000198C_258C(0x3A, temp);
    memcpy(&arg0->unk4, identityMatrix, 0x20);
    arg0->unk4.unk14 = 0x200000;
    arg0->unk4.unk18 = 0xFFE00000;
    arg0->unk4.unk1C = 0x80000;
    arg0->unk24 = 4;
    arg0->unk26 = 0;
    createYRotationMatrix(&arg0->unk4, 0x1E00);
    setCleanupCallback(func_8002F110_2FD10);
    setCallback(func_8002EFD8_2FBD8);
}

void func_8002EFD8_2FBD8(void *untypedArg0) {
    func_8002EFD8_2FBD8_arg *arg0 = (func_8002EFD8_2FBD8_arg *)untypedArg0;
    applyTransformToModel(arg0->unk0, (void *)((s32)arg0 + 4));
    func_800021B8_2DB8(arg0->unk0, arg0->unk24);
    updateModelGeometry(arg0->unk0);
    setCallback(&func_8002F024_2FC24);
}

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002F024_2FC24);

void func_8002F110_2FD10(func_8002EFD8_2FBD8_arg *arg0) {
    func_80002014_2C14(arg0->unk0);
}

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002F12C_2FD2C);

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002F290_2FE90);

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002F36C_2FF6C);

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002F3E4_2FFE4);

void func_8002F518_30118(func_8002F518_30118_arg *s0) {
    volatile u8 padding[0x20];
    u32 new_var;
    GameState *s1 = (GameState *)getCurrentAllocation();
    u32 offset;
    new_var = s1->unk5C6;

    if (new_var == 2) {
        offset = 0xFFF80000;
    } else {
        offset = 0x00080000;
    }

    s0->unk50 += offset;

    memcpy(s0, (void *)((s32)s0 + 0x3C), 0x20);

    s0->unk60++;
    if (s0->unk60 == 4) {
        s0->unk60 = 0;
        s1->unk5C7++;
        setCallback(func_8002F5C8_301C8);
    }

    enqueueDisplayListObject(0, s0);
}

void func_8002F5C8_301C8(void *untypedArg) {
    DisplayListObject *arg0 = (DisplayListObject *)untypedArg;
    volatile u8 pad[0x20];
    if (((GameState *)getCurrentAllocation())->unk5C5 == 1) {
        setCallback(&func_8002F290_2FE90);
    }
    enqueueDisplayListObject(0, arg0);
}

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002F614_30214);

void func_8002F658_30258(func_8002F658_30258_arg *arg0) {
    Mat3x3Padded sp10;
    Mat3x3Padded sp30;
    Mat3x3Padded *new_var;
    void *temp_s3;

    getCurrentAllocation();
    new_var = &sp10;
    temp_s3 = ((void *)((s32)arg0)) + 0x3C;
    memcpy(temp_s3, &identityMatrix, sizeof(Mat3x3Padded));
    memcpy(&sp30, temp_s3, sizeof(Mat3x3Padded));
    memcpy(new_var, &sp30, sizeof(Mat3x3Padded));
    createRotationMatrixYX(&sp10, 0x1000, 0x800);
    createZRotationMatrix(&sp30, 0x1F00);
    func_8006B084_6BC84(&sp10, &sp30, temp_s3);
    arg0->unk60 = 0;
    arg0->unk20 = 0;
    arg0->unk24 = 0;
    arg0->unk28 = 0;
    arg0->unk2C = 0;
    setCleanupCallback(&func_8002F980_30580);
    setCallback(&func_8002F72C_3032C);
}

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002F72C_3032C);

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002F860_30460);

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002F88C_3048C);

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002F948_30548);

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002F980_30580);

void func_8002F9C4_305C4(func_8002FA70_30670_arg *arg0) {
    arg0->unk2C = dmaRequestAndUpdateStateWithSize(&_42F1D0_ROM_START, &_42F1D0_ROM_END, 0x14410);
    setCleanupCallback(&func_8002FA70_30670);
    setCallback(&func_8002FA1C_3061C);
}

void func_8002FA1C_3061C(func_8002FA1C_3061C_arg *arg0) {
    func_800394BC_3A0BC(arg0, arg0->unk2C);
    setCallback(&func_8002FA44_30644);
}

void func_8002FA44_30644(void* arg0) {
    debugEnqueueCallback(1, 0, func_80038420_39020, arg0);
}

void func_8002FA70_30670(func_8002FA70_30670_arg *arg0) {
    arg0->unk2C = freeNodeMemory(arg0->unk2C);
}

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
    s16 unkA;
    s8 unkC;
    s8 unkD;
    s8 unkE;
    s8 unkF;
} func_8002FA9C_3069C_item;

typedef struct {
    func_8002FA9C_3069C_item items[2];
    s8 unk20;
} func_8002FA9C_3069C_arg;

void func_8002FA9C_3069C(func_8002FA9C_3069C_arg *arg0) {
    s32 i;
    void *asset = dmaRequestAndUpdateStateWithSize(&_4237C0_ROM_START, &_4237C0_ROM_END, 0x8A08);

    setCleanupCallback(&func_8002FCA8_308A8);

    for (i = 0; i < 2; i++) {
        arg0->items[i].unk0 = -0x40 + i * 0x60;
        arg0->items[i].unk2 = -0x18;
        arg0->items[i].unk8 = i;
        arg0->items[i].unk4 = asset;
        arg0->items[i].unkA = 0xFF;
        arg0->items[i].unkD = 0;
        arg0->items[i].unkC = 0;
    }

    arg0->unk20 = 0;

    setCallback(&func_8002FB40_30740);
}

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002FB40_30740);

void func_8002FCA8_308A8(func_8002FF28_30B28_arg* arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002FCD4_308D4);

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002FDFC_309FC);

void func_8002FF28_30B28(func_8002FF28_30B28_arg* arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002FF54_30B54);

INCLUDE_ASM("asm/nonmatchings/2F990", func_8003006C_30C6C);

void func_80030194_30D94(func_8002FF28_30B28_arg* arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_800301C0_30DC0(func_800308FC_314FC_arg *arg0) {
    void *temp_s1 = dmaRequestAndUpdateStateWithSize(&_41A1D0_ROM_START, &_41A1D0_ROM_END, 0x1B48);

    setCleanupCallback(&func_80030280_30E80);

    arg0->unk0 = -0x2C;
    arg0->unk2 = -0x14;
    arg0->unk8 = 0xD;
    arg0->unk4 = temp_s1;

    setCallback(&func_80030238_30E38);
}

INCLUDE_ASM("asm/nonmatchings/2F990", func_80030238_30E38);

INCLUDE_ASM("asm/nonmatchings/2F990", func_80030280_30E80);

void func_800302AC_30EAC(func_800302AC_30EAC_arg *arg0) {
    void *asset2;
    void *asset1;
    s32 i;
    s32 *new_var;

    asset1 = dmaRequestAndUpdateStateWithSize(&_3F6950_ROM_START, &_3F6950_ROM_END, 0x508);
    asset2 = dmaRequestAndUpdateStateWithSize(&_3F6670_ROM_START, &_3F6670_ROM_END, 0x388);
    setCleanupCallback(&func_80030480_31080);
    for (i = 0; i < 7; i++) {
        arg0->items[i].unk0 = 0x48 + (i * 8);
        arg0->items[i].unk2 = 0x58;
        arg0->items[i].unk4 = asset1;
    }

    arg0->unk54 = 0x38;
    arg0->unk56 = 0x58;
    arg0->unk5C = 0;
    arg0->unk5E = 0;
    arg0->unk58 = asset2;
    setCallback(&func_80030378_30F78);
}

INCLUDE_ASM("asm/nonmatchings/2F990", func_80030378_30F78);

INCLUDE_ASM("asm/nonmatchings/2F990", func_80030480_31080);

INCLUDE_ASM("asm/nonmatchings/2F990", func_800304B8_310B8);

INCLUDE_ASM("asm/nonmatchings/2F990", func_80030540_31140);

typedef struct {
    u8 _pad[0x4];
    void *unk4;
} func_80030668_31268_arg;

void func_80030668_31268(func_80030668_31268_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_80030694_31294(Struct_80030694 *arg0) {
    void *var1;
    void *var2;
    s32 i;

    getCurrentAllocation();
    arg0->unk00 = loadAsset_34F7E0();
    var2 = dmaRequestAndUpdateStateWithSize(&_419C60_ROM_START, &_419C60_ROM_END, 0x1548);
    setCleanupCallback(func_800308C4_314C4);

    arg0->unk34 = -0x24;
    arg0->unk04 = -0x24;
    arg0->unk06 = 0x1C;
    arg0->unk36 = 0;
    arg0->unk08 = var2;
    arg0->unk0C = 4;

    for (i = 0; i < 3; i++) {
        arg0->substruct[i].unk02 = 0x1C + (i * 8);
        arg0->substruct[i].unk00 = 0xC;
        arg0->substruct[i].unk08 = (void *)((char *)arg0 + 0x38 + (i * 3));
        arg0->substruct[i].unk04 = 0;
    }

    setCallback(func_80030764_31364);
}

INCLUDE_ASM("asm/nonmatchings/2F990", func_80030764_31364);

INCLUDE_ASM("asm/nonmatchings/2F990", func_800308C4_314C4);

void func_800308FC_314FC(func_800308FC_314FC_arg *arg0) {
    void *temp_s1 = dmaRequestAndUpdateStateWithSize(&_419C60_ROM_START, &_419C60_ROM_END, 0x1548);

    setCleanupCallback(&func_800309D4_315D4);

    arg0->unk0 = -0x2C;
    arg0->unk2 = -0x18;
    arg0->unk4 = temp_s1;
    arg0->unk8 = 5;

    setCallback(&func_80030974_31574);
}

void func_80030974_31574(void *arg0) {
    GameState *state = (GameState *)getCurrentAllocation();
    s8 index = state->unk5C8;

    if (state->unk5CA[index] >= 0x80 || state->unk5C9 == 0) {
        debugEnqueueCallback(8, 1, func_8000FED0_10AD0, arg0);
    }
}

void func_800309D4_315D4(func_800308FC_314FC_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

INCLUDE_ASM("asm/nonmatchings/2F990", func_80030A00_31600);

INCLUDE_ASM("asm/nonmatchings/2F990", func_80030AEC_316EC);

INCLUDE_ASM("asm/nonmatchings/2F990", func_80030B70_31770);

INCLUDE_ASM("asm/nonmatchings/2F990", func_80030BA8_317A8);

INCLUDE_RODATA("asm/nonmatchings/2F990", D_8009E47C_9F07C);

INCLUDE_RODATA("asm/nonmatchings/2F990", D_8009E480_9F080);

INCLUDE_RODATA("asm/nonmatchings/2F990", D_8009E488_9F088);

INCLUDE_RODATA("asm/nonmatchings/2F990", D_8009E48C_9F08C);
