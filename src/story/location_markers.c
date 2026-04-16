#include "story/location_markers.h"
#include "D_800AFE8C_A71FC_type.h"
#include "assets.h"
#include "audio/audio.h"
#include "common.h"
#include "graphics/camera_transform.h"
#include "graphics/graphics.h"
#include "math/geometry.h"
#include "os_cont.h"
#include "story/rare_events.h"
#include "system/task_scheduler.h"
#include "text/font_render.h"

// Struct definitions
typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s8 unk12;
    s8 unk13;
} StoryMapLocationMarkerEntry;

typedef struct {
    StoryMapLocationMarkerEntry entries[4];
    char __padding[6];
    void *unk58;
    s16 unk5C;
    s16 unk5E;
    u8 unk60;
    char __padding2[3];
    s16 unk64;
    s16 unk66;
    s16 unk68;
    s16 unk6A;
    s16 unk6C;
    s16 unk6E;
    s16 unk70;
    s8 unk72;
} StoryMapLocationMarkerState;

typedef struct {
    char padding;
    s32 *unk4;
    char padding2[78];
    s32 *unk58;
} StoryMapLocationMarkerCleanupArg;

typedef struct {
    u16 unk0;
    u16 unk2;
    void *unk4;
    u16 unk8;
    u16 unkA;
    u8 unkC;
    u8 unkD;
    char padding[2];
    u8 unk10;
    u8 unk11;
    u8 unk12;
} StoryMapSpecialLocationMarkerState;

typedef struct {
    s16 unk0;
    s16 unk2;
    s32 unk4;
    s16 unk8;
    u8 padding[0x6];
    u8 unk10;
    u8 unk11;
    u8 unk12;
} SpecialLocationMarkerUpdateState;

typedef struct {
    char padding[4];
    s32 *unk4;
} SpecialLocationMarkerCleanupState;

typedef struct {
    /* 0x00 */ Transform3D localMatrix;
    /* 0x20 */ Transform3D worldMatrix;
    /* 0x40 */ u16 *dialogueScript;
    /* 0x44 */ u16 yawAngle;
    /* 0x46 */ s16 targetYaw;
    /* 0x48 */ u16 pitchAngle;
    /* 0x4A */ u8 pad4A[0x13];
    /* 0x5D */ u8 turnDirection;
} StoryMapDialogueState;

typedef struct {
    /* 0x000 */ u8 padding0[0x10];
    /* 0x010 */ u8 unk10;
    /* 0x011 */ u8 padding1[0x62];
    /* 0x073 */ u8 unk73;
    /* 0x074 */ u8 padding2[0x33C];
    /* 0x3B0 */ Transform3D characterTransform;
    /* 0x3D0 */ u8 padding2b[0x30];
    /* 0x400 */ u8 animState;
    /* 0x401 */ u8 updateFlag;
    /* 0x402 */ u8 pauseTimer;
    /* 0x403 */ u8 dialogueResult;
    /* 0x404 */ u8 padding3[0x4];
    /* 0x408 */ s32 npcPosX[2];
    /* 0x410 */ s32 npcPosZ[2];
    /* 0x418 */ u8 padding5[0x4];
    /* 0x41C */ s32 numEntries;
    /* 0x420 */ u8 activeNpcIndex;
    /* 0x421 */ u8 unk421;
    /* 0x422 */ u8 cancelDialogue;
    /* 0x423 */ u8 padding6[0x1];
    /* 0x424 */ u8 locationDiscovered;
    /* 0x425 */ u8 discoveredLocationId;
    /* 0x426 */ u8 padding7[0x4];
    /* 0x42A */ u8 dialogueTurnState;
    /* 0x42B */ u8 padding8[0x1];
    /* 0x42C */ u8 dialogueLineIndex;
    /* 0x42D */ u8 unk42D;
} StoryMapAllocation;

typedef struct {
    u8 padding[1012];
    s16 unk3F4;
    s32 unk3F8;
    u8 padding2[7];
    u8 dialogueResult;
    u8 padding3[28];
    u8 activeNpcIndex;
    u8 padding5[3];
    u8 locationDiscovered;
    u8 discoveredLocationId;
    u8 padding4[7];
    u8 unk42D;
} func_800698BC_6A4BC_return;

