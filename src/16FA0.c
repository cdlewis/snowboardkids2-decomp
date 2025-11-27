#include "20F0.h"
#include "common.h"
#include "task_scheduler.h"

extern void func_8000056C_116C(s32, s32, void *);
extern void func_80000460_1060(s32, void *, void *);
extern void func_80000760_1360(void);
extern void debugEnqueueCallback(u16 index, u8 arg1, void *arg2, void *arg3);
extern void func_80038420_39020(void);

typedef struct {
    s32 unk0;
    void *unk4;
    void *unk8;
    u8 padding[0x7A];
    s8 unk86;
} func_80000710_1310_arg_16FA0;

extern void func_80000710_1310(func_80000710_1310_arg_16FA0 *);

void func_800168BC_174BC(void);
void func_800168D8_174D8(func_80000710_1310_arg_16FA0 *arg0);

typedef struct {
    u8 data[0x20];
} TransformData;

extern TransformData D_8008D5C4_8E1C4[];
extern u16 *D_8008D534_8E134;

typedef struct {
    SceneModel *unk0;   // 0x00
    TransformData unk4; // 0x04 - 0x20 bytes of transform data
    s32 unk24;          // 0x24
    u16 *unk28;         // 0x28 - pointer to u16 animation data
    u16 unk2C;          // 0x2C - current animation state
    u8 unk2E;           // 0x2E
    u8 unk2F;           // 0x2F - index into transform array
    u8 _pad30;          // 0x30
    u8 unk31;           // 0x31
    u8 unk32;           // 0x32
} Struct16B68;

void func_80016C28_17828(Struct16B68 *arg0);
void func_80016A00_17600(Struct16B68 *arg0);
void func_80016DE0_179E0(Struct16B68 *arg0);
void func_80016964_17564(Struct16B68 *arg0);
void func_80016E70_17A70(Struct16B68 *arg0);

typedef struct {
    u8 padding[0x2];
    s16 unk2;
    u8 padding2[0x34];
    void *unk38;
} Struct163F8;

extern void func_800394BC_3A0BC(void *, s32);
extern u8 D_414CF0[];
extern u8 D_418520[];

void func_800163CC_16FCC(Struct163F8 *);
void func_800163A0_16FA0(Struct163F8 *);

void func_800163A0_16FA0(Struct163F8 *arg0) {
    arg0->unk38 = freeNodeMemory(arg0->unk38);
}

void func_800163CC_16FCC(Struct163F8 *arg0) {
    debugEnqueueCallback(8, 0, func_80038420_39020, arg0);
}

void func_800163F8_16FF8(Struct163F8 *arg0) {
    func_800394BC_3A0BC(arg0, (s32)arg0->unk38);
    arg0->unk2 = 0x10;
    setCallback(func_800163CC_16FCC);
}

void func_80016434_17034(Struct163F8 *arg0) {
    arg0->unk38 = dmaRequestAndUpdateStateWithSize(D_414CF0, D_418520, 0x7B50);
    setCleanupCallback(func_800163A0_16FA0);
    setCallback(func_800163F8_16FF8);
}

INCLUDE_ASM("asm/nonmatchings/16FA0", func_80016488_17088);

INCLUDE_ASM("asm/nonmatchings/16FA0", func_800165D8_171D8);

void func_800166FC_172FC(void **arg0) {
    arg0[1] = freeNodeMemory(arg0[1]);
}

INCLUDE_ASM("asm/nonmatchings/16FA0", func_80016728_17328);

INCLUDE_ASM("asm/nonmatchings/16FA0", func_800167B0_173B0);

void func_80016834_17434(void **arg0) {
    arg0[1] = freeNodeMemory(arg0[1]);
}

void func_80016860_17460(s32 arg0) {
    s32 sp28;
    s32 sp10[6];
    void *alloc;

    alloc = getCurrentAllocation();
    func_8000056C_116C(arg0, 2, (u8 *)alloc + 0x1D8);
    setCleanupCallback(func_800168D8_174D8);
    func_80000460_1060(arg0, sp10, &sp28);
    setCallback(func_800168BC_174BC);
}

