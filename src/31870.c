#include "10AD0.h"
#include "20F0.h"
#include "288A0.h"
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
    s16 unk2;
    u8 padding[0x4];
    s16 unk8;
    u8 padding2[0xE];
} func_80032B0C_3370C_arg_element;

typedef struct {
    func_80032B0C_3370C_arg_element elements[4];
    u8 padding[0x24];
    u8 unk84[4];
} func_80032B0C_3370C_arg;

typedef struct {
    u8 padding[0x77C];
    u16 unk77C;
    u8 padding2[0x6];
    u8 unk784[4];
    u8 unk788[17];
    u8 unk799;
    u8 unk79A;
    u8 unk79B;
    u8 unk79C;
    s8 unk79D;
} func_800329A8_335A8_allocation;

typedef struct {
    u8 padding[0x8];
    s16 unk8;
    u8 padding2[0x2];
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s8 unk12;
    s8 unk13;
    u8 unk14;
    u8 padding3[0x3];
} func_800329A8_335A8_arg_item;

typedef struct {
    func_800329A8_335A8_arg_item unk0[4];
    u8 padding2[0x18];
    s16 unk78;
    s16 unk7A;
    u8 padding3[0x8];
    s8 unk84[4];
    char unk88;
} func_800329A8_335A8_arg;

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
} func_80031510_32110_arg;

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
    SceneModel *unk0;
    Transform3D unk4;
    union {
        SceneModel *unk20;
        s32 unk20_s32;
        s16 unk20_s16;
    } unk20_u;
} func_80031ABC_326BC_arg;

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
} func_80032244_32E44_arg;

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
    SceneModel *unk0;
    Transform3D unk4;
    s16 unk24;
    u16 unk26;
    u8 unk28;
} func_80031A0C_3260C_arg;

typedef struct {
    u8 padding[0x8];
    void *unk8;
} func_80033088_33C88_arg;

typedef struct {
    s16 unk0;
    s32 unk4;
} func_80033014_33C14_arg;

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    void *unk8;
    s16 unkC;
    s16 unkE;
    s8 unk10;
} func_80032F90_33B90_arg;

typedef struct {
    s16 unk0;
    s16 unk2;
    u8 padding[0x80];
    u8 unk84;
} func_80032EDC_33ADC_arg;

typedef struct {
    char padding;
    void *unk4;
    char padding2[78];
    void *unk58;
} func_80032504_33104_arg;

typedef struct {
    u8 padding[0x79C];
    u8 unk79C;
} func_80032EDC_33ADC_asset;

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s8 unk12;
    s8 unk13;
    s8 unk14;
    u8 padding[0x6F];
    s8 unk84;
} func_80032DE8_339E8_arg;

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
} func_80032DE8_339E8_asset;

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s8 unk12;
    s8 unk13;
    s8 unk14;
    u8 padding[0x3];
} func_8003253C_3313C_arg_item;

typedef struct {
    func_8003253C_3313C_arg_item unk0[4];
    u8 padding2[0x18];
    s16 unk78;
    s16 unk7A;
    s16 unk7C;
    u8 padding3[0x2];
    void *unk80;
    s8 unk84[4];
    void *unk88;
} func_8003253C_3313C_arg;

typedef struct {
    u8 padding[0x24];
    void *unk24;
    void *unk28;
    void *unk2C;
} func_800319C8_325C8_arg;

typedef struct {
    s16 unk0;
    s16 unk2;
    u8 padding[0x14];
} func_80032CB4_338B4_arg_item;

typedef struct {
    func_80032CB4_338B4_arg_item substruct[4];
    u8 padding2[0x24];
    u8 unk84;
} func_80032CB4_338B4_arg;

typedef struct {
    u8 padding[0x77C];
    s16 unk77C;
} func_80032CB4_338B4_asset;

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
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s8 unk12;
    s8 unk13;
    s8 unk14;
    u8 padding[0x3];
} func_80032628_item;

