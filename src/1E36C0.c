#include "1E36C0.h"
#include "1E2BE0.h"
#include "geometry.h"

extern s8 D_800AB044_A23B4;
extern s8 D_800AB048_A23B8;
extern s32 gButtonsPressed;
extern s32 gControllerInputs;

s16 func_800B6610_1E36C0(cutsceneSys2Wait_exec_asset *arg0) {
    return arg0->unk86;
}

INCLUDE_ASM("asm/nonmatchings/1E36C0", func_800B6618_1E36C8);

extern u8 identityMatrix[];

void func_800B66B4_1E3764(CutsceneSlotData *arg0) {
    memcpy(&arg0->unk04, identityMatrix, 0x20);

    arg0->unk0.bytes[0] = 0;
    arg0->unk04.unk20_u.unk20_s32 = 0;
    arg0->unk28 = 0;
    arg0->unk2C = 0;
    arg0->unk30 = 0;
    arg0->unk34 = 0;
    arg0->unk38 = 0;
    arg0->unk3C = 0;
    arg0->unk40 = 0;
    arg0->unk44 = 0;

    arg0->unk54 = 0x10000;
    arg0->unk58 = 0x10000;
    arg0->unk5C = 0x10000;
    arg0->unk60 = 0x10000;
    arg0->unk64 = 0x10000;
    arg0->unk68 = 0x10000;

    arg0->unk6C = 0;
    arg0->unk70 = 0;
    arg0->unk74 = 0;

    arg0->unk78 = 0;
    arg0->unk7A = 0;
    arg0->unk7C = 0;
    arg0->unk84 = 0;
    arg0->unk86 = 0;
    arg0->unk80 = 0;
    arg0->unk82 = 0;

    arg0->unk0.bytes[1] = 0;

    arg0->unk88 = 0;
    arg0->angle = 0;
}

INCLUDE_ASM("asm/nonmatchings/1E36C0", setupSlotTransform);

void func_800B68F4_1E39A4(unk_func_800B68F4_1E39A4 *arg0, s32 arg1, s32 arg2, s32 arg3) {
    arg0->unk60 = arg1;
    arg0->unk54 = arg1;
    arg0->unk64 = arg2;
    arg0->unk58 = arg2;
    arg0->unk68 = arg3;
    arg0->unk5C = arg3;
}

void func_800B6910_1E39C0(CutsceneSlotData *arg0, func_800B5E64_1E2F14_arg0 *arg1) {
    s32 temp_s0;
    s32 temp_v0;
    s32 temp_a0;
    s32 temp_v1;
    u16 temp_angle;
    s16 *p80;
    s16 *p82;

    if (((gButtonsPressed & 0x30) == 0x30) && (gControllerInputs & 0x2000)) {
        temp_angle = (arg0->unk78 + 0x800) & 0x1FFF;
        arg0->unk78 = temp_angle;
        arg0->unk7A = temp_angle;
        return;
    }
    if ((gButtonsPressed & 0x2010) == 0x2010) {
        arg0->unk28 = 0;
        arg0->unk80 = 0;
        arg0->unk82 = 0;
        return;
    }
    if (gButtonsPressed & 0x2000) {
        arg0->unk78 = arg0->unk78 - (D_800AB048_A23B8 * 4);
        return;
    }
    if (gButtonsPressed & 0x10) {
        arg0->unk28 = arg0->unk28 + (D_800AB044_A23B4 << 12);
        return;
    }
    if (gButtonsPressed & 0x20) {
        p80 = &arg0->unk80;
        *p80 = (u16)(*p80 + (D_800AB044_A23B4 * 4));
        p82 = &arg0->unk82;
        *p82 = (u16)(*p82 + (D_800AB048_A23B8 * 4));
        return;
    }

    temp_s0 = approximateSin(arg1->unk22) * 4;
    temp_v0 = (approximateCos(arg1->unk22) * 4) >> 8;
    temp_a0 = D_800AB048_A23B8 * 16;
    temp_v1 = -D_800AB044_A23B4 * 16;
    temp_s0 = temp_s0 >> 8;

    arg0->unk04.unk20_u.unk20_s32 = arg0->unk04.unk20_u.unk20_s32 + ((temp_v0 * temp_a0) + (temp_s0 * temp_v1));
    arg0->unk2C = arg0->unk2C + ((-temp_s0 * temp_a0) + (temp_v0 * temp_v1));
}

