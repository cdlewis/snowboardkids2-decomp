#include "20F0.h"
#include "288A0.h"
#include "5E590.h"
#include "68CF0.h"
#include "6E840.h"
#include "EepromSaveData_type.h"
#include "common.h"
#include "gamestate.h"
#include "geometry.h"
#include "overlay.h"
#include "task_scheduler.h"

USE_ASSET(_419C60);
USE_ASSET(_41A1D0);
USE_ASSET(_4547D0);

extern u16 D_8008F150_8FD50[];
extern const char D_8009E480_9F080;
extern void func_80032708_33308(void);
extern void func_80038420_39020(void);

typedef struct {
    u8 padding[0x77C];
    u16 unk77C;
    u8 padding2[0x6];
    u8 unk784[4];
    u8 unk788[17];
    u8 unk799;
    u8 unk79A;
    u8 unk79B;
} func_800329A8_335A8_allocation;

typedef struct {
    u8 padding[0xC];
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s8 unk12;
    s8 unk13;
    u8 padding2[0x4];
} func_800329A8_335A8_arg_item;

typedef struct {
    func_800329A8_335A8_arg_item unk0[4];
    u8 padding2[0x18];
    s16 unk78;
    s16 unk7A;
    u8 padding3[0x8];
    s8 unk84[4];
    char unk88;
} func_800329A8_335A8_arg;

typedef struct {
    Node_70B00 *unk0;
    s32 unk4;
    u8 padding[0x18];
    void *unk20;
    void *unk24;
    void *unk28;
    void *unk2C;
    void *unk30;
    u8 padding2[0x8];
    s32 unk3C;
    u8 unk40[0x10];
    s32 unk50;
    s32 unk54;
    s32 unk58;
    s32 unk5C;
    s16 unk60;
    s16 unk62;
    s8 unk64;
} func_80031510_32110_arg;

typedef struct {
    u8 padding[0x7A4];
    u8 unk7A4;
} func_8003316C_33D6C_alloc;

typedef struct {
    SceneModel *unk0;
    applyTransformToModel_arg1 *unk4;
    u8 padding[0x1B];
    s16 unk24;
} func_80031ABC_326BC_arg;

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
} func_80032244_32E44_arg;

typedef struct {
    s16 unk0;
    u16 unk2;
    void *unk4;
    s16 unk8;
    s16 unkA;
    s8 unkC;
    s8 unkD;
    s16 unkE;
} func_800330B4_33CB4_arg;

typedef struct {
    void *unk0;
    Mat3x3Padded unk4;
    s16 unk24;
    s16 unk26;
    s8 unk28;
} func_80031A0C_3260C_arg;

typedef struct {
    u8 padding[0x8];
    void *unk8;
} func_80033088_33C88_arg;

typedef struct {
    s16 unk0;
    s32 unk4;
} func_80033014_33C14_arg;

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s32 unk8;
    s16 unkC;
    s16 unkE;
    s8 unk10;
} func_80032F90_33B90_arg;

typedef struct {
    s16 unk0;
    s16 unk2;
    u8 padding[0x80];
    u8 unk84;
} func_80032EDC_33ADC_arg;

typedef struct {
    char padding;
    void *unk4;
    char padding2[78];
    void *unk58;
} func_80032504_33104_arg;

typedef struct {
    u8 padding[0x79C];
    u8 unk79C;
} func_80032EDC_33ADC_asset;

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s8 unk12;
    s8 unk13;
    s8 unk14;
    u8 padding[0x6F];
    s8 unk84;
} func_80032DE8_339E8_arg;

typedef struct {
    u8 padding[0x780];
    s16 unk780;
    s16 unk782;
    s32 unk784;
    u8 unk788[20];
    u8 unk79C;
    u8 unk79D;
    u8 unk79E;
    u8 unk79F;
    u8 unk7A0;
    s8 unk7A1;
    u8 unk7A2;
} func_80032DE8_339E8_asset;

typedef struct {
    u8 padding[0x24];
    void *unk24;
    void *unk28;
    void *unk2C;
} func_800319C8_325C8_arg;

typedef struct {
    s16 unk0;
    s16 unk2;
    u8 padding[0x14];
} func_80032CB4_338B4_arg_item;

