#include "10AD0.h"
#include "20F0.h"
#include "38C90.h"
#include "D_800AFE8C_A71FC_type.h"
#include "assets.h"
#include "common.h"
#include "displaylist.h"
#include "gamestate.h"
#include "geometry.h"
#include "graphics.h"
#include "rom_loader.h"
#include "task_scheduler.h"

typedef struct {
    SceneModel *model;
    Transform3D matrix;
    u16 characterIndex;
    s16 animationState;
    u8 displayMode;
    u8 timer;
} CharacterPreviewState;

typedef struct {
    u8 _pad0[0x2C];
    void *textureData;
} TextureDataTaskState;

typedef struct {
    s16 xOffset;
    s16 yOffset;
    void *spriteData;
    s16 frameIndex;
} CharacterSelectIndicatorTask;

typedef struct {
    u8 _pad0[0x4];
    void *spriteData;
} CharacterSelectIndicatorCleanupTask;

typedef struct {
    s16 coordX;
    s16 coordY;
    s16 textDisplayX;
    s16 textDisplayY;
    s16 timer;
    u8 _padA[2];
    char *textBuffer;
    s16 secondDisplayX;
    s16 secondDisplayY;
    s16 counter;
    u8 _pad16[2];
    char *formatString;
    char formattedText[16];
} CoordinateDisplayTaskState;

typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ void *spriteData;
    /* 0x08 */ s16 frameIndex;
    /* 0x0A */ s16 scale;
    /* 0x0C */ u8 flags;
    /* 0x0D */ u8 alpha;
    /* 0x0E */ u8 _pad[2];
} CharacterSelectSprite;

typedef struct {
    CharacterSelectSprite sprites[4];
    u8 animTimers[8];
} CharacterSelectSprites;

typedef struct {
    /* 0x00 */ s16 m[9];
    /* 0x12 */ s16 _pad;
    /* 0x14 */ s32 translateX;
    /* 0x18 */ s32 translateY;
    /* 0x1C */ s32 translateZ;
    /* 0x20 */ void *displayList;
    /* 0x24 */ void *textureData1;
    /* 0x28 */ void *textureData2;
    /* 0x2C */ s32 animState;
} CharacterSelectBoardTask;

typedef struct {
    s16 x, y;
} Vec2s;

typedef struct {
    u8 padding[0x592];
    u16 unk592;
    u8 padding2[0xE];
    u8 unk5A2;
} LocalGameState_Player;

typedef struct {
    LocalGameState_Player *players[4];
    u8 pad[0x582];
    u16 unk592[8];
    u8 unk5A2[8];
} LocalGameState;

typedef struct {
    u8 _pad0[0x59A];
    u8 playerSlotState[8];
    u8 _pad5A2[0x1F];
    u8 unk5C1[8];
} func_800B0A54_allocation;

extern void *renderTextPalette;

void func_800B00C0_1DA660(void);
void positionCharacterSelectSprite(CharacterSelectSprite *, u8);
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
void cleanupCharacterSelectIndicator(CharacterSelectIndicatorCleanupTask *);
void updatePlayer2CharacterSelectIndicator(void *);
void cleanupCharacterSelectIndicatorData(CharacterSelectIndicatorCleanupTask *);
void initCharacterSelectTextureRenderState(TextureDataTaskState *);
void renderCharacterSelectBoard(void *);
void cleanupCharacterSelectBoardTask(CharacterSelectBoardTask *);
void updateCoordinateDisplayTask(CoordinateDisplayTaskState *);

extern char gCoordDisplayFormatString[];
extern s16 D_800B1160_1DB700[];
extern s16 D_800B1162_1DB702[];
extern Vec2s D_800B11A0_1DB740[];
extern s16 D_800B11C2_1DB762[];
extern u8 identityMatrix[];
extern void *D_800B1140_1DB6E0;
extern char D_800B115C_1DB6FC[];
extern s32 gButtonsPressed;
extern u8 D_800B1150_1DB6F0[];

INCLUDE_ASM("asm/nonmatchings/1DA660", func_800B00C0_1DA660);

