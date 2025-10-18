#include "20F0.h"

#include "19E80.h"
#include "5E590.h"
#include "D6F0.h"
#include "displaylist.h"
#include "graphics.h"
#include "overlay.h"
#include "task_scheduler.h"

USE_ASSET(_646850);

typedef struct {
    u8 padding[0x24];
    /* 0x24 */ u8 *data_ptr;
    /* 0x28 */ TableEntry_19E80 *index_ptr;
    s8 unk2C;
    s8 unk2D;
} func_80002C14_3814_arg;

typedef struct {
    u8 padding[0x4C];
    s16 unk4C;
    s8 unk4E;
    s8 unk4F;
} func_80001630_2230_arg;

typedef struct {
    u8 padding[0x3C];
    s8 unk3C;
} func_800014F0_20F0_arg;

typedef struct {
    u8 padding[0x13];
    s8 unk13;
} func_80001688_2288_arg_9C;

typedef struct {
    u8 padding[0x89];
    s8 unk89;
    s16 unk8A;
    u8 padding3[0x8];
    s8 unk94;
    u8 padding2[0x4];
    func_80001688_2288_arg_9C *unk9C;
} func_80001688_2288_arg;

typedef struct {
    u8 padding[0x158];
    s32 unk158;
} func_800014FC_20FC_arg;

typedef struct {
    u8 padding[0x15C];
    s8 unk15C;
} func_8000150C_210C_arg;

typedef struct {
    u8 padding[0x96];
    u8 unk96;
    s8 unk97;
} func_80001534_2134_arg;

typedef struct {
    u8 padding[0x3D];
    s8 unk3D;
} func_800015CC_21CC_arg;

typedef struct {
    u8 padding[0x110];
    s32 unk110;
} func_800016E0_22E0_arg;

typedef struct {
    u8 padding[0x14];
    s16 unk14;
    s16 unk16;
    u8 padding2[0x20];
    s16 unk38;
    s16 unk3A;
    u8 padding5[0x52];
    s16 unk8E;
} func_80001970_2570_arg;

typedef struct {
    u8 transformationMatrix[0x20];
    void *unk20;
    void *asset1;
    void *asset2;
    void *asset3;
    u8 padding2[0xC];
} AssetSlot;

typedef struct {
    AssetSlot *unk00;
    AssetSlot *unk04;
    AssetSlot *unk08;
    s16 unk0C;
    s16 unk0E;
    void *unk10;
    s16 unk14;
    s16 unk16;
    u8 transformationMatrix[0x20];
    s16 unk38;
    s16 unk3A;
    u8 unk3C;
    u8 unk3D;
    u8 unk3E;
    u8 unk3F;
    u32 unk40;
    s32 unk44;
    u32 unk48;
    s16 unk4C;
    u8 unk4E;
    u8 unk4F;
    u8 padding3[0x38];
    u8 unk88;
    u8 unk89;
    u8 padding4[4];
    s16 unk8E;
    u8 padding5[4];
    s8 unk94;
    u8 unk95;
    u8 unk96;
    u8 unk97;
    AssetSlot *unk98;
    void *unk9C;
    void *unkA0;
    u8 unkA4[0x4C];
    u8 asset2TransformationMatrix[0x20];
    u32 unk110;
    void *unk114;
    void *unk118;
    void *soundData;
    void *unk120;
    void *unk124;
    u8 padding6[0xC];
    void *unk134;
    s16 unk138;
    u8 unk13A;
    u8 unk13B;
    u8 padding7[0x18];
    s16 unk154;
    s16 unk156;
    u32 unk158;
    u8 unk15C;
} GameEntity;

typedef struct {
    void *assetStart;
    void *assetEnd;
    void *asset2Start;
    void *asset2End;
    s32 asset2Size;
    s32 *unk14;
} D_800895A0_8A1A0_node;

extern D_800895A0_8A1A0_node D_800895A0_8A1A0[];
extern OSThread *__osActiveQueue_0;
extern AssetGroup gameAssets[];
extern s32 D_80089768_8A368;
extern AssetGroup D_8008BD2C_8C92C;
extern u8 identityMatrix[32];
extern void func_80009E68_AA68(void *param, s16 value);
extern u32 D_8008BD38_8C938;

