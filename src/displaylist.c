#include "displaylist.h"

#include "19E80.h"
#include "3E160.h"
#include "5AA90.h"
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
    u8 padding[0x120];
    s32 unk120;
    u8 padding2[0x24];
    u8 unk148;
    u8 unk149;
    u8 unk14A;
    u8 padding3[0xD];
    u8 unk158;
    u8 unk159;
    u8 unk15A;
} D_800AB068_A23D8_arg;

extern D_800AB068_A23D8_arg *D_800AB068_A23D8;
extern Gfx *gRegionAllocPtr;
extern s16 gGraphicsMode;
extern void *D_800A2D40_A3940;
extern void *D_800A2D44_A3944;
extern s32 D_800A2D48_A3948;
extern void func_80065150_65D50(void);
extern void func_800653E0_65FE0(void);
extern void func_80065670_66270(void);
extern void func_800659E4_665E4(void);
extern void func_80065DD8_669D8(void);
extern void func_80066474_67074(void);
extern void func_800670D4_67CD4(void);
extern void func_800680C4_68CC4(void);
extern void func_80064CF4_658F4(void);
extern void func_800643AC_64FAC(DisplayListObject *, s32);

void func_80062CF0_638F0(void);
void setupDisplayListMatrix(DisplayListObject *);
void func_80063A94_64694(void *);
void func_800648EC_654EC(void);
void func_80064F74_65B74(DisplayListObject *);
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

// cross2d calculates the cross product of two 2d vectors
s64 cross2d(s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2) {
    s64 product1;
    s64 product2;

    product1 = (s64)(x2 - x1) * (y0 - y1);
    product2 = (s64)(y2 - y1) * (x0 - x1);

    return product1 - product2;
}

INCLUDE_ASM("asm/nonmatchings/displaylist", func_80060A3C_6163C);

INCLUDE_ASM("asm/nonmatchings/displaylist", func_80060CDC_618DC);

INCLUDE_ASM("asm/nonmatchings/displaylist", func_80061A64_62664);

INCLUDE_ASM("asm/nonmatchings/displaylist", func_80061D6C_6296C);

INCLUDE_ASM("asm/nonmatchings/displaylist", func_800620D0_62CD0);

s16 func_80062254_62E54(GameDataLayout *arg0, u16 arg1) {
    return ((func_80062C98_63898_arg *)arg0->section3Data)[arg1].unkA;
}

INCLUDE_ASM("asm/nonmatchings/displaylist", func_80062274_62E74);

INCLUDE_ASM("asm/nonmatchings/displaylist", func_800625A4_631A4);

INCLUDE_ASM("asm/nonmatchings/displaylist", func_800626C4_632C4);

INCLUDE_ASM("asm/nonmatchings/displaylist", func_80062918_63518);

typedef struct {
    s16 x;
    s16 y;
    s16 z;
} Vertex6;

typedef struct {
    u8 padding[0x16];
    u16 unk16;
    u8 padding2[0x4];
    u16 unk1C;
    u8 padding3[0x6];
} Element24;

typedef struct {
    void *unk0;
    Vertex6 *unk4;
    void *unk8;
    Element24 *unkC;
} func_80062B1C_arg0;

u16 func_80062B1C_6371C(void *arg0_void, u16 arg1, void *arg2_void, void *arg3_void) {
    func_80062B1C_arg0 *arg0 = (func_80062B1C_arg0 *)arg0_void;
    Vec3s32 *arg2 = (Vec3s32 *)arg2_void;
    Vec3s32 *arg3 = (Vec3s32 *)arg3_void;
    Vertex6 *verts;
    u16 idx0;
    u16 idx1;
    Vertex6 *vert0;
    Vertex6 *vert1;

    arg2->unk0 = arg0->unk4[arg0->unkC[arg1].unk16].x << 16;
    arg2->unk4 = arg0->unk4[arg0->unkC[arg1].unk16].y << 16;
    arg2->unk8 = arg0->unk4[arg0->unkC[arg1].unk16].z << 16;

    arg3->unk0 = arg0->unk4[arg0->unkC[arg1].unk1C].x << 16;
    arg3->unk4 = arg0->unk4[arg0->unkC[arg1].unk1C].y << 16;
    arg3->unk8 = arg0->unk4[arg0->unkC[arg1].unk1C].z << 16;

    idx0 = arg0->unkC[arg1].unk16;
    verts = arg0->unk4;
    idx1 = arg0->unkC[arg1].unk1C;
    vert0 = (Vertex6 *)((s32)idx0 * sizeof(Vertex6) + (s32)verts);
    vert1 = (Vertex6 *)((s32)idx1 * sizeof(Vertex6) + (s32)verts);

    return (func_8006D21C_6DE1C(vert0->x, vert0->z, vert1->x, vert1->z) - 0x1000) & 0xFFFF;
}

