#pragma once

#include "common.h"
#include "gbi.h"
#include "math/geometry.h"
#include "os_message.h"
#include "sptask.h"

// Object culling is an axis-aligned box around the active viewport
// translation, not a spherical/radial draw-distance check.
// The half-extent offset converts a signed range check into an unsigned comparison:
//   (u32)(camera - object + HALF_EXTENT) > FULL_EXTENT  ⟺  |camera - object| > HALF_EXTENT
#define RACE_CULL_BOX_HALF_EXTENT_FIXED 0x0FEA0000
#define RACE_CULL_BOX_FULL_EXTENT_FIXED 0x1FD40000

#define BUFFER_SIZE 0x10000

// gCallbackEntrySegment overlaps with the lower 2 bytes of gCurrentDoubleBufferIndex
#define gCallbackEntrySegment (*(u16 *)((u8 *)&gCurrentDoubleBufferIndex + 2))

typedef struct {
    u8 light1R;
    u8 light1G;
    u8 light1B;
    u8 pad14B;
    u8 light1R_dup;
    u8 light1G_dup;
    u8 light1B_dup;
    u8 unk14F;
    u8 light2R;
    u8 light2G;
    u8 light2B;
    u8 padding[0x5];
} ViewportNode_ColorData;

/* Render callback pool entry — linked list of draw callbacks */
typedef struct CallbackEntry {
    struct CallbackEntry *next;
    void *callback;
    void *callbackData;
    u8 _padC[3];
    u8 poolIndex;
} CallbackEntry;

typedef struct {
    u8 padding[0x8];
    CallbackEntry *unk8;
    s32 unkC;
} CallbackPoolSlot;

/* RSP task message sent to the scheduler for each viewport group */
typedef struct {
    OSTask t;
    OSMesgQueue *msgQueue;
    s32 msgData;
    void *auxBuffer;
    u16 scanlineValue;
    u16 taskFlags;
} FrameCallbackMsg;

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

typedef struct {
    u64 *ucode;
    u64 *ucode_data;
} UcodeEntry;

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

typedef struct {
    /* 0x00 */ s16 clipLeft;
    /* 0x02 */ s16 clipTop;
    /* 0x04 */ s16 clipRight;
    /* 0x06 */ s16 clipBottom;
    /* 0x08 */ s16 offsetX;
    /* 0x0A */ s16 offsetY;
} TextClipAndOffsetData;

typedef struct ViewportNode {
    /* 0x00 */ union {
        struct ViewportNode *next;
        u16 counter;
    } unk0;
    /* 0x04 */ struct ViewportNode *prev;
    /* 0x08 */ union {
        struct ViewportNode *list2_next;
        u16 counter;
    } unk8;
    /* 0x0C */ struct ViewportNode *list2_prev;
    /* 0x10 */ struct ViewportNode *list3_next;
    /* 0x14 */ s8 renderOrder;
    /* 0x15 */ u8 viewportFlags;
    /* 0x16 */ u16 callbackSlotIndex;
    /* 0x18 */ CallbackEntry pool[7];
    /* 0x88 */ void *unk88;
    /* 0x8C */ u8 padding8C[0xC];
    /* 0x98 */ void *displayListPtr;
    /* 0x9C */ FrameCallbackMsg *frameCallbackMsg;
    /* 0xA0 */ s16 originX;
    /* 0xA2 */ s16 originY;
    /* 0xA4 */ s16 viewportLeft;
    /* 0xA6 */ s16 viewportTop;
    /* 0xA8 */ s16 viewportRight;
    /* 0xAA */ s16 viewportBottom;
    /* 0xAC */ s16 offsetX;
    /* 0xAE */ s16 offsetY;
    /* 0xB0 */ s16 clipLeft;
    /* 0xB2 */ s16 clipTop;
    /* 0xB4 */ s16 clipRight;
    /* 0xB6 */ s16 clipBottom;
    /* 0xB8 */ u8 displayFlags;
    /* 0xB9 */ u8 overlayR;
    /* 0xBA */ u8 overlayG;
    /* 0xBB */ u8 overlayB;
    /* 0xBC */ u8 envR;
    /* 0xBD */ u8 envG;
    /* 0xBE */ u8 envB;
    /* 0xBF */ u8 prevFadeValue;
    /* 0xC0 */ u8 fadeValue;
    /* 0xC1 */ u8 fadeMode;
    /* 0xC2 */ u8 paddingC2[0x6];
    /* 0xC8 */ s16 viewportWidth;
    /* 0xCA */ s16 viewportHeight;
    /* 0xCC */ s16 unkCC;
    /* 0xCE */ s16 unkCE;
    /* 0xD0 */ s16 unkD0;
    /* 0xD2 */ s16 unkD2;
    /* 0xD4 */ s16 unkD4;
    /* 0xD6 */ s16 unkD6;
    /* 0xD8 */ u16 perspNorm;
    /* 0xDA */ u16 viewportId;
    /* 0xDC */ s16 unkDC;
    /* 0xDE */ s16 unkDE;
    /* 0xE0 */ Mtx projectionMatrix;
    Transform3D viewTransform;
    u16 numLights;
    u8 padding140[6];
    ViewportNode_ColorData unk148[1];
    u8 padding158[0x70];
    s16 fogStartPermille;
    s16 fogEndPermille;
    u8 fogR;
    u8 fogG;
    u8 fogB;
    u8 fogA;
    f32 scaleY;
    u8 padding1D0[0x2];
} ViewportNode;

