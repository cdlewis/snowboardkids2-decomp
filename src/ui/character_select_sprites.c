#include "ui/character_select_sprites.h"

#include "D_800AFE8C_A71FC_type.h"
#include "assets.h"
#include "common.h"
#include "gamestate.h"
#include "graphics/displaylist.h"
#include "graphics/graphics.h"
#include "graphics/sprite_rdp.h"
#include "graphics/tiled_sprite_grid.h"
#include "math/geometry.h"
#include "os_cont.h"
#include "system/task_scheduler.h"
#include "text/font_assets.h"
#include "ui/level_preview_3d.h"

typedef struct {
    /* 0x00 */ Vec2s selectedCoordinate;
    /* 0x04 */ TextData coordinateLabel;
    /* 0x10 */ TextData selectionMarker;
    /* 0x1C */ u8 formattedText[16];
} CoordinateDisplayTaskState;

extern s32 gButtonsPressed;

u32 D_800B1140_1DB6E0[] = { 0x00000000, 0x01000000, 0x00000000, 0x00000000 };
u8 D_800B1150_1DB6F0[] = { 0x0A, 0x0A, 0x01, 0x0A, 0x0A, 0x0D, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00 };
u8 D_800B115C_1DB6FC[4] = "+";
s16 D_800B1160_1DB700[] = { -6, -128, 20,  -112, 14, -128, -20, -112, -35, -128, 0,   -112, 44, -112, -40, -112,
                            40, -128, -12, -128, 12, -128, -36, -128, 36,  -128, -60, -128, 60, -128, -84, -128 };
