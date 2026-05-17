#include "graphics/displaylist.h"

#include "assets.h"
#include "common.h"
#include "data/data_table.h"
#include "gbi.h"
#include "graphics/graphics.h"
#include "gu.h"
#include "math/geometry.h"
#include "mbi.h"
#include "race/race_session.h"
#include "system/rom_loader.h"
#include "system/task_scheduler.h"
#include "text/font_assets.h"

USE_OVERLAY(rand);

typedef struct {
    /* 0x00 */ s32 vertices;
    /* 0x04 */ s32 posX;
    /* 0x08 */ s32 posY;
    /* 0x0C */ s32 posZ;
    /* 0x10 */ u8 *textureData;
    /* 0x14 */ u8 *paletteData;
    /* 0x18 */ u8 width;
    /* 0x19 */ u8 height;
    /* 0x1A */ u8 padding[2];
    /* 0x1C */ Mtx *matrix;
} TexturedSpriteState;

typedef struct {
    /* 0x00 */ s32 vertices;
    /* 0x04 */ s16 rotation[3][3];
    /* 0x16 */ u16 padding;
    /* 0x18 */ s32 posX;
    /* 0x1C */ s32 posY;
    /* 0x20 */ s32 posZ;
    /* 0x24 */ u8 *textureData;
    /* 0x28 */ u8 *paletteData;
    /* 0x2C */ u8 width;
    /* 0x2D */ u8 height;
    /* 0x2E */ u8 padding2[2];
    /* 0x30 */ Mtx *matrix;
} RotatedBillboardSprite;

typedef struct {
    /* 0x00 */ s32 vertices;
    /* 0x04 */ s32 posX;
    /* 0x08 */ s32 posY;
    /* 0x0C */ s32 posZ;
    /* 0x10 */ u8 *textureData;
    /* 0x14 */ u8 *paletteData;
    /* 0x18 */ u8 width;
    /* 0x19 */ u8 height;
    /* 0x1A */ u8 alpha;
    /* 0x1B */ u8 padding;
    /* 0x1C */ Mtx *matrix;
} AlphaSpriteState;

typedef struct {
    DataTable_19E80 *fontDataTable;
} FontSystemInitData;

extern s32 gLookAtPtr;
extern ActiveViewportOverlay *gActiveViewport;
extern Gfx *gDisplayListAllocPtr;
extern s16 gGraphicsMode;
extern void *D_800A2D40_A3940;
extern void *D_800A2D44_A3944;
extern void *D_800A2D48_A3948;
extern Gfx D_8009A780_9B380[];
extern u32 D_800A2D4C_A394C;
extern u32 D_800A2D50_A3950;
extern u8 D_800A2D54_A3954;

Gfx gAlphaSpriteSetupDL[] = {
    gsSPClearGeometryMode(
        G_ZBUFFER | G_SHADE | G_CULL_BOTH | G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR | G_SHADING_SMOOTH
    ),
    gsSPSetGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_SHADING_SMOOTH),
    gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
    gsDPPipeSync(),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsDPSetCycleType(G_CYC_2CYCLE),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, ENVIRONMENT, 0, 0, 0, 0, COMBINED, 0, 0, 0, COMBINED),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_XLU_SURF2),
    gsDPSetTextureLUT(G_TT_RGBA16),
    gsSPEndDisplayList(),
};

void renderCameraRelativeDisplayList(DisplayListObject *arg0);
void renderTexturedBillboardSprite(TexturedSpriteState *);
void renderTexturedBillboardSpriteTile(TexturedSpriteState *);
void renderRotatedBillboardSprite(RotatedBillboardSprite *);
void renderAlphaSprite(AlphaSpriteState *);
void renderAlphaBillboardSprite(AlphaSpriteState *);
void scheduleGameInitialization(void);
void prepareDisplayListRenderState(DisplayListObject *);
void setupDisplayListMatrix(DisplayListObject *);
void setupBillboardDisplayListMatrix(DisplayListObject *);
void initializeFontSystemAndTransitionToMainMenu(void);

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
    gameData->section3Data = (void *)parser;
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

u16 findTrackSector(void *arg0, u16 sectorIndex, void *arg2) {
#ifdef CC_CHECK
    TrackGeometryFaceData *trackGeom = (TrackGeometryFaceData *)arg0;
    Vec3i *pos = (Vec3i *)arg2;
    s16 currentSector;
    s16 i;
#else
    register TrackGeometryFaceData *trackGeom __asm__("$19") = (TrackGeometryFaceData *)arg0;
    register Vec3i *pos __asm__("$20") = (Vec3i *)arg2;
    s16 currentSector;
    register s16 i __asm__("$16");
#endif
    s32 v0x;
    s32 v1x;
    s32 v0z;
    s32 v1z;
    s32 faceGroupOffset0;
    s32 faceGroupOffset1;
    s32 v2x;
    s32 v3x;
    s32 v3z;
    s32 v2z;

    currentSector = sectorIndex;
    i = 0;

    do {
        {
            s32 idx = currentSector & 0xFFFF;
            s32 fgAddr = (s32)trackGeom->faceGroups;
            TrackFaceGroup *group;
            faceGroupOffset0 = ((idx << 3) + idx) << 2;
            group = (TrackFaceGroup *)(faceGroupOffset0 + fgAddr);
            v0x = ((Vertex6 *)(group->vertexIdx0 * 6 + (s32)trackGeom->vertices))->x << 16;
            v0z = ((Vertex6 *)(group->vertexIdx0 * 6 + (s32)trackGeom->vertices))->z << 16;
            v1x = ((Vertex6 *)(group->vertexIdx1 * 6 + (s32)trackGeom->vertices))->x << 16;
            v1z = ((Vertex6 *)(group->vertexIdx1 * 6 + (s32)trackGeom->vertices))->z << 16;
        }

        if (cross2d(pos->x, pos->z, v1x, v1z, v0x, v0z) > 0) {
            s16 neighbor = ((TrackFaceGroup *)(faceGroupOffset0 + (s32)trackGeom->faceGroups))->neighbor0;
            if (neighbor >= 0) {
                currentSector = neighbor;
                goto next;
            }
        }

        {
            s32 idx = currentSector & 0xFFFF;
            s32 fgAddr = (s32)trackGeom->faceGroups;
            TrackFaceGroup *group;
            faceGroupOffset1 = ((idx << 3) + idx) << 2;
            group = (TrackFaceGroup *)(faceGroupOffset1 + fgAddr);
            v2x = ((Vertex6 *)(group->vertexIdx2 * 6 + (s32)trackGeom->vertices))->x << 16;
            v2z = ((Vertex6 *)(group->vertexIdx2 * 6 + (s32)trackGeom->vertices))->z << 16;
            v3x = ((Vertex6 *)(group->vertexIdx3 * 6 + (s32)trackGeom->vertices))->x << 16;
            v3z = ((Vertex6 *)(group->vertexIdx3 * 6 + (s32)trackGeom->vertices))->z << 16;
        }

        if (cross2d(pos->x, pos->z, v2x, v2z, v3x, v3z) > 0) {
            s16 neighbor = ((TrackFaceGroup *)(faceGroupOffset1 + (s32)trackGeom->faceGroups))->neighbor1;
            if (neighbor >= 0) {
                currentSector = neighbor;
                goto next;
            }
        }

        if (cross2d(pos->x, pos->z, v3x, v3z, v1x, v1z) > 0) {
            s32 idx = currentSector & 0xFFFF;
            s32 fgAddr = (s32)trackGeom->faceGroups;
            s16 neighbor = ((TrackFaceGroup *)(((idx << 3) + idx) * 4 + fgAddr))->neighbor2;
            if (neighbor >= 0) {
                currentSector = neighbor;
                goto next;
            }
        }

        if (cross2d(pos->x, pos->z, v0x, v0z, v2x, v2z) > 0) {
            s32 idx = currentSector & 0xFFFF;
            s32 fgAddr = (s32)trackGeom->faceGroups;
            s16 neighbor = ((TrackFaceGroup *)(((idx << 3) + idx) * 4 + fgAddr))->neighbor3;
            if (neighbor < 0) {
                break;
            }
            currentSector = neighbor;
            goto next;
        }

        return currentSector & 0xFFFF;

    next:
        i += 1;
    } while (i < 4);

    return currentSector & 0xFFFF;
}

#define TRACK_WALL_VERTEX_X(trackGeom, sectorIndex, vertexField)                               \
    (((TrackGeometryFaceData *)trackGeom)                                                      \
         ->vertices[((TrackGeometryFaceData *)trackGeom)->faceGroups[sectorIndex].vertexField] \
         .x                                                                                    \
     << 16)

#define TRACK_WALL_VERTEX_Z(trackGeom, sectorIndex, vertexField)                               \
    (((TrackGeometryFaceData *)trackGeom)                                                      \
         ->vertices[((TrackGeometryFaceData *)trackGeom)->faceGroups[sectorIndex].vertexField] \
         .z                                                                                    \
     << 16)

#define RESOLVE_TRACK_SIDE_WALL(neighborField, startX, startZ, endX, endZ)                        \
    if (((TrackGeometryFaceData *)trackGeom)->faceGroups[sectorIndex].neighborField < 0) {        \
        dx = (endX) - (startX);                                                                   \
        dz = (endZ) - (startZ);                                                                   \
        relX = workPos.x - (startX);                                                              \
        zTemp = workPos.z;                                                                        \
        relZ = zTemp - (startZ);                                                                  \
        wallLen = isqrt64((((s64)dx) * dx) + (((s64)dz) * dz));                                   \
        dzNorm = (s32)((((s64)dz) * 0x2000) / wallLen);                                           \
        dxNorm = (s32)((((s64)dx) * 0x2000) / wallLen);                                           \
        dz = -dzNorm;                                                                             \
        dx = (s32)(((((s64)relX) * dz) + (((s64)relZ) * dxNorm)) / 0x2000);                       \
        if ((-collisionRadius) < dx) {                                                            \
            dx = (-collisionRadius) - dx;                                                         \
            result = ((TrackGeometryFaceData *)trackGeom)->faceGroups[sectorIndex].neighborField; \
            hitWall = 1;                                                                          \
            workPos.x += (((s64)dx) * dz) / 0x2000;                                               \
            workPos.z += (((s64)dx) * dxNorm) / 0x2000;                                           \
            sectorIndex = findTrackSector(trackGeom, sectorIndex, &workPos);                      \
        }                                                                                         \
    }

#define TRACK_WALL_ENDPOINT_COLLIDES()                           \
    if (dz < 0) {                                                \
        endpointDistSq = ((s64)dz) * dz;                         \
    } else {                                                     \
        endpointDistSq = ((s64)(wallLen - dz)) * (wallLen - dz); \
    }                                                            \
    perpDistSq = ((s64)dx) * dx;                                 \
    TRACK_WALL_REGISTER_CLOBBER();                               \
    collisionDistSq = endpointDistSq + perpDistSq;               \
    if (collisionDistSq < (((s64)collisionRadius) * collisionRadius))

#ifdef CC_CHECK
#define TRACK_WALL_REGISTER_CLOBBER()
#else
#define TRACK_WALL_REGISTER_CLOBBER() \
    __asm__ volatile("" : : : "t0", "t1", "t2", "t3", "t4", "t5", "t8", "t9", "s0", "s1")
#endif

#define APPLY_TRACK_DIAGONAL_WALL_PUSH(neighborField, groupOffset, groupIdx, scaleTemp, xDivisor, zDivisor)           \
    dx = (-collisionRadius) - dx;                                                                                     \
    result =                                                                                                          \
        ((TrackFaceGroup *)((groupOffset) + ((s32)((TrackGeometryFaceData *)trackGeom)->faceGroups)))->neighborField; \
    scaleTemp = 0x2000;                                                                                               \
    hitWall = 1;                                                                                                      \
    workPos.x += (((s64)dx) * temp_s7) / (xDivisor);                                                                  \
    workPos.z += (((s64)dx) * totalDistSq) / (zDivisor);                                                              \
    sectorIndex = findTrackSector(trackGeom, groupIdx, &workPos)

