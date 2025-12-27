#include "20F0.h"
#include "common.h"
#include "geometry.h"
#include "task_scheduler.h"

typedef struct {
    /* 0x00 */ SceneModel *unk0;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 unkA;
} Struct69B0;

typedef struct {
    /* 0x00 */ SceneModel *unk0;
    /* 0x04 */ u8 unk4[0x20];
    /* 0x24 */ void *unk24;
    /* 0x28 */ void *unk28;
    /* 0x2C */ void *unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ u8 _pad34[0x8];
} Struct69B0_3C; // Size: 0x3C (60 bytes)

extern s32 D_8009A8A4_9B4A4;
extern u8 identityMatrix[];
extern u8 D_80088640_89240[];
void func_80005E54_6A54(void);
void func_80005FBC_6BBC(Struct69B0_3C *);
void func_8000605C_6C5C(Struct69B0 *);
void func_800061C0_6DC0(void);

void func_80005DB0_69B0(Struct69B0_3C *arg0) {
    s32 i;

    for (i = 0; i < 4; i++) {
        memcpy(arg0[i].unk4, identityMatrix, 0x20);
        arg0[i].unk28 = loadAssetGroupDisplayList(arg0->unk0);
        arg0[i].unk2C = loadAssetGroupVertexData(arg0->unk0);
        arg0[i].unk30 = 0;
        arg0[i].unk24 = D_80088640_89240;
    }
    setCleanupCallback(func_80005FBC_6BBC);
    setCallback(func_80005E54_6A54);
}

INCLUDE_ASM("asm/nonmatchings/69B0", func_80005E54_6A54);

void func_80005FBC_6BBC(Struct69B0_3C *arg0) {
    s32 i = 0;

    do {
        arg0[i].unk28 = freeNodeMemory(arg0[i].unk28);
        arg0[i].unk2C = freeNodeMemory(arg0[i].unk2C);
    } while (++i < 4);
}

void func_80006010_6C10(Struct69B0 *arg0) {
    setCleanupCallback(func_800061C0_6DC0);
    arg0->unkA = 0;
    arg0->unk8 = 0;
    arg0->unk6 = 0;
    arg0->unk4 = 0;
    setCallback(func_8000605C_6C5C);
}

void func_8000605C_6C5C(Struct69B0 *arg0) {
    Mat3x3Padded sp10;
    SceneModel *temp_a0;
    s16 temp_v1_2;
    s32 temp_a0_2;
    s32 temp_v0;
    s32 var_a0;
    s32 var_a1;
    s16 var_v0;
    s32 *s0;

    temp_a0 = arg0->unk0;
    if (temp_a0->isDestroyed == 1) {
        func_80069CF8_6A8F8();
        return;
    }

    switch (temp_a0->unk3D) {
        default:
        case 0:
            arg0->unkA = 0;
            arg0->unk8 = 0;
            arg0->unk6 = 0;
            arg0->unk4 = 0;
            break;
        case 1:
            arg0->unk8 = 0x2000;
            break;
        case 2:
            arg0->unk8 = -0x2000;
            break;
        case 3:
            var_v0 = 0x2000;
            goto block_9;
        case 4:
            var_v0 = -0x2000;
        block_9:
            arg0->unkA = var_v0;
            break;
    }

    temp_v1_2 = arg0->unk4;
    var_a0 = arg0->unk8 - temp_v1_2;
    if (var_a0 < 0) {
        var_a0 += 7;
    }
    var_a1 = arg0->unkA - arg0->unk6;
    temp_a0_2 = var_a0 >> 3;
    if (var_a1 < 0) {
        var_a1 += 7;
    }
    arg0->unk4 = temp_v1_2 + temp_a0_2;
    temp_v0 = var_a1 >> 3;
    arg0->unk6 = (u16)arg0->unk6 + temp_v0;
    if (temp_a0_2 == 0) {
        arg0->unk8 = 0;
        arg0->unk4 = 0;
    }
    if (temp_v0 == 0) {
        arg0->unkA = 0;
        arg0->unk6 = 0;
    }

    s0 = &D_8009A8A4_9B4A4;
    *s0 = 0;
    *(s0 + 1) = (s32)0xFFF00000;
    *(s0 + 2) = 0;

    createRotationMatrixYX(&sp10, (u16)arg0->unk6, (u16)arg0->unk4);

    sp10.unk14 = 0;
    sp10.unk18 = 0x100000;
    sp10.unk1C = 0;

    func_8006B084_6BC84(s0 - 5, &sp10, (u8 *)arg0->unk0 + 0xF0);
}

void func_800061C0_6DC0(void) {
}
