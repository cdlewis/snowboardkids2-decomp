#include "6E840.h"

#include "common.h"
#include "geometry.h"
#include "memory_allocator.h"
#include "thread_manager.h"

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
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
} ViewportStruct;

typedef struct {
    u8 padding[0x134];
    u32 cameraX;
    u32 cameraY;
    u32 cameraZ;
} D_800AB068_A23D8_type;

extern D_800AB068_A23D8_type *D_800AB068_A23D8;

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
extern s32 D_8009AFC8_9BBC8;
extern s32 D_8009AFCC_9BBCC;
extern void *D_800A3570_A4170;
extern s32 gFrameCounter;
extern s16 identityMatrix[];

void *LinearAlloc(size_t size);
void func_8006F6F4_702F4(void);
void func_8006FA58_70658(Node_70B00 *arg0);

INCLUDE_ASM("asm/nonmatchings/6E840", func_8006DC40_6E840);

void func_8006DEE4_6EAE4(void) {
    Node_70B00 *node;
    Node_70B00 *temp;

    temp = D_800A3370_A3F70.list3_next;
    D_800A356C_A416C = 0;
    if (temp == NULL) {
        temp = &D_800A3370_A3F70;
    }
    node = temp;
    if (node != NULL) {
        do {
            if (node->unk9C != 0) {
                D_800A3548_A4148[D_8009AFC8_9BBC8] = 1;
                sendMessageToThreadSyncQueue((OSMesg)node->unk9C);
            }
            node = node->list3_next;
        } while (node != NULL);
    }
    gFrameCounter = (gFrameCounter + 1) & 0x0FFFFFFF;
    D_8009AFC8_9BBC8 = (D_8009AFC8_9BBC8 + 1) & 1;
    D_8009AFCC_9BBCC = D_8009AFCC_9BBCC + 1;
    if (D_8009AFCC_9BBCC >= 3) {
        D_8009AFCC_9BBCC = 0;
    }
    sendMessageToThreadSyncQueue((OSMesg)((u8 *)D_800A3570_A4170 + (D_8009AFCC_9BBCC * 0x150)));
}

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

void func_8006F82C_7042C(void) {
    Node_70B00 *temp_v0;
    Node_70B00 *node;
    u16 var_s0;
    u16 var_t9;
    u16 var_t8;
    u16 var_t7;
    u16 var_t6;
    u16 var_t5;
    s16 temp_b0;
    s16 temp_b2;

    var_s0 = 0xA0;
    var_t9 = 0x78;
    var_t8 = 0;
    var_t7 = 0;
    var_t6 = 0x13F;
    node = &D_800A3370_A3F70;
    var_t5 = 0xEF;

    if (node != NULL) {
        do {
            temp_v0 = node->unk0.next;
            if (temp_v0 != NULL) {
                var_s0 = temp_v0->unkAC;
                var_t9 = temp_v0->unkAE;
                var_t8 = temp_v0->unkB0;
                var_t7 = temp_v0->unkB2;
                var_t6 = temp_v0->unkB4;
                var_t5 = temp_v0->unkB6;
            }
            node->unkAC = var_s0 + (u16)node->unkA0;
            node->unkAE = var_t9 + (u16)node->unkA2;
            node->unkD0 = node->unkAC * 4;
            node->unkD2 = node->unkAE * 4;
            node->unkB0 = (u16)node->unkAC + (u16)node->unkA4;
            node->unkB2 = (u16)node->unkAE + (u16)node->unkA6;
            node->unkB4 = (u16)node->unkAC + (u16)node->unkA8;
            node->unkB6 = (u16)node->unkAE + (u16)node->unkAA;
            if (node->unkB0 < (s16)var_t8) {
                node->unkB0 = var_t8;
            }
            if (node->unkB2 < (s16)var_t7) {
                node->unkB2 = var_t7;
            }
            if ((s16)var_t6 < node->unkB4) {
                node->unkB4 = var_t6;
            }
            if ((s16)var_t5 < node->unkB6) {
                node->unkB6 = var_t5;
            }
            temp_b0 = node->unkB0;
            if (node->unkB4 < temp_b0) {
                node->unkB4 = temp_b0;
            }
            temp_b2 = node->unkB2;
            if (node->unkB6 < temp_b2) {
                node->unkB6 = temp_b2;
            }
            node = node->unk8.list2_next;
        } while (node != NULL);
    }
}

void setModelCameraTransform(void *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s16 arg5, s16 arg6) {
    Node_70B00 *node = (Node_70B00 *)arg0;
    node->unkA0 = arg1;
    node->unkA2 = arg2;
    node->unkA4 = arg3;
    node->unkA6 = arg4;
    node->unkA8 = arg5;
    node->unkAA = arg6;
}

