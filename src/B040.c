#include "B040.h"
#include "90F0.h"
#include "common.h"
#include "geometry.h"
#include "task_scheduler.h"

typedef enum {
    ANIM_STATE_INIT,
    ANIM_STATE_RAMP_UP,
    ANIM_STATE_EASE_IN,
    ANIM_STATE_HOLD,
    ANIM_STATE_RAMP_OUT,
    ANIM_STATE_EASE_OUT
} AnimState;

#define FILL_STRUCT(v)       \
    (v)->unk0 = arg0;        \
    (v)->unk4 = arg1;        \
    (v)->unk6 = arg2;        \
    (v)->unk8 = arg3;        \
    (v)->unkA = arg6;        \
    (v)->unkB = 0;           \
    (v)->unk10 = arg4->unk0; \
    (v)->unk14 = arg4->unk4; \
    (v)->unk18 = arg4->unk8; \
    (v)->unkC = arg5;        \
    (v)->unk1C = arg9;       \
    (v)->unk1E = arg10;

typedef union {
    s16 unk70;
    u8 unk70_u8;
} func_8000B510_C110_arg_unk70;

typedef struct {
    u8 _pad0[0x6];
    s16 unk6;
    u8 _pad8[0x4];
    s32 unkC;
    u8 _pad10[0x10];
    SpriteAssetState unk20;
    s32 unk6C;
    func_8000B510_C110_arg_unk70 unk70;
} func_8000B510_C110_arg;

typedef struct {
    void *romStart;
    void *romEnd;
    s32 size;
} DmaEntry;

typedef struct {
    u8 _pad0[0x6];
    s16 unk6;
} ArrayElement_B934; // size 0x8

typedef struct {
    s32 unk0;
    s32 unk4;
} Table2DRow;

typedef void (*SchedulerFunc)(func_8000B510_C110_arg *);

extern u8 *D_8008C92C_8D52C;
void func_8000A854_B454(func_8000B510_C110_arg *arg0);
void func_8000A9A4_B5A4(func_8000B510_C110_arg *arg0);
void func_8000AD24_B924(func_8000B510_C110_arg *arg0);
void func_8000B044_BC44(func_8000B510_C110_arg *arg0);
void func_8000B1CC_BDCC(func_8000B510_C110_arg *arg0);

void func_8000B38C_BF8C(func_8000B510_C110_arg *arg0);
void func_8000B52C_C12C(func_8000B510_C110_arg *arg0);

void func_8000A440_B040(SceneModel *arg0, s16 arg1, s16 arg2, s16 arg3, void *arg4, s32 arg5, s8 arg6) {
    func_8000A49C_B09C(arg0, arg1, arg2, arg3, arg4, arg5, arg6, 0, 0, 0);
}

void func_8000A49C_B09C(
    SceneModel *arg0,
    s16 arg1,
    s16 arg2,
    s16 arg3,
    void *arg4,
    s32 arg5,
    s8 arg6,
    u8 arg7,
    u8 arg8,
    s16 arg9
) {
    u8 new_var;
    new_var = arg7;
    func_8000A510_B110(arg0, arg1, arg2, arg3, arg4, arg5, arg6, new_var, arg8, arg9, 0);
}

