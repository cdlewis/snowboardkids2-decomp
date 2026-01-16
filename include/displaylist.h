#pragma once

#include "19E80.h"
#include "36BE0.h"
#include "common.h"
#include "geometry.h"

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
    /* 0x37 */ u8 numParts;
    /* 0x38 */ u8 light2R;
    /* 0x39 */ u8 light2G;
    /* 0x3A */ u8 light2B;
    /* 0x3B */ u8 envColorAlpha;
} DisplayListObject;

void enqueueDisplayListObject(s32 arg0, DisplayListObject *arg1);
void enqueueDisplayListObjectWithSegments(s32 arg0, DisplayListObject *arg1);

typedef struct {
    u8 padding[0x20];
    s32 *unk20;
    u8 padding3[0x9];
    s32 unk30;
    u8 padding2[0x3];
    s8 numParts;
} enqueueMultiPartDisplayList_arg1;

void renderMultiPartOverlayDisplayLists(DisplayListObject *displayObjects);
void renderTexturedOpaqueSprite(DisplayListObject *arg0);
void renderTexturedTransparentSprite(DisplayListObject *arg0);
void renderTexturedOverlaySprite(DisplayListObject *arg0);
void enqueuePreLitMultiPartDisplayList(s32 arg0, enqueueMultiPartDisplayList_arg1 *arg1, s32 arg2);
void enqueueMultiPartDisplayList(s32 arg0, enqueueMultiPartDisplayList_arg1 *arg1, s32 arg2);

typedef struct {
    u16 *dataStart;
    u16 *section1Data;
    u16 *section2Data;
    void *section3Data;
    u16 finalValue;
} GameDataLayout;

void parseGameDataLayout(GameDataLayout *gameData);

void initializeOverlaySystem(void);

/* Base struct without trailing fields - 0x18 bytes */
typedef struct loadAssetMetadata_arg_base {
    struct loadAssetMetadata_arg *assetTemplate;
    /* 0x4 */ Vec3i position;
    /* 0x10 */ u8 *data_ptr;
    /* 0x14 */ TableEntry_19E80 *index_ptr;
} loadAssetMetadata_arg_base;

/* Full struct with alpha/animation fields - 0x1C bytes */
typedef struct loadAssetMetadata_arg {
    struct loadAssetMetadata_arg *assetTemplate;
    /* 0x4 */ Vec3i position;
    /* 0x10 */ u8 *data_ptr;
    /* 0x14 */ TableEntry_19E80 *index_ptr;
    u8 unk18;
    u8 unk19;
    u8 alpha;
} loadAssetMetadata_arg;

void loadAssetMetadata(loadAssetMetadata_arg *, void *, s32);

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

typedef struct {
    /* 0x00 */ s32 vertices;
    /* 0x04 */ Vec3i position;
    /* 0x10 */ u8 *textureData;
    /* 0x14 */ u8 *paletteData;
    /* 0x18 */ u8 width;
    /* 0x19 */ u8 height;
    /* 0x1A */ u8 pad1A[2];
    /* 0x1C */ Mtx *matrix;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s16 unk30;
    /* 0x32 */ s16 unk32;
} TexturedBillboardSprite;

void enqueueTexturedBillboardSprite(s32 arg0, TexturedBillboardSprite *arg1);
void enqueueAlphaBillboardSprite(s32 arg0, loadAssetMetadata_arg *arg1);

void enqueueTexturedBillboardSpriteTile(u16 arg0, TexturedBillboardSprite *arg1);

typedef struct {
    u8 padding0[4];
    s32 unk4;
    s32 unk8;
    s32 unkC;
    /* 0x10 */ u8 *data_ptr;
    /* 0x14 */ TableEntry_19E80 *index_ptr;
    s8 unk18;
    u8 unk19;
    u8 alpha;
} loadAssetMetadataByIndex_arg;

void loadAssetMetadataByIndex(
    loadAssetMetadataByIndex_arg *arg0,
    DataTable_19E80 *table,
    s32 entry_index,
    s32 sub_index
);

void enqueueAlphaSprite(s32, loadAssetMetadata_arg *);

void buildOverlayDisplayListSegment(DisplayListObject *obj);

void enqueueBillboardedDisplayListObject(s32 arg0, DisplayListObject *arg1);

void enqueueCameraRelativeDisplayList(s32 arg0, DisplayListObject *arg1);

u16 getTrackEndInfo(void *arg0, void *arg1);

u16 func_80060A3C_6163C(void *arg0, u16 arg1, void *arg2);

void func_80060CDC_618DC(void *arg0, u16 arg1, void *arg2, s32 arg3, Vec3i *arg4);

s32 func_80061A64_62664(void *arg0, u16 arg1, void *arg2);

s32 func_80061D6C_6296C(void *arg0, u16 arg1, void *arg2, s32 arg3);

s32 func_80062274_62E74(void *arg0, u16 arg1);

typedef struct TrackGeometryFaceData TrackGeometryFaceData;
void findTrackFaceAtPosition(TrackGeometryFaceData *arg0, u16 arg1, Vec3i *arg2, u8 *arg3, u8 *arg4);

void prepareDisplayListRenderStateWithLights(DisplayListObject *obj);
void renderOpaqueDisplayListWithLights(DisplayListObject *obj);
void renderTransparentDisplayListWithLights(DisplayListObject *obj);
void renderOverlayDisplayListWithLights(DisplayListObject *obj);
void enqueueDisplayListObjectWithLights(u16 renderLayer, DisplayListObject *displayListObj);
void renderMultiPartOpaqueDisplayListsWithLights(DisplayListObject *displayObjects);
void renderMultiPartTransparentDisplayListsWithLights(DisplayListObject *displayObjects);
void renderMultiPartOverlayDisplayListsWithLights(DisplayListObject *displayObjects);

u16 getTrackSegmentWaypoints(void *trackGeom, u16 waypointIdx, void *waypointStart, void *waypointEnd);

typedef struct {
    s16 linkedSegmentIdx;
    u8 _pad[0x8];
    s16 finishZoneFlag;
    u8 _pad2[0x18];
} TrackSegmentEntry;

s16 getTrackSegmentFinishZoneFlag(GameDataLayout *gameData, u16 index);

s32 resolveTrackSegmentIndex(TrackSegmentEntry **arg0, u16 index);

typedef struct {
    u8 _pad0[0x4];
    s16 matrix[3][3];
    u8 _pad16[0xE];
    u8 *data_ptr;
    void *index_ptr;
    u8 _pad2C[0x4];
    s32 unk30;
} MatrixEntry_202A0;
void enqueueRotatedBillboardSprite(s32 arg0, MatrixEntry_202A0 *arg1);