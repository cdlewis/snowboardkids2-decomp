#include "animation/easing_state.h"
#include "assets.h"
#include "audio/audio.h"
#include "common.h"
#include "cutscene/sys_effect.h"
#include "effects/cutscene_keyframes.h"
#include "graphics/graphics.h"
#include "graphics/sprite_rdp.h"
#include "graphics/sprite_table.h"
#include "math/geometry.h"
#include "os_cont.h"
#include "race/race_session.h"
#include "system/task_scheduler.h"
#include "text/font_assets.h"
#include "text/font_render.h"
#include "ui/level_preview_3d.h"
#include "ui/save_data.h"

extern u8 D_8009DF6C_9EB6C[];

typedef struct {
    s32 *layout;
    s32 count;
} MenuOptionConfig;

typedef struct {
    u8 x;
    u8 y;
    s8 pad[2];
    u8 spriteFrame;
    s8 labelIndex;
    s8 numExtra;
    s8 unlockSlotIndex;
    u8 pad2[4];
} GalleryItemEntry;

typedef struct {
    GalleryItemEntry *items;
    s32 count;
} GalleryCategoryData;

typedef struct {
    s8 menuState;
    s8 selectedOption;
    s8 menuType;
    s8 viewerComplete;
    u8 pad4[0x4];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
} ViewerState;

typedef struct {
    s16 x;
    s16 y;
    s16 config;
    s16 alpha;
    s32 label;
} LabelRenderEntry;

typedef struct {
    s16 x;
    s16 y;
    u8 pad4[4];
    s16 frameIndex;
    s16 shade;
    u8 padC[1];
    u8 transparency;
    u8 padE[2];
} SpriteSlot;

typedef struct {
    s16 x;
    s16 y;
    u8 pad4[6];
    s16 alpha;
    u8 padC[4];
} ExtraSpriteSlot;

typedef struct {
    s8 menuState;
    s8 selectedOption;
    s8 menuType;
    s8 viewerComplete;
    u8 pad4[0x4];
    void *unk8;
    void *unkC;
    u8 pad10[0x5E8];
    u8 unk5F8[0x174];
    SpriteSlot spriteSlots[27];
    ExtraSpriteSlot extraSpriteSlots[27];
    u8 unkACC[0x50];
    s16 unkB1C;
    s16 unkB1E;
    u8 padB20[0x4];
    s16 unkB24;
    s16 unkB26;
    u8 padB28[0x4];
    u8 unkB2C[0xA];
    s16 unkB36;
    u8 padB38[0x4];
    u8 unkB3C[0xC];
    u8 unkB48[0x8];
    s16 unkB50;
    u8 padB52[0x2];
    LabelRenderEntry labelEntries[27];
    u8 unkC98[0xC];
    u8 unkCA4[0x40];
} GalleryAlloc;

/* Data section definitions */

s16 gCurrentBgmId[2] = { 5, 0 };

s32 gMenuOptionLayout_Type0[] = { 0xFFD0FFE8, 0x00010103, 0x0000FFE8, 0x01020204, 0x0030FFE8,
                                  0x02040306, 0xFFE80010, 0x03030405, 0x00180010, 0x04050507 };

s32 gMenuOptionLayout_Type1[] = { 0xFFD0FFE8, 0x00010103, 0x0000FFE8, 0x01020204, 0x0030FFE8, 0x02040306,
                                  0xFFD00010, 0x03030405, 0x00000010, 0x04050507, 0x00300010, 0x00060608 };

MenuOptionConfig gMenuOptionConfig[] = {
    { gMenuOptionLayout_Type0, 5 },
    { gMenuOptionLayout_Type1, 6 },
};

u8 gNavigationCycleIndices[] = { 0x00, 0x01, 0x02, 0x01 };

s32 gDefaultMenuOptionCount = 4;
s32 gViewerDefaultPosX = 0x00285000;
s32 gViewerDefaultPosY = 0xFFDE3000;
s32 gViewerDefaultPosZ = 0x003A7000;

s32 gViewerInitPosition[] = { 0x00A92000, 0, 0, 0 };

extern const char D_8009DEB0_9EAB0[];
extern const char D_8009DEB4_9EAB4[];
extern const char D_8009DEB8_9EAB8[];
extern const char D_8009DEBC_9EABC[];
extern const char D_8009DEC0_9EAC0[];
extern const char D_8009DEC4_9EAC4[];
extern const char D_8009DEC8_9EAC8[];
extern const char D_8009DECC_9EACC[];
extern const char D_8009DED0_9EAD0[];
extern const char D_8009DED4_9EAD4[];
extern const char D_8009DED8_9EAD8[];
extern const char D_8009DEDC_9EADC[];
extern const char D_8009DEE0_9EAE0[];
extern const char D_8009DEE4_9EAE4[];
extern const char D_8009DEE8_9EAE8[];
extern const char D_8009DEEC_9EAEC[];
extern const char D_8009DEF0_9EAF0[];
extern const char D_8009DEF4_9EAF4[];
extern const char D_8009DEF8_9EAF8[];
extern const char D_8009DEFC_9EAFC[];
extern const char D_8009DF00_9EB00[];
extern const char D_8009DF04_9EB04[];
extern const char D_8009DF08_9EB08[];
extern const char D_8009DF0C_9EB0C[];
extern const char D_8009DF10_9EB10[];
extern const char D_8009DF14_9EB14[];
extern const char D_8009DF18_9EB18[];
extern const char D_8009DF1C_9EB1C[];
extern const char D_8009DF20_9EB20[];
extern const char D_8009DF24_9EB24[];
extern const char D_8009DF28_9EB28[];
extern const char D_8009DF2C_9EB2C[];
extern const char D_8009DF30_9EB30[];
extern const char D_8009DF34_9EB34[];
extern u16 D_8009ADE0_9B9E0;

s32 *gGalleryLabelPtrs[] = {
    (s32 *)D_8009DF34_9EB34, (s32 *)D_8009DF30_9EB30, (s32 *)D_8009DF2C_9EB2C, (s32 *)D_8009DF28_9EB28,
    (s32 *)D_8009DF24_9EB24, (s32 *)D_8009DF20_9EB20, (s32 *)D_8009DF1C_9EB1C, (s32 *)D_8009DF18_9EB18,
    (s32 *)D_8009DF14_9EB14, (s32 *)D_8009DF10_9EB10, (s32 *)D_8009DF0C_9EB0C, (s32 *)D_8009DF08_9EB08,
    (s32 *)D_8009DF04_9EB04, (s32 *)D_8009DF00_9EB00, (s32 *)D_8009DEFC_9EAFC, (s32 *)D_8009DEF8_9EAF8,
    (s32 *)D_8009DEF4_9EAF4, (s32 *)D_8009DEF0_9EAF0, (s32 *)D_8009DEEC_9EAEC, (s32 *)D_8009DEE8_9EAE8,
    (s32 *)D_8009DEE4_9EAE4, (s32 *)D_8009DEE0_9EAE0, (s32 *)D_8009DEDC_9EADC, (s32 *)D_8009DED8_9EAD8,
    (s32 *)D_8009DED4_9EAD4, (s32 *)D_8009DED0_9EAD0, (s32 *)D_8009DECC_9EACC, (s32 *)D_8009DEC8_9EAC8,
    (s32 *)D_8009DEC4_9EAC4, (s32 *)D_8009DEC0_9EAC0, (s32 *)D_8009DEBC_9EABC, (s32 *)D_8009DEB8_9EAB8,
    (s32 *)D_8009DEB4_9EAB4, (s32 *)D_8009DEB0_9EAB0
};

GalleryItemEntry gGalleryItems_SaveSlots[] = {
    { 0xA0, 0xB8, { 0x07, 0x00 }, 0x0C, 0x01, 0x01, 0x00, { 0 } },
    { 0xB8, 0xB8, { 0x07, 0x01 }, 0x0C, 0x02, 0x02, 0x00, { 0 } },
    { 0xD0, 0xB8, { 0x07, 0x02 }, 0x0C, 0x03, 0x02, 0x00, { 0 } },
    { 0xE8, 0xB8, { 0x07, 0x03 }, 0x0C, 0x04, 0x03, 0x00, { 0 } },
    { 0x00, 0xB8, { 0x07, 0x04 }, 0x0C, 0x05, 0x03, 0x00, { 0 } },
    { 0x18, 0xB8, { 0x07, 0x05 }, 0x0C, 0x06, 0x04, 0x00, { 0 } },
    { 0x30, 0xB8, { 0x07, 0x06 }, 0x0C, 0x07, 0x03, 0x00, { 0 } },
    { 0x48, 0xB8, { 0x07, 0x07 }, 0x0C, 0x08, 0x04, 0x00, { 0 } },
    { 0xAC, 0xD0, { 0x07, 0x08 }, 0x0C, 0x09, 0x04, 0x00, { 0 } },
    { 0xC4, 0xD0, { 0x07, 0x09 }, 0x0C, 0x0A, 0x04, 0x00, { 0 } },
    { 0xDC, 0xD0, { 0x07, 0x0A }, 0x0C, 0x0B, 0x04, 0x00, { 0 } },
    { 0xF4, 0xD0, { 0x07, 0x0B }, 0x0C, 0x0C, 0x05, 0x00, { 0 } },
    { 0x0C, 0xD0, { 0x07, 0x0C }, 0x0C, 0x1F, 0x03, 0x00, { 0 } },
    { 0x24, 0xD0, { 0x07, 0x0D }, 0x0C, 0x20, 0x03, 0x00, { 0 } },
    { 0x3C, 0xD0, { 0x07, 0x0E }, 0x0C, 0x21, 0x03, 0x00, { 0 } },
};

