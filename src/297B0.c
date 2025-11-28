#include "20F0.h"
#include "common.h"
#include "geometry.h"
#include "graphics.h"
#include "rand.h"
#include "task_scheduler.h"

extern void func_8002AE80_2BA80(void *);
extern void func_8002B248_2BE48(void *);
extern void func_8002A2D0_2AED0(void *);
extern void func_80029954_2A554(void);
extern void func_80028C08_29808(void);
extern void func_80028DF0_299F0(void);
extern s32 func_8002A390_2AF90(void *);
extern void func_80028AEC_296EC(void *);
extern void func_800291CC_29DCC(void);
extern void func_80029EA8_2AAA8(void);
extern void func_80029C90_2A890(void);
extern void func_80029AA4_2A6A4(void);
extern void func_8002900C_29C0C(void);
extern void func_80029360_29F60(void);
extern void func_8000A49C_B09C(s32, s16, s16, s16, void *, s32, s8, u8, u8, s16);

typedef struct Func8002A154Arg Func8002A154Arg;
void func_8002A154_2AD54(Func8002A154Arg *);
void func_8002A200_2AE00(Func8002A154Arg *);

struct Func8002A154Arg {
    /* 0x00 */ void *model;
    /* 0x04 */ Mat3x3Padded matrix;
    /* 0x24 */ u8 pad24[0xC];
    /* 0x30 */ u16 rotation;
    /* 0x32 */ u8 pad32[0xA];
    /* 0x3C */ void *callback;
    /* 0x40 */ u8 pad40[0x10];
    /* 0x50 */ u16 unk50;
    /* 0x52 */ u8 pad52[0x4];
    /* 0x56 */ u16 unk56;
    /* 0x58 */ u8 pad58[0x6];
    /* 0x5E */ u8 state;
    /* 0x5F */ u8 unk5F;
};

typedef Func8002A154Arg Func8002A200Arg;

typedef struct {
    /* 0x000 */ u8 pad0[0x408];
    /* 0x408 */ s32 unk408;
    /* 0x40C */ u8 pad40C[0x4];
    /* 0x410 */ s32 unk410;
    /* 0x414 */ u8 pad414[0x16];
    /* 0x42A */ u8 unk42A;
} AllocationData;

typedef struct {
    /* 0x00 */ SceneModel *model;
    /* 0x04 */ Mat3x3Padded matrix;
    /* 0x24 */ u8 pad24[0x8];
    /* 0x2C */ u16 rotation;
    /* 0x2E */ u8 pad2E[0x12];
    /* 0x40 */ s32 unk40;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ s32 unk48;
    /* 0x4C */ u8 pad4C[0xE];
    /* 0x5A */ s16 unk5A;
    /* 0x5C */ u8 unk5C;
    /* 0x5D */ u8 unk5D;
    /* 0x5E */ u8 unk5E;
    /* 0x5F */ u8 pad5F[0x2];
    /* 0x61 */ u8 unk61;
    /* 0x62 */ u8 unk62;
} Func297D8Arg;

typedef struct {
    /* 0x00 */ SceneModel *unk0;
    /* 0x04 */ applyTransformToModel_arg1 unk4;
    /* 0x28 */ u8 pad28[0x28];
    /* 0x50 */ s16 unk50;
} func_8002A290_2AE90_arg;

void func_8002A290_2AE90(void *);

void func_80028BB0_297B0(Func297D8Arg *arg0) {
    arg0->unk5E = 0;
    arg0->unk61 = 0;
    arg0->unk62 = 0;
    arg0->unk5A = randB() & 0xF;
    createYRotationMatrix(&arg0->matrix, arg0->rotation);
    func_8002A290_2AE90(arg0);
    setCallback(func_80028C08_29808);
}

INCLUDE_ASM("asm/nonmatchings/297B0", func_80028C08_29808);

void func_80028D90_29990(Func297D8Arg *arg0) {
    arg0->unk5E = 0;
    arg0->unk61 = 0;
    arg0->unk62 = 0;
    arg0->unk5A = arg0->unk5A + (randB() & 0xF);
    createYRotationMatrix(&arg0->matrix, arg0->rotation);
    func_8002A290_2AE90(arg0);
    setCallback(func_80028DF0_299F0);
}

INCLUDE_ASM("asm/nonmatchings/297B0", func_80028DF0_299F0);

void func_80028FAC_29BAC(Func297D8Arg *arg0) {
    arg0->unk5E = 0;
    arg0->unk61 = 0;
    arg0->unk62 = 0;
    arg0->unk5A = 0;
    createYRotationMatrix(&arg0->matrix, arg0->rotation);
    func_8002A290_2AE90(arg0);
    arg0->unk48 = 0;
    arg0->unk40 = 0;
    arg0->unk44 = 0x280000;
    setCallback(func_8002900C_29C0C);
}

INCLUDE_ASM("asm/nonmatchings/297B0", func_8002900C_29C0C);

