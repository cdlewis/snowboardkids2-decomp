#include "10AD0.h"
#include "19E80.h"
#include "20F0.h"
#include "232F0.h"
#include "38C90.h"
#include "5E590.h"
#include "68CF0.h"
#include "D_800AFE8C_A71FC_type.h"
#include "EepromSaveData_type.h"
#include "assets.h"
#include "common.h"
#include "displaylist.h"
#include "gamestate.h"
#include "geometry.h"
#include "graphics.h"
#include "rom_loader.h"
#include "task_scheduler.h"

typedef struct {
    u8 padding[0x24];
    void *unk24;
    void *unk28;
    void *unk2C;
} func_8002494C_arg;

typedef struct {
    u16 rotationX;
    u16 rotationY;
    u8 padding[0x28];
    void *effectAsset;
} CharSelectBackgroundEffectState;

typedef struct {
    SceneModel *model;
} SceneModelHolder;

typedef struct {
    SceneModel *model;
    Transform3D transform;
    union {
        SceneModel *unk20;
        s32 targetX;
        s16 unk20_s16;
    } unk20_u;
    u8 playerIndex;
} CharSelectBoardPreview;

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
    s16 baseY;
    s16 x;
    void *spriteAsset;
    s16 spriteIndex;
    s16 scaleX;
    s16 scaleY;
    s16 currentY;
    s16 unk10;
    u8 padding;
    u8 maxItems;
} CharSelectIconEntry;

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
    u8 unkC;
    u8 blinkState;
    u8 paddingE[2];
} CharacterNameSprite;

extern struct {
    u16 unk0;
    u16 unk2;
} D_8008DE54_8EA54[];

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

extern Vec2_u16 playerNumberPositions[];
extern u8 D_8008DE18_8EA18[];
extern u8 D_8008DE64_8EA64[];
extern struct {
    u16 x;
    u16 y;
} D_8008DE9C_8EA9C[];
extern CharSelectModelPositions D_8008DD2C_8E92C;
extern Vec3s charSelectIconPositions[];
extern char D_8009E288_9EE88[];

void animateCharSelectIconReveal(CharSelectIconsState *);
void cleanupCharSelectIcons(SimpleSpriteEntry *);
void updateCharSelectIconsDelay(CharSelectIconsState *);
void updateCharSelectIconTargets(CharSelectIconTargetState *);
void updateCharSelectBoardSlideOut(CharSelectBoardPreview *);
void updateCharSelectBoardPreview(CharSelectBoardPreview *);
void updateCharSelectNameSprites(CharSelectNameSpritesState *);
void cleanupCharSelectNameSprites(SimpleSpriteEntry *);
void updateBoardSelectArrows(void *);
void cleanupBoardSelectArrows(SimpleSpriteEntry *);
void updateCharSelectMenu(SelectionMenuState *);
void cleanupCharSelectMenu(SimpleSpriteEntry *);
void updateCharSelectPreviewModel(CharSelectPreviewModel *);
void reloadCharSelectPreviewAssets(CharSelectPreviewModel *);
void initCharSelectSlidePosition(CharSelectPreviewModel *);
void cleanupCharSelectPreviewAssets(CharSelectPreviewModel *);
void updateCharSelectPreviewLighting(CharSelectPreviewModel *, u8);
void animateCharSelectP2NameReveal(P2NameAnimationState *);
void animateCharSelectP2NameHide(P2NameHideState *);
void func_800269C8_275C8(void *);
void func_80026BAC_277AC(SimpleSpriteEntry *);
void initCharSelectIconHideSprites(CharSelectIconHideState *);
void updateCharSelectIconsLockedState(CharSelectIconHideState *);
void showCharSelectIcons(CharSelectIconHideState *);
void updateCharSelectSecondarySlide(CharSelectSecondarySlot *);
void cleanupCharSelectSecondaryAssets(func_8002494C_arg *);
void recreateCharSelectBoardModel(CharSelectBoardPreview *);
void recreateCharSelectBoardModelForSlideIn(CharSelectBoardPreview *);
void initCharSelectBoardSlideIn(CharSelectBoardPreview *);
void waitForCharSelectBoardState(CharSelectBoardPreview *);
void updateCharSelectBoardSlideIn(CharSelectBoardPreview *);
void initCharSelectBoardSlideOut(CharSelectBoardPreview *);
SceneModel *cleanupSceneModelHolder(SceneModelHolder *arg0);
void cleanupCharSelectPlayerLabels(SimpleSpriteEntry *);
void updateCharSelectPlayerLabels(PlayerLabelSpritesState *);
void func_800262D4_26ED4(SelectionArrowsState *);
void initCharSelectBoardPreview(CharSelectBoardPreview *);
void cleanupCharSelectBoardModel(CharSelectBoardPreview *);
void cleanupCharSelectArrows(SimpleSpriteEntry *);
void updateCharSelectPlayerNumbers(u8 *);
void cleanupCharSelectPlayerNumbers(SimpleSpriteEntry *);
void updateCharSelectPlayer1NameSprite(SimpleSpriteEntry *);
void cleanupCharSelectPlayer1NameSprite(SimpleSpriteEntry *);
void cleanupCharSelectPlayer2NameSprites(SimpleSpriteEntry *arg0);
void waitForCharSelectP2NameReveal(P2NameRevealState *arg0);
void setupCharSelectP2NamePositions(volatile P2NameSpriteEntry *arg0);
void cleanupCharSelectBackgroundEffect(CharSelectBackgroundEffectState *state);
void setupCharSelectBackgroundEffect(CharSelectBackgroundEffectState *state);
void updateCharSelectBackgroundEffect(CharSelectBackgroundEffectState *state);
void renderCharSelectScaledSprite(void *);
void cleanupCharSelectScaledSprite(ScaledSpriteEntry *);
void updateCharSelectPostSlide(CharSelectSlideState *);
void updateCharSelectSlide(CharSelectSlideState *);
void hideCharSelectIcons(CharSelectIconHideState *);
void cleanupCharSelectIconHideAsset(SimpleSpriteEntry *);

void initCharSelectPreviewModel(CharSelectPreviewModel *arg0) {
    Transform3D sp10;
    Transform3D sp30;
    Transform3D *sp10Ptr;
    s32 paletteIndex;
    s32 sinCosResult;
    u8 charIndex;
    Transform3D *rotMatPtr;
    Transform3D *posMatPtr;
    u8 *gameState;
    u8 *globalPtr;

    gameState = (u8 *)getCurrentAllocation();
    posMatPtr = &arg0->positionMatrix;
    memcpy(posMatPtr, &identityMatrix, 0x20);
    rotMatPtr = &arg0->rotationMatrix;
    memcpy(rotMatPtr, posMatPtr, 0x20);
    memcpy(&sp30, rotMatPtr, 0x20);
    sp10Ptr = &sp10;
    memcpy(sp10Ptr, &sp30, 0x20);
    createRotationMatrixYX(sp10Ptr, 0x1000, 0x800);
    createZRotationMatrix(&sp30, 0x1F00);
    func_8006B084_6BC84(sp10Ptr, &sp30, rotMatPtr);

    sinCosResult = -(approximateSin(0x1800) * 0x1600);
    if (sinCosResult < 0) {
        sinCosResult += 0x1FFF;
    }
    arg0->positionMatrix.translation.x = (sinCosResult >> 0xD) << 8;

    sinCosResult = -(approximateCos(0x1800) * 0x1600);
    if (sinCosResult < 0) {
        sinCosResult += 0x1FFF;
    }
    arg0->positionMatrix.translation.z = (sinCosResult >> 0xD) << 8;
    memcpy(&sp10.translation, &arg0->positionMatrix.translation.x, 0xC);

    func_8006B084_6BC84(rotMatPtr, posMatPtr, sp10Ptr);
    func_8006B084_6BC84(sp10Ptr, gameState + ((arg0->playerIndex << 5) + 0x17F8), arg0);

    globalPtr = (u8 *)D_800AFE8C_A71FC;
    charIndex = *(globalPtr + arg0->playerIndex + 0xD);
    arg0->charPaletteIndex = charIndex;
    paletteIndex = EepromSaveData->character_or_settings[charIndex] - 1;

    arg0->modelAsset = loadAssetByIndex_95728(charIndex);
    arg0->animationAsset = loadAssetByIndex_95500(charIndex);
    arg0->skeletonAsset = loadAssetByIndex_95590(charIndex);
    arg0->paletteAsset = loadAssetByIndex_95668(paletteIndex & 0xFF);

    arg0->lightR = 0xFF;
    arg0->lightG = 0xFF;
    arg0->lightB = 0xFF;
    arg0->ambientR = 0x80;
    arg0->ambientG = 0x80;
    arg0->ambientB = 0x80;
    arg0->selectionState = *(gameState + arg0->playerIndex + 0x18C8);

    setCleanupCallback(cleanupCharSelectPreviewAssets);
    setCallback(updateCharSelectPreviewModel);
}

