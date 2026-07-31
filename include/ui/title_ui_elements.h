#pragma once

#include "common.h"
#include "core/main.h"
#include "graphics/graphics.h"
#include "graphics/sprite_rdp.h"
#include "graphics/tiled_sprite_grid.h"
#include "math/geometry.h"
#include "ui/level_preview_3d.h"

typedef struct {
    SpriteRenderArg copyrightNotice;
    SpriteRenderArg licenseNotice;
} TitleLegalNoticeSprites;

typedef struct {
    TextRenderArg menuOptions[6];
    TitleLegalNoticeSprites legalNotices;
} TitleMenuOptionsState;

typedef struct {
    TileMapScrollRenderState tileMap;
    u8 padding[0xC];
    TileMapTextureAsset *tileMapAsset;
} TitleLogoTask;

typedef struct {
    SpriteRenderArg sprite;
    u8 blinkDelay;
    u8 blinkCounter;
    u8 alternateFrame;
    u8 padding;
} TitlePressStartPromptState;

typedef struct {
    /* 0x000 */ ViewportNode mainViewport;
    /* 0x1D8 */ ViewportNode menuViewport;
    /* 0x3B0 */ TileMapTextureAsset *titleLogoData;
    /* 0x3B4 */ SpriteSheetData *menuGraphicsData;
    /* 0x3B8 */ u16 idleFrameCounter;
    /* 0x3BA */ u8 reserved3BA;
    /* 0x3BB */ u8 menuSelection;
    /* 0x3BC */ u8 menuOptionCount;
    /* 0x3BD */ u8 menuMode;
    /* 0x3BE */ u8 reserved3BE;
    /* 0x3BF */ u8 partialUnlockCheatProgress;
    /* 0x3C0 */ u8 unlockAllCheatProgress;
    /* 0x3C1 */ u8 initialMusicDelay;
} TitleScreenState;

typedef struct {
    SceneModel *sceneModel;
    Transform3D transform;
    s32 yVelocity;
    u16 *animSequencePtr;
    u16 currentAnim;
    u8 animSequenceIndex;
    u8 characterIndex;
    u8 padding30;
    u8 animTimer;
    u8 flyAwayState;
} TitleCharacterState;

void loadTitleLogoAsset(TitleLogoTask *arg0);
void initControllerSlotDisplay(TitleMenuOptionsState *);
void initPressStartPrompt(TitlePressStartPromptState *arg0);
void initTitleEffectModel(ModelEntity *arg0);
void initTitleCharacterModel(TitleCharacterState *arg0);