GalleryItemEntry gGalleryItems_Characters[] = {
    { 0x94, 0xB8, { 0x08, 0x00 }, 0x0D, 0x1C, 0x00, 0x00, { 0 } },
    { 0xAC, 0xB8, { 0x08, 0x01 }, 0x0D, 0x1D, 0x00, 0x00, { 0 } },
    { 0xC4, 0xB8, { 0x08, 0x02 }, 0x0D, 0x1E, 0x00, 0x00, { 0 } },
    { 0xDC, 0xB8, { 0x08, 0x03 }, 0x0E, 0x1C, 0x00, 0x00, { 0 } },
    { 0xF4, 0xB8, { 0x08, 0x04 }, 0x0E, 0x1D, 0x00, 0x00, { 0 } },
    { 0x0C, 0xB8, { 0x08, 0x05 }, 0x0E, 0x1E, 0x00, 0x00, { 0 } },
    { 0x24, 0xB8, { 0x08, 0x06 }, 0x0F, 0x1C, 0x00, 0x00, { 0 } },
    { 0x3C, 0xB8, { 0x08, 0x07 }, 0x0F, 0x1D, 0x00, 0x00, { 0 } },
    { 0x54, 0xB8, { 0x08, 0x08 }, 0x0F, 0x1E, 0x00, 0x00, { 0 } },
    { 0x94, 0xD0, { 0x08, 0x09 }, 0x10, 0xFF, 0x00, 0x00, { 0 } },
    { 0xAC, 0xD0, { 0x08, 0x0A }, 0x11, 0xFF, 0x00, 0x00, { 0 } },
    { 0xC4, 0xD0, { 0x08, 0x0B }, 0x12, 0xFF, 0x00, 0x00, { 0 } },
    { 0xDC, 0xD0, { 0x08, 0x0C }, 0x13, 0xFF, 0x00, 0x00, { 0 } },
    { 0xF4, 0xD0, { 0x08, 0x0D }, 0x14, 0xFF, 0x00, 0x00, { 0 } },
    { 0x0C, 0xD0, { 0x08, 0x0E }, 0x15, 0xFF, 0x00, 0x00, { 0 } },
    { 0x24, 0xD0, { 0x08, 0x0F }, 0x16, 0xFF, 0x00, 0x00, { 0 } },
    { 0x3C, 0xD0, { 0x08, 0x10 }, 0x17, 0xFF, 0x00, 0x00, { 0 } },
    { 0x54, 0xD0, { 0x08, 0x11 }, 0x18, 0xFF, 0x00, 0x00, { 0 } },
};

GalleryItemEntry gGalleryItems_Boards[] = {
    { 0x94, 0xC8, { 0x09, 0x00 }, 0x26, 0xFF, 0x00, 0x00, { 0 } },
    { 0xAC, 0xC8, { 0x09, 0x01 }, 0x27, 0xFF, 0x00, 0x00, { 0 } },
    { 0xC4, 0xC8, { 0x09, 0x02 }, 0x28, 0xFF, 0x00, 0x00, { 0 } },
    { 0xDC, 0xC8, { 0x09, 0x03 }, 0x29, 0xFF, 0x00, 0x00, { 0 } },
    { 0xF4, 0xC8, { 0x09, 0x04 }, 0x2A, 0xFF, 0x00, 0x00, { 0 } },
    { 0x0C, 0xC8, { 0x09, 0x05 }, 0x2B, 0xFF, 0x00, 0x00, { 0 } },
    { 0x24, 0xC8, { 0x09, 0x06 }, 0x2C, 0xFF, 0x00, 0x00, { 0 } },
    { 0x3C, 0xC8, { 0x09, 0x08 }, 0x2D, 0xFF, 0x00, 0x00, { 0 } },
    { 0x54, 0xC8, { 0x09, 0x07 }, 0x2E, 0xFF, 0x00, 0x00, { 0 } },
};

GalleryItemEntry gGalleryItems_Prizes[] = {
    { 0x94, 0xB8, { 0x0A, 0x00 }, 0x19, 0x01, 0x01, 0xFF, { 0 } },
    { 0xAC, 0xB8, { 0x0A, 0x01 }, 0x19, 0x02, 0x19, 0xFF, { 0 } },
    { 0xC4, 0xB8, { 0x0A, 0x02 }, 0x19, 0x03, 0x03, 0xFF, { 0 } },
    { 0xDC, 0xB8, { 0x0A, 0x03 }, 0x19, 0x04, 0x02, 0xFF, { 0 } },
    { 0xF4, 0xB8, { 0x0A, 0x04 }, 0x19, 0x05, 0x04, 0xFF, { 0 } },
    { 0x0C, 0xB8, { 0x0A, 0x05 }, 0x19, 0x06, 0x05, 0xFF, { 0 } },
    { 0x24, 0xB8, { 0x0A, 0x06 }, 0x19, 0x07, 0x06, 0xFF, { 0 } },
    { 0x3C, 0xB8, { 0x0A, 0x07 }, 0x19, 0x08, 0x0D, 0x0C, { 0 } },
    { 0x54, 0xB8, { 0x0A, 0x08 }, 0x19, 0x09, 0x14, 0x0E, { 0 } },
    { 0x94, 0xC8, { 0x0A, 0x09 }, 0x19, 0x0A, 0x0C, 0x00, { 0 } },
    { 0xAC, 0xC8, { 0x0A, 0x0A }, 0x19, 0x0B, 0x13, 0x01, { 0 } },
    { 0xC4, 0xC8, { 0x0A, 0x0B }, 0x19, 0x0C, 0x11, 0x02, { 0 } },
    { 0xDC, 0xC8, { 0x0A, 0x0C }, 0x19, 0x0D, 0x0E, 0x04, { 0 } },
    { 0xF4, 0xC8, { 0x0A, 0x0D }, 0x19, 0x0E, 0x0F, 0x05, { 0 } },
    { 0x0C, 0xC8, { 0x0A, 0x0E }, 0x19, 0x0F, 0x10, 0x06, { 0 } },
    { 0x24, 0xC8, { 0x0A, 0x0F }, 0x19, 0x10, 0x12, 0x08, { 0 } },
    { 0x3C, 0xC8, { 0x0A, 0x10 }, 0x19, 0x11, 0x15, 0x09, { 0 } },
    { 0x54, 0xC8, { 0x0A, 0x11 }, 0x19, 0x12, 0x16, 0x0A, { 0 } },
    { 0x94, 0xD8, { 0x0A, 0x12 }, 0x19, 0x13, 0x1F, 0x03, { 0 } },
    { 0xAC, 0xD8, { 0x0A, 0x13 }, 0x19, 0x14, 0x17, 0x0B, { 0 } },
    { 0xC4, 0xD8, { 0x0A, 0x14 }, 0x19, 0x15, 0x1B, 0x00, { 0 } },
    { 0xDC, 0xD8, { 0x0A, 0x15 }, 0x19, 0x16, 0x18, 0x00, { 0 } },
    { 0xF4, 0xD8, { 0x0A, 0x16 }, 0x19, 0x17, 0x1A, 0x00, { 0 } },
    { 0x0C, 0xD8, { 0x0A, 0x17 }, 0x19, 0x18, 0x1C, 0x00, { 0 } },
    { 0x24, 0xD8, { 0x0A, 0x18 }, 0x19, 0x19, 0x1D, 0x09, { 0 } },
    { 0x3C, 0xD8, { 0x0A, 0x19 }, 0x19, 0x1A, 0x1E, 0x0A, { 0 } },
    { 0x54, 0xD8, { 0x0A, 0x1A }, 0x19, 0x1B, 0x0B, 0xFE, { 0 } },
};

