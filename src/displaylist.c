#include "displaylist.h"

#include "19E80.h"
#include "5AA90.h"
#include "68CF0.h"
#include "6E840.h"
#include "common.h"
#include "gamestate.h"
#include "gbi.h"
#include "geometry.h"
#include "race_session.h"
#include "rom_loader.h"
#include "task_scheduler.h"

USE_OVERLAY(rand);
USE_ASSET(_215D70);

typedef struct {
    u8 padding[0x120];
    s32 unk120;
    u8 padding2[0x10];
    s32 unk134;
    s32 unk138;
    s32 unk13C;
    u8 padding3[0x8];
    u8 unk148;
    u8 unk149;
    u8 unk14A;
    u8 padding4[0xD];
    u8 unk158;
    u8 unk159;
    u8 unk15A;
} D_800AB068_A23D8_arg;

typedef struct {
    /* 0x00 */ s32 unk0;
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
    /* 0x00 */ void *unk0;
    /* 0x04 */ u8 unk4[0x14];
    /* 0x18 */ s32 posX;
    /* 0x1C */ s32 posY;
    /* 0x20 */ s32 posZ;
    /* 0x24 */ u8 *textureData;
    /* 0x28 */ u8 *paletteData;
    /* 0x2C */ u8 width;
    /* 0x2D */ u8 height;
    /* 0x2E */ u8 padding2[2];
    /* 0x30 */ Mtx *matrix;
} ExtendedSpriteState;

typedef struct {
    /* 0x00 */ s32 unk0;
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

extern s32 D_800A8B14_9FE84;
extern D_800AB068_A23D8_arg *D_800AB068_A23D8;
extern Gfx *gRegionAllocPtr;
extern s16 gGraphicsMode;
extern void *D_800A2D40_A3940;
extern void *D_800A2D44_A3944;
extern void *D_800A2D48_A3948;
extern s32 D_8009A8A4_9B4A4;
extern Gfx D_8009A780_9B380[];
extern Gfx D_8009A7D0_9B3D0[];
extern u32 D_800A2D4C_A394C;
extern u32 D_800A2D50_A3950;
extern u8 D_800A2D54_A3954;
extern u32 D_800A2D4C_A394C;
extern u32 D_800A2D50_A3950;

void func_80065150_65D50(DisplayListObject *displayObjects);
void func_800653E0_65FE0(DisplayListObject *displayObjects);
void func_80065670_66270(DisplayListObject *displayObjects);
void func_800659E4_665E4(DisplayListObject *arg0);
void func_80065DD8_669D8(TexturedSpriteState *);
void func_80066474_67074(ExtendedSpriteState *);
void func_800677F0_683F0(AlphaSpriteState *);
void func_800670D4_67CD4(AlphaSpriteState *);
void func_800680C4_68CC4(void);
void func_80064CF4_658F4(DisplayListObject *);
void prepareDisplayListRenderState(DisplayListObject *);
void setupDisplayListMatrix(DisplayListObject *);
void func_80063A94_64694(DisplayListObject *);
void func_800648EC_654EC(DisplayListObject *);
void func_80064F74_65B74(DisplayListObject *);
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

INCLUDE_ASM("asm/nonmatchings/displaylist", func_80060A3C_6163C);

INCLUDE_ASM("asm/nonmatchings/displaylist", func_80060CDC_618DC);

INCLUDE_ASM("asm/nonmatchings/displaylist", func_80061A64_62664);

INCLUDE_ASM("asm/nonmatchings/displaylist", func_80061D6C_6296C);

typedef struct {
    s16 x;
    s16 y;
    s16 z;
} Vertex6;

typedef struct {
    u16 v0;
    u16 v1;
    u16 v2;
    u8 flags;
    u8 surfaceIndex;
} TrackFace;

typedef struct {
    s16 unk0;
    u8 padding[0xA];
    u16 baseIndex;
    u16 count;
    u8 padding2[0x14];
} TrackFaceGroup;

typedef struct {
    /* 0x00 */ s16 nextElementIdx;
    /* 0x02 */ u8 padding[0x14];
    /* 0x16 */ u16 vertexIdx1;
    /* 0x18 */ u8 padding2[0x4];
    /* 0x1C */ u16 vertexIdx2;
    /* 0x1E */ u8 padding3[0x6];
} TrackSegmentElement;

typedef struct {
    /* 0x00 */ void *unk0;
    /* 0x04 */ Vertex6 *vertices;
    /* 0x08 */ void *unk8;
    /* 0x0C */ TrackSegmentElement *elements;
} TrackGeometryData;

struct TrackGeometryFaceData {
    void *unk0;
    Vertex6 *vertices;
    TrackFace *faces;
    TrackFaceGroup *faceGroups;
};

typedef struct {
    s16 x;
    u8 pad[6];
    s16 z;
} PositionXZ;

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
    return ((TrackSegmentEntry *)gameData->section3Data)[index].finishZoneFlag;
}

INCLUDE_ASM("asm/nonmatchings/displaylist", func_80062274_62E74);

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

    return (func_8006D21C_6DE1C(vert0->x, vert0->z, vert1->x, vert1->z) - 0x1000) & 0xFFFF;
}

