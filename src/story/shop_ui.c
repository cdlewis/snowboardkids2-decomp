#include "story/shop_ui.h"
#include "assets.h"
#include "common.h"
#include "common_bss.h"
#include "data/asset_metadata.h"
#include "data/course_data.h"
#include "font_encoding.h"
#include "gamestate.h"
#include "graphics/clip_text_render.h"
#include "graphics/displaylist.h"
#include "graphics/graphics.h"
#include "graphics/sprite_rdp.h"
#include "math/geometry.h"
#include "os_cont.h"
#include "race/race_session.h"
#include "system/task_scheduler.h"
#include "text/font_assets.h"
#include "text/text_layout.h"
#include "ui/level_preview_3d.h"
#include "ui/save_data.h"

struct StoryMapShopBackgroundState {
    u8 padding[0x2C];
    void *backgroundAsset;
};

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    /* 0x08 */ char *cameraYString;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    /* 0x14 */ char *cameraYRotationString;
    /* 0x18 */ s32 cameraY;
    /* 0x1C */ s16 cameraRotation;
    /* 0x1E */ char unk1E;
    u8 padding3[0x1F];
    /* 0x3E */ char unk3E;
} cameraState;

typedef struct {
    u8 padding[0x3C];
    s32 unk3C;
    u8 padding2[0x10];
    u32 unk50;
    u8 padding3[0xC];
    u8 unk60;
} S0;

typedef struct {
    SpriteRenderArg digits[6];
    u8 unk48[0x18];
    char priceBuffer[8];
} StoryMapShopItemPriceDisplayState;

struct StoryMapShopFairyState {
    SceneModel *model;
    Transform3D transform;
    s16 animationFrame;
    s8 animationType;
};

struct StoryMapShopGoldDisplayState {
    SpriteRenderArg digits[7];
    SpriteRenderArg goldIcon;
    char goldAmountBuffer[8];
};

typedef struct {
    s16 itemOffsets[2];
    struct {
        u16 start;
        u16 duration;
    } fairyAnim[5];
} ShopItemData;

extern s32 gButtonsPressed[];
extern s32 *gGameSessionContext;
extern s16 D_8008F0C6_8FCC6[];
// Cast to u8* to access EEPROM save data as raw bytes for offset-based access
extern u8 *EepromSaveData;

s32 D_8008F070_8FC70[16] = {
    0x00000000, 0x0000C350, 0x000186A0, 0x00000000, 0x0000AFC8, 0x00015F90, 0x00000000, 0x0000EA60,
    0x0001D4C0, 0x000249F0, 0x00001388, 0x000003E8, 0x00013880, 0x00000064, 0x00013880, 0x0000C350,
};

struct {
    s16 itemOffsets[2];
    struct {
        u16 start;
        u16 duration;
    } fairyAnim[4];
    s16 D_8008F0C6_8FCC6_field[38];
} D_8008F0B0_8FCB0 = {
    { 0x0003, 0x0D40 },
    { { 0x0006, 0x1A80 }, { 0x0005, 0x0001 }, { 0x0008, 0x0003 }, { 0x000B, 0x0001 } },
    {
     0x0016,  0x0027, 0x0010,  0x0020, 0x0028,  0x0013, 0x0018,  0x0033,  0x0022,  0x0000,  0x0015,  0x000D,  0x0018,
     0x0010,  0x000C, 0x0016,  0x0014, 0x0007,  0x000F, 0x0000,  -0x0028, 0x0028,  -0x0024, 0x0020,  -0x0024, 0x0020,
     -0x0020, 0x0018, -0x0024, 0x0020, -0x0020, 0x0018, -0x0024, 0x0020,  -0x0024, 0x0020,  -0x0024, 0x0020,
     },
};

u8 boardShopChooseBoardPromptText[64] = { _("Which board do you want@") };

void updateStoryMapShopGoldDisplay(StoryMapShopGoldDisplayState *);
void cleanupStoryMapShopGoldDisplay(StoryMapShopGoldDisplayState *arg0);
void updateStoryMapShopItemPriceDisplay(StoryMapShopItemPriceDisplayState *arg0);
void cleanupStoryMapShopItemPriceDisplay(StoryMapShopItemPriceDisplayState *);
void updateStoryMapShopFairyInitial(StoryMapShopFairyState *);
void updateStoryMapShopFairy(StoryMapShopFairyState *);
void destroyStoryMapShopFairy(StoryMapShopFairyState *);
void updateStoryMapShopItemCard(StoryMapShopItemCardState *);
void transitionStoryMapShopItemCard(StoryMapShopItemCardState *);
void reloadStoryMapShopItemCard(StoryMapShopItemCardState *);
void slideStoryMapShopItemCard(StoryMapShopItemCardState *);
void awaitStoryMapShopItemCardIdle(DisplayListObject *);
void destroyStoryMapShopItemCard(StoryMapShopItemCardState *);
void prepareSlideInStoryMapShopItemCard(StoryMapShopItemCardState *);
void displaySlideInStoryMapShopItemCard(DisplayListObject *);
void animateSlideInStoryMapShopItemCard(StoryMapShopItemCardState *arg0);
void awaitSlideInStoryMapShopItemCardIdle(void);
void destroySlideInStoryMapShopItemCard(StoryMapShopItemCardState *);
void initStoryMapShopBackgroundRenderState(StoryMapShopBackgroundState *);
void enqueueStoryMapShopBackgroundRender(void *);
void cleanupStoryMapShopBackground(StoryMapShopBackgroundState *);
void updateUnlockScreenScrollArrows(UnlockScreenScrollArrowsState *);
void cleanupUnlockScreenScrollArrows(UnlockScreenScrollArrowsState *state);
void updateStoryMapShopItemIcon(TextRenderArg *);
void cleanupStoryMapShopItemIcon(TextRenderArg *);
void updateStoryMapShopItemStatLabel(TextRenderArg *);
void cleanupStoryMapShopItemStatLabel(TextRenderArg *);
void updateStoryMapShopExitOverlay(void *arg0);
void cleanupStoryMapShopExitOverlay(SpriteRenderArg *arg0);
void updateStoryMapShopItemStatsDisplay(UnlockScreenItemStatsDisplay *arg0);
void updateStoryMapShopSoldOutLabel(void *);
void cleanupStoryMapShopSoldOutLabel(SpriteRenderArg *);
void drawUnlockScreenItemIcons(void *);
void cleanupUnlockScreenItemIcons(UnlockScreenItemIconsState *);
void updateDebugCameraYState(cameraState *arg0);

