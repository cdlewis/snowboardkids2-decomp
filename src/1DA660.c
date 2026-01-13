#include "10AD0.h"
#include "20F0.h"
#include "38C90.h"
#include "6E840.h"
#include "D_800AFE8C_A71FC_type.h"
#include "common.h"
#include "displaylist.h"
#include "gamestate.h"
#include "geometry.h"
#include "rom_loader.h"
#include "task_scheduler.h"

USE_ASSET(_41A1D0);
USE_ASSET(_41AD80);
USE_ASSET(_422C60);
USE_ASSET(_1DC0D0);

typedef struct {
    u8 _pad0[0x2C];
    s32 unk2C;
} func_800B0DD0_arg;

typedef struct {
    SceneModel *model;
    Transform3D matrix;
    u16 characterIndex;
    s16 animationState;
    u8 displayMode;
    u8 timer;
} CharacterPreviewState;

typedef struct {
    u8 _pad[0x24];
    void *unk24;
    void *unk28;
} func_800B1104_arg;

typedef struct {
    u8 _pad0[0x2C];
    void *unk2C;
} func_800B0D4C_arg;

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
} func_800B0F18_arg;

typedef struct {
    u8 _pad0[0x4];
    void *unk4;
} func_800B0FE0_arg;

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    u8 _padA[2];
    char *unkC;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    u8 _pad16[2];
    char *unk18;
    char unk1C[16];
} func_800B0BEC_arg;

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
    s16 m[9];
    s16 _pad;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    void *unk20;
    void *unk24;
    void *unk28;
    s32 unk2C;
} func_800B100C_arg;

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
    u8 unk59A[8];
    u8 _pad5A2[0x1F];
    u8 unk5C1[8];
} func_800B0A54_allocation;

extern void *renderTextPalette;

void func_800B00C0_1DA660(void);
void positionCharacterSelectSprite(CharacterSelectSprite *, u8);
void func_800B0DF8_1DB398(void *);
void awaitCharacterPreviewReady(CharacterPreviewState *);
void updateCharacterPreviewAnimation(CharacterPreviewState *);
void cleanupCharacterPreview(CharacterPreviewState *);
void checkCharacterPreviewState(CharacterPreviewState *);
void animateCharacterPreview(CharacterPreviewState *);
void awaitCharacterPreviewRotationReset(CharacterPreviewState *);
void updateCharacterSelectSprites(CharacterSelectSprites *);
void func_800B0BC0_1DB160(func_800B0FE0_arg *);
void func_800B0E94_1DB434(void *);
void func_800B0EEC_1DB48C(func_800B0FE0_arg *);
void func_800B0F88_1DB528(void *);
void func_800B0FE0_1DB580(func_800B0FE0_arg *);
void func_800B0DD0_1DB370(func_800B0DD0_arg *);
void func_800B10D4_1DB674(void *);
void func_800B1104_1DB6A4(func_800B1104_arg *);
void func_800B0C54_1DB1F4(func_800B0BEC_arg *);

extern char D_800B11F0_1DB790[];
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
                    u8 state = allocation->unk59A[i];
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
                u8 state = allocation->unk59A[i];
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

    setCleanupCallback(func_800B0BC0_1DB160);
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
        state = allocation->unk59A[i];

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

        if (allocation->unk59A[i] == 2) {
            allocation->unk5C1[i] = 0;
            arg0->sprites[i].alpha = 0;
            arg0->animTimers[i] = 0;
            arg0->sprites[i].scale = 0xFF;
        }

        debugEnqueueCallback(8, 0, func_80012004_12C04, &arg0->sprites[i]);
    }
}

