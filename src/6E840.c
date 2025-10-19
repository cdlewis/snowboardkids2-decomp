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
    u8 padding[0x8];
    Node *unk8;
    s32 unkC;
} Item_A4188;

typedef struct {
    u8 padding[0xB8];
    s8 unkB8;
} func_8006FEBC_70ABC_arg;

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
} ViewportStruct;

typedef struct {
    u8 padding[0x134];
    u32 unk134;
    u32 unk138;
    u32 unk13C;
} D_800AB068_A23D8_type;

extern D_800AB068_A23D8_type *D_800AB068_A23D8;
extern Node_70B00 D_800A3370_A3F70;
extern ViewportStruct D_800A3410_A4010;
extern s8 D_800A3429_A4029;
extern s8 D_800A342A_A402A;
extern s8 D_800A342B_A402B;
extern s8 D_800A342C_A402C;
extern s8 D_800A342D_A402D;
extern s8 D_800A342E_A402E;
extern s8 D_800A342F_A402F;
extern s8 D_800A3431_A4031;
extern s32 D_800A35C8_A41C8[];
extern s16 D_800A8A9A_9FE0A;
extern s16 D_800AB478_A27E8;

extern ViewportStruct D_800A3410_A4010;
extern Item_A4188 *D_800A3588_A4188[];
extern u32 __additional_scanline_0;
extern s32 gRegionAllocEnd;
extern void **gLinearArenaRegions;
extern s32 gRegionAllocPtr;
extern void *gArenaBasePtr;
extern void *gLinearAllocPtr;
extern void *gLinearAllocEnd;
extern s32 D_800A3548_A4148[];
extern s32 D_800A3550_A4150[];
extern s32 gBufferedFrameCounter;
extern void *D_800A3564_A4164;
extern u32 D_800A3568_A4168;
extern void *gLinearArenaBuffer;
extern u8 D_800A356C_A416C;
extern void *D_800A355C_A415C[];
extern void *D_800A3560_A4160;

void *LinearAlloc(size_t size);
void func_8006F6F4_702F4(void);
void func_8006FA58_70658(Node_70B00 *);
void func_8006DEE4_6EAE4(void);

INCLUDE_ASM("asm/nonmatchings/6E840", func_8006DC40_6E840);

INCLUDE_ASM("asm/nonmatchings/6E840", func_8006DEE4_6EAE4);

