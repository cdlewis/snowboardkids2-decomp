#include "10AD0.h"
#include "20F0.h"
#include "38C90.h"
#include "5E590.h"
#include "6E840.h"
#include "D_800AFE8C_A71FC_type.h"
#include "EepromSaveData_type.h"
#include "common.h"
#include "displaylist.h"
#include "gamestate.h"
#include "geometry.h"
#include "overlay.h"
#include "task_scheduler.h"

USE_ASSET(_458E30);
USE_ASSET(_459310);
USE_ASSET(_4237C0);

typedef struct {
    u8 padding[0x24];
    void *unk24;
    void *unk28;
    void *unk2C;
} func_8002494C_arg;

typedef struct {
    u16 unk0;
    u16 unk2;
    u8 padding[0x28];
    void *unk2C;
} func_80027A28_28628_arg;

typedef struct {
    void *unk0;
} func_80025280_25E80_arg;

typedef struct {
    SceneModel *unk0;
    applyTransformToModel_arg1 unk4;
    u8 unk28;
} func_80024C8C_2588C_arg;

typedef struct {
    SceneModel *unk0;
    applyTransformToModel_arg1 unk4;
    u8 unk28;
} func_80025130_25D30_arg;

typedef struct {
    u8 padding[0x20];
    void *unk20;
    void *unk24;
    void *unk28;
    void *unk2C;
    u8 padding2[0x72];
    u8 unkA2;
} func_80024220_24E20_arg;

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
} func_80025FFC_26BFC_arg;

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
    s16 unkA;
    u8 unkC;
    u8 unkD;
    u8 paddingE[2];
} func_80025C64_entry;

typedef struct {
    func_80025C64_entry entries[3];
    u8 unk30[3];
    u8 unk33;
    u8 unk34;
} func_80025C64_arg;

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    u8 unk12;
    u8 unk13;
} func_800255A0_entry;

typedef struct {
    func_800255A0_entry entries[3];
    u8 padding[0x16];
    u8 unk52;
} func_800255A0_arg;

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    u8 unk12;
    u8 unk13;
    u8 unk14;
} func_80027AD8_286D8_arg;

typedef struct {
    u8 padding[0x24];
    u8 unk24;
} func_800272FC_27EFC_arg;

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
    u8 unkA;
} func_80027348_entry;

typedef struct {
    func_80027348_entry entries[3];
    u8 unk24;
} func_80025824_arg;

typedef struct {
    u8 padding[0x53];
    u8 unk53;
} func_800253E0_25FE0_arg;

typedef struct {
    u8 pad0[0x34];
    u8 unk34;
    u8 unk35;
    u8 unk36;
    u8 pad37;
    u8 unk38;
    u8 unk39;
    u8 unk3A;
} func_80027BC8_arg;

typedef struct {
    u8 pad0[0x3C];
    Mat3x3Padded unk3C;
    Mat3x3Padded unk5C;
    Mat3x3Padded unk7C;
    s32 unk9C;
    u8 padA0;
    u8 unkA1;
} func_80024518_arg;

typedef struct {
    u8 pad0[0x20];
    void *unk20;
    void *unk24;
    void *unk28;
    void *unk2C;
    u8 pad30[0xC];
    Mat3x3Padded unk3C;
    Mat3x3Padded unk5C;
    Mat3x3Padded unk7C;
    u8 pad9C[0x5];
    u8 unkA1;
} func_80024644_arg;

typedef struct {
    u8 pad0[0x20];
    void *unk20;
    void *unk24;
    void *unk28;
    void *unk2C;
    u8 pad30[4];
    u8 unk34;
    u8 unk35;
    u8 unk36;
    u8 pad37;
    u8 unk38;
    u8 unk39;
    u8 unk3A;
    u8 pad3B;
    Mat3x3Padded unk3C;
    Mat3x3Padded unk5C;
    Mat3x3Padded unk7C;
    s32 unk9C;
    u8 unkA0;
    u8 unkA1;
    u8 unkA2;
} func_80024048_arg;

extern s32 identityMatrix[];
extern s32 D_8008DD2C_8E92C[];
extern u16 D_8008DD4E_8E94E[][3];
extern u16 D_8008DD6C_8E96C;
extern u16 D_8008DD6E_8E96E;
extern u16 D_8008DD70_8E970;
extern u16 D_8008DDD8_8E9D8[];
extern u16 D_8008DDDA_8E9DA[];
extern u32 D_41A1D0;
extern u32 D_41AD80;

void func_80025DAC_269AC(func_80025FFC_26BFC_arg *);
void func_80025FFC_26BFC(func_80025FFC_26BFC_arg *);

void func_80024048_24C48(func_80024048_arg *);
void func_80024220_24E20(func_80024220_24E20_arg *);
void func_80024298_24E98(func_80024048_arg *);
void func_80024600_25200(func_8002494C_arg *);
void func_80027BC8_287C8(func_80027BC8_arg *, u8);

