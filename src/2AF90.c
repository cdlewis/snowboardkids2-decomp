#include "common.h"
#include "gamestate.h"
#include "geometry.h"
#include "graphics.h"
#include "task_scheduler.h"

extern s32 func_8002A4AC_2B0AC(void *, u8);
extern s32 func_8002A7CC_2B3CC(void *);
extern u16 D_8008E768_8F368[];
extern u16 D_8008E75C_8F35C[];

extern s16 D_8009F240_9FE40;
extern s16 D_8009F242_9FE42;
extern s32 D_800AB06C_A23DC;
extern Vec3i D_800AFF20_A7290;

s32 isNpcFacingPlayer(s32, s32, s16);

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
    angleToNpc = func_8006D21C_6DE1C(state->unk3EC, state->unk3F0, npcX, npcZ);

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
            arg0->unk4C = D_8008E768_8F368;
            dialogueIndex = arg0->unk4C[0];
            arg0->unk37 = 0;
            arg0->unk50 = dialogueIndex;
            arg0->unk4C += 1;
            func_80058220_58E20(0xBE, 1);
        }
    }
    if (arg0->unk5C == 3) {
        if (arg0->unk5D == 8) {
            arg0->unk4C = D_8008E75C_8F35C;
            dialogueIndex = arg0->unk4C[0];
            arg0->unk37 = 0;
            arg0->unk50 = dialogueIndex;
            arg0->unk4C += 1;
            func_80058220_58E20(0xAA, 1);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/2AF90", func_8002B598_2C198);

INCLUDE_ASM("asm/nonmatchings/2AF90", func_8002B760_2C360);

typedef struct {
    u8 pad0[0x14];
    s16 screenX;
    s16 screenXHi;
    u8 pad18[4];
    s16 screenZ;
    s16 screenZHi;
} ParallaxSprite;

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