void func_8006E000_6EC00(s32 arg0) {
    s32 temp_a0 = arg0 & 0xF;
    D_800A3548_A4148[temp_a0] = 0;
    gBufferedFrameCounter = D_800A3550_A4150[temp_a0];
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

void func_8006F504_70104(void) {
    u8 exists;

    D_800A355C_A415C[0] = allocateMemoryNode(0, 0x18000, &exists);
    D_800A3560_A4160 = allocateMemoryNode(0, 0x18000, &exists);
}

void func_8006F550_70150(s32 arg0) {
    void *temp_v0 = D_800A355C_A415C[arg0];
    D_800A3564_A4164 = temp_v0;
    D_800A3568_A4168 = (s32)temp_v0 + 0x18000;
}

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

void func_8006F5C0_701C0(void) {
    void *result;
    u8 nodeExists;

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

void initLinearArenaRegions(void) {
    s32 temp;
    void *result;
    u8 nodeExists;

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

void func_8006F6F4_702F4(void) {
    D_800A3410_A4010.unk0 = D_800AB478_A27E8;
    D_800A3410_A4010.unk2 = D_800A8A9A_9FE0A;
}

void func_8006F718_70318(void) {
    s32 i;
    s32 *ptr;

    D_800A3370_A3F70.unk0.next = NULL;
    D_800A3370_A3F70.slot_index = 0xFFFF;
    D_800A3410_A4010.unk4 = -0xA0;
    D_800A3410_A4010.unk6 = -0x78;
    D_800A3410_A4010.unk8 = 0xA0;
    D_800A3370_A3F70.prev = NULL;
    D_800A3370_A3F70.unk8.list2_next = NULL;
    D_800A3370_A3F70.list2_prev = NULL;
    D_800A3370_A3F70.list3_next = NULL;
    D_800A3370_A3F70.unk14 = 0;
    D_800A3410_A4010.unk0 = 0;
    D_800A3410_A4010.unk2 = 0;
    D_800A3410_A4010.unkA = 0x78;
    D_800A342C_A402C = 0;
    D_800A342D_A402D = 0;
    D_800A342E_A402E = 0;
    D_800A342F_A402F = 0;
    D_800A3431_A4031 = 0;
    D_800AB478_A27E8 = 0;
    D_800A8A9A_9FE0A = 0;
    D_800A3429_A4029 = 0;
    D_800A342A_A402A = 0;
    D_800A342B_A402B = 0;
    func_8006FA58_70658(&D_800A3370_A3F70);
    resetLinearAllocator();
    func_8006F6F4_702F4();

    i = 0x10;
    ptr = &D_800A35C8_A41C8[0];
    do {
        *ptr = 0;
        i--;
        ptr--;
    } while (i >= 0);
}

INCLUDE_ASM("asm/nonmatchings/6E840", func_8006F82C_7042C);

INCLUDE_ASM("asm/nonmatchings/6E840", setModelCameraTransform);

void func_8006F9BC_705BC(Node_70B00 *arg0, f32 arg1, f32 arg2) {
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

void func_8006FDA0_709A0(Node_70B00 *arg0, u8 arg1, u8 arg2) {
    Node_70B00 *var_a0;

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

void func_8006FE28_70A28(Node_70B00 *arg0, s8 arg1, s8 arg2, s8 arg3) {
    Node_70B00 *var_a0;

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

void func_8006FEE8_70AE8(func_8006FEBC_70ABC_arg *arg0) {
    arg0->unkB8 = (u8)(arg0->unkB8 & 0xFD);
}

INCLUDE_ASM("asm/nonmatchings/6E840", func_8006FEF8_70AF8);

void unlinkNode(Node_70B00 *node) {
    Node_70B00 *current;
    Node_70B00 *next;

    current = &D_800A3370_A3F70;
    D_800A3588_A4188[node->slot_index] = NULL;

    next = D_800A3370_A3F70.unk8.list2_next;
    while (next != 0) {
        if (current->unk0.next == node) {
            current->unk0.next = node->unk0.next;
        }

        current = current->unk8.list2_next;
        next = current->unk8.list2_next;
    }

    if (node->unk8.list2_next != 0) {
        node->unk8.list2_next->prev = node->prev;
    }

    node->prev->unk8.list2_next = node->unk8.list2_next;
    if (node->list3_next != 0) {
        node->list3_next->list2_prev = node->list2_prev;
    }

    node->list2_prev->list3_next = node->list3_next;
}

void debugEnqueueCallback(u16 index, u8 slotIndex, void *callback, void *callbackData) {
    Item_A4188 *manager;
    Node *block;
    Item_A4188 *slot;

    manager = D_800A3588_A4188[index];
    if (manager != NULL) {
        block = (Node *)linearAlloc(0x10);
        if (block != NULL) {
            slot = &manager[slotIndex];
            block->next = slot[1].unk8;
            block->unk4 = callback;
            block->unk8 = callbackData;
            block->unkF = slotIndex;
            slot[1].unk8 = block;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/6E840", func_8007001C_70C1C);

void func_80070094_70C94(u16 id, u8 listIndex, void *data1, void *data2) {
    Node_70B00 *mgr;
    Node *newNode;
    u8 *listPtr;
    Node *oldHead;

    mgr = &D_800A3370_A3F70;

    while (mgr != NULL) {
        if (mgr->id == id) {
            newNode = (Node *)linearAlloc(0x10);
            if (newNode != NULL) {
                listPtr = (u8 *)mgr + (listIndex << 4);
                oldHead = *(Node **)(listPtr + 0x18);
                newNode->unk4 = data1;
                newNode->unk8 = data2;
                newNode->unkF = listIndex;
                newNode->next = oldHead;
                *(Node **)(listPtr + 0x18) = newNode;
            }
        }
        mgr = mgr->list3_next;
    }
}

s32 func_80070140_70D40(func_80070140_70D40_arg *arg0) {
    if (D_800AB068_A23D8->unk134 - arg0->unk0 + 0x0FEA0000 > 0x1FD40000) {
        return 1;
    }

    if (D_800AB068_A23D8->unk138 - arg0->unk4 + 0x0FEA0000 > 0x1FD40000) {
        return 1;
    }

    // compiler nonsense
    if (((!arg0) && (!arg0)) && (!arg0)) {}

    return D_800AB068_A23D8->unk13C - arg0->unk8 + 0x0FEA0000 > 0x1FD40000;
}