GalleryItemEntry gGalleryItems_Extras[] = {
    { 0xA0, 0xB8, { 0x0B, 0x00 }, 0x1B, 0x01, 0x00, 0x00, { 0 } },
    { 0xB8, 0xB8, { 0x0B, 0x01 }, 0x1B, 0x02, 0x00, 0x00, { 0 } },
    { 0xD0, 0xB8, { 0x0B, 0x02 }, 0x1B, 0x03, 0x00, 0x00, { 0 } },
    { 0xE8, 0xB8, { 0x0B, 0x03 }, 0x1B, 0x04, 0x00, 0x00, { 0 } },
    { 0x00, 0xB8, { 0x0B, 0x04 }, 0x1B, 0x05, 0x00, 0x00, { 0 } },
    { 0x18, 0xB8, { 0x0B, 0x05 }, 0x1B, 0x06, 0x00, 0x00, { 0 } },
    { 0x30, 0xB8, { 0x0B, 0x06 }, 0x1B, 0x07, 0x00, 0x00, { 0 } },
    { 0x48, 0xB8, { 0x0B, 0x07 }, 0x1B, 0x08, 0x00, 0x00, { 0 } },
    { 0xAC, 0xD0, { 0x0B, 0x08 }, 0x1B, 0x09, 0x00, 0x00, { 0 } },
    { 0xC4, 0xD0, { 0x0B, 0x09 }, 0x1B, 0x0A, 0x00, 0x00, { 0 } },
    { 0xDC, 0xD0, { 0x0B, 0x0A }, 0x1B, 0x0B, 0x00, 0x00, { 0 } },
    { 0xF4, 0xD0, { 0x0B, 0x0B }, 0x1B, 0x0C, 0x00, 0x00, { 0 } },
    { 0x0C, 0xD0, { 0x0B, 0x0C }, 0x1B, 0x0D, 0x00, 0x00, { 0 } },
    { 0x24, 0xD0, { 0x0B, 0x0D }, 0x1B, 0x0E, 0x00, 0x00, { 0 } },
    { 0x3C, 0xD0, { 0x0B, 0x0E }, 0x1B, 0x0F, 0x00, 0x00, { 0 } },
};

GalleryCategoryData gGalleryCategories[] = {
    { gGalleryItems_SaveSlots,  15 },
    { gGalleryItems_Characters, 18 },
    { gGalleryItems_Boards,     9  },
    { gGalleryItems_Prizes,     27 },
    { gGalleryItems_Extras,     15 },
};

void *gGalleryCategory_Exit_Ptr = (void *)5;

s32 gViewerStateConfig[] = { 0x00000004, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
                             0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF, 0x00000030,
                             0x00000000, 0x01000000, 0x00000001, 0x00000000, 0xFFFFFFFF };

/* Local types for renderGalleryMenu */
typedef struct {
    u16 x;
    u16 y;
    u8 pad4;
    u8 texFlags;
    s8 textRow;
    u8 labelIndex;
} MenuLayoutEntry;

typedef struct {
    u16 posX;
    u16 posY;
    s32 spriteData;
    s16 frameIndex;
    s16 shadeA;
    s16 shadeB;
    s16 zValue;
    s16 alpha;
    s8 pad12;
    s8 texFlags;
    u8 alphaByte;
    u8 _pad15[3];
} GalleryRenderSlot;

typedef struct {
    s16 m[3][3];
    u8 _pad12[2];
    s32 tx;
    s32 ty;
    s32 tz;
} Transform3D_local;

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
    s16 unkA;
    s8 unkC;
    s8 unkD;
    u8 _padE[2];
} GalleryLabelSlot;

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
    u8 _padA[2];
} GallerySmallSlot;

typedef struct {
    /* 0x000 */ s8 menuState;
    /* 0x001 */ s8 selectedOption;
    /* 0x002 */ s8 menuType;
    /* 0x003 */ s8 viewerComplete;
    /* 0x004 */ u8 pad4[4];
    /* 0x008 */ void *textRenderer;
    /* 0x00C */ void *textTable;
    /* 0x010 */ s16 bgmFadeTimer;
    /* 0x012 */ s16 fadeTimer;
    /* 0x014 */ s16 animTimer;
    /* 0x016 */ u8 isExiting;
    /* 0x017 */ u8 pad17;
    /* 0x018 */ void *spriteAsset;
    /* 0x01C */ u8 pad1C[4];
    /* 0x020 */ ViewportNode overlayViewport;
    /* 0x1F8 */ ViewportNode fadeNode;
    /* 0x3D0 */ ViewportNode menuViewport;
    /* 0x5A8 */ ColorData lightColors[3];
    /* 0x5C0 */ u8 ambientColor[4];
    /* 0x5C4 */ void *tiledTextureData1;
    /* 0x5C8 */ u8 pad5C8[0x2C];
    /* 0x5F4 */ void *tiledTextureData2;
    /* 0x5F8 */ u8 pad5F8[0x2C];
    /* 0x624 */ SceneModel *menuModel;
    /* 0x628 */ GalleryRenderSlot iconSlots[6];
    /* 0x6B8 */ GalleryRenderSlot labelSlots[6];
    /* 0x748 */ s32 alphaValues[6];
    /* 0x760 */ u8 animFrames[6];
    /* 0x766 */ s8 animTimers[6];
    /* 0x76C */ GalleryLabelSlot categoryLabels[27];
    /* 0x91C */ GalleryLabelSlot categoryLabels2[27];
    /* 0xACC */ GalleryLabelSlot pageLabels[5];
    /* 0xB1C */ GalleryLabelSlot fixedSlotA;
    /* 0xB2C */ GalleryLabelSlot fixedSlotB;
    /* 0xB3C */ GallerySmallSlot fixedSlotC;
    /* 0xB48 */ GallerySmallSlot fixedSlotD;
    /* 0xB54 */ u8 padB54[0x144];
    /* 0xC98 */ s16 unkC98;
    /* 0xC9A */ s16 unkC9A;
    /* 0xC9C */ s16 unkC9C;
    /* 0xC9E */ u8 padC9E[2];
    /* 0xCA0 */ void *unkCA0;
    /* 0xCA4 */ u8 unkCA4[4];
} GalleryMenuState;

void playBgmTrack(GalleryMenuState *arg0, s16 bgmId) {
    gCurrentBgmId[0] = bgmId;
    arg0->bgmFadeTimer = 0x28;
    playMusicTrackWithFadeIn(bgmId, 0x80, 0);
}

void beginMenuFadeOut(GalleryMenuState *arg0) {
    setViewportFadeValue(&arg0->fadeNode, 0xFF, 10);
    arg0->fadeTimer = 10;
}

void beginMenuFadeIn(GalleryMenuState *arg0) {
    setViewportFadeValue(&arg0->fadeNode, 0, 10);
    arg0->fadeTimer = 10;
}

s32 getMenuOptionCount(GalleryMenuState *arg0) {
    s8 menuType = arg0->menuType;

    if (menuType < 2) {
        return gMenuOptionConfig[menuType].count;
    }
    return 0;
}

void *getMenuOptionEntry(GalleryMenuState *arg0, s32 index) {
    void *result = NULL;
    s8 menuType = arg0->menuType;

    if (menuType < 2) {
        MenuOptionConfig *config = &gMenuOptionConfig[menuType];
        if (index < config->count) {
            result = (u8 *)config->layout + index * 8;
        }
    }
    return result;
}

void setMenuAnimation(GalleryMenuState *arg0, s16 animIndex, s16 transitionAnimIndex, s8 loopCount, s16 animTimer) {
    setModelAnimationLooped(arg0->menuModel, animIndex, transitionAnimIndex, loopCount);
    arg0->animTimer = animTimer;
}

