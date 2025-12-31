#include "202A0.h"
#include "10AD0.h"
#include "20F0.h"
#include "33FE0.h"
#include "36B80.h"
#include "38C90.h"
#include "3E160.h"
#include "56910.h"
#include "6E840.h"
#include "B040.h"
#include "EepromSaveData_type.h"
#include "common.h"
#include "displaylist.h"
#include "geometry.h"
#include "overlay.h"
#include "task_scheduler.h"

extern u8 identityMatrix[];
extern void func_8001FFE4_20BE4(void);
extern void func_8001FA00_20600(void);
extern void func_8006D7B0_6E3B0(void *, s32, s32, s32, s32, s32, s32, s32, s32, s32);

USE_ASSET(_458E30);
USE_ASSET(_43A000);
USE_ASSET(_43F050);
USE_ASSET(_459310);
USE_ASSET(_41A1D0);

typedef struct {
    u8 _pad0[0xB2F];
    u8 menuState;
    u8 _padB30[0x15];
    u8 unkB45;
    u8 unkB46;
    u8 unkB47;
} Allocation_202A0;

typedef struct {
    SceneModel *unk0;
    Transform3D unk4;
    s16 unk20;
    s16 unk22;
} Func8002144CArg;

typedef struct {
    u16 unk0;
    u16 unk2;
    u8 _pad4[0x28];
    void *unk2C;
} Func800216ACArg;

typedef struct {
    void *unk0;
    void *unk4;
    u8 _pad8[0x2C];
    void *unk34;
    u8 _pad38[0x1C];
    void *unk54;
} Func80021B88Arg;

typedef struct {
    u8 data[0x10];
} Func80021A20SubStruct;

typedef struct {
    u8 _pad0[0xA];
    s16 unkA;
    u8 _padC[0xE];
    s16 unk1A;
    u8 _pad1C[0x10];
    u8 unk2C[0x14];
    u8 unk40[0x14];
    void *unk54;
    s16 unk58;
    u8 _pad5A[0x6];
    u8 unk60;
} Func80021A20Arg;

typedef struct {
    u8 _pad0[0x2C];
    void *unk2C;
    s16 unk30;
    s16 unk32;
    void *unk34;
    u16 unk38;
} Func80021238Arg;

typedef struct {
    u8 _pad0[0x8];
    void *unk8;
} Func80021880Arg;

typedef struct {
    u8 high;
    u8 low;
} Func80021810Bytes;

typedef union {
    s16 asS16;
    Func80021810Bytes asBytes;
} Func80021810S16OrBytes;

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    void *unk8;
    Func80021810S16OrBytes unkC;
    Func80021810S16OrBytes unkE;
    u8 unk10;
} Func80021810Arg;

typedef struct {
    void *unk0;
    void *unk4;
    u8 _pad8[0x98];
    void *unkA0;
} Func80020FA4Arg;

typedef struct {
    u8 _pad0[0xF8];
    void *portraitAsset;
} LevelPreviewPortraitState;

typedef struct {
    u8 _pad0[0x48A];
    u16 unk48A;
} Allocation_8001FEB4;

typedef struct {
    u8 _pad0[0x3B0];  // 0x000-0x3AF
    u8 unk3B0[0x77C]; // 0x3B0-0xB2B
    s8 unkB2C;        // 0xB2C
    u8 _padB2D[0x6];  // 0xB2D-0xB32
    u8 unkB33[12];    // 0xB33
} Allocation_80020418;

typedef struct {
    s32 unk0;        // 0x00
    u8 _pad4[0x4];   // 0x04
    s32 unk8;        // 0x08
    u8 _padC[0xC];   // 0x0C
    u8 unk18[0x3A];  // 0x18
    u16 unk52;       // 0x52
    u8 _pad54[0x2];  // 0x54
    u16 unk56;       // 0x56
    u8 _pad58[0x2];  // 0x58
    u16 unk5A;       // 0x5A
    u8 _pad5C[0x16]; // 0x5C
    u16 unk72;       // 0x72
    u16 unk74;       // 0x74
    u8 unk76;        // 0x76
} Func80020418Arg;

typedef struct {
    u8 data[0x1C];
    volatile s32 unk1C;
} Mat2WithTemp;

extern u8 D_8008D9F0_8E5F0[];