typedef struct {
    u8 padding[0x120];
    s32 cameraRotationMatrix;
    u8 padding2[0x10];
    u32 cameraX;
    u32 cameraY;
    u32 cameraZ;
    u8 padding3[0x8];
    u8 defaultLight1R;
    u8 defaultLight1G;
    u8 defaultLight1B;
    u8 padding4[0xD];
    u8 defaultLight2R;
    u8 defaultLight2G;
    u8 defaultLight2B;
} ActiveViewportState;

typedef struct {
    /* 0x0 */ u8 r;
    /* 0x1 */ u8 g;
    /* 0x2 */ u8 b;
    /* 0x3 */ u8 pad;
    /* 0x4 */ s8 r2;
    /* 0x5 */ s8 g2;
    /* 0x6 */ s8 b2;
    /* 0x7 */ u8 pad2;
} ColorData;

extern ViewportNode gRootViewport;
extern s32 gCurrentDoubleBufferIndex;
extern s32 gCurrentDisplayBufferIndex;
extern s32 gFrameSkipCounter;
extern u32 __additional_scanline_0;
extern u8 gNeedsDisplayListInit;
extern u8 gDisplayFramePending;
extern s32 gFrameBufferFlags[];
extern s32 gFrameBufferCounters[];
extern s32 gFrameCounter;
extern s32 gBufferedFrameCounter;
extern ActiveViewportState *gActiveViewport;
extern s16 gGraphicsMode;
extern s16 gCurrentPoolIndex;
extern s32 gCallbackCounter;
extern TextClipAndOffsetData gTextClipAndOffsetData;
extern OSMesgQueue mainMessageQueue;
extern Gfx gInitDisplayList[];
extern Gfx gDefaultRenderDisplayList[];
extern Gfx gFadeOverlayDisplayList[];

// Location of S2Dex code/rodata segments
extern u64 gspS2DEX_fifoTextStart[];
extern u64 gspS2DEX_fifoDataStart[];

extern UcodeEntry microcodeGroups[];

void selectGraphicsArena(s32 index);

void linearAllocSelectRegion(s32 index);

void initViewportCallbackPool(ViewportNode *node);

s32 isRegionAllocSpaceLow(void);

void resetLinearAllocator(void);

void updateViewportBounds(void);

void setViewportFadeValue(ViewportNode *node, u8 fadeValue, u8 fadeMode);

void setViewportFadeValueBySlotIndex(u16 slotIndex, u8 fadeValue, u8 fadeMode);

void enqueueCallbackBySlotIndex(u16 index, u8 arg1, void *arg2, void *arg3);

void *arenaAlloc16(s32 size);

void *advanceLinearAlloc(s32 size);

void tryProcessDisplayFrameUpdate(void);

void processDisplayFrameUpdate(void);

void handleFrameBufferComplete(s32 bufferIndex);

void setViewportEnvColor(ViewportNode *node, u8 r, u8 g, u8 b);

void setViewportFogById(u16 viewportId, s16 fogStartPermille, s16 fogEndPermille, u8 fogR, u8 fogG, u8 fogB);

void setViewportScale(ViewportNode *arg0, f32 scaleX, f32 scaleY);

void renderFrame(u32);

void initDisplayBuffers(void);

void func_8006FEF8_70AF8(ViewportNode *node, u16 viewportId);

void setViewportId(ViewportNode *node, u16 viewportId);

void setViewportTransformById(u16 viewportId, void *transformMatrix);

void initViewportNode(ViewportNode *node, ViewportNode *parent, s32 slotIndex, s32 renderOrder, s32 viewportFlags);

void setViewportPerspective(ViewportNode *node, f32 fov, f32 aspect, f32 near, f32 far);

void setModelCameraTransform(ViewportNode *, s16, s16, s16, s16, s16, s16);

void unlinkNode(ViewportNode *player);

void unlinkViewportNode(ViewportNode *arg0);

s32 getViewportFadeMode(ViewportNode *);

s32 isObjectCulled(Vec3i *arg0);

void disableViewportOverlay(ViewportNode *arg0);

void enableViewportDisplayList(void *arg0);

void disableViewportDisplayList(ViewportNode *arg0);

void func_8006FEE8_70AE8(ViewportNode *arg0);

void setViewportOverlayRgbAndEnable(ViewportNode *arg0, s8 r, s8 g, s8 b);

void setViewportLightColors(u16 viewportId, u16 colorCount, ColorData *lightColors, ColorData *ambientColor);
