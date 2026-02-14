#include "20F0.h"
#include "38C90.h"
#include "4050.h"
#include "B040.h"
#include "audio.h"
#include "common.h"
#include "graphics.h"
#include "race_session.h"
#include "task_scheduler.h"

extern u8 D_8009DF6C_9EB6C[];

typedef struct {
    s32 value;
    void *ptr;
} ArrayElement_8;

typedef struct {
    void *ptr;
    s32 count;
} LookupEntry;

typedef struct {
    u8 unk0;
    u8 unk1;
    s8 pad[5];
    s8 unlockSlotIndex;
    u8 pad2[4];
} GalleryItemEntry;

typedef struct {
    GalleryItemEntry *items;
    s32 count;
} GalleryCategoryData;

/* Data section definitions */

s16 gCurrentBgmId[2] = { 5, 0 };

s32 gMenuOptionLayout_Type0[] = { 0xFFD0FFE8, 0x00010103, 0x0000FFE8, 0x01020204, 0x0030FFE8,
                                  0x02040306, 0xFFE80010, 0x03030405, 0x00180010, 0x04050507 };

s32 gMenuOptionLayout_Type1[] = { 0xFFD0FFE8, 0x00010103, 0x0000FFE8, 0x01020204, 0x0030FFE8, 0x02040306,
                                  0xFFD00010, 0x03030405, 0x00000010, 0x04050507, 0x00300010, 0x00060608 };

void *gMenuOptionLookupPtr = gMenuOptionLayout_Type0;

s32 gMenuOptionCounts[] = { 5, (s32)gMenuOptionLayout_Type1, 6 };

u8 gNavigationCycleIndices[] = { 0x00, 0x01, 0x02, 0x01 };

s32 gDefaultMenuOptionCount = 4;
s32 gViewerDefaultPosX = 0x00285000;
s32 gViewerDefaultPosY = 0xFFDE3000;
s32 gViewerDefaultPosZ = 0x003A7000;

s32 gViewerInitPosition[] = { 0x00A92000, 0, 0, 0 };

extern s32 D_8009DEB0_9EAB0;
extern s32 D_8009DEB4_9EAB4;
extern s32 D_8009DEB8_9EAB8;
extern s32 D_8009DEBC_9EABC;
extern s32 D_8009DEC0_9EAC0;
extern s32 D_8009DEC4_9EAC4;
extern s32 D_8009DEC8_9EAC8;
extern s32 D_8009DECC_9EACC;
extern s32 D_8009DED0_9EAD0;
extern s32 D_8009DED4_9EAD4;
extern s32 D_8009DED8_9EAD8;
extern s32 D_8009DEDC_9EADC;
extern s32 D_8009DEE0_9EAE0;
extern s32 D_8009DEE4_9EAE4;
extern s32 D_8009DEE8_9EAE8;
extern s32 D_8009DEEC_9EAEC;
extern s32 D_8009DEF0_9EAF0;
extern s32 D_8009DEF4_9EAF4;
extern s32 D_8009DEF8_9EAF8;
extern s32 D_8009DEFC_9EAFC;
extern s32 D_8009DF00_9EB00;
extern s32 D_8009DF04_9EB04;
extern s32 D_8009DF08_9EB08;
extern s32 D_8009DF0C_9EB0C;
extern s32 D_8009DF10_9EB10;
extern s32 D_8009DF14_9EB14;
extern s32 D_8009DF18_9EB18;
extern s32 D_8009DF1C_9EB1C;
extern s32 D_8009DF20_9EB20;
extern s32 D_8009DF24_9EB24;
extern s32 D_8009DF28_9EB28;
extern s32 D_8009DF2C_9EB2C;
extern s32 D_8009DF30_9EB30;
extern s32 D_8009DF34_9EB34;

