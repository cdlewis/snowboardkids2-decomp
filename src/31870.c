#include "10AD0.h"
#include "20F0.h"
#include "288A0.h"
#include "2F990.h"
#include "33FE0.h"
#include "36B80.h"
#include "38C90.h"
#include "3E160.h"
#include "5E590.h"
#include "68CF0.h"
#include "6E840.h"
#include "D_800AFE8C_A71FC_type.h"
#include "EepromSaveData_type.h"
#include "common.h"
#include "displaylist.h"
#include "gamestate.h"
#include "geometry.h"
#include "overlay.h"
#include "rand.h"
#include "task_scheduler.h"

USE_ASSET(_419C60);
USE_ASSET(_41A1D0);
USE_ASSET(_4237C0);
USE_ASSET(_4547D0);
USE_ASSET(_3F6950);
USE_ASSET(_3F6670);
USE_ASSET(_4488E0);
USE_ASSET(_4237C0);

extern u16 D_8008F150_8FD50[];
extern const char D_8009E47C_9F07C[];
extern const char D_8009E480_9F080;

typedef struct {
    s16 unk0;
    s16 x;
    u8 padding[0x4];
    s16 spriteIndex;
    u8 padding2[0xE];
} BoardShopCharacterPortrait;

typedef struct {
    BoardShopCharacterPortrait portraits[4];
    u8 padding[0x24];
    u8 animationFrameCounters[4];
} BoardShopCharacterPortraitState;

typedef struct {
    u8 padding[0x77C];
    u16 frameCounter;
    u8 padding2[0x6];
    u8 unk784[4];
    u8 unk788[17];
    u8 selectedIconIndex;
    u8 unk79A;
    u8 shopState;
    u8 unk79C;
    s8 unk79D;
} BoardShopSelectionAllocation;

typedef struct {
    u8 padding[0x8];
    s16 spriteIndex;
    u8 padding2[0x2];
    s16 scaleX;
    s16 scaleY;
    s16 alpha;
    s8 unk12;
    s8 unk13;
    u8 flipX;
    u8 padding3[0x3];
} BoardShopIconDisplayState;

typedef struct {
    BoardShopIconDisplayState icons[4];
    u8 padding2[0x18];
    s16 priceTextX;
    s16 priceTextY;
    u8 padding3[0x8];
    s8 animationCounters[4];
    char priceTextBuffer;
} BoardShopIconSelectionState;

typedef struct {
    Node_70B00 *unk0;
    s32 unk4;
    u8 padding[0x18];
    void *unk20;
    void *unk24;
    void *unk28;
    void *unk2C;
    void *unk30;
    u8 padding2[0x8];
    s32 unk3C;
    u8 unk40[0x10];
    s32 unk50;
    s32 unk54;
    s32 unk58;
    s32 unk5C;
    s16 unk60;
    s16 unk62;
    s8 unk64;
} BoardShopCharacterPreviewState;

typedef struct {
    DisplayListObject unk0;
    s32 unk3C;
    u8 unk40[0x10];
    s32 unk50;
    s32 unk54;
    s32 unk58;
    s32 unk5C;
    s16 unk60;
    s16 unk62;
    s8 unk64;
} func_80031944_32544_arg;

typedef struct {
    u8 padding[0x7A4];
    u8 unk7A4;
} func_8003316C_33D6C_alloc;

typedef struct {
    SceneModel *model;
    Transform3D transform;
    union {
        SceneModel *unk20;
        s32 unk20_s32;
        s16 animationFrame;
    } animFrameUnion;
} BoardShopShopkeeperWaitState;

typedef struct {
    s16 unk0;
    u16 unk2;
    void *unk4;
    s16 unk8;
    s16 unkA;
    s8 unkC;
    s8 unkD;
    s16 unkE;
} func_800330B4_33CB4_arg;

typedef struct {
    SceneModel *model;
    Transform3D transform;
    s16 animationFrame;
    u16 animationEndFrame;
    u8 animationState;
} BoardShopShopkeeperState;

typedef struct {
    u8 padding[0x8];
    void *textAsset;
} BoardShopTitleTextCleanupArg;

typedef struct {
    s16 textWidth;
    s32 textData;
} BoardShopTitleTextUpdateArg;

typedef struct {
    s16 textWidth;
    s16 y;
    void *textData;
    void *textAsset;
    s16 primaryColor;
    s16 secondaryColor;
    s8 textStyle;
} BoardShopTitleTextState;

typedef struct {
    s16 x;
    s16 y;
    u8 padding[0x80];
    u8 frameCounter;
} BoardShopSnowflakeAnimState;

typedef struct {
    char padding;
    void *unk4;
    char padding2[78];
    void *unk58;
} BoardShopGoldDisplayCleanupArg;

typedef struct {
    u8 padding[0x79C];
    u8 unk79C;
} BoardShopSnowflakeAnimAllocation;

typedef struct {
    s16 x;
    s16 y;
    void *asset;
    s16 spriteIndex;
    s16 scaleX;
    s16 scaleY;
    s16 rotation;
    s16 alpha;
    s8 unk12;
    s8 unk13;
    s8 flipX;
    u8 padding[0x6F];
    s8 frameCounter;
} BoardShopSnowflakeSpriteState;

typedef struct {
    u8 padding[0x77C];
    s16 unk77C;
    u8 padding2[0x2];
    s16 unk780;
    s16 unk782;
    u8 unk784[4];
    u8 unk788[20];
    u8 unk79C;
    u8 unk79D;
    u8 unk79E;
    u8 unk79F;
    u8 unk7A0;
    s8 unk7A1;
    u8 unk7A2;
} BoardShopScreenAllocation;

typedef struct {
    s16 x;
    s16 y;
    void *asset;
    s16 spriteIndex;
    s16 scaleX;
    s16 scaleY;
    s16 rotation;
    s16 alpha;
    s8 unk12;
    s8 unk13;
    s8 flipX;
    u8 padding[0x3];
} BoardShopBoardIconState;

typedef struct {
    BoardShopBoardIconState icons[4];
    u8 padding2[0x18];
    s16 priceTextX;
    s16 priceTextY;
    s16 priceTextStyle;
    u8 padding3[0x2];
    void *priceTextPtr;
    s8 animationCounters[4];
    void *priceTextBuffer;
} BoardShopBoardIconsState;

typedef struct {
    u8 padding[0x24];
    void *unk24;
    void *unk28;
    void *unk2C;
} func_800319C8_325C8_arg;

typedef struct {
    s16 x;
    s16 y;
    u8 padding[0x14];
} BoardShopSlideOutIconState;

typedef struct {
    BoardShopSlideOutIconState icons[4];
    u8 padding2[0x24];
    u8 slidingIconCount;
} BoardShopBoardIconsSlideOutState;

typedef struct {
    u8 padding[0x77C];
    s16 animationComplete;
} BoardShopSlideOutAllocation;

typedef struct {
    u8 padding[0x2C];
    void *backgroundAsset;
} BoardShopBackgroundState;