void renderGalleryMenu(GalleryMenuState *arg0) {
    Transform3D_local viewportTransform;
    Transform3D_local menuTransform;
    int new_var;
    s32 i;
    u16 *textEntry;
    MenuLayoutEntry *entry;
    s32 halfWidth;
    s16 alpha;
    s8 menuState;

    memcpy(&menuTransform, &identityMatrix, 0x20);
    if (arg0->menuState == 7) {
        return;
    }
    if (arg0->bgmFadeTimer == 0) {
        if (arg0->selectedOption != 5) {
            playBgmTrack(arg0, *gCurrentBgmId);
        }
    } else {
        arg0->bgmFadeTimer = arg0->bgmFadeTimer - 1;
    }
    createViewportTransform(&viewportTransform, 0, 0, *gViewerInitPosition, 0, 0, 0);
    setViewportTransformById(arg0->menuViewport.id, &viewportTransform);
    createYRotationMatrix(
        (Transform3D *)&menuTransform,
        (computeAngleToPosition(gViewerDefaultPosX, gViewerDefaultPosZ, viewportTransform.tx, viewportTransform.tz) +
         0x1000) &
            0xFFFF
    );
    menuTransform.tx = gViewerDefaultPosX;
    menuTransform.ty = gViewerDefaultPosY;
    menuTransform.tz = gViewerDefaultPosZ;
    applyTransformToModel(arg0->menuModel, (Transform3D *)&menuTransform);
    enableEntityRendering(arg0->menuModel);
    setModelHeight(arg0->menuModel, menuTransform.ty);
    clearModelRotation(arg0->menuModel);
    updateModelGeometry(arg0->menuModel);
    debugEnqueueCallback(1, 4, renderTiledTexture, arg0->pad5C8);
    menuState = arg0->menuState;
    if (menuState <= 0) {
        goto menu_end;
    }
    if (menuState < 4) {
        goto menu_body;
    }
    if (menuState >= 7) {
        goto menu_end;
    }
    if (menuState < 5) {
        goto menu_end;
    }
menu_body: {
    __asm__("");
    textEntry = getTable2DEntry(arg0->textTable, 0, 0);
}
    halfWidth = getMaxLinePixelWidth(textEntry) / 2;
    halfWidth = -halfWidth;
    func_80035260_35E60(arg0->textRenderer, textEntry, halfWidth, -0x48, 0xFF, arg0->pad4[1], 5, 2, 0);
    entry = getMenuOptionEntry(arg0, arg0->selectedOption);
    textEntry = getTable2DEntry(arg0->textTable, entry->textRow, 0);
    halfWidth = (new_var = getMaxLinePixelWidth(textEntry) / 2);
    halfWidth = -halfWidth;
    func_80035260_35E60(arg0->textRenderer, textEntry, halfWidth, 0x30, 0xFF, arg0->pad4[1], 5, 2, 0);
    for (i = 0; i < getMenuOptionCount(arg0); i++) {
        entry = getMenuOptionEntry(arg0, i);
        if (arg0->animTimers[i] == 0) {
            arg0->animTimers[i] = 4;
            if (i == arg0->selectedOption) {
                arg0->animFrames[i] = arg0->animFrames[i] + 1;
            } else if (((s8)arg0->animFrames[i]) != 0) {
                arg0->animFrames[i] = (u8)(((s8)arg0->animFrames[i]) + 1);
            }
            if (((s8)arg0->animFrames[i]) >= gDefaultMenuOptionCount) {
                arg0->animFrames[i] = 0;
            }
            arg0->iconSlots[i].frameIndex = (s16)((s8)gNavigationCycleIndices[(s8)arg0->animFrames[i]]);
        } else {
            arg0->animTimers[i] = arg0->animTimers[i] - 1;
        }
        if (i == arg0->selectedOption) {
            arg0->iconSlots[i].shadeB = 0x370;
            arg0->iconSlots[i].shadeA = 0x370;
            arg0->alphaValues[i] += 0xF0000;
            if (arg0->alphaValues[i] > 0xFF0000) {
                arg0->alphaValues[i] = 0xFF0000;
            }
        } else {
            arg0->iconSlots[i].shadeB = 0x400;
            arg0->iconSlots[i].shadeA = 0x400;
            arg0->alphaValues[i] += (s32)0xFFF10000;
            if (arg0->alphaValues[i] <= ((s32)0x95FFFF)) {
                arg0->alphaValues[i] = 0x960000;
            }
        }
        arg0->iconSlots[i].posX = entry->x;
        arg0->iconSlots[i].posY = entry->y;
        arg0->iconSlots[i].texFlags = entry->texFlags;
        alpha = (s16)(arg0->alphaValues[i] >> 16);
        arg0->iconSlots[i].alphaByte = (s8)alpha;
        arg0->iconSlots[i].alpha = alpha & 0xFF;
        debugEnqueueCallback(2, 4, func_80011924_12524, &arg0->iconSlots[i]);
        arg0->labelSlots[i].posX = entry->x;
        arg0->labelSlots[i].posY = entry->y;
        arg0->labelSlots[i].texFlags = 0;
        alpha = (s16)(arg0->alphaValues[i] >> 16);
        arg0->labelSlots[i].alphaByte = (s8)alpha;
        arg0->labelSlots[i].alpha = alpha & 0xFF;
        arg0->labelSlots[i].frameIndex = (s16)((s8)entry->labelIndex);
        debugEnqueueCallback(2, 5, func_80011924_12524, &arg0->labelSlots[i]);
    }
menu_end:;
    ;
    ;
}

void cleanupGalleryMenu(GalleryMenuState *arg0) {
    destroySceneModel(arg0->menuModel);
    arg0->spriteAsset = freeNodeMemory(arg0->spriteAsset);
    arg0->textTable = freeNodeMemory(arg0->textTable);
    arg0->textRenderer = freeNodeMemory(arg0->textRenderer);
    arg0->tiledTextureData1 = freeNodeMemory(arg0->tiledTextureData1);
    arg0->tiledTextureData2 = freeNodeMemory(arg0->tiledTextureData2);
    unlinkNode(&arg0->menuViewport);
    unlinkNode(&arg0->overlayViewport);
    unlinkNode(&arg0->fadeNode);
}

void activateGalleryMenu(GalleryMenuState *arg0) {
    initTiledTextureRenderState(&arg0->pad5C8, (s32)arg0->tiledTextureData1);
    initTiledTextureRenderState(&arg0->pad5F8, (s32)arg0->tiledTextureData2);
    setViewportFadeValue(NULL, 0, 10);
    setMenuAnimation(arg0, 0x90, 0x90, -1, 0);
    arg0->fadeTimer = 10;
    arg0->menuState = 2;
}

void waitForMenuFadeIn(GalleryMenuState *arg0) {
    s16 temp = arg0->fadeTimer;

    if (temp == 0) {
        if (arg0->menuModel->unk16 == 0x15B) {
            setMenuAnimation(arg0, 0x15C, 0x90, 1, -1);
        }
        arg0->menuState = 2;
    } else {
        arg0->fadeTimer = temp - 1;
    }
}

extern s32 gControllerInputs;

void handleGalleryMenuInput(GalleryMenuState *arg0) {
    s32 inputs;
    u8 selection;
    s8 sel_signed;
    s32 sound;
    s32 sel2;

    inputs = gControllerInputs;
    selection = arg0->selectedOption;

    // B button - exit
    if (inputs & B_BUTTON) {
        arg0->menuState = 5;
        return;
    }

    // A button - confirm
    if (inputs & A_BUTTON) {
        beginMenuFadeOut(arg0);
        if (arg0->selectedOption == 5) {
            setMusicFadeOut(10);
            setViewportFadeValue(0, 0xFF, 10);
        }
        setMenuAnimation(arg0, 0x15A, 0x15B, 1, 0x14);
        sound = 0xD3;
        arg0->menuState = 3;
        goto play_sound;
    }

    // Left / D-pad left (0x80200)
    if (inputs & 0x80200) {
        sel_signed = (s8)selection;
        if (sel_signed <= 0) {
            goto check_bounds;
        }
        if (sel_signed < 3) {
            arg0->selectedOption = selection - 1;
            goto check_bounds;
        }
        if (sel_signed >= 6) {
            goto check_bounds;
        }
        if (sel_signed < 4) {
            goto check_bounds;
        }
        arg0->selectedOption = selection - 1;
        goto check_bounds;
    }

    // Right / D-pad right (0x40100)
    if (inputs & 0x40100) {
        sel_signed = (s8)selection;
        if (sel_signed < 0) {
            goto set_zero;
        }
        if (sel_signed < 2) {
            arg0->selectedOption = selection + 1;
            goto check_bounds;
        }
        if (sel_signed >= 5) {
            goto check_bounds;
        }
        if (sel_signed < 3) {
            goto check_bounds;
        }
        arg0->selectedOption = selection + 1;
        goto check_bounds;
    }

    // Up / D-pad up (0x10800)
    if (inputs & 0x10800) {
        sel_signed = (s8)selection;
        if (sel_signed >= 6) {
            goto check_bounds;
        }
        if (sel_signed < 3) {
            goto check_bounds;
        }
        arg0->selectedOption = selection - 2;
        if (arg0->menuType == 0) {
            goto check_bounds;
        }
        arg0->selectedOption = selection - 3;
        goto check_bounds;
    }

    // Down / D-pad down (0x20400)
    if (!(inputs & 0x20400)) {
        goto check_bounds;
    }
    sel_signed = (s8)selection;
    if (sel_signed == 0) {
        goto add_three;
    }
    if (sel_signed < 0) {
        goto check_bounds;
    }
    if (sel_signed >= 3) {
        goto check_bounds;
    }
    if (arg0->menuType != 0) {
        goto add_three;
    }
    arg0->selectedOption = selection - 1;

add_three:
    arg0->selectedOption = arg0->selectedOption + 3;

check_bounds:
    if (arg0->selectedOption >= 0) {
        goto check_max;
    }

set_zero:
    arg0->selectedOption = 0;

check_max:
    sel2 = arg0->selectedOption;
    if (getMenuOptionCount(arg0) - 1 < sel2) {
        arg0->selectedOption = getMenuOptionCount(arg0) - 1;
    }

    if (arg0->selectedOption == (s8)selection) {
        return;
    }
    sound = 0x2B;

play_sound:
    playSoundEffect(sound);
}

