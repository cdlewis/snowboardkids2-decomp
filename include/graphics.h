#pragma once

#include "36BE0.h"
#include "common.h"
#include "displaylist.h"
#include "geometry.h"

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

typedef struct PoolEntry {
    struct PoolEntry *next;
    void *callback;
    void *callbackData;
    u8 _padC[3];
    u8 poolIndex;
} PoolEntry;

/* Render callback pool entry — linked list of draw callbacks */
typedef struct CallbackEntry {
    struct CallbackEntry *next;
    void *callback;
    void *callbackData;
    u8 _padC[3];
    u8 poolIndex;
} CallbackEntry;

/* RSP task message sent to the scheduler for each viewport group */
typedef struct {
    OSTask t;
    OSMesgQueue *msgQueue;
    s32 msgData;
    void *auxBuffer;
    u16 scanlineValue;
    u16 taskFlags;
} FrameCallbackMsg;

typedef struct ViewportNode {
    /* 0x00 */ union {
        struct ViewportNode *next;
        u16 callback_selector;
    } unk0;
    /* 0x04 */ struct ViewportNode *prev;
    /* 0x08 */ union {
        struct ViewportNode *list2_next;
        u16 callback_selector;
    } unk8;
    /* 0x0C */ struct ViewportNode *list2_prev;
    /* 0x10 */ struct ViewportNode *list3_next;
    /* 0x14 */ s8 unk14;
    /* 0x15 */ u8 priority;
    /* 0x16 */ u16 slot_index;
    /* 0x18 */ PoolEntry pool[7];
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
    /* 0xDA */ u16 id;
    /* 0xDC */ s16 unkDC;
    /* 0xDE */ s16 unkDE;
    /* 0xE0 */ Mtx perspectiveMatrix;
    Transform3D modelingMatrix;
    u16 numLights;
    u8 padding140[6];
    ViewportNode_ColorData unk148[1];
    u8 padding158[0x70];
    s16 fogMin;
    s16 fogMax;
    u8 fogR;
    u8 fogG;
    u8 fogB;
    u8 fogA;
    f32 scaleY;
    u8 padding1D0[0x2];
} ViewportNode;

extern ViewportNode gRootViewport;

void setViewportFadeValue(ViewportNode *node, u8 fadeValue, u8 fadeMode);

void setViewportFadeValueBySlotIndex(u16 slotIndex, u8 fadeValue, u8 fadeMode);

void debugEnqueueCallback(u16 index, u8 arg1, void *arg2, void *arg3);

void *arenaAlloc16(s32 size);

void *advanceLinearAlloc(s32 size);

void tryProcessDisplayFrameUpdate(void);

void processDisplayFrameUpdate(void);

void handleFrameBufferComplete(s32 bufferIndex);

void setViewportEnvColor(ViewportNode *node, u8 r, u8 g, u8 b);

void setViewportFogById(u16 viewportId, s16 fogMin, s16 fogMax, u8 fogR, u8 fogG, u8 fogB);

void setViewportScale(ViewportNode *arg0, f32 scaleX, f32 scaleY);

void renderFrame(u32);

void func_8006DC40_6E840(void);

void func_8006FEF8_70AF8(ViewportNode *node, u16 id);

void setViewportId(ViewportNode *node, u16 id);

void setViewportTransformById(u16 viewportId, void *transformMatrix);

void initViewportNode(ViewportNode *node, ViewportNode *parent, s32 slotIndex, s32 priority, s32 flags);

void func_8006FA0C_7060C(ViewportNode *node, f32 fov, f32 aspect, f32 near, f32 far);

void setModelCameraTransform(void *, s16, s16, s16, s16, s16, s16);

void unlinkNode(ViewportNode *player);

void n_alSeqpDelete(ViewportNode *arg0);

s32 getViewportFadeMode(ViewportNode *);

s32 isObjectCulled(Vec3i *arg0);

void disableViewportOverlay(ViewportNode *arg0);
void enableViewportDisplayList(void *arg0);
void disableViewportDisplayList(ViewportNode *arg0);
void func_8006FEE8_70AE8(ViewportNode *arg0);
void setViewportOverlayRgbAndEnable(ViewportNode *arg0, s8 r, s8 g, s8 b);

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

void setViewportLightColors(u16 viewportId, u16 colorCount, ColorData *lightColors, ColorData *ambientColor);