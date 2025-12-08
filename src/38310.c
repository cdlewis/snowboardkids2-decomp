#include "38310.h"
#include "10AD0.h"
#include "33FE0.h"
#include "36B80.h"
#include "3E160.h"
#include "D_800AFE8C_A71FC_type.h"
#include "common.h"
#include "overlay.h"
#include "task_scheduler.h"

USE_ASSET(_4196E0);
USE_ASSET(_419C60);
USE_ASSET(_4237C0);
USE_ASSET(_426EF0);

typedef struct {
    u8 pad[0x8];
    void *unk8;
    u8 pad2[0xC];
    void *unk18;
} func_80037874_38474_arg;

typedef struct {
    u8 pad0[0x4];
    void *unk4;
    u8 pad[0xD0];
    void *unkD8;
} func_80037BC4_387C4_arg;

typedef struct {
    void *unk0;
    void *unk4;
    u8 pad[0x40];
    void *unk48;
} func_80037E40_38A40_arg;

typedef struct {
    u8 pad0[0x4];
    void *unk4;
} func_80037FB0_38BB0_arg;

typedef struct {
    u8 pad[0x1E2];
    u16 unk1E2;
    u8 pad2[0x4];
    u8 unk1E8[4];
    u8 unk1EC;
} func_80037F14_alloc;

extern u8 D_8008FD30_90930[];
extern void *D_8008FE54_90A54[];

void func_80037E40_38A40(func_80037E40_38A40_arg *);
void func_80037D18_38918(void);
void func_80037874_38474(func_80037874_38474_arg *);
void func_800377FC_383FC(u8 *);

void func_80037710_38310(func_80037710_38310_arg *arg0) {
    void *s1;
    void *s2;

    s1 = func_80035F80_36B80(1);
    s2 = dmaRequestAndUpdateStateWithSize(&_4196E0_ROM_START, &_4196E0_ROM_END, 0xBB8);
    setCleanupCallback(func_80037874_38474);

    arg0->unk0 = -0x50;
    arg0->unk2 = -0x58;
    arg0->unk4 = D_8008FD30_90930;
    arg0->unk8 = s1;
    arg0->unkC = 0xFF;
    arg0->unkE = 0xFF;
    arg0->unk10 = 0;
    arg0->unk14 = -0x90;
    arg0->unk16 = -0x60;
    arg0->unk18 = s2;
    arg0->unk1C = 2;
    arg0->unk1E = 0xFF;
    arg0->unk20 = 1;
    arg0->unk21 = 0;
    arg0->unk24 = 0x58;
    arg0->unk26 = -0x60;
    arg0->unk28 = s2;
    arg0->unk2C = 2;
    arg0->unk2E = 0xFF;
    arg0->unk30 = 0;
    arg0->unk31 = 0;

    setCallback(func_800377FC_383FC);
}

void func_800377FC_383FC(u8 *arg0) {
    s32 i;

    debugEnqueueCallback(8, 0, func_80035408_36008, arg0);

    for (i = 0; i < 2; i++) {
        debugEnqueueCallback(8, 0, func_80012004_12C04, arg0 + 0x14 + i * 0x10);
    }
}

void func_80037874_38474(func_80037874_38474_arg *arg0) {
    arg0->unk8 = freeNodeMemory(arg0->unk8);
    arg0->unk18 = freeNodeMemory(arg0->unk18);
}

INCLUDE_ASM("asm/nonmatchings/38310", func_800378AC_384AC);

typedef struct {
    u8 pad0[0xA];
    s16 unkA;
    u8 unkC;
    u8 unkD;
    u8 unkE;
    u8 unkF;
} func_80037A64_Entry10;

typedef union {
    s16 asShort;
    u8 asBytes[2];
} func_80037A64_UnkCUnion;

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    u8 pad8[4];
    func_80037A64_UnkCUnion unkC;
    u8 unkE;
    u8 unkF;
    u8 unk10;
    u8 unk11;
    u8 unk12;
    u8 unk13;
} func_80037A64_Entry14;

typedef struct {
    func_80037A64_Entry10 entries10[6];
    func_80037A64_Entry14 entries14[6];
    void *unkD8;
} func_80037A64_Arg0Struct;

