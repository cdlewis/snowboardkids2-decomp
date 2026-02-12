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

#define FILL_STRUCT(v)                \
    (v)->unk0 = arg0;                 \
    (v)->unk4 = arg1;                 \
    (v)->unk6 = arg2;                 \
    (v)->unk8 = arg3;                 \
    (v)->unkA = arg6;                 \
    (v)->unkB = 0;                    \
    (v)->unk10 = arg4->animDuration1; \
    (v)->unk14 = arg4->animDuration2; \
    (v)->unk18 = arg4->animDuration3; \
    (v)->unkC = arg5;                 \
    (v)->unk1C = arg9;                \
    (v)->unk1E = arg10;

typedef union {
    s16 unk70;
    u8 unk70_u8;
} SpriteEffectTaskState_unk70;

typedef struct {
    u8 _pad0[0x6];
    s16 unk6;
    u8 _pad8[0x4];
    s32 unkC;
    u8 _pad10[0x10];
    SpriteAssetState unk20;
    s32 unk6C;
    SpriteEffectTaskState_unk70 unk70;
} SpriteEffectTaskState;

typedef struct {
    void *romStart;
    void *romEnd;
    s32 size;
} DmaEntry;

typedef struct {
    s32 arrayOffset;
    s32 count;
} Table2DRow;

typedef void (*SchedulerFunc)(SpriteEffectTaskState *);

s32 D_8008C270_8CE70[] = { 0x00000000, 0x0000000A };
s32 D_8008C278_8CE78[] = { 0x00000000, 0x0001000A };
s32 D_8008C280_8CE80[] = { 0x00000000, 0x0002000A };
s32 D_8008C288_8CE88[] = { 0x00000000, 0x0003000A };
s32 D_8008C290_8CE90[] = { 0x00000000, 0x0004000A };
s32 D_8008C298_8CE98[] = { 0x00000000, 0x0005000A };
s32 D_8008C2A0_8CEA0[] = { 0x00000000, 0x0006000A };
s32 D_8008C2A8_8CEA8[] = { 0x00000000, 0x0007000A };
s32 D_8008C2B0_8CEB0[] = {
    0x00000000, 0x00080006, 0x00000000, 0x00090006, 0x00000000, 0x000A0006,
};
s32 D_8008C2C8_8CEC8[] = {
    0x00000000, 0x000B0008, 0x00000000, 0x000C0008, 0x00000000, 0x000D0008, 0x00000001, 0xFFFF0000,
};
s32 D_8008C2E8_8CEE8[] = { 0x00000000, 0x000E000A };
s32 D_8008C2F0_8CEF0[] = {
    0x00000000,
    0x000F0006,
    0x00000000,
    0x00100006,
};
s32 D_8008C300_8CF00[] = { 0x00000000, 0x0011000A };
s32 D_8008C308_8CF08[] = { 0x00000000, 0x0012000A };
s32 D_8008C310_8CF10[] = { 0x00000000, 0x0013000A };
s32 D_8008C318_8CF18[] = { 0x00000000, 0x0014000A };
s32 D_8008C320_8CF20[] = { 0x00000000, 0x0015000A };
s32 D_8008C328_8CF28[] = { 0x00000000, 0x0016000A };
s32 D_8008C330_8CF30[] = { 0x00000000, 0x0017000A };
s32 D_8008C338_8CF38[] = { 0x00000000, 0x0018000A };
s32 D_8008C340_8CF40[] = { 0x00000000, 0x0019000A };
s32 D_8008C348_8CF48[] = { 0x00000000, 0x001A000A };
s32 D_8008C350_8CF50[] = { 0x00000000, 0x001B000A };
s32 D_8008C358_8CF58[] = { 0x00000000, 0x001C000A };
s32 D_8008C360_8CF60[] = { 0x00000000, 0x001D000A };
s32 D_8008C368_8CF68[] = { 0x00000000, 0x001E000A };
s32 D_8008C370_8CF70[] = { 0x00000000, 0x001F000A };
s32 D_8008C378_8CF78[] = { 0x00000000, 0x0020000A };
s32 D_8008C380_8CF80[] = { 0x00000000, 0x0021000A };
s32 D_8008C388_8CF88[] = { 0x00000000, 0x0022000A };
s32 D_8008C390_8CF90[] = {
    0x00000000, 0x00230004, 0x00000000, 0x00240004, 0x00000000, 0x00250004, 0x00000000, 0x00260004,
};
s32 D_8008C3B0_8CFB0[] = {
    0x00000000, 0x0027000A, 0x00000000, 0x0028000A, 0x00000000, 0x0029000A, 0x00000001, 0xFFFF0002,
};
s32 D_8008C3D0_8CFD0[] = { 0x00000000, 0x002A000A };
s32 D_8008C3D8_8CFD8[] = { 0x00000000, 0x002B000A };
s32 D_8008C3E0_8CFE0[] = { 0x00000000, 0x002C000A };
s32 D_8008C3E8_8CFE8[] = { 0x00000000, 0x002D000A };
s32 D_8008C3F0_8CFF0[] = {
    0x00000000,
    0x002E0005,
    0x02010000,
    0x002E0005,
};
s32 D_8008C400_8D000[] = {
    0x00000000,
    0x002F000A,
    0x00000000,
    0x0030000A,
};
s32 D_8008C410_8D010[] = { 0x00000000, 0x0031000A };
s32 D_8008C418_8D018[] = {
    0x00000000,
    0x00320008,
    0x00000000,
    0x00330008,
};
s32 D_8008C428_8D028[] = { 0x00000000, 0x0034000A };
s32 D_8008C430_8D030[] = {
    0x00000000, 0x00350008, 0x00000000, 0x00360008, 0x00000000, 0x00370008,
};
s32 D_8008C448_8D048[] = { 0x00000000, 0x0038000A };
s32 D_8008C450_8D050[] = { 0x00000000, 0x0039000A };
s32 D_8008C458_8D058[] = { 0x00000000, 0x003A000A };
s32 D_8008C460_8D060[] = {
    0x00000000, 0x003B0004, 0x00000000, 0x003C0004, 0x00000000, 0x003D0006, 0x00000002, 0x00000000,
};
s32 D_8008C480_8D080[] = { 0x00000000, 0x003E000A };
s32 D_8008C488_8D088[] = { 0x00000000, 0x003F000A };
s32 D_8008C490_8D090[] = {
    0x00000000, 0x00400006, 0x00000000, 0x00410006, 0x00000000, 0x00420006,
};
s32 D_8008C4A8_8D0A8[] = {
    0x00000000,
    0x00430005,
    0x00000000,
    0x00440005,
};
s32 D_8008C4B8_8D0B8[] = {
    0x00000000,
    0x00450005,
    0x00000000,
    0x00460005,
};
s32 D_8008C4C8_8D0C8[] = { 0x00000000, 0x0047000A };
s32 D_8008C4D0_8D0D0[] = { 0x00000000, 0x0048000A };
s32 D_8008C4D8_8D0D8[] = { 0x00000000, 0x0049000A };
s32 D_8008C4E0_8D0E0[] = { 0x00000000, 0x004A000A };
s32 D_8008C4E8_8D0E8[] = {
    0x00000000,
    0x004B0005,
    0x00000000,
    0x004C0005,
};
s32 D_8008C4F8_8D0F8[] = {
    0x00000000, 0x004D0003, 0x00000000, 0x004E0003, 0x00000000,
    0x004F0003, 0x00000000, 0x00500003, 0x00000002, 0x00000000,
};
s32 D_8008C520_8D120[] = { 0x00000000, 0x0051000A };
s32 D_8008C528_8D128[] = {
    0x00000000,
    0x00520006,
    0x00000000,
    0x00530006,
};
s32 D_8008C538_8D138[] = { 0x00000000, 0x0054000A };
s32 D_8008C540_8D140[] = {
    0x00000000,
    0x00550004,
    0x00000000,
    0x00560004,
};
s32 D_8008C550_8D150[] = {
    0x00000000,
    0x00570006,
    0x00000000,
    0x00580006,
};
s32 D_8008C560_8D160[] = {
    0x00000000,
    0x00590008,
    0x00000000,
    0x005A0008,
};
s32 D_8008C570_8D170[] = {
    0x00000000,
    0x005B0008,
    0x00000000,
    0x005C0008,
};
s32 D_8008C580_8D180[] = {
    0x00000000, 0x005D0006, 0x00000000, 0x005E0006, 0x00000000, 0x005F0006,
};
s32 D_8008C598_8D198[] = { 0x00000000, 0x0060000A };
s32 D_8008C5A0_8D1A0[] = {
    0x00000000, 0x00610006, 0x00000000, 0x00620006, 0x00000000, 0x00630006, 0x00000001, 0xFFFF0001,
};