s32 func_8000A510_B110(
    SceneModel *arg0,
    s16 arg1,
    s16 arg2,
    s16 arg3,
    cutsceneEffectDisp_exec_arg *arg4,
    s32 arg5,
    s8 arg6,
    u8 arg7,
    u8 arg8,
    s16 arg9,
    s16 arg10
) {
    s32 i;
    u8 temp_v1;
    ScheduledTask *temp_v0;
    ScheduledTask *temp_v0_2;
    ScheduledTask *var_a0_2;

    temp_v1 = (D_8008C92C_8D52C + ((s32)(arg2 << 0x10) >> 0xD))[7];
    if (arg0 == NULL) {
        return 0;
    }

    switch (temp_v1) {
        case 0:
            var_a0_2 = scheduleTask(&func_8000A854_B454, arg7, arg8, 0);
            if (var_a0_2 == NULL) {
                break;
            }

            FILL_STRUCT(var_a0_2)
            return 1;
        case 1:
            temp_v0 = scheduleTask(&func_8000A9A4_B5A4, arg7, arg8, 0);
            if (temp_v0 == NULL) {
                break;
            }

            FILL_STRUCT(temp_v0)
            temp_v0->unk6C = 1;
            temp_v0->unk70 = 1;
            return 1;
        case 2:
            for (i = 0; i < 4; i++) {
                temp_v0_2 = scheduleTask(&func_8000AD24_B924, arg7, arg8, 0);
                if (temp_v0_2 == NULL) {
                    continue;
                }

                FILL_STRUCT(temp_v0_2)
                temp_v0_2->unk6C = i << 0xB;
                temp_v0_2->unk70 = 0;
            }
            return 1;
        case 3:
            var_a0_2 = scheduleTask(&func_8000B044_BC44, arg7, arg8, 0);
            if (var_a0_2 != NULL) {
                FILL_STRUCT(var_a0_2)
                var_a0_2->unk6C = 0;
                return 1;
            }
            // fallthrough
        case 4:
            var_a0_2 = scheduleTask(&func_8000B1CC_BDCC, arg7, arg8, 0);
            if (var_a0_2 == NULL) {
                break;
            }

            FILL_STRUCT(var_a0_2)
            var_a0_2->unk6C = 0;
            return 1;
        case 5:
            var_a0_2 = scheduleTask(&func_8000B38C_BF8C, arg7, arg8, 0);
            if (var_a0_2 != NULL) {
                FILL_STRUCT(var_a0_2)
                return 1;
            }
            // fallthrough
        case 6:
            var_a0_2 = scheduleTask(&func_8000B52C_C12C, arg7, arg8, 0);
            if (var_a0_2 == NULL) {
                break;
            }

            FILL_STRUCT(var_a0_2)
            return 1;
    }

    return 0;
}

typedef struct {
    void *unk0;
    u8 _pad4[0x14];
    u8 unk18[1];
} func_8000A834_B434_arg;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
} func_8000A834_B434_ret;

typedef struct {
    /* 0x00 */ void *unk0;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ u8 _pad6[2];
    /* 0x08 */ s16 unk8;
    /* 0x0A */ u8 unkA;
    /* 0x0B */ u8 _padB[1];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ void *unk20;
} func_8000A8B8_arg;

typedef struct {
    /* 0x00 */ func_8000A834_B434_arg *unk0;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ u8 _pad6[2];
    /* 0x08 */ s16 unk8;
    /* 0x0A */ u8 unkA;
    /* 0x0B */ s8 state;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ func_8000A410_B010_arg unk20;
    u8 padding[0x6C - (0x20 + sizeof(func_8000A410_B010_arg))];
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ s32 unk70;
    /* 0x74 */ s32 unk74;
    /* 0x78 */ s32 unk78;
} func_8000AA08_B608_arg;

void func_8000A8B8_B4B8(func_8000A8B8_arg *);
void func_8000A988_B588(func_8000B510_C110_arg *);
void func_8000AA08_B608(func_8000AA08_B608_arg *);
void func_8000AD08_B908(func_8000B510_C110_arg *);

func_8000A834_B434_ret *func_8000A834_B434(func_8000A834_B434_arg *arg0, s16 arg1) {
    if (arg1 != 0 && arg0->unk0 != NULL) {
        return arg0->unk0;
    }
    return (func_8000A834_B434_ret *)&arg0->unk18;
}

void func_8000A854_B454(func_8000B510_C110_arg *arg0) {
    loadSpriteAsset(&arg0->unk20, 0);
    func_80009F90_AB90(&arg0->unk20, 0x10000, arg0->unk6, -1);
    setCleanupCallback(func_8000A988_B588);
    setCallback(func_8000A8B8_B4B8);
}

void func_8000A8B8_B4B8(func_8000A8B8_arg *arg0) {
    func_8000A834_B434_ret *temp;
    void *ptr;
    s32 x, y, z;
    s16 a1;

    ptr = &arg0->unk20;

    if (arg0->unk8 == 0) {
        func_80069CF8_6A8F8();
        return;
    }

    if (func_8000A030_AC30(ptr, 0x10000) == 2) {
        func_80069CF8_6A8F8();
        return;
    }

    temp = func_8000A834_B434(arg0->unk0, arg0->unk1E);

    x = temp->unk14 + arg0->unk10;
    y = temp->unk18 + arg0->unk14;
    z = temp->unk1C + arg0->unk18;
    a1 = arg0->unk4;

    func_8000A13C_AD3C(ptr, a1, x, y, z, arg0->unkC, arg0->unkC, arg0->unk1C, arg0->unkA);

    if (arg0->unk8 > 0) {
        arg0->unk8 = arg0->unk8 - 1;
    }
}