void updateCharSelectPreviewModel(CharSelectPreviewModel *arg0) {
    Transform3D sp10;
    GameState *state;
    u8 prevSelState;
    u8 newSelState;
    u8 charIndex;
    u8 paletteIndex;
    u8 assetIndex;
    u16 rotation;
    u16 val;

    state = (GameState *)getCurrentAllocation();

    newSelState = state->unk18C8[arg0->playerIndex];
    prevSelState = arg0->selectionState;

    if (prevSelState != newSelState) {
        arg0->selectionState = newSelState;
        updateCharSelectPreviewLighting(arg0, arg0->playerIndex);
    }

    charIndex = state->unk18A8[arg0->playerIndex];
    paletteIndex = state->unk18B0[arg0->playerIndex];
    assetIndex = paletteIndex + charIndex * 3;

    memcpy(&sp10, &identityMatrix, sizeof(Transform3D));
    memcpy(&sp10.translation, &arg0->positionMatrix.translation.x, 0xC);

    if (state->unk18D2[arg0->playerIndex] == state->unk18CC - 1) {
        val = state->unk1898[arg0->playerIndex];
        if (val != 1) {
            rotation = state->unk1888[arg0->playerIndex];
            createYRotationMatrix(&arg0->positionMatrix, rotation);
            goto after_rotation;
        }
    }

    rotation = state->unk1880[arg0->playerIndex];
    createYRotationMatrix(&arg0->positionMatrix, (0x2000 - rotation) & 0xFFFF);

after_rotation:
    func_8006B084_6BC84(&arg0->rotationMatrix, &arg0->positionMatrix, &sp10);
    func_8006B084_6BC84(&sp10, &state->unk17F8[arg0->playerIndex], arg0);

    val = state->unk1898[arg0->playerIndex];
    if (val == 4 || val == 9) {
        arg0->animationAsset = freeNodeMemory(arg0->animationAsset);
        arg0->skeletonAsset = freeNodeMemory(arg0->skeletonAsset);
        arg0->paletteAsset = freeNodeMemory(arg0->paletteAsset);
        setCallback(initCharSelectSlidePosition);
    } else {
        if (assetIndex != arg0->charPaletteIndex) {
            arg0->charPaletteIndex = assetIndex;
            arg0->animationAsset = freeNodeMemory(arg0->animationAsset);
            arg0->skeletonAsset = freeNodeMemory(arg0->skeletonAsset);
            arg0->paletteAsset = freeNodeMemory(arg0->paletteAsset);
            setCallback(reloadCharSelectPreviewAssets);
        } else {
            enqueueDisplayListObjectWithLights(arg0->playerIndex, (DisplayListObject *)arg0);
        }
    }
}

void reloadCharSelectPreviewAssets(CharSelectPreviewModel *preview) {
    u8 paletteIndex;

    paletteIndex = EepromSaveData->character_or_settings[preview->charPaletteIndex] - 1;

    preview->modelAsset = loadAssetByIndex_95728(preview->charPaletteIndex);
    preview->animationAsset = loadAssetByIndex_95500(preview->charPaletteIndex);
    preview->skeletonAsset = loadAssetByIndex_95590(preview->charPaletteIndex);
    preview->paletteAsset = loadAssetByIndex_95668(paletteIndex);

    setCallback(updateCharSelectPreviewModel);
}

void initCharSelectSlidePosition(CharSelectPreviewModel *arg0) {
    Transform3D sp10;
    Transform3D *localPtr;
    u8 *base;
    GameState *state;
    Transform3D *worldMatPtr;
    Transform3D *posMatPtr;
    s32 tableValue;
    u8 charIndex;
    u8 assetIndex;
    s32 paletteValue;
    u16 rotation;
    s32 offset;

    base = (u8 *)getCurrentAllocation();
    state = (GameState *)base;

    worldMatPtr = &arg0->worldMatrix;
    offset = arg0->playerIndex << 5;
    memcpy(worldMatPtr, (u8 *)(offset + (s32)base + 0x17F8), 0x20);

    tableValue = ((s32 *)&D_8008DD2C_8E92C)[(D_800AFE8C_A71FC->numPlayers * 2) + state->unk18C0[arg0->playerIndex]];
    arg0->worldMatrix.translation.x = tableValue;
    arg0->targetX = tableValue;

    charIndex = state->unk18A8[arg0->playerIndex];
    assetIndex = state->unk18B0[arg0->playerIndex];
    assetIndex += charIndex * 3;
    arg0->charPaletteIndex = assetIndex;

    paletteValue = EepromSaveData->character_or_settings[assetIndex] - 1;
    assetIndex &= 0xFF;

    arg0->modelAsset = loadAssetByIndex_95728(assetIndex);
    arg0->animationAsset = loadAssetByIndex_95500(assetIndex);
    arg0->skeletonAsset = loadAssetByIndex_95590(assetIndex);
    arg0->paletteAsset = loadAssetByIndex_95668((u8)paletteValue);

    localPtr = &sp10;
    memcpy(localPtr, &identityMatrix, 0x20);
    memcpy(&sp10.translation, &arg0->positionMatrix.translation.x, 0xC);

    posMatPtr = &arg0->positionMatrix;
    rotation = state->unk1888[arg0->playerIndex];
    createYRotationMatrix(posMatPtr, rotation);

    func_8006B084_6BC84(&arg0->rotationMatrix, posMatPtr, localPtr);
    func_8006B084_6BC84(localPtr, worldMatPtr, arg0);

    setCallback(updateCharSelectSlide);
}

void updateCharSelectSlide(CharSelectSlideState *arg0) {
    Transform3D sp10;
    GameState *state;
    Transform3D *localPtr;
    s32 adjustment;
    u16 rotation;

    state = (GameState *)getCurrentAllocation();

    localPtr = &sp10;

    adjustment = (-(0 < arg0->targetX) & 0xFFF00000) | 0x100000;

    memcpy(localPtr, &identityMatrix, sizeof(Transform3D));
    memcpy(&sp10.translation, &arg0->positionMatrix.translation.x, 0xC);

    arg0->worldMatrix.translation.x = arg0->worldMatrix.translation.x + adjustment;

    rotation = state->unk1888[arg0->playerIndex];
    createYRotationMatrix(&arg0->positionMatrix, rotation);

    func_8006B084_6BC84(&arg0->rotationMatrix, &arg0->positionMatrix, localPtr);
    func_8006B084_6BC84(localPtr, &arg0->worldMatrix, arg0);

    enqueueDisplayListObjectWithLights(arg0->playerIndex, (DisplayListObject *)arg0);

    if (arg0->worldMatrix.translation.x == 0) {
        state->unk18C0[arg0->playerIndex + 4]++;
        setCallbackWithContinue(updateCharSelectPostSlide);
    }
}

void updateCharSelectPostSlide(CharSelectSlideState *arg0) {
    Transform3D localMatrix;
    Transform3D *localPtr;
    GameState *base;
    u16 rotation;
    u16 val;

    base = (GameState *)getCurrentAllocation();

    localPtr = &localMatrix;
    memcpy(localPtr, &identityMatrix, sizeof(Transform3D));
    memcpy(&localMatrix.translation, &arg0->positionMatrix.translation.x, 0xC);

    rotation = base->unk1888[arg0->playerIndex];
    createYRotationMatrix(&arg0->positionMatrix, rotation);

    func_8006B084_6BC84(&arg0->rotationMatrix, &arg0->positionMatrix, localPtr);
    func_8006B084_6BC84(localPtr, &arg0->worldMatrix, arg0);

    enqueueDisplayListObjectWithLights(arg0->playerIndex, (DisplayListObject *)arg0);

    val = base->unk1898[arg0->playerIndex];
    if (val != 4 && val != 9) {
        setCallback(updateCharSelectPreviewModel);
    }
}

void cleanupCharSelectPreviewAssets(CharSelectPreviewModel *arg0) {
    arg0->animationAsset = freeNodeMemory(arg0->animationAsset);
    arg0->skeletonAsset = freeNodeMemory(arg0->skeletonAsset);
    arg0->paletteAsset = freeNodeMemory(arg0->paletteAsset);
}

void initCharSelectSecondarySlot(CharSelectSecondarySlot *arg0) {
    Transform3D localMatrix1;
    Transform3D localMatrix2;
    Transform3D localMatrix3;
    GameState *state;
    Transform3D *posMatrixPtr;
    Transform3D *rotMatrixPtr;
    Transform3D *localMatrix3Ptr;
    Transform3D *localMatrix2Ptr;
    Transform3D *localMatrix1Ptr;
    Transform3D *worldMatrixPtr;
    u8 charIndex;
    u8 assetIndex;
    u8 paletteIndex;
    u32 offset;

    state = (GameState *)getCurrentAllocation();

    localMatrix3Ptr = &localMatrix3;
    memcpy(localMatrix3Ptr, &identityMatrix, 0x20);
    posMatrixPtr = &arg0->positionMatrix;
    memcpy(posMatrixPtr, localMatrix3Ptr, 0x20);
    rotMatrixPtr = &arg0->rotationMatrix;
    memcpy(rotMatrixPtr, posMatrixPtr, 0x20);
    localMatrix2Ptr = &localMatrix2;
    memcpy(localMatrix2Ptr, rotMatrixPtr, 0x20);
    localMatrix1Ptr = &localMatrix1;
    memcpy(localMatrix1Ptr, localMatrix2Ptr, 0x20);

    offset = arg0->playerIndex << 5;
    worldMatrixPtr = &arg0->worldMatrix;
    memcpy(worldMatrixPtr, (void *)(offset + (u32)state + 0x17F8), 0x20);

    createRotationMatrixYX(localMatrix1Ptr, 0x1000, 0x800);
    createZRotationMatrix(localMatrix2Ptr, 0x1F00);
    func_8006B084_6BC84(localMatrix1Ptr, localMatrix2Ptr, rotMatrixPtr);

    createYRotationMatrix(posMatrixPtr, state->unk1888[arg0->playerIndex]);
    func_8006B084_6BC84(rotMatrixPtr, posMatrixPtr, localMatrix3Ptr);
    func_8006B084_6BC84(localMatrix3Ptr, worldMatrixPtr, arg0);

    charIndex = state->unk18A8[arg0->playerIndex + 4];
    assetIndex = state->unk18B0[arg0->playerIndex + 4];
    assetIndex = assetIndex + charIndex * 3;
    paletteIndex = EepromSaveData->character_or_settings[assetIndex] - 1;

    arg0->modelAsset = loadAssetByIndex_95728(assetIndex);
    arg0->animationAsset = loadAssetByIndex_95500(assetIndex);
    arg0->skeletonAsset = loadAssetByIndex_95590(assetIndex);
    arg0->paletteAsset = loadAssetByIndex_95668(paletteIndex);

    setCleanupCallback(cleanupCharSelectSecondaryAssets);
    setCallbackWithContinue(updateCharSelectSecondarySlide);
}

