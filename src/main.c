#include "main.h"

#include "4050.h"
#include "5520.h"
#include "90F0.h"
#include "common.h"
#include "displaylist.h"
#include "geometry.h"
#include "rand.h"
#include "task_scheduler.h"

typedef struct {
    u8 padding[0x2C];
    void *unk2C;
    void *unk30;
} func_80000968_1568_arg;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s16 unk14;
    s16 unk16;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
} SubEntry;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s16 unk18;
    s16 unk1A;
    s32 unk1C;
    s32 unk20;
} SubEntryVariant;

typedef struct {
    SubEntry *sub_entries;
    u32 param1;
    u32 color1;
    u32 color2;
    char name[8];
    u32 addr1;
    u32 addr2;
    u32 addr3;
    u32 addr4;
    u32 size;
    void *ptr1;
    u32 param2;
    void *ptr2;
    u32 param3;
} DataEntry; // Total: 60 bytes
extern DataEntry D_800891D4_89DD4[];

typedef struct {
    char padding[44];
    void *unk2C;
    void *unk30;
} func_80000BF4_17F4_arg;

typedef struct {
    char padding[0x2C];
    s32 *unk2C;
    s32 *unk30;
    char padding2[0x10];
    s32 *unk44;
} func_80001114_1D14_arg;

typedef struct {
    void *unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s16 unk14;
    s16 unk16;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
} ModelEntityTaskConfig;

typedef struct {
    u8 unk0;
    s32 unk4;
    /* 0x8 */ void *asset1Start;
    /* 0xC */ void *asset1End;
    /* 0x10 */ void *asset2Start;
    /* 0x14 */ void *asset2End;
    /* 0x18 */ s32 asset2Size;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    ModelEntityTaskConfig *taskConfigs;
    s16 taskCount;
    u16 unk32;
    u8 diffuseColorR;
    u8 diffuseColorG;
    u8 diffuseColorB;
    u8 unk37;
    u8 ambientColorR;
    u8 ambientColorG;
    u8 ambientColorB;
    u8 unk3B;
} ModelEntityConfig;
extern ModelEntityConfig modelEntityConfigs[];

typedef struct {
    struct {
        u8 padding[0x14];
        s16 unk14;
        u16 unk16;
    } *ptr;
    u8 padding2[0x80];
    s16 unk84;
    s8 unk86;
    s8 unk87;
} func_80000C2C_182C_arg_unk0;

typedef struct {
    func_80000C2C_182C_arg_unk0 *unk0;
    s8 unk4;
    s8 unk5;
    Mat3x3Padded unk8;
    s32 unk28;
    void *unk2C;
    void *unk30;
    s32 unk34;
    u8 padding[0xC];
    u16 unk44;
    u16 unk46;
    u16 unk48;
    u16 unk4A;
    s16 unk4C;
    s16 unk4E;
    u16 unk50;
    u16 unk52;
    s32 unk54;
    s32 unk58;
    s32 unk5C;
    s32 unk60;
} func_80000C2C_182C_arg;

typedef struct {
    func_80000C2C_182C_arg_unk0 *unk0;
    s8 unk4;
    s8 unk5;
    u8 pad6[2];
    Mat3x3Padded unk8;
    s32 unk28;
    void *unk2C;
    void *unk30;
    s32 unk34;
    u8 padding[0xC];
    void *unk44;
    u8 pad48[4];
    s16 unk4C;
    s16 unk4E;
    u16 unk50;
    u16 unk52;
    s16 unk54;
    s16 unk56;
    s16 unk58;
} func_80000F4C_1B4C_arg;

typedef struct {
    u8 padding[0x8];
    func_80004FF8_5BF8_arg1 *unk8;
} func_800014C8_20C8_arg;

typedef struct {
    u8 padding[0x8];
    void *unk8;
} func_80000DA4_19A4_arg;

typedef struct {
    u8 padding[0x84];
    s16 unk84;
} func_80000DC0_19C0_arg_unk0;

typedef struct {
    func_80000DC0_19C0_arg_unk0 *unk0;
    s8 unk4;
    u8 padding[3];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    void *unk2C;
    void *unk30;
    s32 unk34;
} func_80000DC0_19C0_arg;

s32 D_800894F0_8A0F0[8];