s32 D_8008C5C0_8D1C0[] = {
    (s32)D_8008C270_8CE70, 0x00010001, (s32)D_8008C278_8CE78, 0x00010001, (s32)D_8008C280_8CE80, 0x00010001,
    (s32)D_8008C288_8CE88, 0x00010001, (s32)D_8008C290_8CE90, 0x00010001, (s32)D_8008C298_8CE98, 0x00010001,
    (s32)D_8008C2A0_8CEA0, 0x00010001, (s32)D_8008C2A8_8CEA8, 0x00010001, (s32)D_8008C2B0_8CEB0, 0x00030001,
    (s32)D_8008C2C8_8CEC8, 0x00040001, (s32)D_8008C2E8_8CEE8, 0x00010001, (s32)D_8008C2F0_8CEF0, 0x00020001,
    (s32)D_8008C300_8CF00, 0x00010001, (s32)D_8008C308_8CF08, 0x00010001, (s32)D_8008C310_8CF10, 0x00010001,
    (s32)D_8008C318_8CF18, 0x00010001, (s32)D_8008C320_8CF20, 0x00010001, (s32)D_8008C328_8CF28, 0x00010000,
    (s32)D_8008C330_8CF30, 0x00010000, (s32)D_8008C338_8CF38, 0x00010000, (s32)D_8008C340_8CF40, 0x00010001,
    (s32)D_8008C348_8CF48, 0x00010001, (s32)D_8008C350_8CF50, 0x00010000, (s32)D_8008C358_8CF58, 0x00010001,
    (s32)D_8008C360_8CF60, 0x00010001, (s32)D_8008C368_8CF68, 0x00010001, (s32)D_8008C370_8CF70, 0x00010000,
    (s32)D_8008C378_8CF78, 0x00010001, (s32)D_8008C380_8CF80, 0x00010001, (s32)D_8008C388_8CF88, 0x00010000,
    (s32)D_8008C390_8CF90, 0x00040001, (s32)D_8008C3B0_8CFB0, 0x00040001, (s32)D_8008C3D0_8CFD0, 0x00010000,
    (s32)D_8008C3D8_8CFD8, 0x00010000, (s32)D_8008C3E0_8CFE0, 0x00010001, (s32)D_8008C3E8_8CFE8, 0x00010000,
    (s32)D_8008C3F0_8CFF0, 0x00020000, (s32)D_8008C400_8D000, 0x00020000, (s32)D_8008C410_8D010, 0x00010000,
    (s32)D_8008C418_8D018, 0x00020000, (s32)D_8008C428_8D028, 0x00010000, (s32)D_8008C430_8D030, 0x00030000,
    (s32)D_8008C448_8D048, 0x00010001, (s32)D_8008C450_8D050, 0x00010001, (s32)D_8008C458_8D058, 0x00010000,
    (s32)D_8008C460_8D060, 0x00040000, (s32)D_8008C480_8D080, 0x00010000, (s32)D_8008C488_8D088, 0x00010002,
    (s32)D_8008C490_8D090, 0x00030000, (s32)D_8008C4A8_8D0A8, 0x00020000, (s32)D_8008C4B8_8D0B8, 0x00020000,
    (s32)D_8008C4C8_8D0C8, 0x00010003, (s32)D_8008C4D0_8D0D0, 0x00010000, (s32)D_8008C4D8_8D0D8, 0x00010004,
    (s32)D_8008C4E0_8D0E0, 0x00010000, (s32)D_8008C4E8_8D0E8, 0x00020000, (s32)D_8008C4F8_8D0F8, 0x00050000,
    (s32)D_8008C520_8D120, 0x00010005, (s32)D_8008C528_8D128, 0x00020000, (s32)D_8008C538_8D138, 0x00010006,
    (s32)D_8008C540_8D140, 0x00020000, (s32)D_8008C550_8D150, 0x00020001, (s32)D_8008C560_8D160, 0x00020000,
    (s32)D_8008C570_8D170, 0x00020000, (s32)D_8008C580_8D180, 0x00030001, (s32)D_8008C598_8D198, 0x00010001,
    (s32)D_8008C5A0_8D1A0, 0x00040001,
};