void cleanupLevelPreviewCharacter(LevelPreviewCharacterState *arg0);
void setupLevelPreviewCamera(LevelPreviewCharacterState *arg0);

void initLevelPreviewCharacter(LevelPreviewCharacterState *arg0) {
    Allocation_80020418 *allocation;
    s8 temp;
    u8 charIndex;
    u8 value;
    u8 value2;

    allocation = (Allocation_80020418 *)getCurrentAllocation();

    // Read character index from allocation
    temp = allocation->unkB2C;
    charIndex = allocation->unkB33[temp];
    value = D_8008D9F0_8E5F0[charIndex];

    // Initialize arg0 structure
    arg0->posZ = 0;
    arg0->posY = 0;
    arg0->posX = 0;
    arg0->currentRotation = 0;
    arg0->startWaypoint = value;

    arg0->sceneModel = createSceneModel(6, allocation->unk3B0);

    // Read character index again
    temp = allocation->unkB2C;
    charIndex = allocation->unkB33[temp];
    value2 = D_8008D9F0_8E5F0[charIndex];

    arg0->cameraHorzOffset = 0x2E000;
    arg0->cameraDistance = 0x800000;
    arg0->targetZ = 0;
    arg0->targetY = 0;
    arg0->targetX = 0;
    arg0->animationPhase = 0;
    arg0->extraRotation = 0;
    arg0->heightOffset = 0xA00000;
    arg0->altHeightOffset = 0xA00000;
    arg0->currentWaypoint = value2;

    // Read character index again for conditional
    temp = allocation->unkB2C;
    charIndex = allocation->unkB33[temp];

    if (charIndex == 9) {
        arg0->heightOffset += 0xFFB00000;
    } else {
        if (charIndex == 5) {
            arg0->extraRotation = 0x1E0;
        }
    }
    arg0->frameTimer = 0;

    // Read character index one more time
    temp = allocation->unkB2C;
    charIndex = allocation->unkB33[temp];

    *(void **)arg0->gameData = func_80055D34_56934(charIndex);

    setCleanupCallback(&cleanupLevelPreviewCharacter);
    setCallback(&setupLevelPreviewCamera);
}

typedef struct {
    u8 _pad0[0x48A];
    u16 unk48A;
    u8 _pad48C[0x6A0];
    s8 unkB2C;
    u8 _padB2D[0x6];
    u8 unkB33[12];
} Allocation_F7C8;

void setupLevelPreviewCamera(LevelPreviewCharacterState *state) {
    Allocation_F7C8 *allocation;
    Transform3D cameraTransform;
    Transform3D offsetTransform;
    Transform3D lookAtTransform;
    s32 waypointStart[4];
    s32 waypointEnd[4];
    s32 scaled;
    u16 angle;

    allocation = (Allocation_F7C8 *)getCurrentAllocation();
    parseGameDataLayout((GameDataLayout *)state->gameData);

    func_80062B1C_6371C(state->gameData, state->startWaypoint, waypointStart, waypointEnd);

    memcpy(&state->transform, identityMatrix, sizeof(Transform3D));
    memcpy(state, waypointEnd, 0xC);

    state->posY = func_80061A64_62664(state->gameData, state->startWaypoint, state);

    if (allocation->unkB33[allocation->unkB2C] == 5) {
        state->posY = state->posY + state->altHeightOffset;
    } else {
        state->posY = state->posY + state->heightOffset;
    }

    memcpy(&state->transform.translation, state, 0xC);

    createYRotationMatrix(&state->transform, state->currentRotation);
    applyTransformToModel(state->sceneModel, &state->transform);
    setModelAnimation(state->sceneModel, 0x90);

    angle = (func_8006D21C_6DE1C(waypointStart[0], waypointStart[2], waypointEnd[0], waypointEnd[2]) - 0x1000) & 0x1FFF;
    state->currentRotation = angle;
    state->targetRotation = angle;
    state->turnDirection = 0;

    scaled = approximateSin((s16)state->currentRotation) * 5 << 12;
    if (scaled < 0) {
        scaled += 0x1FFF;
    }
    state->targetX = (scaled >> 13) << 8;

    scaled = approximateCos((s16)state->currentRotation) * 5 << 12;
    if (scaled < 0) {
        scaled += 0x1FFF;
    }
    state->targetZ = (scaled >> 13) << 8;

    state->targetX = state->targetX + state->posX;
    state->targetZ = state->targetZ + state->posZ;

    angle = func_80060A3C_6163C(state->gameData, state->currentWaypoint, &state->targetX);
    state->currentWaypoint = angle;

    state->targetY = func_80061A64_62664(state->gameData, angle, &state->targetX);
    state->targetY = state->targetY + state->heightOffset;

    computeLookAtMatrix(&state->targetX, state, &lookAtTransform);

    memcpy(&offsetTransform, identityMatrix, 0x20);
    offsetTransform.translation.z = state->cameraDistance;

    func_8006B084_6BC84(&offsetTransform, &lookAtTransform, &cameraTransform);
    func_8006FD3C_7093C(allocation->unk48A, &cameraTransform);
    setCallback(func_8001FA00_20600);
}