typedef struct {
    s32 unk0;
    s32 unk4;
} D_8008F810_90410_item;

// Function declarations
void updateDiscoveryMarkerDisplay(void *);
void updateStoryMapLocationMarker(void *);
void cleanupDiscoveryLocationMarker(void *);
void initDiscoveryLocationMarker(StoryMapLocationMarkerState *);
void initTownExitMarker(StoryMapSpecialLocationMarkerState *);
void checkTownExitTrigger(void *);
void cleanupTownExitMarker(void *);
void updateStoryMapSpecialLocationMarker(SpecialLocationMarkerUpdateState *);

void func_80036AF8_376F8(void);
void updateStoryMapDialogueTurn(void *arg0);
void setupStoryMapCharacterDialogue(StoryMapDialogueState *);
void func_800175E0_181E0(void);

// Global variables and externs
extern s32 gControllerInputs;
extern s32 renderTextSprite;

// Data definitions
D_8008F810_90410_item D_8008F810_90410[0xC] = {
    { 0x00835400, 0xFFDF9500 },
    { 0x005F7C00, 0xFF9F4B00 },
    { 0x00241800, 0xFF7C9E00 },
    { 0xFFDC8500, 0xFF7C7800 },
    { 0xFFA08800, 0xFF9F4B00 },
    { 0xFF7C0D00, 0xFFDE2100 },
    { 0x00051800, 0xFF789E00 },
    { 0x00428E00, 0xFF896600 },
    { 0xFFD28600, 0xFFB63B00 },
    { 0xFF882100, 0xFFBC3300 },
    { 0xFF7A5C00, 0x00191E00 },
    { 0x0086EC00, 0x000AB000 }
};

u8 D_8008F870_90470[] = {
    0xFF, 0xD8, 0xFF, 0xE8, 0x00, 0x28, 0xFF, 0xE8, 0xFF, 0xD8, 0x00, 0x08, 0x00, 0x28, 0x00, 0x08
};

u8 D_8008F880_90480[] = { 0x00, 0x19, 0x80, 0x2E, 0x80, 0x36, 0x80, 0x3B, 0x80, 0x41, 0xFF, 0xFB,
                          0x00, 0x1C, 0x80, 0x35, 0x80, 0x3C, 0x80, 0x3D, 0xFF, 0xFF, 0x00, 0x00 };

u8 D_8008F898_90498[] = { 0x00, 0x20, 0x80, 0x32, 0x80, 0x3B, 0x80, 0x31, 0x80, 0x46, 0x80, 0x4B,
                          0x80, 0x40, 0xFF, 0xFD, 0xA0, 0x12, 0x80, 0x3B, 0x80, 0x41, 0x80, 0x32,
                          0x80, 0x3F, 0x80, 0x3B, 0x80, 0x32, 0x80, 0x41, 0xFF, 0xFF, 0x00, 0x00 };

u8 D_8008F8BC_904BC[] = { 0x00, 0x1C, 0x80, 0x35, 0x80, 0x3C, 0x80, 0x3C, 0x80, 0x41, 0xFF, 0xFB,
                          0x00, 0x0C, 0x80, 0x3F, 0x80, 0x3C, 0x80, 0x40, 0x80, 0x40, 0xFF, 0xFF };

u8 D_8008F8D4_904D4[] = { 0x00, 0x0C, 0x80, 0x35, 0x80, 0x2E, 0x80, 0x3F, 0x80, 0x2E, 0x80, 0x30,
                          0x80, 0x41, 0x80, 0x32, 0x80, 0x3F, 0xFF, 0xFD, 0x00, 0x1C, 0x80, 0x32,
                          0x80, 0x39, 0x80, 0x32, 0x80, 0x30, 0x80, 0x41, 0xFF, 0xFF, 0x00, 0x00 };