void func_80000DA4_19A4(func_80000DA4_19A4_arg *arg0);
void func_80001264_1E64(func_80000C2C_182C_arg *arg0);
void func_800011DC_1DDC(func_80000C2C_182C_arg *);
void func_80001280_1E80(func_80000C2C_182C_arg *arg0);
void func_80000CAC_18AC(func_80000C2C_182C_arg *);
void func_80000A68_1668(func_80000C2C_182C_arg *);
void func_80000BF4_17F4(func_80000BF4_17F4_arg *);

typedef struct {
    func_80000C2C_182C_arg_unk0 *unk0;
    s8 unk4;
    s8 unk5;
    DisplayListObject unk8;
    u16 unk44;
    u16 unk46;
    u16 unk48;
    u16 unk4A;
    s16 unk4C;
    s16 unk4E;
    u16 unk50;
    u16 unk52;
    s32 unk54;
    s32 unk58;
    s32 unk5C;
    s32 unk60;
} func_80001040_1C40_arg;

void enqueueDisplayListIfVisible(func_80000C2C_182C_arg_unk0 *state, void *displayList);
void func_800013B8_1FB8(func_80000C2C_182C_arg *arg0);
void func_800014C8_20C8(func_800014C8_20C8_arg *arg0);
void updateRotatingModelTask(func_80000C2C_182C_arg *arg0);
void func_80000968_1568(func_80000968_1568_arg *arg0);
void func_80000E84_1A84(func_80000C2C_182C_arg *arg0);
void func_80000F14_1B14(func_80000BF4_17F4_arg *arg0);
void func_80001040_1C40(func_80001040_1C40_arg *arg0);
void func_80001114_1D14(func_80001114_1D14_arg *arg0);

extern s32 identityMatrix[];

void setModelRenderMode(setModelRenderMode_arg *arg0, s8 arg1) {
    arg0->unk87 = arg1;
}

s32 osVoiceCheckWord(u8 *data) {
    return 0xE;
}

void setupModelEntityLighting(ModelEntity *entity, ColorData *lightColors, ColorData *ambientColor) {
    s32 temp_v0;
    s32 quarterAverage;
    int colorSum;
    s32 halfAverage;
    s32 average;
    u8 blueValue;
    s32 new_var;
    ModelEntityConfig *config;

    config = &modelEntityConfigs[entity->configIndex];
    colorSum = config->diffuseColorR + config->diffuseColorG;
    temp_v0 = colorSum;
    blueValue = config->diffuseColorB;
    new_var = (temp_v0 + blueValue) / 3;
    average = new_var;
    temp_v0 = ((u32)new_var) >> 0x1F;
    temp_v0 = average + ((s8)temp_v0);
    new_var = temp_v0 >> 0x1;
    halfAverage = new_var;
    if (average < 0) {
        average += 3;
    }
    lightColors[0].r2 = 0;
    lightColors[0].g2 = 0x7F;
    lightColors[0].b2 = 0x7F;
    lightColors[0].r = config->diffuseColorR;
    lightColors[0].g = config->diffuseColorG;
    quarterAverage = average >> 2;
    lightColors[0].b = config->diffuseColorB;
    lightColors[1].r = quarterAverage;
    lightColors[1].g = quarterAverage;
    lightColors[1].b = quarterAverage;
    lightColors[1].r2 = 0x7F;
    lightColors[1].g2 = 0x7F;
    lightColors[1].b2 = 0;
    lightColors[2].r2 = -0x7F;
    lightColors[2].g2 = 0x7F;
    lightColors[2].b2 = 0;
    lightColors[2].r = halfAverage;
    lightColors[2].g = halfAverage;
    lightColors[2].b = halfAverage;
    ambientColor[0].r = config->ambientColorR;
    ambientColor[0].g = config->ambientColorG;
    ambientColor[0].b = config->ambientColorB;
    func_8006FC70_70870(entity->parent->unkDA, 3, lightColors, ambientColor);
}

typedef struct {
    void *unk0;
    u8 unk4;
    u8 unk5;
} initModelEntity_task;