s32 D_8008C7D8_8D3D8[] = {
    0x00000000, 0x0000000C, 0x00000000, 0x0001000C, 0x00000000, 0x0002000C, (s32)D_8008C7D8_8D3D8, 0x00030000,
};

s32 D_8008C7F8_8D3F8[] = {
    0x00000000, 0x00000003, 0x00000000, 0x00010003, 0x00000000, 0x00020003,
};

s32 D_8008C810_8D410[] = {
    0x00000000,
    0x00030003,
    0x00010000,
    0x00040003,
    0x00020000,
    0x00050003,
    0x00030000,
    0x00060003,
    (s32)D_8008C7F8_8D3F8,
    0x00030000,
    (s32)D_8008C810_8D410,
    0x00040000,
};

s32 D_8008C840_8D440[] = {
    0x00000000, 0x00000004, 0x00000000, 0x00010004, 0x00000000, 0x00020004, (s32)D_8008C840_8D440, 0x00030000,
};

s32 D_8008C860_8D460[] = {
    0x00000000,
    0x00020002,
    0x00000000,
    0x00030002,
    0x00000000,
    0x00040002,
    0x00000000,
    0x00050002,
    0x00000000,
    0x00060002,
    (s32)D_8008C860_8D460,
    0x00050000,
};

s32 D_8008C890_8D490[] = {
    0x00000000, 0x00000003, 0x00000000,
    0x00010003, 0x00000000, 0x00020003,
    0x00000000, 0x00030003, (s32)D_8008C890_8D490,
    0x00040000,
};

s32 D_8008C8B8_8D4B8[] = {
    0x00000000,
    0x00000004,
    0x00000000,
    0x00010004,
    0x00000000,
    0x00020004,
    0x00000000,
    0x00030004,
    0x00000000,
    0x00040004,
    (s32)D_8008C8B8_8D4B8,
    0x00050000,
};

s32 D_8008C8E8_8D4E8[] = {
    0x00000000, 0x00000004, (s32)D_8008C8E8_8D4E8, 0x00010000, (s32)D_8008C8E8_8D4E8, 0x00010000,
};

s32 D_8008C900_8D500[] = { 0x00000000, 0x00010003 };

s32 D_8008C908_8D508[] = {
    0x00000000, 0x00000003, (s32)D_8008C900_8D500, 0x00010000, (s32)D_8008C908_8D508, 0x00010000,
};

s32 D_8008C920_8D520[] = { 0x00648EB0, 0x0064EFD0, 0x000147C8 };

u8 *D_8008C92C_8D52C = (u8 *)D_8008C5C0_8D1C0;

void initSimpleSpriteEffect(SpriteEffectTaskState *arg0);
void initScalingSpriteEffect(SpriteEffectTaskState *arg0);
void initRadialBurstSpriteEffect(SpriteEffectTaskState *arg0);
void initSpinFadeSpriteEffect(SpriteEffectTaskState *arg0);
void initDropShrinkSpriteEffect(SpriteEffectTaskState *arg0);

void initRiseStretchSpriteEffect(SpriteEffectTaskState *arg0);
void initFloatBobbingSpriteEffect(SpriteEffectTaskState *arg0);

void spawnSpriteEffect(SceneModel *arg0, s16 arg1, s16 arg2, s16 arg3, void *arg4, s32 arg5, s8 arg6) {
    spawnSpriteEffectEx(arg0, arg1, arg2, arg3, arg4, arg5, arg6, 0, 0, 0);
}

void spawnSpriteEffectEx(
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
    spawnSpriteEffectInternal(arg0, arg1, arg2, arg3, arg4, arg5, arg6, new_var, arg8, arg9, 0);
}