u8 D_8008F8F8_904F8[] = { 0x00, 0x0B, 0x80, 0x3C, 0x80, 0x2E, 0x80, 0x3F, 0x80, 0x31, 0xFF, 0xFB,
                          0x00, 0x1C, 0x80, 0x35, 0x80, 0x3C, 0x80, 0x3D, 0xFF, 0xFF, 0x00, 0x00 };

u8 D_8008F910_90510[] = { 0x00, 0x1D, 0x80, 0x3F, 0x80, 0x36, 0x80, 0x30, 0x80, 0x38, 0xFF, 0xFB,
                          0x00, 0x10, 0x80, 0x2E, 0x80, 0x3A, 0x80, 0x32, 0xFF, 0xFF, 0x00, 0x00 };

u8 D_8008F928_90528[] = { 0x00, 0x1C, 0x80, 0x2E, 0x80, 0x43, 0x80, 0x32, 0xFF, 0xFB, 0x80, 0x5B,
                          0xFF, 0xFB, 0x00, 0x0E, 0x80, 0x45, 0x80, 0x36, 0x80, 0x41, 0xFF, 0xFF };

u8 D_8008F940_90540[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF
};

u8 D_8008F950_90550[] = { 0x00, 0x1C, 0x80, 0x3D, 0x80, 0x32, 0x80, 0x32, 0x80, 0x31, 0xFF, 0xFB,
                          0x00, 0x0C, 0x80, 0x3F, 0x80, 0x3C, 0x80, 0x40, 0x80, 0x40, 0xFF, 0xFF };

u8 D_8008F968_90568[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF
};

u8 D_8008F978_90578[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x80, 0x00,
                          0x10, 0x00, 0x70, 0x00, 0x20, 0x00, 0x70, 0x00, 0x10, 0x00, 0x60, 0x00, 0x20, 0x00, 0x70,
                          0x00, 0x10, 0x00, 0x70, 0x00, 0x10, 0x00, 0x80, 0x00, 0x10, 0x00, 0x80, 0x00, 0x10, 0x00,
                          0x70, 0x00, 0x10, 0x00, 0x80, 0x00, 0x20, 0x00, 0x40, 0x00, 0x10, 0x00, 0x40, 0x00, 0x10 };

void *D_8008F9B4_905B4[] = { D_8008F880_90480,
                             D_8008F898_90498,
                             D_8008F8BC_904BC,
                             D_8008F8D4_904D4,
                             D_8008F8F8_904F8,
                             D_8008F910_90510,
                             D_8008F928_90528,
                             D_8008F940_90540,
                             D_8008F950_90550,
                             D_8008F968_90568,
                             D_8008F978_90578,
                             D_8008F978_90578,
                             NULL,
                             NULL,
                             NULL };

u16 D_8008F9F0_905F0[][7] = {
    { 0x0000, 0x005C, 0x005C, 0x0060, 0x005B, 0x005C, 0x005E },
    { 0x0074, 0x0000, 0x0074, 0x0074, 0x0074, 0x0074, 0x0076 },
    { 0x008A, 0x008A, 0x0000, 0x0090, 0x008A, 0x008A, 0x008D },
    { 0x00AA, 0x00A2, 0x00AA, 0x0000, 0x00A2, 0x00A2, 0x00A5 },
    { 0x00BB, 0x00BB, 0x00BB, 0x00BB, 0x0000, 0x00BB, 0x00BE },
    { 0x00D0, 0x00D1, 0x00D1, 0x00D1, 0x00D0, 0x0000, 0x00D4 },
    { 0x0000, 0x005C, 0x005C, 0x0060, 0x005B, 0x005C, 0x0074 },
    { 0x0000, 0x0074, 0x0074, 0x0074, 0x0074, 0x008A, 0x008A },
    { 0x0000, 0x0090, 0x008A, 0x008A, 0x00A7, 0x00A2, 0x00A7 },
    { 0x0000, 0x00A2, 0x00A2, 0x00BB, 0x00BB, 0x00BB, 0x00BB },
    { 0x0000, 0x00BB, 0x00D0, 0x00D1, 0x00D1, 0x00D1, 0x00D0 },
    { 0x0000, 0x010B, 0x010B, 0x010B, 0x010B, 0x010B, 0x010B },
};

