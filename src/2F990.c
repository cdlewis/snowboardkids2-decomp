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
    s32 *unk2C;
} func_8002FA70_30670_arg;

typedef struct {
    DisplayListObject displayList;
    Transform3D transform;
    s32 translationStep;
    u8 updateCounter;
    s8 unk61;
    s8 unk62;
} func_8002F658_30258_arg;

typedef struct {
    SceneModel *unk0;
    s32 unk4;
    u8 padding[0x1B];
    s16 unk24;
} func_8002EFD8_2FBD8_arg;

typedef struct {
    u8 padding[0x2C];
    void *unk2C;
} func_8002FA1C_3061C_arg;

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
    s16 unkA;
    s8 unkC;
    u8 unkD;
} func_8002FDFC_309FC_arg;

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
    func_800308FC_314FC_arg items[6];
    u8 unk48[0x18];
    char unk60[8];
} func_80030540_31140_arg;

typedef struct {
    void *unk0;
    Transform3D unk4;
    s16 unk24;
    s8 unk26;
} func_8002EF3C_2FB3C_arg;

typedef struct {
    DisplayListObject unk0;
    void *unk3C;
    u8 padding[0x10];
    s32 unk50;
    u8 padding3[0xC];
    u8 unk60;
    s8 unk61;
} func_8002F518_30118_arg;

typedef struct {
    func_800308FC_314FC_arg items[7];
    s16 unk54;
    s16 unk56;
    void *unk58;
    s16 unk5C;
    s8 unk5E;
    char pad5F;
    char unk60[8];
} func_800302AC_30EAC_arg;

typedef struct {
    u8 _pad[0x4];
    void *unk4;
} func_8002FF28_30B28_arg;

typedef struct {
    u8 _pad[0x4];
    void *unk4;
    u8 _pad2[0x30];
    void *unk38;
} func_80030B70_31770_arg;

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

void func_80030378_30F78(func_800302AC_30EAC_arg *);
void func_80030480_31080(func_800302AC_30EAC_arg *arg0);
void func_80030540_31140(func_80030540_31140_arg *arg0);
void func_8002EFD8_2FBD8(void *);
void func_8002F024_2FC24(func_8002EF3C_2FB3C_arg *);
void func_8002F110_2FD10(func_8002EFD8_2FBD8_arg *);
void func_8002F290_2FE90(func_8002F658_30258_arg *);
void func_8002F36C_2FF6C(func_8002F658_30258_arg *);
void func_8002F3E4_2FFE4(func_8002F518_30118_arg *);
void func_8002F518_30118(func_8002F518_30118_arg *);
void func_8002F5C8_301C8(DisplayListObject *);
void func_8002F614_30214(func_8002F658_30258_arg *);
void func_8002F72C_3032C(func_8002F658_30258_arg *);
void func_8002F860_30460(DisplayListObject *);
void func_8002F88C_3048C(func_8002F658_30258_arg *arg0);
void func_8002F948_30548(void);
void func_8002F980_30580(func_8002F658_30258_arg *);
void func_8002FA1C_3061C(func_8002FA1C_3061C_arg *);
void func_8002FA44_30644(void *);
void func_8002FA70_30670(func_8002FA70_30670_arg *);
void func_8002FB40_30740(func_8002FA9C_3069C_arg *);
void func_8002FCA8_308A8(func_8002FF28_30B28_arg *arg0);
void func_8002FDFC_309FC(func_8002FDFC_309FC_arg *);
void func_8002FF28_30B28(func_8002FF28_30B28_arg *);
void func_8003006C_30C6C(func_8002FA9C_3069C_item *);
void func_80030194_30D94(func_8002FF28_30B28_arg *);
void func_80030238_30E38(void *arg0);
void func_80030280_30E80(func_8002FF28_30B28_arg *arg0);
void func_80030764_31364(ItemStatsDisplay *arg0);

void func_80030974_31574(void *);
void func_800309D4_315D4(func_800308FC_314FC_arg *);
void func_80030AEC_316EC(void *);
void func_80030B70_31770(func_80030B70_31770_arg *);
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