typedef struct FD98_struct {
    s8 viewerState;
    s8 navigationMode;
    s8 cursorIndex;
    u8 pageUpCursorDest;
    u8 pageDownCursorDest;
    u8 pad5[0x3];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
} FD98_struct;

extern void initGalleryViewer(FD98_struct *);

void processGalleryMenuSelection(GalleryMenuState *arg0) {
    s16 temp = arg0->fadeTimer;

    if (temp == 0) {
        if (arg0->selectedOption == 5) {
            cleanupGalleryMenu(arg0);
            setCutsceneSelection(0, 2);
            createTaskQueue(loadCutsceneOverlay, 0x64);
            arg0->isExiting = 1;
            arg0->menuState = 7;
        } else {
            scheduleTask(initGalleryViewer, 0, 0, 0);
            arg0->viewerComplete = 0;
            arg0->menuState = 4;
        }
    } else {
        arg0->fadeTimer = temp - 1;
    }
}

void waitForGalleryViewerExit(GalleryMenuState *arg0) {
    if (arg0->viewerComplete != 0) {
        terminateTasksByType(1);
        beginMenuFadeIn(arg0);
        arg0->menuState = 1;
    }
}

void beginGalleryMenuExit(GalleryMenuState *arg0) {
    playSoundEffect(0xDB);
    setViewportFadeValue(NULL, 0xFF, 10);
    setMenuAnimation(arg0, 0x15A, 0x15B, 1, -1);
    arg0->fadeTimer = 10;
    arg0->menuState = 6;
}

extern void onGalleryMenuExit(void);

s32 completeGalleryMenuExit(GalleryMenuState *arg0) {
    s16 temp;

    if (arg0->fadeTimer == 8) {
        setMusicFadeOut(8);
    }
    temp = arg0->fadeTimer;
    if (temp == 0) {
        cleanupGalleryMenu(arg0);
        terminateSchedulerWithCallback(onGalleryMenuExit);
        return 1;
    }
    arg0->fadeTimer = temp - 1;
    return 0;
}

void setupGalleryMenuState(void);
void updateGalleryMenu(void);

void initGalleryMenu(void) {
    allocateTaskMemory(0xCC0);
    gCurrentBgmId[0] = 5;
    setupTaskSchedulerNodes(0x40, 4, 0, 0, 0, 0, 0, 0);
    playSoundEffect(0xD0);
    setGameStateHandler(setupGalleryMenuState);
}

void setupGalleryMenuState(void) {
    Transform3D viewportTransform;
    GalleryMenuState *alloc;
    s32 i;

    alloc = getCurrentAllocation();
    alloc->menuState = 0;
    alloc->selectedOption = 0;
    alloc->menuType = isCreditsUnlocked();
    *(s32 *)alloc->pad4 = 0xFF0000;
    alloc->fadeTimer = 0;
    alloc->viewerComplete = 0;
    alloc->animTimer = -1;
    alloc->isExiting = 0;
    alloc->tiledTextureData1 = loadCompressedData(&_67FB80_ROM_START, &_67FB80_ROM_END, 0xC010);
    alloc->textRenderer = loadTextRenderAsset(1);
    alloc->textTable = loadDmaAsset(4);
    alloc->spriteAsset = loadSpriteAssetData(8);
    alloc->tiledTextureData2 = loadCompressedData(&_684630_ROM_START, &_6848B0_ROM_START, 0xD00);
    for (i = 0; i < 6; i++) {
        alloc->iconSlots[i].posX = 0;
        alloc->iconSlots[i].posY = 0;
        alloc->iconSlots[i].spriteData = (s32)alloc->spriteAsset;
        alloc->iconSlots[i].frameIndex = 0;
        alloc->iconSlots[i].shadeA = 0x400;
        alloc->iconSlots[i].shadeB = 0x400;
        alloc->iconSlots[i].zValue = 0;
        alloc->iconSlots[i].alpha = 0xFF;
        alloc->iconSlots[i].pad12 = 0;
        alloc->iconSlots[i].texFlags = 0;
        alloc->iconSlots[i].alphaByte = 0x96;
        alloc->labelSlots[i].posX = 0;
        alloc->labelSlots[i].posY = 0;
        alloc->labelSlots[i].spriteData = (s32)alloc->spriteAsset;
        alloc->labelSlots[i].frameIndex = 0;
        alloc->labelSlots[i].shadeA = 0x400;
        alloc->labelSlots[i].shadeB = 0x400;
        alloc->labelSlots[i].zValue = 0;
        alloc->labelSlots[i].alpha = 0xFF;
        alloc->labelSlots[i].pad12 = 0;
        alloc->labelSlots[i].texFlags = 0;
        alloc->labelSlots[i].alphaByte = 0x96;
        alloc->alphaValues[i] = 0x960000;
        alloc->animFrames[i] = 0;
        alloc->animTimers[i] = 4;
    }

    alloc->menuModel = createSceneModel(0x2D, &alloc->menuViewport);
    gScaleMatrix.translation.x = 0x400000;
    gScaleMatrix.translation.y = -0x400000;
    gScaleMatrix.translation.z = 0;
    applyTransformToModel(alloc->menuModel, &gScaleMatrix);
    for (i = 0; i < 27; i++) {
        alloc->categoryLabels[i].unk0 = 0;
        alloc->categoryLabels[i].unk2 = 0;
        alloc->categoryLabels[i].unk4 = alloc->spriteAsset;
        alloc->categoryLabels[i].unk8 = 0;
        alloc->categoryLabels[i].unkA = 0xFF;
        alloc->categoryLabels[i].unkC = 0;
        alloc->categoryLabels[i].unkD = 0;
        alloc->categoryLabels2[i].unk0 = 0;
        alloc->categoryLabels2[i].unk2 = 0;
        alloc->categoryLabels2[i].unk4 = alloc->spriteAsset;
        alloc->categoryLabels2[i].unk8 = 0x1A;
        alloc->categoryLabels2[i].unkA = 0xFF;
        alloc->categoryLabels2[i].unkC = 0;
        alloc->categoryLabels2[i].unkD = 0;
    }

    for (i = 0; i < 5; i++) {
        alloc->pageLabels[i].unk0 = -8 + i * 0x10;
        alloc->pageLabels[i].unk2 = 0x2C;
        alloc->pageLabels[i].unk4 = alloc->spriteAsset;
        alloc->pageLabels[i].unk8 = 0x2F;
        alloc->pageLabels[i].unkA = 0xFF;
        alloc->pageLabels[i].unkC = 0;
        alloc->pageLabels[i].unkD = 0;
    }

    alloc->fixedSlotA.unk0 = 0;
    alloc->fixedSlotA.unk2 = 0;
    alloc->fixedSlotA.unkC = 0;
    alloc->fixedSlotA.unkD = 0;
    alloc->fixedSlotB.unkC = 0;
    alloc->fixedSlotB.unkD = 0;
    alloc->fixedSlotA.unk4 = alloc->spriteAsset;
    alloc->fixedSlotB.unk4 = alloc->spriteAsset;
    alloc->fixedSlotC.unk4 = alloc->spriteAsset;
    alloc->fixedSlotD.unk4 = alloc->spriteAsset;
    alloc->fixedSlotA.unk8 = 0x0A;
    alloc->fixedSlotA.unkA = 0xFF;
    alloc->fixedSlotB.unk0 = -0x24;
    alloc->fixedSlotB.unk2 = -0x10;
    alloc->fixedSlotB.unk8 = 9;
    alloc->fixedSlotB.unkA = 0xFF;
    alloc->fixedSlotC.unk0 = 0x18;
    alloc->fixedSlotC.unk2 = 0x28;
    alloc->fixedSlotC.unk8 = 0x25;
    alloc->fixedSlotD.unk0 = 0x48;
    alloc->fixedSlotD.unk8 = 0x30;
    alloc->unkC98 = -0x68;
    alloc->unkC9A = 0x20;
    alloc->unkC9C = 1;
    alloc->unkCA0 = &alloc->unkCA4;
    alloc->fixedSlotD.unk2 = 0x28;
    initViewportNode(&alloc->menuViewport, 0, 0, 0xE, 1);
    setViewportScale(&alloc->menuViewport, 1.0f, 1.0f);
    setViewportId(&alloc->menuViewport, 1);
    setModelCameraTransform(&alloc->menuViewport, 0, 0, -0xA0, -0x78, 0x9F, 0x77);
    setViewportPerspective(&alloc->menuViewport, 40.0f, 1.3333334f, 10.0f, 10000.0f);
    createViewportTransform(&viewportTransform, 0, 0, 0x01400000, 0, 0, 0);
    setViewportTransformById(alloc->menuViewport.id, &viewportTransform);
    initViewportNode(&alloc->overlayViewport, 0, 1, 0xA, 0);
    setViewportScale(&alloc->overlayViewport, 1.0f, 1.0f);
    setViewportId(&alloc->overlayViewport, 1);
    setModelCameraTransform(&alloc->overlayViewport, 0, 0, -0xA0, -0x78, 0x9F, 0x77);
    initViewportNode(&alloc->fadeNode, 0, 2, 0xC, 0);
    setViewportScale(&alloc->fadeNode, 1.0f, 1.0f);
    setViewportId(&alloc->fadeNode, 1);
    setModelCameraTransform(&alloc->fadeNode, 0, 0, -0x6F, -0x56, 0x6E, 0x46);
    setViewportFadeValue(0, 0xFF, 0);
    setViewportEnvColor(&alloc->menuViewport, 0, 0, 0);
    setViewportFadeValue(&alloc->menuViewport, 0, 0);
    setViewportEnvColor(&alloc->overlayViewport, 0, 0, 0);
    setViewportFadeValue(&alloc->overlayViewport, 0, 0);
    setViewportEnvColor(&alloc->fadeNode, 0, 0x70, 0x80);
    setViewportFadeValue(&alloc->fadeNode, 0, 0);
    alloc->lightColors[0].r2 = 0;
    alloc->lightColors[0].g2 = 0x7F;
    alloc->lightColors[0].b2 = 0x7F;
    alloc->lightColors[0].r = 0xFF;
    alloc->lightColors[0].g = 0xFF;
    alloc->lightColors[0].b = 0xFF;
    alloc->lightColors[1].r = 0x3F;
    alloc->lightColors[1].g = 0x3F;
    alloc->lightColors[1].b = 0x3F;
    alloc->lightColors[1].r2 = 0x7F;
    alloc->lightColors[1].g2 = 0x7F;
    alloc->lightColors[1].b2 = 0;
    alloc->lightColors[2].r2 = -0x7F;
    alloc->lightColors[2].g2 = 0x7F;
    alloc->lightColors[2].b2 = 0;
    alloc->lightColors[2].r = 0x7F;
    alloc->lightColors[2].g = 0x7F;
    alloc->lightColors[2].b = 0x7F;
    alloc->ambientColor[0] = 0xD2;
    alloc->ambientColor[1] = 0xD2;
    alloc->ambientColor[2] = 0xD2;
    setViewportLightColors(alloc->menuViewport.id, 3, alloc->lightColors, (ColorData *)alloc->ambientColor);
    playBgmTrack(alloc, gCurrentBgmId[0]);
    setGameStateHandler(updateGalleryMenu);
}