s32 *gGalleryLabelPtrs[] = {
    &D_8009DF34_9EB34, &D_8009DF30_9EB30, &D_8009DF2C_9EB2C, &D_8009DF28_9EB28, &D_8009DF24_9EB24, &D_8009DF20_9EB20,
    &D_8009DF1C_9EB1C, &D_8009DF18_9EB18, &D_8009DF14_9EB14, &D_8009DF10_9EB10, &D_8009DF0C_9EB0C, &D_8009DF08_9EB08,
    &D_8009DF04_9EB04, &D_8009DF00_9EB00, &D_8009DEFC_9EAFC, &D_8009DEF8_9EAF8, &D_8009DEF4_9EAF4, &D_8009DEF0_9EAF0,
    &D_8009DEEC_9EAEC, &D_8009DEE8_9EAE8, &D_8009DEE4_9EAE4, &D_8009DEE0_9EAE0, &D_8009DEDC_9EADC, &D_8009DED8_9EAD8,
    &D_8009DED4_9EAD4, &D_8009DED0_9EAD0, &D_8009DECC_9EACC, &D_8009DEC8_9EAC8, &D_8009DEC4_9EAC4, &D_8009DEC0_9EAC0,
    &D_8009DEBC_9EABC, &D_8009DEB8_9EAB8, &D_8009DEB4_9EAB4, &D_8009DEB0_9EAB0
};

GalleryItemEntry gGalleryItems_SaveSlots[] = {
    { 0xA0, 0xB8, { 0x07, 0x00, 0x0C, 0x01, 0x01 }, 0x00, { 0 } },
    { 0xB8, 0xB8, { 0x07, 0x01, 0x0C, 0x02, 0x02 }, 0x00, { 0 } },
    { 0xD0, 0xB8, { 0x07, 0x02, 0x0C, 0x03, 0x02 }, 0x00, { 0 } },
    { 0xE8, 0xB8, { 0x07, 0x03, 0x0C, 0x04, 0x03 }, 0x00, { 0 } },
    { 0x00, 0xB8, { 0x07, 0x04, 0x0C, 0x05, 0x03 }, 0x00, { 0 } },
    { 0x18, 0xB8, { 0x07, 0x05, 0x0C, 0x06, 0x04 }, 0x00, { 0 } },
    { 0x30, 0xB8, { 0x07, 0x06, 0x0C, 0x07, 0x03 }, 0x00, { 0 } },
    { 0x48, 0xB8, { 0x07, 0x07, 0x0C, 0x08, 0x04 }, 0x00, { 0 } },
    { 0xAC, 0xD0, { 0x07, 0x08, 0x0C, 0x09, 0x04 }, 0x00, { 0 } },
    { 0xC4, 0xD0, { 0x07, 0x09, 0x0C, 0x0A, 0x04 }, 0x00, { 0 } },
    { 0xDC, 0xD0, { 0x07, 0x0A, 0x0C, 0x0B, 0x04 }, 0x00, { 0 } },
    { 0xF4, 0xD0, { 0x07, 0x0B, 0x0C, 0x0C, 0x05 }, 0x00, { 0 } },
    { 0x0C, 0xD0, { 0x07, 0x0C, 0x0C, 0x1F, 0x03 }, 0x00, { 0 } },
    { 0x24, 0xD0, { 0x07, 0x0D, 0x0C, 0x20, 0x03 }, 0x00, { 0 } },
    { 0x3C, 0xD0, { 0x07, 0x0E, 0x0C, 0x21, 0x03 }, 0x00, { 0 } },
};

GalleryItemEntry gGalleryItems_Characters[] = {
    { 0x94, 0xB8, { 0x08, 0x00, 0x0D, 0x1C, 0x00 }, 0x00, { 0 } },
    { 0xAC, 0xB8, { 0x08, 0x01, 0x0D, 0x1D, 0x00 }, 0x00, { 0 } },
    { 0xC4, 0xB8, { 0x08, 0x02, 0x0D, 0x1E, 0x00 }, 0x00, { 0 } },
    { 0xDC, 0xB8, { 0x08, 0x03, 0x0E, 0x1C, 0x00 }, 0x00, { 0 } },
    { 0xF4, 0xB8, { 0x08, 0x04, 0x0E, 0x1D, 0x00 }, 0x00, { 0 } },
    { 0x0C, 0xB8, { 0x08, 0x05, 0x0E, 0x1E, 0x00 }, 0x00, { 0 } },
    { 0x24, 0xB8, { 0x08, 0x06, 0x0F, 0x1C, 0x00 }, 0x00, { 0 } },
    { 0x3C, 0xB8, { 0x08, 0x07, 0x0F, 0x1D, 0x00 }, 0x00, { 0 } },
    { 0x54, 0xB8, { 0x08, 0x08, 0x0F, 0x1E, 0x00 }, 0x00, { 0 } },
    { 0x94, 0xD0, { 0x08, 0x09, 0x10, 0xFF, 0x00 }, 0x00, { 0 } },
    { 0xAC, 0xD0, { 0x08, 0x0A, 0x11, 0xFF, 0x00 }, 0x00, { 0 } },
    { 0xC4, 0xD0, { 0x08, 0x0B, 0x12, 0xFF, 0x00 }, 0x00, { 0 } },
    { 0xDC, 0xD0, { 0x08, 0x0C, 0x13, 0xFF, 0x00 }, 0x00, { 0 } },
    { 0xF4, 0xD0, { 0x08, 0x0D, 0x14, 0xFF, 0x00 }, 0x00, { 0 } },
    { 0x0C, 0xD0, { 0x08, 0x0E, 0x15, 0xFF, 0x00 }, 0x00, { 0 } },
    { 0x24, 0xD0, { 0x08, 0x0F, 0x16, 0xFF, 0x00 }, 0x00, { 0 } },
    { 0x3C, 0xD0, { 0x08, 0x10, 0x17, 0xFF, 0x00 }, 0x00, { 0 } },
    { 0x54, 0xD0, { 0x08, 0x11, 0x18, 0xFF, 0x00 }, 0x00, { 0 } },
};