s32 func_800018F4_24F4(SceneModel *);
s32 func_80001904_2504(s16);
s32 setModelRotation(SceneModel *, s16);
void initializeGameEntity(GameEntity *, s32, void *, s8, s8, s8, s16);
void func_80002220_2E20(SceneModel *, s16, s16, s8, s16);
void *func_80002040_2C40(SceneModel *arg0);
void *func_80009F5C_AB5C(s32);

void func_800014F0_20F0(func_800014F0_20F0_arg *arg0) {
    arg0->unk3C = 1;
}

void func_800014FC_20FC(func_800014FC_20FC_arg *arg0, s32 arg1) {
    arg0->unk158 = arg1;
}

void func_80001504_2104(func_8000150C_210C_arg *arg0) {
    arg0->unk15C = 0;
}

void func_8000150C_210C(func_8000150C_210C_arg *arg0) {
    arg0->unk15C = 1;
}

void func_80001518_2118(func_80001534_2134_arg *arg0) {
    arg0->unk97 = 1;
}

void func_80001524_2124(func_80001534_2134_arg *arg0) {
    arg0->unk97 = 0;
}

void func_8000152C_212C(func_80001534_2134_arg *arg0, s8 arg1) {
    arg0->unk96 = arg1;
}

u8 func_80001534_2134(func_80001534_2134_arg *arg0) {
    return arg0->unk96;
}

void applyTransformToModel(SceneModel *arg0, applyTransformToModel_arg1 *arg1) {
    if (arg0 != NULL) {
        memcpy(&arg0->unk18, arg1, 0x20);
    }
}

OSThread *__osGetActiveQueue(void) {
    return __osActiveQueue_0;
}

s32 func_8000156C_216C(s32 arg0) {
    return arg0 + 0x18;
}

s16 func_80001574_2174(s16 arg0) {
    return gameAssets[arg0].unk22;
}

s8 func_800015A0_21A0(s16 arg0) {
    return gameAssets[arg0].numAssets;
}

void func_800015CC_21CC(func_800015CC_21CC_arg *arg0, s8 arg1) {
    arg0->unk3D = arg1;
}

s8 func_800015D4_21D4(func_800015CC_21CC_arg *arg0) {
    return arg0->unk3D;
}

s32 func_800015DC_21DC(void) {
    return 0x98;
}

s32 func_800015E4_21E4(void) {
    return 0x9B;
}

s32 func_800015EC_21EC(void) {
    return 6;
}

s32 func_800015F4_21F4(void) {
    return 6;
}

void setModelVisibility(setModelVisibility_arg *arg0, s8 arg1) {
    arg0->unk88 = arg1;
}

void func_80001604_2204(func_80001604_2204_arg *arg0, s8 arg1) {
    arg0->unk3F = arg1;
}

void func_8000160C_220C(SceneModel *arg0, s8 arg1) {
    if (arg0->unk4C != -1) {
        arg0->unk4E = arg1;
    }
}

s32 func_80001624_2224(void) {
    return D_80089768_8A368;
}

void func_80001630_2230(func_80001630_2230_arg *arg0, s8 arg1) {
    arg0->unk4F = arg1;
}

s16 func_80001638_2238(func_80001688_2288_arg *arg0) {
    return arg0->unk8A;
}

void func_80001640_2240(func_80001688_2288_arg *arg0) {
    arg0->unk89 |= 1;
}

void func_80001650_2250(func_80001688_2288_arg *arg0) {
    arg0->unk89 &= 0xFE;
}

s8 func_80001660_2260(func_80001688_2288_arg *arg0) {
    return arg0->unk94;
}

s8 func_80001668_2268(func_80001688_2288_arg *arg0) {
    func_80001688_2288_arg_9C *temp_v0;

    temp_v0 = arg0->unk9C;
    if (temp_v0 != NULL) {
        return temp_v0->unk13;
    }
    return 0;
}

void func_80001688_2288(func_80001688_2288_arg *arg0, s8 arg1) {
    if (arg1 < func_80001668_2268(arg0)) {
        arg0->unk94 = arg1;
    }
}