void updateCharSelectSecondarySlide(CharSelectSecondarySlot *arg0) {
    Transform3D localMatrix;
    Transform3D *localMatrixPtr;
    Transform3D *posMatrixPtr;
    s32 target;
    s32 adjustment;
    GameState *state;

    state = (GameState *)getCurrentAllocation();

    localMatrixPtr = &localMatrix;
    target =
        ((s32 *)&D_8008DD2C_8E92C)[(D_800AFE8C_A71FC->numPlayers * 2) + ((state->unk18C0[arg0->playerIndex] + 1) & 1)];
    adjustment = (-(target < 0) & 0xFFF00000) | 0x100000;

    memcpy(localMatrixPtr, &identityMatrix, 0x20);
    memcpy(&localMatrix.translation, &arg0->positionMatrix.translation.x, 0xC);

    arg0->worldMatrix.translation.x += adjustment;

    posMatrixPtr = &arg0->positionMatrix;
    createYRotationMatrix(posMatrixPtr, state->unk1888[arg0->playerIndex]);

    func_8006B084_6BC84(&arg0->rotationMatrix, posMatrixPtr, localMatrixPtr);
    func_8006B084_6BC84(localMatrixPtr, &arg0->worldMatrix, arg0);

    if (arg0->worldMatrix.translation.x == target) {
        state->unk18C0[arg0->playerIndex + 4]++;
        terminateCurrentTask();
    } else {
        enqueueDisplayListObject(arg0->playerIndex, (DisplayListObject *)arg0);
    }
}

void cleanupCharSelectSecondaryAssets(func_8002494C_arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
    arg0->unk2C = freeNodeMemory(arg0->unk2C);
}

void initCharSelectBoardModel(CharSelectBoardPreview *arg0) {
    GameState *state;
    u8 playerIdx;
    u32 boardType;
    s32 sinVal;
    s32 cosVal;

    state = (GameState *)getCurrentAllocation();
    playerIdx = arg0->playerIndex;

    boardType = D_800AFE8C_A71FC->unk9[playerIdx];
    if (boardType == 7) {
        arg0->model =
            createSceneModelEx(0x39, (void *)state + (playerIdx * 0x1D8), state->PAD_22[playerIdx], -1, -1, -1);
    } else {
        arg0->model =
            createSceneModelEx(boardType, (void *)state + (playerIdx * 0x1D8), state->PAD_22[playerIdx], -1, -1, -1);
    }

    memcpy(&arg0->transform, &identityMatrix, 0x20);

    sinVal = -(approximateSin(0x800) * 0x1600);
    if (sinVal < 0) {
        sinVal += 0x1FFF;
    }
    arg0->transform.translation.x = (sinVal >> 13) << 8;

    cosVal = -(approximateCos(0x800) * 0x1600);
    if (cosVal < 0) {
        cosVal += 0x1FFF;
    }
    arg0->transform.translation.z = (cosVal >> 13) << 8;
    arg0->transform.translation.y = 0xFFF00000;

    setCleanupCallback(cleanupCharSelectBoardModel);
    setCallback(initCharSelectBoardPreview);
}

void initCharSelectBoardPreview(CharSelectBoardPreview *arg0) {
    Transform3D localMatrix;
    Transform3D *localPtr;
    GameState *state;
    Transform3D *transformPtr;
    u16 rotation;

    state = (GameState *)getCurrentAllocation();

    localPtr = &localMatrix;
    memcpy(localPtr, &identityMatrix, sizeof(Transform3D));

    transformPtr = &arg0->transform;
    rotation = state->unk1880[arg0->playerIndex];
    createYRotationMatrix(transformPtr, 0x2000 - rotation);

    func_8006B084_6BC84(transformPtr, &state->unk17F8[arg0->playerIndex], localPtr);

    applyTransformToModel(arg0->model, localPtr);

    if (D_800AFE8C_A71FC->unk9[arg0->playerIndex] == 7) {
        setModelAnimation(arg0->model, 4);
    } else {
        setModelAnimation(arg0->model, 0x90);
    }

    updateModelGeometry(arg0->model);
    setCallback(updateCharSelectBoardPreview);
}

void updateCharSelectBoardPreview(CharSelectBoardPreview *arg0) {
    Transform3D localMatrix;
    Transform3D *localPtr;
    GameState *state;
    Transform3D *transformPtr;
    u16 rotation;
    u16 val;

    state = (GameState *)getCurrentAllocation();

    localPtr = &localMatrix;
    memcpy(localPtr, &identityMatrix, sizeof(Transform3D));

    transformPtr = &arg0->transform;
    rotation = state->unk1880[arg0->playerIndex];
    createYRotationMatrix(transformPtr, 0x2000 - rotation);

    func_8006B084_6BC84(transformPtr, &state->unk17F8[arg0->playerIndex], localPtr);

    applyTransformToModel(arg0->model, localPtr);

    clearModelRotation(arg0->model);
    updateModelGeometry(arg0->model);

    val = state->unk1898[arg0->playerIndex];
    if (val == 0x10) {
        destroySceneModel(arg0->model);
        setCallback(recreateCharSelectBoardModelForSlideIn);
    }
}

void dispatchCharSelectBoardState(CharSelectBoardPreview *arg0) {
    u16 *statePtr;
    u16 boardState;

    statePtr = (u16 *)getCurrentAllocation();

    clearModelRotation(arg0->model);
    updateModelGeometry(arg0->model);

    boardState = *(statePtr + arg0->playerIndex + (0x1898 / 2));

    if (boardState == 0x10) {
        destroySceneModel(arg0->model);
        setCallback(recreateCharSelectBoardModelForSlideIn);
    } else if (boardState == 0) {
        arg0->transform.translation.x = 0xFFEA0000;
        destroySceneModel(arg0->model);
        setCallback(recreateCharSelectBoardModel);
    } else if (boardState == 0x11) {
        arg0->transform.translation.x = 0xFFEA0000;
        setCallback(updateCharSelectBoardPreview);
    }
}

void recreateCharSelectBoardModel(CharSelectBoardPreview *arg0) {
    u8 *sessionPtr;
    s32 boardType;
    u8 *alloc;
    s32 playerIdx;

    alloc = (u8 *)getCurrentAllocation();
    playerIdx = arg0->playerIndex;

    sessionPtr = (u8 *)D_800AFE8C_A71FC;
    sessionPtr = sessionPtr + playerIdx;
    boardType = sessionPtr[9];

    if (boardType != 7) {
        arg0->model =
            createSceneModelEx(boardType, alloc + (playerIdx * 0x1D8), (alloc + playerIdx)[0x18B8], -1, -1, -1);
    } else {
        arg0->model = createSceneModelEx(0x39, alloc + (playerIdx * 0x1D8), (alloc + playerIdx)[0x18B8], -1, -1, -1);
    }
    setCallback(initCharSelectBoardPreview);
}

void recreateCharSelectBoardModelForSlideIn(CharSelectBoardPreview *arg0) {
    GameState *state;
    u32 boardType;

    state = (GameState *)getCurrentAllocation();
    boardType = D_800AFE8C_A71FC->unk9[arg0->playerIndex];
    if (boardType == 7) {
        arg0->model = createSceneModelEx(
            0x39,
            (u8 *)state + (arg0->playerIndex * 0x1D8),
            state->PAD_22[arg0->playerIndex],
            -1,
            -1,
            -1
        );
    } else {
        arg0->model = createSceneModelEx(
            boardType,
            (u8 *)state + (arg0->playerIndex * 0x1D8),
            state->PAD_22[arg0->playerIndex],
            -1,
            -1,
            -1
        );
    }
    setCallback(initCharSelectBoardSlideIn);
}

void initCharSelectBoardSlideIn(CharSelectBoardPreview *preview) {
    u8 *state;
    u8 *sessionPtr;
    u8 playerIdx;

    state = (u8 *)getCurrentAllocation();

    memcpy(&preview->transform, &identityMatrix, 0x20);

    playerIdx = preview->playerIndex;
    preview->transform.translation.x =
        ((s32 *)&D_8008DD2C_8E92C)[D_800AFE8C_A71FC->numPlayers * 2 + (state + playerIdx)[0x18C0]];
    preview->transform.translation.z = 0;
    preview->transform.translation.y = 0xFFF00000;
    preview->unk20_u.targetX = preview->transform.translation.x;

    applyTransformToModel(preview->model, &preview->transform);

    sessionPtr = (u8 *)D_800AFE8C_A71FC;
    sessionPtr = sessionPtr + preview->playerIndex;
    if (sessionPtr[9] == 7) {
        setModelAnimation(preview->model, 4);
    } else {
        setModelAnimation(preview->model, 0x90);
    }

    updateModelGeometry(preview->model);
    setCallback(updateCharSelectBoardSlideIn);
}

