typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef int s32;
typedef unsigned int u32;
typedef long long s64;

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} Vec3i;

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
    u16 baseIndex2;
    u16 count2;
    u16 quadV0;
    u8 padding2[0x2];
    u16 quadV1;
    u16 quadV2;
    u8 padding3[0x2];
    u16 quadV3;
    u8 padding4[0x4];
} TrackFaceGroup;

typedef struct {
    void *unk0;
    Vertex6 *vertices;
    TrackFace *faces;
    TrackFaceGroup *faceGroups;
} TrackGeometryFaceData;

extern s64 cross2d(s32, s32, s32, s32, s32, s32);

u16 func_800626C4_632C4(TrackGeometryFaceData *arg0, Vec3i *pos) {
    s16 sp26[1];
    s32 numFaces;
    TrackFaceGroup *faceGroup;
    Vertex6 *verts;
    s32 v0x, v0z, v1x, v1z, v2x, v2z, v3x, v3z;
    s32 temp;

    temp = (s32)arg0->vertices + *(u16 *)arg0->unk0 * 6;
    temp = (s32)arg0->faces + *(u16 *)temp * 8;
    numFaces = (s32)((TrackFace *)temp)->v0;

    sp26[0] = 0;
    do {
        faceGroup = (TrackFaceGroup *)((s32)sp26[0] * 36 + (s32)arg0->faceGroups);
        verts = arg0->vertices;

        v0x = (s32)((Vertex6 *)(faceGroup->quadV0 * 6 + (s32)verts))->x << 16;
        v0z = (s32)((Vertex6 *)(faceGroup->quadV0 * 6 + (s32)verts))->z << 16;

        v1x = (s32)((Vertex6 *)(faceGroup->quadV1 * 6 + (s32)verts))->x << 16;
        v1z = (s32)((Vertex6 *)(faceGroup->quadV1 * 6 + (s32)verts))->z << 16;

        v2x = (s32)((Vertex6 *)(faceGroup->quadV2 * 6 + (s32)verts))->x << 16;
        v2z = (s32)((Vertex6 *)(faceGroup->quadV2 * 6 + (s32)verts))->z << 16;

        v3x = (s32)((Vertex6 *)(faceGroup->quadV3 * 6 + (s32)verts))->x << 16;
        v3z = (s32)((Vertex6 *)(faceGroup->quadV3 * 6 + (s32)verts))->z << 16;

        if (cross2d(pos->x, pos->z, v1x, v1z, v0x, v0z) > 0) goto next_face;
        if (cross2d(pos->x, pos->z, v2x, v2z, v3x, v3z) > 0) goto next_face;
        if (cross2d(pos->x, pos->z, v3x, v3z, v1x, v1z) > 0) goto next_face;
        if (cross2d(pos->x, pos->z, v0x, v0z, v2x, v2z) <= 0) goto found;
next_face:
        sp26[0]++;
    } while ((s16)sp26[0] != numFaces);

    return 0;
found:
    return (u16)sp26[0];
}
