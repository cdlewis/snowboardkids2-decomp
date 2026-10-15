#pragma once

#include "common.h"
#include "data/data_table.h"
#include "gbi.h"
#include "math/geometry.h"

/* Callback pass mask, computed from the display lists present across parts. */
#define DISPLAY_LIST_PASS_OPAQUE 1
#define DISPLAY_LIST_PASS_TRANSPARENT 2
#define DISPLAY_LIST_PASS_OVERLAY 4

/* DisplayLists.flags: constructs and emits reflection LookAt lighting vectors.
 * This is a separate flag domain from the callback pass mask above.
 */
#define DISPLAY_LIST_FLAG_REFLECTION_LOOK_AT 1

typedef struct {
    /* 0x0 */ u32 flags;
    /* 0x4 */ Gfx *opaqueDisplayList;
    /* 0x8 */ Gfx *transparentDisplayList;
    /* 0xC */ Gfx *overlayDisplayList;
} DisplayLists;

typedef struct {
    /* 0x00 */ Transform3D transform;
    /* 0x20 */ DisplayLists *displayLists;
    /* 0x24 */ void *segment1;
    /* 0x28 */ void *segment2;
    /* 0x2C */ void *segment3;
    /* 0x30 */ Mtx *transformMatrix;
    /* 0x34 */ u8 light1R;
    /* 0x35 */ u8 light1G;
    /* 0x36 */ u8 light1B;
    /* 0x37 */ u8 numParts; // For multi-part display-list roots only. Not valid as SceneModel bone count.
    /* 0x38 */ u8 light2R;
    /* 0x39 */ u8 light2G;
    /* 0x3A */ u8 light2B;
    /* 0x3B */ u8 envColorAlpha;
} DisplayListObject;

typedef struct {
    u16 v0;
    u16 v1;
    u16 v2;
    u8 flags;
    u8 surfaceIndex;
} TrackFace;

typedef struct {
    /* 0x00 */ s16 nextSectorIndex;
    /* 0x02 */ s16 previousSectorIndex;
    /* 0x04 */ s16 rightSectorIndex;
    /* 0x06 */ s16 leftSectorIndex;
    /* 0x08 */ u16 segmentLength;
    /* 0x0A */ s16 lapProgressRemaining;
    /* 0x0C */ u16 baseFaceIndex;
    /* 0x0E */ u16 faceCount;
    /* 0x10 */ u16 baseHeightFaceIndex;
    /* 0x12 */ u16 heightFaceCount;
    /* 0x14 */ u16 startLeftVertexIndex;
    /* 0x16 */ u16 startCenterVertexIndex;
    /* 0x18 */ u16 startRightVertexIndex;
    /* 0x1A */ u16 endLeftVertexIndex;
    /* 0x1C */ u16 endCenterVertexIndex;
    /* 0x1E */ u16 endRightVertexIndex;
    /* 0x20 */ u32 unused;
} TrackSector;

typedef struct {
    /* 0x00 */ u16 *serializedData;
    /* 0x04 */ Vec3s *vertices;
    /* 0x08 */ TrackFace *faces;
    /* 0x0C */ TrackSector *sectors;
    /* 0x10 */ u16 sectorCount;
} TrackData;

typedef struct {
    /* 0x00 */ Vtx *vertices;
    /* 0x04 */ Transform3D transform;
    /* 0x24 */ u8 *textureData;
    /* 0x28 */ u16 *paletteData;
    /* 0x2C */ u8 textureWidth;
    /* 0x2D */ u8 textureHeight;
    /* 0x2E */ u8 alpha;
    /* 0x30 */ Mtx *matrix;
} RotatedBillboardSprite;

typedef struct {
    /* 0x00 */ Vtx *vertices;
    /* 0x04 */ Vec3i position;
    /* 0x10 */ u8 *textureData;
    /* 0x14 */ u16 *paletteData;
    /* 0x18 */ u8 textureWidth;
    /* 0x19 */ u8 textureHeight;
    /* 0x1A */ u8 alpha;
    /* 0x1C */ Mtx *matrix;
} BillboardSprite;

void loadAssetMetadataByIndex(BillboardSprite *arg0, DataTable_19E80 *table, s32 entry_index, s32 sub_index);

void enqueueDisplayListObject(s32 arg0, DisplayListObject *arg1);

void enqueueDisplayListObjectWithSegments(s32 arg0, DisplayListObject *arg1);

void prepareDisplayListRenderState(DisplayListObject *obj);

void setupDisplayListMatrix(DisplayListObject *obj);

void setupBillboardDisplayListMatrix(DisplayListObject *obj);