INCLUDE_ASM("asm/nonmatchings/202A0", func_8001FA00_20600);

void updateLevelPreviewCamera(LevelPreviewCharacterState *state);

void holdLevelPreviewCamera(LevelPreviewCharacterState *state) {
    getCurrentAllocation();

    state->frameTimer++;
    if (state->frameTimer == 150) {
        state->frameTimer = 0;
        setCallback(&updateLevelPreviewCamera);
    }
}

void updateLevelPreviewCamera(LevelPreviewCharacterState *state) {
    Allocation_8001FEB4 *allocation;
    u8 cameraTransform[0x20];
    Mat2WithTemp offsetTransform;
    u8 lookAtTransform[0x20];
    s32 waypointStart[4];
    s32 waypointEnd[4];
    u16 waypoint;
    u16 rotation;
    void *gameData;

    allocation = (Allocation_8001FEB4 *)getCurrentAllocation();
    memcpy(cameraTransform, identityMatrix, 0x20);

    gameData = state->gameData;
    waypoint = func_80060A3C_6163C(gameData, state->startWaypoint, state);
    state->startWaypoint = waypoint;

    func_80062B1C_6371C(gameData, waypoint, waypointStart, waypointEnd);

    state->targetRotation =
        (func_8006D21C_6DE1C(waypointEnd[0], waypointEnd[2], state->posX, state->posZ) - 0x1000) & 0x1FFF;

    rotation = (state->currentRotation + 0x1000) & 0x1FFF;
    state->currentRotation = rotation;

    if (((state->targetRotation - rotation) & 0x1FFF) >= 0x1001) {
        state->turnDirection = 1;
    } else {
        state->turnDirection = 0;
    }

    computeLookAtMatrix(&state->targetX, state, lookAtTransform);

    memcpy(&offsetTransform, identityMatrix, 0x20);

    offsetTransform.unk1C = state->cameraDistance;

    func_8006B084_6BC84(&offsetTransform, lookAtTransform, cameraTransform);

    func_8006FD3C_7093C(allocation->unk48A, cameraTransform);

    setCallback(&func_8001FFE4_20BE4);
}

INCLUDE_ASM("asm/nonmatchings/202A0", func_8001FFE4_20BE4);

void resumeLevelPreviewAfterHold(Func80020418Arg *arg0) {
    Allocation_80020418 *allocation;
    u8 mat1[0x20];
    s32 pos1[4];
    s32 pos2[4];
    u16 temp;
    void *unk18;

    allocation = (Allocation_80020418 *)getCurrentAllocation();
    arg0->unk72++;

    if (arg0->unk72 == 0x5A) {
        arg0->unk72 = 0;
        memcpy(mat1, identityMatrix, 0x20);

        unk18 = &arg0->unk18;
        temp = func_80060A3C_6163C(unk18, arg0->unk52, arg0);
        arg0->unk52 = temp;

        func_80062B1C_6371C(unk18, temp, pos1, pos2);

        arg0->unk5A = (func_8006D21C_6DE1C(pos1[0], pos1[2], arg0->unk0, arg0->unk8) - 0x1000) & 0x1FFF;

        arg0->unk56 = (arg0->unk56 + 0x1000) & 0x1FFF;

        arg0->unk76 = 0;
        if (((arg0->unk5A - arg0->unk56) & 0x1FFF) >= 0x1001) {
            arg0->unk76 = 1;
        }

        setCallback(&func_8001FA00_20600);

        if (allocation->unkB33[allocation->unkB2C] == 5) {
            arg0->unk74 = 0x1E0;
        }
    }
}

