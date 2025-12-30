#include "A9A40.h"
#include "56910.h"
#include "common.h"
#include "geometry.h"
#include "rand.h"
#include "task_scheduler.h"

typedef struct {
    /* 0x00 */ s16 next;
    /* 0x02 */ s16 pad;
    /* 0x04 */ s16 alt2;
    /* 0x06 */ s16 alt;
    /* 0x08 */ char unk_08[0x1C];
} Waypoint; // size = 0x24

typedef struct {
    /* 0x00 */ char unk_00[0x0C];
    /* 0x0C */ Waypoint *waypoints;
    /* 0x10 */ char unk_10[0x1C];
    /* 0x2C */ u8 defaultPosIndex;
} CourseData;

void func_800BA4B8_AA368(Player *, CourseData *, s16, Vec3i *);
void func_800B9EF0_A9DA0(Player *, CourseData *, s16, Vec3i *);

void func_800B9B90_A9A40(Player *player) {
    Vec3i dir;
    Vec3i waypointPos;
    Vec3i nextWaypointPos;
    Vec3i tempVec;
    Vec3i targetPos;
    CourseData *courseData;
    Waypoint *waypoint;
    D_80090F90_91B90_item *defaultPos;
    s32 *pathFlags;
    s32 unkB94;
    s16 angle;
    s32 distance;
    s32 maxDist;

    courseData = (CourseData *)((u8 *)getCurrentAllocation() + 0x30);
    unkB94 = player->unkB94;

    if (courseData->waypoints[unkB94].next < 0) {
        defaultPos = func_80055D10_56910(courseData->defaultPosIndex);
        player->unkA7C = defaultPos->unk0;
        player->unkA84 = defaultPos->unk4;
        return;
    }

    func_800BA4B8_AA368(player, courseData, (s16)unkB94, &waypointPos);
    func_800B9EF0_A9DA0(player, courseData, (s16)unkB94, &nextWaypointPos);

    targetPos.x = player->worldPos.x - waypointPos.x;
    targetPos.z = player->worldPos.z - waypointPos.z;

    angle = func_8006D21C_6DE1C(nextWaypointPos.x, nextWaypointPos.z, waypointPos.x, waypointPos.z);
    rotateVectorY(&targetPos, -angle, &tempVec);
    tempVec.x = 0;
    rotateVectorY(&tempVec, angle, &targetPos);

    targetPos.x += waypointPos.x;
    targetPos.z += waypointPos.z;

    while (TRUE) {
        func_800B9EF0_A9DA0(player, courseData, (s16)unkB94, &nextWaypointPos);

        dir.x = nextWaypointPos.x - targetPos.x;
        dir.z = nextWaypointPos.z - targetPos.z;

        distance = distance_2d(dir.x, dir.z);

        if (distance > 0xA00000) {
            maxDist = 0xA00000;
            dir.x = (((s64)dir.x * maxDist) / distance);
            dir.z = (((s64)dir.z * maxDist) / distance);
            break;
        }

        if (courseData->waypoints[unkB94].next < 0) {
            break;
        }

        pathFlags = (s32 *)player->unk28;
        if (pathFlags != NULL) {
            if (*(s8 *)&pathFlags[unkB94] == -1) {
                unkB94 = courseData->waypoints[unkB94].alt;
            }
            if (*(s8 *)&pathFlags[unkB94] == 0) {
                unkB94 = courseData->waypoints[unkB94].next;
            }
            if (*(s8 *)&pathFlags[unkB94] == 1) {
                unkB94 = courseData->waypoints[unkB94].alt2;
            }
        } else {
            unkB94 = courseData->waypoints[unkB94].next;
        }
    }

    dir.x += targetPos.x;
    dir.z += targetPos.z;

    func_800BA4B8_AA368(player, courseData, (s16)unkB94, &waypointPos);

    dir.x -= waypointPos.x;
    dir.z -= waypointPos.z;

    angle = func_8006D21C_6DE1C(nextWaypointPos.x, nextWaypointPos.z, waypointPos.x, waypointPos.z);
    rotateVectorY(&dir, -angle, &tempVec);
    tempVec.x = 0;
    rotateVectorY(&tempVec, angle, &dir);

    dir.x += waypointPos.x;
    dir.z += waypointPos.z;

    player->unkA7C = dir.x;
    player->unkA84 = dir.z;
}