void func_8006F9BC_705BC(Node_70B00 *arg0, f32 arg1, f32 arg2) {
    arg0->unk1D0 = arg2;
    arg0->unkC8 = (s16)(arg1 * 640.0f);
    arg0->unkCA = (s16)(arg2 * 480.0f);
}

void func_8006FA0C_7060C(Node_70B00 *arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4) {
    guPerspective(&arg0->perspectiveMatrix, &arg0->perspNorm, arg1, arg2, arg3, arg4, 1.0f);
}

void func_8006FA58_70658(Node_70B00 *arg0) {
    s32 i;

    i = 7;
    while (i >= 0) {
        arg0->pool[i].unk4 = NULL;
        i--;
    }

    i = 1;
    while (i < 8) {
        arg0->pool[i - 1].next = &arg0->pool[i];
        i++;
    }

    arg0->unk88 = NULL;
}

void func_8006FAA4_706A4(Node_70B00 *arg0, Node_70B00 *arg1, s32 arg2, s32 arg3, s32 arg4) {
    Node_70B00 *temp_v0;
    Node_70B00 *var_a0;
    u8 arg4_byte = (u8)arg4;

    D_800A3588_A4188[arg2 & 0xFFFF] = (Item_A4188 *)arg0;

    if (arg1 == NULL) {
        arg0->unk0.next = &D_800A3370_A3F70;
        arg0->prev = &D_800A3370_A3F70;
        temp_v0 = D_800A3370_A3F70.unk8.list2_next;
        arg0->unk8.list2_next = temp_v0;
        if (temp_v0 != NULL) {
            temp_v0->prev = arg0;
        }
        D_800A3370_A3F70.unk8.list2_next = arg0;
    } else {
        arg0->unk0.next = arg1;
        arg0->prev = arg1;
        temp_v0 = arg1->unk8.list2_next;
        arg0->unk8.list2_next = temp_v0;
        if (temp_v0 != NULL) {
            temp_v0->prev = arg0;
        }
        arg1->unk8.list2_next = arg0;
    }

    var_a0 = &D_800A3370_A3F70;
    if (D_800A3370_A3F70.list3_next != NULL) {
        do {
            Node_70B00 *temp_v1 = var_a0->list3_next;
            if ((u8)arg3 < (u8)temp_v1->unk14) {
                break;
            }
            var_a0 = temp_v1;
        } while (var_a0->list3_next != NULL);
    }

    arg0->list2_prev = var_a0;
    arg0->list3_next = var_a0->list3_next;
    var_a0->list3_next = arg0;
    temp_v0 = arg0->list3_next;
    if (temp_v0 != NULL) {
        temp_v0->list2_prev = arg0;
    }

    arg0->unk14 = (s8)arg3;
    arg0->slot_index = (u16)arg2;
    arg0->unk15 = (s8)arg4_byte;
    arg0->padding2b[0] = 0;
    arg0->id = 0;
    arg0->unk140 = 0;
    arg0->unkC8 = 0x280;
    arg0->unkCA = 0x1E0;
    arg0->unkCC = 0x1FF;
    arg0->unkCE = 0;
    arg0->unkD0 = 0x280;
    arg0->unkD2 = 0x1E0;
    arg0->unkD4 = 0x1FF;
    arg0->unkD6 = 0;
    memcpy(arg0->padding4, identityMatrix, 0x20);
    guPerspective(&arg0->perspectiveMatrix, &arg0->perspNorm, 30.0f, 1.3333334f, 20.0f, 2000.0f, 1.0f);
    arg0->padding1CF = 0xFF;
    arg0->unk1C8 = 0x3DE;
    arg0->unk1CE = 0;
    arg0->unk1CD = 0;
    arg0->unk1CC = 0;
    arg0->unk1CA = 0x3E6;
    arg0->unkBC = 0;
    arg0->unkBD = 0;
    arg0->unkBE = 0;
    arg0->unkBF = 0;
    arg0->unkC1 = 0;
    arg0->unk1D0 = 1.0f;
    func_8006FA58_70658(arg0);
}

void func_8006FC68_70868(void) {
}

