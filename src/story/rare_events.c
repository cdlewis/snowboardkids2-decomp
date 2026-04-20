#include "story/rare_events.h"
#include "D_800AFE8C_A71FC_type.h"
#include "audio/audio.h"
#include "common.h"
#include "gamestate.h"
#include "math/geometry.h"
#include "story/npc_dialogue.h"
#include "system/task_scheduler.h"
#include "triggers/town_collision.h"

// Parallax screen coordinate history buffer
typedef struct {
    s16 historyIndex;      // Current write position (element [0])
    s16 coordHistory[502]; // X/Z coordinate pairs (elements [1-502])
} ParallaxScreenHistory;

typedef struct {
    /* 0x00 */ void *model;
    /* 0x04 */ Transform3D matrix;
    /* 0x24 */ s32 speed;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s16 targetAngle;
    /* 0x2E */ s16 currentAngle;
    /* 0x30 */ u8 pad30[0x7];
    /* 0x37 */ u8 animFrameComplete;
    /* 0x38 */ u8 pad38[0x14];
    /* 0x4C */ u16 *animSequencePtr;
    /* 0x50 */ u16 animFrame;
    /* 0x52 */ u8 pad52[0x2];
    /* 0x54 */ u16 savedAnimFrame;
    /* 0x56 */ u16 unk56;
    /* 0x58 */ u16 animSpeed;
    /* 0x5A */ u16 unk5A;
    /* 0x5C */ u8 npcIndex;
    /* 0x5D */ u8 itemType;
    /* 0x5E */ u8 state;
    /* 0x5F */ u8 pad5F[0x2];
    /* 0x61 */ u8 prevState;
} Func8002A390Arg;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 screenX;
    /* 0x18 */ u8 pad18[4];
    /* 0x1C */ s32 screenZ;
} ParallaxSprite;

typedef struct {
    /* 0x00 */ Transform3D matrix;
    /* 0x20 */ s32 speed;
    /* 0x24 */ s16 *waypointData;
    /* 0x28 */ s16 facingAngle;
    /* 0x2A */ s16 headingAngle;
    /* 0x2C */ u8 pad2C[0x8];
    /* 0x34 */ u8 moveDirection;
    /* 0x35 */ u8 waypointCount;
    /* 0x36 */ u8 currentWaypoint;
} WaypointMoveState;

u32 sNpcInteractionColors[] = {
    0x00BB00BB, 0x000000BB, 0x00BE00D0, 0x00D100D1, 0x00D100D0, 0x000000D4,
};

u16 sNpcInteractionSoundIds[][6] = {
    0x0000, 0x005C, 0x005C, 0x0060, 0x005B, 0x005C, 0x0074, 0x0000, 0x0074, 0x0074, 0x0074, 0x0074, 0x008A, 0x008A,
    0x0000, 0x0090, 0x008A, 0x008A, 0x00A7, 0x00A2, 0x00A7, 0x0000, 0x00A2, 0x00A2, 0x00BB, 0x00BB, 0x00BB, 0x00BB,
    0x0000, 0x00BB, 0x00D0, 0x00D1, 0x00D1, 0x00D1, 0x00D0, 0x0000, 0x010B, 0x010B, 0x010B, 0x010B, 0x010B, 0x010B,
};

u16 sDialogueSequence13_15[] = {
    0x000D,
    0x000E,
    0x000F,
    0xFFFF,
};

u16 sDialogueSequence10_12[] = {
    0x000A,
    0x000B,
    0x000C,
    0xFFFF,
};

u16 sDialogueSequence20_22[] = {
    0x0014,
    0x0015,
    0x0016,
    0xFFFF,
};

u16 sDialogueSequence22_24[] = {
    0x0016,
    0x0017,
    0x0018,
    0xFFFF,
};

u16 sDialogueSequence17_19[] = {
    0x0011,
    0x0012,
    0x0013,
    0xFFFF,
};

u16 sDialogueSequenceEmpty[] = {
    0x0000,
    0xFFFF,
};

u16 sNpcDialoguePointerTable[] = {
    0x001B, 0x001B, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x8008, 0xE73C, 0x8008, 0xE73C, 0x8008, 0xE754, 0x8008,
    0xE73C, 0x8008, 0xE73C, 0x8008, 0xE744, 0x8008, 0xE73C, 0x0000, 0x0000, 0x8008, 0xE73C, 0x8008, 0xE73C,
    0x8008, 0xE73C, 0x8008, 0xE73C, 0x8008, 0xE744, 0x8008, 0xE73C, 0x8008, 0xE73C, 0x0000, 0x0000, 0x8008,
    0xE73C, 0x8008, 0xE73C, 0x8008, 0xE73C, 0x8008, 0xE744, 0x8008, 0xE75C, 0x8008, 0xE73C, 0x8008, 0xE75C,
    0x0000, 0x0000, 0x8008, 0xE73C, 0x8008, 0xE73C, 0x8008, 0xE744, 0x8008, 0xE73C, 0x8008, 0xE73C, 0x8008,
    0xE73C, 0x8008, 0xE73C, 0x0000, 0x0000, 0x8008, 0xE73C, 0x8008, 0xE744, 0x8008, 0xE73C, 0x8008, 0xE73C,
    0x8008, 0xE73C, 0x8008, 0xE73C, 0x8008, 0xE73C, 0x0000, 0x0000, 0x8008, 0xE744,
};