typedef struct {
    func_80032CB4_338B4_arg_item substruct[4];
    u8 padding2[0x24];
    u8 unk84;
} func_80032CB4_338B4_arg;

typedef struct {
    u8 padding[0x77C];
    s16 unk77C;
} func_80032CB4_338B4_asset;

typedef struct {
    u8 unk0[4];
    u8 unk4;
} func_80031100_31D00_arg;

extern void func_80012004_12C04(void);
extern void func_80035408_36008(void);
extern s32 func_80035F80_36B80(s32);
extern void func_800136E0_142E0(void);
extern void func_80031138_31D38(void);

void func_800317D4_323D4(void);
void func_800313A4_31FA4(void);
void func_800315C0_321C0(void);
void func_80031C4C_3284C(func_80031A0C_3260C_arg *arg0);
void func_80031ABC_326BC(func_80031ABC_326BC_arg *arg0);
void func_80031B30_32730(void);
void func_800322BC_32EBC(void);
void func_80032304_32F04(func_80032244_32E44_arg *arg0);
void func_8003316C_33D6C(func_800330B4_33CB4_arg *arg0);
void func_800331CC_33DCC(func_800330B4_33CB4_arg *arg0);
void func_80033014_33C14(func_80033014_33C14_arg *arg0);
void func_80033088_33C88(func_80033088_33C88_arg *arg0);
void func_80032EDC_33ADC(func_80032EDC_33ADC_arg *arg0);
void func_80032EA4_33AA4(void *);
void func_80032F64_33B64(func_80032244_32E44_arg *);
void func_800319C8_325C8(func_800319C8_325C8_arg *arg0);
void func_80031944_32544(void);

extern s32 D_8008F18C_8FD8C;
extern s32 D_8008F200_8FE00[];
extern u16 D_8008F20A_8FE0A[];
extern u16 D_8008F20C_8FE0C;
extern s32 identityMatrix;

void func_80030F6C_31B6C(void);
void func_80030E54_31A54(void);
void func_80030C70_31870(func_80031510_32110_arg *arg0) {
    s32 sp20[8];
    Mat3x3Padded matrixA;
    Mat3x3Padded matrixB;

    u8 temp_s0;
    s32 temp_s0_2;
    u8 temp_s2;
    void *temp_s3;
    func_80032DE8_339E8_asset *asset;
    Node_70B00 *temp_v0;
    Mat3x3Padded *pMatrixB;

    pMatrixB = &matrixB;

    asset = getCurrentAllocation();
    temp_v0 = allocateNodeMemory(0x1D8);
    arg0->unk0 = temp_v0;
    func_80027CA0_288A0(temp_v0, 1, 0xB, 0);
    arg0->unk60 = -0x34;
    arg0->unk62 = 0x34;
    asset->unk780 = (u16)arg0->unk60;
    asset->unk782 = (u16)arg0->unk62;

    setModelCameraTransform(arg0->unk0, 0, 0, -0x98, arg0->unk60, 0x97, arg0->unk62);
    func_8006BEDC_6CADC(sp20, 0, 0, 0x580000, 0, 0, 0);
    func_8006FD3C_7093C(arg0->unk0->id, sp20);

    temp_s3 = &arg0->unk40;
    memcpy(temp_s3, &identityMatrix, 0x20);
    memcpy(pMatrixB, temp_s3, 0x20);
    memcpy(&matrixA, pMatrixB, 0x20);

    createRotationMatrixYX(&matrixA, 0x1000, 0x800);
    createZRotationMatrix(pMatrixB, 0x1F00);

    func_8006B084_6BC84(&matrixA, pMatrixB, temp_s3);

    arg0->unk5C = 0xFFF80000;
    temp_s0 = asset->unk7A2 + (asset->unk7A1 * 3);
    temp_s2 = EepromSaveData->character_or_settings[temp_s0 & 0xFF];

    memcpy(&arg0->unk4, temp_s3, 0x20);

    temp_s0 = temp_s0 & 0xFF;
    arg0->unk24 = loadAssetByIndex_95728(temp_s0);
    arg0->unk28 = loadAssetByIndex_95500(temp_s0);
    arg0->unk2C = loadAssetByIndex_95590(temp_s0);
    arg0->unk30 = loadAssetByIndex_95668(temp_s2 - 1);

    setCleanupCallback(&func_80030F6C_31B6C);
    arg0->unk64 = 0xC;
    setCallback(&func_80030E54_31A54);
}

