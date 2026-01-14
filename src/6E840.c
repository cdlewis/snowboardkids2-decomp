#include "6E840.h"

#include "common.h"
#include "geometry.h"
#include "memory_allocator.h"
#include "thread_manager.h"

typedef struct Node {
    struct Node *next;
    void *callback;
    void *callbackData;
    u8 padding[0x3];
    u8 poolIndex;
} Node;

typedef struct {
    u8 padding[0x8];
    Node *unk8;
    s32 unkC;
} Item_A4188;

typedef struct {
    s16 originX;
    s16 originY;
    s16 left;
    s16 top;
    s16 right;
    s16 bottom;
} Viewport;

typedef struct {
    u8 padding[0x134];
    u32 cameraX;
    u32 cameraY;
    u32 cameraZ;
    u8 pad14C[4];
    u8 defaultLight1R;
    u8 defaultLight1G;
    u8 defaultLight1B;
    u8 pad14B;
    u8 pad14C_2[3];
    u8 defaultLight2R;
    u8 defaultLight2G;
    u8 defaultLight2B;
} D_800AB068_A23D8_type;

extern D_800AB068_A23D8_type *D_800AB068_A23D8;

extern Viewport D_800A3410_A4010;
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

// Array view of arena regions [gLinearArenaRegions, gLinearArenaBuffer]
#define gLinearArenaRegionsArray ((s32 *)&gLinearArenaRegions)
extern s32 gFrameBufferFlags[];
extern s32 gFrameBufferCounters[];
extern s32 gBufferedFrameCounter;
extern void *gGraphicsArenaCurr;
extern u32 gGraphicsArenaEnd;
extern void *gLinearArenaBuffer;
extern u8 gDisplayFramePending;
extern void *gGraphicsArenaPtrs[];
extern void *gGraphicsArena0;
extern s32 gCurrentDoubleBufferIndex;
extern s32 gCurrentDisplayBufferIndex;
extern void *gDisplayBufferMsgs;
extern s32 gFrameCounter;
extern s16 identityMatrix[];

void *LinearAlloc(size_t size);
void restoreViewportOffsets(void);
void initViewportCallbackPool(Node_70B00 *arg0);

INCLUDE_ASM("asm/nonmatchings/6E840", func_8006DC40_6E840);

void processDisplayFrameUpdate(void) {
    Node_70B00 *node;
    Node_70B00 *temp;

    temp = D_800A3370_A3F70.list3_next;
    gDisplayFramePending = 0;
    if (temp == NULL) {
        temp = &D_800A3370_A3F70;
    }
    node = temp;
    if (node != NULL) {
        do {
            if (node->frameCallbackMsg != 0) {
                gFrameBufferFlags[gCurrentDoubleBufferIndex] = 1;
                sendMessageToThreadSyncQueue((OSMesg)node->frameCallbackMsg);
            }
            node = node->list3_next;
        } while (node != NULL);
    }
    gFrameCounter = (gFrameCounter + 1) & 0x0FFFFFFF;
    gCurrentDoubleBufferIndex = (gCurrentDoubleBufferIndex + 1) & 1;
    gCurrentDisplayBufferIndex = gCurrentDisplayBufferIndex + 1;
    if (gCurrentDisplayBufferIndex >= 3) {
        gCurrentDisplayBufferIndex = 0;
    }
    sendMessageToThreadSyncQueue((OSMesg)((u8 *)gDisplayBufferMsgs + (gCurrentDisplayBufferIndex * 0x150)));
}

void handleFrameBufferComplete(s32 bufferIndex) {
    s32 index = bufferIndex & 0xF;
    gFrameBufferFlags[index] = 0;
    gBufferedFrameCounter = gFrameBufferCounters[index];
}

void tryProcessDisplayFrameUpdate(void) {
    if (gDisplayFramePending != 0) {
        processDisplayFrameUpdate();
    }
}

INCLUDE_ASM("asm/nonmatchings/6E840", func_8006E054_6EC54);

void osViExtendVStart(u32 arg0) {
    __additional_scanline_0 = arg0;
}

void initGraphicsArenas(void) {
    u8 exists;

    gGraphicsArenaPtrs[0] = allocateMemoryNode(0, 0x18000, &exists);
    gGraphicsArena0 = allocateMemoryNode(0, 0x18000, &exists);
}