void updateGalleryMenu(void) {
    GalleryMenuState *s0;

    s0 = getCurrentAllocation();
    switch (s0->menuState) {
        case 0:
            activateGalleryMenu(s0);
            break;
        case 1:
            waitForMenuFadeIn(s0);
            break;
        case 2:
            handleGalleryMenuInput(s0);
            break;
        case 3:
            processGalleryMenuSelection(s0);
            break;
        case 4:
            waitForGalleryViewerExit(s0);
            break;
        case 5:
        default:
            beginGalleryMenuExit(s0);
            break;
        case 6:
            if (completeGalleryMenuExit(s0)) {
                return;
            }
            break;
        case 7:
            if (getSchedulerReturnValue()) {
                setGameStateHandler(setupGalleryMenuState);
            }
            return;
    }
    renderGalleryMenu(s0);
}

void onGalleryMenuExit(void) {
    gCurrentBgmId[0] = 5;
    returnToParentScheduler(1);
}

const char D_8009DEB0_9EAB0[] = "S3";
const char D_8009DEB4_9EAB4[] = "S2";
const char D_8009DEB8_9EAB8[] = "S1";
const char D_8009DEBC_9EABC[] = "L3";
const char D_8009DEC0_9EAC0[] = "L2";
const char D_8009DEC4_9EAC4[] = "L1";
const char D_8009DEC8_9EAC8[] = "27";
const char D_8009DECC_9EACC[] = "26";
const char D_8009DED0_9EAD0[] = "25";
const char D_8009DED4_9EAD4[] = "24";
const char D_8009DED8_9EAD8[] = "23";
const char D_8009DEDC_9EADC[] = "22";
const char D_8009DEE0_9EAE0[] = "21";
const char D_8009DEE4_9EAE4[] = "20";
const char D_8009DEE8_9EAE8[] = "19";
const char D_8009DEEC_9EAEC[] = "18";
const char D_8009DEF0_9EAF0[] = "17";
const char D_8009DEF4_9EAF4[] = "16";
const char D_8009DEF8_9EAF8[] = "15";
const char D_8009DEFC_9EAFC[] = "14";
const char D_8009DF00_9EB00[] = "13";
const char D_8009DF04_9EB04[] = "12";
const char D_8009DF08_9EB08[] = "11";
const char D_8009DF0C_9EB0C[] = "10";
const char D_8009DF10_9EB10[] = " 9";
const char D_8009DF14_9EB14[] = " 8";
const char D_8009DF18_9EB18[] = " 7";
const char D_8009DF1C_9EB1C[] = " 6";
const char D_8009DF20_9EB20[] = " 5";
const char D_8009DF24_9EB24[] = " 4";
const char D_8009DF28_9EB28[] = " 3";
const char D_8009DF2C_9EB2C[] = " 2";
const char D_8009DF30_9EB30[] = " 1";
const char D_8009DF34_9EB34[] = " 0";

u8 isGalleryItemUnlocked(u8 itemIndex) {
    GalleryMenuState *menu;
    s8 categoryIndex;
    GalleryItemEntry *item;
    s8 unlockSlot;
    u8 itemIndexMasked;

    menu = getCurrentAllocation();
    categoryIndex = menu->selectedOption;
    itemIndexMasked = itemIndex;
    item = &gGalleryCategories[categoryIndex].items[itemIndexMasked];

    switch (categoryIndex) {
        case 1:
            return getCharacterPaletteId(itemIndex) & 0xFF;
        case 2:
            return isBoardUnlocked(itemIndex) & 0xFF;
        case 3:
            unlockSlot = item->unlockSlotIndex;
            if (unlockSlot == -1) {
                return 1;
            }
            if (unlockSlot == -2) {
                return isCreditsUnlocked() & 0xFF;
            }
            return isSaveSlotUnlocked(unlockSlot & 0xFF) & 0xFF;
        case 0:
        case 4:
            return isSaveSlotUnlocked(itemIndex) & 0xFF;
        case 5:
            return isCreditsUnlocked() & 0xFF;
    }
    return 1;
}

