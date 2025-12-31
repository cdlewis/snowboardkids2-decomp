#include "10AD0.h"
#include "19E80.h"
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
USE_ASSET(_41A1D0);

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
    Transform3D unk4;
    union {
        SceneModel *unk20;
        s32 unk20_s32;
        s16 unk20_s16;
    } unk20_u;
    u8 unk28;
} func_80024C8C_2588C_arg;

typedef struct {
    SceneModel *unk0;
    Transform3D unk4;
    union {
        SceneModel *unk20;
        s32 unk20_s32;
        s16 unk20_s16;
    } unk20_u;
    u8 unk28;
} func_80025130_25D30_arg;

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
} func_80025FFC_26BFC_arg;

typedef struct {
    s16 x;
    s16 y;
    void *asset;
    s16 spriteIndex;
    s16 alpha;
    u8 unkC;
    u8 blinkState;
    u8 paddingE[2];
} SelectionEntry;

typedef struct {
    SelectionEntry entries[3];
    u8 unk30[3];
    u8 numEntries;
    u8 playerIndex;
} SelectionMenuState;

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
    func_800255A0_entry entries[3];
    u8 padding[0x14];
    u8 unk50;
    u8 unk51;
    u8 padding2;
    u8 unk53;
} func_8002529C_25E9C_arg;

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
    Transform3D unk3C;
    Transform3D unk5C;
    Transform3D unk7C;
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
    Transform3D unk3C;
    Transform3D unk5C;
    Transform3D unk7C;
    u8 pad9C[0x5];
    u8 unkA1;
} func_80024644_arg;

typedef struct {
    u8 pad0[0x20];
    void *modelAsset;
    void *animationAsset;
    void *skeletonAsset;
    void *paletteAsset;
    u8 pad30[4];
    u8 lightR;
    u8 lightG;
    u8 lightB;
    u8 pad37;
    u8 ambientR;
    u8 ambientG;
    u8 ambientB;
    u8 pad3B;
    Transform3D rotationMatrix;
    Transform3D positionMatrix;
    Transform3D worldMatrix;
    s32 targetX;
    u8 selectionState;
    u8 playerIndex;
    u8 charPaletteIndex;
} CharSelectPreviewModel;

typedef struct {
    SelectionEntry entries[8];
    u8 unk80[4];
} func_80026564_arg;

typedef struct {
    func_80027348_entry entries[3];
    u8 unk24;
} func_80027544_arg;

typedef struct {
    s16 x;
    s16 y;
    void *asset;
    s16 spriteIndex;
    s16 alpha;
    u8 unkC;
    u8 blinkState;
    u8 paddingE[2];
} CharacterNameSprite;

extern struct {
    u16 unk0;
    u16 unk2;
} D_8008DE54_8EA54[];

typedef struct {
    SelectionEntry entries[4];
    s16 unk40;
    s16 unk42;
    void *unk44;
    s16 unk48;
} func_80026BD8_arg;

typedef struct {
    u16 x;
    u16 y;
} Vec2_u16;

typedef struct {
    u16 x;
    u16 y;
    s16 inc;
} PositionConfig_DDBE;

typedef struct {
    u16 y;
    u16 x;
    u16 inc;
} PositionConfig_DDE6;

typedef struct {
    u16 x;
    u16 y;
    u16 inc;
} PositionConfig_DE1A;

typedef struct {
    func_80027348_entry entries[3];
} func_800260EC_26CEC_arg;

extern Vec2_u16 D_8008DDD8_8E9D8[];
extern PositionConfig_DDBE D_8008DDBE_8E9BE[];
extern PositionConfig_DDE6 D_8008DDE6_8E9E6[];
extern u8 D_8008DE18_8EA18[];
extern PositionConfig_DE1A D_8008DE1A_8EA1A[];
extern Vec3s D_8008DE3A_8EA3A[];
extern u16 D_8008DE7A_8EA7A[];
extern struct {
    u16 unk0;
    u16 unk2;
} D_8008DE9C_8EA9C[];
extern u8 D_8008DD8D_8E98D[];
extern u8 D_8008DD8E_8E98E[];
extern s32 identityMatrix[];
extern s32 D_8008DD2C_8E92C[];
extern Vec3s D_8008DD4E_8E94E[];
extern Vec3s D_8008DD6C_8E96C;

void func_80025418_26018(void *);
void func_8002567C_2627C(func_80025FFC_26BFC_arg *);
void func_800253E0_25FE0(func_800253E0_25FE0_arg *);
void func_800251AC_25DAC(func_80025130_25D30_arg *);
void func_80024BA0_257A0(func_80024C8C_2588C_arg *);
void func_80026D34_27934(func_80026BD8_arg *);
void func_80026FC8_27BC8(func_80025FFC_26BFC_arg *);
void func_8002667C_2727C(void *);
void func_80026834_27434(func_80025FFC_26BFC_arg *);
void func_80025DAC_269AC(SelectionMenuState *);
void func_80025FFC_26BFC(func_80025FFC_26BFC_arg *);
void updateCharSelectPreviewModel(CharSelectPreviewModel *);
void reloadCharSelectPreviewAssets(CharSelectPreviewModel *);
void func_80024298_24E98(CharSelectPreviewModel *);
void func_80024600_25200(func_8002494C_arg *);
void func_80027BC8_287C8(func_80027BC8_arg *, u8);
void func_80027400_28000(func_80025824_arg *);
void func_80027544_28144(func_80027544_arg *);
void func_800269C8_275C8(void *);
void func_80026BAC_277AC(func_80025FFC_26BFC_arg *);
void func_80025904_26504(void);
void func_80024810_25410(func_80024644_arg *);
void func_8002494C_2554C(func_8002494C_arg *);
void func_80024D40_25940(func_80024C8C_2588C_arg *);
void func_80024DCC_259CC(func_80024C8C_2588C_arg *);
void func_80024E58_25A58(func_80024C8C_2588C_arg *);
void func_80024FEC_25BEC(func_80024C8C_2588C_arg *);
void func_80024F48_25B48(func_80024C8C_2588C_arg *);
void func_80026190_26D90(func_80025FFC_26BFC_arg *);
void func_800260EC_26CEC(func_800260EC_26CEC_arg *);
void func_800262D4_26ED4(func_80026564_arg *);
void func_80026538_27138(func_80025FFC_26BFC_arg *);
void func_800270B8_27CB8(u8 *);
void func_8002712C_27D2C(func_80025FFC_26BFC_arg *);
void func_800271E4_27DE4(func_80025FFC_26BFC_arg *);
void func_8002723C_27E3C(func_80025FFC_26BFC_arg *);
void func_8002764C_2824C(func_80025FFC_26BFC_arg *arg0);
void func_800272FC_27EFC(func_800272FC_27EFC_arg *arg0);
void func_80027348_27F48(volatile func_80027348_entry *arg0);
void func_80027AAC_286AC(func_80027A28_28628_arg *arg0);
void func_80027A28_28628(func_80027A28_28628_arg *arg0);
void func_80027A50_28650(func_80027A28_28628_arg *arg0);
void func_80027B70_28770(void *);
void func_80027B9C_2879C(func_80025FFC_26BFC_arg *);
void func_80024518_25118(func_80024518_arg *);
void func_80024414_25014(func_80024518_arg *);