void cleanupLevelPreviewCharacter(LevelPreviewCharacterState *state) {
    *(void **)state->gameData = freeNodeMemory(*(void **)state->gameData);
    destroySceneModel(state->sceneModel);
}

s32 sampleMaxSurroundingTerrainHeight(LevelPreviewCharacterState *state) {
    Allocation_80020418 *allocation;
    s32 samplePos[4];
    s32 i;
    void *gameData;
    s32 sampleRadius;
    s32 maxHeight;
    u16 sampleAngle;
    s32 scaled;
    s32 terrainHeight;

    allocation = (Allocation_80020418 *)getCurrentAllocation();
    maxHeight = state->targetY;
    sampleAngle = (u16)((state->animationPhase - 0x400) & 0x1FFF);
    sampleRadius = 0x500;

    if (allocation->unkB33[allocation->unkB2C] == 9) {
        sampleRadius = 0x5000;
    }

    i = 0;
    gameData = state->gameData;

    do {
        sampleAngle = (sampleAngle + 0x800) & 0x1FFF;

        scaled = approximateSin((s16)sampleAngle) * sampleRadius;
        if (scaled < 0) {
            scaled += 0x1FFF;
        }
        samplePos[0] = (scaled >> 13) << 8;

        scaled = approximateCos((s16)sampleAngle) * sampleRadius;
        if (scaled < 0) {
            scaled += 0x1FFF;
        }
        samplePos[2] = (scaled >> 13) << 8;

        samplePos[0] += state->targetX;
        samplePos[2] += state->targetZ;

        terrainHeight = func_80061A64_62664(
            gameData,
            func_80060A3C_6163C(gameData, state->currentWaypoint, samplePos) & 0xFFFF,
            samplePos
        );
        if (maxHeight < terrainHeight) {
            maxHeight = terrainHeight;
        }
        i++;
    } while (i < 4);

    return maxHeight;
}

void func_80020708_21308(void);
void cleanupLevelPreviewPortraits(LevelPreviewPortraitState *state);

void initLevelPreviewPortraits(LevelPreviewPortraitState *state) {
    s32 pad[4];
    state->portraitAsset = loadCompressedData(&_43A000_ROM_START, &_43A000_ROM_END, 0xB198);
    setCleanupCallback(&cleanupLevelPreviewPortraits);
    setCallback(&func_80020708_21308);
}

INCLUDE_ASM("asm/nonmatchings/202A0", func_80020708_21308);

typedef struct {
    u8 _pad[0xC];
} LevelPreviewPortraitEntry;

extern void func_80020924_21524(void);