void func_80023E30_24A30(func_80024048_arg *arg0) {
    Mat3x3Padded sp10;
    Mat3x3Padded sp30;
    Mat3x3Padded *sp10Ptr;
    s32 temp_s1;
    s32 var_v0;
    u8 temp_s0;
    Mat3x3Padded *temp_s3;
    Mat3x3Padded *temp_s4;
    u8 *temp_s5;
    u8 *temp_ptr;

    temp_s5 = (u8 *)getCurrentAllocation();
    temp_s4 = &arg0->unk5C;
    memcpy(temp_s4, identityMatrix, 0x20);
    temp_s3 = &arg0->unk3C;
    memcpy(temp_s3, temp_s4, 0x20);
    memcpy(&sp30, temp_s3, 0x20);
    sp10Ptr = &sp10;
    memcpy(sp10Ptr, &sp30, 0x20);
    createRotationMatrixYX(sp10Ptr, 0x1000, 0x800);
    createZRotationMatrix(&sp30, 0x1F00);
    func_8006B084_6BC84(sp10Ptr, &sp30, temp_s3);

    var_v0 = -(approximateSin(0x1800) * 0x1600);
    if (var_v0 < 0) {
        var_v0 += 0x1FFF;
    }
    arg0->unk5C.unk14 = (var_v0 >> 0xD) << 8;

    var_v0 = -(approximateCos(0x1800) * 0x1600);
    if (var_v0 < 0) {
        var_v0 += 0x1FFF;
    }
    arg0->unk5C.unk1C = (var_v0 >> 0xD) << 8;
    memcpy(&sp10.unk14, &arg0->unk5C.unk14, 0xC);

    func_8006B084_6BC84(temp_s3, temp_s4, sp10Ptr);
    func_8006B084_6BC84(sp10Ptr, temp_s5 + ((arg0->unkA1 << 5) + 0x17F8), arg0);

    temp_ptr = (u8 *)D_800AFE8C_A71FC;
    temp_s0 = *(temp_ptr + arg0->unkA1 + 0xD);
    arg0->unkA2 = temp_s0;
    temp_s1 = EepromSaveData->character_or_settings[temp_s0] - 1;

    arg0->unk20 = loadAssetByIndex_95728(temp_s0);
    arg0->unk24 = loadAssetByIndex_95500(temp_s0);
    arg0->unk28 = loadAssetByIndex_95590(temp_s0);
    arg0->unk2C = loadAssetByIndex_95668(temp_s1 & 0xFF);

    arg0->unk34 = 0xFF;
    arg0->unk35 = 0xFF;
    arg0->unk36 = 0xFF;
    arg0->unk38 = 0x80;
    arg0->unk39 = 0x80;
    arg0->unk3A = 0x80;
    arg0->unkA0 = *(temp_s5 + arg0->unkA1 + 0x18C8);

    setCleanupCallback(func_80024600_25200);
    setCallback(func_80024048_24C48);
}

void func_80024048_24C48(func_80024048_arg *arg0) {
    Mat3x3Padded sp10;
    GameState *state;
    u8 prevA0;
    u8 newA0;
    u8 charIndex;
    u8 paletteIndex;
    u8 assetIndex;
    u16 rotation;
    u16 val;

    state = (GameState *)getCurrentAllocation();

    newA0 = state->unk18C8[arg0->unkA1];
    prevA0 = arg0->unkA0;

    if (prevA0 != newA0) {
        arg0->unkA0 = newA0;
        func_80027BC8_287C8((func_80027BC8_arg *)arg0, arg0->unkA1);
    }

    charIndex = state->unk18A8[arg0->unkA1];
    paletteIndex = state->unk18B0[arg0->unkA1];
    assetIndex = paletteIndex + charIndex * 3;

    memcpy(&sp10, identityMatrix, sizeof(Mat3x3Padded));
    memcpy(&sp10.unk14, &arg0->unk5C.unk14, 0xC);

    if (state->unk18D2[arg0->unkA1] == state->unk18CC - 1) {
        val = state->unk1898[arg0->unkA1];
        if (val != 1) {
            rotation = state->unk1888[arg0->unkA1];
            createYRotationMatrix(&arg0->unk5C, rotation);
            goto after_rotation;
        }
    }

    rotation = state->unk1880[arg0->unkA1];
    createYRotationMatrix(&arg0->unk5C, (0x2000 - rotation) & 0xFFFF);

after_rotation:
    func_8006B084_6BC84(&arg0->unk3C, &arg0->unk5C, &sp10);
    func_8006B084_6BC84(&sp10, &state->unk17F8[arg0->unkA1], arg0);

    val = state->unk1898[arg0->unkA1];
    if (val == 4 || val == 9) {
        arg0->unk24 = freeNodeMemory(arg0->unk24);
        arg0->unk28 = freeNodeMemory(arg0->unk28);
        arg0->unk2C = freeNodeMemory(arg0->unk2C);
        setCallback(func_80024298_24E98);
    } else {
        if (assetIndex != arg0->unkA2) {
            arg0->unkA2 = assetIndex;
            arg0->unk24 = freeNodeMemory(arg0->unk24);
            arg0->unk28 = freeNodeMemory(arg0->unk28);
            arg0->unk2C = freeNodeMemory(arg0->unk2C);
            setCallback(func_80024220_24E20);
        } else {
            func_800650B4_65CB4(arg0->unkA1, (DisplayListObject *)arg0);
        }
    }
}

void func_80024220_24E20(func_80024220_24E20_arg *arg0) {
    u8 charIndex;

    charIndex = EepromSaveData->character_or_settings[arg0->unkA2] - 1;

    arg0->unk20 = loadAssetByIndex_95728(arg0->unkA2);
    arg0->unk24 = loadAssetByIndex_95500(arg0->unkA2);
    arg0->unk28 = loadAssetByIndex_95590(arg0->unkA2);
    arg0->unk2C = loadAssetByIndex_95668(charIndex);

    setCallback(func_80024048_24C48);
}

void func_80024518_25118(func_80024518_arg *);
void func_80024414_25014(func_80024518_arg *);

