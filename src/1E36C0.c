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

extern s16 func_800B6618_1E36C8(CutsceneSlotData *, s16, s16, s16);

void func_800B6B6C_1E3C1C(CutsceneSlotData *arg0) {
    s16 temp;

    temp = func_800B6618_1E36C8(arg0, 0, arg0->unk7A, arg0->unk78);

    if (temp > 0) {
        arg0->unk7C = 0xAA;
        arg0->unk78 -= 0xAA;
    } else if (temp < 0) {
        arg0->unk7C = -0xAA;
        arg0->unk78 += 0xAA;
    } else {
        arg0->unk7C = 0;
    }
}

extern s32 func_800B6CD8_1E3D88(CutsceneSlotData *, SceneModel *, s32, s32, s32, s16, s16, s32, s32);
extern void func_800B6AB8_1E3B68(CutsceneSlotData *, s32);

void func_800B6C04_1E3CB4(CutsceneSlotData *arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5, s16 arg6);

void func_800B6BDC_1E3C8C(CutsceneSlotData *arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4) {
    func_800B6C04_1E3CB4(arg0, arg1, arg2, arg3, arg4, 0, 0);
}

void func_800B6C04_1E3CB4(CutsceneSlotData *arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5, s16 arg6) {
    s16 temp_v0;

    temp_v0 = arg4 & 0x1FFF;

    arg0->unk04.unk20_u.unk20_s32 = arg1;
    arg0->unk30 = arg1;
    arg0->unk0.bytes[0] = 0;
    arg0->unk28 = arg2;
    arg0->unk2C = arg3;
    arg0->unk34 = arg2;
    arg0->unk38 = arg3;
    arg0->unk3C = 0;
    arg0->unk40 = 0;
    arg0->unk44 = 0;
    arg0->unk6C = 0;
    arg0->unk70 = 0;
    arg0->unk74 = 0;
    arg0->unk7C = 0;
    arg0->unk84 = 0;
    arg0->unk86 = 0;
    arg0->unk0.bytes[1] = 0;
    arg0->unk88 = 0;
    arg0->angle = 0;
    arg0->unk78 = temp_v0;
    arg0->unk7A = temp_v0;
    arg0->unk80 = arg5;
    arg0->unk82 = arg6;

    func_800B6AB8_1E3B68(arg0, 0);
}

s32 func_800B6C8C_1E3D3C(CutsceneSlotData *arg0, SceneModel *arg1, s32 arg2, s32 arg3, s32 arg4, s16 arg5, s16 arg6) {
    return func_800B6CD8_1E3D88(arg0, arg1, arg2, arg3, arg4, arg5, arg6, 0, 1);
}

INCLUDE_ASM("asm/nonmatchings/1E36C0", func_800B6CD8_1E3D88);

INCLUDE_ASM("asm/nonmatchings/1E36C0", func_800B6FA4_1E4054);

INCLUDE_ASM("asm/nonmatchings/1E36C0", func_800B7128_1E41D8);

INCLUDE_ASM("asm/nonmatchings/1E36C0", func_800B734C_1E43FC);

INCLUDE_ASM("asm/nonmatchings/1E36C0", func_800B7450_1E4500);

s32 func_800B75C4_1E4674(CutsceneSlotData *arg0, SceneModel *arg1, s16 arg2, s32 arg3, s32 arg4, s32 arg5) {
    s16 temp_v0;

    temp_v0 = arg4;

    arg0->unk0.Two = 0xB;
    arg0->unk3C = 0;
    arg0->unk40 = 0;
    arg0->unk44 = 0;
    arg0->unk7A = arg2;
    arg0->unk84 = temp_v0;
    arg0->unk86 = temp_v0;

    if (temp_v0 != 0) {
        arg0->unk7C = arg5 * arg3;
    } else {
        arg0->unk7C = 0;
    }

    return 0;
}

INCLUDE_ASM("asm/nonmatchings/1E36C0", func_800B7620_1E46D0);

INCLUDE_ASM("asm/nonmatchings/1E36C0", func_800B7760_1E4810);

INCLUDE_ASM("asm/nonmatchings/1E36C0", func_800B77C4_1E4874);

void func_800B7828_1E48D8(CutsceneSlotData *arg0, s32 arg1, s16 arg2) {
    s32 delta;

    if (arg2 > 0) {
        delta = arg1 - arg0->unk5C;
        arg0->unk68 = arg1;
        arg0->unk74 = delta / arg2;
    } else {
        arg0->unk5C = arg1;
        arg0->unk68 = arg1;
        arg0->unk74 = 0;
    }
}

extern s32 approximateSin(s16);
extern s32 approximateCos(s16);

void func_800B788C_1E493C(CutsceneSlotData *arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 sinResult;
    s32 cosResult;
    s32 scaledArg1;

    arg0->unk0.bytes[0] = 7;

    sinResult = approximateSin(arg0->unk78);
    scaledArg1 = arg1 >> 8;
    arg0->unk3C = (scaledArg1 * (sinResult >> 4));

    arg0->unk40 = arg2;

    cosResult = approximateCos(arg0->unk78);
    arg0->unk44 = (scaledArg1 * (cosResult >> 4));

    arg0->unkA0 = arg3;
}

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

s32 func_800B8D34_1E5DE4(CutsceneSlotData *arg0) {
    arg0->unk04.unk20_u.unk20_s32 += arg0->unk3C;
    arg0->unk28 += arg0->unk40;
    arg0->unk2C += arg0->unk44;

    if (arg0->unk40 < 0) {
        if (arg0->unk28 < 0) {
            arg0->unk3C = 0;
            arg0->unk40 = 0;
            arg0->unk44 = 0;
            arg0->unk28 = 0;
            arg0->unkA0 = 0;
        }
    }

    arg0->unk40 += arg0->unkA0;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/1E36C0", func_800B8DA8_1E5E58);

INCLUDE_ASM("asm/nonmatchings/1E36C0", syncModelFromSlot);