void selectGraphicsArena(s32 arg0) {
    void *temp_v0 = gGraphicsArenaPtrs[arg0];
    gGraphicsArenaCurr = temp_v0;
    gGraphicsArenaEnd = (s32)temp_v0 + 0x18000;
}

void *arenaAlloc16(s32 size) {
    void *result;
    u32 alignedSize = (size + 0xF) & ~0xF;
    u32 *cur = (u32 *)gGraphicsArenaCurr;
    u32 *end = (u32 *)gGraphicsArenaEnd;

    if ((u32 *)((u8 *)cur + alignedSize) > end) {
        return NULL;
    }

    result = cur;
    gGraphicsArenaCurr = (void *)((u8 *)cur + alignedSize);
    return result;
}

void initLinearAllocator(void) {
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

    temp_v0 = gLinearArenaRegionsArray[region];

    gRegionAllocPtr = temp_v0;
    gRegionAllocEnd = temp_v0 + 0x10000;
}

s32 isRegionAllocSpaceLow(void) {
    return (u32)(gRegionAllocEnd - gRegionAllocPtr) < 0x1AE1U;
}

void restoreViewportOffsets(void) {
    D_800A3410_A4010.originX = D_800AB478_A27E8;
    D_800A3410_A4010.originY = D_800A8A9A_9FE0A;
}

void initGraphicsSystem(void) {
    s32 i;
    s32 *ptr;

    D_800A3370_A3F70.unk0.next = NULL;
    D_800A3370_A3F70.slot_index = 0xFFFF;
    D_800A3410_A4010.left = -0xA0;
    D_800A3410_A4010.top = -0x78;
    D_800A3410_A4010.right = 0xA0;
    D_800A3370_A3F70.prev = NULL;
    D_800A3370_A3F70.unk8.list2_next = NULL;
    D_800A3370_A3F70.list2_prev = NULL;
    D_800A3370_A3F70.list3_next = NULL;
    D_800A3370_A3F70.unk14 = 0;
    D_800A3410_A4010.originX = 0;
    D_800A3410_A4010.originY = 0;
    D_800A3410_A4010.bottom = 0x78;
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
    initViewportCallbackPool(&D_800A3370_A3F70);
    resetLinearAllocator();
    restoreViewportOffsets();

    i = 0x10;
    ptr = &D_800A35C8_A41C8[0];
    do {
        *ptr = 0;
        i--;
        ptr--;
    } while (i >= 0);
}