void func_8002EF3C_2FB3C(func_8002EF3C_2FB3C_arg *arg0) {
    GameState *temp = (GameState *)getCurrentAllocation();
    arg0->unk0 = createSceneModel(0x3A, temp);
    memcpy(&arg0->unk4, identityMatrix, 0x20);
    arg0->unk4.translation.x = 0x200000;
    arg0->unk4.translation.y = 0xFFE00000;
    arg0->unk4.translation.z = 0x80000;
    arg0->unk24 = 4;
    arg0->unk26 = 0;
    createYRotationMatrix(&arg0->unk4, 0x1E00);
    setCleanupCallback(func_8002F110_2FD10);
    setCallback(func_8002EFD8_2FBD8);
}

void func_8002EFD8_2FBD8(void *untypedArg0) {
    func_8002EFD8_2FBD8_arg *arg0 = (func_8002EFD8_2FBD8_arg *)untypedArg0;
    applyTransformToModel(arg0->unk0, (void *)((s32)arg0 + 4));
    setModelAnimation(arg0->unk0, arg0->unk24);
    updateModelGeometry(arg0->unk0);
    setCallback(&func_8002F024_2FC24);
}

typedef struct {
    u8 padding[0x5D6];
    u8 unk5D6;
} func_8002F024_2FC24_state;

void func_8002F024_2FC24(func_8002EF3C_2FB3C_arg *arg0) {
    func_8002F024_2FC24_state *state;
    u8 animIndex;
    u16 frameCounter;
    s32 idx;
    volatile u8 pad[8];

    state = (func_8002F024_2FC24_state *)getCurrentAllocation();
    applyTransformToModel(arg0->unk0, &arg0->unk4);
    do {
        if (clearModelRotation(arg0->unk0) != 0) {
            animIndex = arg0->unk26;
            idx = animIndex * 2;
            if (animIndex != 0) {
                frameCounter = arg0->unk24 + 1;
                arg0->unk24 = frameCounter;
                if (frameCounter == (u16)(D_8008F0B4_8FCB4[idx] + D_8008F0B6_8FCB6[idx])) {
                    arg0->unk26 = 0;
                    arg0->unk24 = 4;
                }
                setModelAnimation(arg0->unk0, arg0->unk24);
            }
        }
    } while (0);
    updateModelGeometry(arg0->unk0);
    animIndex = state->unk5D6;
    if (animIndex != 0) {
        u16 start;
        arg0->unk26 = animIndex;
        start = D_8008F0B4_8FCB4[state->unk5D6 * 2];
        arg0->unk24 = start;
        setModelAnimation(arg0->unk0, (s16)start);
        state->unk5D6 = 0;
    }
}

void func_8002F110_2FD10(func_8002EFD8_2FBD8_arg *arg0) {
    destroySceneModel(arg0->unk0);
}

void func_8002F12C_2FD2C(func_8002F658_30258_arg *arg0) {
    Transform3D sp10;
    Transform3D sp30;
    s16 temp_s0;
    u16 temp_s1;
    Transform3D *temp_s3;
    GameState *temp_s4;
    Transform3D *sp30_ptr;
    Transform3D *sp10_ptr;

    temp_s3 = &arg0->transform;
    temp_s4 = (GameState *)getCurrentAllocation();
    memcpy(temp_s3, identityMatrix, 0x20);
    sp30_ptr = &sp30;
    memcpy(sp30_ptr, temp_s3, 0x20);
    sp10_ptr = &sp10;
    memcpy(sp10_ptr, sp30_ptr, 0x20);
    createRotationMatrixYX(sp10_ptr, 0x1000, 0x800);
    createZRotationMatrix(sp30_ptr, 0x1F00);
    func_8006B084_6BC84(sp10_ptr, sp30_ptr, temp_s3);

    if (temp_s4->unk5C9 != 1) {
        arg0->transform.translation.x = 0x200000 - ((2 - arg0->unk62) << 21);
    }

    temp_s1 = temp_s4->unk5CA[arg0->unk61];
    memcpy(arg0, temp_s3, 0x20);
    do {
        temp_s0 = temp_s1 & 0x1F;
        temp_s1 = temp_s0;
    } while (0);

    arg0->displayList.unk20 = loadAssetByIndex_95728(temp_s0);
    arg0->displayList.unk24 = loadAssetByIndex_95500(temp_s0);
    arg0->displayList.unk28 = loadAssetByIndex_95590(temp_s0);
    arg0->displayList.unk2C = loadAssetByIndex_95668(temp_s1 / 3);
    arg0->updateCounter = 0;

    setCleanupCallback(func_8002F614_30214);
    setCallback(func_8002F290_2FE90);
}