s32 spawnSpriteEffectInternal(
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
            var_a0_2 = scheduleTask(&initSimpleSpriteEffect, arg7, arg8, 0);
            if (var_a0_2 == NULL) {
                break;
            }

            FILL_STRUCT(var_a0_2)
            return 1;
        case 1:
            temp_v0 = scheduleTask(&initScalingSpriteEffect, arg7, arg8, 0);
            if (temp_v0 == NULL) {
                break;
            }

            FILL_STRUCT(temp_v0)
            temp_v0->unk6C = 1;
            temp_v0->unk70 = 1;
            return 1;
        case 2:
            for (i = 0; i < 4; i++) {
                temp_v0_2 = scheduleTask(&initRadialBurstSpriteEffect, arg7, arg8, 0);
                if (temp_v0_2 == NULL) {
                    continue;
                }

                FILL_STRUCT(temp_v0_2)
                temp_v0_2->unk6C = i << 0xB;
                temp_v0_2->unk70 = 0;
            }
            return 1;
        case 3:
            var_a0_2 = scheduleTask(&initSpinFadeSpriteEffect, arg7, arg8, 0);
            if (var_a0_2 != NULL) {
                FILL_STRUCT(var_a0_2)
                var_a0_2->unk6C = 0;
                return 1;
            }
            // fallthrough
        case 4:
            var_a0_2 = scheduleTask(&initDropShrinkSpriteEffect, arg7, arg8, 0);
            if (var_a0_2 == NULL) {
                break;
            }

            FILL_STRUCT(var_a0_2)
            var_a0_2->unk6C = 0;
            return 1;
        case 5:
            var_a0_2 = scheduleTask(&initRiseStretchSpriteEffect, arg7, arg8, 0);
            if (var_a0_2 != NULL) {
                FILL_STRUCT(var_a0_2)
                return 1;
            }
            // fallthrough
        case 6:
            var_a0_2 = scheduleTask(&initFloatBobbingSpriteEffect, arg7, arg8, 0);
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
} SpriteEffectPositionSource;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
} SpriteEffectPosition;

typedef struct {
    /* 0x00 */ void *positionSource;
    /* 0x04 */ s16 layer;
    /* 0x06 */ u8 _pad6[2];
    /* 0x08 */ s16 duration;
    /* 0x0A */ u8 opacity;
    /* 0x0B */ u8 _padB[1];
    /* 0x0C */ s32 scale;
    /* 0x10 */ s32 offsetX;
    /* 0x14 */ s32 offsetY;
    /* 0x18 */ s32 offsetZ;
    /* 0x1C */ s16 rotation;
    /* 0x1E */ s16 useParentPos;
    /* 0x20 */ void *spriteState;
} SimpleSpriteEffectState;

typedef struct {
    /* 0x00 */ SpriteEffectPositionSource *positionSource;
    /* 0x04 */ s16 layer;
    /* 0x06 */ u8 _pad6[2];
    /* 0x08 */ s16 duration;
    /* 0x0A */ u8 opacity;
    /* 0x0B */ s8 state;
    /* 0x0C */ s32 targetScale;
    /* 0x10 */ s32 offsetX;
    /* 0x14 */ s32 offsetY;
    /* 0x18 */ s32 offsetZ;
    /* 0x1C */ s16 rotation;
    /* 0x1E */ s16 useParentPos;
    /* 0x20 */ TableLookupContext spriteState;
    u8 padding[0x6C - (0x20 + sizeof(TableLookupContext))];
    /* 0x6C */ s32 scaleX;
    /* 0x70 */ s32 scaleY;
    /* 0x74 */ s32 rampSpeed;
    /* 0x78 */ s32 easingFactor;
} ScalingSpriteEffectState;

void updateSimpleSpriteEffect(SimpleSpriteEffectState *);
void cleanupSimpleSpriteEffect(SpriteEffectTaskState *);
void updateScalingSpriteEffect(ScalingSpriteEffectState *);
void cleanupScalingSpriteEffect(SpriteEffectTaskState *);

SpriteEffectPosition *getSpriteEffectPosition(SpriteEffectPositionSource *source, s16 useParent) {
    if (useParent != 0 && source->unk0 != NULL) {
        return source->unk0;
    }
    return (SpriteEffectPosition *)&source->unk18;
}

void initSimpleSpriteEffect(SpriteEffectTaskState *arg0) {
    loadSpriteAsset(&arg0->unk20, 0);
    setSpriteAnimation(&arg0->unk20, 0x10000, arg0->unk6, -1);
    setCleanupCallback(cleanupSimpleSpriteEffect);
    setCallback(updateSimpleSpriteEffect);
}

void updateSimpleSpriteEffect(SimpleSpriteEffectState *arg0) {
    SpriteEffectPosition *pos;
    void *spriteState;
    s32 x, y, z;
    s16 layer;

    spriteState = &arg0->spriteState;

    if (arg0->duration == 0) {
        terminateCurrentTask();
        return;
    }

    if (updateSpriteAnimation(spriteState, 0x10000) == 2) {
        terminateCurrentTask();
        return;
    }

    pos = getSpriteEffectPosition(arg0->positionSource, arg0->useParentPos);

    x = pos->unk14 + arg0->offsetX;
    y = pos->unk18 + arg0->offsetY;
    z = pos->unk1C + arg0->offsetZ;
    layer = arg0->layer;

    renderOpaqueSprite(spriteState, layer, x, y, z, arg0->scale, arg0->scale, arg0->rotation, arg0->opacity);

    if (arg0->duration > 0) {
        arg0->duration = arg0->duration - 1;
    }
}

