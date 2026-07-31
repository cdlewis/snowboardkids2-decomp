#include "ui/title_ui_elements.h"
#include "assets.h"
#include "common.h"
#include "core/main.h"
#include "gamestate.h"
#include "graphics/graphics.h"
#include "graphics/sprite_rdp.h"
#include "math/geometry.h"
#include "system/task_scheduler.h"
#include "ui/level_preview_3d.h"

extern u8 gConnectedControllerMask;
extern Transform3D gTitleCharacterTransforms[];
extern u16 *gTitleCharacterAnimSequences[2];

void cleanupTitleLogoTask(TitleLogoTask *);
void enqueueTitleLogoRender(TitleLogoTask *);
void cleanupControllerSlotDisplay(TitleMenuOptionsState *);
void updateControllerSlotHighlights(TitleMenuOptionsState *);
void updatePressStartPrompt(TitlePressStartPromptState *);
void cleanupPressStartPrompt(TitlePressStartPromptState *);
void renderTitleEffectModel(ModelEntityRenderState *arg0);
void cleanupTitleEffectModel(EffectState *arg0);
void setupTitleCharacterTransform(TitleCharacterState *arg0);
void updateTitleCharacterAnimation(TitleCharacterState *arg0);
void handleUnlockAllCharacterAnim(TitleCharacterState *arg0);
void updateCharacterFlyAway(TitleCharacterState *arg0);
void handlePartialUnlockCharacterAnim(TitleCharacterState *arg0);
void updatePartialUnlockAnim(TitleCharacterState *arg0);
void cleanupTitleCharacterModel(TitleCharacterState *arg0);

void cleanupTitleLogoTask(TitleLogoTask *arg0) {
    arg0->tileMapAsset = freeNodeMemory(arg0->tileMapAsset);
}

void enqueueTitleLogoRender(TitleLogoTask *arg0) {
    enqueueCallbackBySlotIndex(8, 0, renderTiledTextureMap, &arg0->tileMap);
}

void initTitleLogoRenderState(TitleLogoTask *arg0) {
    initScrollingTileMapState(&arg0->tileMap, arg0->tileMapAsset);
    arg0->tileMap.y = 0x10;
    setCallback(enqueueTitleLogoRender);
}

void loadTitleLogoAsset(TitleLogoTask *arg0) {
    arg0->tileMapAsset = loadCompressedData(&titleLogo_ROM_START, &titleLogo_ROM_END, 0x7B50);
    setCleanupCallback(cleanupTitleLogoTask);
    setCallback(initTitleLogoRenderState);
}

void initControllerSlotDisplay(TitleMenuOptionsState *state) {
    TitleScreenState *titleState;
    s32 i;
    SpriteSheetData *spriteAsset;

    titleState = (TitleScreenState *)getCurrentAllocation();
    spriteAsset = loadCompressedData(&titleScreenSprites_ROM_START, &titleScreenSprites_ROM_END, 0x2238);
    setCleanupCallback(cleanupControllerSlotDisplay);

    for (i = 0; i < titleState->menuOptionCount; i++) {
        state->menuOptions[i].x = -0x38;
        state->menuOptions[i].y = 0x26 + (i * 0x10);
        state->menuOptions[i].overridePaletteCount = 0;
        state->menuOptions[i].spriteData = spriteAsset;
        state->menuOptions[i].frameIndex = i;
        state->menuOptions[i].color.paletteAndAlpha = 0x80;
        state->menuOptions[i].transparency = 0xF0;
        state->menuOptions[i].tileMode = 0;
    }

    for (i = 0; i < 2; i++) {
        state->menuOptions[i + 4].x = -0x38;
        state->menuOptions[i + 4].y = 0x2E + i * 0x12;
        state->menuOptions[i + 4].overridePaletteCount = 0;
        state->menuOptions[i + 4].spriteData = spriteAsset;
        state->menuOptions[i + 4].frameIndex = 3;
        state->menuOptions[i + 4].frameIndex += i;
        state->menuOptions[i + 4].color.paletteAndAlpha = 0x80;
        state->menuOptions[i + 4].transparency = 0xF0;
        state->menuOptions[i + 4].tileMode = 0;
    }

    state->legalNotices.copyrightNotice.y = 0x55;
    state->legalNotices.copyrightNotice.x = -0x48;
    state->legalNotices.copyrightNotice.frameIndex = 5;
    state->legalNotices.copyrightNotice.spriteData = spriteAsset;
    state->legalNotices.licenseNotice.x = -0x48;
    state->legalNotices.licenseNotice.spriteData = spriteAsset;
    state->legalNotices.licenseNotice.frameIndex = 6;
    state->legalNotices.licenseNotice.y = state->legalNotices.copyrightNotice.y + 0xE;
    setCallback(updateControllerSlotHighlights);
}