void func_8000A988_B588(func_8000B510_C110_arg *arg0) {
    releaseNodeMemoryRef((void **)&arg0->unk20);
}

void func_8000A9A4_B5A4(func_8000B510_C110_arg *arg0) {
    loadSpriteAsset(&arg0->unk20, 0);
    func_80009F90_AB90(&arg0->unk20, 0x10000, arg0->unk6, -1);
    setCleanupCallback(func_8000AD08_B908);
    setCallback(func_8000AA08_B608);
}

void func_8000AA08_B608(func_8000AA08_B608_arg *arg0) {
    func_8000A834_B434_ret *temp;
    s32 result;
    s32 val;
    s32 temp_val;
    s32 temp_a0;
    s32 temp_v0;
    s32 temp_v1;
    void *ptr;
    s16 a1;
    s32 x;
    s32 y;
    s32 z;

    switch (arg0->state) {
        case ANIM_STATE_INIT:
            result = func_8000A410_B010(&arg0->unk20);
            if (result < 17) {
                arg0->unk78 = 0xCCCC;
                arg0->unk74 = (arg0->unkC >> 8) * 0xB3;
            } else if (result < 33) {
                arg0->unk78 = 0xB333;
                arg0->unk74 = (arg0->unkC >> 8) * 0x99;
            } else {
                arg0->unk78 = 0x8000;
                arg0->unk74 = (arg0->unkC >> 8) * 0x66;
            }
            arg0->state = ANIM_STATE_RAMP_UP;
            /* fallthrough */
        case ANIM_STATE_RAMP_UP:
            arg0->unk6C += arg0->unk74;
            if ((arg0->unkC * 3) / 2 < arg0->unk6C) {
                arg0->unk6C = (arg0->unkC * 3) / 2;
                arg0->state = ANIM_STATE_EASE_IN;
            }
            break;
        case ANIM_STATE_EASE_IN:
            val = ((arg0->unkC - arg0->unk6C) >> 8) * (arg0->unk78 >> 8);
            if (val != 0) {
                arg0->unk6C = arg0->unk6C + val;
            } else {
                arg0->unk6C = arg0->unkC;
                arg0->state = ANIM_STATE_HOLD;
            }
            break;
        case ANIM_STATE_HOLD:
            if (arg0->unk8 == 0) {
                arg0->state = ANIM_STATE_RAMP_OUT;
            }
            break;
        case ANIM_STATE_RAMP_OUT:
            arg0->unk6C = arg0->unk6C + arg0->unk74;
            temp_val = (arg0->unkC * 5) / 3;
            if (temp_val < arg0->unk6C) {
                arg0->unk6C = temp_val;
                arg0->state = ANIM_STATE_EASE_OUT;
            }
            break;
        case ANIM_STATE_EASE_OUT:
            val = (-arg0->unk6C >> 8) * (arg0->unk78 >> 8);
            if (val != 0) {
                arg0->unk6C = arg0->unk6C + val;
            } else {
                func_80069CF8_6A8F8();
                return;
            }
            break;
    }

    if (arg0->unk8 != 0) {
        temp_v1 = (arg0->unkC - arg0->unk70) / 4;
        if (temp_v1 != 0) {
            arg0->unk70 = arg0->unk70 + temp_v1;
        } else {
            arg0->unk70 = arg0->unkC;
        }
    } else {
        arg0->unk70 = arg0->unk70 + (arg0->unkC / 48);
    }

    ptr = &arg0->unk20;
    if (func_8000A030_AC30(ptr, 0x10000) == 2) {
        func_80069CF8_6A8F8();
        return;
    }

    temp = func_8000A834_B434(arg0->unk0, arg0->unk1E);

    a1 = arg0->unk4;
    x = temp->unk14 + arg0->unk10;
    y = temp->unk18 + arg0->unk14;
    z = temp->unk1C + arg0->unk18;

    func_8000A13C_AD3C(ptr, a1, x, y, z, arg0->unk6C, arg0->unk70, (s16)arg0->unk1C, arg0->unkA);

    if (arg0->unk8 > 0) {
        arg0->unk8--;
    }
}

void func_8000AD08_B908(func_8000B510_C110_arg *arg0) {
    releaseNodeMemoryRef((void **)&arg0->unk20);
}

void func_8000AD88_B988(void);
void func_8000B028_BC28(func_8000B510_C110_arg *);