typedef struct {
    u8 padding[0x77C];
    s16 unk77C;
} func_80032628_alloc;

typedef struct {
    s16 x;
    s16 y;
    void *sprite;
    s16 speed;
    s16 unkA;
} SnowParticle;

typedef struct {
    SnowParticle *particles;
    u8 delayTimer;
} SnowParticleState;

void animateBoardShopSnowParticles(void);
void blinkBoardShopBoardIconConfirmation(BoardShopIconSelectionState *arg0);
void freeBoardShopCharacterPreviewAssets(BoardShopCharacterPreviewState *arg0);
void waitBoardShopCharacterPreview(void);
void animateBoardShopCharacterSlideIn(BoardShopCharacterPreviewState *arg0);
void updateBoardShopCharacterPreview(BoardShopCharacterPreviewState *arg0);
void loadBoardShopCharacterAssets(BoardShopCharacterPreviewState *arg0);
void animateBoardShopCharacterSwitch(BoardShopCharacterPreviewState *arg0);
void freeBoardShopPurchaseAssets(func_800319C8_325C8_arg *arg0);
void animateBoardShopCharacterSlideOut(BoardShopCharacterPreviewState *arg0);
void loadBoardShopPurchaseAssets(BoardShopCharacterPreviewState *arg0);
void cleanupBoardShopShopkeeper(BoardShopShopkeeperState *arg0);
void waitBoardShopShopkeeper(BoardShopShopkeeperWaitState *arg0);
void updateBoardShopShopkeeper(BoardShopShopkeeperState *arg0);
void initBoardShopBackgroundRenderState(BoardShopBackgroundState *arg0);
void cleanupBoardShopBackground(BoardShopBackgroundState *arg0);
void cleanupBoardShopColumnSelectorArrow(void *);
void updateBoardShopExitOverlay(void *arg0);
void cleanupBoardShopExitOverlay(SpriteDisplayState *arg0);
void func_8003316C_33D6C(func_800330B4_33CB4_arg *arg0);
void func_800331CC_33DCC(func_800330B4_33CB4_arg *arg0);
void updateBoardShopTitleText(BoardShopTitleTextUpdateArg *arg0);
void cleanupBoardShopTitleText(BoardShopTitleTextCleanupArg *arg0);
void animateBoardShopSnowflakeSlideIn(BoardShopSnowflakeAnimState *arg0);
void queueBoardShopSnowflakeRender(void *);
void cleanupBoardShopSnowflakeSprite(SpriteDisplayState *);
void cleanupBoardShopBoardIcons(BoardShopBoardIconsState *arg0);
void freeBoardShopCharacterTransitionAssets(func_800319C8_325C8_arg *arg0);
void animateBoardShopCharacterTransition(func_80031944_32544_arg *arg0);
void enqueueBoardShopBackgroundRender(void *arg0);
void cleanupBoardShopSnowParticles(SnowParticleState *arg0);
void waitBoardShopSnowParticles(SnowParticleState *arg0);
void animateBoardShopCharacterPortraitsSlideIn(BoardShopCharacterPortraitState *arg0);
void animateBoardShopBoardIconsSlideIn(BoardShopBoardIconsState *arg0);
void updateBoardShopGoldDisplay(BoardShopGoldDisplayState *arg0);
void initBoardShopCharacterPortraitsSlideIn(BoardShopCharacterPortraitState *arg0);
void animateBoardShopBoardIconsSlideOut(BoardShopBoardIconsSlideOutState *arg0);
void cleanupBoardShopGoldDisplay(BoardShopGoldDisplayCleanupArg *arg0);
void cleanupBoardShopPreviewWipe(BoardShopCharacterPreviewState *arg0);
void animateBoardShopPreviewWipe(BoardShopCharacterPreviewState *arg0);
void waitBoardShopPreviewWipe(BoardShopCharacterPreviewState *arg0);
void updateBoardShopBoardIconSelection(BoardShopIconSelectionState *arg0);

extern u16 D_8008F16C_8FD6C[];
extern u16 D_8008F16E_8FD6E[];
extern s32 D_8008F18C_8FD8C;
extern s32 D_8008F200_8FE00[];
extern u16 D_8008F20A_8FE0A[];
extern u16 D_8008F20C_8FE0C;
extern s32 identityMatrix;
extern s16 boardIconTargetYPositions[];
extern u16 D_8008F184_8FD84[];

void initBoardShopPreviewWipe(BoardShopCharacterPreviewState *arg0) {
    s32 perspectiveParams[8];
    Transform3D rotationYX;
    Transform3D rotationZ;

    u8 charIndex;
    s32 unused;
    u8 paletteId;
    void *transformMatrix;
    BoardShopScreenAllocation *state;
    Node_70B00 *cameraNode;
    Transform3D *rotationZPtr;

    rotationZPtr = &rotationZ;

    state = getCurrentAllocation();
    cameraNode = allocateNodeMemory(0x1D8);
    arg0->unk0 = cameraNode;
    initMenuCameraNode(cameraNode, 1, 0xB, 0);
    arg0->unk60 = -0x34;
    arg0->unk62 = 0x34;
    state->unk780 = (u16)arg0->unk60;
    state->unk782 = (u16)arg0->unk62;

    setModelCameraTransform(arg0->unk0, 0, 0, -0x98, arg0->unk60, 0x97, arg0->unk62);
    func_8006BEDC_6CADC(perspectiveParams, 0, 0, 0x580000, 0, 0, 0);
    func_8006FD3C_7093C(arg0->unk0->id, perspectiveParams);

    transformMatrix = &arg0->unk40;
    memcpy(transformMatrix, &identityMatrix, 0x20);
    memcpy(rotationZPtr, transformMatrix, 0x20);
    memcpy(&rotationYX, rotationZPtr, 0x20);

    createRotationMatrixYX(&rotationYX, 0x1000, 0x800);
    createZRotationMatrix(rotationZPtr, 0x1F00);

    func_8006B084_6BC84(&rotationYX, rotationZPtr, transformMatrix);

    arg0->unk5C = 0xFFF80000;
    charIndex = state->unk7A2 + (state->unk7A1 * 3);
    paletteId = EepromSaveData->character_or_settings[charIndex & 0xFF];

    memcpy(&arg0->unk4, transformMatrix, 0x20);

    charIndex = charIndex & 0xFF;
    arg0->unk24 = loadAssetByIndex_95728(charIndex);
    arg0->unk28 = loadAssetByIndex_95500(charIndex);
    arg0->unk2C = loadAssetByIndex_95590(charIndex);
    arg0->unk30 = loadAssetByIndex_95668(paletteId - 1);

    setCleanupCallback(&cleanupBoardShopPreviewWipe);
    arg0->unk64 = 0xC;
    setCallback(&waitBoardShopPreviewWipe);
}