extern s16 func_800B6618_1E36C8(CutsceneSlotData *, s16, s16, s16);

void func_800B6AB8_1E3B68(CutsceneSlotData *arg0, s16 arg1) {
    s32 temp;
    s16 delta;

    temp = func_800B6618_1E36C8(arg0, 0, arg0->unk7A, arg0->unk78);

    if (arg1 == 1)
        goto set_aa;

    if (arg1 < 2) {
        if (arg1 == 0) {
            delta = 0x100;
        }
    }
    goto check_temp;

set_aa:
    delta = 0xAA;

check_temp:
    if (temp > 0) {
        arg0->unk7C = delta;
        arg0->unk78 -= delta;
    } else if (temp < 0) {
        arg0->unk7C = -delta;
        arg0->unk78 += delta;
    } else {
        arg0->unk7C = 0;
    }
}

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

s32 func_800B734C_1E43FC(CutsceneSlotData *arg0, s32 unused, s16 arg1) {
    s32 retval;
    s32 temp;
    s16 var_v1;

    retval = 0;
    arg0->unk7A = arg1;
    arg0->unk0.Two = 4;
    arg0->unk3C = 0;
    arg0->unk40 = 0;
    arg0->unk44 = 0;
    temp = (s16)func_800B6618_1E36C8(arg0, 0, arg1, arg0->unk78);

    if ((temp >= 0 ? temp : -temp) >= 0xAAB) {
        retval = 2;
        arg0->unk78 = ((u16)arg0->unk78 + 0x1000) & 0x1FFF;
        if (temp > 0) {
            retval = 1;
        }
    }
    func_800B6AB8_1E3B68(arg0, 0);
    var_v1 = arg0->unk7C;
    if (var_v1 != 0) {
        arg0->unk84 = (temp >= 0 ? temp : -temp) / (var_v1 >= 0 ? var_v1 : -var_v1);
    } else {
        arg0->unk84 = 0;
    }
    arg0->unk86 = arg0->unk84;
    return retval;
}

s32 func_800B7450_1E4500(CutsceneSlotData *arg0, SceneModel *unused, s16 arg2, s16 arg3, u16 arg4) {
    s32 temp_v0;
    s32 temp_a1;
    s32 var_s2;
    s32 temp_lo;
    s32 abs_lo;
    s32 temp_v0_2;
    s16 arg4_s16;

    var_s2 = 0;
    arg0->unk0.Two = 4;
    arg0->unk3C = 0;
    arg0->unk40 = 0;
    arg0->unk44 = 0;
    arg0->unk7A = arg2;
    temp_v0 = (s16)func_800B6618_1E36C8(arg0, arg3, arg2, arg0->unk78);

    if ((temp_v0 >= 0 ? temp_v0 : -temp_v0) >= 0xAAB) {
        var_s2 = 2;
        arg0->unk78 = ((u16)arg0->unk78 + 0x1000) & 0x1FFF;
        if (temp_v0 > 0) {
            var_s2 = 1;
        }
    }
    temp_a1 = (s16)func_800B6618_1E36C8(arg0, 0, arg0->unk7A, arg0->unk78);
    arg4_s16 = (s16)arg4;
    arg0->unk84 = arg4;
    arg0->unk86 = arg4;
    if (arg4_s16 != 0) {
        temp_lo = temp_a1 / arg4_s16;
        arg0->unk7C = temp_lo;
        abs_lo = (s16)temp_lo >= 0 ? (s16)temp_lo : -(s16)temp_lo;
        if (abs_lo >= 0x101) {
            s16 result;
            if ((s16)temp_lo != 0) {
                result = (((s32)(temp_lo << 0x10) >> 0x1F) & ~0xFF) | 0x100;
            } else {
                result = 0;
            }
            arg0->unk7C = result;
            temp_v0_2 = temp_a1 >= 0 ? temp_a1 : -temp_a1;
            if (temp_v0_2 < 0) {
                temp_v0_2 += 0xFF;
            }
            temp_v0_2 = temp_v0_2 >> 8;
            arg0->unk84 = temp_v0_2;
            arg0->unk86 = temp_v0_2;
        }
    } else {
        arg0->unk7C = 0;
    }
    return var_s2;
}

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