void func_800016E0_22E0(func_800016E0_22E0_arg *arg0, s32 arg1) {
    arg0->unk110 = (s32)(arg0->unk110 | (1 << arg1));
}

void func_800016F8_22F8(func_800016E0_22E0_arg *arg0, s32 arg1) {
    arg0->unk110 = (s32)(arg0->unk110 & ~(1 << arg1));
}

void *loadAssetDataByMode(s16 groupIndex, s16 entityIndex, s16 mode) {
    AssetGroup *group;
    Asset *entity;

    group = &gameAssets[groupIndex];
    if (group->Assets == NULL) {
        return NULL;
    }

    if (entityIndex >= group->count) {
        return NULL;
    }

    entity = &group->Assets[entityIndex];
    if (entity == NULL) {
        return NULL;
    }

    switch (mode) {
        case MODE_DMA:
            return dmaRequestAndUpdateState(entity->displayListStart, entity->displayListEnd);

        case MODE_QUEUED_DMA:
            return dmaRequestAndUpdateStateWithSize(entity->vertexDataStart, entity->vertexDataEnd, entity->romBSize);

        case MODE_DIRECT_FETCH:
            return entity->unk14;

        default:
            return NULL;
    }
}

s32 func_800017F4_23F4(func_80002B50_3750_arg *arg0) {
    s32 new_var;
    int new_var2 = 0;
    s32 var_v0 = 0;

    if ((arg0 != 0) && ((new_var = arg0->unk88) != new_var2)) {
        var_v0 = arg0->unk3F != 0;
    }

    new_var = var_v0;
    return new_var;
}

void *loadAssetGroupSoundData(SceneModel *arg0) {
    s16 idx = arg0->index;
    AssetGroup *entry = &gameAssets[idx];
    void *allocatedSpaceStart = NULL;

    if (entry->soundSequenceDataStart != NULL) {
        allocatedSpaceStart = dmaRequestAndUpdateStateWithSize(
            entry->soundSequenceDataStart,
            entry->soundSequenceDataEnd,
            entry->soundSequenceDataSize
        );
    }

    return allocatedSpaceStart;
}

void *loadAssetGroupDisplayList(SceneModel *arg0) {
    AssetGroup *entity = &gameAssets[arg0->index];
    return dmaRequestAndUpdateState(entity->displayListStart, entity->displayListEnd);
}

void *loadAssetGroupVertexData(SceneModel *arg0) {
    AssetGroup *entity = &gameAssets[arg0->index];
    return dmaRequestAndUpdateStateWithSize(entity->vertexDataStart, entity->vertexDataEnd, entity->size);
}

s32 func_800018F4_24F4(SceneModel *arg0) {
    return arg0->unk0->unk3E0 != 0;
}

s32 func_80001904_2504(s16 arg0) {
    return gameAssets[arg0].displayListStart == NULL;
}

AssetGroup *func_80001934_2534(s32 arg0) {
    if (arg0 != -1) {
        return &gameAssets[arg0];
    }

    return &D_8008BD2C_8C92C;
}

void func_80001970_2570(func_80001970_2570_arg *arg0) {
    arg0->unk14 = -1;
    arg0->unk16 = -1;
    arg0->unk3A = -1;
    arg0->unk38 = -1;
    arg0->unk8E = -1;
}

void *func_8000198C_258C(s32 arg0, void *arg1) {
    return func_800019B8_25B8(arg0, arg1, 0, -1, -1, -1);
}

void *func_800019B8_25B8(s32 arg0, void *arg1, s8 arg2, s8 arg3, s8 arg4, s16 arg5) {
    GameState *obj = allocateNodeMemory(0x160);
    if (obj != NULL) {
        initializeGameEntity(obj, arg0, arg1, arg2, arg3, arg4, arg5);
    }
    return obj;
}