void initCharSelectPreviewModel(CharSelectPreviewModel *arg0) {
    Transform3D sp10;
    Transform3D sp30;
    Transform3D *sp10Ptr;
    s32 paletteIndex;
    s32 sinCosResult;
    u8 charIndex;
    Transform3D *rotMatPtr;
    Transform3D *posMatPtr;
    u8 *gameState;
    u8 *globalPtr;

    gameState = (u8 *)getCurrentAllocation();
    posMatPtr = &arg0->positionMatrix;
    memcpy(posMatPtr, identityMatrix, 0x20);
    rotMatPtr = &arg0->rotationMatrix;
    memcpy(rotMatPtr, posMatPtr, 0x20);
    memcpy(&sp30, rotMatPtr, 0x20);
    sp10Ptr = &sp10;
    memcpy(sp10Ptr, &sp30, 0x20);
    createRotationMatrixYX(sp10Ptr, 0x1000, 0x800);
    createZRotationMatrix(&sp30, 0x1F00);
    func_8006B084_6BC84(sp10Ptr, &sp30, rotMatPtr);

    sinCosResult = -(approximateSin(0x1800) * 0x1600);
    if (sinCosResult < 0) {
        sinCosResult += 0x1FFF;
    }
    arg0->positionMatrix.translation.x = (sinCosResult >> 0xD) << 8;

    sinCosResult = -(approximateCos(0x1800) * 0x1600);
    if (sinCosResult < 0) {
        sinCosResult += 0x1FFF;
    }
    arg0->positionMatrix.translation.z = (sinCosResult >> 0xD) << 8;
    memcpy(&sp10.translation, &arg0->positionMatrix.translation.x, 0xC);

    func_8006B084_6BC84(rotMatPtr, posMatPtr, sp10Ptr);
    func_8006B084_6BC84(sp10Ptr, gameState + ((arg0->playerIndex << 5) + 0x17F8), arg0);

    globalPtr = (u8 *)D_800AFE8C_A71FC;
    charIndex = *(globalPtr + arg0->playerIndex + 0xD);
    arg0->charPaletteIndex = charIndex;
    paletteIndex = EepromSaveData->character_or_settings[charIndex] - 1;

    arg0->modelAsset = loadAssetByIndex_95728(charIndex);
    arg0->animationAsset = loadAssetByIndex_95500(charIndex);
    arg0->skeletonAsset = loadAssetByIndex_95590(charIndex);
    arg0->paletteAsset = loadAssetByIndex_95668(paletteIndex & 0xFF);

    arg0->lightR = 0xFF;
    arg0->lightG = 0xFF;
    arg0->lightB = 0xFF;
    arg0->ambientR = 0x80;
    arg0->ambientG = 0x80;
    arg0->ambientB = 0x80;
    arg0->selectionState = *(gameState + arg0->playerIndex + 0x18C8);

    setCleanupCallback(func_80024600_25200);
    setCallback(updateCharSelectPreviewModel);
}

void updateCharSelectPreviewModel(CharSelectPreviewModel *arg0) {
    Transform3D sp10;
    GameState *state;
    u8 prevSelState;
    u8 newSelState;
    u8 charIndex;
    u8 paletteIndex;
    u8 assetIndex;
    u16 rotation;
    u16 val;

    state = (GameState *)getCurrentAllocation();

    newSelState = state->unk18C8[arg0->playerIndex];
    prevSelState = arg0->selectionState;

    if (prevSelState != newSelState) {
        arg0->selectionState = newSelState;
        func_80027BC8_287C8((func_80027BC8_arg *)arg0, arg0->playerIndex);
    }

    charIndex = state->unk18A8[arg0->playerIndex];
    paletteIndex = state->unk18B0[arg0->playerIndex];
    assetIndex = paletteIndex + charIndex * 3;

    memcpy(&sp10, identityMatrix, sizeof(Transform3D));
    memcpy(&sp10.translation, &arg0->positionMatrix.translation.x, 0xC);

    if (state->unk18D2[arg0->playerIndex] == state->unk18CC - 1) {
        val = state->unk1898[arg0->playerIndex];
        if (val != 1) {
            rotation = state->unk1888[arg0->playerIndex];
            createYRotationMatrix(&arg0->positionMatrix, rotation);
            goto after_rotation;
        }
    }

    rotation = state->unk1880[arg0->playerIndex];
    createYRotationMatrix(&arg0->positionMatrix, (0x2000 - rotation) & 0xFFFF);

after_rotation:
    func_8006B084_6BC84(&arg0->rotationMatrix, &arg0->positionMatrix, &sp10);
    func_8006B084_6BC84(&sp10, &state->unk17F8[arg0->playerIndex], arg0);

    val = state->unk1898[arg0->playerIndex];
    if (val == 4 || val == 9) {
        arg0->animationAsset = freeNodeMemory(arg0->animationAsset);
        arg0->skeletonAsset = freeNodeMemory(arg0->skeletonAsset);
        arg0->paletteAsset = freeNodeMemory(arg0->paletteAsset);
        setCallback(func_80024298_24E98);
    } else {
        if (assetIndex != arg0->charPaletteIndex) {
            arg0->charPaletteIndex = assetIndex;
            arg0->animationAsset = freeNodeMemory(arg0->animationAsset);
            arg0->skeletonAsset = freeNodeMemory(arg0->skeletonAsset);
            arg0->paletteAsset = freeNodeMemory(arg0->paletteAsset);
            setCallback(reloadCharSelectPreviewAssets);
        } else {
            func_800650B4_65CB4(arg0->playerIndex, (DisplayListObject *)arg0);
        }
    }
}