void func_80024298_24E98(func_80024048_arg *arg0) {
    Mat3x3Padded sp10;
    Mat3x3Padded *localPtr;
    u8 *base;
    GameState *state;
    Mat3x3Padded *unk7CPtr;
    Mat3x3Padded *unk5CPtr;
    s32 tableValue;
    u8 charIndex;
    u8 assetIndex;
    s32 paletteValue;
    u16 rotation;
    s32 offset;

    base = (u8 *)getCurrentAllocation();
    state = (GameState *)base;

    unk7CPtr = &arg0->unk7C;
    offset = arg0->unkA1 << 5;
    memcpy(unk7CPtr, (u8 *)(offset + (s32)base + 0x17F8), 0x20);

    tableValue = D_8008DD2C_8E92C[(D_800AFE8C_A71FC->unk8 * 2) + state->unk18C0[arg0->unkA1]];
    arg0->unk7C.unk14 = tableValue;
    arg0->unk9C = tableValue;

    charIndex = state->unk18A8[arg0->unkA1];
    assetIndex = state->unk18B0[arg0->unkA1];
    assetIndex += charIndex * 3;
    arg0->unkA2 = assetIndex;

    paletteValue = EepromSaveData->character_or_settings[assetIndex] - 1;
    assetIndex &= 0xFF;

    arg0->unk20 = loadAssetByIndex_95728(assetIndex);
    arg0->unk24 = loadAssetByIndex_95500(assetIndex);
    arg0->unk28 = loadAssetByIndex_95590(assetIndex);
    arg0->unk2C = loadAssetByIndex_95668((u8)paletteValue);

    localPtr = &sp10;
    memcpy(localPtr, identityMatrix, 0x20);
    memcpy(&sp10.unk14, &arg0->unk5C.unk14, 0xC);

    unk5CPtr = &arg0->unk5C;
    rotation = state->unk1888[arg0->unkA1];
    createYRotationMatrix(unk5CPtr, rotation);

    func_8006B084_6BC84(&arg0->unk3C, unk5CPtr, localPtr);
    func_8006B084_6BC84(localPtr, unk7CPtr, arg0);

    setCallback(func_80024414_25014);
}

void func_80024414_25014(func_80024518_arg *arg0) {
    Mat3x3Padded sp10;
    GameState *state;
    Mat3x3Padded *localPtr;
    s32 adjustment;
    u16 rotation;

    state = (GameState *)getCurrentAllocation();

    localPtr = &sp10;

    adjustment = (-(0 < arg0->unk9C) & 0xFFF00000) | 0x100000;

    memcpy(localPtr, identityMatrix, sizeof(Mat3x3Padded));
    memcpy(&sp10.unk14, &arg0->unk5C.unk14, 0xC);

    arg0->unk7C.unk14 = arg0->unk7C.unk14 + adjustment;

    rotation = state->unk1888[arg0->unkA1];
    createYRotationMatrix(&arg0->unk5C, rotation);

    func_8006B084_6BC84(&arg0->unk3C, &arg0->unk5C, localPtr);
    func_8006B084_6BC84(localPtr, &arg0->unk7C, arg0);

    func_800650B4_65CB4(arg0->unkA1, (DisplayListObject *)arg0);

    if (arg0->unk7C.unk14 == 0) {
        state->unk18C0[arg0->unkA1 + 4]++;
        setCallbackWithContinue(func_80024518_25118);
    }
}

void func_80024518_25118(func_80024518_arg *arg0) {
    Mat3x3Padded localMatrix;
    Mat3x3Padded *localPtr;
    GameState *base;
    u16 rotation;
    u16 val;

    base = (GameState *)getCurrentAllocation();

    localPtr = &localMatrix;
    memcpy(localPtr, identityMatrix, sizeof(Mat3x3Padded));
    memcpy(&localMatrix.unk14, &arg0->unk5C.unk14, 0xC);

    rotation = base->unk1888[arg0->unkA1];
    createYRotationMatrix(&arg0->unk5C, rotation);

    func_8006B084_6BC84(&arg0->unk3C, &arg0->unk5C, localPtr);
    func_8006B084_6BC84(localPtr, &arg0->unk7C, arg0);

    func_800650B4_65CB4(arg0->unkA1, (DisplayListObject *)arg0);

    val = base->unk1898[arg0->unkA1];
    if (val != 4 && val != 9) {
        setCallback(func_80024048_24C48);
    }
}

void func_80024600_25200(func_8002494C_arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
    arg0->unk2C = freeNodeMemory(arg0->unk2C);
}

void func_80024810_25410(func_80024644_arg *);
void func_8002494C_2554C(func_8002494C_arg *);

