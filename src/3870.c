#include "20F0.h"
#include "6E840.h"
#include "common.h"
#include "geometry.h"

extern s32 func_800018F4_24F4(void *);
extern Gfx *gRegionAllocPtr;
extern s16 gGraphicsMode;
extern u8 identityMatrix[];
extern void *D_8008BD80_8C980;
typedef struct {
    u8 pad0[0x14];
    s32 translation[3];
} ShadowModelData;

typedef struct {
    ShadowModelData *unk00;
    u8 pad04[0x14];
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    s16 unk24;
    s16 unk26;
    s16 unk28;
    u8 pad2A[2];
    Vec3i unk2C;
    u8 pad38[0x17];
    s8 unk4F;
    u8 pad50[0x30];
    Vtx *unk80;
    Mtx *unk84;
    u8 pad88[0xE];
    u8 unk96;
} ShadowEntity;

void func_80002C70_3870(ShadowEntity *arg0) {
    Transform3D matrix;
    s32 temp_a3;
    s32 temp_v1;
    s16 temp_s1;
    s16 temp_s0;
    Vtx *vtx;
    s32 i;
    s32 posValue;
    s16 negValue;
    s32 shadowScale;

    memcpy(&matrix, identityMatrix, sizeof(Transform3D));

    if (arg0->unk4F <= 0) {
        return;
    }

    vtx = arenaAlloc16(0x40);

    temp_v1 = arg0->unk2C.y;
    temp_v1 = MAX(temp_v1, 0);
    temp_a3 = (0x50 - (temp_v1 >> 16));
    temp_a3 = (temp_a3 * arg0->unk96) >> 8;

    arg0->unk80 = vtx;

    if (temp_a3 < 2) {
        return;
    }

    shadowScale = 0x1CA00;
    posValue = arg0->unk4F * shadowScale;

    if (vtx != NULL) {
        vtx[0].v.ob[0] = (-posValue) >> 14;
        arg0->unk80[0].v.ob[2] = (-posValue) >> 14;
        arg0->unk80[1].v.ob[0] = (-posValue) >> 14;
        arg0->unk80[1].v.ob[2] = posValue >> 14;
        arg0->unk80[2].v.ob[0] = posValue >> 14;
        arg0->unk80[2].v.ob[2] = posValue >> 14;
        arg0->unk80[3].v.ob[0] = posValue >> 14;
        arg0->unk80[3].v.ob[2] = (-posValue) >> 14;

        for (i = 0; i < 4; i++) {
            arg0->unk80[i].v.ob[1] = 0;
            arg0->unk80[i].v.cn[0] = 0;
            arg0->unk80[i].v.cn[1] = 0;
            arg0->unk80[i].v.cn[2] = 0;
            arg0->unk80[i].v.cn[3] = temp_a3;
        }

        arg0->unk80[0].v.tc[0] = -0x20;
        arg0->unk80[0].v.tc[1] = -0x20;
        arg0->unk80[1].v.tc[0] = -0x20;
        arg0->unk80[1].v.tc[1] = 0x3E0;
        arg0->unk80[2].v.tc[0] = 0x3E0;
        arg0->unk80[2].v.tc[1] = 0x3E0;
        arg0->unk80[3].v.tc[0] = 0x3E0;
        arg0->unk80[3].v.tc[1] = -0x20;
    }

    arg0->unk84 = arenaAlloc16(0x40);
    if (arg0->unk84 != NULL) {
        temp_s1 = distance_3d(arg0->unk18, arg0->unk1E, arg0->unk24);
        temp_s0 = distance_3d(arg0->unk1A, arg0->unk20, arg0->unk26);

        scaleMatrix(&matrix, temp_s1, temp_s0, distance_3d(arg0->unk1C, arg0->unk22, arg0->unk28));

        if (arg0->unk00 != NULL && hasModelGraphicsData((SceneModel *)arg0) == 0) {
            memcpy(&matrix.translation, arg0->unk00->translation, sizeof(Vec3i));
        } else {
            memcpy(&matrix.translation, &arg0->unk2C, sizeof(Vec3i));
        }

        matrix.translation.y = 0;
        func_8006C130_6CD30(&matrix, arg0->unk84);
    }

    if (arg0->unk80 == NULL || arg0->unk84 == NULL) {
        return;
    }

    gSPMatrix(gRegionAllocPtr++, arg0->unk84, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    if (gGraphicsMode != 0x200) {
        gGraphicsMode = 0x200;
        gSPDisplayList(gRegionAllocPtr++, &D_8008BD80_8C980);
    }

    gSPVertex(gRegionAllocPtr++, arg0->unk80, 4, 0);

    gSP1Quadrangle(gRegionAllocPtr++, 0, 1, 2, 3, 0);
}