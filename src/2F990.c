#include "2F990.h"
#include "10AD0.h"
#include "20F0.h"
#include "33FE0.h"
#include "38C90.h"
#include "3E160.h"
#include "56910.h"
#include "5E590.h"
#include "68CF0.h"
#include "6E840.h"
#include "common.h"
#include "displaylist.h"
#include "gamestate.h"
#include "geometry.h"
#include "overlay.h"
#include "task_scheduler.h"

USE_ASSET(_419C60);
USE_ASSET(_42F1D0);
USE_ASSET(_41A1D0);
USE_ASSET(_4237C0);
USE_ASSET(_3F6670);
USE_ASSET(_3F6950);
USE_ASSET(_3F6BB0);

typedef struct {
    u8 padding[0x2C];
    void *backgroundAsset;
} StoryMapShopBackgroundState;

typedef struct {
    DisplayListObject displayList;
    Transform3D transform;
    s32 translationStep;
    u8 updateCounter;
    s8 itemIndex;
    s8 slotPosition;
} StoryMapShopItemCardState;

typedef struct {
    s16 x;
    s16 y;
    void *asset;
    s16 spriteIndex;
    s16 alpha;
    s8 unkC;
    u8 unkD;
} StoryMapShopItemIconState;

typedef struct {
    u8 padding[0x5C0];
    u16 unk5C0;
    u8 padding2[0x3];
    u8 unk5C5;
    u8 unk5C6;
    u8 unk5C7;
    s8 unk5C8;
    u8 unk5C9;
    u8 unk5CA[0];
} GameStateSub;

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
    SpriteDisplayState digits[6];
    u8 unk48[0x18];
    char priceBuffer[8];
} StoryMapShopItemPriceDisplayState;

typedef struct {
    SceneModel *model;
    Transform3D transform;
    s16 animationFrame;
    s8 animationType;
} StoryMapShopFairyState;

typedef struct {
    DisplayListObject displayList;
    void *baseTransform;
    u8 padding[0x10];
    s32 translationX;
    u8 padding3[0xC];
    u8 slideFrameCounter;
    s8 nextItemIndex;
} SlidingItemCardState;

typedef struct {
    SpriteDisplayState digits[7];
    s16 goldIconX;
    s16 goldIconY;
    void *goldIconAsset;
    s16 unk5C;
    s8 unk5E;
    char pad5F;
    char goldAmountBuffer[8];
} StoryMapShopGoldDisplayState;

typedef struct {
    u8 _pad[0x4];
    void *unk4;
} func_8002FF28_30B28_arg;

typedef struct {
    u8 _pad[0x4];
    void *unk4;
    u8 _pad2[0x30];
    void *unk38;
} UnlockScreenItemIconsCleanupArg;

typedef struct {
    u8 _pad[0x4];
    void *unk4;
} func_80030668_31268_arg;

extern s32 gButtonsPressed[];
extern s32 gControllerInputs[4];
extern u8 identityMatrix[];
extern s32 D_8008F110_8FD10;
extern s16 D_8008F0B2_8FCB2[];
extern u16 D_8008F0B4_8FCB4[];
extern u16 D_8008F0B6_8FCB6[];
extern s16 D_8008F0C6_8FCC6[];
extern s32 *D_800AFE8C_A71FC;

void updateStoryMapShopGoldDisplay(StoryMapShopGoldDisplayState *);
void cleanupStoryMapShopGoldDisplay(StoryMapShopGoldDisplayState *arg0);
void updateStoryMapShopItemPriceDisplay(StoryMapShopItemPriceDisplayState *arg0);
void updateStoryMapShopFairyInitial(StoryMapShopFairyState *);
void updateStoryMapShopFairy(StoryMapShopFairyState *);
void destroyStoryMapShopFairy(StoryMapShopFairyState *);
void updateStoryMapShopItemCard(StoryMapShopItemCardState *);
void transitionStoryMapShopItemCard(StoryMapShopItemCardState *);
void reloadStoryMapShopItemCard(SlidingItemCardState *);
void slideStoryMapShopItemCard(SlidingItemCardState *);
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
void cleanupUnlockScreenScrollArrows(func_8002FF28_30B28_arg *arg0);
void updateStoryMapShopItemIcon(StoryMapShopItemIconState *);
void cleanupStoryMapShopItemIcon(StoryMapShopItemIconState *);
void updateStoryMapShopItemStatLabel(ScrollArrowSprite *);
void cleanupStoryMapShopItemStatLabel(func_8002FF28_30B28_arg *);
void updateStoryMapShopExitOverlay(void *arg0);
void cleanupStoryMapShopExitOverlay(func_8002FF28_30B28_arg *arg0);
void updateStoryMapShopItemStatsDisplay(ItemStatsDisplay *arg0);