s32 func_80062C98_63898(func_80062C98_63898_arg **arg0, u16 index) {
    s16 temp_v0;
    s16 var_a1 = index;

    temp_v0 = arg0[3][index].unk0;

    if (temp_v0 >= 0) {
        var_a1 = temp_v0;
    }
    return var_a1 & 0xFFFF;
}

s32 func_80062CD0_638D0(s32 arg0) {
    arg0 &= 0xF;

    if (arg0 == 1) {
        return 1;
    }
    if (arg0 == 2) {
        return 1;
    }
    return arg0;
}

INCLUDE_ASM("asm/nonmatchings/displaylist", func_80062CF0_638F0);

void func_8006300C_63C0C(DisplayListObject *arg0) {
    func_80062CF0_638F0();
    gSPDisplayList(gRegionAllocPtr++, arg0->unk20->opaqueDisplayList);
}

void func_80063058_63C58(DisplayListObject *arg0) {
    func_80062CF0_638F0();
    gSPDisplayList(gRegionAllocPtr++, arg0->unk20->transparentDisplayList);
}

void func_800630A4_63CA4(DisplayListObject *arg0) {
    func_80062CF0_638F0();
    gSPDisplayList(gRegionAllocPtr++, arg0->unk20->overlayDisplayList);
}

void func_800630F0_63CF0(s32 arg0, void *arg1) {
    ((DisplayListObject *)arg1)->unk30 = 0;

    if (((DisplayListObject *)arg1)->unk20->opaqueDisplayList != NULL) {
        debugEnqueueCallback(arg0, 1, func_8006300C_63C0C, arg1);
    }

    if (((DisplayListObject *)arg1)->unk20->transparentDisplayList != NULL) {
        debugEnqueueCallback(arg0, 3, func_80063058_63C58, arg1);
    }

    if (((DisplayListObject *)arg1)->unk20->overlayDisplayList != NULL) {
        debugEnqueueCallback(arg0, 5, func_800630A4_63CA4, arg1);
    }
}

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

    if (arg0->unk20->flags & 1) {
        temp_v0 = (LookAt *)arenaAlloc16(0x20);
        if (temp_v0 == NULL) {
            return;
        }

        matrixToEulerAngles(&D_800AB068_A23D8->unk120, (s32 *)arg0, &sp70, &sp74, &sp78, &sp7C, &sp80, &sp84);
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
    gSPDisplayList(gRegionAllocPtr++, arg0->unk20->opaqueDisplayList);
}

void func_80063534_64134(DisplayListObject *arg0) {
    DisplayListObject *obj = arg0;
    Gfx *dl;

    setupDisplayListMatrix(obj);

    dl = gRegionAllocPtr;
    dl->words.w0 = 0xDE000000;
    dl->words.w1 = (u32)obj->unk20->transparentDisplayList;
    gRegionAllocPtr = dl + 1;
}

void func_80063580_64180(DisplayListObject *arg0) {
    DisplayListObject *obj = arg0;
    Gfx *dl;

    setupDisplayListMatrix(obj);

    dl = gRegionAllocPtr;
    dl->words.w0 = 0xDE000000;
    dl->words.w1 = (u32)obj->unk20->overlayDisplayList;
    gRegionAllocPtr = dl + 1;
}

void enqueueDisplayListObject(s32 arg0, DisplayListObject *arg1) {
    arg1->unk30 = 0;
    if (arg1->unk20->opaqueDisplayList != NULL) {
        debugEnqueueCallback(arg0 & 0xFFFF, 1, &func_800634E8_640E8, arg1);
    }
    if (arg1->unk20->transparentDisplayList != NULL) {
        debugEnqueueCallback(arg0 & 0xFFFF, 3, &func_80063534_64134, arg1);
    }
    if (arg1->unk20->overlayDisplayList != NULL) {
        debugEnqueueCallback(arg0 & 0xFFFF, 5, &func_80063580_64180, arg1);
    }
}

void func_80063668_64268(DisplayListObject *arg0) {
    if (!isObjectCulled(&arg0->unk10.position)) {
        setupDisplayListMatrix(arg0);
        gSPDisplayList(gRegionAllocPtr++, arg0->unk20->opaqueDisplayList);
    }
}