// Story map NPC configuration table
// First 10 entries (unk5C=0-9): arrays of 6 dialogue sequence pointers each
// Entries 10+ contain sound data mixed with pointers
// Layout per entry: 24 bytes
typedef struct {
    u32 dialogueTable[10][6];
    u32 extraData[118];
} StoryMapNpcDialogueConfig;

StoryMapNpcDialogueConfig sStoryMapNpcDialogueTable = {
    {
     0x00000000, (u32)sDialogueSequence13_15,
     (u32)sDialogueSequence13_15,
     (u32)sDialogueSequence22_24,
     (u32)sDialogueSequence13_15,
     (u32)sDialogueSequence13_15,
     (u32)sDialogueSequence13_15,
     0x00000000, (u32)sDialogueSequence13_15,
     (u32)sDialogueSequence13_15,
     (u32)sDialogueSequence13_15,
     (u32)sDialogueSequence13_15,
     (u32)sDialogueSequence13_15,
     (u32)sDialogueSequence13_15,
     0x00000000, (u32)sDialogueSequence13_15,
     (u32)sDialogueSequence13_15,
     (u32)sDialogueSequence13_15,
     (u32)sDialogueSequence20_22,
     (u32)sDialogueSequence13_15,
     (u32)sDialogueSequence20_22,
     0x00000000, (u32)sDialogueSequence13_15,
     (u32)sDialogueSequence13_15,
     (u32)sDialogueSequence13_15,
     (u32)sDialogueSequence13_15,
     (u32)sDialogueSequence13_15,
     (u32)sDialogueSequence13_15,
     0x00000000, (u32)sDialogueSequence13_15,
     (u32)sDialogueSequence13_15,
     (u32)sDialogueSequence13_15,
     (u32)sDialogueSequence13_15,
     (u32)sDialogueSequence13_15,
     (u32)sDialogueSequence13_15,
     0x00000000, (u32)sDialogueSequence13_15,
     (u32)sDialogueSequence13_15,
     (u32)sDialogueSequence13_15,
     (u32)sDialogueSequence13_15,
     (u32)sDialogueSequence13_15,
     (u32)sDialogueSequence13_15,
     0x00000000, 0x00000000,
     (u32)sDialogueSequence13_15,
     (u32)sDialogueSequence13_15,
     (u32)sDialogueSequenceEmpty,
     0x00000000, 0x00000000,
     0x00000000, 0x00000000,
     0x00000000, 0x00000000,
     (u32)sDialogueSequence13_15,
     (u32)sDialogueSequence13_15,
     (u32)sDialogueSequence13_15,
     (u32)sDialogueSequenceEmpty,
     (u32)sDialogueSequence13_15,
     (u32)sDialogueSequenceEmpty,
     (u32)sDialogueSequenceEmpty,
     },
    {
     (u32)sDialogueSequenceEmpty,
     (u32)sDialogueSequenceEmpty,
     (u32)sDialogueSequence13_15,
     (u32)sDialogueSequenceEmpty,
     0x0000005C, 0x005E0000,
     0x0000005E, 0x00000000,
     0x00790074, 0x00000077,
     0x00000000, 0x00740000,
     0x00000000, 0x008D008E,
     0x0000008D, 0x008A0000,
     0x00AB00A2, 0x00A50000,
     0x000000A5, 0x00A20000,
     0x00C000BB, 0x000000BF,
     0x000000BE, 0x00000000,
     0x00D60000, 0x00D400D5,
     0x000000D4, 0x00D00000,
     0x0051006D, 0x00000000,
     0x00000000, 0x000000F5,
     0x00E30000, 0x00000000,
     0x00830099, 0x000000C9,
     0x000000F4, 0x00E10000,
     0x00000000, 0x00830000,
     0x000000C9, 0x000000F2,
     0x00E30000, 0x00590072,
     0x008800A0, 0x00B900CE,
     0x03030303, 0x03010000,
     0x524F4F54, 0x204D4F56,
     0x45000000, 0x00000041,
     0x564F4944, 0x00000000,
     0x00000000, 0x00005741,
     0x49540000, 0x00000000,
     0x00000000, 0x00524554,
     0x55524E00, 0x00000000,
     0x00000000, 0x57414954,
     0x32000000, 0x00000000,
     0x00000000, 0x00000000,
     0x00000000, 0x00000000,
     0x00020304, 0x05060907,
     0x00010304, 0x05060809,
     0x00010204, 0x05060907,
     0x00010203, 0x05060807,
     0x00010203, 0x04060907,
     0x00010203, 0x04050807,
     0x000D0602, 0x000D0602,
     0x000D0602, 0x000D0602,
     0x0306090D, 0x0306090D,
     0x0306090D, 0x0306090D,
     0x020E0F07, 0x020E0F07,
     0x020E0F07, 0x020E0F07,
     0x080E0407, 0x080E0407,
     0x080E0407, 0x080E0407,
     0x01050A0B, 0x01050A0B,
     0x01050A0B, 0x01050A0B,
     0x090A0E0F, 0x090A0E0F,
     0x090A0E0F, 0x090A0E0F,
     0x0C020304, 0x0C020304,
     0x0C020304, 0x0C020304,
     0x0D0E0F0D, 0x0E0F0D0E,
     0x0F0D0E0F, 0x0D0E0F0D,
     0x0D0E0F0D, 0x0E0F0D0E,
     0x0F0D0E0F, 0x0D0E0F0D,
     },
};

extern ParallaxScreenHistory gParallaxScreenHistory;
extern Vec3i gParallaxOffset;
extern s32 gParallaxFrameCounter;
extern s16 gParallaxWobbleAngle;
extern s32 D_800AB248_A25B8;
extern s32 D_8009F230_9FE30;
extern s32 D_8009F234_9FE34;
extern s16 D_8009F238_9FE38;
extern s16 gStoryMapItemWaypointCounts[];

