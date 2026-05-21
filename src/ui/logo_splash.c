#include "assets.h"
#include "common.h"
#include "graphics/graphics.h"
#include "graphics/sprite_rdp.h"
#include "math/geometry.h"
#include "race/race_session.h"
#include "system/task_scheduler.h"
#include "ui/save_data.h"

typedef struct {
    s16 x;
    s16 y;
    s16 frameIndex;
    s16 pad6;
} FootprintSpritePlacement;

typedef struct {
    s16 x;
    s16 y;
    s32 spriteData;
    s16 frameIndex;
    s16 scaleX;
    s16 scaleY;
    s16 rotation;
    s16 shade;
    s8 tileMode;
    s8 overridePaletteCount;
    s8 alpha;
    u8 pad15[3];
} LogoSplashFootprintSprite;

typedef struct {
    u8 pad0[0x10];
    s16 clipX;
    s16 clipY;
    u8 pad14[0x18];
} LogoSplashTileMapState;

typedef enum {
    LOGO_SPLASH_STATE_INIT_RENDER_STATE,
    LOGO_SPLASH_STATE_START_ATLUS_LOGO,
    LOGO_SPLASH_STATE_UPDATE_ATLUS_LOGO,
    LOGO_SPLASH_STATE_START_RACDYM_LOGO,
    LOGO_SPLASH_STATE_UPDATE_RACDYM_FADE_IN,
    LOGO_SPLASH_STATE_UPDATE_FOOTPRINT_REVEAL,
    LOGO_SPLASH_STATE_UPDATE_RACDYM_FADE_OUT,
    LOGO_SPLASH_STATE_CLEANUP,
} LogoSplashStateId;

typedef struct {
    s8 state;
    s8 visibleFootprintCount;
    s16 frameTimer;
    u8 pad4[0x4];
    ViewportNode viewport;
    void *atlusLogoTileMap;
    void *racdymLogoTileMap;
    void *racdymFootprintSprites;
    LogoSplashTileMapState atlusLogoTileMapState;
    LogoSplashTileMapState racdymLogoTileMapState;
    LogoSplashFootprintSprite footprintSprites[6];
    s32 footprintAlphaTimers[6];
} LogoSplashState;

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

    initScrollingTileMapState(&arg0->atlusLogoTileMapState, (s32)arg0->atlusLogoTileMap);
    initScrollingTileMapState(&arg0->racdymLogoTileMapState, (s32)arg0->racdymLogoTileMap);

    arg0->atlusLogoTileMapState.clipX = 0;
    arg0->atlusLogoTileMapState.clipY = 0;
    arg0->racdymLogoTileMapState.clipX = 0;
    arg0->racdymLogoTileMapState.clipY = 0;

    for (i = 0; i < 6; i++) {
        arg0->footprintSprites[i].x = racdymFootprintPlacements[i].x;
        arg0->footprintSprites[i].y = racdymFootprintPlacements[i].y;
        arg0->footprintSprites[i].spriteData = (s32)arg0->racdymFootprintSprites;
        arg0->footprintSprites[i].frameIndex = racdymFootprintPlacements[i].frameIndex;
        arg0->footprintSprites[i].scaleX = 0x400;
        arg0->footprintSprites[i].scaleY = 0x400;
        arg0->footprintSprites[i].rotation = 0;
        arg0->footprintSprites[i].shade = 0xFF;
        arg0->footprintSprites[i].tileMode = 0;
        arg0->footprintSprites[i].overridePaletteCount = 0;
        arg0->footprintSprites[i].alpha = 0;
        arg0->footprintAlphaTimers[i] = 0;
    }

    arg0->state = LOGO_SPLASH_STATE_START_ATLUS_LOGO;
}

void startAtlusLogoSequence(LogoSplashState *arg0) {
    beginLogoSplashFadeIn(arg0);
    arg0->frameTimer = 100;
    arg0->state = LOGO_SPLASH_STATE_UPDATE_ATLUS_LOGO;
}

void updateAtlusLogoSequence(LogoSplashState *arg0) {
    arg0->frameTimer--;
    if (arg0->frameTimer == 15) {
        beginLogoSplashFadeOut(arg0);
    } else if (arg0->frameTimer == 0) {
        beginLogoSplashFadeIn(arg0);
        arg0->state = LOGO_SPLASH_STATE_START_RACDYM_LOGO;
    }
}

void startRacdymLogoSequence(LogoSplashState *arg0) {
    beginLogoSplashFadeIn(arg0);
    arg0->frameTimer = 100;
    arg0->state = LOGO_SPLASH_STATE_UPDATE_RACDYM_FADE_IN;
}

void updateRacdymLogoFadeIn(LogoSplashState *arg0) {
    arg0->frameTimer--;
    if (arg0->frameTimer == 0x50) {
        arg0->state = LOGO_SPLASH_STATE_UPDATE_FOOTPRINT_REVEAL;
        arg0->visibleFootprintCount++;
    }
}

