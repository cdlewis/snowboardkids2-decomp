#include "20F0.h"
#include "common.h"
#include "memory_allocator.h"

INCLUDE_ASM("asm/nonmatchings/7850", func_80006C50_7850);

INCLUDE_ASM("asm/nonmatchings/7850", func_80006CBC_78BC);

INCLUDE_ASM("asm/nonmatchings/7850", func_80006E44_7A44);

void func_80069CC0_6A8C0(void *);
void func_80069CE8_6A8E8(void *);
void memcpy(void *, void *, s32);

extern void *D_80088720_89320;
extern void *func_80006EE0_7AE0;
extern void *func_80007030_7C30;
extern u8 D_8009A870_9B470[];

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
    memcpy(arg0->unk4, D_8009A870_9B470, 0x20);
    arg0->unk28 = loadAssetGroupDisplayList(arg0->unk0);
    arg0->unk2C = loadAssetGroupVertexData(arg0->unk0);
    arg0->unk30 = 0;
    arg0->unk24 = &D_80088720_89320;
    arg0->unk40 = 0;
    arg0->unk42 = 0;
    func_80069CE8_6A8E8(&func_80007030_7C30);
    func_80069CC0_6A8C0(&func_80006EE0_7AE0);
}

INCLUDE_ASM("asm/nonmatchings/7850", func_80006EE0_7AE0);

INCLUDE_ASM("asm/nonmatchings/7850", func_80007030_7C30);