s32 isNpcFacingPlayer(s32 npcX, s32 npcZ, s16 npcFacingAngle);
s32 moveStoryMapItemAlongWaypoints(WaypointMoveState *, u8);
s32 updateStoryMapNpcBehavior(Func8002A390Arg *);
s16 stepAngleTowardsTarget(s16 targetAngle, s16 currentAngle);

s32 tryStoryMapNpcInteraction(Func8002A390Arg *arg0) {
    GameState *state;
    s32 stateVal;

    state = getCurrentAllocation();
    stateVal = arg0->state;

    switch (stateVal) {
        case 0:
            if (moveStoryMapItemAlongWaypoints((WaypointMoveState *)&arg0->matrix, arg0->itemType) != 0) {
                return 1;
            }
            if (state->unk421 != 0) {
                return 0;
            }
            if (isNpcFacingPlayer(arg0->matrix.translation.x, arg0->matrix.translation.z, arg0->currentAngle) == 0) {
                return 0;
            }
            arg0->savedAnimFrame = arg0->animFrame;
            if (arg0->itemType == 5) {
                arg0->animFrame = 0x21;
            } else {
                arg0->animFrame = 0;
            }
            arg0->prevState = arg0->state;
            arg0->state = 0x44;
            break;
        case 1:
        case 2:
        case 3:
        case 4:
            if (updateStoryMapNpcBehavior(arg0) == 0) {
                return 0;
            }
            arg0->prevState = arg0->state;
            arg0->state = 0;
            break;
        case 0x44:
            if (updateStoryMapNpcBehavior(arg0) == 0) {
                return 0;
            }
            arg0->prevState = arg0->state;
            arg0->state = 0;
            arg0->animFrame = arg0->savedAnimFrame;
            break;
    }

    return 0;
}

s32 moveStoryMapItemAlongWaypoints(WaypointMoveState *state, u8 itemType) {
    s32 targetX;
    s32 targetZ;
    s32 reachedEndpoint;
    s32 absDeltaZ;
    s32 absDeltaX;
    s32 deltaZ;
    s32 deltaX;
    s32 threshold;
    const s32 highSpeed = 0x1C000;
    const s32 lowSpeed = 0x10000;
    s8 wpIdx;

    getCurrentAllocation();
    reachedEndpoint = 0;

    if (state->moveDirection != 0) {
        wpIdx = ((state->currentWaypoint + 1) % state->waypointCount) * 2;
    } else {
        wpIdx = (state->currentWaypoint - 1) * 2;
        if (wpIdx < 0) {
            wpIdx = (state->waypointCount - 1) * 2;
        }
    }

    targetX = state->waypointData[wpIdx + 1] << 16;
    targetZ = state->waypointData[wpIdx + 2] << 16;

    state->matrix.translation.x += ((approximateSin(state->headingAngle) * (state->speed >> 4)) / 0x2000) * 16;
    state->matrix.translation.z += ((approximateCos(state->headingAngle) * (state->speed >> 4)) / 0x2000) * 16;

    {
        s16 angle = stepAngleTowardsTarget(state->headingAngle, state->facingAngle);
        state->facingAngle = angle;
        createYRotationMatrix(&state->matrix, angle & 0x1FFF);
    }

    threshold = -(state->speed >= 0x1B000) & highSpeed;
    threshold |= lowSpeed;

    deltaX = targetX - state->matrix.translation.x;
    absDeltaX = ABS(deltaX);

    if (absDeltaX <= threshold) {
        deltaZ = targetZ - state->matrix.translation.z;
        absDeltaZ = ABS(deltaZ);
        if (absDeltaZ <= threshold) {
            if (state->moveDirection != 0) {
                state->currentWaypoint = state->currentWaypoint + 1;
            } else {
                state->currentWaypoint = state->currentWaypoint - 1;
            }

            if (state->currentWaypoint == 0xFF) {
                state->currentWaypoint = state->waypointCount - 1;
            } else {
                state->currentWaypoint = state->currentWaypoint % state->waypointCount;
            }

            if (state->moveDirection != 0) {
                wpIdx = ((state->currentWaypoint + 1) % state->waypointCount) * 2;
            } else {
                wpIdx = (state->currentWaypoint - 1) * 2;
                if (wpIdx < 0) {
                    wpIdx = (state->waypointCount - 1) * 2;
                }
            }

            if ((state->currentWaypoint == 0 || state->currentWaypoint == state->waypointCount - 1) &&
                gStoryMapItemWaypointCounts[itemType] != 4) {
                reachedEndpoint = 1;
            } else {
                state->headingAngle = computeAngleToPosition(
                    state->waypointData[wpIdx + 1] << 16,
                    state->waypointData[wpIdx + 2] << 16,
                    state->matrix.translation.x,
                    state->matrix.translation.z
                );
            }
        }
    }

    return reachedEndpoint;
}