void updateRacdymFootprintReveal(LogoSplashState *arg0) {
    s16 temp = arg0->frameTimer - 0x29;
    arg0->frameTimer--;
    switch (temp) {
        case 11:
        case 17:
        case 23:
        case 29:
        case 35:
            arg0->visibleFootprintCount++;
            break;
        case 0:
            arg0->state = LOGO_SPLASH_STATE_UPDATE_RACDYM_FADE_OUT;
            break;
    }
}

void updateRacdymLogoFadeOut(LogoSplashState *arg0) {
    arg0->frameTimer--;
    if (arg0->frameTimer == 15) {
        beginLogoSplashFadeOut(arg0);
    } else if (arg0->frameTimer == 0) {
        arg0->state = LOGO_SPLASH_STATE_CLEANUP;
    }
}

void cleanupLogoSplash(LogoSplashState *arg0) {
    arg0->atlusLogoTileMap = freeNodeMemory(arg0->atlusLogoTileMap);
    arg0->racdymLogoTileMap = freeNodeMemory(arg0->racdymLogoTileMap);
    arg0->racdymFootprintSprites = freeNodeMemory(arg0->racdymFootprintSprites);
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

    logoScreen = (LogoSplashState *)allocateTaskMemory(0x2F0);
    logoScreen->state = LOGO_SPLASH_STATE_INIT_RENDER_STATE;
    logoScreen->frameTimer = 0;
    logoScreen->visibleFootprintCount = 0;
    logoScreen->atlusLogoTileMap = loadCompressedData(&atlusLogoTileMap_ROM_START, &atlusLogoTileMap_ROM_END, 0x6350);
    logoScreen->racdymLogoTileMap =
        loadCompressedData(&racdymLogoTileMap_ROM_START, &racdymLogoTileMap_ROM_END, 0x4320);
    logoScreen->racdymFootprintSprites =
        loadCompressedData(&racdymFootprintSprites_ROM_START, &racdymFootprintSprites_ROM_END, 0x1A68);

    node = &logoScreen->viewport;
    initViewportNode(node, 0, 0, 10, 0);
    setViewportScale(node, 1.0f, 1.0f);
    setModelCameraTransform(node, 0, 0, -0xA0, -0x78, 0x9F, 0x77);
    setViewportId(node, 1);
    createViewportTransform(&nodeParams, 0, 0, 0x01000000, 0, 0, 0);
    setViewportTransformById(logoScreen->viewport.id, &nodeParams);
    setViewportEnvColor(node, 0, 0, 0);
    setViewportFadeValue(node, 0xFF, 0);
    setGameStateHandler(updateLogoSplash);
}

void updateLogoSplash(void) {
    LogoSplashState *logoScreen = getCurrentAllocation();
    s8 state;
    s32 i;

    state = logoScreen->state;
    switch (state) {
        case LOGO_SPLASH_STATE_INIT_RENDER_STATE:
            initLogoSplashRenderState(logoScreen);
            break;
        case LOGO_SPLASH_STATE_START_ATLUS_LOGO:
            startAtlusLogoSequence(logoScreen);
            break;
        case LOGO_SPLASH_STATE_UPDATE_ATLUS_LOGO:
            updateAtlusLogoSequence(logoScreen);
            break;
        case LOGO_SPLASH_STATE_START_RACDYM_LOGO:
            startRacdymLogoSequence(logoScreen);
            break;
        case LOGO_SPLASH_STATE_UPDATE_RACDYM_FADE_IN:
            updateRacdymLogoFadeIn(logoScreen);
            break;
        case LOGO_SPLASH_STATE_UPDATE_FOOTPRINT_REVEAL:
            updateRacdymFootprintReveal(logoScreen);
            break;
        case LOGO_SPLASH_STATE_UPDATE_RACDYM_FADE_OUT:
            updateRacdymLogoFadeOut(logoScreen);
            break;
        case LOGO_SPLASH_STATE_CLEANUP:
        default:
            cleanupLogoSplash(logoScreen);
            return;
    }

    state = logoScreen->state;
    if (state == LOGO_SPLASH_STATE_UPDATE_ATLUS_LOGO) {
        enqueueCallbackBySlotIndex(0, 4, renderTiledTextureMap, &logoScreen->atlusLogoTileMapState);
    } else if (state >= 2) {
        if (state < LOGO_SPLASH_STATE_CLEANUP) {
            if (state >= LOGO_SPLASH_STATE_UPDATE_RACDYM_FADE_IN) {
                enqueueCallbackBySlotIndex(0, 4, renderTiledTextureMap, &logoScreen->racdymLogoTileMapState);

                for (i = 0; i < logoScreen->visibleFootprintCount; i++) {
                    logoScreen->footprintAlphaTimers[i] += 0x330000;
                    if (logoScreen->footprintAlphaTimers[i] > 0xFFFFFF) {
                        logoScreen->footprintAlphaTimers[i] = 0xFF0000;
                    }
                    logoScreen->footprintSprites[i].alpha = (s8)(logoScreen->footprintAlphaTimers[i] >> 16);
                    enqueueCallbackBySlotIndex(0, 3, renderScaledAlphaSpriteFrame, &logoScreen->footprintSprites[i]);
                }
            }
        }
    }
}

void finishLogoSplash(void) {
    returnToParentScheduler(1);
}