void updateViewportBounds(void) {
    Node_70B00 *childNode;
    Node_70B00 *node;
    u16 inheritedCenterX;
    u16 inheritedCenterY;
    u16 inheritedMinX;
    u16 inheritedMinY;
    u16 inheritedMaxX;
    u16 inheritedMaxY;
    s16 computedLeft;
    s16 computedTop;

    inheritedCenterX = 0xA0; // screen width / 2 (320 / 2)
    inheritedCenterY = 0x78; // screen height / 2 (240 / 2)
    inheritedMinX = 0;
    inheritedMinY = 0;
    inheritedMaxX = 0x13F; // screen width - 1 (320 - 1)
    node = &D_800A3370_A3F70;
    inheritedMaxY = 0xEF; // screen height - 1 (240 - 1)

    if (node != NULL) {
        do {
            childNode = node->unk0.next;
            if (childNode != NULL) {
                inheritedCenterX = childNode->unkAC;
                inheritedCenterY = childNode->unkAE;
                inheritedMinX = childNode->unkB0;
                inheritedMinY = childNode->unkB2;
                inheritedMaxX = childNode->unkB4;
                inheritedMaxY = childNode->unkB6;
            }
            node->unkAC = inheritedCenterX + (u16)node->unkA0;
            node->unkAE = inheritedCenterY + (u16)node->unkA2;
            node->unkD0 = node->unkAC * 4;
            node->unkD2 = node->unkAE * 4;
            node->unkB0 = (u16)node->unkAC + (u16)node->unkA4;
            node->unkB2 = (u16)node->unkAE + (u16)node->unkA6;
            node->unkB4 = (u16)node->unkAC + (u16)node->unkA8;
            node->unkB6 = (u16)node->unkAE + (u16)node->unkAA;
            if (node->unkB0 < (s16)inheritedMinX) {
                node->unkB0 = inheritedMinX;
            }
            if (node->unkB2 < (s16)inheritedMinY) {
                node->unkB2 = inheritedMinY;
            }
            if ((s16)inheritedMaxX < node->unkB4) {
                node->unkB4 = inheritedMaxX;
            }
            if ((s16)inheritedMaxY < node->unkB6) {
                node->unkB6 = inheritedMaxY;
            }
            computedLeft = node->unkB0;
            if (node->unkB4 < computedLeft) {
                node->unkB4 = computedLeft;
            }
            computedTop = node->unkB2;
            if (node->unkB6 < computedTop) {
                node->unkB6 = computedTop;
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

void setViewportScale(Node_70B00 *arg0, f32 scaleX, f32 scaleY) {
    arg0->scaleY = scaleY;
    arg0->viewportWidth = (s16)(scaleX * 640.0f);
    arg0->viewportHeight = (s16)(scaleY * 480.0f);
}

void func_8006FA0C_7060C(Node_70B00 *node, f32 fov, f32 aspect, f32 near, f32 far) {
    guPerspective(&node->perspectiveMatrix, &node->perspNorm, fov, aspect, near, far, 1.0f);
}

void initViewportCallbackPool(Node_70B00 *node) {
    s32 i;

    i = 7;
    while (i >= 0) {
        node->pool[i].callback = NULL;
        i--;
    }

    i = 1;
    while (i < 8) {
        node->pool[i - 1].next = &node->pool[i];
        i++;
    }

    node->unk88 = NULL;
}

void initViewportNode(Node_70B00 *arg0, Node_70B00 *arg1, s32 arg2, s32 arg3, s32 arg4) {
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
    arg0->viewportWidth = 0x280;
    arg0->viewportHeight = 0x1E0;
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
    arg0->scaleY = 1.0f;
    initViewportCallbackPool(arg0);
}

void nullViewportFunction(void) {
}

void setViewportLightColors(u16 viewportId, u16 colorCount, ColorData *lightColors, ColorData *ambientColor) {
    Node_70B00 *viewport;
    s32 i;

    viewport = D_800A3370_A3F70.unk8.list2_next;

    if (viewport == NULL) {
        return;
    }

    while (viewport != NULL) {
        if (viewport->id == viewportId) {
            for (i = 0; i < colorCount; i++) {
                viewport->unk148[i].light1R = viewport->unk148[i].light1R_dup = lightColors[i].r;
                viewport->unk148[i].light1G = viewport->unk148[i].light1G_dup = lightColors[i].g;
                viewport->unk148[i].light1B = viewport->unk148[i].light1B_dup = lightColors[i].b;
                viewport->unk148[i].light2R = lightColors[i].r2;
                viewport->unk148[i].light2G = lightColors[i].g2;
                viewport->unk148[i].light2B = lightColors[i].b2;
            }

            viewport->unk148[i].light1R = viewport->unk148[i].light1R_dup = ambientColor[0].r;
            viewport->unk148[i].light1G = viewport->unk148[i].light1G_dup = ambientColor[0].g;
            viewport->unk148[i].light1B = viewport->unk148[i].light1B_dup = ambientColor[0].b;

            viewport->unk140 = colorCount;
        }

        viewport = viewport->unk8.list2_next;
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
            block->callback = callback;
            block->callbackData = callbackData;
            block->poolIndex = slotIndex;
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
        entry->callback = arg2;
        entry->callbackData = arg3;
        entry->poolIndex = arg1;
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
                newNode->callback = data1;
                newNode->callbackData = data2;
                newNode->poolIndex = listIndex;
                newNode->next = oldHead;
                *(Node **)(listPtr + 0x18) = newNode;
            }
        }
        mgr = mgr->list3_next;
    }
}

s32 isObjectCulled(Vec3i *arg0) {
    if (D_800AB068_A23D8->cameraX - arg0->x + 0x0FEA0000 > 0x1FD40000) {
        return TRUE;
    }

    if (D_800AB068_A23D8->cameraY - arg0->y + 0x0FEA0000 > 0x1FD40000) {
        return TRUE;
    }

    // compiler nonsense
    if (((!arg0) && (!arg0)) && (!arg0)) {}

    return D_800AB068_A23D8->cameraZ - arg0->z + 0x0FEA0000 > 0x1FD40000;
}