void updateStoryMapShopSoldOutLabel(void *);
void cleanupStoryMapShopSoldOutLabel(SpriteDisplayState *);
void drawUnlockScreenItemIcons(void *);
void cleanupUnlockScreenItemIcons(UnlockScreenItemIconsCleanupArg *);
void *func_80035F80_36B80(s32);
void func_8006FF90_70B90(s32, s32, void *, void *);
void updateDebugCameraYState(cameraState *arg0);

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
    func_8006BEDC_6CADC(&sp20, 0, arg0->cameraY << 0x10, 0x600000, 0, (s32)(arg0->cameraRotation), 0);
    func_8006FD3C_7093C(temp_s0->unkDA, &sp20);

    cameraYString = &arg0->unk3E;
    sprintf(cameraYString, "CAMERA Y = %d", arg0->cameraY);
    cameraYRotation = &arg0->unk1E;
    sprintf(cameraYRotation, "CAMERA RX = %X", arg0->cameraRotation);
    arg0->cameraYString = cameraYString;
    arg0->cameraYRotationString = cameraYRotation;

    debugEnqueueCallback(8, 7, &renderTextPalette, arg0);
    debugEnqueueCallback(8, 7, &renderTextPalette, ((void *)((s32)arg0)) + 0xC);
}

void initStoryMapShopFairyModel(StoryMapShopFairyState *arg0) {
    GameState *state = (GameState *)getCurrentAllocation();

    arg0->model = createSceneModel(0x3A, state);
    memcpy(&arg0->transform, identityMatrix, 0x20);
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

typedef struct {
    u8 padding[0x5D6];
    u8 pendingFairyAnimation;
} ShopFairyGameState;

void updateStoryMapShopFairy(StoryMapShopFairyState *fairy) {
    ShopFairyGameState *state;
    u8 animIndex;
    u16 frameCounter;
    s32 idx;
    volatile u8 pad[8];

    state = (ShopFairyGameState *)getCurrentAllocation();
    applyTransformToModel(fairy->model, &fairy->transform);
    do {
        if (clearModelRotation(fairy->model) != 0) {
            animIndex = fairy->animationType;
            idx = animIndex * 2;
            if (animIndex != 0) {
                frameCounter = fairy->animationFrame + 1;
                fairy->animationFrame = frameCounter;
                if (frameCounter == (u16)(D_8008F0B4_8FCB4[idx] + D_8008F0B6_8FCB6[idx])) {
                    fairy->animationType = 0;
                    fairy->animationFrame = 4;
                }
                setModelAnimation(fairy->model, fairy->animationFrame);
            }
        }
    } while (0);
    updateModelGeometry(fairy->model);
    animIndex = state->pendingFairyAnimation;
    if (animIndex != 0) {
        u16 start;
        fairy->animationType = animIndex;
        start = D_8008F0B4_8FCB4[state->pendingFairyAnimation * 2];
        fairy->animationFrame = start;
        setModelAnimation(fairy->model, (s16)start);
        state->pendingFairyAnimation = 0;
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
    memcpy(cardTransform, identityMatrix, 0x20);
    rotationZPtr = &rotationZ;
    memcpy(rotationZPtr, cardTransform, 0x20);
    rotationYXPtr = &rotationYX;
    memcpy(rotationYXPtr, rotationZPtr, 0x20);
    createRotationMatrixYX(rotationYXPtr, 0x1000, 0x800);
    createZRotationMatrix(rotationZPtr, 0x1F00);
    func_8006B084_6BC84(rotationYXPtr, rotationZPtr, cardTransform);

    if (state->unk5C9 != 1) {
        card->transform.translation.x = 0x200000 - ((2 - card->slotPosition) << 21);
    }

    itemData = state->unk5CA[card->itemIndex];
    memcpy(card, cardTransform, 0x20);
    do {
        itemId = itemData & 0x1F;
        itemData = itemId;
    } while (0);

    card->displayList.unk20 = loadAssetByIndex_95728(itemId);
    card->displayList.unk24 = loadAssetByIndex_95500(itemId);
    card->displayList.unk28 = loadAssetByIndex_95590(itemId);
    card->displayList.unk2C = loadAssetByIndex_95668(itemData / 3);
    card->updateCounter = 0;

    setCleanupCallback(destroyStoryMapShopItemCard);
    setCallback(updateStoryMapShopItemCard);
}

void updateStoryMapShopItemCard(StoryMapShopItemCardState *card) {
    volatile u8 padding[0x20];
    GameState *state;

    state = (GameState *)getCurrentAllocation();

    if (state->unk5C5 == 2) {
        if (state->unk5C6 == 2) {
            if (card->slotPosition == 0) {
                card->slotPosition = 2;
                card->transform.translation.x = 0x400000;
                card->updateCounter = 1;
                goto end;
            }
        }
        if (state->unk5C6 == 1) {
            if (card->slotPosition == 2) {
                card->transform.translation.x = 0xFFC00000;
                card->slotPosition = 0;
                card->updateCounter = 1;
                goto end;
            }
        }
        if (state->unk5C6 == 2) {
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
        card->displayList.unk24 = freeNodeMemory(card->displayList.unk24);
        card->displayList.unk28 = freeNodeMemory(card->displayList.unk28);
        card->displayList.unk2C = freeNodeMemory(card->displayList.unk2C);
        if (card->updateCounter != 0) {
            goto end;
        }
    }
    enqueueDisplayListObject(0, &card->displayList);
end:
    setCallback(&reloadStoryMapShopItemCard);
}

void reloadStoryMapShopItemCard(SlidingItemCardState *card) {
    volatile u8 padding[0x20];
    GameState *state = (GameState *)getCurrentAllocation();
    s8 itemId;
    s8 itemIndex;

    if (card->slideFrameCounter == 1) {
        if (state->unk5C6 == 2) {
            card->nextItemIndex = state->unk5C8 + 1;
            if (card->nextItemIndex == state->unk5C9) {
                card->nextItemIndex = 0;
            }
        } else {
            card->nextItemIndex = state->unk5C8 - 1;
            if (card->nextItemIndex < 0) {
                card->nextItemIndex = state->unk5C9 - 1;
            }
        }

        itemId = state->unk5CA[card->nextItemIndex] & 0x1F;
        itemIndex = itemId;

        memcpy(card, &card->baseTransform, 0x20);

        card->displayList.unk20 = loadAssetByIndex_95728(itemIndex);
        card->displayList.unk24 = loadAssetByIndex_95500(itemIndex);
        card->displayList.unk28 = loadAssetByIndex_95590(itemIndex);
        card->displayList.unk2C = loadAssetByIndex_95668(itemId / 3);
        card->slideFrameCounter = 0;
    } else {
        enqueueDisplayListObject(0, (DisplayListObject *)card);
    }

    setCallback(&slideStoryMapShopItemCard);
}

void slideStoryMapShopItemCard(SlidingItemCardState *card) {
    volatile u8 padding[0x20];
    u32 scrollDirection;
    GameState *state = (GameState *)getCurrentAllocation();
    u32 translationStep;
    scrollDirection = state->unk5C6;

    if (scrollDirection == 2) {
        translationStep = 0xFFF80000;
    } else {
        translationStep = 0x00080000;
    }

    card->translationX += translationStep;

    memcpy(card, (void *)((s32)card + 0x3C), 0x20);

    card->slideFrameCounter++;
    if (card->slideFrameCounter == 4) {
        card->slideFrameCounter = 0;
        state->unk5C7++;
        setCallback(awaitStoryMapShopItemCardIdle);
    }

    enqueueDisplayListObject(0, &card->displayList);
}

void awaitStoryMapShopItemCardIdle(DisplayListObject *displayList) {
    volatile u8 pad[0x20];
    if (((GameState *)getCurrentAllocation())->unk5C5 == 1) {
        setCallback(&updateStoryMapShopItemCard);
    }
    enqueueDisplayListObject(0, displayList);
}

void destroyStoryMapShopItemCard(StoryMapShopItemCardState *card) {
    card->displayList.unk24 = freeNodeMemory(card->displayList.unk24);
    card->displayList.unk28 = freeNodeMemory(card->displayList.unk28);
    card->displayList.unk2C = freeNodeMemory(card->displayList.unk2C);
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
    func_8006B084_6BC84(&rotationYX, &rotationZ, cardTransform);
    card->updateCounter = 0;
    card->displayList.unk20 = NULL;
    card->displayList.unk24 = NULL;
    card->displayList.unk28 = NULL;
    card->displayList.unk2C = 0;
    setCleanupCallback(&destroySlideInStoryMapShopItemCard);
    setCallback(&prepareSlideInStoryMapShopItemCard);
}

void prepareSlideInStoryMapShopItemCard(StoryMapShopItemCardState *card) {
    GameState *state;
    s8 wrappedItemIndex;
    u8 itemData;
    s16 itemId;

    state = (GameState *)getCurrentAllocation();
    if (state->unk5C5 == 2) {
        if (state->unk5C6 == 1) {
            card->transform.translation.x = 0x200000;
            card->translationStep = 0x80000;
            wrappedItemIndex = state->unk5C8 + 2;
            card->itemIndex = wrappedItemIndex;
            if (wrappedItemIndex >= (s32)state->unk5C9) {
                card->itemIndex = wrappedItemIndex - state->unk5C9;
            }
        } else {
            card->transform.translation.x = 0xFFE00000;
            card->translationStep = 0xFFF80000;
            wrappedItemIndex = state->unk5C8 - 2;
            card->itemIndex = wrappedItemIndex;
            if (wrappedItemIndex < 0) {
                card->itemIndex = state->unk5C9 + wrappedItemIndex;
            }
        }
        itemData = state->unk5CA[card->itemIndex];
        memcpy(card, &card->transform, 0x20);
        itemData &= 0x1F;
        itemId = itemData;
        card->displayList.unk20 = loadAssetByIndex_95728(itemId);
        card->displayList.unk24 = loadAssetByIndex_95500(itemId);
        card->displayList.unk28 = loadAssetByIndex_95590(itemId);
        card->displayList.unk2C = loadAssetByIndex_95668(itemData / 3);
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
    memcpy(card, &card->transform, sizeof(Transform3D));

    card->updateCounter++;
    if (card->updateCounter == 4) {
        state->unk5C7++;
        card->updateCounter = 0;
        card->displayList.unk24 = freeNodeMemory(card->displayList.unk24);
        card->displayList.unk28 = freeNodeMemory(card->displayList.unk28);
        card->displayList.unk2C = freeNodeMemory(card->displayList.unk2C);
        setCallbackWithContinue(awaitSlideInStoryMapShopItemCardIdle);
    } else {
        enqueueDisplayListObject(0, &card->displayList);
    }
}

void awaitSlideInStoryMapShopItemCardIdle(void) {
    GameState *state = (GameState *)getCurrentAllocation();
    if (state->unk5C5 != 2) {
        setCallbackWithContinue(prepareSlideInStoryMapShopItemCard);
    }
}

void destroySlideInStoryMapShopItemCard(StoryMapShopItemCardState *card) {
    card->displayList.unk24 = freeNodeMemory(card->displayList.unk24);
    card->displayList.unk28 = freeNodeMemory(card->displayList.unk28);
    card->displayList.unk2C = freeNodeMemory(card->displayList.unk2C);
}

void loadStoryMapShopBackground(StoryMapShopBackgroundState *state) {
    state->backgroundAsset = loadCompressedData(&_42F1D0_ROM_START, &_42F1D0_ROM_END, 0x14410);
    setCleanupCallback(&cleanupStoryMapShopBackground);
    setCallback(&initStoryMapShopBackgroundRenderState);
}

void initStoryMapShopBackgroundRenderState(StoryMapShopBackgroundState *state) {
    func_800394BC_3A0BC(state, (s32)state->backgroundAsset);
    setCallback(&enqueueStoryMapShopBackgroundRender);
}

void enqueueStoryMapShopBackgroundRender(void *state) {
    debugEnqueueCallback(1, 0, func_80038420_39020, state);
}

void cleanupStoryMapShopBackground(StoryMapShopBackgroundState *state) {
    state->backgroundAsset = freeNodeMemory(state->backgroundAsset);
}

void initUnlockScreenScrollArrows(UnlockScreenScrollArrowsState *state) {
    s32 i;
    void *asset = loadCompressedData(&_4237C0_ROM_START, &_4237C0_ROM_END, 0x8A08);

    setCleanupCallback(&cleanupUnlockScreenScrollArrows);

    for (i = 0; i < 2; i++) {
        state->arrows[i].x = -0x40 + i * 0x60;
        state->arrows[i].y = -0x18;
        state->arrows[i].spriteIndex = i;
        state->arrows[i].asset = asset;
        state->arrows[i].alpha = 0xFF;
        state->arrows[i].unkD = 0;
        state->arrows[i].unkC = 0;
    }

    state->animationCounter = 0;

    setCallback(&updateUnlockScreenScrollArrows);
}

void updateUnlockScreenScrollArrows(UnlockScreenScrollArrowsState *arrowState) {
    GameState *state = getCurrentAllocation();
    s32 i;

    if (state->unk5C5 > 0 && state->unk5C5 < 4) {
        if (state->unk5C5 == 1) {
            arrowState->animationCounter++;
            if (state->unk5C9 >= 3) {
                if ((u8)(arrowState->animationCounter) < 0x11) {
                    arrowState->arrows[0].alpha -= 8;
                    arrowState->arrows[1].alpha -= 8;
                } else {
                    arrowState->arrows[0].alpha += 8;
                    arrowState->arrows[1].alpha += 8;
                }
            } else if (state->unk5C9 == 2) {
                if ((u8)(arrowState->animationCounter) < 0x11) {
                    if (state->unk5C8 == 1) {
                        arrowState->arrows[1].alpha = 0xFF;
                        arrowState->arrows[0].alpha -= 8;
                    } else {
                        arrowState->arrows[0].alpha = 0xFF;
                        arrowState->arrows[1].alpha -= 8;
                    }
                } else {
                    if (state->unk5C8 == 1) {
                        arrowState->arrows[1].alpha = 0xFF;
                        arrowState->arrows[0].alpha += 8;
                    } else {
                        arrowState->arrows[0].alpha = 0xFF;
                        arrowState->arrows[1].alpha += 8;
                    }
                }
            } else {
                arrowState->animationCounter = 0;
                arrowState->arrows[0].alpha = 0xFF;
                arrowState->arrows[1].alpha = 0xFF;
            }
        } else {
            arrowState->animationCounter = 0;
            arrowState->arrows[0].alpha = 0xFF;
            arrowState->arrows[1].alpha = 0xFF;
        }

        arrowState->animationCounter &= 0x1F;

        for (i = 0; i < 2; i++) {
            debugEnqueueCallback(8, 0, &func_80012004_12C04, &arrowState->arrows[i]);
        }
    }
}

void cleanupUnlockScreenScrollArrows(func_8002FF28_30B28_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void initStoryMapShopItemIcon(StoryMapShopItemIconState *iconState) {
    void *dmaResult;
    GameStateSub *state;
    u8 itemValue;

    state = getCurrentAllocation();
    dmaResult = loadCompressedData(&_4237C0_ROM_START, &_4237C0_ROM_END, 0x8A08);
    setCleanupCallback(cleanupStoryMapShopItemIcon);

    iconState->y = -0x18;

    itemValue = state->unk5CA[state->unk5C8];

    if (itemValue < 9) {
        u8 tempValue;
        iconState->x = -0x30;
        tempValue = state->unk5CA[state->unk5C8];
        iconState->spriteIndex = (tempValue / 3) + 0x1D;
    } else {
        s16 tableVal = D_8008F0B2_8FCB2[itemValue] + 0x18;
        s16 tableVal2 = D_8008F0C6_8FCC6[itemValue];

        iconState->spriteIndex = itemValue + 0x23;
        iconState->x = ((0x120 - tableVal) / 2) - tableVal2 - 0x96;
    }

    iconState->alpha = 0xFF;
    iconState->unkC = 0;
    iconState->unkD = 0;
    iconState->asset = dmaResult;

    setCallback(updateStoryMapShopItemIcon);
}

void updateStoryMapShopItemIcon(StoryMapShopItemIconState *iconState) {
    GameStateSub *state;
    s32 pad;
    u8 itemValue;
    state = (GameStateSub *)getCurrentAllocation();
    itemValue = state->unk5CA[state->unk5C8];
    if (itemValue < 0x80) {
        if ((state->unk5C5 != 0) && (state->unk5C5 != 2)) {
            u8 masked = itemValue & 0x1F;
            itemValue = masked;
            if (itemValue < 9) {
                iconState->x = -0x30;
                iconState->spriteIndex = (masked / 3) + 0x1D;
            } else {
                s16 tableVal = D_8008F0B2_8FCB2[masked];
                s16 tableVal2 = D_8008F0C6_8FCC6[masked];
                iconState->spriteIndex = masked + 0x23;
                iconState->x = (((0x120 - ((s16)(tableVal + 0x18))) / 2) - tableVal2) - 0x96;
            }
            if (state->unk5C5 == 3) {
                if (state->unk5C0 & 1) {
                    iconState->unkD = 0xFF;
                } else {
                    iconState->unkD = 0;
                }
            }
            itemValue = 0;
            debugEnqueueCallback(8, itemValue, &func_80012004_12C04, iconState);
        }
    }
}

void cleanupStoryMapShopItemIcon(StoryMapShopItemIconState *iconState) {
    iconState->asset = freeNodeMemory(iconState->asset);
}

void initStoryMapShopItemStatLabel(ScrollArrowSprite *arg0) {
    GameState *state;
    GameState *temp_s0;
    void *dmaResult;
    u8 itemValue;

    state = (GameState *)getCurrentAllocation();
    dmaResult = loadCompressedData(&_4237C0_ROM_START, &_4237C0_ROM_END, 0x8A08);
    setCleanupCallback(cleanupStoryMapShopItemStatLabel);

    arg0->y = -0x18;

    temp_s0 = (GameState *)((u8 *)state + state->unk5C8);
    itemValue = temp_s0->unk5CA[0];

    if (itemValue < 9) {
        arg0->x = 0x12;
        arg0->spriteIndex = ((itemValue % 3) & 0xFF) + 0x24;
    } else {
        s16 tableVal = D_8008F0B2_8FCB2[itemValue];
        arg0->spriteIndex = 0x35;
        arg0->x = tableVal + ((0x120 - (s16)(tableVal + 0x18)) / 2) - 0x96;
    }

    arg0->alpha = 0xFF;
    arg0->unkC = 0;
    arg0->unkD = 0;
    arg0->asset = dmaResult;

    setCallback(updateStoryMapShopItemStatLabel);
}

void updateStoryMapShopItemStatLabel(ScrollArrowSprite *arg0) {
    GameState *state = (GameState *)getCurrentAllocation();
    u8 itemValue;

    itemValue = state->unk5CA[state->unk5C8];

    if (itemValue >= 0x80) {
        return;
    }

    if (state->unk5C5 != 0 && state->unk5C5 != 2) {
        if (itemValue < 9) {
            arg0->x = 0x12;
            arg0->spriteIndex = ((itemValue & 0x1F) % 3) + 0x24;
        } else {
            s16 tableVal = D_8008F0B2_8FCB2[itemValue];
            arg0->spriteIndex = 0x35;
            arg0->x = tableVal + ((0x120 - (s16)(tableVal + 0x18)) / 2) - 0x96;
        }

        if (state->unk5C5 == 3) {
            if (state->unk5C0 & 1) {
                arg0->unkD = 0xFF;
            } else {
                arg0->unkD = 0;
            }
        }

        debugEnqueueCallback(8, 0, &func_80012004_12C04, arg0);
    }
}

void cleanupStoryMapShopItemStatLabel(func_8002FF28_30B28_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void initStoryMapShopExitOverlay(SpriteDisplayState *arg0) {
    void *overlayAsset = loadCompressedData(&_41A1D0_ROM_START, &_41A1D0_ROM_END, 0x1B48);

    setCleanupCallback(&cleanupStoryMapShopExitOverlay);

    arg0->x = -0x2C;
    arg0->y = -0x14;
    arg0->spriteIndex = 0xD;
    arg0->asset = overlayAsset;

    setCallback(&updateStoryMapShopExitOverlay);
}

void updateStoryMapShopExitOverlay(void *arg0) {
    GameState *state = getCurrentAllocation();

    if (state->unk5C5 == 4) {
        debugEnqueueCallback(8, 7, &func_8000FED0_10AD0, arg0);
    }
}

void cleanupStoryMapShopExitOverlay(func_8002FF28_30B28_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void initStoryMapShopGoldDisplay(StoryMapShopGoldDisplayState *arg0) {
    void *goldIconAsset;
    void *digitSpriteAsset;
    s32 i;
    s32 *new_var;

    digitSpriteAsset = loadCompressedData(&_3F6950_ROM_START, &_3F6950_ROM_END, 0x508);
    goldIconAsset = loadCompressedData(&_3F6670_ROM_START, &_3F6670_ROM_END, 0x388);
    setCleanupCallback(&cleanupStoryMapShopGoldDisplay);
    for (i = 0; i < 7; i++) {
        arg0->digits[i].x = 0x48 + (i * 8);
        arg0->digits[i].y = 0x58;
        arg0->digits[i].asset = digitSpriteAsset;
    }

    arg0->goldIconX = 0x38;
    arg0->goldIconY = 0x58;
    arg0->unk5C = 0;
    arg0->unk5E = 0;
    arg0->goldIconAsset = goldIconAsset;
    setCallback(&updateStoryMapShopGoldDisplay);
}

void updateStoryMapShopGoldDisplay(StoryMapShopGoldDisplayState *arg0) {
    s32 i;
    SpriteDisplayState *digit;
    s8 paletteIndex;
    s32 space;

    if (*D_800AFE8C_A71FC < 100) {
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

    sprintf(arg0->goldAmountBuffer, "%7d", *D_800AFE8C_A71FC);

    i = 0;
    space = ' ';
    digit = &arg0->digits[0];
    do {
        char c = arg0->goldAmountBuffer[i];
        if (c != space) {
            digit->spriteIndex = c - '0';
            debugEnqueueCallback(8, 0, &func_80010240_10E40, digit);
        }
        digit++;
    } while (++i < 7);

    debugEnqueueCallback(8, 0, &func_80010240_10E40, &arg0->goldIconX);
}

void cleanupStoryMapShopGoldDisplay(StoryMapShopGoldDisplayState *arg0) {
    arg0->digits[0].asset = freeNodeMemory(arg0->digits[0].asset);
    arg0->goldIconAsset = freeNodeMemory(arg0->goldIconAsset);
}

void cleanupStoryMapShopItemPriceDisplay(func_80030668_31268_arg *);

void initStoryMapShopItemPriceDisplay(SpriteDisplayState *arg0) {
    void *digitSpriteAsset;
    s32 i;
    s32 y;
    s16 x;

    digitSpriteAsset = loadCompressedData(&_3F6950_ROM_START, &_3F6BB0_ROM_START, 0x508);
    setCleanupCallback(cleanupStoryMapShopItemPriceDisplay);

    i = 0;
    y = 0x38;
    x = -0x1C;

    do {
        arg0[i].x = x;
        arg0[i].y = y;
        arg0[i].asset = digitSpriteAsset;
        i++;
        x += 8;
    } while (i < 6);

    setCallback(updateStoryMapShopItemPriceDisplay);
}

extern s32 D_8008F070_8FC70[];

void updateStoryMapShopItemPriceDisplay(StoryMapShopItemPriceDisplayState *arg0) {
    GameState *state = (GameState *)getCurrentAllocation();
    s32 i;
    u8 itemValue;
    s32 price;
    SpriteDisplayState *digit;
    s8 paletteIndex;
    s32 space;

    if (state->unk5C5 == 0x14) {
        return;
    }

    itemValue = state->unk5CA[state->unk5C8];

    if (itemValue >= 0x80) {
        return;
    }

    price = D_8008F070_8FC70[itemValue & 0x1F];

    paletteIndex = 1;
    if (*D_800AFE8C_A71FC < price) {
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
            digit->spriteIndex = c - '0';
            debugEnqueueCallback(8, 0, &func_80010240_10E40, digit);
        }
        digit++;
    } while (++i < 6);
}

void cleanupStoryMapShopItemPriceDisplay(func_80030668_31268_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void initStoryMapShopItemStatsDisplay(ItemStatsDisplay *display) {
    void *spriteAsset;
    s32 i;

    getCurrentAllocation();
    display->progressBarAsset = loadAsset_34F7E0();
    spriteAsset = loadCompressedData(&_419C60_ROM_START, &_419C60_ROM_END, 0x1548);
    setCleanupCallback(cleanupStoryMapShopItemStatsDisplay);

    display->progressBarX = -0x24;
    display->priceLabelX = -0x24;
    display->priceLabelY = 0x1C;
    display->progressBarY = 0;
    display->spriteAsset = spriteAsset;
    display->priceSpriteIndex = 4;

    for (i = 0; i < 3; i++) {
        display->statLabels[i].y = 0x1C + (i * 8);
        display->statLabels[i].x = 0xC;
        display->statLabels[i].text = display->statBuffers[i];
        display->statLabels[i].palette = 0;
    }

    setCallback(updateStoryMapShopItemStatsDisplay);
}

extern void func_8006D7B0_6E3B0(void *, s16, s16, s32, s32, s32, s32, s32, s32, s32);

void updateStoryMapShopItemStatsDisplay(ItemStatsDisplay *arg0) {
    GameState *state;
    s16 progressBarY;
    s32 currentItem;
    s32 isValidItem;
    s32 labelIndex;
    s32 labelOffset;
    char *formatStr;

    state = (GameState *)getCurrentAllocation();
    progressBarY = 0x30 - (state->unk5D7 * 8);
    arg0->progressBarY = progressBarY;
    func_8006D7B0_6E3B0(
        arg0->progressBarAsset,
        arg0->progressBarX,
        progressBarY,
        4,
        state->unk5D7,
        0,
        0x20,
        0xB0,
        8,
        0
    );

    if (state->unk5C5 != 0x14) {
        currentItem = state->unk5CA[state->unk5C8];
        isValidItem = (u32)currentItem < 0x80U;
        if (isValidItem != 0) {
            debugEnqueueCallback(8U, 1U, &func_8000FED0_10AD0, &arg0->priceLabelX);
        }
        if (state->unk5C5 != 2) {
            if (isValidItem != 0) {
                isValidItem = currentItem & 0xFF;
                formatStr = "%2d";
                sprintf(arg0->statBuffers[0], formatStr, getItemStat1(isValidItem) & 0xFF);
                sprintf(arg0->statBuffers[1], formatStr, getItemStat2(isValidItem) & 0xFF);
                sprintf(arg0->statBuffers[2], formatStr, getItemStat3(isValidItem) & 0xFF);

                labelIndex = 0;
                labelOffset = 0x10;
                do {
                    debugEnqueueCallback(8U, 1U, &renderTextPalette, (char *)arg0 + labelOffset);
                    labelIndex += 1;
                    labelOffset += 0xC;
                } while (labelIndex < 3);
            }
        }
    }
}

void cleanupStoryMapShopItemStatsDisplay(ItemStatsDisplay *arg0) {
    arg0->progressBarAsset = freeNodeMemory(arg0->progressBarAsset);
    arg0->spriteAsset = freeNodeMemory(arg0->spriteAsset);
}

void initStoryMapShopSoldOutLabel(SpriteDisplayState *arg0) {
    void *temp_s1 = loadCompressedData(&_419C60_ROM_START, &_419C60_ROM_END, 0x1548);

    setCleanupCallback(&cleanupStoryMapShopSoldOutLabel);

    arg0->x = -0x2C;
    arg0->y = -0x18;
    arg0->asset = temp_s1;
    arg0->spriteIndex = 5;

    setCallback(&updateStoryMapShopSoldOutLabel);
}

void updateStoryMapShopSoldOutLabel(void *arg0) {
    GameState *state = (GameState *)getCurrentAllocation();
    s8 index = state->unk5C8;

    if (state->unk5CA[index] >= 0x80 || state->unk5C9 == 0) {
        debugEnqueueCallback(8, 1, func_8000FED0_10AD0, arg0);
    }
}

void cleanupStoryMapShopSoldOutLabel(SpriteDisplayState *arg0) {
    arg0->asset = freeNodeMemory(arg0->asset);
}

void initUnlockScreenItemIcons(UnlockScreenItemIconsState *arg0) {
    void *iconAsset;
    void *titleAsset;
    s32 i;

    iconAsset = loadCompressedData(&_419C60_ROM_START, &_419C60_ROM_END, 0x1548);
    titleAsset = func_80035F80_36B80(1);
    setCleanupCallback(&cleanupUnlockScreenItemIcons);

    for (i = 0; i < 4; i++) {
        arg0->items[i].x = (i & 1) * 0x80 - 0x80;
        arg0->items[i].y = (i / 2) * 0x10 - 0x66;
        arg0->items[i].asset = iconAsset;
        arg0->items[i].spriteIndex = i;
    }

    arg0->titleX = -0x68;
    arg0->titleY = -0x60;
    arg0->titleData = &D_8008F110_8FD10;
    arg0->titleAsset = titleAsset;
    arg0->titleAlpha1 = 0xFF;
    arg0->titleAlpha2 = 0xFF;
    arg0->titlePalette = 5;

    setCallback(&drawUnlockScreenItemIcons);
}

void drawUnlockScreenItemIcons(void *untypedArg0) {
    UnlockScreenItemIconsState *arg0 = (UnlockScreenItemIconsState *)untypedArg0;
    GameState *alloc;
    s32 i;

    alloc = (GameState *)getCurrentAllocation();

    if (alloc->unk5D8 != 0) {
        for (i = 0; i < 4; i++) {
            debugEnqueueCallback(8, 0, &func_8000FED0_10AD0, &arg0->items[i]);
            debugEnqueueCallback(8, 1, &func_80035408_36008, &arg0->titleX);
        }
    }
}

void cleanupUnlockScreenItemIcons(UnlockScreenItemIconsCleanupArg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
    arg0->unk38 = freeNodeMemory(arg0->unk38);
}

extern u8 *EepromSaveData;

s32 func_80030BA8_317A8(u8 *buffer) {
    s32 temp_t0;
    s32 i;
    s32 j;

    // Clear 12 bytes
    temp_t0 = 0;
    for (i = 11; i >= 0; i--) {
        buffer[i] = 0;
    }

    // Check 3x3 grid
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (EepromSaveData[i * 3 + (u64)j + 0x30] == 0) {
                buffer[temp_t0] = i * 3 + j;
                temp_t0++;
                break;
            }
        }
    }

    // Check setting_42 array entries
    for (i = 0; i < 9; i++) {
        u8 value = EepromSaveData[(u64)i + 0x42];
        if (value != 0) {
            if (EepromSaveData[(u64)value + 0x30] == 0) {
                buffer[temp_t0] = value;
                temp_t0++;
            }
        }
    }

    return temp_t0 & 0xFF;
}

INCLUDE_RODATA("asm/nonmatchings/2F990", D_8009E47C_9F07C);

INCLUDE_RODATA("asm/nonmatchings/2F990", D_8009E480_9F080);

INCLUDE_RODATA("asm/nonmatchings/2F990", D_8009E488_9F088);

INCLUDE_RODATA("asm/nonmatchings/2F990", D_8009E48C_9F08C);