void func_800B7760_1E4810(CutsceneSlotData *arg0, s32 arg1, s16 arg2) {
    s32 diff;
    s32 delta;

    if (arg2 > 0) {
        diff = arg1 - arg0->unk54;
        delta = diff / arg2;
        arg0->unk60 = arg1;
        arg0->unk6C = delta;
    } else {
        arg0->unk54 = arg1;
        arg0->unk60 = arg1;
        arg0->unk6C = 0;
    }
}

void func_800B77C4_1E4874(CutsceneSlotData *arg0, s32 arg1, s16 arg2) {
    if (arg2 > 0) {
        arg0->unk64 = arg1;
        arg0->unk70 = (arg1 - arg0->unk58) / arg2;
    } else {
        arg0->unk58 = arg1;
        arg0->unk64 = arg1;
        arg0->unk70 = 0;
    }
}

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

void func_800B7914_1E49C4(CutsceneSlotData *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    volatile s32 *ptr30;
    volatile s32 *ptr34;
    volatile s32 *ptr38;
    s16 temp_v0;
    s16 var_t0;
    s32 temp_a3;
    s32 temp_t1;
    s32 temp_a1;
    s32 div1, div2, div3;

    temp_v0 = arg4 + 1;
    var_t0 = temp_v0;
    if ((s32)(temp_v0 << 16) <= 0) {
        var_t0 = 1;
    }
    ptr30 = &arg0->unk30;
    *ptr30 = arg1;
    temp_t1 = *ptr30 - arg0->unk04.unk20_u.unk20_s32;
    div1 = temp_t1 / (s16)var_t0;
    ptr34 = &arg0->unk34;
    *ptr34 = arg2;
    temp_a1 = *ptr34 - arg0->unk28;
    div2 = temp_a1 / (s16)var_t0;
    ptr38 = &arg0->unk38;
    *ptr38 = arg3;
    temp_a3 = *ptr38 - arg0->unk2C;
    div3 = temp_a3 / (s16)var_t0;
    arg0->unk84 = var_t0;
    arg0->unk86 = var_t0;
    arg0->angle = 0;
    arg0->unk0.Two = 8;
    arg0->unk3C = div1;
    arg0->unk40 = div2;
    arg0->unk44 = div3;
    if ((u32)temp_t1 < 1 && (u32)temp_a3 < 1) {
    } else {
        arg0->unk7A = atan2Fixed(temp_t1, temp_a3) & 0x1FFF;
    }
    func_800B6AB8_1E3B68(arg0, 0);
}

void func_800B7A60_1E4B10(
    CutsceneSlotData *arg0,
    SceneModel *unused,
    s32 targetX,
    s32 targetY,
    s32 targetZ,
    s16 duration
) {
    s16 temp_v0;
    s16 var_t1;

    temp_v0 = duration + 1;
    var_t1 = temp_v0;
    if ((temp_v0 << 16) <= 0) {
        var_t1 = 1;
    }

    arg0->unk30 = targetX;
    arg0->unk34 = targetY;
    arg0->unk38 = targetZ;

    arg0->unk3C = (arg0->unk30 - arg0->unk04.unk20_u.unk20_s32) / var_t1;
    arg0->unk40 = (arg0->unk34 - arg0->unk28) / var_t1;
    arg0->unk44 = (arg0->unk38 - arg0->unk2C) / var_t1;

    arg0->unk84 = var_t1;
    arg0->unk86 = var_t1;
    arg0->angle = 0;
    arg0->unk7C = 0;
    arg0->unk7E = 0;
    arg0->unk0.bytes[0] = 1;
    arg0->unk7A = arg0->unk78;
}