s32 initModelEntity(ModelEntity *entity, s16 index, void *arg2) {
    ModelEntityConfig *entry;
    s32 i;

    entity->configIndex = index;
    entity->isVisible = 1;
    entry = &modelEntityConfigs[index];
    entity->parent = arg2;
    entity->isDisposed = 0;

    if (entry->unk32 == 0xFFFF) {
        entity->modelData = loadUncompressedData(entry->asset1Start, entry->asset1End);
        entity->textureData = loadCompressedData(entry->asset2Start, entry->asset2End, entry->asset2Size);

        memcpy(&entity->primaryMatrix, &identityMatrix, 0x20);

        entity->animState = 0;
        entity->activeModel = entity->modelData;
        entity->activeTexture = entity->textureData;
        entity->displayConfig = entry->unk1C;

        if (entry->unk24 != 0) {
            memcpy(&entity->secondaryMatrix, identityMatrix, 0x20);
            entity->secondaryModel = entity->modelData;
            entity->secondaryTexture = entity->textureData;
            entity->secondaryConfig = entry->unk24;
        } else {
            entity->secondaryConfig = 0;
            entity->secondaryTexture = 0;
            entity->secondaryModel = 0;
        }

        entity->unk74 = 0;

        for (i = 0; i < entry->taskCount; i++) {
            initModelEntity_task *task = scheduleTask((entry->taskConfigs + i)->unk0, 0, 0, 0xC8);
            if (task != NULL) {
                task->unk0 = entity;
                task->unk4 = i;
                task->unk5 = 0;
            }
        }

    } else {
        entity->modelData = 0;
        entity->textureData = 0;
        entity->activeModel = 0;
        entity->activeTexture = 0;
        entity->animState = 0;
        entity->displayConfig = 0;
        entity->secondaryModel = 0;
        entity->secondaryTexture = 0;
        entity->secondaryConfig = 0;
        entity->unk74 = 0;

        return 1;
    }

    return 0;
}

void freeEffectResources(EffectState *state) {
    if (state->unk4 != 0) {
        state->unk8 = freeNodeMemory(state->unk8);
        state->unk4 = freeNodeMemory(state->unk4);
    }
    state->isDisposed = 1;
}

void renderModelEntity(ModelEntityRenderState *state) {
    if (state->isVisible != 0) {
        if (state->primaryDisplayList.unk24 != 0) {
            enqueueDisplayListObject(state->parent->slotIndex, &state->primaryDisplayList);
        }
        if (state->hasSecondaryDisplayList != 0) {
            func_800630F0_63CF0(state->parent->slotIndex, &state->secondaryDisplayList);
        }
    }
}

void enqueueDisplayListIfVisible(func_80000C2C_182C_arg_unk0 *state, void *displayList) {
    if (state->unk87 != 0) {
        enqueueDisplayListObject(state->ptr->unk16, displayList);
    }
}

void initRotatingModelTask(func_80000C2C_182C_arg *arg0) {
    ModelEntityConfig *config = &modelEntityConfigs[arg0->unk0->unk84];
    ModelEntityTaskConfig *taskConfig = &config->taskConfigs[arg0->unk4];

    setCleanupCallback(&func_80000968_1568);
    memcpy(&arg0->unk8, &identityMatrix, sizeof(Mat3x3Padded));

    arg0->unk2C = loadUncompressedData(config->asset1Start, config->asset1End);
    arg0->unk30 = loadCompressedData(config->asset2Start, config->asset2End, config->asset2Size);
    arg0->unk34 = 0;
    arg0->unk28 = taskConfig->unk4;
    arg0->unk8.unk14 = taskConfig->unk8;
    arg0->unk8.unk18 = taskConfig->unkC;
    arg0->unk8.unk1C = taskConfig->unk10;
    arg0->unk44 = 0;

    setCallback(&updateRotatingModelTask);
}

void updateRotatingModelTask(func_80000C2C_182C_arg *arg0) {
    u16 rotationAngle;
    DataEntry *entry = &D_800891D4_89DD4[arg0->unk0->unk84];
    SubEntry *subEntry = &entry->sub_entries[arg0->unk4];

    if (arg0->unk0->unk86 != 0) {
        func_80069CF8_6A8F8();
    }

    rotationAngle = ((SubEntryVariant *)subEntry)->unk14;
    rotationAngle = arg0->unk44 + rotationAngle;
    arg0->unk44 = rotationAngle;

    createYRotationMatrix(&arg0->unk8, rotationAngle);
    enqueueDisplayListIfVisible(arg0->unk0, &arg0->unk8);
}

void func_80000968_1568(func_80000968_1568_arg *arg0) {
    arg0->unk30 = freeNodeMemory(arg0->unk30);
    arg0->unk2C = freeNodeMemory(arg0->unk2C);
}

