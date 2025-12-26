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
    s8 unkC;
    u8 padding[3];
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
    s8 unk6;
    s8 unk7;
    s8 unk8;
    s8 unk9;
} StateEntryItem;

typedef struct {
    u8 pad[0x210];
    s32 unk210;
} Func8000C268Arg;

#ifndef STATE_ENTRY_DECLARED
#define STATE_ENTRY_DECLARED
typedef struct StateEntry StateEntry;
#endif

struct StateEntry {
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
};

typedef struct {
    u8 padding[0x16];
    u16 unk16;
} UIResource;

typedef struct {
    union {
        s32 One;
        s8 Two;
        s8 bytes[4];
    } unk0;
    Mat3x3Padded unk04;
    union {
        SceneModel *unk20;
        s32 unk20_s32;
        s16 unk20_s16;
    } unk20_u;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ s32 unk40;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ s32 unk48;
    /* 0x4C */ s32 unk4C;
    /* 0x50 */ s32 unk50;
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
    /* 0x7E */ s16 unk7E;
    /* 0x80 */ s16 unk80;
    /* 0x82 */ s16 unk82;
    /* 0x84 */ s16 unk84;
    /* 0x86 */ s16 unk86;
    /* 0x88 */ s16 unk88;
    /* 0x8A */ s16 unk8A;
    /* 0x8C */ s16 unk8C;
    /* 0x8E */ s16 angle;
    /* 0x90 */ s16 unk90;
    /* 0x92 */ s16 unk92;
    /* 0x94 */ s32 unk94;
    /* 0x98 */ s32 unk98;
    union {
        s32 unk9C_s32;
        struct {
            s16 unk9C_low;
            s16 unk9E;
        } s;
    } unk9C_u;
    /* 0xA0 */ s32 unkA0;
    /* 0xA4 */ union {
        SceneModel *ptr;
        s8 byte;
    } unkA4;
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
    u8 padding[0x18];
    /* 0x40 */ u16 unk40;
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
    /* 0xFF8 */ Func8000C268Arg unkFF8;
    u8 padding[0xC];
    /* 0x1218 */ void *shadowModel;
    /* 0x121C */ void *reflectionModel;
    /* 0x1220 */ s32 cameraAnimationTimer;
} CutsceneManager;

typedef struct {
    u8 padding[0xC];
    void *unkC;
    func_80000710_1310_arg unk10;
    u8 padding2[0x20];
    s32 unkB8;
    u8 padding3[0x2C];
    u16 unkE8;
    u8 padding4[0x6];
    SceneModel *unkF0;
    u8 padding5[0x0];
} func_800B2C78_arg;

void func_800B29F0_1DFAA0(CutsceneManager *arg0);
void func_800B29FC_1DFAAC(CutsceneManager *arg0);
void func_800B2A24_1DFAD4(CutsceneManager *arg0, s16 arg1);
void func_800B2A50_1DFB00(CutsceneManager *arg0, s16 arg1);
CutsceneSlot *func_800B2A78_1DFB28(CutsceneManager *arg0, s16 arg1);
void func_800B2AA0(CutsceneManager *, Node_70B00 *arg1, void *arg2, void *arg3);
void func_800B2C78(func_800B2C78_arg *arg0);
void func_800B2D04(CutsceneSlot *arg0);
void func_800B2D68_1DFE18(CutsceneSlot *arg0);
void func_800B2DCC_1DFE7C(CutsceneSlot *arg0);
void func_800B2E48_1DFEF8(CutsceneManager *manager);
s32 processCutsceneFrame(CutsceneManager *uiManager);
s16 func_800B3360(s16, s16);
void *func_800B3420_1E04D0(void);
void *func_800B3430_1E04E0(void);
u16 func_800B3440_1E04F0(void);
u8 func_800B3450_1E0500(void);
u8 getCutsceneSlotCount(void);
u16 func_800B3470_1E0520(void);
u16 func_800B3480_1E0530(void);
s16 func_800B3490_1E0540(void);
void func_800B34A0_1E0550(s16 arg0);
StateEntryItem *func_800B34B0_1E0560(s32 arg0);
s16 func_800B34D0_1E0580(void);
void func_800B34E0_1E0590(s16 arg0);
u8 func_800B34F0(void);
void func_800B3500_1E05B0(u8 arg0);
s32 func_800B3510_1E05C0(void);
s32 func_800B3540_1E05F0(void);
s32 func_800B36C0(void *arg0);
s32 func_800B3790_1E0840(void);
u16 func_800B384C_1E08FC(void);
void resetScriptState(u8 *arg0);
void func_800B388C_1E093C(s32 arg0);
void func_800B3B40(void);
u16 func_800B3B68_1E0C18(u8 arg0, u16 arg1, s32 arg2);
s32 findEventAtFrame(u8 a0, u16 a1);
s32 func_800B3D24_1E0DD4(u8, u16);
void func_800B3E58_1E0F08(u16 entryIndex, u16 oldPrevIndex, u16 newPrevIndex);
StateEntry *getStateEntry(u16 arg0);
u16 func_800B41E0_1E1290(void);
u16 func_800B4258_1E1308(u8 arg0);
StateEntry *func_800B4288_1E1338(void);
u8 *func_800B4294_1E1344(void);
u16 func_800B42B0_1E1360(u16 arg0);
u16 func_800B42E8_1E1398(u16 arg0);
void func_800B4320_1E13D0(void);
void func_800B4378_1E1428(u8 slotIndex, s16 frameNumber);
void func_800B44A8_1E1558(u8 arg0, u16 arg1);
void func_800B4534_1E15E4(s32, s32);
void func_800B462C_1E16DC(u8 arg0, u16 arg1, s32 arg2);
void *func_800B4680_1E1730(s8 arg0);
void func_800B46E0(s32 arg0, s8 arg1, s16 arg2);
void func_800B477C_1E182C(void *);
void initializeCutsceneSystem(void *);
