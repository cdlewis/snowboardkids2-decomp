#include "DC90.h"
#include "36B80.h"
#include "3E160.h"
#include "56910.h"
#include "B040.h"
#include "common.h"
#include "graphics.h"
#include "task_scheduler.h"

extern s32 gControllerInputs;

extern void func_8006D7B0_6E3B0(s32, s16, s16, s16, s16, u8, u8, u8, u8, u8);
extern void *func_8000D144_DD44(DC90TaskStruct *arg0);

void func_8000D2C8_DEC8(DC90TaskStruct *arg0);
void func_8000D448_E048(DC90TaskStruct *arg0);

void func_8000D090_DC90(DC90TaskStruct *arg0) {
    void *temp = arg0->unk28;
    arg0->unk20.asS16 = 0xFF;
    arg0->unk22.asS16 = 0xFF;
    arg0->unk14 = 0;
    arg0->unk16 = 0;
    arg0->unk24 = 0;
    arg0->unkC = 1;
    arg0->unk1C = temp;
}

void func_8000D0BC_DCBC(DC90TaskStruct *arg0) {
    arg0->unk34++;
    if (arg0->unk34 >= 13) {
        arg0->unkC = 2;
    }
    if (arg0->unk3C < 65) {
        arg0->unk3C = 64;
    } else {
        arg0->unk3C -= 16;
    }
}

void func_8000D100_DD00(DC90TaskStruct *arg0) {
    s16 temp;
    s16 temp2;

    if (++arg0->unk38 >= 6) {
        arg0->unkC = 3;
    }

    temp2 = temp = arg0->unk3C;
    arg0->unk3C = (temp < 0x41) ? 0x40 : temp2 - 0x10;
}

void *func_8000D144_DD44(DC90TaskStruct *arg0) {
    void *temp_v0 = func_8000B714_C314((Table_B934 *)arg0->unk8, arg0->unkE, arg0->unk10);
    unsigned long new_var;
    new_var = temp_v0 == 0;
    if (new_var) {
        arg0->unkC = 6;
        arg0->unk18 = temp_v0;
    } else {
        arg0->unk18 = temp_v0;
    }
    if (gControllerInputs & A_BUTTON) {
        func_800585C8_591C8(45);
        arg0->unkC = 4;
    }
    return temp_v0;
}

void func_8000D1BC_DDBC(DC90TaskStruct *arg0) {
    arg0->unk38--;
    if (arg0->unk38 < 2) {
        arg0->unkC = 5;
    }
    if (arg0->unk3E > 0) {
        arg0->unk3E -= 16;
    } else {
        arg0->unk3E = 0;
    }
}

void func_8000D200_DE00(DC90TaskStruct *arg0) {
    arg0->unk34--;
    if (arg0->unk34 < 2) {
        arg0->unkC = 6;
    }
    if (arg0->unk3E > 0) {
        arg0->unk3E -= 16;
    } else {
        arg0->unk3E = 0;
    }
}

void func_8000D244_DE44(DC90TaskStruct *arg0) {
    arg0->unkC = 0;
    arg0->unk10 = 0;
    arg0->unk4 = loadAsset_34F7E0();
    arg0->unk28 = func_80035F80_36B80(1);
    arg0->unk8 = func_8000B6B8_C2B8(2);
    arg0->unk34 = 1;
    arg0->unk38 = 1;
    arg0->unk3C = 0xFF;
    arg0->unk3E = 0xC0;
    func_800585C8_591C8(0x2C);
    setCleanupCallback(&func_8000D448_E048);
    setCallback(&func_8000D2C8_DEC8);
}

void func_8000D2C8_DEC8(DC90TaskStruct *arg0) {
    void *result = 0;
    s32 flag = 0;

    switch (arg0->unkC) {
        case 0:
            func_8000D090_DC90(arg0);
            break;
        case 1:
            func_8000D0BC_DCBC(arg0);
            break;
        case 2:
            func_8000D100_DD00(arg0);
            break;
        case 3:
            result = func_8000D144_DD44(arg0);
            flag = 1;
            break;
        case 4:
            func_8000D1BC_DDBC(arg0);
            break;
        case 5:
            func_8000D200_DE00(arg0);
            break;
        case 6:
            arg0->unk0[0xFF7] = 0;
            func_80069CF8_6A8F8();
            return;
    }

    arg0->unk2C = -(arg0->unk34 << 3);
    arg0->unk30 = -(arg0->unk38 << 3);

    if (result != 0) {
        s16 temp16;
        arg0->unk14 = arg0->unk2C;
        temp16 = arg0->unk30;
        arg0->unk16 = temp16;
        func_80035260_35E60(
            arg0->unk1C,
            arg0->unk18,
            arg0->unk14,
            temp16,
            arg0->unk20.asBytes.low,
            arg0->unk22.asBytes.low,
            arg0->unk24,
            1,
            0
        );
    }

    func_8006D7B0_6E3B0(
        (s32)arg0->unk4,
        (s16)(arg0->unk2C),
        (s16)(arg0->unk30),
        (s16)(arg0->unk34),
        (s16)(arg0->unk38),
        flag,
        (u8)arg0->unk3C,
        (u8)arg0->unk3E,
        1,
        0
    );
}

void func_8000D448_E048(DC90TaskStruct *arg0) {
    arg0->unk28 = freeNodeMemory(arg0->unk28);
    arg0->unk4 = freeNodeMemory(arg0->unk4);
    arg0->unk8 = freeNodeMemory(arg0->unk8);
}