s32 updateStoryMapNpcBehavior(Func8002A390Arg *s0) {
    Vec3i *new_var;
    Vec3i sp10;
    GameState *s3;
    s32 s4;
    s16 a2;
    s32 v0;
    s32 a0;
    s32 a1;
    s32 t0;
    s32 t1;
    Vec3i *s1;
    Vec3i *s2;
    s3 = getCurrentAllocation();
    s4 = 0;
    switch (s0->state) {
        case 1:
            s1 = &sp10;
            s2 = &s0->matrix.translation;
            memcpy(&sp10, s2, sizeof(Vec3i));
            v0 = approximateSin(s0->currentAngle) * (s0->speed >> 4);
            if (v0 < 0) {
                v0 += 0x1FFF;
            }
            sp10.x += (v0 >> 13) * 16;
            v0 = approximateCos(s0->currentAngle) * (s0->speed >> 4);
            if (v0 < 0) {
                v0 += 0x1FFF;
            }
            t1 = sp10.z + ((v0 >> 13) * 16);
            sp10.z = t1;
            if (checkTownPlayerCollision(sp10.x, t1, 0) != 0) {
                memcpy(&sp10, s2, sizeof(Vec3i));
                s0->prevState = s0->state;
                s0->state = 2;
            }
            v0 = checkTownLamppostCollision(sp10.x, sp10.z, s3->npcCollisionRadius[0]);
            if (v0 != 0) {
                resolveTownLamppostCollision(&sp10, s3->npcCollisionRadius[0], v0);
            }
            if (((u32)(distance_2d(sp10.x, sp10.z) + 0xFFDC0000)) > 0x640000) {
                memcpy(&sp10, s2, sizeof(Vec3i));
                s0->prevState = s0->state;
                s0->state = 2;
                s0->currentAngle = computeAngleToPosition(D_8009F230_9FE30, D_8009F234_9FE34, sp10.x, sp10.z);
                break;
            }
            memcpy(s2, &sp10, sizeof(Vec3i));
            a2 = stepAngleTowardsTarget(s0->currentAngle, s0->targetAngle);
            if (a2 == (s0->targetAngle + 0x1000)) {
                s0->animFrame = 1;
            }
            s0->targetAngle = a2;
            createYRotationMatrix(&s0->matrix, a2 & 0x1FFF);
            if (distance_2d(sp10.x - D_8009F230_9FE30, sp10.z - D_8009F234_9FE34) > 0x100000) {
                s0->prevState = s0->state;
                s0->state = 2;
                s0->currentAngle = computeAngleToPosition(s3->unk3EC, s3->unk3F0, sp10.x, s1->z);
            }
            break;

        case 2:
            a2 = stepAngleTowardsTarget(s0->currentAngle, s0->targetAngle);
            if (a2 == (s0->targetAngle + 0x1000)) {
                s0->animFrame = 1;
            }
            s0->targetAngle = a2;
            createYRotationMatrix(&s0->matrix, a2 & 0x1FFF);
            if (s0->currentAngle == s0->targetAngle) {
                s0->animFrame = 0;
            } else {
                s0->animFrame = s0->animSpeed;
            }
            if (isNpcFacingPlayer(D_8009F230_9FE30, D_8009F234_9FE34, D_8009F238_9FE38) != 0) {
                return s4;
            }
            s0->prevState = s0->state;
            s0->animFrame = s0->animSpeed;
            if (D_8009F230_9FE30 == s0->matrix.translation.x) {
                if (D_8009F234_9FE34 == s0->matrix.translation.z) {
                    s0->state = 0;
                    s0->currentAngle = D_8009F238_9FE38;
                    break;
                }
            }
            s0->state = 3;
            s0->currentAngle = computeAngleToPosition(
                D_8009F230_9FE30,
                D_8009F234_9FE34,
                s0->matrix.translation.x,
                s0->matrix.translation.z
            );
            break;

        case 3:
            new_var = &sp10;
            s2 = new_var;
            memcpy(new_var, &s0->matrix.translation, sizeof(Vec3i));
            v0 = approximateSin(s0->currentAngle) * (s0->speed >> 4);
            if (v0 < 0) {
                v0 += 0x1FFF;
            }
            sp10.x += (v0 >> 13) * 16;
            v0 = approximateCos(s0->currentAngle) * (s0->speed >> 4);
            if (v0 < 0) {
                v0 += 0x1FFF;
            }
            sp10.z += (v0 >> 13) * 16;
            memcpy(&s0->matrix.translation, new_var, sizeof(Vec3i));
            a2 = stepAngleTowardsTarget(s0->currentAngle, s0->targetAngle);
            if (a2 == (s0->targetAngle + 0x1000)) {
                s0->animFrame = 1;
            }
            s0->targetAngle = a2;
            createYRotationMatrix(&s0->matrix, a2 & 0x1FFF);
            if (isNpcFacingPlayer(sp10.x, sp10.z, s0->currentAngle) == 0) {
                if (distance_2d(sp10.x - D_8009F230_9FE30, sp10.z - D_8009F234_9FE34) <= 0xBFFF) {
                    s4 = 1;
                    s0->currentAngle = D_8009F238_9FE38;
                }
            } else {
                s0->prevState = s0->state;
                s0->state = 4;
                s0->animFrame = 0;
            }
            break;

        case 4:
            if (isNpcFacingPlayer(s0->matrix.translation.x, s0->matrix.translation.z, s0->currentAngle) != 0) {
                break;
                return s4;
            }
            s0->prevState = s0->state;
            s0->state = 3;
            s0->animFrame = s0->animSpeed;
            s0->currentAngle = computeAngleToPosition(
                D_8009F230_9FE30,
                D_8009F234_9FE34,
                s0->matrix.translation.x,
                s0->matrix.translation.z
            );
            break;

        case 0x44:
            if (isNpcFacingPlayer(s0->matrix.translation.x, s0->matrix.translation.z, s0->currentAngle) == 0) {
                s4 = 1;
            }
    }

    return s4;
}