void func_800009A0_15A0(func_80000C2C_182C_arg *arg0) {
    ModelEntityConfig *temp_s0;
    ModelEntityTaskConfig *temp_s2;

    temp_s0 = &modelEntityConfigs[arg0->unk0->unk84];
    temp_s2 = &temp_s0->taskConfigs[arg0->unk4];

    setCleanupCallback(&func_80000BF4_17F4);

    memcpy(&arg0->unk8, &identityMatrix, 0x20);

    arg0->unk2C = loadUncompressedData(temp_s0->asset1Start, temp_s0->asset1End);
    arg0->unk30 = loadCompressedData(temp_s0->asset2Start, temp_s0->asset2End, temp_s0->asset2Size);
    arg0->unk34 = 0;
    arg0->unk28 = temp_s2->unk4;
    arg0->unk44 = 0;

    setCallback(&func_80000A68_1668);
}

void func_80000A68_1668(func_80000C2C_182C_arg *arg0) {
    Mat3x3Padded yRotMatrix;
    Mat3x3Padded zRotMatrix;
    DataEntry *entry;
    SubEntry *subEntry;

    entry = &D_800891D4_89DD4[arg0->unk0->unk84];
    subEntry = &entry->sub_entries[arg0->unk4];
    if (arg0->unk0->unk86 != 0) {
        func_80069CF8_6A8F8();
    }

    switch (arg0->unk5) {
        case 0:
            arg0->unk44 += subEntry->unk18;
            if (subEntry->unk18 > 0) {
                if ((s16)arg0->unk44 > subEntry->unk1C) {
                    arg0->unk5 = 1;
                }
            } else {
                if ((s16)arg0->unk44 < -subEntry->unk1C) {
                    arg0->unk5 = 1;
                }
            }
            break;

        case 1:
            arg0->unk44 -= subEntry->unk18;
            if (subEntry->unk18 > 0) {
                if ((s16)arg0->unk44 < -subEntry->unk1C) {
                    arg0->unk5 = 0;
                }
            } else {
                if ((s16)arg0->unk44 > subEntry->unk1C) {
                    arg0->unk5 = 0;
                }
            }
            break;
    }

    createYRotationMatrix(&yRotMatrix, subEntry->unk16);
    createZRotationMatrix(&zRotMatrix, arg0->unk44);
    func_8006B084_6BC84(&zRotMatrix, &yRotMatrix, &arg0->unk8);

    arg0->unk8.unk14 = subEntry->unk8;
    arg0->unk8.unk18 = subEntry->unkC;
    arg0->unk8.unk1C = subEntry->unk10;

    enqueueDisplayListIfVisible(arg0->unk0, &arg0->unk8);
}

void func_80000BF4_17F4(func_80000BF4_17F4_arg *arg0) {
    arg0->unk30 = freeNodeMemory(arg0->unk30);
    arg0->unk2C = freeNodeMemory(arg0->unk2C);
}

void func_80000C2C_182C(func_80000C2C_182C_arg *arg0) {
    DataEntry *entry = &D_800891D4_89DD4[arg0->unk0->unk84];
    SubEntry *subEntry = &entry->sub_entries[arg0->unk4];
    setCleanupCallback(&func_80000DA4_19A4);
    func_80009E68_AA68((SpriteAssetState *)&arg0->unk8, subEntry->unk16);
    setCallback(&func_80000CAC_18AC);
}

void func_80000CAC_18AC(func_80000C2C_182C_arg *arg0) {
    DataEntry *entry = &D_800891D4_89DD4[arg0->unk0->unk84];
    SubEntryVariant *subEntry = (SubEntryVariant *)&entry->sub_entries[arg0->unk4];

    if (arg0->unk0->unk86 != 0) {
        func_80069CF8_6A8F8();
    }

    switch (arg0->unk5) {
        case 0:
            func_80009F90_AB90(&arg0->unk8, 0x10000, subEntry->unk1A, -1);
            arg0->unk5 = 1;
            break;
        case 1:
            func_8000A030_AC30(&arg0->unk8, 0x10000);
            break;
    }

    if (arg0->unk0->unk87 != 0) {
        func_8000A13C_AD3C(
            &arg0->unk8,
            arg0->unk0->ptr->unk16,
            subEntry->unk8,
            subEntry->unkC,
            subEntry->unk10,
            subEntry->unk1C,
            subEntry->unk1C,
            0,
            0
        );
    }
}

