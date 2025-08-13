#include "displaylist.h"

#include "19E80.h"
#include "68CF0.h"
#include "6E840.h"
#include "common.h"
#include "gamestate.h"
#include "gbi.h"
#include "geometry.h"
#include "overlay.h"
#include "task_scheduler.h"

USE_OVERLAY(rand);
USE_ASSET(_215D70);

typedef struct {
    u8 padding[0x20];
    s32 *unk20;
    u8 padding3[0x9];
    s32 unk30;
    u8 padding2[0x3];
    s8 unk37;
} enqueueMultiPartDisplayList_arg1;

typedef struct {
    u8 padding[0x10];
    u8 *data_ptr;
    TableEntry_19E80 *index_ptr;
    s8 unk18;
    u8 unk19;
} loadAssetMetadataByIndex_arg;

typedef struct {
    u8 padding[0x148];
    u8 unk148;
    u8 unk149;
    u8 unk14A;
    u8 padding2[0xD];
    u8 unk158;
    u8 unk159;
    u8 unk15A;
} D_800AB068_A23D8_arg;

typedef struct {
    s16 unk0;
    u8 _pad[0x22];
} func_80062C98_63898_arg;

extern D_800AB068_A23D8_arg *D_800AB068_A23D8;
extern Gfx *gRegionAllocPtr;
extern s16 gGraphicsMode;
extern s32 D_800A2D40_A3940;
extern s32 D_800A2D44_A3944;
extern s32 D_800A2D48_A3948;
extern void func_8006395C_6455C(void);
extern void func_80065150_65D50(void);
extern void func_800653E0_65FE0(void);
extern void func_80065670_66270(void);
extern void func_800659E4_665E4(void);
extern void func_80065DD8_669D8(void);
extern void func_80066474_67074(void);
extern void func_800670D4_67CD4(void);
extern void func_800680C4_68CC4(void);
extern void guLookAtReflect(
    Mtx *m,
    LookAt *l,
    float xEye,
    float yEye,
    float zEye,
    float xAt,
    float yAt,
    float zAt,
    float xUp,
    float yUp,
    float zUp
);

void func_80062CF0_638F0();
void setupDisplayListMatrix(DisplayListObject *);
void func_80063A94_64694(void *);
void func_800648EC_654EC(void);
void *func_8006C130_6CD30(void *, LookAt *);
void func_80068060_68C60(void);

void parseGameDataLayout(GameDataLayout *gameData) {
    u16 *parser;
    u16 section1Count, section2Count, configValue;

    parser = gameData->dataStart;
    section1Count = parser[0];
    parser = parser + 1;
    gameData->section1Data = parser;

    parser = parser + section1Count * 3;
    section2Count = parser[0];
    parser = parser + 1;
    gameData->section2Data = parser;

    parser = parser + section2Count * 4;
    configValue = parser[0];
    parser = parser + 1;
    gameData->section3Data = parser;
    gameData->finalValue = configValue;
}

INCLUDE_ASM("asm/nonmatchings/displaylist", func_800609E8_615E8);

INCLUDE_ASM("asm/nonmatchings/displaylist", func_80060A3C_6163C);

INCLUDE_ASM("asm/nonmatchings/displaylist", func_80060CDC_618DC);

INCLUDE_ASM("asm/nonmatchings/displaylist", func_80061A64_62664);

INCLUDE_ASM("asm/nonmatchings/displaylist", func_80061D6C_6296C);

INCLUDE_ASM("asm/nonmatchings/displaylist", func_800620D0_62CD0);

INCLUDE_ASM("asm/nonmatchings/displaylist", func_80062254_62E54);

INCLUDE_ASM("asm/nonmatchings/displaylist", func_80062274_62E74);

INCLUDE_ASM("asm/nonmatchings/displaylist", func_800625A4_631A4);

INCLUDE_ASM("asm/nonmatchings/displaylist", func_800626C4_632C4);