void waitBoardShopPreviewWipe(BoardShopCharacterPreviewState *arg0) {
    u8 delayTimer;
    arg0->unk64--;
    delayTimer = arg0->unk64;
    if (delayTimer == 0) {
        setCallback(&animateBoardShopPreviewWipe);
    }
    func_8006FED8_70AD8(arg0->unk0);
    enqueueDisplayListObject(1, (DisplayListObject *)&arg0->unk4);
}

void animateBoardShopPreviewWipe(BoardShopCharacterPreviewState *arg0) {
    BoardShopScreenAllocation *state = getCurrentAllocation();

    arg0->unk60++;
    arg0->unk62--;
    state->unk780 = arg0->unk60;
    state->unk782 = arg0->unk62;

    setModelCameraTransform(arg0->unk0, 0, 0, -0x98, arg0->unk60, 0x97, arg0->unk62);

    if (arg0->unk60 == 0) {
        state->unk77C = 1;
        unlinkNode(arg0->unk0);
        func_80069CF8_6A8F8();
    } else {
        func_8006FED8_70AD8(arg0->unk0);
        enqueueDisplayListObject(1, (DisplayListObject *)&arg0->unk4);
    }
}

void cleanupBoardShopPreviewWipe(BoardShopCharacterPreviewState *arg0) {
    arg0->unk0 = (Node_70B00 *)freeNodeMemory(arg0->unk0);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
    arg0->unk2C = freeNodeMemory(arg0->unk2C);
    arg0->unk30 = freeNodeMemory(arg0->unk30);
}

void initBoardShopSnowParticles(SnowParticleState *arg0) {
    BoardShopScreenAllocation *state;
    void *snowflakeSprite;
    s32 i;
    int new_var;
    s32 j;
    s32 randOffset;

    state = getCurrentAllocation();
    snowflakeSprite = loadCompressedData(&_4547D0_ROM_START, &_4547D0_ROM_END, 0x9488);
    arg0->particles = allocateNodeMemory(0xF0);
    setCleanupCallback(cleanupBoardShopSnowParticles);

    i = j = 0;
    while (i < 0x14) {
        if (i < 10) {
            arg0->particles[j].x = -0x24 + (i * 6);
            randOffset = (randB() & 7) - 10;
            arg0->particles[j].y = state->unk780 + randOffset;
        } else {
            arg0->particles[j].x = -0x60 + (i * 6);
            arg0->particles[j].y = state->unk782 + (new_var = (randB() & 7) - 10);
        }

        arg0->particles[j].sprite = snowflakeSprite;
        arg0->particles[j].speed = (randB() & 7) | 0x10;

        i++;
        j++;
    }

    arg0->delayTimer = 0xC;
    setCallback(waitBoardShopSnowParticles);
}

void waitBoardShopSnowParticles(SnowParticleState *arg0) {
    arg0->delayTimer--;
    if (arg0->delayTimer == 0) {
        setCallback(animateBoardShopSnowParticles);
    }
}

INCLUDE_ASM("asm/nonmatchings/31870", animateBoardShopSnowParticles);

void cleanupBoardShopSnowParticles(SnowParticleState *arg0) {
    ((Node_70B00 *)arg0->particles)->prev = freeNodeMemory(((Node_70B00 *)arg0->particles)->prev);
    arg0->particles = freeNodeMemory(arg0->particles);
}

void initBoardShopCharacterPreview(BoardShopCharacterPreviewState *arg0) {
    Transform3D rotationYX;
    Transform3D *pRotationZ;
    Transform3D *pRotationYX;
    Transform3D rotationZ;
    u8 paletteIndex;
    void *transformMatrix;

    pRotationZ = &rotationZ;
    pRotationYX = &rotationYX;

    getCurrentAllocation();
    transformMatrix = &arg0->unk3C;
    memcpy(transformMatrix, &identityMatrix, 0x20);
    memcpy(pRotationZ, transformMatrix, 0x20);
    memcpy(pRotationYX, pRotationZ, 0x20);
    createRotationMatrixYX(pRotationYX, 0x1000, 0x800);
    createZRotationMatrix(pRotationZ, 0x1F00);
    func_8006B084_6BC84(pRotationYX, pRotationZ, transformMatrix);
    arg0->unk50 = 0x600000;
    arg0->unk58 = 0xFFF80000;
    paletteIndex = EepromSaveData->character_or_settings[0];
    memcpy(arg0, transformMatrix, 0x20);
    arg0->unk20 = loadAssetByIndex_95728(0);
    arg0->unk24 = loadAssetByIndex_95500(0);
    arg0->unk28 = loadAssetByIndex_95590(0);
    arg0->unk2C = loadAssetByIndex_95668(paletteIndex - 1);
    setCleanupCallback(&freeBoardShopCharacterPreviewAssets);
    setCallback(&waitBoardShopCharacterPreview);
}

void waitBoardShopCharacterPreview(void) {
    BoardShopScreenAllocation *allocation;

    allocation = getCurrentAllocation();
    if (allocation->unk788[19] == 1) {
        setCallbackWithContinue(animateBoardShopCharacterSlideIn);
    }
}

void animateBoardShopCharacterSlideIn(BoardShopCharacterPreviewState *arg0) {
    BoardShopScreenAllocation *allocation;

    allocation = getCurrentAllocation();

    arg0->unk50 += 0xFFF00000;

    memcpy(arg0, &arg0->unk3C, 0x20);

    if (arg0->unk50 == 0) {
        allocation->unk788[19] = 0xC;
        setCallbackWithContinue(&updateBoardShopCharacterPreview);
    }

    enqueueDisplayListObject(0, (DisplayListObject *)arg0);
}

void updateBoardShopCharacterPreview(BoardShopCharacterPreviewState *arg0) {
    BoardShopScreenAllocation *allocation;
    u8 state;

    allocation = getCurrentAllocation();
    state = allocation->unk788[19];

    if (state == 3 || state == 0x1A) {
        if (state == 3) {
            arg0->unk24 = freeNodeMemory(arg0->unk24);
            arg0->unk28 = freeNodeMemory(arg0->unk28);
            arg0->unk2C = freeNodeMemory(arg0->unk2C);
            setCallback(&loadBoardShopCharacterAssets);
        } else {
            setCallback(&freeBoardShopPurchaseAssets);
            enqueueDisplayListObject(0, (DisplayListObject *)arg0);
        }
    } else if (state == 0x32) {
        setCallback(&animateBoardShopCharacterSlideOut);
        enqueueDisplayListObject(0, (DisplayListObject *)arg0);
    } else {
        enqueueDisplayListObject(0, (DisplayListObject *)arg0);
    }
}