void func_800B0BC0_1DB160(func_800B0FE0_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_800B0BEC_1DB18C(func_800B0BEC_arg *arg0) {
    arg0->unk0 = -7;
    arg0->unk2 = -7;
    arg0->unk4 = -120;
    arg0->unk6 = -104;
    arg0->unkC = arg0->unk1C;
    arg0->unk8 = 0;
    arg0->unk14 = 0;
    arg0->unk18 = D_800B115C_1DB6FC;
    arg0->unk10 = arg0->unk0;
    arg0->unk12 = arg0->unk2;
    setCallback(func_800B0C54_1DB1F4);
}

void func_800B0C54_1DB1F4(func_800B0BEC_arg *arg0) {
    s32 *buttons = &gButtonsPressed;

    if (*buttons & U_JPAD) {
        arg0->unk2--;
    }
    if (*buttons & D_JPAD) {
        arg0->unk2++;
    }
    if (*buttons & R_JPAD) {
        arg0->unk0++;
    }
    if (*buttons & L_JPAD) {
        arg0->unk0--;
    }

    sprintf(arg0->unk1C, D_800B11F0_1DB790, arg0->unk0 + 7, arg0->unk2 + 7);

    debugEnqueueCallback(0, 7, &renderTextPalette, &arg0->unk4);

    arg0->unk10 = arg0->unk0;
    arg0->unk12 = arg0->unk2;

    debugEnqueueCallback(0, 7, &renderTextPalette, &arg0->unk10);
}

void func_800B0D4C_1DB2EC(func_800B0D4C_arg *arg0) {
    arg0->unk2C = freeNodeMemory(arg0->unk2C);
}

void func_800B0D78_1DB318(func_800B0D4C_arg *arg0) {
    arg0->unk2C = loadCompressedData(&_41AD80_ROM_START, &_41AD80_ROM_END, 0x13FF0);
    setCleanupCallback(func_800B0D4C_1DB2EC);
    setCallback(func_800B0DD0_1DB370);
}

void func_800B0DD0_1DB370(func_800B0DD0_arg *arg0) {
    func_800394BC_3A0BC(arg0, arg0->unk2C);
    setCallback(&func_800B0DF8_1DB398);
}

void func_800B0DF8_1DB398(void *arg0) {
    debugEnqueueCallback(9, 0, func_80038420_39020, arg0);
}

void func_800B0E24_1DB3C4(func_800B0F18_arg *arg0) {
    void *temp;
    getCurrentAllocation();
    temp = loadCompressedData(&_41A1D0_ROM_START, &_41A1D0_ROM_END, 0x1B48);
    arg0->unk0 = -44;
    arg0->unk2 = -20;
    arg0->unk8 = 13;
    arg0->unk4 = temp;
    setCleanupCallback(func_800B0EEC_1DB48C);
    setCallbackWithContinue(func_800B0E94_1DB434);
}

void func_800B0E94_1DB434(void *arg0) {
    GameState *state = getCurrentAllocation();
    if (state->unk59A[0] == 3) {
        debugEnqueueCallback(8, 0, func_8000FED0_10AD0, arg0);
    } else {
        func_80069CF8_6A8F8();
    }
}

void func_800B0EEC_1DB48C(func_800B0FE0_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_800B0F18_1DB4B8(func_800B0F18_arg *arg0) {
    void *temp;
    getCurrentAllocation();
    temp = loadCompressedData(&_41A1D0_ROM_START, &_41A1D0_ROM_END, 0x1B48);
    arg0->unk0 = -76;
    arg0->unk2 = -8;
    arg0->unk8 = 12;
    arg0->unk4 = temp;
    setCleanupCallback(func_800B0FE0_1DB580);
    setCallbackWithContinue(func_800B0F88_1DB528);
}

void func_800B0F88_1DB528(void *arg0) {
    GameState *state = getCurrentAllocation();
    if (state->unk59A[0] == 2) {
        debugEnqueueCallback(8, 0, func_8000FED0_10AD0, arg0);
    } else {
        func_80069CF8_6A8F8();
    }
}

void func_800B0FE0_1DB580(func_800B0FE0_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_800B100C_1DB5AC(func_800B100C_arg *arg0) {
    void *temp1;
    void *temp2;

    temp1 = loadUncompressedData(&_1DC0D0_ROM_START, &_1DC0D0_ROM_END);
    temp2 = loadUncompressedData(&_422C60_ROM_START, &_422C60_ROM_END);
    setCleanupCallback(func_800B1104_1DB6A4);

    memcpy(arg0, identityMatrix, 0x20);
    arg0->unk20 = &D_800B1140_1DB6E0;
    arg0->unk14 = 0x2C0000;
    arg0->unk1C = (s32)0xFF9F0000;
    arg0->unk24 = temp1;
    arg0->unk28 = temp2;
    arg0->unk2C = 0;
    arg0->unk18 = (s32)0xFFF40000;

    createZRotationMatrix((Transform3D *)arg0, 0x1F50);
    setCallback(func_800B10D4_1DB674);
}

void func_800B10D4_1DB674(void *arg0) {
    getCurrentAllocation();
    enqueueDisplayListObjectWithFullRenderState(0, arg0);
}

void func_800B1104_1DB6A4(func_800B1104_arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}
