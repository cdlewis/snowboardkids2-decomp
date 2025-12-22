#include "1E64A0.h"
#include "36B80.h"
#include "3E160.h"
#include "90F0.h"
#include "B040.h"
#include "common.h"
#include "cutscene/cutscene_sys2.h"
#include "task_scheduler.h"

extern void func_800B99A0_1E6A50(void);
extern s16 D_800BADF0_1E7EA0;
extern s16 D_800BADF2_1E7EA2;

void func_800B9C20_1E6CD0(cutsceneSys2Wait_exec_asset *arg0);

s32 func_800B93F0_1E64A0(s32 arg0) {
    s32 result;

    if (arg0 == 0) {
        arg0 = 1;
    }

    result = 0x4000000 / arg0;

    if (result >= 0x4000) {
        result = 0x4000;
    }

    return result;
}

void func_800B9440_1E64F0(cutsceneSys2Wait_exec_asset *arg0) {
    s32 i;
    s32 offset;
    s32 one;

    i = 0;
    one = 1;
    offset = 0;

    do {
        arg0->slots[i].unkA = func_800B93F0_1E64A0(arg0->unk74);
        arg0->slots[i].unkC = func_800B93F0_1E64A0(arg0->unk78);

        if (arg0->slots[i].unkA == 0) {
            arg0->slots[i].unkA = one;
        }
        if (arg0->slots[i].unkC == 0) {
            arg0->slots[i].unkC = one;
        }

        arg0->slots[i].unk0 = arg0->unk10 + ((*(s16 *)((u8 *)&D_800BADF0_1E7EA0 + offset) << 10) / arg0->slots[i].unkA);
        arg0->slots[i].unk2 = arg0->unk12 + ((*(s16 *)((u8 *)&D_800BADF2_1E7EA2 + offset) << 10) / arg0->slots[i].unkC);

        i++;
        offset += 4;
    } while (i < 4);
}

INCLUDE_ASM("asm/nonmatchings/1E64A0", func_800B956C_1E661C);

s32 func_800B9680_1E6730(cutsceneSys2Wait_exec_asset *arg0) {
    arg0->unk80 = 0x6600;
    arg0->unk7C = 0x8000;
    func_800B9440_1E64F0(arg0);
    func_800585C8_591C8(0x2C);
    return 2;
}

s32 func_800B96B4_1E6764(cutsceneSys2Wait_exec_asset *arg0) {
    s32 result = 2;

    arg0->unk74 += arg0->unk80;

    if (arg0->unk74 > 0x18000) {
        arg0->unk74 = 0x18000;
        result = 3;
    }

    func_800B9440_1E64F0(arg0);

    return result;
}

s32 func_800B9708_1E67B8(cutsceneSys2Wait_exec_asset *arg0) {
    s32 temp_a1;
    s32 temp_v0;
    s32 result;

    temp_a1 = arg0->unk74;
    temp_v0 = ((0x10000 - temp_a1) >> 8) * (arg0->unk7C >> 8);
    result = 3;

    if (temp_v0 != 0) {
        arg0->unk74 = temp_a1 + temp_v0;
    } else {
        arg0->unk74 = 0x10000;
        result = 4;
    }

    func_800B9440_1E64F0(arg0);

    return result;
}

void *func_800B9774_1E6824(cutsceneSys2Wait_exec_asset *arg0) {
    void *result;

    result = func_8000B714_C314(arg0->unkC, arg0->unk86, arg0->unk88);

    if (result == NULL) {
        arg0->unk84 = 6;
    }

    arg0->unk90 = result;
    arg0->unk84 = 5;

    return result;
}

extern s32 gControllerInputs;

void *func_800B97C8_1E6878(cutsceneSys2Wait_exec_asset *arg0) {
    void *var_s1;
    u8 temp_a0;
    u16 temp_v1_2;
    u16 *temp_a3;

    var_s1 = func_8000B714_C314(arg0->unkC, arg0->unk86, arg0->unk88);
    temp_a0 = arg0->unk9D;
    arg0->unk90 = var_s1;

    if ((temp_a0 & 0xFF) != 0x64) {
        temp_a3 = (u16 *)((temp_a0 & 0xFF) * 2 + (s32)var_s1);
        temp_v1_2 = *temp_a3;

        if (temp_v1_2 != 0xFFFF) {
            switch (temp_v1_2) {
                case 0xFFF1:
                case 0xFFFD:
                    arg0->unk9D = arg0->unk9D + 1;
                    break;
                case 0xFFF0:
                    func_800021E8_2DE8(
                        (SceneModel *)arg0->unk4,
                        *(s16 *)((u8 *)temp_a3 + 2),
                        *(s16 *)((u8 *)temp_a3 + 6),
                        *(s8 *)((u8 *)temp_a3 + 5)
                    );
                    arg0->unk9D = arg0->unk9D + 4;
                    break;
                case 0xFFFC:
                    arg0->unk9D = temp_a0 + 2;
                    break;
            }
            arg0->unk9D = arg0->unk9D + 1;
        } else {
            arg0->unk9D = 0x64;
        }
    }

    if (*(s8 *)((u8 *)arg0->unk0 + 0xFF7) != 0) {
        if (gControllerInputs & A_BUTTON) {
            if (arg0->unk9D != 0x64) {
                arg0->unk9D = 0x64;
            } else {
                s16 temp_v0 = arg0->unk88 + 1;
                arg0->unk88 = temp_v0;
                var_s1 = func_8000B714_C314(arg0->unkC, arg0->unk86, temp_v0);
                if (var_s1 == NULL) {
                    *(s8 *)((u8 *)arg0->unk0 + 0xFF7) = 0;
                    arg0->unk84 = 6;
                } else {
                    func_800585C8_591C8(0x2B);
                    arg0->unk9D = 0;
                    arg0->unk84 = 4;
                }
            }
        }
    } else {
        arg0->unk84 = 6;
    }

    return var_s1;
}

void func_800B993C_1E69EC(cutsceneSys2Wait_exec_asset *arg0) {
    arg0->unk84 = 0;
    arg0->unk88 = 0;
    arg0->unk8 = func_80009EF8_AAF8(0);
    arg0->unkA0 = func_80035F80_36B80(1);
    arg0->unkC = func_8000B6B8_C2B8(0);
    setCleanupCallback(&func_800B9C20_1E6CD0);
    setCallback(&func_800B99A0_1E6A50);
}

INCLUDE_ASM("asm/nonmatchings/1E64A0", func_800B99A0_1E6A50);

void func_800B9C20_1E6CD0(cutsceneSys2Wait_exec_asset *arg0) {
    arg0->unkA0 = freeNodeMemory(arg0->unkA0);
    arg0->unk8 = freeNodeMemory(arg0->unk8);
    arg0->unkC = freeNodeMemory(arg0->unkC);
}
