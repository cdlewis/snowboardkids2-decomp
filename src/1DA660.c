#include "20F0.h"
#include "6E840.h"
#include "common.h"
#include "task_scheduler.h"

extern void func_800394BC_3A0BC(void *, s32);
extern void func_8000FED0_10AD0(void);
extern void func_800630F0_63CF0(s32, void *);
extern void debugEnqueueCallback(u16, u8, void *, void *);
extern void func_80038420_39020(void);
extern u8 D_41A1D0[];
extern u8 D_41AD80[];
extern u8 D_422C60[];
typedef struct {
    u8 _pad0[0x2C];
    s32 unk2C;
} func_800B0DD0_arg;

typedef struct {
    SceneModel *unk0;
    u8 _pad4[0x22];
    s16 unk26;
    u8 unk28;
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

void func_800B0DF8_1DB398(void *);
void func_800B0638_1DABD8(void);
void func_800B0E94_1DB434(void *);
void func_800B0EEC_1DB48C(func_800B0FE0_arg *);
void func_800B0F88_1DB528(void *);
void func_800B0FE0_1DB580(func_800B0FE0_arg *);
void func_800B0DD0_1DB370(func_800B0DD0_arg *);

INCLUDE_ASM("asm/nonmatchings/1DA660", func_800B00C0_1DA660);

INCLUDE_ASM("asm/nonmatchings/1DA660", func_800B0218_1DA7B8);

INCLUDE_ASM("asm/nonmatchings/1DA660", func_800B0368_1DA908);

INCLUDE_ASM("asm/nonmatchings/1DA660", func_800B0598_1DAB38);

void func_800B05DC_1DAB7C(func_800B08FC_arg *arg0) {
    applyTransformToModel(arg0->unk0, (applyTransformToModel_arg1 *)&arg0->_pad4);
    if (arg0->unk28 != 2) {
        func_800021B8_2DB8(arg0->unk0, arg0->unk26);
        updateModelGeometry(arg0->unk0);
    }
    setCallback(func_800B0638_1DABD8);
}

INCLUDE_ASM("asm/nonmatchings/1DA660", func_800B0638_1DABD8);

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

INCLUDE_ASM("asm/nonmatchings/1DA660", func_800B0980_1DAF20);

INCLUDE_ASM("asm/nonmatchings/1DA660", func_800B0A54_1DAFF4);

void func_800B0BC0_1DB160(func_800B0FE0_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

INCLUDE_ASM("asm/nonmatchings/1DA660", func_800B0BEC_1DB18C);

INCLUDE_ASM("asm/nonmatchings/1DA660", func_800B0C54_1DB1F4);

void func_800B0D4C_1DB2EC(func_800B0D4C_arg *arg0) {
    arg0->unk2C = freeNodeMemory(arg0->unk2C);
}

void func_800B0D78_1DB318(func_800B0D4C_arg *arg0) {
    arg0->unk2C = dmaRequestAndUpdateStateWithSize(D_41AD80, D_422C60, 0x13FF0);
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
    temp = dmaRequestAndUpdateStateWithSize(D_41A1D0, D_41AD80, 0x1B48);
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
    temp = dmaRequestAndUpdateStateWithSize(D_41A1D0, D_41AD80, 0x1B48);
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

INCLUDE_ASM("asm/nonmatchings/1DA660", func_800B100C_1DB5AC);

void func_800B10D4_1DB674(void *arg0) {
    getCurrentAllocation();
    func_800630F0_63CF0(0, arg0);
}

void func_800B1104_1DB6A4(func_800B1104_arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}
