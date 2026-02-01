#include "graphics.h"

#include "buffers.h"
#include "common.h"
#include "geometry.h"
#include "memory_allocator.h"
#include "thread_manager.h"

#define BUFFER_SIZE 0x10000
#define MEMORY_HEAP_SIZE 0x200000
#define gMemoryHeapEnd (gMemoryHeapBase + MEMORY_HEAP_SIZE)

// Array view of arena regions [gLinearArenaRegions, gLinearArenaBuffer]
#define gLinearArenaRegionsArray ((s32 *)&gLinearArenaRegions)

typedef struct Node {
    struct Node *next;
    void *callback;
    void *callbackData;
    u8 padding[0x3];
    u8 poolIndex;
} Node;

// Microcode pointer pair
typedef struct {
    u64 *ucode;
    u64 *ucode_data;
} UcodeEntry;

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
} gActiveViewport_type;

typedef struct {
    /* 0x00 */ u32 type;
    /* 0x04 */ u32 flags;
    /* 0x08 */ void *ucode_boot;
    /* 0x0C */ u32 ucode_boot_size;
    /* 0x10 */ void *ucode;
    /* 0x14 */ u32 ucode_size;
    /* 0x18 */ void *output_buff_size;
    /* 0x1C */ u32 ucode_data_size;
    /* 0x20 */ void *ucode_data;
    /* 0x24 */ u32 dram_stack_size;
    /* 0x28 */ void *dram_stack;
    /* 0x2C */ u32 task_2C;
    /* 0x30 */ void *data_ptr;
    /* 0x34 */ u32 data_size;
    /* 0x38 */ void *output_buff;
    /* 0x3C */ u32 yield_data_size;
    /* 0x40 */ u32 pad40[2];
    /* 0x48 */ void *yield_data_ptr;
    /* 0x4C */ u16 unk4C;
    /* 0x4E */ u16 unk4E;
    /* 0x50 */ Gfx displayList[15];
    u32 pad[34];
} DisplayBufferMsg;

// Screen border overlay region with color
typedef struct {
    s16 clipLeft;
    s16 clipTop;
    s16 clipRight;
    s16 clipBottom;
    u8 displayFlags;
    u8 overlayR;
    u8 overlayG;
    u8 overlayB;
    u8 envR;
    u8 envG;
    u8 envB;
    u8 envA;
} BorderData;

// Text rendering clip region and offset
typedef struct {
    s16 clipLeft;
    s16 clipTop;
    s16 clipRight;
    s16 clipBottom;
    s16 offsetX;
    s16 offsetY;
} TextClipAndOffsetData;

extern void *gDisplayBufferMsgs;
extern s32 D_800A35C8_A41C8[];
extern s16 gViewportOriginY;
extern s16 gViewportOriginX;
extern Item_A4188 *D_800A3588_A4188[];
extern u32 __additional_scanline_0;
extern s32 gRegionAllocEnd;
extern void **gLinearArenaRegions;
extern Gfx *gRegionAllocPtr;
extern void *gArenaBasePtr;
extern void *gLinearAllocPtr;
extern void *gLinearAllocEnd;
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

extern s32 gFrameCounter;
extern s16 identityMatrix[];
extern gActiveViewport_type *gActiveViewport;

extern s32 gFrameSkipCounter;

extern void *gDramStack;
extern void *gOutputBuffer;
extern void *gYieldBuffer;

extern Gfx gDefaultRenderDisplayList[];
extern UcodeEntry microcodeGroups[];
extern Gfx gInitDisplayList[];

extern Gfx gFadeOverlayDisplayList[];
extern u16 gCallbackEntrySegment;

extern u8 gNeedsDisplayListInit;

extern ViewportNode *gLastViewportInGroup;
extern s32 gCallbackCounter;
extern s16 gCurrentPoolIndex;
extern s16 gGraphicsMode;
extern s16 gTextureEnabled;
extern OSMesgQueue mainMessageQueue;

extern void *gLookAtPtr;
extern TextClipAndOffsetData gTextClipAndOffsetData;

void selectGraphicsArena(s32);
void linearAllocSelectRegion(s32);
void updateViewportBounds(void);
void initViewportCallbackPool(ViewportNode *);
s32 isRegionAllocSpaceLow(void);
void resetLinearAllocator(void);

void *LinearAlloc(size_t size);
void restoreViewportOffsets(void);

void initGraphicsSystem(void);
void initGraphicsArenas(void);
void initLinearAllocator(void);
void initLinearArenaRegions(void);

