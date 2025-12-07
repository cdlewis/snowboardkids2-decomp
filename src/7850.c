#include "20F0.h"
#include "common.h"
#include "memory_allocator.h"
#include "task_scheduler.h"

typedef struct {
    u8 _pad[0x16];
    u16 unk16;
} func_80006CBC_unk10;

typedef struct {
    u8 _pad[0x10];
    func_80006CBC_unk10 *unk10;
    u8 _pad2[0x18];
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    u8 _pad4[0x4];
    s8 unk3C;
    s8 unk3D;
    u8 _pad5;
    s8 unk3F;
    u8 _pad6[0x48];
    s8 unk88;
} func_80006CBC_inner;

typedef struct {
    func_80006CBC_inner *unk0;
    s32 unk4;
    u8 padding[0x4C];
    s32 unk54;
    s32 unk58;
} func_80006C50_7850_arg;

typedef struct {
    u8 padding[0x28];
    void *unk28;
    void *unk2C;
} func_80007030_7C30_arg;

extern s32 func_80009E68_AA68(void *, s16);
extern void func_80009F5C_AB5C(s32 *);

void func_80007030_7C30(func_80007030_7C30_arg *);
void func_80009F90_AB90(s32 *, s32, s32, s32);
void func_80069CF8_6A8F8(void);
s32 func_8000A030_AC30(void *, s32);
void func_8000A190_AD90(s32 *ptr, u16 arg1, s32 x, s32 y, s32 z, s32 scaleX, s32 scaleY, u8 arg7, u8 arg8, u8 arg9);

void func_80006CBC_78BC(func_80006C50_7850_arg *);
extern void func_80006E44_7A44(func_80006C50_7850_arg *);
extern void *D_80088720_89320;
extern void *func_80006EE0_7AE0(void);
extern u8 identityMatrix[];

void func_80006C50_7850(func_80006C50_7850_arg *arg0) {
    s32 *temp_s0;

    temp_s0 = &arg0->unk4;
    arg0->unk54 = 0xC80000;
    arg0->unk58 = 0x80000;
    func_80009E68_AA68(temp_s0, 7);
    func_80009F90_AB90(temp_s0, 0x10000, 0, -1);
    setCleanupCallback(&func_80006E44_7A44);
    setCallback(&func_80006CBC_78BC);
}

void func_80006CBC_78BC(func_80006C50_7850_arg *arg0) {
    s8 sp28[2];
    func_80006CBC_inner *temp;
    func_80006CBC_inner *v0;
    s32 s3;
    s32 s4;
    s32 s1;

    do {
    } while (0);

    sp28[0] = 1;
    sp28[1] = -1;

    if (arg0->unk0->unk3C == 1) {
        func_80069CF8_6A8F8();
        return;
    }

    switch (arg0->unk0->unk3D) {
        default:

        case 0:
            arg0->unk54 += arg0->unk58;
            if (arg0->unk54 > 0xC80000) {
                arg0->unk58 = 0xFFF80000;
            } else if (arg0->unk54 <= 0x63FFFF) {
                arg0->unk58 = 0x80000;
            }
            break;

        case 1:
            arg0->unk54 += 0xFFF80000;
            if (arg0->unk54 < 0) {
                arg0->unk54 = 0;
            }
            break;

        case 2:
            arg0->unk54 += 0x40000;
            if (arg0->unk54 > 0xC80000) {
                v0 = arg0->unk0;
                arg0->unk58 = 0xC80000;
                v0->unk3D = 0;
            }
            break;
    }

    v0 = arg0->unk0;
    s3 = v0->unk2C;
    s4 = v0->unk30;
    s1 = v0->unk34;
    func_8000A030_AC30(&arg0->unk4, 0x10000);

    temp = arg0->unk0;
    if (temp->unk88 == 0) {
        return;
    }

    if (temp->unk3F == 0) {
        return;
    }

    func_8000A190_AD90(&arg0->unk4, temp->unk10->unk16, s3, s4, s1, 0x10000, 0x10000, 0, 0, ((u8 *)(&arg0->unk54))[1]);
}

void func_80006E44_7A44(func_80006C50_7850_arg *arg0) {
    func_80009F5C_AB5C(&arg0->unk4);
}

typedef struct {
    void *unk0;
    u8 unk4[0x20];
    void *unk24;
    MemoryAllocatorNode *unk28;
    MemoryAllocatorNode *unk2C;
    s32 unk30;
    u8 padding[0xC];
    s16 unk40;
    s16 unk42;
} func_80006E60_7A60_arg;

void func_80006E60_7A60(func_80006E60_7A60_arg *arg0) {
    memcpy(arg0->unk4, identityMatrix, 0x20);
    arg0->unk28 = loadAssetGroupDisplayList(arg0->unk0);
    arg0->unk2C = loadAssetGroupVertexData(arg0->unk0);
    arg0->unk30 = 0;
    arg0->unk24 = &D_80088720_89320;
    arg0->unk40 = 0;
    arg0->unk42 = 0;
    setCleanupCallback(&func_80007030_7C30);
    setCallback(&func_80006EE0_7AE0);
}

INCLUDE_ASM("asm/nonmatchings/7850", func_80006EE0_7AE0);

void func_80007030_7C30(func_80007030_7C30_arg *arg0) {
    arg0->unk28 = freeNodeMemory(arg0->unk28);
    arg0->unk2C = freeNodeMemory(arg0->unk2C);
}