void renderLevelPreviewPortraits(LevelPreviewPortraitEntry *portraitEntries) {
    Allocation_202A0 *allocation;
    s32 i;

    allocation = (Allocation_202A0 *)getCurrentAllocation();
    i = 0;

    if (allocation->menuState == 1) {
        setCallbackWithContinue(&func_80020924_21524);
    } else {
        for (i = 0; i < 2; i++) {
            debugEnqueueCallback(8, 7, func_8000FED0_10AD0, &portraitEntries[i]);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/202A0", func_80020924_21524);

INCLUDE_ASM("asm/nonmatchings/202A0", func_80020A00_21600);

void cleanupLevelPreviewPortraits(LevelPreviewPortraitState *state) {
    state->portraitAsset = freeNodeMemory(state->portraitAsset);
}

INCLUDE_ASM("asm/nonmatchings/202A0", func_80020B44_21744);

INCLUDE_ASM("asm/nonmatchings/202A0", func_80020D18_21918);

void func_80020FA4_21BA4(Func80020FA4Arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
    arg0->unkA0 = freeNodeMemory(arg0->unkA0);
}

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
} Func80020FDCArg;

typedef struct {
    void *unk0;
    void *unk4;
} Func8002109CArg;

void func_80021054_21C54(void *arg0);
void func_8002109C_21C9C(Func8002109CArg *arg0);

void func_80020FDC_21BDC(Func80020FDCArg *arg0) {
    void *temp_s1 = loadCompressedData(&_41A1D0_ROM_START, &_41A1D0_ROM_END, 0x1B48);

    setCleanupCallback(&func_8002109C_21C9C);

    arg0->unk0 = -0x2C;
    arg0->unk2 = 4;
    arg0->unk8 = 0xD;
    arg0->unk4 = temp_s1;

    setCallback(&func_80021054_21C54);
}

void func_80021054_21C54(void *arg0) {
    Allocation_202A0 *allocation = (Allocation_202A0 *)getCurrentAllocation();

    if (allocation->menuState == 2) {
        debugEnqueueCallback(8, 7, func_8000FED0_10AD0, arg0);
    }
}

void func_8002109C_21C9C(Func8002109CArg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

typedef struct {
    u8 _pad0[0x4];
    u8 unk4;
    u8 _pad5[0x2];
    u8 unk7;
} D_800AFE8C_type_202A0;

extern D_800AFE8C_type_202A0 *D_800AFE8C_A71FC;

extern u16 D_8009ADE0_9B9E0;

void func_80021184_21D84(Func80021238Arg *arg0);
void func_8002115C_21D5C(Func80021238Arg *arg0);
void func_80021238_21E38(Func80021238Arg *arg0);

void func_800210C8_21CC8(Func80021238Arg *arg0) {
    void *temp_s1;

    arg0->unk2C = loadCompressedData(&_43F050_ROM_START, &_43F050_ROM_END, 0x14010);
    temp_s1 = loadCompressedData(&_459310_ROM_START, &_459310_ROM_END, 0x2278);
    setCleanupCallback(&func_80021238_21E38);

    arg0->unk30 = 0x40;
    arg0->unk32 = 0x40;
    arg0->unk34 = temp_s1;
    arg0->unk38 = 0x13;

    setCallback(&func_8002115C_21D5C);
}

void func_8002115C_21D5C(Func80021238Arg *arg0) {
    func_800394BC_3A0BC(arg0, (s32)arg0->unk2C);
    setCallback(&func_80021184_21D84);
}

void func_80021184_21D84(Func80021238Arg *arg0) {
    u16 temp;

    debugEnqueueCallback(0xA, 0, func_80038420_39020, arg0);

    if (D_800AFE8C_A71FC->unk4 == 0) {
        if (EepromSaveData->save_slot_status[0] == 5) {
            if ((D_8009ADE0_9B9E0 & 7) == 0) {
                temp = arg0->unk38 + 1;
                arg0->unk38 = temp;
                if (temp < 0x15) {
                    // continue
                } else {
                    arg0->unk38 = 0x13;
                }
            }
            debugEnqueueCallback(8, 7, func_8000FED0_10AD0, &arg0->unk30);
        }
    }
}

void func_80021238_21E38(Func80021238Arg *arg0) {
    arg0->unk2C = freeNodeMemory(arg0->unk2C);
    arg0->unk34 = freeNodeMemory(arg0->unk34);
}

extern u16 D_8008DAC0_8E6C0[];
extern u16 D_8008DAB0_8E6B0[];
extern u16 D_8008DAB8_8E6B8[];
extern u16 D_8008DAC8_8E6C8[];

void func_8002152C_2212C(Func8002144CArg *arg0);
void func_800213C8_21FC8(Func8002144CArg *arg0);

void func_80021270_21E70(Func8002144CArg *arg0) {
    Allocation_202A0 *temp_s2;
    u16 temp_s3;
    u16 temp_s0;
    u16 temp_v1;

    temp_s2 = (Allocation_202A0 *)getCurrentAllocation();

    if (temp_s2->unkB45 != 0) {
        s32 idx = D_800AFE8C_A71FC->unk7 * 2;
        temp_v1 = D_8008DAC0_8E6C0[D_800AFE8C_A71FC->unk7];
        temp_s3 = D_8008DAB0_8E6B0[D_800AFE8C_A71FC->unk7];
        temp_s0 = D_8008DAB8_8E6B8[D_800AFE8C_A71FC->unk7];
        arg0->unk20 = temp_v1;
    } else {
        temp_s3 = 0x3A;
        arg0->unk20 = 0xD;
    }

    if (temp_s2->unkB45 == 0) {
        arg0->unk0 = createSceneModel(temp_s3, temp_s2);
    } else {
        arg0->unk0 = createSceneModelEx(temp_s3, temp_s2, (s8)temp_s0, -1, -1, -1);
    }

    memcpy((u8 *)&arg0->unk4, identityMatrix, 0x20);

    if ((temp_s2->unkB45 != 0) && (temp_s3 != 0x3A)) {
        createYRotationMatrix((&arg0->unk4), 0x1E00);
        arg0->unk4.translation.x = 0x280000;
    } else {
        createYRotationMatrix((&arg0->unk4), 0x200);
        if (temp_s2->unkB45 == 0) {
            arg0->unk4.translation.x = 0xFFD80000;
        } else {
            arg0->unk4.translation.x = 0xFFD00000;
        }
    }

    arg0->unk4.translation.y = 0xFFD60000;
    arg0->unk4.translation.z = 0x100000;

    setCleanupCallback(&func_8002152C_2212C);
    setCallback(&func_800213C8_21FC8);
}

void func_8002144C_2204C(Func8002144CArg *arg0);
void func_800215DC_221DC(Func8002144CArg *arg0);
void func_80021548_22148(u8 arg0, Func8002144CArg *arg1);

void func_800213C8_21FC8(Func8002144CArg *arg0) {
    Allocation_202A0 *allocation = (Allocation_202A0 *)getCurrentAllocation();

    if (allocation->unkB45 != 0) {
        if (D_800AFE8C_A71FC->unk7 == 0xC) {
            scaleMatrix((&arg0->unk4), 0x1000, 0x1000, 0x1000);
        }
    }

    applyTransformToModel(arg0->unk0, &arg0->unk4);
    setModelAnimation(arg0->unk0, arg0->unk20);
    updateModelGeometry(arg0->unk0);
    setCallback(&func_8002144C_2204C);
}

void func_8002144C_2204C(Func8002144CArg *arg0) {
    Allocation_202A0 *allocation;
    s32 temp;
    u16 unk24;

    allocation = (Allocation_202A0 *)getCurrentAllocation();
    applyTransformToModel(arg0->unk0, &arg0->unk4);
    temp = clearModelRotation(arg0->unk0);
    func_8006FED8_70AD8(allocation);
    updateModelGeometry(arg0->unk0);

    if (allocation->unkB45 != 0) {
        if (allocation->unkB47 != 0) {
            func_80021548_22148(allocation->unkB47, arg0);
        } else if (temp != 0 && allocation->menuState == 8) {
            func_800215DC_221DC(arg0);
        }
    } else if (temp != 0) {
        unk24 = arg0->unk20;
        if (unk24 == 0xD) {
            arg0->unk20 = unk24 + 1;
            setModelAnimation(arg0->unk0, unk24 + 1);
        }
    }
}

void func_8002152C_2212C(Func8002144CArg *arg0) {
    destroySceneModel(arg0->unk0);
}

void func_80021548_22148(u8 arg0, Func8002144CArg *arg1) {
    Allocation_202A0 *allocation = (Allocation_202A0 *)getCurrentAllocation();

    allocation->unkB47 = 0;

    if (arg0 == 2) {
        arg1->unk20 = D_8008DAC0_8E6C0[D_800AFE8C_A71FC->unk7];
    } else {
        arg1->unk20 = D_8008DAC8_8E6C8[D_800AFE8C_A71FC->unk7];
    }

    setModelAnimation(arg1->unk0, arg1->unk20);
}

void func_800215DC_221DC(Func8002144CArg *arg0) {
    u16 unk24;

    getCurrentAllocation();

    if (D_800AFE8C_A71FC->unk7 == 0xD) {
        unk24 = arg0->unk20;
        if (unk24 == 0xD) {
            arg0->unk20 = unk24 + 1;
            setModelAnimation(arg0->unk0, (s16)(unk24 + 1));
        }
    } else if (D_800AFE8C_A71FC->unk7 == 0xE) {
        unk24 = arg0->unk20;
        if (unk24 == 1) {
            arg0->unk20 = 2;
            setModelAnimation(arg0->unk0, 2);
        }
    }
}

void func_8002174C_2234C(Func800216ACArg *arg0);
void func_800216AC_222AC(Func800216ACArg *arg0);
void func_800216D4_222D4(Func800216ACArg *arg0);

void func_80021658_22258(Func800216ACArg *arg0) {
    arg0->unk2C = loadCompressedData(&_458E30_ROM_START, &_458E30_ROM_END, 0xAE0);
    setCleanupCallback(&func_8002174C_2234C);
    setCallback(&func_800216AC_222AC);
}

void func_800216AC_222AC(Func800216ACArg *arg0) {
    func_800394BC_3A0BC(arg0, (s32)arg0->unk2C);
    setCallback(&func_800216D4_222D4);
}

void func_800216D4_222D4(Func800216ACArg *arg0) {
    Allocation_202A0 *allocation = (Allocation_202A0 *)getCurrentAllocation();

    if (allocation->menuState == 8) {
        arg0->unk0++;
        arg0->unk2++;
        arg0->unk0 &= 0x3FF;
        arg0->unk2 &= 0x3FF;
        debugEnqueueCallback(0xB, 0, func_80038420_39020, arg0);
    }
}

void func_8002174C_2234C(Func800216ACArg *arg0) {
    arg0->unk2C = freeNodeMemory(arg0->unk2C);
}

extern void *D_8008DC2C_8E82C[];

void func_80021810_22410(Func80021810Arg *arg0);
void func_80021880_22480(Func80021880Arg *arg0);

void func_80021778_22378(Func80021810Arg *arg0) {
    void *temp_s1;
    D_800AFE8C_type_202A0 *temp_v1;
    s32 idx;

    temp_s1 = func_80035F80_36B80(1);
    setCleanupCallback(&func_80021880_22480);

    arg0->unk10 = 5;
    temp_v1 = D_800AFE8C_A71FC;
    arg0->unk0 = -0x50;
    arg0->unk2 = -0x10;
    arg0->unkC.asS16 = 0xFF;
    arg0->unkE.asS16 = 0xFF;
    arg0->unk8 = temp_s1;
    idx = temp_v1->unk7 - 0xC;
    arg0->unk4 = D_8008DC2C_8E82C[idx];

    setCallback(&func_80021810_22410);
}

void func_80021810_22410(Func80021810Arg *arg0) {
    Allocation_202A0 *allocation = (Allocation_202A0 *)getCurrentAllocation();

    if (allocation->menuState == 8) {
        func_80035260_35E60(
            arg0->unk8,
            arg0->unk4,
            arg0->unk0,
            arg0->unk2,
            arg0->unkC.asBytes.low,
            arg0->unkE.asBytes.low,
            arg0->unk10,
            0xB,
            1
        );
    }
}

void func_80021880_22480(Func80021880Arg *arg0) {
    arg0->unk8 = freeNodeMemory(arg0->unk8);
}

extern u32 D_4237C0;
extern u32 D_426EF0;
extern u16 D_8008DC38_8E838;

void func_80021A20_22620(Func80021A20Arg *arg0);
void func_80021B88_22788(Func80021B88Arg *arg0);

typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ void *spriteAsset;
    /* 0x08 */ s16 frameIndex;
    /* 0x0A */ s16 alpha;
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 unkD;
    /* 0x0E */ u8 _padE[2];
} PrizeSpriteEntry;

typedef struct {
    /* 0x00 */ PrizeSpriteEntry spriteEntries[2];
    /* 0x20 */ u8 _pad20[0xC];
    /* 0x2C */ s16 titleX;
    /* 0x2E */ s16 titleY;
    /* 0x30 */ u16 *titleText;
    /* 0x34 */ void *textRenderAsset;
    /* 0x38 */ s16 titleColor1;
    /* 0x3A */ s16 titleColor2;
    /* 0x3C */ u8 titlePriority;
    /* 0x3D */ u8 _pad3D[3];
    /* 0x40 */ s16 counterX;
    /* 0x42 */ s16 counterY;
    /* 0x44 */ void *counterValuePtr;
    /* 0x48 */ void *counterRenderAsset;
    /* 0x4C */ s16 counterColor1;
    /* 0x4E */ s16 counterColor2;
    /* 0x50 */ u8 counterPriority;
    /* 0x51 */ u8 _pad51[3];
    /* 0x54 */ void *backgroundAsset;
    /* 0x58 */ s16 prizeCount;
    /* 0x5A */ u16 previousCount;
    /* 0x5C */ u8 _pad5C[4];
    /* 0x60 */ u8 animationTimer;
} PrizeDisplayState;

void func_800218AC_224AC(PrizeDisplayState *arg0) {
    Allocation_202A0 *allocation;
    void *textRenderAsset;
    void *spriteAsset;
    u16 *titleText;
    s16 centeredX;
    s16 titleWidth;
    s32 i;
    s32 xPos;
    PrizeSpriteEntry *entry;
    s32 yPos;
    s32 alpha;
    u8 prizeCountValue;
    s16 spriteY;

    allocation = (Allocation_202A0 *)getCurrentAllocation();
    textRenderAsset = func_80035F80_36B80(1);
    arg0->backgroundAsset = loadAsset_34F7E0();
    spriteAsset = loadCompressedData(&D_4237C0, &D_426EF0, 0x8A08);
    setCleanupCallback(func_80021B88_22788);
    titleText = &D_8008DC38_8E838;
    arg0->animationTimer = 0;
    titleWidth = getMaxLinePixelWidth(titleText);
    i = 0;
    yPos = 0x1C;
    alpha = 0xFF;
    entry = arg0->spriteEntries;
    xPos = -0x28;
    centeredX = ((0x120 - titleWidth) / 2) - 0x90;
    arg0->titleY = -8;
    arg0->titleColor1 = 0xFF;
    arg0->titleColor2 = 0xFF;
    arg0->titleX = centeredX;
    arg0->textRenderAsset = textRenderAsset;
    arg0->titleText = titleText;
    arg0->titlePriority = 5;
L1:
    entry->x = xPos;
    entry->y = yPos;
    entry->spriteAsset = spriteAsset;
    entry->frameIndex = i;
    entry->alpha = alpha;
    entry->unkD = 0;
    entry->unkC = 0;
    entry++;
    i++;
    xPos += 0x40;
    if (i < 2)
        goto L1;

    prizeCountValue = allocation->unkB46;
    spriteY = arg0->spriteEntries[0].y;
    arg0->previousCount = 0xFFFF;
    arg0->counterX = -6;
    arg0->counterValuePtr = &arg0->prizeCount;
    arg0->counterColor1 = 0xFF;
    arg0->counterColor2 = 0xFF;
    arg0->counterRenderAsset = textRenderAsset;
    arg0->counterPriority = 5;
    arg0->prizeCount = (s16)prizeCountValue;
    arg0->counterY = (s16)(spriteY + 2);

    setCallback(func_80021A20_22620);
}

void func_80021A20_22620(Func80021A20Arg *arg0) {
    Allocation_202A0 *allocation;
    s32 i;
    Func80021A20SubStruct *subStructs;

    allocation = getCurrentAllocation();

    if (allocation->menuState == 3) {
        arg0->unk60++;
        if (arg0->unk60 < 0x11) {
            arg0->unkA -= 8;
            arg0->unk1A -= 8;
        } else {
            arg0->unkA += 8;
            arg0->unk1A += 8;
        }

        if (arg0->unk60 == 0x20) {
            arg0->unk60 = 0;
            arg0->unkA = 0xFF;
            arg0->unk1A = 0xFF;
        }
    } else {
        arg0->unk60 = 0;
        arg0->unkA = 0xFF;
        arg0->unk1A = 0xFF;
    }

    if ((u32)(allocation->menuState - 2) < 2) {
        func_8006D7B0_6E3B0(arg0->unk54, -0x40, -0x8, 8, 4, 0, 0x60, 0xC0, 8, 0);
        debugEnqueueCallback(8, 1, func_80035408_36008, &arg0->unk2C);

        subStructs = (Func80021A20SubStruct *)arg0;
        for (i = 0; i < 2; i++) {
            debugEnqueueCallback(8, 1, func_80012004_12C04, &subStructs[i]);
        }

        arg0->unk58 = allocation->unkB46;
        debugEnqueueCallback(8, 1, func_80035408_36008, &arg0->unk40);
    }
}

void func_80021B88_22788(Func80021B88Arg *arg0) {
    arg0->unk34 = freeNodeMemory(arg0->unk34);
    arg0->unk54 = freeNodeMemory(arg0->unk54);
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}