u16 D_8008FA98_90698[] = { 0x000D, 0x000E, 0x000F, 0xFFFF };
u16 D_8008FAA0_906A0[] = { 0x000A, 0x000B, 0x000C, 0xFFFF };
u16 D_8008FAA8_906A8[] = { 0x0014, 0x0015, 0x0016, 0xFFFF };
u16 D_8008FAB0_906B0[] = { 0x0016, 0x0017, 0x0018, 0xFFFF };
u16 D_8008FAB8_906B8[] = { 0x0011, 0x0012, 0x0013, 0xFFFF };

u16 D_8008FAC0_906C0[] = { 0x0000, 0xFFFF, 0x001B, 0x001B, 0xFFFF, 0x0000 };

u16 *D_8008FACC_906CC[][7] = {
    { NULL,
     D_8008FA98_90698,        D_8008FA98_90698,
     D_8008FAB0_906B0,                                            D_8008FA98_90698,
     D_8008FA98_90698,                                                                                D_8008FAA0_906A0 },
    { D_8008FA98_90698,
     NULL,                    D_8008FA98_90698,
     D_8008FA98_90698,                                            D_8008FA98_90698,
     D_8008FA98_90698,                                                                                D_8008FAA0_906A0 },
    { D_8008FA98_90698,
     D_8008FA98_90698,        NULL,
     D_8008FA98_90698,                                            D_8008FA98_90698,
     D_8008FA98_90698,                                                                                D_8008FAA0_906A0 },
    { D_8008FAB8_906B8,
     D_8008FA98_90698,        D_8008FAB8_906B8,
     NULL,                                                        D_8008FA98_90698,
     D_8008FA98_90698,                                                                                D_8008FAA0_906A0 },
    { D_8008FA98_90698,
     D_8008FA98_90698,        D_8008FA98_90698,
     D_8008FA98_90698,                                            NULL,
     D_8008FA98_90698,                                                                                D_8008FAA0_906A0 },
    { D_8008FA98_90698,
     D_8008FA98_90698,        D_8008FA98_90698,
     D_8008FA98_90698,                                            D_8008FA98_90698,
     NULL,                                                                                            D_8008FAA0_906A0 },
    { NULL,
     D_8008FA98_90698,        D_8008FA98_90698,
     D_8008FAB0_906B0,                                            D_8008FA98_90698,
     D_8008FA98_90698,                                                                                D_8008FA98_90698 },
    { NULL,
     D_8008FA98_90698,        D_8008FA98_90698,
     D_8008FA98_90698,                                            D_8008FA98_90698,
     D_8008FA98_90698,                                                                                D_8008FA98_90698 },
    { NULL,
     D_8008FA98_90698,        D_8008FA98_90698,
     D_8008FA98_90698,                                            D_8008FAA8_906A8,
     D_8008FA98_90698,                                                                                D_8008FAA8_906A8 },
    { NULL,
     D_8008FA98_90698,        D_8008FA98_90698,
     D_8008FA98_90698,                                            D_8008FA98_90698,
     D_8008FA98_90698,                                                                                D_8008FA98_90698 },
    { NULL,
     D_8008FA98_90698,        D_8008FA98_90698,
     D_8008FA98_90698,                                            D_8008FA98_90698,
     D_8008FA98_90698,                                                                                D_8008FA98_90698 },
    { NULL,
     D_8008FA98_90698,        D_8008FA98_90698,
     D_8008FA98_90698,                                            D_8008FA98_90698,
     D_8008FA98_90698,                                                                                D_8008FA98_90698 },
    { NULL,             NULL, D_8008FA98_90698, D_8008FA98_90698, D_8008FAC0_906C0, NULL,             NULL             },
    { NULL,             NULL, NULL,             NULL,             D_8008FA98_90698, D_8008FA98_90698, D_8008FA98_90698 },
};

