#include "20F0.h"
#include "common.h"
#include "memory_allocator.h"
#include "task_scheduler.h"

extern void *D_80088720_89320;
extern void *func_80006EE0_7AE0;
extern void *func_80007030_7C30;
extern u8 identityMatrix[];

INCLUDE_ASM("asm/nonmatchings/7850", func_80006C50_7850);

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