GalleryItemEntry gGalleryItems_Boards[] = {
    { 0x94, 0xC8, { 0x09, 0x00, 0x26, 0xFF, 0x00 }, 0x00, { 0 } },
    { 0xAC, 0xC8, { 0x09, 0x01, 0x27, 0xFF, 0x00 }, 0x00, { 0 } },
    { 0xC4, 0xC8, { 0x09, 0x02, 0x28, 0xFF, 0x00 }, 0x00, { 0 } },
    { 0xDC, 0xC8, { 0x09, 0x03, 0x29, 0xFF, 0x00 }, 0x00, { 0 } },
    { 0xF4, 0xC8, { 0x09, 0x04, 0x2A, 0xFF, 0x00 }, 0x00, { 0 } },
    { 0x0C, 0xC8, { 0x09, 0x05, 0x2B, 0xFF, 0x00 }, 0x00, { 0 } },
    { 0x24, 0xC8, { 0x09, 0x06, 0x2C, 0xFF, 0x00 }, 0x00, { 0 } },
    { 0x3C, 0xC8, { 0x09, 0x08, 0x2D, 0xFF, 0x00 }, 0x00, { 0 } },
    { 0x54, 0xC8, { 0x09, 0x07, 0x2E, 0xFF, 0x00 }, 0x00, { 0 } },
};