void func_80000DA4_19A4(func_80000DA4_19A4_arg *arg0) {
    func_80009F5C_AB5C((func_80009F5C_AB5C_arg **)&arg0->unk8);
}

void func_80000DC0_19C0(func_80000DC0_19C0_arg *arg0) {
    ModelEntityConfig *var_s0;
    ModelEntityTaskConfig *var_s2;
    s16 temp;
    s8 temp2;

    temp = arg0->unk0->unk84;
    temp2 = arg0->unk4;
    var_s0 = &modelEntityConfigs[temp];
    var_s2 = &var_s0->taskConfigs[temp2];

    setCleanupCallback(func_80000F14_1B14);

    memcpy(&arg0->unk8, &identityMatrix, 0x20);

    arg0->unk2C = loadUncompressedData(var_s0->asset1Start, var_s0->asset1End);
    arg0->unk30 = loadCompressedData(var_s0->asset2Start, var_s0->asset2End, var_s0->asset2Size);
    arg0->unk34 = 0;
    arg0->unk28 = var_s2->unk4;

    setCallback(func_80000E84_1A84);
}

void func_80000E84_1A84(func_80000C2C_182C_arg *arg0) {
    DataEntry *entry = &D_800891D4_89DD4[arg0->unk0->unk84];
    SubEntry *subEntry = &entry->sub_entries[arg0->unk4];

    if (arg0->unk0->unk86 != 0) {
        func_80069CF8_6A8F8();
    }

    arg0->unk8.unk14 = subEntry->unk8;
    arg0->unk8.unk18 = subEntry->unkC;
    arg0->unk8.unk1C = subEntry->unk10;
    enqueueDisplayListIfVisible(arg0->unk0, &arg0->unk8);
}

void func_80000F14_1B14(func_80000BF4_17F4_arg *arg0) {
    arg0->unk30 = freeNodeMemory(arg0->unk30);
    arg0->unk2C = freeNodeMemory(arg0->unk2C);
}

void func_80000F4C_1B4C(func_80000F4C_1B4C_arg *arg0) {
    ModelEntityConfig *temp_s0;
    ModelEntityTaskConfig *temp_s2;

    temp_s0 = &modelEntityConfigs[arg0->unk0->unk84];
    temp_s2 = &temp_s0->taskConfigs[arg0->unk4];

    setCleanupCallback(&func_80001114_1D14);
    memcpy(&arg0->unk8, &identityMatrix, 0x20);

    arg0->unk2C = loadUncompressedData(temp_s0->asset1Start, temp_s0->asset1End);
    arg0->unk30 = loadCompressedData(temp_s0->asset2Start, temp_s0->asset2End, temp_s0->asset2Size);
    arg0->unk34 = 0;
    arg0->unk28 = temp_s2->unk4;
    arg0->unk44 = func_80009EF8_AAF8(temp_s2->unk16);
    arg0->unk4C = 0;
    arg0->unk4E = 0;
    arg0->unk50 = temp_s2->unk18;
    arg0->unk52 = temp_s2->unk1C;
    arg0->unk54 = 0;
    arg0->unk56 = 0;
    arg0->unk58 = 0;

    setCallback(&func_80001040_1C40);
}

void func_80001040_1C40(func_80001040_1C40_arg *arg0) {
    DataEntry *entry;
    SubEntry *subEntry;

    entry = &D_800891D4_89DD4[arg0->unk0->unk84];
    subEntry = &entry->sub_entries[arg0->unk4];

    if (arg0->unk0->unk86 != 0) {
        func_80069CF8_6A8F8();
    }

    arg0->unk4C = arg0->unk4C + arg0->unk50;
    arg0->unk4E = arg0->unk4E + arg0->unk52;
    arg0->unk4C = (u8)arg0->unk4C;
    arg0->unk4E = (u8)arg0->unk4E;
    arg0->unk8.unk10.position.x = subEntry->unk8;
    arg0->unk8.unk10.position.y = subEntry->unkC;
    arg0->unk8.unk10.position.z = subEntry->unk10;

    if (arg0->unk0->unk87 != 0) {
        func_80004FF8_5BF8(arg0->unk0->ptr->unk16, &arg0->unk8);
    }
}