void func_800B7B70_1E4C20(CutsceneSlotData *arg0, s32 *arg1, s16 arg2, s32 arg3, s32 arg4) {
    s16 duration;
    s32 delta_x, delta_z;

    if (arg2 == 0) {
        arg2 = 1;
    }

    duration = arg2;

    arg0->unk86 = duration;
    arg0->unk84 = duration;
    arg0->unk0.Two = 10;

    arg0->unk30 = arg1[0];
    arg0->unk34 = arg1[1];
    arg0->unk38 = arg1[2];

    delta_x = arg1[0] - arg0->unk04.unk20_u.unk20_s32;
    arg0->unk3C = delta_x / duration;
    arg0->unk48 = delta_x / duration;

    arg0->unk40 = arg3;
    arg0->unk4C = arg3;

    delta_z = arg1[2] - arg0->unk2C;
    arg0->unk44 = delta_z / duration;
    arg0->unk50 = delta_z / duration;

    arg0->unkA0 = arg4;
}

INCLUDE_ASM("asm/nonmatchings/1E36C0", func_800B7C48_1E4CF8);

extern s32 func_800B7C48_1E4CF8(CutsceneSlotData *, SceneModel *);

s16 func_800B826C_1E531C(CutsceneSlotData *arg0, SceneModel *arg1) {
    s16 temp_v0;
    s16 temp_a0;
    s32 var_s2;

    var_s2 = 0;
    if (arg0->unk84 > 0) {
        arg0->unk04.unk20_u.unk20_s32 += arg0->unk3C;
        arg0->unk28 += arg0->unk40;
        arg0->unk2C += arg0->unk44;
        arg0->unk78 += arg0->unk7C;

        temp_v0 = func_800B6618_1E36C8(arg0, 0, arg0->unk7A, arg0->unk78);
        temp_a0 = arg0->unk7C;

        if (((temp_a0 > 0) & (temp_v0 < 0)) || ((temp_a0 < 0) & (temp_v0 > 0))) {
            arg0->unk7C = 0;
            arg0->unk78 = arg0->unk7A;
        } else if (temp_a0 == 0) {
            arg0->unk78 = arg0->unk7A;
        }

        var_s2 = 1;
        arg0->unk84--;
    } else {
        arg0->unk3C = 0;
        arg0->unk40 = 0;
        arg0->unk44 = 0;
        arg0->unk7C = 0;
        arg0->unk78 = arg0->unk7A;

        if ((arg1->unk38 != -1) && (arg1->unk3E <= 0)) {
            func_800021B8_2DB8(arg1, arg1->unk38);
        }
        arg0->unk0.Two = 0;
    }
    return var_s2 | func_800B7C48_1E4CF8(arg0, arg1);
}

s32 func_800B83B8_1E5468(CutsceneSlotData *arg0, StateEntry *arg1) {
    s32 result;
    s16 nextIndex;

    result = 0;
    if (arg0->unk84 > 0) {
        arg0->unk78 += arg0->unk7C;
        arg0->unk84--;
        result = 1;
    } else {
        arg0->unk7C = 0;
        nextIndex = arg1->next_index;
        if ((nextIndex == -1) || ((s8)arg1->unk3E > 0)) {
            arg0->unk0.bytes[0] = 0;
        } else {
            func_800021B8_2DB8((SceneModel *)arg1, nextIndex);
            arg0->unk0.bytes[0] = 0;
        }
    }
    arg0->unk7A = arg0->unk78;
    return result;
}

INCLUDE_ASM("asm/nonmatchings/1E36C0", func_800B844C_1E54FC);

INCLUDE_ASM("asm/nonmatchings/1E36C0", func_800B8874_1E5924);