GalleryItemEntry gGalleryItems_Prizes[] = {
    { 0x94, 0xB8, { 0x0A, 0x00, 0x19, 0x01, 0x01 }, 0xFF, { 0 } },
    { 0xAC, 0xB8, { 0x0A, 0x01, 0x19, 0x02, 0x19 }, 0xFF, { 0 } },
    { 0xC4, 0xB8, { 0x0A, 0x02, 0x19, 0x03, 0x03 }, 0xFF, { 0 } },
    { 0xDC, 0xB8, { 0x0A, 0x03, 0x19, 0x04, 0x02 }, 0xFF, { 0 } },
    { 0xF4, 0xB8, { 0x0A, 0x04, 0x19, 0x05, 0x04 }, 0xFF, { 0 } },
    { 0x0C, 0xB8, { 0x0A, 0x05, 0x19, 0x06, 0x05 }, 0xFF, { 0 } },
    { 0x24, 0xB8, { 0x0A, 0x06, 0x19, 0x07, 0x06 }, 0xFF, { 0 } },
    { 0x3C, 0xB8, { 0x0A, 0x07, 0x19, 0x08, 0x0D }, 0x0C, { 0 } },
    { 0x54, 0xB8, { 0x0A, 0x08, 0x19, 0x09, 0x14 }, 0x0E, { 0 } },
    { 0x94, 0xC8, { 0x0A, 0x09, 0x19, 0x0A, 0x0C }, 0x00, { 0 } },
    { 0xAC, 0xC8, { 0x0A, 0x0A, 0x19, 0x0B, 0x13 }, 0x01, { 0 } },
    { 0xC4, 0xC8, { 0x0A, 0x0B, 0x19, 0x0C, 0x11 }, 0x02, { 0 } },
    { 0xDC, 0xC8, { 0x0A, 0x0C, 0x19, 0x0D, 0x0E }, 0x04, { 0 } },
    { 0xF4, 0xC8, { 0x0A, 0x0D, 0x19, 0x0E, 0x0F }, 0x05, { 0 } },
    { 0x0C, 0xC8, { 0x0A, 0x0E, 0x19, 0x0F, 0x10 }, 0x06, { 0 } },
    { 0x24, 0xC8, { 0x0A, 0x0F, 0x19, 0x10, 0x12 }, 0x08, { 0 } },
    { 0x3C, 0xC8, { 0x0A, 0x10, 0x19, 0x11, 0x15 }, 0x09, { 0 } },
    { 0x54, 0xC8, { 0x0A, 0x11, 0x19, 0x12, 0x16 }, 0x0A, { 0 } },
    { 0x94, 0xD8, { 0x0A, 0x12, 0x19, 0x13, 0x1F }, 0x03, { 0 } },
    { 0xAC, 0xD8, { 0x0A, 0x13, 0x19, 0x14, 0x17 }, 0x0B, { 0 } },
    { 0xC4, 0xD8, { 0x0A, 0x14, 0x19, 0x15, 0x1B }, 0x00, { 0 } },
    { 0xDC, 0xD8, { 0x0A, 0x15, 0x19, 0x16, 0x18 }, 0x00, { 0 } },
    { 0xF4, 0xD8, { 0x0A, 0x16, 0x19, 0x17, 0x1A }, 0x00, { 0 } },
    { 0x0C, 0xD8, { 0x0A, 0x17, 0x19, 0x18, 0x1C }, 0x00, { 0 } },
    { 0x24, 0xD8, { 0x0A, 0x18, 0x19, 0x19, 0x1D }, 0x09, { 0 } },
    { 0x3C, 0xD8, { 0x0A, 0x19, 0x19, 0x1A, 0x1E }, 0x0A, { 0 } },
    { 0x54, 0xD8, { 0x0A, 0x1A, 0x19, 0x1B, 0x0B }, 0xFE, { 0 } },
};