s32 resolveTrackWallCollision(
    void *trackGeom,
    u16 sectorIndex,
    void *position,
    s32 collisionRadius,
    Vec3i *pushOffset
) {
    int new_var6;
    Vec3i workPos;
    s32 v0x;
    s64 new_var4;
    s32 v2x;
    s32 v3x;
    s32 v0z;
    s32 v1z;
    s32 v2z;
    s32 v3z;
    s32 v1x;
    s64 new_var5;
    s32 dz;
    s32 dx;
    s32 wallLen;
    s32 relX;
#ifdef CC_CHECK
    s32 relZ;
    s32 zTemp;
#else
    register s32 relZ __asm__("$23");
    register s32 zTemp __asm__("$3");
#endif
    s16 dzNorm;
    s16 dxNorm;
    s32 temp_s7;
    s16 iterations;
    u16 hitWall;
    s32 result;
#ifdef CC_CHECK
    s64 endpointDistSq;
#else
    register s64 endpointDistSq __asm__("$2");
#endif
    s64 perpDistSq;
    s64 collisionDistSq;
    s64 totalDistSq;
    s32 cond;

    workPos.x = ((Vec3i *)position)->x;
    workPos.z = ((Vec3i *)position)->z;
    iterations = 0;
    result = 0;
    do {
        hitWall = 0;
        v2x = TRACK_WALL_VERTEX_X(trackGeom, sectorIndex, vertexIdx2);
        v2z = TRACK_WALL_VERTEX_Z(trackGeom, sectorIndex, vertexIdx2);
        v3x = TRACK_WALL_VERTEX_X(trackGeom, sectorIndex, vertexIdx3);
        v3z = TRACK_WALL_VERTEX_Z(trackGeom, sectorIndex, vertexIdx3);
        RESOLVE_TRACK_SIDE_WALL(neighbor1, v2x, v2z, v3x, v3z);
        v0x = TRACK_WALL_VERTEX_X(trackGeom, sectorIndex, vertexIdx0);
        v0z = TRACK_WALL_VERTEX_Z(trackGeom, sectorIndex, vertexIdx0);
        v1x = TRACK_WALL_VERTEX_X(trackGeom, sectorIndex, vertexIdx1);
        v1z = TRACK_WALL_VERTEX_Z(trackGeom, sectorIndex, vertexIdx1);
        RESOLVE_TRACK_SIDE_WALL(neighbor0, v1x, v1z, v0x, v0z);
        {
            TrackFaceGroup *new_var3;
            s32 groupIdx2;
            s32 groupOffset2;
            int new_var;

            groupIdx2 = sectorIndex;
            new_var3 = ((TrackGeometryFaceData *)trackGeom)->faceGroups;
            groupOffset2 = ((groupIdx2 << 3) + groupIdx2) << 2;
            if (((TrackFaceGroup *)(groupOffset2 + ((s32)new_var3)))->neighbor2 < 0) {
                dx = v1x - v3x;
                dz = v1z - v3z;
                relX = workPos.x - v3x;
                zTemp = workPos.z;
                cond = zTemp - v3z;
                relZ = cond;
                wallLen = isqrt64((((s64)dx) * dx) + (((s64)dz) * dz));
                dzNorm = (s32)((((s64)dz) * 0x2000) / wallLen);
                dxNorm = (s32)((2 * (((s64)dx) * 4096)) / wallLen);
                new_var5 = (s64)relX;
                totalDistSq = dxNorm;
                dz = dzNorm;
                temp_s7 = -dzNorm;
                dz = (s32)(((new_var5 * dxNorm) + (((s64)relZ) * dzNorm)) / 0x2000);
                dx = (s32)(((((s64)relX) * temp_s7) + (((s64)relZ) * totalDistSq)) / 0x2000);
                if ((-collisionRadius) < dx) {
                    do {
                        if (((dz > 0) && (dz < wallLen)) || (dx >= 0)) {
                            APPLY_TRACK_DIAGONAL_WALL_PUSH(
                                neighbor2,
                                groupOffset2,
                                groupIdx2,
                                new_var,
                                0x2000,
                                new_var
                            );
                        } else {
                            TRACK_WALL_ENDPOINT_COLLIDES() {
                                APPLY_TRACK_DIAGONAL_WALL_PUSH(
                                    neighbor2,
                                    groupOffset2,
                                    groupIdx2,
                                    new_var,
                                    0x2000,
                                    new_var
                                );
                            }
                        }
                    } while (0);
                }
            }
            {
                TrackFaceGroup *new_var2;
                s32 groupIdx3;
                s32 groupOffset3;

                groupIdx3 = sectorIndex;
                new_var2 = ((TrackGeometryFaceData *)trackGeom)->faceGroups;
                groupOffset3 = ((groupIdx3 << 3) + groupIdx3) << 2;
                if (((TrackFaceGroup *)(groupOffset3 + ((s32)new_var2)))->neighbor3 < 0) {
                    dx = v2x - v0x;
                    dz = v2z - v0z;
                    relX = workPos.x - v0x;
                    zTemp = workPos.z;
                    relZ = zTemp - v0z;
                    wallLen = isqrt64((((s64)dx) * dx) + (((s64)dz) * dz));
                    dzNorm = (s32)((((s64)dz) * 0x2000) / wallLen);
                    dxNorm = (s32)((((s64)dx) * 0x2000) / wallLen);
                    new_var4 = (s64)relX;
                    totalDistSq = dxNorm;
                    dz = dzNorm;
                    temp_s7 = -dzNorm;
                    dz = (s32)(((new_var4 * dxNorm) + (((s64)relZ) * dzNorm)) / 0x2000);
                    dx = (s32)(((((s64)relX) * temp_s7) + (((s64)relZ) * totalDistSq)) / 0x2000);
                    if ((-collisionRadius) < dx) {
                        if (((dz > 0) && (dz < wallLen)) || (dx >= 0)) {
                            APPLY_TRACK_DIAGONAL_WALL_PUSH(
                                neighbor3,
                                groupOffset3,
                                groupIdx3,
                                new_var6,
                                new_var6,
                                0x2000
                            );
                        } else {
                            TRACK_WALL_ENDPOINT_COLLIDES() {
                                APPLY_TRACK_DIAGONAL_WALL_PUSH(
                                    neighbor3,
                                    groupOffset3,
                                    groupIdx3,
                                    new_var6,
                                    new_var6,
                                    0x2000
                                );
                            }
                        }
                    }
                }
            }
        }
        iterations++;
        cond = (hitWall != 1) || (iterations == 2);
    } while (!cond);

    pushOffset->x = workPos.x - ((Vec3i *)position)->x;
    pushOffset->z = workPos.z - ((Vec3i *)position)->z;
    return result;
}

#undef TRACK_WALL_VERTEX_X
#undef TRACK_WALL_VERTEX_Z
#undef RESOLVE_TRACK_SIDE_WALL
#undef TRACK_WALL_ENDPOINT_COLLIDES
#undef TRACK_WALL_REGISTER_CLOBBER
#undef APPLY_TRACK_DIAGONAL_WALL_PUSH

s32 getTrackHeightAtPosition(void *trackGeom_void, u16 groupIdx, void *pos_void) {
    TrackGeometryFaceData *trackGeom = (TrackGeometryFaceData *)trackGeom_void;
    Vec3i *pos = (Vec3i *)pos_void;
    s32 sp1C;
    s32 sp24;
    s32 v0x;
    s32 v0z;
    s32 v1x;
    s32 v1z;
    s32 v2x;
    s32 v2z;
    TrackFace *temp_a0;
    Vertex6 *temp_a1;
    TrackFace *temp_a0_2;
    Vertex6 *temp_a3;
    s32 y0;
    s32 dY_v1;
    s32 dY_v2;
    s32 cross_a;
    s32 cross_b;
    s32 area;
    s32 t0;
    s32 t3;
    s32 var_fp;
    s32 temp_v1;
    s32 idx;
    TrackFaceGroup *base;
    TrackFaceGroup *temp_v0;
    TrackFaceGroup *temp_v0_2;

    idx = groupIdx;
    base = trackGeom->faceGroups;
    temp_v1 = ((idx << 3) + idx) << 2;
    temp_v0 = (TrackFaceGroup *)(temp_v1 + (s32)base);
    sp1C = temp_v0->baseIndex;
    var_fp = sp1C << 3;
    sp24 = temp_v1;

    if (sp1C < sp1C + temp_v0->count) {
        do {
            temp_a0 = (TrackFace *)(var_fp + (s32)trackGeom->faces);
            temp_a1 = trackGeom->vertices;
            v0x = ((Vertex6 *)((temp_a0->v0 * 6) + (s32)temp_a1))->x;
            v0z = ((Vertex6 *)((temp_a0->v0 * 6) + (s32)temp_a1))->z;
            v1x = ((Vertex6 *)((temp_a0->v1 * 6) + (s32)temp_a1))->x;
            v1z = ((Vertex6 *)((temp_a0->v1 * 6) + (s32)temp_a1))->z;
            v2x = ((Vertex6 *)((temp_a0->v2 * 6) + (s32)temp_a1))->x;
            v2z = ((Vertex6 *)((temp_a0->v2 * 6) + (s32)temp_a1))->z;

            if ((temp_a0->flags & 1) || (cross2d(pos->x, pos->z, v0x << 16, v0z << 16, v1x << 16, v1z << 16) >= 0)) {
                if ((((TrackFace *)(var_fp + (s32)trackGeom->faces))->flags & 2) ||
                    (cross2d(pos->x, pos->z, v1x << 16, v1z << 16, v2x << 16, v2z << 16) >= 0)) {
                    t0 = v0x << 16;
                    t3 = v0z << 16;
                    if (cross2d(pos->x, pos->z, v2x << 16, v2z << 16, t0, t3) >= 0) {
                        temp_a0_2 = (TrackFace *)(var_fp + (s32)trackGeom->faces);
                        temp_a3 = trackGeom->vertices;
                        y0 = ((Vertex6 *)((temp_a0_2->v0 * 6) + (s32)temp_a3))->y;
                        dY_v2 = ((Vertex6 *)((temp_a0_2->v2 * 6) + (s32)temp_a3))->y - y0;
                        v1z = v1z - v0z;
                        cross_a = dY_v2 * v1z;

                        dY_v1 = ((Vertex6 *)((temp_a0_2->v1 * 6) + (s32)temp_a3))->y - y0;
                        v2z = v2z - v0z;
                        cross_b = v2z * dY_v1;

                        v1x = v1x - v0x;
                        v2x = v2x - v0x;

                        v0x = pos->x - t0;
                        v0z = pos->z - t3;
                        area = (v2z * v1x) - (v2x * v1z);

                        return (s32)((-((s64)(cross_a - cross_b) * v0x) -
                                      ((s64)((v2x * dY_v1) - (dY_v2 * v1x)) * v0z)) /
                                     area) +
                               (y0 << 16);
                    }
                }
            }

            temp_v0_2 = (TrackFaceGroup *)(sp24 + (s32)trackGeom->faceGroups);
            var_fp += 8;
            sp1C += 1;
        } while (sp1C < temp_v0_2->baseIndex + temp_v0_2->count);
    }

    return -0x3E800000;
}

s32 getTrackHeightWithNormalAtPosition(void *trackGeom_void, u16 groupIdx, void *pos_void, s32 arg3) {
    TrackGeometryFaceData *trackGeom = (TrackGeometryFaceData *)trackGeom_void;
    Vec3i *pos;
    s32 *outNormal;
    s32 sp24;
    s32 var_fp;
    s32 v0x;
    s32 v0z;
    s32 v1x;
    s32 v1z;
    s32 v2x;
    s32 v2z;
    TrackFace *temp_a0;
    Vertex6 *temp_a1;
    TrackFace *temp_a0_2;
    Vertex6 *temp_a3;
    s32 y0;
    s32 dY_v1;
    s32 dY_v2;
    s32 t0;
    s32 t3;
    s32 sp1C;
    s32 temp_v1;
    s32 idx;
    TrackFaceGroup *base;
    TrackFaceGroup *temp_v0;
    TrackFaceGroup *temp_v0_2;

    idx = groupIdx;
    base = trackGeom->faceGroups;
    temp_v1 = ((idx << 3) + idx) << 2;
    temp_v0 = (TrackFaceGroup *)(temp_v1 + (s32)base);
    sp1C = temp_v0->baseIndex;
    outNormal = (s32 *)arg3;
    var_fp = sp1C << 3;
    pos = (Vec3i *)pos_void;
    sp24 = temp_v1;

    if (sp1C < sp1C + temp_v0->count) {
        do {
            temp_a0 = (TrackFace *)(var_fp + (s32)trackGeom->faces);
            temp_a1 = trackGeom->vertices;
            v0x = ((Vertex6 *)((temp_a0->v0 * 6) + (s32)temp_a1))->x;
            v0z = ((Vertex6 *)((temp_a0->v0 * 6) + (s32)temp_a1))->z;
            v1x = ((Vertex6 *)((temp_a0->v1 * 6) + (s32)temp_a1))->x;
            v1z = ((Vertex6 *)((temp_a0->v1 * 6) + (s32)temp_a1))->z;
            v2x = ((Vertex6 *)((temp_a0->v2 * 6) + (s32)temp_a1))->x;
            v2z = ((Vertex6 *)((temp_a0->v2 * 6) + (s32)temp_a1))->z;

            if ((temp_a0->flags & 1) || (cross2d(pos->x, pos->z, v0x << 16, v0z << 16, v1x << 16, v1z << 16) >= 0)) {
                if ((((TrackFace *)(var_fp + (s32)trackGeom->faces))->flags & 2) ||
                    (cross2d(pos->x, pos->z, v1x << 16, v1z << 16, v2x << 16, v2z << 16) >= 0)) {
                    t0 = v0x << 16;
                    t3 = v0z << 16;
                    if (cross2d(pos->x, pos->z, v2x << 16, v2z << 16, t0, t3) >= 0) {
                        temp_a0_2 = (TrackFace *)(var_fp + (s32)trackGeom->faces);
                        temp_a3 = trackGeom->vertices;
                        y0 = ((Vertex6 *)((temp_a0_2->v0 * 6) + (s32)temp_a3))->y;
                        dY_v2 = ((Vertex6 *)((temp_a0_2->v2 * 6) + (s32)temp_a3))->y - y0;
                        v1z = v1z - v0z;

                        dY_v1 = ((Vertex6 *)((temp_a0_2->v1 * 6) + (s32)temp_a3))->y - y0;
                        v2z = v2z - v0z;

                        v1x = v1x - v0x;
                        v2x = v2x - v0x;

                        v0x = pos->x - t0;
                        v0z = pos->z - t3;

                        outNormal[0] = (dY_v2 * v1z) - (v2z * dY_v1);
                        outNormal[1] = (v2z * v1x) - (v2x * v1z);
                        outNormal[2] = (v2x * dY_v1) - (dY_v2 * v1x);

                        return (s32)((-((s64)outNormal[0] * v0x) - ((s64)outNormal[2] * v0z)) / outNormal[1]) +
                               (((Vertex6 *)(((TrackFace *)(var_fp + (s32)trackGeom->faces))->v0 * 6 +
                                             (s32)trackGeom->vertices))
                                    ->y
                                << 16);
                    }
                }
            }

            temp_v0_2 = (TrackFaceGroup *)(sp24 + (s32)trackGeom->faceGroups);
            sp1C += 1;
            var_fp += 8;
        } while (sp1C < temp_v0_2->baseIndex + temp_v0_2->count);
    }

    return -0x3E800000;
}