void func_800636C8_642C8(DisplayListObject *arg0) {
    if (!isObjectCulled(&arg0->unk10.position)) {
        setupDisplayListMatrix(arg0);
        gSPDisplayList(gRegionAllocPtr++, arg0->unk20->transparentDisplayList);
    }
}

void func_80063728_64328(DisplayListObject *arg0) {
    s32 *temp_v1;

    if (!isObjectCulled(&arg0->unk10.position)) {
        setupDisplayListMatrix(arg0);
        gSPDisplayList(gRegionAllocPtr++, arg0->unk20->overlayDisplayList);
    }
}

void enqueueDisplayListWithFrustumCull(s32 arg0, DisplayListObject *arg1) {
    arg1->unk30 = 0;
    if (arg1->unk20->opaqueDisplayList != NULL) {
        debugEnqueueCallback(arg0, 1, &func_80063668_64268, arg1);
    }
    if (arg1->unk20->transparentDisplayList != NULL) {
        debugEnqueueCallback(arg0, 3, &func_800636C8_642C8, arg1);
    }
    if (arg1->unk20->overlayDisplayList != NULL) {
        debugEnqueueCallback(arg0, 5, &func_80063728_64328, arg1);
    }
}

void buildDisplayListSegment(DisplayListObject *arg0) {
    if (!isObjectCulled(&arg0->unk10.position)) {
        setupDisplayListMatrix(arg0);

        gDPPipeSync(gRegionAllocPtr++);

        gDPSetColor(gRegionAllocPtr++, 0xFB, arg0->unk3B | ~0xFF);

        gSPDisplayList(gRegionAllocPtr++, arg0->unk20->opaqueDisplayList);
    }
}

void func_800638C0_644C0(DisplayListObject *arg0) {
    Gfx *dl;

    if (!isObjectCulled(&arg0->unk10.position)) {
        setupDisplayListMatrix(arg0);

        dl = gRegionAllocPtr;
        gRegionAllocPtr += 1;
        dl->words.w0 = 0xE7000000;
        dl->words.w1 = 0;

        gRegionAllocPtr += 1;
        dl[1].words.w0 = 0xFB000000;
        dl[1].words.w1 = arg0->unk3B | 0xFFFFFF00;

        gRegionAllocPtr += 1;
        dl[2].words.w0 = 0xDE000000;
        dl[2].words.w1 = (u32)arg0->unk20->transparentDisplayList;
    }
}

void func_8006395C_6455C(DisplayListObject *arg0) {
    if (!isObjectCulled(&arg0->unk10.position)) {
        setupDisplayListMatrix(arg0);

        gDPPipeSync(gRegionAllocPtr++);
        gDPSetColor(gRegionAllocPtr++, 0xFB, arg0->unk3B | ~0xFF);
        gSPDisplayList(gRegionAllocPtr++, arg0->unk20->overlayDisplayList);
    }
}

void func_800639F8_645F8(s32 arg0, DisplayListObject *arg1) {
    arg1->unk30 = 0;
    if (arg1->unk20->opaqueDisplayList != NULL) {
        debugEnqueueCallback(arg0 & 0xFFFF, 1, &buildDisplayListSegment, arg1);
    }
    if (arg1->unk20->transparentDisplayList != NULL) {
        debugEnqueueCallback(arg0 & 0xFFFF, 3, &func_800638C0_644C0, arg1);
    }
    if (arg1->unk20->overlayDisplayList != NULL) {
        debugEnqueueCallback(arg0 & 0xFFFF, 5, &func_8006395C_6455C, arg1);
    }
}

INCLUDE_ASM("asm/nonmatchings/displaylist", func_80063A94_64694);

void func_8006405C_64C5C(DisplayListObject *arg0) {
    if (!isObjectCulled(&arg0->unk10.position)) {
        func_80063A94_64694(arg0);
        gSPDisplayList(gRegionAllocPtr++, arg0->unk20->opaqueDisplayList);
    }
}

void func_800640BC_64CBC(DisplayListObject *arg0) {
    if (!isObjectCulled(&arg0->unk10.position)) {
        func_80063A94_64694(arg0);
        gSPDisplayList(gRegionAllocPtr++, arg0->unk20->transparentDisplayList);
    }
}

