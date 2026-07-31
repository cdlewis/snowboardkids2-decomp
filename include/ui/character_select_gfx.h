#pragma once

#include "common.h"
#include "graphics/sprite_rdp.h"
#include "graphics/tiled_sprite_grid.h"
#include "math/geometry.h"
#include "text/font_assets.h"
#include "ui/level_preview_3d.h"

typedef struct {
    TileMapScrollRenderState renderState;
    TileMapTextureAsset *tileMapAsset;
} CharSelectTiledBackgroundState;

typedef struct {
    TextRenderArg entries[3];
    u8 blinkTimers[3];
    u8 numEntries;
    u8 playerIndex;
} SelectionMenuState;

typedef struct {
    CharSelectIconEntry entries[3];
    u8 padding[0x16];
    u8 playerIndex;
} CharSelectIconTargetState;

typedef struct {
    u8 padding[0x24];
    u8 playerIndex;
} P2NameRevealState;

typedef struct {
    SpriteRenderArg entries[3];
    u8 playerIndex;
} P2NameAnimationState;

typedef P2NameAnimationState CharSelectIconHideState;

typedef struct {
    CharSelectIconEntry entries[3];
    u8 padding[0x14];
    u8 numVisibleIcons;
    u8 revealCounter;
    u8 playerIndex;
    u8 delayTimer;
} CharSelectIconsState;

typedef struct {
    TextRenderArg entries[8];
    u8 blinkTimers[4];
} SelectionArrowsState;

typedef struct {
    SpriteRenderArg entries[3];
    u8 playerIndex;
} P2NameHideState;

typedef struct {
    TextRenderArg entries[4];
    SpriteRenderArg singlePlayerSprite;
} CharSelectNameSpritesState;

typedef struct {
    u16 x;
    u16 y;
} Vec2_u16;

typedef struct {
    u16 x;
    u16 y;
    s16 inc;
} PositionConfig_DDBE;

typedef struct {
    u16 y;
    u16 x;
    u16 inc;
} PositionConfig_DDE6;

typedef struct {
    u16 x;
    u16 y;
    u16 inc;
} PositionConfig_DE1A;

typedef struct {
    SpriteRenderArg entries[3];
} PlayerLabelSpritesState;

typedef struct {
    SpriteRenderArg spriteEntries[6];
    TextData textEntries[3];
    char charBufs[3][3];
    u8 playerIndex;
} CharSelectStatsState;

typedef struct {
    DisplayListObject displayObject;
    Transform3D rotationMatrix;
    Transform3D positionMatrix;
    Transform3D worldMatrix;
    s32 targetX;
    u8 selectionState;
    u8 playerIndex;
    u8 charPaletteIndex;
} CharSelectPreviewTaskState;

typedef struct {
    SceneModel *model;
    Transform3D transform;
    s32 slideTargetX;
    u8 playerIndex;
} CharSelectBoardPreview;