void updateControllerSlotHighlights(TitleMenuOptionsState *arg0) {
    TitleScreenState *state;
    s32 i;
    s32 numOptions;
    s32 selectedOption;
    s32 unused[2];
    TextRenderArg *option;
    s32 selectedAlpha;
    s32 unselectedAlpha;

    state = (TitleScreenState *)getCurrentAllocation();

    if (gConnectedControllerMask != 0) {
        if (state->menuMode != 0) {
            numOptions = 2;
        } else {
            numOptions = state->menuOptionCount;
        }

        if (numOptions != 0) {
            i = 0;
            selectedAlpha = 0xFF;
            unselectedAlpha = 0x80;
            do {
                selectedOption = state->menuSelection;
                if (i == selectedOption) {
                    option = &arg0->menuOptions[i + (state->menuMode << 2)];
                    option->color.paletteAndAlpha = selectedAlpha;
                } else {
                    option = &arg0->menuOptions[i + (state->menuMode << 2)];
                    option->color.paletteAndAlpha = unselectedAlpha;
                }

                enqueueCallbackBySlotIndex(
                    8,
                    1,
                    renderTextSpriteWithTransparency,
                    &arg0->menuOptions[i + (state->menuMode << 2)]
                );
                i++;
            } while (i < numOptions);
        }
    }

    enqueueCallbackBySlotIndex(8, 1, renderSpriteFrame, &arg0->legalNotices.copyrightNotice);
    enqueueCallbackBySlotIndex(8, 1, renderSpriteFrame, &arg0->legalNotices.licenseNotice);
}

void cleanupControllerSlotDisplay(TitleMenuOptionsState *state) {
    state->menuOptions[0].spriteData = freeNodeMemory(state->menuOptions[0].spriteData);
}

void initPressStartPrompt(TitlePressStartPromptState *arg0) {
    void *dmaResult;

    dmaResult = loadCompressedData(&titleScreenSprites_ROM_START, &titleScreenSprites_ROM_END, 0x2238);
    setCleanupCallback(cleanupPressStartPrompt);
    arg0->sprite.x = 0x58;
    arg0->sprite.y = 0x30;
    arg0->sprite.frameIndex = 0x7;
    arg0->sprite.spriteData = dmaResult;
    arg0->blinkDelay = 0x1E;
    arg0->alternateFrame = 0;
    arg0->blinkCounter = 0;
    setCallback(updatePressStartPrompt);
}

void updatePressStartPrompt(TitlePressStartPromptState *arg0) {
    if (arg0->blinkDelay == 0) {
        arg0->blinkCounter++;
        if ((arg0->blinkCounter & 1) == 0) {
            arg0->alternateFrame = (arg0->alternateFrame + 1) & 1;
            arg0->sprite.frameIndex = arg0->alternateFrame + 7;
        }
        if (arg0->blinkCounter == 0x10) {
            arg0->blinkCounter = 0;
            arg0->blinkDelay = 0x1E;
        }
    } else {
        arg0->blinkDelay--;
    }
    enqueueCallbackBySlotIndex(8, 1, renderSpriteFrame, &arg0->sprite);
}