s32 projectPositionOntoTrackSegment(TrackGeometryData *arg0, u16 arg1, PositionXZ *arg2) {
    s32 dz;
    s32 dx;
    Vertex6 *verts;
    s32 temp;
    s32 roundUp;
    Vertex6 *vertsTemp;
    u16 idx0;
    u16 idx1;
    s32 distSq;
    s32 dist;
    s32 unitX;
    s32 unitZ;
    s32 result;

    idx0 = arg0->elements[arg1].vertexIdx1;
    verts = arg0->vertices;
    idx1 = arg0->elements[arg1].vertexIdx2;

    dx = verts[idx0].x - verts[idx1].x;
    dz = verts[idx0].z - verts[idx1].z;

    distSq = (dx * dx) + (dz * dz);
    dist = isqrt64(distSq);

    unitX = (dx << 13) / dist;
    temp = (dz << 13) / dist;
    unitZ = temp;

    idx1 = arg0->elements[arg1].vertexIdx2;
    temp = arg2->x;
    dx = temp;
    vertsTemp = arg0->vertices;
    roundUp = 0x1FFF;
    verts = vertsTemp;
    dx = dx - verts[idx1].x;
    dz = arg2->z - verts[idx1].z;

    result = (unitX * dx) + (unitZ * dz);

    if (result < 0) {
        result += roundUp;
    }

    return (result << 3) >> 16;
}

s16 getTrackSegmentFinishZoneFlag(GameDataLayout *gameData, u16 index) {
    return gameData->section3Data[index].finishZoneFlag;
}

s32 computeSectorTrackHeight(TrackGeometryFaceData *geom, u16 groupIdx, Vec3i *pos, s32 yOffset) {
    s32 i;
    s32 v0x;
    s32 v0z;
    s32 v1x;
    s32 v1z;
    s32 v2x;
    s32 v2z;
    TrackFace *face;
    Vertex6 *verts;
    s32 y0;
    s32 dY_v1;
    s32 dY_v2;
    s32 cross_a;
    s32 cross_b;
    s64 num_a;
    s64 num_b;
    s32 area;
    s32 result;
    s32 defaultVal = 0x3E800000;

    for (i = geom->faceGroups[groupIdx].baseIndex2;
         i < geom->faceGroups[groupIdx].baseIndex2 + geom->faceGroups[groupIdx].count2;
         i++) {
        face = (TrackFace *)((i * sizeof(TrackFace)) + (s32)geom->faces);
        verts = geom->vertices;
        v0x = verts[face->v0].x;
        v0z = verts[face->v0].z;
        v1x = verts[face->v2].x;
        v1z = verts[face->v2].z;
        v2x = verts[face->v1].x;
        v2z = verts[face->v1].z;

        if (cross2d(pos->x, pos->z, v0x << 16, v0z << 16, v1x << 16, v1z << 16) >= 0) {
            if (cross2d(pos->x, pos->z, v1x << 16, v1z << 16, v2x << 16, v2z << 16) >= 0) {
                if (cross2d(pos->x, pos->z, v2x << 16, v2z << 16, v0x << 16, v0z << 16) >= 0) {
                    y0 = geom->vertices[geom->faces[i].v0].y;
                    dY_v1 = geom->vertices[geom->faces[i].v1].y - y0;
                    v1z = v1z - v0z;
                    cross_a = dY_v1 * v1z;

                    dY_v2 = geom->vertices[geom->faces[i].v2].y - y0;
                    v2z = v2z - v0z;
                    cross_b = v2z * dY_v2;

                    v1x = v1x - v0x;
                    v2x = v2x - v0x;

                    v0x = pos->x - (v0x << 16);
                    v0z = pos->z - (v0z << 16);
                    area = (v2z * v1x) - (v2x * v1z);

                    y0 <<= 16;
                    y0 += (s32)((-((s64)(cross_a - cross_b) * v0x) - ((s64)((v2x * dY_v2) - (dY_v1 * v1x)) * v0z)) /
                                area);
                    return y0 - yOffset;
                }
            }
        }
    }

    return defaultVal - yOffset;
}

u16 getTrackEndInfo(void *arg0, void *arg1) {
    s32 var_v1;
    s32 temp_v0;
    TrackSegmentElement *elements;
    Vertex6 *verts;
    u16 idx0;
    u16 idx1;
    Vertex6 *vert0;
    Vertex6 *vert1;

    var_v1 = 0;
    elements = ((TrackGeometryData *)arg0)->elements;

    while (1) {
        temp_v0 = elements[var_v1].nextElementIdx;
        if (temp_v0 < 0) {
            break;
        }
        var_v1 = temp_v0;
    }

    ((Vec3i *)arg1)->x = ((TrackGeometryData *)arg0)->vertices[(elements + var_v1)->vertexIdx2].x << 16;
    ((Vec3i *)arg1)->y =
        ((TrackGeometryData *)arg0)->vertices[(((TrackGeometryData *)arg0)->elements + var_v1)->vertexIdx2].y << 16;
    ((Vec3i *)arg1)->z =
        ((TrackGeometryData *)arg0)->vertices[(((TrackGeometryData *)arg0)->elements + var_v1)->vertexIdx2].z << 16;

    idx0 = (((TrackGeometryData *)arg0)->elements + var_v1)->vertexIdx1;
    verts = ((TrackGeometryData *)arg0)->vertices;
    idx1 = (((TrackGeometryData *)arg0)->elements + var_v1)->vertexIdx2;
    vert0 = (Vertex6 *)((s32)idx0 * sizeof(Vertex6) + (s32)verts);
    vert1 = (Vertex6 *)((s32)idx1 * sizeof(Vertex6) + (s32)verts);

    return (computeAngleToPosition(vert0->x, vert0->z, vert1->x, vert1->z) - 0x1000) & 0xFFFF;
}

u16 findFaceGroupAtPosition(TrackGeometryFaceData *geom, Vec3i *pos) {
    s16 i;
    s32 numFaceGroups;
    s32 v0x, v0z, v1x, v1z, v2x, v2z, v3x, v3z;
    TrackFace *faceEntry;
    Vertex6 *vertexIdx;

    vertexIdx = geom->unk0;
    vertexIdx = (Vertex6 *)(u32)((u16)geom->unk0->x);
    faceEntry = (TrackFace *)&geom->vertices[(s32)(vertexIdx)];
    faceEntry = &geom->faces[(u16)((Vertex6 *)faceEntry)->x];
    numFaceGroups = faceEntry->v0;

    i = 0;
    do {
        v0x = geom->vertices[geom->faceGroups[i].vertexIdx0].x << 16;
        v0z = geom->vertices[geom->faceGroups[i].vertexIdx0].z << 16;

        v1x = geom->vertices[geom->faceGroups[i].vertexIdx1].x << 16;
        v1z = geom->vertices[geom->faceGroups[i].vertexIdx1].z << 16;

        v2x = geom->vertices[geom->faceGroups[i].vertexIdx2].x << 16;
        v2z = geom->vertices[geom->faceGroups[i].vertexIdx2].z << 16;

        v3x = geom->vertices[geom->faceGroups[i].vertexIdx3].x << 16;
        v3z = geom->vertices[geom->faceGroups[i].vertexIdx3].z << 16;

        if (cross2d(pos->x, pos->z, v1x, v1z, v0x, v0z) > 0)
            continue;
        if (cross2d(pos->x, pos->z, v2x, v2z, v3x, v3z) > 0)
            continue;
        if (cross2d(pos->x, pos->z, v3x, v3z, v1x, v1z) > 0)
            continue;
        if (cross2d(pos->x, pos->z, v0x, v0z, v2x, v2z) > 0)
            continue;
        goto found;
    } while (++i != numFaceGroups);

    return 0;

found:
    return i;
}

void findTrackFaceAtPosition(TrackGeometryFaceData *arg0, u16 arg1, Vec3i *arg2, u8 *arg3, u8 *arg4) {
    s32 sp24;
    s32 sp2C;
    s32 temp_fp;
    s32 temp_s4;
    s32 temp_s5;
    s32 temp_s6;
    s32 temp_s7;
    s32 var_s2;
    s32 var_s3;
    TrackFace *temp_a0;
    Vertex6 *temp_a1;
    Vertex6 *temp_v0_2;
    Vertex6 *temp_v0_3;
    Vertex6 *temp_v0_4;
    TrackFaceGroup *temp_v0;
    TrackFaceGroup *temp_v0_5;
    s32 temp_v1;
    s32 idx;
    TrackFaceGroup *base;

    idx = arg1;
    base = arg0->faceGroups;
    temp_v1 = ((idx << 3) + idx) << 2;
    temp_v0 = (TrackFaceGroup *)(temp_v1 + (s32)base);
    var_s3 = temp_v0->baseIndex;
    sp2C = temp_v1;
    var_s2 = var_s3 << 3;
    if (var_s3 < (var_s3 + temp_v0->count)) {
        do {
            temp_a0 = (TrackFace *)(var_s2 + (s32)arg0->faces);
            temp_a1 = arg0->vertices;
            temp_v0_2 = (Vertex6 *)((temp_a0->v0 * 6) + (s32)temp_a1);
            temp_fp = (s32)temp_v0_2->x;
            sp24 = (s32)temp_v0_2->z;
            temp_v0_3 = (Vertex6 *)((temp_a0->v1 * 6) + (s32)temp_a1);
            temp_s4 = (s32)temp_v0_3->x;
            temp_s5 = (s32)temp_v0_3->z;
            temp_v0_4 = (Vertex6 *)((temp_a0->v2 * 6) + (s32)temp_a1);
            temp_s6 = (s32)temp_v0_4->x;
            temp_s7 = (s32)temp_v0_4->z;
            if ((temp_a0->flags & 1) ||
                (cross2d(arg2->x, arg2->z, temp_fp << 0x10, sp24 << 0x10, temp_s4 << 0x10, temp_s5 << 0x10) >= 0)) {
                if ((((TrackFace *)(var_s2 + (s32)arg0->faces))->flags & 2) ||
                    (cross2d(arg2->x, arg2->z, temp_s4 << 0x10, temp_s5 << 0x10, temp_s6 << 0x10, temp_s7 << 0x10) >=
                     0)) {
                    if (cross2d(arg2->x, arg2->z, temp_s6 << 0x10, temp_s7 << 0x10, temp_fp << 0x10, sp24 << 0x10) >=
                        0) {
                        *arg3 = ((TrackFace *)(var_s2 + (s32)arg0->faces))->flags >> 2;
                        *arg4 = ((TrackFace *)(var_s2 + (s32)arg0->faces))->surfaceIndex;
                        return;
                    }
                }
            }
            temp_v0_5 = (TrackFaceGroup *)(sp2C + (s32)arg0->faceGroups);
            var_s3 += 1;
            var_s2 += 8;
        } while (var_s3 < (temp_v0_5->baseIndex + temp_v0_5->count));
    }
}