void func_8006411C_64D1C(DisplayListObject *arg0) {
    if (!isObjectCulled(&arg0->unk10.position)) {
        func_80063A94_64694(arg0);
        gSPDisplayList(gRegionAllocPtr++, arg0->unk20->overlayDisplayList);
    }
}

void func_8006417C_64D7C(u16 arg0, DisplayListObject *arg1) {
    arg1->unk30 = 0;

    if (arg1->unk20->opaqueDisplayList != NULL) {
        debugEnqueueCallback(arg0, 1, &func_8006405C_64C5C, arg1);
    }

    if (arg1->unk20->transparentDisplayList != NULL) {
        debugEnqueueCallback(arg0, 3, &func_800640BC_64CBC, arg1);
    }

    if (arg1->unk20->overlayDisplayList != NULL) {
        debugEnqueueCallback(arg0, 5, &func_8006411C_64D1C, arg1);
    }
}

void func_80064218_64E18(DisplayListObject *arg0) {
    DisplayListObject *var_s0;
    Gfx *temp_v1;
    s32 temp_v0;
    s32 var_s1;
    void *v1_temp;
    void *temp_a0_val;
    s32 count;
    volatile u8 padding[0x44];

    if (arg0->unk30 == 0) {
        temp_v0 = (s32)arenaAlloc16(arg0->unk37 << 6);
        arg0->unk30 = temp_v0;
        if (temp_v0 == 0) {
            return;
        }
        count = arg0->unk37;
        var_s1 = 0;
        if (count > 0) {
            var_s0 = arg0;
            do {
                var_s0->unk30 = arg0->unk30 + (var_s1 << 6);
                func_8006C130_6CD30(var_s0, (LookAt *)var_s0->unk30);
                var_s1 += 1;
                var_s0 = (DisplayListObject *)((u8 *)var_s0 + 0x3C);
            } while (var_s1 < arg0->unk37);
        }
    }

    if (gGraphicsMode == 3) {
        return;
    }

    gDPPipeSync(gRegionAllocPtr++);
    gDPSetTexturePersp(gRegionAllocPtr++, 0x80000);
    v1_temp = arg0->unk24;
    gGraphicsMode = 3;

    if (v1_temp != NULL) {
        gSPSegment(gRegionAllocPtr++, 1, arg0->unk24);
    }

    if (arg0->unk28 != NULL) {
        temp_v1 = gRegionAllocPtr;
        temp_v1->words.w0 = 0xDB060008;
        temp_a0_val = arg0->unk28;
        gRegionAllocPtr = temp_v1 + 1;
        temp_v1->words.w1 = (u32)temp_a0_val;
    }

    if (arg0->unk2C != 0) {
        temp_v1 = gRegionAllocPtr;
        temp_v1->words.w0 = 0xDB06000C;
        temp_a0_val = (void *)arg0->unk2C;
        gRegionAllocPtr = temp_v1 + 1;
        temp_v1->words.w1 = (u32)temp_a0_val;
    }

    D_800A2D40_A3940 = arg0->unk24;
    D_800A2D44_A3944 = arg0->unk28;
    D_800A2D48_A3948 = arg0->unk2C;
}

INCLUDE_ASM("asm/nonmatchings/displaylist", func_800643AC_64FAC);

void func_80064628_65228(DisplayListObject *arg0) {
    s32 i;
    DisplayListObject *temp_s0;
    u32 *temp_v1;
    volatile u8 padding[0x40];

    func_80064218_64E18(arg0);

    for (i = 0; i < arg0->unk37; i++) {
        temp_s0 = (DisplayListObject *)((u8 *)arg0 + i * 0x3C);
        if (temp_s0->unk20->opaqueDisplayList != NULL) {
            func_800643AC_64FAC(arg0, i);
            temp_v1 = (u32 *)gRegionAllocPtr;
            temp_v1[0] = 0xDE000000;
            temp_v1[1] = (u32)temp_s0->unk20->opaqueDisplayList;
            gRegionAllocPtr = (Gfx *)(temp_v1 + 2);
        }
    }
}

void func_800646C8_652C8(DisplayListObject *arg0) {
    DisplayListObject *elements;
    s32 i;
    u32 *temp;
    s32 v0;
    u8 pad[0x48];

    func_80064218_64E18(arg0);

    v0 = arg0->unk37;
    if (v0 > 0) {
        elements = arg0;
        i = 0;
        do {
            if (elements[i].unk20->transparentDisplayList != NULL) {
                func_800643AC_64FAC(arg0, i);
                temp = (u32 *)gRegionAllocPtr;
                temp[0] = 0xDE000000;
                temp[1] = (u32)elements[i].unk20->transparentDisplayList;
                gRegionAllocPtr = (void *)(temp + 2);
            }
            i++;
            v0 = arg0->unk37;
        } while (i < v0);
    }
}