void *loadTextRenderAsset(s32);
void initDebugCameraController(cameraState *arg0) {
    arg0->cameraY = 0;
    arg0->cameraRotation = 0;
    arg0->unk0 = -0x80;
    arg0->unk2 = -0x58;
    arg0->cameraYString = &arg0->unk3E;
    arg0->unkE = -0x50;
    arg0->unk4 = 0;
    arg0->unkC = -0x80;
    arg0->unk10 = 0;
    arg0->cameraYRotationString = &arg0->unk1E;

    setCallback(&updateDebugCameraYState);
}

void updateDebugCameraYState(cameraState *arg0) {
    s32 sp20[8];
    u64 pad;
    char *cameraYRotation;
    char *cameraYString;
    GameState *temp_s0 = (GameState *)getCurrentAllocation();

    if (gButtonsPressed[1] & R_TRIG) {
        arg0->cameraRotation++;
    } else if (gButtonsPressed[1] & L_TRIG) {
        arg0->cameraRotation--;
    }
    do {
        arg0->cameraRotation = arg0->cameraRotation & 0x1FFF;
    } while (0);
    if (gControllerInputs[1] & U_JPAD) {
        arg0->cameraY--;
    } else if (gControllerInputs[1] & D_JPAD) {
        arg0->cameraY++;
    }
    createViewportTransform(&sp20, 0, arg0->cameraY << 0x10, 0x600000, 0, (s32)(arg0->cameraRotation), 0);
    setViewportTransformById(temp_s0->unkDA, &sp20);

    cameraYString = &arg0->unk3E;
    sprintf(cameraYString, "CAMERA Y = %d", arg0->cameraY);
    cameraYRotation = &arg0->unk1E;
    sprintf(cameraYRotation, "CAMERA RX = %X", arg0->cameraRotation);
    arg0->cameraYString = cameraYString;
    arg0->cameraYRotationString = cameraYRotation;

    enqueueCallbackBySlotIndex(8, 7, &renderTextPalette, arg0);
    enqueueCallbackBySlotIndex(8, 7, &renderTextPalette, &arg0->unkC);
}

void initStoryMapShopFairyModel(StoryMapShopFairyState *arg0) {
    GameState *state = (GameState *)getCurrentAllocation();

    arg0->model = createSceneModel(0x3A, state);
    memcpy(&arg0->transform, &identityMatrix, sizeof(Transform3D));
    arg0->transform.translation.x = 0x200000;
    arg0->transform.translation.y = 0xFFE00000;
    arg0->transform.translation.z = 0x80000;
    arg0->animationFrame = 4;
    arg0->animationType = 0;
    createYRotationMatrix(&arg0->transform, 0x1E00);
    setCleanupCallback(destroyStoryMapShopFairy);
    setCallback(updateStoryMapShopFairyInitial);
}

void updateStoryMapShopFairyInitial(StoryMapShopFairyState *arg0) {
    applyTransformToModel(arg0->model, &arg0->transform);
    setModelAnimation(arg0->model, arg0->animationFrame);
    updateModelGeometry(arg0->model);
    setCallback(&updateStoryMapShopFairy);
}

void updateStoryMapShopFairy(StoryMapShopFairyState *fairy) {
    GameState *state;
    u8 animIndex;
    u16 frameCounter;
    s32 idx;
    volatile u8 pad[8];

    state = (GameState *)getCurrentAllocation();
    applyTransformToModel(fairy->model, &fairy->transform);
    do {
        if (clearModelRotation(fairy->model) != 0) {
            animIndex = fairy->animationType;
            idx = animIndex * 2;
            if (animIndex != 0) {
                frameCounter = fairy->animationFrame + 1;
                fairy->animationFrame = frameCounter;
                if (frameCounter == (u16)(D_8008F0B0_8FCB0.fairyAnim[animIndex].start +
                                          D_8008F0B0_8FCB0.fairyAnim[animIndex].duration)) {
                    fairy->animationType = 0;
                    fairy->animationFrame = 4;
                }
                setModelAnimation(fairy->model, fairy->animationFrame);
            }
        }
    } while (0);
    updateModelGeometry(fairy->model);
    animIndex = state->modeData.unlockScreen.pendingFairyAnimation;
    if (animIndex != 0) {
        u16 start;
        fairy->animationType = animIndex;
        start = D_8008F0B0_8FCB0.fairyAnim[state->modeData.unlockScreen.pendingFairyAnimation].start;
        fairy->animationFrame = start;
        setModelAnimation(fairy->model, (s16)start);
        state->modeData.unlockScreen.pendingFairyAnimation = 0;
    }
}

void destroyStoryMapShopFairy(StoryMapShopFairyState *arg0) {
    destroySceneModel(arg0->model);
}

