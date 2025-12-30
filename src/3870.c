#include "20F0.h"
#include "6E840.h"
#include "common.h"
#include "geometry.h"

extern Gfx *gRegionAllocPtr;
extern s16 gGraphicsMode;
extern u8 identityMatrix[];
extern void *D_8008BD80_8C980;
typedef struct {
    u8 pad0[0x14];
    s32 position[3];
} ModelPositionData;

typedef struct {
    ModelPositionData *modelData;
    u8 pad04[0x14];
    s16 rotationMatrix[3][3];
    u8 pad2A[2];
    Vec3i position;
    u8 pad38[0x17];
    s8 shadowRadius;
    u8 pad50[0x30];
    Vtx *shadowVertices;
    Mtx *shadowMatrix;
    u8 pad88[0xE];
    u8 alpha;
} ShadowEntity;

void renderShadow(ShadowEntity *entity) {
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

    memcpy(&matrix, identityMatrix, sizeof(Transform3D));

    if (entity->shadowRadius <= 0) {
        return;
    }

    vtx = arenaAlloc16(0x40);

    heightOffset = entity->position.y;
    heightOffset = MAX(heightOffset, 0);
    shadowAlpha = (0x50 - (heightOffset >> 16));
    shadowAlpha = (shadowAlpha * entity->alpha) >> 8;

    entity->shadowVertices = vtx;

    if (shadowAlpha < 2) {
        return;
    }

    shadowScale = 0x1CA00;
    quadSize = entity->shadowRadius * shadowScale;

    if (vtx != NULL) {
        vtx[0].v.ob[0] = (-quadSize) >> 14;
        entity->shadowVertices[0].v.ob[2] = (-quadSize) >> 14;
        entity->shadowVertices[1].v.ob[0] = (-quadSize) >> 14;
        entity->shadowVertices[1].v.ob[2] = quadSize >> 14;
        entity->shadowVertices[2].v.ob[0] = quadSize >> 14;
        entity->shadowVertices[2].v.ob[2] = quadSize >> 14;
        entity->shadowVertices[3].v.ob[0] = quadSize >> 14;
        entity->shadowVertices[3].v.ob[2] = (-quadSize) >> 14;

        for (i = 0; i < 4; i++) {
            entity->shadowVertices[i].v.ob[1] = 0;
            entity->shadowVertices[i].v.cn[0] = 0;
            entity->shadowVertices[i].v.cn[1] = 0;
            entity->shadowVertices[i].v.cn[2] = 0;
            entity->shadowVertices[i].v.cn[3] = shadowAlpha;
        }

        entity->shadowVertices[0].v.tc[0] = -0x20;
        entity->shadowVertices[0].v.tc[1] = -0x20;
        entity->shadowVertices[1].v.tc[0] = -0x20;
        entity->shadowVertices[1].v.tc[1] = 0x3E0;
        entity->shadowVertices[2].v.tc[0] = 0x3E0;
        entity->shadowVertices[2].v.tc[1] = 0x3E0;
        entity->shadowVertices[3].v.tc[0] = 0x3E0;
        entity->shadowVertices[3].v.tc[1] = -0x20;
    }

    entity->shadowMatrix = arenaAlloc16(0x40);
    if (entity->shadowMatrix != NULL) {
        scaleX = distance_3d(entity->rotationMatrix[0][0], entity->rotationMatrix[1][0], entity->rotationMatrix[2][0]);
        scaleY = distance_3d(entity->rotationMatrix[0][1], entity->rotationMatrix[1][1], entity->rotationMatrix[2][1]);

        scaleMatrix(
            &matrix,
            scaleX,
            scaleY,
            distance_3d(entity->rotationMatrix[0][2], entity->rotationMatrix[1][2], entity->rotationMatrix[2][2])
        );

        if (entity->modelData != NULL && hasModelGraphicsData((SceneModel *)entity) == 0) {
            memcpy(&matrix.translation, entity->modelData->position, sizeof(Vec3i));
        } else {
            memcpy(&matrix.translation, &entity->position, sizeof(Vec3i));
        }

        matrix.translation.y = 0;
        func_8006C130_6CD30(&matrix, entity->shadowMatrix);
    }

    if (entity->shadowVertices == NULL || entity->shadowMatrix == NULL) {
        return;
    }

    gSPMatrix(gRegionAllocPtr++, entity->shadowMatrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    if (gGraphicsMode != 0x200) {
        gGraphicsMode = 0x200;
        gSPDisplayList(gRegionAllocPtr++, &D_8008BD80_8C980);
    }

    gSPVertex(gRegionAllocPtr++, entity->shadowVertices, 4, 0);

    gSP1Quadrangle(gRegionAllocPtr++, 0, 1, 2, 3, 0);
}