#ifdef NON_MATCHING
void initializeGameEntity(
    GameEntity *entity,
    s32 assetGroupIndex,
    void *param3,
    s8 assetPairIndex,
    s8 param5,
    s8 param6,
    s16 yetAnotherAssetIndex
) {
    AssetGroup *assetEntry;
    s32 i;
    AssetSlot *slot;
    void *asset1;
    void *asset2;
    D_800895A0_8A1A0_node *node8A1A0;
    Node *task;

    assetEntry = &gameAssets[assetGroupIndex];

    entity->unk00 = allocateNodeMemory(0x780);
    entity->unk0C = assetGroupIndex;
    entity->unk14 = -1;
    entity->unk16 = -1;
    entity->unk0E = (s8)assetEntry->numAssets;
    entity->unk3A = -1;
    entity->unk38 = -1;
    entity->unk8E = -1;
    entity->unk89 = 0;
    entity->unk40 = 0;
    entity->unk44 = NULL;
    entity->unk48 = 0;
    entity->unk95 = 0;

    memcpy(&entity->asset2TransformationMatrix, identityMatrix, 0x20);

    entity->unk110 = -1;
    entity->unk96 = 0xFF;
    entity->unk4C = yetAnotherAssetIndex;
    entity->unk97 = 0;

    for (i = 0; i < 0x20; i++) {
        entity->unk00[i].unk20 = entity->unk00[i].asset1 = entity->unk00[i].asset2 = entity->unk00[i].asset3 = NULL;
    }

    if (assetEntry->displayListStart != NULL) {
        asset1 = dmaRequestAndUpdateState(assetEntry->displayListStart, assetEntry->displayListEnd);
        asset2 =
            dmaRequestAndUpdateStateWithSize(assetEntry->vertexDataStart, assetEntry->vertexDataEnd, assetEntry->size);

        for (i = 0; i < assetEntry->numAssets; i++) {
            entity->unk00[i].unk20 = &assetEntry->unk1C[i];
            entity->unk00[i].asset1 = asset1;
            entity->unk00[i].asset2 = asset2;
            memcpy(entity->unk00[i].transformationMatrix, identityMatrix, 0x20);
        }
    } else if (assetEntry->assetGroupIndex != -1) {
        asset1 = loadAssetByIndex_94F90(assetEntry->assetGroupIndex, assetPairIndex);
        asset2 = loadAssetByIndex_95200(assetEntry->assetGroupIndex, assetPairIndex);

        for (i = 0; i < assetEntry->numAssets; i++) {
            entity->unk00[i].unk20 = &loadAssetByIndex_95380(assetEntry->assetGroupIndex, assetPairIndex)[i];
            entity->unk00[i].asset1 = asset1;
            entity->unk00[i].asset2 = asset2;
            memcpy(entity->unk00[i].transformationMatrix, identityMatrix, 0x20);
        }
    }

    if (assetEntry->unk22) {
        if (assetEntry->anotherAssetIndex == -1) {
            entity->unk08 = dmaRequestAndUpdateStateWithSize(
                assetEntry->asset3Start,
                assetEntry->asset3End,
                assetEntry->asset3Size
            );
            entity->unk04 = allocateNodeMemory(0x980);
        } else {
            func_800585C8_591C8(0);
        }
    } else {
        if (assetEntry->anotherAssetIndex != -1) {
            entity->unk08 = loadAssetByIndex_953E0(assetEntry->anotherAssetIndex);
            entity->unk04 = allocateNodeMemory(0x980);
        } else {
            entity->unk08 = NULL;
            entity->unk04 = NULL;
        }
    }

    if (param5 != -1) {
        entity->unk00[16].unk20 = loadAssetByIndex_95728(param5);
        entity->unk00[16].asset1 = loadAssetByIndex_95500(param5);
        entity->unk00[16].asset2 = loadAssetByIndex_95590(param5);
        entity->unk00[16].asset3 = loadAssetByIndex_95668(param6);

        memcpy(&entity->unk00[16].transformationMatrix, identityMatrix, 0x20);
    }

    if (yetAnotherAssetIndex != -1 && yetAnotherAssetIndex < D_80089768_8A368) {
        node8A1A0 = &D_800895A0_8A1A0[yetAnotherAssetIndex];

        entity->unk00[17].unk20 = node8A1A0->unk14;
        entity->unk00[17].asset1 = dmaRequestAndUpdateState(node8A1A0->assetStart, node8A1A0->assetEnd);
        entity->unk00[17].asset2 =
            dmaRequestAndUpdateStateWithSize(node8A1A0->asset2Start, node8A1A0->asset2End, node8A1A0->asset2Size);
        entity->unk00[17].asset3 = NULL;

        memcpy(&entity->unk00[17].transformationMatrix, identityMatrix, 0x20);
    }

    if (assetEntry->initCallback != NULL) {
        task = scheduleTask(assetEntry->initCallback, 0, 0, 0);
        if (task != NULL) {
            task->prev = (Node *)entity;
        }
    }

    entity->unk98 = allocateNodeMemory(0x3C);
    slot = entity->unk98;

    entity->unkA0 = NULL;

    slot->asset3 = NULL;
    slot->asset2 = NULL;
    slot->asset1 = NULL;

    entity->unk94 = -1;
    entity->unk9C = NULL;

    if (assetEntry->Assets != NULL && assetPairIndex < assetEntry->count) {
        entity->unk9C = &assetEntry->Assets[assetPairIndex];
    }

    entity->unk98->asset1 = loadAssetDataByMode(assetGroupIndex, assetPairIndex, 0);
    entity->unk98->asset2 = loadAssetDataByMode(assetGroupIndex, assetPairIndex, 1);
    entity->unkA0 = loadAssetDataByMode(assetGroupIndex, assetPairIndex, 2);
    entity->unk98->unk20 = loadAssetDataByMode(assetGroupIndex, assetPairIndex, 2);

    entity->unk10 = param3;

    memcpy(&entity->transformationMatrix, identityMatrix, 0x20);

    entity->unk3C = 0;
    entity->unk3F = 1;
    entity->unk88 = 1;
    entity->unk4E = 0;
    entity->unk4F = assetEntry->unk31;
    entity->unk3D = 0;

    func_80009E68_AA68(&entity->unkA4, 0);
    entity->unk114 = func_8000CD88_D988();
    entity->unk118 = func_8000CDB4_D9B4();
    entity->soundData = loadAssetGroupSoundData(entity);
    entity->unk15C = 0;
    entity->unk158 = 0;

    entity->unk120 = dmaRequestAndUpdateStateWithSize(_646850_ROM_START, _646850_ROM_END, 0x238);
    entity->unk124 = &D_8008BD38_8C938;
    entity->unk13A = 0x50;
    entity->unk154 = 0;
    entity->unk156 = 0;
    entity->unk138 = 0;
    entity->unk13B = 0;
    entity->unk134 = entity->unk120;
}
#else
INCLUDE_ASM("asm/nonmatchings/20F0", initializeGameEntity);
#endif