void func_80024644_25244(func_80024644_arg *arg0) {
    Mat3x3Padded sp10;
    Mat3x3Padded sp30;
    Mat3x3Padded sp50;
    GameState *state;
    Mat3x3Padded *unk5CPtr;
    Mat3x3Padded *unk3CPtr;
    Mat3x3Padded *sp50Ptr;
    Mat3x3Padded *sp30Ptr;
    Mat3x3Padded *sp10Ptr;
    Mat3x3Padded *unk7CPtr;
    u8 charIndex;
    u8 assetIndex;
    u8 paletteIndex;
    u32 offset;

    state = (GameState *)getCurrentAllocation();

    sp50Ptr = &sp50;
    memcpy(sp50Ptr, identityMatrix, 0x20);
    unk5CPtr = &arg0->unk5C;
    memcpy(unk5CPtr, sp50Ptr, 0x20);
    unk3CPtr = &arg0->unk3C;
    memcpy(unk3CPtr, unk5CPtr, 0x20);
    sp30Ptr = &sp30;
    memcpy(sp30Ptr, unk3CPtr, 0x20);
    sp10Ptr = &sp10;
    memcpy(sp10Ptr, sp30Ptr, 0x20);

    offset = arg0->unkA1 << 5;
    unk7CPtr = &arg0->unk7C;
    memcpy(unk7CPtr, (void *)(offset + (u32)state + 0x17F8), 0x20);

    createRotationMatrixYX(sp10Ptr, 0x1000, 0x800);
    createZRotationMatrix(sp30Ptr, 0x1F00);
    func_8006B084_6BC84(sp10Ptr, sp30Ptr, unk3CPtr);

    createYRotationMatrix(unk5CPtr, state->unk1888[arg0->unkA1]);
    func_8006B084_6BC84(unk3CPtr, unk5CPtr, sp50Ptr);
    func_8006B084_6BC84(sp50Ptr, unk7CPtr, arg0);

    charIndex = state->unk18A8[arg0->unkA1 + 4];
    assetIndex = state->unk18B0[arg0->unkA1 + 4];
    assetIndex = assetIndex + charIndex * 3;
    paletteIndex = EepromSaveData->character_or_settings[assetIndex] - 1;

    arg0->unk20 = loadAssetByIndex_95728(assetIndex);
    arg0->unk24 = loadAssetByIndex_95500(assetIndex);
    arg0->unk28 = loadAssetByIndex_95590(assetIndex);
    arg0->unk2C = loadAssetByIndex_95668(paletteIndex);

    setCleanupCallback(func_8002494C_2554C);
    setCallbackWithContinue(func_80024810_25410);
}

INCLUDE_ASM("asm/nonmatchings/24A30", func_80024810_25410);

void func_8002494C_2554C(func_8002494C_arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
    arg0->unk2C = freeNodeMemory(arg0->unk2C);
}

INCLUDE_ASM("asm/nonmatchings/24A30", func_80024990_25590);

void func_80024BA0_257A0(func_80024C8C_2588C_arg *);

void func_80024AAC_256AC(func_80024C8C_2588C_arg *arg0) {
    Mat3x3Padded localMatrix;
    Mat3x3Padded *localPtr;
    GameState *state;
    Mat3x3Padded *matrix;
    u16 rotation;

    state = (GameState *)getCurrentAllocation();

    localPtr = &localMatrix;
    memcpy(localPtr, identityMatrix, sizeof(Mat3x3Padded));

    matrix = (Mat3x3Padded *)&arg0->unk4;
    rotation = state->unk1880[arg0->unk28];
    createYRotationMatrix(matrix, 0x2000 - rotation);

    func_8006B084_6BC84(matrix, &state->unk17F8[arg0->unk28], localPtr);

    applyTransformToModel(arg0->unk0, (applyTransformToModel_arg1 *)localPtr);

    if (D_800AFE8C_A71FC->unk9[arg0->unk28] == 7) {
        func_800021B8_2DB8(arg0->unk0, 4);
    } else {
        func_800021B8_2DB8(arg0->unk0, 0x90);
    }

    updateModelGeometry(arg0->unk0);
    setCallback(func_80024BA0_257A0);
}

void func_80024D40_25940(func_80024C8C_2588C_arg *);
void func_80024DCC_259CC(func_80024C8C_2588C_arg *);
void func_80024E58_25A58(func_80024C8C_2588C_arg *);
void func_80024FEC_25BEC(func_80024C8C_2588C_arg *);
void func_80024F48_25B48(func_80024C8C_2588C_arg *);

void func_80024BA0_257A0(func_80024C8C_2588C_arg *arg0) {
    Mat3x3Padded localMatrix;
    Mat3x3Padded *localPtr;
    u8 *base;
    Mat3x3Padded *matrix;
    u16 rotation;
    u16 val;

    base = (u8 *)getCurrentAllocation();

    localPtr = &localMatrix;
    memcpy(localPtr, identityMatrix, sizeof(Mat3x3Padded));

    matrix = (Mat3x3Padded *)&arg0->unk4;
    rotation = *(u16 *)(base + arg0->unk28 * 2 + 0x1880);
    createYRotationMatrix(matrix, 0x2000 - rotation);

    func_8006B084_6BC84(matrix, base + (arg0->unk28 * 32 + 0x17F8), localPtr);

    applyTransformToModel(arg0->unk0, (applyTransformToModel_arg1 *)localPtr);

    clearModelRotation(arg0->unk0);
    updateModelGeometry(arg0->unk0);

    val = *(u16 *)(base + arg0->unk28 * 2 + 0x1898);
    if (val == 0x10) {
        func_80002014_2C14(arg0->unk0);
        setCallback(func_80024DCC_259CC);
    }
}

void func_80024C8C_2588C(func_80024C8C_2588C_arg *arg0) {
    u16 *base;
    u16 val;

    base = (u16 *)getCurrentAllocation();

    clearModelRotation(arg0->unk0);
    updateModelGeometry(arg0->unk0);

    val = *(base + arg0->unk28 + (0x1898 / 2));

    if (val == 0x10) {
        func_80002014_2C14(arg0->unk0);
        setCallback(func_80024DCC_259CC);
    } else if (val == 0) {
        arg0->unk4.unk14 = 0xFFEA0000;
        func_80002014_2C14(arg0->unk0);
        setCallback(func_80024D40_25940);
    } else if (val == 0x11) {
        arg0->unk4.unk14 = 0xFFEA0000;
        setCallback(func_80024BA0_257A0);
    }
}

INCLUDE_ASM("asm/nonmatchings/24A30", func_80024D40_25940);

INCLUDE_ASM("asm/nonmatchings/24A30", func_80024DCC_259CC);