void updateCharSelectBoardSlideIn(CharSelectBoardPreview *preview) {
    u8 *state;
    s32 targetX;
    s32 slideStep;
    u8 *statePtr;

    state = (u8 *)getCurrentAllocation();

    targetX = preview->unk20_u.targetX;

    slideStep = (-(0 < targetX) & 0xFFF00000) | 0x100000;

    preview->transform.translation.x = preview->transform.translation.x + slideStep;

    applyTransformToModel(preview->model, &preview->transform);
    clearModelRotation(preview->model);
    updateModelGeometry(preview->model);

    if (preview->transform.translation.x == 0) {
        statePtr = state + preview->playerIndex;
        statePtr[0x18C4]++;
        setCallbackWithContinue(waitForCharSelectBoardState);
    }
}

void waitForCharSelectBoardState(CharSelectBoardPreview *preview) {
    u16 *state;
    u16 boardState;

    state = (u16 *)getCurrentAllocation();

    clearModelRotation(preview->model);
    updateModelGeometry(preview->model);

    boardState = *(state + preview->playerIndex + (0x1898 / 2));

    if (boardState != 0x10) {
        setCallback(dispatchCharSelectBoardState);
    }
}

void cleanupCharSelectBoardModel(CharSelectBoardPreview *preview) {
    destroySceneModel(preview->model);
}

void initCharSelectBoardModelForSlideOut(CharSelectBoardPreview *arg0) {
    u8 *alloc;
    u8 playerIndex;
    u32 boardType;

    alloc = (u8 *)getCurrentAllocation();
    playerIndex = arg0->playerIndex;
    boardType = D_800AFE8C_A71FC->unk9[playerIndex];

    if (boardType != 7) {
        arg0->model =
            createSceneModelEx(boardType, alloc + (playerIndex * 0x1D8), (alloc + playerIndex)[0x18BC], -1, -1, -1);
    } else {
        arg0->model =
            createSceneModelEx(0x39, alloc + (playerIndex * 0x1D8), (alloc + playerIndex)[0x18BC], -1, -1, -1);
    }
    memcpy(&arg0->transform, &identityMatrix, 0x20);
    arg0->transform.translation.x = 0;
    arg0->transform.translation.z = 0;
    arg0->transform.translation.y = 0xFFF00000;
    setCleanupCallback(cleanupSceneModelHolder);
    setCallback(initCharSelectBoardSlideOut);
}
void initCharSelectBoardSlideOut(CharSelectBoardPreview *preview) {
    u8 *base;

    getCurrentAllocation();

    applyTransformToModel(preview->model, &preview->transform);

    base = (u8 *)D_800AFE8C_A71FC;
    if (*(base + preview->playerIndex + 9) == 7) {
        setModelAnimation(preview->model, 4);
    } else {
        setModelAnimation(preview->model, 0x90);
    }

    updateModelGeometry(preview->model);
    setCallback(&updateCharSelectBoardSlideOut);
}

void updateCharSelectBoardSlideOut(CharSelectBoardPreview *preview) {
    u8 *state;
    s32 target;
    s32 slideStep = 0x100000;
    s32 slideMask = 0xFFF00000;

    state = (u8 *)getCurrentAllocation();

    target = ((
        s32 *
    )&D_8008DD2C_8E92C)[D_800AFE8C_A71FC->numPlayers * 2 + (((state + preview->playerIndex)[0x18C0] + 1) & 1)];

    preview->transform.translation.x += ((target >> 31) & slideMask) | slideStep;

    applyTransformToModel(preview->model, &preview->transform);
    clearModelRotation(preview->model);
    updateModelGeometry(preview->model);

    if (preview->transform.translation.x == target) {
        (state + preview->playerIndex)[0x18C4]++;
        terminateCurrentTask();
    }
}

SceneModel *cleanupSceneModelHolder(SceneModelHolder *arg0) {
    return destroySceneModel(arg0->model);
}

#define ICON_TABLE_INDEX 0xB

void initCharSelectIcons(CharSelectIconsState *arg0) {
    OutputStruct_19E80 sp10;
    u8 *tablePtr;
    volatile CharSelectIconEntry *iconEntry;
    DataTable_19E80 *spriteAsset;
    s16 scaleX;
    s16 scaleY;
    s32 i;
    s32 xIncrement;
    u16 yPos;
    u16 xPos;
    u16 xTemp;
    u16 xIncrementU16;
    s32 temp_v0;
    u8 numPlayers;
    s32 iconTableIndex;

    spriteAsset = loadCompressedData(&_4237C0_ROM_START, &_4237C0_ROM_END, 0x8A08);
    setCleanupCallback(cleanupCharSelectIcons);
    getTableEntryByU16Index(spriteAsset, 0xB, &sp10);

    numPlayers = D_800AFE8C_A71FC->numPlayers;

    yPos = *(u16 *)((u8 *)playerNumberPositions + numPlayers * 6 + 14);
    xTemp = *(u16 *)((u8 *)playerNumberPositions + numPlayers * 6 + 16);
    xIncrementU16 = *(u16 *)((u8 *)playerNumberPositions + numPlayers * 6 + 18);

    scaleX = 0x400;
    if (numPlayers == 1) {
        scaleX = 0x200;
        scaleY = 0x200;
    } else {
        scaleY = 0x400;
    }

    i = 0;
    xIncrement = (s32)(s16)xIncrementU16;
    iconTableIndex = ICON_TABLE_INDEX;
    tablePtr = D_8008DE18_8EA18;
    xPos = xTemp;
    iconEntry = (volatile CharSelectIconEntry *)arg0;

    do {
        u8 tableVal;
        iconEntry->baseY = yPos;
        iconEntry->x = xPos;
        iconEntry->spriteIndex = iconTableIndex;
        tableVal = tablePtr[1];
        tablePtr += 2;
        iconEntry->padding = 0;
        iconEntry->scaleX = scaleX;
        iconEntry->scaleY = scaleY;
        iconEntry->spriteAsset = spriteAsset;
        iconEntry->currentY = 0;
        iconEntry->maxItems = (s8)(tableVal + 1);
        xPos += xIncrement;
        i++;
        iconEntry->unk10 = sp10.field2;
        iconEntry++;
    } while (i < 3);

    arg0->numVisibleIcons = 1;
    arg0->revealCounter = 0;
    arg0->delayTimer = 8;
    setCallback(updateCharSelectIconsDelay);
}

void updateCharSelectIconsDelay(CharSelectIconsState *arg0) {
    if (--arg0->delayTimer == 0) {
        setCallback(animateCharSelectIconReveal);
    }
}

extern u8 D_8008DD8C_8E98C[];
extern u8 D_8008DDEC_8E9EC[];

void animateCharSelectIconReveal(CharSelectIconsState *arg0) {
    u8 *gameState;
    s32 i;
    s32 iconsStillAnimating;
    CharSelectIconEntry *entry;
    u8 charIndex;
    u8 paletteIndex;
    u8 itemIconIndex;
    s16 targetY;
    u16 currentY;
    s16 newY;
    u8 *ptr;

    gameState = (u8 *)getCurrentAllocation();
    iconsStillAnimating = 0;

    // Animate icon Y positions towards their target values
    for (i = 0; i < arg0->numVisibleIcons; i++) {
        ptr = gameState + arg0->playerIndex;
        charIndex = ptr[0x18A8];
        paletteIndex = ptr[0x18B0];
        // Each character has 3 board options (palette 0-2), with 3 items each
        itemIconIndex = D_8008DD8C_8E98C[(((u8)(paletteIndex + charIndex * 3)) * 3) + i];
        targetY = *(s16 *)(D_8008DDEC_8E9EC + itemIconIndex * 2 + 22);

        entry = &arg0->entries[i];
        currentY = entry->currentY;

        if ((currentY & 0xFFFF) < targetY) {
            if (currentY < 0x10) {
                newY = currentY + 8;
            } else {
                newY = currentY + 0xC;
            }
            entry->currentY = newY;
            iconsStillAnimating++;
        }
    }

    // Gradually reveal more icons (1 -> 2 -> 3) over time
    if (arg0->numVisibleIcons < 3) {
        arg0->revealCounter = (arg0->revealCounter + 1) & 3;
        if (arg0->revealCounter == 0) {
            arg0->numVisibleIcons++;
        }
    } else {
        if (iconsStillAnimating == 0) {
            setCallback(updateCharSelectIconTargets);
        }
    }

    // Render all 3 icon slots
    for (i = 0; i < 3; i++) {
        debugEnqueueCallback(arg0->playerIndex + 8, 0, func_80010C98_11898, &arg0->entries[i]);
    }

    // If character selection is confirmed, skip animation
    if (((u16 *)gameState)[(arg0->playerIndex * 2 + 0x1898) / 2] == 9) {
        setCallback(updateCharSelectIconTargets);
    }
}