s32 func_800B8AC4_1E5B74(CutsceneSlotData *arg0, SceneModel *arg1) {
    s32 angle_new;
    s32 cosVal;
    s32 sinVal;
    s32 scaledCos;
    s32 scaledSin;
    s32 rotResult;
    s32 *anglePtr = (s32 *)&arg0->unk9C_low;
    s16 temp;

    if (arg0->unk84 > 0 || arg0->unk86 == -1) {
        sinVal = *anglePtr;
        *anglePtr = (angle_new = sinVal + arg0->unk94);
        cosVal = approximateCos((s16)angle_new);
        scaledCos = cosVal << 2;
        scaledCos = scaledCos >> 8;
        sinVal = approximateSin(arg0->unk9E);
        scaledSin = (sinVal << 2) >> 8;

        // Store scaled results
        arg0->unk2C = scaledCos * (arg0->unk98 >> 8);
        arg0->unk04.unk20_u.unk20_s32 = scaledSin * (arg0->unk98 >> 8);

        // Update target angle based on direction
        if (arg0->unk94 > 0) {
            angle_new = *anglePtr + 0x800;
            arg0->unk7A = angle_new & 0x1FFF;
        } else if (arg0->unk94 < 0) {
            angle_new = *anglePtr - 0x800;
            arg0->unk7A = angle_new & 0x1FFF;
        }

        // Update rotation and check result
        arg0->unk78 += arg0->unk7C;
        rotResult = func_800B6618_1E36C8(arg0, 0, arg0->unk7A, arg0->unk78);

        // Check for rotation completion
        if ((arg0->unk7C > 0 && rotResult < 0) || (arg0->unk7C < 0 && rotResult > 0)) {
            arg0->unk7C = 0;
            arg0->unk78 = arg0->unk7A;
        } else if (arg0->unk7C == 0) {
            arg0->unk78 = arg0->unk7A;
        }
    } else {
        temp = arg1->unk38;
        if (temp != -1) {
            func_800021B8_2DB8(arg1, temp);
        }
        arg0->unk0.bytes[0] = 0;
    }

    if (arg0->unk84 > 0) {
        arg0->unk84--;
    }

    return 1;
}

void func_800B8C3C_1E5CEC(CutsceneSlotData *arg0) {
    s32 temp;

    // First section: unk54 with velocity unk6C and max unk60
    temp = arg0->unk6C;
    if (temp > 0) {
        arg0->unk54 += temp;
        if (arg0->unk54 > arg0->unk60) {
            arg0->unk54 = arg0->unk60;
            arg0->unk6C = 0;
        }
    } else if (temp < 0) {
        arg0->unk54 += temp;
        if (arg0->unk54 < arg0->unk60) {
            arg0->unk54 = arg0->unk60;
            arg0->unk6C = 0;
        }
    }

    // Second section: unk58 with velocity unk70 and max unk64
    temp = arg0->unk70;
    if (temp > 0) {
        arg0->unk58 += temp;
        if (arg0->unk58 > arg0->unk64) {
            arg0->unk58 = arg0->unk64;
            arg0->unk70 = 0;
        }
    } else if (temp < 0) {
        arg0->unk58 += temp;
        if (arg0->unk58 < arg0->unk64) {
            arg0->unk58 = arg0->unk64;
            arg0->unk70 = 0;
        }
    }

    // Third section: unk5C with velocity unk74 and max unk68
    temp = arg0->unk74;
    if (temp > 0) {
        arg0->unk5C += temp;
        if (arg0->unk5C > arg0->unk68) {
            arg0->unk5C = arg0->unk68;
            arg0->unk74 = 0;
        }
    } else if (temp < 0) {
        arg0->unk5C += temp;
        if (arg0->unk5C < arg0->unk68) {
            arg0->unk5C = arg0->unk68;
            arg0->unk74 = 0;
        }
    }
}

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

s32 func_800B8DA8_1E5E58(CutsceneSlotData *arg0) {
    arg0->unk04.unk20_u.unk20_s32 += arg0->unk3C;
    arg0->unk28 += arg0->unk40;
    arg0->unk2C += arg0->unk44;

    if (arg0->unk40 < 0) {
        if (arg0->unk28 < 0) {
            arg0->unk40 = 0;
            arg0->unk28 = 0;
            arg0->unkA0 = 0;
        }
    }

    arg0->unk40 += arg0->unkA0;

    if (arg0->unk84 == 0) {
        arg0->unk0.bytes[0] = 0;
    } else {
        arg0->unk84--;
    }

    return 1;
}

INCLUDE_ASM("asm/nonmatchings/1E36C0", syncModelFromSlot);
