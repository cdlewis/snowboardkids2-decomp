#include "ui/logo_splash.h"
#include "assets.h"
#include "common.h"
#include "graphics/graphics.h"
#include "graphics/sprite_rdp.h"
#include "graphics/tiled_sprite_grid.h"
#include "math/geometry.h"
#include "system/task_scheduler.h"

typedef struct {
    s16 x;
    s16 y;
    s16 frameIndex;
    s16 padding6;
} FootprintSpritePlacement;

void finishLogoSplash(void);
void loadLogoSplashAssets(void);
void updateLogoSplash(void);

FootprintSpritePlacement racdymFootprintPlacements[] = {
    { (s16)0xFF78, 0x0058,      0x0000 },
    { (s16)0xFFA8, 0x0050,      0x0001 },
    { (s16)0xFFA0, 0x0020,      0x0002 },
    { (s16)0xFFC0, 0x0008,      0x0003 },
    { (s16)0xFFB8, (s16)0xFFC8, 0x0004 },
    { (s16)0xFFE8, (s16)0xFFA8, 0x0005 },
};

void beginLogoSplashFadeOut(LogoSplashState *arg0) {
    setViewportEnvColor(&arg0->viewport, 0, 0, 0);
    setViewportFadeValue(&arg0->viewport, 0xFF, 0xF);
}

void beginLogoSplashFadeIn(LogoSplashState *arg0) {
    setViewportEnvColor(&arg0->viewport, 0, 0, 0);
    setViewportFadeValue(&arg0->viewport, 0, 0xF);
}

void initLogoSplashRenderState(LogoSplashState *arg0) {
    s32 i;

    initScrollingTileMapState(&arg0->atlusLogoRenderState, arg0->atlusLogoTileMapAsset);
    initScrollingTileMapState(&arg0->racdymLogoRenderState, arg0->racdymLogoTileMapAsset);

    arg0->atlusLogoRenderState.clipX = 0;
    arg0->atlusLogoRenderState.clipY = 0;
    arg0->racdymLogoRenderState.clipX = 0;
    arg0->racdymLogoRenderState.clipY = 0;

    for (i = 0; i < 6; i++) {
        arg0->footprintSprites[i].x = racdymFootprintPlacements[i].x;
        arg0->footprintSprites[i].y = racdymFootprintPlacements[i].y;
        arg0->footprintSprites[i].spriteData = arg0->racdymFootprintSpriteAsset;
        arg0->footprintSprites[i].frameIndex = racdymFootprintPlacements[i].frameIndex;
        arg0->footprintSprites[i].scaleX = 0x400;
        arg0->footprintSprites[i].scaleY = 0x400;
        arg0->footprintSprites[i].rotation = 0;
        arg0->footprintSprites[i].shade.shadeWithPadding = 0xFF;
        arg0->footprintSprites[i].tileMode = 0;
        arg0->footprintSprites[i].overridePaletteCount = 0;
        arg0->footprintSprites[i].alpha = 0;
        arg0->footprintAlphaAccumulators[i] = 0;
    }

    arg0->phase = LOGO_SPLASH_PHASE_START_ATLUS_LOGO;
}

void startAtlusLogoSequence(LogoSplashState *arg0) {
    beginLogoSplashFadeIn(arg0);
    arg0->sequenceTimer = 100;
    arg0->phase = LOGO_SPLASH_PHASE_UPDATE_ATLUS_LOGO;
}

void updateAtlusLogoSequence(LogoSplashState *arg0) {
    arg0->sequenceTimer--;
    if (arg0->sequenceTimer == 15) {
        beginLogoSplashFadeOut(arg0);
    } else if (arg0->sequenceTimer == 0) {
        beginLogoSplashFadeIn(arg0);
        arg0->phase = LOGO_SPLASH_PHASE_START_RACDYM_LOGO;
    }
}

void startRacdymLogoSequence(LogoSplashState *arg0) {
    beginLogoSplashFadeIn(arg0);
    arg0->sequenceTimer = 100;
    arg0->phase = LOGO_SPLASH_PHASE_UPDATE_RACDYM_FADE_IN;
}

void updateRacdymLogoFadeIn(LogoSplashState *arg0) {
    arg0->sequenceTimer--;
    if (arg0->sequenceTimer == 0x50) {
        arg0->phase = LOGO_SPLASH_PHASE_UPDATE_FOOTPRINT_REVEAL;
        arg0->visibleFootprintCount++;
    }
}

void updateRacdymFootprintReveal(LogoSplashState *arg0) {
    s16 temp = arg0->sequenceTimer - 0x29;
    arg0->sequenceTimer--;
    switch (temp) {
        case 11:
        case 17:
        case 23:
        case 29:
        case 35:
            arg0->visibleFootprintCount++;
            break;
        case 0:
            arg0->phase = LOGO_SPLASH_PHASE_UPDATE_RACDYM_FADE_OUT;
            break;
    }
}

void updateRacdymLogoFadeOut(LogoSplashState *arg0) {
    arg0->sequenceTimer--;
    if (arg0->sequenceTimer == 15) {
        beginLogoSplashFadeOut(arg0);
    } else if (arg0->sequenceTimer == 0) {
        arg0->phase = LOGO_SPLASH_PHASE_CLEANUP;
    }
}

