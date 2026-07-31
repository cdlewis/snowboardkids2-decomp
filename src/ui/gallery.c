#include "ui/gallery.h"
#include "animation/easing_state.h"
#include "assets.h"
#include "audio/audio.h"
#include "common.h"
#include "common_bss.h"
#include "cutscene/sys_effect.h"
#include "data/global_frame_counter.h"
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

typedef struct {
    u16 x;
    u16 y;
    u8 padding4;
    u8 overridePaletteCount;
    s8 textRow;
    u8 labelIndex;
} MenuLayoutEntry;

typedef struct {
    MenuLayoutEntry *layout;
    s32 count;
} MenuOptionConfig;

typedef struct {
    s8 row;
    s8 column;
} GalleryTextTableCoordinate;

typedef struct {
    u8 x;
    u8 y;
    GalleryTextTableCoordinate textTable;
    u8 spriteFrame;
    s8 labelIndex;
    s8 numExtra;
    s8 unlockSlotIndex;
    u8 padding8[4];
} GalleryItemEntry;

typedef struct {
    GalleryItemEntry *items;
    s32 count;
} GalleryCategoryData;

extern const u8 D_8009DF6C_9EB6C[];
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
void setupGalleryMenuState(void);
void updateGalleryMenu(void);

/* Data section definitions */

s16 gCurrentBgmId[2] = { 5, 0 };

s32 gMenuOptionLayout_Type0[] = { 0xFFD0FFE8, 0x00010103, 0x0000FFE8, 0x01020204, 0x0030FFE8,
                                  0x02040306, 0xFFE80010, 0x03030405, 0x00180010, 0x04050507 };

s32 gMenuOptionLayout_Type1[] = { 0xFFD0FFE8, 0x00010103, 0x0000FFE8, 0x01020204, 0x0030FFE8, 0x02040306,
                                  0xFFD00010, 0x03030405, 0x00000010, 0x04050507, 0x00300010, 0x00060608 };

MenuOptionConfig gMenuOptionConfig[] = {
    { (MenuLayoutEntry *)gMenuOptionLayout_Type0, 5 },
    { (MenuLayoutEntry *)gMenuOptionLayout_Type1, 6 },
};

u8 gNavigationCycleIndices[] = { 0x00, 0x01, 0x02, 0x01 };

s32 gDefaultMenuOptionCount = 4;
s32 gViewerDefaultPosX = 0x00285000;
s32 gViewerDefaultPosY = 0xFFDE3000;
s32 gViewerDefaultPosZ = 0x003A7000;

s32 gViewerInitPosition[] = { 0x00A92000, 0, 0, 0 };

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

void playBgmTrack(GalleryMenuState *arg0, s16 bgmId) {
    gCurrentBgmId[0] = bgmId;
    arg0->bgmFadeTimer = 0x28;
    playMusicTrackWithFadeIn(bgmId, 0x80, 0);
}

void beginMenuFadeOut(GalleryMenuState *arg0) {
    setViewportFadeValue(&arg0->fadeViewport, 0xFF, 10);
    arg0->fadeTimer = 10;
}

void beginMenuFadeIn(GalleryMenuState *arg0) {
    setViewportFadeValue(&arg0->fadeViewport, 0, 10);
    arg0->fadeTimer = 10;
}

s32 getMenuOptionCount(GalleryMenuState *arg0) {
    s8 menuType = arg0->menuType;

    if (menuType < 2) {
        return gMenuOptionConfig[menuType].count;
    }
    return 0;
}

MenuLayoutEntry *getMenuOptionEntry(GalleryMenuState *arg0, s32 index) {
    MenuLayoutEntry *result = NULL;
    s8 menuType = arg0->menuType;

    if (menuType < 2) {
        MenuOptionConfig *config = &gMenuOptionConfig[menuType];
        if (index < config->count) {
            result = &config->layout[index];
        }
    }
    return result;
}

void setMenuAnimation(GalleryMenuState *arg0, s16 animIndex, s16 transitionAnimIndex, s8 loopCount, s16 animTimer) {
    setModelAnimationLooped(arg0->menuModel, animIndex, transitionAnimIndex, loopCount);
    arg0->animTimer = animTimer;
}