void positionCharacterSelectSprite(CharacterSelectSprite *arg0, u8 arg1) {
    LocalGameState *allocation;
    u8 playerState;
    u8 count;
    s32 i;
    volatile u8 padding[8];

    allocation = (LocalGameState *)getCurrentAllocation();

    arg0->x = D_800B11A0_1DB740[allocation->unk592[arg1]].x;
    arg0->y = D_800B11A0_1DB740[allocation->unk592[arg1]].y;

    if (allocation->unk5A2[arg1] >= 3) {
        arg0->x += 0x10;
    }

    if (!(allocation->unk5A2[arg1] & 1)) {
        arg0->y += 0x10;
    }

    count = 0;
    for (i = 0; i < D_800AFE8C_A71FC->numPlayers; i++) {
        if (allocation->unk592[arg1] == allocation->unk592[i]) {
            count++;
        }
    }

    playerState = allocation->unk5A2[arg1];
    if ((playerState == 3) && (count == 3)) {
        arg0->y += 0x10;
    }

    count--;
    arg0->frameIndex = D_800B11C2_1DB762[count * 4 + allocation->unk5A2[arg1]] + arg1;
}

void initCharacterPreview(CharacterPreviewState *arg0) {
    GameState *allocation;
    Transform3D *matrix;
    u16 modelIndex;
    s32 pad[2];

    allocation = getCurrentAllocation();
    modelIndex = arg0->characterIndex + 0x32;
    arg0->displayMode = 0;

    if (arg0->characterIndex == 6) {
        if (allocation->unk5B2 == -1) {
            arg0->displayMode = 2;
        }
    }

    if (arg0->characterIndex == 7) {
        if (allocation->unk5AE == -1) {
            arg0->displayMode = 1;
            modelIndex = 0xF;
        }
    }

    if (arg0->characterIndex == 8) {
        if (allocation->unk5AD == -1) {
            arg0->displayMode = 1;
            modelIndex = 0xF;
        }
    }

    matrix = &arg0->matrix;
    arg0->model = createSceneModel(modelIndex, allocation);
    memcpy(matrix, identityMatrix, 0x20);

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
        allocation->isStoryMapInitializing = 0;
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
    D_800AFE8C_A71FC_type *ptr;
    s32 pad[2];
    s32 count;

    allocation = getCurrentAllocation();

    if (arg0->displayMode != 2) {
        applyTransformToModel(arg0->model, &arg0->matrix);
        clearModelRotation(arg0->model);
        updateModelGeometry(arg0->model);
    }

    if (arg0->displayMode == 0) {
        ptr = D_800AFE8C_A71FC;
        count = ptr->numPlayers;
        if (count > 0) {
            i = 0;
            do {
                if (ptr->unk9[i] == arg0->characterIndex) {
                    u8 state = allocation->playerSlotState[i];
                    if (state == 1 || state == 3) {
                        arg0->timer = 0;
                        setCallback(animateCharacterPreview);
                        return;
                    }
                }
                ptr = D_800AFE8C_A71FC;
                i++;
            } while (i < ptr->numPlayers);
        }
    }
}