GalleryItemEntry gGalleryItems_Extras[] = {
    { 0xA0, 0xB8, { 0x0B, 0x00, 0x1B, 0x01, 0x00 }, 0x00, { 0 } },
    { 0xB8, 0xB8, { 0x0B, 0x01, 0x1B, 0x02, 0x00 }, 0x00, { 0 } },
    { 0xD0, 0xB8, { 0x0B, 0x02, 0x1B, 0x03, 0x00 }, 0x00, { 0 } },
    { 0xE8, 0xB8, { 0x0B, 0x03, 0x1B, 0x04, 0x00 }, 0x00, { 0 } },
    { 0x00, 0xB8, { 0x0B, 0x04, 0x1B, 0x05, 0x00 }, 0x00, { 0 } },
    { 0x18, 0xB8, { 0x0B, 0x05, 0x1B, 0x06, 0x00 }, 0x00, { 0 } },
    { 0x30, 0xB8, { 0x0B, 0x06, 0x1B, 0x07, 0x00 }, 0x00, { 0 } },
    { 0x48, 0xB8, { 0x0B, 0x07, 0x1B, 0x08, 0x00 }, 0x00, { 0 } },
    { 0xAC, 0xD0, { 0x0B, 0x08, 0x1B, 0x09, 0x00 }, 0x00, { 0 } },
    { 0xC4, 0xD0, { 0x0B, 0x09, 0x1B, 0x0A, 0x00 }, 0x00, { 0 } },
    { 0xDC, 0xD0, { 0x0B, 0x0A, 0x1B, 0x0B, 0x00 }, 0x00, { 0 } },
    { 0xF4, 0xD0, { 0x0B, 0x0B, 0x1B, 0x0C, 0x00 }, 0x00, { 0 } },
    { 0x0C, 0xD0, { 0x0B, 0x0C, 0x1B, 0x0D, 0x00 }, 0x00, { 0 } },
    { 0x24, 0xD0, { 0x0B, 0x0D, 0x1B, 0x0E, 0x00 }, 0x00, { 0 } },
    { 0x3C, 0xD0, { 0x0B, 0x0E, 0x1B, 0x0F, 0x00 }, 0x00, { 0 } },
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

typedef struct {
    s8 menuState;
    s8 selectedOption;
    s8 menuType;
    s8 viewerComplete;
    u8 pad4[0x4];
    void *unk8;
    void *unkC;
    s16 unk10;
    s16 fadeTimer;
    s16 animTimer;
    u8 unk16;
    u8 pad17;
    void *unk18;
    u8 pad1C[0x4];
    ViewportNode unk20;
    ViewportNode fadeNode;
    ViewportNode unk3D0;
    u8 pad5A8[0x1C];
    void *unk5C4;
    u8 pad5C8[0x2C];
    void *unk5F4;
    u8 pad5F8[0x2C];
    SceneModel *menuModel;
} E770_struct;

void playBgmTrack(E770_struct *arg0, s16 bgmId) {
    gCurrentBgmId[0] = bgmId;
    arg0->unk10 = 0x28;
    playMusicTrackWithFadeIn(bgmId, 0x80, 0);
}

void beginMenuFadeOut(E770_struct *arg0) {
    setViewportFadeValue(&arg0->fadeNode, 0xFF, 10);
    arg0->fadeTimer = 10;
}

void beginMenuFadeIn(E770_struct *arg0) {
    setViewportFadeValue(&arg0->fadeNode, 0, 10);
    arg0->fadeTimer = 10;
}

s32 getMenuOptionCount(E770_struct *arg0) {
    s8 temp = arg0->menuType;

    if (temp < 2) {
        return ((ArrayElement_8 *)gMenuOptionCounts)[temp].value;
    }
    return 0;
}

void *getMenuOptionEntry(E770_struct *arg0, s32 index) {
    void *result;
    s8 menuType;
    LookupEntry *entry;

    result = NULL;
    menuType = arg0->menuType;

    if (menuType < 2) {
        entry = &((LookupEntry *)&gMenuOptionLookupPtr)[menuType];
        if (index < entry->count) {
            result = (u8 *)entry->ptr + index * 8;
        }
    }
    return result;
}

void setMenuAnimation(E770_struct *arg0, s16 animIndex, s16 transitionAnimIndex, s8 loopCount, s16 animTimer) {
    setModelAnimationLooped(arg0->menuModel, animIndex, transitionAnimIndex, loopCount);
    arg0->animTimer = animTimer;
}

INCLUDE_ASM("asm/nonmatchings/E770", func_8000DCD8_E8D8);

void cleanupGalleryMenu(E770_struct *arg0) {
    destroySceneModel(arg0->menuModel);
    arg0->unk18 = freeNodeMemory(arg0->unk18);
    arg0->unkC = freeNodeMemory(arg0->unkC);
    arg0->unk8 = freeNodeMemory(arg0->unk8);
    arg0->unk5C4 = freeNodeMemory(arg0->unk5C4);
    arg0->unk5F4 = freeNodeMemory(arg0->unk5F4);
    unlinkNode(&arg0->unk3D0);
    unlinkNode(&arg0->unk20);
    unlinkNode(&arg0->fadeNode);
}

void activateGalleryMenu(E770_struct *arg0) {
    initTiledTextureRenderState(&arg0->pad5C8, (s32)arg0->unk5C4);
    initTiledTextureRenderState(&arg0->pad5F8, (s32)arg0->unk5F4);
    setViewportFadeValue(NULL, 0, 10);
    setMenuAnimation(arg0, 0x90, 0x90, -1, 0);
    arg0->fadeTimer = 10;
    arg0->menuState = 2;
}

void waitForMenuFadeIn(E770_struct *arg0) {
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

void handleGalleryMenuInput(E770_struct *arg0) {
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

void processGalleryMenuSelection(E770_struct *arg0) {
    s16 temp = arg0->fadeTimer;

    if (temp == 0) {
        if (arg0->selectedOption == 5) {
            cleanupGalleryMenu(arg0);
            setCutsceneSelection(0, 2);
            createTaskQueue(loadCutsceneOverlay, 0x64);
            arg0->unk16 = 1;
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

void waitForGalleryViewerExit(E770_struct *arg0) {
    if (arg0->viewerComplete != 0) {
        terminateTasksByType(1);
        beginMenuFadeIn(arg0);
        arg0->menuState = 1;
    }
}

void beginGalleryMenuExit(E770_struct *arg0) {
    playSoundEffect(0xDB);
    setViewportFadeValue(NULL, 0xFF, 10);
    setMenuAnimation(arg0, 0x15A, 0x15B, 1, -1);
    arg0->fadeTimer = 10;
    arg0->menuState = 6;
}

extern void onGalleryMenuExit(void);

s32 completeGalleryMenuExit(E770_struct *arg0) {
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

extern void func_8000E6E0_F2E0(void);

void initGalleryMenu(void) {
    allocateTaskMemory(0xCC0);
    gCurrentBgmId[0] = 5;
    setupTaskSchedulerNodes(0x40, 4, 0, 0, 0, 0, 0, 0);
    playSoundEffect(0xD0);
    setGameStateHandler(func_8000E6E0_F2E0);
}

INCLUDE_ASM("asm/nonmatchings/E770", func_8000E6E0_F2E0);

extern void func_8000DCD8_E8D8(E770_struct *);

void updateGalleryMenu(void) {
    E770_struct *s0;

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
                setGameStateHandler(func_8000E6E0_F2E0);
            }
            return;
    }
    func_8000DCD8_E8D8(s0);
}

void onGalleryMenuExit(void) {
    gCurrentBgmId[0] = 5;
    returnToParentScheduler(1);
}

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEB0_9EAB0);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEB4_9EAB4);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEB8_9EAB8);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEBC_9EABC);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEC0_9EAC0);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEC4_9EAC4);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEC8_9EAC8);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DECC_9EACC);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DED0_9EAD0);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DED4_9EAD4);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DED8_9EAD8);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEDC_9EADC);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEE0_9EAE0);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEE4_9EAE4);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEE8_9EAE8);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEEC_9EAEC);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEF0_9EAF0);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEF4_9EAF4);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEF8_9EAF8);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DEFC_9EAFC);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF00_9EB00);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF04_9EB04);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF08_9EB08);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF0C_9EB0C);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF10_9EB10);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF14_9EB14);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF18_9EB18);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF1C_9EB1C);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF20_9EB20);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF24_9EB24);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF28_9EB28);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF2C_9EB2C);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF30_9EB30);

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF34_9EB34);