void cleanupSimpleSpriteEffect(SpriteEffectTaskState *arg0) {
    releaseNodeMemoryRef((void **)&arg0->unk20);
}

void initScalingSpriteEffect(SpriteEffectTaskState *arg0) {
    loadSpriteAsset(&arg0->unk20, 0);
    setSpriteAnimation(&arg0->unk20, 0x10000, arg0->unk6, -1);
    setCleanupCallback(cleanupScalingSpriteEffect);
    setCallback(updateScalingSpriteEffect);
}

void updateScalingSpriteEffect(ScalingSpriteEffectState *arg0) {
    SpriteEffectPosition *pos;
    s32 result;
    s32 val;
    s32 temp_val;
    s32 temp_a0;
    s32 temp_v0;
    s32 temp_v1;
    void *spriteState;
    s16 layer;
    s32 x;
    s32 y;
    s32 z;

    switch (arg0->state) {
        case ANIM_STATE_INIT:
            result = getTableEntryValue(&arg0->spriteState);
            if (result < 17) {
                arg0->easingFactor = 0xCCCC;
                arg0->rampSpeed = (arg0->targetScale >> 8) * 0xB3;
            } else if (result < 33) {
                arg0->easingFactor = 0xB333;
                arg0->rampSpeed = (arg0->targetScale >> 8) * 0x99;
            } else {
                arg0->easingFactor = 0x8000;
                arg0->rampSpeed = (arg0->targetScale >> 8) * 0x66;
            }
            arg0->state = ANIM_STATE_RAMP_UP;
            /* fallthrough */
        case ANIM_STATE_RAMP_UP:
            arg0->scaleX += arg0->rampSpeed;
            if ((arg0->targetScale * 3) / 2 < arg0->scaleX) {
                arg0->scaleX = (arg0->targetScale * 3) / 2;
                arg0->state = ANIM_STATE_EASE_IN;
            }
            break;
        case ANIM_STATE_EASE_IN:
            val = ((arg0->targetScale - arg0->scaleX) >> 8) * (arg0->easingFactor >> 8);
            if (val != 0) {
                arg0->scaleX = arg0->scaleX + val;
            } else {
                arg0->scaleX = arg0->targetScale;
                arg0->state = ANIM_STATE_HOLD;
            }
            break;
        case ANIM_STATE_HOLD:
            if (arg0->duration == 0) {
                arg0->state = ANIM_STATE_RAMP_OUT;
            }
            break;
        case ANIM_STATE_RAMP_OUT:
            arg0->scaleX = arg0->scaleX + arg0->rampSpeed;
            temp_val = (arg0->targetScale * 5) / 3;
            if (temp_val < arg0->scaleX) {
                arg0->scaleX = temp_val;
                arg0->state = ANIM_STATE_EASE_OUT;
            }
            break;
        case ANIM_STATE_EASE_OUT:
            val = (-arg0->scaleX >> 8) * (arg0->easingFactor >> 8);
            if (val != 0) {
                arg0->scaleX = arg0->scaleX + val;
            } else {
                terminateCurrentTask();
                return;
            }
            break;
    }

    if (arg0->duration != 0) {
        temp_v1 = (arg0->targetScale - arg0->scaleY) / 4;
        if (temp_v1 != 0) {
            arg0->scaleY = arg0->scaleY + temp_v1;
        } else {
            arg0->scaleY = arg0->targetScale;
        }
    } else {
        arg0->scaleY = arg0->scaleY + (arg0->targetScale / 48);
    }

    spriteState = &arg0->spriteState;
    if (updateSpriteAnimation(spriteState, 0x10000) == 2) {
        terminateCurrentTask();
        return;
    }

    pos = getSpriteEffectPosition(arg0->positionSource, arg0->useParentPos);

    layer = arg0->layer;
    x = pos->unk14 + arg0->offsetX;
    y = pos->unk18 + arg0->offsetY;
    z = pos->unk1C + arg0->offsetZ;

    renderOpaqueSprite(spriteState, layer, x, y, z, arg0->scaleX, arg0->scaleY, arg0->rotation, arg0->opacity);

    if (arg0->duration > 0) {
        arg0->duration--;
    }
}

void cleanupScalingSpriteEffect(SpriteEffectTaskState *arg0) {
    releaseNodeMemoryRef((void **)&arg0->unk20);
}

typedef struct {
    /* 0x00 */ SpriteEffectPositionSource *positionSource;
    /* 0x04 */ s16 layer;
    /* 0x06 */ u8 _pad6[2];
    /* 0x08 */ s16 duration;
    /* 0x0A */ u8 opacity;
    /* 0x0B */ s8 state;
    /* 0x0C */ s32 targetScale;
    /* 0x10 */ s32 offsetX;
    /* 0x14 */ s32 offsetY;
    /* 0x18 */ s32 offsetZ;
    /* 0x1C */ s16 rotation;
    /* 0x1E */ s16 useParentPos;
    /* 0x20 */ TableLookupContext spriteState;
    u8 unk32[0x6C - (0x20 + sizeof(TableLookupContext))];
    /* 0x6C */ s16 bobPhaseHi;
    /* 0x6E */ s16 bobPhaseLo;
    /* 0x70 */ s32 currentScale;
} RadialBurstSpriteEffectState;

void updateRadialBurstSpriteEffect(RadialBurstSpriteEffectState *arg0);
void cleanupRadialBurstSpriteEffect(SpriteEffectTaskState *);