void animateCharacterPreview(CharacterPreviewState *arg0) {
    func_800B0A54_allocation *allocation;
    D_800AFE8C_A71FC_type *ptr;
    s32 i;
    s32 count;
    s32 clearResult;
    u8 counter;
    u16 index;
    s32 pad[2];

    allocation = (func_800B0A54_allocation *)getCurrentAllocation();
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

    ptr = D_800AFE8C_A71FC;
    count = ptr->numPlayers;
    i = 0;
    if (count > 0) {
        D_800AFE8C_A71FC_type *localPtr = ptr;
        u16 localIndex = arg0->characterIndex;
        u8 localCount = count;
        do {
            if (localPtr->unk9[i] != localIndex) {
                i++;
            } else {
                u8 state = allocation->playerSlotState[i];
                if (state == 1 || state == 3) {
                    break;
                }
                i++;
            }
        } while (i < localCount);
    }

    if (i == D_800AFE8C_A71FC->numPlayers) {
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
    allocation = loadCompressedData(&_41A1D0_ROM_START, &_41AD80_ROM_START, 0x1B48);

    for (i = 0; i < D_800AFE8C_A71FC->numPlayers; i++) {
        arg0->sprites[i].spriteData = allocation;
        arg0->sprites[i].scale = 0xFF;
        arg0->sprites[i].alpha = 0;
        arg0->sprites[i].flags = 0;
        arg0->animTimers[i] = 0;
        positionCharacterSelectSprite(&arg0->sprites[i], i);
    }

    setCleanupCallback(cleanupCharacterSelectSprites);
    setCallback(updateCharacterSelectSprites);
}

void updateCharacterSelectSprites(CharacterSelectSprites *arg0) {
    func_800B0A54_allocation *allocation;
    s32 i;
    u8 state;

    allocation = (func_800B0A54_allocation *)getCurrentAllocation();
    func_800B00C0_1DA660();

    for (i = 0; i < D_800AFE8C_A71FC->numPlayers; i++) {
        positionCharacterSelectSprite(&arg0->sprites[i], i);
        state = allocation->playerSlotState[i];

        if (state == 10) {
            arg0->sprites[i].scale = 0xFF;
            arg0->animTimers[i] = 0;
            if (allocation->unk5C1[i] & 1) {
                arg0->sprites[i].alpha = 0xFF;
            } else {
                arg0->sprites[i].alpha = 0;
            }
        } else if (state == 0) {
            arg0->animTimers[i] = arg0->animTimers[i] % 30;
            if (arg0->animTimers[i] < 15) {
                arg0->sprites[i].scale = arg0->sprites[i].scale - 8;
            } else {
                arg0->sprites[i].scale = arg0->sprites[i].scale + 8;
            }
            arg0->animTimers[i] = arg0->animTimers[i] + 1;
        } else {
            arg0->animTimers[i] = 0;
            arg0->sprites[i].scale = 0xFF;
        }

        if (allocation->playerSlotState[i] == 2) {
            allocation->unk5C1[i] = 0;
            arg0->sprites[i].alpha = 0;
            arg0->animTimers[i] = 0;
            arg0->sprites[i].scale = 0xFF;
        }

        debugEnqueueCallback(8, 0, func_80012004_12C04, &arg0->sprites[i]);
    }
}

void cleanupCharacterSelectSprites(CharacterSelectSprites *arg0) {
    arg0->sprites[0].spriteData = freeNodeMemory(arg0->sprites[0].spriteData);
}

void initCoordinateDisplayTask(CoordinateDisplayTaskState *arg0) {
    arg0->coordX = -7;
    arg0->coordY = -7;
    arg0->textDisplayX = -120;
    arg0->textDisplayY = -104;
    arg0->textBuffer = arg0->formattedText;
    arg0->timer = 0;
    arg0->counter = 0;
    arg0->formatString = D_800B115C_1DB6FC;
    arg0->secondDisplayX = arg0->coordX;
    arg0->secondDisplayY = arg0->coordY;
    setCallback(updateCoordinateDisplayTask);
}

void updateCoordinateDisplayTask(CoordinateDisplayTaskState *arg0) {
    s32 *buttons = &gButtonsPressed;

    if (*buttons & U_JPAD) {
        arg0->coordY--;
    }
    if (*buttons & D_JPAD) {
        arg0->coordY++;
    }
    if (*buttons & R_JPAD) {
        arg0->coordX++;
    }
    if (*buttons & L_JPAD) {
        arg0->coordX--;
    }

    sprintf(arg0->formattedText, gCoordDisplayFormatString, arg0->coordX + 7, arg0->coordY + 7);

    debugEnqueueCallback(0, 7, &renderTextPalette, &arg0->textDisplayX);

    arg0->secondDisplayX = arg0->coordX;
    arg0->secondDisplayY = arg0->coordY;

    debugEnqueueCallback(0, 7, &renderTextPalette, &arg0->secondDisplayX);
}

void cleanupCharacterSelectTextureData(TextureDataTaskState *arg0) {
    arg0->textureData = freeNodeMemory(arg0->textureData);
}

void initCharacterSelectTextureDataLoad(TextureDataTaskState *arg0) {
    arg0->textureData = loadCompressedData(&_41AD80_ROM_START, &_41AD80_ROM_END, 0x13FF0);
    setCleanupCallback(cleanupCharacterSelectTextureData);
    setCallback(initCharacterSelectTextureRenderState);
}

void initCharacterSelectTextureRenderState(TextureDataTaskState *arg0) {
    initTiledTextureRenderState(arg0, (s32)arg0->textureData);
    setCallback(&enqueueCharacterSelectTextureRender);
}

void enqueueCharacterSelectTextureRender(void *arg0) {
    debugEnqueueCallback(9, 0, renderTiledTexture, arg0);
}

void initPlayer3CharacterSelectIndicator(CharacterSelectIndicatorTask *arg0) {
    void *temp;
    getCurrentAllocation();
    temp = loadCompressedData(&_41A1D0_ROM_START, &_41A1D0_ROM_END, 0x1B48);
    arg0->xOffset = -44;
    arg0->yOffset = -20;
    arg0->frameIndex = 13;
    arg0->spriteData = temp;
    setCleanupCallback(cleanupCharacterSelectIndicator);
    setCallbackWithContinue(updatePlayer3CharacterSelectIndicator);
}

void updatePlayer3CharacterSelectIndicator(void *arg0) {
    GameState *state = getCurrentAllocation();
    if (state->playerSlotState[0] == 3) {
        debugEnqueueCallback(8, 0, renderSpriteFrame, arg0);
    } else {
        terminateCurrentTask();
    }
}

void cleanupCharacterSelectIndicator(CharacterSelectIndicatorCleanupTask *arg0) {
    arg0->spriteData = freeNodeMemory(arg0->spriteData);
}

void initPlayer2CharacterSelectIndicator(CharacterSelectIndicatorTask *arg0) {
    void *temp;
    getCurrentAllocation();
    temp = loadCompressedData(&_41A1D0_ROM_START, &_41A1D0_ROM_END, 0x1B48);
    arg0->xOffset = -76;
    arg0->yOffset = -8;
    arg0->frameIndex = 12;
    arg0->spriteData = temp;
    setCleanupCallback(cleanupCharacterSelectIndicatorData);
    setCallbackWithContinue(updatePlayer2CharacterSelectIndicator);
}

void updatePlayer2CharacterSelectIndicator(void *arg0) {
    GameState *state = getCurrentAllocation();
    if (state->playerSlotState[0] == 2) {
        debugEnqueueCallback(8, 0, renderSpriteFrame, arg0);
    } else {
        terminateCurrentTask();
    }
}

void cleanupCharacterSelectIndicatorData(CharacterSelectIndicatorCleanupTask *arg0) {
    arg0->spriteData = freeNodeMemory(arg0->spriteData);
}

void initCharacterSelectBoardTask(CharacterSelectBoardTask *arg0) {
    void *texture1;
    void *texture2;

    texture1 = loadUncompressedData(&_1DC0D0_ROM_START, &_1DC0D0_ROM_END);
    texture2 = loadUncompressedData(&_422C60_ROM_START, &_422C60_ROM_END);
    setCleanupCallback(cleanupCharacterSelectBoardTask);

    memcpy(arg0, identityMatrix, 0x20);
    arg0->displayList = &D_800B1140_1DB6E0;
    arg0->translateX = 0x2C0000;
    arg0->translateZ = (s32)0xFF9F0000;
    arg0->textureData1 = texture1;
    arg0->textureData2 = texture2;
    arg0->animState = 0;
    arg0->translateY = (s32)0xFFF40000;

    createZRotationMatrix((Transform3D *)arg0, 0x1F50);
    setCallback(renderCharacterSelectBoard);
}

void renderCharacterSelectBoard(void *arg0) {
    getCurrentAllocation();
    enqueueDisplayListObjectWithFullRenderState(0, arg0);
}

void cleanupCharacterSelectBoardTask(CharacterSelectBoardTask *arg0) {
    arg0->textureData1 = freeNodeMemory(arg0->textureData1);
    arg0->textureData2 = freeNodeMemory(arg0->textureData2);
}