void func_80002014_2C14(SceneModel *arg0) {
    func_80002040_2C40(arg0);
    freeNodeMemory(arg0);
}

void *func_80002040_2C40(SceneModel *arg0) {
    SceneModel_unk98 *temp_s0;
    SceneModel_unk0 *temp_s0_2;
    void *new_var;
    SceneModel_unk0 *temp_s0_3;
    if (arg0 == 0) {
        return 0;
    }
    arg0->unk3C = 1;
    arg0->unk120 = freeNodeMemory(arg0->unk120);
    arg0->unk11C = freeNodeMemory(arg0->unk11C);
    arg0->unk118 = func_8000CDFC_D9FC(arg0->unk118);
    arg0->unk114 = func_8000CDE0_D9E0(arg0->unk114);
    func_80009F5C_AB5C(((s32)arg0) + 0xA4);
    temp_s0 = arg0->unk98;
    temp_s0->unk24 = freeNodeMemory(temp_s0->unk24);
    temp_s0->unk28 = freeNodeMemory(temp_s0->unk28);
    new_var = freeNodeMemory(arg0->unk98);
    temp_s0_2 = arg0->unk0;
    arg0->unk98 = new_var;
    temp_s0_2->unk420 = freeNodeMemory(temp_s0_2->unk420);
    temp_s0_2->unk424 = freeNodeMemory(temp_s0_2->unk424);
    temp_s0_3 = arg0->unk0;
    temp_s0_3->unk3E4 = freeNodeMemory(temp_s0_3->unk3E4);
    temp_s0_3->unk3E8 = freeNodeMemory(temp_s0_3->unk3E8);
    temp_s0_3->unk3EC = freeNodeMemory(temp_s0_3->unk3EC);
    arg0->unk4 = freeNodeMemory(arg0->unk4);
    arg0->unk8 = freeNodeMemory(arg0->unk8);
    arg0->unk0->unk24 = freeNodeMemory(arg0->unk0->unk24);
    arg0->unk0->unk28 = freeNodeMemory(arg0->unk0->unk28);
    arg0->unk0 = freeNodeMemory(arg0->unk0);
    return arg0;
}

