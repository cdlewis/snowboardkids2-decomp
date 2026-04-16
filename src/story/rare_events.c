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
    /* 0x37 */ u8 unk37;
    /* 0x38 */ u8 pad38[0x14];
    /* 0x4C */ u16 *unk4C;
    /* 0x50 */ u16 animFrame;
    /* 0x52 */ u8 pad52[0x2];
    /* 0x54 */ u16 unk54;
    /* 0x56 */ u16 unk56;
    /* 0x58 */ u16 animSpeed;
    /* 0x5A */ u16 unk5A;
    /* 0x5C */ u8 unk5C;
    /* 0x5D */ u8 unk5D;
    /* 0x5E */ u8 state;
    /* 0x5F */ u8 pad5F[0x2];
    /* 0x61 */ u8 prevState;
} Func8002A390Arg;

typedef struct {
    u8 pad0[0x14];
    union {
        s16 low;
        s32 full;
    } screenX;
    u8 pad18[4];
    union {
        s16 low;
        s32 full;
    } screenZ;
} ParallaxSprite;

u32 sNpcInteractionColors[] = {
    0x00BB00BB, 0x000000BB, 0x00BE00D0, 0x00D100D1, 0x00D100D0, 0x000000D4,
};

u16 sNpcInteractionSoundIds[] = {
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
u32 sStoryMapNpcDialogueTable[] = {
    0x00000000,
    (u32)sDialogueSequence13_15,
    (u32)sDialogueSequence13_15,
    (u32)sDialogueSequence22_24,
    (u32)sDialogueSequence13_15,
    (u32)sDialogueSequence13_15,
    (u32)sDialogueSequence13_15,
    0x00000000,
    (u32)sDialogueSequence13_15,
    (u32)sDialogueSequence13_15,
    (u32)sDialogueSequence13_15,
    (u32)sDialogueSequence13_15,
    (u32)sDialogueSequence13_15,
    (u32)sDialogueSequence13_15,
    0x00000000,
    (u32)sDialogueSequence13_15,
    (u32)sDialogueSequence13_15,
    (u32)sDialogueSequence13_15,
    (u32)sDialogueSequence20_22,
    (u32)sDialogueSequence13_15,
    (u32)sDialogueSequence20_22,
    0x00000000,
    (u32)sDialogueSequence13_15,
    (u32)sDialogueSequence13_15,
    (u32)sDialogueSequence13_15,
    (u32)sDialogueSequence13_15,
    (u32)sDialogueSequence13_15,
    (u32)sDialogueSequence13_15,
    0x00000000,
    (u32)sDialogueSequence13_15,
    (u32)sDialogueSequence13_15,
    (u32)sDialogueSequence13_15,
    (u32)sDialogueSequence13_15,
    (u32)sDialogueSequence13_15,
    (u32)sDialogueSequence13_15,
    0x00000000,
    (u32)sDialogueSequence13_15,
    (u32)sDialogueSequence13_15,
    (u32)sDialogueSequence13_15,
    (u32)sDialogueSequence13_15,
    (u32)sDialogueSequence13_15,
    (u32)sDialogueSequence13_15,
    0x00000000,
    0x00000000,
    (u32)sDialogueSequence13_15,
    (u32)sDialogueSequence13_15,
    (u32)sDialogueSequenceEmpty,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    (u32)sDialogueSequence13_15,
    (u32)sDialogueSequence13_15,
    (u32)sDialogueSequence13_15,
    (u32)sDialogueSequenceEmpty,
    (u32)sDialogueSequence13_15,
    (u32)sDialogueSequenceEmpty,
    (u32)sDialogueSequenceEmpty,
    (u32)sDialogueSequenceEmpty,
    (u32)sDialogueSequenceEmpty,
    (u32)sDialogueSequence13_15,
    (u32)sDialogueSequenceEmpty,
    0x0000005C,
    0x005E0000,
    0x0000005E,
    0x00000000,
    0x00790074,
    0x00000077,
    0x00000000,
    0x00740000,
    0x00000000,
    0x008D008E,
    0x0000008D,
    0x008A0000,
    0x00AB00A2,
    0x00A50000,
    0x000000A5,
    0x00A20000,
    0x00C000BB,
    0x000000BF,
    0x000000BE,
    0x00000000,
    0x00D60000,
    0x00D400D5,
    0x000000D4,
    0x00D00000,
    0x0051006D,
    0x00000000,
    0x00000000,
    0x000000F5,
    0x00E30000,
    0x00000000,
    0x00830099,
    0x000000C9,
    0x000000F4,
    0x00E10000,
    0x00000000,
    0x00830000,
    0x000000C9,
    0x000000F2,
    0x00E30000,
    0x00590072,
    0x008800A0,
    0x00B900CE,
    0x03030303,
    0x03010000,
    0x524F4F54,
    0x204D4F56,
    0x45000000,
    0x00000041,
    0x564F4944,
    0x00000000,
    0x00000000,
    0x00005741,
    0x49540000,
    0x00000000,
    0x00000000,
    0x00524554,
    0x55524E00,
    0x00000000,
    0x00000000,
    0x57414954,
    0x32000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00020304,
    0x05060907,
    0x00010304,
    0x05060809,
    0x00010204,
    0x05060907,
    0x00010203,
    0x05060807,
    0x00010203,
    0x04060907,
    0x00010203,
    0x04050807,
    0x000D0602,
    0x000D0602,
    0x000D0602,
    0x000D0602,
    0x0306090D,
    0x0306090D,
    0x0306090D,
    0x0306090D,
    0x020E0F07,
    0x020E0F07,
    0x020E0F07,
    0x020E0F07,
    0x080E0407,
    0x080E0407,
    0x080E0407,
    0x080E0407,
    0x01050A0B,
    0x01050A0B,
    0x01050A0B,
    0x01050A0B,
    0x090A0E0F,
    0x090A0E0F,
    0x090A0E0F,
    0x090A0E0F,
    0x0C020304,
    0x0C020304,
    0x0C020304,
    0x0C020304,
    0x0D0E0F0D,
    0x0E0F0D0E,
    0x0F0D0E0F,
    0x0D0E0F0D,
    0x0D0E0F0D,
    0x0E0F0D0E,
    0x0F0D0E0F,
    0x0D0E0F0D,
};

extern ParallaxScreenHistory gParallaxScreenHistory;
extern Vec3i gParallaxOffset;
extern s32 D_800AB06C_A23DC;
extern s32 D_8009F230_9FE30;
extern s32 D_8009F234_9FE34;
extern s16 D_8009F238_9FE38;

s32 isNpcFacingPlayer(s32 npcX, s32 npcZ, s16 npcFacingAngle);
s32 func_8002A4AC_2B0AC(void *, u8);
s32 updateStoryMapNpcBehavior(Func8002A390Arg *);
s16 stepAngleTowardsTarget(s16 targetAngle, s16 currentAngle);

s32 tryStoryMapNpcInteraction(Func8002A390Arg *arg0) {
    GameState *state;
    s32 stateVal;

    state = getCurrentAllocation();
    stateVal = arg0->state;

    switch (stateVal) {
        case 0:
            if (func_8002A4AC_2B0AC(&arg0->matrix, arg0->unk5D) != 0) {
                return 1;
            }
            if (state->unk421 != 0) {
                return 0;
            }
            if (isNpcFacingPlayer(arg0->matrix.translation.x, arg0->matrix.translation.z, arg0->currentAngle) == 0) {
                return 0;
            }
            arg0->unk54 = arg0->animFrame;
            if (arg0->unk5D == 5) {
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
            arg0->animFrame = arg0->unk54;
            break;
    }

    return 0;
}

// 98.12% https://decomp.me/scratch/bevnt
INCLUDE_ASM("asm/nonmatchings/story/rare_events", func_8002A4AC_2B0AC);

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

// 98.24% https://decomp.me/scratch/y0CS7
INCLUDE_ASM("asm/nonmatchings/story/rare_events", func_8002AE80_2BA80);

s16 signedAngleDifference(s16 fromAngle, s16 toAngle);

void animateNpcTurnToTarget(Func297D8Arg *npc) {
    GameState *alloc;
    s16 angleDiff;
    s32 turnComplete;
    s32 absAngleDiff;

    alloc = getCurrentAllocation();
    turnComplete = 0;

    switch (npc->unk32) {
        case 0:
            angleDiff = signedAngleDifference(npc->unk30, npc->unk2E);
            if (angleDiff < 0) {
                npc->unk36 = 1;
            } else {
                npc->unk36 = 0;
            }
            absAngleDiff = (angleDiff >= 0) ? angleDiff : -angleDiff;
            if (absAngleDiff >= 0xAAB) {
                npc->unk50 = 1;
                npc->unk30 = (npc->unk30 + 0x1000) & 0x1FFF;
                if (npc->unk5D == 5) {
                    npc->unk50 = 0x22;
                }
                npc->unk37 = 0;
                if (signedAngleDifference(npc->unk30, npc->unk2E) < 0) {
                    npc->unk36 = 1;
                } else {
                    npc->unk36 = 0;
                }
            } else {
                npc->unk37 = 1;
                if (absAngleDiff < 0x2AA) {
                    if (npc->unk5D != 5) {
                        npc->unk50 = 0;
                    } else {
                        npc->unk50 = 0x21;
                    }
                } else {
                    npc->unk50 = 2;
                    if (npc->unk5D == 5) {
                        npc->unk50 = 0x1D;
                    }
                }
            }
            npc->unk32 = 1;
            break;

        case 1:
            angleDiff = signedAngleDifference(npc->unk30, npc->unk2E);
            absAngleDiff = (angleDiff >= 0) ? angleDiff : -angleDiff;
            if (absAngleDiff < 0xA0) {
                angleDiff = absAngleDiff;
                if (npc->unk37 != 0) {
                    turnComplete = 1;
                } else {
                    npc->unk32 = 2;
                }
                if (npc->unk50 == 2 || npc->unk50 == 0x1D) {
                    npc->unk50 = 0;
                    if (npc->unk5D == 5) {
                        npc->unk50 = 0x21;
                    }
                }
            } else {
                angleDiff = 0xA0;
            }
            if (npc->unk36 != 0) {
                s32 temp = angleDiff;
                angleDiff = -temp;
            }
            npc->unk30 = npc->unk30 + angleDiff;
            if (npc->unk50 == 1 || npc->unk50 == 0x22) {
                if (npc->unk37 != 0) {
                    if (npc->unk5D == 5) {
                        npc->unk50 = 0x21;
                    } else {
                        npc->unk50 = 0;
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
        npc->unk50 = npc->unk56;
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

    if (arg0->unk5C == 4) {
        if (arg0->unk5D == 5) {
            arg0->unk4C = sNpcDialoguePointerTable;
            dialogueIndex = arg0->unk4C[0];
            arg0->unk37 = 0;
            arg0->animFrame = dialogueIndex;
            arg0->unk4C += 1;
            playSoundEffectOnChannelNoPriority(0xBE, 1);
        }
    }
    if (arg0->unk5C == 3) {
        if (arg0->unk5D == 8) {
            arg0->unk4C = sDialogueSequence17_19;
            dialogueIndex = arg0->unk4C[0];
            arg0->unk37 = 0;
            arg0->animFrame = dialogueIndex;
            arg0->unk4C += 1;
            playSoundEffectOnChannelNoPriority(0xAA, 1);
        }
    }
}

// 85.96% https://decomp.me/scratch/00Z3Q
INCLUDE_ASM("asm/nonmatchings/story/rare_events", func_8002B598_2C198);

// NO_SCRATCH
INCLUDE_ASM("asm/nonmatchings/story/rare_events", func_8002B760_2C360);

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

    historyIndex = D_800AB06C_A23DC;
    currentPtr = &currentPosition;
    newPtr = &newPosition;

    xCoord = sprite->screenX.low;
    gParallaxScreenHistory.coordHistory[historyIndex * 2] = xCoord;
    zCoord = sprite->screenZ.low;
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

    sprite->screenX.full = newPosition.x;
    sprite->screenZ.full = newPosition.z - 0x440000;

    D_800AB06C_A23DC++;

    if (newPosition.z > 0) {
        if ((u32)(newPosition.x - 0x100001) <= 0xFFFFE) {
            gParallaxScreenHistory.historyIndex = D_800AB06C_A23DC;
            readX = sprite->screenX.low;
            gParallaxScreenHistory.coordHistory[D_800AB06C_A23DC * 2] = readX;
            readZ = sprite->screenZ.low;
            gParallaxScreenHistory.coordHistory[D_800AB06C_A23DC * 2 + 1] = readZ;
        }
    }
}

// 84.56% https://decomp.me/scratch/aiCFG
INCLUDE_ASM("asm/nonmatchings/story/rare_events", func_8002BAEC_2C6EC);