void renderGalleryViewerContent(ViewerState *arg0) {
    s32 *new_var2;
    GalleryAlloc *alloc;
    GalleryItemEntry *item;
    s8 new_var;
    s32 i;
    s32 configVal;
    s16 val;
    s32 stat1;
    s32 stat2;
    GalleryCategoryData *category;
    s32 sp34;
    s32 sp3C;
    s32 tableArg1;
    s32 tableArg2;
    void *tableEntry;
    alloc = getCurrentAllocation();
    category = &gGalleryCategories[alloc->selectedOption];
    if (arg0->menuState < 5) {
        if (arg0->menuState > 0) {
            arg0->unk10 += arg0->unk14;
            if (0xFF0000 < arg0->unk10) {
                arg0->unk10 = 0xFF0000;
                arg0->unk14 = (s32)0xFFF10000;
            } else if (arg0->unk10 <= 0x77FFFF) {
                arg0->unk10 = 0x780000;
                arg0->unk14 = 0xF0000;
            }
            arg0->unk8 += arg0->unkC;
            if (0xFF0000 < arg0->unk8) {
                arg0->unk8 = 0xFF0000;
                arg0->unkC = (s32)0xFFF10000;
            } else if (arg0->unk8 <= 0x77FFFF) {
                arg0->unk8 = 0x780000;
                arg0->unkC = 0xF0000;
            }
            if (arg0->selectedOption == 0) {
                arg0->unk14 = (s32)0xFFF10000;
            } else {
                arg0->unkC = (s32)0xFFF10000;
            }
            if (alloc->selectedOption == 3) {
                sp34 = 7;
                sp3C = 7;
            } else {
                sp34 = 7;
                sp3C = 0xF;
            }
            for (i = 0; i < category->count; i++) {
                item = &category->items[i];
                if (item->labelIndex != (-1)) {
                    new_var2 = gViewerStateConfig;
                    configVal = *new_var2;
                    alloc->labelEntries[i].x = ((s8)item->x) + sp34;
                    new_var = (s8)item->y;
                    alloc->labelEntries[i].config = configVal;
                    alloc->labelEntries[i].y = new_var + sp3C;
                    alloc->labelEntries[i].label = (s32)gGalleryLabelPtrs[item->labelIndex];
                    if (isGalleryItemUnlocked(i)) {
                        alloc->labelEntries[i].alpha = 0xFF;
                    } else {
                        alloc->labelEntries[i].alpha = 0x64;
                    }
                    debugEnqueueCallback(2, 4, renderTextColored, &alloc->labelEntries[i]);
                }
            }

            item = &category->items[arg0->menuType];
            alloc->unkB26 = arg0->unk8 >> 16;
            alloc->unkB1C = (s8)item->x;
            alloc->unkB1E = (s8)item->y;
            if (alloc->selectedOption == 3) {
                alloc->unkB24 = 0xB;
            } else {
                alloc->unkB24 = 0xA;
            }
            if (arg0->selectedOption == 0) {
                debugEnqueueCallback(2, 4, renderTextSprite, &alloc->unkB1C);
            }
            if (alloc->selectedOption == 2) {
                sp3C = 4;
                sp34 = 4;
            } else {
                sp3C = 0;
                sp34 = 0;
            }
            for (i = 0; i < category->count; i++) {
                item = &category->items[i];
                alloc->spriteSlots[i].x = ((s8)item->x) + sp34;
                alloc->spriteSlots[i].y = ((s8)item->y) + sp3C;
                alloc->spriteSlots[i].frameIndex = (s8)item->spriteFrame;
                if (isGalleryItemUnlocked(i & 0xFF) & 0xFF) {
                    alloc->spriteSlots[i].shade = 0xFF;
                    alloc->spriteSlots[i].transparency = 0;
                } else if (alloc->selectedOption == 2) {
                    alloc->spriteSlots[i].transparency = 0x14;
                    alloc->spriteSlots[i].shade = 0x96;
                } else {
                    alloc->spriteSlots[i].transparency = 0;
                    alloc->spriteSlots[i].shade = 0x64;
                }
                debugEnqueueCallback(2, 4, renderTextSprite, &alloc->spriteSlots[i]);
                if (alloc->selectedOption == 2) {
                    alloc->extraSpriteSlots[i].x = (s8)item->x;
                    alloc->extraSpriteSlots[i].y = (s8)item->y;
                    if (isGalleryItemUnlocked(i & 0xFF) & 0xFF) {
                        alloc->extraSpriteSlots[i].alpha = 0xFF;
                    } else {
                        alloc->extraSpriteSlots[i].alpha = 0x96;
                    }
                    debugEnqueueCallback(2, 4, renderTextSprite, &alloc->extraSpriteSlots[i]);
                }
            }

            alloc->unkB36 = (s16)(arg0->unk10 >> 16);
            debugEnqueueCallback(2, 4, renderTextSprite, alloc->unkB2C);
            item = &category->items[arg0->menuType];
            if (arg0->selectedOption == 0) {
                if (isGalleryItemUnlocked(arg0->menuType & 0xFF) & 0xFF) {
                    switch (alloc->selectedOption) {
                        case 0:
                            for (i = 0; i < item->numExtra; i++) {
                                debugEnqueueCallback(2, 4, renderTextSprite, &alloc->unkACC[i * 0x10]);
                            }

                            break;

                        case 1:
                            stat1 = getItemStat1((u8)arg0->menuType);
                            stat2 = getItemStat2((u8)arg0->menuType);
                            sprintf(
                                (char *)alloc->unkCA4,
                                "SPEED=%2d TURN=%2d JUMP=%2d",
                                stat1 & 0xFF,
                                stat2,
                                getItemStat3((s32)((u8)arg0->menuType)) & 0xFF
                            );
                            debugEnqueueCallback(2, 4, renderTextPalette, alloc->unkC98);
                            break;

                        case 3:
                            if (D_8009ADE0_9B9E0 & 8) {
                                alloc->unkB50 = 0x30;
                            } else {
                                alloc->unkB50 = 0x31;
                            }
                            debugEnqueueCallback(2, 4, renderSpriteFrame, alloc->unkB48);
                            debugEnqueueCallback(2, 4, renderSpriteFrame, alloc->unkB3C);
                            break;
                    }

                    tableArg1 = item->pad[0];
                    tableArg2 = item->pad[1];
                } else {
                    tableArg1 = 0xC;
                    tableArg2 = 0;
                }
                func_80035260_35E60(
                    alloc->unk8,
                    getTable2DEntry(alloc->unkC, tableArg1, tableArg2),
                    -0x68,
                    0xC,
                    0xFF,
                    0xFF,
                    5,
                    2,
                    3
                );
            }
            debugEnqueueCallback(2, 3, renderTiledTexture, alloc->unk5F8);
        }
    }
}

void startViewerFadeIn(GalleryMenuState *arg0) {
    void *alloc = getCurrentAllocation();
    arg0->menuState = 1;
    beginMenuFadeIn(alloc);
}

void waitForViewerFadeIn(GalleryMenuState *arg0) {
    GalleryMenuState *alloc = getCurrentAllocation();
    s16 temp;

    temp = alloc->fadeTimer;
    if (temp == 0) {
        if (alloc->menuModel->unk16 == 0x15B) {
            setMenuAnimation(alloc, 0x15C, 0x90, 1, -1);
        }
        arg0->menuState = 2;
    } else {
        alloc->fadeTimer = temp - 1;
    }
}

void handleViewerGridNavigation_TwoPage(FD98_struct *arg0) {
    s32 inputs;
    s8 temp;
    u8 val;

    inputs = gControllerInputs;

    if (inputs & 0x80200) {
        if (arg0->cursorIndex == 0) {
            arg0->cursorIndex = 14;
        } else {
            arg0->cursorIndex = arg0->cursorIndex - 1;
        }
        playSoundEffect(0x2B);
        return;
    }

    if (inputs & 0x40100) {
        if (arg0->cursorIndex == 14) {
            arg0->cursorIndex = 0;
        } else {
            arg0->cursorIndex = arg0->cursorIndex + 1;
        }
        playSoundEffect(0x2B);
        return;
    }

    if (inputs & 0x10800) {
        val = arg0->cursorIndex;
        if ((u8)(val - 8) < 7u) {
            arg0->cursorIndex = val - 7;
        } else {
            if (val == 0) {
                arg0->pageUpCursorDest = val + 8;
                arg0->pageDownCursorDest = arg0->cursorIndex;
            } else {
                arg0->pageUpCursorDest = val + 7;
                arg0->pageDownCursorDest = arg0->cursorIndex;
            }
            arg0->navigationMode = 1;
        }
        playSoundEffect(0x2B);
        return;
    }

    if (inputs & 0x20400) {
        temp = arg0->cursorIndex;
        if (temp == 0) {
            arg0->cursorIndex = 8;
        } else if ((u8)(temp - 1) < 7u) {
            arg0->cursorIndex = temp + 7;
        } else {
            arg0->pageUpCursorDest = temp;
            arg0->navigationMode = 1;
            arg0->pageDownCursorDest = arg0->cursorIndex - 7;
        }
        playSoundEffect(0x2B);
    }
}

void handleViewerGridNavigation_TwoColumn(FD98_struct *arg0) {
    s32 inputs;
    s8 temp2;
    s8 newVal;

    inputs = gControllerInputs;

    if (inputs & 0x80200) {
        s8 temp = arg0->cursorIndex;
        if (!temp) {
            newVal = 8;
        } else if (temp == 9) {
            newVal = 17;
        } else {
            newVal = temp - 1;
        }
        arg0->cursorIndex = newVal;
        playSoundEffect(0x2B);
        return;
    }

    if (inputs & 0x40100) {
        s8 temp = arg0->cursorIndex;
        if (temp == 8) {
            arg0->cursorIndex = 0;
        } else if (temp == 17) {
            arg0->cursorIndex = 9;
        } else {
            arg0->cursorIndex = temp + 1;
        }
        playSoundEffect(0x2B);
        return;
    }

    if (inputs & 0x10800) {
        u8 val = arg0->cursorIndex;
        temp2 = val - 9;
        if ((u8)temp2 < 9u) {
            arg0->cursorIndex = temp2;
        } else {
            arg0->pageUpCursorDest = val + 9;
            arg0->navigationMode = 1;
            arg0->pageDownCursorDest = arg0->cursorIndex;
        }
        playSoundEffect(0x2B);
        return;
    }

    if (inputs & 0x20400) {
        u8 val = arg0->cursorIndex;
        if (val < 9u) {
            arg0->cursorIndex = val + 9;
        } else {
            arg0->pageUpCursorDest = val;
            arg0->navigationMode = 1;
            arg0->pageDownCursorDest = arg0->cursorIndex - 9;
        }
        playSoundEffect(0x2B);
    }
}