INCLUDE_ASM("asm/nonmatchings/31870", func_80030E54_31A54);

INCLUDE_ASM("asm/nonmatchings/31870", func_80030EAC_31AAC);

INCLUDE_ASM("asm/nonmatchings/31870", func_80030F6C_31B6C);

INCLUDE_ASM("asm/nonmatchings/31870", func_80030FBC_31BBC);

void func_80031100_31D00(func_80031100_31D00_arg* arg0) {
    arg0->unk4--;
    if (arg0->unk4 == 0) {
        setCallback(func_80031138_31D38);
    }
}

INCLUDE_ASM("asm/nonmatchings/31870", func_80031138_31D38);

INCLUDE_ASM("asm/nonmatchings/31870", func_80031248_31E48);

void func_80031288_31E88(func_80031510_32110_arg *arg0) {
    Mat3x3Padded matrixA;
    Mat3x3Padded *pMatrixB;
    Mat3x3Padded *pMatrixA;
    Mat3x3Padded matrixB;
    u8 temp_s0;
    void *temp_s3;

    pMatrixB = &matrixB;
    pMatrixA = &matrixA;

    getCurrentAllocation();
    temp_s3 = &arg0->unk3C;
    memcpy(temp_s3, &identityMatrix, 0x20);
    memcpy(pMatrixB, temp_s3, 0x20);
    memcpy(pMatrixA, pMatrixB, 0x20);
    createRotationMatrixYX(pMatrixA, 0x1000, 0x800);
    createZRotationMatrix(pMatrixB, 0x1F00);
    func_8006B084_6BC84(pMatrixA, pMatrixB, temp_s3);
    arg0->unk50 = 0x600000;
    arg0->unk58 = 0xFFF80000;
    temp_s0 = EepromSaveData->character_or_settings[0];
    memcpy(arg0, temp_s3, 0x20);
    arg0->unk20 = loadAssetByIndex_95728(0);
    arg0->unk24 = loadAssetByIndex_95500(0);
    arg0->unk28 = loadAssetByIndex_95590(0);
    arg0->unk2C = loadAssetByIndex_95668(temp_s0 - 1);
    setCleanupCallback(&func_800317D4_323D4);
    setCallback(&func_800313A4_31FA4);
}

INCLUDE_ASM("asm/nonmatchings/31870", func_800313A4_31FA4);

INCLUDE_ASM("asm/nonmatchings/31870", func_800313DC_31FDC);

INCLUDE_ASM("asm/nonmatchings/31870", func_80031458_32058);

void func_80031510_32110(func_80031510_32110_arg *arg0) {
    u8 assetIndex;
    u8 temp_v1;
    func_80032DE8_339E8_asset *state;

    state = getCurrentAllocation();

    if (state->unk79C == 0) {
        arg0->unk50 = 0xFFA00000;
    } else {
        arg0->unk50 = 0x600000;
    }

    assetIndex = EepromSaveData->character_or_settings[state->unk79E];

    memcpy(arg0, (u8 *)arg0 + 0x3C, 0x20);

    arg0->unk20 = loadAssetByIndex_95728(state->unk79E);
    arg0->unk24 = loadAssetByIndex_95500(state->unk79E);
    arg0->unk28 = loadAssetByIndex_95590(state->unk79E);
    arg0->unk2C = loadAssetByIndex_95668(assetIndex - 1);

    setCallback(&func_800315C0_321C0);
}

INCLUDE_ASM("asm/nonmatchings/31870", func_800315C0_321C0);

INCLUDE_ASM("asm/nonmatchings/31870", func_8003165C_3225C);

INCLUDE_ASM("asm/nonmatchings/31870", func_800316AC_322AC);

INCLUDE_ASM("asm/nonmatchings/31870", func_80031758_32358);

INCLUDE_ASM("asm/nonmatchings/31870", func_800317D4_323D4);