void loadBoardShopCharacterAssets(BoardShopCharacterPreviewState *arg0) {
    u8 paletteIndex;
    u8 temp_v1;
    BoardShopScreenAllocation *allocation;

    allocation = getCurrentAllocation();

    if (allocation->unk79C == 0) {
        arg0->unk50 = 0xFFA00000;
    } else {
        arg0->unk50 = 0x600000;
    }

    paletteIndex = EepromSaveData->character_or_settings[allocation->unk79E];

    memcpy(arg0, &arg0->unk3C, 0x20);

    arg0->unk20 = loadAssetByIndex_95728(allocation->unk79E);
    arg0->unk24 = loadAssetByIndex_95500(allocation->unk79E);
    arg0->unk28 = loadAssetByIndex_95590(allocation->unk79E);
    arg0->unk2C = loadAssetByIndex_95668(paletteIndex - 1);

    setCallback(&animateBoardShopCharacterSwitch);
}

void animateBoardShopCharacterSwitch(BoardShopCharacterPreviewState *arg0) {
    BoardShopScreenAllocation *allocation;
    s32 slideSpeed;

    allocation = (BoardShopScreenAllocation *)getCurrentAllocation();

    if (allocation->unk79C == 1) {
        slideSpeed = 0xFFF00000;
    } else {
        slideSpeed = 0x100000;
    }

    arg0->unk50 += slideSpeed;

    memcpy(arg0, &arg0->unk3C, 0x20);

    if (arg0->unk50 == 0) {
        allocation->unk79D--;
        setCallback(&updateBoardShopCharacterPreview);
    }

    enqueueDisplayListObject(0, (DisplayListObject *)arg0);
}

void freeBoardShopPurchaseAssets(func_800319C8_325C8_arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
    arg0->unk2C = freeNodeMemory(arg0->unk2C);
    setCallback(&loadBoardShopPurchaseAssets);
}

void loadBoardShopPurchaseAssets(BoardShopCharacterPreviewState *arg0) {
    BoardShopScreenAllocation *allocation;
    s16 assetIndex;
    int new_var;
    u8 characterIndex;

    allocation = getCurrentAllocation();
    new_var = allocation->unk784[allocation->unk788[17]];
    assetIndex = allocation->unk7A1;
    characterIndex = allocation->unk7A2;
    assetIndex = (characterIndex + (assetIndex * 3)) & 0xFF;
    characterIndex = allocation->unk788[new_var];

    memcpy(arg0, &arg0->unk3C, 0x20);

    arg0->unk20 = loadAssetByIndex_95728(assetIndex);
    arg0->unk24 = loadAssetByIndex_95500(assetIndex);
    arg0->unk28 = loadAssetByIndex_95590(assetIndex);
    arg0->unk2C = loadAssetByIndex_95668(characterIndex);

    setCallback(&updateBoardShopCharacterPreview);
}

void animateBoardShopCharacterSlideOut(BoardShopCharacterPreviewState *arg0) {
    BoardShopSelectionAllocation *allocation;

    allocation = getCurrentAllocation();
    arg0->unk50 += 0x100000;
    memcpy(arg0, &arg0->unk3C, 0x20);

    if (arg0->unk50 == 0x600000) {
        allocation->unk79A = 1;
        func_80069CF8_6A8F8();
    } else {
        enqueueDisplayListObject(0, (DisplayListObject *)arg0);
    }
}

void freeBoardShopCharacterPreviewAssets(BoardShopCharacterPreviewState *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
    arg0->unk2C = freeNodeMemory(arg0->unk2C);
}

void initBoardShopCharacterTransition(BoardShopCharacterPreviewState *arg0) {
    BoardShopScreenAllocation *state;
    Transform3D rotationYX;
    Transform3D rotationZ;
    u8 characterIndex;
    u8 paletteIndex;
    Transform3D *pRotationYX;
    Transform3D *pRotationZ;

    state = getCurrentAllocation();

    memcpy(&arg0->unk3C, &identityMatrix, sizeof(Transform3D));

    pRotationZ = &rotationZ;
    memcpy(pRotationZ, &arg0->unk3C, sizeof(Transform3D));

    pRotationYX = &rotationYX;
    memcpy(pRotationYX, pRotationZ, sizeof(Transform3D));

    createRotationMatrixYX(pRotationYX, 0x1000, 0x800);
    createZRotationMatrix(pRotationZ, 0x1F00);
    func_8006B084_6BC84(pRotationYX, pRotationZ, &arg0->unk3C);

    arg0->unk58 = 0xFFF80000;
    memcpy(arg0, &arg0->unk3C, 0x20U);

    characterIndex = state->unk79F;
    paletteIndex = EepromSaveData->character_or_settings[characterIndex];

    arg0->unk20 = loadAssetByIndex_95728(characterIndex);
    arg0->unk24 = loadAssetByIndex_95500(characterIndex);
    arg0->unk28 = loadAssetByIndex_95590(characterIndex);
    arg0->unk2C = loadAssetByIndex_95668(paletteIndex - 1);

    setCleanupCallback(&freeBoardShopCharacterTransitionAssets);
    setCallbackWithContinue(&animateBoardShopCharacterTransition);
}

void animateBoardShopCharacterTransition(func_80031944_32544_arg *arg0) {
    s32 pad[8];
    GameState *state;
    s32 slideSpeed;
    s32 targetPosition;
    s32 newPosition;

    state = getCurrentAllocation();

    slideSpeed = 0x100000;
    targetPosition = 0x600000;

    if (state->unk79C == 1) {
        slideSpeed = 0xFFF00000;
        targetPosition = 0xFFA00000;
    }

    newPosition = arg0->unk50 + slideSpeed;
    arg0->unk50 = newPosition;
    memcpy(&arg0->unk0, &arg0->unk3C, 0x20);

    enqueueDisplayListObject(0, &arg0->unk0);

    if (arg0->unk50 == targetPosition) {
        func_80069CF8_6A8F8();
    }
}

void freeBoardShopCharacterTransitionAssets(func_800319C8_325C8_arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
    arg0->unk2C = freeNodeMemory(arg0->unk2C);
}

void initBoardShopShopkeeper(BoardShopShopkeeperState *arg0) {
    arg0->model = createSceneModelEx(0x3A, &((GameState *)getCurrentAllocation())->audioPlayer2, 0, -1, 0, 0x12);

    memcpy(&arg0->transform, &identityMatrix, sizeof(Transform3D));

    arg0->transform.translation.x = 0xFFE70000;
    arg0->transform.translation.y = 0xFFE00000;
    arg0->transform.translation.z = 0;

    createYRotationMatrix(&arg0->transform, 0x200);

    arg0->animationFrame = 0x10;
    arg0->animationEndFrame = 0x10;
    arg0->animationState = 0;

    setCleanupCallback(&cleanupBoardShopShopkeeper);
    setCallback(&waitBoardShopShopkeeper);
}

void waitBoardShopShopkeeper(BoardShopShopkeeperWaitState *arg0) {
    GameState *state;

    state = (GameState *)getCurrentAllocation();
    applyTransformToModel(arg0->model, &arg0->transform);
    setItemDisplayEnabled(arg0->model, 1);
    setModelAnimation(arg0->model, arg0->animFrameUnion.animationFrame);
    updateModelGeometry(arg0->model);
    if (state->unk79B != 0) {
        setCallback(&updateBoardShopShopkeeper);
    }
}