void renderGalleryMenu(GalleryMenuState *arg0) {
    Transform3D viewportTransform;
    Transform3D menuTransform;
    int new_var;
    s32 i;
    u16 *textEntry;
    MenuLayoutEntry *entry;
    s32 halfWidth;
    s16 alpha;
    s8 menuState;

    memcpy(&menuTransform, &identityMatrix, sizeof(Transform3D));
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
    setViewportTransformById(arg0->menuViewport.viewportId, &viewportTransform);
    createYRotationMatrix(
        &menuTransform,
        (computeAngleToPosition(
             gViewerDefaultPosX,
             gViewerDefaultPosZ,
             viewportTransform.translation.x,
             viewportTransform.translation.z
         ) +
         0x1000) &
            0xFFFF
    );
    menuTransform.translation.x = gViewerDefaultPosX;
    menuTransform.translation.y = gViewerDefaultPosY;
    menuTransform.translation.z = gViewerDefaultPosZ;
    applyTransformToModel(arg0->menuModel, &menuTransform);
    enableEntityRendering(arg0->menuModel);
    setModelHeight(arg0->menuModel, menuTransform.translation.y);
    clearModelRotation(arg0->menuModel);
    updateModelGeometry(arg0->menuModel);
    enqueueCallbackBySlotIndex(1, 4, renderTiledTextureMap, &arg0->backgroundTileMap);
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
    enqueueTextLayout(
        arg0->textRenderer,
        textEntry,
        halfWidth,
        -0x48,
        0xFF,
        arg0->menuTextColor.components.alpha,
        5,
        2,
        0
    );
    entry = getMenuOptionEntry(arg0, arg0->selectedOption);
    textEntry = getTable2DEntry(arg0->textTable, entry->textRow, 0);
    halfWidth = (new_var = getMaxLinePixelWidth(textEntry) / 2);
    halfWidth = -halfWidth;
    enqueueTextLayout(
        arg0->textRenderer,
        textEntry,
        halfWidth,
        0x30,
        0xFF,
        arg0->menuTextColor.components.alpha,
        5,
        2,
        0
    );
    for (i = 0; i < getMenuOptionCount(arg0); i++) {
        entry = getMenuOptionEntry(arg0, i);
        if (arg0->menuOptionAnimTimers[i] == 0) {
            arg0->menuOptionAnimTimers[i] = 4;
            if (i == arg0->selectedOption) {
                arg0->menuOptionAnimFrames[i] = arg0->menuOptionAnimFrames[i] + 1;
            } else if (((s8)arg0->menuOptionAnimFrames[i]) != 0) {
                arg0->menuOptionAnimFrames[i] = (u8)(((s8)arg0->menuOptionAnimFrames[i]) + 1);
            }
            if (((s8)arg0->menuOptionAnimFrames[i]) >= gDefaultMenuOptionCount) {
                arg0->menuOptionAnimFrames[i] = 0;
            }
            arg0->menuOptionSprites[i].frameIndex =
                (s16)((s8)gNavigationCycleIndices[(s8)arg0->menuOptionAnimFrames[i]]);
        } else {
            arg0->menuOptionAnimTimers[i] = arg0->menuOptionAnimTimers[i] - 1;
        }
        if (i == arg0->selectedOption) {
            arg0->menuOptionSprites[i].scaleY = 0x370;
            arg0->menuOptionSprites[i].scaleX = 0x370;
            arg0->menuOptionAlpha[i] += 0xF0000;
            if (arg0->menuOptionAlpha[i] > 0xFF0000) {
                arg0->menuOptionAlpha[i] = 0xFF0000;
            }
        } else {
            arg0->menuOptionSprites[i].scaleY = 0x400;
            arg0->menuOptionSprites[i].scaleX = 0x400;
            arg0->menuOptionAlpha[i] += (s32)0xFFF10000;
            if (arg0->menuOptionAlpha[i] <= ((s32)0x95FFFF)) {
                arg0->menuOptionAlpha[i] = 0x960000;
            }
        }
        arg0->menuOptionSprites[i].x = entry->x;
        arg0->menuOptionSprites[i].y = entry->y;
        arg0->menuOptionSprites[i].overridePaletteCount = entry->overridePaletteCount;
        alpha = (s16)(arg0->menuOptionAlpha[i] >> 16);
        arg0->menuOptionSprites[i].alpha = (s8)alpha;
        arg0->menuOptionSprites[i].shade.shadeWithPadding = alpha & 0xFF;
        enqueueCallbackBySlotIndex(2, 4, renderScaledAlphaSpriteFrame, &arg0->menuOptionSprites[i]);
        arg0->menuOptionLabels[i].x = entry->x;
        arg0->menuOptionLabels[i].y = entry->y;
        arg0->menuOptionLabels[i].overridePaletteCount = 0;
        alpha = (s16)(arg0->menuOptionAlpha[i] >> 16);
        arg0->menuOptionLabels[i].alpha = (s8)alpha;
        arg0->menuOptionLabels[i].shade.shadeWithPadding = alpha & 0xFF;
        arg0->menuOptionLabels[i].frameIndex = (s16)((s8)entry->labelIndex);
        enqueueCallbackBySlotIndex(2, 5, renderScaledAlphaSpriteFrame, &arg0->menuOptionLabels[i]);
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
    arg0->backgroundTileMapAsset = freeNodeMemory(arg0->backgroundTileMapAsset);
    arg0->overlayTileMapAsset = freeNodeMemory(arg0->overlayTileMapAsset);
    unlinkNode(&arg0->menuViewport);
    unlinkNode(&arg0->overlayViewport);
    unlinkNode(&arg0->fadeViewport);
}

void activateGalleryMenu(GalleryMenuState *arg0) {
    initScrollingTileMapState(&arg0->backgroundTileMap, arg0->backgroundTileMapAsset);
    initScrollingTileMapState(&arg0->overlayTileMap, arg0->overlayTileMapAsset);
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

void handleGalleryMenuInput(GalleryMenuState *arg0) {
    s32 inputs;
    u8 selection;
    s8 sel_signed;
    s32 sound;
    s32 sel2;

    inputs = gControllerInputs[0];
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
    alloc->menuTextColor.value = 0xFF0000;
    alloc->fadeTimer = 0;
    alloc->viewerComplete = 0;
    alloc->animTimer = -1;
    alloc->isExiting = 0;
    alloc->backgroundTileMapAsset =
        loadCompressedData(&galleryBackgroundAsset_ROM_START, &galleryBackgroundAsset_ROM_END, 0xC010);
    alloc->textRenderer = loadTextRenderAsset(1);
    alloc->textTable = loadDmaAsset(4);
    alloc->spriteAsset = loadSpriteAssetData(8);
    alloc->overlayTileMapAsset =
        loadCompressedData(&galleryOverlayTexture_ROM_START, &MUSIC_BANK_UNUSED_01_ROM_START, 0xD00);
    for (i = 0; i < 6; i++) {
        alloc->menuOptionSprites[i].x = 0;
        alloc->menuOptionSprites[i].y = 0;
        alloc->menuOptionSprites[i].spriteData = alloc->spriteAsset;
        alloc->menuOptionSprites[i].frameIndex = 0;
        alloc->menuOptionSprites[i].scaleX = 0x400;
        alloc->menuOptionSprites[i].scaleY = 0x400;
        alloc->menuOptionSprites[i].rotation = 0;
        alloc->menuOptionSprites[i].shade.shadeWithPadding = 0xFF;
        alloc->menuOptionSprites[i].tileMode = 0;
        alloc->menuOptionSprites[i].overridePaletteCount = 0;
        alloc->menuOptionSprites[i].alpha = 0x96;
        alloc->menuOptionLabels[i].x = 0;
        alloc->menuOptionLabels[i].y = 0;
        alloc->menuOptionLabels[i].spriteData = alloc->spriteAsset;
        alloc->menuOptionLabels[i].frameIndex = 0;
        alloc->menuOptionLabels[i].scaleX = 0x400;
        alloc->menuOptionLabels[i].scaleY = 0x400;
        alloc->menuOptionLabels[i].rotation = 0;
        alloc->menuOptionLabels[i].shade.shadeWithPadding = 0xFF;
        alloc->menuOptionLabels[i].tileMode = 0;
        alloc->menuOptionLabels[i].overridePaletteCount = 0;
        alloc->menuOptionLabels[i].alpha = 0x96;
        alloc->menuOptionAlpha[i] = 0x960000;
        alloc->menuOptionAnimFrames[i] = 0;
        alloc->menuOptionAnimTimers[i] = 4;
    }

    alloc->menuModel = createSceneModel(0x2D, &alloc->menuViewport);
    gScaleMatrix.translation.x = 0x400000;
    gScaleMatrix.translation.y = -0x400000;
    gScaleMatrix.translation.z = 0;
    applyTransformToModel(alloc->menuModel, &gScaleMatrix);
    for (i = 0; i < 27; i++) {
        alloc->itemSprites[i].x = 0;
        alloc->itemSprites[i].y = 0;
        alloc->itemSprites[i].spriteData = alloc->spriteAsset;
        alloc->itemSprites[i].frameIndex = 0;
        alloc->itemSprites[i].color.paletteAndAlpha = 0xFF;
        alloc->itemSprites[i].tileMode = 0;
        alloc->itemSprites[i].overridePaletteCount = 0;
        alloc->boardOverlaySprites[i].x = 0;
        alloc->boardOverlaySprites[i].y = 0;
        alloc->boardOverlaySprites[i].spriteData = alloc->spriteAsset;
        alloc->boardOverlaySprites[i].frameIndex = 0x1A;
        alloc->boardOverlaySprites[i].color.paletteAndAlpha = 0xFF;
        alloc->boardOverlaySprites[i].tileMode = 0;
        alloc->boardOverlaySprites[i].overridePaletteCount = 0;
    }

    for (i = 0; i < 5; i++) {
        alloc->extraItemSprites[i].x = -8 + i * 0x10;
        alloc->extraItemSprites[i].y = 0x2C;
        alloc->extraItemSprites[i].spriteData = alloc->spriteAsset;
        alloc->extraItemSprites[i].frameIndex = 0x2F;
        alloc->extraItemSprites[i].color.paletteAndAlpha = 0xFF;
        alloc->extraItemSprites[i].tileMode = 0;
        alloc->extraItemSprites[i].overridePaletteCount = 0;
    }

    alloc->selectionCursor.x = 0;
    alloc->selectionCursor.y = 0;
    alloc->selectionCursor.tileMode = 0;
    alloc->selectionCursor.overridePaletteCount = 0;
    alloc->pageIndicator.tileMode = 0;
    alloc->pageIndicator.overridePaletteCount = 0;
    alloc->selectionCursor.spriteData = alloc->spriteAsset;
    alloc->pageIndicator.spriteData = alloc->spriteAsset;
    alloc->prizeIcon.spriteData = alloc->spriteAsset;
    alloc->flashingPrizeIcon.spriteData = alloc->spriteAsset;
    alloc->selectionCursor.frameIndex = 0x0A;
    alloc->selectionCursor.color.paletteAndAlpha = 0xFF;
    alloc->pageIndicator.x = -0x24;
    alloc->pageIndicator.y = -0x10;
    alloc->pageIndicator.frameIndex = 9;
    alloc->pageIndicator.color.paletteAndAlpha = 0xFF;
    alloc->prizeIcon.x = 0x18;
    alloc->prizeIcon.y = 0x28;
    alloc->prizeIcon.frameIndex = 0x25;
    alloc->flashingPrizeIcon.x = 0x48;
    alloc->flashingPrizeIcon.frameIndex = 0x30;
    alloc->itemStatsText.x = -0x68;
    alloc->itemStatsText.y = 0x20;
    alloc->itemStatsText.palette = 1;
    alloc->itemStatsText.string = alloc->itemStatsBuffer;
    alloc->flashingPrizeIcon.y = 0x28;
    initViewportNode(&alloc->menuViewport, 0, 0, 0xE, 1);
    setViewportScale(&alloc->menuViewport, 1.0f, 1.0f);
    setViewportId(&alloc->menuViewport, 1);
    setModelCameraTransform(&alloc->menuViewport, 0, 0, -0xA0, -0x78, 0x9F, 0x77);
    setViewportPerspective(&alloc->menuViewport, 40.0f, 1.3333334f, 10.0f, 10000.0f);
    createViewportTransform(&viewportTransform, 0, 0, 0x01400000, 0, 0, 0);
    setViewportTransformById(alloc->menuViewport.viewportId, &viewportTransform);
    initViewportNode(&alloc->overlayViewport, 0, 1, 0xA, 0);
    setViewportScale(&alloc->overlayViewport, 1.0f, 1.0f);
    setViewportId(&alloc->overlayViewport, 1);
    setModelCameraTransform(&alloc->overlayViewport, 0, 0, -0xA0, -0x78, 0x9F, 0x77);
    initViewportNode(&alloc->fadeViewport, 0, 2, 0xC, 0);
    setViewportScale(&alloc->fadeViewport, 1.0f, 1.0f);
    setViewportId(&alloc->fadeViewport, 1);
    setModelCameraTransform(&alloc->fadeViewport, 0, 0, -0x6F, -0x56, 0x6E, 0x46);
    setViewportFadeValue(0, 0xFF, 0);
    setViewportEnvColor(&alloc->menuViewport, 0, 0, 0);
    setViewportFadeValue(&alloc->menuViewport, 0, 0);
    setViewportEnvColor(&alloc->overlayViewport, 0, 0, 0);
    setViewportFadeValue(&alloc->overlayViewport, 0, 0);
    setViewportEnvColor(&alloc->fadeViewport, 0, 0x70, 0x80);
    setViewportFadeValue(&alloc->fadeViewport, 0, 0);
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
    setViewportLightColors(alloc->menuViewport.viewportId, 3, alloc->lightColors, (ColorData *)alloc->ambientColor);
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

void renderGalleryViewerContent(GalleryViewerState *arg0) {
    s32 *new_var2;
    GalleryMenuState *alloc;
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
    if (arg0->state < 5) {
        if (arg0->state > 0) {
            arg0->pageIndicatorAlpha += arg0->pageIndicatorAlphaVelocity;
            if (0xFF0000 < arg0->pageIndicatorAlpha) {
                arg0->pageIndicatorAlpha = 0xFF0000;
                arg0->pageIndicatorAlphaVelocity = (s32)0xFFF10000;
            } else if (arg0->pageIndicatorAlpha <= 0x77FFFF) {
                arg0->pageIndicatorAlpha = 0x780000;
                arg0->pageIndicatorAlphaVelocity = 0xF0000;
            }
            arg0->cursorAlpha += arg0->cursorAlphaVelocity;
            if (0xFF0000 < arg0->cursorAlpha) {
                arg0->cursorAlpha = 0xFF0000;
                arg0->cursorAlphaVelocity = (s32)0xFFF10000;
            } else if (arg0->cursorAlpha <= 0x77FFFF) {
                arg0->cursorAlpha = 0x780000;
                arg0->cursorAlphaVelocity = 0xF0000;
            }
            if (arg0->navigationState == 0) {
                arg0->pageIndicatorAlphaVelocity = (s32)0xFFF10000;
            } else {
                arg0->cursorAlphaVelocity = (s32)0xFFF10000;
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
                    alloc->itemLabels[i].x = ((s8)item->x) + sp34;
                    new_var = (s8)item->y;
                    alloc->itemLabels[i].palette = configVal;
                    alloc->itemLabels[i].y = new_var + sp3C;
                    alloc->itemLabels[i].string = (u8 *)gGalleryLabelPtrs[item->labelIndex];
                    if (isGalleryItemUnlocked(i)) {
                        alloc->itemLabels[i].shade.value = 0xFF;
                    } else {
                        alloc->itemLabels[i].shade.value = 0x64;
                    }
                    enqueueCallbackBySlotIndex(2, 4, renderTextColored, &alloc->itemLabels[i]);
                }
            }

            item = &category->items[arg0->cursorIndex];
            alloc->selectionCursor.color.paletteAndAlpha = arg0->cursorAlpha >> 16;
            alloc->selectionCursor.x = (s8)item->x;
            alloc->selectionCursor.y = (s8)item->y;
            if (alloc->selectedOption == 3) {
                alloc->selectionCursor.frameIndex = 0xB;
            } else {
                alloc->selectionCursor.frameIndex = 0xA;
            }
            if (arg0->navigationState == 0) {
                enqueueCallbackBySlotIndex(2, 4, renderTextSprite, &alloc->selectionCursor);
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
                alloc->itemSprites[i].x = ((s8)item->x) + sp34;
                alloc->itemSprites[i].y = ((s8)item->y) + sp3C;
                alloc->itemSprites[i].frameIndex = (s8)item->spriteFrame;
                if (isGalleryItemUnlocked(i & 0xFF) & 0xFF) {
                    alloc->itemSprites[i].color.paletteAndAlpha = 0xFF;
                    alloc->itemSprites[i].overridePaletteCount = 0;
                } else if (alloc->selectedOption == 2) {
                    alloc->itemSprites[i].overridePaletteCount = 0x14;
                    alloc->itemSprites[i].color.paletteAndAlpha = 0x96;
                } else {
                    alloc->itemSprites[i].overridePaletteCount = 0;
                    alloc->itemSprites[i].color.paletteAndAlpha = 0x64;
                }
                enqueueCallbackBySlotIndex(2, 4, renderTextSprite, &alloc->itemSprites[i]);
                if (alloc->selectedOption == 2) {
                    alloc->boardOverlaySprites[i].x = (s8)item->x;
                    alloc->boardOverlaySprites[i].y = (s8)item->y;
                    if (isGalleryItemUnlocked(i & 0xFF) & 0xFF) {
                        alloc->boardOverlaySprites[i].color.paletteAndAlpha = 0xFF;
                    } else {
                        alloc->boardOverlaySprites[i].color.paletteAndAlpha = 0x96;
                    }
                    enqueueCallbackBySlotIndex(2, 4, renderTextSprite, &alloc->boardOverlaySprites[i]);
                }
            }

            alloc->pageIndicator.color.paletteAndAlpha = (s16)(arg0->pageIndicatorAlpha >> 16);
            enqueueCallbackBySlotIndex(2, 4, renderTextSprite, &alloc->pageIndicator);
            item = &category->items[arg0->cursorIndex];
            if (arg0->navigationState == 0) {
                if (isGalleryItemUnlocked(arg0->cursorIndex & 0xFF) & 0xFF) {
                    switch (alloc->selectedOption) {
                        case 0:
                            for (i = 0; i < item->numExtra; i++) {
                                enqueueCallbackBySlotIndex(2, 4, renderTextSprite, &alloc->extraItemSprites[i]);
                            }

                            break;

                        case 1:
                            stat1 = getItemStat1((u8)arg0->cursorIndex);
                            stat2 = getItemStat2((u8)arg0->cursorIndex);
                            sprintf(
                                (char *)alloc->itemStatsBuffer,
                                "SPEED=%2d TURN=%2d JUMP=%2d",
                                stat1 & 0xFF,
                                stat2,
                                getItemStat3((s32)((u8)arg0->cursorIndex)) & 0xFF
                            );
                            enqueueCallbackBySlotIndex(2, 4, renderTextPalette, &alloc->itemStatsText);
                            break;

                        case 3:
                            if (gGlobalFrameCounter & 8) {
                                alloc->flashingPrizeIcon.frameIndex = 0x30;
                            } else {
                                alloc->flashingPrizeIcon.frameIndex = 0x31;
                            }
                            enqueueCallbackBySlotIndex(2, 4, renderSpriteFrame, &alloc->flashingPrizeIcon);
                            enqueueCallbackBySlotIndex(2, 4, renderSpriteFrame, &alloc->prizeIcon);
                            break;
                    }

                    tableArg1 = item->textTable.row;
                    tableArg2 = item->textTable.column;
                } else {
                    tableArg1 = 0xC;
                    tableArg2 = 0;
                }
                enqueueTextLayout(
                    alloc->textRenderer,
                    getTable2DEntry(alloc->textTable, tableArg1, tableArg2),
                    -0x68,
                    0xC,
                    0xFF,
                    0xFF,
                    5,
                    2,
                    3
                );
            }
            enqueueCallbackBySlotIndex(2, 3, renderTiledTextureMap, &alloc->overlayTileMap);
        }
    }
}

void startViewerFadeIn(GalleryViewerState *arg0) {
    GalleryMenuState *alloc = getCurrentAllocation();
    arg0->state = 1;
    beginMenuFadeIn(alloc);
}

void waitForViewerFadeIn(GalleryViewerState *arg0) {
    GalleryMenuState *alloc = getCurrentAllocation();
    s16 temp;

    temp = alloc->fadeTimer;
    if (temp == 0) {
        if (alloc->menuModel->unk16 == 0x15B) {
            setMenuAnimation(alloc, 0x15C, 0x90, 1, -1);
        }
        arg0->state = 2;
    } else {
        alloc->fadeTimer = temp - 1;
    }
}

void handleViewerGridNavigation_TwoPage(GalleryViewerState *arg0) {
    s32 inputs;
    s8 temp;
    u8 val;

    inputs = gControllerInputs[0];

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
                arg0->pageUpCursorDestination = val + 8;
                arg0->pageDownCursorDestination = arg0->cursorIndex;
            } else {
                arg0->pageUpCursorDestination = val + 7;
                arg0->pageDownCursorDestination = arg0->cursorIndex;
            }
            arg0->navigationState = 1;
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
            arg0->pageUpCursorDestination = temp;
            arg0->navigationState = 1;
            arg0->pageDownCursorDestination = arg0->cursorIndex - 7;
        }
        playSoundEffect(0x2B);
    }
}