void func_8002F290_2FE90(func_8002F658_30258_arg *arg0) {
    volatile u8 padding[0x20];
    GameState *state;

    state = (GameState *)getCurrentAllocation();

    if (state->unk5C5 == 2) {
        if (state->unk5C6 == 2) {
            if (arg0->unk62 == 0) {
                arg0->unk62 = 2;
                arg0->transform.translation.x = 0x400000;
                arg0->updateCounter = 1;
                goto end;
            }
        }
        if (state->unk5C6 == 1) {
            if (arg0->unk62 == 2) {
                arg0->transform.translation.x = 0xFFC00000;
                arg0->unk62 = 0;
                arg0->updateCounter = 1;
                goto end;
            }
        }
        if (state->unk5C6 == 2) {
            arg0->unk62 = arg0->unk62 - 1;
        } else {
            arg0->unk62 = arg0->unk62 + 1;
        }
        arg0->updateCounter = 0;

    end:
        setCallback(&func_8002F36C_2FF6C);
    }

    enqueueDisplayListObject(0, &arg0->displayList);
}

void func_8002F36C_2FF6C(func_8002F658_30258_arg *arg0) {
    volatile u8 padding[0x20];

    getCurrentAllocation();
    if (arg0->updateCounter != 0) {
        arg0->displayList.unk24 = freeNodeMemory(arg0->displayList.unk24);
        arg0->displayList.unk28 = freeNodeMemory(arg0->displayList.unk28);
        arg0->displayList.unk2C = freeNodeMemory(arg0->displayList.unk2C);
        if (arg0->updateCounter != 0) {
            goto end;
        }
    }
    enqueueDisplayListObject(0, &arg0->displayList);
end:
    setCallback(&func_8002F3E4_2FFE4);
}

void func_8002F3E4_2FFE4(func_8002F518_30118_arg *arg0) {
    volatile u8 padding[0x20];
    GameState *state = (GameState *)getCurrentAllocation();
    s8 s0;
    s8 s1;

    if (arg0->unk60 == 1) {
        if (state->unk5C6 == 2) {
            arg0->unk61 = state->unk5C8 + 1;
            if (arg0->unk61 == state->unk5C9) {
                arg0->unk61 = 0;
            }
        } else {
            arg0->unk61 = state->unk5C8 - 1;
            if (arg0->unk61 < 0) {
                arg0->unk61 = state->unk5C9 - 1;
            }
        }

        s0 = state->unk5CA[arg0->unk61] & 0x1F;
        s1 = s0;

        memcpy(arg0, &arg0->unk3C, 0x20);

        arg0->unk0.unk20 = loadAssetByIndex_95728(s1);
        arg0->unk0.unk24 = loadAssetByIndex_95500(s1);
        arg0->unk0.unk28 = loadAssetByIndex_95590(s1);
        arg0->unk0.unk2C = loadAssetByIndex_95668(s0 / 3);
        arg0->unk60 = 0;
    } else {
        enqueueDisplayListObject(0, (DisplayListObject *)arg0);
    }

    setCallback(&func_8002F518_30118);
}

void func_8002F518_30118(func_8002F518_30118_arg *s0) {
    volatile u8 padding[0x20];
    u32 new_var;
    GameState *s1 = (GameState *)getCurrentAllocation();
    u32 offset;
    new_var = s1->unk5C6;

    if (new_var == 2) {
        offset = 0xFFF80000;
    } else {
        offset = 0x00080000;
    }

    s0->unk50 += offset;

    memcpy(s0, (void *)((s32)s0 + 0x3C), 0x20);

    s0->unk60++;
    if (s0->unk60 == 4) {
        s0->unk60 = 0;
        s1->unk5C7++;
        setCallback(func_8002F5C8_301C8);
    }

    enqueueDisplayListObject(0, &s0->unk0);
}

