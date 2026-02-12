#include "A9A40.h"
#include "56910.h"
#include "common.h"
#include "geometry.h"
#include "rand.h"
#include "task_scheduler.h"

// Macro definitions
#define SEC3(gs) ((Section3Entry *)((gs)->gameData.section3Data))
#define SEC1(gs) ((Vec3s *)((gs)->gameData.section1Data))

// AI path choice values
#define PATH_CHOICE_MAIN 0
#define PATH_CHOICE_SHORTCUT 8

// AI path flags
#define PATH_FLAG_SHORTCUT_AVAILABLE 8

// Random thresholds for AI decision making
#define SHORTCUT_SKIP_CHANCE 0xC0 // 192/256 = 75% take rate in special mode

// Maximum look-ahead distance for AI target calculation
#define AI_MAX_LOOKAHEAD_DISTANCE 0xA00000

// AI lateral offset scaling factors
#define LATERAL_OFFSET_SCALE 0x2000
#define LANE_WIDTH_MULTIPLIER 6

// Struct definitions
typedef struct {
    /* 0x00 */ s16 next;
    /* 0x02 */ s16 pad02;
    /* 0x04 */ s16 alt2;
    /* 0x06 */ s16 alt;
    /* 0x08 */ char pad08[0x1C];
} Waypoint; // size = 0x24

typedef struct {
    /* 0x00 */ char pad00[0x0C];
    /* 0x0C */ Waypoint *waypoints;
    /* 0x10 */ char pad10[0x1C];
    /* 0x2C */ u8 defaultPosIndex;
} CourseData;

typedef struct {
    /* 0x00 */ u8 unk00;
    /* 0x01 */ u8 unk01;
    /* 0x02 */ u8 unk02;
    /* 0x03 */ s8 pathPreference;
} AIPathPreference;

typedef struct {
    u8 pad14[0x14];
    u16 trackStartIdx;
    u8 pad16[0x2];
    u16 trackEndIdx;
    u8 pad1A[0xA];
} Section3Entry;

typedef struct {
    s32 dirX;
    s32 _pad1;
    s32 dirZ;
    s32 _pad2;
} StackSpillVars;

// Global variables
extern u8 gShortcutChanceByMemoryPool[];

// Function declarations (for functions defined via INCLUDE_ASM below)
void func_800BA4B8_AA368(Player *, CourseData *, s16, Vec3i *);
void func_800B9EF0_A9DA0(Player *, CourseData *, s16, Vec3i *);

void calculateAITargetPosition(Player *player) {
    Vec3i finalWaypointPos;
    Vec3i currentWaypointPos;
    Vec3i nextWaypointPos;
    Vec3i rotatedPos;
    Vec3i projectedPlayerPos;
    CourseData *courseData;
    LevelConfig *levelConfig;
    s32 *pathChoiceData;
    s32 currentSectorIndex;
    s16 pathAngle;
    s32 distanceToWaypoint;
    s32 maxDistance;
    GameState *gs;

    gs = getCurrentAllocation();
    courseData = (CourseData *)&gs->gameData;
    currentSectorIndex = player->sectorIndex;

    if (courseData->waypoints[currentSectorIndex].next < 0) {
        levelConfig = getLevelConfig(courseData->defaultPosIndex);
        player->aiTarget.x = levelConfig->shortcutPosX;
        player->aiTarget.z = levelConfig->shortcutPosZ;
        return;
    }

    func_800BA4B8_AA368(player, courseData, (s16)currentSectorIndex, &currentWaypointPos);
    func_800B9EF0_A9DA0(player, courseData, (s16)currentSectorIndex, &nextWaypointPos);

    // Project player's position onto the path centerline
    projectedPlayerPos.x = player->worldPos.x - currentWaypointPos.x;
    projectedPlayerPos.z = player->worldPos.z - currentWaypointPos.z;

    pathAngle =
        computeAngleToPosition(nextWaypointPos.x, nextWaypointPos.z, currentWaypointPos.x, currentWaypointPos.z);
    rotateVectorY(&projectedPlayerPos, -pathAngle, &rotatedPos);
    rotatedPos.x = 0;
    rotateVectorY(&rotatedPos, pathAngle, &projectedPlayerPos);

    projectedPlayerPos.x += currentWaypointPos.x;
    projectedPlayerPos.z += currentWaypointPos.z;

    while (TRUE) {
        func_800B9EF0_A9DA0(player, courseData, (s16)currentSectorIndex, &nextWaypointPos);

        finalWaypointPos.x = nextWaypointPos.x - projectedPlayerPos.x;
        finalWaypointPos.z = nextWaypointPos.z - projectedPlayerPos.z;

        distanceToWaypoint = distance_2d(finalWaypointPos.x, finalWaypointPos.z);

        if (distanceToWaypoint > AI_MAX_LOOKAHEAD_DISTANCE) {
            maxDistance = AI_MAX_LOOKAHEAD_DISTANCE;
            finalWaypointPos.x = (((s64)finalWaypointPos.x * maxDistance) / distanceToWaypoint);
            finalWaypointPos.z = (((s64)finalWaypointPos.z * maxDistance) / distanceToWaypoint);
            break;
        }

        if (courseData->waypoints[currentSectorIndex].next < 0) {
            break;
        }

        pathChoiceData = (s32 *)player->aiPathData;
        if (pathChoiceData != NULL) {
            if (*(s8 *)&pathChoiceData[currentSectorIndex] == -1) {
                currentSectorIndex = courseData->waypoints[currentSectorIndex].alt;
            }
            if (*(s8 *)&pathChoiceData[currentSectorIndex] == 0) {
                currentSectorIndex = courseData->waypoints[currentSectorIndex].next;
            }
            if (*(s8 *)&pathChoiceData[currentSectorIndex] == 1) {
                currentSectorIndex = courseData->waypoints[currentSectorIndex].alt2;
            }
        } else {
            currentSectorIndex = courseData->waypoints[currentSectorIndex].next;
        }
    }

    finalWaypointPos.x += projectedPlayerPos.x;
    finalWaypointPos.z += projectedPlayerPos.z;

    func_800BA4B8_AA368(player, courseData, (s16)currentSectorIndex, &currentWaypointPos);

    finalWaypointPos.x -= currentWaypointPos.x;
    finalWaypointPos.z -= currentWaypointPos.z;

    pathAngle =
        computeAngleToPosition(nextWaypointPos.x, nextWaypointPos.z, currentWaypointPos.x, currentWaypointPos.z);
    rotateVectorY(&finalWaypointPos, -pathAngle, &rotatedPos);
    rotatedPos.x = 0;
    rotateVectorY(&rotatedPos, pathAngle, &finalWaypointPos);

    finalWaypointPos.x += currentWaypointPos.x;
    finalWaypointPos.z += currentWaypointPos.z;

    player->aiTarget.x = finalWaypointPos.x;
    player->aiTarget.z = finalWaypointPos.z;
}