void func_8000AD24_B924(func_8000B510_C110_arg *arg0) {
    loadSpriteAsset(&arg0->unk20, 0);
    func_80009F90_AB90(&arg0->unk20, 0x10000, arg0->unk6, -1);
    setCleanupCallback(func_8000B028_BC28);
    setCallback(func_8000AD88_B988);
}

INCLUDE_ASM("asm/nonmatchings/B040", func_8000AD88_B988);

void func_8000B028_BC28(func_8000B510_C110_arg *arg0) {
    releaseNodeMemoryRef((void **)&arg0->unk20);
}

typedef struct {
    /* 0x00 */ func_8000A834_B434_arg *unk0;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ u8 _pad6[4];
    /* 0x0A */ u8 unkA;
    /* 0x0B */ s8 unkB;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u16 unk1C;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ u8 unk20[0x4C];
    /* 0x6C */ s32 unk6C;
} func_8000B0A8_arg;

void func_8000B0A8_BCA8(func_8000B0A8_arg *);
void func_8000B1B0_BDB0(func_8000B510_C110_arg *);

void func_8000B044_BC44(func_8000B510_C110_arg *arg0) {
    loadSpriteAsset(&arg0->unk20, 0);
    func_80009F90_AB90(&arg0->unk20, 0x10000, arg0->unk6, -1);
    setCleanupCallback(func_8000B1B0_BDB0);
    setCallback(func_8000B0A8_BCA8);
}

void func_8000B0A8_BCA8(func_8000B0A8_arg *arg0) {
    func_8000A834_B434_ret *temp;
    void *ptr;
    s32 x, y, z;
    s16 a1;

    arg0->unk1C -= 0x155;

    switch (arg0->unkB) {
        case 0:
            arg0->unk6C += 0x4CCC;
            if (arg0->unk6C > 0x10000) {
                arg0->unkB = 1;
            }
            break;
        case 1: {
            s32 val = arg0->unk6C;
            s32 decrement = (-val >> 8) * 76;
            val += decrement;
            arg0->unk6C = val;
            if (val <= 0) {
                func_80069CF8_6A8F8();
                return;
            }
        } break;
    }

    ptr = &arg0->unk20;
    temp = func_8000A834_B434(arg0->unk0, arg0->unk1E);

    x = temp->unk14 + arg0->unk10;
    y = temp->unk18 + arg0->unk14;
    z = temp->unk1C + arg0->unk18;
    a1 = arg0->unk4;

    func_8000A13C_AD3C(ptr, a1, x, y, z, arg0->unk6C, arg0->unk6C, (s16)arg0->unk1C, arg0->unkA);
}

void func_8000B1B0_BDB0(func_8000B510_C110_arg *arg0) {
    releaseNodeMemoryRef((void **)&arg0->unk20);
}

typedef struct {
    /* 0x00 */ func_8000A834_B434_arg *unk0;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ u8 _pad6[2];
    /* 0x08 */ s16 unk8;
    /* 0x0A */ u8 unkA;
    /* 0x0B */ s8 unkB;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ u8 unk20[0x4C];
    /* 0x6C */ s32 unk6C;
} func_8000B230_arg;

void func_8000B230_BE30(func_8000B230_arg *);
void func_8000B370_BF70(func_8000B510_C110_arg *);

void func_8000B1CC_BDCC(func_8000B510_C110_arg *arg0) {
    loadSpriteAsset(&arg0->unk20, 0);
    func_80009F90_AB90(&arg0->unk20, 0x10000, arg0->unk6, -1);
    setCleanupCallback(func_8000B370_BF70);
    setCallback(func_8000B230_BE30);
}

void func_8000B230_BE30(func_8000B230_arg *arg0) {
    func_8000A834_B434_ret *temp;
    void *ptr;
    s32 x, y, z;
    s16 a1;

    switch (arg0->unkB) {
        case 0:
            arg0->unk6C -= 0x51EB;
            if (0x66666 < (arg0->unk6C < 0 ? -arg0->unk6C : arg0->unk6C)) {
                arg0->unkB = 1;
            }
            break;
        case 1: {
            s16 cnt = arg0->unk8;
            if (cnt == 0) {
                arg0->unkB = 2;
            } else {
                arg0->unk8 = cnt - 1;
            }
            break;
        }
        case 2: {
            s32 decrement = -arg0->unkC;
            s32 val = arg0->unkC;
            if (decrement < 0) {
                decrement += 3;
            }
            decrement >>= 2;
            val += decrement;
            arg0->unkC = val;
            if (decrement == 0) {
                func_80069CF8_6A8F8();
                return;
            }
        } break;
    }

    ptr = &arg0->unk20;
    temp = func_8000A834_B434(arg0->unk0, arg0->unk1E);

    x = temp->unk14 + arg0->unk10;
    y = temp->unk18 + arg0->unk14 + arg0->unk6C;
    z = temp->unk1C + arg0->unk18;
    a1 = arg0->unk4;

    func_8000A13C_AD3C(ptr, a1, x, y, z, arg0->unkC, arg0->unkC, arg0->unk1C, arg0->unkA);
}

