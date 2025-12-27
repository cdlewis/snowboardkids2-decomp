#include "20F0.h"
#include "common.h"
#include "geometry.h"
#include "task_scheduler.h"

typedef struct {
    func_80002B50_3750_arg *unk0;
    DisplayListObject unk4;
    u16 unk40;
    s16 unk42;
    s8 unk44;
    u8 _pad45[3];
    s32 unk48;
    s32 unk4C;
} func_8A40_arg;

extern u8 identityMatrix[];
extern void *D_800882B0_88EB0;
void func_80007EC8_8AC8(func_8A40_arg *);

typedef struct {
    SceneModel *unk0;
    u8 unk4[0x20];
    void *unk24;
    void *unk28;
    void *unk2C;
    s32 unk30;
    u8 _pad34[0x10];
    u8 unk44;
    u8 _pad45[3];
    s32 unk48;
    s32 unk4C;
} func_800080B8_8CB8_arg;

void func_800080B8_8CB8(func_800080B8_8CB8_arg *);

void func_80007E40_8A40(func_800080B8_8CB8_arg *arg0) {
    memcpy(arg0->unk4, identityMatrix, 0x20);
    arg0->unk28 = loadAssetGroupDisplayList(arg0->unk0);
    arg0->unk2C = loadAssetGroupVertexData(arg0->unk0);
    arg0->unk24 = &D_800882B0_88EB0;
    arg0->unk30 = 0;
    arg0->unk44 = 0;
    arg0->unk48 = 0;
    arg0->unk4C = 0xA3D7;
    setCleanupCallback(&func_800080B8_8CB8);
    setCallback(&func_80007EC8_8AC8);
}

void func_80007EC8_8AC8(func_8A40_arg *arg0) {
    Mat3x3Padded sp10;
    Mat3x3Padded sp30;
    s32 div;
    s32 temp;

    if (arg0->unk0->isDestroyed == 1) {
        func_80069CF8_6A8F8();
        return;
    }

    switch (arg0->unk0->actionMode) {
        case 0:
        default:
            arg0->unk42 = 0;
            break;
        case 1:
            div = -arg0->unk42 / 12;
            if (div == 0) {
                arg0->unk42 = 0;
            } else {
                arg0->unk42 += div;
            }
            break;
        case 2:
            div = (0x1C7 - arg0->unk42) / 12;
            if (div == 0) {
                arg0->unk42 = 0x1C7;
            } else {
                arg0->unk42 += div;
            }
            break;
        case 3:
            arg0->unk44 = 1;
            break;
        case 4:
            arg0->unk44 = 0;
            arg0->unk4C = 0xA3D7;
            break;
    }

    if (arg0->unk44 != 0) {
        arg0->unk48 = arg0->unk48 + arg0->unk4C;
        if (arg0->unk48 > 0) {
            arg0->unk4C -= 0x1062;
        } else {
            arg0->unk4C += 0x1062;
        }
    } else {
        temp = -arg0->unk48;
        if (-arg0->unk48 < 0) {
            temp += 0xF;
        }
        arg0->unk48 = temp >> 4;
    }

    createXRotationMatrix(arg0->unk0->unkF0, arg0->unk42);
    arg0->unk0->unk104 = 0;
    arg0->unk0->unk108 = arg0->unk48;
    arg0->unk0->unk10C = 0;

    arg0->unk40 += 0x2AA;
    createYRotationMatrix(&sp10, arg0->unk40);

    sp10.unk14 = 0;
    sp10.unk18 = 0x499999;
    sp10.unk1C = 0;

    func_8006B084_6BC84(&sp10, arg0->unk0->unkF0, &sp30);
    func_8006B084_6BC84(&sp30, arg0->unk0->matrix18, &arg0->unk4);
    func_80002B50_3750(arg0->unk0, &arg0->unk4);
}

void func_800080B8_8CB8(func_800080B8_8CB8_arg *arg0) {
    arg0->unk28 = freeNodeMemory(arg0->unk28);
    arg0->unk2C = freeNodeMemory(arg0->unk2C);
}

void func_800081B0_8DB0(void);
void func_8000811C_8D1C(func_800080B8_8CB8_arg *);

void func_800080F0_8CF0(void) {
    setCleanupCallback(func_800081B0_8DB0);
    setCallback(func_8000811C_8D1C);
}

void func_8000811C_8D1C(func_800080B8_8CB8_arg *arg0) {
    if (arg0->unk0->isDestroyed == 1) {
        func_80069CF8_6A8F8();
        return;
    }

    memcpy(&arg0->unk0->unkF0, identityMatrix, 0x20);

    if (arg0->unk0->actionMode == 0) {
        return;
    }
    if (arg0->unk0->actionMode == 1) {
        createXRotationMatrix(arg0->unk0->unkF0, 0xFCE4);
        arg0->unk0->unk10C += 0x66666;
    }
}

void func_800081B0_8DB0(void) {
}