void func_80031818_32418(func_80031510_32110_arg *arg0) {
    func_80032DE8_339E8_asset *state;
    Mat3x3Padded matrixB;
    Mat3x3Padded matrixA;
    u8 temp_v0;
    u8 temp_s1;
    Mat3x3Padded *pMatrixB;
    Mat3x3Padded *pMatrixA;

    state = getCurrentAllocation();

    memcpy(&arg0->unk3C, &identityMatrix, sizeof(Mat3x3Padded));

    pMatrixA = &matrixA;
    memcpy(pMatrixA, &arg0->unk3C, sizeof(Mat3x3Padded));

    pMatrixB = &matrixB;
    memcpy(pMatrixB, pMatrixA, sizeof(Mat3x3Padded));

    createRotationMatrixYX(pMatrixB, 0x1000, 0x800);
    createZRotationMatrix(pMatrixA, 0x1F00);
    func_8006B084_6BC84(pMatrixB, pMatrixA, &arg0->unk3C);

    arg0->unk58 = 0xFFF80000;
    memcpy(arg0, &arg0->unk3C, 0x20U);

    temp_v0 = state->unk79F;
    temp_s1 = EepromSaveData->character_or_settings[temp_v0];

    arg0->unk20 = loadAssetByIndex_95728(temp_v0);
    arg0->unk24 = loadAssetByIndex_95500(temp_v0);
    arg0->unk28 = loadAssetByIndex_95590(temp_v0);
    arg0->unk2C = loadAssetByIndex_95668(temp_s1 - 1);

    setCleanupCallback(&func_800319C8_325C8);
    setCallbackWithContinue(&func_80031944_32544);
}

INCLUDE_ASM("asm/nonmatchings/31870", func_80031944_32544);

void func_800319C8_325C8(func_800319C8_325C8_arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
    arg0->unk2C = freeNodeMemory(arg0->unk2C);
}

void func_80031A0C_3260C(func_80031A0C_3260C_arg *arg0) {
    arg0->unk0 = func_800019B8_25B8(0x3A, &((GameState *)getCurrentAllocation())->audioPlayer2, 0, -1, 0, 0x12);

    memcpy(&arg0->unk4, &identityMatrix, sizeof(Mat3x3Padded));

    arg0->unk4.unk14 = 0xFFE70000;
    arg0->unk4.unk18 = 0xFFE00000;
    arg0->unk4.unk1C = 0;

    createYRotationMatrix(&arg0->unk4, 0x200);

    arg0->unk24 = 0x10;
    arg0->unk26 = 0x10;
    arg0->unk28 = 0;

    setCleanupCallback(&func_80031C4C_3284C);
    setCallback(&func_80031ABC_326BC);
}

void func_80031ABC_326BC(func_80031ABC_326BC_arg *arg0) {
    GameState *temp_s1;

    temp_s1 = (GameState *)getCurrentAllocation();
    applyTransformToModel(arg0->unk0, &arg0->unk4);
    func_8000160C_220C(arg0->unk0, 1);
    func_800021B8_2DB8(arg0->unk0, arg0->unk24);
    updateModelGeometry(arg0->unk0);
    if (temp_s1->unk79B != 0) {
        setCallback(&func_80031B30_32730);
    }
}

INCLUDE_ASM("asm/nonmatchings/31870", func_80031B30_32730);

void func_80031C4C_3284C(func_80031A0C_3260C_arg *arg0) {
    func_80002014_2C14(arg0->unk0);
}

INCLUDE_ASM("asm/nonmatchings/31870", func_80031C68_32868);

INCLUDE_ASM("asm/nonmatchings/31870", func_80031CC0_328C0);

void func_80031CE8_328E8(void *arg0) {
    debugEnqueueCallback(9, 0, &func_80038420_39020, arg0);
}

INCLUDE_ASM("asm/nonmatchings/31870", func_80031D14_32914);

INCLUDE_ASM("asm/nonmatchings/31870", func_80031D40_32940);

INCLUDE_ASM("asm/nonmatchings/31870", func_80031DE4_329E4);

INCLUDE_ASM("asm/nonmatchings/31870", func_80031F68_32B68);

INCLUDE_ASM("asm/nonmatchings/31870", func_80031F94_32B94);

INCLUDE_ASM("asm/nonmatchings/31870", func_80032020_32C20);

typedef struct {
    void *unk0;
    void *unk4;
} func_800320B8_32CB8_arg;