void initRadialBurstSpriteEffect(SpriteEffectTaskState *arg0) {
    loadSpriteAsset(&arg0->unk20, 0);
    setSpriteAnimation(&arg0->unk20, 0x10000, arg0->unk6, -1);
    setCleanupCallback(cleanupRadialBurstSpriteEffect);
    setCallback(updateRadialBurstSpriteEffect);
}

void updateRadialBurstSpriteEffect(RadialBurstSpriteEffectState *arg0) {
    SpriteEffectPosition *pos;
    s32 scale;
    s32 cosResult;
    s32 targetScale;
    s32 yOffsetFactor;
    s32 xOffset;
    s32 yOffset;
    s32 baseX;
    s32 baseY;
    s32 baseZ;
    s32 currentScale;
    s32 decrement;
    s32 zOffset;
    SpriteEffectPosition *temp_pos;
    s32 phase;

    scale = arg0->targetScale;
    targetScale = ((scale >> 8) * 273) * 9;
    yOffsetFactor = (scale >> 8) * 0xF5;

    if (updateSpriteAnimation(&arg0->spriteState, 0x10000) == 2) {
        terminateCurrentTask();
        return;
    }

    temp_pos = getSpriteEffectPosition(arg0->positionSource, arg0->useParentPos);
    pos = temp_pos;
    {
        s32 posX = pos->unk14;
        baseX = posX + arg0->offsetX;
        baseY = pos->unk18 + arg0->offsetY;
        baseZ = pos->unk1C + arg0->offsetZ;
        phase = (*(s32 *)&arg0->bobPhaseHi - 0xAA) & 0x1FFF;
        *(s32 *)&arg0->bobPhaseHi = phase;
        xOffset = (arg0->currentScale >> 8) * ((approximateSin(phase) << 3) >> 8);
    }

    yOffset = (yOffsetFactor >> 8) * ((approximateSin((s16)(*(s32 *)&arg0->bobPhaseHi * 3)) << 3) >> 8);
    cosResult = approximateCos(arg0->bobPhaseLo) << 3;
    currentScale = arg0->currentScale;
    zOffset = (currentScale >> 8) * (cosResult >> 8);

    switch (arg0->state) {
        case 0: {
            s32 diff = targetScale - currentScale;
            if (diff < 0) {
                diff += 3;
            }
            diff >>= 2;
            decrement = diff;
            if (decrement != 0) {
                arg0->currentScale = currentScale + decrement;
            } else {
                arg0->currentScale = targetScale;
                arg0->state = 1;
            }
            break;
        }
        case 1:
            if (arg0->duration == 0) {
                arg0->state = 2;
            }
            break;
        case 2:
            decrement = -targetScale / 10;
            yOffset = 0;
            if (currentScale <= 0) {
                terminateCurrentTask();
                return;
            }
            arg0->currentScale = currentScale + decrement;
            scale = (arg0->targetScale * (currentScale >> 8)) / (targetScale >> 8);
            break;
    }

    renderOpaqueSprite(
        &arg0->spriteState,
        arg0->layer,
        baseX + xOffset,
        baseY + yOffset,
        baseZ + zOffset,
        scale,
        scale,
        arg0->rotation,
        arg0->opacity
    );

    if (arg0->duration > 0) {
        arg0->duration = arg0->duration - 1;
    }
}

void cleanupRadialBurstSpriteEffect(SpriteEffectTaskState *arg0) {
    releaseNodeMemoryRef((void **)&arg0->unk20);
}

typedef struct {
    /* 0x00 */ SpriteEffectPositionSource *positionSource;
    /* 0x04 */ s16 layer;
    /* 0x06 */ u8 _pad6[4];
    /* 0x0A */ u8 opacity;
    /* 0x0B */ s8 state;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 offsetX;
    /* 0x14 */ s32 offsetY;
    /* 0x18 */ s32 offsetZ;
    /* 0x1C */ u16 rotation;
    /* 0x1E */ s16 useParentPos;
    /* 0x20 */ u8 spriteState[0x4C];
    /* 0x6C */ s32 scale;
} SpinFadeSpriteEffectState;

void updateSpinFadeSpriteEffect(SpinFadeSpriteEffectState *);
void cleanupSpinFadeSpriteEffect(SpriteEffectTaskState *);

void initSpinFadeSpriteEffect(SpriteEffectTaskState *arg0) {
    loadSpriteAsset(&arg0->unk20, 0);
    setSpriteAnimation(&arg0->unk20, 0x10000, arg0->unk6, -1);
    setCleanupCallback(cleanupSpinFadeSpriteEffect);
    setCallback(updateSpinFadeSpriteEffect);
}

void updateSpinFadeSpriteEffect(SpinFadeSpriteEffectState *arg0) {
    SpriteEffectPosition *pos;
    void *spriteState;
    s32 x, y, z;
    s16 layer;

    arg0->rotation -= 0x155;

    switch (arg0->state) {
        case 0:
            arg0->scale += 0x4CCC;
            if (arg0->scale > 0x10000) {
                arg0->state = 1;
            }
            break;
        case 1: {
            s32 val = arg0->scale;
            s32 decrement = (-val >> 8) * 76;
            val += decrement;
            arg0->scale = val;
            if (val <= 0) {
                terminateCurrentTask();
                return;
            }
        } break;
    }

    spriteState = &arg0->spriteState;
    pos = getSpriteEffectPosition(arg0->positionSource, arg0->useParentPos);

    x = pos->unk14 + arg0->offsetX;
    y = pos->unk18 + arg0->offsetY;
    z = pos->unk1C + arg0->offsetZ;
    layer = arg0->layer;

    renderOpaqueSprite(spriteState, layer, x, y, z, arg0->scale, arg0->scale, (s16)arg0->rotation, arg0->opacity);
}