void func_80024E58_25A58(func_80024C8C_2588C_arg *arg0) {
    u8 *base;
    u8 *ptr;
    u8 index;

    base = (u8 *)getCurrentAllocation();

    memcpy(&arg0->unk4, identityMatrix, 0x20);

    index = arg0->unk28;
    arg0->unk4.unk14 = D_8008DD2C_8E92C[D_800AFE8C_A71FC->unk8 * 2 + (base + index)[0x18C0]];
    arg0->unk4.unk1C = 0;
    arg0->unk4.unk18 = 0xFFF00000;
    arg0->unk4.unk20_u.unk20_s32 = arg0->unk4.unk14;

    applyTransformToModel(arg0->unk0, &arg0->unk4);

    ptr = (u8 *)D_800AFE8C_A71FC;
    ptr = ptr + arg0->unk28;
    if (ptr[9] == 7) {
        func_800021B8_2DB8(arg0->unk0, 4);
    } else {
        func_800021B8_2DB8(arg0->unk0, 0x90);
    }

    updateModelGeometry(arg0->unk0);
    setCallback(func_80024F48_25B48);
}

void func_80024F48_25B48(func_80024C8C_2588C_arg *arg0) {
    u8 *base;
    s32 unk24;
    s32 adjustment;
    u8 *ptr;

    base = (u8 *)getCurrentAllocation();

    unk24 = arg0->unk4.unk20_u.unk20_s32;

    adjustment = (-(0 < unk24) & 0xFFF00000) | 0x100000;

    arg0->unk4.unk14 = arg0->unk4.unk14 + adjustment;

    applyTransformToModel(arg0->unk0, &arg0->unk4);
    clearModelRotation(arg0->unk0);
    updateModelGeometry(arg0->unk0);

    if (arg0->unk4.unk14 == 0) {
        ptr = base + arg0->unk28;
        ptr[0x18C4]++;
        setCallbackWithContinue(func_80024FEC_25BEC);
    }
}

void func_80024FEC_25BEC(func_80024C8C_2588C_arg *arg0) {
    u16 *base;
    u16 val;

    base = (u16 *)getCurrentAllocation();

    clearModelRotation(arg0->unk0);
    updateModelGeometry(arg0->unk0);

    val = *(base + arg0->unk28 + (0x1898 / 2));

    if (val != 0x10) {
        setCallback(func_80024C8C_2588C);
    }
}

void func_80025058_25C58(func_80025280_25E80_arg *arg0) {
    func_80002014_2C14(arg0->unk0);
}

INCLUDE_ASM("asm/nonmatchings/24A30", func_80025074_25C74);

void func_800251AC_25DAC(func_80025130_25D30_arg *);

void func_80025130_25D30(func_80025130_25D30_arg *arg0) {
    u8 *base;

    getCurrentAllocation();

    applyTransformToModel(arg0->unk0, &arg0->unk4);

    base = (u8 *)D_800AFE8C_A71FC;
    if (*(base + arg0->unk28 + 9) == 7) {
        func_800021B8_2DB8(arg0->unk0, 4);
    } else {
        func_800021B8_2DB8(arg0->unk0, 0x90);
    }

    updateModelGeometry(arg0->unk0);
    setCallback(&func_800251AC_25DAC);
}

void func_800251AC_25DAC(func_80025130_25D30_arg *arg0) {
    u8 *base;
    s32 target;
    s32 adj2 = 0x100000;
    s32 adj1 = 0xFFF00000;

    base = (u8 *)getCurrentAllocation();

    target = D_8008DD2C_8E92C[D_800AFE8C_A71FC->unk8 * 2 + (((base + arg0->unk28)[0x18C0] + 1) & 1)];

    arg0->unk4.unk14 += ((target >> 31) & adj1) | adj2;

    applyTransformToModel(arg0->unk0, &arg0->unk4);
    clearModelRotation(arg0->unk0);
    updateModelGeometry(arg0->unk0);

    if (arg0->unk4.unk14 == target) {
        (base + arg0->unk28)[0x18C4]++;
        func_80069CF8_6A8F8();
    }
}

void *func_80025280_25E80(func_80025280_25E80_arg *arg0) {
    return func_80002014_2C14(arg0->unk0);
}

INCLUDE_ASM("asm/nonmatchings/24A30", func_8002529C_25E9C);

void func_80025418_26018(void *);

void func_800253E0_25FE0(func_800253E0_25FE0_arg *arg0) {
    if (--arg0->unk53 == 0) {
        setCallback(func_80025418_26018);
    }
}

INCLUDE_ASM("asm/nonmatchings/24A30", func_80025418_26018);

extern u8 D_8008DD8C_8E98C[];
extern u16 D_8008DE02_8EA02[];

void func_800255A0_261A0(func_800255A0_arg *arg0) {
    u8 *base;
    s32 i;
    func_800255A0_entry *ptr;
    u8 idx;
    u8 val1, val2;

    base = (u8 *)getCurrentAllocation();
    i = 0;

    do {
        val1 = (base + arg0->unk52)[0x18A8];
        val2 = (base + arg0->unk52)[0x18B0];
        idx = D_8008DD8C_8E98C[((u8)(val2 + val1 * 3)) * 3 + i];
        ptr = &((func_800255A0_entry *)arg0)[i];
        ptr->unkE = D_8008DE02_8EA02[idx];
        debugEnqueueCallback(arg0->unk52 + 8, 0, func_80010C98_11898, ptr);
        i++;
    } while (i < 3);
}

