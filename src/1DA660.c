#include "10AD0.h"
#include "20F0.h"
#include "38C90.h"
#include "6E840.h"
#include "D_800AFE8C_A71FC_type.h"
#include "common.h"
#include "displaylist.h"
#include "gamestate.h"
#include "geometry.h"
#include "overlay.h"
#include "task_scheduler.h"

USE_ASSET(_41A1D0);
USE_ASSET(_41AD80);
USE_ASSET(_422C60);

typedef struct {
    u8 _pad0[0x2C];
    s32 unk2C;
} func_800B0DD0_arg;

typedef struct {
    SceneModel *unk0;
    Mat3x3Padded matrix;
    u16 unk24;
    s16 unk26;
    u8 unk28;
    u8 unk29;
} func_800B08FC_arg;

typedef struct {
    u8 _pad[0x24];
    void *unk24;
    void *unk28;
} func_800B1104_arg;

typedef struct {
    u8 _pad0[0x2C];
    void *unk2C;
} func_800B0D4C_arg;

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
} func_800B0F18_arg;

typedef struct {
    u8 _pad0[0x4];
    void *unk4;
} func_800B0FE0_arg;

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    u8 _padA[2];
    char *unkC;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    u8 _pad16[2];
    char *unk18;
    char unk1C[16];
} func_800B0BEC_arg;

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
    s16 unkA;
    u8 unkC;
    u8 unkD;
    u8 _padE[2];
} func_800B0980_element;

typedef struct {
    func_800B0980_element elements[4];
    u8 unk40[8];
} func_800B0980_container;

typedef struct {
    s16 m[9];
    s16 _pad;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    void *unk20;
    void *unk24;
    void *unk28;
    s32 unk2C;
} func_800B100C_arg;

typedef struct {
    s16 x, y;
} Vec2s;

typedef struct {
    u8 padding[0x592];
    u16 unk592;
    u8 padding2[0xE];
    u8 unk5A2;
} LocalGameState_Player;

typedef struct {
    LocalGameState_Player *players[4];
    u8 pad[0x582];
    u16 unk592[8];
    u8 unk5A2[8];
} LocalGameState;

typedef struct {
    u8 _pad0[0x59A];
    u8 unk59A[8];
    u8 _pad5A2[0x1F];
    u8 unk5C1[8];
} func_800B0A54_allocation;

extern void *renderTextPalette;

void func_800B00C0_1DA660(void);
void func_800B0218(func_800B0980_element *, u8);
void func_800B0DF8_1DB398(void *);
void func_800B0598_1DAB38(func_800B08FC_arg *);
void func_800B05DC_1DAB7C(func_800B08FC_arg *);
void func_800B0964_1DAF04(func_800B08FC_arg *);
void func_800B0638_1DABD8(func_800B08FC_arg *);
void func_800B0720_1DACC0(func_800B08FC_arg *);
void func_800B0A54_1DAFF4(func_800B0980_container *);
void func_800B0BC0_1DB160(func_800B0FE0_arg *);
void func_800B0E94_1DB434(void *);
void func_800B0EEC_1DB48C(func_800B0FE0_arg *);
void func_800B0F88_1DB528(void *);
void func_800B0FE0_1DB580(func_800B0FE0_arg *);
void func_800B0DD0_1DB370(func_800B0DD0_arg *);
void func_800B10D4_1DB674(void *);
void func_800B1104_1DB6A4(func_800B1104_arg *);
void func_800B0C54_1DB1F4(func_800B0BEC_arg *);

extern char D_800B11F0_1DB790[];
extern s16 D_800B1160_1DB700[];
extern s16 D_800B1162_1DB702[];
extern Vec2s D_800B11A0_1DB740[];
extern s16 D_800B11C2_1DB762[];
extern u8 D_1DC0D0[];
extern u8 D_1DC260[];
extern u8 identityMatrix[];
extern void *D_800B1140_1DB6E0;
extern char D_800B115C_1DB6FC[];
extern s32 gButtonsPressed;

INCLUDE_ASM("asm/nonmatchings/1DA660", func_800B00C0_1DA660);