INCLUDE_ASM("asm/nonmatchings/displaylist", func_80062918_63518);

INCLUDE_ASM("asm/nonmatchings/displaylist", func_80062B1C_6371C);

s32 func_80062C98_63898(func_80062C98_63898_arg **arg0, u16 index) {
    s16 temp_v0;
    s16 var_a1 = index;

    temp_v0 = arg0[3][index].unk0;

    if (temp_v0 >= 0) {
        var_a1 = temp_v0;
    }
    return var_a1 & 0xFFFF;
}

INCLUDE_ASM("asm/nonmatchings/displaylist", func_80062CD0_638D0);

INCLUDE_ASM("asm/nonmatchings/displaylist", func_80062CF0_638F0);

void func_8006300C_63C0C(DisplayListObject *arg0) {
    func_80062CF0_638F0();
    gSPDisplayList(gRegionAllocPtr++, arg0->unk20->unk4);
}

INCLUDE_ASM("asm/nonmatchings/displaylist", func_80063058_63C58);

void func_800630A4_63CA4(DisplayListObject *arg0) {
    func_80062CF0_638F0();
    gSPDisplayList(gRegionAllocPtr++, arg0->unk20->unkC);
}

INCLUDE_ASM("asm/nonmatchings/displaylist", func_800630F0_63CF0);

