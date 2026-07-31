#pragma once

#include "common.h"
#include "graphics/graphics.h"
#include "graphics/sprite_rdp.h"
#include "graphics/tiled_sprite_grid.h"

typedef enum {
    LOGO_SPLASH_PHASE_INIT_RENDER_STATE,
    LOGO_SPLASH_PHASE_START_ATLUS_LOGO,
    LOGO_SPLASH_PHASE_UPDATE_ATLUS_LOGO,
    LOGO_SPLASH_PHASE_START_RACDYM_LOGO,
    LOGO_SPLASH_PHASE_UPDATE_RACDYM_FADE_IN,
    LOGO_SPLASH_PHASE_UPDATE_FOOTPRINT_REVEAL,
    LOGO_SPLASH_PHASE_UPDATE_RACDYM_FADE_OUT,
    LOGO_SPLASH_PHASE_CLEANUP,
} LogoSplashPhase;

typedef struct {
    /* 0x000 */ s8 phase;
    /* 0x001 */ s8 visibleFootprintCount;
    /* 0x002 */ s16 sequenceTimer;
    /* 0x004 */ u8 padding4[4];
    /* 0x008 */ ViewportNode viewport;
    /* 0x1E0 */ TileMapTextureAsset *atlusLogoTileMapAsset;
    /* 0x1E4 */ TileMapTextureAsset *racdymLogoTileMapAsset;
    /* 0x1E8 */ SpriteSheetData *racdymFootprintSpriteAsset;
    /* 0x1EC */ TileMapScrollRenderState atlusLogoRenderState;
    /* 0x218 */ TileMapScrollRenderState racdymLogoRenderState;
    /* 0x244 */ FrameSpriteEntry footprintSprites[6];
    /* 0x2D4 */ s32 footprintAlphaAccumulators[6];
    /* 0x2EC */ u8 padding2EC[4];
} LogoSplashState;

void initLogoSplash(void);