void func_8002F5C8_301C8(DisplayListObject *arg0) {
    volatile u8 pad[0x20];
    if (((GameState *)getCurrentAllocation())->unk5C5 == 1) {
        setCallback(&func_8002F290_2FE90);
    }
    enqueueDisplayListObject(0, arg0);
}

void func_8002F614_30214(func_8002F658_30258_arg *arg0) {
    arg0->displayList.unk24 = freeNodeMemory(arg0->displayList.unk24);
    arg0->displayList.unk28 = freeNodeMemory(arg0->displayList.unk28);
    arg0->displayList.unk2C = freeNodeMemory(arg0->displayList.unk2C);
}

void func_8002F658_30258(func_8002F658_30258_arg *arg0) {
    Transform3D sp10;
    Transform3D sp30;
    Transform3D *new_var;
    Transform3D *matrix;

    getCurrentAllocation();
    new_var = &sp10;
    matrix = &arg0->transform;
    memcpy(matrix, &identityMatrix, sizeof(Transform3D));
    memcpy(&sp30, matrix, sizeof(Transform3D));
    memcpy(new_var, &sp30, sizeof(Transform3D));
    createRotationMatrixYX(&sp10, 0x1000, 0x800);
    createZRotationMatrix(&sp30, 0x1F00);
    func_8006B084_6BC84(&sp10, &sp30, matrix);
    arg0->updateCounter = 0;
    arg0->displayList.unk20 = NULL;
    arg0->displayList.unk24 = NULL;
    arg0->displayList.unk28 = NULL;
    arg0->displayList.unk2C = 0;
    setCleanupCallback(&func_8002F980_30580);
    setCallback(&func_8002F72C_3032C);
}

void func_8002F72C_3032C(func_8002F658_30258_arg *arg0) {
    GameState *state;
    s8 temp_v1;
    u8 temp_s1;
    s16 s0;

    state = (GameState *)getCurrentAllocation();
    if (state->unk5C5 == 2) {
        if (state->unk5C6 == 1) {
            arg0->transform.translation.x = 0x200000;
            arg0->translationStep = 0x80000;
            temp_v1 = state->unk5C8 + 2;
            arg0->unk61 = temp_v1;
            if (temp_v1 >= (s32)state->unk5C9) {
                arg0->unk61 = temp_v1 - state->unk5C9;
            }
        } else {
            arg0->transform.translation.x = 0xFFE00000;
            arg0->translationStep = 0xFFF80000;
            temp_v1 = state->unk5C8 - 2;
            arg0->unk61 = temp_v1;
            if (temp_v1 < 0) {
                arg0->unk61 = state->unk5C9 + temp_v1;
            }
        }
        temp_s1 = state->unk5CA[arg0->unk61];
        memcpy(arg0, &arg0->transform, 0x20);
        temp_s1 &= 0x1F;
        s0 = temp_s1;
        arg0->displayList.unk20 = loadAssetByIndex_95728(s0);
        arg0->displayList.unk24 = loadAssetByIndex_95500(s0);
        arg0->displayList.unk28 = loadAssetByIndex_95590(s0);
        arg0->displayList.unk2C = loadAssetByIndex_95668(temp_s1 / 3);
        setCallback(func_8002F860_30460);
    }
}

void func_8002F860_30460(DisplayListObject *arg0) {
    enqueueDisplayListObject(0, arg0);
    setCallback(func_8002F88C_3048C);
}

void func_8002F88C_3048C(func_8002F658_30258_arg *arg0) {
    GameState *state = (GameState *)getCurrentAllocation();

    arg0->transform.translation.x += arg0->translationStep;
    memcpy(arg0, &arg0->transform, sizeof(Transform3D));

    arg0->updateCounter++;
    if (arg0->updateCounter == 4) {
        state->unk5C7++;
        arg0->updateCounter = 0;
        arg0->displayList.unk24 = freeNodeMemory(arg0->displayList.unk24);
        arg0->displayList.unk28 = freeNodeMemory(arg0->displayList.unk28);
        arg0->displayList.unk2C = freeNodeMemory(arg0->displayList.unk2C);
        setCallbackWithContinue(func_8002F948_30548);
    } else {
        enqueueDisplayListObject(0, &arg0->displayList);
    }
}