u16 getTrackSegmentWaypoints(TrackGeometryData *trackGeom, u16 waypointIdx, void *waypointStart, void *waypointEnd) {
    Vec3i *startPos = (Vec3i *)waypointStart;
    Vec3i *endPos = (Vec3i *)waypointEnd;
    Vertex6 *verts;
    u16 startVertexIdx;
    u16 endVertexIdx;
    Vertex6 *startVert;
    Vertex6 *endVert;

    startPos->x = trackGeom->vertices[trackGeom->elements[waypointIdx].vertexIdx1].x << 16;
    startPos->y = trackGeom->vertices[trackGeom->elements[waypointIdx].vertexIdx1].y << 16;
    startPos->z = trackGeom->vertices[trackGeom->elements[waypointIdx].vertexIdx1].z << 16;

    endPos->x = trackGeom->vertices[trackGeom->elements[waypointIdx].vertexIdx2].x << 16;
    endPos->y = trackGeom->vertices[trackGeom->elements[waypointIdx].vertexIdx2].y << 16;
    endPos->z = trackGeom->vertices[trackGeom->elements[waypointIdx].vertexIdx2].z << 16;

    startVertexIdx = trackGeom->elements[waypointIdx].vertexIdx1;
    verts = trackGeom->vertices;
    endVertexIdx = trackGeom->elements[waypointIdx].vertexIdx2;
    startVert = (Vertex6 *)((s32)startVertexIdx * sizeof(Vertex6) + (s32)verts);
    endVert = (Vertex6 *)((s32)endVertexIdx * sizeof(Vertex6) + (s32)verts);

    return (computeAngleToPosition(startVert->x, startVert->z, endVert->x, endVert->z) - 0x1000) & 0xFFFF;
}

s32 resolveTrackSegmentIndex(TrackSegmentEntry **arg0, u16 index) {
    s16 linkedIdx;
    s16 result = index;

    linkedIdx = arg0[3][index].linkedSegmentIdx;

    if (linkedIdx >= 0) {
        result = linkedIdx;
    }
    return result & 0xFFFF;
}

s32 normalizeSurfaceType(s32 surfaceType) {
    surfaceType &= 0xF;

    if (surfaceType == 1) {
        return 1;
    }
    if (surfaceType == 2) {
        return 1;
    }
    return surfaceType;
}

