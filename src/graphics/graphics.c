#include "graphics/graphics.h"

#include "common.h"
#include "core/buffers.h"
#include "gbi.h"
#include "gu.h"
#include "math/geometry.h"
#include "mbi.h"
#include "os_message.h"
#include "system/memory_allocator.h"
#include "system/thread_manager.h"
#include "ucode.h"

#define MEMORY_HEAP_SIZE 0x200000
#define gMemoryHeapEnd (gMemoryHeapBase + MEMORY_HEAP_SIZE)

// Array view of arena regions [gLinearArenaRegions, gLinearArenaBuffer]
#define gLinearArenaRegionsArray ((s32 *)&gLinearArenaRegions)

// Data segment definitions

Gfx gInitDisplayList[] = {
    gsDPPipeSync(),
    gsDPSetEnvColor(0, 0, 0, 0),
    gsDPSetPrimColor(0, 0, 0, 0, 0, 0),
    gsDPSetBlendColor(0, 0, 0, 0),
    gsDPSetFogColor(0, 0, 0, 0),
    gsDPSetFillColor(0),
    gsDPSetPrimDepth(0, 0),
    gsDPSetConvert(0, 0, 0, 0, 0, 0),
    gsDPSetKeyR(0, 0, 0),
    gsDPSetKeyGB(0, 0, 0, 0, 0, 0),
    gsDPNoOp(),
    gsDPSetTileSize(0, 0, 0, 0, 0),
    gsDPSetTileSize(1, 0, 0, 0, 0),
    gsDPSetTileSize(2, 0, 0, 0, 0),
    gsDPSetTileSize(3, 0, 0, 0, 0),
    gsDPSetTileSize(4, 0, 0, 0, 0),
    gsDPSetTileSize(5, 0, 0, 0, 0),
    gsDPSetTileSize(6, 0, 0, 0, 0),
    gsDPSetTileSize(7, 0, 0, 0, 0),
    gsDPSetTile(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
    gsDPSetTile(0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0),
    gsDPSetTile(0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0),
    gsDPSetTile(0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0),
    gsDPSetTile(0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0),
    gsDPSetTile(0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0),
    gsDPSetTile(0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0),
    gsDPSetTile(0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0),
    gsSPEndDisplayList(),
};

Gfx gDefaultRenderDisplayList[] = {
    gsDPPipeSync(),
    gsDPPipelineMode(G_PM_NPRIMITIVE),
    gsDPSetCombineKey(G_CK_NONE),
    gsDPSetTextureConvert(G_TC_FILT),
    gsDPSetDepthSource(G_ZS_PIXEL),
    gsDPSetAlphaCompare(G_AC_NONE),
    gsDPSetColorDither(G_CD_MAGICSQ),
    gsDPSetAlphaDither(G_AD_DISABLE),
    gsDPSetTextureLOD(G_TL_TILE),
    gsDPSetTextureDetail(G_TD_CLAMP),
    gsSPEndDisplayList(),
};

u32 D_8009AF28_9BB28[] = {
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
};

Gfx gFadeOverlayDisplayList[] = {
    gsDPPipeSync(),
    gsDPSetColorDither(G_CD_DISABLE),
    gsDPSetCycleType(G_CYC_1CYCLE),
    gsDPSetTexturePersp(G_TP_NONE),
    gsDPSetTextureFilter(G_TF_POINT),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPSetCombineMode(G_CC_MODULATEI_PRIM, G_CC_MODULATEI_PRIM),
    gsDPSetRenderMode(G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2),
    gsDPSetTextureImage(G_IM_FMT_I, G_IM_SIZ_8b, 8, D_8009AF28_9BB28),
    gsDPSetTile(
        G_IM_FMT_I,
        G_IM_SIZ_8b,
        1,
        0,
        G_TX_LOADTILE,
        0,
        G_TX_NOMIRROR | G_TX_WRAP,
        2,
        G_TX_NOLOD,
        G_TX_NOMIRROR | G_TX_WRAP,
        4,
        G_TX_NOLOD
    ),
    gsDPLoadSync(),
    gsDPLoadTile(G_TX_LOADTILE, 0, 0, 32, 16),
    gsDPPipeSync(),
    gsDPSetTile(
        G_IM_FMT_I,
        G_IM_SIZ_4b,
        1,
        0,
        G_TX_RENDERTILE,
        0,
        G_TX_NOMIRROR | G_TX_WRAP,
        2,
        G_TX_NOLOD,
        G_TX_NOMIRROR | G_TX_WRAP,
        4,
        G_TX_NOLOD
    ),
    gsDPSetTileSize(G_TX_RENDERTILE, 0, 0, 64, 16),
    gsSPEndDisplayList(),
};

s32 gCurrentDoubleBufferIndex = 0;
s32 gCurrentDisplayBufferIndex = 0;
s32 gFrameSkipCounter = 0;
u32 __additional_scanline_0 = 0;

UcodeEntry microcodeGroups[] = {
    { (u64 *)gspS2DEX_fifoTextStart,  (u64 *)gspS2DEX_fifoDataStart  },
    { (u64 *)gspF3DEX2_fifoTextStart, (u64 *)gspF3DEX2_fifoDataStart },
};

u8 gNeedsDisplayListInit = 1;

extern s16 gViewportOriginY;
extern s16 gViewportOriginX;
extern Gfx *gDisplayListAllocPtr;
extern s16 gTextureEnabled;
extern void *gLookAtPtr;

void *gDramStack BSS = 0;
void *gOutputBuffer BSS = 0;
void *gYieldBuffer BSS = 0;
u8 D_800A336C_9FDFC[4] BSS = { 0 };
ViewportNode gRootViewport BSS = { 0 };
s32 gFrameBufferFlags[2] BSS = { 0 };
s32 gFrameBufferCounters[2] BSS = { 0 };
s32 gRegionAllocEnd BSS = 0;
void *gGraphicsArenaPtrs[1] BSS = { 0 };
void *gGraphicsArena0 BSS = 0;
void *gGraphicsArenaCurr BSS = 0;
u32 gGraphicsArenaEnd BSS = 0;
u8 gDisplayFramePending BSS = 0;
void *gDisplayBufferMsgs BSS = 0;
void *gArenaBasePtr BSS = 0;
void *gLinearAllocPtr BSS = 0;
void *gLinearAllocEnd BSS = 0;
void **gLinearArenaRegions BSS = 0;
void *gLinearArenaBuffer BSS = 0;
ViewportNode *gViewportCallbackPools[0x10] BSS = { 0 };
s32 D_800A35C8_A0058[2] BSS = { 0 };

void restoreViewportOffsets(void);
void initGraphicsSystem(void);
void initGraphicsArenas(void);
void initLinearAllocator(void);
void initLinearArenaRegions(void);

void initDisplayBuffers(void) {
    DisplayBufferTask *msg;
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

    gDisplayBufferMsgs = msg = allocateMemoryNode(0, 3 * sizeof(DisplayBufferTask), &exists);

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

        msg->graphicsTask.framebuffer = &gAuxFrameBuffers[i];
        msg->graphicsTask.frameIndex = 0;
        msg->graphicsTask.flags = 2;

        msg->graphicsTask.task.t.data_ptr = (u64 *)msg->displayList;
        msg->graphicsTask.task.t.data_size = 0x78;
        msg->graphicsTask.task.t.type = 1;
        msg->graphicsTask.task.t.flags = 0;
        msg->graphicsTask.task.t.ucode_boot = (u64 *)rspbootTextStart;

        msg->graphicsTask.task.t.ucode_boot_size = (u32)aspMainTextStart;
        msg->graphicsTask.task.t.ucode_boot_size -= (u32)rspbootTextStart;

        msg->graphicsTask.task.t.ucode = microcodeGroups[1].ucode;
        msg->graphicsTask.task.t.ucode_data = microcodeGroups[1].ucode_data;
        msg->graphicsTask.task.t.ucode_data_size = 0x800;
        msg->graphicsTask.task.t.dram_stack = gDramStack;
        msg->graphicsTask.task.t.dram_stack_size = 0x400;
        msg->graphicsTask.task.t.output_buff = gOutputBuffer;
        msg->graphicsTask.task.t.output_buff_size = (u64 *)((u32)gOutputBuffer + BUFFER_SIZE);
        msg->graphicsTask.task.t.yield_data_ptr = gYieldBuffer;
        msg->graphicsTask.task.t.yield_data_size = 0xC00;
    }
}