void func_8006DC40_6E840(void) {
    DisplayBufferMsg *msg;
    u8 exists;
    s32 i;
    Gfx *gfx;

    initLinearArenaRegions();
    initLinearAllocator();
    initGraphicsArenas();

    gDramStack = allocateMemoryNode(0, 0x400, &exists);
    gOutputBuffer = allocateMemoryNode(0, BUFFER_SIZE, &exists);
    gYieldBuffer = allocateMemoryNode(0, 0xC00, &exists);
    initGraphicsSystem();

    gFrameBufferFlags[0] = 0;
    gFrameBufferFlags[1] = 0;
    gFrameCounter = 1;
    gBufferedFrameCounter = 0;
    gFrameSkipCounter = 0;
    __additional_scanline_0 = 0;
    gDisplayFramePending = 0;

    gDisplayBufferMsgs = msg = allocateMemoryNode(0, 3 * sizeof(DisplayBufferMsg), &exists);

    for (i = 0; i < 3; msg++, i++) {
        gfx = msg->displayList;
        gSPSegment(gfx++, 0, 0);
        gSPDisplayList(gfx++, gDefaultRenderDisplayList);
        gDPSetScissor(gfx++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        gDPSetCycleType(gfx++, G_CYC_FILL);
        gDPSetRenderMode(gfx++, G_RM_NOOP, G_RM_NOOP2);
        gDPSetColorImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WIDTH, &gFrameBuffer);
        gDPSetFillColor(gfx++, 0xFFFCFFFC);
        gDPFillRectangle(gfx++, 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
        gDPPipeSync(gfx++);
        gDPSetColorImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WIDTH, &gAuxFrameBuffers[i]);
        gDPSetFillColor(gfx++, 0x10001);
        gDPFillRectangle(gfx++, 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
        gDPPipeSync(gfx++);
        gDPFullSync(gfx++);
        gSPEndDisplayList(gfx++);

        msg->yield_data_ptr = &gAuxFrameBuffers[i];
        msg->unk4C = 0;
        msg->unk4E = 2;

        msg->data_ptr = msg->displayList;
        msg->data_size = 0x78;
        msg->type = 1;
        msg->flags = 0;
        msg->ucode_boot = rspbootTextStart;

        msg->ucode_boot_size = (u32)aspMainTextStart;
        msg->ucode_boot_size = msg->ucode_boot_size - ((u32)rspbootTextStart);

        msg->ucode = (void *)microcodeGroups[1].ucode;
        msg->output_buff_size = (void *)microcodeGroups[1].ucode_data;
        msg->ucode_data_size = 0x800;
        msg->ucode_data = gDramStack;
        msg->dram_stack_size = 0x400;
        msg->dram_stack = gOutputBuffer;
        msg->task_2C = (u32)msg->dram_stack + BUFFER_SIZE;
        msg->output_buff = gYieldBuffer;
        msg->yield_data_size = 0xC00;
    }
}

void processDisplayFrameUpdate(void) {
    ViewportNode *node;
    ViewportNode *temp;

    temp = gRootViewport.list3_next;
    gDisplayFramePending = 0;
    if (temp == NULL) {
        temp = &gRootViewport;
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

/*
 * Main per-frame rendering dispatch.
 *
 * This function orchestrates the entire frame rendering pipeline:
 *   Phase 0: Update fade animations on all viewports
 *   Early return: Skip rendering if framebuffer is busy or delay counter active
 *   Phase 1: Build RSP tasks for each viewport priority group
 *   Phase 2: Construct RDP display lists for each visible viewport
 *
 * viScanline: current VI scanline position, used for task scheduling
 */
void renderFrame(u32 viScanline) {
    ViewportNode *node;
    ViewportNode *rootNode;
    ViewportNode **rootNodePtr;
    Gfx *displayListStart;
    Gfx *displayListEnd;
    s32 needsDisplayListSetup;
    CallbackEntry *callbackEntry;
    void *viewportAlloc;
    void *projectionAlloc;
    s32 *lookAtAlloc;
    Light *lightArray;
    s32 i;
    u32 storedViScanline;
    s32 temp;
    s32 pipeSyncW;
    u8 padding[0x20];

    // Phase 0: Update fade animations
    for (node = &gRootViewport; node != NULL; node = node->list3_next) {
        if (node->fadeMode != 0) {
            temp = node->fadeValue - node->prevFadeValue;
            temp /= node->fadeMode;
            node->prevFadeValue += temp;
            node->fadeMode--;
        }
    }

    // Early return: framebuffer busy or delay active
    if (gFrameBufferFlags[gCurrentDoubleBufferIndex] != 0 || gFrameSkipCounter != 0) {
        if (gFrameSkipCounter != 0) {
            gFrameSkipCounter = gFrameSkipCounter - 1;
        }

        for (node = &gRootViewport; node != NULL; node = node->list3_next) {
            initViewportCallbackPool(node);
        }

        resetLinearAllocator();
        return;
    }

    // Main rendering path setup
    selectGraphicsArena(gCurrentDoubleBufferIndex);
    linearAllocSelectRegion(gCurrentDoubleBufferIndex);
    gFrameSkipCounter = __additional_scanline_0;
    gFrameBufferCounters[gCurrentDoubleBufferIndex] = gFrameCounter;
    updateViewportBounds();

    // Find the root viewport node
    rootNodePtr = &gRootViewport.list3_next;
    rootNode = *rootNodePtr;
    if (!rootNode) {
        rootNode = &gRootViewport;
    }

    node = rootNode;
    needsDisplayListSetup = TRUE;
    if (node != NULL) {
        pipeSyncW = 0xE7000000;
        storedViScanline = viScanline + 3;

        do {
            temp = node->priority;

            while (node->list3_next != NULL) {
                node->frameCallbackMsg = NULL;
                if (node->list3_next->priority != (u8)temp) {
                    break;
                }

                node = node->list3_next;
            }

            node->frameCallbackMsg = arenaAlloc16(0x50);
            displayListStart = gRegionAllocPtr;

            gSPSegment(gRegionAllocPtr++, 0, 0);
            node->displayListPtr = gRegionAllocPtr;
            {
                Gfx *_g = gRegionAllocPtr++;
                _g->words.w0 = pipeSyncW;
                _g->words.w1 = 0;
            }
            {
                Gfx *_g = gRegionAllocPtr++;
                _g->words.w0 = pipeSyncW;
                _g->words.w1 = 0;
            }
            gDPFullSync(gRegionAllocPtr++);
            gSPEndDisplayList(gRegionAllocPtr++);
            displayListEnd = gRegionAllocPtr;

            if (node->list3_next == NULL) {
                node->frameCallbackMsg->taskFlags = 1;
                node->frameCallbackMsg->msgQueue = &mainMessageQueue;
                // Construct segmented address: offset (lower 16 bits) | segment (upper 16 bits)
                callbackEntry = (CallbackEntry *)((u32)callbackEntry & 0xFFFF);
                callbackEntry = (CallbackEntry *)((u32)callbackEntry | (gCallbackEntrySegment << 16));
                node->frameCallbackMsg->msgData = (s32)callbackEntry;
            } else {
                node->frameCallbackMsg->taskFlags = 0;
            }

            node->frameCallbackMsg->auxBuffer =
                (void *)((u8 *)gAuxFrameBuffers +
                         ((gCurrentDisplayBufferIndex * 5 * 16 - gCurrentDisplayBufferIndex * 5) << 11));
            node->frameCallbackMsg->scanlineValue = storedViScanline + __additional_scanline_0 * 2;

            node->frameCallbackMsg->t.t.data_ptr = (u64 *)displayListStart;
            node->frameCallbackMsg->t.t.data_size = (s32)displayListEnd - (s32)displayListStart;
            node->frameCallbackMsg->t.t.type = M_GFXTASK;
            node->frameCallbackMsg->t.t.flags = 0;
            node->frameCallbackMsg->t.t.ucode_boot = (u64 *)rspbootTextStart;
            node->frameCallbackMsg->t.t.ucode_boot_size = (s32)aspMainTextStart - (s32)rspbootTextStart;
            node->frameCallbackMsg->t.t.ucode = microcodeGroups[temp].ucode;
            node->frameCallbackMsg->t.t.ucode_data = microcodeGroups[temp].ucode_data;
            node->frameCallbackMsg->t.t.ucode_data_size = 0x800;
            node->frameCallbackMsg->t.t.dram_stack = (u64 *)gDramStack;
            node->frameCallbackMsg->t.t.dram_stack_size = 0x400;
            node->frameCallbackMsg->t.t.output_buff = (u64 *)gOutputBuffer;
            node->frameCallbackMsg->t.t.output_buff_size = (u64 *)((s32)gOutputBuffer + BUFFER_SIZE);
            node->frameCallbackMsg->t.t.yield_data_ptr = (u64 *)gYieldBuffer;
            node->frameCallbackMsg->t.t.yield_data_size = 0xC00;
            node = node->list3_next;
        } while (node != NULL);
    }

    node = rootNode;
    needsDisplayListSetup = TRUE;
    if (node != NULL) {
        for (node = rootNode; node != NULL; node = node->list3_next) {
            // Future cleanup: ActiveViewport is probably just ViewportNode
            gActiveViewport = (gActiveViewport_type *)node;

            if (!isRegionAllocSpaceLow() && node->clipLeft < node->clipRight && node->clipTop < node->clipBottom) {

                if (needsDisplayListSetup) {
                    displayListStart = gRegionAllocPtr;
                    if (gNeedsDisplayListInit != 0) {
                        gNeedsDisplayListInit = 0;
                        gSPDisplayList(gRegionAllocPtr++, gInitDisplayList);
                    }

                    gSPDisplayList(gRegionAllocPtr++, gDefaultRenderDisplayList);
                } else {
                    gDPPipeSync(gRegionAllocPtr++);
                }

                gDPSetScissor(
                    gRegionAllocPtr++,
                    G_SC_NON_INTERLACE,
                    node->clipLeft,
                    node->clipTop,
                    node->clipRight,
                    node->clipBottom
                );

                if (node->displayFlags & 0x2) { // VIEWPORT_DISPLAY_CLEAR_SCREEN flag
                    gDPSetCycleType(gRegionAllocPtr++, G_CYC_FILL);
                    gDPSetRenderMode(gRegionAllocPtr++, G_RM_NOOP, G_RM_NOOP2);
                    gDPSetColorImage(gRegionAllocPtr++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WIDTH, &gFrameBuffer);
                    gDPSetFillColor(gRegionAllocPtr++, 0xFFFCFFFC);
                    gDPFillRectangle(
                        gRegionAllocPtr++,
                        node->clipLeft,
                        node->clipTop,
                        node->clipRight,
                        node->clipBottom
                    );
                    needsDisplayListSetup = TRUE;
                }

                if (node->displayFlags & 0x1) { // VIEWPORT_DISPLAY_OVERLAY flag
                    gDPPipeSync(gRegionAllocPtr++);
                    gDPSetColorImage(
                        gRegionAllocPtr++,
                        G_IM_FMT_RGBA,
                        G_IM_SIZ_16b,
                        320,
                        (void *)((u8 *)gAuxFrameBuffers +
                                 ((gCurrentDisplayBufferIndex * 5 * 16 - gCurrentDisplayBufferIndex * 5) << 11))
                    );
                    gDPSetCycleType(gRegionAllocPtr++, G_CYC_1CYCLE);
                    gDPSetEnvColor(gRegionAllocPtr++, node->overlayR, node->overlayG, node->overlayB, 0xFF);
                    gDPSetCombineLERP(
                        gRegionAllocPtr++,
                        1,
                        0,
                        ENVIRONMENT,
                        0,
                        1,
                        0,
                        ENVIRONMENT,
                        0,
                        1,
                        0,
                        ENVIRONMENT,
                        0,
                        1,
                        0,
                        ENVIRONMENT,
                        0
                    );
                    gDPSetRenderMode(gRegionAllocPtr++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
                    gDPFillRectangle(
                        gRegionAllocPtr++,
                        node->clipLeft,
                        node->clipTop,
                        node->clipRight + 1,
                        node->clipBottom + 1
                    );
                    gDPPipeSync(gRegionAllocPtr++);
                    gDPSetDepthImage(gRegionAllocPtr++, &gFrameBuffer);
                    needsDisplayListSetup = FALSE;
                }

                if (needsDisplayListSetup) {
                    needsDisplayListSetup = FALSE;
                    gDPPipeSync(gRegionAllocPtr++);
                    gDPSetColorImage(
                        gRegionAllocPtr++,
                        G_IM_FMT_RGBA,
                        G_IM_SIZ_16b,
                        320,
                        (void *)((u8 *)gAuxFrameBuffers +
                                 ((gCurrentDisplayBufferIndex * 5 * 16 - gCurrentDisplayBufferIndex * 5) << 11))
                    );
                    gDPSetDepthImage(gRegionAllocPtr++, &gFrameBuffer);
                }

                gTextClipAndOffsetData.clipLeft = node->clipLeft;
                gTextClipAndOffsetData.clipTop = node->clipTop;
                gTextClipAndOffsetData.clipRight = node->clipRight;
                gTextClipAndOffsetData.clipBottom = node->clipBottom;
                gTextClipAndOffsetData.offsetX = node->offsetX;
                gTextClipAndOffsetData.offsetY = node->offsetY;

                gTextureEnabled = node->priority;
                gGraphicsMode = -1;

                if (node->priority == 0) {
                    gDPSetColorDither(gRegionAllocPtr++, G_CD_DISABLE);

                    for (callbackEntry = (CallbackEntry *)node->pool; callbackEntry != NULL;
                         callbackEntry = callbackEntry->next) {
                        if (callbackEntry->callback == NULL) {
                            continue;
                        }

                        if (isRegionAllocSpaceLow()) {
                            break;
                        }

                        gCurrentPoolIndex = callbackEntry->poolIndex;
                        ((void (*)(void *))callbackEntry->callback)(callbackEntry->callbackData);
                        gCallbackCounter++;
                    }

                    if (node->prevFadeValue != 0) {
                        gSPDisplayList(gRegionAllocPtr++, gFadeOverlayDisplayList);
                        gDPSetPrimColor(
                            gRegionAllocPtr++,
                            0,
                            0,
                            node->envR,
                            node->envG,
                            node->envB,
                            node->prevFadeValue
                        );
                        gSPTextureRectangle(
                            gRegionAllocPtr++,
                            node->clipLeft << 2,
                            node->clipTop << 2,
                            node->clipRight << 2,
                            node->clipBottom << 2,
                            0,
                            0,
                            0,
                            0x400,
                            0x400
                        );
                        gDPPipeSync(gRegionAllocPtr++);
                    }
                } else {
                    // Allocate Vp (16 bytes), projection matrix (64 bytes), and LookAt matrices (192 bytes)
                    viewportAlloc = arenaAlloc16(sizeof(node->viewportWidth) * 8);
                    projectionAlloc = arenaAlloc16(sizeof(node->perspectiveMatrix));
                    lookAtAlloc = arenaAlloc16(48 * sizeof(s32));

                    if (node->numLights > 0) {
                        // Allocate light array: numLights + 1 (the +1 is for ambient light)
                        lightArray = arenaAlloc16((node->numLights + 1) * sizeof(Light));
                        if (lightArray != NULL) {
                            // Copy each light from node->unk148 to allocated array
                            for (i = 0; i < node->numLights; i++) {
                                memcpy(
                                    (Light *)(i * sizeof(Light) + (u32)lightArray),
                                    (u8 *)(i * sizeof(Light) + (u32)node) + 0x148,
                                    sizeof(Light)
                                );
                                gSPLight(gRegionAllocPtr++, (Light *)(i * sizeof(Light) + (u32)lightArray), i + 1);
                            }

                            // Copy ambient light (at index numLights)
                            memcpy(
                                (Light *)(i * sizeof(Light) + (u32)lightArray),
                                (u8 *)(i * sizeof(Light) + (u32)node) + 0x148,
                                sizeof(Light)
                            );
                            gSPLight(gRegionAllocPtr++, (Light *)(i * sizeof(Light) + (u32)lightArray), i + 1);

                            gSPNumLights(gRegionAllocPtr++, node->numLights);
                        } else {
                            goto bail;
                        }
                    }

                    if (lookAtAlloc != NULL) {
                        memcpy(viewportAlloc, &node->viewportWidth, 16);
                        memcpy(projectionAlloc, &node->perspectiveMatrix, sizeof(node->perspectiveMatrix));

                        lookAtAlloc[0] = ((node->modelingMatrix.m[0][0] << 3) & 0xFFFF0000) +
                                         (u16)(((u16)node->modelingMatrix.m[1][0] << 16) >> 29);
                        lookAtAlloc[1] = (node->modelingMatrix.m[2][0] << 3) & 0xFFFF0000;
                        lookAtAlloc[2] = ((node->modelingMatrix.m[0][1] << 3) & 0xFFFF0000) +
                                         (u16)(((u16)node->modelingMatrix.m[1][1] << 16) >> 29);
                        lookAtAlloc[3] = (node->modelingMatrix.m[2][1] << 3) & 0xFFFF0000;
                        lookAtAlloc[4] = ((node->modelingMatrix.m[0][2] << 3) & 0xFFFF0000) +
                                         (u16)(((u16)node->modelingMatrix.m[1][2] << 16) >> 29);
                        lookAtAlloc[5] = (node->modelingMatrix.m[2][2] << 3) & 0xFFFF0000;
                        lookAtAlloc[6] = 0;
                        lookAtAlloc[7] = 1;
                        lookAtAlloc[8] = ((node->modelingMatrix.m[0][0] << 19) & 0xFFFF0000) +
                                         ((node->modelingMatrix.m[1][0] << 3) & 0xFFFF);
                        lookAtAlloc[9] = (node->modelingMatrix.m[2][0] << 19) & 0xFFFF0000;
                        lookAtAlloc[10] = ((node->modelingMatrix.m[0][1] << 19) & 0xFFFF0000) +
                                          ((node->modelingMatrix.m[1][1] << 3) & 0xFFFF);
                        lookAtAlloc[11] = (node->modelingMatrix.m[2][1] << 19) & 0xFFFF0000;
                        lookAtAlloc[12] = ((node->modelingMatrix.m[0][2] << 19) & 0xFFFF0000) +
                                          ((node->modelingMatrix.m[1][2] << 3) & 0xFFFF);
                        lookAtAlloc[13] = (node->modelingMatrix.m[2][2] << 19) & 0xFFFF0000;
                        lookAtAlloc[14] = 0;
                        lookAtAlloc[15] = 0;

                        lookAtAlloc[16] = BUFFER_SIZE;
                        lookAtAlloc[17] = 0;
                        lookAtAlloc[18] = 1;
                        lookAtAlloc[19] = 0;
                        lookAtAlloc[20] = 0;
                        lookAtAlloc[21] = BUFFER_SIZE;
                        lookAtAlloc[22] = ((-node->modelingMatrix.translation.x) & 0xFFFF0000) +
                                          (u16)(((u32)(-node->modelingMatrix.translation.y)) >> 16);
                        lookAtAlloc[23] = ((-node->modelingMatrix.translation.z) & 0xFFFF0000) + 1;
                        lookAtAlloc[24] = 0;
                        lookAtAlloc[25] = 0;
                        lookAtAlloc[26] = 0;
                        lookAtAlloc[27] = 0;
                        lookAtAlloc[28] = 0;
                        lookAtAlloc[29] = 0;
                        lookAtAlloc[30] =
                            -(node->modelingMatrix.translation.x << 16) + (u16)(-node->modelingMatrix.translation.y);
                        lookAtAlloc[31] = (-node->modelingMatrix.translation.z) << 16;

                        lookAtAlloc[32] = ((node->modelingMatrix.m[0][0] << 3) & 0xFFFF0000) +
                                          (u16)(((u16)node->modelingMatrix.m[0][1] << 16) >> 29);
                        lookAtAlloc[33] = (node->modelingMatrix.m[0][2] << 3) & 0xFFFF0000;
                        lookAtAlloc[34] = ((node->modelingMatrix.m[1][0] << 3) & 0xFFFF0000) +
                                          (u16)(((u16)node->modelingMatrix.m[1][1] << 16) >> 29);
                        lookAtAlloc[35] = (node->modelingMatrix.m[1][2] << 3) & 0xFFFF0000;
                        lookAtAlloc[36] = ((node->modelingMatrix.m[2][0] << 3) & 0xFFFF0000) +
                                          (u16)(((u16)node->modelingMatrix.m[2][1] << 16) >> 29);
                        lookAtAlloc[37] = (node->modelingMatrix.m[2][2] << 3) & 0xFFFF0000;
                        lookAtAlloc[38] = 0;
                        lookAtAlloc[39] = 1;
                        lookAtAlloc[40] = ((node->modelingMatrix.m[0][0] << 19) & 0xFFFF0000) +
                                          ((node->modelingMatrix.m[0][1] << 3) & 0xFFFF);
                        lookAtAlloc[41] = (node->modelingMatrix.m[0][2] << 19) & 0xFFFF0000;
                        lookAtAlloc[42] = ((node->modelingMatrix.m[1][0] << 19) & 0xFFFF0000) +
                                          ((node->modelingMatrix.m[1][1] << 3) & 0xFFFF);
                        lookAtAlloc[43] = (node->modelingMatrix.m[1][2] << 19) & 0xFFFF0000;
                        lookAtAlloc[44] = ((node->modelingMatrix.m[2][0] << 19) & 0xFFFF0000) +
                                          ((node->modelingMatrix.m[2][1] << 3) & 0xFFFF);
                        lookAtAlloc[45] = (node->modelingMatrix.m[2][2] << 19) & 0xFFFF0000;
                        lookAtAlloc[46] = 0;
                        lookAtAlloc[47] = 0;

                        gLookAtPtr = (void *)&lookAtAlloc[32];

                        gSPViewport(gRegionAllocPtr++, viewportAlloc);
                        gSPPerspNormalize(gRegionAllocPtr++, node->perspNorm);
                        gSPMatrix(gRegionAllocPtr++, projectionAlloc, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
                        gSPMatrix(gRegionAllocPtr++, lookAtAlloc, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
                        gSPMatrix(gRegionAllocPtr++, &lookAtAlloc[16], G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);

                        gSPFogPosition(gRegionAllocPtr++, node->fogMin, node->fogMax);
                        gDPSetFogColor(gRegionAllocPtr++, node->fogR, node->fogG, node->fogB, node->fogA);
                    } else {
                        goto bail;
                    }

                    for (callbackEntry = (CallbackEntry *)node->pool; callbackEntry != NULL;
                         callbackEntry = callbackEntry->next) {
                        if (callbackEntry->callback == NULL) {
                            continue;
                        }

                        if (isRegionAllocSpaceLow() != 0) {
                            break;
                        }

                        gCurrentPoolIndex = callbackEntry->poolIndex;
                        ((void (*)(void *))callbackEntry->callback)(callbackEntry->callbackData);
                        gCallbackCounter++;
                    }

                bail:
                    if (node->prevFadeValue != 0) {
                        gSPDisplayList(gRegionAllocPtr++, gFadeOverlayDisplayList);
                        gDPSetPrimColor(
                            gRegionAllocPtr++,
                            0,
                            0,
                            node->envR,
                            node->envG,
                            node->envB,
                            node->prevFadeValue
                        );
                        gSPTextureRectangle(
                            gRegionAllocPtr++,
                            node->clipLeft << 2,
                            node->clipTop << 2,
                            node->clipRight << 2,
                            node->clipBottom << 2,
                            0,
                            0,
                            0,
                            0x400,
                            0x400
                        );
                        gDPPipeSync(gRegionAllocPtr++);
                        gDPSetColorDither(gRegionAllocPtr++, G_CD_MAGICSQ);
                    }
                }
            }

            if (node->frameCallbackMsg != NULL) {
                if (!needsDisplayListSetup) {
                    // If this is the last viewport and root has a fade value, render root's fade overlay
                    if (gRootViewport.prevFadeValue != 0 && node->list3_next == 0) {
                        BorderData *bd = (BorderData *)&gRootViewport.clipLeft;

                        gSPDisplayList(gRegionAllocPtr++, gFadeOverlayDisplayList);

                        gDPSetScissor(
                            gRegionAllocPtr++,
                            G_SC_NON_INTERLACE,
                            bd->clipLeft,
                            bd->clipTop,
                            bd->clipRight,
                            bd->clipBottom
                        );

                        gDPSetPrimColor(gRegionAllocPtr++, 0, 0, bd->envR, bd->envG, bd->envB, bd->envA);

                        gSPTextureRectangle(
                            gRegionAllocPtr++,
                            bd->clipLeft << 2,
                            bd->clipTop << 2,
                            bd->clipRight << 2,
                            bd->clipBottom << 2,
                            0,
                            0,
                            0,
                            0x400,
                            0x400
                        );
                    }

                    gSPEndDisplayList(gRegionAllocPtr++);
                    gSPDisplayList(node->displayListPtr, displayListStart);
                }
                needsDisplayListSetup = TRUE;
            }

            initViewportCallbackPool(node);
        }
    }

    if (gFrameBufferFlags[(gCurrentDoubleBufferIndex + 1) & 1] != 0) {
        gDisplayFramePending = TRUE;
    } else {
        processDisplayFrameUpdate();
    }

    resetLinearAllocator();
}

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

    result = allocateMemoryNode(0, BUFFER_SIZE, &nodeExists);
    gArenaBasePtr = result;
}

void resetLinearAllocator(void) {
    gLinearAllocPtr = gArenaBasePtr;
    gLinearAllocEnd = gArenaBasePtr + BUFFER_SIZE;
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

    result = allocateMemoryNode(0, BUFFER_SIZE, &nodeExists);
    gLinearArenaRegions = result;

    result = allocateMemoryNode(0, BUFFER_SIZE, &nodeExists);
    gLinearArenaBuffer = result;
}

void linearAllocSelectRegion(s32 region) {
    s32 temp_v0;

    temp_v0 = gLinearArenaRegionsArray[region];

    gRegionAllocPtr = (Gfx *)temp_v0;
    gRegionAllocEnd = temp_v0 + BUFFER_SIZE;
}

s32 isRegionAllocSpaceLow(void) {
    return (u32)(gRegionAllocEnd - (u32)gRegionAllocPtr) < 0x1AE1U;
}

void restoreViewportOffsets(void) {
    gRootViewport.originX = gViewportOriginX;
    gRootViewport.originY = gViewportOriginY;
}

void initGraphicsSystem(void) {
    s32 i;
    s32 *ptr;

    gRootViewport.unk0.next = NULL;
    gRootViewport.slot_index = 0xFFFF;
    gRootViewport.viewportLeft = -0xA0;
    gRootViewport.viewportTop = -0x78;
    gRootViewport.viewportRight = 0xA0;
    gRootViewport.prev = NULL;
    gRootViewport.unk8.list2_next = NULL;
    gRootViewport.list2_prev = NULL;
    gRootViewport.list3_next = NULL;
    gRootViewport.unk14 = 0;
    gRootViewport.originX = 0;
    gRootViewport.originY = 0;
    gRootViewport.viewportBottom = 0x78;
    gRootViewport.envR = 0;
    gRootViewport.envG = 0;
    gRootViewport.envB = 0;
    gRootViewport.prevFadeValue = 0;
    gRootViewport.fadeMode = 0;
    gViewportOriginX = 0;
    gViewportOriginY = 0;
    gRootViewport.overlayR = 0;
    gRootViewport.overlayG = 0;
    gRootViewport.overlayB = 0;
    initViewportCallbackPool(&gRootViewport);
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
    ViewportNode *childNode;
    ViewportNode *node;
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
    node = &gRootViewport;
    inheritedMaxY = 0xEF; // screen height - 1 (240 - 1)

    if (node != NULL) {
        do {
            childNode = node->unk0.next;
            if (childNode != NULL) {
                inheritedCenterX = childNode->offsetX;
                inheritedCenterY = childNode->offsetY;
                inheritedMinX = childNode->clipLeft;
                inheritedMinY = childNode->clipTop;
                inheritedMaxX = childNode->clipRight;
                inheritedMaxY = childNode->clipBottom;
            }
            node->offsetX = inheritedCenterX + (u16)node->originX;
            node->offsetY = inheritedCenterY + (u16)node->originY;
            node->unkD0 = node->offsetX * 4;
            node->unkD2 = node->offsetY * 4;
            node->clipLeft = (u16)node->offsetX + (u16)node->viewportLeft;
            node->clipTop = (u16)node->offsetY + (u16)node->viewportTop;
            node->clipRight = (u16)node->offsetX + (u16)node->viewportRight;
            node->clipBottom = (u16)node->offsetY + (u16)node->viewportBottom;
            if (node->clipLeft < (s16)inheritedMinX) {
                node->clipLeft = inheritedMinX;
            }
            if (node->clipTop < (s16)inheritedMinY) {
                node->clipTop = inheritedMinY;
            }
            if ((s16)inheritedMaxX < node->clipRight) {
                node->clipRight = inheritedMaxX;
            }
            if ((s16)inheritedMaxY < node->clipBottom) {
                node->clipBottom = inheritedMaxY;
            }
            computedLeft = node->clipLeft;
            if (node->clipRight < computedLeft) {
                node->clipRight = computedLeft;
            }
            computedTop = node->clipTop;
            if (node->clipBottom < computedTop) {
                node->clipBottom = computedTop;
            }
            node = node->unk8.list2_next;
        } while (node != NULL);
    }
}

void setModelCameraTransform(void *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s16 arg5, s16 arg6) {
    ViewportNode *node = (ViewportNode *)arg0;
    node->originX = arg1;
    node->originY = arg2;
    node->viewportLeft = arg3;
    node->viewportTop = arg4;
    node->viewportRight = arg5;
    node->viewportBottom = arg6;
}

void setViewportScale(ViewportNode *arg0, f32 scaleX, f32 scaleY) {
    arg0->scaleY = scaleY;
    arg0->viewportWidth = (s16)(scaleX * 640.0f);
    arg0->viewportHeight = (s16)(scaleY * 480.0f);
}

void func_8006FA0C_7060C(ViewportNode *node, f32 fov, f32 aspect, f32 near, f32 far) {
    guPerspective(&node->perspectiveMatrix, &node->perspNorm, fov, aspect, near, far, 1.0f);
}

void initViewportCallbackPool(ViewportNode *node) {
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

void initViewportNode(ViewportNode *arg0, ViewportNode *arg1, s32 arg2, s32 arg3, s32 arg4) {
    ViewportNode *temp_v0;
    ViewportNode *var_a0;
    u8 arg4_byte = (u8)arg4;

    D_800A3588_A4188[arg2 & 0xFFFF] = (Item_A4188 *)arg0;

    if (arg1 == NULL) {
        arg0->unk0.next = &gRootViewport;
        arg0->prev = &gRootViewport;
        temp_v0 = gRootViewport.unk8.list2_next;
        arg0->unk8.list2_next = temp_v0;
        if (temp_v0 != NULL) {
            temp_v0->prev = arg0;
        }
        gRootViewport.unk8.list2_next = arg0;
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

    var_a0 = &gRootViewport;
    if (gRootViewport.list3_next != NULL) {
        do {
            ViewportNode *temp_v1 = var_a0->list3_next;
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
    arg0->priority = (s8)arg4_byte;
    arg0->displayFlags = 0;
    arg0->id = 0;
    arg0->numLights = 0;
    arg0->viewportWidth = 0x280;
    arg0->viewportHeight = 0x1E0;
    arg0->unkCC = 0x1FF;
    arg0->unkCE = 0;
    arg0->unkD0 = 0x280;
    arg0->unkD2 = 0x1E0;
    arg0->unkD4 = 0x1FF;
    arg0->unkD6 = 0;
    memcpy(&arg0->modelingMatrix, identityMatrix, sizeof(Transform3D));
    guPerspective(&arg0->perspectiveMatrix, &arg0->perspNorm, 30.0f, 1.3333334f, 20.0f, 2000.0f, 1.0f);
    arg0->fogA = 0xFF;
    arg0->fogMin = 0x3DE;
    arg0->fogB = 0;
    arg0->fogG = 0;
    arg0->fogR = 0;
    arg0->fogMax = 0x3E6;
    arg0->envR = 0;
    arg0->envG = 0;
    arg0->envB = 0;
    arg0->prevFadeValue = 0;
    arg0->fadeMode = 0;
    arg0->scaleY = 1.0f;
    initViewportCallbackPool(arg0);
}

void nullViewportFunction(void) {
}

void setViewportLightColors(u16 viewportId, u16 colorCount, ColorData *lightColors, ColorData *ambientColor) {
    ViewportNode *viewport;
    s32 i;

    viewport = gRootViewport.unk8.list2_next;

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

            viewport->numLights = colorCount;
        }

        viewport = viewport->unk8.list2_next;
    }
}

void setViewportTransformById(u16 viewportId, void *transformMatrix) {
    ViewportNode *node;

    node = gRootViewport.unk8.list2_next;

    while (node != NULL) {
        if (node->id == viewportId) {
            memcpy(&node->modelingMatrix, transformMatrix, sizeof(Transform3D));
        }
        node = node->unk8.list2_next;
    }
}

void setViewportFadeValue(ViewportNode *node, u8 fadeValue, u8 fadeMode) {
    ViewportNode *targetNode;

    targetNode = node;
    if (targetNode == NULL) {
        targetNode = &gRootViewport;
    }

    targetNode->fadeValue = fadeValue;
    targetNode->fadeMode = fadeMode;
    if (!(fadeMode & 0xFF)) {
        targetNode->prevFadeValue = fadeValue;
    }
}

void setViewportFadeValueBySlotIndex(u16 slotIndex, u8 fadeValue, u8 fadeMode) {
    ViewportNode *node;

    node = &gRootViewport;
    while (node != NULL) {
        if (node->slot_index == slotIndex) {
            node->fadeValue = fadeValue;
            node->fadeMode = fadeMode;
            if (!(fadeMode & 0xFF)) {
                node->prevFadeValue = fadeValue;
            }
        }
        node = node->list3_next;
    }
}

s32 getViewportFadeMode(ViewportNode *arg0) {
    if (arg0 == NULL) {
        arg0 = &gRootViewport;
    }
    return arg0->fadeMode;
}

void setViewportEnvColor(ViewportNode *node, u8 r, u8 g, u8 b) {
    if (node == NULL) {
        node = &gRootViewport;
    }
    node->envR = r;
    node->envG = g;
    node->envB = b;
}

void setViewportFogById(u16 viewportId, s16 fogMin, s16 fogMax, u8 fogR, u8 fogG, u8 fogB) {
    ViewportNode *node;

    node = gRootViewport.unk8.list2_next;

    while (node != NULL) {
        if (node->id == viewportId) {
            node->fogMin = fogMin;
            node->fogMax = fogMax;
            node->fogR = fogR;
            node->fogG = fogG;
            node->fogB = fogB;
        }
        node = node->unk8.list2_next;
    }
}

void setViewportOverlayRgbAndEnable(ViewportNode *arg0, s8 r, s8 g, s8 b) {
    if (arg0 != NULL) {
        arg0->overlayR = r;
        arg0->overlayG = g;
        arg0->overlayB = b;
        arg0->displayFlags = (u8)(arg0->displayFlags | 1);
    }
}

void disableViewportOverlay(ViewportNode *arg0) {
    if (arg0 != NULL) {
        arg0->displayFlags = (u8)(arg0->displayFlags & 0xFE);
    }
}

void enableViewportDisplayList(void *arg0) {
    ((ViewportNode *)arg0)->displayFlags |= 0x2;
}

void disableViewportDisplayList(ViewportNode *arg0) {
    arg0->displayFlags = (u8)(arg0->displayFlags & 0xFD);
}

void setViewportId(ViewportNode *node, u16 id) {
    node->id = id;
}

void unlinkNode(ViewportNode *node) {
    ViewportNode *current;
    ViewportNode *next;

    current = &gRootViewport;
    D_800A3588_A4188[node->slot_index] = NULL;

    next = gRootViewport.unk8.list2_next;
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

void enqueueViewportCallback(ViewportNode *viewport, u8 poolIndex, void *callback, void *callbackData) {
    PoolEntry *newEntry;
    PoolEntry *oldHead;

    newEntry = (PoolEntry *)linearAlloc(0x10);
    if (newEntry != NULL) {
        oldHead = viewport->pool[poolIndex].next;
        newEntry->callback = callback;
        newEntry->callbackData = callbackData;
        newEntry->poolIndex = poolIndex;
        newEntry->next = oldHead;
        viewport->pool[poolIndex].next = newEntry;
    }
}

void enqueueViewportCallbackById(u16 viewportId, u8 poolIndex, void *callback, void *callbackData) {
    ViewportNode *viewport;
    PoolEntry *newEntry;
    PoolEntry *oldHead;

    viewport = &gRootViewport;

    while (viewport != NULL) {
        if (viewport->id == viewportId) {
            newEntry = (PoolEntry *)linearAlloc(sizeof(PoolEntry));
            if (newEntry != NULL) {
                oldHead = viewport->pool[poolIndex].next;
                newEntry->callback = callback;
                newEntry->callbackData = callbackData;
                newEntry->poolIndex = poolIndex;
                newEntry->next = oldHead;
                viewport->pool[poolIndex].next = newEntry;
            }
        }
        viewport = viewport->list3_next;
    }
}

s32 isObjectCulled(Vec3i *arg0) {
    if (gActiveViewport->cameraX - arg0->x + 0x0FEA0000 > 0x1FD40000) {
        return TRUE;
    }

    if (gActiveViewport->cameraY - arg0->y + 0x0FEA0000 > 0x1FD40000) {
        return TRUE;
    }

    // compiler nonsense
    if (((!arg0) && (!arg0)) && (!arg0)) {}

    return gActiveViewport->cameraZ - arg0->z + 0x0FEA0000 > 0x1FD40000;
}