void func_8002567C_2627C(func_80025FFC_26BFC_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

INCLUDE_ASM("asm/nonmatchings/24A30", func_800256A8_262A8);

void func_80025904_26504(void);

void func_80025824_26424(func_80025824_arg *arg0) {
    func_80027348_entry *ptr;
    GameState *base;
    s32 i;
    s16 val;
    u8 constant;
    func_80027348_entry *entryPtr;

    base = (GameState *)getCurrentAllocation();

    i = 0;
    ptr = (func_80027348_entry *)arg0;
    do {
        debugEnqueueCallback(arg0->unk24 + 8, 0, func_80010240_10E40, ptr);
        ptr++;
        i++;
    } while (i < 3);

    if (base->unk1898[arg0->unk24] == 3) {
        val = 0xD;
        if (D_800AFE8C_A71FC->unk8 == 1) {
            val = 0x12;
        }

        i = 0;
        constant = 8;
        entryPtr = arg0->entries;
        do {
            ((volatile func_80027348_entry *)entryPtr)->unk8 = val;
            ((volatile func_80027348_entry *)entryPtr)->unkA = constant;
            entryPtr++;
            i++;
        } while (i < 3);

        setCallback(func_80025904_26504);
    }
}

INCLUDE_ASM("asm/nonmatchings/24A30", func_80025904_26504);

typedef struct {
    u8 padding[0x24];
    u8 unk24;
} func_80025A88_26688_arg;

void func_80025A88_26688(func_80025A88_26688_arg *arg0) {
    GameState *base;
    s32 i;
    s32 s2;
    s32 idx;
    func_80027348_entry *ptr;
    u8 val1, val2;
    u8 *tableBase;
    u8 *tablePtr;

    base = (GameState *)getCurrentAllocation();

    s2 = 0xC;
    if (D_800AFE8C_A71FC->unk8 == 1) {
        s2 = 0x11;
    }

    val1 = base->unk18A8[arg0->unk24];
    i = 0;
    tableBase = D_8008DD8C_8E98C;
    val2 = base->unk18B0[arg0->unk24];
    ptr = (func_80027348_entry *)arg0;
    idx = ((u8)(val2 + val1 * 3)) * 3;

loop:
    tablePtr = (u8 *)((idx + i) + (u32)tableBase);
    ptr->unk8 = s2 + (*tablePtr - 1) / 2;
    i++;
    ptr->unkA = (u8)(((*tablePtr - 1) / 2 + 7) & 0xFF) % 11;
    debugEnqueueCallback(arg0->unk24 + 8, 0, func_80010240_10E40, ptr);
    ptr++;
    if (i < 3)
        goto loop;

    if (base->unk1898[arg0->unk24] == 3) {
        s32 constant;
        u8 *a0;

        s2 = 0xD;
        if (D_800AFE8C_A71FC->unk8 == 1) {
            s2 = 0x12;
        }

        i = 0;
        constant = 8;
        a0 = (u8 *)arg0;
        do {
            ((volatile s16 *)a0)[4] = s2;
            ((volatile u8 *)a0)[0xA] = constant;
            a0 += 0xC;
            i++;
        } while (i < 3);

        setCallback(func_80025904_26504);
    }
}

void func_80025C38_26838(func_80025FFC_26BFC_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_80025C64_26864(func_80025C64_arg *arg0) {
    void *dmaResult;
    u16 x;
    u16 y;
    u16 xIncrement;
    s32 unk8Base;
    s32 count;
    s32 i;
    s32 incrementSigned;
    s32 pad[6];

    (void)pad;

    dmaResult = dmaRequestAndUpdateStateWithSize(&_4237C0_ROM_START, &_4237C0_ROM_END, 0x8A08);
    setCleanupCallback(func_80025FFC_26BFC);

    unk8Base = 8;
    if (D_800AFE8C_A71FC->unk8 == 1) {
        unk8Base = 5;
    }

    arg0->unk33 = 3;

    if (D_800AFE8C_A71FC->unk4 == 0) {
        x = D_8008DD6C_8E96C;
        y = D_8008DD6E_8E96E;
        xIncrement = D_8008DD70_8E970;
        unk8Base = 6;
        arg0->unk33 = 2;
    } else {
        s32 index = D_800AFE8C_A71FC->unk8;
        x = D_8008DD4E_8E94E[index][0];
        y = D_8008DD4E_8E94E[index][1];
        xIncrement = D_8008DD4E_8E94E[index][2];
    }

    count = arg0->unk33;
    if (count > 0) {
        i = 0;
        incrementSigned = (s16)xIncrement;
        do {
            arg0->entries[i].unk0 = x;
            arg0->entries[i].unk2 = y;
            arg0->entries[i].unk8 = unk8Base + i;
            arg0->entries[i].unkD = 0;
            arg0->entries[i].unkC = 0;
            arg0->entries[i].unkA = 0xFF;
            arg0->entries[i].unk4 = dmaResult;
            arg0->unk30[i] = 0;
            i++;
            x += incrementSigned;
        } while (i < (s32)arg0->unk33);
    }

    setCallback(func_80025DAC_269AC);
}

INCLUDE_ASM("asm/nonmatchings/24A30", func_80025DAC_269AC);

INCLUDE_ASM("asm/nonmatchings/24A30", func_80025EE4_26AE4);

void func_80025FFC_26BFC(func_80025FFC_26BFC_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

INCLUDE_ASM("asm/nonmatchings/24A30", func_80026028_26C28);

typedef struct {
    func_80027348_entry entries[3];
} func_800260EC_26CEC_arg;

void func_800260EC_26CEC(func_800260EC_26CEC_arg *arg0) {
    s32 i;
    s32 j;
    u16 index;
    func_80027348_entry *ptr;

    i = 0;
    if (i < (D_800AFE8C_A71FC)->unk8) {
        j = 0;
        do {
            index = i + 8;
            ptr = arg0->entries;
            do {
                debugEnqueueCallback(index, 7, &func_8000FED0_10AD0, ptr);
                j++;
                ptr++;
            } while (j < 3);
            i++;
            j = 0;
        } while (i < (D_800AFE8C_A71FC)->unk8);
    }
}

void func_80026190_26D90(func_80025FFC_26BFC_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

INCLUDE_ASM("asm/nonmatchings/24A30", func_800261BC_26DBC);

INCLUDE_ASM("asm/nonmatchings/24A30", func_800262D4_26ED4);

void func_80026538_27138(func_80025FFC_26BFC_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

INCLUDE_ASM("asm/nonmatchings/24A30", func_80026564_27164);

INCLUDE_ASM("asm/nonmatchings/24A30", func_8002667C_2727C);

void func_80026834_27434(func_80025FFC_26BFC_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

INCLUDE_ASM("asm/nonmatchings/24A30", func_80026860_27460);

INCLUDE_ASM("asm/nonmatchings/24A30", func_800269C8_275C8);

void func_80026BAC_277AC(func_80025FFC_26BFC_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

INCLUDE_ASM("asm/nonmatchings/24A30", func_80026BD8_277D8);

INCLUDE_ASM("asm/nonmatchings/24A30", func_80026D34_27934);

void func_80026FC8_27BC8(func_80025FFC_26BFC_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_800270B8_27CB8(u8 *);
void func_8002712C_27D2C(func_80025FFC_26BFC_arg *);

void func_80026FF4_27BF4(func_80025FFC_26BFC_arg *arg0) {
    void *dmaResult;
    D_800AFE8C_A71FC_type *global;
    u8 count;
    s32 i;
    s32 unk8Base;
    u16 x;
    u16 y;
    volatile func_80025FFC_26BFC_arg *ptr;
    s32 pad[4];

    (void)pad;

    dmaResult = dmaRequestAndUpdateStateWithSize(&D_41A1D0, &D_41AD80, 0x1B48);
    setCleanupCallback(func_8002712C_27D2C);

    global = D_800AFE8C_A71FC;
    count = global->unk8;
    unk8Base = (count >= 3) * 8;
    x = D_8008DDD8_8E9D8[count * 2];
    y = D_8008DDDA_8E9DA[count * 2];

    i = 0;
    if (count != 0) {
        ptr = arg0;
        do {
            ptr->unk0 = x;
            ptr->unk2 = y;
            ptr->unk8 = unk8Base + i;
            ptr->unk4 = dmaResult;
            ptr++;
            i++;
        } while (i < global->unk8);
    }

    setCallback(func_800270B8_27CB8);
}

void func_800270B8_27CB8(u8 *arg0) {
    s32 i;
    s32 pad[2];
    u8 *ptr;
    s32 count;

    count = D_800AFE8C_A71FC->unk8;
    if (count > 0) {
        i = 0;
        ptr = arg0;
        do {
            debugEnqueueCallback((u16)(i + 0xC), 0, func_8000FED0_10AD0, ptr);
            i++;
            ptr += 0xC;
        } while (i < D_800AFE8C_A71FC->unk8);
    }
}

void func_8002712C_27D2C(func_80025FFC_26BFC_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_800271E4_27DE4(func_80025FFC_26BFC_arg *);
void func_8002723C_27E3C(func_80025FFC_26BFC_arg *);

void func_80027158_27D58(func_80025FFC_26BFC_arg *arg0) {
    GameState *state;
    void *dmaResult;

    state = getCurrentAllocation();
    dmaResult = dmaRequestAndUpdateStateWithSize(&_4237C0_ROM_START, &_4237C0_ROM_END, 0x8A08);
    setCleanupCallback(func_8002723C_27E3C);

    arg0->unk0 = -0x20;
    arg0->unk2 = 8;
    arg0->unk8 = state->unk18A8[0] + 0x16;
    arg0->unk4 = dmaResult;

    setCallback(func_800271E4_27DE4);
}

void func_800271E4_27DE4(func_80025FFC_26BFC_arg *arg0) {
    GameState *state = getCurrentAllocation();

    if (state->unk1898[0] == 3) {
        arg0->unk8 = state->unk18A8[0] + 0x16;
        debugEnqueueCallback(0xC, 0, func_8000FED0_10AD0, arg0);
    }
}

void func_8002723C_27E3C(func_80025FFC_26BFC_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_8002764C_2824C(func_80025FFC_26BFC_arg *arg0);
void func_800272FC_27EFC(func_800272FC_27EFC_arg *arg0);

void func_80027268_27E68(func_80025FFC_26BFC_arg *arg0) {
    void *dmaResult;
    s32 loopCount;
    s32 i;

    dmaResult = dmaRequestAndUpdateStateWithSize(&_4237C0_ROM_START, &_4237C0_ROM_END, 0x8A08);

    loopCount = (D_800AFE8C_A71FC->unk8 == 2) ? 3 : 2;

    for (i = 0; i < loopCount; i++) {
        arg0[i].unk4 = dmaResult;
    }

    setCleanupCallback(func_8002764C_2824C);
    setCallback(func_800272FC_27EFC);
}

void func_80027348_27F48(volatile func_80027348_entry *arg0);

void func_800272FC_27EFC(func_800272FC_27EFC_arg *arg0) {
    GameState *state = (GameState *)getCurrentAllocation();

    if (state->unk1898[arg0->unk24] == 0x1A) {
        setCallbackWithContinue(func_80027348_27F48);
    }
}

void func_80027400_28000(void);

void func_80027348_27F48(volatile func_80027348_entry *arg0) {
    s32 xBase;
    s32 yBase;
    s32 xIncrement;
    s32 yIncrement;
    s32 unk8Base;
    s32 count;
    s32 i;
    s32 countMasked;
    s32 xInc2;
    s32 yInc2;
    s32 constant;
    s32 x;
    s32 y;
    s32 *dummyPtr;
    s32 dummy;

    dummyPtr = &dummy;

    xBase = -0x38;
    if (D_800AFE8C_A71FC->unk8 == 2) {
        xBase = -0x58;
        yBase = -0x64;
        xIncrement = 0;
        yIncrement = 0x10;
        unk8Base = 0x27;
        count = 3;
    } else {
        yBase = -0x5C;
        xIncrement = 0x38;
        yIncrement = 0;
        unk8Base = 0x2A;
        count = 2;
    }

    countMasked = count & 0xFF;
    i = 0;
    if (countMasked > 0) {
        xInc2 = xIncrement;
        yInc2 = (s16)yIncrement;
        constant = 0xC;
        y = yBase;
        x = xBase;
        do {
            arg0->unk2 = (s16)y;
            y = y + yInc2;
            arg0->unk0 = (s16)x;
            arg0->unk8 = (s16)(unk8Base + i);
            arg0->unkA = (u8)constant;
            arg0++;
            i++;
            x = x + xInc2;
        } while (i < countMasked);
    }

    setCallback(func_80027400_28000);
}

INCLUDE_ASM("asm/nonmatchings/24A30", func_80027400_28000);

INCLUDE_ASM("asm/nonmatchings/24A30", func_80027544_28144);

void func_8002764C_2824C(func_80025FFC_26BFC_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

INCLUDE_ASM("asm/nonmatchings/24A30", func_80027678_28278);

INCLUDE_ASM("asm/nonmatchings/24A30", func_800277F4_283F4);

void func_800279A8_285A8(func_80025FFC_26BFC_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_80027AAC_286AC(func_80027A28_28628_arg *arg0);
void func_80027A28_28628(func_80027A28_28628_arg *arg0);

void func_800279D4_285D4(func_80027A28_28628_arg *arg0) {
    arg0->unk2C = dmaRequestAndUpdateStateWithSize(&_458E30_ROM_START, &_458E30_ROM_END, 0xAE0);
    setCleanupCallback(func_80027AAC_286AC);
    setCallback(func_80027A28_28628);
}

void func_80027A50_28650(func_80027A28_28628_arg *arg0);

void func_80027A28_28628(func_80027A28_28628_arg *arg0) {
    func_800394BC_3A0BC(arg0, (s32)arg0->unk2C);
    setCallback(func_80027A50_28650);
}

void func_80027A50_28650(func_80027A28_28628_arg *arg0) {
    arg0->unk0++;
    arg0->unk2++;
    arg0->unk0 &= 0x3FF;
    arg0->unk2 &= 0x3FF;
    debugEnqueueCallback(7, 0, func_80038420_39020, arg0);
}

void func_80027AAC_286AC(func_80027A28_28628_arg *arg0) {
    arg0->unk2C = freeNodeMemory(arg0->unk2C);
}

void func_80027B70_28770(void *);
void func_80027B9C_2879C(func_80025FFC_26BFC_arg *);

void func_80027AD8_286D8(func_80027AD8_286D8_arg *arg0) {
    void *dmaResult;

    dmaResult = dmaRequestAndUpdateStateWithSize(&_4237C0_ROM_START, &_4237C0_ROM_END, 0x8A08);
    setCleanupCallback(func_80027B9C_2879C);

    arg0->unk8 = 6;
    arg0->unkA = 0x300;
    arg0->unkC = 0x300;
    arg0->unk10 = 0xFF;
    arg0->unk0 = 0;
    arg0->unk2 = 0;
    arg0->unkE = 0;
    arg0->unk13 = 0;
    arg0->unk12 = 0;
    arg0->unk14 = 0x80;
    arg0->unk4 = dmaResult;

    setCallback(func_80027B70_28770);
}

void func_80027B70_28770(void *arg0) {
    debugEnqueueCallback(0xC, 7, func_80011924_12524, arg0);
}

void func_80027B9C_2879C(func_80025FFC_26BFC_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_80027BC8_287C8(func_80027BC8_arg *arg0, u8 arg1) {
    u8 *allocation;
    u8 byte;
    u8 val;

    allocation = getCurrentAllocation();
    allocation += arg1;
    byte = allocation[0x18C8];

    if (byte == 1) {
        val = 0x50;
        arg0->unk34 = val;
        arg0->unk35 = val;
        arg0->unk36 = val;
    } else if (byte == 0) {
        val = 0xFF;
        arg0->unk34 = val;
        arg0->unk35 = val;
        arg0->unk36 = val;
        val = 0x80;
    } else {
        val = 0xFF;
        arg0->unk34 = val;
        arg0->unk35 = val;
        arg0->unk36 = val;
    }

    arg0->unk38 = val;
    arg0->unk39 = val;
    arg0->unk3A = val;
}

u8 func_80027C44_28844(u8 arg0) {
    return D_8008DD8C_8E98C[arg0 * 3];
}

extern u8 D_8008DD8D_8E98D[];

u8 func_80027C60_28860(u8 arg0) {
    return D_8008DD8D_8E98D[arg0 * 3];
}

extern u8 D_8008DD8E_8E98E[];

u8 func_80027C7C_2887C(u8 arg0) {
    return D_8008DD8E_8E98E[arg0 * 3];
}
