#include "audio.h"
#include "common.h"
#include "gamestate.h"
#include "geometry.h"
#include "task_scheduler.h"

typedef struct {
    /* 0x00 */ void *model;
    /* 0x04 */ Transform3D matrix;
    /* 0x24 */ u8 pad24[0xA];
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ u8 pad30[0x7];
    /* 0x37 */ u8 unk37;
    /* 0x38 */ u8 pad38[0x14];
    /* 0x4C */ u16 *unk4C;
    /* 0x50 */ u16 unk50;
    /* 0x52 */ u8 pad52[0x2];
    /* 0x54 */ u16 unk54;
    /* 0x56 */ u8 pad56[0x6];
    /* 0x5C */ u8 unk5C;
    /* 0x5D */ u8 unk5D;
    /* 0x5E */ u8 unk5E;
    /* 0x5F */ u8 pad5F[0x2];
    /* 0x61 */ u8 unk61;
} Func8002A390Arg;

typedef struct {
    u8 pad0[0x14];
    s16 screenX;
    s16 screenXHi;
    u8 pad18[4];
    s16 screenZ;
    s16 screenZHi;
} ParallaxSprite;

extern s32 func_8002A4AC_2B0AC(void *, u8);
extern s32 func_8002A7CC_2B3CC(void *);
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

extern s16 D_8009F240_9FE40;
extern s16 D_8009F242_9FE42;
extern s32 D_800AB06C_A23DC;
extern Vec3i D_800AFF20_A7290;

s32 isNpcFacingPlayer(s32 npcX, s32 npcZ, s16 npcFacingAngle);

s32 tryStoryMapNpcInteraction(Func8002A390Arg *arg0) {
    GameState *state;
    s32 stateVal;

    state = getCurrentAllocation();
    stateVal = arg0->unk5E;

    switch (stateVal) {
        case 0:
            if (func_8002A4AC_2B0AC(&arg0->matrix, arg0->unk5D) != 0) {
                return 1;
            }
            if (state->unk421 != 0) {
                return 0;
            }
            if (isNpcFacingPlayer(arg0->matrix.translation.x, arg0->matrix.translation.z, arg0->unk2E) == 0) {
                return 0;
            }
            arg0->unk54 = arg0->unk50;
            if (arg0->unk5D == 5) {
                arg0->unk50 = 0x21;
            } else {
                arg0->unk50 = 0;
            }
            arg0->unk61 = arg0->unk5E;
            arg0->unk5E = 0x44;
            break;
        case 1:
        case 2:
        case 3:
        case 4:
            if (func_8002A7CC_2B3CC(arg0) == 0) {
                return 0;
            }
            arg0->unk61 = arg0->unk5E;
            arg0->unk5E = 0;
            break;
        case 0x44:
            if (func_8002A7CC_2B3CC(arg0) == 0) {
                return 0;
            }
            arg0->unk61 = arg0->unk5E;
            arg0->unk5E = 0;
            arg0->unk50 = arg0->unk54;
            break;
    }

    return 0;
}

INCLUDE_ASM("asm/nonmatchings/2AF90", func_8002A4AC_2B0AC);

INCLUDE_ASM("asm/nonmatchings/2AF90", func_8002A7CC_2B3CC);

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

INCLUDE_ASM("asm/nonmatchings/2AF90", func_8002AE80_2BA80);

INCLUDE_ASM("asm/nonmatchings/2AF90", func_8002B248_2BE48);

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
            arg0->unk50 = dialogueIndex;
            arg0->unk4C += 1;
            playSoundEffectOnChannelNoPriority(0xBE, 1);
        }
    }
    if (arg0->unk5C == 3) {
        if (arg0->unk5D == 8) {
            arg0->unk4C = sDialogueSequence17_19;
            dialogueIndex = arg0->unk4C[0];
            arg0->unk37 = 0;
            arg0->unk50 = dialogueIndex;
            arg0->unk4C += 1;
            playSoundEffectOnChannelNoPriority(0xAA, 1);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/2AF90", func_8002B598_2C198);

INCLUDE_ASM("asm/nonmatchings/2AF90", func_8002B760_2C360);

void updateParallaxPosition(ParallaxSprite *sprite) {
    s32 newPosition[3];
    s32 currentPosition[3];
    s32 zAdjust;
    s32 xAdjust;
    s32 historyIndex;
    s32 nextIndex;
    s16 *positionHistoryX;
    s16 *positionHistoryZ;
    s32 *currentPtr;
    s32 *newPtr;
    s32 xCoord;
    s32 zCoord;
    s32 readX;
    s32 readZ;
    s32 xScaled;
    s32 multiplier;

    historyIndex = D_800AB06C_A23DC;
    currentPtr = currentPosition;
    newPtr = newPosition;
    positionHistoryX = &D_8009F242_9FE42;
    xCoord = sprite->screenX;
    positionHistoryX[historyIndex * 2] = xCoord;
    positionHistoryZ = positionHistoryX + 1;
    zCoord = sprite->screenZ;
    positionHistoryZ[historyIndex * 2] = zCoord;

    memcpy(currentPtr, &D_800AFF20_A7290, 12);
    memcpy(newPosition, currentPtr, 12);

    zAdjust = currentPtr[2] >> 8;
    zAdjust = zAdjust * 8;
    zAdjust = zAdjust / 224;
    zCoord = zAdjust;
    newPtr[0] += zCoord << 12;

    multiplier = -8;
    xScaled = currentPtr[0] >> 8;
    xAdjust = xScaled * multiplier;
    if (xAdjust < 0) {
        xAdjust += 0x7FF;
    }
    xAdjust >>= 11;
    zCoord = xAdjust;
    newPosition[2] += zCoord << 12;

    memcpy(&D_800AFF20_A7290, newPosition, 12);

    *(s32 *)&sprite->screenX = newPosition[0];
    *(s32 *)&sprite->screenZ = newPosition[2] - 0x440000;

    nextIndex = D_800AB06C_A23DC + 1;
    D_800AB06C_A23DC = nextIndex;

    if (newPosition[2] > 0) {
        if ((u32)(newPosition[0] - 0x100001) <= 0xFFFFE) {
            do {
                D_8009F240_9FE40 = nextIndex;
            } while (0);
            readX = sprite->screenX;
            positionHistoryX[nextIndex * 2] = readX;
            readZ = sprite->screenZ;
            positionHistoryZ[nextIndex * 2] = readZ;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/2AF90", func_8002BAEC_2C6EC);
