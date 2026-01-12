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
    /* 0x24 */ void *unk24;
    /* 0x28 */ void *unk28;
    /* 0x2C */ void *unk2C;
    /* 0x30 */ Mtx *unk30;
    /* 0x34 */ u8 unk34;
    /* 0x35 */ u8 unk35;
    /* 0x36 */ u8 unk36;
    /* 0x37 */ u8 unk37;
    /* 0x38 */ u8 unk38;
    /* 0x39 */ u8 unk39;
    /* 0x3A */ u8 unk3A;
    /* 0x3B */ u8 envColorAlpha;
} DisplayListObject;

void enqueueDisplayListObject(s32 arg0, DisplayListObject *arg1);

typedef struct {
    u8 padding[0x20];
    s32 *unk20;
    u8 padding3[0x9];
    s32 unk30;
    u8 padding2[0x3];
    s8 unk37;
} enqueueMultiPartDisplayList_arg1;

void func_80064808_65408(s32 arg0, enqueueMultiPartDisplayList_arg1 *arg1, s32 arg2);
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
    struct loadAssetMetadata_arg *unk0;
    /* 0x4 */ Vec3i position;
    /* 0x10 */ u8 *data_ptr;
    /* 0x14 */ TableEntry_19E80 *index_ptr;
} loadAssetMetadata_arg_base;

/* Full struct with alpha/animation fields - 0x1C bytes */
typedef struct loadAssetMetadata_arg {
    struct loadAssetMetadata_arg *unk0;
    /* 0x4 */ Vec3i position;
    /* 0x10 */ u8 *data_ptr;
    /* 0x14 */ TableEntry_19E80 *index_ptr;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
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
    s32 unk0;
    Vec3i unk4;
    u8 padding[0xC];
    s32 unk1C;
    u8 padding2[0x4];
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    s16 unk30;
    s16 unk36;
} func_80066444_67044_arg1;

void func_80066444_67044(s32 arg0, func_80066444_67044_arg1 *arg1);
void func_800677C0_683C0(s32 arg0, loadAssetMetadata_arg *arg1);

void func_800670A4_67CA4(u16 arg0, func_80066444_67044_arg1 *arg1);

typedef struct {
    u8 padding0[4];
    s32 unk4;
    s32 unk8;
    s32 unkC;
    /* 0x10 */ u8 *data_ptr;
    /* 0x14 */ TableEntry_19E80 *index_ptr;
    s8 unk18;
    u8 unk19;
    u8 unk1A;
} loadAssetMetadataByIndex_arg;

void loadAssetMetadataByIndex(
    loadAssetMetadataByIndex_arg *arg0,
    DataTable_19E80 *table,
    s32 entry_index,
    s32 sub_index
);

void func_80067EDC_68ADC(s32, loadAssetMetadata_arg *);

void buildOverlayDisplayListSegment(DisplayListObject *obj);

void func_800639F8_645F8(s32 arg0, DisplayListObject *arg1);

void func_8006417C_64D7C(s32 arg0, DisplayListObject *arg1);

void func_80065DA8_669A8(s32 arg0, DisplayListObject *arg1);

u16 getTrackEndInfo(void *arg0, void *arg1);

u16 func_80060A3C_6163C(void *arg0, u16 arg1, void *arg2);

void func_80060CDC_618DC(void *arg0, u16 arg1, void *arg2, s32 arg3, Vec3i *arg4);

s32 func_80061A64_62664(void *arg0, u16 arg1, void *arg2);

s32 func_80061D6C_6296C(void *arg0, u16 arg1, void *arg2, s32 arg3);

s32 func_80062274_62E74(void *arg0, u16 arg1);

typedef struct TrackGeometryFaceData TrackGeometryFaceData;
void findTrackFaceAtPosition(TrackGeometryFaceData *arg0, u16 arg1, Vec3i *arg2, u8 *arg3, u8 *arg4);

void func_800650B4_65CB4(u16 arg0, DisplayListObject *arg1);

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
    u8 _pad16[0x1A];
    s32 unk30;
} MatrixEntry_202A0;
void func_80066AC0_676C0(s32 arg0, MatrixEntry_202A0 *arg1);