void handleViewerGridNavigation_TwoColumn(GalleryViewerState *arg0) {
    s32 inputs;
    s8 temp2;
    s8 newVal;

    inputs = gControllerInputs[0];

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
            arg0->pageUpCursorDestination = val + 9;
            arg0->navigationState = 1;
            arg0->pageDownCursorDestination = arg0->cursorIndex;
        }
        playSoundEffect(0x2B);
        return;
    }

    if (inputs & 0x20400) {
        u8 val = arg0->cursorIndex;
        if (val < 9u) {
            arg0->cursorIndex = val + 9;
        } else {
            arg0->pageUpCursorDestination = val;
            arg0->navigationState = 1;
            arg0->pageDownCursorDestination = arg0->cursorIndex - 9;
        }
        playSoundEffect(0x2B);
    }
}

void handleViewerGridNavigation_SingleRow(GalleryViewerState *arg0) {
    s32 inputs;

    inputs = gControllerInputs[0];

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
        arg0->pageUpCursorDestination = arg0->cursorIndex;
        arg0->pageDownCursorDestination = arg0->cursorIndex;
        arg0->navigationState = 1;
        playSoundEffect(0x2B);
    }
}

void handleViewerGridNavigation_ThreeRow(GalleryViewerState *arg0) {
    u8 sp30[0xC];
    GalleryMenuState *alloc;
    GalleryItemEntry *entry;
    u8 *sp30_ptr;

    alloc = getCurrentAllocation();
    sp30_ptr = sp30;
    entry = &gGalleryCategories[alloc->selectedOption].items[arg0->cursorIndex];
    memcpy(sp30_ptr, D_8009DF6C_9EB6C, sizeof(Vec3i));

    if (gControllerInputs[0] & CONT_A) {
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

    if (gControllerInputs[0] & (STICK_LEFT | CONT_LEFT)) {
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

    if (gControllerInputs[0] & (STICK_RIGHT | CONT_RIGHT)) {
        s8 temp = arg0->cursorIndex;
        if (temp == 0x1A) {
            arg0->cursorIndex = 0;
        } else {
            arg0->cursorIndex = temp + 1;
        }
        playSoundEffect(0x2B);
        return;
    }

    if (gControllerInputs[0] & (STICK_UP | CONT_UP)) {
        u8 val = arg0->cursorIndex;
        s8 temp = val - 9;
        if ((u8)temp < 0x12u) {
            arg0->cursorIndex = temp;
        } else {
            arg0->pageUpCursorDestination = val + 0x12;
            arg0->navigationState = 1;
            arg0->pageDownCursorDestination = arg0->cursorIndex;
        }
        playSoundEffect(0x2B);
        return;
    }

    if (gControllerInputs[0] & (STICK_DOWN | CONT_DOWN)) {
        u8 val = arg0->cursorIndex;
        if (val < 0x12u) {
            arg0->cursorIndex = val + 9;
        } else {
            arg0->pageUpCursorDestination = val;
            arg0->navigationState = 1;
            arg0->pageDownCursorDestination = arg0->cursorIndex - 0x12;
        }
        playSoundEffect(0x2B);
    }
}

void handleViewerGridNavigation_Prize(GalleryViewerState *arg0) {
    s32 inputs;
    s8 temp;
    u8 val;

    inputs = gControllerInputs[0];

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
                arg0->pageUpCursorDestination = val + 8;
                arg0->pageDownCursorDestination = arg0->cursorIndex;
            } else {
                arg0->pageUpCursorDestination = val + 7;
                arg0->pageDownCursorDestination = arg0->cursorIndex;
            }
            arg0->navigationState = 1;
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
            arg0->pageUpCursorDestination = temp;
            arg0->navigationState = 1;
            arg0->pageDownCursorDestination = arg0->cursorIndex - 7;
        }
        playSoundEffect(0x2B);
    }
}