void updateCharSelectIconTargets(CharSelectIconTargetState *arg0) {
    GameState *state;
    s32 i;
    CharSelectIconEntry *entry;
    u8 tableIndex;
    u8 charIndex;
    u8 paletteIndex;

    state = (GameState *)getCurrentAllocation();
    i = 0;

    do {
        charIndex = state->unk18A8[arg0->playerIndex];
        paletteIndex = state->unk18B0[arg0->playerIndex];
        tableIndex = D_8008DD8C_8E98C[((u8)(paletteIndex + charIndex * 3)) * 3 + i];
        entry = &arg0->entries[i];
        entry->currentY = *(s16 *)(D_8008DDEC_8E9EC + tableIndex * 2 + 22);
        debugEnqueueCallback(arg0->playerIndex + 8, 0, func_80010C98_11898, entry);
        i++;
    } while (i < 3);
}

void cleanupCharSelectIcons(SimpleSpriteEntry *arg0) {
    arg0->asset = freeNodeMemory(arg0->asset);
}

// Initialize character select item icon sprites for display/hide animation
// Sets up 3 item icons per character, positioned based on player count
void initCharSelectIconHideSprites(CharSelectIconHideState *arg0) {
    void *spriteAsset;
    u8 numPlayers;
    s32 i;
    s32 iconBaseIndex;
    u16 x;
    u16 y;
    s32 yIncrement;
    volatile P2NameSpriteEntry *entry;
    u8 *iconMappingTable;
    s32 pad[4];

    (void)pad;

    spriteAsset = loadCompressedData(&_4237C0_ROM_START, &_426EF0_ROM_START, 0x8A08);
    setCleanupCallback(cleanupCharSelectIconHideAsset);

    numPlayers = D_800AFE8C_A71FC->numPlayers;
    iconBaseIndex = 0xC;
    if (numPlayers == 1) {
        iconBaseIndex = 0x11;
    }

    x = charSelectIconPositions[numPlayers].y;
    i = 0;
    do {
    } while (0);
    yIncrement = *(s16 *)((u8 *)&charSelectIconPositions[numPlayers] + 6);
    iconMappingTable = D_8008DD8C_8E98C;
    y = charSelectIconPositions[numPlayers].z;
    entry = (volatile P2NameSpriteEntry *)arg0->entries;

    do {
        D_800AFE8C_A71FC_type *global;
        u8 charIndex;
        u8 tableValue;
        s32 tableOffset;

        global = D_800AFE8C_A71FC;
        entry->x = x;
        entry->y = y;

        charIndex = ((u8 *)global + arg0->playerIndex)[0xD];
        tableOffset = charIndex * 3 + i;
        tableValue = *(u8 *)(tableOffset + (s32)iconMappingTable);

        entry->spriteIndex = iconBaseIndex + (tableValue - 1) / 2;

        global = D_800AFE8C_A71FC;
        charIndex = ((u8 *)global + arg0->playerIndex)[0xD];
        tableOffset = charIndex * 3 + i;
        tableValue = *(u8 *)(tableOffset + (s32)iconMappingTable);

        y += yIncrement;
        i += 1;
        entry->asset = spriteAsset;
        entry->paletteIndex = (u8)(((tableValue - 1) / 2 + 7) & 0xFF) % 11;
        entry++;
    } while (i < 3);

    setCallback(hideCharSelectIcons);
}

// Render the 3 item icons for character selection
// If character selection is confirmed (state 3), switch to locked state icons
void hideCharSelectIcons(CharSelectIconHideState *arg0) {
    P2NameSpriteEntry *entry;
    GameState *state;
    s32 i;
    s16 iconIndex;
    u8 constant;

    state = (GameState *)getCurrentAllocation();

    i = 0;
    entry = arg0->entries;
    do {
        debugEnqueueCallback(arg0->playerIndex + 8, 0, renderSpriteFrameWithPalette, entry);
        entry++;
        i++;
    } while (i < 3);

    if (state->unk1898[arg0->playerIndex] == 3) {
        iconIndex = 0xD;
        if (D_800AFE8C_A71FC->numPlayers == 1) {
            iconIndex = 0x12;
        }

        i = 0;
        constant = 8;
        do {
            arg0->entries[i].spriteIndex = iconIndex;
            arg0->entries[i].paletteIndex = constant;
            i++;
        } while (i < 3);

        setCallback(updateCharSelectIconsLockedState);
    }
}

void updateCharSelectIconsLockedState(CharSelectIconHideState *arg0) {
    GameState *state;
    s32 i;
    P2NameSpriteEntry *entry;
    u16 charSelectState;
    s32 iconBaseIndex;
    u8 charIndex;
    u8 paletteIndex;
    s32 itemTableOffset;
    u8 *itemIconTable;
    u8 *itemIconPtr;

    state = (GameState *)getCurrentAllocation();

    // Render the 3 item icons for the current character selection
    i = 0;
    entry = arg0->entries;
    do {
        debugEnqueueCallback(arg0->playerIndex + 8, 0, renderSpriteFrameWithPalette, entry);
        entry++;
        i++;
    } while (i < 3);

    charSelectState = state->unk1898[arg0->playerIndex];

    // If character selection is confirmed, transition to showing the icons
    if (charSelectState == 0xA) {
        setCallback(showCharSelectIcons);
        return;
    }

    // If still in character selection mode (states 0-2), update icons to match
    // the selected character's item set
    if (charSelectState < 3) {
        // Icon base index depends on player count (different layouts for 1P vs 2P+)
        iconBaseIndex = 0xC;
        if (D_800AFE8C_A71FC->numPlayers == 1) {
            iconBaseIndex = 0x11;
        }

        charIndex = state->unk18A8[arg0->playerIndex];
        i = 0;
        itemIconTable = D_8008DD8C_8E98C;
        paletteIndex = state->unk18B0[arg0->playerIndex];
        // Calculate offset into item icon table:
        // Each character has 3 board options (palette 0-2), and 3 items per option
        itemTableOffset = ((u8)(paletteIndex + charIndex * 3)) * 3;

        // Update each of the 3 item icons based on the selected character+board combo
        do {
            itemIconPtr = (u8 *)((itemTableOffset + i) + (u32)itemIconTable);
            arg0->entries[i].spriteIndex = iconBaseIndex + (*itemIconPtr - 1) / 2;
            arg0->entries[i].paletteIndex = (u8)(((*itemIconPtr - 1) / 2 + 7) & 0xFF) % 11;
            i++;
        } while (i < 3);

        setCallback(hideCharSelectIcons);
    }
}

// Show character select item icons after selection is confirmed
// Renders the 3 item icons and transitions to locked state if needed
void showCharSelectIcons(CharSelectIconHideState *arg0) {
    GameState *state;
    s32 i;
    s32 iconBaseIndex;
    s32 tableOffset;
    P2NameSpriteEntry *entry;
    u8 charIndex;
    u8 paletteIndex;
    u8 *tableBase;
    u8 *tablePtr;

    state = (GameState *)getCurrentAllocation();

    iconBaseIndex = 0xC;
    if (D_800AFE8C_A71FC->numPlayers == 1) {
        iconBaseIndex = 0x11;
    }

    charIndex = state->unk18A8[arg0->playerIndex];
    i = 0;
    tableBase = D_8008DD8C_8E98C;
    paletteIndex = state->unk18B0[arg0->playerIndex];
    entry = arg0->entries;
    tableOffset = ((u8)(paletteIndex + charIndex * 3)) * 3;

loop:
    tablePtr = (u8 *)((tableOffset + i) + (u32)tableBase);
    entry->spriteIndex = iconBaseIndex + (*tablePtr - 1) / 2;
    i++;
    entry->paletteIndex = (u8)(((*tablePtr - 1) / 2 + 7) & 0xFF) % 11;
    debugEnqueueCallback(arg0->playerIndex + 8, 0, renderSpriteFrameWithPalette, entry);
    entry++;
    if (i < 3)
        goto loop;

    if (state->unk1898[arg0->playerIndex] == 3) {
        iconBaseIndex = 0xD;
        if (D_800AFE8C_A71FC->numPlayers == 1) {
            iconBaseIndex = 0x12;
        }

        i = 0;
        do {
            arg0->entries[i].spriteIndex = iconBaseIndex;
            arg0->entries[i].paletteIndex = 8;
            i++;
        } while (i < 3);

        setCallback(updateCharSelectIconsLockedState);
    }
}

void cleanupCharSelectIconHideAsset(SimpleSpriteEntry *arg0) {
    arg0->asset = freeNodeMemory(arg0->asset);
}