typedef struct {
    func_80032628_item unk0[4];
} func_80032628_arg;

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
void func_800329A8_335A8(func_800329A8_335A8_arg *arg0);
void func_800317D4_323D4(func_80031510_32110_arg *arg0);
void waitBoardShopCharacterPreview(void);
void animateBoardShopCharacterSlideIn(func_80031510_32110_arg *arg0);
void updateBoardShopCharacterPreview(func_80031510_32110_arg *arg0);
void func_80031510_32110(func_80031510_32110_arg *arg0);
void func_800315C0_321C0(func_80031510_32110_arg *arg0);
void func_8003165C_3225C(func_800319C8_325C8_arg *arg0);
void func_80031758_32358(func_80031510_32110_arg *arg0);
void func_800316AC_322AC(func_80031510_32110_arg *arg0);
void func_80031C4C_3284C(func_80031A0C_3260C_arg *arg0);
void func_80031ABC_326BC(func_80031ABC_326BC_arg *arg0);
void func_80031B30_32730(func_80031A0C_3260C_arg *arg0);
void func_80031CC0_328C0(func_8002FA1C_3061C_arg *arg0);
void func_80031D14_32914(func_80031510_32110_arg *arg0);
void func_80032218_32E18(void *);
void func_800322BC_32EBC(void *arg0);
void func_80032304_32F04(func_80032244_32E44_arg *arg0);
void func_8003316C_33D6C(func_800330B4_33CB4_arg *arg0);
void func_800331CC_33DCC(func_800330B4_33CB4_arg *arg0);
void func_80033014_33C14(func_80033014_33C14_arg *arg0);
void func_80033088_33C88(func_80033088_33C88_arg *arg0);
void func_80032EDC_33ADC(func_80032EDC_33ADC_arg *arg0);
void func_80032EA4_33AA4(void *);
void func_80032F64_33B64(func_80032244_32E44_arg *);
void func_80032DBC_339BC(func_80032F90_33B90_arg *arg0);
void func_800319C8_325C8(func_800319C8_325C8_arg *arg0);
void func_80031944_32544(func_80031944_32544_arg *arg0);
void func_80031CE8_328E8(void *arg0);
void cleanupBoardShopSnowParticles(SnowParticleState *arg0);
void waitBoardShopSnowParticles(SnowParticleState *arg0);
void func_80032BCC_337CC(func_80032B0C_3370C_arg *arg0);
void func_80032628_33228(func_80032628_arg *arg0);
void func_800323FC_32FFC(func_80032330_32F30_arg *arg0);
void func_80032B0C_3370C(func_80032B0C_3370C_arg *arg0);
void func_80032CB4_338B4(func_80032CB4_338B4_arg *arg0);
void func_80032504_33104(func_80032504_33104_arg *arg0);
void cleanupBoardShopPreviewWipe(func_80031510_32110_arg *arg0);
void animateBoardShopPreviewWipe(func_80031510_32110_arg *arg0);
void waitBoardShopPreviewWipe(func_80031510_32110_arg *arg0);
void func_80032708_33308(func_800329A8_335A8_arg *arg0);

extern u16 D_8008F16C_8FD6C[];
extern u16 D_8008F16E_8FD6E[];
extern s32 D_8008F18C_8FD8C;
extern s32 D_8008F200_8FE00[];
extern u16 D_8008F20A_8FE0A[];
extern u16 D_8008F20C_8FE0C;
extern s32 identityMatrix;
extern s16 D_8008F17C_8FD7C[];
extern u16 D_8008F184_8FD84[];

