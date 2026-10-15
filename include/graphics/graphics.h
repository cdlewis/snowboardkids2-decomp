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

#define VIEWPORT_CALLBACK_LAYER_COUNT 8

/* Cached rendering setup; INVALID forces the next draw to restore its state. */
typedef enum {
    GRAPHICS_MODE_INVALID = -1,
    GRAPHICS_MODE_COLOR_RECT = 0,
    GRAPHICS_MODE_FONT = 1,
    GRAPHICS_MODE_DISPLAY_LIST = 3,
    GRAPHICS_MODE_TEXTURED_BILLBOARD = 6,
    GRAPHICS_MODE_ALPHA_BILLBOARD = 7,
    GRAPHICS_MODE_SPRITE_RDP = 0x100,
    GRAPHICS_MODE_NONRACE_SHADOW = 0x200,
    GRAPHICS_MODE_OPAQUE_SPRITE = 0x202,
    GRAPHICS_MODE_TRANSLUCENT_SPRITE = 0x203,
} GraphicsMode;

/*
 * Layers execute in ascending order, and entries within one layer execute
 * LIFO. These names describe conventional use; callbacks configure their own
 * rendering state, so the phases are not type-enforced.
 */
typedef enum {
    VIEWPORT_CALLBACK_LAYER_INITIAL = 0,
    VIEWPORT_CALLBACK_LAYER_OPAQUE = 1,
    VIEWPORT_CALLBACK_LAYER_POST_OPAQUE = 2,
    VIEWPORT_CALLBACK_LAYER_TRANSLUCENT = 3,
    VIEWPORT_CALLBACK_LAYER_SPRITES = 4,
    VIEWPORT_CALLBACK_LAYER_OVERLAY = 5,
    VIEWPORT_CALLBACK_LAYER_ALPHA_OVERLAY = 6,
    VIEWPORT_CALLBACK_LAYER_FINAL = 7,
} ViewportCallbackLayer;

// gCallbackEntrySegment overlaps with the lower 2 bytes of gCurrentDoubleBufferIndex
#define gCallbackEntrySegment (*(u16 *)((u8 *)&gCurrentDoubleBufferIndex + 2))

typedef struct {
    u8 r;
    u8 g;
    u8 b;
    u8 padding3;
} RgbColor;

typedef struct {
    u8 r;
    u8 g;
    u8 b;
    u8 padding3;
    s8 directionX;
    s8 directionY;
    s8 directionZ;
    u8 padding7;
} DirectionalLightData;

typedef struct {
    RgbColor ambient;
    RgbColor fog;
} EnvironmentColorData;

typedef struct {
    RgbColor color;
    u8 padding4[4];
} AmbientLightData;

/* Render callback pool entry — linked list of draw callbacks */
typedef struct CallbackEntry {
    struct CallbackEntry *next;
    void *callback;
    void *callbackData;
    u8 paddingC[3];
    u8 callbackLayer;
} CallbackEntry;

typedef struct {
    /* 0x00 */ OSTask task;
    /* 0x40 */ OSMesgQueue *messageQueue;
    /* 0x44 */ OSMesg completionMessage;
    /* 0x48 */ void *framebuffer;
    /* 0x4C */ u16 frameIndex;
    /* 0x4E */ u16 flags;
} GraphicsTask;

typedef struct {
    /* 0x000 */ GraphicsTask graphicsTask;
    /* 0x050 */ Gfx displayList[32];
} DisplayBufferTask;

typedef struct {
    u64 *ucode;
    u64 *ucode_data;
} UcodeEntry;

typedef struct {
    /* 0x00 */ s16 clipLeft;
    /* 0x02 */ s16 clipTop;
    /* 0x04 */ s16 clipRight;
    /* 0x06 */ s16 clipBottom;
    /* 0x08 */ s16 offsetX;
    /* 0x0A */ s16 offsetY;
} TextClipAndOffsetData;

