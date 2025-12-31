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

s32 func_8002ACFC_2B8FC(s32, s32, s16);

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

s32 func_8002A390_2AF90(Func8002A390Arg *arg0) {
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
            if (func_8002ACFC_2B8FC(arg0->matrix.translation.x, arg0->matrix.translation.z, arg0->unk2E) == 0) {
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

s32 func_8002ACFC_2B8FC(s32 arg0, s32 arg1, s16 arg2) {
    GameState *state;
    s16 angle;

    state = getCurrentAllocation();
    angle = func_8006D21C_6DE1C(state->unk3EC, state->unk3F0, arg0, arg1);

    if (arg2 - 0x238 < angle && angle < arg2 + 0x238) {
        if (distance_2d(state->unk3EC - arg0, state->unk3F0 - arg1) <= 0x280000) {
            return 1;
        }
    }
    return 0;
}

s16 func_8002ADB4_2B9B4(s16 arg0, s16 arg1) {
    s16 target;
    s16 current;
    s32 diff;
    s16 absDiff;
    s16 direction;

    target = arg0;
    current = arg1;

    if (arg0 == arg1) {
        goto done;
    }

    // Calculate absolute difference for wrap check
    diff = arg0 - arg1;
    diff = ABS(diff);

    // If difference >= half circle, need to wrap
    if ((s16)diff >= 0x1001) {
        if (arg1 < arg0) {
            current = arg1 + 0x2000;
        } else {
            target = arg0 + 0x2000;
        }
    }

    // Re-extract signed values
    arg0 = target;
    arg1 = current;

    // Calculate new difference after possible wrap
    diff = arg0 - arg1;
    absDiff = ABS(diff);

    // Large diff: fixed large step
    if (absDiff >= 0xAAB) {
        current += 0x1000;
        goto done;
    }

    // Calculate direction
    if (arg1 < arg0) {
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

s16 func_8002B4B8_2C0B8(s16 arg0, s16 arg1) {
    s16 diff;

    arg0 &= 0x1FFF;
    arg1 &= 0x1FFF;
    diff = (arg1 - arg0) & 0x1FFF;

    if (diff >= 0x1001) {
        diff |= 0xE000;
    }

    return diff;
}

#ifdef __sgi
#pragma reorder_frequency
#endif

void func_8002B4E4_2C0E4(Func8002A390Arg *arg0) {
    u16 val;

    if (arg0->unk5C == 4) {
        if (arg0->unk5D == 5) {
            arg0->unk4C = D_8008E768_8F368;
            val = arg0->unk4C[0];
            arg0->unk37 = 0;
            arg0->unk50 = val;
            arg0->unk4C += 1;
            func_80058220_58E20(0xBE, 1);
        }
    }
    if (arg0->unk5C == 3) {
        if (arg0->unk5D == 8) {
            arg0->unk4C = D_8008E75C_8F35C;
            val = arg0->unk4C[0];
            arg0->unk37 = 0;
            arg0->unk50 = val;
            arg0->unk4C += 1;
            func_80058220_58E20(0xAA, 1);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/2AF90", func_8002B598_2C198);

INCLUDE_ASM("asm/nonmatchings/2AF90", func_8002B760_2C360);

typedef struct {
    u8 pad0[0x14];
    s16 unk14;
    s16 unk16;
    u8 pad18[4];
    s16 unk1C;
    s16 unk1E;
} Func8002B94CArg;

void func_8002B94C_2C54C(Func8002B94CArg *arg0) {
    s32 target[3];
    s32 source[3];
    s32 zAdjust;
    s32 xAdjust;
    s32 arrayIndex;
    s32 nextIndex;
    s16 *arrayBase;
    s16 *arrayBase2;
    s32 *sourcePtr;
    s32 *targetPtr;
    s32 coord14;
    s32 coord1C;
    s32 readVal14;
    s32 readVal1C;
    s32 xScaled;
    s32 multiplier;

    arrayIndex = D_800AB06C_A23DC;
    sourcePtr = source;
    targetPtr = target;
    arrayBase = &D_8009F242_9FE42;
    coord14 = arg0->unk14;
    arrayBase[arrayIndex * 2] = coord14;
    arrayBase2 = arrayBase + 1;
    coord1C = arg0->unk1C;
    arrayBase2[arrayIndex * 2] = coord1C;

    memcpy(sourcePtr, &D_800AFF20_A7290, 12);
    memcpy(target, sourcePtr, 12);

    zAdjust = sourcePtr[2] >> 8;
    zAdjust = zAdjust * 8;
    zAdjust = zAdjust / 224;
    coord1C = zAdjust;
    targetPtr[0] += coord1C << 12;

    multiplier = -8;
    xScaled = sourcePtr[0] >> 8;
    xAdjust = xScaled * multiplier;
    if (xAdjust < 0) {
        xAdjust += 0x7FF;
    }
    xAdjust >>= 11;
    coord1C = xAdjust;
    target[2] += coord1C << 12;

    memcpy(&D_800AFF20_A7290, target, 12);

    *(s32 *)&arg0->unk14 = target[0];
    *(s32 *)&arg0->unk1C = target[2] - 0x440000;

    nextIndex = D_800AB06C_A23DC + 1;
    D_800AB06C_A23DC = nextIndex;

    if (target[2] > 0) {
        if ((u32)(target[0] - 0x100001) <= 0xFFFFE) {
            do {
                D_8009F240_9FE40 = nextIndex;
            } while (0);
            readVal14 = arg0->unk14;
            arrayBase[nextIndex * 2] = readVal14;
            readVal1C = arg0->unk1C;
            arrayBase2[nextIndex * 2] = readVal1C;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/2AF90", func_8002BAEC_2C6EC);
