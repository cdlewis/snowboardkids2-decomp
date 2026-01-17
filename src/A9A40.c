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

void calculateAITargetPosition(Player *player) {
    Vec3i dir;
    Vec3i waypointPos;
    Vec3i nextWaypointPos;
    Vec3i tempVec;
    Vec3i targetPos;
    CourseData *courseData;
    Waypoint *waypoint;
    LevelConfig *defaultPos;
    s32 *pathFlags;
    s32 sectorIndex;
    s16 angle;
    s32 distance;
    s32 maxDist;

    courseData = (CourseData *)((u8 *)getCurrentAllocation() + 0x30);
    sectorIndex = player->sectorIndex;

    if (courseData->waypoints[sectorIndex].next < 0) {
        defaultPos = getLevelConfig(courseData->defaultPosIndex);
        player->aiTargetX = defaultPos->shortcutPosX;
        player->aiTargetZ = defaultPos->shortcutPosZ;
        return;
    }

    func_800BA4B8_AA368(player, courseData, (s16)sectorIndex, &waypointPos);
    func_800B9EF0_A9DA0(player, courseData, (s16)sectorIndex, &nextWaypointPos);

    targetPos.x = player->worldPos.x - waypointPos.x;
    targetPos.z = player->worldPos.z - waypointPos.z;

    angle = computeAngleToPosition(nextWaypointPos.x, nextWaypointPos.z, waypointPos.x, waypointPos.z);
    rotateVectorY(&targetPos, -angle, &tempVec);
    tempVec.x = 0;
    rotateVectorY(&tempVec, angle, &targetPos);

    targetPos.x += waypointPos.x;
    targetPos.z += waypointPos.z;

    while (TRUE) {
        func_800B9EF0_A9DA0(player, courseData, (s16)sectorIndex, &nextWaypointPos);

        dir.x = nextWaypointPos.x - targetPos.x;
        dir.z = nextWaypointPos.z - targetPos.z;

        distance = distance_2d(dir.x, dir.z);

        if (distance > 0xA00000) {
            maxDist = 0xA00000;
            dir.x = (((s64)dir.x * maxDist) / distance);
            dir.z = (((s64)dir.z * maxDist) / distance);
            break;
        }

        if (courseData->waypoints[sectorIndex].next < 0) {
            break;
        }

        pathFlags = (s32 *)player->aiPathData;
        if (pathFlags != NULL) {
            if (*(s8 *)&pathFlags[sectorIndex] == -1) {
                sectorIndex = courseData->waypoints[sectorIndex].alt;
            }
            if (*(s8 *)&pathFlags[sectorIndex] == 0) {
                sectorIndex = courseData->waypoints[sectorIndex].next;
            }
            if (*(s8 *)&pathFlags[sectorIndex] == 1) {
                sectorIndex = courseData->waypoints[sectorIndex].alt2;
            }
        } else {
            sectorIndex = courseData->waypoints[sectorIndex].next;
        }
    }

    dir.x += targetPos.x;
    dir.z += targetPos.z;

    func_800BA4B8_AA368(player, courseData, (s16)sectorIndex, &waypointPos);

    dir.x -= waypointPos.x;
    dir.z -= waypointPos.z;

    angle = computeAngleToPosition(nextWaypointPos.x, nextWaypointPos.z, waypointPos.x, waypointPos.z);
    rotateVectorY(&dir, -angle, &tempVec);
    tempVec.x = 0;
    rotateVectorY(&tempVec, angle, &dir);

    dir.x += waypointPos.x;
    dir.z += waypointPos.z;

    player->aiTargetX = dir.x;
    player->aiTargetZ = dir.z;
}

INCLUDE_ASM("asm/nonmatchings/A9A40", func_800B9EF0_A9DA0);

INCLUDE_ASM("asm/nonmatchings/A9A40", func_800BA4B8_AA368);

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    s8 pathPreference;
} AIPathPreference;

typedef struct {
    u8 pad0[0x14];
    u16 trackStartIdx;
    u8 pad16[0x2];
    u16 trackEndIdx;
    u8 pad1A[0xA];
} Section3Entry;

typedef struct {
    s32 spillX;
    s32 _pad1;
    s32 spillZ;
    s32 _pad2;
} TrackCalcStackVars;

