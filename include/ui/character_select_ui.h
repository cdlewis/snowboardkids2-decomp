#ifndef _232F0_H
#define _232F0_H

#include "math/geometry.h"
#include "ui/level_preview_3d.h"

#define CHAR_SELECT_MENU_NAV 0
#define CHAR_SELECT_MENU_ROTATING 1
#define CHAR_SELECT_MENU_CONFIRMING 2
#define CHAR_SELECT_CHAR_ROW_BROWSE 3
#define CHAR_SELECT_CHAR_ROW_SLIDE 4
#define CHAR_SELECT_CHAR_ROW_FLASH 5
#define CHAR_SELECT_CHAR_CONFIRMED 6
#define CHAR_SELECT_CHAR_VARIANT_SLIDE 9
#define CHAR_SELECT_CHAR_VARIANT_BROWSE 10
#define CHAR_SELECT_BOARD_BROWSE 15
#define CHAR_SELECT_BOARD_SLIDE 16
#define CHAR_SELECT_BOARD_FLASH 17
#define CHAR_SELECT_CANCEL_EXIT 20
#define CHAR_SELECT_READY_CONFIRM 25
#define CHAR_SELECT_P2_NAME_REVEAL 26
#define CHAR_SELECT_READY_WAIT 27
#define CHAR_SELECT_P2_CANCEL 30

typedef struct {
    s16 carouselXPositions[17];
    Vec3s menuPositions[5];
} CharSelectModelPositions;

typedef struct {
    Vec3s positions;
    Vec3s yIncrements[4];
    s16 iconYIncrement;
} CharSelectIconPositions;

typedef struct {
    u8 numStats;
    u8 numColumns;
    u8 itemStats[48];
    s16 iconTargetY[13];
} CharSelectItemData;

typedef struct {
    s16 positions[7];
    s16 D_8008DDE6[3];
} PlayerNumberPositions;

typedef struct {
    s16 iconPositions[11];
    s16 D_8008DE02[11];
} CharSelectStatsPositions;

typedef struct {
    u8 arrowSpriteIndices[2];
    s16 arrowSpriteOffsets[2];
    s8 arrowOffsets[28];
    s16 arrowPositions[13];
} CharSelectArrowData;

typedef struct {
    s16 boardPositions[11];
    u16 boardWidths[17];
} CharSelectBoardData;

typedef struct {
    s16 statSpacing[1];
    s16 statPositions[16];
    s16 namePositions[17];
} CharSelectAnimData;

typedef struct {
    u8 pad0[0x20];
    void *modelAsset;
    void *animationAsset;
    void *skeletonAsset;
    void *paletteAsset;
    u8 pad30[4];
    u8 lightR;
    u8 lightG;
    u8 lightB;
    u8 pad37;
    u8 ambientR;
    u8 ambientG;
    u8 ambientB;
    u8 pad3B;
    Transform3D rotationMatrix;
    Transform3D positionMatrix;
    Transform3D worldMatrix;
    s32 targetX;
    u8 selectionState;
    u8 playerIndex;
    u8 charPaletteIndex;
} CharSelectPreviewModel;

typedef struct {
    SceneModel *model;
    Transform3D transform;
    union {
        SceneModel *unk20;
        s32 slideTargetX;
        s32 targetX;
        s16 unk20_s16;
    } unk20_u;
    u8 playerIndex;
} CharSelectBoardPreview;

s32 countUnlockedSlotsInCategory(u8 category);
void initCharacterSelectScreen(void);
void initCharSelectSlidePosition(CharSelectPreviewModel *);
void reloadCharSelectPreviewAssets(CharSelectPreviewModel *);
void updateCharSelectPreviewLighting(CharSelectPreviewModel *, u8);
void recreateCharSelectBoardModelForSlideIn(CharSelectBoardPreview *);
void updateCharSelectPreviewModel(CharSelectPreviewModel *);
void updateCharSelectBoardPreview(CharSelectBoardPreview *);
void cleanupCharSelectPreviewAssets(CharSelectPreviewModel *);
void cleanupCharSelectBoardModel(CharSelectBoardPreview *);
SceneModel *cleanupSceneModelHolder(SceneModel **);

#endif