void cleanupPressStartPrompt(TitlePressStartPromptState *state) {
    state->sprite.spriteData = freeNodeMemory(state->sprite.spriteData);
}

void initTitleEffectModel(ModelEntity *arg0) {
    ColorData sp10[3];
    ColorData sp28;
    TitleScreenState *alloc;

    alloc = (TitleScreenState *)getCurrentAllocation();
    initModelEntity(arg0, 2, &alloc->menuViewport);
    setCleanupCallback(cleanupTitleEffectModel);
    setupModelEntityLighting(arg0, sp10, &sp28);
    setCallback(renderTitleEffectModel);
}

void renderTitleEffectModel(ModelEntityRenderState *state) {
    renderModelEntity(state);
}

void cleanupTitleEffectModel(EffectState *arg0) {
    freeEffectResources(arg0);
}

void initTitleCharacterModel(TitleCharacterState *arg0) {
    TitleScreenState *alloc;
    s32 temp;

    alloc = (TitleScreenState *)getCurrentAllocation();
    arg0->sceneModel = createSceneModel(arg0->characterIndex + 0x32, &alloc->menuViewport);
    setCleanupCallback(cleanupTitleCharacterModel);
    temp = 0x8000;
    arg0->flyAwayState = 0;
    arg0->yVelocity = temp;
    arg0->animSequenceIndex = 0;
    temp = (s32)gTitleCharacterAnimSequences[0];
    arg0->animTimer = 0;
    arg0->animSequencePtr = (u16 *)temp;
    setCallback(setupTitleCharacterTransform);
}

void setupTitleCharacterTransform(TitleCharacterState *arg0) {
    getCurrentAllocation();
    applyTransformToModel(arg0->sceneModel, &gTitleCharacterTransforms[arg0->characterIndex]);
    memcpy(&arg0->transform, &gTitleCharacterTransforms[arg0->characterIndex], sizeof(Transform3D));
    arg0->currentAnim = *arg0->animSequencePtr;
    arg0->animSequencePtr = arg0->animSequencePtr + 1;
    setModelAnimation(arg0->sceneModel, arg0->currentAnim);
    updateModelGeometry(arg0->sceneModel);
    setCallback(updateTitleCharacterAnimation);
}

void updateTitleCharacterAnimation(TitleCharacterState *arg0) {
    TitleScreenState *alloc;
    s32 clearResult;
    u16 animValue;

    alloc = (TitleScreenState *)getCurrentAllocation();
    clearResult = clearModelRotation(arg0->sceneModel);
    updateModelGeometry(arg0->sceneModel);

    if (clearResult != 0) {
        animValue = *arg0->animSequencePtr;
        arg0->animSequencePtr = arg0->animSequencePtr + 1;

        if (animValue == 0xFFFF) {
            arg0->animSequenceIndex = (arg0->animSequenceIndex + 1) & 1;
            arg0->animSequencePtr = gTitleCharacterAnimSequences[arg0->animSequenceIndex];
            animValue = *arg0->animSequencePtr;
            arg0->animSequencePtr = arg0->animSequencePtr + 1;

            if (arg0->animSequenceIndex == 1) {
                if (alloc->initialMusicDelay != 0) {
                    alloc->initialMusicDelay = 1;
                }

                if (arg0->characterIndex != 5) {
                    if (arg0->characterIndex != 4) {
                        setAnimationIndex(arg0->sceneModel, 0);
                    } else {
                        setAnimationIndex(arg0->sceneModel, 4);
                    }
                }
            } else {
                setAnimationIndex(arg0->sceneModel, -1);
            }
        }

        arg0->currentAnim = animValue;
        setModelAnimation(arg0->sceneModel, (s16)animValue);
    }

    if (arg0->characterIndex == 6) {
        if (alloc->unlockAllCheatProgress == 0xF0) {
            setCallback(handleUnlockAllCharacterAnim);
            return;
        }
    }

    if (arg0->characterIndex == 5) {
        if (alloc->partialUnlockCheatProgress == 0xF0) {
            setCallback(handlePartialUnlockCharacterAnim);
        }
    }
}