void initCharSelectMenu(SelectionMenuState *arg0) {
    void *dmaResult;
    u16 x;
    u16 y;
    u16 xIncrement;
    s32 spriteIndexBase;
    s32 count;
    s32 i;
    s32 incrementSigned;
    s32 pad[6];

    (void)pad;

    dmaResult = loadCompressedData(&_4237C0_ROM_START, &_4237C0_ROM_END, 0x8A08);
    setCleanupCallback(cleanupCharSelectMenu);

    spriteIndexBase = 8;
    if (D_800AFE8C_A71FC->numPlayers == 1) {
        spriteIndexBase = 5;
    }

    arg0->numEntries = 3;

    if (D_800AFE8C_A71FC->unk4 == 0) {
        x = charSelectIconPositions[0].x;
        y = charSelectIconPositions[0].y;
        xIncrement = charSelectIconPositions[0].z;
        spriteIndexBase = 6;
        arg0->numEntries = 2;
    } else {
        s32 index = D_800AFE8C_A71FC->numPlayers;
        x = D_8008DD2C_8E92C.unk22[index].x;
        y = D_8008DD2C_8E92C.unk22[index].y;
        xIncrement = D_8008DD2C_8E92C.unk22[index].z;
    }

    count = arg0->numEntries;
    if (count > 0) {
        i = 0;
        incrementSigned = (s16)xIncrement;
        do {
            arg0->entries[i].x = x;
            arg0->entries[i].y = y;
            arg0->entries[i].spriteIndex = spriteIndexBase + i;
            arg0->entries[i].blinkState = 0;
            arg0->entries[i].unkC = 0;
            arg0->entries[i].alpha = 0xFF;
            arg0->entries[i].asset = dmaResult;
            arg0->blinkTimers[i] = 0;
            i++;
            x += incrementSigned;
        } while (i < (s32)arg0->numEntries);
    }

    setCallback(updateCharSelectMenu);
}

void updateCharSelectMenuConfirm(SelectionMenuState *);

void updateCharSelectMenu(SelectionMenuState *menu) {
    GameState *state;
    s32 entryIndex;
    SelectionEntry *entries;
    u8 blinkCounter;
    u32 blinkPhase;
    s16 fullAlpha;
    s32 pad[2];
    s32 numEntries;

    (void)pad;

    state = getCurrentAllocation();
    entryIndex = 0;
    numEntries = menu->numEntries;
    if (numEntries > 0) {
        fullAlpha = 0xFF;
        entries = menu->entries;
        do {
            if (state->unk18D2[menu->playerIndex] == entryIndex) {
                if (state->unk1898[menu->playerIndex] == 0) {
                    blinkCounter = menu->blinkTimers[entryIndex] + 1;
                    blinkPhase = blinkCounter & 0xFF;
                    menu->blinkTimers[entryIndex] = blinkCounter;
                    if (blinkPhase < 0x11) {
                        entries[entryIndex].alpha = fullAlpha - (blinkPhase * 8);
                    } else {
                        entries[entryIndex].alpha = (blinkPhase * 8) - 1;
                    }
                    menu->blinkTimers[entryIndex] = menu->blinkTimers[entryIndex] & 0x1F;
                } else {
                    menu->blinkTimers[entryIndex] = 0;
                    entries[entryIndex].alpha = fullAlpha;
                }
            } else {
                entries[entryIndex].alpha = 0x50;
                menu->blinkTimers[entryIndex] = 0;
            }
            debugEnqueueCallback(menu->playerIndex + 0xC, 0, renderTextSprite, &entries[entryIndex]);
            entryIndex++;
        } while (entryIndex < (s32)menu->numEntries);
    }

    if (state->unk1898[menu->playerIndex] == 2) {
        setCallbackWithContinue(updateCharSelectMenuConfirm);
    }
}

void updateCharSelectMenuConfirm(SelectionMenuState *menu) {
    u8 *state;
    s32 entryIndex;
    s32 selectedIndex;
    SelectionEntry *entries;
    s32 pad[2];
    s32 numEntries;

    (void)pad;

    state = (u8 *)getCurrentAllocation();
    numEntries = menu->numEntries;
    entryIndex = 0;
    if (numEntries > 0) {
        entries = menu->entries;
        do {
            menu->blinkTimers[entryIndex] = 0;
            if ((state + menu->playerIndex)[0x18D2] == entryIndex) {
                entries[entryIndex].alpha = 0xFF;
                selectedIndex = entryIndex;
                if (*(u16 *)(state + menu->playerIndex * 2 + 0x18A0) & 1) {
                    entries[entryIndex].blinkState = 0xFF;
                } else {
                    entries[entryIndex].blinkState = 0;
                }
            } else {
                entries[entryIndex].alpha = 0x50;
            }
            debugEnqueueCallback(menu->playerIndex + 0xC, 0, renderTextSprite, &entries[entryIndex]);
            entryIndex++;
        } while (entryIndex < (s32)menu->numEntries);
    }

    if (*(u16 *)(state + menu->playerIndex * 2 + 0x1898) != 2) {
        menu->entries[selectedIndex].blinkState = 0;
        setCallbackWithContinue(updateCharSelectMenu);
    }
}

void cleanupCharSelectMenu(SimpleSpriteEntry *arg0) {
    arg0->asset = freeNodeMemory(arg0->asset);
}

void initCharSelectPlayerLabels(SimpleSpriteEntry *arg0) {
    void *dmaResult;
    s32 i;
    s32 index;
    u16 x;
    s32 increment;
    u16 y;

    dmaResult = loadCompressedData(&_4237C0_ROM_START, &_4237C0_ROM_END, 0x8A08);
    setCleanupCallback(cleanupCharSelectPlayerLabels);

    i = 0;
    index = D_800AFE8C_A71FC->numPlayers;
    x = *(u16 *)(D_8008DD8C_8E98C + index * 6 + 50);
    increment = *(s16 *)(D_8008DD8C_8E98C + index * 6 + 54);
    y = *(u16 *)(D_8008DD8C_8E98C + index * 6 + 52);

    do {
        arg0[i].y = y;
        y += increment;
        arg0[i].x = x;
        arg0[i].spriteIndex = i + 2;
        arg0[i].asset = dmaResult;
        i++;
    } while (i < 3);

    setCallback(updateCharSelectPlayerLabels);
}

void updateCharSelectPlayerLabels(PlayerLabelSpritesState *arg0) {
    s32 i;
    s32 j;
    u16 index;
    func_80027348_entry *ptr;

    i = 0;
    if (i < (D_800AFE8C_A71FC)->numPlayers) {
        j = 0;
        do {
            index = i + 8;
            ptr = arg0->entries;
            do {
                debugEnqueueCallback(index, 7, &renderSpriteFrame, ptr);
                j++;
                ptr++;
            } while (j < 3);
            i++;
            j = 0;
        } while (i < (D_800AFE8C_A71FC)->numPlayers);
    }
}

void cleanupCharSelectPlayerLabels(SimpleSpriteEntry *arg0) {
    arg0->asset = freeNodeMemory(arg0->asset);
}

void initCharSelectArrows(SelectionArrowsState *arg0) {
    void *dmaResult;
    u8 count;
    s32 i;
    s32 j;
    s32 index;
    u16 xBase;
    u16 y;
    u16 xInc;
    s32 xIncrement;
    SelectionEntry *ptr;
    u16 x;
    s32 iTimesTwo;
    s32 pad[2];

    (void)pad;

    getCurrentAllocation();
    dmaResult = loadCompressedData(&_4237C0_ROM_START, &_4237C0_ROM_END, 0x8A08);
    setCleanupCallback(cleanupCharSelectArrows);

    count = D_800AFE8C_A71FC->numPlayers;
    xBase = *(u16 *)(D_8008DE18_8EA18 + count * 6 + 2);
    y = *(u16 *)(D_8008DE18_8EA18 + count * 6 + 4);
    xInc = *(u16 *)(D_8008DE18_8EA18 + count * 6 + 6);

    i = 0;
    if (count != 0) {
        xIncrement = (s16)xInc;
        do {
            j = 0;
            iTimesTwo = i * 2;
            x = xBase;
            do {
                ptr = &arg0->entries[iTimesTwo + j];
                ((volatile SelectionEntry *)ptr)->x = x;
                ((volatile SelectionEntry *)ptr)->spriteIndex = j;
                j++;
                ptr->y = y;
                ptr->asset = dmaResult;
                ptr->alpha = 0xFF;
                ptr->blinkState = 0;
                ptr->unkC = 0;
                x += xIncrement;
            } while (j < 2);
            arg0->blinkTimers[i] = 0;
            i++;
        } while (i < D_800AFE8C_A71FC->numPlayers);
    }

    setCallback(func_800262D4_26ED4);
}

INCLUDE_ASM("asm/nonmatchings/24A30", func_800262D4_26ED4);

void cleanupCharSelectArrows(SimpleSpriteEntry *arg0) {
    arg0->asset = freeNodeMemory(arg0->asset);
}

void initBoardSelectArrows(SelectionArrowsState *state) {
    void *arrowAsset;
    u8 numPlayers;
    s32 playerIdx;
    s32 arrowIdx;
    s32 unused;
    u16 xBase;
    u16 y;
    u16 xSpacing;
    s32 xOffset;
    SelectionEntry *arrow;
    u16 x;
    s32 entryOffset;
    s32 pad[2];

    (void)pad;

    getCurrentAllocation();
    arrowAsset = loadCompressedData(&_4237C0_ROM_START, &_4237C0_ROM_END, 0x8A08);
    setCleanupCallback(cleanupBoardSelectArrows);

    numPlayers = D_800AFE8C_A71FC->numPlayers;
    unused = numPlayers * 3;
    xBase = *(s16 *)(D_8008DE18_8EA18 + numPlayers * 6 + 34);
    y = *(s16 *)(D_8008DE18_8EA18 + numPlayers * 6 + 36);
    xSpacing = *(s16 *)(D_8008DE18_8EA18 + numPlayers * 6 + 38);

    playerIdx = 0;
    if (numPlayers != 0) {
        xOffset = (s16)xSpacing;
        do {
            state->blinkTimers[playerIdx] = 0;
            arrowIdx = 0;
            entryOffset = playerIdx * 2;
            x = xBase;
            do {
                arrow = &state->entries[entryOffset + arrowIdx];
                ((volatile SelectionEntry *)arrow)->x = x;
                ((volatile SelectionEntry *)arrow)->spriteIndex = arrowIdx;
                arrowIdx++;
                arrow->y = y;
                arrow->asset = arrowAsset;
                arrow->alpha = 0xFF;
                arrow->blinkState = 0;
                arrow->unkC = 0;
                x += xOffset;
            } while (arrowIdx < 2);
            playerIdx++;
        } while (playerIdx < D_800AFE8C_A71FC->numPlayers);
    }

    setCallback(updateBoardSelectArrows);
}