void func_8002F948_30548(void) {
    GameState *state = (GameState *)getCurrentAllocation();
    if (state->unk5C5 != 2) {
        setCallbackWithContinue(func_8002F72C_3032C);
    }
}

void func_8002F980_30580(func_8002F658_30258_arg *arg0) {
    arg0->displayList.unk24 = freeNodeMemory(arg0->displayList.unk24);
    arg0->displayList.unk28 = freeNodeMemory(arg0->displayList.unk28);
    arg0->displayList.unk2C = freeNodeMemory(arg0->displayList.unk2C);
}

void func_8002F9C4_305C4(func_8002FA70_30670_arg *arg0) {
    arg0->unk2C = loadCompressedData(&_42F1D0_ROM_START, &_42F1D0_ROM_END, 0x14410);
    setCleanupCallback(&func_8002FA70_30670);
    setCallback(&func_8002FA1C_3061C);
}

void func_8002FA1C_3061C(func_8002FA1C_3061C_arg *arg0) {
    func_800394BC_3A0BC(arg0, (s32)arg0->unk2C);
    setCallback(&func_8002FA44_30644);
}

void func_8002FA44_30644(void *arg0) {
    debugEnqueueCallback(1, 0, func_80038420_39020, arg0);
}

void func_8002FA70_30670(func_8002FA70_30670_arg *arg0) {
    arg0->unk2C = freeNodeMemory(arg0->unk2C);
}

void func_8002FA9C_3069C(func_8002FA9C_3069C_arg *arg0) {
    s32 i;
    void *asset = loadCompressedData(&_4237C0_ROM_START, &_4237C0_ROM_END, 0x8A08);

    setCleanupCallback(&func_8002FCA8_308A8);

    for (i = 0; i < 2; i++) {
        arg0->items[i].unk0 = -0x40 + i * 0x60;
        arg0->items[i].unk2 = -0x18;
        arg0->items[i].unk8 = i;
        arg0->items[i].unk4 = asset;
        arg0->items[i].unkA = 0xFF;
        arg0->items[i].unkD = 0;
        arg0->items[i].unkC = 0;
    }

    arg0->unk20 = 0;

    setCallback(&func_8002FB40_30740);
}

void func_8002FB40_30740(func_8002FA9C_3069C_arg *arg0) {
    GameState *state = getCurrentAllocation();
    s32 i;

    if (state->unk5C5 > 0 && state->unk5C5 < 4) {
        if (state->unk5C5 == 1) {
            arg0->unk20++;
            if (state->unk5C9 >= 3) {
                if ((u8)(arg0->unk20) < 0x11) {
                    arg0->items[0].unkA -= 8;
                    arg0->items[1].unkA -= 8;
                } else {
                    arg0->items[0].unkA += 8;
                    arg0->items[1].unkA += 8;
                }
            } else if (state->unk5C9 == 2) {
                if ((u8)(arg0->unk20) < 0x11) {
                    if (state->unk5C8 == 1) {
                        arg0->items[1].unkA = 0xFF;
                        arg0->items[0].unkA -= 8;
                    } else {
                        arg0->items[0].unkA = 0xFF;
                        arg0->items[1].unkA -= 8;
                    }
                } else {
                    if (state->unk5C8 == 1) {
                        arg0->items[1].unkA = 0xFF;
                        arg0->items[0].unkA += 8;
                    } else {
                        arg0->items[0].unkA = 0xFF;
                        arg0->items[1].unkA += 8;
                    }
                }
            } else {
                arg0->unk20 = 0;
                arg0->items[0].unkA = 0xFF;
                arg0->items[1].unkA = 0xFF;
            }
        } else {
            arg0->unk20 = 0;
            arg0->items[0].unkA = 0xFF;
            arg0->items[1].unkA = 0xFF;
        }

        arg0->unk20 &= 0x1F;

        for (i = 0; i < 2; i++) {
            debugEnqueueCallback(8, 0, &func_80012004_12C04, &arg0->items[i]);
        }
    }
}