void initStoryMapShopItemCard(StoryMapShopItemCardState *card) {
    Transform3D rotationYX;
    Transform3D rotationZ;
    s16 itemId;
    u16 itemData;
    Transform3D *cardTransform;
    GameState *state;
    Transform3D *rotationZPtr;
    Transform3D *rotationYXPtr;

    cardTransform = &card->transform;
    state = (GameState *)getCurrentAllocation();
    memcpy(cardTransform, &identityMatrix, sizeof(Transform3D));
    rotationZPtr = &rotationZ;
    memcpy(rotationZPtr, cardTransform, sizeof(Transform3D));
    rotationYXPtr = &rotationYX;
    memcpy(rotationYXPtr, rotationZPtr, sizeof(Transform3D));
    createRotationMatrixYX(rotationYXPtr, 0x1000, 0x800);
    createZRotationMatrix(rotationZPtr, 0x1F00);
    composeTransform3D(rotationYXPtr, rotationZPtr, cardTransform);

    if (state->modeData.unlockScreen.unlockedItemCount != 1) {
        card->transform.translation.x = 0x200000 - ((2 - card->slotPosition) << 21);
    }

    itemData = state->modeData.unlockScreen.itemIds[card->itemIndex];
    memcpy(&card->displayList.transform, cardTransform, sizeof(Transform3D));
    do {
        itemId = itemData & 0x1F;
        itemData = itemId;
    } while (0);

    card->displayList.displayLists = loadAssetByIndex_95728(itemId);
    card->displayList.segment1 = loadAssetByIndex_95500(itemId);
    card->displayList.segment2 = loadAssetByIndex_95590(itemId);
    card->displayList.segment3 = loadAssetByIndex_95668(itemData / 3);
    card->updateCounter = 0;

    setCleanupCallback(destroyStoryMapShopItemCard);
    setCallback(updateStoryMapShopItemCard);
}

void updateStoryMapShopItemCard(StoryMapShopItemCardState *card) {
    volatile u8 padding[0x20];
    GameState *state;

    state = (GameState *)getCurrentAllocation();

    if (state->modeData.unlockScreen.screenPhase == 2) {
        if (state->modeData.unlockScreen.scrollDirection == 2) {
            if (card->slotPosition == 0) {
                card->slotPosition = 2;
                card->transform.translation.x = 0x400000;
                card->updateCounter = 1;
                goto end;
            }
        }
        if (state->modeData.unlockScreen.scrollDirection == 1) {
            if (card->slotPosition == 2) {
                card->transform.translation.x = 0xFFC00000;
                card->slotPosition = 0;
                card->updateCounter = 1;
                goto end;
            }
        }
        if (state->modeData.unlockScreen.scrollDirection == 2) {
            card->slotPosition = card->slotPosition - 1;
        } else {
            card->slotPosition = card->slotPosition + 1;
        }
        card->updateCounter = 0;

    end:
        setCallback(&transitionStoryMapShopItemCard);
    }

    enqueueDisplayListObject(0, &card->displayList);
}

void transitionStoryMapShopItemCard(StoryMapShopItemCardState *card) {
    volatile u8 padding[0x20];

    getCurrentAllocation();
    if (card->updateCounter != 0) {
        card->displayList.segment1 = freeNodeMemory(card->displayList.segment1);
        card->displayList.segment2 = freeNodeMemory(card->displayList.segment2);
        card->displayList.segment3 = freeNodeMemory(card->displayList.segment3);
        if (card->updateCounter != 0) {
            goto end;
        }
    }
    enqueueDisplayListObject(0, &card->displayList);
end:
    setCallback(&reloadStoryMapShopItemCard);
}

void reloadStoryMapShopItemCard(StoryMapShopItemCardState *card) {
    volatile u8 padding[0x20];
    GameState *state = (GameState *)getCurrentAllocation();
    s8 itemId;
    s8 itemIndex;

    if (card->updateCounter == 1) {
        if (state->modeData.unlockScreen.scrollDirection == 2) {
            card->itemIndex = state->modeData.unlockScreen.selectedItemIndex + 1;
            if (card->itemIndex == state->modeData.unlockScreen.unlockedItemCount) {
                card->itemIndex = 0;
            }
        } else {
            card->itemIndex = state->modeData.unlockScreen.selectedItemIndex - 1;
            if (card->itemIndex < 0) {
                card->itemIndex = state->modeData.unlockScreen.unlockedItemCount - 1;
            }
        }

        itemId = state->modeData.unlockScreen.itemIds[card->itemIndex] & 0x1F;
        itemIndex = itemId;

        memcpy(&card->displayList.transform, &card->transform, sizeof(Transform3D));

        card->displayList.displayLists = loadAssetByIndex_95728(itemIndex);
        card->displayList.segment1 = loadAssetByIndex_95500(itemIndex);
        card->displayList.segment2 = loadAssetByIndex_95590(itemIndex);
        card->displayList.segment3 = loadAssetByIndex_95668(itemId / 3);
        card->updateCounter = 0;
    } else {
        enqueueDisplayListObject(0, (DisplayListObject *)card);
    }

    setCallback(&slideStoryMapShopItemCard);
}

void slideStoryMapShopItemCard(StoryMapShopItemCardState *card) {
    volatile u8 padding[0x20];
    u32 scrollDirection;
    GameState *state = (GameState *)getCurrentAllocation();
    u32 translationStep;
    scrollDirection = state->modeData.unlockScreen.scrollDirection;

    if (scrollDirection == 2) {
        translationStep = 0xFFF80000;
    } else {
        translationStep = 0x00080000;
    }

    card->transform.translation.x += translationStep;

    memcpy(&card->displayList.transform, &card->transform, sizeof(Transform3D));

    card->updateCounter++;
    if (card->updateCounter == 4) {
        card->updateCounter = 0;
        state->modeData.unlockScreen.completedScrollSteps++;
        setCallback(awaitStoryMapShopItemCardIdle);
    }

    enqueueDisplayListObject(0, &card->displayList);
}

