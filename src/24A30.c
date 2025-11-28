#include "20F0.h"
#include "5E590.h"
#include "D_800AFE8C_A71FC_type.h"
#include "EepromSaveData_type.h"
#include "common.h"
#include "overlay.h"
#include "task_scheduler.h"

USE_ASSET(_458E30);

extern u8 D_459310[];

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
    void *unk0;
    void *unk4;
} func_80025FFC_26BFC_arg;

extern void func_800394BC_3A0BC(void *, s32);

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

INCLUDE_ASM("asm/nonmatchings/24A30", func_80024BA0_257A0);

INCLUDE_ASM("asm/nonmatchings/24A30", func_80024C8C_2588C);

INCLUDE_ASM("asm/nonmatchings/24A30", func_80024D40_25940);

INCLUDE_ASM("asm/nonmatchings/24A30", func_80024DCC_259CC);

INCLUDE_ASM("asm/nonmatchings/24A30", func_80024E58_25A58);

INCLUDE_ASM("asm/nonmatchings/24A30", func_80024F48_25B48);

INCLUDE_ASM("asm/nonmatchings/24A30", func_80024FEC_25BEC);

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

INCLUDE_ASM("asm/nonmatchings/24A30", func_800253E0_25FE0);

INCLUDE_ASM("asm/nonmatchings/24A30", func_80025418_26018);

INCLUDE_ASM("asm/nonmatchings/24A30", func_800255A0_261A0);

INCLUDE_ASM("asm/nonmatchings/24A30", func_8002567C_2627C);

INCLUDE_ASM("asm/nonmatchings/24A30", func_800256A8_262A8);

INCLUDE_ASM("asm/nonmatchings/24A30", func_80025824_26424);

INCLUDE_ASM("asm/nonmatchings/24A30", func_80025904_26504);

INCLUDE_ASM("asm/nonmatchings/24A30", func_80025A88_26688);

INCLUDE_ASM("asm/nonmatchings/24A30", func_80025C38_26838);

INCLUDE_ASM("asm/nonmatchings/24A30", func_80025C64_26864);

INCLUDE_ASM("asm/nonmatchings/24A30", func_80025DAC_269AC);

INCLUDE_ASM("asm/nonmatchings/24A30", func_80025EE4_26AE4);

void func_80025FFC_26BFC(func_80025FFC_26BFC_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

INCLUDE_ASM("asm/nonmatchings/24A30", func_80026028_26C28);

INCLUDE_ASM("asm/nonmatchings/24A30", func_800260EC_26CEC);

INCLUDE_ASM("asm/nonmatchings/24A30", func_80026190_26D90);

INCLUDE_ASM("asm/nonmatchings/24A30", func_800261BC_26DBC);

INCLUDE_ASM("asm/nonmatchings/24A30", func_800262D4_26ED4);

INCLUDE_ASM("asm/nonmatchings/24A30", func_80026538_27138);

INCLUDE_ASM("asm/nonmatchings/24A30", func_80026564_27164);

INCLUDE_ASM("asm/nonmatchings/24A30", func_8002667C_2727C);

INCLUDE_ASM("asm/nonmatchings/24A30", func_80026834_27434);

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

INCLUDE_ASM("asm/nonmatchings/24A30", func_80027158_27D58);

INCLUDE_ASM("asm/nonmatchings/24A30", func_800271E4_27DE4);

INCLUDE_ASM("asm/nonmatchings/24A30", func_8002723C_27E3C);

INCLUDE_ASM("asm/nonmatchings/24A30", func_80027268_27E68);

INCLUDE_ASM("asm/nonmatchings/24A30", func_800272FC_27EFC);

INCLUDE_ASM("asm/nonmatchings/24A30", func_80027348_27F48);

INCLUDE_ASM("asm/nonmatchings/24A30", func_80027400_28000);

INCLUDE_ASM("asm/nonmatchings/24A30", func_80027544_28144);

void func_8002764C_2824C(func_80025FFC_26BFC_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

INCLUDE_ASM("asm/nonmatchings/24A30", func_80027678_28278);

INCLUDE_ASM("asm/nonmatchings/24A30", func_800277F4_283F4);

INCLUDE_ASM("asm/nonmatchings/24A30", func_800279A8_285A8);

void func_80027AAC_286AC(void);
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

INCLUDE_ASM("asm/nonmatchings/24A30", func_80027AAC_286AC);

INCLUDE_ASM("asm/nonmatchings/24A30", func_80027AD8_286D8);

INCLUDE_ASM("asm/nonmatchings/24A30", func_80027B70_28770);

INCLUDE_ASM("asm/nonmatchings/24A30", func_80027B9C_2879C);

INCLUDE_ASM("asm/nonmatchings/24A30", func_80027BC8_287C8);

INCLUDE_ASM("asm/nonmatchings/24A30", func_80027C44_28844);

INCLUDE_ASM("asm/nonmatchings/24A30", func_80027C60_28860);

INCLUDE_ASM("asm/nonmatchings/24A30", func_80027C7C_2887C);