INCLUDE_ASM("asm/nonmatchings/A9A40", func_800B9EF0_A9DA0);

INCLUDE_ASM("asm/nonmatchings/A9A40", func_800BA4B8_AA368);

s8 determineAIPathChoice(Player *player) {
    GameState *gs;
    volatile StackSpillVars spill;
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
        spill.dirX = trackDirX;

        trackStartIdx = SEC3(gs)[player->sectorIndex].trackStartIdx;
        trackLengthSq = trackDirX * trackDirX;
        trackEndIdx = SEC3(gs)[player->sectorIndex].trackEndIdx;
        trackDirZ = SEC1(gs)[trackStartIdx].z - SEC1(gs)[trackEndIdx].z;
        spill.dirZ = trackDirZ;
        trackLengthSq += trackDirZ * trackDirZ;

        trackLength = isqrt64(trackLengthSq);
        normalizedDirX = (spill.dirX << 13) / trackLength;
        normalizedDirZ = (spill.dirZ << 13) / trackLength;

        trackStartIdx = SEC3(gs)[player->sectorIndex].trackStartIdx;
        playerToStartX = player->worldPos.x - (SEC1(gs)[trackStartIdx].x << 16);
        spill.dirX = playerToStartX;

        trackStartIdx = SEC3(gs)[player->sectorIndex].trackStartIdx;
        playerToStartZ = player->worldPos.z - (SEC1(gs)[trackStartIdx].z << 16);
        spill.dirZ = playerToStartZ;

        // Calculate lateral offset (perpendicular distance) from track center line
        // trackLength is reused here to store the lateral distance from center
        lateralOffset =
            ((s64)(-((s16)normalizedDirZ)) * playerToStartX) + ((s64)((s16)normalizedDirX) * playerToStartZ);
        trackLength = -((s32)(lateralOffset / LATERAL_OFFSET_SCALE));

        // If player is close enough to the center line, follow the stored path preference
        if (trackLength < (player->aiLaneWidth * LANE_WIDTH_MULTIPLIER)) {
            return ((AIPathPreference *)player->aiPathData)[player->sectorIndex].pathPreference;
        }
    }

    // Reset shortcut choice if no shortcut available
    if (!(player->pathFlags & PATH_FLAG_SHORTCUT_AVAILABLE)) {
        player->aiShortcutChosen = 0;
    }

    // Special mode: unk86 is set (possibly time attack or special mode)
    if (gs->unk86 != 0) {
        if (player->aiShortcutChosen == 0 && (player->pathFlags & PATH_FLAG_SHORTCUT_AVAILABLE)) {
            // 25% chance to skip shortcut
            if ((randA() & 0xFF) >= SHORTCUT_SKIP_CHANCE) {
                return PATH_CHOICE_MAIN;
            }
            player->aiShortcutChosen = 1;
            return PATH_CHOICE_SHORTCUT;
        }
        return PATH_CHOICE_MAIN;
    }

    // Normal race mode (not race type 9)
    if (gs->raceType != 9) {
        if (player->aiShortcutChosen == 0 && (player->pathFlags & PATH_FLAG_SHORTCUT_AVAILABLE)) {
            // Check random shortcut chance based on memory pool
            if ((randA() & 0xFF) < gShortcutChanceByMemoryPool[gs->memoryPoolId]) {
                player->aiShortcutChosen = 1;
                return PATH_CHOICE_SHORTCUT;
            }
            // Boss characters (ID >= 6) always take shortcuts
            if (player->characterId >= 6) {
                player->aiShortcutChosen = 1;
                return PATH_CHOICE_SHORTCUT;
            }
        }
    }

    return PATH_CHOICE_MAIN;
}