u8 isGalleryItemUnlocked(u8 itemIndex) {
    E770_struct *menu;
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

INCLUDE_ASM("asm/nonmatchings/E770", func_8000EE88_FA88);

void startViewerFadeIn(E770_struct *arg0) {
    void *alloc = getCurrentAllocation();
    arg0->menuState = 1;
    beginMenuFadeIn(alloc);
}

void waitForViewerFadeIn(E770_struct *arg0) {
    E770_struct *alloc = getCurrentAllocation();
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
    E770_struct *alloc;
    GalleryItemEntry *entry;
    u8 *sp30_ptr;

    alloc = getCurrentAllocation();
    sp30_ptr = sp30;
    entry = &gGalleryCategories[alloc->selectedOption].items[arg0->cursorIndex];
    memcpy(sp30_ptr, D_8009DF6C_9EB6C, 0xC);

    if (gControllerInputs & 0x8000) {
        if (isGalleryItemUnlocked(arg0->cursorIndex)) {
            playBgmTrack(alloc, entry->pad[4]);
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

INCLUDE_RODATA("asm/nonmatchings/E770", D_8009DF6C_9EB6C);

void handleViewerInput(FD98_struct *arg0) {
    FD98_struct *alloc;
    s32 inputs;
    s8 temp;
    u8 newVal;
    s32 sound;

    alloc = getCurrentAllocation();
    inputs = gControllerInputs;

    if (inputs & B_BUTTON) {
        setMenuAnimation((E770_struct *)alloc, 0x90, 0x90, -1, -1);
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
        setMenuAnimation((E770_struct *)alloc, 0x90, 0x90, -1, -1);
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

void startViewerFadeOut(E770_struct *arg0) {
    void *alloc = getCurrentAllocation();
    beginMenuFadeOut(alloc);
    arg0->menuState = 4;
}

s32 updateViewerFadeOut(E770_struct *arg0) {
    E770_struct *alloc = getCurrentAllocation();
    s16 temp = alloc->fadeTimer;

    if (temp == 0) {
        terminateCurrentTask();
        return 1;
    }
    alloc->fadeTimer = temp - 1;
    return 0;
}

extern void onGalleryViewerCleanup(void);
extern void updateGalleryViewer(E770_struct *);

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

extern void func_8000EE88_FA88(E770_struct *);

void updateGalleryViewer(E770_struct *arg0) {
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
    func_8000EE88_FA88(arg0);
}

void onGalleryViewerCleanup(void) {
    E770_struct *alloc = getCurrentAllocation();
    alloc->viewerComplete = 1;
}
