#include "6E840.h"

#include "common.h"
#include "memory_allocator.h"

typedef struct Node {
    struct Node *next;
    void *unk4;
    void *unk8;
    u8 padding[0x3];
    u8 unkF;
} Node;

typedef struct {
    u8 data[0x10];
} CallbackSlot;

typedef struct
{
    u8 data[0x18];
    Node *unk18;
} CallbackData;

typedef struct {
    u8 padding[0xBC];
    s8 unkBC;
    s8 unkBD;
    s8 unkBE;
} func_8006FE28_70A28_arg;

typedef struct {
    u8 padding[0xB8];
    s8 unkB8;
} func_8006FEBC_70ABC_arg;

typedef struct {
    u8 padding[0xB8];
    s8 unkB8;
} func_8006FEE8_70AE8_arg;

typedef struct {
    u8 padding[0xB8];
    s8 unkB8;
    s8 unkB9;
    s8 unkBA;
    s8 unkBB;
} func_8006FE94_70A94_arg;

typedef struct
{
    u8 padding[0xC8];
    s16 unkC8;
    s16 unkCA;
    u8 padding2[0x104];
    f32 unk1D0;
} func_8006F9BC_705BC_arg;

extern CallbackSlot *D_800A3588_A4188[];
extern u32 __additional_scanline_0;
extern s32 gRegionAllocEnd;
extern void **gLinearArenaRegions;
extern s32 gRegionAllocPtr;
extern u16 D_800A3410_A4010;
extern u16 D_800A3412_A4012;
extern u16 D_800A8A9A_9FE0A;
extern u16 D_800AB478_A27E8;
void *LinearAlloc(size_t size);
extern func_8006FDA0_709A0_arg *D_800A3370_A3F70;
extern void *gArenaBasePtr;
extern void *gLinearAllocPtr;
extern void *gLinearAllocEnd;
extern s32 D_800A3548_A4148[];
extern s32 D_800A3550_A4150[];
extern s32 D_800AB12C_A249C;
extern void *D_800A3564_A4164;
extern u32 D_800A3568_A4168;
extern void *gLinearArenaBuffer;
void func_8006DEE4_6EAE4();
extern u8 D_800A356C_A416C;

INCLUDE_ASM("asm/nonmatchings/6E840", func_8006DC40_6E840);

INCLUDE_ASM("asm/nonmatchings/6E840", func_8006DEE4_6EAE4);

void func_8006E000_6EC00(s32 arg0) {
    s32 temp_a0 = arg0 & 0xF;
    D_800A3548_A4148[temp_a0] = 0;
    D_800AB12C_A249C = D_800A3550_A4150[temp_a0];
}

void func_8006E02C_6EC2C(void) {
    if (D_800A356C_A416C != 0) {
        func_8006DEE4_6EAE4();
    }
}

INCLUDE_ASM("asm/nonmatchings/6E840", func_8006E054_6EC54);

void osViExtendVStart(u32 arg0) {
    __additional_scanline_0 = arg0;
}

INCLUDE_ASM("asm/nonmatchings/6E840", func_8006F504_70104);

INCLUDE_ASM("asm/nonmatchings/6E840", func_8006F550_70150);

void *arenaAlloc16(s32 size) {
    void *result;
    u32 alignedSize = (size + 0xF) & ~0xF;
    u32 *cur = (u32 *)D_800A3564_A4164;
    u32 *end = (u32 *)D_800A3568_A4168;

    if ((u32 *)((u8 *)cur + alignedSize) > end) {
        return NULL;
    }

    result = cur;
    D_800A3564_A4164 = (void *)((u8 *)cur + alignedSize);
    return result;
}

void func_8006F5C0_701C0() {
    void *result;
    u8 *nodeExists;

    result = allocateMemoryNode(0, 0x10000, &nodeExists);
    gArenaBasePtr = result;
}

void resetLinearAllocator(void) {
    gLinearAllocPtr = gArenaBasePtr;
    gLinearAllocEnd = gArenaBasePtr + 0x10000;
}

void *linearAlloc(size_t size) {
    // Load the current pointer and limit
    u8 *base = (u8 *)gLinearAllocPtr;
    u8 *limit = (u8 *)gLinearAllocEnd;

    // Compute the new pointer
    u8 *newPtr = base + size;

    // If we exceed the limit, return NULL
    if (newPtr > limit) {
        return NULL;
    }

    // Otherwise, update the global "current" pointer and return the old base
    gLinearAllocPtr = newPtr;
    return base;
}

void *advanceLinearAlloc(s32 arg0) {
    // ensure next allocation will be aligned (0x8 boundary)
    return linearAlloc((arg0 + 7) & ~7);
}

void initLinearArenaRegions() {
    s32 temp;
    void *result;
    u8 *nodeExists;

    result = allocateMemoryNode(0, 0x10000, &nodeExists);
    gLinearArenaRegions = result;

    result = allocateMemoryNode(0, 0x10000, &nodeExists);
    gLinearArenaBuffer = result;
}