u16 *D_8008FC54_90854[] = { D_8008FAC0_906C0, D_8008FA98_90698, D_8008FAC0_906C0, D_8008FAC0_906C0,
                            D_8008FAC0_906C0, D_8008FAC0_906C0, D_8008FA98_90698, D_8008FAC0_906C0 };

u16 D_8008FC74_90874[][8] = {
    { 0x0000, 0x005C, 0x005E, 0x0000, 0x0000, 0x005E, 0x0000, 0x0000 },
    { 0x0079, 0x0074, 0x0000, 0x0077, 0x0000, 0x0000, 0x0074, 0x0000 },
    { 0x0000, 0x0000, 0x008D, 0x008E, 0x0000, 0x008D, 0x008A, 0x0000 },
    { 0x00AB, 0x00A2, 0x00A5, 0x0000, 0x0000, 0x00A5, 0x00A2, 0x0000 },
    { 0x00C0, 0x00BB, 0x0000, 0x00BF, 0x0000, 0x00BE, 0x0000, 0x0000 },
    { 0x00D6, 0x0000, 0x00D4, 0x00D5, 0x0000, 0x00D4, 0x00D0, 0x0000 },
    { 0x0051, 0x006D, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x00F5 },
    { 0x00E3, 0x0000, 0x0000, 0x0000, 0x0083, 0x0099, 0x0000, 0x00C9 },
    { 0x0000, 0x00F4, 0x00E1, 0x0000, 0x0000, 0x0000, 0x0083, 0x0000 },
};

u16 D_8008FC74_90874_tail[6] = { 0x0000, 0x00C9, 0x0000, 0x00F2, 0x00E3, 0x0000 };

u16 D_8008FD10_90910[] = { 0x0059, 0x0072, 0x0088, 0x00A0, 0x00B9, 0x00CE };