typedef struct {
    u8 padding[0x1D8];
    void *audioPlayer2;
    u8 padding2[0x5A2];
    u16 unk77E;
    u8 padding3[0x23];
    u8 unk7A3;
} BoardShopUpdateAllocation;

void updateBoardShopShopkeeper(BoardShopShopkeeperState *arg0) {
    BoardShopUpdateAllocation *allocation;
    s32 result;
    s32 animState;
    u16 frame;
    volatile u8 pad[8];

    allocation = (BoardShopUpdateAllocation *)getCurrentAllocation();
    animState = arg0->animationState;

    if (animState != 2) {
        result = clearModelRotation(arg0->model);
        animState = arg0->animationState;

        if (animState == 1) {
            if (result != NULL) {
                frame = arg0->animationFrame + 1;
                arg0->animationFrame = frame;

                if (arg0->animationEndFrame < frame) {
                    arg0->animationState = 0;
                    arg0->animationFrame = 0x10;
                }

                setModelAnimation(arg0->model, arg0->animationFrame);
                animState = arg0->animationState;
            }
        }
    }

    if (animState != 1 || allocation->unk7A3 != 0) {
        if (allocation->unk77E != 0) {
            arg0->animationFrame = D_8008F16C_8FD6C[allocation->unk77E * 2];
            arg0->animationEndFrame = D_8008F16E_8FD6E[allocation->unk77E * 2];
            setModelAnimation(arg0->model, arg0->animationFrame);
            allocation->unk77E = 0;
            arg0->animationState = 1;
            allocation->unk7A3 = 0;
        }
    }

    func_8006FED8_70AD8(&allocation->audioPlayer2);
    updateModelGeometry(arg0->model);
}

void cleanupBoardShopShopkeeper(BoardShopShopkeeperState *arg0) {
    destroySceneModel(arg0->model);
}

void loadBoardShopBackground(BoardShopBackgroundState *state) {
    state->backgroundAsset = loadCompressedData(&_4488E0_ROM_START, &_4488E0_ROM_END, 0x14410);
    setCleanupCallback(&cleanupBoardShopBackground);
    setCallback(&initBoardShopBackgroundRenderState);
}

void initBoardShopBackgroundRenderState(BoardShopBackgroundState *state) {
    func_800394BC_3A0BC(state, (s32)state->backgroundAsset);
    setCallback(&enqueueBoardShopBackgroundRender);
}

void enqueueBoardShopBackgroundRender(void *state) {
    debugEnqueueCallback(9, 0, &func_80038420_39020, state);
}

void cleanupBoardShopBackground(BoardShopBackgroundState *state) {
    state->backgroundAsset = freeNodeMemory(state->backgroundAsset);
}

typedef struct {
    void *unk0;
    void *asset;
} BoardShopComparisonIconsCleanupArg;

void cleanupBoardShopComparisonIcons(BoardShopComparisonIconsCleanupArg *arg0);

typedef struct {
    s16 x;
    s16 y;
    void *asset;
    s16 spriteIndex;
    s16 alpha;
    s8 unkC;
    s8 unkD;
    s16 unkE;
} BoardShopComparisonIconState;

typedef struct {
    BoardShopComparisonIconState icons[2];
    u8 animationCounter;
} BoardShopComparisonIconsState;

void updateBoardShopComparisonIcons(BoardShopComparisonIconsState *arg0);

void initBoardShopComparisonIcons(BoardShopComparisonIconsState *arg0) {
    s32 i;
    void *spriteAsset;

    spriteAsset = loadCompressedData(&_4237C0_ROM_START, &_4237C0_ROM_END, 0x8A08);
    setCleanupCallback(&cleanupBoardShopComparisonIcons);

    for (i = 0; i < 2; i++) {
        arg0->icons[i].x = i * 0x50 + -0x30;
        arg0->icons[i].y = -0x18;
        arg0->icons[i].spriteIndex = i;
        arg0->icons[i].asset = spriteAsset;
        arg0->icons[i].alpha = 0xFF;
        arg0->icons[i].unkD = 0;
        arg0->icons[i].unkC = 0;
    }

    arg0->animationCounter = 0;
    setCallback(&updateBoardShopComparisonIcons);
}

void updateBoardShopComparisonIcons(BoardShopComparisonIconsState *arg0) {
    BoardShopSelectionAllocation *allocation;
    BoardShopComparisonIconState *icon;
    u8 state;
    s16 s4;
    s16 s3;

    allocation = getCurrentAllocation();

    if (allocation->shopState < 15) {
        s4 = 0xFF;
        s3 = 8;
        icon = arg0->icons;

    loop:
        state = allocation->shopState;
        if ((state == 2) | (state == 5)) {
            if (arg0->animationCounter < 16) {
                icon->alpha = icon->alpha - 8;
            } else {
                icon->alpha = icon->alpha + 8;
            }
        } else {
            arg0->animationCounter = 0;
            icon->alpha = s4;
            state = allocation->shopState;
            if ((state == 4) & (state == 7)) {
                if (allocation->frameCounter & 1) {
                    icon->unkD = s4;
                } else {
                    icon->unkD = 0;
                }
            }
        }

        state = allocation->shopState;
        if (state >= 5) {
            icon->y = s3;
        } else {
            icon->y = -24;
        }

        if (allocation->shopState == 3) {
            if (allocation->unk79D < 0) {
                icon->y = s3;
            }
        }

        debugEnqueueCallback(8, 0, func_80012004_12C04, icon);
        icon++;
        if ((s32)icon < (s32)(arg0->icons + 2)) {
            goto loop;
        }

        state = allocation->shopState;
        if ((state != 4) & (state != 7)) {
            if (state != 3) {
                arg0->animationCounter = (arg0->animationCounter + 1) & 0x1F;
            }
        }
    }
}

void cleanupBoardShopComparisonIcons(BoardShopComparisonIconsCleanupArg *arg0) {
    arg0->asset = freeNodeMemory(arg0->asset);
}

typedef struct {
    void *unk0;
    void *asset;
} BoardShopRowSelectorArrowCleanupArg;

void cleanupBoardShopRowSelectorArrow(BoardShopRowSelectorArrowCleanupArg *arg0);

typedef struct {
    s16 x;
    s16 y;
    void *asset;
    s16 spriteIndex;
    s16 alpha;
    s8 unkC;
    s8 unkD;
} BoardShopRowSelectorArrowState;

typedef struct {
    u8 _pad[0x77C];
    u16 unk77C;
    u8 _pad2[0x1D];
    u8 unk79B;
    u8 _pad3[0x5];
    s8 unk7A1;
    s8 unk7A2;
} BoardShopRowSelectorArrowAllocation;

void updateBoardShopRowSelectorArrow(BoardShopRowSelectorArrowState *arg0);