INCLUDE_ASM("asm/nonmatchings/displaylist", func_800626C4_632C4);

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
                    (cross2d(arg2->x, arg2->z, temp_s4 << 0x10, temp_s5 << 0x10, temp_s6 << 0x10, temp_s7 << 0x10) >= 0
                    )) {
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

u16 getTrackSegmentWaypoints(void *arg0_void, u16 waypointIdx, void *waypointStart, void *waypointEnd) {
    TrackGeometryData *trackGeom = (TrackGeometryData *)arg0_void;
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

    return (func_8006D21C_6DE1C(startVert->x, startVert->z, endVert->x, endVert->z) - 0x1000) & 0xFFFF;
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

    if (obj->unk30 == NULL) {
        obj->unk30 = arenaAlloc16(0x40);
        if (obj->unk30 == NULL) {
            return;
        }
        func_8006BFB8_6CBB8(obj, obj->unk30);
    }

    if (obj->displayLists->flags & 1) {
        lookAt = (LookAt *)arenaAlloc16(0x20);
        if (lookAt == NULL) {
            return;
        }

        matrixToEulerAngles(&D_800AB068_A23D8->unk120, (s32 *)obj, &lookAtX, &lookAtY, &lookAtZ, &upX, &upY, &upZ);
        guLookAtReflect(&tempMatrix, lookAt, 0.0f, 0.0f, 0.0f, lookAtX, lookAtY, lookAtZ, upX, upY, upZ);
        gSPLookAt(gRegionAllocPtr++, lookAt);
    }

    if (gGraphicsMode != 3) {
        gDPPipeSync(gRegionAllocPtr++);
        gDPSetTexturePersp(gRegionAllocPtr++, 0x80000);

        gGraphicsMode = 3;

        if (obj->unk24 != 0) {
            gSPSegment(gRegionAllocPtr++, 1, obj->unk24);
        }

        if (obj->unk28 != 0) {
            gSPSegment(gRegionAllocPtr++, 2, obj->unk28);
        }

        if (obj->unk2C != 0) {
            gSPSegment(gRegionAllocPtr++, 3, obj->unk2C);
        }
    } else {
        if (obj->unk24 != 0 && obj->unk24 != D_800A2D40_A3940) {
            gSPSegment(gRegionAllocPtr++, 1, obj->unk24);
        }

        if (obj->unk28 != 0 && obj->unk28 != D_800A2D44_A3944) {
            gSPSegment(gRegionAllocPtr++, 2, obj->unk28);
        }

        if (obj->unk2C != 0 && obj->unk2C != D_800A2D48_A3948) {
            gSPSegment(gRegionAllocPtr++, 3, obj->unk2C);
        }
    }

    D_800A2D40_A3940 = obj->unk24;
    D_800A2D44_A3944 = obj->unk28;
    D_800A2D48_A3948 = obj->unk2C;

    gSPMatrix(gRegionAllocPtr++, obj->unk30, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
}

void renderOpaqueDisplayList(DisplayListObject *arg0) {
    prepareDisplayListRenderState(arg0);
    gSPDisplayList(gRegionAllocPtr++, arg0->displayLists->opaqueDisplayList);
}

void renderTransparentDisplayList(DisplayListObject *arg0) {
    prepareDisplayListRenderState(arg0);
    gSPDisplayList(gRegionAllocPtr++, arg0->displayLists->transparentDisplayList);
}

void renderOverlayDisplayList(DisplayListObject *arg0) {
    prepareDisplayListRenderState(arg0);
    gSPDisplayList(gRegionAllocPtr++, arg0->displayLists->overlayDisplayList);
}

void enqueueDisplayListObjectWithFullRenderState(s32 arg0, void *arg1) {
    ((DisplayListObject *)arg1)->unk30 = 0;

    if (((DisplayListObject *)arg1)->displayLists->opaqueDisplayList != NULL) {
        debugEnqueueCallback(arg0, 1, renderOpaqueDisplayList, arg1);
    }

    if (((DisplayListObject *)arg1)->displayLists->transparentDisplayList != NULL) {
        debugEnqueueCallback(arg0, 3, renderTransparentDisplayList, arg1);
    }

    if (((DisplayListObject *)arg1)->displayLists->overlayDisplayList != NULL) {
        debugEnqueueCallback(arg0, 5, renderOverlayDisplayList, arg1);
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
        arg0->unk30 = arenaAlloc16(0x40);
        if (arg0->unk30 == NULL) {
            return;
        }
        func_8006C130_6CD30((Transform3D *)arg0, arg0->unk30);
    }

    if (arg0->displayLists->flags & 1) {
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

void renderOpaqueDisplayListCallback(DisplayListObject *obj) {
    setupDisplayListMatrix(obj);
    gSPDisplayList(gRegionAllocPtr++, obj->displayLists->opaqueDisplayList);
}

void renderTransparentDisplayListCallback(DisplayListObject *obj) {
    setupDisplayListMatrix(obj);
    gSPDisplayList(gRegionAllocPtr++, obj->displayLists->transparentDisplayList);
}

void renderOverlayDisplayListCallback(DisplayListObject *obj) {
    setupDisplayListMatrix(obj);
    gSPDisplayList(gRegionAllocPtr++, obj->displayLists->overlayDisplayList);
}

void enqueueDisplayListObject(s32 arg0, DisplayListObject *arg1) {
    arg1->unk30 = 0;
    if (arg1->displayLists->opaqueDisplayList != NULL) {
        debugEnqueueCallback(arg0 & 0xFFFF, 1, renderOpaqueDisplayListCallback, arg1);
    }
    if (arg1->displayLists->transparentDisplayList != NULL) {
        debugEnqueueCallback(arg0 & 0xFFFF, 3, renderTransparentDisplayListCallback, arg1);
    }
    if (arg1->displayLists->overlayDisplayList != NULL) {
        debugEnqueueCallback(arg0 & 0xFFFF, 5, renderOverlayDisplayListCallback, arg1);
    }
}

void renderOpaqueDisplayListWithFrustumCull(DisplayListObject *arg0) {
    if (!isObjectCulled(&arg0->transform.translation)) {
        setupDisplayListMatrix(arg0);
        gSPDisplayList(gRegionAllocPtr++, arg0->displayLists->opaqueDisplayList);
    }
}

void renderTransparentDisplayListWithFrustumCull(DisplayListObject *arg0) {
    if (!isObjectCulled(&arg0->transform.translation)) {
        setupDisplayListMatrix(arg0);
        gSPDisplayList(gRegionAllocPtr++, arg0->displayLists->transparentDisplayList);
    }
}

void renderOverlayDisplayListWithFrustumCull(DisplayListObject *arg0) {
    s32 *temp_v1;

    if (!isObjectCulled(&arg0->transform.translation)) {
        setupDisplayListMatrix(arg0);
        gSPDisplayList(gRegionAllocPtr++, arg0->displayLists->overlayDisplayList);
    }
}

void enqueueDisplayListWithFrustumCull(s32 arg0, DisplayListObject *arg1) {
    arg1->unk30 = 0;
    if (arg1->displayLists->opaqueDisplayList != NULL) {
        debugEnqueueCallback(arg0, 1, &renderOpaqueDisplayListWithFrustumCull, arg1);
    }
    if (arg1->displayLists->transparentDisplayList != NULL) {
        debugEnqueueCallback(arg0, 3, &renderTransparentDisplayListWithFrustumCull, arg1);
    }
    if (arg1->displayLists->overlayDisplayList != NULL) {
        debugEnqueueCallback(arg0, 5, &renderOverlayDisplayListWithFrustumCull, arg1);
    }
}

void buildDisplayListSegment(DisplayListObject *arg0) {
    if (!isObjectCulled(&arg0->transform.translation)) {
        setupDisplayListMatrix(arg0);

        gDPPipeSync(gRegionAllocPtr++);

        gDPSetColor(gRegionAllocPtr++, 0xFB, arg0->envColorAlpha | ~0xFF);

        gSPDisplayList(gRegionAllocPtr++, arg0->displayLists->opaqueDisplayList);
    }
}

void buildTransparentDisplayListSegment(DisplayListObject *arg0) {
    Gfx *dl;

    if (!isObjectCulled(&arg0->transform.translation)) {
        setupDisplayListMatrix(arg0);

        dl = gRegionAllocPtr;
        gRegionAllocPtr += 1;
        dl->words.w0 = 0xE7000000;
        dl->words.w1 = 0;

        gRegionAllocPtr += 1;
        dl[1].words.w0 = 0xFB000000;
        dl[1].words.w1 = arg0->envColorAlpha | 0xFFFFFF00;

        gRegionAllocPtr += 1;
        dl[2].words.w0 = 0xDE000000;
        dl[2].words.w1 = (u32)arg0->displayLists->transparentDisplayList;
    }
}

void buildOverlayDisplayListSegment(DisplayListObject *obj) {
    if (!isObjectCulled(&obj->transform.translation)) {
        setupDisplayListMatrix(obj);

        gDPPipeSync(gRegionAllocPtr++);
        gDPSetColor(gRegionAllocPtr++, 0xFB, obj->envColorAlpha | ~0xFF);
        gSPDisplayList(gRegionAllocPtr++, obj->displayLists->overlayDisplayList);
    }
}

void func_800639F8_645F8(s32 arg0, DisplayListObject *arg1) {
    arg1->unk30 = 0;
    if (arg1->displayLists->opaqueDisplayList != NULL) {
        debugEnqueueCallback(arg0 & 0xFFFF, 1, &buildDisplayListSegment, arg1);
    }
    if (arg1->displayLists->transparentDisplayList != NULL) {
        debugEnqueueCallback(arg0 & 0xFFFF, 3, &buildTransparentDisplayListSegment, arg1);
    }
    if (arg1->displayLists->overlayDisplayList != NULL) {
        debugEnqueueCallback(arg0 & 0xFFFF, 5, &buildOverlayDisplayListSegment, arg1);
    }
}

void func_80063A94_64694(DisplayListObject *obj) {
    Mtx tempMtx;
    f32 eyeX;
    f32 eyeY;
    int fracMask;
    s32 unused;
    f32 eyeZ;
    f32 upX;
    f32 upY;
    f32 upZ;
    LookAt *lookAt;
    Mtx *matrices;
    Gfx *dl;
    s32 posXInt;
    s32 posZInt;
    s16 *rotationMatrix = (s16 *)obj;

    if (obj->unk30 == NULL) {
        matrices = arenaAlloc16(0x80);
        obj->unk30 = matrices;
        if (matrices == NULL) {
            return;
        }
        /* First matrix: translation matrix */
        /* Integer portion (s32[0-7]) */
        ((s32 *)obj->unk30)[0] = 0x10000; /* m[0][0] = 1.0 (integer part: 1, high; 0, low) */
        ((s32 *)obj->unk30)[1] = 0;       /* m[0][1] = 0, m[0][2] = 0 */
        ((s32 *)obj->unk30)[2] = 1;       /* m[0][3] = 0 (int=0, frac packed), m[1][0] = 0 (int=1 for perspective) */
        ((s32 *)obj->unk30)[3] = 0;       /* m[1][1] = 0, m[1][2] = 0 */
        ((s32 *)obj->unk30)[4] = 0;       /* m[1][3] = 0, m[2][0] = 0 */
        ((s32 *)obj->unk30)[5] = 0x10000; /* m[2][1] = 0, m[2][2] = 1.0 (integer part) */
        ((s32 *)obj->unk30)[6] =
            (obj->transform.translation.x & 0xFFFF0000) + ((u16 *)&obj->transform.translation.y)[0];
        ((s32 *)obj->unk30)[7] = (obj->transform.translation.z & 0xFFFF0000) + 1;
        /* Fractional portion (s32[8-15]) */
        ((s32 *)obj->unk30)[8] = 0;
        ((s32 *)obj->unk30)[9] = 0;
        ((s32 *)obj->unk30)[10] = 0;
        ((s32 *)obj->unk30)[11] = 0;
        ((s32 *)obj->unk30)[12] = 0;
        ((s32 *)obj->unk30)[13] = 0;
        ((s32 *)obj->unk30)[14] = (obj->transform.translation.x << 16) + ((u16 *)&obj->transform.translation.y)[1];
        fracMask = 0xFFFF;
        ((s32 *)obj->unk30)[15] = obj->transform.translation.z << 16;
        /* Second matrix: rotation matrix built from Transform3D at offset 0 */
        /* Integer portion (s32[16-23]) - extracts high bits from s15 rotation values */
        ((s32 *)obj->unk30)[16] =
            ((rotationMatrix[0] * 2) & 0xFFFF0000) + (-(s32)((u16)rotationMatrix[1] >> 15) & fracMask);
        ((s32 *)obj->unk30)[17] = (rotationMatrix[2] * 2) & 0xFFFF0000;
        ((s32 *)obj->unk30)[18] =
            ((rotationMatrix[3] * 2) & 0xFFFF0000) + (-(s32)((u16)rotationMatrix[4] >> 15) & fracMask);
        ((s32 *)obj->unk30)[19] = (rotationMatrix[5] * 2) & 0xFFFF0000;
        ((s32 *)obj->unk30)[20] =
            ((rotationMatrix[6] * 2) & 0xFFFF0000) + (-(s32)((u16)rotationMatrix[7] >> 15) & 0xFFFF);
        ((s32 *)obj->unk30)[21] = (rotationMatrix[8] * 2) & 0xFFFF0000;
        ((s32 *)obj->unk30)[22] = 0;
        ((s32 *)obj->unk30)[23] = 1;
        /* Fractional portion (s32[24-31]) - extracts low bits from s15 rotation values */
        ((s32 *)obj->unk30)[24] = ((rotationMatrix[0] << 17) & 0xFFFF0000) + ((rotationMatrix[1] * 2) & fracMask);
        ((s32 *)obj->unk30)[25] = (rotationMatrix[2] << 17) & 0xFFFF0000;
        ((s32 *)obj->unk30)[26] = ((rotationMatrix[3] << 17) & 0xFFFF0000) + ((rotationMatrix[4] * 2) & fracMask);
        ((s32 *)obj->unk30)[27] = (rotationMatrix[5] << 17) & 0xFFFF0000;
        ((s32 *)obj->unk30)[28] = ((rotationMatrix[6] << 17) & 0xFFFF0000) + ((rotationMatrix[7] * 2) & fracMask);
        ((s32 *)obj->unk30)[29] = (rotationMatrix[8] << 17) & 0xFFFF0000;
        ((s32 *)obj->unk30)[30] = 0;
        ((s32 *)obj->unk30)[31] = 0;
    }

    if (obj->displayLists->flags & 1) {
        lookAt = arenaAlloc16(0x20);
        if (lookAt == NULL) {
            return;
        }

        matrixToEulerAngles(&D_800AB068_A23D8->unk120, (s32 *)obj, &eyeX, &eyeY, &eyeZ, &upX, &upY, &upZ);
        guLookAtReflect(&tempMtx, lookAt, 0.0f, 0.0f, 0.0f, eyeX, eyeY, eyeZ, upX, upY, upZ);
        gSPLookAt(gRegionAllocPtr++, lookAt);
    }

    if (gGraphicsMode != 3) {
        gDPPipeSync(gRegionAllocPtr++);
        gDPSetTexturePersp(gRegionAllocPtr++, 0x80000);

        gGraphicsMode = 3;

        if (obj->unk24 != NULL) {
            gSPSegment(gRegionAllocPtr++, 1, obj->unk24);
        }

        if (obj->unk28 != NULL) {
            gSPSegment(gRegionAllocPtr++, 2, obj->unk28);
        }

        if (obj->unk2C != NULL) {
            gSPSegment(gRegionAllocPtr++, 3, obj->unk2C);
        }

        D_800A2D40_A3940 = obj->unk24;
        D_800A2D44_A3944 = obj->unk28;
        D_800A2D48_A3948 = obj->unk2C;
    } else {
        if (obj->unk24 != D_800A2D40_A3940) {
            if (obj->unk24 != NULL) {
                gSPSegment(gRegionAllocPtr++, 1, obj->unk24);
            }
            D_800A2D40_A3940 = obj->unk24;
        }

        if (obj->unk28 != D_800A2D44_A3944) {
            if (obj->unk28 != NULL) {
                gSPSegment(gRegionAllocPtr++, 2, obj->unk28);
            }
            D_800A2D44_A3944 = obj->unk28;
        }

        if (obj->unk2C != D_800A2D48_A3948) {
            if (obj->unk2C != NULL) {
                gSPSegment(gRegionAllocPtr++, 3, obj->unk2C);
            }
            D_800A2D48_A3948 = obj->unk2C;
        }
    }

    gSPMatrix(gRegionAllocPtr++, obj->unk30, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    gSPMatrix(gRegionAllocPtr++, D_800A8B14_9FE84, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);

    gSPMatrix(gRegionAllocPtr++, &obj->unk30[1], G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
}

void func_8006405C_64C5C(DisplayListObject *arg0) {
    if (!isObjectCulled(&arg0->transform.translation)) {
        func_80063A94_64694(arg0);
        gSPDisplayList(gRegionAllocPtr++, arg0->displayLists->opaqueDisplayList);
    }
}

void func_800640BC_64CBC(DisplayListObject *arg0) {
    if (!isObjectCulled(&arg0->transform.translation)) {
        func_80063A94_64694(arg0);
        gSPDisplayList(gRegionAllocPtr++, arg0->displayLists->transparentDisplayList);
    }
}

void func_8006411C_64D1C(DisplayListObject *arg0) {
    if (!isObjectCulled(&arg0->transform.translation)) {
        func_80063A94_64694(arg0);
        gSPDisplayList(gRegionAllocPtr++, arg0->displayLists->overlayDisplayList);
    }
}

void func_8006417C_64D7C(s32 arg0, DisplayListObject *arg1) {
    arg1->unk30 = 0;

    if (arg1->displayLists->opaqueDisplayList != NULL) {
        debugEnqueueCallback(arg0, 1, &func_8006405C_64C5C, arg1);
    }

    if (arg1->displayLists->transparentDisplayList != NULL) {
        debugEnqueueCallback(arg0, 3, &func_800640BC_64CBC, arg1);
    }

    if (arg1->displayLists->overlayDisplayList != NULL) {
        debugEnqueueCallback(arg0, 5, &func_8006411C_64D1C, arg1);
    }
}

void func_80064218_64E18(DisplayListObject *arg0) {
    DisplayListObject *var_s0;
    Gfx *temp_v1;
    s32 var_s1;
    void *v1_temp;
    void *temp_a0_val;
    s32 count;
    volatile u8 padding[0x44];

    if (arg0->unk30 == NULL) {
        arg0->unk30 = arenaAlloc16(arg0->unk37 << 6);
        if (arg0->unk30 == NULL) {
            return;
        }
        count = arg0->unk37;
        var_s1 = 0;
        if (count > 0) {
            var_s0 = arg0;
            do {
                var_s0->unk30 = arg0->unk30 + var_s1;
                func_8006C130_6CD30((Transform3D *)var_s0, var_s0->unk30);
                var_s1 += 1;
                var_s0++;
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
        temp_a0_val = arg0->unk2C;
        gRegionAllocPtr = temp_v1 + 1;
        temp_v1->words.w1 = (u32)temp_a0_val;
    }

    D_800A2D40_A3940 = arg0->unk24;
    D_800A2D44_A3944 = arg0->unk28;
    D_800A2D48_A3948 = arg0->unk2C;
}

void func_800643AC_64FAC(DisplayListObject *arg0, s32 arg1) {
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

        matrixToEulerAngles(&D_800AB068_A23D8->unk120, (s32 *)arg0, &sp70, &sp74, &sp78, &sp7C, &sp80, &sp84);
        guLookAtReflect(&sp30, lookat, 0.0f, 0.0f, 0.0f, sp70, sp74, sp78, sp7C, sp80, sp84);

        gSPLookAt(gRegionAllocPtr++, lookat);
    }

    temp = obj->unk24;
    if (temp != D_800A2D40_A3940) {
        if (temp != NULL) {
            gSPSegment(gRegionAllocPtr++, 1, obj->unk24);
        }
        D_800A2D40_A3940 = obj->unk24;
    }

    elem = (DisplayListObject *)(arg1 * sizeof(DisplayListObject) + (s32)arg0);
    temp = elem->unk28;
    if (temp != D_800A2D44_A3944) {
        if (temp != NULL) {
            gSPSegment(gRegionAllocPtr++, 2, elem->unk28);
        }
        D_800A2D44_A3944 = elem->unk28;
    }

    elem = (DisplayListObject *)(arg1 * sizeof(DisplayListObject) + (s32)arg0);
    temp = elem->unk2C;
    if (temp != D_800A2D48_A3948) {
        if (temp != NULL) {
            gSPSegment(gRegionAllocPtr++, 3, elem->unk2C);
        }
        D_800A2D48_A3948 = elem->unk2C;
    }

    gSPMatrix(gRegionAllocPtr++, (arg1 + arg0)->unk30, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
}

void func_80064628_65228(DisplayListObject *displayObjects) {
    s32 i;
    DisplayListObject *currentObject;
    Gfx *displayListCmd;
    volatile u8 padding[0x40];

    func_80064218_64E18(displayObjects);

    for (i = 0; i < displayObjects->unk37; i++) {
        currentObject = &displayObjects[i];
        if (currentObject->displayLists->opaqueDisplayList != NULL) {
            func_800643AC_64FAC(displayObjects, i);
            displayListCmd = gRegionAllocPtr;
            displayListCmd->words.w0 = 0xDE000000;
            displayListCmd->words.w1 = (u32)currentObject->displayLists->opaqueDisplayList;
            gRegionAllocPtr = displayListCmd + 1;
        }
    }
}

void func_800646C8_652C8(DisplayListObject *displayObjects) {
    DisplayListObject *currentObject;
    s32 i;
    Gfx *displayListCmd;
    s32 objectCount;
    u8 pad[0x48];

    func_80064218_64E18(displayObjects);

    objectCount = displayObjects->unk37;
    if (objectCount > 0) {
        currentObject = displayObjects;
        i = 0;
        do {
            if (currentObject[i].displayLists->transparentDisplayList != NULL) {
                func_800643AC_64FAC(displayObjects, i);
                displayListCmd = gRegionAllocPtr;
                displayListCmd->words.w0 = 0xDE000000;
                displayListCmd->words.w1 = (u32)currentObject[i].displayLists->transparentDisplayList;
                gRegionAllocPtr = displayListCmd + 1;
            }
            i++;
            objectCount = displayObjects->unk37;
        } while (i < objectCount);
    }
}

void func_80064768_65368(DisplayListObject *displayObjects) {
    DisplayListObject *currentObject;
    s32 i;
    Gfx *displayListCmd;
    s32 objectCount;
    u8 pad[0x48];

    func_80064218_64E18(displayObjects);

    objectCount = displayObjects->unk37;
    if (objectCount > 0) {
        currentObject = displayObjects;
        i = 0;
        do {
            if (currentObject[i].displayLists->overlayDisplayList != NULL) {
                func_800643AC_64FAC(displayObjects, i);
                displayListCmd = gRegionAllocPtr;
                displayListCmd->words.w0 = 0xDE000000;
                displayListCmd->words.w1 = (u32)currentObject[i].displayLists->overlayDisplayList;
                gRegionAllocPtr = displayListCmd + 1;
            }
            i++;
            objectCount = displayObjects->unk37;
        } while (i < objectCount);
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

void func_800648EC_654EC(DisplayListObject *arg0) {
    Mtx sp30;
    f32 sp70;
    f32 sp74;
    f32 sp78;
    f32 sp7C;
    f32 sp80;
    f32 sp84;
    LookAt *lookat;

    if (arg0->unk30 == NULL) {
        arg0->unk30 = arenaAlloc16(0x40);
        if (arg0->unk30 == NULL) {
            return;
        }
        func_8006C130_6CD30((Transform3D *)arg0, arg0->unk30);
    }

    if (arg0->displayLists->flags & 1) {
        lookat = arenaAlloc16(0x20);
        if (lookat == NULL) {
            return;
        }

        matrixToEulerAngles(&D_800AB068_A23D8->unk120, (s32 *)arg0, &sp70, &sp74, &sp78, &sp7C, &sp80, &sp84);
        guLookAtReflect(&sp30, lookat, 0.0f, 0.0f, 0.0f, sp70, sp74, sp78, sp7C, sp80, sp84);
        gSPLookAt(gRegionAllocPtr++, lookat);
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
    } else {
        if (arg0->unk24 != 0 && arg0->unk24 != D_800A2D40_A3940) {
            gSPSegment(gRegionAllocPtr++, 1, arg0->unk24);
        }

        if (arg0->unk28 != 0 && arg0->unk28 != D_800A2D44_A3944) {
            gSPSegment(gRegionAllocPtr++, 2, arg0->unk28);
        }

        if (arg0->unk2C != 0 && arg0->unk2C != D_800A2D48_A3948) {
            gSPSegment(gRegionAllocPtr++, 3, arg0->unk2C);
        }
    }

    D_800A2D40_A3940 = arg0->unk24;
    D_800A2D44_A3944 = arg0->unk28;
    D_800A2D48_A3948 = arg0->unk2C;

    gSPLightColor(gRegionAllocPtr++, LIGHT_1, arg0->unk34 << 24 | arg0->unk35 << 16 | arg0->unk36 << 8);
    gSPLightColor(gRegionAllocPtr++, LIGHT_2, arg0->unk38 << 24 | arg0->unk39 << 16 | arg0->unk3A << 8);
    gSPMatrix(gRegionAllocPtr++, arg0->unk30, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
}

void func_80064CF4_658F4(DisplayListObject *arg0) {
    func_800648EC_654EC(arg0);

    gSPDisplayList(gRegionAllocPtr++, arg0->displayLists->opaqueDisplayList);

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

void func_80064E34_65A34(DisplayListObject *arg0) {
    func_800648EC_654EC(arg0);

    gSPDisplayList(gRegionAllocPtr++, arg0->displayLists->transparentDisplayList);

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
    func_800648EC_654EC(arg0);

    gSPDisplayList(gRegionAllocPtr++, arg0->displayLists->overlayDisplayList);

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

    if (arg1->displayLists->opaqueDisplayList != NULL) {
        debugEnqueueCallback(arg0, 1, &func_80064CF4_658F4, arg1);
    }

    if (arg1->displayLists->transparentDisplayList != NULL) {
        debugEnqueueCallback(arg0, 3, &func_80064E34_65A34, arg1);
    }

    if (arg1->displayLists->overlayDisplayList != NULL) {
        debugEnqueueCallback(arg0, 5, &func_80064F74_65B74, arg1);
    }
}

void func_80065150_65D50(DisplayListObject *displayObjects) {
    DisplayListObject *currentObject;
    s32 i;
    Gfx *displayListCmd;
    s32 objectCount;
    u8 pad[0x44];

    func_80064218_64E18(displayObjects);

    gSPLightColor(
        gRegionAllocPtr++,
        LIGHT_1,
        displayObjects->unk34 << 0x18 | displayObjects->unk35 << 0x10 | displayObjects->unk36 << 8
    );

    gSPLightColor(
        gRegionAllocPtr++,
        LIGHT_2,
        displayObjects->unk38 << 0x18 | displayObjects->unk39 << 0x10 | displayObjects->unk3A << 8
    );

    i = 0;
    objectCount = displayObjects->unk37;
    if (objectCount > 0) {
        currentObject = displayObjects;
        do {
            if (currentObject[i].displayLists->opaqueDisplayList != NULL) {
                func_800643AC_64FAC(displayObjects, i);
                displayListCmd = gRegionAllocPtr;
                displayListCmd->words.w0 = 0xDE000000;
                displayListCmd->words.w1 = (u32)currentObject[i].displayLists->opaqueDisplayList;
                gRegionAllocPtr = displayListCmd + 1;
            }
            i += 1;
        } while (i < (s32)displayObjects->unk37);
    }

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

void func_800653E0_65FE0(DisplayListObject *displayObjects) {
    DisplayListObject *currentObject;
    s32 i;
    Gfx *displayListCmd;
    s32 objectCount;
    u8 pad[0x44];

    func_80064218_64E18(displayObjects);

    gSPLightColor(
        gRegionAllocPtr++,
        LIGHT_1,
        displayObjects->unk34 << 0x18 | displayObjects->unk35 << 0x10 | displayObjects->unk36 << 8
    );

    gSPLightColor(
        gRegionAllocPtr++,
        LIGHT_2,
        displayObjects->unk38 << 0x18 | displayObjects->unk39 << 0x10 | displayObjects->unk3A << 8
    );

    i = 0;
    objectCount = displayObjects->unk37;
    if (objectCount > 0) {
        currentObject = displayObjects;
        do {
            if (currentObject[i].displayLists->transparentDisplayList != NULL) {
                func_800643AC_64FAC(displayObjects, i);
                displayListCmd = gRegionAllocPtr;
                displayListCmd->words.w0 = 0xDE000000;
                displayListCmd->words.w1 = (u32)currentObject[i].displayLists->transparentDisplayList;
                gRegionAllocPtr = displayListCmd + 1;
            }
            i += 1;
        } while (i < (s32)displayObjects->unk37);
    }

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

void func_80065670_66270(DisplayListObject *displayObjects) {
    DisplayListObject *currentObject;
    s32 i;
    Gfx *displayListCmd;
    s32 objectCount;
    u8 pad[0x44];

    func_80064218_64E18(displayObjects);

    gSPLightColor(
        gRegionAllocPtr++,
        LIGHT_1,
        displayObjects->unk34 << 0x18 | displayObjects->unk35 << 0x10 | displayObjects->unk36 << 8
    );

    gSPLightColor(
        gRegionAllocPtr++,
        LIGHT_2,
        displayObjects->unk38 << 0x18 | displayObjects->unk39 << 0x10 | displayObjects->unk3A << 8
    );

    i = 0;
    objectCount = displayObjects->unk37;
    if (objectCount > 0) {
        currentObject = displayObjects;
        do {
            if (currentObject[i].displayLists->overlayDisplayList != NULL) {
                func_800643AC_64FAC(displayObjects, i);
                displayListCmd = gRegionAllocPtr;
                displayListCmd->words.w0 = 0xDE000000;
                displayListCmd->words.w1 = (u32)currentObject[i].displayLists->overlayDisplayList;
                gRegionAllocPtr = displayListCmd + 1;
            }
            i += 1;
        } while (i < (s32)displayObjects->unk37);
    }

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

void func_800659E4_665E4(DisplayListObject *arg0) {
    Mtx sp30;
    f32 sp70;
    f32 sp74;
    f32 sp78;
    f32 sp7C;
    f32 sp80;
    f32 sp84;
    LookAt *temp_v0;
    void *alloc;

    arg0->unk30 = arenaAlloc16(0x40);
    if (arg0->unk30 == NULL) {
        return;
    }

    memcpy(&D_8009A8A4_9B4A4, &D_800AB068_A23D8->padding2[0x10], 0xC);
    func_8006BFB8_6CBB8(&D_8009A8A4_9B4A4 - 5, arg0->unk30);

    if (arg0->displayLists->flags & 1) {
        temp_v0 = arenaAlloc16(0x20);
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
    } else {
        if (arg0->unk24 != 0 && arg0->unk24 != D_800A2D40_A3940) {
            gSPSegment(gRegionAllocPtr++, 1, arg0->unk24);
        }

        if (arg0->unk28 != 0 && arg0->unk28 != D_800A2D44_A3944) {
            gSPSegment(gRegionAllocPtr++, 2, arg0->unk28);
        }

        if (arg0->unk2C != 0 && arg0->unk2C != D_800A2D48_A3948) {
            gSPSegment(gRegionAllocPtr++, 3, arg0->unk2C);
        }
    }

    D_800A2D40_A3940 = arg0->unk24;
    D_800A2D44_A3944 = arg0->unk28;
    D_800A2D48_A3948 = arg0->unk2C;

    gSPMatrix(gRegionAllocPtr++, arg0->unk30, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);

    if (arg0->displayLists->opaqueDisplayList != 0) {
        gSPDisplayList(gRegionAllocPtr++, arg0->displayLists->opaqueDisplayList);
    }

    if (arg0->displayLists->transparentDisplayList != 0) {
        gSPDisplayList(gRegionAllocPtr++, arg0->displayLists->transparentDisplayList);
    }

    if (arg0->displayLists->overlayDisplayList != 0) {
        gSPDisplayList(gRegionAllocPtr++, arg0->displayLists->overlayDisplayList);
    }
}

void func_80065DA8_669A8(s32 arg0, DisplayListObject *arg1) {
    arg1->unk30 = 0;
    debugEnqueueCallback(arg0 & 0xFFFF, 0, &func_800659E4_665E4, arg1);
}

void func_80065DD8_669D8(TexturedSpriteState *state) {
    if ((u32)((D_800AB068_A23D8->unk134 - state->posX) + 0x0FEA0000) > 0x1FD40000U) {
        return;
    }
    if ((u32)((D_800AB068_A23D8->unk13C - state->posZ) + 0x0FEA0000) > 0x1FD40000U) {
        return;
    }
    if ((u32)((D_800AB068_A23D8->unk138 - state->posY) + 0x0FEA0000) > 0x1FD40000U) {
        return;
    }

    if (state->matrix == NULL) {
        state->matrix = arenaAlloc16(0x40);
        if (state->matrix == NULL) {
            return;
        }
        memcpy(&D_8009A8A4_9B4A4, &state->posX, 0xC);
        func_8006BFB8_6CBB8(&D_8009A8A4_9B4A4 - 5, state->matrix);
    }

    if (gGraphicsMode != 6) {
        gGraphicsMode = 6;

        gSPDisplayList(gRegionAllocPtr++, D_8009A780_9B380);

        gDPLoadTextureBlock_4b(
            gRegionAllocPtr++,
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

        gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, state->paletteData);

        D_800A2D4C_A394C = (u32)state->textureData;
        D_800A2D50_A3950 = (u32)state->paletteData;
    } else {
        if (D_800A2D4C_A394C != (u32)state->textureData) {
            gDPLoadTextureBlock_4b(
                gRegionAllocPtr++,
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
            gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, state->paletteData);

            D_800A2D50_A3950 = (u32)state->paletteData;
        }
    }

    gSPMatrix(gRegionAllocPtr++, state->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPMatrix(gRegionAllocPtr++, D_800A8B14_9FE84, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    gSPVertex(gRegionAllocPtr++, state->unk0, 4, 0);
    gSP2Triangles(gRegionAllocPtr++, 0, 3, 2, 0, 2, 1, 0, 0);
}

void func_80066444_67044(s32 arg0, func_80066444_67044_arg1 *arg1) {
    arg1->unk1C = 0;
    debugEnqueueCallback(arg0 & 0xFFFF, 4, &func_80065DD8_669D8, arg1);
}

void func_80066474_67074(ExtendedSpriteState *state) {
    if ((u32)((D_800AB068_A23D8->unk134 - state->posX) + 0x0FEA0000) > 0x1FD40000U) {
        return;
    }
    if ((u32)((D_800AB068_A23D8->unk13C - state->posZ) + 0x0FEA0000) > 0x1FD40000U) {
        return;
    }
    if ((u32)((D_800AB068_A23D8->unk138 - state->posY) + 0x0FEA0000) > 0x1FD40000U) {
        return;
    }

    if (state->matrix == NULL) {
        state->matrix = arenaAlloc16(0x40);
        if (state->matrix == NULL) {
            return;
        }
        func_8006BFB8_6CBB8(&state->unk4, state->matrix);
    }

    if (gGraphicsMode != 6) {
        gGraphicsMode = 6;

        gSPDisplayList(gRegionAllocPtr++, D_8009A780_9B380);

        gDPLoadTextureBlock_4b(
            gRegionAllocPtr++,
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

        gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, state->paletteData);

        D_800A2D4C_A394C = (u32)state->textureData;
        D_800A2D50_A3950 = (u32)state->paletteData;
    } else {
        if (D_800A2D4C_A394C != (u32)state->textureData) {
            gDPLoadTextureBlock_4b(
                gRegionAllocPtr++,
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
            gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, state->paletteData);

            D_800A2D50_A3950 = (u32)state->paletteData;
        }
    }

    gSPMatrix(gRegionAllocPtr++, state->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPMatrix(gRegionAllocPtr++, D_800A8B14_9FE84, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    gSPVertex(gRegionAllocPtr++, state->unk0, 4, 0);
    gSP2Triangles(gRegionAllocPtr++, 0, 3, 2, 0, 2, 1, 0, 0);
}

void func_80066AC0_676C0(s32 arg0, MatrixEntry_202A0 *arg1) {
    arg1->unk30 = 0;
    debugEnqueueCallback(arg0 & 0xFFFF, 4, &func_80066474_67074, arg1);
}

void func_80066AF0_676F0(TexturedSpriteState *state) {
    if ((u32)((D_800AB068_A23D8->unk134 - state->posX) + 0x0FEA0000) > 0x1FD40000U) {
        return;
    }
    if ((u32)((D_800AB068_A23D8->unk13C - state->posZ) + 0x0FEA0000) > 0x1FD40000U) {
        return;
    }
    if ((u32)((D_800AB068_A23D8->unk138 - state->posY) + 0x0FEA0000) > 0x1FD40000U) {
        return;
    }

    if (state->matrix == NULL) {
        state->matrix = arenaAlloc16(0x40);
        if (state->matrix == NULL) {
            return;
        }
        memcpy(&D_8009A8A4_9B4A4, &state->posX, 0xC);
        func_8006BFB8_6CBB8(&D_8009A8A4_9B4A4 - 5, state->matrix);
    }

    if (gGraphicsMode != 6) {
        gGraphicsMode = 6;

        gSPDisplayList(gRegionAllocPtr++, D_8009A780_9B380);

        gDPLoadTextureTile_4b(
            gRegionAllocPtr++,
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

        gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, state->paletteData);

        D_800A2D4C_A394C = (u32)state->textureData;
        D_800A2D50_A3950 = (u32)state->paletteData;
    } else {
        if (D_800A2D4C_A394C != (u32)state->textureData) {
            gDPLoadTextureTile_4b(
                gRegionAllocPtr++,
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
            gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, state->paletteData);

            D_800A2D50_A3950 = (u32)state->paletteData;
        }
    }

    gSPMatrix(gRegionAllocPtr++, state->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPMatrix(gRegionAllocPtr++, D_800A8B14_9FE84, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    gSPVertex(gRegionAllocPtr++, state->unk0, 4, 0);
    gSP2Triangles(gRegionAllocPtr++, 0, 3, 2, 0, 2, 1, 0, 0);
}

void func_800670A4_67CA4(u16 arg0, func_80066444_67044_arg1 *arg1) {
    arg1->unk1C = 0;
    debugEnqueueCallback(arg0 & 0xFFFF, 4, &func_80066AF0_676F0, arg1);
}

void func_800670D4_67CD4(AlphaSpriteState *state) {
    if ((u32)((D_800AB068_A23D8->unk134 - state->posX) + 0x0FEA0000) > 0x1FD40000U) {
        return;
    }
    if ((u32)((D_800AB068_A23D8->unk13C - state->posZ) + 0x0FEA0000) > 0x1FD40000U) {
        return;
    }
    if ((u32)((D_800AB068_A23D8->unk138 - state->posY) + 0x0FEA0000) > 0x1FD40000U) {
        return;
    }

    if (state->matrix == NULL) {
        state->matrix = arenaAlloc16(0x40);
        if (state->matrix == NULL) {
            return;
        }
        memcpy(&D_8009A8A4_9B4A4, &state->posX, 0xC);
        func_8006BFB8_6CBB8(&D_8009A8A4_9B4A4 - 5, state->matrix);
    }

    if (gGraphicsMode != 7) {
        gGraphicsMode = 7;

        gSPDisplayList(gRegionAllocPtr++, D_8009A7D0_9B3D0);

        gDPLoadTextureBlock_4b(
            gRegionAllocPtr++,
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

        gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, state->paletteData);

        gDPSetEnvColor(gRegionAllocPtr++, 0xFF, 0xFF, 0xFF, state->alpha);

        D_800A2D4C_A394C = (u32)state->textureData;
        D_800A2D50_A3950 = (u32)state->paletteData;
        D_800A2D54_A3954 = state->alpha;
    } else {
        if (D_800A2D4C_A394C != (u32)state->textureData) {
            gDPLoadTextureBlock_4b(
                gRegionAllocPtr++,
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
            gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, state->paletteData);

            D_800A2D50_A3950 = (u32)state->paletteData;
        }

        if (state->alpha != D_800A2D54_A3954) {
            gDPPipeSync(gRegionAllocPtr++);
            gDPSetEnvColor(gRegionAllocPtr++, 0xFF, 0xFF, 0xFF, state->alpha);
            D_800A2D54_A3954 = state->alpha;
        }
    }

    gSPMatrix(gRegionAllocPtr++, state->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPMatrix(gRegionAllocPtr++, D_800A8B14_9FE84, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    gSPVertex(gRegionAllocPtr++, state->unk0, 4, 0);
    gSP2Triangles(gRegionAllocPtr++, 0, 3, 2, 0, 2, 1, 0, 0);
}

void func_800677C0_683C0(s32 arg0, loadAssetMetadata_arg *arg1) {
    (arg1 + 1)->unk0 = 0;
    debugEnqueueCallback(arg0 & 0xFFFF, 6, &func_800670D4_67CD4, arg1);
}

void func_800677F0_683F0(AlphaSpriteState *state) {
    if ((u32)((D_800AB068_A23D8->unk134 - state->posX) + 0x0FEA0000) > 0x1FD40000U) {
        return;
    }
    if ((u32)((D_800AB068_A23D8->unk13C - state->posZ) + 0x0FEA0000) > 0x1FD40000U) {
        return;
    }
    if ((u32)((D_800AB068_A23D8->unk138 - state->posY) + 0x0FEA0000) > 0x1FD40000U) {
        return;
    }

    if (state->matrix == NULL) {
        state->matrix = arenaAlloc16(0x40);
        if (state->matrix == NULL) {
            return;
        }
        memcpy(&D_8009A8A4_9B4A4, &state->posX, 0xC);
        func_8006C130_6CD30((Transform3D *)((s32 *)&D_8009A8A4_9B4A4 - 5), state->matrix);
    }

    if (gGraphicsMode != 7) {
        gGraphicsMode = 7;

        gSPDisplayList(gRegionAllocPtr++, D_8009A7D0_9B3D0);

        gDPLoadTextureBlock_4b(
            gRegionAllocPtr++,
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

        gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, state->paletteData);

        gDPSetEnvColor(gRegionAllocPtr++, 0xFF, 0xFF, 0xFF, state->alpha);

        D_800A2D4C_A394C = (u32)state->textureData;
        D_800A2D50_A3950 = (u32)state->paletteData;
        D_800A2D54_A3954 = state->alpha;
    } else {
        if (D_800A2D4C_A394C != (u32)state->textureData) {
            gDPLoadTextureBlock_4b(
                gRegionAllocPtr++,
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
            gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, state->paletteData);

            D_800A2D50_A3950 = (u32)state->paletteData;
        }

        if (state->alpha != D_800A2D54_A3954) {
            gDPPipeSync(gRegionAllocPtr++);
            gDPSetEnvColor(gRegionAllocPtr++, 0xFF, 0xFF, 0xFF, state->alpha);
            D_800A2D54_A3954 = state->alpha;
        }
    }

    gSPMatrix(gRegionAllocPtr++, state->matrix, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPMatrix(gRegionAllocPtr++, D_800A8B14_9FE84, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    gSPVertex(gRegionAllocPtr++, state->unk0, 4, 0);
    gSP2Triangles(gRegionAllocPtr++, 0, 3, 2, 0, 2, 1, 0, 0);
}

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
    *temp_s0 = loadCompressedData(&_215D70_ROM_START, &_215D70_ROM_END, 0x918);
    setGameStateHandler(&func_80068060_68C60);
}

typedef struct {
    DataTable_19E80 *unk0;
} alloc_68C60;

void func_80068060_68C60(void) {
    alloc_68C60 *alloc = (alloc_68C60 *)getCurrentAllocation();
    func_800680F0_68CF0(alloc->unk0);
    terminateSchedulerWithCallback(&func_800680C4_68CC4);
}

void func_80068090_68C90(void) {
    if (func_80069810_6A410() << 0x10 != 0) {
        terminateSchedulerWithCallback(&func_800680C4_68CC4);
    }
}

void func_800680C4_68CC4(void) {
    func_800693C4_69FC4(&func_80014480_15080, 0xC8);
}