void awaitStoryMapShopItemCardIdle(DisplayListObject *displayList) {
    volatile u8 pad[0x20];
    if (((GameState *)getCurrentAllocation())->modeData.unlockScreen.screenPhase == 1) {
        setCallback(&updateStoryMapShopItemCard);
    }
    enqueueDisplayListObject(0, displayList);
}

void destroyStoryMapShopItemCard(StoryMapShopItemCardState *card) {
    card->displayList.segment1 = freeNodeMemory(card->displayList.segment1);
    card->displayList.segment2 = freeNodeMemory(card->displayList.segment2);
    card->displayList.segment3 = freeNodeMemory(card->displayList.segment3);
}

void initSlideInStoryMapShopItemCard(StoryMapShopItemCardState *card) {
    Transform3D rotationYX;
    Transform3D rotationZ;
    Transform3D *rotationYXPtr;
    Transform3D *cardTransform;

    getCurrentAllocation();
    rotationYXPtr = &rotationYX;
    cardTransform = &card->transform;
    memcpy(cardTransform, &identityMatrix, sizeof(Transform3D));
    memcpy(&rotationZ, cardTransform, sizeof(Transform3D));
    memcpy(rotationYXPtr, &rotationZ, sizeof(Transform3D));
    createRotationMatrixYX(&rotationYX, 0x1000, 0x800);
    createZRotationMatrix(&rotationZ, 0x1F00);
    composeTransform3D(&rotationYX, &rotationZ, cardTransform);
    card->updateCounter = 0;
    card->displayList.displayLists = NULL;
    card->displayList.segment1 = NULL;
    card->displayList.segment2 = NULL;
    card->displayList.segment3 = 0;
    setCleanupCallback(&destroySlideInStoryMapShopItemCard);
    setCallback(&prepareSlideInStoryMapShopItemCard);
}

void prepareSlideInStoryMapShopItemCard(StoryMapShopItemCardState *card) {
    GameState *state;
    s8 wrappedItemIndex;
    u8 itemData;
    s16 itemId;

    state = (GameState *)getCurrentAllocation();
    if (state->modeData.unlockScreen.screenPhase == 2) {
        if (state->modeData.unlockScreen.scrollDirection == 1) {
            card->transform.translation.x = 0x200000;
            card->translationStep = 0x80000;
            wrappedItemIndex = state->modeData.unlockScreen.selectedItemIndex + 2;
            card->itemIndex = wrappedItemIndex;
            if (wrappedItemIndex >= (s32)state->modeData.unlockScreen.unlockedItemCount) {
                card->itemIndex = wrappedItemIndex - state->modeData.unlockScreen.unlockedItemCount;
            }
        } else {
            card->transform.translation.x = 0xFFE00000;
            card->translationStep = 0xFFF80000;
            wrappedItemIndex = state->modeData.unlockScreen.selectedItemIndex - 2;
            card->itemIndex = wrappedItemIndex;
            if (wrappedItemIndex < 0) {
                card->itemIndex = state->modeData.unlockScreen.unlockedItemCount + wrappedItemIndex;
            }
        }
        itemData = state->modeData.unlockScreen.itemIds[card->itemIndex];
        memcpy(&card->displayList.transform, &card->transform, sizeof(Transform3D));
        itemData &= 0x1F;
        itemId = itemData;
        card->displayList.displayLists = loadAssetByIndex_95728(itemId);
        card->displayList.segment1 = loadAssetByIndex_95500(itemId);
        card->displayList.segment2 = loadAssetByIndex_95590(itemId);
        card->displayList.segment3 = loadAssetByIndex_95668(itemData / 3);
        setCallback(displaySlideInStoryMapShopItemCard);
    }
}

void displaySlideInStoryMapShopItemCard(DisplayListObject *arg0) {
    enqueueDisplayListObject(0, arg0);
    setCallback(animateSlideInStoryMapShopItemCard);
}

void animateSlideInStoryMapShopItemCard(StoryMapShopItemCardState *card) {
    GameState *state = (GameState *)getCurrentAllocation();

    card->transform.translation.x += card->translationStep;
    memcpy(&card->displayList.transform, &card->transform, sizeof(Transform3D));

    card->updateCounter++;
    if (card->updateCounter == 4) {
        state->modeData.unlockScreen.completedScrollSteps++;
        card->updateCounter = 0;
        card->displayList.segment1 = freeNodeMemory(card->displayList.segment1);
        card->displayList.segment2 = freeNodeMemory(card->displayList.segment2);
        card->displayList.segment3 = freeNodeMemory(card->displayList.segment3);
        setCallbackWithContinue(awaitSlideInStoryMapShopItemCardIdle);
    } else {
        enqueueDisplayListObject(0, &card->displayList);
    }
}

void awaitSlideInStoryMapShopItemCardIdle(void) {
    GameState *state = (GameState *)getCurrentAllocation();
    if (state->modeData.unlockScreen.screenPhase != 2) {
        setCallbackWithContinue(prepareSlideInStoryMapShopItemCard);
    }
}

void destroySlideInStoryMapShopItemCard(StoryMapShopItemCardState *card) {
    card->displayList.segment1 = freeNodeMemory(card->displayList.segment1);
    card->displayList.segment2 = freeNodeMemory(card->displayList.segment2);
    card->displayList.segment3 = freeNodeMemory(card->displayList.segment3);
}

void loadStoryMapShopBackground(StoryMapShopBackgroundState *state) {
    state->backgroundAsset = loadCompressedData(&shopBackgroundAsset_ROM_START, &shopBackgroundAsset_ROM_END, 0x14410);
    setCleanupCallback(&cleanupStoryMapShopBackground);
    setCallback(&initStoryMapShopBackgroundRenderState);
}

void initStoryMapShopBackgroundRenderState(StoryMapShopBackgroundState *state) {
    initScrollingTileMapState(state, (s32)state->backgroundAsset);
    setCallback(&enqueueStoryMapShopBackgroundRender);
}