INCLUDE_ASM("asm/nonmatchings/A9A40", func_800B9EF0_A9DA0);

INCLUDE_ASM("asm/nonmatchings/A9A40", func_800BA4B8_AA368);

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    s8 unk3;
} PlayerUnk28Entry;

typedef struct {
    u8 pad0[0x14];
    u16 unk14;
    u8 pad16[0x2];
    u16 unk18;
    u8 pad1A[0xA];
} Section3Entry;

typedef struct {
    s32 valX;
    s32 _pad1;
    s32 valZ;
    s32 _pad2;
} StackVars_A9A40;

extern u8 D_800BADD0_AAC80[];

#define SEC3(gs) ((Section3Entry *)((gs)->gameData.section3Data))
#define SEC1(gs) ((Vec3s *)((gs)->gameData.section1Data))

s8 func_800BA694_AA544(Player *player) {
    GameState *gs;
    volatile StackVars_A9A40 sv;
    s32 segmentDirX;
    s32 segmentDirZ;
    s32 distSq;
    s32 dist;
    s32 normalizedDirX;
    s32 normalizedDirZ;
    s32 playerOffsetX;
    s32 playerOffsetZ;
    s64 crossProduct;
    u16 segmentStartIdx;
    u16 segmentEndIdx;

    gs = getCurrentAllocation();

    if (player->unk28 != NULL && ((PlayerUnk28Entry *)player->unk28)[player->unkB94].unk3 != 0) {
        segmentStartIdx = SEC3(gs)[player->unkB94].unk14;
        segmentEndIdx = SEC3(gs)[player->unkB94].unk18;
        segmentDirX = SEC1(gs)[segmentStartIdx].x - SEC1(gs)[segmentEndIdx].x;
        sv.valX = segmentDirX;

        segmentStartIdx = SEC3(gs)[player->unkB94].unk14;
        distSq = segmentDirX * segmentDirX;
        segmentEndIdx = SEC3(gs)[player->unkB94].unk18;
        segmentDirZ = SEC1(gs)[segmentStartIdx].z - SEC1(gs)[segmentEndIdx].z;
        sv.valZ = segmentDirZ;
        distSq += segmentDirZ * segmentDirZ;

        dist = isqrt64(distSq);
        normalizedDirX = (sv.valX << 13) / dist;
        normalizedDirZ = (sv.valZ << 13) / dist;

        segmentStartIdx = SEC3(gs)[player->unkB94].unk14;
        playerOffsetX = player->worldPos.x - (SEC1(gs)[segmentStartIdx].x << 16);
        sv.valX = playerOffsetX;

        segmentStartIdx = SEC3(gs)[player->unkB94].unk14;
        playerOffsetZ = player->worldPos.z - (SEC1(gs)[segmentStartIdx].z << 16);
        sv.valZ = playerOffsetZ;

        crossProduct =
            (((s64)(-((s16)normalizedDirZ))) * playerOffsetX) + (((s64)((s16)normalizedDirX)) * playerOffsetZ);
        dist = -((s32)(crossProduct / 0x2000));
        if (dist < (player->unkAA8 * 6)) {
            return ((PlayerUnk28Entry *)player->unk28)[player->unkB94].unk3;
        }
    }

    if (!(player->unkBDE & 8)) {
        player->unkBE5 = 0;
    }

    if (gs->unk86 != 0) {
        if (player->unkBE5 == 0 && (player->unkBDE & 8)) {
            if ((randA() & 0xFF) >= 0xC0) {
                return 0;
            }
            player->unkBE5 = 1;
            return 8;
        }
        return 0;
    }

    if (gs->unk7A != 9) {
        if (player->unkBE5 == 0 && (player->unkBDE & 8)) {
            if ((randA() & 0xFF) < D_800BADD0_AAC80[gs->memoryPoolId]) {
                player->unkBE5 = 1;
                return 8;
            }
            if (player->unkBB9 >= 6) {
                player->unkBE5 = 1;
                return 8;
            }
        }
    }

    return 0;
}