void setupDisplayListMatrix(DisplayListObject *arg0) {
    Mtx sp30;
    f32 sp70;
    f32 sp74;
    f32 sp78;
    f32 sp7C;
    f32 sp80;
    f32 sp84;
    LookAt *temp_v0;

    if (arg0->unk30 == NULL) {
        arg0->unk30 = (s32)arenaAlloc16(0x40);
        if (arg0->unk30 == NULL) {
            return;
        }
        func_8006C130_6CD30(arg0, (LookAt *)arg0->unk30);
    }

    if (arg0->unk20->unk0 & 1) {
        temp_v0 = (LookAt *)arenaAlloc16(0x20);
        if (temp_v0 == NULL) {
            return;
        }

        matrixToEulerAngles((s32 *)(D_800AB068_A23D8) + 0x48, (s32 *)arg0, &sp70, &sp74, &sp78, &sp7C, &sp80, &sp84);
        guLookAtReflect(&sp30, temp_v0, 0.0f, 0.0f, 0.0f, sp70, sp74, sp78, sp7C, sp80, sp84);
        gSPLookAt(gRegionAllocPtr++, temp_v0);
    }

    if (gGraphicsMode != 3) {
        gDPPipeSync(gRegionAllocPtr++);
        gDPSetTexturePersp(gRegionAllocPtr++, 0x80000);

        gGraphicsMode = 3;

        if (arg0->unk24 != 0) {
            gSPSegment(gRegionAllocPtr++, 1, arg0->unk24);
        }

        if (arg0->unk28 != 0) {
            gSPSegment(gRegionAllocPtr++, 2, arg0->unk28);
        }

        if (arg0->unk2C != 0) {
            gSPSegment(gRegionAllocPtr++, 3, arg0->unk2C);
        }

        D_800A2D40_A3940 = arg0->unk24;
        D_800A2D44_A3944 = arg0->unk28;
        D_800A2D48_A3948 = arg0->unk2C;
    } else {
        if (arg0->unk24 != D_800A2D40_A3940) {
            if (arg0->unk24 != 0) {
                gSPSegment(gRegionAllocPtr++, 1, arg0->unk24);
            }
            D_800A2D40_A3940 = arg0->unk24;
        }

        if (arg0->unk28 != D_800A2D44_A3944) {
            if (arg0->unk28 != 0) {
                gSPSegment(gRegionAllocPtr++, 2, arg0->unk28);
            }
            D_800A2D44_A3944 = arg0->unk28;
        }

        if (arg0->unk2C != D_800A2D48_A3948) {
            if (arg0->unk2C != 0) {
                gSPSegment(gRegionAllocPtr++, 3, arg0->unk2C);
            }
            D_800A2D48_A3948 = arg0->unk2C;
        }
    }

    gSPMatrix(gRegionAllocPtr++, arg0->unk30, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
}

void func_800634E8_640E8(DisplayListObject *arg0) {
    setupDisplayListMatrix(arg0);
    gSPDisplayList(gRegionAllocPtr++, arg0->unk20->unk4);
}

INCLUDE_ASM("asm/nonmatchings/displaylist", func_80063534_64134);

INCLUDE_ASM("asm/nonmatchings/displaylist", func_80063580_64180);

void enqueueDisplayListObject(s32 arg0, DisplayListObject *arg1) {
    arg1->unk30 = 0;
    if (arg1->unk20->unk4 != 0) {
        debugEnqueueCallback(arg0 & 0xFFFF, 1, &func_800634E8_640E8, arg1);
    }
    if (arg1->unk20->unk8 != 0) {
        debugEnqueueCallback(arg0 & 0xFFFF, 3, &func_80063534_64134, arg1);
    }
    if (arg1->unk20->unkC != 0) {
        debugEnqueueCallback(arg0 & 0xFFFF, 5, &func_80063580_64180, arg1);
    }
}

void func_80063668_64268(DisplayListObject *arg0) {
    if (func_80070140_70D40((void *)((s32)arg0 + 0x14)) == NULL) {
        setupDisplayListMatrix(arg0);
        gSPDisplayList(gRegionAllocPtr++, arg0->unk20->unk4);
    }
}

void func_800636C8_642C8(DisplayListObject *arg0) {
    if (func_80070140_70D40((void *)(s32)arg0 + 0x14) == 0) {
        setupDisplayListMatrix(arg0);
        gSPDisplayList(gRegionAllocPtr++, arg0->unk20->unk8);
    }
}

void func_80063728_64328(DisplayListObject *arg0) {
    s32 *temp_v1;

    if (func_80070140_70D40((void *)((s32)arg0 + 0x14)) == NULL) {
        setupDisplayListMatrix(arg0);
        gSPDisplayList(gRegionAllocPtr++, arg0->unk20->unkC);
    }
}

void enqueueDisplayListWithFrustumCull(s32 arg0, DisplayListObject *arg1) {
    arg1->unk30 = 0;
    if (arg1->unk20->unk4 != 0) {
        debugEnqueueCallback(arg0, 1, &func_80063668_64268, arg1);
    }
    if (arg1->unk20->unk8 != 0) {
        debugEnqueueCallback(arg0, 3, &func_800636C8_642C8, arg1);
    }
    if (arg1->unk20->unkC != 0) {
        debugEnqueueCallback(arg0, 5, &func_80063728_64328, arg1);
    }
}

void buildDisplayListSegment(DisplayListObject *arg0) {
    if (func_80070140_70D40((void *)((s32)arg0) + 0x14) == 0) {
        setupDisplayListMatrix(arg0);

        gDPPipeSync(gRegionAllocPtr++);

        gDPSetColor(gRegionAllocPtr++, 0xFB, arg0->unk3B | ~0xFF);

        gSPDisplayList(gRegionAllocPtr++, arg0->unk20->unk4);
    }
}

INCLUDE_ASM("asm/nonmatchings/displaylist", func_800638C0_644C0);

INCLUDE_ASM("asm/nonmatchings/displaylist", func_8006395C_6455C);

void func_800639F8_645F8(s32 arg0, DisplayListObject *arg1) {
    arg1->unk30 = 0;
    if (arg1->unk20->unk4 != 0) {
        debugEnqueueCallback(arg0 & 0xFFFF, 1, &buildDisplayListSegment, arg1);
    }
    if (arg1->unk20->unk8 != 0) {
        debugEnqueueCallback(arg0 & 0xFFFF, 3, &func_800638C0_644C0, arg1);
    }
    if (arg1->unk20->unkC != 0) {
        debugEnqueueCallback(arg0 & 0xFFFF, 5, &func_8006395C_6455C, arg1);
    }
}

INCLUDE_ASM("asm/nonmatchings/displaylist", func_80063A94_64694);

INCLUDE_ASM("asm/nonmatchings/displaylist", func_8006405C_64C5C);

INCLUDE_ASM("asm/nonmatchings/displaylist", func_800640BC_64CBC);

void func_8006411C_64D1C(DisplayListObject *arg0) {
    if (func_80070140_70D40((void *)(s32)arg0 + 0x14) == 0) {
        func_80063A94_64694(arg0);
        gSPDisplayList(gRegionAllocPtr++, arg0->unk20->unkC);
    }
}

INCLUDE_ASM("asm/nonmatchings/displaylist", func_8006417C_64D7C);

INCLUDE_ASM("asm/nonmatchings/displaylist", func_80064218_64E18);

INCLUDE_ASM("asm/nonmatchings/displaylist", func_800643AC_64FAC);

INCLUDE_ASM("asm/nonmatchings/displaylist", func_80064628_65228);

INCLUDE_ASM("asm/nonmatchings/displaylist", func_800646C8_652C8);

INCLUDE_ASM("asm/nonmatchings/displaylist", func_80064768_65368);

INCLUDE_ASM("asm/nonmatchings/displaylist", func_80064808_65408);

INCLUDE_ASM("asm/nonmatchings/displaylist", func_800648EC_654EC);

INCLUDE_ASM("asm/nonmatchings/displaylist", func_80064CF4_658F4);

void func_80064E34_65A34(DisplayListObject *arg0) {
    func_800648EC_654EC();

    gSPDisplayList(gRegionAllocPtr++, arg0->unk20->unk8);

    gSPLightColor(
        gRegionAllocPtr++,
        LIGHT_1,
        D_800AB068_A23D8->unk148 << 0x18 | D_800AB068_A23D8->unk149 << 0x10 | D_800AB068_A23D8->unk14A << 8
    );

    gSPLightColor(
        gRegionAllocPtr++,
        LIGHT_2,
        D_800AB068_A23D8->unk158 << 0x18 | D_800AB068_A23D8->unk159 << 0x10 | D_800AB068_A23D8->unk15A << 8
    );
}

INCLUDE_ASM("asm/nonmatchings/displaylist", func_80064F74_65B74);

INCLUDE_ASM("asm/nonmatchings/displaylist", func_800650B4_65CB4);

INCLUDE_ASM("asm/nonmatchings/displaylist", func_80065150_65D50);

INCLUDE_ASM("asm/nonmatchings/displaylist", func_800653E0_65FE0);

INCLUDE_ASM("asm/nonmatchings/displaylist", func_80065670_66270);

void enqueueMultiPartDisplayList(s32 arg0, enqueueMultiPartDisplayList_arg1 *arg1, s32 arg2) {
    enqueueMultiPartDisplayList_arg1 *new_var;
    s32 var_a1;
    s32 var_s0;
    s32 *temp_v1;
    enqueueMultiPartDisplayList_arg1 *var_a0;
    volatile u8 padding[0x1];
    var_a1 = 0;
    var_s0 = 0;
    arg1->unk37 = arg2;
    if (arg2 > 0) {
        var_a0 = arg1;
        do {
            temp_v1 = var_a0->unk20;
            (new_var = var_a0)->unk30 = 0;
            if (temp_v1[1] != 0) {
                var_s0 |= 1;
            }
            if (temp_v1[2] != 0) {
                var_s0 |= 2;
            }
            if (temp_v1[0x3] != 0) {
                var_s0 |= 4;
            }
            var_a1 += 1;
            var_a0 = (enqueueMultiPartDisplayList_arg1 *)(((s32)var_a0) + 0x3C);
        } while (var_a1 < arg2);
    }
    if (var_s0 & 1) {
        debugEnqueueCallback(arg0 & 0xFFFF, 1, &func_80065150_65D50, arg1);
    }
    new_var = arg1;
    if (var_s0 & 2) {
        debugEnqueueCallback((arg0 & 0xFFFF) ^ 0, 3, &func_800653E0_65FE0, new_var);
    }
    if (var_s0 & 4) {
        debugEnqueueCallback(arg0 & 0xFFFF, 5, &func_80065670_66270, arg1);
    }
}

INCLUDE_ASM("asm/nonmatchings/displaylist", func_800659E4_665E4);

void func_80065DA8_669A8(s32 arg0, DisplayListObject *arg1) {
    arg1->unk30 = 0;
    debugEnqueueCallback(arg0 & 0xFFFF, 0, &func_800659E4_665E4, arg1);
}

INCLUDE_ASM("asm/nonmatchings/displaylist", func_80065DD8_669D8);

void func_80066444_67044(s32 arg0, func_80066444_67044_arg1 *arg1) {
    arg1->unk1C = 0;
    debugEnqueueCallback(arg0 & 0xFFFF, 4, &func_80065DD8_669D8, arg1);
}

INCLUDE_ASM("asm/nonmatchings/displaylist", func_80066474_67074);

void func_80066AC0_676C0(s32 arg0, func_80066444_67044_arg1 *arg1) {
    arg1->unk30 = 0;
    debugEnqueueCallback(arg0 & 0xFFFF, 4, &func_80066474_67074, arg1);
}

INCLUDE_ASM("asm/nonmatchings/displaylist", func_80066AF0_676F0);

INCLUDE_ASM("asm/nonmatchings/displaylist", func_800670A4_67CA4);

INCLUDE_ASM("asm/nonmatchings/displaylist", func_800670D4_67CD4);

void func_800677C0_683C0(s32 arg0, func_80066444_67044_arg1 *arg1) {
    arg1->unk1C = 0;
    debugEnqueueCallback(arg0 & 0xFFFF, 6, &func_800670D4_67CD4, arg1);
}

INCLUDE_ASM("asm/nonmatchings/displaylist", func_800677F0_683F0);

INCLUDE_ASM("asm/nonmatchings/displaylist", func_80067EDC_68ADC);

void loadAssetMetadata(loadAssetMetadata_arg *arg0, void *arg1, s32 arg2) {
    OutputStruct_19E80 result;

    getTableEntryByU16Index(arg1, (s16)arg2, &result);
    arg0->data_ptr = result.data_ptr;
    arg0->index_ptr = result.index_ptr;
    arg0->unk18 = result.field1;
    arg0->unk19 = result.field2;
}

void loadAssetMetadataByIndex(
    loadAssetMetadataByIndex_arg *arg0,
    DataTable_19E80 *table,
    s32 entry_index,
    s32 sub_index
) {
    OutputStruct_19E80 result;

    getTableEntryByIndex(table, (u16)entry_index, (u8)sub_index, &result);
    arg0->data_ptr = result.data_ptr;
    arg0->index_ptr = result.index_ptr;
    arg0->unk18 = result.field1;
    arg0->unk19 = result.field2;
}

void initializeOverlaySystem(void) {
    void **temp_s0;

    temp_s0 = (void **)allocateTaskMemory(4);
    LOAD_OVERLAY(rand)
    *temp_s0 = dmaRequestAndUpdateStateWithSize(&_215D70_ROM_START, &_215D70_ROM_END, 0x918);
    setGameStateHandler(&func_80068060_68C60);
}

void func_80068060_68C60(void) {
    func_800680F0_68CF0(((GameState *)getCurrentAllocation())->audioPlayer0);
    terminateSchedulerWithCallback(&func_800680C4_68CC4);
}

void func_80068090_68C90(void) {
    if ((func_80069810_6A410() << 0x10) != 0) {
        terminateSchedulerWithCallback(&func_800680C4_68CC4);
    }
}

INCLUDE_ASM("asm/nonmatchings/displaylist", func_800680C4_68CC4);
