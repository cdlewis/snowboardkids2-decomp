#pragma once

#include "20F0.h"
#include "common.h"
#include "6E840.h"

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
    Node_70B00 unk20;
    Node_70B00 fadeNode;
    Node_70B00 unk3D0;
    u8 pad5A8[0x1C];
    void *unk5C4;
    u8 pad5C8[0x2C];
    void *unk5F4;
    u8 pad5F8[0x2C];
    SceneModel *menuModel;
} E770_struct;

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

/* ArrayElement_8 D_8008CEA0_8DAA0[]; */
/* LookupEntry D_8008CE9C_8DA9C[]; */
/* GalleryCategoryData gGalleryCategories[]; */

void playBgmTrack(E770_struct *arg0, s16 bgmId);
void beginMenuFadeOut(E770_struct *arg0);
void beginMenuFadeIn(E770_struct *arg0);
s32 getMenuOptionCount(E770_struct *arg0);
void *getMenuOptionEntry(E770_struct *arg0, s32 index);
void setMenuAnimation(E770_struct *arg0, s16 animIndex, s16 transitionAnimIndex, s8 loopCount, s16 animTimer);
void cleanupGalleryMenu(E770_struct *arg0);
void activateGalleryMenu(E770_struct *arg0);
void waitForMenuFadeIn(E770_struct *arg0);
void handleGalleryMenuInput(E770_struct *arg0);
void processGalleryMenuSelection(E770_struct *arg0);
void waitForGalleryViewerExit(E770_struct *arg0);
void beginGalleryMenuExit(E770_struct *arg0);
s32 completeGalleryMenuExit(E770_struct *arg0);
void initGalleryMenu(void);
void updateGalleryMenu(void);
void onGalleryMenuExit(void);
u8 isGalleryItemUnlocked(u8 itemIndex);
void startViewerFadeIn(E770_struct *arg0);
void waitForViewerFadeIn(E770_struct *arg0);
void handleViewerGridNavigation_TwoPage(FD98_struct *arg0);
void handleViewerGridNavigation_TwoColumn(FD98_struct *arg0);
void handleViewerGridNavigation_SingleRow(FD98_struct *arg0);
void handleViewerGridNavigation_ThreeRow(FD98_struct *arg0);
void handleViewerGridNavigation_Prize(FD98_struct *arg0);
void handleViewerInput(FD98_struct *arg0);
void startViewerFadeOut(E770_struct *arg0);
s32 updateViewerFadeOut(E770_struct *arg0);
void initGalleryViewer(FD98_struct *arg0);
void updateGalleryViewer(E770_struct *arg0);
void onGalleryViewerCleanup(void);