typedef struct ViewportNode {
    /* 0x00 */ struct ViewportNode *parent;
    /* 0x04 */ struct ViewportNode *hierarchyPrev;
    /* 0x08 */ struct ViewportNode *nextSibling;
    /* 0x0C */ struct ViewportNode *renderPrev;
    /* 0x10 */ struct ViewportNode *renderNext;
    /* 0x14 */ s8 renderOrder;
    /* 0x15 */ u8 uses3DRendering;
    /* 0x16 */ u16 callbackSlotIndex;
    /* 0x18 */ CallbackEntry callbackLayers[VIEWPORT_CALLBACK_LAYER_COUNT];
    /* 0x98 */ void *displayListPtr;
    /* 0x9C */ GraphicsTask *graphicsTask;
    /* 0xA0 */ s16 originX;
    /* 0xA2 */ s16 originY;
    /* 0xA4 */ s16 viewportLeft;   // Center-relative extent
    /* 0xA6 */ s16 viewportTop;    // Center-relative extent
    /* 0xA8 */ s16 viewportRight;  // Logical boundary before clamping
    /* 0xAA */ s16 viewportBottom; // Logical boundary before clamping
    /* 0xAC */ s16 offsetX;
    /* 0xAE */ s16 offsetY;
    /* 0xB0 */ s16 clipLeft;
    /* 0xB2 */ s16 clipTop;
    /* 0xB4 */ s16 clipRight;  // Resolved inclusive CPU clip bound
    /* 0xB6 */ s16 clipBottom; // Resolved inclusive CPU clip bound
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
    /* 0xC8 */ Vp viewport;
    /* 0xD8 */ u16 perspNorm;
    /* 0xDA */ u16 viewportId;
    /* 0xE0 */ Mtx projectionMatrix;
    /* 0x120 */ Transform3D viewTransform;
    /* 0x140 */ u16 numLights;
    /* 0x148 */ Light lights[8];
    /* 0x1C8 */ s16 fogStartPermille;
    /* 0x1CA */ s16 fogEndPermille;
    /* 0x1CC */ u8 fogR;
    /* 0x1CD */ u8 fogG;
    /* 0x1CE */ u8 fogB;
    /* 0x1CF */ u8 fogA;
    /* 0x1D0 */ f32 scaleY;
} ViewportNode;

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
extern ViewportNode *gActiveViewport;
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

/* Pushes onto a layer head; callbacks within a layer therefore execute LIFO. */
void pushViewportCallbackBySlot(u16 viewportSlot, u8 callbackLayer, void *callback, void *callbackData);

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

void setViewportId(ViewportNode *node, u16 viewportId);

void setViewportTransformById(u16 viewportId, void *transformMatrix);

void initViewportNode(ViewportNode *node, ViewportNode *parent, s32 callbackSlot, s32 renderOrder, s32 uses3DRendering);

void setViewportPerspective(ViewportNode *node, f32 fov, f32 aspect, f32 near, f32 far);

/*
 * Edges are center-relative coordinates. updateViewportBounds() clamps them
 * to inclusive clip bounds, so 320 becomes 319 at the screen edge while an
 * interior logical boundary such as 160 remains 160.
 */
void setModelCameraTransform(
    ViewportNode *node,
    s16 originX,
    s16 originY,
    s16 viewportLeft,
    s16 viewportTop,
    s16 viewportRight,
    s16 viewportBottom
);

void unlinkNode(ViewportNode *player);

void unlinkViewportNode(ViewportNode *arg0);

s32 getViewportFadeMode(ViewportNode *);

s32 isObjectCulled(Vec3i *arg0);

void disableViewportOverlay(ViewportNode *arg0);

void enableViewportDisplayList(void *arg0);

void disableViewportDisplayList(ViewportNode *arg0);

void setViewportOverlayRgbAndEnable(ViewportNode *arg0, s8 r, s8 g, s8 b);

void setViewportLightColors(u16 viewportId, u16 lightCount, DirectionalLightData *lightData, RgbColor *ambientColor);