#define D_800B1162_1DB702 (D_800B1160_1DB700 + 1)
Vec2s D_800B11A0_1DB740[] = {
    { -80,  -92 },
    { -26,  -92 },
    { 14,   -92 },
    { 64,   -92 },
    { -100, 70  },
    { -56,  70  },
    { -12,  70  },
    { 32,   70  }
};
u8 D_800B11C2_1DB762_data[] = { 0x00, 0x56, 0x00, 0x46, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0x08,
                                0x00, 0x08, 0x00, 0x08, 0x00, 0x08, 0x00, 0x08, 0x00, 0x08, 0x00, 0x08,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
#define D_800B11C2_1DB762 ((s16 *)(void *)&D_800B11C2_1DB762_data[2])
char gCoordDisplayFormatString[16] = "X=%d Y=%d  ";

void sortPlayersByCharacterRank(void);
void positionCharacterSelectSprite(TextRenderArg *, u8);
void enqueueCharacterSelectTextureRender(void *);
void awaitCharacterPreviewReady(CharacterPreviewState *);
void updateCharacterPreviewAnimation(CharacterPreviewState *);
void cleanupCharacterPreview(CharacterPreviewState *);
void checkCharacterPreviewState(CharacterPreviewState *);
void animateCharacterPreview(CharacterPreviewState *);
void awaitCharacterPreviewRotationReset(CharacterPreviewState *);
void updateCharacterSelectSprites(CharacterSelectSprites *);
void cleanupCharacterSelectSprites(CharacterSelectSprites *);
void updatePlayer3CharacterSelectIndicator(void *);
void cleanupCharacterSelectIndicator(SpriteRenderArg *);
void updatePlayer2CharacterSelectIndicator(void *);
void cleanupCharacterSelectIndicatorData(SpriteRenderArg *);
void initCharacterSelectTextureRenderState(CharacterSelectBackgroundState *);
void renderCharacterSelectBoard(void *);
void cleanupCharacterSelectBoardTask(DisplayListObject *);
void updateCoordinateDisplayTask(CoordinateDisplayTaskState *);

void sortPlayersByCharacterRank(void) {
    u8 matchingPlayers[8];
    s32 numMatching;
    GameState *allocation;
    u8 numPlayers;
    s32 playerIndex;
    s32 i;
    s32 j;
    u8 swapTemp;

    allocation = getCurrentAllocation();
    numPlayers = gGameSessionContext->numPlayers;

    for (playerIndex = 0; playerIndex < numPlayers; playerIndex++) {
        i = 0;
        for (j = 0; j < numPlayers; j++) {
            if (allocation->modeData.characterSelect.selectedCharacterIds[playerIndex] ==
                allocation->modeData.characterSelect.selectedCharacterIds[j]) {
                matchingPlayers[i] = j;
                i++;
            } else {
                matchingPlayers[j] = 99;
            }
            numMatching = i;
        }

        for (j = 0; j < (u8)numMatching - 1; j++) {
            for (i = j + 1; i < (u8)numMatching; i++) {
                if (allocation->modeData.characterSelect.characterRanks[matchingPlayers[j]] >
                    allocation->modeData.characterSelect.characterRanks[matchingPlayers[i]]) {
                    swapTemp = matchingPlayers[i];
                    matchingPlayers[i] = matchingPlayers[j];
                    matchingPlayers[j] = swapTemp;
                }
            }
        }

        for (j = 0; j < (u8)numMatching; j++) {
            allocation->modeData.characterSelect.characterRanks[matchingPlayers[j]] = j + 1;
        }
    }
}

void positionCharacterSelectSprite(TextRenderArg *arg0, u8 arg1) {
    GameState *allocation;
    u8 playerState;
    u8 count;
    s32 i;
    volatile u8 padding[8];

    allocation = getCurrentAllocation();

    arg0->x = D_800B11A0_1DB740[allocation->modeData.characterSelect.selectedCharacterIds[arg1]].x;
    arg0->y = D_800B11A0_1DB740[allocation->modeData.characterSelect.selectedCharacterIds[arg1]].y;

    if (allocation->modeData.characterSelect.characterRanks[arg1] >= 3) {
        arg0->x += 0x10;
    }

    if (!(allocation->modeData.characterSelect.characterRanks[arg1] & 1)) {
        arg0->y += 0x10;
    }

    count = 0;
    for (i = 0; i < gGameSessionContext->numPlayers; i++) {
        if (allocation->modeData.characterSelect.selectedCharacterIds[arg1] ==
            allocation->modeData.characterSelect.selectedCharacterIds[i]) {
            count++;
        }
    }

    playerState = allocation->modeData.characterSelect.characterRanks[arg1];
    if ((playerState == 3) && (count == 3)) {
        arg0->y += 0x10;
    }

    count--;
    arg0->frameIndex = D_800B11C2_1DB762[count * 4 + allocation->modeData.characterSelect.characterRanks[arg1]] + arg1;
}

void initCharacterPreview(CharacterPreviewState *arg0) {
    GameState *allocation;
    Transform3D *matrix;
    u16 modelIndex;

    allocation = getCurrentAllocation();
    modelIndex = arg0->characterIndex + 0x32;
    arg0->displayMode = 0;

    if (arg0->characterIndex == 6) {
        if (allocation->modeData.storyMap.locationBlocked[8] == -1) {
            arg0->displayMode = 2;
        }
    }

    if (arg0->characterIndex == 7) {
        if (allocation->modeData.storyMap.locationBlocked[4] == -1) {
            arg0->displayMode = 1;
            modelIndex = 0xF;
        }
    }

    if (arg0->characterIndex == 8) {
        if (allocation->modeData.storyMap.locationBlocked[3] == -1) {
            arg0->displayMode = 1;
            modelIndex = 0xF;
        }
    }

    matrix = &arg0->matrix;
    arg0->model = createSceneModel(modelIndex, allocation);
    memcpy(matrix, &identityMatrix, sizeof(Transform3D));

    arg0->matrix.translation.x = D_800B1160_1DB700[arg0->characterIndex * 2] << 16;
    arg0->matrix.translation.z = D_800B1162_1DB702[arg0->characterIndex * 2] << 16;

    if (arg0->matrix.translation.z == (s32)0xFF900000) {
        arg0->matrix.translation.y = (s32)0xFFE80000;
    }

    createYRotationMatrix(matrix, atan2Fixed(arg0->matrix.translation.x, arg0->matrix.translation.z));

    if (arg0->displayMode != 2) {
        setModelHeight(arg0->model, arg0->matrix.translation.y);
        enableEntityRendering(arg0->model);
    }

    if (arg0->characterIndex == 0) {
        createXRotationMatrix((s16(*)[3])matrix, 0x100);
    } else if (arg0->characterIndex == 4) {
        arg0->matrix.translation.z = arg0->matrix.translation.z - 0x10;
    } else if (arg0->characterIndex == 7) {
        if (arg0->displayMode == 0) {
            arg0->matrix.translation.z = arg0->matrix.translation.z + 0x40000;
        } else {
            arg0->animationState = 0x90;
            goto after_animationState;
        }
    }

    if (arg0->displayMode == 0) {
        arg0->animationState = 0;
    } else {
        arg0->animationState = 0x90;
    }

after_animationState:
    if (arg0->characterIndex >= 7 && arg0->displayMode != 0) {
        if (arg0->characterIndex == 8) {
            scaleMatrix(&arg0->matrix, 0x1000, 0x1000, 0x1000);
            arg0->matrix.translation.x = arg0->matrix.translation.x + 0x80000;
        } else {
            scaleMatrix(&arg0->matrix, 0x800, 0x800, 0x800);
        }
    }

    setCleanupCallback(cleanupCharacterPreview);
    setCallback(awaitCharacterPreviewReady);
}

void awaitCharacterPreviewReady(CharacterPreviewState *arg0) {
    GameState *allocation = getCurrentAllocation();

    if (arg0->characterIndex == 6) {
        allocation->modeData.storyMap.isStoryMapInitializing = 0;
    }
    setCallback(updateCharacterPreviewAnimation);
}

void updateCharacterPreviewAnimation(CharacterPreviewState *arg0) {
    applyTransformToModel(arg0->model, &arg0->matrix);
    if (arg0->displayMode != 2) {
        setModelAnimation(arg0->model, arg0->animationState);
        updateModelGeometry(arg0->model);
    }
    setCallback(checkCharacterPreviewState);
}

void checkCharacterPreviewState(CharacterPreviewState *arg0) {
    GameState *allocation;
    s32 i;
    GameSessionContext *ptr;
    s32 pad[2];
    s32 count;

    allocation = getCurrentAllocation();

    if (arg0->displayMode != 2) {
        applyTransformToModel(arg0->model, &arg0->matrix);
        clearModelRotation(arg0->model);
        updateModelGeometry(arg0->model);
    }

    if (arg0->displayMode == 0) {
        ptr = gGameSessionContext;
        count = ptr->numPlayers;
        if (count > 0) {
            i = 0;
            do {
                if (ptr->playerBoardIds[i] == arg0->characterIndex) {
                    u8 state = allocation->modeData.storyMap.selectionState[i];
                    if (state == 1 || state == 3) {
                        arg0->timer = 0;
                        setCallback(animateCharacterPreview);
                        return;
                    }
                }
                ptr = gGameSessionContext;
                i++;
            } while (i < ptr->numPlayers);
        }
    }
}

void animateCharacterPreview(CharacterPreviewState *arg0) {
    GameState *allocation;
    GameSessionContext *ptr;
    s32 i;
    s32 count;
    s32 clearResult;
    u8 counter;
    u16 index;
    s32 pad[2];

    allocation = getCurrentAllocation();
    clearResult = clearModelRotation(arg0->model);

    counter = arg0->timer;

    if (counter < D_800B1150_1DB6F0[arg0->characterIndex]) {
        arg0->timer = counter + 1;
        if ((u8)(counter + 1) == D_800B1150_1DB6F0[arg0->characterIndex]) {
            arg0->animationState = 1;
            setModelAnimation(arg0->model, 1);
            index = arg0->characterIndex;
            if ((index < 4) || (index == 6)) {
                setAnimationIndex(arg0->model, 0);
            } else if (index == 4) {
                setAnimationIndex(arg0->model, 4);
            } else {
                setAnimationIndex(arg0->model, -1);
            }
        }
    } else if (clearResult != 0) {
        arg0->animationState = 2;
        setModelAnimation(arg0->model, 2);
    }

    updateModelGeometry(arg0->model);

    ptr = gGameSessionContext;
    count = ptr->numPlayers;
    i = 0;
    if (count > 0) {
        GameSessionContext *localPtr = ptr;
        u16 localIndex = arg0->characterIndex;
        u8 localCount = count;
        do {
            if (localPtr->playerBoardIds[i] != localIndex) {
                i++;
            } else {
                u8 state = allocation->modeData.storyMap.selectionState[i];
                if (state == 1 || state == 3) {
                    break;
                }
                i++;
            }
        } while (i < localCount);
    }

    if (i == gGameSessionContext->numPlayers) {
        arg0->animationState = 3;
        setModelAnimation(arg0->model, 3);
        index = arg0->characterIndex;
        if (index == 6) {
            setAnimationIndex(arg0->model, 2);
        } else if (index == 7) {
            setAnimationIndex(arg0->model, 0);
        } else {
            setAnimationIndex(arg0->model, -1);
        }
        setCallback(awaitCharacterPreviewRotationReset);
    }
}

void awaitCharacterPreviewRotationReset(CharacterPreviewState *arg0) {
    s32 result;

    result = clearModelRotation(arg0->model);
    updateModelGeometry(arg0->model);

    if (result != 0) {
        arg0->animationState = 0;
        setModelAnimation(arg0->model, 0);
        setAnimationIndex(arg0->model, -1);
        setCallback(checkCharacterPreviewState);
    }
}

void cleanupCharacterPreview(CharacterPreviewState *arg0) {
    destroySceneModel(arg0->model);
}

void initCharacterSelectSprites(CharacterSelectSprites *arg0) {
    s32 i;
    void *allocation;

    getCurrentAllocation();
    allocation = loadCompressedData(&okPromptSprites_ROM_START, &characterSelectBoardTexture_ROM_START, 0x1B48);

    for (i = 0; i < gGameSessionContext->numPlayers; i++) {
        arg0->playerMarkers[i].spriteData = allocation;
        arg0->playerMarkers[i].color.paletteAndAlpha = 0xFF;
        arg0->playerMarkers[i].overridePaletteCount = 0;
        arg0->playerMarkers[i].tileMode = 0;
        arg0->pulseTimers[i] = 0;
        positionCharacterSelectSprite(&arg0->playerMarkers[i], i);
    }

    setCleanupCallback(cleanupCharacterSelectSprites);
    setCallback(updateCharacterSelectSprites);
}

void updateCharacterSelectSprites(CharacterSelectSprites *arg0) {
    GameState *allocation;
    s32 i;
    u8 state;

    allocation = getCurrentAllocation();
    sortPlayersByCharacterRank();

    for (i = 0; i < gGameSessionContext->numPlayers; i++) {
        positionCharacterSelectSprite(&arg0->playerMarkers[i], i);
        state = allocation->modeData.storyMap.selectionState[i];

        if (state == 10) {
            arg0->playerMarkers[i].color.paletteAndAlpha = 0xFF;
            arg0->pulseTimers[i] = 0;
            if (allocation->modeData.characterSelect.playerBlinkTimers[i] & 1) {
                arg0->playerMarkers[i].overridePaletteCount = 0xFF;
            } else {
                arg0->playerMarkers[i].overridePaletteCount = 0;
            }
        } else if (state == 0) {
            arg0->pulseTimers[i] = arg0->pulseTimers[i] % 30;
            if (arg0->pulseTimers[i] < 15) {
                arg0->playerMarkers[i].color.paletteAndAlpha = arg0->playerMarkers[i].color.paletteAndAlpha - 8;
            } else {
                arg0->playerMarkers[i].color.paletteAndAlpha = arg0->playerMarkers[i].color.paletteAndAlpha + 8;
            }
            arg0->pulseTimers[i] = arg0->pulseTimers[i] + 1;
        } else {
            arg0->pulseTimers[i] = 0;
            arg0->playerMarkers[i].color.paletteAndAlpha = 0xFF;
        }

        if (allocation->modeData.storyMap.selectionState[i] == 2) {
            allocation->modeData.characterSelect.playerBlinkTimers[i] = 0;
            arg0->playerMarkers[i].overridePaletteCount = 0;
            arg0->pulseTimers[i] = 0;
            arg0->playerMarkers[i].color.paletteAndAlpha = 0xFF;
        }

        enqueueCallbackBySlotIndex(8, 0, renderTextSprite, &arg0->playerMarkers[i]);
    }
}

void cleanupCharacterSelectSprites(CharacterSelectSprites *arg0) {
    arg0->playerMarkers[0].spriteData = freeNodeMemory(arg0->playerMarkers[0].spriteData);
}

void initCoordinateDisplayTask(CoordinateDisplayTaskState *arg0) {
    arg0->selectedCoordinate.x = -7;
    arg0->selectedCoordinate.y = -7;
    arg0->coordinateLabel.x = -120;
    arg0->coordinateLabel.y = -104;
    arg0->coordinateLabel.string = arg0->formattedText;
    arg0->coordinateLabel.palette = 0;
    arg0->selectionMarker.palette = 0;
    arg0->selectionMarker.string = D_800B115C_1DB6FC;
    arg0->selectionMarker.x = arg0->selectedCoordinate.x;
    arg0->selectionMarker.y = arg0->selectedCoordinate.y;
    setCallback(updateCoordinateDisplayTask);
}

void updateCoordinateDisplayTask(CoordinateDisplayTaskState *arg0) {
    s32 *buttons = &gButtonsPressed;

    if (*buttons & U_JPAD) {
        arg0->selectedCoordinate.y--;
    }
    if (*buttons & D_JPAD) {
        arg0->selectedCoordinate.y++;
    }
    if (*buttons & R_JPAD) {
        arg0->selectedCoordinate.x++;
    }
    if (*buttons & L_JPAD) {
        arg0->selectedCoordinate.x--;
    }

    sprintf(
        (char *)arg0->formattedText,
        gCoordDisplayFormatString,
        arg0->selectedCoordinate.x + 7,
        arg0->selectedCoordinate.y + 7
    );

    enqueueCallbackBySlotIndex(0, 7, &renderTextPalette, &arg0->coordinateLabel);

    arg0->selectionMarker.x = arg0->selectedCoordinate.x;
    arg0->selectionMarker.y = arg0->selectedCoordinate.y;

    enqueueCallbackBySlotIndex(0, 7, &renderTextPalette, &arg0->selectionMarker);
}

void cleanupCharacterSelectTextureData(CharacterSelectBackgroundState *arg0) {
    arg0->textureAsset = freeNodeMemory(arg0->textureAsset);
}

void initCharacterSelectTextureDataLoad(CharacterSelectBackgroundState *arg0) {
    arg0->textureAsset =
        loadCompressedData(&characterSelectBoardTexture_ROM_START, &characterSelectBoardTexture_ROM_END, 0x13FF0);
    setCleanupCallback(cleanupCharacterSelectTextureData);
    setCallback(initCharacterSelectTextureRenderState);
}

void initCharacterSelectTextureRenderState(CharacterSelectBackgroundState *arg0) {
    initScrollingTileMapState(&arg0->renderState, arg0->textureAsset);
    setCallback(&enqueueCharacterSelectTextureRender);
}

void enqueueCharacterSelectTextureRender(void *arg0) {
    CharacterSelectBackgroundState *state = arg0;

    enqueueCallbackBySlotIndex(9, 0, renderTiledTextureMap, &state->renderState);
}

void initPlayer3CharacterSelectIndicator(SpriteRenderArg *arg0) {
    void *temp;
    getCurrentAllocation();
    temp = loadCompressedData(&okPromptSprites_ROM_START, &okPromptSprites_ROM_END, 0x1B48);
    arg0->x = -44;
    arg0->y = -20;
    arg0->frameIndex = 13;
    arg0->spriteData = temp;
    setCleanupCallback(cleanupCharacterSelectIndicator);
    setCallbackWithContinue(updatePlayer3CharacterSelectIndicator);
}

void updatePlayer3CharacterSelectIndicator(void *arg0) {
    GameState *state = getCurrentAllocation();
    if (state->modeData.storyMap.selectionState[0] == 3) {
        enqueueCallbackBySlotIndex(8, 0, renderSpriteFrame, arg0);
    } else {
        terminateCurrentTask();
    }
}

void cleanupCharacterSelectIndicator(SpriteRenderArg *arg0) {
    arg0->spriteData = freeNodeMemory(arg0->spriteData);
}

void initPlayer2CharacterSelectIndicator(SpriteRenderArg *arg0) {
    void *temp;
    getCurrentAllocation();
    temp = loadCompressedData(&okPromptSprites_ROM_START, &okPromptSprites_ROM_END, 0x1B48);
    arg0->x = -76;
    arg0->y = -8;
    arg0->frameIndex = 12;
    arg0->spriteData = temp;
    setCleanupCallback(cleanupCharacterSelectIndicatorData);
    setCallbackWithContinue(updatePlayer2CharacterSelectIndicator);
}

void updatePlayer2CharacterSelectIndicator(void *arg0) {
    GameState *state = getCurrentAllocation();
    if (state->modeData.storyMap.selectionState[0] == 2) {
        enqueueCallbackBySlotIndex(8, 0, renderSpriteFrame, arg0);
    } else {
        terminateCurrentTask();
    }
}

void cleanupCharacterSelectIndicatorData(SpriteRenderArg *arg0) {
    arg0->spriteData = freeNodeMemory(arg0->spriteData);
}

void initCharacterSelectBoardTask(DisplayListObject *arg0) {
    void *texture1;
    void *texture2;

    texture1 = loadUncompressedData(&characterSelectPrimaryTexture_ROM_START, &characterSelectPrimaryTexture_ROM_END);
    texture2 =
        loadUncompressedData(&characterSelectSecondaryTexture_ROM_START, &characterSelectSecondaryTexture_ROM_END);
    setCleanupCallback(cleanupCharacterSelectBoardTask);

    memcpy(arg0, &identityMatrix, sizeof(Transform3D));
    arg0->displayLists = (DisplayLists *)&D_800B1140_1DB6E0;
    arg0->transform.translation.x = 0x2C0000;
    arg0->transform.translation.z = (s32)0xFF9F0000;
    arg0->segment1 = texture1;
    arg0->segment2 = texture2;
    arg0->segment3 = NULL;
    arg0->transform.translation.y = (s32)0xFFF40000;

    createZRotationMatrix(&arg0->transform, 0x1F50);
    setCallback(renderCharacterSelectBoard);
}

void renderCharacterSelectBoard(void *arg0) {
    getCurrentAllocation();
    enqueueDisplayListObjectWithFullRenderState(0, arg0);
}

void cleanupCharacterSelectBoardTask(DisplayListObject *arg0) {
    arg0->segment1 = freeNodeMemory(arg0->segment1);
    arg0->segment2 = freeNodeMemory(arg0->segment2);
}