void func_800168BC_174BC(void) {
    func_80000760_1360();
}

void func_800168D8_174D8(func_80000710_1310_arg_16FA0 *arg0) {
    func_80000710_1310(arg0);
}

void func_800168F4_174F4(Struct16B68 *arg0) {
    void *alloc;
    s32 temp;

    alloc = getCurrentAllocation();
    arg0->unk0 = func_8000198C_258C(arg0->unk2F + 0x32, (u8 *)alloc + 0x1D8);
    setCleanupCallback(func_80016E70_17A70);
    temp = 0x8000;
    arg0->unk32 = 0;
    arg0->unk24 = temp;
    arg0->unk2E = 0;
    temp = (s32)D_8008D534_8E134;
    arg0->unk31 = 0;
    arg0->unk28 = (u16 *)temp;
    setCallback(func_80016964_17564);
}

void func_80016964_17564(Struct16B68 *arg0) {
    getCurrentAllocation();
    applyTransformToModel(arg0->unk0, (applyTransformToModel_arg1 *)&D_8008D5C4_8E1C4[arg0->unk2F]);
    memcpy(&arg0->unk4, &D_8008D5C4_8E1C4[arg0->unk2F], 0x20);
    arg0->unk2C = *arg0->unk28;
    arg0->unk28 = arg0->unk28 + 1;
    func_800021B8_2DB8(arg0->unk0, arg0->unk2C);
    updateModelGeometry(arg0->unk0);
    setCallback(func_80016A00_17600);
}

INCLUDE_ASM("asm/nonmatchings/16FA0", func_80016A00_17600);

void func_80016B68_17768(Struct16B68 *arg0) {
    s32 clearResult;
    u16 animValue;

    getCurrentAllocation();
    clearResult = clearModelRotation(arg0->unk0);
    updateModelGeometry(arg0->unk0);

    if (clearResult == 0) {
        return;
    }

    if (arg0->unk2C == 4) {
        func_800021B8_2DB8(arg0->unk0, 8);
        setCallback(func_80016C28_17828);
        return;
    }

    animValue = *arg0->unk28;
    if ((u16)animValue != 0xFFFF) {
        arg0->unk2C = animValue;
        arg0->unk28 += 1;
    } else {
        arg0->unk2C = 8;
        setCallback(func_80016C28_17828);
    }
    func_800021B8_2DB8(arg0->unk0, (s16)arg0->unk2C);
}

INCLUDE_ASM("asm/nonmatchings/16FA0", func_80016C28_17828);

void func_80016D0C_1790C(Struct16B68 *arg0) {
    s32 clearResult;
    u16 animValue;

    getCurrentAllocation();
    clearResult = clearModelRotation(arg0->unk0);
    updateModelGeometry(arg0->unk0);

    if (clearResult == 0) {
        return;
    }

    if (arg0->unk2C == 4) {
        arg0->unk2C = 8;
        arg0->unk31 = 0x11;
        func_800021B8_2DB8(arg0->unk0, 8);
        setCallback(func_80016DE0_179E0);
        return;
    }

    animValue = *arg0->unk28;
    if ((u16)animValue != 0xFFFF) {
        arg0->unk2C = animValue;
        arg0->unk28 += 1;
    } else {
        arg0->unk2C = 8;
        arg0->unk31 = 0x11;
        setCallback(func_80016DE0_179E0);
    }
    func_800021B8_2DB8(arg0->unk0, (s16)arg0->unk2C);
}

void func_80016DE0_179E0(Struct16B68 *arg0) {
    s32 clearResult;

    clearResult = clearModelRotation(arg0->unk0);
    updateModelGeometry(arg0->unk0);

    if (clearResult != 0 && arg0->unk2C == 8) {
        arg0->unk2C++;
        func_800021B8_2DB8(arg0->unk0, (s16)arg0->unk2C);
    }

    if (arg0->unk31 != 0) {
        arg0->unk31--;
        if (arg0->unk31 == 0) {
            func_80001688_2288(arg0->unk0, 2);
        }
    }
}

void func_80016E70_17A70(Struct16B68 *arg0) {
    arg0->unk0 = func_80002014_2C14(arg0->unk0);
}