void func_8006FC70_70870(u16 searchId, u16 colorCount, ColorData *srcColors, ColorData *finalColor) {
    Node_70B00 *node;
    s32 i;
    Node_70B00_ColorData *dst;

    node = D_800A3370_A3F70.unk8.list2_next;

    if (node == NULL) {
        return;
    }

    while (node != NULL) {
        if (node->id == searchId) {
            dst = &node->unk148[0];
            for (i = 0; i < colorCount; i++) {
                node->unk148[i].unk148 = node->unk148[i].unk14C = srcColors[i].r;
                node->unk148[i].unk149 = node->unk148[i].unk14D = srcColors[i].g;
                node->unk148[i].unk14A = node->unk148[i].unk14E = srcColors[i].b;
                node->unk148[i].unk150 = srcColors[i].r2;
                node->unk148[i].unk151 = srcColors[i].g2;
                node->unk148[i].unk152 = srcColors[i].b2;
            }

            node->unk148[i].unk148 = node->unk148[i].unk14C = finalColor[0].r;
            node->unk148[i].unk149 = node->unk148[i].unk14D = finalColor[0].g;
            node->unk148[i].unk14A = node->unk148[i].unk14E = finalColor[0].b;

            node->unk140 = colorCount;
        }

        node = node->unk8.list2_next;
    }
}

void func_8006FD3C_7093C(u16 arg0, void *arg1) {
    Node_70B00 *node;

    node = D_800A3370_A3F70.unk8.list2_next;

    while (node != NULL) {
        if (node->id == arg0) {
            memcpy(node->padding4, arg1, 0x20);
        }
        node = node->unk8.list2_next;
    }
}

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

void func_8006FDC8_709C8(u16 arg0, u8 arg1, u8 arg2) {
    Node_70B00 *node;

    node = &D_800A3370_A3F70;
    while (node != NULL) {
        if (node->slot_index == arg0) {
            node->unkC0 = arg1;
            node->unkC1 = arg2;
            if (!(arg2 & 0xFF)) {
                node->unkBF = arg1;
            }
        }
        node = node->list3_next;
    }
}

s32 func_8006FE10_70A10(Node_70B00 *arg0) {
    if (arg0 == NULL) {
        arg0 = &D_800A3370_A3F70;
    }
    return arg0->unkC1;
}

void func_8006FE28_70A28(Node_70B00 *arg0, u8 arg1, u8 arg2, u8 arg3) {
    Node_70B00 *var_a0;

    var_a0 = arg0;
    if (var_a0 == NULL) {
        var_a0 = &D_800A3370_A3F70;
    }
    var_a0->unkBC = arg1;
    var_a0->unkBD = arg2;
    var_a0->unkBE = arg3;
}

void func_8006FE48_70A48(u16 arg0, s16 arg1, s16 arg2, u8 arg3, u8 arg4, u8 arg5) {
    Node_70B00 *node;

    node = D_800A3370_A3F70.unk8.list2_next;

    while (node != NULL) {
        if (node->id == arg0) {
            node->unk1C8 = arg1;
            node->unk1CA = arg2;
            node->unk1CC = arg3;
            node->unk1CD = arg4;
            node->unk1CE = arg5;
        }
        node = node->unk8.list2_next;
    }
}

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

void func_8006FED8_70AD8(void *arg0) {
    ((func_8006FEBC_70ABC_arg *)arg0)->unkB8 = (u8)(((func_8006FEBC_70ABC_arg *)arg0)->unkB8 | 0x2);
}

void func_8006FEE8_70AE8(func_8006FEBC_70ABC_arg *arg0) {
    arg0->unkB8 = (u8)(arg0->unkB8 & 0xFD);
}

void func_8006FEF8_70AF8(Node_70B00 *arg0, u16 arg1) {
    arg0->id = arg1;
}

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

void func_8007001C_70C1C(Node_70B00 *arg0, u8 arg1, void *arg2, void *arg3) {
    PoolEntry *entry;
    PoolEntry *prev;

    entry = (PoolEntry *)linearAlloc(0x10);
    if (entry != NULL) {
        prev = arg0->pool[arg1].next;
        entry->unk4 = arg2;
        entry->unk8 = arg3;
        entry->unkF = arg1;
        entry->next = prev;
        arg0->pool[arg1].next = entry;
    }
}

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

s32 isObjectCulled(Vec3s32 *arg0) {
    if (D_800AB068_A23D8->cameraX - arg0->unk0 + 0x0FEA0000 > 0x1FD40000) {
        return TRUE;
    }

    if (D_800AB068_A23D8->cameraY - arg0->unk4 + 0x0FEA0000 > 0x1FD40000) {
        return TRUE;
    }

    // compiler nonsense
    if (((!arg0) && (!arg0)) && (!arg0)) {}

    return D_800AB068_A23D8->cameraZ - arg0->unk8 + 0x0FEA0000 > 0x1FD40000;
}