void reloadCharSelectPreviewAssets(CharSelectPreviewModel *arg0) {
    u8 paletteIndex;

    paletteIndex = EepromSaveData->character_or_settings[arg0->charPaletteIndex] - 1;

    arg0->modelAsset = loadAssetByIndex_95728(arg0->charPaletteIndex);
    arg0->animationAsset = loadAssetByIndex_95500(arg0->charPaletteIndex);
    arg0->skeletonAsset = loadAssetByIndex_95590(arg0->charPaletteIndex);
    arg0->paletteAsset = loadAssetByIndex_95668(paletteIndex);

    setCallback(updateCharSelectPreviewModel);
}

void func_80024298_24E98(CharSelectPreviewModel *arg0) {
    Transform3D sp10;
    Transform3D *localPtr;
    u8 *base;
    GameState *state;
    Transform3D *worldMatPtr;
    Transform3D *posMatPtr;
    s32 tableValue;
    u8 charIndex;
    u8 assetIndex;
    s32 paletteValue;
    u16 rotation;
    s32 offset;

    base = (u8 *)getCurrentAllocation();
    state = (GameState *)base;

    worldMatPtr = &arg0->worldMatrix;
    offset = arg0->playerIndex << 5;
    memcpy(worldMatPtr, (u8 *)(offset + (s32)base + 0x17F8), 0x20);

    tableValue = D_8008DD2C_8E92C[(D_800AFE8C_A71FC->numPlayers * 2) + state->unk18C0[arg0->playerIndex]];
    arg0->worldMatrix.translation.x = tableValue;
    arg0->targetX = tableValue;

    charIndex = state->unk18A8[arg0->playerIndex];
    assetIndex = state->unk18B0[arg0->playerIndex];
    assetIndex += charIndex * 3;
    arg0->charPaletteIndex = assetIndex;

    paletteValue = EepromSaveData->character_or_settings[assetIndex] - 1;
    assetIndex &= 0xFF;

    arg0->modelAsset = loadAssetByIndex_95728(assetIndex);
    arg0->animationAsset = loadAssetByIndex_95500(assetIndex);
    arg0->skeletonAsset = loadAssetByIndex_95590(assetIndex);
    arg0->paletteAsset = loadAssetByIndex_95668((u8)paletteValue);

    localPtr = &sp10;
    memcpy(localPtr, identityMatrix, 0x20);
    memcpy(&sp10.translation, &arg0->positionMatrix.translation.x, 0xC);

    posMatPtr = &arg0->positionMatrix;
    rotation = state->unk1888[arg0->playerIndex];
    createYRotationMatrix(posMatPtr, rotation);

    func_8006B084_6BC84(&arg0->rotationMatrix, posMatPtr, localPtr);
    func_8006B084_6BC84(localPtr, worldMatPtr, arg0);

    setCallback(func_80024414_25014);
}

void func_80024414_25014(func_80024518_arg *arg0) {
    Transform3D sp10;
    GameState *state;
    Transform3D *localPtr;
    s32 adjustment;
    u16 rotation;

    state = (GameState *)getCurrentAllocation();

    localPtr = &sp10;

    adjustment = (-(0 < arg0->unk9C) & 0xFFF00000) | 0x100000;

    memcpy(localPtr, identityMatrix, sizeof(Transform3D));
    memcpy(&sp10.translation, &arg0->unk5C.translation.x, 0xC);

    arg0->unk7C.translation.x = arg0->unk7C.translation.x + adjustment;

    rotation = state->unk1888[arg0->unkA1];
    createYRotationMatrix(&arg0->unk5C, rotation);

    func_8006B084_6BC84(&arg0->unk3C, &arg0->unk5C, localPtr);
    func_8006B084_6BC84(localPtr, &arg0->unk7C, arg0);

    func_800650B4_65CB4(arg0->unkA1, (DisplayListObject *)arg0);

    if (arg0->unk7C.translation.x == 0) {
        state->unk18C0[arg0->unkA1 + 4]++;
        setCallbackWithContinue(func_80024518_25118);
    }
}

void func_80024518_25118(func_80024518_arg *arg0) {
    Transform3D localMatrix;
    Transform3D *localPtr;
    GameState *base;
    u16 rotation;
    u16 val;

    base = (GameState *)getCurrentAllocation();

    localPtr = &localMatrix;
    memcpy(localPtr, identityMatrix, sizeof(Transform3D));
    memcpy(&localMatrix.translation, &arg0->unk5C.translation.x, 0xC);

    rotation = base->unk1888[arg0->unkA1];
    createYRotationMatrix(&arg0->unk5C, rotation);

    func_8006B084_6BC84(&arg0->unk3C, &arg0->unk5C, localPtr);
    func_8006B084_6BC84(localPtr, &arg0->unk7C, arg0);

    func_800650B4_65CB4(arg0->unkA1, (DisplayListObject *)arg0);

    val = base->unk1898[arg0->unkA1];
    if (val != 4 && val != 9) {
        setCallback(updateCharSelectPreviewModel);
    }
}

void func_80024600_25200(func_8002494C_arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
    arg0->unk2C = freeNodeMemory(arg0->unk2C);
}

