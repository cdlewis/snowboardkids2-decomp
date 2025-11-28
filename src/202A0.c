#include "20F0.h"
#include "common.h"
#include "geometry.h"
#include "overlay.h"
#include "task_scheduler.h"

extern void func_80021548_22148(u8 arg0, void *arg1);
extern void func_800215DC_221DC(void *arg0);
extern s32 func_8006FED8_70AD8(void *arg0);
extern void func_800394BC_3A0BC(void *, s32);
extern void *freeNodeMemory(void *);
extern void debugEnqueueCallback(u16 index, u8 arg1, void *arg2, void *arg3);
extern void func_8000FED0_10AD0(void);

USE_ASSET(_458E30);

extern u8 D_459310[];
extern u32 *D_43A000;
extern u32 *D_43F050;

typedef struct {
    u8 _pad0[0xB2F];
    u8 unkB2F;
    u8 _padB30[0x15];
    u8 unkB45;
    u8 _padB46;
    u8 unkB47;
} Allocation_202A0;

typedef struct {
    u8 padding[0x14];
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s16 unk20;
    s16 unk22;
} TransformData_202A0;

typedef struct {
    SceneModel *unk0;
    TransformData_202A0 unk4;
} Func8002144CArg;

typedef struct {
    u8 padding[0x2C];
    void *unk2C;
} Func800216ACArg;

typedef struct {
    u8 _pad0[0x18];
    void *unk18;
    u8 _pad1C[0x10];
    void *unk2C;
} Func80020528Arg;

typedef struct {
    void *unk0;
    void *unk4;
    u8 _pad8[0x2C];
    void *unk34;
    u8 _pad38[0x1C];
    void *unk54;
} Func80021B88Arg;

typedef struct {
    u8 _pad0[0x2C];
    void *unk2C;
    u8 _pad30[0x4];
    void *unk34;
} Func80021238Arg;

typedef struct {
    u8 _pad0[0x8];
    void *unk8;
} Func80021880Arg;

typedef struct {
    void *unk0;
    void *unk4;
    u8 _pad8[0x98];
    void *unkA0;
} Func80020FA4Arg;

typedef struct {
    u8 _pad0[0xF8];
    void *unkF8;
} Func800206B4Arg;

typedef struct {
    u8 _pad0[0x72];
    u16 unk72;
} Func8001FE64Arg;

INCLUDE_ASM("asm/nonmatchings/202A0", func_8001F6A0_202A0);

INCLUDE_ASM("asm/nonmatchings/202A0", func_8001F7C8_203C8);

INCLUDE_ASM("asm/nonmatchings/202A0", func_8001FA00_20600);

extern void func_8001FEB4_20AB4(void);

void func_8001FE64_20A64(Func8001FE64Arg *arg0) {
    getCurrentAllocation();

    arg0->unk72++;
    if (arg0->unk72 == 150) {
        arg0->unk72 = 0;
        setCallback(&func_8001FEB4_20AB4);
    }
}

INCLUDE_ASM("asm/nonmatchings/202A0", func_8001FEB4_20AB4);

INCLUDE_ASM("asm/nonmatchings/202A0", func_8001FFE4_20BE4);

INCLUDE_ASM("asm/nonmatchings/202A0", func_80020418_21018);

void func_80020528_21128(Func80020528Arg *arg0) {
    arg0->unk18 = freeNodeMemory(arg0->unk18);
    func_80002014_2C14(arg0->unk2C);
}

INCLUDE_ASM("asm/nonmatchings/202A0", func_8002055C_2115C);

void func_80020708_21308(void);
void func_80020B18_21718(Func800206B4Arg *arg0);

void func_800206B4_212B4(Func800206B4Arg *arg0) {
    s32 pad[4];
    arg0->unkF8 = dmaRequestAndUpdateStateWithSize(&D_43A000, &D_43F050, 0xB198);
    setCleanupCallback(&func_80020B18_21718);
    setCallback(&func_80020708_21308);
}

INCLUDE_ASM("asm/nonmatchings/202A0", func_80020708_21308);

INCLUDE_ASM("asm/nonmatchings/202A0", func_800208AC_214AC);

INCLUDE_ASM("asm/nonmatchings/202A0", func_80020924_21524);

INCLUDE_ASM("asm/nonmatchings/202A0", func_80020A00_21600);

void func_80020B18_21718(Func800206B4Arg *arg0) {
    arg0->unkF8 = freeNodeMemory(arg0->unkF8);
}

INCLUDE_ASM("asm/nonmatchings/202A0", func_80020B44_21744);

INCLUDE_ASM("asm/nonmatchings/202A0", func_80020D18_21918);

void func_80020FA4_21BA4(Func80020FA4Arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
    arg0->unkA0 = freeNodeMemory(arg0->unkA0);
}

INCLUDE_ASM("asm/nonmatchings/202A0", func_80020FDC_21BDC);