void handleUnlockAllCharacterAnim(TitleCharacterState *arg0) {
    s32 clearResult;
    u16 animValue;

    getCurrentAllocation();
    clearResult = clearModelRotation(arg0->sceneModel);
    updateModelGeometry(arg0->sceneModel);

    if (clearResult == 0) {
        return;
    }

    if (arg0->currentAnim == 4) {
        setModelAnimation(arg0->sceneModel, 8);
        setCallback(updateCharacterFlyAway);
        return;
    }

    animValue = *arg0->animSequencePtr;
    if (animValue != 0xFFFF) {
        arg0->currentAnim = animValue;
        arg0->animSequencePtr += 1;
    } else {
        arg0->currentAnim = 8;
        setCallback(updateCharacterFlyAway);
    }
    setModelAnimation(arg0->sceneModel, (s16)arg0->currentAnim);
}

void updateCharacterFlyAway(TitleCharacterState *arg0) {
    s32 clearResult;

    getCurrentAllocation();
    clearResult = clearModelRotation(arg0->sceneModel);
    updateModelGeometry(arg0->sceneModel);

    switch (arg0->flyAwayState) {
        case 0:
            if (clearResult != 0) {
                setModelAnimation(arg0->sceneModel, 9);
                arg0->flyAwayState = 1;
            }
            break;
        case 1: {
            s32 temp = arg0->yVelocity;
            s32 div = temp / 100;
            temp = temp + div * 9;
            arg0->yVelocity = temp;
            arg0->transform.translation.y = arg0->transform.translation.y + temp;
            applyTransformToModel(arg0->sceneModel, &arg0->transform);
            if (arg0->transform.translation.y > 0x57FFFF) {
                terminateCurrentTask();
            }
        } break;
    }
}

void handlePartialUnlockCharacterAnim(TitleCharacterState *arg0) {
    s32 clearResult;
    u16 animValue;

    getCurrentAllocation();
    clearResult = clearModelRotation(arg0->sceneModel);
    updateModelGeometry(arg0->sceneModel);

    if (clearResult == 0) {
        return;
    }

    if (arg0->currentAnim == 4) {
        arg0->currentAnim = 8;
        arg0->animTimer = 0x11;
        setModelAnimation(arg0->sceneModel, 8);
        setCallback(updatePartialUnlockAnim);
        return;
    }

    animValue = *arg0->animSequencePtr;
    if (animValue != 0xFFFF) {
        arg0->currentAnim = animValue;
        arg0->animSequencePtr += 1;
    } else {
        arg0->currentAnim = 8;
        arg0->animTimer = 0x11;
        setCallback(updatePartialUnlockAnim);
    }
    setModelAnimation(arg0->sceneModel, (s16)arg0->currentAnim);
}

void updatePartialUnlockAnim(TitleCharacterState *arg0) {
    s32 clearResult;

    clearResult = clearModelRotation(arg0->sceneModel);
    updateModelGeometry(arg0->sceneModel);

    if (clearResult != 0 && arg0->currentAnim == 8) {
        arg0->currentAnim++;
        setModelAnimation(arg0->sceneModel, (s16)arg0->currentAnim);
    }

    if (arg0->animTimer != 0) {
        arg0->animTimer--;
        if (arg0->animTimer == 0) {
            setAnimationIndex(arg0->sceneModel, 2);
        }
    }
}

void cleanupTitleCharacterModel(TitleCharacterState *arg0) {
    arg0->sceneModel = destroySceneModel(arg0->sceneModel);
}