void handleViewerGridNavigation_SingleRow(FD98_struct *arg0) {
    s32 inputs;

    inputs = gControllerInputs;

    if (inputs & 0x80200) {
        if (arg0->cursorIndex == 0) {
            arg0->cursorIndex = 8;
        } else {
            arg0->cursorIndex = arg0->cursorIndex - 1;
        }
        playSoundEffect(0x2B);
        return;
    }

    if (inputs & 0x40100) {
        if (arg0->cursorIndex == 8) {
            arg0->cursorIndex = 0;
        } else {
            arg0->cursorIndex = arg0->cursorIndex + 1;
        }
        playSoundEffect(0x2B);
        return;
    }

    if (inputs & 0x10800) {
        goto set_values;
    }
    if (inputs & 0x20400) {
    set_values:
        arg0->pageUpCursorDest = arg0->cursorIndex;
        arg0->pageDownCursorDest = arg0->cursorIndex;
        arg0->navigationMode = 1;
        playSoundEffect(0x2B);
    }
}

void handleViewerGridNavigation_ThreeRow(FD98_struct *arg0) {
    u8 sp30[0xC];
    GalleryMenuState *alloc;
    GalleryItemEntry *entry;
    u8 *sp30_ptr;

    alloc = getCurrentAllocation();
    sp30_ptr = sp30;
    entry = &gGalleryCategories[alloc->selectedOption].items[arg0->cursorIndex];
    memcpy(sp30_ptr, D_8009DF6C_9EB6C, 0xC);

    if (gControllerInputs & 0x8000) {
        if (isGalleryItemUnlocked(arg0->cursorIndex)) {
            playBgmTrack(alloc, entry->numExtra);
            if (alloc->menuModel->unk16 != 0x92) {
                spawnSpriteEffectInternal(
                    alloc->menuModel,
                    0,
                    9,
                    -1,
                    (cutsceneEffectDisp_exec_arg *)sp30_ptr,
                    0xCCCC,
                    0,
                    1,
                    0,
                    0,
                    1
                );
            }
            setMenuAnimation(alloc, 0x92, 0x92, -1, 0);
            return;
        }
        playSoundEffect(9);
        return;
    }

    if (gControllerInputs & 0x80200) {
        s8 temp_v0 = arg0->cursorIndex;
        s8 temp_v1 = temp_v0;
        if (temp_v0 == 0) {
            arg0->cursorIndex = 0x1A;
        } else {
            arg0->cursorIndex = temp_v1 - 1;
        }
        playSoundEffect(0x2B);
        return;
    }

    if (gControllerInputs & 0x40100) {
        s8 temp = arg0->cursorIndex;
        if (temp == 0x1A) {
            arg0->cursorIndex = 0;
        } else {
            arg0->cursorIndex = temp + 1;
        }
        playSoundEffect(0x2B);
        return;
    }

    if (gControllerInputs & 0x10800) {
        u8 val = arg0->cursorIndex;
        s8 temp = val - 9;
        if ((u8)temp < 0x12u) {
            arg0->cursorIndex = temp;
        } else {
            arg0->pageUpCursorDest = val + 0x12;
            arg0->navigationMode = 1;
            arg0->pageDownCursorDest = arg0->cursorIndex;
        }
        playSoundEffect(0x2B);
        return;
    }

    if (gControllerInputs & 0x20400) {
        u8 val = arg0->cursorIndex;
        if (val < 0x12u) {
            arg0->cursorIndex = val + 9;
        } else {
            arg0->pageUpCursorDest = val;
            arg0->navigationMode = 1;
            arg0->pageDownCursorDest = arg0->cursorIndex - 0x12;
        }
        playSoundEffect(0x2B);
    }
}

void handleViewerGridNavigation_Prize(FD98_struct *arg0) {
    s32 inputs;
    s8 temp;
    u8 val;

    inputs = gControllerInputs;

    if (inputs & 0x80200) {
        if (arg0->cursorIndex == 0) {
            arg0->cursorIndex = 14;
        } else {
            arg0->cursorIndex = arg0->cursorIndex - 1;
        }
        playSoundEffect(0x2B);
        return;
    }

    if (inputs & 0x40100) {
        if (arg0->cursorIndex == 14) {
            arg0->cursorIndex = 0;
        } else {
            arg0->cursorIndex = arg0->cursorIndex + 1;
        }
        playSoundEffect(0x2B);
        return;
    }

    if (inputs & 0x10800) {
        val = arg0->cursorIndex;
        if ((u8)(val - 8) < 7u) {
            arg0->cursorIndex = val - 7;
        } else {
            if (val == 0) {
                arg0->pageUpCursorDest = val + 8;
                arg0->pageDownCursorDest = arg0->cursorIndex;
            } else {
                arg0->pageUpCursorDest = val + 7;
                arg0->pageDownCursorDest = arg0->cursorIndex;
            }
            arg0->navigationMode = 1;
        }
        playSoundEffect(0x2B);
        return;
    }

    if (inputs & 0x20400) {
        temp = arg0->cursorIndex;
        if (temp == 0) {
            arg0->cursorIndex = 8;
        } else if ((u8)(temp - 1) < 7u) {
            arg0->cursorIndex = temp + 7;
        } else {
            arg0->pageUpCursorDest = temp;
            arg0->navigationMode = 1;
            arg0->pageDownCursorDest = arg0->cursorIndex - 7;
        }
        playSoundEffect(0x2B);
    }
}

INCLUDE_RODATA("asm/nonmatchings/ui/gallery", D_8009DF6C_9EB6C);

void handleViewerInput(FD98_struct *arg0) {
    FD98_struct *alloc;
    s32 inputs;
    s8 temp;
    u8 newVal;
    s32 sound;

    alloc = getCurrentAllocation();
    inputs = gControllerInputs;

    if (inputs & B_BUTTON) {
        setMenuAnimation((GalleryMenuState *)alloc, 0x90, 0x90, -1, -1);
        sound = 0x2E;
        arg0->viewerState = 3;
        goto play_sound;
    }

    temp = arg0->navigationMode;
    if (temp == 0) {
        goto do_switch;
    }
    if (temp == 1) {
        goto button_check;
    }
    return;

do_switch:
    switch (alloc->navigationMode) {
        case 0:
            handleViewerGridNavigation_TwoPage(arg0);
            break;
        case 1:
            handleViewerGridNavigation_TwoColumn(arg0);
            break;
        case 2:
            handleViewerGridNavigation_SingleRow(arg0);
            break;
        case 3:
            handleViewerGridNavigation_ThreeRow(arg0);
            break;
        case 4:
            handleViewerGridNavigation_Prize(arg0);
            break;
    }
    return;

button_check:
    if (inputs & A_BUTTON) {
        arg0->viewerState = 3;
        setMenuAnimation((GalleryMenuState *)alloc, 0x90, 0x90, -1, -1);
        sound = 0x2E;
        goto play_sound;
    }
    if (inputs & 0x10800) {
        newVal = arg0->pageUpCursorDest;
        goto set_2b;
    }
    if (inputs & 0x20400) {
        newVal = arg0->pageDownCursorDest;
    set_2b:
        sound = 0x2B;
        arg0->navigationMode = 0;
        arg0->cursorIndex = newVal;
    play_sound:
        playSoundEffect(sound);
    }
}

void startViewerFadeOut(GalleryMenuState *arg0) {
    void *alloc = getCurrentAllocation();
    beginMenuFadeOut(alloc);
    arg0->menuState = 4;
}

s32 updateViewerFadeOut(GalleryMenuState *arg0) {
    GalleryMenuState *alloc = getCurrentAllocation();
    s16 temp = alloc->fadeTimer;

    if (temp == 0) {
        terminateCurrentTask();
        return 1;
    }
    alloc->fadeTimer = temp - 1;
    return 0;
}

extern void onGalleryViewerCleanup(void);
extern void updateGalleryViewer(GalleryMenuState *);

void initGalleryViewer(FD98_struct *arg0) {
    getCurrentAllocation();
    setCleanupCallback(onGalleryViewerCleanup);
    arg0->unk10 = 0x780000;
    arg0->unk14 = 0x0F0000;
    arg0->unk8 = 0xFF0000;
    arg0->viewerState = 0;
    arg0->navigationMode = 0;
    arg0->cursorIndex = 0;
    arg0->unkC = 0xFFF10000;
    setCallback(updateGalleryViewer);
}

void updateGalleryViewer(GalleryMenuState *arg0) {
    getCurrentAllocation();
    switch (arg0->menuState) {
        case 0:
            startViewerFadeIn(arg0);
            break;
        case 1:
            waitForViewerFadeIn(arg0);
            break;
        case 2:
            handleViewerInput((FD98_struct *)arg0);
            break;
        case 3:
            startViewerFadeOut(arg0);
            break;
        case 4:
            if (updateViewerFadeOut(arg0)) {
                return;
            }
            break;
    }
    renderGalleryViewerContent((ViewerState *)arg0);
}

void onGalleryViewerCleanup(void) {
    GalleryMenuState *alloc = getCurrentAllocation();
    alloc->viewerComplete = 1;
}