void initBoardShopRowSelectorArrow(BoardShopRowSelectorArrowState *arg0) {
    void *asset;

    getCurrentAllocation();
    asset = loadCompressedData(&_4237C0_ROM_START, &_4237C0_ROM_END, 0x8A08);
    setCleanupCallback(&cleanupBoardShopRowSelectorArrow);
    arg0->x = -0x1C;
    arg0->y = -0x18;
    arg0->spriteIndex = 0x1D;
    arg0->alpha = 0xFF;
    arg0->unkC = 0;
    arg0->unkD = 0;
    arg0->asset = asset;
    setCallback(&updateBoardShopRowSelectorArrow);
}

void updateBoardShopRowSelectorArrow(BoardShopRowSelectorArrowState *arg0) {
    BoardShopRowSelectorArrowAllocation *allocation;
    u8 temp;

    allocation = getCurrentAllocation();

    if (allocation->unk79B < 0xF) {
        if (allocation->unk79B != 3) {
            arg0->spriteIndex = allocation->unk7A1 + 0x1D;

            if (allocation->unk79B == 4) {
                if (allocation->unk77C & 1) {
                    temp = 0xFF;
                    arg0->unkD = temp;
                } else {
                    arg0->unkD = 0;
                }
            }

            debugEnqueueCallback(8, 0, func_80012004_12C04, arg0);
        }
    }
}

void cleanupBoardShopRowSelectorArrow(BoardShopRowSelectorArrowCleanupArg *arg0) {
    arg0->asset = freeNodeMemory(arg0->asset);
}

typedef struct {
    s16 x;
    s16 y;
    void *asset;
    s16 spriteIndex;
    s16 alpha;
    s8 unkC;
    s8 unkD;
} BoardShopColumnSelectorArrowState;

void updateBoardShopColumnSelectorArrow(BoardShopColumnSelectorArrowState *arg0);

void initBoardShopColumnSelectorArrow(BoardShopColumnSelectorArrowState *arg0) {
    void *asset;

    getCurrentAllocation();
    asset = loadCompressedData(&_4237C0_ROM_START, &_4237C0_ROM_END, 0x8A08);
    setCleanupCallback(&cleanupBoardShopColumnSelectorArrow);
    arg0->x = -8;
    arg0->y = 8;
    arg0->spriteIndex = 0x24;
    arg0->alpha = 0xFF;
    arg0->unkC = 0;
    arg0->unkD = 0;
    arg0->asset = asset;
    setCallback(&updateBoardShopColumnSelectorArrow);
}

void updateBoardShopColumnSelectorArrow(BoardShopColumnSelectorArrowState *arg0) {
    BoardShopRowSelectorArrowAllocation *allocation;
    u8 state;
    u8 temp;

    allocation = getCurrentAllocation();
    state = allocation->unk79B;

    if (state < 0xF) {
        if ((state >= 5) && (state != 6)) {
            arg0->spriteIndex = allocation->unk7A2 + 0x24;

            if (allocation->unk79B == 7) {
                if (allocation->unk77C & 1) {
                    temp = 0xFF;
                    arg0->unkD = temp;
                } else {
                    arg0->unkD = 0;
                }
            }

            debugEnqueueCallback(8, 0, &func_80012004_12C04, arg0);
        }
    }
}

typedef struct {
    void *unk0;
    void *asset;
} BoardShopColumnSelectorArrowCleanupArg;

void cleanupBoardShopColumnSelectorArrow(void *arg0) {
    BoardShopColumnSelectorArrowCleanupArg *temp = arg0;
    temp->asset = freeNodeMemory(temp->asset);
}

void initBoardShopExitOverlay(SpriteDisplayState *arg0) {
    void *overlayAsset = loadCompressedData(&_41A1D0_ROM_START, &_41A1D0_ROM_END, 0x1B48);
    setCleanupCallback(&cleanupBoardShopExitOverlay);
    arg0->x = -0x2C;
    arg0->y = -0x14;
    arg0->spriteIndex = 0xD;
    arg0->asset = overlayAsset;
    setCallback(&updateBoardShopExitOverlay);
}

void updateBoardShopExitOverlay(void *arg0) {
    BoardShopScreenAllocation *allocation;

    allocation = getCurrentAllocation();
    if (allocation->unk788[0x13] == 0x19) {
        debugEnqueueCallback(8, 7, &func_8000FED0_10AD0, arg0);
    }
}

void cleanupBoardShopExitOverlay(SpriteDisplayState *arg0) {
    arg0->asset = freeNodeMemory(arg0->asset);
}

void initBoardShopGoldDisplay(BoardShopGoldDisplayState *arg0) {
    void *digitAsset = loadCompressedData(&_3F6950_ROM_START, &_3F6950_ROM_END, 0x508);
    void *iconAsset = loadCompressedData(&_3F6670_ROM_START, &_3F6670_ROM_END, 0x388);
    s32 i;

    setCleanupCallback(&cleanupBoardShopGoldDisplay);

    for (i = 0; i < 7; i++) {
        arg0->digits[i].x = 0x48 + (i * 8);
        arg0->digits[i].y = 0x58;
        arg0->digits[i].asset = digitAsset;
    }

    arg0->iconX = 0x38;
    arg0->iconY = 0x58;
    arg0->unk5C = 0;
    arg0->unk5E = 0;
    arg0->iconAsset = iconAsset;

    setCallback(&updateBoardShopGoldDisplay);
}

void updateBoardShopGoldDisplay(BoardShopGoldDisplayState *arg0) {
    BoardShopGoldDigitState *digit;
    s32 space;
    s32 i;
    s32 colorStyle;

    if (D_800AFE8C_A71FC->gold < 100) {
        colorStyle = 1;
        i = 6;
        do {
            arg0->digits[i].colorStyle = colorStyle;
        } while (--i >= 0);
    } else {
        colorStyle = 2;
        i = 6;
        do {
            arg0->digits[i].colorStyle = colorStyle;
        } while (--i >= 0);
    }

    sprintf(arg0->goldString, D_8009E47C_9F07C, D_800AFE8C_A71FC->gold);

    i = 0;
    space = 0x20;
    digit = arg0->digits;

    for (; i < 7; i++) {
        if (arg0->goldString[i] != space) {
            digit->digitValue = arg0->goldString[i] - 0x30;
            debugEnqueueCallback(9, 7, func_80010240_10E40, digit);
        }
        digit++;
    }

    debugEnqueueCallback(9, 7, func_80010240_10E40, &arg0->iconX);
}

void cleanupBoardShopGoldDisplay(BoardShopGoldDisplayCleanupArg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
    arg0->unk58 = freeNodeMemory(arg0->unk58);
}