s8 D_8008FD1C_9091C[] = { 3, 3, 3, 3, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

// Function implementations
void initDiscoveryDisplaySystem(s8 *arg0) {
    *arg0 = 0;
    setCallback(&updateDiscoveryMarkerDisplay);
}

void updateDiscoveryMarkerDisplay(void *arg0) {
    func_800698BC_6A4BC_return *temp_v0;
    StoryMapAllocation *temp_v0_2;
    StoryMapAllocation *temp_v0_3;

    temp_v0 = (func_800698BC_6A4BC_return *)getCurrentAllocation();
    if (temp_v0->locationDiscovered != 0) {
        if (*((u8 *)arg0) == 0) {
            if (temp_v0->discoveredLocationId < 0xAU) {
                temp_v0_2 = scheduleTask(&initDiscoveryLocationMarker, 0, 0, 0x64);
                if (temp_v0_2 != NULL) {
                    temp_v0_2->unk73 = temp_v0->discoveredLocationId;
                }
            } else {
                temp_v0_3 = scheduleTask(&initTownExitMarker, 0, 0, 0x64);
                if (temp_v0_3 != NULL) {
                    temp_v0_3->unk10 = temp_v0->discoveredLocationId;
                }
            }
            *(u8 *)arg0 = 1;
        }
    } else {
        *(u8 *)arg0 = 0;
    }
}

void initDiscoveryLocationMarker(StoryMapLocationMarkerState *arg0) {
    void *resource;
    void *temp_value;
    s32 i;

    resource = loadCompressedData(&_45A890_ROM_START, &_45A890_ROM_END, 0x3108);
    temp_value = loadTextRenderAsset(1);

    setCleanupCallback(&cleanupDiscoveryLocationMarker);

    arg0->unk6E = 0x2CC;
    arg0->unk70 = 0x2CC;
    arg0->unk6A = 0x4000;
    arg0->unk6C = 0x4000;
    arg0->unk72 = 0;
    arg0->unk64 = 0;
    arg0->unk66 = 0;
    arg0->unk68 = 0xFF;

    for (i = 0; i < 4; i++) {
        StoryMapLocationMarkerEntry *var_v1 = &arg0->entries[i];
        var_v1->unk0 = 0;
        var_v1->unk2 = 0;
        var_v1->unk8 = i;
        var_v1->unk13 = 0;
        var_v1->unk12 = 0;
        var_v1->unkE = 0;
        var_v1->unk10 = 0xFF;
        var_v1->unkA = 0x400;
        var_v1->unkC = 0x400;
        var_v1->unk4 = resource;
    }

    arg0->unk58 = temp_value;
    arg0->unk5C = 0xFF;
    arg0->unk60 = 5;
    arg0->unk5E = 0xFF;

    setCallback(&updateStoryMapLocationMarker);
}

// NO_SCRATCH
INCLUDE_ASM("asm/nonmatchings/story/location_markers", updateStoryMapLocationMarker);

void cleanupDiscoveryLocationMarker(void *untypedArg) {
    StoryMapLocationMarkerCleanupArg *arg0 = (StoryMapLocationMarkerCleanupArg *)untypedArg;
    arg0->unk4 = freeNodeMemory(arg0->unk4);
    arg0->unk58 = freeNodeMemory(arg0->unk58);
}

void initTownExitMarker(StoryMapSpecialLocationMarkerState *arg0) {
    void *resource;

    resource = loadCompressedData(&_45A890_ROM_START, &_45A890_ROM_END, 0x3108);
    setCleanupCallback(&cleanupTownExitMarker);
    arg0->unk8 = 8;
    arg0->unkA = 0xFF;
    arg0->unk0 = 0;
    arg0->unk2 = 0;
    arg0->unk4 = resource;
    arg0->unkD = 0;
    if (arg0->unk10 == 0xB) {
        arg0->unkC = 1;
    } else {
        arg0->unkC = 0;
    }
    arg0->unk11 = 0;
    arg0->unk12 = 0;
    setCallback(&updateStoryMapSpecialLocationMarker);
}

void updateStoryMapSpecialLocationMarker(SpecialLocationMarkerUpdateState *arg0) {
    Vec3i sp10;
    s32 sp20;
    s32 sp24;
    s32 temp_sp20;
    s32 temp_sp24;
    func_800698BC_6A4BC_return *temp_v0;
    s32 temp_val;

    temp_v0 = getCurrentAllocation();
    sp10.x = D_8008F810_90410[arg0->unk10].unk0;
    temp_val = D_8008F810_90410[arg0->unk10].unk4;
    sp10.y = 0x4B0000;
    sp10.z = temp_val;

    worldToScreenCoords(&sp20, &sp24, &sp10);

    arg0->unk0 = (s16)(sp20 - 0xA0);
    arg0->unk2 = (s16)(sp24 - 0x78);

    arg0->unk12 = (arg0->unk12 + 1) & 7;
    if (arg0->unk12 == 0) {
        volatile u8 padding[0x2];
        arg0->unk11 = (arg0->unk11 + 1) & 3;
    }

    arg0->unk8 = (s16)(arg0->unk11 + 8);

    if (temp_v0->locationDiscovered == 0) {
        terminateCurrentTask();
        return;
    }

    debugEnqueueCallback(8, 7, &renderTextSprite, arg0);
}

void cleanupTownExitMarker(void *untypedArg) {
    SpecialLocationMarkerCleanupState *arg0 = (SpecialLocationMarkerCleanupState *)untypedArg;
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void initTownExitTrigger(void *arg0) {
    StoryMapSpecialLocationTriggerState *state = (StoryMapSpecialLocationTriggerState *)arg0;
    state->baseLocationIndex = 0xA;
    state->padding = 0;
    setCallback(&checkTownExitTrigger);
}

void checkTownExitTrigger(void *arg0) {
    s16 temp_v0_2;
    s16 var_v1;
    func_800698BC_6A4BC_return *temp_v0 = (func_800698BC_6A4BC_return *)getCurrentAllocation();

    if (temp_v0->unk3F8 > 0x400000) {
        temp_v0_2 = temp_v0->unk3F4;
        var_v1 = temp_v0_2;
        if (temp_v0_2 >= 0x1001) {
            var_v1 -= 0x2000;
        }
        if (var_v1 >= 0x821) {
            temp_v0->locationDiscovered = 1;
            temp_v0->discoveredLocationId = *(u8 *)arg0;
            return;
        }
        if (var_v1 < -0x844) {
            temp_v0->locationDiscovered = 1;
            temp_v0->discoveredLocationId = *((u8 *)arg0) + 1;
        }
    }
}

// 97.70% https://decomp.me/scratch/OXSn6
INCLUDE_ASM("asm/nonmatchings/story/location_markers", func_80036AF8_376F8);

s32 checkStoryMapLocationSelection(StoryMapPlayerState *player) {
    StoryMapAllocation *allocation;
    struct {
        s32 distances[1];
        s32 maxDistance;
    } data;
    s32 i;
    s32 deltaX;
    s32 deltaZ;

    allocation = (StoryMapAllocation *)getCurrentAllocation();
    if (allocation->numEntries == 0) {
        return 0;
    }

    if (allocation->unk421 == 1) {
        return 0;
    }

    if (allocation->cancelDialogue == 1) {
        return 0;
    }

    data.maxDistance = 0x01000000;
    if (allocation->dialogueTurnState == 0) {
        for (i = 0; i < allocation->numEntries; i++) {
            deltaX = allocation->npcPosX[i] - player->positionX;
            deltaZ = allocation->npcPosZ[i] - player->positionZ;
            deltaX = distance_2d(deltaX, deltaZ);
            data.distances[i] = deltaX;

            if (deltaX <= 0x37FFFF) {
                if (gControllerInputs & A_BUTTON) {
                    allocation->dialogueTurnState = 0x11;
                }
            }
        }
    }

    if (allocation->dialogueTurnState != 0x11) {
        return 0;
    }

    if (data.distances[0] < data.maxDistance) {
        return 1;
    }

    return 2;
}

void updateStoryMapDialogueTurn(void *arg0) {
    Transform3D tempMatrix;
    StoryMapDialogueState *state = arg0;
    StoryMapAllocation *allocation;
    s32 switchState;
    s16 angleDiff;
    s32 posX;
    s32 posZ;
    s32 combinedAngle;
    s16 targetAngle;
    s32 absDiff;
    s32 soundId;
    s32 temp;

    allocation = (StoryMapAllocation *)getCurrentAllocation();
    switchState = allocation->dialogueTurnState >> 4;
    allocation->dialogueTurnState = allocation->dialogueTurnState & 0xF;

    switch (switchState) {
        case 1:
            if (allocation->numEntries == 2) {
                posX = (allocation->npcPosX[0] + allocation->npcPosX[1]) / 2;
                posZ = (allocation->npcPosZ[0] + allocation->npcPosZ[1]) / 2;
            } else {
                posX = allocation->npcPosX[0];
                posZ = allocation->npcPosZ[0];
            }

            combinedAngle = (state->yawAngle + state->pitchAngle) & 0x1FFF;
            targetAngle =
                computeAngleToPosition(posX, posZ, state->worldMatrix.translation.x, state->worldMatrix.translation.z);

            angleDiff = signedAngleDifference(combinedAngle, targetAngle);
            if (angleDiff < 0) {
                state->turnDirection = 1;
            } else {
                state->turnDirection = 0;
            }
            absDiff = ABS(angleDiff);
            if (absDiff >= 0xAAB) {
                temp = state->yawAngle + state->pitchAngle + 0x1000;
                allocation->animState = 1;
                allocation->updateFlag = 0;
                angleDiff = signedAngleDifference(temp & 0x1FFF, targetAngle);
                if (angleDiff < 0) {
                    state->turnDirection = 1;
                } else {
                    state->turnDirection = 0;
                }
                state->yawAngle = (state->yawAngle + 0x1000) & 0x1FFF;
            } else {
                if (absDiff < 0x2AA) {
                    allocation->animState = 0;
                } else {
                    allocation->animState = 2;
                }
                allocation->updateFlag = 1;
            }
            state->targetYaw = (state->yawAngle + angleDiff) & 0x1FFF;
            createYRotationMatrix(&state->localMatrix, state->yawAngle & 0x1FFF);
            createYRotationMatrix(&state->worldMatrix, state->pitchAngle);
            switchState = 2;

            func_8006B084_6BC84(&state->localMatrix, &state->worldMatrix, &tempMatrix);
            memcpy(&allocation->characterTransform, &tempMatrix, sizeof(Transform3D));
            break;

        case 2:
            angleDiff = signedAngleDifference((s16)state->yawAngle, state->targetYaw);
            absDiff = ABS(angleDiff);
            if (absDiff < 0x80) {
                allocation->animState = 0;
                switchState = 3;
                angleDiff = absDiff;
            } else {
                angleDiff = 0x80;
            }
            if (state->turnDirection != 0) {
                s32 tmp = angleDiff;
                angleDiff = -tmp;
            }
            if (allocation->animState == 1 && allocation->updateFlag != 0) {
                allocation->animState = 0;
            }

            state->yawAngle += angleDiff;

            createYRotationMatrix(&state->localMatrix, state->yawAngle & 0x1FFF);
            createYRotationMatrix(&state->worldMatrix, state->pitchAngle);
            func_8006B084_6BC84(&state->localMatrix, &state->worldMatrix, &tempMatrix);
            memcpy(&allocation->characterTransform, &tempMatrix, sizeof(Transform3D));
            break;

        case 3:
            if (allocation->dialogueTurnState == 5) {
                if (allocation->numEntries == 1) {
                    state->dialogueScript =
                        D_8008FACC_906CC[D_800AFE8C_A71FC->playerBoardIds[0]][allocation->activeNpcIndex];
                    soundId = D_8008F9F0_905F0[D_800AFE8C_A71FC->playerBoardIds[0]][allocation->activeNpcIndex];
                } else {
                    state->dialogueScript = D_8008FC54_90854[allocation->dialogueLineIndex];
                    soundId = D_8008FC74_90874[D_800AFE8C_A71FC->playerBoardIds[0]][allocation->dialogueLineIndex];
                }

                playSoundEffectOnChannelNoPriority(soundId, 0);
                setupStoryMapCharacterDialogue(state);

                allocation->animState = *state->dialogueScript;
                state->dialogueScript++;
                allocation->updateFlag = 0;

                if (allocation->animState || allocation->numEntries != 2) {
                    switchState = 4;
                } else {
                    switchState = 6;
                    allocation->pauseTimer = 0;
                }
            } else if (allocation->animState == 1 && allocation->updateFlag) {
                allocation->animState = 0;
            }
            break;

        case 4:
            if (allocation->updateFlag != 0) {
                allocation->updateFlag = 0;
                if (*state->dialogueScript == 0xFFFF) {
                    switchState = 5;
                    if (allocation->animState == 0) {
                        switchState = 6;
                        allocation->pauseTimer = 0;
                    } else {
                        allocation->updateFlag = 0;
                        allocation->animState = 0;
                    }
                } else {
                    allocation->animState = *state->dialogueScript;
                    state->dialogueScript += 1;
                }
            }
            break;

        case 5:
            switchState = 0;
            allocation->dialogueResult = 0xFF;
            setCallback(func_800175E0_181E0);
            break;

        case 6:
            allocation->pauseTimer++;
            if (allocation->pauseTimer == 0x14) {
                allocation->pauseTimer = 0;
                switchState = 5;
            }
            break;
    }

    allocation->dialogueTurnState |= switchState << 4;
    if (allocation->cancelDialogue == 1) {
        allocation->dialogueTurnState = 0;
        setCallback(func_800175E0_181E0);
    }
}

void setupStoryMapCharacterDialogue(StoryMapDialogueState *state) {
    func_800698BC_6A4BC_return *allocation = (func_800698BC_6A4BC_return *)getCurrentAllocation();

    if (allocation->activeNpcIndex == 3 && allocation->unk42D == 8) {
        state->dialogueScript = D_8008FAC0_906C0;
        playSoundEffectOnChannelNoPriority(D_8008FD10_90910[D_800AFE8C_A71FC->playerBoardIds[0]], 0);
        allocation->dialogueResult = D_8008FD1C_9091C[D_800AFE8C_A71FC->playerBoardIds[0]];
    }
}