void enqueueStoryMapShopBackgroundRender(void *state) {
    enqueueCallbackBySlotIndex(1, 0, renderTiledTextureMap, state);
}

void cleanupStoryMapShopBackground(StoryMapShopBackgroundState *state) {
    state->backgroundAsset = freeNodeMemory(state->backgroundAsset);
}

void initUnlockScreenScrollArrows(UnlockScreenScrollArrowsState *state) {
    s32 i;
    void *asset = loadCompressedData(&menuUiSprites_ROM_START, &menuUiSprites_ROM_END, 0x8A08);

    setCleanupCallback(&cleanupUnlockScreenScrollArrows);

    for (i = 0; i < 2; i++) {
        state->arrows[i].x = -0x40 + i * 0x60;
        state->arrows[i].y = -0x18;
        state->arrows[i].frameIndex = i;
        state->arrows[i].spriteData = asset;
        state->arrows[i].color.paletteAndAlpha = 0xFF;
        state->arrows[i].overridePaletteCount = 0;
        state->arrows[i].tileMode = 0;
    }

    state->animationCounter = 0;

    setCallback(&updateUnlockScreenScrollArrows);
}

void updateUnlockScreenScrollArrows(UnlockScreenScrollArrowsState *arrowState) {
    GameState *state = getCurrentAllocation();
    s32 i;

    if (state->modeData.unlockScreen.screenPhase > 0 && state->modeData.unlockScreen.screenPhase < 4) {
        if (state->modeData.unlockScreen.screenPhase == 1) {
            arrowState->animationCounter++;
            if (state->modeData.unlockScreen.unlockedItemCount >= 3) {
                if ((u8)(arrowState->animationCounter) < 0x11) {
                    arrowState->arrows[0].color.paletteAndAlpha -= 8;
                    arrowState->arrows[1].color.paletteAndAlpha -= 8;
                } else {
                    arrowState->arrows[0].color.paletteAndAlpha += 8;
                    arrowState->arrows[1].color.paletteAndAlpha += 8;
                }
            } else if (state->modeData.unlockScreen.unlockedItemCount == 2) {
                if ((u8)(arrowState->animationCounter) < 0x11) {
                    if (state->modeData.unlockScreen.selectedItemIndex == 1) {
                        arrowState->arrows[1].color.paletteAndAlpha = 0xFF;
                        arrowState->arrows[0].color.paletteAndAlpha -= 8;
                    } else {
                        arrowState->arrows[0].color.paletteAndAlpha = 0xFF;
                        arrowState->arrows[1].color.paletteAndAlpha -= 8;
                    }
                } else {
                    if (state->modeData.unlockScreen.selectedItemIndex == 1) {
                        arrowState->arrows[1].color.paletteAndAlpha = 0xFF;
                        arrowState->arrows[0].color.paletteAndAlpha += 8;
                    } else {
                        arrowState->arrows[0].color.paletteAndAlpha = 0xFF;
                        arrowState->arrows[1].color.paletteAndAlpha += 8;
                    }
                }
            } else {
                arrowState->animationCounter = 0;
                arrowState->arrows[0].color.paletteAndAlpha = 0xFF;
                arrowState->arrows[1].color.paletteAndAlpha = 0xFF;
            }
        } else {
            arrowState->animationCounter = 0;
            arrowState->arrows[0].color.paletteAndAlpha = 0xFF;
            arrowState->arrows[1].color.paletteAndAlpha = 0xFF;
        }

        arrowState->animationCounter &= 0x1F;

        for (i = 0; i < 2; i++) {
            enqueueCallbackBySlotIndex(8, 0, &renderTextSprite, &arrowState->arrows[i]);
        }
    }
}

void cleanupUnlockScreenScrollArrows(UnlockScreenScrollArrowsState *state) {
    state->arrows[0].spriteData = freeNodeMemory(state->arrows[0].spriteData);
}

void initStoryMapShopItemIcon(TextRenderArg *iconState) {
    void *dmaResult;
    GameState *state;
    u8 itemValue;

    state = getCurrentAllocation();
    dmaResult = loadCompressedData(&menuUiSprites_ROM_START, &menuUiSprites_ROM_END, 0x8A08);
    setCleanupCallback(cleanupStoryMapShopItemIcon);

    iconState->y = -0x18;

    itemValue = state->modeData.unlockScreen.itemIds[state->modeData.unlockScreen.selectedItemIndex];

    if (itemValue < 9) {
        u8 tempValue;
        iconState->x = -0x30;
        tempValue = state->modeData.unlockScreen.itemIds[state->modeData.unlockScreen.selectedItemIndex];
        iconState->frameIndex = (tempValue / 3) + 0x1D;
    } else {
        s16 tableVal = D_8008F0B0_8FCB0.D_8008F0C6_8FCC6_field[itemValue - 9] + 0x18;
        s16 tableVal2 = D_8008F0C6_8FCC6[itemValue];

        iconState->frameIndex = itemValue + 0x23;
        iconState->x = ((0x120 - tableVal) / 2) - tableVal2 - 0x96;
    }

    iconState->color.paletteAndAlpha = 0xFF;
    iconState->tileMode = 0;
    iconState->overridePaletteCount = 0;
    iconState->spriteData = dmaResult;

    setCallback(updateStoryMapShopItemIcon);
}