void cleanupSpinFadeSpriteEffect(SpriteEffectTaskState *arg0) {
    releaseNodeMemoryRef((void **)&arg0->unk20);
}

typedef struct {
    /* 0x00 */ SpriteEffectPositionSource *positionSource;
    /* 0x04 */ s16 layer;
    /* 0x06 */ u8 _pad6[2];
    /* 0x08 */ s16 duration;
    /* 0x0A */ u8 opacity;
    /* 0x0B */ s8 state;
    /* 0x0C */ s32 scale;
    /* 0x10 */ s32 offsetX;
    /* 0x14 */ s32 offsetY;
    /* 0x18 */ s32 offsetZ;
    /* 0x1C */ s16 rotation;
    /* 0x1E */ s16 useParentPos;
    /* 0x20 */ u8 spriteState[0x4C];
    /* 0x6C */ s32 dropOffset;
} DropShrinkSpriteEffectState;

void updateDropShrinkSpriteEffect(DropShrinkSpriteEffectState *);
void cleanupDropShrinkSpriteEffect(SpriteEffectTaskState *);

void initDropShrinkSpriteEffect(SpriteEffectTaskState *arg0) {
    loadSpriteAsset(&arg0->unk20, 0);
    setSpriteAnimation(&arg0->unk20, 0x10000, arg0->unk6, -1);
    setCleanupCallback(cleanupDropShrinkSpriteEffect);
    setCallback(updateDropShrinkSpriteEffect);
}

void updateDropShrinkSpriteEffect(DropShrinkSpriteEffectState *arg0) {
    SpriteEffectPosition *pos;
    void *spriteState;
    s32 x, y, z;
    s16 layer;

    switch (arg0->state) {
        case 0:
            arg0->dropOffset -= 0x51EB;
            if (0x66666 < (arg0->dropOffset < 0 ? -arg0->dropOffset : arg0->dropOffset)) {
                arg0->state = 1;
            }
            break;
        case 1: {
            s16 cnt = arg0->duration;
            if (cnt == 0) {
                arg0->state = 2;
            } else {
                arg0->duration = cnt - 1;
            }
            break;
        }
        case 2: {
            s32 decrement = -arg0->scale;
            s32 val = arg0->scale;
            if (decrement < 0) {
                decrement += 3;
            }
            decrement >>= 2;
            val += decrement;
            arg0->scale = val;
            if (decrement == 0) {
                terminateCurrentTask();
                return;
            }
        } break;
    }

    spriteState = &arg0->spriteState;
    pos = getSpriteEffectPosition(arg0->positionSource, arg0->useParentPos);

    x = pos->unk14 + arg0->offsetX;
    y = pos->unk18 + arg0->offsetY + arg0->dropOffset;
    z = pos->unk1C + arg0->offsetZ;
    layer = arg0->layer;

    renderOpaqueSprite(spriteState, layer, x, y, z, arg0->scale, arg0->scale, arg0->rotation, arg0->opacity);
}

void cleanupDropShrinkSpriteEffect(SpriteEffectTaskState *arg0) {
    releaseNodeMemoryRef((void **)&arg0->unk20);
}

typedef struct {
    /* 0x00 */ SpriteEffectPositionSource *unk0;
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
} RiseStretchSpriteEffectState;

void updateRiseStretchSpriteEffect(RiseStretchSpriteEffectState *);
void cleanupRiseStretchSpriteEffect(SpriteEffectTaskState *);

void initRiseStretchSpriteEffect(SpriteEffectTaskState *arg0) {
    loadSpriteAsset(&arg0->unk20, 0);
    setSpriteAnimation(&arg0->unk20, 0x10000, arg0->unk6, -1);
    arg0->unkC = 0;
    arg0->unk6C = 0;
    arg0->unk70.unk70_u8 = 0xFF;
    setCleanupCallback(cleanupRiseStretchSpriteEffect);
    setCallback(updateRiseStretchSpriteEffect);
}