void func_80002170_2D70(func_80002B50_3750_arg *arg0, s16 arg1) {
    if (arg1 < arg0->unkE) {
        enqueueDisplayListObject(arg0->unk10->unk16, (void *)arg0->unk0 + (arg1 * 0x3C));
    }
}

void func_800021B8_2DB8(SceneModel *arg0, s16 arg1) {
    func_80002220_2E20(arg0, arg1, -1, -1, -1);
}

void func_800021E8_2DE8(SceneModel *arg0, s16 arg1, s16 arg2, s8 arg3) {
    func_80002220_2E20(arg0, arg1, arg2, arg3, -1);
}

void func_80002260_2E60(SceneModel *arg0, s16 arg1, s16 arg2, s8 arg3, short arg4, s8 arg5);

void func_80002220_2E20(SceneModel *arg0, s16 arg1, s16 arg2, s8 arg3, s16 arg4) {
    func_80002260_2E60(arg0, arg1, arg2, arg3, (s32)arg4, 1);
}

void func_80002260_2E60(SceneModel *arg0, s16 arg1, s16 arg2, s8 arg3, short arg4, s8 arg5) {
    int new_var;
    s16 var_t0;
    s16 var_a1;
    AssetGroup *entity;

    s16 local_arg2 = arg2;
    s16 local_arg1 = arg1;
    s16 local_arg4 = arg4;

    entity = &gameAssets[arg0->index];

    arg0->unk89 &= 0xFD;
    if (arg5) {
        arg0->unk89 |= 2;
    }

    if (arg0->unk8) {
        s16 temp_v0 = (local_arg1 < entity->unk22) ? -1 : 0;
        var_t0 = local_arg1 & temp_v0;
        temp_v0 = var_t0;

        if (local_arg2 >= entity->unk22) {
            local_arg2 = -1;
        }

        if (((s16)local_arg4) >= entity->unk22) {
            local_arg4 = -1;
        }

        new_var = -1;
        if (arg0->unk3A != new_var) {
            local_arg1 = local_arg4;
        }

        if ((arg0->unk16 != local_arg1) | ((~((s16)local_arg4)) != 0)) {
            arg0->unk95 = 0;
            arg0->unk8C = 0;
            temp_v0 = arg0->unk16;
            arg0->unk16 = var_t0;
            arg0->unk14 = temp_v0;

            if (arg3 == new_var) {
                if (var_t0 != local_arg2) {
                    arg0->unk38 = local_arg2;
                    arg0->unk8E = local_arg2;
                } else {
                    arg0->unk38 = -1;
                    arg0->unk8E = new_var;
                }
            } else {
                arg0->unk38 = local_arg2;
                arg0->unk8E = local_arg2;
            }

            arg0->unk3A = local_arg4;
            arg0->unk3E = arg3;
            arg0->unk90 = 0;

            if (arg0->unk3A == -1) {
                var_a1 = var_t0;
            } else {
                var_a1 = local_arg4;
                if (arg0->unk3E != -1) {
                    arg0->unk3E++;
                }
            }

            {
                s32 i;
                s32 count = func_8006097C_6157C(arg0->unk8, var_a1);
                for (i = 0; i < count; i++) {
                    func_8005DE98_5EA98(arg0->unk8, var_a1, (s16)i, arg0->unk4 + i);
                }
            }

            clearModelRotation(arg0);
        }
    }
}

void clearModelRotation(SceneModel *arg0) {
    setModelRotation(arg0, -1);
}