// clang-format off
const u8 D_8009DF6C_9EB6C[] = {
    0x00, 0x03, 0x33, 0x33,
    0x00, 0x1C, 0xCC, 0xCC,
    0x00, 0x00, 0x00, 0x00,
};
// clang-format on

void handleViewerInput(GalleryViewerState *arg0) {
    GalleryMenuState *alloc;
    s32 inputs;
    s8 temp;
    u8 newVal;
    s32 sound;

    alloc = getCurrentAllocation();
    inputs = gControllerInputs[0];

    if (inputs & B_BUTTON) {
        setMenuAnimation(alloc, 0x90, 0x90, -1, -1);
        sound = 0x2E;
        arg0->state = 3;
        goto play_sound;
    }

    temp = arg0->navigationState;
    if (temp == 0) {
        goto do_switch;
    }
    if (temp == 1) {
        goto button_check;
    }
    return;

do_switch:
    switch (alloc->selectedOption) {
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
        arg0->state = 3;
        setMenuAnimation(alloc, 0x90, 0x90, -1, -1);
        sound = 0x2E;
        goto play_sound;
    }
    if (inputs & 0x10800) {
        newVal = arg0->pageUpCursorDestination;
        goto set_2b;
    }
    if (inputs & 0x20400) {
        newVal = arg0->pageDownCursorDestination;
    set_2b:
        sound = 0x2B;
        arg0->navigationState = 0;
        arg0->cursorIndex = newVal;
    play_sound:
        playSoundEffect(sound);
    }
}