void func_8002917C_29D7C(Func297D8Arg *arg0) {
    arg0->unk5E = 0;
    arg0->unk61 = 0;
    arg0->unk62 = 0;
    arg0->unk5A = 0;
    createYRotationMatrix(&arg0->matrix, arg0->rotation);
    func_8002A290_2AE90(arg0);
    setCallback(func_800291CC_29DCC);
}

INCLUDE_ASM("asm/nonmatchings/297B0", func_800291CC_29DCC);

void func_80029300_29F00(Func297D8Arg *arg0) {
    arg0->unk5E = 0;
    arg0->unk61 = 0;
    arg0->unk62 = 0;
    arg0->unk5A = 0;
    arg0->unk48 = 0;
    arg0->unk40 = 0;
    arg0->unk44 = 0x260000;
    createYRotationMatrix(&arg0->matrix, arg0->rotation);
    func_8002A290_2AE90(arg0);
    setCallback(func_80029360_29F60);
}

INCLUDE_ASM("asm/nonmatchings/297B0", func_80029360_29F60);

void func_80029584_2A184(Func297D8Arg *arg0) {
    arg0->unk5E = 0;
    arg0->unk61 = 0;
    arg0->unk62 = 0;
    arg0->unk5A = 0;
    createYRotationMatrix(&arg0->matrix, arg0->rotation);
    func_80001688_2288(arg0->model, 4);
    func_8002A290_2AE90(arg0);
    setCallback(func_80029954_2A554);
}

void func_800295E0_2A1E0(Func297D8Arg *arg0) {
    arg0->unk5E = 0;
    arg0->unk61 = 0;
    arg0->unk62 = 0;
    arg0->unk5A = 0;
    createYRotationMatrix(&arg0->matrix, arg0->rotation);
    func_8002A290_2AE90(arg0);
    setCallback(func_80029954_2A554);
}

void func_80029630_2A230(Func297D8Arg *arg0) {
    arg0->unk5E = 0;
    arg0->unk61 = 0;
    arg0->unk62 = 0;
    arg0->unk5A = 0;
    createYRotationMatrix(&arg0->matrix, arg0->rotation);
    func_8002A290_2AE90(arg0);
    setCallback(func_80029954_2A554);
}

void func_80029680_2A280(Func297D8Arg *arg0) {
    arg0->unk44 = 0x260000;
    arg0->unk5E = 0;
    arg0->unk61 = 0;
    arg0->unk62 = 0;
    arg0->unk48 = 0;
    arg0->unk40 = 0;
    func_8000A49C_B09C((s32)arg0->model, 0, 0x29, -1, &arg0->unk40, 0x10000, 0, 2, 0, 0);
    arg0->unk5A = 0;
    createYRotationMatrix(&arg0->matrix, arg0->rotation);
    func_80001688_2288(arg0->model, 1);
    func_8002A290_2AE90(arg0);
    setCallback(func_80029954_2A554);
}

void func_80029724_2A324(Func297D8Arg *arg0) {
    arg0->unk44 = 0x260000;
    arg0->unk5E = 0;
    arg0->unk61 = 0;
    arg0->unk62 = 0;
    arg0->unk48 = 0;
    arg0->unk40 = 0;
    func_8000A49C_B09C((s32)arg0->model, 0, 9, -1, &arg0->unk40, 0x10000, 0, 2, 0, 0);
    arg0->unk5A = 0;
    createYRotationMatrix(&arg0->matrix, arg0->rotation);
    if (arg0->unk5C == 1) {
        func_80001688_2288(arg0->model, 0);
    }
    func_8002A290_2AE90(arg0);
    setCallback(func_80029954_2A554);
}

void func_800297D8_2A3D8(Func297D8Arg *arg0) {
    arg0->unk5E = 0;
    arg0->unk61 = 0;
    arg0->unk62 = 0;
    arg0->unk5A = (randB() & 0x1F) + 0x14;
    createYRotationMatrix(&arg0->matrix, arg0->rotation);
    func_80001688_2288(arg0->model, 4);
    func_8002A290_2AE90(arg0);
    setCallback(func_80029954_2A554);
}

void func_80029840_2A440(Func297D8Arg *arg0) {
    arg0->unk5E = 0;
    arg0->unk61 = 0;
    arg0->unk62 = 0;
    arg0->unk5A = (randB() & 0x1F) + 0x14;
    createYRotationMatrix(&arg0->matrix, arg0->rotation);
    func_8002A290_2AE90(arg0);
    setCallback(func_80029954_2A554);
}

void func_8002989C_2A49C(Func297D8Arg *arg0) {
    arg0->unk5E = 0;
    arg0->unk61 = 0;
    arg0->unk62 = 0;
    arg0->unk5A = (randB() & 0x1F) + 0x14;
    createYRotationMatrix(&arg0->matrix, arg0->rotation);
    func_8002A290_2AE90(arg0);
    setCallback(func_80029954_2A554);
}