void func_8002FCA8_308A8(func_8002FF28_30B28_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_8002FCD4_308D4(func_8002FDFC_309FC_arg *arg0) {
    void *dmaResult;
    GameStateSub *state;
    u8 itemValue;

    state = getCurrentAllocation();
    dmaResult = loadCompressedData(&_4237C0_ROM_START, &_4237C0_ROM_END, 0x8A08);
    setCleanupCallback(func_8002FF28_30B28);

    arg0->unk2 = -0x18;

    itemValue = state->unk5CA[state->unk5C8];

    if (itemValue < 9) {
        u8 tempValue;
        arg0->unk0 = -0x30;
        tempValue = state->unk5CA[state->unk5C8];
        arg0->unk8 = (tempValue / 3) + 0x1D;
    } else {
        s16 tableVal = D_8008F0B2_8FCB2[itemValue] + 0x18;
        s16 tableVal2 = D_8008F0C6_8FCC6[itemValue];

        arg0->unk8 = itemValue + 0x23;
        arg0->unk0 = ((0x120 - tableVal) / 2) - tableVal2 - 0x96;
    }

    arg0->unkA = 0xFF;
    arg0->unkC = 0;
    arg0->unkD = 0;
    arg0->unk4 = dmaResult;

    setCallback(func_8002FDFC_309FC);
}

void func_8002FDFC_309FC(func_8002FDFC_309FC_arg *arg0) {
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
                arg0->unk0 = -0x30;
                arg0->unk8 = (masked / 3) + 0x1D;
            } else {
                s16 tableVal = D_8008F0B2_8FCB2[masked];
                s16 tableVal2 = D_8008F0C6_8FCC6[masked];
                arg0->unk8 = masked + 0x23;
                arg0->unk0 = (((0x120 - ((s16)(tableVal + 0x18))) / 2) - tableVal2) - 0x96;
            }
            if (state->unk5C5 == 3) {
                if (state->unk5C0 & 1) {
                    arg0->unkD = 0xFF;
                } else {
                    arg0->unkD = 0;
                }
            }
            itemValue = 0;
            debugEnqueueCallback(8, itemValue, &func_80012004_12C04, arg0);
        }
    }
}