void func_80064768_65368(DisplayListObject *arg0) {
    DisplayListObject *elements;
    s32 i;
    u32 *temp;
    s32 v0;
    u8 pad[0x48];

    func_80064218_64E18(arg0);

    v0 = arg0->unk37;
    if (v0 > 0) {
        elements = arg0;
        i = 0;
        do {
            if (elements[i].unk20->overlayDisplayList != NULL) {
                func_800643AC_64FAC(arg0, i);
                temp = (u32 *)gRegionAllocPtr;
                temp[0] = 0xDE000000;
                temp[1] = (u32)elements[i].unk20->overlayDisplayList;
                gRegionAllocPtr = (void *)(temp + 2);
            }
            i++;
            v0 = arg0->unk37;
        } while (i < v0);
    }
}

void func_80064808_65408(s32 arg0, enqueueMultiPartDisplayList_arg1 *arg1, s32 arg2) {
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
        debugEnqueueCallback(arg0 & 0xFFFF, 1, &func_80064628_65228, arg1);
    }
    new_var = arg1;
    if (var_s0 & 2) {
        debugEnqueueCallback((arg0 & 0xFFFF) ^ 0, 3, &func_800646C8_652C8, new_var);
    }
    if (var_s0 & 4) {
        debugEnqueueCallback(arg0 & 0xFFFF, 5, &func_80064768_65368, arg1);
    }
}

INCLUDE_ASM("asm/nonmatchings/displaylist", func_800648EC_654EC);

INCLUDE_ASM("asm/nonmatchings/displaylist", func_80064CF4_658F4);

void func_80064E34_65A34(DisplayListObject *arg0) {
    func_800648EC_654EC();

    gSPDisplayList(gRegionAllocPtr++, arg0->unk20->transparentDisplayList);

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

void func_80064F74_65B74(DisplayListObject *arg0) {
    func_800648EC_654EC();

    gSPDisplayList(gRegionAllocPtr++, arg0->unk20->overlayDisplayList);

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

void func_800650B4_65CB4(u16 arg0, DisplayListObject *arg1) {
    arg1->unk30 = 0;

    if (arg1->unk20->opaqueDisplayList != NULL) {
        debugEnqueueCallback(arg0, 1, &func_80064CF4_658F4, arg1);
    }

    if (arg1->unk20->transparentDisplayList != NULL) {
        debugEnqueueCallback(arg0, 3, &func_80064E34_65A34, arg1);
    }

    if (arg1->unk20->overlayDisplayList != NULL) {
        debugEnqueueCallback(arg0, 5, &func_80064F74_65B74, arg1);
    }
}

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

typedef struct {
    u8 padding[0x30];
    s32 unk30;
} func_80066AC0_676C0_arg;
void func_80066AC0_676C0(s32 arg0, func_80066AC0_676C0_arg *arg1) {
    arg1->unk30 = 0;
    debugEnqueueCallback(arg0 & 0xFFFF, 4, &func_80066474_67074, arg1);
}

INCLUDE_ASM("asm/nonmatchings/displaylist", func_80066AF0_676F0);

extern void func_80066AF0_676F0(void);

void func_800670A4_67CA4(u16 arg0, func_80066444_67044_arg1 *arg1) {
    arg1->unk1C = 0;
    debugEnqueueCallback(arg0 & 0xFFFF, 4, &func_80066AF0_676F0, arg1);
}

INCLUDE_ASM("asm/nonmatchings/displaylist", func_800670D4_67CD4);

void func_800677C0_683C0(s32 arg0, loadAssetMetadata_arg *arg1) {
    (arg1 + 1)->unk0 = 0;
    debugEnqueueCallback(arg0 & 0xFFFF, 6, &func_800670D4_67CD4, arg1);
}

INCLUDE_ASM("asm/nonmatchings/displaylist", func_800677F0_683F0);

extern void func_800677F0_683F0(void);

void func_80067EDC_68ADC(s32 arg0, loadAssetMetadata_arg *arg1) {
    (arg1 + 1)->unk0 = 0;
    debugEnqueueCallback(arg0, 6, &func_800677F0_683F0, arg1);
}

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

void func_800680C4_68CC4(void) {
    func_800693C4_69FC4(&func_80014480_15080, 0xC8);
}
