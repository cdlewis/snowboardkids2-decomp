#include "common.h"
#include "geometry.h"
#include "task_scheduler.h"

extern u8 identityMatrix[];
extern s32 D_8008C150_8CD50;
extern u16 D_8008C154_8CD54;
extern s32 D_8008C180_8CD80;

void func_80006C3C_783C(void);

typedef struct {
    u8 padding[0x3C];
    s8 unk3C;
    s8 unk3D;
    u8 padding2[0xB2];
    u8 unkF0[0x20];
} func_80006B04_Unk0;

typedef struct {
    func_80006B04_Unk0 *unk0;
    Mat3x3Padded unk4;
    s16 unk24;
    u8 padding[0x2];
    s16 unk28;
} func_80006AB0_76B0_arg;

void func_80006B04_7704(func_80006AB0_76B0_arg *arg0);

void func_80006AB0_76B0(func_80006AB0_76B0_arg *arg0) {
    memcpy(&arg0->unk4, identityMatrix, 0x20);
    arg0->unk24 = 0;
    arg0->unk28 = 0;
    setCleanupCallback(func_80006C3C_783C);
    setCallback(func_80006B04_7704);
}

void func_80006B04_7704(func_80006AB0_76B0_arg *arg0) {
    Mat3x3Padded *matrix;
    s32 temp;

    if (arg0->unk0->unk3C == 1) {
        func_80069CF8_6A8F8();
        return;
    }

    if (arg0->unk0->unk3D == 0) {
        goto copy_identity;
    }
    if (arg0->unk0->unk3D == 1) {
        goto do_rotation;
    }

copy_identity:
    memcpy(&arg0->unk4, identityMatrix, 0x20);
    goto final_copy;

do_rotation:
    if (arg0->unk24 == 0) {
        matrix = &arg0->unk4;
        memcpy(matrix, identityMatrix, 0x20);
        createZRotationMatrix(matrix, (&D_8008C154_8CD54)[arg0->unk28 * 4]);
        temp = (&D_8008C150_8CD50)[arg0->unk28 * 2];
        arg0->unk24 = 4;
        arg0->unk4.unk18 = temp;
        arg0->unk28 = (arg0->unk28 + 1) % D_8008C180_8CD80;
    }
    arg0->unk24 -= 1;

final_copy:
    memcpy(arg0->unk0->unkF0, &arg0->unk4, 0x20);
}

void func_80006C3C_783C(void) {
}
