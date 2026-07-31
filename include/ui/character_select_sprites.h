#pragma once

#include "common.h"
#include "graphics/displaylist.h"
#include "graphics/sprite_rdp.h"
#include "graphics/tiled_sprite_grid.h"
#include "level_preview_3d.h"
#include "math/geometry.h"

typedef struct {
    /* 0x00 */ TileMapScrollRenderState renderState;
    /* 0x2C */ TileMapTextureAsset *textureAsset;
} CharacterSelectBackgroundState;

typedef struct {
    /* 0x00 */ TextRenderArg playerMarkers[4];
    /* 0x40 */ u8 pulseTimers[8];
} CharacterSelectSprites;

typedef struct {
    SceneModel *model;
    Transform3D matrix;
    u16 characterIndex;
    s16 animationState;
    u8 displayMode;
    u8 timer;
} CharacterPreviewState;

void initCharacterSelectTextureDataLoad(CharacterSelectBackgroundState *state);
void initCharacterSelectSprites(CharacterSelectSprites *state);
void initCharacterSelectBoardTask(DisplayListObject *board);
void initCharacterPreview(CharacterPreviewState *state);
void initPlayer3CharacterSelectIndicator(SpriteRenderArg *indicator);
void initPlayer2CharacterSelectIndicator(SpriteRenderArg *indicator);
