#pragma once

#include "common.h"
#include "data/data_table.h"
#include "graphics/displaylist.h"
#include "graphics/graphics.h"
#include "graphics/sprite_rdp.h"
#include "graphics/tiled_sprite_grid.h"
#include "math/geometry.h"
#include "text/font_assets.h"
#include "ui/level_preview_3d.h"

typedef struct {
    s32 posX;                // 0x00
    s32 posY;                // 0x04
    s32 posZ;                // 0x08
    s32 targetX;             // 0x0C
    s32 targetY;             // 0x10
    s32 targetZ;             // 0x14
    GameDataLayout gameData; // 0x18
    void *sceneModel;        // 0x2C
    Transform3D transform;   // 0x30
    u16 currentWaypoint;     // 0x50
    u16 startWaypoint;       // 0x52
    s16 animationPhase;      // 0x54
    u16 currentRotation;     // 0x56
    u8 _pad58[0x2];          // 0x58
    u16 targetRotation;      // 0x5A
    s32 cameraHorzOffset;    // 0x5C
    s32 cameraDistance;      // 0x60
    s32 heightOffset;        // 0x64
    s32 altHeightOffset;     // 0x68
    u16 turnSpeed;           // 0x6C
    u8 _pad6E[0x4];          // 0x6E
    u16 frameTimer;          // 0x72
    u16 extraRotation;       // 0x74
    u8 turnDirection;        // 0x76
} LevelPreviewCharacterState;

typedef struct {
    TileMapScrollRenderState tileMap;
    void *imageAsset;
    s16 x;
    s16 y;
    void *spriteSheetAsset;
    u16 frameIndex;
} UnlockNotificationState;

/**
 * Initialization view over a portrait matrix slot. The transform begins at
 * offset 0x1C from each 0x34-byte slot origin, so successive views overlap.
 * Once initialized, LevelPreviewPortraitState::matrices provides the normal
 * render-time view beginning 0x18 bytes into the state.
 */
typedef struct {
    u8 _pad00[4];
    s16 rotation[3][3];
    u8 _pad16[2];
    s32 vertices;
    Transform3D transform;
    u8 *textureData;
    TableEntry_19E80 *paletteData;
    u8 textureWidth;
    u8 textureHeight;
    u8 alpha;
} LevelPreviewPortraitMatrixInitView;

typedef struct {
    /* 0x00 */ SpriteRenderArg portraitEntries[2];
    /* 0x18 */ MatrixEntry_202A0 matrices[4];
    /* 0xE8 */ s16 frameIndices[4];
    /* 0xF0 */ u16 rotations[4];
    /* 0xF8 */ DataTable_19E80 *portraitAsset;
} LevelPreviewPortraitState;

typedef struct {
    /* 0x000 */ ViewportNode cameraNode;
    /* 0x1D8 */ ViewportNode secondaryCameraNode;
    /* 0x3B0 */ ViewportNode previewNode;
    /* 0x588 */ ViewportNode viewportParentNode;
    /* 0x760 */ ViewportNode detailNode;
    /* 0x938 */ ViewportNode tertiaryCameraNode;
    /* 0xB10 */ void *portraitAsset;
    /* 0xB14 */ void *imageAsset;
    /* 0xB18 */ void *tiledBackgroundAsset;
    /* 0xB1C */ void *textRenderAsset;
    /* 0xB20 */ void *uiAsset;
    /* 0xB24 */ s32 loadStartFrame;
    /* 0xB28 */ u16 transitionCounter;
    /* 0xB2A */ u16 _padB2A;
    /* 0xB2C */ s8 selectedIndex;
    /* 0xB2D */ u8 exitMode;
    /* 0xB2E */ u8 previewLoadCounter;
    /* 0xB2F */ u8 menuState;
    /* 0xB30 */ u8 selectedLevelId;
    /* 0xB31 */ u8 previousLevelId;
    /* 0xB32 */ u8 scrollDirection;
    /* 0xB33 */ u8 levelIdList[12];
    /* 0xB3F */ u8 menuItemCount;
    /* 0xB40 */ u8 _padB40[3];
    /* 0xB43 */ u8 maxLevelCount;
    /* 0xB44 */ u8 isLoadingPreview;
    /* 0xB45 */ u8 showDetailView;
    /* 0xB46 */ u8 selectedLapCount;
    /* 0xB47 */ u8 pendingDetailAnimation;
} LevelSelectState;

typedef enum {
    MENU_STATE_NAVIGATE = 0,      // Main level selection
    MENU_STATE_SCROLL = 1,        // Portrait transition between levels
    MENU_STATE_CONFIRM = 2,       // Confirm selected level
    MENU_STATE_NUMBER_SELECT = 3, // Adjust custom lap count
    MENU_STATE_UNKNOWN = 4,
    MENU_STATE_PROMPT = 5,       // Await input before opening detail view
    MENU_STATE_DETAIL_OPEN = 6,  // Fade into detail view
    MENU_STATE_DETAIL_WAIT = 7,  // Wait for detail-view fade
    MENU_STATE_DETAIL = 8,       // Interactive detail view
    MENU_STATE_DETAIL_CLOSE = 9, // Fade out of detail view
} LevelSelectMenuState;