void processDisplayFrameUpdate(void) {
    ViewportNode *node;
    ViewportNode *temp;

    temp = gRootViewport.renderNext;
    gDisplayFramePending = 0;
    if (temp == NULL) {
        temp = &gRootViewport;
    }
    node = temp;
    if (node != NULL) {
        do {
            if (node->graphicsTask != 0) {
                gFrameBufferFlags[gCurrentDoubleBufferIndex] = 1;
                submitDisplayTask((OSMesg)node->graphicsTask);
            }
            node = node->renderNext;
        } while (node != NULL);
    }
    gFrameCounter = (gFrameCounter + 1) & 0x0FFFFFFF;
    gCurrentDoubleBufferIndex = (gCurrentDoubleBufferIndex + 1) & 1;
    gCurrentDisplayBufferIndex = gCurrentDisplayBufferIndex + 1;
    if (gCurrentDisplayBufferIndex >= 3) {
        gCurrentDisplayBufferIndex = 0;
    }
    submitDisplayTask((OSMesg)((u8 *)gDisplayBufferMsgs + (gCurrentDisplayBufferIndex * 0x150)));
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
    for (node = &gRootViewport; node != NULL; node = node->renderNext) {
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

        for (node = &gRootViewport; node != NULL; node = node->renderNext) {
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
    rootNodePtr = &gRootViewport.renderNext;
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
            temp = node->uses3DRendering;

            while (node->renderNext != NULL) {
                node->graphicsTask = NULL;
                if (node->renderNext->uses3DRendering != (u8)temp) {
                    break;
                }

                node = node->renderNext;
            }

            node->graphicsTask = arenaAlloc16(0x50);
            displayListStart = gDisplayListAllocPtr;

            gSPSegment(gDisplayListAllocPtr++, 0, 0);
            node->displayListPtr = gDisplayListAllocPtr;
            {
                Gfx *_g = gDisplayListAllocPtr++;
                _g->words.w0 = pipeSyncW;
                _g->words.w1 = 0;
            }
            {
                Gfx *_g = gDisplayListAllocPtr++;
                _g->words.w0 = pipeSyncW;
                _g->words.w1 = 0;
            }
            gDPFullSync(gDisplayListAllocPtr++);
            gSPEndDisplayList(gDisplayListAllocPtr++);
            displayListEnd = gDisplayListAllocPtr;

            if (node->renderNext == NULL) {
                node->graphicsTask->flags = 1;
                node->graphicsTask->messageQueue = &mainMessageQueue;
                // Construct segmented address: offset (lower 16 bits) | segment (upper 16 bits)
                callbackEntry = (CallbackEntry *)((u32)callbackEntry & 0xFFFF);
                callbackEntry = (CallbackEntry *)((u32)callbackEntry | (gCallbackEntrySegment << 16));
                node->graphicsTask->completionMessage = (OSMesg)callbackEntry;
            } else {
                node->graphicsTask->flags = 0;
            }

            node->graphicsTask->framebuffer =
                (void *)((u8 *)gAuxFrameBuffers +
                         ((gCurrentDisplayBufferIndex * 5 * 16 - gCurrentDisplayBufferIndex * 5) << 11));
            node->graphicsTask->frameIndex = storedViScanline + __additional_scanline_0 * 2;

            node->graphicsTask->task.t.data_ptr = (u64 *)displayListStart;
            node->graphicsTask->task.t.data_size = (s32)displayListEnd - (s32)displayListStart;
            node->graphicsTask->task.t.type = M_GFXTASK;
            node->graphicsTask->task.t.flags = 0;
            node->graphicsTask->task.t.ucode_boot = (u64 *)rspbootTextStart;
            node->graphicsTask->task.t.ucode_boot_size = (s32)aspMainTextStart - (s32)rspbootTextStart;
            node->graphicsTask->task.t.ucode = microcodeGroups[temp].ucode;
            node->graphicsTask->task.t.ucode_data = microcodeGroups[temp].ucode_data;
            node->graphicsTask->task.t.ucode_data_size = 0x800;
            node->graphicsTask->task.t.dram_stack = (u64 *)gDramStack;
            node->graphicsTask->task.t.dram_stack_size = 0x400;
            node->graphicsTask->task.t.output_buff = (u64 *)gOutputBuffer;
            node->graphicsTask->task.t.output_buff_size = (u64 *)((s32)gOutputBuffer + BUFFER_SIZE);
            node->graphicsTask->task.t.yield_data_ptr = (u64 *)gYieldBuffer;
            node->graphicsTask->task.t.yield_data_size = 0xC00;
            node = node->renderNext;
        } while (node != NULL);
    }

    node = rootNode;
    needsDisplayListSetup = TRUE;
    if (node != NULL) {
        for (node = rootNode; node != NULL; node = node->renderNext) {
            gActiveViewport = node;

            if (!isRegionAllocSpaceLow() && node->clipLeft < node->clipRight && node->clipTop < node->clipBottom) {

                if (needsDisplayListSetup) {
                    displayListStart = gDisplayListAllocPtr;
                    if (gNeedsDisplayListInit != 0) {
                        gNeedsDisplayListInit = 0;
                        gSPDisplayList(gDisplayListAllocPtr++, gInitDisplayList);
                    }

                    gSPDisplayList(gDisplayListAllocPtr++, gDefaultRenderDisplayList);
                } else {
                    gDPPipeSync(gDisplayListAllocPtr++);
                }

                gDPSetScissor(
                    gDisplayListAllocPtr++,
                    G_SC_NON_INTERLACE,
                    node->clipLeft,
                    node->clipTop,
                    node->clipRight,
                    node->clipBottom
                );

                if (node->displayFlags & 0x2) { // VIEWPORT_DISPLAY_CLEAR_SCREEN flag
                    gDPSetCycleType(gDisplayListAllocPtr++, G_CYC_FILL);
                    gDPSetRenderMode(gDisplayListAllocPtr++, G_RM_NOOP, G_RM_NOOP2);
                    gDPSetColorImage(gDisplayListAllocPtr++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WIDTH, &gFrameBuffer);
                    gDPSetFillColor(gDisplayListAllocPtr++, 0xFFFCFFFC);
                    gDPFillRectangle(
                        gDisplayListAllocPtr++,
                        node->clipLeft,
                        node->clipTop,
                        node->clipRight,
                        node->clipBottom
                    );
                    needsDisplayListSetup = TRUE;
                }

                if (node->displayFlags & 0x1) { // VIEWPORT_DISPLAY_OVERLAY flag
                    gDPPipeSync(gDisplayListAllocPtr++);
                    gDPSetColorImage(
                        gDisplayListAllocPtr++,
                        G_IM_FMT_RGBA,
                        G_IM_SIZ_16b,
                        320,
                        (void *)((u8 *)gAuxFrameBuffers +
                                 ((gCurrentDisplayBufferIndex * 5 * 16 - gCurrentDisplayBufferIndex * 5) << 11))
                    );
                    gDPSetCycleType(gDisplayListAllocPtr++, G_CYC_1CYCLE);
                    gDPSetEnvColor(gDisplayListAllocPtr++, node->overlayR, node->overlayG, node->overlayB, 0xFF);
                    gDPSetCombineLERP(
                        gDisplayListAllocPtr++,
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
                    gDPSetRenderMode(gDisplayListAllocPtr++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
                    gDPFillRectangle(
                        gDisplayListAllocPtr++,
                        node->clipLeft,
                        node->clipTop,
                        node->clipRight + 1,
                        node->clipBottom + 1
                    );
                    gDPPipeSync(gDisplayListAllocPtr++);
                    gDPSetDepthImage(gDisplayListAllocPtr++, &gFrameBuffer);
                    needsDisplayListSetup = FALSE;
                }

                if (needsDisplayListSetup) {
                    needsDisplayListSetup = FALSE;
                    gDPPipeSync(gDisplayListAllocPtr++);
                    gDPSetColorImage(
                        gDisplayListAllocPtr++,
                        G_IM_FMT_RGBA,
                        G_IM_SIZ_16b,
                        320,
                        (void *)((u8 *)gAuxFrameBuffers +
                                 ((gCurrentDisplayBufferIndex * 5 * 16 - gCurrentDisplayBufferIndex * 5) << 11))
                    );
                    gDPSetDepthImage(gDisplayListAllocPtr++, &gFrameBuffer);
                }

                gTextClipAndOffsetData.clipLeft = node->clipLeft;
                gTextClipAndOffsetData.clipTop = node->clipTop;
                gTextClipAndOffsetData.clipRight = node->clipRight;
                gTextClipAndOffsetData.clipBottom = node->clipBottom;
                gTextClipAndOffsetData.offsetX = node->offsetX;
                gTextClipAndOffsetData.offsetY = node->offsetY;

                gTextureEnabled = node->uses3DRendering;
                gGraphicsMode = GRAPHICS_MODE_INVALID;

                if (node->uses3DRendering == 0) {
                    gDPSetColorDither(gDisplayListAllocPtr++, G_CD_DISABLE);

                    for (callbackEntry = node->callbackLayers; callbackEntry != NULL;
                         callbackEntry = callbackEntry->next) {
                        if (callbackEntry->callback == NULL) {
                            continue;
                        }

                        if (isRegionAllocSpaceLow()) {
                            break;
                        }

                        gCurrentPoolIndex = callbackEntry->callbackLayer;
                        ((void (*)(void *))callbackEntry->callback)(callbackEntry->callbackData);
                        gCallbackCounter++;
                    }

                    if (node->prevFadeValue != 0) {
                        gSPDisplayList(gDisplayListAllocPtr++, gFadeOverlayDisplayList);
                        gDPSetPrimColor(
                            gDisplayListAllocPtr++,
                            0,
                            0,
                            node->envR,
                            node->envG,
                            node->envB,
                            node->prevFadeValue
                        );
                        gSPTextureRectangle(
                            gDisplayListAllocPtr++,
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
                        gDPPipeSync(gDisplayListAllocPtr++);
                    }
                } else {
                    // Allocate Vp (16 bytes), projection matrix (64 bytes), and LookAt matrices (192 bytes)
                    viewportAlloc = arenaAlloc16(sizeof(node->viewport));
                    projectionAlloc = arenaAlloc16(sizeof(node->projectionMatrix));
                    lookAtAlloc = arenaAlloc16(48 * sizeof(s32));

                    if (node->numLights > 0) {
                        // Allocate light array: numLights + 1 (the +1 is for ambient light)
                        lightArray = arenaAlloc16((node->numLights + 1) * sizeof(Light));
                        if (lightArray != NULL) {
                            // Keep the Light-sized index separate so IDO adds the typed lights member offset last.
                            // Copy each directional light to the frame arena.
                            for (i = 0; i < node->numLights; i++) {
                                memcpy(
                                    (Light *)(i * sizeof(Light) + (u32)lightArray),
                                    &((ViewportNode *)(i * sizeof(Light) + (u32)node))->lights[0],
                                    sizeof(Light)
                                );
                                gSPLight(gDisplayListAllocPtr++, (Light *)(i * sizeof(Light) + (u32)lightArray), i + 1);
                            }

                            // Copy ambient light (at index numLights)
                            memcpy(
                                (Light *)(i * sizeof(Light) + (u32)lightArray),
                                &((ViewportNode *)(i * sizeof(Light) + (u32)node))->lights[0],
                                sizeof(Light)
                            );
                            gSPLight(gDisplayListAllocPtr++, (Light *)(i * sizeof(Light) + (u32)lightArray), i + 1);

                            gSPNumLights(gDisplayListAllocPtr++, node->numLights);
                        } else {
                            goto bail;
                        }
                    }

                    if (lookAtAlloc != NULL) {
                        memcpy(viewportAlloc, &node->viewport, sizeof(Vp));
                        memcpy(projectionAlloc, &node->projectionMatrix, sizeof(node->projectionMatrix));

                        lookAtAlloc[0] = ((node->viewTransform.m[0][0] << 3) & 0xFFFF0000) +
                                         (u16)(((u16)node->viewTransform.m[1][0] << 16) >> 29);
                        lookAtAlloc[1] = (node->viewTransform.m[2][0] << 3) & 0xFFFF0000;
                        lookAtAlloc[2] = ((node->viewTransform.m[0][1] << 3) & 0xFFFF0000) +
                                         (u16)(((u16)node->viewTransform.m[1][1] << 16) >> 29);
                        lookAtAlloc[3] = (node->viewTransform.m[2][1] << 3) & 0xFFFF0000;
                        lookAtAlloc[4] = ((node->viewTransform.m[0][2] << 3) & 0xFFFF0000) +
                                         (u16)(((u16)node->viewTransform.m[1][2] << 16) >> 29);
                        lookAtAlloc[5] = (node->viewTransform.m[2][2] << 3) & 0xFFFF0000;
                        lookAtAlloc[6] = 0;
                        lookAtAlloc[7] = 1;
                        lookAtAlloc[8] = ((node->viewTransform.m[0][0] << 19) & 0xFFFF0000) +
                                         ((node->viewTransform.m[1][0] << 3) & 0xFFFF);
                        lookAtAlloc[9] = (node->viewTransform.m[2][0] << 19) & 0xFFFF0000;
                        lookAtAlloc[10] = ((node->viewTransform.m[0][1] << 19) & 0xFFFF0000) +
                                          ((node->viewTransform.m[1][1] << 3) & 0xFFFF);
                        lookAtAlloc[11] = (node->viewTransform.m[2][1] << 19) & 0xFFFF0000;
                        lookAtAlloc[12] = ((node->viewTransform.m[0][2] << 19) & 0xFFFF0000) +
                                          ((node->viewTransform.m[1][2] << 3) & 0xFFFF);
                        lookAtAlloc[13] = (node->viewTransform.m[2][2] << 19) & 0xFFFF0000;
                        lookAtAlloc[14] = 0;
                        lookAtAlloc[15] = 0;

                        lookAtAlloc[16] = BUFFER_SIZE;
                        lookAtAlloc[17] = 0;
                        lookAtAlloc[18] = 1;
                        lookAtAlloc[19] = 0;
                        lookAtAlloc[20] = 0;
                        lookAtAlloc[21] = BUFFER_SIZE;
                        lookAtAlloc[22] = ((-node->viewTransform.translation.x) & 0xFFFF0000) +
                                          (u16)(((u32)(-node->viewTransform.translation.y)) >> 16);
                        lookAtAlloc[23] = ((-node->viewTransform.translation.z) & 0xFFFF0000) + 1;
                        lookAtAlloc[24] = 0;
                        lookAtAlloc[25] = 0;
                        lookAtAlloc[26] = 0;
                        lookAtAlloc[27] = 0;
                        lookAtAlloc[28] = 0;
                        lookAtAlloc[29] = 0;
                        lookAtAlloc[30] =
                            -(node->viewTransform.translation.x << 16) + (u16)(-node->viewTransform.translation.y);
                        lookAtAlloc[31] = (-node->viewTransform.translation.z) << 16;

                        lookAtAlloc[32] = ((node->viewTransform.m[0][0] << 3) & 0xFFFF0000) +
                                          (u16)(((u16)node->viewTransform.m[0][1] << 16) >> 29);
                        lookAtAlloc[33] = (node->viewTransform.m[0][2] << 3) & 0xFFFF0000;
                        lookAtAlloc[34] = ((node->viewTransform.m[1][0] << 3) & 0xFFFF0000) +
                                          (u16)(((u16)node->viewTransform.m[1][1] << 16) >> 29);
                        lookAtAlloc[35] = (node->viewTransform.m[1][2] << 3) & 0xFFFF0000;
                        lookAtAlloc[36] = ((node->viewTransform.m[2][0] << 3) & 0xFFFF0000) +
                                          (u16)(((u16)node->viewTransform.m[2][1] << 16) >> 29);
                        lookAtAlloc[37] = (node->viewTransform.m[2][2] << 3) & 0xFFFF0000;
                        lookAtAlloc[38] = 0;
                        lookAtAlloc[39] = 1;
                        lookAtAlloc[40] = ((node->viewTransform.m[0][0] << 19) & 0xFFFF0000) +
                                          ((node->viewTransform.m[0][1] << 3) & 0xFFFF);
                        lookAtAlloc[41] = (node->viewTransform.m[0][2] << 19) & 0xFFFF0000;
                        lookAtAlloc[42] = ((node->viewTransform.m[1][0] << 19) & 0xFFFF0000) +
                                          ((node->viewTransform.m[1][1] << 3) & 0xFFFF);
                        lookAtAlloc[43] = (node->viewTransform.m[1][2] << 19) & 0xFFFF0000;
                        lookAtAlloc[44] = ((node->viewTransform.m[2][0] << 19) & 0xFFFF0000) +
                                          ((node->viewTransform.m[2][1] << 3) & 0xFFFF);
                        lookAtAlloc[45] = (node->viewTransform.m[2][2] << 19) & 0xFFFF0000;
                        lookAtAlloc[46] = 0;
                        lookAtAlloc[47] = 0;

                        gLookAtPtr = (void *)&lookAtAlloc[32];

                        gSPViewport(gDisplayListAllocPtr++, viewportAlloc);
                        gSPPerspNormalize(gDisplayListAllocPtr++, node->perspNorm);
                        gSPMatrix(
                            gDisplayListAllocPtr++,
                            projectionAlloc,
                            G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION
                        );
                        gSPMatrix(gDisplayListAllocPtr++, lookAtAlloc, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
                        gSPMatrix(
                            gDisplayListAllocPtr++,
                            &lookAtAlloc[16],
                            G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION
                        );

                        // gSPFogPosition uses normalized depth positions, not world units.
                        // 0 maps to the near plane and 1000 maps to the current projection far plane.
                        // The world-space fog distance therefore depends on the viewport farPlane.
                        gSPFogPosition(gDisplayListAllocPtr++, node->fogStartPermille, node->fogEndPermille);
                        gDPSetFogColor(gDisplayListAllocPtr++, node->fogR, node->fogG, node->fogB, node->fogA);
                    } else {
                        goto bail;
                    }

                    for (callbackEntry = node->callbackLayers; callbackEntry != NULL;
                         callbackEntry = callbackEntry->next) {
                        if (callbackEntry->callback == NULL) {
                            continue;
                        }

                        if (isRegionAllocSpaceLow() != 0) {
                            break;
                        }

                        gCurrentPoolIndex = callbackEntry->callbackLayer;
                        ((void (*)(void *))callbackEntry->callback)(callbackEntry->callbackData);
                        gCallbackCounter++;
                    }

                bail:
                    if (node->prevFadeValue != 0) {
                        gSPDisplayList(gDisplayListAllocPtr++, gFadeOverlayDisplayList);
                        gDPSetPrimColor(
                            gDisplayListAllocPtr++,
                            0,
                            0,
                            node->envR,
                            node->envG,
                            node->envB,
                            node->prevFadeValue
                        );
                        gSPTextureRectangle(
                            gDisplayListAllocPtr++,
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
                        gDPPipeSync(gDisplayListAllocPtr++);
                        gDPSetColorDither(gDisplayListAllocPtr++, G_CD_MAGICSQ);
                    }
                }
            }

            if (node->graphicsTask != NULL) {
                if (!needsDisplayListSetup) {
                    // If this is the last viewport and root has a fade value, render root's fade overlay
                    if (gRootViewport.prevFadeValue != 0 && node->renderNext == 0) {
                        gSPDisplayList(gDisplayListAllocPtr++, gFadeOverlayDisplayList);

                        gDPSetScissor(
                            gDisplayListAllocPtr++,
                            G_SC_NON_INTERLACE,
                            gRootViewport.clipLeft,
                            gRootViewport.clipTop,
                            gRootViewport.clipRight,
                            gRootViewport.clipBottom
                        );

                        gDPSetPrimColor(
                            gDisplayListAllocPtr++,
                            0,
                            0,
                            gRootViewport.envR,
                            gRootViewport.envG,
                            gRootViewport.envB,
                            gRootViewport.prevFadeValue
                        );

                        gSPTextureRectangle(
                            gDisplayListAllocPtr++,
                            gRootViewport.clipLeft << 2,
                            gRootViewport.clipTop << 2,
                            gRootViewport.clipRight << 2,
                            gRootViewport.clipBottom << 2,
                            0,
                            0,
                            0,
                            0x400,
                            0x400
                        );
                    }

                    gSPEndDisplayList(gDisplayListAllocPtr++);
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

    gDisplayListAllocPtr = (Gfx *)temp_v0;
    gRegionAllocEnd = temp_v0 + BUFFER_SIZE;
}

s32 isRegionAllocSpaceLow(void) {
    return (u32)(gRegionAllocEnd - (u32)gDisplayListAllocPtr) < 0x1AE1U;
}

void restoreViewportOffsets(void) {
    gRootViewport.originX = gViewportOriginX;
    gRootViewport.originY = gViewportOriginY;
}

void initGraphicsSystem(void) {
    s32 i;
    s32 *ptr;

    gRootViewport.parent = NULL;
    gRootViewport.callbackSlotIndex = 0xFFFF;
    gRootViewport.viewportLeft = -0xA0;
    gRootViewport.viewportTop = -0x78;
    gRootViewport.viewportRight = 0xA0;
    gRootViewport.hierarchyPrev = NULL;
    gRootViewport.nextSibling = NULL;
    gRootViewport.renderPrev = NULL;
    gRootViewport.renderNext = NULL;
    gRootViewport.renderOrder = 0;
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
    ptr = &D_800A35C8_A0058[0];
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
            childNode = node->parent;
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
            node->viewport.vp.vtrans[0] = node->offsetX * 4;
            node->viewport.vp.vtrans[1] = node->offsetY * 4;
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
            node = node->nextSibling;
        } while (node != NULL);
    }
}

void setModelCameraTransform(
    ViewportNode *node,
    s16 originX,
    s16 originY,
    s16 viewportLeft,
    s16 viewportTop,
    s16 viewportRight,
    s16 viewportBottom
) {
    node->originX = originX;
    node->originY = originY;
    node->viewportLeft = viewportLeft;
    node->viewportTop = viewportTop;
    node->viewportRight = viewportRight;
    node->viewportBottom = viewportBottom;
}

void setViewportScale(ViewportNode *arg0, f32 scaleX, f32 scaleY) {
    arg0->scaleY = scaleY;
    arg0->viewport.vp.vscale[0] = (s16)(scaleX * 640.0f);
    arg0->viewport.vp.vscale[1] = (s16)(scaleY * 480.0f);
}

void setViewportPerspective(ViewportNode *node, f32 fov, f32 aspect, f32 near, f32 far) {
    guPerspective(&node->projectionMatrix, &node->perspNorm, fov, aspect, near, far, 1.0f);
}

void initViewportCallbackPool(ViewportNode *node) {
    s32 i;

    i = 7;
    while (i >= 0) {
        node->callbackLayers[i].callback = NULL;
        i--;
    }

    i = 1;
    while (i < 8) {
        node->callbackLayers[i - 1].next = &node->callbackLayers[i];
        i++;
    }

    node->callbackLayers[VIEWPORT_CALLBACK_LAYER_COUNT - 1].next = NULL;
}

void initViewportNode(
    ViewportNode *node,
    ViewportNode *parent,
    s32 callbackSlot,
    s32 renderOrder,
    s32 uses3DRendering
) {
    ViewportNode *temp_v0;
    ViewportNode *var_a0;
    u8 uses3DRenderingByte = (u8)uses3DRendering;

    gViewportCallbackPools[callbackSlot & 0xFFFF] = node;

    if (parent == NULL) {
        node->parent = &gRootViewport;
        node->hierarchyPrev = &gRootViewport;
        temp_v0 = gRootViewport.nextSibling;
        node->nextSibling = temp_v0;
        if (temp_v0 != NULL) {
            temp_v0->hierarchyPrev = node;
        }
        gRootViewport.nextSibling = node;
    } else {
        node->parent = parent;
        node->hierarchyPrev = parent;
        temp_v0 = parent->nextSibling;
        node->nextSibling = temp_v0;
        if (temp_v0 != NULL) {
            temp_v0->hierarchyPrev = node;
        }
        parent->nextSibling = node;
    }

    var_a0 = &gRootViewport;
    if (gRootViewport.renderNext != NULL) {
        do {
            ViewportNode *temp_v1 = var_a0->renderNext;
            if ((u8)renderOrder < (u8)temp_v1->renderOrder) {
                break;
            }
            var_a0 = temp_v1;
        } while (var_a0->renderNext != NULL);
    }

    node->renderPrev = var_a0;
    node->renderNext = var_a0->renderNext;
    var_a0->renderNext = node;
    temp_v0 = node->renderNext;
    if (temp_v0 != NULL) {
        temp_v0->renderPrev = node;
    }

    node->renderOrder = (s8)renderOrder;
    node->callbackSlotIndex = (u16)callbackSlot;
    node->uses3DRendering = (s8)uses3DRenderingByte;
    node->displayFlags = 0;
    node->viewportId = 0;
    node->numLights = 0;
    node->viewport.vp.vscale[0] = 0x280;
    node->viewport.vp.vscale[1] = 0x1E0;
    node->viewport.vp.vscale[2] = 0x1FF;
    node->viewport.vp.vscale[3] = 0;
    node->viewport.vp.vtrans[0] = 0x280;
    node->viewport.vp.vtrans[1] = 0x1E0;
    node->viewport.vp.vtrans[2] = 0x1FF;
    node->viewport.vp.vtrans[3] = 0;
    memcpy(&node->viewTransform, &identityMatrix, sizeof(Transform3D));
    guPerspective(&node->projectionMatrix, &node->perspNorm, 30.0f, 1.3333334f, 20.0f, 2000.0f, 1.0f);
    node->fogA = 0xFF;
    node->fogStartPermille = 0x3DE;
    node->fogB = 0;
    node->fogG = 0;
    node->fogR = 0;
    node->fogEndPermille = 0x3E6;
    node->envR = 0;
    node->envG = 0;
    node->envB = 0;
    node->prevFadeValue = 0;
    node->fadeMode = 0;
    node->scaleY = 1.0f;
    initViewportCallbackPool(node);
}

void nullViewportFunction(void) {
}

void setViewportLightColors(u16 viewportId, u16 lightCount, DirectionalLightData *lightData, RgbColor *ambientColor) {
    ViewportNode *viewport;
    s32 i;

    viewport = gRootViewport.nextSibling;

    if (viewport == NULL) {
        return;
    }

    while (viewport != NULL) {
        if (viewport->viewportId == viewportId) {
            for (i = 0; i < lightCount; i++) {
                viewport->lights[i].l.col[0] = viewport->lights[i].l.colc[0] = lightData[i].r;
                viewport->lights[i].l.col[1] = viewport->lights[i].l.colc[1] = lightData[i].g;
                viewport->lights[i].l.col[2] = viewport->lights[i].l.colc[2] = lightData[i].b;
                viewport->lights[i].l.dir[0] = lightData[i].directionX;
                viewport->lights[i].l.dir[1] = lightData[i].directionY;
                viewport->lights[i].l.dir[2] = lightData[i].directionZ;
            }

            viewport->lights[i].l.col[0] = viewport->lights[i].l.colc[0] = ambientColor->r;
            viewport->lights[i].l.col[1] = viewport->lights[i].l.colc[1] = ambientColor->g;
            viewport->lights[i].l.col[2] = viewport->lights[i].l.colc[2] = ambientColor->b;

            viewport->numLights = lightCount;
        }

        viewport = viewport->nextSibling;
    }
}

void setViewportTransformById(u16 viewportId, void *transformMatrix) {
    ViewportNode *node;

    node = gRootViewport.nextSibling;

    while (node != NULL) {
        if (node->viewportId == viewportId) {
            memcpy(&node->viewTransform, transformMatrix, sizeof(Transform3D));
        }
        node = node->nextSibling;
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
        if (node->callbackSlotIndex == slotIndex) {
            node->fadeValue = fadeValue;
            node->fadeMode = fadeMode;
            if (!(fadeMode & 0xFF)) {
                node->prevFadeValue = fadeValue;
            }
        }
        node = node->renderNext;
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

void setViewportFogById(u16 viewportId, s16 fogStartPermille, s16 fogEndPermille, u8 fogR, u8 fogG, u8 fogB) {
    ViewportNode *node;

    node = gRootViewport.nextSibling;

    while (node != NULL) {
        if (node->viewportId == viewportId) {
            node->fogStartPermille = fogStartPermille;
            node->fogEndPermille = fogEndPermille;
            node->fogR = fogR;
            node->fogG = fogG;
            node->fogB = fogB;
        }
        node = node->nextSibling;
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
    node->viewportId = id;
}

void unlinkNode(ViewportNode *node) {
    ViewportNode *current;
    ViewportNode *next;

    current = &gRootViewport;
    gViewportCallbackPools[node->callbackSlotIndex] = NULL;

    next = gRootViewport.nextSibling;
    while (next != 0) {
        if (current->parent == node) {
            current->parent = node->parent;
        }

        current = current->nextSibling;
        next = current->nextSibling;
    }

    if (node->nextSibling != 0) {
        node->nextSibling->hierarchyPrev = node->hierarchyPrev;
    }

    node->hierarchyPrev->nextSibling = node->nextSibling;
    if (node->renderNext != 0) {
        node->renderNext->renderPrev = node->renderPrev;
    }

    node->renderPrev->renderNext = node->renderNext;
}

void pushViewportCallbackBySlot(u16 viewportSlot, u8 callbackLayer, void *callback, void *callbackData) {
    ViewportNode *viewport;
    CallbackEntry *entry;

    viewport = gViewportCallbackPools[viewportSlot];
    if (viewport != NULL) {
        entry = (CallbackEntry *)linearAlloc(sizeof(CallbackEntry));
        if (entry != NULL) {
            entry->next = viewport->callbackLayers[callbackLayer].next;
            entry->callback = callback;
            entry->callbackData = callbackData;
            entry->callbackLayer = callbackLayer;
            viewport->callbackLayers[callbackLayer].next = entry;
        }
    }
}

void pushViewportCallback(ViewportNode *viewport, u8 callbackLayer, void *callback, void *callbackData) {
    CallbackEntry *newEntry;
    CallbackEntry *oldHead;

    newEntry = (CallbackEntry *)linearAlloc(0x10);
    if (newEntry != NULL) {
        oldHead = viewport->callbackLayers[callbackLayer].next;
        newEntry->callback = callback;
        newEntry->callbackData = callbackData;
        newEntry->callbackLayer = callbackLayer;
        newEntry->next = oldHead;
        viewport->callbackLayers[callbackLayer].next = newEntry;
    }
}

void pushViewportCallbackById(u16 viewportId, u8 callbackLayer, void *callback, void *callbackData) {
    ViewportNode *viewport;
    CallbackEntry *newEntry;
    CallbackEntry *oldHead;

    viewport = &gRootViewport;

    while (viewport != NULL) {
        if (viewport->viewportId == viewportId) {
            newEntry = (CallbackEntry *)linearAlloc(sizeof(CallbackEntry));
            if (newEntry != NULL) {
                oldHead = viewport->callbackLayers[callbackLayer].next;
                newEntry->callback = callback;
                newEntry->callbackData = callbackData;
                newEntry->callbackLayer = callbackLayer;
                newEntry->next = oldHead;
                viewport->callbackLayers[callbackLayer].next = newEntry;
            }
        }
        viewport = viewport->renderNext;
    }
}

s32 isObjectCulled(Vec3i *arg0) {
    if ((u32)(gActiveViewport->viewTransform.translation.x - arg0->x + RACE_CULL_BOX_HALF_EXTENT_FIXED) >
        RACE_CULL_BOX_FULL_EXTENT_FIXED) {
        return TRUE;
    }

    if ((u32)(gActiveViewport->viewTransform.translation.y - arg0->y + RACE_CULL_BOX_HALF_EXTENT_FIXED) >
        RACE_CULL_BOX_FULL_EXTENT_FIXED) {
        return TRUE;
    }

    // compiler nonsense
    if (((!arg0) && (!arg0)) && (!arg0)) {}

    return (u32)(gActiveViewport->viewTransform.translation.z - arg0->z + RACE_CULL_BOX_HALF_EXTENT_FIXED) >
           RACE_CULL_BOX_FULL_EXTENT_FIXED;
}