s32 isNpcFacingPlayer(s32 npcX, s32 npcZ, s16 npcFacingAngle) {
    GameState *state;
    s16 angleToNpc;

    state = getCurrentAllocation();
    angleToNpc = computeAngleToPosition(state->unk3EC, state->unk3F0, npcX, npcZ);

    if (npcFacingAngle - 0x238 < angleToNpc && angleToNpc < npcFacingAngle + 0x238) {
        if (distance_2d(state->unk3EC - npcX, state->unk3F0 - npcZ) <= 0x280000) {
            return 1;
        }
    }
    return 0;
}

s16 stepAngleTowardsTarget(s16 targetAngle, s16 currentAngle) {
    s16 target;
    s16 current;
    s32 diff;
    s16 absDiff;
    s16 direction;

    target = targetAngle;
    current = currentAngle;

    if (targetAngle == currentAngle) {
        goto done;
    }

    // Calculate absolute difference for wrap check
    diff = targetAngle - currentAngle;
    diff = ABS(diff);

    // If difference >= half circle, need to wrap
    if ((s16)diff >= 0x1001) {
        if (currentAngle < targetAngle) {
            current = currentAngle + 0x2000;
        } else {
            target = targetAngle + 0x2000;
        }
    }

    // Re-extract signed values
    targetAngle = target;
    currentAngle = current;

    // Calculate new difference after possible wrap
    diff = targetAngle - currentAngle;
    absDiff = ABS(diff);

    // Large diff: fixed large step
    if (absDiff >= 0xAAB) {
        current += 0x1000;
        goto done;
    }

    // Calculate direction
    if (currentAngle < targetAngle) {
        direction = 1;
    } else {
        direction = -1;
    }

    // Medium diff: fixed step of 128
    if (absDiff >= 0x80) {
        current += direction << 7;
        goto done;
    }

    // Small diff: snap towards target
    current = (current + absDiff * direction) & 0x1FFF;

done:
    return current;
}

typedef struct {
    /* 0x00 */ void *model;
    /* 0x04 */ Transform3D matrix;
    /* 0x24 */ s32 speed;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s16 targetAngle;
    /* 0x2E */ s16 currentAngle;
    /* 0x30 */ u16 turnAngle;
    /* 0x32 */ s16 dialogueTargetAngle;
    /* 0x34 */ u8 pad34[0x2];
    /* 0x36 */ u8 turnDirection;
    /* 0x37 */ u8 animFrameComplete;
    /* 0x38 */ u8 pad38[0x14];
    /* 0x4C */ u16 *animSequencePtr;
    /* 0x50 */ s16 animFrame;
    /* 0x52 */ u8 pad52[0x2];
    /* 0x54 */ u16 unk54;
    /* 0x56 */ u16 unk56;
    /* 0x58 */ u16 animSpeed;
    /* 0x5A */ u16 unk5A;
    /* 0x5C */ u8 npcIndex;
    /* 0x5D */ u8 itemType;
    /* 0x5E */ u8 state;
    /* 0x5F */ u8 pad5F[0x2];
    /* 0x61 */ u8 prevState;
} RareEventNpc;

void initStoryMapNpcSpecialDialogue(Func8002A390Arg *);

