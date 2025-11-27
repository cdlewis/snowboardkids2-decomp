#include "20F0.h"
#include "common.h"
#include "geometry.h"
#include "rand.h"
#include "task_scheduler.h"

extern void func_8002AE80_2BA80(void *);
extern void func_8002B248_2BE48(void *);
extern void func_8002A2D0_2AED0(void *);
extern void func_80029954_2A554(void);
extern void func_80028C08_29808(void);

typedef struct {
    /* 0x00 */ void *model;
    /* 0x04 */ Mat3x3Padded matrix;
    /* 0x24 */ u8 pad24[0xC];
    /* 0x30 */ u16 rotation;
    /* 0x32 */ u8 pad32[0xA];
    /* 0x3C */ void *callback;
    /* 0x40 */ u8 pad40[0x1E];
    /* 0x5E */ u8 state;
} Func8002A200Arg;

typedef struct {
    /* 0x000 */ u8 pad0[0x42A];
    /* 0x42A */ u8 unk42A;
} AllocationData;

typedef struct {
    /* 0x00 */ SceneModel *model;
    /* 0x04 */ Mat3x3Padded matrix;
    /* 0x24 */ u8 pad24[0x8];
    /* 0x2C */ u16 rotation;
    /* 0x2E */ u8 pad2E[0x2C];
    /* 0x5A */ s16 unk5A;
    /* 0x5C */ u8 pad5C[0x2];
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

INCLUDE_ASM("asm/nonmatchings/297B0", func_80028D90_29990);

INCLUDE_ASM("asm/nonmatchings/297B0", func_80028DF0_299F0);

INCLUDE_ASM("asm/nonmatchings/297B0", func_80028FAC_29BAC);

INCLUDE_ASM("asm/nonmatchings/297B0", func_8002900C_29C0C);

INCLUDE_ASM("asm/nonmatchings/297B0", func_8002917C_29D7C);

INCLUDE_ASM("asm/nonmatchings/297B0", func_800291CC_29DCC);

INCLUDE_ASM("asm/nonmatchings/297B0", func_80029300_29F00);

INCLUDE_ASM("asm/nonmatchings/297B0", func_80029360_29F60);

INCLUDE_ASM("asm/nonmatchings/297B0", func_80029584_2A184);

INCLUDE_ASM("asm/nonmatchings/297B0", func_800295E0_2A1E0);

INCLUDE_ASM("asm/nonmatchings/297B0", func_80029630_2A230);

INCLUDE_ASM("asm/nonmatchings/297B0", func_80029680_2A280);

INCLUDE_ASM("asm/nonmatchings/297B0", func_80029724_2A324);

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

INCLUDE_ASM("asm/nonmatchings/297B0", func_800298F8_2A4F8);

INCLUDE_ASM("asm/nonmatchings/297B0", func_80029954_2A554);

INCLUDE_ASM("asm/nonmatchings/297B0", func_80029A54_2A654);

INCLUDE_ASM("asm/nonmatchings/297B0", func_80029AA4_2A6A4);

INCLUDE_ASM("asm/nonmatchings/297B0", func_80029C40_2A840);

INCLUDE_ASM("asm/nonmatchings/297B0", func_80029C90_2A890);

INCLUDE_ASM("asm/nonmatchings/297B0", func_80029E58_2AA58);

INCLUDE_ASM("asm/nonmatchings/297B0", func_80029EA8_2AAA8);

INCLUDE_ASM("asm/nonmatchings/297B0", func_8002A044_2AC44);

INCLUDE_ASM("asm/nonmatchings/297B0", func_8002A0DC_2ACDC);

INCLUDE_ASM("asm/nonmatchings/297B0", func_8002A108_2AD08);

INCLUDE_ASM("asm/nonmatchings/297B0", func_8002A154_2AD54);

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