void initBoardShopBoardIcons(BoardShopBoardIconsState *arg0) {
    BoardShopScreenAllocation *state;
    void *spriteAsset;
    s32 i;
    u8 boardIndex;

    state = getCurrentAllocation();
    spriteAsset = loadCompressedData(&_4547D0_ROM_START, &_4547D0_ROM_END, 0x9488);

    for (i = 0; i < 4; i++) {
        arg0->icons[i].x = 0x60;
        arg0->icons[i].y = -0x91;

        boardIndex = state->unk784[i];
        boardIndex = state->unk788[boardIndex];

        arg0->icons[i].spriteIndex = boardIndex;
        arg0->icons[i].scaleX = 0x400;
        arg0->icons[i].scaleY = 0x400;
        arg0->icons[i].rotation = 0;
        arg0->icons[i].alpha = 0xFF;
        arg0->icons[i].unk13 = 0;
        arg0->icons[i].unk12 = 0;
        arg0->icons[i].flipX = 0;
        arg0->icons[i].asset = spriteAsset;
        arg0->animationCounters[i] = 0;
    }

    arg0->priceTextX = 0x58;
    arg0->priceTextY = -0x2A;
    arg0->priceTextStyle = 0;
    arg0->priceTextPtr = &arg0->priceTextBuffer;

    setCleanupCallback(&cleanupBoardShopBoardIcons);
    setCallback(&animateBoardShopBoardIconsSlideIn);
}

void animateBoardShopBoardIconsSlideIn(BoardShopBoardIconsState *arg0) {
    func_80032628_alloc *allocation;
    s32 animatingCount;
    s32 i;
    s16 currentY;
    s32 delta;
    s16 absDelta;

    allocation = getCurrentAllocation();
    animatingCount = 0;

    for (i = 0; i < 4; i++) {
        currentY = arg0->icons[i].y;
        if (currentY < boardIconTargetYPositions[i]) {
            delta = boardIconTargetYPositions[i] - currentY;
            absDelta = ABS(delta);
            if (absDelta >= 20) {
                arg0->icons[i].y = currentY + 20;
            } else {
                arg0->icons[i].y = currentY + absDelta;
            }
            animatingCount++;
        }
        debugEnqueueCallback(8, 0, func_800136E0_142E0, &arg0->icons[i]);
    }

    if ((animatingCount & 0xFF) == 0) {
        allocation->unk77C = 1;
        setCallback(updateBoardShopBoardIconSelection);
    }
}

void updateBoardShopBoardIconSelection(BoardShopIconSelectionState *arg0) {
    BoardShopSelectionAllocation *state;
    s32 i;
    int new_var;
    u8 temp;
    s16 adjustment;
    state = getCurrentAllocation();
    for (i = 0; i < 4; i++) {
        if (state->selectedIconIndex == i) {
            arg0->icons[i].alpha = 0xFF;
            if (state->frameCounter >= 5) {
                temp = arg0->animationCounters[i];
                if (temp < 30) {
                    adjustment = D_8008F184_8FD84[temp / 10];
                    arg0->icons[i].scaleX = arg0->icons[i].scaleX + adjustment;
                } else {
                    adjustment = D_8008F184_8FD84[2 - ((temp - 30) / 10)];
                    arg0->icons[i].scaleX = arg0->icons[i].scaleX - adjustment;
                }
                arg0->animationCounters[i]++;
                temp = arg0->animationCounters[i];
                if (temp == 60) {
                    arg0->animationCounters[i] = 0;
                    arg0->icons[i].scaleX = 0x400;
                } else if (temp == 30) {
                    arg0->icons[i].flipX = (arg0->icons[i].flipX + 1) & 1;
                }
            }
        } else {
            arg0->icons[i].alpha = 0x80;
            arg0->animationCounters[i] = 0;
            arg0->icons[i].flipX = 0;
            arg0->icons[i].scaleX = 0x400;
        }
        debugEnqueueCallback(8, 0, func_800136E0_142E0, &arg0->icons[i]);
    }

    if (state->shopState == 0x11) {
        for (i = 0; i < 4; i++) {
            arg0->icons[i].alpha = 0x80;
            arg0->icons[i].scaleX = 0x400;
            arg0->icons[i].flipX = 0;
            arg0->icons[i].spriteIndex = state->unk788[state->unk784[i]];
        }

        arg0->animationCounters[0] = 1;
        setCallback(animateBoardShopBoardIconsSlideOut);
    } else if (state->shopState == 0x13) {
        new_var = 0x400;
        for (i = 3; i >= 0; i--) {
            arg0->icons[i].scaleX = new_var;
        }

        setCallback(initBoardShopCharacterPortraitsSlideIn);
    } else {
        temp = state->unk788[state->unk784[state->selectedIconIndex]];
        sprintf(&arg0->priceTextBuffer, &D_8009E480_9F080, D_8008F150_8FD50[temp]);
        arg0->priceTextY = (state->selectedIconIndex * 0x28) - 0x2A;
        debugEnqueueCallback(8, 7, renderTextPalette, &arg0->priceTextX);
        if (state->shopState == 0x14) {
            setCallback(blinkBoardShopBoardIconConfirmation);
        }
    }
}

void blinkBoardShopBoardIconConfirmation(BoardShopIconSelectionState *arg0) {
    BoardShopSelectionAllocation *state;
    s32 i;
    u8 temp;
    s16 temp2;

    state = getCurrentAllocation();

    for (i = 0; i < 4; i++) {
        arg0->icons[i].scaleX = 0x400;
        arg0->animationCounters[i] = 0;
        temp2 = 0xFF;

        if (state->selectedIconIndex == i) {
            arg0->icons[i].alpha = 0xFF;
            arg0->icons[i].unk13 = 0;
            if (state->shopState == 0x14) {
                if ((state->frameCounter & 1) != 0) {
                    __asm__ volatile("" ::: "memory");
                    arg0->icons[i].unk13 = temp2;
                }
            }
        } else {
            arg0->icons[i].alpha = 0x80;
        }

        debugEnqueueCallback(8, 0, &func_800136E0_142E0, &arg0->icons[i]);
    }

    temp = state->unk784[state->selectedIconIndex];
    temp = state->unk788[temp];

    sprintf(&arg0->priceTextBuffer, &D_8009E480_9F080, D_8008F150_8FD50[temp]);
    arg0->priceTextY = state->selectedIconIndex * 0x28 - 0x2A;
    debugEnqueueCallback(8, 7, &renderTextPalette, &arg0->priceTextX);
    if (state->shopState < 0x14) {
        setCallback(&updateBoardShopBoardIconSelection);
    }
}

void initBoardShopCharacterPortraitsSlideIn(BoardShopCharacterPortraitState *arg0) {
    BoardShopScreenAllocation *allocation;
    s32 i;
    s32 startX;
    s16 currentX;

    allocation = getCurrentAllocation();

    if (allocation->unk79C == 1) {
        startX = -0x11;
    } else {
        startX = -0x61;
    }

    i = 0;
    currentX = startX;

    for (i = 0; i < 4; i++) {
        arg0->portraits[i].x = currentX;
        arg0->portraits[i].spriteIndex = allocation->unk788[allocation->unk784[i]];
        arg0->animationFrameCounters[i] = 0;
        debugEnqueueCallback(8, 0, &func_800136E0_142E0, &arg0->portraits[i]);
        currentX += 0x28;
    }

    setCallback(&animateBoardShopCharacterPortraitsSlideIn);
}