void func_800B0218_1DA7B8(func_800B0980_element *arg0, u8 arg1) {
    LocalGameState *allocation;
    u8 playerState;
    u8 count;
    s32 i;
    volatile u8 padding[8];

    allocation = (LocalGameState *)getCurrentAllocation();

    arg0->unk0 = D_800B11A0_1DB740[allocation->unk592[arg1]].x;
    arg0->unk2 = D_800B11A0_1DB740[allocation->unk592[arg1]].y;

    if (allocation->unk5A2[arg1] >= 3) {
        arg0->unk0 += 0x10;
    }

    if (!(allocation->unk5A2[arg1] & 1)) {
        arg0->unk2 += 0x10;
    }

    count = 0;
    for (i = 0; i < D_800AFE8C_A71FC->unk8; i++) {
        if (allocation->unk592[arg1] == allocation->unk592[i]) {
            count++;
        }
    }

    playerState = allocation->unk5A2[arg1];
    if ((playerState == 3) && (count == 3)) {
        arg0->unk2 += 0x10;
    }

    count--;
    arg0->unk8 = D_800B11C2_1DB762[count * 4 + allocation->unk5A2[arg1]] + arg1;
}

void func_800B0368_1DA908(func_800B08FC_arg *arg0) {
    GameState *allocation;
    Mat3x3Padded *matrix;
    u16 modelIndex;
    s32 pad[2];

    allocation = getCurrentAllocation();
    modelIndex = arg0->unk24 + 0x32;
    arg0->unk28 = 0;

    if (arg0->unk24 == 6) {
        if (allocation->unk5B2 == -1) {
            arg0->unk28 = 2;
        }
    }

    if (arg0->unk24 == 7) {
        if (allocation->unk5AE == -1) {
            arg0->unk28 = 1;
            modelIndex = 0xF;
        }
    }

    if (arg0->unk24 == 8) {
        if (allocation->unk5AD == -1) {
            arg0->unk28 = 1;
            modelIndex = 0xF;
        }
    }

    matrix = &arg0->matrix;
    arg0->unk0 = func_8000198C_258C(modelIndex, allocation);
    memcpy(matrix, identityMatrix, 0x20);

    arg0->matrix.unk14 = D_800B1160_1DB700[arg0->unk24 * 2] << 16;
    arg0->matrix.unk1C = D_800B1162_1DB702[arg0->unk24 * 2] << 16;

    if (arg0->matrix.unk1C == (s32)0xFF900000) {
        arg0->matrix.unk18 = (s32)0xFFE80000;
    }

    createYRotationMatrix(matrix, atan2Fixed(arg0->matrix.unk14, arg0->matrix.unk1C));

    if (arg0->unk28 != 2) {
        func_800014FC_20FC(arg0->unk0, arg0->matrix.unk18);
        func_8000150C_210C(arg0->unk0);
    }

    if (arg0->unk24 == 0) {
        createXRotationMatrix((s16(*)[3])matrix, 0x100);
    } else if (arg0->unk24 == 4) {
        arg0->matrix.unk1C = arg0->matrix.unk1C - 0x10;
    } else if (arg0->unk24 == 7) {
        if (arg0->unk28 == 0) {
            arg0->matrix.unk1C = arg0->matrix.unk1C + 0x40000;
        } else {
            arg0->unk26 = 0x90;
            goto after_unk26;
        }
    }

    if (arg0->unk28 == 0) {
        arg0->unk26 = 0;
    } else {
        arg0->unk26 = 0x90;
    }

after_unk26:
    if (arg0->unk24 >= 7 && arg0->unk28 != 0) {
        if (arg0->unk24 == 8) {
            scaleMatrix(&arg0->matrix, 0x1000, 0x1000, 0x1000);
            arg0->matrix.unk14 = arg0->matrix.unk14 + 0x80000;
        } else {
            scaleMatrix(&arg0->matrix, 0x800, 0x800, 0x800);
        }
    }

    setCleanupCallback(func_800B0964_1DAF04);
    setCallback(func_800B0598_1DAB38);
}

void func_800B0598_1DAB38(func_800B08FC_arg *arg0) {
    GameState *allocation = getCurrentAllocation();

    if (arg0->unk24 == 6) {
        allocation->unk5B3 = 0;
    }
    setCallback(func_800B05DC_1DAB7C);
}

void func_800B05DC_1DAB7C(func_800B08FC_arg *arg0) {
    applyTransformToModel(arg0->unk0, &arg0->matrix);
    if (arg0->unk28 != 2) {
        func_800021B8_2DB8(arg0->unk0, arg0->unk26);
        updateModelGeometry(arg0->unk0);
    }
    setCallback(func_800B0638_1DABD8);
}