void func_80001114_1D14(func_80001114_1D14_arg *arg0) {
    arg0->unk44 = freeNodeMemory(arg0->unk44);
    arg0->unk30 = freeNodeMemory(arg0->unk30);
    arg0->unk2C = freeNodeMemory(arg0->unk2C);
}

void func_80001158_1D58(func_80000C2C_182C_arg *arg0) {
    DataEntry *entry = &D_800891D4_89DD4[arg0->unk0->unk84];
    SubEntry *subEntry = &entry->sub_entries[arg0->unk4];
    setCleanupCallback(&func_80001264_1E64);
    func_80009E68_AA68((SpriteAssetState *)&arg0->unk8, subEntry->unk16);
    *(s16 *)&arg0->unk54 = 0;
    setCallback(&func_800011DC_1DDC);
}

void func_800011DC_1DDC(func_80000C2C_182C_arg *arg0) {
    s16 temp;
    func_80000C2C_182C_arg *task;
    u8 tempUnk4;

    if (arg0->unk0->unk86 != 0) {
        func_80069CF8_6A8F8();
    }

    temp = *(s16 *)&arg0->unk54;
    if (temp <= 0) {
        task = scheduleTask(func_80001280_1E80, 0, 0, 0xC8);
        if (task != NULL) {
            task->unk0 = arg0->unk0;
            tempUnk4 = arg0->unk4;
            task->unk5 = 0;
            task->unk4 = tempUnk4;
        } else {
            *(s16 *)&arg0->unk54 = 0;
        }
    } else {
        *(s16 *)&arg0->unk54 = temp - 1;
    }
}

void func_80001264_1E64(func_80000C2C_182C_arg *arg0) {
    func_80009F5C_AB5C((func_80009F5C_AB5C_arg **)&arg0->unk8);
}

void func_80001280_1E80(func_80000C2C_182C_arg *arg0) {
    volatile s32 sp10;
    volatile s32 sp14;
    volatile s32 sp18;
    volatile s32 sp1C;
    Mat3x3Padded sp20;
    DataEntry *entry;
    SubEntry *subEntry;

    entry = &D_800891D4_89DD4[arg0->unk0->unk84];
    subEntry = &entry->sub_entries[arg0->unk4];

    memcpy(&sp20, &identityMatrix, sizeof(Mat3x3Padded));
    setCleanupCallback(&func_800014C8_20C8);
    func_80009E68_AA68((SpriteAssetState *)&arg0->unk8, subEntry->unk16);

    sp10 = ((randA() & 0x1F) - 0x10) << 0x10;
    sp14 = ((randA() & 1) - 4) << 0x10;
    sp18 = 0;

    createYRotationMatrix(&sp20, 0x1D83);
    transformVector((s16 *)&sp10, (s16 *)&sp20, &arg0->unk54);

    arg0->unk54 = arg0->unk54 + subEntry->unk8;
    arg0->unk58 = arg0->unk58 + subEntry->unkC;
    arg0->unk5C = arg0->unk5C + subEntry->unk10;
    arg0->unk60 = D_800894F0_8A0F0[randA() & 3];

    setCallback(&func_800013B8_1FB8);
}

void func_800013B8_1FB8(func_80000C2C_182C_arg *arg0) {
    DataEntry *entry;
    SubEntryVariant *subEntry;

    entry = &D_800891D4_89DD4[arg0->unk0->unk84];
    subEntry = (SubEntryVariant *)&entry->sub_entries[arg0->unk4];

    if (arg0->unk0->unk86 != 0) {
        func_80069CF8_6A8F8();
    }

    switch (arg0->unk5) {
        case 0:
            func_80009F90_AB90(&arg0->unk8, 0x10000, subEntry->unk1A, -1);
            arg0->unk5 = 1;
            break;
        case 1:
            if (func_8000A030_AC30(&arg0->unk8, 0x10000) != 0) {
                func_80069CF8_6A8F8();
                return;
            }
            break;
    }

    if (arg0->unk0->unk87 != 0) {
        func_8000A13C_AD3C(
            &arg0->unk8,
            arg0->unk0->ptr->unk16,
            arg0->unk54,
            arg0->unk58,
            arg0->unk5C,
            arg0->unk60,
            arg0->unk60,
            0,
            0
        );
    }
}

void func_800014C8_20C8(func_800014C8_20C8_arg *arg0) {
    func_80009F5C_AB5C((func_80009F5C_AB5C_arg **)&arg0->unk8);
}