void startViewerFadeOut(GalleryViewerState *arg0) {
    GalleryMenuState *alloc = getCurrentAllocation();
    beginMenuFadeOut(alloc);
    arg0->state = 4;
}

s32 updateViewerFadeOut(GalleryViewerState *arg0) {
    GalleryMenuState *alloc = getCurrentAllocation();
    s16 temp = alloc->fadeTimer;

    if (temp == 0) {
        terminateCurrentTask();
        return 1;
    }
    alloc->fadeTimer = temp - 1;
    return 0;
}

void initGalleryViewer(GalleryViewerState *arg0) {
    getCurrentAllocation();
    setCleanupCallback(onGalleryViewerCleanup);
    arg0->pageIndicatorAlpha = 0x780000;
    arg0->pageIndicatorAlphaVelocity = 0x0F0000;
    arg0->cursorAlpha = 0xFF0000;
    arg0->state = 0;
    arg0->navigationState = 0;
    arg0->cursorIndex = 0;
    arg0->cursorAlphaVelocity = 0xFFF10000;
    setCallback(updateGalleryViewer);
}

void updateGalleryViewer(GalleryViewerState *arg0) {
    getCurrentAllocation();
    switch (arg0->state) {
        case 0:
            startViewerFadeIn(arg0);
            break;
        case 1:
            waitForViewerFadeIn(arg0);
            break;
        case 2:
            handleViewerInput(arg0);
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
    renderGalleryViewerContent(arg0);
}

void onGalleryViewerCleanup(void) {
    GalleryMenuState *alloc = getCurrentAllocation();
    alloc->viewerComplete = 1;
}