void func_80021054_21C54(void *arg0) {
    Allocation_202A0 *allocation = (Allocation_202A0 *)getCurrentAllocation();

    if (allocation->unkB2F == 2) {
        debugEnqueueCallback(8, 7, func_8000FED0_10AD0, arg0);
    }
}

typedef struct {
    void *unk0;
    void *unk4;
} Func8002109CArg;

void func_8002109C_21C9C(Func8002109CArg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

INCLUDE_ASM("asm/nonmatchings/202A0", func_800210C8_21CC8);

void func_80021184_21D84(void);

void func_8002115C_21D5C(Func800216ACArg *arg0) {
    func_800394BC_3A0BC(arg0, (s32)arg0->unk2C);
    setCallback(&func_80021184_21D84);
}

INCLUDE_ASM("asm/nonmatchings/202A0", func_80021184_21D84);

void func_80021238_21E38(Func80021238Arg *arg0) {
    arg0->unk2C = freeNodeMemory(arg0->unk2C);
    arg0->unk34 = freeNodeMemory(arg0->unk34);
}

INCLUDE_ASM("asm/nonmatchings/202A0", func_80021270_21E70);

typedef struct {
    u8 _pad0[0x7];
    u8 unk7;
} D_800AFE8C_type_202A0;

extern D_800AFE8C_type_202A0 *D_800AFE8C_A71FC;

void func_8002144C_2204C(Func8002144CArg *arg0);

void func_800213C8_21FC8(Func8002144CArg *arg0) {
    Allocation_202A0 *allocation = (Allocation_202A0 *)getCurrentAllocation();

    if (allocation->unkB45 != 0) {
        if (D_800AFE8C_A71FC->unk7 == 0xC) {
            scaleMatrix((Mat3x3Padded *)&arg0->unk4, 0x1000, 0x1000, 0x1000);
        }
    }

    applyTransformToModel(arg0->unk0, (applyTransformToModel_arg1 *)&arg0->unk4);
    func_800021B8_2DB8(arg0->unk0, arg0->unk4.unk20);
    updateModelGeometry(arg0->unk0);
    setCallback(&func_8002144C_2204C);
}

void func_8002144C_2204C(Func8002144CArg *arg0) {
    Allocation_202A0 *allocation;
    s32 temp;
    u16 unk24;

    allocation = (Allocation_202A0 *)getCurrentAllocation();
    applyTransformToModel(arg0->unk0, (applyTransformToModel_arg1 *)&arg0->unk4);
    temp = clearModelRotation(arg0->unk0);
    func_8006FED8_70AD8(allocation);
    updateModelGeometry(arg0->unk0);

    if (allocation->unkB45 != 0) {
        if (allocation->unkB47 != 0) {
            func_80021548_22148(allocation->unkB47, arg0);
        } else if (temp != 0 && allocation->unkB2F == 8) {
            func_800215DC_221DC(arg0);
        }
    } else if (temp != 0) {
        unk24 = arg0->unk4.unk20;
        if (unk24 == 0xD) {
            arg0->unk4.unk20 = unk24 + 1;
            func_800021B8_2DB8(arg0->unk0, unk24 + 1);
        }
    }
}

void func_8002152C_2212C(Func8002144CArg *arg0) {
    func_80002014_2C14(arg0->unk0);
}

INCLUDE_ASM("asm/nonmatchings/202A0", func_80021548_22148);

INCLUDE_ASM("asm/nonmatchings/202A0", func_800215DC_221DC);

void func_8002174C_2234C(Func800216ACArg *arg0);
void func_800216AC_222AC(Func800216ACArg *arg0);

void func_80021658_22258(Func800216ACArg *arg0) {
    arg0->unk2C = dmaRequestAndUpdateStateWithSize(&_458E30_ROM_START, D_459310, 0xAE0);
    setCleanupCallback(&func_8002174C_2234C);
    setCallback(&func_800216AC_222AC);
}

void func_800216D4_222D4(void);

void func_800216AC_222AC(Func800216ACArg *arg0) {
    func_800394BC_3A0BC(arg0, (s32)arg0->unk2C);
    setCallback(&func_800216D4_222D4);
}

INCLUDE_ASM("asm/nonmatchings/202A0", func_800216D4_222D4);

void func_8002174C_2234C(Func800216ACArg *arg0) {
    arg0->unk2C = freeNodeMemory(arg0->unk2C);
}

INCLUDE_ASM("asm/nonmatchings/202A0", func_80021778_22378);

INCLUDE_ASM("asm/nonmatchings/202A0", func_80021810_22410);

void func_80021880_22480(Func80021880Arg *arg0) {
    arg0->unk8 = freeNodeMemory(arg0->unk8);
}

INCLUDE_ASM("asm/nonmatchings/202A0", func_800218AC_224AC);

INCLUDE_ASM("asm/nonmatchings/202A0", func_80021A20_22620);

void func_80021B88_22788(Func80021B88Arg *arg0) {
    arg0->unk34 = freeNodeMemory(arg0->unk34);
    arg0->unk54 = freeNodeMemory(arg0->unk54);
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}
