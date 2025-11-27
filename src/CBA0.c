#include "6E840.h"
#include "common.h"
#include "task_scheduler.h"

extern Gfx *gRegionAllocPtr;
extern u8 D_8016A000[];

void func_8000BFA0_CBA0(void *arg0) {
    gDPSetColorImage(gRegionAllocPtr++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 320, D_8016A000);
}

INCLUDE_ASM("asm/nonmatchings/CBA0", func_8000BFD0_CBD0);

extern void func_8000BFD0_CBD0(void *);

typedef struct {
    Node_70B00 *unk0;
} Func8000C208Arg;

void func_8000C208_CE08(Func8000C208Arg *arg0) {
    debugEnqueueCallback(arg0->unk0->slot_index, 7, func_8000BFD0_CBD0, arg0);
}

void func_8000C238_CE38(Func8000C208Arg *arg0) {
    debugEnqueueCallback(arg0->unk0->slot_index, 0, func_8000BFA0_CBA0, arg0);
}

typedef struct {
    u8 pad[0x210];
    s32 unk210;
} Func8000C268Arg;

void func_8000C268_CE68(Func8000C268Arg *arg0) {
    arg0->unk210 |= 1;
}

void func_8000C278_CE78(Func8000C268Arg *arg0) {
    arg0->unk210 &= ~1;
}

typedef struct {
    u8 pad[0x1E4];
    u8 unk1E4[0xC];
} Func8000C28CArg;

void func_8000C28C_CE8C(Func8000C28CArg *arg0, void *arg1) {
    memcpy(arg0->unk1E4, arg1, 0xC);
}

typedef struct {
    u8 pad[0x204];
    s32 unk204;
} Func8000C2ACArg;

void func_8000C2AC_CEAC(Func8000C2ACArg *arg0, s32 arg1) {
    arg0->unk204 = arg1;
}

typedef struct {
    u8 pad[0x218];
    s8 unk218;
} Func8000C2B4Arg;

void func_8000C2B4_CEB4(Func8000C2B4Arg *arg0, s32 arg1) {
    arg0->unk218 = arg1;
}

typedef struct {
    u8 pad[0x214];
    u8 unk214;
    u8 unk215;
    u8 unk216;
} Func8000C2BCArg;

void func_8000C2BC_CEBC(Func8000C2BCArg *arg0, u8 arg1, u8 arg2, u8 arg3) {
    arg0->unk214 = arg1;
    arg0->unk215 = arg2;
    arg0->unk216 = arg3;
}

typedef struct {
    u8 pad[0x204];
    s32 unk204;
    s32 unk208;
    s16 unk20C;
} Func8000C2CCArg;

void func_8000C2CC_CECC(Func8000C2CCArg *arg0, s32 arg1, s16 arg2) {
    if (arg2 == 0) {
        arg0->unk20C = 0;
        arg0->unk208 = 0;
        arg0->unk204 = arg1;
    } else {
        arg0->unk20C = arg2;
        arg0->unk208 = (arg1 - arg0->unk204) / arg2;
    }
}

INCLUDE_ASM("asm/nonmatchings/CBA0", func_8000C334_CF34);

extern void func_8000C5AC_D1AC(void *);

typedef struct {
    Node_70B00 base;
    ColorData unk1D8;
    ColorData unk1E0;
    u8 pad1E8[0x28];
    s32 unk210;
    u8 pad214[0x4];
    u8 unk218;
} Func8000C440Arg;

void func_8000C440_D040(Func8000C440Arg *arg0, u16 arg1, u8 arg2, u16 arg3, u8 arg4, u16 arg5, u16 arg6) {
    void *task;
    s32 pad[8];

    func_8006FAA4_706A4(arg0, 0, arg1, arg2, 1);
    func_8006F9BC_705BC(&arg0->base, 1.0f, 1.0f);
    func_8006FEF8_70AF8(&arg0->base, arg3);
    setModelCameraTransform(arg0, 0, 0, -0xA0, -0x78, 0x9F, 0x77);
    func_8006FA0C_7060C(&arg0->base, 40.0f, 1.3333334f, 10.0f, 10000.0f);

    arg0->unk1D8.r2 = 0;
    arg0->unk1D8.g2 = 0x7F;
    arg0->unk1D8.b2 = 0x7F;
    arg0->unk1D8.r = 0;
    arg0->unk1D8.g = 0;
    arg0->unk1D8.b = 0;
    arg0->unk1E0.r = 0;
    arg0->unk1E0.g = 0;
    arg0->unk1E0.b = 0;

    func_8006FC70_70870(arg0->base.id, 1, &arg0->unk1D8, &arg0->unk1E0);

    arg0->unk210 = 0;
    arg0->unk218 = arg4;

    task = scheduleTask(func_8000C5AC_D1AC, arg5, arg6, 0);
    if (task != NULL) {
        *(Func8000C440Arg **)task = arg0;
    }
}

void n_alSeqpDelete(Node_70B00 *arg0) {
    unlinkNode(arg0);
}

INCLUDE_ASM("asm/nonmatchings/CBA0", func_8000C5AC_D1AC);

INCLUDE_ASM("asm/nonmatchings/CBA0", func_8000C7A4_D3A4);

extern void *freeNodeMemory(void *);

typedef struct {
    u8 pad0[0x28];
    void *unk28;
    void *unk2C;
    void *unk30;
    u8 pad34[0x30];
    void *unk64;
    void *unk68;
    void *unk6C;
} Func8000CA80Arg;

void func_8000CA80_D680(Func8000CA80Arg *arg0) {
    arg0->unk30 = freeNodeMemory(arg0->unk30);
    arg0->unk2C = freeNodeMemory(arg0->unk2C);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
    arg0->unk6C = freeNodeMemory(arg0->unk6C);
    arg0->unk68 = freeNodeMemory(arg0->unk68);
    arg0->unk64 = freeNodeMemory(arg0->unk64);
}