void func_80024644_25244(func_80024644_arg *arg0) {
    Transform3D sp10;
    Transform3D sp30;
    Transform3D sp50;
    GameState *state;
    Transform3D *unk5CPtr;
    Transform3D *unk3CPtr;
    Transform3D *sp50Ptr;
    Transform3D *sp30Ptr;
    Transform3D *sp10Ptr;
    Transform3D *unk7CPtr;
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

void func_80024810_25410(func_80024644_arg *arg0) {
    Transform3D sp10;
    Transform3D *sp10Ptr;
    Transform3D *unk5CPtr;
    s32 target;
    s32 adjustment;
    GameState *state;

    state = (GameState *)getCurrentAllocation();

    sp10Ptr = &sp10;
    target = D_8008DD2C_8E92C[(D_800AFE8C_A71FC->numPlayers * 2) + ((state->unk18C0[arg0->unkA1] + 1) & 1)];
    adjustment = (-(target < 0) & 0xFFF00000) | 0x100000;

    memcpy(sp10Ptr, identityMatrix, 0x20);
    memcpy(&sp10.translation, &arg0->unk5C.translation.x, 0xC);

    arg0->unk7C.translation.x += adjustment;

    unk5CPtr = &arg0->unk5C;
    createYRotationMatrix(unk5CPtr, state->unk1888[arg0->unkA1]);

    func_8006B084_6BC84(&arg0->unk3C, unk5CPtr, sp10Ptr);
    func_8006B084_6BC84(sp10Ptr, &arg0->unk7C, arg0);

    if (arg0->unk7C.translation.x == target) {
        state->unk18C0[arg0->unkA1 + 4]++;
        func_80069CF8_6A8F8();
    } else {
        enqueueDisplayListObject(arg0->unkA1, (DisplayListObject *)arg0);
    }
}

void func_8002494C_2554C(func_8002494C_arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
    arg0->unk2C = freeNodeMemory(arg0->unk2C);
}

INCLUDE_ASM("asm/nonmatchings/24A30", func_80024990_25590);

void func_80024AAC_256AC(func_80024C8C_2588C_arg *arg0) {
    Transform3D localMatrix;
    Transform3D *localPtr;
    GameState *state;
    Transform3D *matrix;
    u16 rotation;

    state = (GameState *)getCurrentAllocation();

    localPtr = &localMatrix;
    memcpy(localPtr, identityMatrix, sizeof(Transform3D));

    matrix = &arg0->unk4;
    rotation = state->unk1880[arg0->unk28];
    createYRotationMatrix(matrix, 0x2000 - rotation);

    func_8006B084_6BC84(matrix, &state->unk17F8[arg0->unk28], localPtr);

    applyTransformToModel(arg0->unk0, localPtr);

    if (D_800AFE8C_A71FC->unk9[arg0->unk28] == 7) {
        setModelAnimation(arg0->unk0, 4);
    } else {
        setModelAnimation(arg0->unk0, 0x90);
    }

    updateModelGeometry(arg0->unk0);
    setCallback(func_80024BA0_257A0);
}

void func_80024BA0_257A0(func_80024C8C_2588C_arg *arg0) {
    Transform3D localMatrix;
    Transform3D *localPtr;
    GameState *state;
    Transform3D *matrix;
    u16 rotation;
    u16 val;

    state = (GameState *)getCurrentAllocation();

    localPtr = &localMatrix;
    memcpy(localPtr, identityMatrix, sizeof(Transform3D));

    matrix = &arg0->unk4;
    rotation = state->unk1880[arg0->unk28];
    createYRotationMatrix(matrix, 0x2000 - rotation);

    func_8006B084_6BC84(matrix, &state->unk17F8[arg0->unk28], localPtr);

    applyTransformToModel(arg0->unk0, localPtr);

    clearModelRotation(arg0->unk0);
    updateModelGeometry(arg0->unk0);

    val = state->unk1898[arg0->unk28];
    if (val == 0x10) {
        destroySceneModel(arg0->unk0);
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
        destroySceneModel(arg0->unk0);
        setCallback(func_80024DCC_259CC);
    } else if (val == 0) {
        arg0->unk4.translation.x = 0xFFEA0000;
        destroySceneModel(arg0->unk0);
        setCallback(func_80024D40_25940);
    } else if (val == 0x11) {
        arg0->unk4.translation.x = 0xFFEA0000;
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
    arg0->unk4.translation.x = D_8008DD2C_8E92C[D_800AFE8C_A71FC->numPlayers * 2 + (base + index)[0x18C0]];
    arg0->unk4.translation.z = 0;
    arg0->unk4.translation.y = 0xFFF00000;
    arg0->unk20_u.unk20_s32 = arg0->unk4.translation.x;

    applyTransformToModel(arg0->unk0, &arg0->unk4);

    ptr = (u8 *)D_800AFE8C_A71FC;
    ptr = ptr + arg0->unk28;
    if (ptr[9] == 7) {
        setModelAnimation(arg0->unk0, 4);
    } else {
        setModelAnimation(arg0->unk0, 0x90);
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

    unk24 = arg0->unk20_u.unk20_s32;

    adjustment = (-(0 < unk24) & 0xFFF00000) | 0x100000;

    arg0->unk4.translation.x = arg0->unk4.translation.x + adjustment;

    applyTransformToModel(arg0->unk0, &arg0->unk4);
    clearModelRotation(arg0->unk0);
    updateModelGeometry(arg0->unk0);

    if (arg0->unk4.translation.x == 0) {
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
    destroySceneModel(arg0->unk0);
}

INCLUDE_ASM("asm/nonmatchings/24A30", func_80025074_25C74);

void func_80025130_25D30(func_80025130_25D30_arg *arg0) {
    u8 *base;

    getCurrentAllocation();

    applyTransformToModel(arg0->unk0, &arg0->unk4);

    base = (u8 *)D_800AFE8C_A71FC;
    if (*(base + arg0->unk28 + 9) == 7) {
        setModelAnimation(arg0->unk0, 4);
    } else {
        setModelAnimation(arg0->unk0, 0x90);
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

    target = D_8008DD2C_8E92C[D_800AFE8C_A71FC->numPlayers * 2 + (((base + arg0->unk28)[0x18C0] + 1) & 1)];

    arg0->unk4.translation.x += ((target >> 31) & adj1) | adj2;

    applyTransformToModel(arg0->unk0, &arg0->unk4);
    clearModelRotation(arg0->unk0);
    updateModelGeometry(arg0->unk0);

    if (arg0->unk4.translation.x == target) {
        (base + arg0->unk28)[0x18C4]++;
        func_80069CF8_6A8F8();
    }
}

void *func_80025280_25E80(func_80025280_25E80_arg *arg0) {
    return destroySceneModel(arg0->unk0);
}

#define CONST_0xB_8002529C 0xB

void func_8002529C_25E9C(func_8002529C_25E9C_arg *arg0) {
    OutputStruct_19E80 sp10;
    u8 *tablePtr;
    volatile func_800255A0_entry *ptr;
    DataTable_19E80 *dmaResult;
    s16 scaleX;
    s16 scaleY;
    s32 i;
    s32 xIncrement;
    u16 unk0Val;
    u16 x;
    u16 xTemp;
    u16 xIncrementU16;
    s32 temp_v0;
    u8 unk8;
    s32 const_0xB;
    s32 pad[4];

    (void)pad;

    dmaResult = loadCompressedData(&_4237C0_ROM_START, &_4237C0_ROM_END, 0x8A08);
    setCleanupCallback(func_8002567C_2627C);
    getTableEntryByU16Index(dmaResult, 0xB, &sp10);

    unk8 = D_800AFE8C_A71FC->numPlayers;

    unk0Val = D_8008DDE6_8E9E6[unk8].y;
    xTemp = D_8008DDE6_8E9E6[unk8].x;
    xIncrementU16 = D_8008DDE6_8E9E6[unk8].inc;

    scaleX = 0x400;
    if (unk8 == 1) {
        scaleX = 0x200;
        scaleY = 0x200;
    } else {
        scaleY = 0x400;
    }

    i = 0;
    xIncrement = (s32)(s16)xIncrementU16;
    const_0xB = CONST_0xB_8002529C;
    tablePtr = D_8008DE18_8EA18;
    x = xTemp;
    ptr = (volatile func_800255A0_entry *)arg0;

    do {
        u8 tableVal;
        ptr->unk0 = unk0Val;
        ptr->unk2 = x;
        ptr->unk8 = const_0xB;
        tableVal = tablePtr[1];
        tablePtr += 2;
        ptr->unk12 = 0;
        ptr->unkA = scaleX;
        ptr->unkC = scaleY;
        ptr->unk4 = dmaResult;
        ptr->unkE = 0;
        ptr->unk13 = (s8)(tableVal + 1);
        x += xIncrement;
        i++;
        ptr->unk10 = sp10.field2;
        ptr++;
    } while (i < 3);

    arg0->unk50 = 1;
    arg0->unk51 = 0;
    arg0->unk53 = 8;
    setCallback(func_800253E0_25FE0);
}

void func_800253E0_25FE0(func_800253E0_25FE0_arg *arg0) {
    if (--arg0->unk53 == 0) {
        setCallback(func_80025418_26018);
    }
}

INCLUDE_ASM("asm/nonmatchings/24A30", func_80025418_26018);

extern u8 D_8008DD8C_8E98C[];
extern u16 D_8008DE02_8EA02[];

void func_800255A0_261A0(func_800255A0_arg *arg0) {
    GameState *state;
    s32 i;
    func_800255A0_entry *entry;
    u8 tableIndex;
    u8 charIndex;
    u8 paletteIndex;

    state = (GameState *)getCurrentAllocation();
    i = 0;

    do {
        charIndex = state->unk18A8[arg0->unk52];
        paletteIndex = state->unk18B0[arg0->unk52];
        tableIndex = D_8008DD8C_8E98C[((u8)(paletteIndex + charIndex * 3)) * 3 + i];
        entry = &arg0->entries[i];
        entry->unkE = D_8008DE02_8EA02[tableIndex];
        debugEnqueueCallback(arg0->unk52 + 8, 0, func_80010C98_11898, entry);
        i++;
    } while (i < 3);
}

void func_8002567C_2627C(func_80025FFC_26BFC_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

INCLUDE_ASM("asm/nonmatchings/24A30", func_800256A8_262A8);

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
        if (D_800AFE8C_A71FC->numPlayers == 1) {
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
    if (D_800AFE8C_A71FC->numPlayers == 1) {
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
        if (D_800AFE8C_A71FC->numPlayers == 1) {
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

void func_80025C64_26864(SelectionMenuState *arg0) {
    void *dmaResult;
    u16 x;
    u16 y;
    u16 xIncrement;
    s32 spriteIndexBase;
    s32 count;
    s32 i;
    s32 incrementSigned;
    s32 pad[6];

    (void)pad;

    dmaResult = loadCompressedData(&_4237C0_ROM_START, &_4237C0_ROM_END, 0x8A08);
    setCleanupCallback(func_80025FFC_26BFC);

    spriteIndexBase = 8;
    if (D_800AFE8C_A71FC->numPlayers == 1) {
        spriteIndexBase = 5;
    }

    arg0->numEntries = 3;

    if (D_800AFE8C_A71FC->unk4 == 0) {
        x = D_8008DD6C_8E96C.x;
        y = D_8008DD6C_8E96C.y;
        xIncrement = D_8008DD6C_8E96C.z;
        spriteIndexBase = 6;
        arg0->numEntries = 2;
    } else {
        s32 index = D_800AFE8C_A71FC->numPlayers;
        x = D_8008DD4E_8E94E[index].x;
        y = D_8008DD4E_8E94E[index].y;
        xIncrement = D_8008DD4E_8E94E[index].z;
    }

    count = arg0->numEntries;
    if (count > 0) {
        i = 0;
        incrementSigned = (s16)xIncrement;
        do {
            arg0->entries[i].x = x;
            arg0->entries[i].y = y;
            arg0->entries[i].spriteIndex = spriteIndexBase + i;
            arg0->entries[i].blinkState = 0;
            arg0->entries[i].unkC = 0;
            arg0->entries[i].alpha = 0xFF;
            arg0->entries[i].asset = dmaResult;
            arg0->unk30[i] = 0;
            i++;
            x += incrementSigned;
        } while (i < (s32)arg0->numEntries);
    }

    setCallback(func_80025DAC_269AC);
}

void func_80025EE4_26AE4(SelectionMenuState *);

void func_80025DAC_269AC(SelectionMenuState *menu) {
    GameState *state;
    s32 entryIndex;
    SelectionEntry *entries;
    u8 blinkCounter;
    u32 blinkPhase;
    s16 fullAlpha;
    s32 pad[2];
    s32 numEntries;

    (void)pad;

    state = getCurrentAllocation();
    entryIndex = 0;
    numEntries = menu->numEntries;
    if (numEntries > 0) {
        fullAlpha = 0xFF;
        entries = menu->entries;
        do {
            if (state->unk18D2[menu->playerIndex] == entryIndex) {
                if (state->unk1898[menu->playerIndex] == 0) {
                    blinkCounter = menu->unk30[entryIndex] + 1;
                    blinkPhase = blinkCounter & 0xFF;
                    menu->unk30[entryIndex] = blinkCounter;
                    if (blinkPhase < 0x11) {
                        entries[entryIndex].alpha = fullAlpha - (blinkPhase * 8);
                    } else {
                        entries[entryIndex].alpha = (blinkPhase * 8) - 1;
                    }
                    menu->unk30[entryIndex] = menu->unk30[entryIndex] & 0x1F;
                } else {
                    menu->unk30[entryIndex] = 0;
                    entries[entryIndex].alpha = fullAlpha;
                }
            } else {
                entries[entryIndex].alpha = 0x50;
                menu->unk30[entryIndex] = 0;
            }
            debugEnqueueCallback(menu->playerIndex + 0xC, 0, func_80012004_12C04, &entries[entryIndex]);
            entryIndex++;
        } while (entryIndex < (s32)menu->numEntries);
    }

    if (state->unk1898[menu->playerIndex] == 2) {
        setCallbackWithContinue(func_80025EE4_26AE4);
    }
}

void func_80025EE4_26AE4(SelectionMenuState *menu) {
    u8 *state;
    s32 entryIndex;
    s32 selectedIndex;
    SelectionEntry *entries;
    s32 pad[2];
    s32 numEntries;

    (void)pad;

    state = (u8 *)getCurrentAllocation();
    numEntries = menu->numEntries;
    entryIndex = 0;
    if (numEntries > 0) {
        entries = menu->entries;
        do {
            menu->unk30[entryIndex] = 0;
            if ((state + menu->playerIndex)[0x18D2] == entryIndex) {
                entries[entryIndex].alpha = 0xFF;
                selectedIndex = entryIndex;
                if (*(u16 *)(state + menu->playerIndex * 2 + 0x18A0) & 1) {
                    entries[entryIndex].blinkState = 0xFF;
                } else {
                    entries[entryIndex].blinkState = 0;
                }
            } else {
                entries[entryIndex].alpha = 0x50;
            }
            debugEnqueueCallback(menu->playerIndex + 0xC, 0, func_80012004_12C04, &entries[entryIndex]);
            entryIndex++;
        } while (entryIndex < (s32)menu->numEntries);
    }

    if (*(u16 *)(state + menu->playerIndex * 2 + 0x1898) != 2) {
        menu->entries[selectedIndex].blinkState = 0;
        setCallbackWithContinue(func_80025DAC_269AC);
    }
}

void func_80025FFC_26BFC(func_80025FFC_26BFC_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_80026028_26C28(func_80025FFC_26BFC_arg *arg0) {
    void *dmaResult;
    s32 i;
    s32 index;
    u16 x;
    s32 increment;
    u16 y;
    s32 pad[4];

    dmaResult = loadCompressedData(&_4237C0_ROM_START, &_4237C0_ROM_END, 0x8A08);
    setCleanupCallback(func_80026190_26D90);

    i = 0;
    index = D_800AFE8C_A71FC->numPlayers;
    x = D_8008DDBE_8E9BE[index].x;
    increment = D_8008DDBE_8E9BE[index].inc;
    y = D_8008DDBE_8E9BE[index].y;

    do {
        arg0[i].unk2 = y;
        y += increment;
        arg0[i].unk0 = x;
        arg0[i].unk8 = i + 2;
        arg0[i].unk4 = dmaResult;
        i++;
    } while (i < 3);

    setCallback(func_800260EC_26CEC);
}

void func_800260EC_26CEC(func_800260EC_26CEC_arg *arg0) {
    s32 i;
    s32 j;
    u16 index;
    func_80027348_entry *ptr;

    i = 0;
    if (i < (D_800AFE8C_A71FC)->numPlayers) {
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
        } while (i < (D_800AFE8C_A71FC)->numPlayers);
    }
}

void func_80026190_26D90(func_80025FFC_26BFC_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_800261BC_26DBC(func_80026564_arg *arg0) {
    void *dmaResult;
    u8 count;
    s32 i;
    s32 j;
    s32 index;
    u16 xBase;
    u16 y;
    u16 xInc;
    s32 xIncrement;
    SelectionEntry *ptr;
    u16 x;
    s32 iTimesTwo;
    s32 pad[6];

    (void)pad;

    getCurrentAllocation();
    dmaResult = loadCompressedData(&_4237C0_ROM_START, &_4237C0_ROM_END, 0x8A08);
    setCleanupCallback(func_80026538_27138);

    count = D_800AFE8C_A71FC->numPlayers;
    xBase = D_8008DE1A_8EA1A[count].x;
    y = D_8008DE1A_8EA1A[count].y;
    xInc = D_8008DE1A_8EA1A[count].inc;

    i = 0;
    if (count != 0) {
        xIncrement = (s16)xInc;
        do {
            j = 0;
            iTimesTwo = i * 2;
            x = xBase;
            do {
                ptr = &arg0->entries[iTimesTwo + j];
                ((volatile SelectionEntry *)ptr)->x = x;
                ((volatile SelectionEntry *)ptr)->spriteIndex = j;
                j++;
                ptr->y = y;
                ptr->asset = dmaResult;
                ptr->alpha = 0xFF;
                ptr->blinkState = 0;
                ptr->unkC = 0;
                x += xIncrement;
            } while (j < 2);
            arg0->unk80[i] = 0;
            i++;
        } while (i < D_800AFE8C_A71FC->numPlayers);
    }

    setCallback(func_800262D4_26ED4);
}

INCLUDE_ASM("asm/nonmatchings/24A30", func_800262D4_26ED4);

void func_80026538_27138(func_80025FFC_26BFC_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_80026564_27164(func_80026564_arg *arg0) {
    void *dmaResult;
    u8 count;
    s32 i;
    s32 j;
    s32 index;
    u16 xBase;
    u16 y;
    u16 xInc;
    s32 xIncrement;
    SelectionEntry *ptr;
    u16 x;
    s32 iTimesTwo;
    s32 pad[5];

    (void)pad;

    getCurrentAllocation();
    dmaResult = loadCompressedData(&_4237C0_ROM_START, &_4237C0_ROM_END, 0x8A08);
    setCleanupCallback(func_80026834_27434);

    count = D_800AFE8C_A71FC->numPlayers;
    index = count * 3;
    xBase = D_8008DE3A_8EA3A[count].x;
    y = D_8008DE3A_8EA3A[count].y;
    xInc = D_8008DE3A_8EA3A[count].z;

    i = 0;
    if (count != 0) {
        xIncrement = (s16)xInc;
        do {
            arg0->unk80[i] = 0;
            j = 0;
            iTimesTwo = i * 2;
            x = xBase;
            do {
                ptr = &arg0->entries[iTimesTwo + j];
                ((volatile SelectionEntry *)ptr)->x = x;
                ((volatile SelectionEntry *)ptr)->spriteIndex = j;
                j++;
                ptr->y = y;
                ptr->asset = dmaResult;
                ptr->alpha = 0xFF;
                ptr->blinkState = 0;
                ptr->unkC = 0;
                x += xIncrement;
            } while (j < 2);
            i++;
        } while (i < D_800AFE8C_A71FC->numPlayers);
    }

    setCallback(func_8002667C_2727C);
}

INCLUDE_ASM("asm/nonmatchings/24A30", func_8002667C_2727C);

void func_80026834_27434(func_80025FFC_26BFC_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_80026860_27460(CharacterNameSprite *sprites) {
    GameState *gameState;
    void *spriteAsset;
    s32 i;
    u16 xPos;
    u16 yPos;
    s32 spriteIdx;
    u8 charIndex;
    u8 numPlayers;
    s32 pad[2];

    gameState = (GameState *)getCurrentAllocation();
    spriteAsset = loadCompressedData(&_4237C0_ROM_START, &_4237C0_ROM_END, 0x8A08);
    setCleanupCallback(func_80026BAC_277AC);

    numPlayers = D_800AFE8C_A71FC->numPlayers;
    xPos = D_8008DE54_8EA54[numPlayers].unk0;
    yPos = D_8008DE54_8EA54[numPlayers].unk2;

    for (i = 0; i < D_800AFE8C_A71FC->numPlayers; i++) {
        numPlayers = gameState->unk18A8[i];
        if (numPlayers == 3) {
            numPlayers = D_800AFE8C_A71FC->numPlayers;
            if (numPlayers == 1) {
                xPos += 0x18;
                spriteIdx = D_800AFE8C_A71FC->unk9[i + 4] + 0x30;
            } else {
                charIndex = D_800AFE8C_A71FC->unk9[i + 4];
                spriteIdx = charIndex + 0x23;
                xPos = *((u16 *)&D_8008DE9C_8EA9C + numPlayers * 2) - D_8008DE7A_8EA7A[charIndex];
            }
            sprites[i].spriteIndex = spriteIdx;
        } else {
            spriteIdx = 0x1D;
            if (D_800AFE8C_A71FC->numPlayers == 1) {
                spriteIdx = 0x36;
            }
            sprites[i].spriteIndex = numPlayers + spriteIdx;
        }
        sprites[i].blinkState = 0;
        sprites[i].unkC = 0;
        sprites[i].x = xPos;
        sprites[i].y = yPos;
        sprites[i].alpha = 0xFF;
        sprites[i].asset = spriteAsset;
    }

    setCallback(func_800269C8_275C8);
}

INCLUDE_ASM("asm/nonmatchings/24A30", func_800269C8_275C8);

void func_80026BAC_277AC(func_80025FFC_26BFC_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_80026BD8_277D8(func_80026BD8_arg *arg0) {
    u8 *allocation;
    void *dmaResult;
    D_800AFE8C_A71FC_type *global;
    u8 count;
    s32 i;
    u16 unk0Val;
    u16 unk2Val;
    s32 unk8Val;
    s32 const_1;
    s32 const_ff;
    volatile SelectionEntry *ptr;
    s32 pad[4];

    (void)pad;

    allocation = (u8 *)getCurrentAllocation();
    dmaResult = loadCompressedData(&_4237C0_ROM_START, &_4237C0_ROM_END, 0x8A08);
    setCleanupCallback(func_80026FC8_27BC8);

    global = D_800AFE8C_A71FC;
    count = global->numPlayers;
    unk0Val = D_8008DE9C_8EA9C[count].unk0;
    unk2Val = D_8008DE9C_8EA9C[count].unk2;

    i = 0;
    if (count != 0) {
        const_1 = 1;
        const_ff = 0xFF;
        ptr = (volatile SelectionEntry *)arg0;
        do {
            if (global->unk9[i + 4] >= 9) {
                unk8Val = 0x35;
            } else {
                unk8Val = 0x24;
                if (global->numPlayers == const_1) {
                    unk8Val = 0x43;
                }
                unk8Val = unk8Val + (allocation + i)[0x18B0];
            }
            ptr->blinkState = 0;
            ptr->unkC = 0;
            global = D_800AFE8C_A71FC;
            ptr->x = unk0Val;
            ptr->y = unk2Val;
            ptr->alpha = const_ff;
            ptr->spriteIndex = unk8Val;
            ptr->asset = dmaResult;
            i++;
            ptr++;
        } while (i < global->numPlayers);
    }

    if (D_800AFE8C_A71FC->numPlayers == 1) {
        arg0->unk40 = 0x38;
        arg0->unk42 = -0x58;
        arg0->unk44 = dmaResult;
        arg0->unk48 = 0x42;
        if (D_800AFE8C_A71FC->unk9[4] >= 9) {
            arg0->unk40 = 0x50;
        }
    }

    setCallback(func_80026D34_27934);
}

INCLUDE_ASM("asm/nonmatchings/24A30", func_80026D34_27934);

void func_80026FC8_27BC8(func_80025FFC_26BFC_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

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

    dmaResult = loadCompressedData(&_41A1D0_ROM_START, &_41A1D0_ROM_END, 0x1B48);
    setCleanupCallback(func_8002712C_27D2C);

    global = D_800AFE8C_A71FC;
    count = global->numPlayers;
    unk8Base = (count >= 3) * 8;
    x = D_8008DDD8_8E9D8[count].x;
    y = D_8008DDD8_8E9D8[count].y;

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
        } while (i < global->numPlayers);
    }

    setCallback(func_800270B8_27CB8);
}

void func_800270B8_27CB8(u8 *arg0) {
    s32 i;
    s32 pad[2];
    u8 *ptr;
    s32 count;

    count = D_800AFE8C_A71FC->numPlayers;
    if (count > 0) {
        i = 0;
        ptr = arg0;
        do {
            debugEnqueueCallback((u16)(i + 0xC), 0, func_8000FED0_10AD0, ptr);
            i++;
            ptr += 0xC;
        } while (i < D_800AFE8C_A71FC->numPlayers);
    }
}

void func_8002712C_27D2C(func_80025FFC_26BFC_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_80027158_27D58(func_80025FFC_26BFC_arg *arg0) {
    GameState *state;
    void *dmaResult;

    state = getCurrentAllocation();
    dmaResult = loadCompressedData(&_4237C0_ROM_START, &_4237C0_ROM_END, 0x8A08);
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

void func_80027268_27E68(func_80025FFC_26BFC_arg *arg0) {
    void *dmaResult;
    s32 loopCount;
    s32 i;

    dmaResult = loadCompressedData(&_4237C0_ROM_START, &_4237C0_ROM_END, 0x8A08);

    loopCount = (D_800AFE8C_A71FC->numPlayers == 2) ? 3 : 2;

    for (i = 0; i < loopCount; i++) {
        arg0[i].unk4 = dmaResult;
    }

    setCleanupCallback(func_8002764C_2824C);
    setCallback(func_800272FC_27EFC);
}

void func_800272FC_27EFC(func_800272FC_27EFC_arg *arg0) {
    GameState *state = (GameState *)getCurrentAllocation();

    if (state->unk1898[arg0->unk24] == 0x1A) {
        setCallbackWithContinue(func_80027348_27F48);
    }
}

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
    if (D_800AFE8C_A71FC->numPlayers == 2) {
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

void func_80027400_28000(func_80025824_arg *arg0) {
    s16 minY;
    s32 yIncrement;
    s32 loopCount;
    s32 i;
    volatile func_80027348_entry *ptr;
    GameState *state;
    int new_var;
    u16 val;
    s32 pad[2];
    (void)pad;
    state = (GameState *)getCurrentAllocation();
    minY = -0x10;
    if (D_800AFE8C_A71FC->numPlayers == 2) {
        minY = -0x14;
        yIncrement = 0x14;
        loopCount = 3;
    } else {
        yIncrement = 0x13;
        loopCount = 2;
    }
    yIncrement &= -(state->unk1898[arg0->unk24] == 0x1A);
    i = 0;
    if (((s32)(loopCount & 0xFF)) > 0) {
        new_var = loopCount & 0xFF;
        ptr = (volatile func_80027348_entry *)arg0;
        do {
            i += 1;
            ptr->unk2 = ptr->unk2 + yIncrement;
            debugEnqueueCallback(arg0->unk24 + 0xC, 0, func_80010240_10E40, (void *)ptr);
            ptr++;
        } while (i < ((s32)new_var));
    }
    val = state->unk1898[arg0->unk24];
    if (val == 0x1A) {
        if (arg0->entries[0].unk2 == minY) {
            state->unk1898[arg0->unk24] = 0x1B;
        }
    } else if (val == 0x1E) {
        setCallback(func_80027544_28144);
    }
}

void func_80027544_28144(func_80027544_arg *arg0) {
    u8 *allocation;
    s16 target;
    unsigned char var_v0;
    s32 count;
    s32 i;
    s16 increment;
    allocation = (u8 *)getCurrentAllocation();
    target = -0x5C;
    if (D_800AFE8C_A71FC->numPlayers == 2) {
        target = -0x64;
        var_v0 = 3;
        increment = -0x14;
    } else {
        var_v0 = 2;
        increment = -0x13;
    }
    for (i = 0; i < (var_v0 & 0xFF); i++) {
        arg0->entries[i].unk2 += increment;
        debugEnqueueCallback(arg0->unk24 + 0xC, 0, func_80010240_10E40, (void *)(&arg0->entries[i]));
    }

    if (arg0->entries[0].unk2 == target) {
        *((u16 *)((allocation + (arg0->unk24 * 2)) + 0x1898)) = 0;
        setCallback(func_800272FC_27EFC);
    }
}

void func_8002764C_2824C(func_80025FFC_26BFC_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

INCLUDE_ASM("asm/nonmatchings/24A30", func_80027678_28278);

INCLUDE_ASM("asm/nonmatchings/24A30", func_800277F4_283F4);

void func_800279A8_285A8(func_80025FFC_26BFC_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_800279D4_285D4(func_80027A28_28628_arg *arg0) {
    arg0->unk2C = loadCompressedData(&_458E30_ROM_START, &_458E30_ROM_END, 0xAE0);
    setCleanupCallback(func_80027AAC_286AC);
    setCallback(func_80027A28_28628);
}

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

void func_80027AD8_286D8(func_80027AD8_286D8_arg *arg0) {
    void *dmaResult;

    dmaResult = loadCompressedData(&_4237C0_ROM_START, &_4237C0_ROM_END, 0x8A08);
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

u8 func_80027C60_28860(u8 arg0) {
    return D_8008DD8D_8E98D[arg0 * 3];
}

u8 func_80027C7C_2887C(u8 arg0) {
    return D_8008DD8E_8E98E[arg0 * 3];
}