void updateStoryMapShopItemIcon(TextRenderArg *iconState) {
    GameState *state;
    s32 pad;
    u8 itemValue;
    state = (GameState *)getCurrentAllocation();
    itemValue = state->modeData.unlockScreen.itemIds[state->modeData.unlockScreen.selectedItemIndex];
    if (itemValue < 0x80) {
        if ((state->modeData.unlockScreen.screenPhase != 0) && (state->modeData.unlockScreen.screenPhase != 2)) {
            u8 masked = itemValue & 0x1F;
            itemValue = masked;
            if (itemValue < 9) {
                iconState->x = -0x30;
                iconState->frameIndex = (masked / 3) + 0x1D;
            } else {
                s16 tableVal = D_8008F0B0_8FCB0.D_8008F0C6_8FCC6_field[masked - 9];
                s16 tableVal2 = D_8008F0C6_8FCC6[masked];
                iconState->frameIndex = masked + 0x23;
                iconState->x = (((0x120 - ((s16)(tableVal + 0x18))) / 2) - tableVal2) - 0x96;
            }
            if (state->modeData.unlockScreen.screenPhase == 3) {
                if (state->modeData.unlockScreen.frameCounter & 1) {
                    iconState->overridePaletteCount = 0xFF;
                } else {
                    iconState->overridePaletteCount = 0;
                }
            }
            itemValue = 0;
            enqueueCallbackBySlotIndex(8, itemValue, &renderTextSprite, iconState);
        }
    }
}

void cleanupStoryMapShopItemIcon(TextRenderArg *iconState) {
    iconState->spriteData = freeNodeMemory(iconState->spriteData);
}

void initStoryMapShopItemStatLabel(TextRenderArg *arg0) {
    GameState *state;
    void *dmaResult;
    u8 itemValue;

    state = (GameState *)getCurrentAllocation();
    dmaResult = loadCompressedData(&menuUiSprites_ROM_START, &menuUiSprites_ROM_END, 0x8A08);
    setCleanupCallback(cleanupStoryMapShopItemStatLabel);

    arg0->y = -0x18;

    itemValue = state->modeData.unlockScreen.itemIds[state->modeData.unlockScreen.selectedItemIndex];

    if (itemValue < 9) {
        arg0->x = 0x12;
        arg0->frameIndex = ((itemValue % 3) & 0xFF) + 0x24;
    } else {
        s16 tableVal = D_8008F0B0_8FCB0.D_8008F0C6_8FCC6_field[itemValue - 9];
        arg0->frameIndex = 0x35;
        arg0->x = tableVal + ((0x120 - (s16)(tableVal + 0x18)) / 2) - 0x96;
    }

    arg0->color.paletteAndAlpha = 0xFF;
    arg0->tileMode = 0;
    arg0->overridePaletteCount = 0;
    arg0->spriteData = dmaResult;

    setCallback(updateStoryMapShopItemStatLabel);
}

void updateStoryMapShopItemStatLabel(TextRenderArg *arg0) {
    GameState *state = (GameState *)getCurrentAllocation();
    u8 itemValue;

    itemValue = state->modeData.unlockScreen.itemIds[state->modeData.unlockScreen.selectedItemIndex];

    if (itemValue >= 0x80) {
        return;
    }

    if (state->modeData.unlockScreen.screenPhase != 0 && state->modeData.unlockScreen.screenPhase != 2) {
        if (itemValue < 9) {
            arg0->x = 0x12;
            arg0->frameIndex = ((itemValue & 0x1F) % 3) + 0x24;
        } else {
            s16 tableVal = D_8008F0B0_8FCB0.D_8008F0C6_8FCC6_field[itemValue - 9];
            arg0->frameIndex = 0x35;
            arg0->x = tableVal + ((0x120 - (s16)(tableVal + 0x18)) / 2) - 0x96;
        }

        if (state->modeData.unlockScreen.screenPhase == 3) {
            if (state->modeData.unlockScreen.frameCounter & 1) {
                arg0->overridePaletteCount = 0xFF;
            } else {
                arg0->overridePaletteCount = 0;
            }
        }

        enqueueCallbackBySlotIndex(8, 0, &renderTextSprite, arg0);
    }
}

void cleanupStoryMapShopItemStatLabel(TextRenderArg *arg0) {
    arg0->spriteData = freeNodeMemory(arg0->spriteData);
}

void initStoryMapShopExitOverlay(SpriteRenderArg *arg0) {
    void *overlayAsset = loadCompressedData(&okPromptSprites_ROM_START, &okPromptSprites_ROM_END, 0x1B48);

    setCleanupCallback(&cleanupStoryMapShopExitOverlay);

    arg0->x = -0x2C;
    arg0->y = -0x14;
    arg0->frameIndex = 0xD;
    arg0->spriteData = overlayAsset;

    setCallback(&updateStoryMapShopExitOverlay);
}

void updateStoryMapShopExitOverlay(void *arg0) {
    GameState *state = getCurrentAllocation();

    if (state->modeData.unlockScreen.screenPhase == 4) {
        enqueueCallbackBySlotIndex(8, 7, &renderSpriteFrame, arg0);
    }
}

void cleanupStoryMapShopExitOverlay(SpriteRenderArg *arg0) {
    arg0->spriteData = freeNodeMemory(arg0->spriteData);
}

void initStoryMapShopGoldDisplay(StoryMapShopGoldDisplayState *arg0) {
    void *goldIconAsset;
    void *digitSpriteAsset;
    s32 i;
    s32 *new_var;

    digitSpriteAsset = loadCompressedData(&digit_sprite_ROM_START, &COSTUME_SLOT_00_COMPRESSED_DATA_ROM_START, 0x508);
    goldIconAsset = loadCompressedData(&goldIconSprite_ROM_START, &goldIconSprite_ROM_END, 0x388);
    setCleanupCallback(&cleanupStoryMapShopGoldDisplay);
    for (i = 0; i < 7; i++) {
        arg0->digits[i].x = 0x48 + (i * 8);
        arg0->digits[i].y = 0x58;
        arg0->digits[i].spriteData = digitSpriteAsset;
    }

    arg0->goldIcon.x = 0x38;
    arg0->goldIcon.y = 0x58;
    arg0->goldIcon.frameIndex = 0;
    arg0->goldIcon.paletteIndex = 0;
    arg0->goldIcon.spriteData = goldIconAsset;
    setCallback(&updateStoryMapShopGoldDisplay);
}