void animateBoardShopCharacterPortraitsSlideIn(BoardShopCharacterPortraitState *arg0) {
    BoardShopScreenAllocation *gameState;
    s32 portraitIndex;
    BoardShopCharacterPortrait *portraits;

    gameState = getCurrentAllocation();
    portraitIndex = 0;
    portraits = arg0->portraits;

    while (portraitIndex < 4) {
        arg0->animationFrameCounters[portraitIndex]++;

        if (gameState->unk79C == 1) {
            portraits[portraitIndex].x -= 10;
        } else {
            portraits[portraitIndex].x += 10;
        }

        debugEnqueueCallback(8, 0, &func_800136E0_142E0, &portraits[portraitIndex]);
        portraitIndex++;
    }

    if (arg0->animationFrameCounters[0] == 4) {
        u8 *frameCounterPtr;
        gameState->unk788[19] = 0x10;
        portraitIndex = 3;
        frameCounterPtr = (u8 *)arg0 + 3;

        do {
            frameCounterPtr[0x84] = 0;
            portraitIndex--;
            frameCounterPtr--;
        } while (portraitIndex >= 0);

        setCallback(&updateBoardShopBoardIconSelection);
    }
}

void animateBoardShopBoardIconsSlideOut(BoardShopBoardIconsSlideOutState *arg0) {
    BoardShopSlideOutAllocation *allocation;
    BoardShopSlideOutIconState *icons;
    u8 slideCount;
    s32 i;

    allocation = (BoardShopSlideOutAllocation *)getCurrentAllocation();

    icons = &arg0->icons[0];
    for (i = 0; i < 4; i++) {
        if (i >= (4 - arg0->slidingIconCount)) {
            icons[i].y -= 0x14;
            if (i != 0) {
                slideCount = arg0->slidingIconCount & 0xFF;
                if (arg0->icons[4 - slideCount].y == arg0->icons[3 - slideCount].y) {
                    arg0->slidingIconCount++;
                }
            }
        }
        debugEnqueueCallback(8, 0, &func_800136E0_142E0, &icons[i]);
    }

    if (arg0->icons[0].y < (-0x88)) {
        allocation->animationComplete = 1;
        func_80069CF8_6A8F8();
    }
}

void cleanupBoardShopBoardIcons(BoardShopBoardIconsState *arg0) {
    arg0->icons[0].asset = freeNodeMemory(arg0->icons[0].asset);
}

void initBoardShopSnowflakeSlideIn(BoardShopSnowflakeSpriteState *arg0) {
    void *snowflakeAsset;
    BoardShopScreenAllocation *allocation = getCurrentAllocation();

    snowflakeAsset = loadCompressedData(&_4547D0_ROM_START, &_4547D0_ROM_END, 0x9488);

    arg0->x = 0x60;
    arg0->spriteIndex = allocation->unk788[allocation->unk7A0];
    arg0->scaleX = 0x400;
    arg0->scaleY = 0x400;
    arg0->rotation = 0;
    arg0->alpha = 0x80;
    arg0->unk13 = 0;
    arg0->unk12 = 0;
    arg0->flipX = 0;
    arg0->asset = snowflakeAsset;
    arg0->frameCounter = 0;

    if (allocation->unk79C != 1) {
        arg0->y = 0x3F;
    } else {
        arg0->y = -0x39;
    }

    setCleanupCallback(&cleanupBoardShopSnowflakeSprite);
    setCallback(&queueBoardShopSnowflakeRender);
}

void queueBoardShopSnowflakeRender(void *arg0) {
    debugEnqueueCallback(8, 0, &func_800136E0_142E0, arg0);
    setCallback(&animateBoardShopSnowflakeSlideIn);
}

void animateBoardShopSnowflakeSlideIn(BoardShopSnowflakeAnimState *arg0) {
    BoardShopSnowflakeAnimAllocation *allocation = (BoardShopSnowflakeAnimAllocation *)getCurrentAllocation();

    arg0->frameCounter++;
    if (allocation->unk79C == 1) {
        arg0->y = arg0->y - 0x14;
    } else {
        arg0->y = arg0->y + 0x14;
    }

    debugEnqueueCallback(8, 0, &func_800136E0_142E0, arg0);

    if (arg0->frameCounter == 4) {
        func_80069CF8_6A8F8();
    }
}

void cleanupBoardShopSnowflakeSprite(SpriteDisplayState *arg0) {
    arg0->asset = freeNodeMemory(arg0->asset);
}

void initBoardShopTitleText(BoardShopTitleTextState *arg0) {
    void *textAsset = func_80035F80_36B80(1);
    setCleanupCallback(&cleanupBoardShopTitleText);
    arg0->y = -0x60;
    arg0->textData = &D_8008F18C_8FD8C;
    arg0->primaryColor = 0xFF;
    arg0->secondaryColor = 0xFF;
    arg0->textAsset = textAsset;
    arg0->textWidth = D_8008F20C_8FE0C;
    arg0->textStyle = 5;
    setCallback(&updateBoardShopTitleText);
}

void updateBoardShopTitleText(BoardShopTitleTextUpdateArg *arg0) {
    func_8003316C_33D6C_alloc *allocation = (func_8003316C_33D6C_alloc *)getCurrentAllocation();
    if (allocation->unk7A4 != 0) {
        arg0->textWidth = D_8008F20A_8FE0A[allocation->unk7A4];
        arg0->textData = D_8008F200_8FE00[allocation->unk7A4];
        debugEnqueueCallback(9, 7, &func_80035408_36008, arg0);
    }
}

void cleanupBoardShopTitleText(BoardShopTitleTextCleanupArg *arg0) {
    arg0->textAsset = freeNodeMemory(arg0->textAsset);
}

void func_800330B4_33CB4(func_800330B4_33CB4_arg *arg0) {
    s32 i;
    void *asset = loadCompressedData(&_419C60_ROM_START, &_419C60_ROM_END, 0x1548);
    setCleanupCallback(&func_800331CC_33DCC);

    for (i = 0; i < 4; i++) {
        func_800330B4_33CB4_arg *j = &arg0[i];

        if (i % 2 != 0) {
            j->unk0 = -0x80;
        } else {
            j->unk0 = 0;
        }

        j->unk2 = (s16)(((i / 2) * 0x10) - 0x66);
        j->unk4 = asset;
        j->unk8 = i;
        j->unkA = 0xFF;
        j->unkD = 0;
        j->unkC = 1;
    }

    setCallback(&func_8003316C_33D6C);
}

void func_8003316C_33D6C(func_800330B4_33CB4_arg *arg0) {
    s32 i;

    if (((func_8003316C_33D6C_alloc *)getCurrentAllocation())->unk7A4 != NULL) {
        for (i = 0; i < 4; i++) {
            debugEnqueueCallback(9, 1, &func_80012004_12C04, &arg0[i]);
        }
    }
}

void func_800331CC_33DCC(func_800330B4_33CB4_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}
