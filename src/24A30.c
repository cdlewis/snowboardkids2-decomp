#include "20F0.h"
#include "5E590.h"
#include "D_800AFE8C_A71FC_type.h"
#include "EepromSaveData_type.h"
#include "common.h"
#include "gamestate.h"
#include "geometry.h"
#include "overlay.h"
#include "task_scheduler.h"

USE_ASSET(_458E30);

extern u8 D_459310[];

extern u8 D_4237C0[];
extern u8 D_426EF0[];

typedef struct {
    u8 padding[0x24];
    void *unk24;
    void *unk28;
    void *unk2C;
} func_8002494C_arg;

typedef struct {
    u8 padding[0x2C];
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
    u8 padding[0x24];
    u8 unk24;
} func_800272FC_27EFC_arg;

typedef struct {
    u8 padding[0x53];
    u8 unk53;
} func_800253E0_25FE0_arg;

extern void func_800394BC_3A0BC(void *, s32);
extern void func_8000FED0_10AD0(void);
extern void debugEnqueueCallback(u16, u8, void *, void *);

INCLUDE_ASM("asm/nonmatchings/24A30", func_80023E30_24A30);

void func_80024048_24C48(void *);
INCLUDE_ASM("asm/nonmatchings/24A30", func_80024048_24C48);

void func_80024220_24E20(func_80024220_24E20_arg *arg0) {
    u8 charIndex;

    charIndex = EepromSaveData->character_or_settings[arg0->unkA2] - 1;

    arg0->unk20 = loadAssetByIndex_95728(arg0->unkA2);
    arg0->unk24 = loadAssetByIndex_95500(arg0->unkA2);
    arg0->unk28 = loadAssetByIndex_95590(arg0->unkA2);
    arg0->unk2C = loadAssetByIndex_95668(charIndex);

    setCallback(func_80024048_24C48);
}

INCLUDE_ASM("asm/nonmatchings/24A30", func_80024298_24E98);

INCLUDE_ASM("asm/nonmatchings/24A30", func_80024414_25014);

INCLUDE_ASM("asm/nonmatchings/24A30", func_80024518_25118);

void func_80024600_25200(func_8002494C_arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
    arg0->unk2C = freeNodeMemory(arg0->unk2C);
}

INCLUDE_ASM("asm/nonmatchings/24A30", func_80024644_25244);

INCLUDE_ASM("asm/nonmatchings/24A30", func_80024810_25410);

void func_8002494C_2554C(func_8002494C_arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
    arg0->unk2C = freeNodeMemory(arg0->unk2C);
}

INCLUDE_ASM("asm/nonmatchings/24A30", func_80024990_25590);

INCLUDE_ASM("asm/nonmatchings/24A30", func_80024AAC_256AC);

extern s32 identityMatrix[];

void func_80024D40_25940(func_80024C8C_2588C_arg *);
void func_80024DCC_259CC(func_80024C8C_2588C_arg *);

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

INCLUDE_ASM("asm/nonmatchings/24A30", func_80024E58_25A58);

INCLUDE_ASM("asm/nonmatchings/24A30", func_80024F48_25B48);

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

void func_800251AC_25DAC(void);

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

INCLUDE_ASM("asm/nonmatchings/24A30", func_800251AC_25DAC);

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

INCLUDE_ASM("asm/nonmatchings/24A30", func_800255A0_261A0);

void func_8002567C_2627C(func_80025FFC_26BFC_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

INCLUDE_ASM("asm/nonmatchings/24A30", func_800256A8_262A8);

INCLUDE_ASM("asm/nonmatchings/24A30", func_80025824_26424);

INCLUDE_ASM("asm/nonmatchings/24A30", func_80025904_26504);

INCLUDE_ASM("asm/nonmatchings/24A30", func_80025A88_26688);

void func_80025C38_26838(func_80025FFC_26BFC_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

INCLUDE_ASM("asm/nonmatchings/24A30", func_80025C64_26864);

INCLUDE_ASM("asm/nonmatchings/24A30", func_80025DAC_269AC);

INCLUDE_ASM("asm/nonmatchings/24A30", func_80025EE4_26AE4);

void func_80025FFC_26BFC(func_80025FFC_26BFC_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

INCLUDE_ASM("asm/nonmatchings/24A30", func_80026028_26C28);

INCLUDE_ASM("asm/nonmatchings/24A30", func_800260EC_26CEC);

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

INCLUDE_ASM("asm/nonmatchings/24A30", func_80026FF4_27BF4);

INCLUDE_ASM("asm/nonmatchings/24A30", func_800270B8_27CB8);

void func_8002712C_27D2C(func_80025FFC_26BFC_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_800271E4_27DE4(func_80025FFC_26BFC_arg *);
void func_8002723C_27E3C(func_80025FFC_26BFC_arg *);

void func_80027158_27D58(func_80025FFC_26BFC_arg *arg0) {
    GameState *state;
    void *dmaResult;

    state = getCurrentAllocation();
    dmaResult = dmaRequestAndUpdateStateWithSize(D_4237C0, D_426EF0, 0x8A08);
    setCleanupCallback(func_8002723C_27E3C);

    arg0->unk0 = -0x20;
    arg0->unk2 = 8;
    arg0->unk8 = state->unk18A8 + 0x16;
    arg0->unk4 = dmaResult;

    setCallback(func_800271E4_27DE4);
}

void func_800271E4_27DE4(func_80025FFC_26BFC_arg *arg0) {
    GameState *state = getCurrentAllocation();

    if (state->unk1898[0] == 3) {
        arg0->unk8 = state->unk18A8 + 0x16;
        debugEnqueueCallback(0xC, 0, func_8000FED0_10AD0, arg0);
    }
}

void func_8002723C_27E3C(func_80025FFC_26BFC_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

INCLUDE_ASM("asm/nonmatchings/24A30", func_80027268_27E68);

void func_80027348_27F48(void);

void func_800272FC_27EFC(func_800272FC_27EFC_arg *arg0) {
    GameState *state = (GameState *)getCurrentAllocation();

    if (state->unk1898[arg0->unk24] == 0x1A) {
        setCallbackWithContinue(func_80027348_27F48);
    }
}

INCLUDE_ASM("asm/nonmatchings/24A30", func_80027348_27F48);

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
    arg0->unk2C = dmaRequestAndUpdateStateWithSize(&_458E30_ROM_START, D_459310, 0xAE0);
    setCleanupCallback(func_80027AAC_286AC);
    setCallback(func_80027A28_28628);
}

void func_80027A50_28650(void);

void func_80027A28_28628(func_80027A28_28628_arg *arg0) {
    func_800394BC_3A0BC(arg0, (s32)arg0->unk2C);
    setCallback(&func_80027A50_28650);
}

INCLUDE_ASM("asm/nonmatchings/24A30", func_80027A50_28650);

void func_80027AAC_286AC(func_80027A28_28628_arg *arg0) {
    arg0->unk2C = freeNodeMemory(arg0->unk2C);
}

INCLUDE_ASM("asm/nonmatchings/24A30", func_80027AD8_286D8);

extern void debugEnqueueCallback(u16 index, u8 arg1, void *arg2, void *arg3);
extern void func_80011924_12524(void);

void func_80027B70_28770(void *arg0) {
    debugEnqueueCallback(0xC, 7, func_80011924_12524, arg0);
}

void func_80027B9C_2879C(func_80025FFC_26BFC_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

INCLUDE_ASM("asm/nonmatchings/24A30", func_80027BC8_287C8);

extern u8 D_8008DD8C_8E98C[];

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
