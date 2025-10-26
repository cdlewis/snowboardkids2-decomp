#include "20F0.h"
#include "common.h"
#include "memory_allocator.h"
#include "task_scheduler.h"

typedef struct {
    s32 unk0;
    s32 unk4;
    u8 padding[0x4C];
    s32 unk54;
    s32 unk58;
} func_80006C50_7850_arg;

extern void *D_80088720_89320;
extern void *func_80006EE0_7AE0;
extern void *func_80007030_7C30;
extern u8 identityMatrix[];

void func_80009E68_AA68(s32 *, s32);
void func_80009F90_AB90(s32 *, s32, s32, s32);
extern void func_80006CBC_78BC(void);
extern void func_80006E44_7A44(void);

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

INCLUDE_ASM("asm/nonmatchings/7850", func_80006CBC_78BC);

INCLUDE_ASM("asm/nonmatchings/7850", func_80006E44_7A44);

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

INCLUDE_ASM("asm/nonmatchings/7850", func_80007030_7C30);