void linearAllocSelectRegion(s32 region) {
    s32 temp_v0;

    temp_v0 = ((s32 *)&gLinearArenaRegions)[region];

    gRegionAllocPtr = temp_v0;
    gRegionAllocEnd = temp_v0 + 0x10000;
}

s32 func_8006F6D8_702D8(void) {
    return (u32)(gRegionAllocEnd - gRegionAllocPtr) < 0x1AE1U;
}

void func_8006F6F4_702F4() {
    D_800A3410_A4010 = D_800AB478_A27E8;
    D_800A3412_A4012 = D_800A8A9A_9FE0A;
}

INCLUDE_ASM("asm/nonmatchings/6E840", func_8006F718_70318);

INCLUDE_ASM("asm/nonmatchings/6E840", func_8006F82C_7042C);

INCLUDE_ASM("asm/nonmatchings/6E840", func_8006F994_70594);

void func_8006F9BC_705BC(func_8006F9BC_705BC_arg *arg0, f32 arg1, f32 arg2) {
    arg0->unk1D0 = arg2;
    arg0->unkC8 = (s16)(arg1 * 640.0f);
    arg0->unkCA = (s16)(arg2 * 480.0f);
}

INCLUDE_ASM("asm/nonmatchings/6E840", func_8006FA0C_7060C);

INCLUDE_ASM("asm/nonmatchings/6E840", func_8006FA58_70658);

INCLUDE_ASM("asm/nonmatchings/6E840", func_8006FAA4_706A4);

void func_8006FC68_70868(void) {
}

INCLUDE_ASM("asm/nonmatchings/6E840", func_8006FC70_70870);

INCLUDE_ASM("asm/nonmatchings/6E840", func_8006FD3C_7093C);

void func_8006FDA0_709A0(func_8006FDA0_709A0_arg *arg0, u8 arg1, u8 arg2) {
    func_8006FDA0_709A0_arg *var_a0;

    var_a0 = arg0;
    if (var_a0 == NULL) {
        var_a0 = &D_800A3370_A3F70;
    }

    var_a0->unkC0 = arg1;
    var_a0->unkC1 = arg2;
    if (!(arg2 & 0xFF)) {
        var_a0->unkBF = arg1;
    }
}

INCLUDE_ASM("asm/nonmatchings/6E840", func_8006FDC8_709C8);

INCLUDE_ASM("asm/nonmatchings/6E840", func_8006FE10_70A10);

void func_8006FE28_70A28(func_8006FE28_70A28_arg *arg0, s8 arg1, s8 arg2, s8 arg3) {
    func_8006FE28_70A28_arg *var_a0;

    var_a0 = arg0;
    if (var_a0 == NULL) {
        var_a0 = &D_800A3370_A3F70;
    }
    var_a0->unkBC = arg1;
    var_a0->unkBD = arg2;
    var_a0->unkBE = arg3;
}
INCLUDE_ASM("asm/nonmatchings/6E840", func_8006FE48_70A48);

void func_8006FE94_70A94(func_8006FE94_70A94_arg *arg0, s8 arg1, s8 arg2, s8 arg3) {
    if (arg0 != NULL) {
        arg0->unkB9 = arg1;
        arg0->unkBA = arg2;
        arg0->unkBB = arg3;
        arg0->unkB8 = (u8)(arg0->unkB8 | 1);
    }
}

void func_8006FEBC_70ABC(func_8006FEBC_70ABC_arg *arg0) {
    if (arg0 != NULL) {
        arg0->unkB8 = (u8)(arg0->unkB8 & 0xFE);
    }
}

INCLUDE_ASM("asm/nonmatchings/6E840", func_8006FED8_70AD8);

void func_8006FEE8_70AE8(func_8006FEE8_70AE8_arg *arg0) {
    arg0->unkB8 = (u8)(arg0->unkB8 & 0xFD);
}

INCLUDE_ASM("asm/nonmatchings/6E840", func_8006FEF8_70AF8);

INCLUDE_ASM("asm/nonmatchings/6E840", n_alSynRemovePlayer);

void debugEnqueueCallback(u16 index, u8 slotIndex, void *callbackFn, void *callbackData) {
    CallbackSlot *manager = D_800A3588_A4188[index];
    if (manager != NULL) {
        Node *block = (Node *)linearAlloc(0x10);
        if (block != NULL) {
            CallbackData *slot = (CallbackData *)&manager[slotIndex];
            block->next = slot->unk18;
            block->unk4 = callbackFn;
            block->unk8 = callbackData;
            block->unkF = slotIndex;
            slot->unk18 = block;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/6E840", func_8007001C_70C1C);

INCLUDE_ASM("asm/nonmatchings/6E840", func_80070094_70C94);

INCLUDE_ASM("asm/nonmatchings/6E840", func_80070140_70D40);