void func_8002FF28_30B28(func_8002FF28_30B28_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_8002FF54_30B54(func_8002FA9C_3069C_item *arg0) {
    GameState *state;
    GameState *temp_s0;
    void *dmaResult;
    u8 itemValue;

    state = (GameState *)getCurrentAllocation();
    dmaResult = loadCompressedData(&_4237C0_ROM_START, &_4237C0_ROM_END, 0x8A08);
    setCleanupCallback(func_80030194_30D94);

    arg0->unk2 = -0x18;

    temp_s0 = (GameState *)((u8 *)state + state->unk5C8);
    itemValue = temp_s0->unk5CA[0];

    if (itemValue < 9) {
        arg0->unk0 = 0x12;
        arg0->unk8 = ((itemValue % 3) & 0xFF) + 0x24;
    } else {
        s16 tableVal = D_8008F0B2_8FCB2[itemValue];
        arg0->unk8 = 0x35;
        arg0->unk0 = tableVal + ((0x120 - (s16)(tableVal + 0x18)) / 2) - 0x96;
    }

    arg0->unkA = 0xFF;
    arg0->unkC = 0;
    arg0->unkD = 0;
    arg0->unk4 = dmaResult;

    setCallback(func_8003006C_30C6C);
}

void func_8003006C_30C6C(func_8002FA9C_3069C_item *arg0) {
    GameState *state = (GameState *)getCurrentAllocation();
    u8 itemValue;

    itemValue = state->unk5CA[state->unk5C8];

    if (itemValue >= 0x80) {
        return;
    }

    if (state->unk5C5 != 0 && state->unk5C5 != 2) {
        if (itemValue < 9) {
            arg0->unk0 = 0x12;
            arg0->unk8 = ((itemValue & 0x1F) % 3) + 0x24;
        } else {
            s16 tableVal = D_8008F0B2_8FCB2[itemValue];
            arg0->unk8 = 0x35;
            arg0->unk0 = tableVal + ((0x120 - (s16)(tableVal + 0x18)) / 2) - 0x96;
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

void func_80030194_30D94(func_8002FF28_30B28_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_800301C0_30DC0(func_800308FC_314FC_arg *arg0) {
    void *temp_s1 = loadCompressedData(&_41A1D0_ROM_START, &_41A1D0_ROM_END, 0x1B48);

    setCleanupCallback(&func_80030280_30E80);

    arg0->unk0 = -0x2C;
    arg0->unk2 = -0x14;
    arg0->unk8 = 0xD;
    arg0->unk4 = temp_s1;

    setCallback(&func_80030238_30E38);
}

void func_80030238_30E38(void *arg0) {
    GameState *allocation = getCurrentAllocation();

    if (allocation->unk5C5 == 4) {
        debugEnqueueCallback(8, 7, &func_8000FED0_10AD0, arg0);
    }
}

void func_80030280_30E80(func_8002FF28_30B28_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_800302AC_30EAC(func_800302AC_30EAC_arg *arg0) {
    void *asset2;
    void *asset1;
    s32 i;
    s32 *new_var;

    asset1 = loadCompressedData(&_3F6950_ROM_START, &_3F6950_ROM_END, 0x508);
    asset2 = loadCompressedData(&_3F6670_ROM_START, &_3F6670_ROM_END, 0x388);
    setCleanupCallback(&func_80030480_31080);
    for (i = 0; i < 7; i++) {
        arg0->items[i].unk0 = 0x48 + (i * 8);
        arg0->items[i].unk2 = 0x58;
        arg0->items[i].unk4 = asset1;
    }

    arg0->unk54 = 0x38;
    arg0->unk56 = 0x58;
    arg0->unk5C = 0;
    arg0->unk5E = 0;
    arg0->unk58 = asset2;
    setCallback(&func_80030378_30F78);
}

void func_80030378_30F78(func_800302AC_30EAC_arg *arg0) {
    s32 i;
    func_800308FC_314FC_arg *item;
    s8 value;
    s32 space;

    if (*D_800AFE8C_A71FC < 100) {
        value = 1;
        i = 6;
        do {
            arg0->items[i].unkA = value;
        } while (--i >= 0);
    } else {
        value = 2;
        i = 6;
        do {
            arg0->items[i].unkA = value;
        } while (--i >= 0);
    }

    sprintf(arg0->unk60, "%7d", *D_800AFE8C_A71FC);

    i = 0;
    space = ' ';
    item = &arg0->items[0];
    do {
        char c = arg0->unk60[i];
        if (c != space) {
            item->unk8 = c - '0';
            debugEnqueueCallback(8, 0, &func_80010240_10E40, item);
        }
        item++;
    } while (++i < 7);

    debugEnqueueCallback(8, 0, &func_80010240_10E40, &arg0->unk54);
}

void func_80030480_31080(func_800302AC_30EAC_arg *arg0) {
    arg0->items[0].unk4 = freeNodeMemory(arg0->items[0].unk4);
    arg0->unk58 = freeNodeMemory(arg0->unk58);
}

void func_80030668_31268(func_80030668_31268_arg *);

void func_800304B8_310B8(func_800308FC_314FC_arg *arg0) {
    void *temp_s1;
    s32 i;
    s32 temp_a1;
    s16 temp_v1;

    temp_s1 = loadCompressedData(&_3F6950_ROM_START, &_3F6BB0_ROM_START, 0x508);
    setCleanupCallback(func_80030668_31268);

    i = 0;
    temp_a1 = 0x38;
    temp_v1 = -0x1C;

    do {
        arg0[i].unk0 = temp_v1;
        arg0[i].unk2 = temp_a1;
        arg0[i].unk4 = temp_s1;
        i++;
        temp_v1 += 8;
    } while (i < 6);

    setCallback(func_80030540_31140);
}

extern s32 D_8008F070_8FC70[];

void func_80030540_31140(func_80030540_31140_arg *arg0) {
    GameState *state = (GameState *)getCurrentAllocation();
    s32 i;
    u8 itemValue;
    s32 tableValue;
    func_800308FC_314FC_arg *item;
    s8 value;
    s32 space;

    if (state->unk5C5 == 0x14) {
        return;
    }

    itemValue = state->unk5CA[state->unk5C8];

    if (itemValue >= 0x80) {
        return;
    }

    tableValue = D_8008F070_8FC70[itemValue & 0x1F];

    value = 1;
    if (*D_800AFE8C_A71FC < tableValue) {
        i = 5;
        do {
            arg0->items[i].unkA = value;
        } while (--i >= 0);
    } else {
        value = 2;
        i = 5;
        do {
            arg0->items[i].unkA = value;
        } while (--i >= 0);
    }

    sprintf(arg0->unk60, "%6d", tableValue);

    i = 0;
    space = ' ';
    item = &arg0->items[0];
    do {
        char c = arg0->unk60[i];
        if (c != space) {
            item->unk8 = c - '0';
            debugEnqueueCallback(8, 0, &func_80010240_10E40, item);
        }
        item++;
    } while (++i < 6);
}

void func_80030668_31268(func_80030668_31268_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_80030694_31294(ItemStatsDisplay *display) {
    void *spriteAsset;
    s32 i;

    getCurrentAllocation();
    display->progressBarAsset = loadAsset_34F7E0();
    spriteAsset = loadCompressedData(&_419C60_ROM_START, &_419C60_ROM_END, 0x1548);
    setCleanupCallback(func_800308C4_314C4);

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

    setCallback(func_80030764_31364);
}

extern void func_8006D7B0_6E3B0(void *, s16, s16, s32, s32, s32, s32, s32, s32, s32);

void func_80030764_31364(ItemStatsDisplay *arg0) {
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
                sprintf(arg0->statBuffers[2], formatStr, func_80027C7C_2887C(isValidItem) & 0xFF);

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

void func_800308C4_314C4(ItemStatsDisplay *arg0) {
    arg0->progressBarAsset = freeNodeMemory(arg0->progressBarAsset);
    arg0->spriteAsset = freeNodeMemory(arg0->spriteAsset);
}

void func_800308FC_314FC(func_800308FC_314FC_arg *arg0) {
    void *temp_s1 = loadCompressedData(&_419C60_ROM_START, &_419C60_ROM_END, 0x1548);

    setCleanupCallback(&func_800309D4_315D4);

    arg0->unk0 = -0x2C;
    arg0->unk2 = -0x18;
    arg0->unk4 = temp_s1;
    arg0->unk8 = 5;

    setCallback(&func_80030974_31574);
}

void func_80030974_31574(void *arg0) {
    GameState *state = (GameState *)getCurrentAllocation();
    s8 index = state->unk5C8;

    if (state->unk5CA[index] >= 0x80 || state->unk5C9 == 0) {
        debugEnqueueCallback(8, 1, func_8000FED0_10AD0, arg0);
    }
}

void func_800309D4_315D4(func_800308FC_314FC_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_80030A00_31600(func_80030A00_31600_arg *arg0) {
    void *temp_s1;
    void *temp_s2;
    s32 i;

    temp_s1 = loadCompressedData(&_419C60_ROM_START, &_419C60_ROM_END, 0x1548);
    temp_s2 = func_80035F80_36B80(1);
    setCleanupCallback(&func_80030B70_31770);

    for (i = 0; i < 4; i++) {
        arg0->items[i].unk0 = (i & 1) * 0x80 - 0x80;
        arg0->items[i].unk2 = (i / 2) * 0x10 - 0x66;
        arg0->items[i].unk4 = temp_s1;
        arg0->items[i].unk8 = i;
    }

    arg0->unk30 = -0x68;
    arg0->unk32 = -0x60;
    arg0->unk34 = &D_8008F110_8FD10;
    arg0->unk38 = temp_s2;
    arg0->unk3C = 0xFF;
    arg0->unk3E = 0xFF;
    arg0->unk40 = 5;

    setCallback(&func_80030AEC_316EC);
}

void func_80030AEC_316EC(void *untypedArg0) {
    func_80030A00_31600_arg *arg0 = (func_80030A00_31600_arg *)untypedArg0;
    GameState *alloc;
    s32 i;

    alloc = (GameState *)getCurrentAllocation();

    if (alloc->unk5D8 != 0) {
        for (i = 0; i < 4; i++) {
            debugEnqueueCallback(8, 0, &func_8000FED0_10AD0, &arg0->items[i]);
            debugEnqueueCallback(8, 1, &func_80035408_36008, &arg0->unk30);
        }
    }
}

void func_80030B70_31770(func_80030B70_31770_arg *arg0) {
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