void updateStoryMapShopGoldDisplay(StoryMapShopGoldDisplayState *arg0) {
    s32 i;
    SpriteRenderArg *digit;
    s8 paletteIndex;
    s32 space;

    if (*gGameSessionContext < 100) {
        paletteIndex = 1;
        i = 6;
        do {
            arg0->digits[i].paletteIndex = paletteIndex;
        } while (--i >= 0);
    } else {
        paletteIndex = 2;
        i = 6;
        do {
            arg0->digits[i].paletteIndex = paletteIndex;
        } while (--i >= 0);
    }

    sprintf(arg0->goldAmountBuffer, "%7d", *gGameSessionContext);

    i = 0;
    space = ' ';
    digit = &arg0->digits[0];
    do {
        char c = arg0->goldAmountBuffer[i];
        if (c != space) {
            digit->frameIndex = c - '0';
            enqueueCallbackBySlotIndex(8, 0, &renderSpriteFrameWithPalette, digit);
        }
        digit++;
    } while (++i < 7);

    enqueueCallbackBySlotIndex(8, 0, &renderSpriteFrameWithPalette, &arg0->goldIcon);
}

void cleanupStoryMapShopGoldDisplay(StoryMapShopGoldDisplayState *arg0) {
    arg0->digits[0].spriteData = freeNodeMemory(arg0->digits[0].spriteData);
    arg0->goldIcon.spriteData = freeNodeMemory(arg0->goldIcon.spriteData);
}

void initStoryMapShopItemPriceDisplay(SpriteRenderArg *arg0) {
    void *digitSpriteAsset;
    s32 i;
    s32 y;
    s16 x;

    digitSpriteAsset = loadCompressedData(&digit_sprite_ROM_START, &COSTUME_SLOT_00_COMPRESSED_DATA_ROM_START, 0x508);
    setCleanupCallback(cleanupStoryMapShopItemPriceDisplay);

    i = 0;
    y = 0x38;
    x = -0x1C;

    do {
        arg0[i].x = x;
        arg0[i].y = y;
        arg0[i].spriteData = digitSpriteAsset;
        i++;
        x += 8;
    } while (i < 6);

    setCallback(updateStoryMapShopItemPriceDisplay);
}

void updateStoryMapShopItemPriceDisplay(StoryMapShopItemPriceDisplayState *arg0) {
    GameState *state = (GameState *)getCurrentAllocation();
    s32 i;
    u8 itemValue;
    s32 price;
    SpriteRenderArg *digit;
    s8 paletteIndex;
    s32 space;

    if (state->modeData.unlockScreen.screenPhase == 0x14) {
        return;
    }

    itemValue = state->modeData.unlockScreen.itemIds[state->modeData.unlockScreen.selectedItemIndex];

    if (itemValue >= 0x80) {
        return;
    }

    price = D_8008F070_8FC70[itemValue & 0x1F];

    paletteIndex = 1;
    if (*gGameSessionContext < price) {
        i = 5;
        do {
            arg0->digits[i].paletteIndex = paletteIndex;
        } while (--i >= 0);
    } else {
        paletteIndex = 2;
        i = 5;
        do {
            arg0->digits[i].paletteIndex = paletteIndex;
        } while (--i >= 0);
    }

    sprintf(arg0->priceBuffer, "%6d", price);

    i = 0;
    space = ' ';
    digit = &arg0->digits[0];
    do {
        char c = arg0->priceBuffer[i];
        if (c != space) {
            digit->frameIndex = c - '0';
            enqueueCallbackBySlotIndex(8, 0, &renderSpriteFrameWithPalette, digit);
        }
        digit++;
    } while (++i < 6);
}

void cleanupStoryMapShopItemPriceDisplay(StoryMapShopItemPriceDisplayState *state) {
    state->digits[0].spriteData = freeNodeMemory(state->digits[0].spriteData);
}

void initStoryMapShopItemStatsDisplay(UnlockScreenItemStatsDisplay *display) {
    void *spriteAsset;
    s32 i;

    getCurrentAllocation();
    display->progressBarAsset = loadAsset_34F7E0();
    spriteAsset = loadCompressedData(&uiCornerSprites_ROM_START, &uiCornerSprites_ROM_END, 0x1548);
    setCleanupCallback(cleanupStoryMapShopItemStatsDisplay);

    display->progressBarX = -0x24;
    display->priceLabelSprite.x = -0x24;
    display->priceLabelSprite.y = 0x1C;
    display->progressBarY = 0;
    display->priceLabelSprite.spriteData = spriteAsset;
    display->priceLabelSprite.frameIndex = 4;

    for (i = 0; i < 3; i++) {
        display->statLabels[i].y = 0x1C + (i * 8);
        display->statLabels[i].x = 0xC;
        display->statLabels[i].string = (u8 *)display->statBuffers[i];
        display->statLabels[i].palette = 0;
    }

    setCallback(updateStoryMapShopItemStatsDisplay);
}