void prepareDisplayListRenderState(DisplayListObject *obj) {
    Mtx tempMatrix;
    f32 lookAtX;
    f32 lookAtY;
    f32 lookAtZ;
    f32 upX;
    f32 upY;
    f32 upZ;
    LookAt *lookAt;

    if (obj->transformMatrix == NULL) {
        obj->transformMatrix = arenaAlloc16(0x40);
        if (obj->transformMatrix == NULL) {
            return;
        }
        transform3DToMtx(obj, obj->transformMatrix);
    }

    if (obj->displayLists->flags & 1) {
        lookAt = (LookAt *)arenaAlloc16(0x20);
        if (lookAt == NULL) {
            return;
        }

        matrixToEulerAngles(
            (Transform3D *)&gActiveViewport->cameraRotationMatrix,
            &obj->transform,
            &lookAtX,
            &lookAtY,
            &lookAtZ,
            &upX,
            &upY,
            &upZ
        );
        guLookAtReflect(&tempMatrix, lookAt, 0.0f, 0.0f, 0.0f, lookAtX, lookAtY, lookAtZ, upX, upY, upZ);
        gSPLookAt(gDisplayListAllocPtr++, lookAt);
    }

    if (gGraphicsMode != 3) {
        gDPPipeSync(gDisplayListAllocPtr++);
        gDPSetTexturePersp(gDisplayListAllocPtr++, 0x80000);

        gGraphicsMode = 3;

        if (obj->segment1 != 0) {
            gSPSegment(gDisplayListAllocPtr++, 1, obj->segment1);
        }

        if (obj->segment2 != 0) {
            gSPSegment(gDisplayListAllocPtr++, 2, obj->segment2);
        }

        if (obj->segment3 != 0) {
            gSPSegment(gDisplayListAllocPtr++, 3, obj->segment3);
        }
    } else {
        if (obj->segment1 != 0 && obj->segment1 != D_800A2D40_A3940) {
            gSPSegment(gDisplayListAllocPtr++, 1, obj->segment1);
        }

        if (obj->segment2 != 0 && obj->segment2 != D_800A2D44_A3944) {
            gSPSegment(gDisplayListAllocPtr++, 2, obj->segment2);
        }

        if (obj->segment3 != 0 && obj->segment3 != D_800A2D48_A3948) {
            gSPSegment(gDisplayListAllocPtr++, 3, obj->segment3);
        }
    }

    D_800A2D40_A3940 = obj->segment1;
    D_800A2D44_A3944 = obj->segment2;
    D_800A2D48_A3948 = obj->segment3;

    gSPMatrix(gDisplayListAllocPtr++, obj->transformMatrix, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
}

void renderOpaqueDisplayList(DisplayListObject *arg0) {
    prepareDisplayListRenderState(arg0);
    gSPDisplayList(gDisplayListAllocPtr++, arg0->displayLists->opaqueDisplayList);
}

void renderTransparentDisplayList(DisplayListObject *arg0) {
    prepareDisplayListRenderState(arg0);
    gSPDisplayList(gDisplayListAllocPtr++, arg0->displayLists->transparentDisplayList);
}

void renderOverlayDisplayList(DisplayListObject *arg0) {
    prepareDisplayListRenderState(arg0);
    gSPDisplayList(gDisplayListAllocPtr++, arg0->displayLists->overlayDisplayList);
}

void enqueueDisplayListObjectWithFullRenderState(s32 arg0, void *arg1) {
    ((DisplayListObject *)arg1)->transformMatrix = 0;

    if (((DisplayListObject *)arg1)->displayLists->opaqueDisplayList != NULL) {
        enqueueCallbackBySlotIndex(arg0, 1, renderOpaqueDisplayList, arg1);
    }

    if (((DisplayListObject *)arg1)->displayLists->transparentDisplayList != NULL) {
        enqueueCallbackBySlotIndex(arg0, 3, renderTransparentDisplayList, arg1);
    }

    if (((DisplayListObject *)arg1)->displayLists->overlayDisplayList != NULL) {
        enqueueCallbackBySlotIndex(arg0, 5, renderOverlayDisplayList, arg1);
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

    if (arg0->transformMatrix == NULL) {
        arg0->transformMatrix = arenaAlloc16(0x40);
        if (arg0->transformMatrix == NULL) {
            return;
        }
        transform3DToN64Mtx((Transform3D *)arg0, arg0->transformMatrix);
    }

    if (arg0->displayLists->flags & 1) {
        temp_v0 = (LookAt *)arenaAlloc16(0x20);
        if (temp_v0 == NULL) {
            return;
        }

        matrixToEulerAngles(
            (Transform3D *)&gActiveViewport->cameraRotationMatrix,
            &arg0->transform,
            &sp70,
            &sp74,
            &sp78,
            &sp7C,
            &sp80,
            &sp84
        );
        guLookAtReflect(&sp30, temp_v0, 0.0f, 0.0f, 0.0f, sp70, sp74, sp78, sp7C, sp80, sp84);
        gSPLookAt(gDisplayListAllocPtr++, temp_v0);
    }

    if (gGraphicsMode != 3) {
        gDPPipeSync(gDisplayListAllocPtr++);
        gDPSetTexturePersp(gDisplayListAllocPtr++, 0x80000);

        gGraphicsMode = 3;

        if (arg0->segment1 != 0) {
            gSPSegment(gDisplayListAllocPtr++, 1, arg0->segment1);
        }

        if (arg0->segment2 != 0) {
            gSPSegment(gDisplayListAllocPtr++, 2, arg0->segment2);
        }

        if (arg0->segment3 != 0) {
            gSPSegment(gDisplayListAllocPtr++, 3, arg0->segment3);
        }

        D_800A2D40_A3940 = arg0->segment1;
        D_800A2D44_A3944 = arg0->segment2;
        D_800A2D48_A3948 = arg0->segment3;
    } else {
        if (arg0->segment1 != D_800A2D40_A3940) {
            if (arg0->segment1 != 0) {
                gSPSegment(gDisplayListAllocPtr++, 1, arg0->segment1);
            }
            D_800A2D40_A3940 = arg0->segment1;
        }

        if (arg0->segment2 != D_800A2D44_A3944) {
            if (arg0->segment2 != 0) {
                gSPSegment(gDisplayListAllocPtr++, 2, arg0->segment2);
            }
            D_800A2D44_A3944 = arg0->segment2;
        }

        if (arg0->segment3 != D_800A2D48_A3948) {
            if (arg0->segment3 != 0) {
                gSPSegment(gDisplayListAllocPtr++, 3, arg0->segment3);
            }
            D_800A2D48_A3948 = arg0->segment3;
        }
    }

    gSPMatrix(gDisplayListAllocPtr++, arg0->transformMatrix, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
}

void renderOpaqueDisplayListCallback(DisplayListObject *obj) {
    setupDisplayListMatrix(obj);
    gSPDisplayList(gDisplayListAllocPtr++, obj->displayLists->opaqueDisplayList);
}

void renderTransparentDisplayListCallback(DisplayListObject *obj) {
    setupDisplayListMatrix(obj);
    gSPDisplayList(gDisplayListAllocPtr++, obj->displayLists->transparentDisplayList);
}

void renderOverlayDisplayListCallback(DisplayListObject *obj) {
    setupDisplayListMatrix(obj);
    gSPDisplayList(gDisplayListAllocPtr++, obj->displayLists->overlayDisplayList);
}

void enqueueDisplayListObject(s32 arg0, DisplayListObject *arg1) {
    arg1->transformMatrix = 0;
    if (arg1->displayLists->opaqueDisplayList != NULL) {
        enqueueCallbackBySlotIndex(arg0 & 0xFFFF, 1, renderOpaqueDisplayListCallback, arg1);
    }
    if (arg1->displayLists->transparentDisplayList != NULL) {
        enqueueCallbackBySlotIndex(arg0 & 0xFFFF, 3, renderTransparentDisplayListCallback, arg1);
    }
    if (arg1->displayLists->overlayDisplayList != NULL) {
        enqueueCallbackBySlotIndex(arg0 & 0xFFFF, 5, renderOverlayDisplayListCallback, arg1);
    }
}

void renderOpaqueDisplayListWithFrustumCull(DisplayListObject *arg0) {
    if (!isObjectCulled(&arg0->transform.translation)) {
        setupDisplayListMatrix(arg0);
        gSPDisplayList(gDisplayListAllocPtr++, arg0->displayLists->opaqueDisplayList);
    }
}

void renderTransparentDisplayListWithFrustumCull(DisplayListObject *arg0) {
    if (!isObjectCulled(&arg0->transform.translation)) {
        setupDisplayListMatrix(arg0);
        gSPDisplayList(gDisplayListAllocPtr++, arg0->displayLists->transparentDisplayList);
    }
}

void renderOverlayDisplayListWithFrustumCull(DisplayListObject *arg0) {
    s32 *temp_v1;

    if (!isObjectCulled(&arg0->transform.translation)) {
        setupDisplayListMatrix(arg0);
        gSPDisplayList(gDisplayListAllocPtr++, arg0->displayLists->overlayDisplayList);
    }
}

void enqueueDisplayListWithFrustumCull(s32 arg0, DisplayListObject *arg1) {
    arg1->transformMatrix = 0;
    if (arg1->displayLists->opaqueDisplayList != NULL) {
        enqueueCallbackBySlotIndex(arg0, 1, &renderOpaqueDisplayListWithFrustumCull, arg1);
    }
    if (arg1->displayLists->transparentDisplayList != NULL) {
        enqueueCallbackBySlotIndex(arg0, 3, &renderTransparentDisplayListWithFrustumCull, arg1);
    }
    if (arg1->displayLists->overlayDisplayList != NULL) {
        enqueueCallbackBySlotIndex(arg0, 5, &renderOverlayDisplayListWithFrustumCull, arg1);
    }
}

void buildDisplayListSegment(DisplayListObject *arg0) {
    if (!isObjectCulled(&arg0->transform.translation)) {
        setupDisplayListMatrix(arg0);

        gDPPipeSync(gDisplayListAllocPtr++);

        gDPSetColor(gDisplayListAllocPtr++, 0xFB, arg0->envColorAlpha | ~0xFF);

        gSPDisplayList(gDisplayListAllocPtr++, arg0->displayLists->opaqueDisplayList);
    }
}

void buildTransparentDisplayListSegment(DisplayListObject *arg0) {
    Gfx *dl;

    if (!isObjectCulled(&arg0->transform.translation)) {
        setupDisplayListMatrix(arg0);

        dl = gDisplayListAllocPtr;
        gDisplayListAllocPtr += 1;
        dl->words.w0 = 0xE7000000;
        dl->words.w1 = 0;

        gDisplayListAllocPtr += 1;
        dl[1].words.w0 = 0xFB000000;
        dl[1].words.w1 = arg0->envColorAlpha | 0xFFFFFF00;

        gDisplayListAllocPtr += 1;
        dl[2].words.w0 = 0xDE000000;
        dl[2].words.w1 = (u32)arg0->displayLists->transparentDisplayList;
    }
}

void buildOverlayDisplayListSegment(DisplayListObject *obj) {
    if (!isObjectCulled(&obj->transform.translation)) {
        setupDisplayListMatrix(obj);

        gDPPipeSync(gDisplayListAllocPtr++);
        gDPSetColor(gDisplayListAllocPtr++, 0xFB, obj->envColorAlpha | ~0xFF);
        gSPDisplayList(gDisplayListAllocPtr++, obj->displayLists->overlayDisplayList);
    }
}

void enqueueDisplayListObjectWithSegments(s32 arg0, DisplayListObject *arg1) {
    arg1->transformMatrix = 0;
    if (arg1->displayLists->opaqueDisplayList != NULL) {
        enqueueCallbackBySlotIndex(arg0 & 0xFFFF, 1, &buildDisplayListSegment, arg1);
    }
    if (arg1->displayLists->transparentDisplayList != NULL) {
        enqueueCallbackBySlotIndex(arg0 & 0xFFFF, 3, &buildTransparentDisplayListSegment, arg1);
    }
    if (arg1->displayLists->overlayDisplayList != NULL) {
        enqueueCallbackBySlotIndex(arg0 & 0xFFFF, 5, &buildOverlayDisplayListSegment, arg1);
    }
}

/**
 * Sets up the display list matrix for billboarded sprites.
 *
 * Billboarded sprites always face the camera. This is achieved by:
 * 1. First applying a translation matrix (positions the sprite)
 * 2. Then applying a look-at matrix (makes the sprite face the camera)
 * 3. Finally applying a rotation matrix (applies local sprite rotation)
 *
 * Allocates 128 bytes (2 matrices) if not already allocated.
 */
void setupBillboardDisplayListMatrix(DisplayListObject *obj) {
    Mtx lookAtMatrix;
    f32 eyeX;
    f32 eyeY;
    f32 eyeZ;
    f32 upX;
    f32 upY;
    f32 upZ;
    LookAt *lookAt;

    if (obj->transformMatrix == NULL) {
        obj->transformMatrix = arenaAlloc16(0x80);
        if (obj->transformMatrix == NULL) {
            return;
        }
        /* First matrix: translation (identity rotation + position) */
        obj->transformMatrix->m[0][0] = 0x10000;
        obj->transformMatrix->m[0][1] = 0;
        obj->transformMatrix->m[0][2] = 1;
        obj->transformMatrix->m[0][3] = 0;
        obj->transformMatrix->m[1][0] = 0;
        obj->transformMatrix->m[1][1] = 0x10000;
        obj->transformMatrix->m[1][2] =
            (obj->transform.translation.x & 0xFFFF0000) + ((u16 *)&obj->transform.translation.y)[0];
        obj->transformMatrix->m[1][3] = (obj->transform.translation.z & 0xFFFF0000) + 1;
        obj->transformMatrix->m[2][0] = 0;
        obj->transformMatrix->m[2][1] = 0;
        obj->transformMatrix->m[2][2] = 0;
        obj->transformMatrix->m[2][3] = 0;
        obj->transformMatrix->m[3][0] = 0;
        obj->transformMatrix->m[3][1] = 0;
        obj->transformMatrix->m[3][2] =
            (obj->transform.translation.x << 16) + ((u16 *)&obj->transform.translation.y)[1];
        obj->transformMatrix->m[3][3] = obj->transform.translation.z << 16;
        /* Second matrix: rotation from Transform3D */
        obj->transformMatrix[1].m[0][0] =
            ((obj->transform.m[0][0] * 2) & 0xFFFF0000) + (-(s32)((u16)obj->transform.m[0][1] >> 15) & 0xFFFF);
        obj->transformMatrix[1].m[0][1] = (obj->transform.m[0][2] * 2) & 0xFFFF0000;
        obj->transformMatrix[1].m[0][2] =
            ((obj->transform.m[1][0] * 2) & 0xFFFF0000) + (-(s32)((u16)obj->transform.m[1][1] >> 15) & 0xFFFF);
        obj->transformMatrix[1].m[0][3] = (obj->transform.m[1][2] * 2) & 0xFFFF0000;
        obj->transformMatrix[1].m[1][0] =
            ((obj->transform.m[2][0] * 2) & 0xFFFF0000) + (-(s32)((u16)obj->transform.m[2][1] >> 15) & 0xFFFF);
        obj->transformMatrix[1].m[1][1] = (obj->transform.m[2][2] * 2) & 0xFFFF0000;
        obj->transformMatrix[1].m[1][2] = 0;
        obj->transformMatrix[1].m[1][3] = 1;
        obj->transformMatrix[1].m[2][0] =
            ((obj->transform.m[0][0] << 17) & 0xFFFF0000) + ((obj->transform.m[0][1] * 2) & 0xFFFF);
        obj->transformMatrix[1].m[2][1] = (obj->transform.m[0][2] << 17) & 0xFFFF0000;
        obj->transformMatrix[1].m[2][2] =
            ((obj->transform.m[1][0] << 17) & 0xFFFF0000) + ((obj->transform.m[1][1] * 2) & 0xFFFF);
        obj->transformMatrix[1].m[2][3] = (obj->transform.m[1][2] << 17) & 0xFFFF0000;
        obj->transformMatrix[1].m[3][0] =
            ((obj->transform.m[2][0] << 17) & 0xFFFF0000) + ((obj->transform.m[2][1] * 2) & 0xFFFF);
        obj->transformMatrix[1].m[3][1] = (obj->transform.m[2][2] << 17) & 0xFFFF0000;
        obj->transformMatrix[1].m[3][2] = 0;
        obj->transformMatrix[1].m[3][3] = 0;
    }

    if (obj->displayLists->flags & 1) {
        lookAt = arenaAlloc16(0x20);
        if (lookAt == NULL) {
            return;
        }

        matrixToEulerAngles(
            (Transform3D *)&gActiveViewport->cameraRotationMatrix,
            &obj->transform,
            &eyeX,
            &eyeY,
            &eyeZ,
            &upX,
            &upY,
            &upZ
        );
        guLookAtReflect(&lookAtMatrix, lookAt, 0.0f, 0.0f, 0.0f, eyeX, eyeY, eyeZ, upX, upY, upZ);
        gSPLookAt(gDisplayListAllocPtr++, lookAt);
    }

    if (gGraphicsMode != 3) {
        gDPPipeSync(gDisplayListAllocPtr++);
        gDPSetTexturePersp(gDisplayListAllocPtr++, 0x80000);

        gGraphicsMode = 3;

        if (obj->segment1 != NULL) {
            gSPSegment(gDisplayListAllocPtr++, 1, obj->segment1);
        }

        if (obj->segment2 != NULL) {
            gSPSegment(gDisplayListAllocPtr++, 2, obj->segment2);
        }

        if (obj->segment3 != NULL) {
            gSPSegment(gDisplayListAllocPtr++, 3, obj->segment3);
        }

        D_800A2D40_A3940 = obj->segment1;
        D_800A2D44_A3944 = obj->segment2;
        D_800A2D48_A3948 = obj->segment3;
    } else {
        if (obj->segment1 != D_800A2D40_A3940) {
            if (obj->segment1 != NULL) {
                gSPSegment(gDisplayListAllocPtr++, 1, obj->segment1);
            }
            D_800A2D40_A3940 = obj->segment1;
        }

        if (obj->segment2 != D_800A2D44_A3944) {
            if (obj->segment2 != NULL) {
                gSPSegment(gDisplayListAllocPtr++, 2, obj->segment2);
            }
            D_800A2D44_A3944 = obj->segment2;
        }

        if (obj->segment3 != D_800A2D48_A3948) {
            if (obj->segment3 != NULL) {
                gSPSegment(gDisplayListAllocPtr++, 3, obj->segment3);
            }
            D_800A2D48_A3948 = obj->segment3;
        }
    }

    gSPMatrix(gDisplayListAllocPtr++, obj->transformMatrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    gSPMatrix(gDisplayListAllocPtr++, gLookAtPtr, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);

    gSPMatrix(gDisplayListAllocPtr++, &obj->transformMatrix[1], G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
}

void renderBillboardedOpaqueDisplayList(DisplayListObject *arg0) {
    if (!isObjectCulled(&arg0->transform.translation)) {
        setupBillboardDisplayListMatrix(arg0);
        gSPDisplayList(gDisplayListAllocPtr++, arg0->displayLists->opaqueDisplayList);
    }
}

void renderBillboardedTransparentDisplayList(DisplayListObject *arg0) {
    if (!isObjectCulled(&arg0->transform.translation)) {
        setupBillboardDisplayListMatrix(arg0);
        gSPDisplayList(gDisplayListAllocPtr++, arg0->displayLists->transparentDisplayList);
    }
}

void renderBillboardedOverlayDisplayList(DisplayListObject *arg0) {
    if (!isObjectCulled(&arg0->transform.translation)) {
        setupBillboardDisplayListMatrix(arg0);
        gSPDisplayList(gDisplayListAllocPtr++, arg0->displayLists->overlayDisplayList);
    }
}

void enqueueBillboardedDisplayListObject(s32 arg0, DisplayListObject *arg1) {
    arg1->transformMatrix = 0;

    if (arg1->displayLists->opaqueDisplayList != NULL) {
        enqueueCallbackBySlotIndex(arg0, 1, &renderBillboardedOpaqueDisplayList, arg1);
    }

    if (arg1->displayLists->transparentDisplayList != NULL) {
        enqueueCallbackBySlotIndex(arg0, 3, &renderBillboardedTransparentDisplayList, arg1);
    }

    if (arg1->displayLists->overlayDisplayList != NULL) {
        enqueueCallbackBySlotIndex(arg0, 5, &renderBillboardedOverlayDisplayList, arg1);
    }
}

void initializeMultiPartDisplayListObjects(DisplayListObject *arg0) {
    DisplayListObject *currentObject;
    Gfx *gfxCmd;
    s32 matrixIndex;
    void *segment1;
    void *segmentValue;
    s32 objectCount;
    volatile u8 padding[0x44];

    if (arg0->transformMatrix == NULL) {
        arg0->transformMatrix = arenaAlloc16(arg0->numParts << 6);
        if (arg0->transformMatrix == NULL) {
            return;
        }
        objectCount = arg0->numParts;
        matrixIndex = 0;
        if (objectCount > 0) {
            currentObject = arg0;
            do {
                currentObject->transformMatrix = arg0->transformMatrix + matrixIndex;
                transform3DToN64Mtx((Transform3D *)currentObject, currentObject->transformMatrix);
                matrixIndex += 1;
                currentObject++;
            } while (matrixIndex < arg0->numParts);
        }
    }

    if (gGraphicsMode == 3) {
        return;
    }

    gDPPipeSync(gDisplayListAllocPtr++);
    gDPSetTexturePersp(gDisplayListAllocPtr++, 0x80000);
    segment1 = arg0->segment1;
    gGraphicsMode = 3;

    if (segment1 != NULL) {
        gSPSegment(gDisplayListAllocPtr++, 1, arg0->segment1);
    }

    if (arg0->segment2 != NULL) {
        gfxCmd = gDisplayListAllocPtr;
        gfxCmd->words.w0 = 0xDB060008;
        segmentValue = arg0->segment2;
        gDisplayListAllocPtr = gfxCmd + 1;
        gfxCmd->words.w1 = (u32)segmentValue;
    }

    if (arg0->segment3 != 0) {
        gfxCmd = gDisplayListAllocPtr;
        gfxCmd->words.w0 = 0xDB06000C;
        segmentValue = arg0->segment3;
        gDisplayListAllocPtr = gfxCmd + 1;
        gfxCmd->words.w1 = (u32)segmentValue;
    }

    D_800A2D40_A3940 = arg0->segment1;
    D_800A2D44_A3944 = arg0->segment2;
    D_800A2D48_A3948 = arg0->segment3;
}

void setupMultiPartObjectRenderState(DisplayListObject *arg0, s32 arg1) {
    Mtx sp30;
    f32 sp70;
    f32 sp74;
    f32 sp78;
    f32 sp7C;
    f32 sp80;
    f32 sp84;
    DisplayListObject *obj;
    LookAt *lookat;
    Gfx *dl;
    void *temp;
    DisplayListObject *elem;

    obj = (DisplayListObject *)(arg1 * sizeof(DisplayListObject) + (s32)arg0);

    if (obj->displayLists->flags & 1) {
        lookat = arenaAlloc16(0x20);
        if (lookat == NULL) {
            return;
        }

        matrixToEulerAngles(
            (Transform3D *)&gActiveViewport->cameraRotationMatrix,
            &arg0->transform,
            &sp70,
            &sp74,
            &sp78,
            &sp7C,
            &sp80,
            &sp84
        );
        guLookAtReflect(&sp30, lookat, 0.0f, 0.0f, 0.0f, sp70, sp74, sp78, sp7C, sp80, sp84);

        gSPLookAt(gDisplayListAllocPtr++, lookat);
    }

    temp = obj->segment1;
    if (temp != D_800A2D40_A3940) {
        if (temp != NULL) {
            gSPSegment(gDisplayListAllocPtr++, 1, obj->segment1);
        }
        D_800A2D40_A3940 = obj->segment1;
    }

    elem = (DisplayListObject *)(arg1 * sizeof(DisplayListObject) + (s32)arg0);
    temp = elem->segment2;
    if (temp != D_800A2D44_A3944) {
        if (temp != NULL) {
            gSPSegment(gDisplayListAllocPtr++, 2, elem->segment2);
        }
        D_800A2D44_A3944 = elem->segment2;
    }

    elem = (DisplayListObject *)(arg1 * sizeof(DisplayListObject) + (s32)arg0);
    temp = elem->segment3;
    if (temp != D_800A2D48_A3948) {
        if (temp != NULL) {
            gSPSegment(gDisplayListAllocPtr++, 3, elem->segment3);
        }
        D_800A2D48_A3948 = elem->segment3;
    }

    gSPMatrix(gDisplayListAllocPtr++, (arg1 + arg0)->transformMatrix, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
}

void renderMultiPartOpaqueDisplayLists(DisplayListObject *displayObjects) {
    s32 i;
    DisplayListObject *currentObject;
    Gfx *displayListCmd;
    volatile u8 padding[0x40];

    initializeMultiPartDisplayListObjects(displayObjects);

    for (i = 0; i < displayObjects->numParts; i++) {
        currentObject = &displayObjects[i];
        if (currentObject->displayLists->opaqueDisplayList != NULL) {
            setupMultiPartObjectRenderState(displayObjects, i);
            displayListCmd = gDisplayListAllocPtr;
            displayListCmd->words.w0 = 0xDE000000;
            displayListCmd->words.w1 = (u32)currentObject->displayLists->opaqueDisplayList;
            gDisplayListAllocPtr = displayListCmd + 1;
        }
    }
}

void renderMultiPartTransparentDisplayLists(DisplayListObject *displayObjects) {
    DisplayListObject *currentObject;
    s32 i;
    Gfx *displayListCmd;
    s32 objectCount;
    u8 pad[0x48];

    initializeMultiPartDisplayListObjects(displayObjects);

    objectCount = displayObjects->numParts;
    if (objectCount > 0) {
        currentObject = displayObjects;
        i = 0;
        do {
            if (currentObject[i].displayLists->transparentDisplayList != NULL) {
                setupMultiPartObjectRenderState(displayObjects, i);
                displayListCmd = gDisplayListAllocPtr;
                displayListCmd->words.w0 = 0xDE000000;
                displayListCmd->words.w1 = (u32)currentObject[i].displayLists->transparentDisplayList;
                gDisplayListAllocPtr = displayListCmd + 1;
            }
            i++;
            objectCount = displayObjects->numParts;
        } while (i < objectCount);
    }
}

void renderMultiPartOverlayDisplayLists(DisplayListObject *displayObjects) {
    DisplayListObject *currentObject;
    s32 i;
    Gfx *displayListCmd;
    s32 objectCount;
    u8 pad[0x48];

    initializeMultiPartDisplayListObjects(displayObjects);

    objectCount = displayObjects->numParts;
    if (objectCount > 0) {
        currentObject = displayObjects;
        i = 0;
        do {
            if (currentObject[i].displayLists->overlayDisplayList != NULL) {
                setupMultiPartObjectRenderState(displayObjects, i);
                displayListCmd = gDisplayListAllocPtr;
                displayListCmd->words.w0 = 0xDE000000;
                displayListCmd->words.w1 = (u32)currentObject[i].displayLists->overlayDisplayList;
                gDisplayListAllocPtr = displayListCmd + 1;
            }
            i++;
            objectCount = displayObjects->numParts;
        } while (i < objectCount);
    }
}

void enqueuePreLitMultiPartDisplayList(s32 arg0, enqueueMultiPartDisplayList_arg1 *arg1, s32 arg2) {
    enqueueMultiPartDisplayList_arg1 *new_var;
    s32 i;
    s32 renderFlags;
    s32 *dlPtrs;
    enqueueMultiPartDisplayList_arg1 *currentPart;
    volatile u8 padding[0x1];

    i = 0;
    renderFlags = 0;
    arg1->numParts = arg2;

    if (arg2 > 0) {
        currentPart = arg1;
        do {
            dlPtrs = currentPart->unk20;
            (new_var = currentPart)->unk30 = 0;
            if (dlPtrs[1] != 0) {
                renderFlags |= 1;
            }
            if (dlPtrs[2] != 0) {
                renderFlags |= 2;
            }
            if (dlPtrs[0x3] != 0) {
                renderFlags |= 4;
            }
            i += 1;
            currentPart = (enqueueMultiPartDisplayList_arg1 *)(((s32)currentPart) + 0x3C);
        } while (i < arg2);
    }

    if (renderFlags & 1) {
        enqueueCallbackBySlotIndex(arg0 & 0xFFFF, 1, &renderMultiPartOpaqueDisplayLists, arg1);
    }
    new_var = arg1;
    if (renderFlags & 2) {
        enqueueCallbackBySlotIndex((arg0 & 0xFFFF) ^ 0, 3, &renderMultiPartTransparentDisplayLists, new_var);
    }
    if (renderFlags & 4) {
        enqueueCallbackBySlotIndex(arg0 & 0xFFFF, 5, &renderMultiPartOverlayDisplayLists, arg1);
    }
}

void prepareDisplayListRenderStateWithLights(DisplayListObject *obj) {
    Mtx sp30;
    f32 sp70;
    f32 sp74;
    f32 sp78;
    f32 sp7C;
    f32 sp80;
    f32 sp84;
    LookAt *lookat;

    if (obj->transformMatrix == NULL) {
        obj->transformMatrix = arenaAlloc16(0x40);
        if (obj->transformMatrix == NULL) {
            return;
        }
        transform3DToN64Mtx((Transform3D *)obj, obj->transformMatrix);
    }

    if (obj->displayLists->flags & 1) {
        lookat = arenaAlloc16(0x20);
        if (lookat == NULL) {
            return;
        }

        matrixToEulerAngles(
            (Transform3D *)&gActiveViewport->cameraRotationMatrix,
            &obj->transform,
            &sp70,
            &sp74,
            &sp78,
            &sp7C,
            &sp80,
            &sp84
        );
        guLookAtReflect(&sp30, lookat, 0.0f, 0.0f, 0.0f, sp70, sp74, sp78, sp7C, sp80, sp84);
        gSPLookAt(gDisplayListAllocPtr++, lookat);
    }

    if (gGraphicsMode != 3) {
        gDPPipeSync(gDisplayListAllocPtr++);
        gDPSetTexturePersp(gDisplayListAllocPtr++, 0x80000);

        gGraphicsMode = 3;

        if (obj->segment1 != 0) {
            gSPSegment(gDisplayListAllocPtr++, 1, obj->segment1);
        }

        if (obj->segment2 != 0) {
            gSPSegment(gDisplayListAllocPtr++, 2, obj->segment2);
        }

        if (obj->segment3 != 0) {
            gSPSegment(gDisplayListAllocPtr++, 3, obj->segment3);
        }
    } else {
        if (obj->segment1 != 0 && obj->segment1 != D_800A2D40_A3940) {
            gSPSegment(gDisplayListAllocPtr++, 1, obj->segment1);
        }

        if (obj->segment2 != 0 && obj->segment2 != D_800A2D44_A3944) {
            gSPSegment(gDisplayListAllocPtr++, 2, obj->segment2);
        }

        if (obj->segment3 != 0 && obj->segment3 != D_800A2D48_A3948) {
            gSPSegment(gDisplayListAllocPtr++, 3, obj->segment3);
        }
    }

    D_800A2D40_A3940 = obj->segment1;
    D_800A2D44_A3944 = obj->segment2;
    D_800A2D48_A3948 = obj->segment3;

    gSPLightColor(gDisplayListAllocPtr++, LIGHT_1, obj->light1R << 24 | obj->light1G << 16 | obj->light1B << 8);
    gSPLightColor(gDisplayListAllocPtr++, LIGHT_2, obj->light2R << 24 | obj->light2G << 16 | obj->light2B << 8);
    gSPMatrix(gDisplayListAllocPtr++, obj->transformMatrix, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
}

void renderOpaqueDisplayListWithLights(DisplayListObject *arg0) {
    prepareDisplayListRenderStateWithLights(arg0);

    gSPDisplayList(gDisplayListAllocPtr++, arg0->displayLists->opaqueDisplayList);

    gSPLightColor(
        gDisplayListAllocPtr++,
        LIGHT_1,
        gActiveViewport->defaultLight1R << 0x18 | gActiveViewport->defaultLight1G << 0x10 |
            gActiveViewport->defaultLight1B << 8
    );

    gSPLightColor(
        gDisplayListAllocPtr++,
        LIGHT_2,
        gActiveViewport->defaultLight2R << 0x18 | gActiveViewport->defaultLight2G << 0x10 |
            gActiveViewport->defaultLight2B << 8
    );
}

void renderTransparentDisplayListWithLights(DisplayListObject *arg0) {
    prepareDisplayListRenderStateWithLights(arg0);

    gSPDisplayList(gDisplayListAllocPtr++, arg0->displayLists->transparentDisplayList);

    gSPLightColor(
        gDisplayListAllocPtr++,
        LIGHT_1,
        gActiveViewport->defaultLight1R << 0x18 | gActiveViewport->defaultLight1G << 0x10 |
            gActiveViewport->defaultLight1B << 8
    );

    gSPLightColor(
        gDisplayListAllocPtr++,
        LIGHT_2,
        gActiveViewport->defaultLight2R << 0x18 | gActiveViewport->defaultLight2G << 0x10 |
            gActiveViewport->defaultLight2B << 8
    );
}

void renderOverlayDisplayListWithLights(DisplayListObject *arg0) {
    prepareDisplayListRenderStateWithLights(arg0);

    gSPDisplayList(gDisplayListAllocPtr++, arg0->displayLists->overlayDisplayList);

    gSPLightColor(
        gDisplayListAllocPtr++,
        LIGHT_1,
        gActiveViewport->defaultLight1R << 0x18 | gActiveViewport->defaultLight1G << 0x10 |
            gActiveViewport->defaultLight1B << 8
    );

    gSPLightColor(
        gDisplayListAllocPtr++,
        LIGHT_2,
        gActiveViewport->defaultLight2R << 0x18 | gActiveViewport->defaultLight2G << 0x10 |
            gActiveViewport->defaultLight2B << 8
    );
}

void enqueueDisplayListObjectWithLights(s32 arg0, DisplayListObject *arg1) {
    arg1->transformMatrix = 0;

    if (arg1->displayLists->opaqueDisplayList != NULL) {
        enqueueCallbackBySlotIndex(arg0, 1, &renderOpaqueDisplayListWithLights, arg1);
    }

    if (arg1->displayLists->transparentDisplayList != NULL) {
        enqueueCallbackBySlotIndex(arg0, 3, &renderTransparentDisplayListWithLights, arg1);
    }

    if (arg1->displayLists->overlayDisplayList != NULL) {
        enqueueCallbackBySlotIndex(arg0, 5, &renderOverlayDisplayListWithLights, arg1);
    }
}

void renderMultiPartOpaqueDisplayListsWithLights(DisplayListObject *displayObjects) {
    DisplayListObject *currentObject;
    s32 i;
    Gfx *displayListCmd;
    s32 objectCount;
    u8 pad[0x44];

    initializeMultiPartDisplayListObjects(displayObjects);

    gSPLightColor(
        gDisplayListAllocPtr++,
        LIGHT_1,
        displayObjects->light1R << 0x18 | displayObjects->light1G << 0x10 | displayObjects->light1B << 8
    );

    gSPLightColor(
        gDisplayListAllocPtr++,
        LIGHT_2,
        displayObjects->light2R << 0x18 | displayObjects->light2G << 0x10 | displayObjects->light2B << 8
    );

    i = 0;
    objectCount = displayObjects->numParts;
    if (objectCount > 0) {
        currentObject = displayObjects;
        do {
            if (currentObject[i].displayLists->opaqueDisplayList != NULL) {
                setupMultiPartObjectRenderState(displayObjects, i);
                displayListCmd = gDisplayListAllocPtr;
                displayListCmd->words.w0 = 0xDE000000;
                displayListCmd->words.w1 = (u32)currentObject[i].displayLists->opaqueDisplayList;
                gDisplayListAllocPtr = displayListCmd + 1;
            }
            i += 1;
        } while (i < (s32)displayObjects->numParts);
    }

    gSPLightColor(
        gDisplayListAllocPtr++,
        LIGHT_1,
        gActiveViewport->defaultLight1R << 0x18 | gActiveViewport->defaultLight1G << 0x10 |
            gActiveViewport->defaultLight1B << 8
    );

    gSPLightColor(
        gDisplayListAllocPtr++,
        LIGHT_2,
        gActiveViewport->defaultLight2R << 0x18 | gActiveViewport->defaultLight2G << 0x10 |
            gActiveViewport->defaultLight2B << 8
    );
}

void renderMultiPartTransparentDisplayListsWithLights(DisplayListObject *displayObjects) {
    DisplayListObject *currentObject;
    s32 i;
    Gfx *displayListCmd;
    s32 objectCount;
    u8 pad[0x44];

    initializeMultiPartDisplayListObjects(displayObjects);

    gSPLightColor(
        gDisplayListAllocPtr++,
        LIGHT_1,
        displayObjects->light1R << 0x18 | displayObjects->light1G << 0x10 | displayObjects->light1B << 8
    );

    gSPLightColor(
        gDisplayListAllocPtr++,
        LIGHT_2,
        displayObjects->light2R << 0x18 | displayObjects->light2G << 0x10 | displayObjects->light2B << 8
    );

    i = 0;
    objectCount = displayObjects->numParts;
    if (objectCount > 0) {
        currentObject = displayObjects;
        do {
            if (currentObject[i].displayLists->transparentDisplayList != NULL) {
                setupMultiPartObjectRenderState(displayObjects, i);
                displayListCmd = gDisplayListAllocPtr;
                displayListCmd->words.w0 = 0xDE000000;
                displayListCmd->words.w1 = (u32)currentObject[i].displayLists->transparentDisplayList;
                gDisplayListAllocPtr = displayListCmd + 1;
            }
            i += 1;
        } while (i < (s32)displayObjects->numParts);
    }

    gSPLightColor(
        gDisplayListAllocPtr++,
        LIGHT_1,
        gActiveViewport->defaultLight1R << 0x18 | gActiveViewport->defaultLight1G << 0x10 |
            gActiveViewport->defaultLight1B << 8
    );

    gSPLightColor(
        gDisplayListAllocPtr++,
        LIGHT_2,
        gActiveViewport->defaultLight2R << 0x18 | gActiveViewport->defaultLight2G << 0x10 |
            gActiveViewport->defaultLight2B << 8
    );
}

void renderMultiPartOverlayDisplayListsWithLights(DisplayListObject *displayObjects) {
    DisplayListObject *currentObject;
    s32 i;
    Gfx *displayListCmd;
    s32 objectCount;
    u8 pad[0x44];

    initializeMultiPartDisplayListObjects(displayObjects);

    gSPLightColor(
        gDisplayListAllocPtr++,
        LIGHT_1,
        displayObjects->light1R << 0x18 | displayObjects->light1G << 0x10 | displayObjects->light1B << 8
    );

    gSPLightColor(
        gDisplayListAllocPtr++,
        LIGHT_2,
        displayObjects->light2R << 0x18 | displayObjects->light2G << 0x10 | displayObjects->light2B << 8
    );

    i = 0;
    objectCount = displayObjects->numParts;
    if (objectCount > 0) {
        currentObject = displayObjects;
        do {
            if (currentObject[i].displayLists->overlayDisplayList != NULL) {
                setupMultiPartObjectRenderState(displayObjects, i);
                displayListCmd = gDisplayListAllocPtr;
                displayListCmd->words.w0 = 0xDE000000;
                displayListCmd->words.w1 = (u32)currentObject[i].displayLists->overlayDisplayList;
                gDisplayListAllocPtr = displayListCmd + 1;
            }
            i += 1;
        } while (i < (s32)displayObjects->numParts);
    }

    gSPLightColor(
        gDisplayListAllocPtr++,
        LIGHT_1,
        gActiveViewport->defaultLight1R << 0x18 | gActiveViewport->defaultLight1G << 0x10 |
            gActiveViewport->defaultLight1B << 8
    );

    gSPLightColor(
        gDisplayListAllocPtr++,
        LIGHT_2,
        gActiveViewport->defaultLight2R << 0x18 | gActiveViewport->defaultLight2G << 0x10 |
            gActiveViewport->defaultLight2B << 8
    );
}

void enqueueMultiPartDisplayList(s32 arg0, enqueueMultiPartDisplayList_arg1 *arg1, s32 arg2) {
    enqueueMultiPartDisplayList_arg1 *new_var;
    s32 var_a1;
    s32 var_s0;
    s32 *temp_v1;
    enqueueMultiPartDisplayList_arg1 *var_a0;
    volatile u8 padding[0x1];
    var_a1 = 0;
    var_s0 = 0;
    arg1->numParts = arg2;
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
        enqueueCallbackBySlotIndex(arg0 & 0xFFFF, 1, &renderMultiPartOpaqueDisplayListsWithLights, arg1);
    }
    new_var = arg1;
    if (var_s0 & 2) {
        enqueueCallbackBySlotIndex((arg0 & 0xFFFF) ^ 0, 3, &renderMultiPartTransparentDisplayListsWithLights, new_var);
    }
    if (var_s0 & 4) {
        enqueueCallbackBySlotIndex(arg0 & 0xFFFF, 5, &renderMultiPartOverlayDisplayListsWithLights, arg1);
    }
}

void renderCameraRelativeDisplayList(DisplayListObject *displayListObj) {
    Mtx lookAtMatrix;
    f32 lookAtX;
    f32 lookAtY;
    f32 lookAtZ;
    f32 upX;
    f32 upY;
    f32 upZ;
    LookAt *lookAt;

    displayListObj->transformMatrix = arenaAlloc16(0x40);
    if (displayListObj->transformMatrix == NULL) {
        return;
    }

    memcpy(&gScaleMatrix.translation, &gActiveViewport->cameraX, sizeof(Vec3i));
    transform3DToMtx(&gScaleMatrix, displayListObj->transformMatrix);

    if (displayListObj->displayLists->flags & 1) {
        lookAt = arenaAlloc16(0x20);
        if (lookAt == NULL) {
            return;
        }

        matrixToEulerAngles(
            (Transform3D *)&gActiveViewport->cameraRotationMatrix,
            &displayListObj->transform,
            &lookAtX,
            &lookAtY,
            &lookAtZ,
            &upX,
            &upY,
            &upZ
        );
        guLookAtReflect(&lookAtMatrix, lookAt, 0.0f, 0.0f, 0.0f, lookAtX, lookAtY, lookAtZ, upX, upY, upZ);
        gSPLookAt(gDisplayListAllocPtr++, lookAt);
    }

    if (gGraphicsMode != 3) {
        gDPPipeSync(gDisplayListAllocPtr++);
        gDPSetTexturePersp(gDisplayListAllocPtr++, 0x80000);

        gGraphicsMode = 3;

        if (displayListObj->segment1 != 0) {
            gSPSegment(gDisplayListAllocPtr++, 1, displayListObj->segment1);
        }

        if (displayListObj->segment2 != 0) {
            gSPSegment(gDisplayListAllocPtr++, 2, displayListObj->segment2);
        }

        if (displayListObj->segment3 != 0) {
            gSPSegment(gDisplayListAllocPtr++, 3, displayListObj->segment3);
        }
    } else {
        if (displayListObj->segment1 != 0 && displayListObj->segment1 != D_800A2D40_A3940) {
            gSPSegment(gDisplayListAllocPtr++, 1, displayListObj->segment1);
        }

        if (displayListObj->segment2 != 0 && displayListObj->segment2 != D_800A2D44_A3944) {
            gSPSegment(gDisplayListAllocPtr++, 2, displayListObj->segment2);
        }

        if (displayListObj->segment3 != 0 && displayListObj->segment3 != D_800A2D48_A3948) {
            gSPSegment(gDisplayListAllocPtr++, 3, displayListObj->segment3);
        }
    }

    D_800A2D40_A3940 = displayListObj->segment1;
    D_800A2D44_A3944 = displayListObj->segment2;
    D_800A2D48_A3948 = displayListObj->segment3;

    gSPMatrix(gDisplayListAllocPtr++, displayListObj->transformMatrix, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);

    if (displayListObj->displayLists->opaqueDisplayList != 0) {
        gSPDisplayList(gDisplayListAllocPtr++, displayListObj->displayLists->opaqueDisplayList);
    }

    if (displayListObj->displayLists->transparentDisplayList != 0) {
        gSPDisplayList(gDisplayListAllocPtr++, displayListObj->displayLists->transparentDisplayList);
    }

    if (displayListObj->displayLists->overlayDisplayList != 0) {
        gSPDisplayList(gDisplayListAllocPtr++, displayListObj->displayLists->overlayDisplayList);
    }
}

void enqueueCameraRelativeDisplayList(s32 arg0, DisplayListObject *arg1) {
    arg1->transformMatrix = 0;
    enqueueCallbackBySlotIndex(arg0 & 0xFFFF, 0, &renderCameraRelativeDisplayList, arg1);
}

void renderTexturedBillboardSprite(TexturedSpriteState *state) {
    if ((u32)((gActiveViewport->cameraX - state->posX) + 0x0FEA0000) > 0x1FD40000U) {
        return;
    }
    if ((u32)((gActiveViewport->cameraZ - state->posZ) + 0x0FEA0000) > 0x1FD40000U) {
        return;
    }
    if ((u32)((gActiveViewport->cameraY - state->posY) + 0x0FEA0000) > 0x1FD40000U) {
        return;
    }

    if (state->matrix == NULL) {
        state->matrix = arenaAlloc16(0x40);
        if (state->matrix == NULL) {
            return;
        }
        memcpy(&gScaleMatrix.translation, &state->posX, sizeof(Vec3i));
        transform3DToMtx(&gScaleMatrix, state->matrix);
    }

    if (gGraphicsMode != 6) {
        gGraphicsMode = 6;

        gSPDisplayList(gDisplayListAllocPtr++, D_8009A780_9B380);

        gDPLoadTextureBlock_4b(
            gDisplayListAllocPtr++,
            state->textureData,
            G_IM_FMT_CI,
            state->width,
            state->height,
            0,
            G_TX_CLAMP,
            G_TX_CLAMP,
            G_TX_NOMASK,
            G_TX_NOMASK,
            G_TX_NOLOD,
            G_TX_NOLOD
        );

        gDPLoadTLUT_pal16(gDisplayListAllocPtr++, 0, state->paletteData);

        D_800A2D4C_A394C = (u32)state->textureData;
        D_800A2D50_A3950 = (u32)state->paletteData;
    } else {
        if (D_800A2D4C_A394C != (u32)state->textureData) {
            gDPLoadTextureBlock_4b(
                gDisplayListAllocPtr++,
                state->textureData,
                G_IM_FMT_CI,
                state->width,
                state->height,
                0,
                G_TX_CLAMP,
                G_TX_CLAMP,
                G_TX_NOMASK,
                G_TX_NOMASK,
                G_TX_NOLOD,
                G_TX_NOLOD
            );

            D_800A2D4C_A394C = (u32)state->textureData;
        }

        if (D_800A2D50_A3950 != (u32)state->paletteData) {
            gDPLoadTLUT_pal16(gDisplayListAllocPtr++, 0, state->paletteData);

            D_800A2D50_A3950 = (u32)state->paletteData;
        }
    }

    gSPMatrix(gDisplayListAllocPtr++, state->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPMatrix(gDisplayListAllocPtr++, gLookAtPtr, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    gSPVertex(gDisplayListAllocPtr++, state->vertices, 4, 0);
    gSP2Triangles(gDisplayListAllocPtr++, 0, 3, 2, 0, 2, 1, 0, 0);
}

void enqueueTexturedBillboardSprite(s32 arg0, TexturedBillboardSprite *arg1) {
    arg1->matrix = NULL;
    enqueueCallbackBySlotIndex(arg0 & 0xFFFF, 4, &renderTexturedBillboardSprite, arg1);
}

void renderRotatedBillboardSprite(RotatedBillboardSprite *state) {
    if ((u32)((gActiveViewport->cameraX - state->posX) + 0x0FEA0000) > 0x1FD40000U) {
        return;
    }
    if ((u32)((gActiveViewport->cameraZ - state->posZ) + 0x0FEA0000) > 0x1FD40000U) {
        return;
    }
    if ((u32)((gActiveViewport->cameraY - state->posY) + 0x0FEA0000) > 0x1FD40000U) {
        return;
    }

    if (state->matrix == NULL) {
        state->matrix = arenaAlloc16(0x40);
        if (state->matrix == NULL) {
            return;
        }
        transform3DToMtx(&state->rotation, state->matrix);
    }

    if (gGraphicsMode != 6) {
        gGraphicsMode = 6;

        gSPDisplayList(gDisplayListAllocPtr++, D_8009A780_9B380);

        gDPLoadTextureBlock_4b(
            gDisplayListAllocPtr++,
            state->textureData,
            G_IM_FMT_CI,
            state->width,
            state->height,
            0,
            G_TX_CLAMP,
            G_TX_CLAMP,
            G_TX_NOMASK,
            G_TX_NOMASK,
            G_TX_NOLOD,
            G_TX_NOLOD
        );

        gDPLoadTLUT_pal16(gDisplayListAllocPtr++, 0, state->paletteData);

        D_800A2D4C_A394C = (u32)state->textureData;
        D_800A2D50_A3950 = (u32)state->paletteData;
    } else {
        if (D_800A2D4C_A394C != (u32)state->textureData) {
            gDPLoadTextureBlock_4b(
                gDisplayListAllocPtr++,
                state->textureData,
                G_IM_FMT_CI,
                state->width,
                state->height,
                0,
                G_TX_CLAMP,
                G_TX_CLAMP,
                G_TX_NOMASK,
                G_TX_NOMASK,
                G_TX_NOLOD,
                G_TX_NOLOD
            );

            D_800A2D4C_A394C = (u32)state->textureData;
        }

        if (D_800A2D50_A3950 != (u32)state->paletteData) {
            gDPLoadTLUT_pal16(gDisplayListAllocPtr++, 0, state->paletteData);

            D_800A2D50_A3950 = (u32)state->paletteData;
        }
    }

    gSPMatrix(gDisplayListAllocPtr++, state->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPMatrix(gDisplayListAllocPtr++, gLookAtPtr, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    gSPVertex(gDisplayListAllocPtr++, state->vertices, 4, 0);
    gSP2Triangles(gDisplayListAllocPtr++, 0, 3, 2, 0, 2, 1, 0, 0);
}

void enqueueRotatedBillboardSprite(s32 arg0, MatrixEntry_202A0 *arg1) {
    arg1->unk30 = 0;
    enqueueCallbackBySlotIndex(arg0 & 0xFFFF, 4, &renderRotatedBillboardSprite, arg1);
}

void renderTexturedBillboardSpriteTile(TexturedSpriteState *state) {
    if ((u32)((gActiveViewport->cameraX - state->posX) + 0x0FEA0000) > 0x1FD40000U) {
        return;
    }
    if ((u32)((gActiveViewport->cameraZ - state->posZ) + 0x0FEA0000) > 0x1FD40000U) {
        return;
    }
    if ((u32)((gActiveViewport->cameraY - state->posY) + 0x0FEA0000) > 0x1FD40000U) {
        return;
    }

    if (state->matrix == NULL) {
        state->matrix = arenaAlloc16(0x40);
        if (state->matrix == NULL) {
            return;
        }
        memcpy(&gScaleMatrix.translation, &state->posX, sizeof(Vec3i));
        transform3DToMtx(&gScaleMatrix, state->matrix);
    }

    if (gGraphicsMode != 6) {
        gGraphicsMode = 6;

        gSPDisplayList(gDisplayListAllocPtr++, D_8009A780_9B380);

        gDPLoadTextureTile_4b(
            gDisplayListAllocPtr++,
            state->textureData,
            G_IM_FMT_CI,
            state->width,
            state->height,
            0,
            0,
            state->width,
            state->height,
            0,
            G_TX_CLAMP,
            G_TX_CLAMP,
            G_TX_NOMASK,
            G_TX_NOMASK,
            G_TX_NOLOD,
            G_TX_NOLOD
        );

        gDPLoadTLUT_pal16(gDisplayListAllocPtr++, 0, state->paletteData);

        D_800A2D4C_A394C = (u32)state->textureData;
        D_800A2D50_A3950 = (u32)state->paletteData;
    } else {
        if (D_800A2D4C_A394C != (u32)state->textureData) {
            gDPLoadTextureTile_4b(
                gDisplayListAllocPtr++,
                state->textureData,
                G_IM_FMT_CI,
                state->width,
                state->height,
                0,
                0,
                state->width,
                state->height,
                0,
                G_TX_CLAMP,
                G_TX_CLAMP,
                G_TX_NOMASK,
                G_TX_NOMASK,
                G_TX_NOLOD,
                G_TX_NOLOD
            );

            D_800A2D4C_A394C = (u32)state->textureData;
        }

        if (D_800A2D50_A3950 != (u32)state->paletteData) {
            gDPLoadTLUT_pal16(gDisplayListAllocPtr++, 0, state->paletteData);

            D_800A2D50_A3950 = (u32)state->paletteData;
        }
    }

    gSPMatrix(gDisplayListAllocPtr++, state->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPMatrix(gDisplayListAllocPtr++, gLookAtPtr, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    gSPVertex(gDisplayListAllocPtr++, state->vertices, 4, 0);
    gSP2Triangles(gDisplayListAllocPtr++, 0, 3, 2, 0, 2, 1, 0, 0);
}

void enqueueTexturedBillboardSpriteTile(u16 arg0, TexturedBillboardSprite *arg1) {
    arg1->matrix = NULL;
    enqueueCallbackBySlotIndex(arg0 & 0xFFFF, 4, &renderTexturedBillboardSpriteTile, arg1);
}

void renderAlphaBillboardSprite(AlphaSpriteState *state) {
    if ((u32)((gActiveViewport->cameraX - state->posX) + 0x0FEA0000) > 0x1FD40000U) {
        return;
    }
    if ((u32)((gActiveViewport->cameraZ - state->posZ) + 0x0FEA0000) > 0x1FD40000U) {
        return;
    }
    if ((u32)((gActiveViewport->cameraY - state->posY) + 0x0FEA0000) > 0x1FD40000U) {
        return;
    }

    if (state->matrix == NULL) {
        state->matrix = arenaAlloc16(0x40);
        if (state->matrix == NULL) {
            return;
        }
        memcpy(&gScaleMatrix.translation, &state->posX, sizeof(Vec3i));
        transform3DToMtx(&gScaleMatrix, state->matrix);
    }

    if (gGraphicsMode != 7) {
        gGraphicsMode = 7;

        gSPDisplayList(gDisplayListAllocPtr++, gAlphaSpriteSetupDL);

        gDPLoadTextureBlock_4b(
            gDisplayListAllocPtr++,
            state->textureData,
            G_IM_FMT_CI,
            state->width,
            state->height,
            0,
            G_TX_CLAMP,
            G_TX_CLAMP,
            G_TX_NOMASK,
            G_TX_NOMASK,
            G_TX_NOLOD,
            G_TX_NOLOD
        );

        gDPLoadTLUT_pal16(gDisplayListAllocPtr++, 0, state->paletteData);

        gDPSetEnvColor(gDisplayListAllocPtr++, 0xFF, 0xFF, 0xFF, state->alpha);

        D_800A2D4C_A394C = (u32)state->textureData;
        D_800A2D50_A3950 = (u32)state->paletteData;
        D_800A2D54_A3954 = state->alpha;
    } else {
        if (D_800A2D4C_A394C != (u32)state->textureData) {
            gDPLoadTextureBlock_4b(
                gDisplayListAllocPtr++,
                state->textureData,
                G_IM_FMT_CI,
                state->width,
                state->height,
                0,
                G_TX_CLAMP,
                G_TX_CLAMP,
                G_TX_NOMASK,
                G_TX_NOMASK,
                G_TX_NOLOD,
                G_TX_NOLOD
            );

            D_800A2D4C_A394C = (u32)state->textureData;
        }

        if (D_800A2D50_A3950 != (u32)state->paletteData) {
            gDPLoadTLUT_pal16(gDisplayListAllocPtr++, 0, state->paletteData);

            D_800A2D50_A3950 = (u32)state->paletteData;
        }

        if (state->alpha != D_800A2D54_A3954) {
            gDPPipeSync(gDisplayListAllocPtr++);
            gDPSetEnvColor(gDisplayListAllocPtr++, 0xFF, 0xFF, 0xFF, state->alpha);
            D_800A2D54_A3954 = state->alpha;
        }
    }

    gSPMatrix(gDisplayListAllocPtr++, state->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPMatrix(gDisplayListAllocPtr++, gLookAtPtr, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    gSPVertex(gDisplayListAllocPtr++, state->vertices, 4, 0);
    gSP2Triangles(gDisplayListAllocPtr++, 0, 3, 2, 0, 2, 1, 0, 0);
}

void enqueueAlphaBillboardSprite(s32 arg0, loadAssetMetadata_arg *arg1) {
    (arg1 + 1)->assetTemplate = 0;
    enqueueCallbackBySlotIndex(arg0 & 0xFFFF, 6, &renderAlphaBillboardSprite, arg1);
}

void renderAlphaSprite(AlphaSpriteState *state) {
    // Frustum culling - check if sprite is within view distance
    if ((u32)((gActiveViewport->cameraX - state->posX) + 0x0FEA0000) > 0x1FD40000U) {
        return;
    }
    if ((u32)((gActiveViewport->cameraZ - state->posZ) + 0x0FEA0000) > 0x1FD40000U) {
        return;
    }
    if ((u32)((gActiveViewport->cameraY - state->posY) + 0x0FEA0000) > 0x1FD40000U) {
        return;
    }

    if (state->matrix == NULL) {
        state->matrix = arenaAlloc16(0x40);
        if (state->matrix == NULL) {
            return;
        }
        memcpy(&gScaleMatrix.translation, &state->posX, sizeof(Vec3i));
        transform3DToN64Mtx(&gScaleMatrix, state->matrix);
    }

    // Graphics mode 7 = alpha-blended textured sprites
    if (gGraphicsMode != 7) {
        gGraphicsMode = 7;

        gSPDisplayList(gDisplayListAllocPtr++, gAlphaSpriteSetupDL);

        gDPLoadTextureBlock_4b(
            gDisplayListAllocPtr++,
            state->textureData,
            G_IM_FMT_CI,
            state->width,
            state->height,
            0,
            G_TX_CLAMP,
            G_TX_CLAMP,
            G_TX_NOMASK,
            G_TX_NOMASK,
            G_TX_NOLOD,
            G_TX_NOLOD
        );

        gDPLoadTLUT_pal16(gDisplayListAllocPtr++, 0, state->paletteData);

        gDPSetEnvColor(gDisplayListAllocPtr++, 0xFF, 0xFF, 0xFF, state->alpha);

        D_800A2D4C_A394C = (u32)state->textureData;
        D_800A2D50_A3950 = (u32)state->paletteData;
        D_800A2D54_A3954 = state->alpha;
    } else {
        if (D_800A2D4C_A394C != (u32)state->textureData) {
            gDPLoadTextureBlock_4b(
                gDisplayListAllocPtr++,
                state->textureData,
                G_IM_FMT_CI,
                state->width,
                state->height,
                0,
                G_TX_CLAMP,
                G_TX_CLAMP,
                G_TX_NOMASK,
                G_TX_NOMASK,
                G_TX_NOLOD,
                G_TX_NOLOD
            );

            D_800A2D4C_A394C = (u32)state->textureData;
        }

        if (D_800A2D50_A3950 != (u32)state->paletteData) {
            gDPLoadTLUT_pal16(gDisplayListAllocPtr++, 0, state->paletteData);

            D_800A2D50_A3950 = (u32)state->paletteData;
        }

        if (state->alpha != D_800A2D54_A3954) {
            gDPPipeSync(gDisplayListAllocPtr++);
            gDPSetEnvColor(gDisplayListAllocPtr++, 0xFF, 0xFF, 0xFF, state->alpha);
            D_800A2D54_A3954 = state->alpha;
        }
    }

    gSPMatrix(gDisplayListAllocPtr++, state->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPMatrix(gDisplayListAllocPtr++, gLookAtPtr, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    gSPVertex(gDisplayListAllocPtr++, state->vertices, 4, 0);
    gSP2Triangles(gDisplayListAllocPtr++, 0, 3, 2, 0, 2, 1, 0, 0);
}

void enqueueAlphaSprite(s32 arg0, loadAssetMetadata_arg *arg1) {
    (arg1 + 1)->assetTemplate = 0;
    enqueueCallbackBySlotIndex(arg0, 6, &renderAlphaSprite, arg1);
}

void loadAssetMetadata(loadAssetMetadata_arg *arg0, void *arg1, s32 arg2) {
    OutputStruct_19E80 result;

    getTableEntryByU16Index(arg1, (s16)arg2, &result);
    arg0->data_ptr = result.data_ptr;
    arg0->index_ptr = result.index_ptr;
    arg0->unk18 = result.width;
    arg0->unk19 = result.height;
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
    arg0->unk18 = result.width;
    arg0->unk19 = result.height;
}

void initializeOverlaySystem(void) {
    void **temp_s0;

    temp_s0 = (void **)allocateTaskMemory(4);
    LOAD_OVERLAY(rand)
    *temp_s0 = loadCompressedData(&_215D70_ROM_START, &_215D70_ROM_END, 0x918);
    setGameStateHandler(&initializeFontSystemAndTransitionToMainMenu);
}

void initializeFontSystemAndTransitionToMainMenu(void) {
    FontSystemInitData *data = (FontSystemInitData *)getCurrentAllocation();
    loadFontAssetsFromDataTable(data->fontDataTable);
    terminateSchedulerWithCallback(&scheduleGameInitialization);
}

void checkSchedulerFlagAndTransitionToGameInit(void) {
    if (getSchedulerReturnValue() << 0x10 != 0) {
        terminateSchedulerWithCallback(&scheduleGameInitialization);
    }
}

void scheduleGameInitialization(void) {
    createRootTaskScheduler(&initGameSession, 200);
}