void updateStoryMapNpcDialogueTurn(RareEventNpc *npc) {
    GameState *alloc;
    s16 angleDiff;
    u8 state;
    s32 absAngleDiff;
    s16 savedAngle;

    alloc = getCurrentAllocation();
    state = alloc->dialogueTurnState & 0xF;
    alloc->dialogueTurnState = alloc->dialogueTurnState & 0xF0;

    switch (state) {
        case 1:
            savedAngle = (npc->turnAngle = npc->targetAngle);
            npc->dialogueTargetAngle = computeAngleToPosition(
                alloc->unk3EC,
                alloc->unk3F0,
                npc->matrix.translation.x,
                npc->matrix.translation.z
            );
            angleDiff = signedAngleDifference(savedAngle, npc->dialogueTargetAngle);
            if (angleDiff < 0) {
                npc->turnDirection = 1;
            } else {
                npc->turnDirection = 0;
            }
            absAngleDiff = ABS(angleDiff);
            if (absAngleDiff >= 0xAAB) {
                npc->animFrame = 1;
                npc->turnAngle = (npc->turnAngle + 0x1000) & 0x1FFF;
                if (npc->itemType == 5) {
                    npc->animFrame = 0x22;
                }
                npc->animFrameComplete = 0;
                if (signedAngleDifference(npc->turnAngle, npc->dialogueTargetAngle) < 0) {
                    npc->turnDirection = 1;
                } else {
                    npc->turnDirection = 0;
                }
            } else {
                if (absAngleDiff < 0x2AA) {
                    if (npc->itemType != 5) {
                        npc->animFrame = 0;
                    } else {
                        npc->animFrame = 0x21;
                    }
                } else {
                    npc->animFrame = 2;
                    if (npc->itemType == 5) {
                        npc->animFrame = 0x1D;
                    }
                }
                npc->animFrameComplete = 1;
            }
            state = 2;
            break;

        case 2: {
            s32 absVal;
            angleDiff = signedAngleDifference(npc->turnAngle, npc->dialogueTargetAngle);
            absVal = ABS(angleDiff);
            if (absVal < 0xA0) {
                if (npc->animFrame == 2 || npc->animFrame == 0x1D) {
                    npc->animFrame = 0;
                    if (npc->itemType == 5) {
                        npc->animFrame = 0x21;
                    }
                }
                angleDiff = (angleDiff >= 0) ? angleDiff : -angleDiff;
                state = 3;
            } else {
                angleDiff = 0xA0;
            }
            if (npc->turnDirection != 0) {
                s32 temp = angleDiff;
                angleDiff = -temp;
            }
            if (npc->animFrame == 1 || npc->animFrame == 0x22) {
                if (npc->animFrameComplete != 0) {
                    npc->animFrame = 0;
                    if (npc->itemType == 5) {
                        npc->animFrame = 0x21;
                    }
                }
            }
            npc->turnAngle = npc->turnAngle + angleDiff;
            break;
        }

        case 3:
            savedAngle = alloc->dialogueTurnState;
            if (savedAngle == 0x30 && npc->animFrameComplete != 0) {
                u16 dialogueIndex;
                npc->animSequencePtr =
                    (u16 *)sStoryMapNpcDialogueTable.dialogueTable[npc->npcIndex][D_800AFE8C_A71FC->playerBoardIds[0]];
                dialogueIndex = npc->animSequencePtr[0];
                npc->animFrameComplete = 0;
                npc->animFrame = dialogueIndex;
                npc->animSequencePtr += 1;
                state = 4;
                playSoundEffectOnChannelNoPriority(
                    sNpcInteractionSoundIds[npc->npcIndex][D_800AFE8C_A71FC->playerBoardIds[0]],
                    1
                );
                initStoryMapNpcSpecialDialogue((Func8002A390Arg *)npc);
            } else {
                if ((npc->animFrame == 1 || npc->animFrame == 0x22) && npc->animFrameComplete != 0) {
                    npc->animFrame = 0;
                    if (npc->itemType == 5) {
                        npc->animFrame = 0x21;
                    }
                }
            }
            break;

        case 4:
            if (npc->animFrameComplete != 0) {
                npc->animFrameComplete = 0;
                if (*npc->animSequencePtr == 0xFFFF) {
                    state = 5;
                    npc->animFrame = 0;
                    if (npc->itemType == 5) {
                        npc->animFrame = 0x21;
                    }
                } else {
                    npc->animFrame = *npc->animSequencePtr;
                    npc->animSequencePtr += 1;
                }
            }
            break;

        case 5:
            if (alloc->dialogueTurnState == 0) {
                state = 0xF;
                npc->state = 0x15;
                npc->dialogueTargetAngle = 0;
            }
            break;
    }

    alloc->dialogueTurnState |= state;
}

void animateNpcTurnToTarget(Func297D8Arg *npc) {
    GameState *alloc;
    s16 angleDiff;
    s32 turnComplete;
    s32 absAngleDiff;

    alloc = getCurrentAllocation();
    turnComplete = 0;

    switch (npc->unk32) {
        case 0:
            angleDiff = signedAngleDifference(npc->turnAngle, npc->unk2E);
            if (angleDiff < 0) {
                npc->unk36 = 1;
            } else {
                npc->unk36 = 0;
            }
            absAngleDiff = (angleDiff >= 0) ? angleDiff : -angleDiff;
            if (absAngleDiff >= 0xAAB) {
                npc->animState = 1;
                npc->turnAngle = (npc->turnAngle + 0x1000) & 0x1FFF;
                if (npc->itemType == 5) {
                    npc->animState = 0x22;
                }
                npc->unk37 = 0;
                if (signedAngleDifference(npc->turnAngle, npc->unk2E) < 0) {
                    npc->unk36 = 1;
                } else {
                    npc->unk36 = 0;
                }
            } else {
                npc->unk37 = 1;
                if (absAngleDiff < 0x2AA) {
                    if (npc->itemType != 5) {
                        npc->animState = 0;
                    } else {
                        npc->animState = 0x21;
                    }
                } else {
                    npc->animState = 2;
                    if (npc->itemType == 5) {
                        npc->animState = 0x1D;
                    }
                }
            }
            npc->unk32 = 1;
            break;

        case 1:
            angleDiff = signedAngleDifference(npc->turnAngle, npc->unk2E);
            absAngleDiff = (angleDiff >= 0) ? angleDiff : -angleDiff;
            if (absAngleDiff < 0xA0) {
                angleDiff = absAngleDiff;
                if (npc->unk37 != 0) {
                    turnComplete = 1;
                } else {
                    npc->unk32 = 2;
                }
                if (npc->animState == 2 || npc->animState == 0x1D) {
                    npc->animState = 0;
                    if (npc->itemType == 5) {
                        npc->animState = 0x21;
                    }
                }
            } else {
                angleDiff = 0xA0;
            }
            if (npc->unk36 != 0) {
                s32 temp = angleDiff;
                angleDiff = -temp;
            }
            npc->turnAngle = npc->turnAngle + angleDiff;
            if (npc->animState == 1 || npc->animState == 0x22) {
                if (npc->unk37 != 0) {
                    if (npc->itemType == 5) {
                        npc->animState = 0x21;
                    } else {
                        npc->animState = 0;
                    }
                }
            }
            break;

        case 2:
            if (npc->unk37 != 0) {
                turnComplete = 1;
            }
            break;
    }

    if ((u8)turnComplete != 0) {
        alloc->dialogueTurnState = 0;
        npc->rotation = npc->unk2E;
        npc->animState = npc->unk56;
        npc->unk5E = npc->unk5F;
    }
}

s16 signedAngleDifference(s16 fromAngle, s16 toAngle) {
    s16 diff;

    fromAngle &= 0x1FFF;
    toAngle &= 0x1FFF;
    diff = (toAngle - fromAngle) & 0x1FFF;

    if (diff >= 0x1001) {
        diff |= 0xE000;
    }

    return diff;
}