typedef struct {
    SceneModel *model;
    Transform3D transform;
    s16 animationIndex;
    s16 unk22;
} MenuCharacterModelState;

typedef struct {
    u8 high;
    u8 low;
} ColorBytes;

typedef union {
    s16 asS16;
    ColorBytes asBytes;
} ColorValue;

typedef struct {
    s16 x;
    s16 y;
    void *textString;
    void *textRenderAsset;
    ColorValue color1;
    ColorValue color2;
    u8 priority;
} MinigameDescTextState;

typedef struct {
    /* 0x00 */ TextRenderArg spriteEntries[2];
    /* 0x20 */ u8 _pad20[0xC];
    /* 0x2C */ s16 titleX;
    /* 0x2E */ s16 titleY;
    /* 0x30 */ u16 *titleText;
    /* 0x34 */ void *textRenderAsset;
    /* 0x38 */ s16 titleColor1;
    /* 0x3A */ s16 titleColor2;
    /* 0x3C */ u8 titlePriority;
    /* 0x3D */ u8 _pad3D[3];
    /* 0x40 */ s16 counterX;
    /* 0x42 */ s16 counterY;
    /* 0x44 */ void *counterValuePtr;
    /* 0x48 */ void *counterRenderAsset;
    /* 0x4C */ s16 counterColor1;
    /* 0x4E */ s16 counterColor2;
    /* 0x50 */ u8 counterPriority;
    /* 0x51 */ u8 _pad51[3];
    /* 0x54 */ void *backgroundAsset;
    /* 0x58 */ s16 prizeCount;
    /* 0x5A */ u16 previousCount;
    /* 0x5C */ u8 _pad5C[4];
    /* 0x60 */ u8 animationTimer;
} PrizeDisplayState;

typedef struct {
    TileMapScrollRenderState tileMap;
    void *tiledBackgroundAsset;
} MenuTiledBackgroundState;

typedef struct {
    SpriteRenderArg iconEntries[10];
    SpriteRenderArg sprite78;
    SpriteRenderArg sprite84;
    SpriteRenderArg sprite90;
    SpriteRenderArg sprite9C;
    TextRenderArg textEntries[4];
    TextData textPaletteData;
    char numBuffer[2];
    u16 textAlpha;
    u8 animTimer;
} CharacterSelectDisplayState;

void initCharacterSelectDisplay(CharacterSelectDisplayState *);
void initLevelPreviewCharacter(LevelPreviewCharacterState *arg0);
void initUnlockNotification(UnlockNotificationState *state);
void initLevelPreviewPortraits(LevelPreviewPortraitState *state);
void initLevelPreviewPortraitDisplay(LevelPreviewPortraitState *portraitState);
void cleanupLevelPreviewPortraits(LevelPreviewPortraitState *state);
void initMenuCharacterModel(MenuCharacterModelState *state);
void setupLevelPreviewCamera(LevelPreviewCharacterState *arg0);
void updateLevelPreviewCamera(LevelPreviewCharacterState *state);
void initPortraitRotationFrames(LevelPreviewPortraitState *arg0);
void renderConfirmationIndicator(void *arg0);
void cleanupConfirmationIndicator(SpriteRenderArg *state);
void updatePrizeDisplay(PrizeDisplayState *state);
void cleanupPrizeDisplay(PrizeDisplayState *state);
void renderUnlockNotification(UnlockNotificationState *state);
void initUnlockNotificationSprite(UnlockNotificationState *state);
void cleanupUnlockNotification(UnlockNotificationState *state);
void cleanupMenuCharacterModel(MenuCharacterModelState *state);
void setupMenuCharacterModel(MenuCharacterModelState *state);
void updateMenuCharacterModel(MenuCharacterModelState *state);
void handleMenuCharacterAnimationEnd(MenuCharacterModelState *state);
void setMenuCharacterAnimation(u8 animationType, MenuCharacterModelState *state);
void cleanupMenuBackgroundEffect(MenuTiledBackgroundState *state);
void setupMenuBackgroundEffect(MenuTiledBackgroundState *state);
void updateMenuBackgroundEffect(MenuTiledBackgroundState *state);
void renderMinigameDescText(MinigameDescTextState *state);
void cleanupMinigameDescText(MinigameDescTextState *state);
void initConfirmationIndicator(SpriteRenderArg *state);
void initPrizeDisplay(PrizeDisplayState *arg0);
void initMenuBackgroundEffect(MenuTiledBackgroundState *state);
void initMinigameDescText(MinigameDescTextState *state);
void initLevelSelectTransition(void);
void cleanupLevelPreviewCharacter(LevelPreviewCharacterState *state);
void cleanupCharacterSelectionIcons(CharacterSelectDisplayState *state);
void renderCharacterSelectDisplay(CharacterSelectDisplayState *state);

void animatePortraitRotation(LevelPreviewPortraitState *state);
void renderLevelPreviewPortraits(SpriteRenderArg *entries);