void func_80037A64_38664(func_80037A64_Arg0Struct *arg0) {
    func_80037F14_alloc *alloc;
    s32 i;
    u8 temp;

    alloc = getCurrentAllocation();

    for (i = 0; i < 6; i++) {
        if (i < 2) {
            temp = D_800AFE8C_A71FC->unk1F;
        } else if (i < 4) {
            temp = D_800AFE8C_A71FC->unk20;
        } else {
            temp = (D_800AFE8C_A71FC->unk21 + 1) & 1;
        }

        arg0->entries10[i].unkD = ((temp + (i & 1)) & 1) | 2;

        if (alloc->unk1E2 == 0) {
            u8 idx;
            s32 value;
            idx = alloc->unk1EC;
            value = temp + (idx << 1);
            if (value == i) {
                arg0->entries10[i].unkA = alloc->unk1E8[idx];
                arg0->entries14[i].unkC.asShort = alloc->unk1E8[alloc->unk1EC];
            } else {
                arg0->entries10[i].unkA = 0;
                arg0->entries14[i].unkC.asShort = 0;
            }
        } else {
            arg0->entries10[i].unkA = 0;
            arg0->entries14[i].unkC.asShort = 0;
        }

        debugEnqueueCallback(8, 0, func_80012FA8_13BA8, &arg0->entries10[i]);

        func_80035DE0_369E0(
            arg0->unkD8,
            arg0->entries14[i].unk4,
            arg0->entries14[i].unk0,
            arg0->entries14[i].unk2,
            arg0->entries14[i].unkC.asBytes[1],
            arg0->entries14[i].unk10,
            (void *)8,
            1
        );
    }
}

void func_80037BC4_387C4(func_80037BC4_387C4_arg *arg0) {
    arg0->unkD8 = freeNodeMemory(arg0->unkD8);
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_80037BFC_387FC(func_80037BFC_387FC_arg *arg0) {
    void *allocation;
    s32 i;
    s32 minusE0;
    s16 temp;
    s16 const1;
    s16 const2;
    s16 const3;
    s16 const4;
    u8 const5;

    getCurrentAllocation();
    allocation = dmaRequestAndUpdateStateWithSize(&_4196E0_ROM_START, &_4196E0_ROM_END, 0xBB8);
    arg0->unk90 = func_80035F80_36B80(1);
    setCleanupCallback(func_80037E40_38A40);

    i = 0;
    const1 = -0x80;
    const2 = 1;
    const3 = -0x7C;
    const4 = 0xFF;
    const5 = 5;
    for (i = 0; i < 4; i++) {
        arg0->entries1[i].unk0 = const1;
        arg0->entries1[i].unk2 = -0x20 + 0x20 * i;
        arg0->entries1[i].unk4 = allocation;
        arg0->entries1[i].unk8 = 0;
        arg0->entries1[i].unkA = 0;
        arg0->entries1[i].unkC = 0;
        arg0->entries1[i].unkD = const2;

        arg0->entries2[i].unk0 = const3;
        arg0->entries2[i].unk2 = arg0->entries1[i].unk2;
        arg0->entries2[i].unk4 = D_8008FE54_90A54[i];
        arg0->entries2[i].unk8 = arg0->unk90;
        arg0->entries2[i].unkC = 0;
        arg0->entries2[i].unkE = const4;
        arg0->entries2[i].unk10 = const5;
    }

    arg0->entries1[3].unk0 = -0x44;
    arg0->entries1[3].unk2 = 0x48;
    arg0->entries2[3].unk0 = -0x40;
    arg0->entries2[3].unk2 = arg0->entries1[3].unk2 + 2;

    setCallback(func_80037D18_38918);
}

INCLUDE_ASM("asm/nonmatchings/38310", func_80037D18_38918);

void func_80037E40_38A40(func_80037E40_38A40_arg *arg0) {
    arg0->unk48 = freeNodeMemory(arg0->unk48);
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_80037F14_38B14(Entry1 *);
void func_80037FB0_38BB0(func_80037FB0_38BB0_arg *arg0);

void func_80037E78_38A78(Entry1 *arg0) {
    void *allocation;
    s32 i;
    s32 minus32;
    s32 val;

    getCurrentAllocation();
    allocation = dmaRequestAndUpdateStateWithSize(&_4237C0_ROM_START, &_426EF0_ROM_START, 0x8A08);
    setCleanupCallback(func_80037FB0_38BB0);

    i = 0;
    minus32 = -32;
    val = 8;
    do {
        arg0[i].unk0 = val;
        arg0[i].unk2 = minus32;
        arg0[i].unk4 = allocation;
        arg0[i].unk8 = i;
        arg0[i].unkA = 0;
        arg0[i].unkD = 0;
        arg0[i].unkC = 0;
        i++;
        val += 0x78;
    } while (i < 2);

    setCallback(func_80037F14_38B14);
}

void func_80037F14_38B14(Entry1 *arg0) {
    func_80037F14_alloc *alloc = getCurrentAllocation();
    s32 i;

    for (i = 0; i < 2; i++) {
        if (alloc->unk1E2 != 0) {
            continue;
        }
        if (alloc->unk1EC >= 3) {
            continue;
        }
        arg0[i].unk2 = (alloc->unk1EC * 32) - 32;
        arg0[i].unkA = alloc->unk1E8[alloc->unk1EC];
        debugEnqueueCallback(8, 0, func_80012FA8_13BA8, &arg0[i]);
    }
}

void func_80037FB0_38BB0(func_80037FB0_38BB0_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}