INCLUDE_ASM("asm/nonmatchings/24A30", updateBoardSelectArrows);

void cleanupBoardSelectArrows(SimpleSpriteEntry *arg0) {
    arg0->asset = freeNodeMemory(arg0->asset);
}

void initBoardSelectCharNames(CharacterNameSprite *sprites) {
    GameState *gameState;
    void *spriteAsset;
    s32 i;
    u16 xPos;
    u16 yPos;
    s32 spriteIdx;
    u8 charIndex;
    u8 numPlayers;
    s32 pad[2];

    gameState = (GameState *)getCurrentAllocation();
    spriteAsset = loadCompressedData(&_4237C0_ROM_START, &_4237C0_ROM_END, 0x8A08);
    setCleanupCallback(func_80026BAC_277AC);

    numPlayers = D_800AFE8C_A71FC->numPlayers;
    xPos = D_8008DE54_8EA54[numPlayers].unk0;
    yPos = D_8008DE54_8EA54[numPlayers].unk2;

    for (i = 0; i < D_800AFE8C_A71FC->numPlayers; i++) {
        numPlayers = gameState->unk18A8[i];
        if (numPlayers == 3) {
            numPlayers = D_800AFE8C_A71FC->numPlayers;
            if (numPlayers == 1) {
                xPos += 0x18;
                spriteIdx = D_800AFE8C_A71FC->unk9[i + 4] + 0x30;
            } else {
                charIndex = D_800AFE8C_A71FC->unk9[i + 4];
                spriteIdx = charIndex + 0x23;
                xPos = *((u16 *)&D_8008DE9C_8EA9C + numPlayers * 2) - *(u16 *)(D_8008DE64_8EA64 + charIndex * 2 + 22);
            }
            sprites[i].spriteIndex = spriteIdx;
        } else {
            spriteIdx = 0x1D;
            if (D_800AFE8C_A71FC->numPlayers == 1) {
                spriteIdx = 0x36;
            }
            sprites[i].spriteIndex = numPlayers + spriteIdx;
        }
        sprites[i].blinkState = 0;
        sprites[i].unkC = 0;
        sprites[i].x = xPos;
        sprites[i].y = yPos;
        sprites[i].alpha = 0xFF;
        sprites[i].asset = spriteAsset;
    }

    setCallback(func_800269C8_275C8);
}

INCLUDE_ASM("asm/nonmatchings/24A30", func_800269C8_275C8);

void func_80026BAC_277AC(SimpleSpriteEntry *arg0) {
    arg0->asset = freeNodeMemory(arg0->asset);
}

void initCharSelectNameSprites(CharSelectNameSpritesState *state) {
    u8 *gameState;
    void *spriteAsset;
    D_800AFE8C_A71FC_type *gameConfig;
    u8 numPlayers;
    s32 i;
    u16 xPos;
    u16 yPos;
    s32 spriteIndex;
    s32 const_1;
    s32 const_ff;
    volatile SelectionEntry *entry;
    s32 pad[4];

    (void)pad;

    gameState = (u8 *)getCurrentAllocation();
    spriteAsset = loadCompressedData(&_4237C0_ROM_START, &_4237C0_ROM_END, 0x8A08);
    setCleanupCallback(cleanupCharSelectNameSprites);

    gameConfig = D_800AFE8C_A71FC;
    numPlayers = gameConfig->numPlayers;
    xPos = D_8008DE9C_8EA9C[numPlayers].x;
    yPos = D_8008DE9C_8EA9C[numPlayers].y;

    i = 0;
    if (numPlayers != 0) {
        const_1 = 1;
        const_ff = 0xFF;
        entry = (volatile SelectionEntry *)state;
        do {
            if (gameConfig->unk9[i + 4] >= 9) {
                spriteIndex = 0x35;
            } else {
                spriteIndex = 0x24;
                if (gameConfig->numPlayers == const_1) {
                    spriteIndex = 0x43;
                }
                spriteIndex = spriteIndex + (gameState + i)[0x18B0];
            }
            entry->blinkState = 0;
            entry->unkC = 0;
            gameConfig = D_800AFE8C_A71FC;
            entry->x = xPos;
            entry->y = yPos;
            entry->alpha = const_ff;
            entry->spriteIndex = spriteIndex;
            entry->asset = spriteAsset;
            i++;
            entry++;
        } while (i < gameConfig->numPlayers);
    }

    if (D_800AFE8C_A71FC->numPlayers == 1) {
        state->singlePlayerX = 0x38;
        state->singlePlayerY = -0x58;
        state->singlePlayerAsset = spriteAsset;
        state->singlePlayerSpriteIndex = 0x42;
        if (D_800AFE8C_A71FC->unk9[4] >= 9) {
            state->singlePlayerX = 0x50;
        }
    }

    setCallback(updateCharSelectNameSprites);
}

INCLUDE_ASM("asm/nonmatchings/24A30", updateCharSelectNameSprites);

void cleanupCharSelectNameSprites(SimpleSpriteEntry *arg0) {
    arg0->asset = freeNodeMemory(arg0->asset);
}

void initCharSelectPlayerNumbers(SimpleSpriteEntry *arg0) {
    void *dmaResult;
    D_800AFE8C_A71FC_type *global;
    u8 count;
    s32 i;
    s32 spriteOffset;
    u16 x;
    u16 y;
    volatile SimpleSpriteEntry *ptr;
    s32 pad[4];

    dmaResult = loadCompressedData(&_41A1D0_ROM_START, &_41A1D0_ROM_END, 0x1B48);
    setCleanupCallback(cleanupCharSelectPlayerNumbers);

    global = D_800AFE8C_A71FC;
    count = global->numPlayers;
    spriteOffset = (count >= 3) * 8;
    x = playerNumberPositions[count].x;
    y = playerNumberPositions[count].y;

    i = 0;
    if (count != 0) {
        ptr = arg0;
        do {
            ptr->x = x;
            ptr->y = y;
            ptr->spriteIndex = spriteOffset + i;
            ptr->asset = dmaResult;
            ptr++;
            i++;
        } while (i < global->numPlayers);
    }

    setCallback(updateCharSelectPlayerNumbers);
}

void updateCharSelectPlayerNumbers(u8 *arg0) {
    s32 i;
    s32 pad[2];
    u8 *ptr;
    s32 count;

    count = D_800AFE8C_A71FC->numPlayers;
    if (count > 0) {
        i = 0;
        ptr = arg0;
        do {
            debugEnqueueCallback((u16)(i + 0xC), 0, renderSpriteFrame, ptr);
            i++;
            ptr += 0xC;
        } while (i < D_800AFE8C_A71FC->numPlayers);
    }
}

void cleanupCharSelectPlayerNumbers(SimpleSpriteEntry *arg0) {
    arg0->asset = freeNodeMemory(arg0->asset);
}

void initCharSelectPlayer1NameSprite(SimpleSpriteEntry *arg0) {
    GameState *state;
    void *dmaResult;

    state = getCurrentAllocation();
    dmaResult = loadCompressedData(&_4237C0_ROM_START, &_4237C0_ROM_END, 0x8A08);
    setCleanupCallback(cleanupCharSelectPlayer1NameSprite);

    arg0->x = -0x20;
    arg0->y = 8;
    arg0->spriteIndex = state->unk18A8[0] + 0x16;
    arg0->asset = dmaResult;

    setCallback(updateCharSelectPlayer1NameSprite);
}

void updateCharSelectPlayer1NameSprite(SimpleSpriteEntry *arg0) {
    GameState *state = getCurrentAllocation();

    if (state->unk1898[0] == 3) {
        arg0->spriteIndex = state->unk18A8[0] + 0x16;
        debugEnqueueCallback(0xC, 0, renderSpriteFrame, arg0);
    }
}

void cleanupCharSelectPlayer1NameSprite(SimpleSpriteEntry *arg0) {
    arg0->asset = freeNodeMemory(arg0->asset);
}

void initCharSelectPlayer2NameSprites(SimpleSpriteEntry *arg0) {
    void *dmaResult;
    s32 loopCount;
    s32 i;

    dmaResult = loadCompressedData(&_4237C0_ROM_START, &_4237C0_ROM_END, 0x8A08);

    loopCount = (D_800AFE8C_A71FC->numPlayers == 2) ? 3 : 2;

    for (i = 0; i < loopCount; i++) {
        arg0[i].asset = dmaResult;
    }

    setCleanupCallback(cleanupCharSelectPlayer2NameSprites);
    setCallback(waitForCharSelectP2NameReveal);
}

void waitForCharSelectP2NameReveal(P2NameRevealState *arg0) {
    GameState *state = (GameState *)getCurrentAllocation();

    if (state->unk1898[arg0->playerIndex] == 0x1A) {
        setCallbackWithContinue(setupCharSelectP2NamePositions);
    }
}