void animateCharSelectIconReveal(CharSelectIconsState *);
void cleanupCharSelectIcons(SpriteRenderArg *);
void updateCharSelectIconsDelay(CharSelectIconsState *);
void updateCharSelectIconTargets(CharSelectIconTargetState *);
void updateCharSelectBoardSlideOut(CharSelectBoardPreview *);
void updateCharSelectBoardPreview(CharSelectBoardPreview *);
void updateCharSelectNameSprites(CharSelectNameSpritesState *);
void cleanupCharSelectNameSprites(SpriteRenderArg *);
void updateBoardSelectArrows(SelectionArrowsState *);
void cleanupBoardSelectArrows(SpriteRenderArg *);
void updateCharSelectMenu(SelectionMenuState *);
void cleanupCharSelectMenu(SpriteRenderArg *);
void initCharSelectPreviewModel(CharSelectPreviewTaskState *);
void updateCharSelectPreviewModel(CharSelectPreviewTaskState *);
void reloadCharSelectPreviewAssets(CharSelectPreviewTaskState *);
void initCharSelectSlidePosition(CharSelectPreviewTaskState *);
void cleanupCharSelectPreviewAssets(CharSelectPreviewTaskState *);
void updateCharSelectPreviewLighting(CharSelectPreviewTaskState *, u8);
void animateCharSelectP2NameReveal(P2NameAnimationState *);
void animateCharSelectP2NameHide(P2NameHideState *);
void cleanupBoardSelectCharNames(SpriteRenderArg *);
void initCharSelectIconHideSprites(CharSelectIconHideState *);
void updateCharSelectIconsLockedState(CharSelectIconHideState *);
void showCharSelectIcons(CharSelectIconHideState *);
void initCharSelectSecondarySlot(CharSelectPreviewTaskState *);
void updateCharSelectSecondarySlide(CharSelectPreviewTaskState *);
void cleanupCharSelectSecondaryAssets(CharSelectPreviewTaskState *);
void initCharSelectBoardModel(CharSelectBoardPreview *);
void recreateCharSelectBoardModel(CharSelectBoardPreview *);
void recreateCharSelectBoardModelForSlideIn(CharSelectBoardPreview *);
void initCharSelectBoardSlideIn(CharSelectBoardPreview *);
void waitForCharSelectBoardState(CharSelectBoardPreview *);
void updateCharSelectBoardSlideIn(CharSelectBoardPreview *);
void initCharSelectBoardModelForSlideOut(CharSelectBoardPreview *);
void initCharSelectBoardSlideOut(CharSelectBoardPreview *);
SceneModel *cleanupSceneModelHolder(SceneModel **arg0);
void cleanupCharSelectPlayerLabels(SpriteRenderArg *);
void updateCharSelectPlayerLabels(PlayerLabelSpritesState *);
void initCharSelectArrows(SelectionArrowsState *);
void updateCharSelectArrows(SelectionArrowsState *);
void initCharSelectBoardPreview(CharSelectBoardPreview *);
void cleanupCharSelectBoardModel(CharSelectBoardPreview *);
void cleanupCharSelectArrows(SpriteRenderArg *);
void updateCharSelectPlayerNumbers(u8 *);
void cleanupCharSelectPlayerNumbers(SpriteRenderArg *);
void updateCharSelectPlayer1NameSprite(SpriteRenderArg *);
void cleanupCharSelectPlayer1NameSprite(SpriteRenderArg *);
void initCharSelectPlayer2NameSprites(P2NameAnimationState *);
void cleanupCharSelectPlayer2NameSprites(SpriteRenderArg *arg0);
void waitForCharSelectP2NameReveal(P2NameRevealState *arg0);
void setupCharSelectP2NamePositions(volatile SpriteRenderArg *arg0);
void initCharSelectBackgroundEffect(CharSelectTiledBackgroundState *state);
void cleanupCharSelectBackgroundEffect(CharSelectTiledBackgroundState *state);
void setupCharSelectBackgroundEffect(CharSelectTiledBackgroundState *state);
void updateCharSelectBackgroundEffect(CharSelectTiledBackgroundState *state);
void renderCharSelectScaledSprite(void *);
void cleanupCharSelectScaledSprite(FrameSpriteEntry *);
void updateCharSelectPostSlide(CharSelectPreviewTaskState *);
void updateCharSelectSlide(CharSelectPreviewTaskState *);
void hideCharSelectIcons(CharSelectIconHideState *);
void cleanupCharSelectIconHideAsset(SpriteRenderArg *);
void initCharSelectIcons(CharSelectIconsState *state);
void initCharSelectMenu(SelectionMenuState *arg0);
void initCharSelectPlayerLabels(SpriteRenderArg *arg0);
void initCharSelectPlayerNumbers(SpriteRenderArg *arg0);
void initCharSelectPlayer1NameSprite(SpriteRenderArg *arg0);
void initCharSelectNameSprites(CharSelectNameSpritesState *state);
void initCharSelectStats(CharSelectStatsState *arg0);
void updateCharSelectStats(CharSelectStatsState *arg0);
void cleanupCharSelectStats(SpriteRenderArg *arg0);
void initCharSelectScaledSprite(FrameSpriteEntry *arg0);

void initBoardSelectArrows(SelectionArrowsState *state);
void initBoardSelectCharNames(TextRenderArg *sprites);
