#include "20F0.h"
#include "common.h"
#include "geometry.h"
#include "task_scheduler.h"

extern void func_80021548_22148(u8 arg0, void *arg1);
extern void func_800215DC_221DC(void *arg0);
extern s32 func_8006FED8_70AD8(void *arg0);
extern void func_800394BC_3A0BC(void *, s32);
extern void setCallback(void *);
extern void *freeNodeMemory(void *);

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

INCLUDE_ASM("asm/nonmatchings/202A0", func_8001F6A0_202A0);

INCLUDE_ASM("asm/nonmatchings/202A0", func_8001F7C8_203C8);

INCLUDE_ASM("asm/nonmatchings/202A0", func_8001FA00_20600);

INCLUDE_ASM("asm/nonmatchings/202A0", func_8001FE64_20A64);

INCLUDE_ASM("asm/nonmatchings/202A0", func_8001FEB4_20AB4);

INCLUDE_ASM("asm/nonmatchings/202A0", func_8001FFE4_20BE4);

INCLUDE_ASM("asm/nonmatchings/202A0", func_80020418_21018);

void func_80020528_21128(Func80020528Arg *arg0) {
    arg0->unk18 = freeNodeMemory(arg0->unk18);
    func_80002014_2C14(arg0->unk2C);
}

INCLUDE_ASM("asm/nonmatchings/202A0", func_8002055C_2115C);

INCLUDE_ASM("asm/nonmatchings/202A0", func_800206B4_212B4);

INCLUDE_ASM("asm/nonmatchings/202A0", func_80020708_21308);

INCLUDE_ASM("asm/nonmatchings/202A0", func_800208AC_214AC);

INCLUDE_ASM("asm/nonmatchings/202A0", func_80020924_21524);

INCLUDE_ASM("asm/nonmatchings/202A0", func_80020A00_21600);

INCLUDE_ASM("asm/nonmatchings/202A0", func_80020B18_21718);

INCLUDE_ASM("asm/nonmatchings/202A0", func_80020B44_21744);

INCLUDE_ASM("asm/nonmatchings/202A0", func_80020D18_21918);

INCLUDE_ASM("asm/nonmatchings/202A0", func_80020FA4_21BA4);

INCLUDE_ASM("asm/nonmatchings/202A0", func_80020FDC_21BDC);

INCLUDE_ASM("asm/nonmatchings/202A0", func_80021054_21C54);

INCLUDE_ASM("asm/nonmatchings/202A0", func_8002109C_21C9C);

INCLUDE_ASM("asm/nonmatchings/202A0", func_800210C8_21CC8);

void func_80021184_21D84(void);

void func_8002115C_21D5C(Func800216ACArg *arg0) {
    func_800394BC_3A0BC(arg0, (s32)arg0->unk2C);
    setCallback(&func_80021184_21D84);
}

INCLUDE_ASM("asm/nonmatchings/202A0", func_80021184_21D84);

INCLUDE_ASM("asm/nonmatchings/202A0", func_80021238_21E38);

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

INCLUDE_ASM("asm/nonmatchings/202A0", func_80021658_22258);

void func_800216D4_222D4(void);

void func_800216AC_222AC(Func800216ACArg *arg0) {
    func_800394BC_3A0BC(arg0, (s32)arg0->unk2C);
    setCallback(&func_800216D4_222D4);
}

INCLUDE_ASM("asm/nonmatchings/202A0", func_800216D4_222D4);

INCLUDE_ASM("asm/nonmatchings/202A0", func_8002174C_2234C);

INCLUDE_ASM("asm/nonmatchings/202A0", func_80021778_22378);

INCLUDE_ASM("asm/nonmatchings/202A0", func_80021810_22410);

INCLUDE_ASM("asm/nonmatchings/202A0", func_80021880_22480);

INCLUDE_ASM("asm/nonmatchings/202A0", func_800218AC_224AC);

INCLUDE_ASM("asm/nonmatchings/202A0", func_80021A20_22620);

void func_80021B88_22788(Func80021B88Arg *arg0) {
    arg0->unk34 = freeNodeMemory(arg0->unk34);
    arg0->unk54 = freeNodeMemory(arg0->unk54);
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}