void func_800298F8_2A4F8(Func297D8Arg *arg0) {
    arg0->unk5E = 0;
    arg0->unk61 = 0;
    arg0->unk62 = 0;
    arg0->unk5A = (randB() & 0x1F) + 0x14;
    createYRotationMatrix(&arg0->matrix, arg0->rotation);
    func_8002A290_2AE90(arg0);
    setCallback(func_80029954_2A554);
}

INCLUDE_ASM("asm/nonmatchings/297B0", func_80029954_2A554);

void func_80029A54_2A654(Func297D8Arg *arg0) {
    arg0->unk5E = 0;
    arg0->unk61 = 0;
    arg0->unk62 = 0;
    arg0->unk5A = 0;
    createYRotationMatrix(&arg0->matrix, arg0->rotation);
    func_8002A290_2AE90(arg0);
    setCallback(func_80029AA4_2A6A4);
}

INCLUDE_ASM("asm/nonmatchings/297B0", func_80029AA4_2A6A4);

void func_80029C40_2A840(Func297D8Arg *arg0) {
    arg0->unk5E = 0;
    arg0->unk61 = 0;
    arg0->unk62 = 0;
    arg0->unk5A = 0;
    createYRotationMatrix(&arg0->matrix, arg0->rotation);
    func_8002A290_2AE90(arg0);
    setCallback(func_80029C90_2A890);
}

INCLUDE_ASM("asm/nonmatchings/297B0", func_80029C90_2A890);

void func_80029E58_2AA58(Func297D8Arg *arg0) {
    arg0->unk5E = 0;
    arg0->unk61 = 0;
    arg0->unk62 = 0;
    arg0->unk5A = 0;
    createYRotationMatrix(&arg0->matrix, arg0->rotation);
    func_8002A290_2AE90(arg0);
    setCallback(func_80029EA8_2AAA8);
}

INCLUDE_ASM("asm/nonmatchings/297B0", func_80029EA8_2AAA8);

extern u16 D_8008E568_8F168[];
extern u16 D_8008E57C_8F17C[];

void func_8002A044_2AC44(Func297D8Arg *arg0) {
    arg0->unk5A = 0;

    switch (arg0->unk5D) {
        case 0xB:
            func_800585C8_591C8(0xB2);
            break;
        case 0xC:
            func_800585C8_591C8(0x112);
            break;
        case 0xD:
            func_800585C8_591C8(D_8008E568_8F168[arg0->unk5C]);
            break;
        case 0xE:
            func_800585C8_591C8(D_8008E57C_8F17C[arg0->unk5C]);
            break;
    }
}

extern u16 D_8008E590_8F190[];

void func_8002A0DC_2ACDC(Func297D8Arg *arg0) {
    func_800585C8_591C8(D_8008E590_8F190[arg0->unk5C]);
}

void func_8002A108_2AD08(Func297D8Arg *arg0) {
    arg0->unk5E = 0;
    arg0->unk61 = 0;
    arg0->unk62 = 0;
    createYRotationMatrix(&arg0->matrix, arg0->rotation);
    func_8002A290_2AE90(arg0);
    setCallback(func_8002A154_2AD54);
}

void func_8002A154_2AD54(Func8002A154Arg *arg0) {
    u16 savedUnk50;
    u8 savedState;
    AllocationData *alloc = getCurrentAllocation();

    if (func_8002A390_2AF90(arg0) != 0) {
        setCallback(func_80028AEC_296EC);
    }

    func_8002A2D0_2AED0(arg0);

    alloc->unk408 = arg0->matrix.unk14;
    alloc->unk410 = arg0->matrix.unk1C;

    if (alloc->unk42A == 0x11) {
        savedUnk50 = arg0->unk50;
        savedState = arg0->state;
        arg0->state = 0x14;
        arg0->unk50 = 0;
        arg0->callback = func_8002A154_2AD54;
        arg0->unk56 = savedUnk50;
        arg0->unk5F = savedState;
        setCallback(func_8002A200_2AE00);
    }
}

void func_8002A1F8_2ADF8(void) {
}

void func_8002A200_2AE00(Func8002A200Arg *arg0) {
    AllocationData *alloc = getCurrentAllocation();

    switch (arg0->state) {
        case 0x14:
            func_8002AE80_2BA80(arg0);
            createYRotationMatrix(&arg0->matrix, arg0->rotation);
            break;
        case 0x15:
            func_8002B248_2BE48(arg0);
            createYRotationMatrix(&arg0->matrix, arg0->rotation);
            break;
    }

    func_8002A2D0_2AED0(arg0);

    if (alloc->unk42A == 0) {
        setCallback(arg0->callback);
    }
}

void func_8002A290_2AE90(void *untypedArg0) {
    func_8002A290_2AE90_arg *arg0 = (func_8002A290_2AE90_arg *)untypedArg0;
    applyTransformToModel(arg0->unk0, &arg0->unk4);
    func_800021B8_2DB8(arg0->unk0, arg0->unk50);
    updateModelGeometry(arg0->unk0);
}

INCLUDE_ASM("asm/nonmatchings/297B0", func_8002A2D0_2AED0);
