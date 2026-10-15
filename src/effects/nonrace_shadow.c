#include "effects/nonrace_shadow.h"

#include "common.h"
#include "gbi.h"
#include "graphics/graphics.h"
#include "math/geometry.h"
#include "mbi.h"
#include "ui/level_preview_3d.h"

extern Gfx *gDisplayListAllocPtr;

u32 g_NonRaceShadowTex[];

Gfx g_NonRaceShadowDL[] = { gsSPClearGeometryMode(
                                G_ZBUFFER | G_SHADE | G_CULL_BOTH | G_FOG | G_LIGHTING | G_TEXTURE_GEN |
                                G_TEXTURE_GEN_LINEAR | G_SHADING_SMOOTH
                            ),
                            gsSPSetGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH),
                            gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
                            gsDPPipeSync(),
                            gsDPSetTextureLUT(G_TT_NONE),
                            gsDPSetTextureImage(G_IM_FMT_I, G_IM_SIZ_16b, 1, g_NonRaceShadowTex),
                            gsDPSetTile(
                                G_IM_FMT_I,
                                G_IM_SIZ_16b,
                                0,
                                0x0000,
                                G_TX_LOADTILE,
                                0,
                                G_TX_NOMIRROR | G_TX_CLAMP,
                                4,
                                G_TX_NOLOD,
                                G_TX_NOMIRROR | G_TX_CLAMP,
                                4,
                                G_TX_NOLOD
                            ),
                            gsDPLoadSync(),
                            gsDPLoadBlock(G_TX_LOADTILE, 0, 0, 63, 2048),
                            gsDPPipeSync(),
                            gsDPSetTile(
                                G_IM_FMT_I,
                                G_IM_SIZ_4b,
                                1,
                                0x0000,
                                G_TX_RENDERTILE,
                                0,
                                G_TX_NOMIRROR | G_TX_CLAMP,
                                4,
                                G_TX_NOLOD,
                                G_TX_NOMIRROR | G_TX_CLAMP,
                                4,
                                G_TX_NOLOD
                            ),
                            gsDPSetTileSize(G_TX_RENDERTILE, 0, 0, 60, 60),
                            gsDPSetTextureFilter(G_TF_AVERAGE),
                            gsDPSetCycleType(G_CYC_1CYCLE),
                            gsDPSetCombineMode(G_CC_MODULATEIA, G_CC_MODULATEIA),
                            gsDPSetRenderMode(G_RM_AA_ZB_XLU_DECAL, G_RM_AA_ZB_XLU_DECAL2),
                            gsSPEndDisplayList() };

u32 g_NonRaceShadowTex[] = { 0x00000FFF, 0xFFF00000, 0x000FFFFF, 0xFFFFF000, 0x00FFFFFF, 0xFFFFFF00, 0x0FFFFFFF,
                             0xFFFFFFF0, 0x0FFFFFFF, 0xFFFFFFF0, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                             0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                             0xFFFFFFFF, 0x0FFFFFFF, 0xFFFFFFF0, 0x0FFFFFFF, 0xFFFFFFF0, 0x00FFFFFF, 0xFFFFFF00,
                             0x000FFFFF, 0xFFFFF000, 0x00000FFF, 0xFFF00000, 0x00000000, 0x00000000 };

void renderNonRaceShadow(SceneModel *model) {
    Transform3D matrix;
    s32 shadowAlpha;
    s32 heightOffset;
    s16 scaleX;
    s16 scaleY;
    Vtx *vtx;
    s32 i;
    s32 quadSize;
    s16 negValue;
    s32 shadowScale;

    memcpy(&matrix, &identityMatrix, sizeof(Transform3D));

    if (model->shadowScale <= 0) {
        return;
    }

    vtx = graphicsAlloc(4 * sizeof(Vtx));

    heightOffset = model->transform.translation.y;
    heightOffset = MAX(heightOffset, 0);
    shadowAlpha = (0x50 - (heightOffset >> 16));
    shadowAlpha = (shadowAlpha * model->alpha) >> 8;

    model->shadowVertices = vtx;

    if (shadowAlpha < 2) {
        return;
    }

    shadowScale = 0x1CA00;
    quadSize = model->shadowScale * shadowScale;

    if (vtx != NULL) {
        vtx[0].v.ob[0] = (-quadSize) >> 14;
        model->shadowVertices[0].v.ob[2] = (-quadSize) >> 14;
        model->shadowVertices[1].v.ob[0] = (-quadSize) >> 14;
        model->shadowVertices[1].v.ob[2] = quadSize >> 14;
        model->shadowVertices[2].v.ob[0] = quadSize >> 14;
        model->shadowVertices[2].v.ob[2] = quadSize >> 14;
        model->shadowVertices[3].v.ob[0] = quadSize >> 14;
        model->shadowVertices[3].v.ob[2] = (-quadSize) >> 14;

        for (i = 0; i < 4; i++) {
            model->shadowVertices[i].v.ob[1] = 0;
            model->shadowVertices[i].v.cn[0] = 0;
            model->shadowVertices[i].v.cn[1] = 0;
            model->shadowVertices[i].v.cn[2] = 0;
            model->shadowVertices[i].v.cn[3] = shadowAlpha;
        }

        model->shadowVertices[0].v.tc[0] = -0x20;
        model->shadowVertices[0].v.tc[1] = -0x20;
        model->shadowVertices[1].v.tc[0] = -0x20;
        model->shadowVertices[1].v.tc[1] = 0x3E0;
        model->shadowVertices[2].v.tc[0] = 0x3E0;
        model->shadowVertices[2].v.tc[1] = 0x3E0;
        model->shadowVertices[3].v.tc[0] = 0x3E0;
        model->shadowVertices[3].v.tc[1] = -0x20;
    }

    model->shadowMatrix = graphicsAlloc(sizeof(Mtx));
    if (model->shadowMatrix != NULL) {
        scaleX = distance_3d(model->transform.m[0][0], model->transform.m[1][0], model->transform.m[2][0]);
        scaleY = distance_3d(model->transform.m[0][1], model->transform.m[1][1], model->transform.m[2][1]);

        scaleMatrix(
            &matrix,
            scaleX,
            scaleY,
            distance_3d(model->transform.m[0][2], model->transform.m[1][2], model->transform.m[2][2])
        );

        if (model->boneDisplayObjects != NULL && hasModelGraphicsData(model) == 0) {
            memcpy(&matrix.translation, &model->boneDisplayObjects[0].transform.translation, sizeof(Vec3i));
        } else {
            memcpy(&matrix.translation, &model->transform.translation, sizeof(Vec3i));
        }

        matrix.translation.y = 0;
        transform3DToN64Mtx(&matrix, model->shadowMatrix);
    }

    if (model->shadowVertices == NULL || model->shadowMatrix == NULL) {
        return;
    }

    gSPMatrix(gDisplayListAllocPtr++, model->shadowMatrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    if (gGraphicsMode != GRAPHICS_MODE_NONRACE_SHADOW) {
        gGraphicsMode = GRAPHICS_MODE_NONRACE_SHADOW;
        gSPDisplayList(gDisplayListAllocPtr++, &g_NonRaceShadowDL);
    }

    gSPVertex(gDisplayListAllocPtr++, model->shadowVertices, 4, 0);

    gSP1Quadrangle(gDisplayListAllocPtr++, 0, 1, 2, 3, 0);
}