s32 setModelRotation(SceneModel *arg0, s16 arg1) {
    s8 local_buffer[0x20];
    s32 s3;
    s32 i;
    s32 s4;
    s32 s6;
    s32 s7;
    s16 s5 = arg1;
    s16 temp;

    memcpy(&local_buffer, &identityMatrix, sizeof(local_buffer));

    s7 = 0;
    s4 = 0;
    s6 = 0;

    temp = arg0->unk16;

    if (arg0->unk8 == 0) {
        return 0;
    }

    if (temp == (-1)) {
        return 0;
    }

    arg0->unk8C++;
    if (arg0->unk3E != 0) {
        temp = (arg0->unk3A != -1) ? arg0->unk3A : arg0->unk16;
        s3 = func_8006097C_6157C(arg0->unk8, temp);
        if (s3 >= 32) {
            s3 = 31;
        }

        if (arg1 == -1) {
            for (i = 0; i < s3; i++) {
                s4 |= func_8005ECB8_5F8B8(arg0->unk8, temp, (s16)i, &arg0->unk4[i]);
                s6 |= 1 << i;
                arg0->unk90 |= s4 << i;
            }
        } else {
            for (i = 0; i < s3; i++) {
                func_8005E800_5F400(&arg0->unk4[i], arg1 & 0xFFFF);
            }
        }
    } else if (arg0->unk38 != -1) {
        arg0->unk14 = arg0->unk16;
        arg0->unk16 = arg0->unk38;
        arg0->unk3E = -1;
        arg0->unk38 = -1;

        s3 = func_8006097C_6157C(arg0->unk8, arg0->unk16);
        for (i = 0; i < s3; i++) {
            func_8005DE98_5EA98(arg0->unk8, arg0->unk16, (s16)i, &arg0->unk4[i]);
        }
    }

    if ((arg0->unk90 & s6) == s6) {
        s7 = 1;
        arg0->unk90 = 0;
    }

    if (s7 != 0) {
        arg0->unk95++;
        s7 = arg0->unk95;
        if (arg0->unk3E > 0) {
            arg0->unk3E--;
        }

        arg0->unk3A = -1;
        if (arg0->unk16 != arg0->unk8E) {
            if (arg0->unk16 != arg0->unk14) {
                arg0->unk8A = arg0->unk8C;
            }
            if (arg0->unk16 != arg0->unk8E) {
                arg0->unk8C = 0;
                return s7;
            }
        }

        if (arg0->unk16 == arg0->unk14) {
            if (arg0->unk8A != arg0->unk8C) {
                arg0->unk8A = arg0->unk8C;
            }
        }

        arg0->unk8C = 0;
    }

    return s7;
}

INCLUDE_ASM("asm/nonmatchings/20F0", updateModelGeometry);

void func_80002B50_3750(func_80002B50_3750_arg *arg0, DisplayListObject *arg1) {
    if ((arg0 != NULL) && (arg0->unk88 != 0) && (arg0->unk3F != 0)) {
        enqueueDisplayListObject(arg0->unk10->unk16, arg1);
    }
}

s32 func_80002B8C_378C(SceneModel *arg0) {
    s32 new_var;
    if (func_80001904_2504(arg0->index) == 0) {
        return 0;
    }
    new_var = func_800018F4_24F4(arg0);
    if (new_var != 0) {
        return 0;
    }
    new_var = 0x90;
    return new_var;
}

s32 func_80002BD0_37D0(SceneModel *arg0) {
    u32 index = arg0->index;
    if (func_80001904_2504(index) == 0) {
        return 8;
    }

    if (func_800018F4_24F4(arg0) != 0) {
        index = 8;
        return index;
    }

    return 0x90;
}

void func_80002C14_3814(func_80002C14_3814_arg *arg0, void *arg1, s32 arg2) {
    OutputStruct_19E80 result;
    getTableEntryByU16Index(arg1, (s16)arg2, &result);

    arg0->data_ptr = result.data_ptr;
    arg0->index_ptr = result.index_ptr;
    arg0->unk2C = result.field1;
    arg0->unk2D = result.field2;
}