extern u8 gShortcutChanceByMemoryPool[];

#define SEC3(gs) ((Section3Entry *)((gs)->gameData.section3Data))
#define SEC1(gs) ((Vec3s *)((gs)->gameData.section1Data))

s8 determineAIPathChoice(Player *player) {
    GameState *gs;
    volatile TrackCalcStackVars sv;
    s32 trackDirX;
    s32 trackDirZ;
    s32 trackLengthSq;
    s32 trackLength;
    s32 normalizedDirX;
    s32 normalizedDirZ;
    s32 playerToStartX;
    s32 playerToStartZ;
    s64 lateralOffset;
    u16 trackStartIdx;
    u16 trackEndIdx;

    gs = getCurrentAllocation();

    if (player->aiPathData != NULL &&
        ((AIPathPreference *)player->aiPathData)[player->sectorIndex].pathPreference != 0) {
        trackStartIdx = SEC3(gs)[player->sectorIndex].trackStartIdx;
        trackEndIdx = SEC3(gs)[player->sectorIndex].trackEndIdx;
        trackDirX = SEC1(gs)[trackStartIdx].x - SEC1(gs)[trackEndIdx].x;
        sv.spillX = trackDirX;

        trackStartIdx = SEC3(gs)[player->sectorIndex].trackStartIdx;
        trackLengthSq = trackDirX * trackDirX;
        trackEndIdx = SEC3(gs)[player->sectorIndex].trackEndIdx;
        trackDirZ = SEC1(gs)[trackStartIdx].z - SEC1(gs)[trackEndIdx].z;
        sv.spillZ = trackDirZ;
        trackLengthSq += trackDirZ * trackDirZ;

        trackLength = isqrt64(trackLengthSq);
        normalizedDirX = (sv.spillX << 13) / trackLength;
        normalizedDirZ = (sv.spillZ << 13) / trackLength;

        trackStartIdx = SEC3(gs)[player->sectorIndex].trackStartIdx;
        playerToStartX = player->worldPos.x - (SEC1(gs)[trackStartIdx].x << 16);
        sv.spillX = playerToStartX;

        trackStartIdx = SEC3(gs)[player->sectorIndex].trackStartIdx;
        playerToStartZ = player->worldPos.z - (SEC1(gs)[trackStartIdx].z << 16);
        sv.spillZ = playerToStartZ;

        // Calculate lateral offset (perpendicular distance) from track center line
        lateralOffset =
            ((s64)(-((s16)normalizedDirZ)) * playerToStartX) + ((s64)((s16)normalizedDirX) * playerToStartZ);
        trackLength = -((s32)(lateralOffset / 0x2000));

        // If player is close enough to the center line, follow the stored path preference
        if (trackLength < (player->aiLaneWidth * 6)) {
            return ((AIPathPreference *)player->aiPathData)[player->sectorIndex].pathPreference;
        }
    }

    // Reset shortcut choice if no shortcut available
    if (!(player->pathFlags & 8)) {
        player->aiShortcutChosen = 0;
    }

    // Special mode: unk86 is set (possibly time attack or special mode)
    if (gs->unk86 != 0) {
        if (player->aiShortcutChosen == 0 && (player->pathFlags & 8)) {
            // 25% chance to skip shortcut (0xC0 = 192/256 = 75% take rate)
            if ((randA() & 0xFF) >= 0xC0) {
                return 0;
            }
            player->aiShortcutChosen = 1;
            return 8; // Take shortcut
        }
        return 0;
    }

    // Normal race mode (not race type 9)
    if (gs->raceType != 9) {
        if (player->aiShortcutChosen == 0 && (player->pathFlags & 8)) {
            // Check random shortcut chance based on memory pool
            if ((randA() & 0xFF) < gShortcutChanceByMemoryPool[gs->memoryPoolId]) {
                player->aiShortcutChosen = 1;
                return 8; // Take shortcut
            }
            // Boss characters (ID >= 6) always take shortcuts
            if (player->characterId >= 6) {
                player->aiShortcutChosen = 1;
                return 8; // Take shortcut
            }
        }
    }

    return 0; // Stay on main path
}