#ifdef __sgi
#pragma reorder_frequency
#endif

void initStoryMapNpcSpecialDialogue(Func8002A390Arg *arg0) {
    u16 dialogueIndex;

    if (arg0->npcIndex == 4) {
        if (arg0->itemType == 5) {
            arg0->animSequencePtr = sNpcDialoguePointerTable;
            dialogueIndex = arg0->animSequencePtr[0];
            arg0->animFrameComplete = 0;
            arg0->animFrame = dialogueIndex;
            arg0->animSequencePtr += 1;
            playSoundEffectOnChannelNoPriority(0xBE, 1);
        }
    }
    if (arg0->npcIndex == 3) {
        if (arg0->itemType == 8) {
            arg0->animSequencePtr = sDialogueSequence17_19;
            dialogueIndex = arg0->animSequencePtr[0];
            arg0->animFrameComplete = 0;
            arg0->animFrame = dialogueIndex;
            arg0->animSequencePtr += 1;
            playSoundEffectOnChannelNoPriority(0xAA, 1);
        }
    }
}

void updateParallaxSpriteFast(ParallaxSprite *sprite) {
    Vec3i newPosition;
    Vec3i currentPosition;
    Vec3i *currentPtr;
    Vec3i *newPtr;
    s32 zAdjust;
    s32 xAdjust;
    s32 historyIndex;
    s32 nextIndex;
    u16 *positionHistoryX;
    u16 *positionHistoryZ;
    s32 xCoord;
    s32 zCoord;
    s32 readX;
    s32 readZ;
    s32 zScaled;
    s32 xScaled;
    s32 zMultiplier;
    s32 xMultiplier;
    s32 temp_a1;
    u16 *coordHistoryPtr;
    s32 scaledOffset;

    if (!(gParallaxFrameCounter & 7)) {
        gParallaxScreenHistory.coordHistory[(gParallaxFrameCounter / 8) * 2] = xCoord = (s16)(sprite->screenX >> 16);
        gParallaxScreenHistory.coordHistory[(gParallaxFrameCounter / 8) * 2 + 1] = zCoord =
            (s16)(sprite->screenZ >> 16);
    }

    currentPtr = &currentPosition;
    newPtr = &newPosition;
    memcpy(currentPtr, &sprite->screenX, 12);
    memcpy(newPtr, currentPtr, 12);

    zMultiplier = 24;
    zScaled = currentPosition.z >> 8;
    zAdjust = zScaled * zMultiplier;
    scaledOffset = zAdjust / 19712;
    newPosition.x += scaledOffset << 12;

    xMultiplier = -24;
    xScaled = currentPosition.x >> 8;
    xAdjust = xScaled * xMultiplier;
    scaledOffset = xAdjust / 19712;
    newPosition.z += scaledOffset << 12;

    memcpy(&sprite->screenX, &newPosition, 12);

    gParallaxFrameCounter += 1;

    if ((u16)(atan2Fixed(newPosition.x, newPosition.z) - 0xBB0) < 0x450) {
        gParallaxScreenHistory.historyIndex = gParallaxFrameCounter / 8;

        if (gParallaxFrameCounter & 7) {
            gParallaxScreenHistory.coordHistory[((gParallaxFrameCounter / 8) + 1) * 2] = readX =
                (s16)(sprite->screenX >> 16);
            gParallaxScreenHistory.coordHistory[((gParallaxFrameCounter / 8) + 1) * 2 + 1] = readZ =
                (s16)(sprite->screenZ >> 16);
            gParallaxScreenHistory.historyIndex++;
        }
    }
}

void updateParallaxSpriteWobbling(ParallaxSprite *sprite) {
    Vec3i offset;
    Vec3i *offsetPtr;
    s32 zAdjust;
    s32 xAdjust;
    s32 xCoord;
    s32 zCoord;
    s32 readX;
    s32 readZ;
    s32 zScaled;
    s32 xScaled;
    s32 zRotationSpeed;
    s32 xRotationSpeed;
    s32 scaledOffset;

    if (!(gParallaxFrameCounter & 7)) {
        gParallaxScreenHistory.coordHistory[(gParallaxFrameCounter / 8) * 2] = xCoord = (s16)(sprite->screenX >> 16);
        gParallaxScreenHistory.coordHistory[(gParallaxFrameCounter / 8) * 2 + 1] = zCoord =
            (s16)(sprite->screenZ >> 16);
    }

    offsetPtr = &offset;
    memcpy(offsetPtr, &gParallaxOffset, 12);

    zRotationSpeed = 16;
    zScaled = offset.z >> 8;
    zAdjust = zScaled * zRotationSpeed;
    scaledOffset = zAdjust / 0x4000;
    gParallaxOffset.x += scaledOffset << 12;

    xRotationSpeed = -16;
    xScaled = offset.x >> 8;
    xAdjust = xScaled * xRotationSpeed;
    scaledOffset = xAdjust / 0x4000;
    gParallaxOffset.z += scaledOffset << 12;

    memcpy(&offset, &gParallaxOffset, 12);

    scaledOffset = (approximateSin(gParallaxWobbleAngle) / 2) << 8;
    offset.z += scaledOffset;
    gParallaxWobbleAngle = (((u16)gParallaxWobbleAngle) + 0x40) & 0x1FFF;

    memcpy(&sprite->screenX, &offset, 12);

    gParallaxFrameCounter += 1;

    if ((u16)(atan2Fixed(gParallaxOffset.x, gParallaxOffset.z) - 0xC18) < 0x3E8) {
        gParallaxScreenHistory.historyIndex = gParallaxFrameCounter / 8;

        if (gParallaxFrameCounter & 7) {
            gParallaxScreenHistory.coordHistory[((gParallaxFrameCounter / 8) + 1) * 2] = readX =
                (s16)(sprite->screenX >> 16);
            gParallaxScreenHistory.coordHistory[((gParallaxFrameCounter / 8) + 1) * 2 + 1] = readZ =
                (s16)(sprite->screenZ >> 16);
            gParallaxScreenHistory.historyIndex++;
        }
    }
}

