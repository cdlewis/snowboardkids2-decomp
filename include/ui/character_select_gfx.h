#pragma once

#include "common.h"
#include "graphics/sprite_rdp.h"
#include "math/geometry.h"
#include "text/font_assets.h"
#include "ui/level_preview_3d.h"

typedef struct {
    u8 pad0[0x1898];
    u16 charSelectMenuStates[4];
    u16 charSelectFrameCounters[4];
    u8 charSelectCharRow[8];
    u8 charSelectCharCol[8];
} BoardSelectGameState;

typedef struct {
    u8 padding[0x24];
    void *unk24;
    void *unk28;
    void *unk2C;
} func_8002494C_arg;

typedef struct {
    u16 scrollX;
    u16 scrollY;
    u8 padding[0x28];
    void *tiledBackgroundAsset;
} CharSelectTiledBackgroundState;

typedef struct {
    s16 x;
    s16 y;
    void *asset;
    s16 spriteIndex;
} SimpleSpriteEntry;

typedef struct {
    s16 x;
    s16 y;
    void *asset;
    s16 spriteIndex;
    s16 alpha;
    u8 unkC;
    u8 blinkState;
    u8 paddingE[2];
} SelectionEntry;

typedef struct {
    SelectionEntry entries[3];
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
    s16 x;
    s16 y;
    void *asset;
    s16 spriteIndex;
    s16 scaleX;
    s16 scaleY;
    s16 rotation;
    s16 alpha;
    u8 unk12;
    u8 unk13;
    u8 unk14;
} ScaledSpriteEntry;

typedef struct {
    u8 padding[0x24];
    u8 playerIndex;
} P2NameRevealState;

typedef struct {
    s16 x;
    s16 y;
    void *asset;
    s16 spriteIndex;
    u8 paletteIndex;
} P2NameSpriteEntry;

typedef P2NameSpriteEntry func_80027348_entry;

typedef struct {
    P2NameSpriteEntry entries[3];
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
    u8 pad0[0x3C];
    Transform3D rotationMatrix;
    Transform3D positionMatrix;
    Transform3D worldMatrix;
    s32 targetX;
    u8 padA0;
    u8 playerIndex;
} CharSelectSlideState;

typedef struct {
    u8 pad0[0x20];
    void *modelAsset;
    void *animationAsset;
    void *skeletonAsset;
    void *paletteAsset;
    u8 pad30[0xC];
    Transform3D rotationMatrix;
    Transform3D positionMatrix;
    Transform3D worldMatrix;
    u8 pad9C[0x5];
    u8 playerIndex;
} CharSelectSecondarySlot;

typedef struct {
    SelectionEntry entries[8];
    u8 blinkTimers[4];
} SelectionArrowsState;

typedef struct {
    P2NameSpriteEntry entries[3];
    u8 playerIndex;
} P2NameHideState;

typedef struct {
    s16 x;
    s16 y;
    void *asset;
    s16 spriteIndex;
    s16 alpha;
    u8 layerDepth;
    u8 blinkState;
    u8 paddingE[2];
} CharacterNameSprite;

typedef struct {
    SelectionEntry entries[4];
    s16 singlePlayerX;
    s16 singlePlayerY;
    void *singlePlayerAsset;
    s16 singlePlayerSpriteIndex;
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
    func_80027348_entry entries[3];
} PlayerLabelSpritesState;

typedef struct {
    SpriteRenderArg spriteEntries[6];
    TextData textEntries[3];
    char charBufs[3][3];
    u8 playerIndex;
} CharSelectStatsState;

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