void func_8000B370_BF70(func_8000B510_C110_arg *arg0) {
    releaseNodeMemoryRef((void **)&arg0->unk20);
}

typedef struct {
    /* 0x00 */ func_8000A834_B434_arg *unk0;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ u8 _pad6[2];
    /* 0x08 */ s16 unk8;
    /* 0x0A */ u8 unkA;
    /* 0x0B */ s8 unkB;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ s32 unk20[0x13];
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ u8 unk70;
} func_8000B400_C000_arg;

extern void func_8000A190_AD90(s32 *, s32, s32, s32, s32, s32, s32, s16, u8, u8);

void func_8000B400_C000(func_8000B400_C000_arg *);
void func_8000B510_C110(func_8000B510_C110_arg *);

void func_8000B38C_BF8C(func_8000B510_C110_arg *arg0) {
    loadSpriteAsset(&arg0->unk20, 0);
    func_80009F90_AB90(&arg0->unk20, 0x10000, arg0->unk6, -1);
    arg0->unkC = 0;
    arg0->unk6C = 0;
    arg0->unk70.unk70_u8 = 0xFF;
    setCleanupCallback(func_8000B510_C110);
    setCallback(func_8000B400_C000);
}

void func_8000B400_C000(func_8000B400_C000_arg *arg0) {
    func_8000A834_B434_ret *temp;
    s32 x, y, z;

    if (arg0->unk8 < 0) {
        func_80069CF8_6A8F8();
        return;
    }

    if (arg0->unk8 < 8) {
        u8 val = arg0->unk70;
        if (val >= 0x21) {
            arg0->unk70 = val - 0x20;
        }
    }

    arg0->unk8 = arg0->unk8 - 1;

    if (arg0->unkB == 0) {
        s32 newC;
        arg0->unk6C = arg0->unk6C - 0x6A7E;
        newC = arg0->unkC + 0xCCC;
        arg0->unkC = newC;
        if (newC > 0xFFFF) {
            arg0->unkC = 0x10000;
            arg0->unkB = 1;
        }
    }

    temp = func_8000A834_B434(arg0->unk0, arg0->unk1E);

    x = temp->unk14 + arg0->unk10;
    y = temp->unk18 + arg0->unk14 + arg0->unk6C;
    z = temp->unk1C + arg0->unk18;

    func_8000A190_AD90(
        (s32 *)&arg0->unk20,
        arg0->unk4,
        x,
        y,
        z,
        0x10000,
        arg0->unkC,
        arg0->unk1C,
        arg0->unkA,
        arg0->unk70
    );
}

void func_8000B510_C110(func_8000B510_C110_arg *arg0) {
    releaseNodeMemoryRef((void **)&arg0->unk20);
}

typedef struct {
    /* 0x00 */ func_8000A834_B434_arg *unk0;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ u8 _pad6[2];
    /* 0x08 */ s16 unk8;
    /* 0x0A */ u8 unkA;
    /* 0x0B */ u8 _padB[1];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ s32 unk20[0x13];
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ s16 unk70;
} func_8000B598_arg;

void func_8000B598_C198(func_8000B598_arg *);
void func_8000B684_C284(func_8000B510_C110_arg *);

void func_8000B52C_C12C(func_8000B510_C110_arg *arg0) {
    setCleanupCallback(func_8000B684_C284);
    loadSpriteAsset(&arg0->unk20, 0);
    func_80009F90_AB90(&arg0->unk20, 0x10000, arg0->unk6, -1);
    arg0->unk6C = 0;
    arg0->unk70.unk70 = 0;
    setCallback(func_8000B598_C198);
}