void updateParallaxPosition(ParallaxSprite *sprite) {
    Vec3i newPosition;
    Vec3i currentPosition;
    s32 zAdjust;
    s32 xAdjust;
    s32 historyIndex;
    Vec3i *currentPtr;
    Vec3i *newPtr;
    s32 xCoord;
    s32 zCoord;
    s32 readX;
    s32 readZ;
    s32 xScaled;
    s32 multiplier;

    historyIndex = gParallaxFrameCounter;
    currentPtr = &currentPosition;
    newPtr = &newPosition;

    xCoord = (s16)(sprite->screenX >> 16);
    gParallaxScreenHistory.coordHistory[historyIndex * 2] = xCoord;
    zCoord = (s16)(sprite->screenZ >> 16);
    gParallaxScreenHistory.coordHistory[historyIndex * 2 + 1] = zCoord;

    memcpy(currentPtr, &gParallaxOffset, sizeof(Vec3i));
    memcpy(newPtr, currentPtr, sizeof(Vec3i));

    zAdjust = currentPosition.z >> 8;
    zAdjust = zAdjust * 8;
    zAdjust = zAdjust / 224;
    zCoord = zAdjust;
    newPosition.x += zAdjust << 12;

    multiplier = -8;
    xScaled = currentPosition.x >> 8;
    xAdjust = xScaled * multiplier;
    if (xAdjust < 0) {
        xAdjust += 0x7FF;
    }
    xAdjust >>= 11;
    zCoord = xAdjust;
    newPosition.z += zCoord << 12;

    memcpy(&gParallaxOffset, newPtr, sizeof(Vec3i));

    sprite->screenX = newPosition.x;
    sprite->screenZ = newPosition.z - 0x440000;

    gParallaxFrameCounter++;

    if (newPosition.z > 0) {
        if ((u32)(newPosition.x - 0x100001) <= 0xFFFFE) {
            gParallaxScreenHistory.historyIndex = gParallaxFrameCounter;
            readX = (s16)(sprite->screenX >> 16);
            gParallaxScreenHistory.coordHistory[gParallaxFrameCounter * 2] = readX;
            readZ = (s16)(sprite->screenZ >> 16);
            gParallaxScreenHistory.coordHistory[gParallaxFrameCounter * 2 + 1] = readZ;
        }
    }
}

void func_8002BAEC_2C6EC(ParallaxSprite *sprite) {
    Vec3i newPosition;
    Vec3i currentPosition;
    Vec3i *currentPtr;
    Vec3i *newPtr;
    s32 zAdjust;
    s32 xAdjust;
    s32 xCoord;
    s32 zCoord;
    s32 readX;
    s32 readZ;
    s32 zScaled;
    s32 xScaled;
    s8 multiplier;
    s32 multiplier2;
    s32 temp_v1;
    s16 temp_v0;

    if (!(gParallaxFrameCounter & 7)) {
        gParallaxScreenHistory.coordHistory[(gParallaxFrameCounter / 8) * 2] = xCoord = (s16)(sprite->screenX >> 16);
        gParallaxScreenHistory.coordHistory[(gParallaxFrameCounter / 8) * 2 + 1] = zCoord =
            (s16)(sprite->screenZ >> 16);
    }

    currentPtr = &currentPosition;
    newPtr = &newPosition;
    memcpy(currentPtr, &sprite->screenX, 12);
    memcpy(newPtr, currentPtr, 12);

    if (D_800AB248_A25B8 != 0) {
        multiplier = -24;
    } else {
        multiplier = 24;
    }
    zScaled = currentPosition.z >> 8;
    zAdjust = multiplier * zScaled;
    temp_v1 = zAdjust / 19712;
    newPosition.x += temp_v1 << 12;

    multiplier2 = -multiplier;
    xScaled = currentPosition.x >> 8;
    xAdjust = multiplier2 * xScaled;
    temp_v1 = xAdjust / 19712;
    newPosition.z += temp_v1 << 12;

    memcpy(&sprite->screenX, &newPosition, 12);

    gParallaxFrameCounter += 1;

    temp_v0 = atan2Fixed(newPosition.x, newPosition.z);
    if ((u16)(temp_v0 - 0x1001) < 0x450) {
        gParallaxScreenHistory.historyIndex = gParallaxFrameCounter / 8;

        if (gParallaxFrameCounter & 7) {
            gParallaxScreenHistory.coordHistory[((gParallaxFrameCounter / 8) + 1) * 2] = readX =
                (s16)(sprite->screenX >> 16);
            gParallaxScreenHistory.coordHistory[((gParallaxFrameCounter / 8) + 1) * 2 + 1] = readZ =
                (s16)(sprite->screenZ >> 16);
            gParallaxScreenHistory.historyIndex++;
        }
    } else {
        if ((u16)(temp_v0 - 0x81) < 0x1F) {
            D_800AB248_A25B8 = 1;
        }
    }
}