void setupCharSelectP2NamePositions(volatile P2NameSpriteEntry *arg0) {
    s32 xBase;
    s32 yBase;
    s32 xIncrement;
    s32 yIncrement;
    s32 spriteIndexBase;
    s32 count;
    s32 i;
    s32 countMasked;
    s32 xInc2;
    s32 yInc2;
    s32 priority;
    s32 x;
    s32 y;
    s32 *dummyPtr;
    s32 dummy;

    dummyPtr = &dummy;

    xBase = -0x38;
    if (D_800AFE8C_A71FC->numPlayers == 2) {
        xBase = -0x58;
        yBase = -0x64;
        xIncrement = 0;
        yIncrement = 0x10;
        spriteIndexBase = 0x27;
        count = 3;
    } else {
        yBase = -0x5C;
        xIncrement = 0x38;
        yIncrement = 0;
        spriteIndexBase = 0x2A;
        count = 2;
    }

    countMasked = count & 0xFF;
    i = 0;
    if (countMasked > 0) {
        xInc2 = xIncrement;
        yInc2 = (s16)yIncrement;
        priority = 0xC;
        y = yBase;
        x = xBase;
        do {
            arg0->y = (s16)y;
            y = y + yInc2;
            arg0->x = (s16)x;
            arg0->spriteIndex = (s16)(spriteIndexBase + i);
            arg0->paletteIndex = (u8)priority;
            arg0++;
            i++;
            x = x + xInc2;
        } while (i < countMasked);
    }

    setCallback(animateCharSelectP2NameReveal);
}

void animateCharSelectP2NameReveal(P2NameAnimationState *arg0) {
    s16 minY;
    s32 yIncrement;
    s32 loopCount;
    s32 i;
    volatile P2NameSpriteEntry *ptr;
    GameState *state;
    int new_var;
    u16 val;
    s32 pad[2];
    (void)pad;
    state = (GameState *)getCurrentAllocation();
    minY = -0x10;
    if (D_800AFE8C_A71FC->numPlayers == 2) {
        minY = -0x14;
        yIncrement = 0x14;
        loopCount = 3;
    } else {
        yIncrement = 0x13;
        loopCount = 2;
    }
    yIncrement &= -(state->unk1898[arg0->playerIndex] == 0x1A);
    i = 0;
    if (((s32)(loopCount & 0xFF)) > 0) {
        new_var = loopCount & 0xFF;
        ptr = (volatile P2NameSpriteEntry *)arg0;
        do {
            i += 1;
            ptr->y = ptr->y + yIncrement;
            debugEnqueueCallback(arg0->playerIndex + 0xC, 0, renderSpriteFrameWithPalette, (void *)ptr);
            ptr++;
        } while (i < ((s32)new_var));
    }
    val = state->unk1898[arg0->playerIndex];
    if (val == 0x1A) {
        if (arg0->entries[0].y == minY) {
            state->unk1898[arg0->playerIndex] = 0x1B;
        }
    } else if (val == 0x1E) {
        setCallback(animateCharSelectP2NameHide);
    }
}

void animateCharSelectP2NameHide(P2NameHideState *arg0) {
    u8 *allocation;
    s16 target;
    unsigned char var_v0;
    s32 count;
    s32 i;
    s16 increment;
    allocation = (u8 *)getCurrentAllocation();
    target = -0x5C;
    if (D_800AFE8C_A71FC->numPlayers == 2) {
        target = -0x64;
        var_v0 = 3;
        increment = -0x14;
    } else {
        var_v0 = 2;
        increment = -0x13;
    }
    for (i = 0; i < (var_v0 & 0xFF); i++) {
        arg0->entries[i].y += increment;
        debugEnqueueCallback(arg0->playerIndex + 0xC, 0, renderSpriteFrameWithPalette, (void *)(&arg0->entries[i]));
    }

    if (arg0->entries[0].y == target) {
        *((u16 *)((allocation + (arg0->playerIndex * 2)) + 0x1898)) = 0;
        setCallback(waitForCharSelectP2NameReveal);
    }
}

void cleanupCharSelectPlayer2NameSprites(SimpleSpriteEntry *arg0) {
    arg0->asset = freeNodeMemory(arg0->asset);
}

INCLUDE_ASM("asm/nonmatchings/24A30", initCharSelectStats);

void updateCharSelectStats(CharSelectStatsState *arg0) {
    GameState *gameState;
    s32 i;
    s32 j;
    s32 tableOffset;
    u8 charIndex;
    u8 paletteIndex;
    u8 playerIndex;
    u8 charByte;
    s32 tableBase;
    s32 charMul;

    gameState = getCurrentAllocation();
    playerIndex = arg0->playerIndex;
    charIndex = gameState->unk18A8[playerIndex];
    paletteIndex = gameState->unk18B0[playerIndex];
    charMul = charIndex * 3;
    tableBase = paletteIndex + charMul;

    if ((u32)(gameState->unk1898[playerIndex] - 3) >= 2U) {
        i = 0;
        tableOffset = tableBase * 3;

        for (; i < 3; i++) {
            sprintf(arg0->charBufs[i], D_8009E288_9EE88, D_8008DD8C_8E98C[tableOffset + i]);
            if (D_800AFE8C_A71FC->numPlayers == 1) {
                for (j = 0; j < 2; j++) {
                    charByte = arg0->charBufs[i][j];
                    if (charByte != 0x20) {
                        arg0->spriteEntries[i * 2 + j].frameIndex = charByte - 0x30;
                        debugEnqueueCallback(
                            arg0->playerIndex + 8,
                            7,
                            renderSpriteFrame,
                            &arg0->spriteEntries[i * 2 + j]
                        );
                    }
                }
            } else {
                arg0->textEntries[i].string = (u8 *)arg0->charBufs[i];
                debugEnqueueCallback(arg0->playerIndex + 8, 7, renderTextPalette, &arg0->textEntries[i]);
            }
        }
    }
}

void cleanupCharSelectStats(SimpleSpriteEntry *arg0) {
    arg0->asset = freeNodeMemory(arg0->asset);
}

void initCharSelectBackgroundEffect(CharSelectBackgroundEffectState *state) {
    state->effectAsset = loadCompressedData(&_458E30_ROM_START, &_458E30_ROM_END, 0xAE0);
    setCleanupCallback(cleanupCharSelectBackgroundEffect);
    setCallback(setupCharSelectBackgroundEffect);
}

void setupCharSelectBackgroundEffect(CharSelectBackgroundEffectState *state) {
    initTiledTextureRenderState(state, (s32)state->effectAsset);
    setCallback(updateCharSelectBackgroundEffect);
}

void updateCharSelectBackgroundEffect(CharSelectBackgroundEffectState *state) {
    state->rotationX++;
    state->rotationY++;
    state->rotationX &= 0x3FF;
    state->rotationY &= 0x3FF;
    debugEnqueueCallback(7, 0, renderTiledTexture, state);
}

void cleanupCharSelectBackgroundEffect(CharSelectBackgroundEffectState *state) {
    state->effectAsset = freeNodeMemory(state->effectAsset);
}

void initCharSelectScaledSprite(ScaledSpriteEntry *arg0) {
    void *dmaResult;

    dmaResult = loadCompressedData(&_4237C0_ROM_START, &_4237C0_ROM_END, 0x8A08);
    setCleanupCallback(cleanupCharSelectScaledSprite);

    arg0->spriteIndex = 6;
    arg0->scaleX = 0x300;
    arg0->scaleY = 0x300;
    arg0->alpha = 0xFF;
    arg0->x = 0;
    arg0->y = 0;
    arg0->rotation = 0;
    arg0->unk13 = 0;
    arg0->unk12 = 0;
    arg0->unk14 = 0x80;
    arg0->asset = dmaResult;

    setCallback(renderCharSelectScaledSprite);
}

void renderCharSelectScaledSprite(void *arg0) {
    debugEnqueueCallback(0xC, 7, func_80011924_12524, arg0);
}

void cleanupCharSelectScaledSprite(ScaledSpriteEntry *arg0) {
    arg0->asset = freeNodeMemory(arg0->asset);
}

void updateCharSelectPreviewLighting(CharSelectPreviewModel *arg0, u8 arg1) {
    u8 *allocation;
    u8 byte;
    u8 val;

    allocation = getCurrentAllocation();
    allocation += arg1;
    byte = allocation[0x18C8];

    if (byte == 1) {
        val = 0x50;
        arg0->lightR = val;
        arg0->lightG = val;
        arg0->lightB = val;
    } else if (byte == 0) {
        val = 0xFF;
        arg0->lightR = val;
        arg0->lightG = val;
        arg0->lightB = val;
        val = 0x80;
    } else {
        val = 0xFF;
        arg0->lightR = val;
        arg0->lightG = val;
        arg0->lightB = val;
    }

    arg0->ambientR = val;
    arg0->ambientG = val;
    arg0->ambientB = val;
}

u8 getItemStat1(u8 itemIndex) {
    return D_8008DD8C_8E98C[itemIndex * 3];
}

u8 getItemStat2(u8 itemIndex) {
    return D_8008DD8C_8E98C[itemIndex * 3 + 1];
}

u8 getItemStat3(u8 itemIndex) {
    return D_8008DD8C_8E98C[itemIndex * 3 + 2];
}