void func_800320B8_32CB8(func_800320B8_32CB8_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

INCLUDE_ASM("asm/nonmatchings/31870", func_800320E4_32CE4);

INCLUDE_ASM("asm/nonmatchings/31870", func_80032170_32D70);

typedef struct {
    void *unk0;
    void *unk4;
} func_80032218_32E18_arg;

void func_80032218_32E18(func_80032218_32E18_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_80032244_32E44(func_80032244_32E44_arg *arg0) {
    void *asset = dmaRequestAndUpdateStateWithSize(&_41A1D0_ROM_START, &_41A1D0_ROM_END, 0x1B48);
    setCleanupCallback(&func_80032304_32F04);
    arg0->unk0 = -0x2C;
    arg0->unk2 = -0x14;
    arg0->unk8 = 0xD;
    arg0->unk4 = asset;
    setCallback(&func_800322BC_32EBC);
}

INCLUDE_ASM("asm/nonmatchings/31870", func_800322BC_32EBC);

void func_80032304_32F04(func_80032244_32E44_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

INCLUDE_ASM("asm/nonmatchings/31870", func_80032330_32F30);

INCLUDE_ASM("asm/nonmatchings/31870", func_800323FC_32FFC);

void func_80032504_33104(func_80032504_33104_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
    arg0->unk58 = freeNodeMemory(arg0->unk58);
}

INCLUDE_ASM("asm/nonmatchings/31870", func_8003253C_3313C);

INCLUDE_ASM("asm/nonmatchings/31870", func_80032628_33228);

INCLUDE_ASM("asm/nonmatchings/31870", func_80032708_33308);

void func_800329A8_335A8(func_800329A8_335A8_arg *arg0) {
    func_800329A8_335A8_allocation *state;
    s32 i;
    u8 temp;
    s16 temp2;

    state = getCurrentAllocation();

    for (i = 0; i < 4; i++) {
        arg0->unk0[i].unkC = 0x400;
        arg0->unk84[i] = 0;
        temp2 = 0xFF;

        if (state->unk799 == i) {
            arg0->unk0[i].unk10 = 0xFF;
            arg0->unk0[i].unk13 = 0;
            if (state->unk79B == 0x14) {
                if ((state->unk77C & 1) != 0) {
                    __asm__ volatile("" ::: "memory");
                    arg0->unk0[i].unk13 = temp2;
                }
            }
        } else {
            arg0->unk0[i].unk10 = 0x80;
        }

        debugEnqueueCallback(8, 0, &func_800136E0_142E0, &arg0->unk0[i]);
    }

    temp = state->unk784[state->unk799];
    temp = state->unk788[temp];

    sprintf(&arg0->unk88, &D_8009E480_9F080, D_8008F150_8FD50[temp]);
    arg0->unk7A = state->unk799 * 0x28 - 0x2A;
    debugEnqueueCallback(8, 7, &renderTextPalette, &arg0->unk78);
    if (state->unk79B < 0x14) {
        setCallback(&func_80032708_33308);
    }
}

INCLUDE_ASM("asm/nonmatchings/31870", func_80032B0C_3370C);

INCLUDE_ASM("asm/nonmatchings/31870", func_80032BCC_337CC);

void func_80032CB4_338B4(func_80032CB4_338B4_arg *arg0) {
    func_80032CB4_338B4_asset *s4;
    func_80032CB4_338B4_arg_item *ptr;
    u8 a1;
    s32 i;

    s4 = (func_80032CB4_338B4_asset *)getCurrentAllocation();

    ptr = &arg0->substruct[0];
    for (i = 0; i < 4; i++) {
        if (i >= (4 - arg0->unk84)) {
            ptr[i].unk2 -= 0x14;
            if (i != 0) {
                a1 = arg0->unk84 & 0xFF;
                if (arg0->substruct[4 - a1].unk2 == arg0->substruct[3 - a1].unk2) {
                    arg0->unk84++;
                }
            }
        }
        debugEnqueueCallback(8, 0, &func_800136E0_142E0, &ptr[i]);
    }

    if (arg0->substruct[0].unk2 < (-0x88)) {
        s4->unk77C = 1;
        func_80069CF8_6A8F8();
    }
}

void func_80032DBC_339BC(func_80032F90_33B90_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_80032DE8_339E8(func_80032DE8_339E8_arg *arg0) {
    void *asset;
    func_80032DE8_339E8_asset *temp_s0 = getCurrentAllocation();

    asset = dmaRequestAndUpdateStateWithSize(&_4547D0_ROM_START, &_4547D0_ROM_END, 0x9488);

    arg0->unk0 = 0x60;
    arg0->unk8 = temp_s0->unk788[temp_s0->unk7A0];
    arg0->unkA = 0x400;
    arg0->unkC = 0x400;
    arg0->unkE = 0;
    arg0->unk10 = 0x80;
    arg0->unk13 = 0;
    arg0->unk12 = 0;
    arg0->unk14 = 0;
    arg0->unk4 = asset;
    arg0->unk84 = 0;

    if (temp_s0->unk79C != 1) {
        arg0->unk2 = 0x3F;
    } else {
        arg0->unk2 = -0x39;
    }

    setCleanupCallback(&func_80032F64_33B64);
    setCallback(&func_80032EA4_33AA4);
}

void func_80032EA4_33AA4(void *arg0) {
    debugEnqueueCallback(8, 0, &func_800136E0_142E0, arg0);
    setCallback(&func_80032EDC_33ADC);
}

void func_80032EDC_33ADC(func_80032EDC_33ADC_arg *arg0) {
    func_80032EDC_33ADC_asset *temp_v0 = (func_80032EDC_33ADC_asset *)getCurrentAllocation();

    arg0->unk84++;
    if (temp_v0->unk79C == 1) {
        arg0->unk2 = arg0->unk2 - 0x14;
    } else {
        arg0->unk2 = arg0->unk2 + 0x14;
    }

    debugEnqueueCallback(8, 0, &func_800136E0_142E0, arg0);

    if (arg0->unk84 == 4) {
        func_80069CF8_6A8F8();
    }
}

void func_80032F64_33B64(func_80032244_32E44_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_80032F90_33B90(func_80032F90_33B90_arg *arg0) {
    s32 temp_s1 = func_80035F80_36B80(1);
    setCleanupCallback(&func_80033088_33C88);
    arg0->unk2 = -0x60;
    arg0->unk4 = &D_8008F18C_8FD8C;
    arg0->unkC = 0xFF;
    arg0->unkE = 0xFF;
    arg0->unk8 = temp_s1;
    arg0->unk0 = D_8008F20C_8FE0C;
    arg0->unk10 = 5;
    setCallback(&func_80033014_33C14);
}

void func_80033014_33C14(func_80033014_33C14_arg *arg0) {
    func_8003316C_33D6C_alloc *asset = (func_8003316C_33D6C_alloc *)getCurrentAllocation();
    if (asset->unk7A4 != 0) {
        arg0->unk0 = D_8008F20A_8FE0A[asset->unk7A4];
        arg0->unk4 = D_8008F200_8FE00[asset->unk7A4];
        debugEnqueueCallback(9, 7, &func_80035408_36008, arg0);
    }
}

void func_80033088_33C88(func_80033088_33C88_arg *arg0) {
    arg0->unk8 = freeNodeMemory(arg0->unk8);
}

void func_800330B4_33CB4(func_800330B4_33CB4_arg *arg0) {
    s32 i;
    void *asset = dmaRequestAndUpdateStateWithSize(&_419C60_ROM_START, &_419C60_ROM_END, 0x1548);
    setCleanupCallback(&func_800331CC_33DCC);

    for (i = 0; i < 4; i++) {
        func_800330B4_33CB4_arg *j = &arg0[i];

        if (i % 2 != 0) {
            j->unk0 = -0x80;
        } else {
            j->unk0 = 0;
        }

        j->unk2 = (s16)(((i / 2) * 0x10) - 0x66);
        j->unk4 = asset;
        j->unk8 = i;
        j->unkA = 0xFF;
        j->unkD = 0;
        j->unkC = 1;
    }

    setCallback(&func_8003316C_33D6C);
}

void func_8003316C_33D6C(func_800330B4_33CB4_arg *arg0) {
    s32 i;

    if (((func_8003316C_33D6C_alloc *)getCurrentAllocation())->unk7A4 != NULL) {
        for (i = 0; i < 4; i++) {
            debugEnqueueCallback(9, 1, &func_80012004_12C04, &arg0[i]);
        }
    }
}

void func_800331CC_33DCC(func_800330B4_33CB4_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}
