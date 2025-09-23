#include "6E840.h"
#include "common.h"
#include "overlay.h"
#include "task_scheduler.h"

USE_ASSET(_646910);

typedef struct {
    s8 unk0;
    s8 unk1;
    s8 unk2;
    s8 unk3;
} func_800B9020_arg;

typedef struct {
    void *unk0;
} func_800B9020_arg_task;

typedef struct {
    u8 padding[0x4];
    s32 unk4;
    s32 unk8;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s16 unk12;
    s32 unk14;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
    s8 unk22;
    s8 unk23;
    s8 unk24;
} func_800B90DC_1E618C_arg;

typedef struct {
    s32 unk0;
    s32 unk4;
} func_800B93CC_1E647C_arg;

typedef struct {
    s8 unk0;
    s8 unk1;
    s32 unk4;
} func_800B9264_1E6314_input;

typedef struct {
    s8 unk0;
    s8 unk1;
    s8 unk2;
    s8 unk3;
} func_800B9180_1E6230_arg_unk0;
typedef struct {
    func_800B9180_1E6230_arg_unk0 *unk0;
    u8 padding[0x4];
    union {
        s32 word;
        s16 halfword;
    } unk8;
    s32 unkC;
    s32 unk10;
    u8 padding2[0x10];
    s8 unk24;
} func_800B9180_1E6230_arg;

typedef struct {
    s32 unk0;
    s32 unk4;
    s16 unk8;
} func_800B9290_1E6340_arg;

typedef struct {
    void *unk0;
    s32 unk4;
    s32 unk8;
    s16 unkC;
} func_800B92D4_1E6384_task;

extern s8 D_800BADE0_1E7E90[];

void func_800B90B0_1E6160(void **);
void func_800B91E4_1E6294(func_800B9180_1E6230_arg *);
void func_800B9180_1E6230(func_800B9180_1E6230_arg *);
void func_800B92D4_1E6384(func_800B9180_1E6230_arg *);
void func_800B93CC_1E647C(func_800B93CC_1E647C_arg *);
void func_800B9290_1E6340(func_800B9290_1E6340_arg *);

extern void func_800B9264_1E6314(func_800B9264_1E6314_input **);
extern void func_80011924_12524(void);

void func_800B9020(func_800B9020_arg *arg0) {
    func_800B9020_arg_task *temp_v0;

    arg0->unk0 = 0;
    arg0->unk1 = 0;
    arg0->unk2 = 3;
    arg0->unk3 = 0;
    temp_v0 = (func_800B9020_arg_task *)scheduleTask(&func_800B9290_1E6340, 0, 0, 0);
    if (temp_v0 != NULL) {
        temp_v0->unk0 = arg0;
    }
}

void func_800B9074_1E6124(void **arg0) {
    *arg0 = dmaRequestAndUpdateStateWithSize(&_646910_ROM_START, &_646910_ROM_END, 0x658);
}

void func_800B90B0_1E6160(void **arg0) {
    *arg0 = freeNodeMemory(*arg0);
}

void func_800B90DC_1E618C(func_800B90DC_1E618C_arg *arg0) {
    s8 new_var2;
    s32 temp;

    setCleanupCallback(&func_800B9264_1E6314);
    func_800B9074_1E6124(&arg0->unk4);

    arg0->unk12 = 0;
    temp = arg0->unkC - 1;
    arg0->unk10 = temp << 5;
    arg0->unk14 = arg0->unk4;

    new_var2 = D_800BADE0_1E7E90[arg0->unkC];

    arg0->unk1C = 0x400;
    arg0->unk1A = 0x400;
    arg0->unk1E = 0;
    arg0->unk20 = 0xFF;
    arg0->unk22 = 0;
    arg0->unk23 = 0;
    arg0->unk24 = 0;
    arg0->unk8 = 0;
    arg0->unk18 = new_var2;

    setCallback(&func_800B91E4_1E6294);
}

void func_800B9180_1E6230(func_800B9180_1E6230_arg *arg0) {
    s32 value;
    s32 new_var;

    value = arg0->unk8.word;
    if (value <= 0) {
        func_80069CF8_6A8F8();
        return;
    }

    value -= 0xA0000;
    new_var = value;
    arg0->unk8.word = value;
    if (value < 0) {
        arg0->unk8.word = 0;
    }

    value = (s16)arg0->unk8.halfword;
    new_var = value;
    arg0->unk24 = (s8)new_var;

    debugEnqueueCallback(1, 0, &func_80011924_12524, &arg0->unk10);
}

void func_800B91E4_1E6294(func_800B9180_1E6230_arg *arg0) {
    s32 temp_v0;

    if (arg0->unk0->unk3 == 1) {
        setCallback(&func_800B9180_1E6230);
    }

    temp_v0 = arg0->unk8.word + 0xA0000;
    arg0->unk8.word = temp_v0;
    if (temp_v0 > 0xFF0000) {
        arg0->unk8.word = 0xFF0000;
    }

    temp_v0 = arg0->unk8.halfword;
    arg0->unk24 = (s8)temp_v0;

    debugEnqueueCallback(1, 0, &func_80011924_12524, &arg0->unk10);
}

void func_800B9264_1E6314(func_800B9264_1E6314_input **arg0) {
    func_800B9264_1E6314_input *temp_v1 = *arg0;
    temp_v1->unk1--;
    func_800B90B0_1E6160(arg0 + 1);
}

void func_800B9290_1E6340(func_800B9290_1E6340_arg *arg0) {
    func_800B9074_1E6124(&arg0->unk4);
    arg0->unk8 = 0;
    setCleanupCallback(&func_800B93CC_1E647C);
    setCallback(&func_800B92D4_1E6384);
}

void func_800B92D4_1E6384(func_800B9180_1E6230_arg *arg0) {
    func_800B9180_1E6230_arg_unk0 *unk0;
    func_800B92D4_1E6384_task *temp_v0;

    unk0 = arg0->unk0;
    if (unk0->unk2 == 0) {
        if (arg0->unk8.halfword == 0) {
            unk0->unk3 = 1;

            if (arg0->unk0->unk1 != 0) {
                return;
            }
            arg0->unk0->unk0 = 1;
            func_80069CF8_6A8F8();
            return;
        } else {
            arg0->unk8.halfword--;
        }
    } else if (arg0->unk8.halfword == 0) {
        temp_v0 = (func_800B92D4_1E6384_task *)scheduleTask(&func_800B90DC_1E618C, 0, 0, 0);
        if (temp_v0 != 0) {
            temp_v0->unk0 = (void *)arg0->unk0;
            temp_v0->unkC = (s16)(3 - ((s8)((u8)arg0->unk0->unk2)));
            arg0->unk0->unk1++;
        }

        arg0->unk0->unk2--;

        if (arg0->unk0->unk2 == 0) {
            arg0->unk8.halfword = 0x78;
        } else {
            arg0->unk8.halfword = 0x14;
        }
    } else {
        arg0->unk8.halfword--;
    }
}

void func_800B93CC_1E647C(func_800B93CC_1E647C_arg *arg0) {
    func_800B90B0_1E6160(&arg0->unk4);
}