void initializeMultiPartDisplayListObjects(DisplayListObject *arg0);

void setupMultiPartObjectRenderState(DisplayListObject *arg0, s32 arg1);

void renderMultiPartOpaqueDisplayLists(DisplayListObject *displayObjects);

void renderMultiPartTransparentDisplayLists(DisplayListObject *displayObjects);

void renderMultiPartOverlayDisplayLists(DisplayListObject *displayObjects);

void renderTexturedOpaqueSprite(DisplayListObject *arg0);

void renderTexturedTransparentSprite(DisplayListObject *arg0);

void renderTexturedOverlaySprite(DisplayListObject *arg0);

void enqueuePreLitMultiPartDisplayList(s32 arg0, DisplayListObject *arg1, s32 arg2);

void enqueueMultiPartDisplayList(s32 arg0, DisplayListObject *arg1, s32 arg2);

void parseTrackData(TrackData *trackData);

void initializeOverlaySystem(void);

void loadAssetMetadata(BillboardSprite *, void *, s32);

void enqueueDisplayListWithFrustumCull(s32, DisplayListObject *);

void renderOpaqueDisplayListWithFrustumCull(DisplayListObject *arg0);

void buildDisplayListSegment(DisplayListObject *);

void buildTransparentDisplayListSegment(DisplayListObject *);

void renderOverlayDisplayListCallback(DisplayListObject *obj);

void renderTransparentDisplayListCallback(DisplayListObject *obj);

void renderOpaqueDisplayListCallback(DisplayListObject *obj);

void enqueueDisplayListObjectWithFullRenderState(s32 arg0, void *arg1);

void renderOpaqueDisplayList(DisplayListObject *arg0);

void renderTransparentDisplayList(DisplayListObject *arg0);

void renderOverlayDisplayList(DisplayListObject *arg0);

void enqueueTexturedBillboardSprite(s32 arg0, BillboardSprite *arg1);

void enqueueAlphaBillboardSprite(s32 arg0, BillboardSprite *arg1);

void enqueueTexturedBillboardSpriteTile(u16 arg0, BillboardSprite *arg1);

void enqueueAlphaSprite(s32, BillboardSprite *);

void buildOverlayDisplayListSegment(DisplayListObject *obj);

void enqueueBillboardedDisplayListObject(s32 arg0, DisplayListObject *arg1);

void enqueueCameraRelativeDisplayList(s32 arg0, DisplayListObject *arg1);

u16 getTrackEndInfo(void *arg0, void *arg1);

u16 findTrackSector(void *trackGeom, u16 sectorIndex, void *pos);

s32 resolveTrackWallCollision(void *trackGeom, u16 sectorIndex, void *position, s32 collisionRadius, Vec3i *pushOffset);

s32 getTrackHeightAtPosition(void *trackGeom, u16 groupIdx, void *pos);

s32 getTrackHeightWithNormalAtPosition(void *arg0, u16 arg1, void *arg2, s32 arg3);

s32 computeSectorTrackHeight(TrackData *geom, u16 groupIdx, Vec3i *pos, s32 yOffset);

void findTrackFaceAtPosition(TrackData *arg0, u16 arg1, Vec3i *arg2, u8 *arg3, u8 *arg4);

void prepareDisplayListRenderStateWithLights(DisplayListObject *obj);

void renderOpaqueDisplayListWithLights(DisplayListObject *obj);

void renderTransparentDisplayListWithLights(DisplayListObject *obj);

void renderOverlayDisplayListWithLights(DisplayListObject *obj);

void enqueueDisplayListObjectWithLights(s32 renderLayer, DisplayListObject *displayListObj);

void renderMultiPartOpaqueDisplayListsWithLights(DisplayListObject *displayObjects);

void renderMultiPartTransparentDisplayListsWithLights(DisplayListObject *displayObjects);

void renderMultiPartOverlayDisplayListsWithLights(DisplayListObject *displayObjects);

u16 getTrackSegmentWaypoints(TrackData *trackData, u16 sectorIndex, void *waypointStart, void *waypointEnd);

s16 getTrackLapProgressRemaining(TrackData *trackData, u16 sectorIndex);

s32 resolveTrackSegmentIndex(TrackSector **arg0, u16 index);

void enqueueRotatedBillboardSprite(s32 arg0, RotatedBillboardSprite *arg1);

void renderRotatedBillboardSprite(RotatedBillboardSprite *arg1);

s32 normalizeSurfaceType(s32);

s32 projectPositionOntoTrackSegment(TrackData *trackData, u16 sectorIdx, Vec3i *pos);
