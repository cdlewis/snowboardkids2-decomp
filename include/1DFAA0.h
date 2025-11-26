#pragma once

#include "20F0.h"
#include "6E840.h"
#include "common.h"
#include "main.h"

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
} PackedData;

typedef struct {
    union {
        struct {
            s16 unk0;
            PackedData unk2[2];
            u8 unkA;
        } One;
        struct {
            s32 unk0;
            s32 unk4;
            u8 unk8;
            u8 unk9;
            s8 unkA;
        } Two;
        struct {
            s32 unk0;
            s32 unk4;
            s16 unk8;
            s8 unkA;
        } CurtainPayload;
        struct {
            s16 unk0;
            s16 unk2;
        } TrickPayload;
        struct {
            s16 unk0;
            u8 padding[0xE];
            s32 unk10;
            s32 unk14;
            s32 unk18;
            s32 unk1C;
            s32 unk20;
            s32 unk24;
        } ChrPayload;
    } unk0;
    u8 padding[0x18];
    s8 unk40;
    u8 padding2[0x3];
    s8 unk44;
    u8 padding3[0xA7];
    s8 unkEC;
    u8 padding4[0x3];
    s32 unkF4;
} func_800B2A24_1DFAD4_arg_item;

typedef struct {
    Node_70B00 *unk0;
    u8 header[0xA4];
    func_800B2A24_1DFAD4_arg_item items[18];
    u8 padding[0x50];
    s32 unk1220;
} func_800B2A24_1DFAD4_arg;

typedef struct {
    u8 padding[0xFF7];
    s8 unkFF7;
} func_800B29F0_1DFAA0_arg;

typedef struct {
    void *start;
    void *end;
    u32 size;
    u32 padding;
} D_800BA960_1E7A10_node;

typedef struct {
    s8 unk0;
    s8 unk1;
    s16 unk2;
    s32 unk4;
    u8 padding[0xDC];
    s16 unkE4;
} TaskData;

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
} StateEntryItem;

typedef struct {
    u8 padding0[0xC];
    /* 0xC */ u16 current_index;
    u8 padding[0x2];
    /* 0x10 */ u16 unk10;
    /* 0x12 */ u8 unk12;
    /* 0x13 */ u8 unk13;
    /* 0x14 */ u16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ u8 unk1C;
    u8 padding4[0x3];
    StateEntryItem items[2];
    s32 unk34;
    /* 0x38 */ u16 next_index;
    /* 0x3A */ u16 prev_index;
    s16 unk3C;
    u8 unk3E;
    u8 unk3F;
} StateEntry;

typedef struct {
    u8 padding[0x16];
    u16 unk16;
} UIResource;

typedef struct {
    union {
        s32 One;
        s8 Two;
        u8 bytes[4];
    } unk0;
    applyTransformToModel_arg1 unk04;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ s32 unk40;
    /* 0x44 */ s32 unk44;
    u8 _pad48[0xC];
    /* 0x54 */ s32 unk54;
    /* 0x58 */ s32 unk58;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ s32 unk60;
    /* 0x64 */ s32 unk64;
    /* 0x68 */ s32 unk68;
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ s32 unk70;
    /* 0x74 */ s32 unk74;
    /* 0x78 */ s16 unk78;
    /* 0x7A */ s16 unk7A;
    /* 0x7C */ s16 unk7C;
    u8 _pad7E[0x2];
    /* 0x80 */ s16 unk80;
    /* 0x82 */ s16 unk82;
    /* 0x84 */ s16 unk84;
    /* 0x86 */ s16 unk86;
    /* 0x88 */ s16 unk88;
    u8 _pad8A[0x4];
    /* 0x8E */ s16 angle;
    u8 padding3[0x10];
    /* 0xA0 */ s32 unkA0;
    /* 0xA4 */ s32 unkA4;
} CutsceneSlotData;

typedef struct {
    union {
        struct {
            s16 unk0;
            PackedData unk2[2];
            u8 unkA;
        } One;
        struct {
            s32 unk0;
            s32 unk4;
            u8 unk8;
            u8 unk9;
            s8 unkA;
        } Two;
        struct {
            s32 unk0;
            s32 unk4;
            s16 unk8;
            s8 unkA;
        } CurtainPayload;
        struct {
            s16 unk0;
            s16 unk2;
        } TrickPayload;
        struct {
            s16 unk0;
            u8 padding[0xE];
            s32 unk10;
            s32 unk14;
            s32 unk18;
            s32 unk1C;
            s32 unk20;
            s32 unk24;
        } ChrPayload;
    } unk0;
    u8 padding[0x1A];
    /* 0x42 */ u8 unk42;
    /* 0x43 */ u8 unk43;
    /* 0x44 */ u8 unk44;
    /* 0x45 */ u8 padding3[0x3];
    /* 0x48 */ SceneModel *model;
    /* 0x4C */ CutsceneSlotData slotData;
} CutsceneSlot; /* Total size: 0xF4 */

typedef struct {
    /* 0x0 */ Node_70B00 *uiResource;
    /* 0x4 */ s8 pad4[0x8];
    /* 0xC */ void *sceneContext;
    /* 0x10 */ setModelRenderMode_arg unk10;
    /* 0x98 */ u16 currentFrame;
    /* 0x9A */ u16 maxFrame;
    /* 0x9C */ u16 endFrame;
    /* 0x9E */ s8 pad9E[0x2];
    /* 0xA0 */ s8 debugText[0x8];
    /* 0xA8 */ CutsceneSlot slots[16];
    /* 0xFE8 */ void *textRenderer;
    /* 0xFEC */ s8 padFEC[0x8];
    /* 0xFF4 */ s8 showDebugInfo;
    /* 0xFF5 */ s8 enableTransparency;
    /* 0xFF6 */ s8 unused_FF6;
    /* 0xFF7 */ s8 skipAnimation;
    /* 0xFF8 */ s8 padFF8[0x220];
    /* 0x1218 */ void *shadowModel;
    /* 0x121C */ void *reflectionModel;
    /* 0x1220 */ s32 cameraAnimationTimer;
} CutsceneManager;

void resetScriptState(u8 *arg0);

void func_800B29F0_1DFAA0(func_800B29F0_1DFAA0_arg *arg0);

void func_800B2A50_1DFB00(CutsceneManager *arg0, s16 arg1);

CutsceneSlot *func_800B2A78_1DFB28(CutsceneManager *arg0, s16 arg1);

void func_800B2A24_1DFAD4(CutsceneManager *arg0, s16 arg1);

StateEntryItem *func_800B34B0_1E0560(s32);
