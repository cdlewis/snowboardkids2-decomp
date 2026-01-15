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
} Node_70B00_ColorData;
typedef struct PoolEntry {
    struct PoolEntry *next;
    void *callback;
    void *callbackData;
    u8 _padC[3];
    u8 poolIndex;
} PoolEntry;

typedef struct Node_70B00 {
    /* 0x00 */ union {
        struct Node_70B00 *next;
        u16 callback_selector;
    } unk0;
    /* 0x04 */ struct Node_70B00 *prev;
    /* 0x08 */ union {
        struct Node_70B00 *list2_next;
        u16 callback_selector;
    } unk8;
    /* 0x0C */ struct Node_70B00 *list2_prev;
    /* 0x10 */ struct Node_70B00 *list3_next;
    s8 unk14;
    s8 unk15;
    /* 0x16 */ u16 slot_index;
    /* 0x18 */ PoolEntry pool[7];
    /* 0x88 */ void *unk88;
    u8 padding2[0x10];
    /* 0x9C */ void *frameCallbackMsg;
    /* 0xA0 */ s16 unkA0;
    /* 0xA2 */ s16 unkA2;
    /* 0xA4 */ s16 unkA4;
    /* 0xA6 */ s16 unkA6;
    /* 0xA8 */ s16 unkA8;
    /* 0xAA */ s16 unkAA;
    /* 0xAC */ s16 unkAC;
    /* 0xAE */ s16 unkAE;
    /* 0xB0 */ s16 unkB0;
    /* 0xB2 */ s16 unkB2;
    /* 0xB4 */ s16 unkB4;
    /* 0xB6 */ s16 unkB6;
    u8 padding2b[0x4];
    /* 0xBC */ u8 envR;
    u8 envG;
    u8 envB;
    u8 prevFadeValue;
    u8 fadeValue;
    u8 fadeMode;
    u8 padding8[0x6];
    s16 viewportWidth;
    s16 viewportHeight;
    /* 0xCC */ s16 unkCC;
    /* 0xCE */ s16 unkCE;
    /* 0xD0 */ s16 unkD0;
    /* 0xD2 */ s16 unkD2;
    /* 0xD4 */ s16 unkD4;
    /* 0xD6 */ s16 unkD6;
    /* 0xD8 */ u16 perspNorm;
    /* 0xDA */ u16 id;
    /* 0xE0 */ Mtx perspectiveMatrix;
    u8 modelingMatrix[0x20];
    u16 unk140;
    u8 padding140[6];
    Node_70B00_ColorData unk148[1];
    u8 padding158[0x70];
    s16 fogMin;
    s16 fogMax;
    u8 fogR;
    u8 fogG;
    u8 fogB;
    u8 padding1CF;
    f32 scaleY;
    u8 padding5[0x2];
} Node_70B00;
extern Node_70B00 D_800A3370_A3F70;

void setViewportFadeValue(Node_70B00 *node, u8 fadeValue, u8 fadeMode);

void setViewportFadeValueBySlotIndex(u16 slotIndex, u8 fadeValue, u8 fadeMode);

void debugEnqueueCallback(u16 index, u8 arg1, void *arg2, void *arg3);

void *arenaAlloc16(s32 size);

void *advanceLinearAlloc(s32 size);

void tryProcessDisplayFrameUpdate(void);

void processDisplayFrameUpdate(void);

void handleFrameBufferComplete(s32 bufferIndex);

void setViewportEnvColor(Node_70B00 *node, u8 r, u8 g, u8 b);

void setViewportFogById(u16 viewportId, s16 fogMin, s16 fogMax, u8 fogR, u8 fogG, u8 fogB);

void setViewportScale(Node_70B00 *arg0, f32 scaleX, f32 scaleY);

void func_8006E054_6EC54(u16);

void func_8006DC40_6E840(void);

void func_8006FEF8_70AF8(Node_70B00 *node, u16 id);

void setViewportId(Node_70B00 *node, u16 id);

void setViewportTransformById(u16 viewportId, void *transformMatrix);

void initViewportNode(Node_70B00 *node, Node_70B00 *parent, s32 slotIndex, s32 priority, s32 flags);

void func_8006FA0C_7060C(Node_70B00 *node, f32 fov, f32 aspect, f32 near, f32 far);

void setModelCameraTransform(void *, s16, s16, s16, s16, s16, s16);

void unlinkNode(Node_70B00 *player);

void n_alSeqpDelete(Node_70B00 *arg0);

s32 getViewportFadeMode(Node_70B00 *);

s32 isObjectCulled(Vec3i *arg0);

typedef struct {
    u8 padding[0xB8];
    u8 viewportDisplayFlags;
} ViewportNode;

typedef struct {
    u8 padding[0xB8];
    u8 viewportDisplayFlags;
    u8 overlayR;
    u8 overlayG;
    u8 overlayB;
} ViewportNodeWithOverlay;

void disableViewportOverlay(ViewportNode *arg0);
void enableViewportDisplayList(void *arg0);
void disableViewportDisplayList(ViewportNode *arg0);
void func_8006FEE8_70AE8(ViewportNode *arg0);
void setViewportOverlayRgbAndEnable(ViewportNodeWithOverlay *arg0, s8 r, s8 g, s8 b);

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