void initBoardShopPreviewWipe(func_80031510_32110_arg *arg0) {
    s32 perspectiveParams[8];
    Transform3D rotationYX;
    Transform3D rotationZ;

    u8 charIndex;
    s32 unused;
    u8 paletteId;
    void *transformMatrix;
    func_80032DE8_339E8_asset *state;
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

void waitBoardShopPreviewWipe(func_80031510_32110_arg *arg0) {
    u8 delayTimer;
    arg0->unk64--;
    delayTimer = arg0->unk64;
    if (delayTimer == 0) {
        setCallback(&animateBoardShopPreviewWipe);
    }
    func_8006FED8_70AD8(arg0->unk0);
    enqueueDisplayListObject(1, (DisplayListObject *)&arg0->unk4);
}

void animateBoardShopPreviewWipe(func_80031510_32110_arg *arg0) {
    func_80032DE8_339E8_asset *state = getCurrentAllocation();

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

void cleanupBoardShopPreviewWipe(func_80031510_32110_arg *arg0) {
    arg0->unk0 = (Node_70B00 *)freeNodeMemory(arg0->unk0);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
    arg0->unk2C = freeNodeMemory(arg0->unk2C);
    arg0->unk30 = freeNodeMemory(arg0->unk30);
}

void initBoardShopSnowParticles(SnowParticleState *arg0) {
    func_80032DE8_339E8_asset *state;
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

void initBoardShopCharacterPreview(func_80031510_32110_arg *arg0) {
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
    setCleanupCallback(&func_800317D4_323D4);
    setCallback(&waitBoardShopCharacterPreview);
}

void waitBoardShopCharacterPreview(void) {
    func_80032DE8_339E8_asset *allocation;

    allocation = getCurrentAllocation();
    if (allocation->unk788[19] == 1) {
        setCallbackWithContinue(animateBoardShopCharacterSlideIn);
    }
}

void animateBoardShopCharacterSlideIn(func_80031510_32110_arg *arg0) {
    func_80032DE8_339E8_asset *allocation;

    allocation = getCurrentAllocation();

    arg0->unk50 += 0xFFF00000;

    memcpy(arg0, &arg0->unk3C, 0x20);

    if (arg0->unk50 == 0) {
        allocation->unk788[19] = 0xC;
        setCallbackWithContinue(&updateBoardShopCharacterPreview);
    }

    enqueueDisplayListObject(0, (DisplayListObject *)arg0);
}

void updateBoardShopCharacterPreview(func_80031510_32110_arg *arg0) {
    func_80032DE8_339E8_asset *allocation;
    u8 state;

    allocation = getCurrentAllocation();
    state = allocation->unk788[19];

    if (state == 3 || state == 0x1A) {
        if (state == 3) {
            arg0->unk24 = freeNodeMemory(arg0->unk24);
            arg0->unk28 = freeNodeMemory(arg0->unk28);
            arg0->unk2C = freeNodeMemory(arg0->unk2C);
            setCallback(&func_80031510_32110);
        } else {
            setCallback(&func_8003165C_3225C);
            enqueueDisplayListObject(0, (DisplayListObject *)arg0);
        }
    } else if (state == 0x32) {
        setCallback(&func_80031758_32358);
        enqueueDisplayListObject(0, (DisplayListObject *)arg0);
    } else {
        enqueueDisplayListObject(0, (DisplayListObject *)arg0);
    }
}

void func_80031510_32110(func_80031510_32110_arg *arg0) {
    u8 assetIndex;
    u8 temp_v1;
    func_80032DE8_339E8_asset *state;

    state = getCurrentAllocation();

    if (state->unk79C == 0) {
        arg0->unk50 = 0xFFA00000;
    } else {
        arg0->unk50 = 0x600000;
    }

    assetIndex = EepromSaveData->character_or_settings[state->unk79E];

    memcpy(arg0, &arg0->unk3C, 0x20);

    arg0->unk20 = loadAssetByIndex_95728(state->unk79E);
    arg0->unk24 = loadAssetByIndex_95500(state->unk79E);
    arg0->unk28 = loadAssetByIndex_95590(state->unk79E);
    arg0->unk2C = loadAssetByIndex_95668(assetIndex - 1);

    setCallback(&func_800315C0_321C0);
}

void func_800315C0_321C0(func_80031510_32110_arg *arg0) {
    func_80032DE8_339E8_asset *allocation;
    s32 temp;

    allocation = (func_80032DE8_339E8_asset *)getCurrentAllocation();

    if (allocation->unk79C == 1) {
        temp = 0xFFF00000;
    } else {
        temp = 0x100000;
    }

    arg0->unk50 += temp;

    memcpy(arg0, &arg0->unk3C, 0x20);

    if (arg0->unk50 == 0) {
        allocation->unk79D--;
        setCallback(&updateBoardShopCharacterPreview);
    }

    enqueueDisplayListObject(0, (DisplayListObject *)arg0);
}

void func_8003165C_3225C(func_800319C8_325C8_arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
    arg0->unk2C = freeNodeMemory(arg0->unk2C);
    setCallback(&func_800316AC_322AC);
}

void func_800316AC_322AC(func_80031510_32110_arg *arg0) {
    func_80032DE8_339E8_asset *allocation;
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

void func_80031758_32358(func_80031510_32110_arg *arg0) {
    func_800329A8_335A8_allocation *allocation;

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

void func_800317D4_323D4(func_80031510_32110_arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
    arg0->unk2C = freeNodeMemory(arg0->unk2C);
}

void func_80031818_32418(func_80031510_32110_arg *arg0) {
    func_80032DE8_339E8_asset *state;
    Transform3D matrixB;
    Transform3D matrixA;
    u8 temp_v0;
    u8 temp_s1;
    Transform3D *pMatrixB;
    Transform3D *pMatrixA;

    state = getCurrentAllocation();

    memcpy(&arg0->unk3C, &identityMatrix, sizeof(Transform3D));

    pMatrixA = &matrixA;
    memcpy(pMatrixA, &arg0->unk3C, sizeof(Transform3D));

    pMatrixB = &matrixB;
    memcpy(pMatrixB, pMatrixA, sizeof(Transform3D));

    createRotationMatrixYX(pMatrixB, 0x1000, 0x800);
    createZRotationMatrix(pMatrixA, 0x1F00);
    func_8006B084_6BC84(pMatrixB, pMatrixA, &arg0->unk3C);

    arg0->unk58 = 0xFFF80000;
    memcpy(arg0, &arg0->unk3C, 0x20U);

    temp_v0 = state->unk79F;
    temp_s1 = EepromSaveData->character_or_settings[temp_v0];

    arg0->unk20 = loadAssetByIndex_95728(temp_v0);
    arg0->unk24 = loadAssetByIndex_95500(temp_v0);
    arg0->unk28 = loadAssetByIndex_95590(temp_v0);
    arg0->unk2C = loadAssetByIndex_95668(temp_s1 - 1);

    setCleanupCallback(&func_800319C8_325C8);
    setCallbackWithContinue(&func_80031944_32544);
}

void func_80031944_32544(func_80031944_32544_arg *arg0) {
    s32 pad[8];
    GameState *state;
    s32 offset;
    s32 compareValue;
    s32 temp;

    state = getCurrentAllocation();

    offset = 0x100000;
    compareValue = 0x600000;

    if (state->unk79C == 1) {
        offset = 0xFFF00000;
        compareValue = 0xFFA00000;
    }

    temp = arg0->unk50 + offset;
    arg0->unk50 = temp;
    memcpy(&arg0->unk0, &arg0->unk3C, 0x20);

    enqueueDisplayListObject(0, &arg0->unk0);

    if (arg0->unk50 == compareValue) {
        func_80069CF8_6A8F8();
    }
}

void func_800319C8_325C8(func_800319C8_325C8_arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
    arg0->unk2C = freeNodeMemory(arg0->unk2C);
}

void func_80031A0C_3260C(func_80031A0C_3260C_arg *arg0) {
    arg0->unk0 = createSceneModelEx(0x3A, &((GameState *)getCurrentAllocation())->audioPlayer2, 0, -1, 0, 0x12);

    memcpy(&arg0->unk4, &identityMatrix, sizeof(Transform3D));

    arg0->unk4.translation.x = 0xFFE70000;
    arg0->unk4.translation.y = 0xFFE00000;
    arg0->unk4.translation.z = 0;

    createYRotationMatrix(&arg0->unk4, 0x200);

    arg0->unk24 = 0x10;
    arg0->unk26 = 0x10;
    arg0->unk28 = 0;

    setCleanupCallback(&func_80031C4C_3284C);
    setCallback(&func_80031ABC_326BC);
}

void func_80031ABC_326BC(func_80031ABC_326BC_arg *arg0) {
    GameState *temp_s1;

    temp_s1 = (GameState *)getCurrentAllocation();
    applyTransformToModel(arg0->unk0, &arg0->unk4);
    setItemDisplayEnabled(arg0->unk0, 1);
    setModelAnimation(arg0->unk0, arg0->unk20_u.unk20_s16);
    updateModelGeometry(arg0->unk0);
    if (temp_s1->unk79B != 0) {
        setCallback(&func_80031B30_32730);
    }
}

typedef struct {
    u8 padding[0x1D8];
    void *audioPlayer2;
    u8 padding2[0x5A2];
    u16 unk77E;
    u8 padding3[0x23];
    u8 unk7A3;
} func_80031B30_allocation;

void func_80031B30_32730(func_80031A0C_3260C_arg *arg0) {
    func_80031B30_allocation *allocation;
    s32 result;
    s32 state;
    u16 counter;
    volatile u8 pad[8];

    allocation = (func_80031B30_allocation *)getCurrentAllocation();
    state = arg0->unk28;

    if (state != 2) {
        result = clearModelRotation(arg0->unk0);
        state = arg0->unk28;

        if (state == 1) {
            if (result != NULL) {
                counter = arg0->unk24 + 1;
                arg0->unk24 = counter;

                if (arg0->unk26 < counter) {
                    arg0->unk28 = 0;
                    arg0->unk24 = 0x10;
                }

                setModelAnimation(arg0->unk0, arg0->unk24);
                state = arg0->unk28;
            }
        }
    }

    if (state != 1 || allocation->unk7A3 != 0) {
        if (allocation->unk77E != 0) {
            arg0->unk24 = D_8008F16C_8FD6C[allocation->unk77E * 2];
            arg0->unk26 = D_8008F16E_8FD6E[allocation->unk77E * 2];
            setModelAnimation(arg0->unk0, arg0->unk24);
            allocation->unk77E = 0;
            arg0->unk28 = 1;
            allocation->unk7A3 = 0;
        }
    }

    func_8006FED8_70AD8(&allocation->audioPlayer2);
    updateModelGeometry(arg0->unk0);
}

void func_80031C4C_3284C(func_80031A0C_3260C_arg *arg0) {
    destroySceneModel(arg0->unk0);
}

void func_80031C68_32868(func_8002FA1C_3061C_arg *arg0) {
    arg0->unk2C = loadCompressedData(&_4488E0_ROM_START, &_4488E0_ROM_END, 0x14410);
    setCleanupCallback(&func_80031D14_32914);
    setCallback(&func_80031CC0_328C0);
}

void func_80031CC0_328C0(func_8002FA1C_3061C_arg *arg0) {
    func_800394BC_3A0BC(arg0, (s32)arg0->unk2C);
    setCallback(&func_80031CE8_328E8);
}

void func_80031CE8_328E8(void *arg0) {
    debugEnqueueCallback(9, 0, &func_80038420_39020, arg0);
}

void func_80031D14_32914(func_80031510_32110_arg *arg0) {
    arg0->unk2C = freeNodeMemory(arg0->unk2C);
}

typedef struct {
    void *unk0;
    void *unk4;
} func_80031F68_32B68_arg;

void func_80031F68_32B68(func_80031F68_32B68_arg *arg0);

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
    s16 unkA;
    s8 unkC;
    s8 unkD;
    s16 unkE;
} func_80031D40_32940_arg_element;

typedef struct {
    func_80031D40_32940_arg_element unk0[2];
    u8 unk20;
} func_80031D40_32940_arg;

void func_80031DE4_329E4(func_80031D40_32940_arg *arg0);

void func_80031D40_32940(func_80031D40_32940_arg *arg0) {
    s32 i;
    void *temp_s0;

    temp_s0 = loadCompressedData(&_4237C0_ROM_START, &_4237C0_ROM_END, 0x8A08);
    setCleanupCallback(&func_80031F68_32B68);

    for (i = 0; i < 2; i++) {
        arg0->unk0[i].unk0 = i * 0x50 + -0x30;
        arg0->unk0[i].unk2 = -0x18;
        arg0->unk0[i].unk8 = i;
        arg0->unk0[i].unk4 = temp_s0;
        arg0->unk0[i].unkA = 0xFF;
        arg0->unk0[i].unkD = 0;
        arg0->unk0[i].unkC = 0;
    }

    arg0->unk20 = 0;
    setCallback(&func_80031DE4_329E4);
}

void func_80031DE4_329E4(func_80031D40_32940_arg *arg0) {
    func_800329A8_335A8_allocation *allocation;
    func_80031D40_32940_arg_element *s0;
    u8 state;
    s16 s4;
    s16 s3;

    allocation = getCurrentAllocation();

    if (allocation->unk79B < 15) {
        s4 = 0xFF;
        s3 = 8;
        s0 = arg0->unk0;

    loop:
        state = allocation->unk79B;
        if ((state == 2) | (state == 5)) {
            if (arg0->unk20 < 16) {
                s0->unkA = s0->unkA - 8;
            } else {
                s0->unkA = s0->unkA + 8;
            }
        } else {
            arg0->unk20 = 0;
            s0->unkA = s4;
            state = allocation->unk79B;
            if ((state == 4) & (state == 7)) {
                if (allocation->unk77C & 1) {
                    s0->unkD = s4;
                } else {
                    s0->unkD = 0;
                }
            }
        }

        state = allocation->unk79B;
        if (state >= 5) {
            s0->unk2 = s3;
        } else {
            s0->unk2 = -24;
        }

        if (allocation->unk79B == 3) {
            if (allocation->unk79D < 0) {
                s0->unk2 = s3;
            }
        }

        debugEnqueueCallback(8, 0, func_80012004_12C04, s0);
        s0++;
        if ((s32)s0 < (s32)(arg0->unk0 + 2)) {
            goto loop;
        }

        state = allocation->unk79B;
        if ((state != 4) & (state != 7)) {
            if (state != 3) {
                arg0->unk20 = (arg0->unk20 + 1) & 0x1F;
            }
        }
    }
}

void func_80031F68_32B68(func_80031F68_32B68_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

typedef struct {
    void *unk0;
    void *unk4;
} func_800320B8_32CB8_arg;

void func_800320B8_32CB8(func_800320B8_32CB8_arg *arg0);

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
    s16 unkA;
    s8 unkC;
    s8 unkD;
} func_80031F94_32B94_arg;

typedef struct {
    u8 _pad[0x77C];
    u16 unk77C;
    u8 _pad2[0x1D];
    u8 unk79B;
    u8 _pad3[0x5];
    s8 unk7A1;
    s8 unk7A2;
} func_80032020_32C20_allocation;

void func_80032020_32C20(func_80031F94_32B94_arg *arg0);

void func_80031F94_32B94(func_80031F94_32B94_arg *arg0) {
    void *temp_s1;

    getCurrentAllocation();
    temp_s1 = loadCompressedData(&_4237C0_ROM_START, &_4237C0_ROM_END, 0x8A08);
    setCleanupCallback(&func_800320B8_32CB8);
    arg0->unk0 = -0x1C;
    arg0->unk2 = -0x18;
    arg0->unk8 = 0x1D;
    arg0->unkA = 0xFF;
    arg0->unkC = 0;
    arg0->unkD = 0;
    arg0->unk4 = temp_s1;
    setCallback(&func_80032020_32C20);
}

void func_80032020_32C20(func_80031F94_32B94_arg *arg0) {
    func_80032020_32C20_allocation *allocation;
    u8 temp;

    allocation = getCurrentAllocation();

    if (allocation->unk79B < 0xF) {
        if (allocation->unk79B != 3) {
            arg0->unk8 = allocation->unk7A1 + 0x1D;

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

void func_800320B8_32CB8(func_800320B8_32CB8_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
    s16 unkA;
    s8 unkC;
    s8 unkD;
} func_800320E4_32CE4_arg;

void func_80032170_32D70(func_800320E4_32CE4_arg *arg0);

void func_800320E4_32CE4(func_800320E4_32CE4_arg *arg0) {
    void *temp_s1;

    getCurrentAllocation();
    temp_s1 = loadCompressedData(&_4237C0_ROM_START, &_4237C0_ROM_END, 0x8A08);
    setCleanupCallback(&func_80032218_32E18);
    arg0->unk0 = -8;
    arg0->unk2 = 8;
    arg0->unk8 = 0x24;
    arg0->unkA = 0xFF;
    arg0->unkC = 0;
    arg0->unkD = 0;
    arg0->unk4 = temp_s1;
    setCallback(&func_80032170_32D70);
}

void func_80032170_32D70(func_800320E4_32CE4_arg *arg0) {
    func_80032020_32C20_allocation *allocation;
    u8 state;
    u8 temp;

    allocation = getCurrentAllocation();
    state = allocation->unk79B;

    if (state < 0xF) {
        if ((state >= 5) && (state != 6)) {
            arg0->unk8 = allocation->unk7A2 + 0x24;

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
    void *unk4;
} func_80032218_32E18_arg;

void func_80032218_32E18(void *arg0) {
    func_80032218_32E18_arg *temp = arg0;
    temp->unk4 = freeNodeMemory(temp->unk4);
}

void func_80032244_32E44(func_80032244_32E44_arg *arg0) {
    void *asset = loadCompressedData(&_41A1D0_ROM_START, &_41A1D0_ROM_END, 0x1B48);
    setCleanupCallback(&func_80032304_32F04);
    arg0->unk0 = -0x2C;
    arg0->unk2 = -0x14;
    arg0->unk8 = 0xD;
    arg0->unk4 = asset;
    setCallback(&func_800322BC_32EBC);
}

void func_800322BC_32EBC(void *arg0) {
    func_80032DE8_339E8_asset *allocation;

    allocation = getCurrentAllocation();
    if (allocation->unk788[0x13] == 0x19) {
        debugEnqueueCallback(8, 7, &func_8000FED0_10AD0, arg0);
    }
}

void func_80032304_32F04(func_80032244_32E44_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_80032330_32F30(func_80032330_32F30_arg *arg0) {
    void *temp_v0 = loadCompressedData(&_3F6950_ROM_START, &_3F6950_ROM_END, 0x508);
    void *temp_v0_2 = loadCompressedData(&_3F6670_ROM_START, &_3F6670_ROM_END, 0x388);
    s32 i;
    s16 temp_a0;

    setCleanupCallback(&func_80032504_33104);

    for (i = 0; i < 7; i++) {
        arg0->elements[i].unk0 = 0x48 + (i * 8);
        arg0->elements[i].unk2 = 0x58;
        arg0->elements[i].unk4 = temp_v0;
    }

    arg0->unk54 = 0x38;
    arg0->unk56 = 0x58;
    arg0->unk5C = 0;
    arg0->unk5E = 0;
    arg0->unk58 = temp_v0_2;

    setCallback(&func_800323FC_32FFC);
}

void func_800323FC_32FFC(func_80032330_32F30_arg *arg0) {
    func_80032330_32F30_element *elem;
    s32 space;
    s32 i;
    s32 val;

    if (D_800AFE8C_A71FC->gold < 100) {
        val = 1;
        i = 6;
        do {
            arg0->elements[i].unkA = val;
        } while (--i >= 0);
    } else {
        val = 2;
        i = 6;
        do {
            arg0->elements[i].unkA = val;
        } while (--i >= 0);
    }

    sprintf(arg0->unk60, D_8009E47C_9F07C, D_800AFE8C_A71FC->gold);

    i = 0;
    space = 0x20;
    elem = arg0->elements;

    for (; i < 7; i++) {
        if (arg0->unk60[i] != space) {
            elem->unk8 = arg0->unk60[i] - 0x30;
            debugEnqueueCallback(9, 7, func_80010240_10E40, elem);
        }
        elem++;
    }

    debugEnqueueCallback(9, 7, func_80010240_10E40, &arg0->unk54);
}

void func_80032504_33104(func_80032504_33104_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
    arg0->unk58 = freeNodeMemory(arg0->unk58);
}

void func_8003253C_3313C(func_8003253C_3313C_arg *arg0) {
    func_80032DE8_339E8_asset *state;
    void *asset;
    s32 i;
    u8 temp;

    state = getCurrentAllocation();
    asset = loadCompressedData(&_4547D0_ROM_START, &_4547D0_ROM_END, 0x9488);

    for (i = 0; i < 4; i++) {
        arg0->unk0[i].unk0 = 0x60;
        arg0->unk0[i].unk2 = -0x91;

        temp = state->unk784[i];
        temp = state->unk788[temp];

        arg0->unk0[i].unk8 = temp;
        arg0->unk0[i].unkA = 0x400;
        arg0->unk0[i].unkC = 0x400;
        arg0->unk0[i].unkE = 0;
        arg0->unk0[i].unk10 = 0xFF;
        arg0->unk0[i].unk13 = 0;
        arg0->unk0[i].unk12 = 0;
        arg0->unk0[i].unk14 = 0;
        arg0->unk0[i].unk4 = asset;
        arg0->unk84[i] = 0;
    }

    arg0->unk78 = 0x58;
    arg0->unk7A = -0x2A;
    arg0->unk7C = 0;
    arg0->unk80 = &arg0->unk88;

    setCleanupCallback(&func_80032DBC_339BC);
    setCallback(&func_80032628_33228);
}

void func_80032628_33228(func_80032628_arg *arg0) {
    func_80032628_alloc *allocation;
    s32 counter;
    s32 i;
    s16 current;
    s32 delta;
    s16 var_delta;

    allocation = getCurrentAllocation();
    counter = 0;

    for (i = 0; i < 4; i++) {
        current = arg0->unk0[i].unk2;
        if (current < D_8008F17C_8FD7C[i]) {
            delta = D_8008F17C_8FD7C[i] - current;
            var_delta = ABS(delta);
            if (var_delta >= 20) {
                arg0->unk0[i].unk2 = current + 20;
            } else {
                arg0->unk0[i].unk2 = current + var_delta;
            }
            counter++;
        }
        debugEnqueueCallback(8, 0, func_800136E0_142E0, &arg0->unk0[i]);
    }

    if ((counter & 0xFF) == 0) {
        allocation->unk77C = 1;
        setCallback(func_80032708_33308);
    }
}

void func_80032708_33308(func_800329A8_335A8_arg *arg0) {
    func_800329A8_335A8_allocation *state;
    s32 i;
    int new_var;
    u8 temp;
    s16 adjustment;
    state = getCurrentAllocation();
    for (i = 0; i < 4; i++) {
        if (state->unk799 == i) {
            arg0->unk0[i].unk10 = 0xFF;
            if (state->unk77C >= 5) {
                temp = arg0->unk84[i];
                if (temp < 30) {
                    adjustment = D_8008F184_8FD84[temp / 10];
                    arg0->unk0[i].unkC = arg0->unk0[i].unkC + adjustment;
                } else {
                    adjustment = D_8008F184_8FD84[2 - ((temp - 30) / 10)];
                    arg0->unk0[i].unkC = arg0->unk0[i].unkC - adjustment;
                }
                arg0->unk84[i]++;
                temp = arg0->unk84[i];
                if (temp == 60) {
                    arg0->unk84[i] = 0;
                    arg0->unk0[i].unkC = 0x400;
                } else if (temp == 30) {
                    arg0->unk0[i].unk14 = (arg0->unk0[i].unk14 + 1) & 1;
                }
            }
        } else {
            arg0->unk0[i].unk10 = 0x80;
            arg0->unk84[i] = 0;
            arg0->unk0[i].unk14 = 0;
            arg0->unk0[i].unkC = 0x400;
        }
        debugEnqueueCallback(8, 0, func_800136E0_142E0, &arg0->unk0[i]);
    }

    if (state->unk79B == 0x11) {
        for (i = 0; i < 4; i++) {
            arg0->unk0[i].unk10 = 0x80;
            arg0->unk0[i].unkC = 0x400;
            arg0->unk0[i].unk14 = 0;
            arg0->unk0[i].unk8 = state->unk788[state->unk784[i]];
        }

        arg0->unk84[0] = 1;
        setCallback(func_80032CB4_338B4);
    } else if (state->unk79B == 0x13) {
        new_var = 0x400;
        for (i = 3; i >= 0; i--) {
            arg0->unk0[i].unkC = new_var;
        }

        setCallback(func_80032B0C_3370C);
    } else {
        temp = state->unk788[state->unk784[state->unk799]];
        sprintf(&arg0->unk88, &D_8009E480_9F080, D_8008F150_8FD50[temp]);
        arg0->unk7A = (state->unk799 * 0x28) - 0x2A;
        debugEnqueueCallback(8, 7, renderTextPalette, &arg0->unk78);
        if (state->unk79B == 0x14) {
            setCallback(func_800329A8_335A8);
        }
    }
}

void func_800329A8_335A8(func_800329A8_335A8_arg *arg0) {
    func_800329A8_335A8_allocation *state;
    s32 i;
    u8 temp;
    s16 temp2;

    state = getCurrentAllocation();

    for (i = 0; i < 4; i++) {
        arg0->unk0[i].unkC = 0x400;
        arg0->unk84[i] = 0;
        temp2 = 0xFF;

        if (state->unk799 == i) {
            arg0->unk0[i].unk10 = 0xFF;
            arg0->unk0[i].unk13 = 0;
            if (state->unk79B == 0x14) {
                if ((state->unk77C & 1) != 0) {
                    __asm__ volatile("" ::: "memory");
                    arg0->unk0[i].unk13 = temp2;
                }
            }
        } else {
            arg0->unk0[i].unk10 = 0x80;
        }

        debugEnqueueCallback(8, 0, &func_800136E0_142E0, &arg0->unk0[i]);
    }

    temp = state->unk784[state->unk799];
    temp = state->unk788[temp];

    sprintf(&arg0->unk88, &D_8009E480_9F080, D_8008F150_8FD50[temp]);
    arg0->unk7A = state->unk799 * 0x28 - 0x2A;
    debugEnqueueCallback(8, 7, &renderTextPalette, &arg0->unk78);
    if (state->unk79B < 0x14) {
        setCallback(&func_80032708_33308);
    }
}

void func_80032B0C_3370C(func_80032B0C_3370C_arg *arg0) {
    func_80032DE8_339E8_asset *allocation;
    s32 i;
    s32 temp_a0;
    s16 s2;

    allocation = getCurrentAllocation();

    if (allocation->unk79C == 1) {
        temp_a0 = -0x11;
    } else {
        temp_a0 = -0x61;
    }

    i = 0;
    s2 = temp_a0;

    for (i = 0; i < 4; i++) {
        arg0->elements[i].unk2 = s2;
        arg0->elements[i].unk8 = allocation->unk788[allocation->unk784[i]];
        arg0->unk84[i] = 0;
        debugEnqueueCallback(8, 0, &func_800136E0_142E0, &arg0->elements[i]);
        s2 += 0x28;
    }

    setCallback(&func_80032BCC_337CC);
}

void func_80032BCC_337CC(func_80032B0C_3370C_arg *arg0) {
    func_80032DE8_339E8_asset *gameState;
    s32 playerIndex;
    func_80032B0C_3370C_arg_element *playerIcons;

    gameState = getCurrentAllocation();
    playerIndex = 0;
    playerIcons = arg0->elements;

    while (playerIndex < 4) {
        arg0->unk84[playerIndex]++;

        if (gameState->unk79C == 1) {
            playerIcons[playerIndex].unk2 -= 10;
        } else {
            playerIcons[playerIndex].unk2 += 10;
        }

        debugEnqueueCallback(8, 0, &func_800136E0_142E0, &playerIcons[playerIndex]);
        playerIndex++;
    }

    if (arg0->unk84[0] == 4) {
        u8 *frameCounterPtr;
        gameState->unk788[19] = 0x10;
        playerIndex = 3;
        frameCounterPtr = (u8 *)arg0 + 3;

        do {
            frameCounterPtr[0x84] = 0;
            playerIndex--;
            frameCounterPtr--;
        } while (playerIndex >= 0);

        setCallback(&func_80032708_33308);
    }
}

void func_80032CB4_338B4(func_80032CB4_338B4_arg *arg0) {
    func_80032CB4_338B4_asset *s4;
    func_80032CB4_338B4_arg_item *ptr;
    u8 a1;
    s32 i;

    s4 = (func_80032CB4_338B4_asset *)getCurrentAllocation();

    ptr = &arg0->substruct[0];
    for (i = 0; i < 4; i++) {
        if (i >= (4 - arg0->unk84)) {
            ptr[i].unk2 -= 0x14;
            if (i != 0) {
                a1 = arg0->unk84 & 0xFF;
                if (arg0->substruct[4 - a1].unk2 == arg0->substruct[3 - a1].unk2) {
                    arg0->unk84++;
                }
            }
        }
        debugEnqueueCallback(8, 0, &func_800136E0_142E0, &ptr[i]);
    }

    if (arg0->substruct[0].unk2 < (-0x88)) {
        s4->unk77C = 1;
        func_80069CF8_6A8F8();
    }
}

void func_80032DBC_339BC(func_80032F90_33B90_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_80032DE8_339E8(func_80032DE8_339E8_arg *arg0) {
    void *asset;
    func_80032DE8_339E8_asset *temp_s0 = getCurrentAllocation();

    asset = loadCompressedData(&_4547D0_ROM_START, &_4547D0_ROM_END, 0x9488);

    arg0->unk0 = 0x60;
    arg0->unk8 = temp_s0->unk788[temp_s0->unk7A0];
    arg0->unkA = 0x400;
    arg0->unkC = 0x400;
    arg0->unkE = 0;
    arg0->unk10 = 0x80;
    arg0->unk13 = 0;
    arg0->unk12 = 0;
    arg0->unk14 = 0;
    arg0->unk4 = asset;
    arg0->unk84 = 0;

    if (temp_s0->unk79C != 1) {
        arg0->unk2 = 0x3F;
    } else {
        arg0->unk2 = -0x39;
    }

    setCleanupCallback(&func_80032F64_33B64);
    setCallback(&func_80032EA4_33AA4);
}

void func_80032EA4_33AA4(void *arg0) {
    debugEnqueueCallback(8, 0, &func_800136E0_142E0, arg0);
    setCallback(&func_80032EDC_33ADC);
}

void func_80032EDC_33ADC(func_80032EDC_33ADC_arg *arg0) {
    func_80032EDC_33ADC_asset *temp_v0 = (func_80032EDC_33ADC_asset *)getCurrentAllocation();

    arg0->unk84++;
    if (temp_v0->unk79C == 1) {
        arg0->unk2 = arg0->unk2 - 0x14;
    } else {
        arg0->unk2 = arg0->unk2 + 0x14;
    }

    debugEnqueueCallback(8, 0, &func_800136E0_142E0, arg0);

    if (arg0->unk84 == 4) {
        func_80069CF8_6A8F8();
    }
}

void func_80032F64_33B64(func_80032244_32E44_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_80032F90_33B90(func_80032F90_33B90_arg *arg0) {
    void *temp_s1 = func_80035F80_36B80(1);
    setCleanupCallback(&func_80033088_33C88);
    arg0->unk2 = -0x60;
    arg0->unk4 = &D_8008F18C_8FD8C;
    arg0->unkC = 0xFF;
    arg0->unkE = 0xFF;
    arg0->unk8 = temp_s1;
    arg0->unk0 = D_8008F20C_8FE0C;
    arg0->unk10 = 5;
    setCallback(&func_80033014_33C14);
}

void func_80033014_33C14(func_80033014_33C14_arg *arg0) {
    func_8003316C_33D6C_alloc *asset = (func_8003316C_33D6C_alloc *)getCurrentAllocation();
    if (asset->unk7A4 != 0) {
        arg0->unk0 = D_8008F20A_8FE0A[asset->unk7A4];
        arg0->unk4 = D_8008F200_8FE00[asset->unk7A4];
        debugEnqueueCallback(9, 7, &func_80035408_36008, arg0);
    }
}

void func_80033088_33C88(func_80033088_33C88_arg *arg0) {
    arg0->unk8 = freeNodeMemory(arg0->unk8);
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