void updateStoryMapShopItemStatsDisplay(UnlockScreenItemStatsDisplay *arg0) {
    GameState *state;
    s16 progressBarY;
    s32 currentItem;
    s32 isValidItem;
    s32 labelIndex;
    char *formatStr;

    state = (GameState *)getCurrentAllocation();
    progressBarY = 0x30 - (state->modeData.unlockScreen.delayCounter * 8);
    arg0->progressBarY = progressBarY;
    renderTiledSprite3x3(
        arg0->progressBarAsset,
        arg0->progressBarX,
        progressBarY,
        4,
        state->modeData.unlockScreen.delayCounter,
        0,
        0x20,
        0xB0,
        8,
        0
    );

    if (state->modeData.unlockScreen.screenPhase != 0x14) {
        currentItem = state->modeData.unlockScreen.itemIds[state->modeData.unlockScreen.selectedItemIndex];
        isValidItem = (u32)currentItem < 0x80U;
        if (isValidItem != 0) {
            enqueueCallbackBySlotIndex(8U, 1U, &renderSpriteFrame, &arg0->priceLabelSprite);
        }
        if (state->modeData.unlockScreen.screenPhase != 2) {
            if (isValidItem != 0) {
                isValidItem = currentItem & 0xFF;
                formatStr = "%2d";
                sprintf(arg0->statBuffers[0], formatStr, getItemStat1(isValidItem) & 0xFF);
                sprintf(arg0->statBuffers[1], formatStr, getItemStat2(isValidItem) & 0xFF);
                sprintf(arg0->statBuffers[2], formatStr, getItemStat3(isValidItem) & 0xFF);

                labelIndex = 0;
                do {
                    enqueueCallbackBySlotIndex(8U, 1U, &renderTextPalette, &arg0->statLabels[labelIndex]);
                    labelIndex += 1;
                } while (labelIndex < 3);
            }
        }
    }
}

void cleanupStoryMapShopItemStatsDisplay(UnlockScreenItemStatsDisplay *arg0) {
    arg0->progressBarAsset = freeNodeMemory(arg0->progressBarAsset);
    arg0->priceLabelSprite.spriteData = freeNodeMemory(arg0->priceLabelSprite.spriteData);
}

void initStoryMapShopSoldOutLabel(SpriteRenderArg *arg0) {
    void *temp_s1 = loadCompressedData(&uiCornerSprites_ROM_START, &uiCornerSprites_ROM_END, 0x1548);

    setCleanupCallback(&cleanupStoryMapShopSoldOutLabel);

    arg0->x = -0x2C;
    arg0->y = -0x18;
    arg0->spriteData = temp_s1;
    arg0->frameIndex = 5;

    setCallback(&updateStoryMapShopSoldOutLabel);
}

void updateStoryMapShopSoldOutLabel(void *arg0) {
    GameState *state = (GameState *)getCurrentAllocation();
    s8 index = state->modeData.unlockScreen.selectedItemIndex;

    if (state->modeData.unlockScreen.itemIds[index] >= 0x80 || state->modeData.unlockScreen.unlockedItemCount == 0) {
        enqueueCallbackBySlotIndex(8, 1, renderSpriteFrame, arg0);
    }
}

void cleanupStoryMapShopSoldOutLabel(SpriteRenderArg *arg0) {
    arg0->spriteData = freeNodeMemory(arg0->spriteData);
}

void initUnlockScreenItemIcons(UnlockScreenItemIconsState *arg0) {
    void *iconAsset;
    void *titleAsset;
    s32 i;

    iconAsset = loadCompressedData(&uiCornerSprites_ROM_START, &uiCornerSprites_ROM_END, 0x1548);
    titleAsset = loadTextRenderAsset(1);
    setCleanupCallback(&cleanupUnlockScreenItemIcons);

    for (i = 0; i < 4; i++) {
        arg0->items[i].x = (i & 1) * 0x80 - 0x80;
        arg0->items[i].y = (i / 2) * 0x10 - 0x66;
        arg0->items[i].spriteData = iconAsset;
        arg0->items[i].frameIndex = i;
    }

    arg0->title.startX = -0x68;
    arg0->title.startY = -0x60;
    arg0->title.textData = (u16 *)boardShopChooseBoardPromptText;
    arg0->title.fontAsset = titleAsset;
    arg0->title.shade.value = 0xFF;
    arg0->title.textAlpha.value = 0xFF;
    arg0->title.paletteIndex = 5;

    setCallback(&drawUnlockScreenItemIcons);
}

void drawUnlockScreenItemIcons(void *untypedArg0) {
    UnlockScreenItemIconsState *arg0 = (UnlockScreenItemIconsState *)untypedArg0;
    GameState *alloc;
    s32 i;

    alloc = (GameState *)getCurrentAllocation();

    if (alloc->modeData.unlockScreen.showItemIcons != 0) {
        for (i = 0; i < 4; i++) {
            enqueueCallbackBySlotIndex(8, 0, &renderSpriteFrame, &arg0->items[i]);
            enqueueCallbackBySlotIndex(8, 1, &renderTextLayout, &arg0->title);
        }
    }
}

void cleanupUnlockScreenItemIcons(UnlockScreenItemIconsState *arg0) {
    arg0->items[0].spriteData = freeNodeMemory(arg0->items[0].spriteData);
    arg0->title.fontAsset = freeNodeMemory(arg0->title.fontAsset);
}

s32 getLockedShopItemIndices(u8 *buffer) {
    s32 count;
    s32 i;
    s32 j;

    count = 0;
    for (i = 11; i >= 0; i--) {
        buffer[i] = 0;
    }

    // Check character_or_settings 3x3 grid for first locked item per row
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (EepromSaveData[i * 3 + (u64)j + 0x30] == 0) {
                buffer[count] = i * 3 + j;
                count++;
                break;
            }
        }
    }

    // Check setting_42 referenced items for any that are still locked
    for (i = 0; i < 9; i++) {
        u8 itemIndex = EepromSaveData[(u64)i + 0x42];
        if (itemIndex != 0) {
            if (EepromSaveData[(u64)itemIndex + 0x30] == 0) {
                buffer[count] = itemIndex;
                count++;
            }
        }
    }

    return count & 0xFF;
}

const char D_8009E47C_9F07C[] = "%7d";

const char D_8009E480_9F080[] = "%5dG";

const char gIntegerFormatString[] = "%d";

const char gGoldFormatString7d[] = "%7d";