void func_800B0638_1DABD8(func_800B08FC_arg *arg0) {
    GameState *allocation;
    s32 i;
    D_800AFE8C_A71FC_type *ptr;
    s32 pad[2];
    s32 count;

    allocation = getCurrentAllocation();

    if (arg0->unk28 != 2) {
        applyTransformToModel(arg0->unk0, &arg0->matrix);
        clearModelRotation(arg0->unk0);
        updateModelGeometry(arg0->unk0);
    }

    if (arg0->unk28 == 0) {
        ptr = D_800AFE8C_A71FC;
        count = ptr->unk8;
        if (count > 0) {
            i = 0;
            do {
                if (ptr->unk9[i] == arg0->unk24) {
                    u8 state = allocation->unk59A[i];
                    if (state == 1 || state == 3) {
                        arg0->unk29 = 0;
                        setCallback(func_800B0720_1DACC0);
                        return;
                    }
                }
                ptr = D_800AFE8C_A71FC;
                i++;
            } while (i < ptr->unk8);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/1DA660", func_800B0720_1DACC0);

void func_800B08FC_1DAE9C(func_800B08FC_arg *arg0) {
    s32 result;

    result = clearModelRotation(arg0->unk0);
    updateModelGeometry(arg0->unk0);

    if (result != 0) {
        arg0->unk26 = 0;
        func_800021B8_2DB8(arg0->unk0, 0);
        func_80001688_2288(arg0->unk0, -1);
        setCallback(func_800B0638_1DABD8);
    }
}

void func_800B0964_1DAF04(func_800B08FC_arg *arg0) {
    func_80002014_2C14(arg0->unk0);
}

void func_800B0980_1DAF20(func_800B0980_container *arg0) {
    s32 i;
    void *allocation;

    getCurrentAllocation();
    allocation = dmaRequestAndUpdateStateWithSize(&_41A1D0_ROM_START, &_41AD80_ROM_START, 0x1B48);

    for (i = 0; i < D_800AFE8C_A71FC->unk8; i++) {
        arg0->elements[i].unk4 = allocation;
        arg0->elements[i].unkA = 0xFF;
        arg0->elements[i].unkD = 0;
        arg0->elements[i].unkC = 0;
        arg0->unk40[i] = 0;
        func_800B0218_1DA7B8(&arg0->elements[i], i);
    }

    setCleanupCallback(func_800B0BC0_1DB160);
    setCallback(func_800B0A54_1DAFF4);
}

void func_800B0A54_1DAFF4(func_800B0980_container *arg0) {
    func_800B0A54_allocation *allocation;
    s32 i;
    u8 state;

    allocation = (func_800B0A54_allocation *)getCurrentAllocation();
    func_800B00C0_1DA660();

    for (i = 0; i < D_800AFE8C_A71FC->unk8; i++) {
        func_800B0218_1DA7B8(&arg0->elements[i], i);
        state = allocation->unk59A[i];

        if (state == 10) {
            arg0->elements[i].unkA = 0xFF;
            arg0->unk40[i] = 0;
            if (allocation->unk5C1[i] & 1) {
                arg0->elements[i].unkD = 0xFF;
            } else {
                arg0->elements[i].unkD = 0;
            }
        } else if (state == 0) {
            arg0->unk40[i] = arg0->unk40[i] % 30;
            if (arg0->unk40[i] < 15) {
                arg0->elements[i].unkA = arg0->elements[i].unkA - 8;
            } else {
                arg0->elements[i].unkA = arg0->elements[i].unkA + 8;
            }
            arg0->unk40[i] = arg0->unk40[i] + 1;
        } else {
            arg0->unk40[i] = 0;
            arg0->elements[i].unkA = 0xFF;
        }

        if (allocation->unk59A[i] == 2) {
            allocation->unk5C1[i] = 0;
            arg0->elements[i].unkD = 0;
            arg0->unk40[i] = 0;
            arg0->elements[i].unkA = 0xFF;
        }

        debugEnqueueCallback(8, 0, func_80012004_12C04, &arg0->elements[i]);
    }
}

void func_800B0BC0_1DB160(func_800B0FE0_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_800B0BEC_1DB18C(func_800B0BEC_arg *arg0) {
    arg0->unk0 = -7;
    arg0->unk2 = -7;
    arg0->unk4 = -120;
    arg0->unk6 = -104;
    arg0->unkC = arg0->unk1C;
    arg0->unk8 = 0;
    arg0->unk14 = 0;
    arg0->unk18 = D_800B115C_1DB6FC;
    arg0->unk10 = arg0->unk0;
    arg0->unk12 = arg0->unk2;
    setCallback(func_800B0C54_1DB1F4);
}

void func_800B0C54_1DB1F4(func_800B0BEC_arg *arg0) {
    s32 *buttons = &gButtonsPressed;

    if (*buttons & U_JPAD) {
        arg0->unk2--;
    }
    if (*buttons & D_JPAD) {
        arg0->unk2++;
    }
    if (*buttons & R_JPAD) {
        arg0->unk0++;
    }
    if (*buttons & L_JPAD) {
        arg0->unk0--;
    }

    sprintf(arg0->unk1C, D_800B11F0_1DB790, arg0->unk0 + 7, arg0->unk2 + 7);

    debugEnqueueCallback(0, 7, &renderTextPalette, &arg0->unk4);

    arg0->unk10 = arg0->unk0;
    arg0->unk12 = arg0->unk2;

    debugEnqueueCallback(0, 7, &renderTextPalette, &arg0->unk10);
}

void func_800B0D4C_1DB2EC(func_800B0D4C_arg *arg0) {
    arg0->unk2C = freeNodeMemory(arg0->unk2C);
}

void func_800B0D78_1DB318(func_800B0D4C_arg *arg0) {
    arg0->unk2C = dmaRequestAndUpdateStateWithSize(&_41AD80_ROM_START, &_41AD80_ROM_END, 0x13FF0);
    setCleanupCallback(func_800B0D4C_1DB2EC);
    setCallback(func_800B0DD0_1DB370);
}

void func_800B0DD0_1DB370(func_800B0DD0_arg *arg0) {
    func_800394BC_3A0BC(arg0, arg0->unk2C);
    setCallback(&func_800B0DF8_1DB398);
}

void func_800B0DF8_1DB398(void *arg0) {
    debugEnqueueCallback(9, 0, func_80038420_39020, arg0);
}

void func_800B0E24_1DB3C4(func_800B0F18_arg *arg0) {
    void *temp;
    getCurrentAllocation();
    temp = dmaRequestAndUpdateStateWithSize(&_41A1D0_ROM_START, &_41A1D0_ROM_END, 0x1B48);
    arg0->unk0 = -44;
    arg0->unk2 = -20;
    arg0->unk8 = 13;
    arg0->unk4 = temp;
    setCleanupCallback(func_800B0EEC_1DB48C);
    setCallbackWithContinue(func_800B0E94_1DB434);
}

void func_800B0E94_1DB434(void *arg0) {
    GameState *state = getCurrentAllocation();
    if (state->unk59A[0] == 3) {
        debugEnqueueCallback(8, 0, func_8000FED0_10AD0, arg0);
    } else {
        func_80069CF8_6A8F8();
    }
}

void func_800B0EEC_1DB48C(func_800B0FE0_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_800B0F18_1DB4B8(func_800B0F18_arg *arg0) {
    void *temp;
    getCurrentAllocation();
    temp = dmaRequestAndUpdateStateWithSize(&_41A1D0_ROM_START, &_41A1D0_ROM_END, 0x1B48);
    arg0->unk0 = -76;
    arg0->unk2 = -8;
    arg0->unk8 = 12;
    arg0->unk4 = temp;
    setCleanupCallback(func_800B0FE0_1DB580);
    setCallbackWithContinue(func_800B0F88_1DB528);
}

void func_800B0F88_1DB528(void *arg0) {
    GameState *state = getCurrentAllocation();
    if (state->unk59A[0] == 2) {
        debugEnqueueCallback(8, 0, func_8000FED0_10AD0, arg0);
    } else {
        func_80069CF8_6A8F8();
    }
}

void func_800B0FE0_1DB580(func_800B0FE0_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_800B100C_1DB5AC(func_800B100C_arg *arg0) {
    void *temp1;
    void *temp2;

    temp1 = dmaRequestAndUpdateState(D_1DC0D0, D_1DC260);
    temp2 = dmaRequestAndUpdateState(&_422C60_ROM_START, &_422C60_ROM_END);
    setCleanupCallback(func_800B1104_1DB6A4);

    memcpy(arg0, identityMatrix, 0x20);
    arg0->unk20 = &D_800B1140_1DB6E0;
    arg0->unk14 = 0x2C0000;
    arg0->unk1C = (s32)0xFF9F0000;
    arg0->unk24 = temp1;
    arg0->unk28 = temp2;
    arg0->unk2C = 0;
    arg0->unk18 = (s32)0xFFF40000;

    createZRotationMatrix((Mat3x3Padded *)arg0, 0x1F50);
    setCallback(func_800B10D4_1DB674);
}

void func_800B10D4_1DB674(void *arg0) {
    getCurrentAllocation();
    func_800630F0_63CF0(0, arg0);
}

void func_800B1104_1DB6A4(func_800B1104_arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}