void func_8000B598_C198(func_8000B598_arg *arg0) {
    func_8000A834_B434_ret *temp;
    s32 x, y, z;
    s32 sinVal;

    if (arg0->unk8 < 0) {
        func_80069CF8_6A8F8();
        return;
    }

    arg0->unk8 = arg0->unk8 - 1;

    sinVal = approximateSin(arg0->unk70);

    arg0->unk6C = ((sinVal << 3) >> 8) * 655;

    arg0->unk70 = arg0->unk70 + 0x111;

    temp = func_8000A834_B434(arg0->unk0, arg0->unk1E);

    x = temp->unk14 + arg0->unk10;
    y = temp->unk18 + arg0->unk14 + arg0->unk6C;
    z = temp->unk1C + arg0->unk18;

    func_8000A190_AD90((s32 *)&arg0->unk20, arg0->unk4, x, y, z, 0x10000, arg0->unkC, arg0->unk1C, arg0->unkA, 0xFF);
}

void func_8000B684_C284(func_8000B510_C110_arg *arg0) {
    releaseNodeMemoryRef((void **)&arg0->unk20);
}

extern DmaEntry *D_8008CC2C_8D82C;
extern s32 D_8008CC30_8D830;

DmaEntry *func_8000B6A0_C2A0(void) {
    return D_8008CC2C_8D82C;
}

s32 func_8000B6AC_C2AC(void) {
    return D_8008CC30_8D830;
}

void *func_8000B6B8_C2B8(s16 arg0) {
    DmaEntry *entry;

    if (arg0 >= func_8000B6AC_C2AC()) {
        return 0;
    }
    entry = &func_8000B6A0_C2A0()[arg0];
    return loadCompressedData(entry->romStart, entry->romEnd, entry->size);
}

void *func_8000B714_C314(Table_B934 *arg0, s32 arg1, s32 arg2) {
    s32 count;
    Table2DRow *row;
    s32 *subarray;

    count = *(s32 *)(arg0->count_offset + (s32)arg0);
    if (arg1 >= count) {
        return NULL;
    }
    row = (Table2DRow *)(arg0->array_offset + arg1 * 8 + (s32)arg0);
    if (arg2 >= row->unk4) {
        return NULL;
    }
    subarray = (s32 *)(row->unk0 + (s32)arg0);
    return (void *)(subarray[arg2] + (s32)arg0);
}

s16 func_8000B770_C370(u16 *arg0) {
    s16 i = 0;
    s16 count = 0;
    u16 val;
    s16 maxCount;

    val = arg0[0];
    maxCount = 0;
    if (val != 0xFFFF) {
        do {
            val = arg0[i];
            if (val != 0xFFFC) {
                if (val == 0xFFFD) {
                    count = 0;
                } else {
                    count++;
                }
            } else {
                i++;
            }

            if (count > maxCount) {
                maxCount = count;
            }

            i++;
        } while (arg0[i] != 0xFFFF);
    }

    return maxCount;
}

s16 func_8000B7FC_C3FC(u16 *arg0) {
    s16 i;
    s16 count;
    s16 max;
    u16 val;
    u16 tmp;

    i = 0;
    count = 0;
    val = arg0[0];
    max = 0;
    if (val != 0xFFFF) {
        do {
            val = arg0[i];
            switch (val) {
                case 0xFFFC:
                    i++;
                    break;
                case 0xFFFB:
                    count += 4;
                    break;
                case 0xFFFD:
                    count = 0;
                    break;
                case 0xFFFE:
                    count += 4;
                    break;
                default:
                    tmp = arg0[i] >> 12;
                    if (tmp == 0) {
                        count += 0xC;
                    } else {
                        count += tmp;
                    }
                    break;
            }

            if (count > max) {
                max = count;
            }

            i++;
            val = arg0[i];
        } while (val != 0xFFFF);
    }

    return max;
}

s32 func_8000B8D8_C4D8(u16 *arg0) {
    s16 i = 0;
    s16 count = 0;

    if (arg0[0] != 0xFFFF) {
        do {
            if (arg0[i] == 0xFFFD) {
                count++;
            }
            i++;
        } while (arg0[i] != 0xFFFF);
    }

    return count;
}

s16 func_8000B934_C534(Table_B934 *arg0, s32 arg1) {
    s32 count;
    ArrayElement_B934 *elem;

    count = *(s32 *)(arg0->count_offset + (s32)arg0);
    if (arg1 >= count) {
        return 0;
    }
    elem = (ArrayElement_B934 *)(arg0->array_offset + arg1 * 8 + (s32)arg0);
    return elem->unk6;
}