void updateRiseStretchSpriteEffect(RiseStretchSpriteEffectState *arg0) {
    SpriteEffectPosition *temp;
    s32 x, y, z;

    if (arg0->unk8 < 0) {
        terminateCurrentTask();
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

    temp = getSpriteEffectPosition(arg0->unk0, arg0->unk1E);

    x = temp->unk14 + arg0->unk10;
    y = temp->unk18 + arg0->unk14 + arg0->unk6C;
    z = temp->unk1C + arg0->unk18;

    renderSprite(&arg0->unk20, arg0->unk4, x, y, z, 0x10000, arg0->unkC, arg0->unk1C, arg0->unkA, arg0->unk70);
}

void cleanupRiseStretchSpriteEffect(SpriteEffectTaskState *arg0) {
    releaseNodeMemoryRef((void **)&arg0->unk20);
}

typedef struct {
    /* 0x00 */ SpriteEffectPositionSource *positionSource;
    /* 0x04 */ s16 layer;
    /* 0x06 */ u8 _pad6[2];
    /* 0x08 */ s16 duration;
    /* 0x0A */ u8 opacity;
    /* 0x0B */ u8 _padB[1];
    /* 0x0C */ s32 scale;
    /* 0x10 */ s32 offsetX;
    /* 0x14 */ s32 offsetY;
    /* 0x18 */ s32 offsetZ;
    /* 0x1C */ s16 rotation;
    /* 0x1E */ s16 useParentPos;
    /* 0x20 */ s32 spriteState[0x13];
    /* 0x6C */ s32 bobOffset;
    /* 0x70 */ s16 bobPhase;
} FloatBobbingSpriteEffectState;

void updateFloatBobbingSpriteEffect(FloatBobbingSpriteEffectState *);
void cleanupFloatBobbingSpriteEffect(SpriteEffectTaskState *);

void initFloatBobbingSpriteEffect(SpriteEffectTaskState *arg0) {
    setCleanupCallback(cleanupFloatBobbingSpriteEffect);
    loadSpriteAsset(&arg0->unk20, 0);
    setSpriteAnimation(&arg0->unk20, 0x10000, arg0->unk6, -1);
    arg0->unk6C = 0;
    arg0->unk70.unk70 = 0;
    setCallback(updateFloatBobbingSpriteEffect);
}

void updateFloatBobbingSpriteEffect(FloatBobbingSpriteEffectState *arg0) {
    SpriteEffectPosition *pos;
    s32 x, y, z;
    s32 sinVal;

    if (arg0->duration < 0) {
        terminateCurrentTask();
        return;
    }

    arg0->duration = arg0->duration - 1;

    sinVal = approximateSin(arg0->bobPhase);

    arg0->bobOffset = ((sinVal << 3) >> 8) * 655;

    arg0->bobPhase = arg0->bobPhase + 0x111;

    pos = getSpriteEffectPosition(arg0->positionSource, arg0->useParentPos);

    x = pos->unk14 + arg0->offsetX;
    y = pos->unk18 + arg0->offsetY + arg0->bobOffset;
    z = pos->unk1C + arg0->offsetZ;

    renderSprite(&arg0->spriteState, arg0->layer, x, y, z, 0x10000, arg0->scale, arg0->rotation, arg0->opacity, 0xFF);
}

void cleanupFloatBobbingSpriteEffect(SpriteEffectTaskState *arg0) {
    releaseNodeMemoryRef((void **)&arg0->unk20);
}

extern DmaEntry *D_8008CC2C_8D82C;
extern s32 D_8008CC30_8D830;

DmaEntry *getDmaTable(void) {
    return D_8008CC2C_8D82C;
}

s32 getDmaTableCount(void) {
    return D_8008CC30_8D830;
}

void *loadDmaAsset(s16 arg0) {
    DmaEntry *entry;

    if (arg0 >= getDmaTableCount()) {
        return 0;
    }
    entry = &getDmaTable()[arg0];
    return loadCompressedData(entry->romStart, entry->romEnd, entry->size);
}

void *getTable2DEntry(Table_B934 *table, s32 rowIndex, s32 colIndex) {
    s32 rowCount;
    Table2DRow *row;
    s32 *colOffsets;

    rowCount = *(s32 *)(table->count_offset + (s32)table);
    if (rowIndex >= rowCount) {
        return NULL;
    }
    row = (Table2DRow *)(table->array_offset + rowIndex * 8 + (s32)table);
    if (colIndex >= row->count) {
        return NULL;
    }
    colOffsets = (s32 *)(row->arrayOffset + (s32)table);
    return (void *)(colOffsets[colIndex] + (s32)table);
}

s16 getMaxLineCharCount(u16 *encodedText) {
    s16 i = 0;
    s16 lineLength = 0;
    u16 code;
    s16 maxLength;

    code = encodedText[0];
    maxLength = 0;
    if (code != 0xFFFF) {
        do {
            code = encodedText[i];
            if (code != 0xFFFC) {
                if (code == 0xFFFD) {
                    lineLength = 0;
                } else {
                    lineLength++;
                }
            } else {
                i++;
            }

            if (lineLength > maxLength) {
                maxLength = lineLength;
            }

            i++;
        } while (encodedText[i] != 0xFFFF);
    }

    return maxLength;
}

s16 getMaxLinePixelWidth(u16 *encodedText) {
    s16 i;
    s16 lineWidth;
    s16 maxWidth;
    u16 code;
    u16 charWidth;

    i = 0;
    lineWidth = 0;
    code = encodedText[0];
    maxWidth = 0;
    if (code != 0xFFFF) {
        do {
            code = encodedText[i];
            switch (code) {
                case 0xFFFC:
                    i++;
                    break;
                case 0xFFFB:
                    lineWidth += 4;
                    break;
                case 0xFFFD:
                    lineWidth = 0;
                    break;
                case 0xFFFE:
                    lineWidth += 4;
                    break;
                default:
                    charWidth = encodedText[i] >> 12;
                    if (charWidth == 0) {
                        lineWidth += 0xC;
                    } else {
                        lineWidth += charWidth;
                    }
                    break;
            }

            if (lineWidth > maxWidth) {
                maxWidth = lineWidth;
            }

            i++;
            code = encodedText[i];
        } while (code != 0xFFFF);
    }

    return maxWidth;
}

s32 getNewlineCount(u16 *encodedText) {
    s16 i = 0;
    s16 newlineCount = 0;

    if (encodedText[0] != 0xFFFF) {
        do {
            if (encodedText[i] == 0xFFFD) {
                newlineCount++;
            }
            i++;
        } while (encodedText[i] != 0xFFFF);
    }

    return newlineCount;
}

s16 getTable2DRowCount(Table_B934 *table, s32 rowIndex) {
    s32 rowCount;
    Table2DRow *row;

    rowCount = *(s32 *)(table->count_offset + (s32)table);
    if (rowIndex >= rowCount) {
        return 0;
    }
    row = (Table2DRow *)(table->array_offset + rowIndex * 8 + (s32)table);
    return row->count;
}
