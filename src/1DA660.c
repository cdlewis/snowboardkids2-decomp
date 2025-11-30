#include "20F0.h"
#include "6E840.h"
#include "D_800AFE8C_A71FC_type.h"
#include "common.h"
#include "gamestate.h"
#include "geometry.h"
#include "overlay.h"
#include "task_scheduler.h"

USE_ASSET(_41A1D0);
USE_ASSET(_41AD80);
USE_ASSET(_422C60);

extern u8 D_1DC0D0[];
extern u8 D_1DC260[];
extern s32 gButtonsPressed;
extern char D_800B11F0_1DB790[];

extern void func_800394BC_3A0BC(void *, s32);
extern void func_8000FED0_10AD0(void);
extern void func_800630F0_63CF0(s32, void *);
extern void debugEnqueueCallback(u16, u8, void *, void *);
extern void func_80038420_39020(void);
extern void func_800B0218_1DA7B8(void *, u8);
extern void renderTextPalette(void *);
extern u8 identityMatrix[];
extern void *D_800B1140_1DB6E0;
extern char D_800B115C_1DB6FC[];
typedef struct {
    u8 _pad0[0x2C];
    s32 unk2C;
} func_800B0DD0_arg;

typedef struct {
    SceneModel *unk0;
    u8 _pad4[0x20];
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

void func_800B0DF8_1DB398(void *);
void func_800B05DC_1DAB7C(func_800B08FC_arg *);
void func_800B0638_1DABD8(func_800B08FC_arg *);
void func_800B0720_1DACC0(func_800B08FC_arg *);
void func_800B0A54_1DAFF4(void *);
void func_800B0BC0_1DB160(func_800B0FE0_arg *);
void func_800B0E94_1DB434(void *);
void func_800B0EEC_1DB48C(func_800B0FE0_arg *);
void func_800B0F88_1DB528(void *);
void func_800B0FE0_1DB580(func_800B0FE0_arg *);
void func_800B0DD0_1DB370(func_800B0DD0_arg *);
void func_800B10D4_1DB674(void *);
void func_800B1104_1DB6A4(func_800B1104_arg *);
void func_800B0C54_1DB1F4(func_800B0BEC_arg *);

INCLUDE_ASM("asm/nonmatchings/1DA660", func_800B00C0_1DA660);

INCLUDE_ASM("asm/nonmatchings/1DA660", func_800B0218_1DA7B8);

INCLUDE_ASM("asm/nonmatchings/1DA660", func_800B0368_1DA908);

void func_800B0598_1DAB38(func_800B08FC_arg *arg0) {
    GameState *allocation = getCurrentAllocation();

    if (arg0->unk24 == 6) {
        allocation->unk5B3 = 0;
    }
    setCallback(func_800B05DC_1DAB7C);
}

void func_800B05DC_1DAB7C(func_800B08FC_arg *arg0) {
    applyTransformToModel(arg0->unk0, (applyTransformToModel_arg1 *)&arg0->_pad4);
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
        applyTransformToModel(arg0->unk0, (applyTransformToModel_arg1 *)&arg0->_pad4);
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

void func_800B0980_1DAF20(func_800B0980_element *arg0) {
    s32 i;
    void *allocation;

    getCurrentAllocation();
    allocation = dmaRequestAndUpdateStateWithSize(&_41A1D0_ROM_START, &_41AD80_ROM_START, 0x1B48);

    for (i = 0; i < D_800AFE8C_A71FC->unk8; i++) {
        arg0[i].unk4 = allocation;
        arg0[i].unkA = 0xFF;
        arg0[i].unkD = 0;
        arg0[i].unkC = 0;
        *((u8 *)arg0 + i + 0x40) = 0;
        func_800B0218_1DA7B8(&arg0[i], i);
    }

    setCleanupCallback(func_800B0BC0_1DB160);
    setCallback(func_800B0A54_1DAFF4);
}

INCLUDE_ASM("asm/nonmatchings/1DA660", func_800B0A54_1DAFF4);

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

    if (*buttons & 0x800) {
        arg0->unk2--;
    }
    if (*buttons & 0x400) {
        arg0->unk2++;
    }
    if (*buttons & 0x100) {
        arg0->unk0++;
    }
    if (*buttons & 0x200) {
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
