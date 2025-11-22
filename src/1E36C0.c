#include "1DFAA0.h"
#include "common.h"
#include "cutscene/cutscene_sys2.h"

typedef struct {
    u8 pad[0x54];
    s32 unk54;
    s32 unk58;
    s32 unk5C;
    s32 unk60;
    s32 unk64;
    s32 unk68;
} unk_func_800B68F4_1E39A4;

s16 func_800B6610_1E36C0(cutsceneSys2Wait_exec_asset *arg0) {
    return arg0->unk86;
}

INCLUDE_ASM("asm/nonmatchings/1E36C0", func_800B6618_1E36C8);

INCLUDE_ASM("asm/nonmatchings/1E36C0", func_800B66B4_1E3764);

INCLUDE_ASM("asm/nonmatchings/1E36C0", setupSlotTransform);

void func_800B68F4_1E39A4(unk_func_800B68F4_1E39A4 *arg0, s32 arg1, s32 arg2, s32 arg3) {
    arg0->unk60 = arg1;
    arg0->unk54 = arg1;
    arg0->unk64 = arg2;
    arg0->unk58 = arg2;
    arg0->unk68 = arg3;
    arg0->unk5C = arg3;
}

INCLUDE_ASM("asm/nonmatchings/1E36C0", func_800B6910_1E39C0);

INCLUDE_ASM("asm/nonmatchings/1E36C0", func_800B6AB8_1E3B68);

INCLUDE_ASM("asm/nonmatchings/1E36C0", func_800B6B6C_1E3C1C);

extern void func_800B6C04_1E3CB4(CutsceneSlotData *, s32, s32, s32, s16, s16, s16);

void func_800B6BDC_1E3C8C(CutsceneSlotData *arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4) {
    func_800B6C04_1E3CB4(arg0, arg1, arg2, arg3, arg4, 0, 0);
}

INCLUDE_ASM("asm/nonmatchings/1E36C0", func_800B6C04_1E3CB4);

INCLUDE_ASM("asm/nonmatchings/1E36C0", func_800B6C8C_1E3D3C);

INCLUDE_ASM("asm/nonmatchings/1E36C0", func_800B6CD8_1E3D88);

INCLUDE_ASM("asm/nonmatchings/1E36C0", func_800B6FA4_1E4054);

INCLUDE_ASM("asm/nonmatchings/1E36C0", func_800B7128_1E41D8);

INCLUDE_ASM("asm/nonmatchings/1E36C0", func_800B734C_1E43FC);

INCLUDE_ASM("asm/nonmatchings/1E36C0", func_800B7450_1E4500);

INCLUDE_ASM("asm/nonmatchings/1E36C0", func_800B75C4_1E4674);

INCLUDE_ASM("asm/nonmatchings/1E36C0", func_800B7620_1E46D0);

INCLUDE_ASM("asm/nonmatchings/1E36C0", func_800B7760_1E4810);

INCLUDE_ASM("asm/nonmatchings/1E36C0", func_800B77C4_1E4874);

INCLUDE_ASM("asm/nonmatchings/1E36C0", func_800B7828_1E48D8);

INCLUDE_ASM("asm/nonmatchings/1E36C0", func_800B788C_1E493C);

INCLUDE_ASM("asm/nonmatchings/1E36C0", func_800B7914_1E49C4);

INCLUDE_ASM("asm/nonmatchings/1E36C0", func_800B7A60_1E4B10);

INCLUDE_ASM("asm/nonmatchings/1E36C0", func_800B7B70_1E4C20);

INCLUDE_ASM("asm/nonmatchings/1E36C0", func_800B7C48_1E4CF8);

INCLUDE_ASM("asm/nonmatchings/1E36C0", func_800B826C_1E531C);

INCLUDE_ASM("asm/nonmatchings/1E36C0", func_800B83B8_1E5468);

INCLUDE_ASM("asm/nonmatchings/1E36C0", func_800B844C_1E54FC);

INCLUDE_ASM("asm/nonmatchings/1E36C0", func_800B8874_1E5924);

INCLUDE_ASM("asm/nonmatchings/1E36C0", func_800B8AC4_1E5B74);

INCLUDE_ASM("asm/nonmatchings/1E36C0", func_800B8C3C_1E5CEC);

INCLUDE_ASM("asm/nonmatchings/1E36C0", func_800B8D34_1E5DE4);

INCLUDE_ASM("asm/nonmatchings/1E36C0", func_800B8DA8_1E5E58);

INCLUDE_ASM("asm/nonmatchings/1E36C0", syncModelFromSlot);