void cleanupLogoSplash(LogoSplashState *arg0) {
    arg0->atlusLogoTileMapAsset = freeNodeMemory(arg0->atlusLogoTileMapAsset);
    arg0->racdymLogoTileMapAsset = freeNodeMemory(arg0->racdymLogoTileMapAsset);
    arg0->racdymFootprintSpriteAsset = freeNodeMemory(arg0->racdymFootprintSpriteAsset);
    unlinkNode(&arg0->viewport);
    terminateSchedulerWithCallback(finishLogoSplash);
}

void initLogoSplash(void) {
    setGameStateHandler(loadLogoSplashAssets);
}

void loadLogoSplashAssets(void) {
    LogoSplashState *logoScreen;
    ViewportNode *node;
    u8 nodeParams[0x20];

    logoScreen = (LogoSplashState *)allocateTaskMemory(sizeof(LogoSplashState));
    logoScreen->phase = LOGO_SPLASH_PHASE_INIT_RENDER_STATE;
    logoScreen->sequenceTimer = 0;
    logoScreen->visibleFootprintCount = 0;
    logoScreen->atlusLogoTileMapAsset =
        loadCompressedData(&atlusLogoTileMap_ROM_START, &atlusLogoTileMap_ROM_END, 0x6350);
    logoScreen->racdymLogoTileMapAsset =
        loadCompressedData(&racdymLogoTileMap_ROM_START, &racdymLogoTileMap_ROM_END, 0x4320);
    logoScreen->racdymFootprintSpriteAsset =
        loadCompressedData(&racdymFootprintSprites_ROM_START, &racdymFootprintSprites_ROM_END, 0x1A68);

    node = &logoScreen->viewport;
    initViewportNode(node, 0, 0, 10, 0);
    setViewportScale(node, 1.0f, 1.0f);
    setModelCameraTransform(node, 0, 0, -0xA0, -0x78, 0x9F, 0x77);
    setViewportId(node, 1);
    createViewportTransform(&nodeParams, 0, 0, 0x01000000, 0, 0, 0);
    setViewportTransformById(logoScreen->viewport.viewportId, &nodeParams);
    setViewportEnvColor(node, 0, 0, 0);
    setViewportFadeValue(node, 0xFF, 0);
    setGameStateHandler(updateLogoSplash);
}

void updateLogoSplash(void) {
    LogoSplashState *logoScreen = getCurrentAllocation();
    s8 state;
    s32 i;

    state = logoScreen->phase;
    switch (state) {
        case LOGO_SPLASH_PHASE_INIT_RENDER_STATE:
            initLogoSplashRenderState(logoScreen);
            break;
        case LOGO_SPLASH_PHASE_START_ATLUS_LOGO:
            startAtlusLogoSequence(logoScreen);
            break;
        case LOGO_SPLASH_PHASE_UPDATE_ATLUS_LOGO:
            updateAtlusLogoSequence(logoScreen);
            break;
        case LOGO_SPLASH_PHASE_START_RACDYM_LOGO:
            startRacdymLogoSequence(logoScreen);
            break;
        case LOGO_SPLASH_PHASE_UPDATE_RACDYM_FADE_IN:
            updateRacdymLogoFadeIn(logoScreen);
            break;
        case LOGO_SPLASH_PHASE_UPDATE_FOOTPRINT_REVEAL:
            updateRacdymFootprintReveal(logoScreen);
            break;
        case LOGO_SPLASH_PHASE_UPDATE_RACDYM_FADE_OUT:
            updateRacdymLogoFadeOut(logoScreen);
            break;
        case LOGO_SPLASH_PHASE_CLEANUP:
        default:
            cleanupLogoSplash(logoScreen);
            return;
    }

    state = logoScreen->phase;
    if (state == LOGO_SPLASH_PHASE_UPDATE_ATLUS_LOGO) {
        enqueueCallbackBySlotIndex(0, 4, renderTiledTextureMap, &logoScreen->atlusLogoRenderState);
    } else if (state >= 2) {
        if (state < LOGO_SPLASH_PHASE_CLEANUP) {
            if (state >= LOGO_SPLASH_PHASE_UPDATE_RACDYM_FADE_IN) {
                enqueueCallbackBySlotIndex(0, 4, renderTiledTextureMap, &logoScreen->racdymLogoRenderState);

                for (i = 0; i < logoScreen->visibleFootprintCount; i++) {
                    logoScreen->footprintAlphaAccumulators[i] += 0x330000;
                    if (logoScreen->footprintAlphaAccumulators[i] > 0xFFFFFF) {
                        logoScreen->footprintAlphaAccumulators[i] = 0xFF0000;
                    }
                    logoScreen->footprintSprites[i].alpha = (s8)(logoScreen->footprintAlphaAccumulators[i] >> 16);
                    enqueueCallbackBySlotIndex(0, 3, renderScaledAlphaSpriteFrame, &logoScreen->footprintSprites[i]);
                }
            }
        }
    }
}

void finishLogoSplash(void) {
    returnToParentScheduler(1);
}