void animateCharSelectIconReveal(CharSelectIconsState *);
void cleanupCharSelectIcons(SimpleSpriteEntry *);
void updateCharSelectIconsDelay(CharSelectIconsState *);
void updateCharSelectIconTargets(CharSelectIconTargetState *);
void updateCharSelectBoardSlideOut(CharSelectBoardPreview *);
void updateCharSelectBoardPreview(CharSelectBoardPreview *);
void updateCharSelectNameSprites(CharSelectNameSpritesState *);
void cleanupCharSelectNameSprites(SimpleSpriteEntry *);
void updateBoardSelectArrows(SelectionArrowsState *);
void cleanupBoardSelectArrows(SimpleSpriteEntry *);
void updateCharSelectMenu(SelectionMenuState *);
void cleanupCharSelectMenu(SimpleSpriteEntry *);
void initCharSelectPreviewModel(CharSelectPreviewModel *);
void updateCharSelectPreviewModel(CharSelectPreviewModel *);
void reloadCharSelectPreviewAssets(CharSelectPreviewModel *);
void initCharSelectSlidePosition(CharSelectPreviewModel *);
void cleanupCharSelectPreviewAssets(CharSelectPreviewModel *);
void updateCharSelectPreviewLighting(CharSelectPreviewModel *, u8);
void animateCharSelectP2NameReveal(P2NameAnimationState *);
void animateCharSelectP2NameHide(P2NameHideState *);
void cleanupBoardSelectCharNames(SimpleSpriteEntry *);
void initCharSelectIconHideSprites(CharSelectIconHideState *);
void updateCharSelectIconsLockedState(CharSelectIconHideState *);
void showCharSelectIcons(CharSelectIconHideState *);
void initCharSelectSecondarySlot(CharSelectSecondarySlot *);
void updateCharSelectSecondarySlide(CharSelectSecondarySlot *);
void cleanupCharSelectSecondaryAssets(func_8002494C_arg *);
void initCharSelectBoardModel(CharSelectBoardPreview *);
void recreateCharSelectBoardModel(CharSelectBoardPreview *);
void recreateCharSelectBoardModelForSlideIn(CharSelectBoardPreview *);
void initCharSelectBoardSlideIn(CharSelectBoardPreview *);
void waitForCharSelectBoardState(CharSelectBoardPreview *);
void updateCharSelectBoardSlideIn(CharSelectBoardPreview *);
void initCharSelectBoardModelForSlideOut(CharSelectBoardPreview *);
void initCharSelectBoardSlideOut(CharSelectBoardPreview *);
SceneModel *cleanupSceneModelHolder(SceneModel **arg0);
void cleanupCharSelectPlayerLabels(SimpleSpriteEntry *);
void updateCharSelectPlayerLabels(PlayerLabelSpritesState *);
void initCharSelectArrows(SelectionArrowsState *);
void updateCharSelectArrows(SelectionArrowsState *);
void initCharSelectBoardPreview(CharSelectBoardPreview *);
void cleanupCharSelectBoardModel(CharSelectBoardPreview *);
void cleanupCharSelectArrows(SimpleSpriteEntry *);
void updateCharSelectPlayerNumbers(u8 *);
void cleanupCharSelectPlayerNumbers(SimpleSpriteEntry *);
void updateCharSelectPlayer1NameSprite(SimpleSpriteEntry *);
void cleanupCharSelectPlayer1NameSprite(SimpleSpriteEntry *);
void initCharSelectPlayer2NameSprites(SimpleSpriteEntry *arg0);
void cleanupCharSelectPlayer2NameSprites(SimpleSpriteEntry *arg0);
void waitForCharSelectP2NameReveal(P2NameRevealState *arg0);
void setupCharSelectP2NamePositions(volatile P2NameSpriteEntry *arg0);
void initCharSelectBackgroundEffect(CharSelectTiledBackgroundState *state);
void cleanupCharSelectBackgroundEffect(CharSelectTiledBackgroundState *state);
void setupCharSelectBackgroundEffect(CharSelectTiledBackgroundState *state);
void updateCharSelectBackgroundEffect(CharSelectTiledBackgroundState *state);
void renderCharSelectScaledSprite(void *);
void cleanupCharSelectScaledSprite(ScaledSpriteEntry *);
void updateCharSelectPostSlide(CharSelectSlideState *);
void updateCharSelectSlide(CharSelectSlideState *);
void hideCharSelectIcons(CharSelectIconHideState *);
void cleanupCharSelectIconHideAsset(SimpleSpriteEntry *);
void initCharSelectIcons(CharSelectIconsState *state);
void initCharSelectMenu(SelectionMenuState *arg0);
void initCharSelectPlayerLabels(SimpleSpriteEntry *arg0);
void initCharSelectPlayerNumbers(SimpleSpriteEntry *arg0);
void initCharSelectPlayer1NameSprite(SimpleSpriteEntry *arg0);
void initCharSelectNameSprites(CharSelectNameSpritesState *state);
void initCharSelectStats(CharSelectStatsState *arg0);
void updateCharSelectStats(CharSelectStatsState *arg0);
void cleanupCharSelectStats(SimpleSpriteEntry *arg0);
void initCharSelectScaledSprite(ScaledSpriteEntry *arg0);
