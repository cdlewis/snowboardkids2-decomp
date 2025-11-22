#include "5E590.h"
#include "common.h"
#include "rand.h"
#include "task_scheduler.h"

typedef struct {
    void *unk00;
    void *unk04;
    s8 unk08[0x16];
    u8 unk1E;
    s8 unk1F[0x11];
    s8 unk30[0x0C];
    s16 unk3C;
    s16 unk3E;
    s16 unk40;
    s16 unk42;
    s16 unk44;
} func_800BB388_AF078_arg;

typedef struct {
    s32 unk00;
    s32 unk04;
    s32 unk08;
} Vec3;

typedef struct {
    s16 unk0;
    s16 unk2;
} func_800BB74C_AF43C_arg;

typedef struct {
    void *unk0;
    void *unk4;
} func_800BC340_B0030_arg;

typedef struct {
    u8 _pad[0x24];
    void *unk24;
    void *unk28;
} func_800BBC2C_AF91C_arg;

typedef struct {
    void *unk0;
    void *unk4;
    s8 unk8[0x4];
    s32 unkC;
    s8 unk10[0xE];
    u8 unk1E;
    s8 unk1F[0x5];
    s32 unk24;
    s16 unk28;
} func_800BBC64_AF954_arg;

extern void rotateVectorY(Vec3 *, s32, void *);
extern void *func_80055D7C_5697C(s32);
extern void func_80069CF8_6A8F8(void);
extern void func_800BB2B0_AEFA0(func_800BB388_AF078_arg *);
extern void func_800BB45C_AF14C(void **);
extern void func_800BB488_AF178(func_800BB388_AF078_arg *);
extern void func_800BB5B0_AF2A0(void);
extern void func_800BB778_AF468(void);
extern void func_800BBEAC_AFB9C(void);
extern void func_800BBCE8_AF9D8(void **);
extern void func_800BBD14_AFA04(void);
extern void func_800BC184_AFE74(void);
extern void func_800BC340_B0030(func_800BC340_B0030_arg *);
extern void func_800BC750_B0440(void);

extern void *D_800BC7F0_B04E0;
extern Vec3 D_800BCAA0_B0790;
extern void *D_800BC8C8_B05B8;
extern s8 D_800BC908_B05F8[12];
extern void *D_800BC920_B0610;
extern void *D_800BC960_B0650;
extern s16 D_800BC9DC_B06CC[];

INCLUDE_ASM("asm/nonmatchings/AEFA0", func_800BB2B0_AEFA0);

void func_800BB388_AF078(func_800BB388_AF078_arg *entity) {
    Vec3 vector;
    Vec3 *vector_ptr;
    s32 random_value;
    s32 rotation_angle;

    vector_ptr = &vector;
    memcpy(vector_ptr, &D_800BCAA0_B0790, 12);

    getCurrentAllocation();

    entity->unk00 = load_3ECE40();
    entity->unk04 = &D_800BC7F0_B04E0;
    entity->unk1E = 0;

    random_value = randA() & 0xFF;
    random_value = random_value << 10;
    vector.unk08 = random_value + 0x20000;

    rotation_angle = randA() & 0xFF;
    rotation_angle = rotation_angle << 5;

    rotateVectorY(vector_ptr, rotation_angle, &entity->unk30);

    if (entity->unk3C == 0) {
        entity->unk3E = 0x1A;
    } else {
        entity->unk3E = 0x58;
    }

    entity->unk40 = 1;
    entity->unk44 = 0;
    entity->unk42 = 0x3C;

    setCleanupCallback(func_800BB45C_AF14C);
    setCallbackWithContinue(func_800BB5B0_AF2A0);
}

void func_800BB45C_AF14C(void **arg0) {
    *arg0 = freeNodeMemory(*arg0);
}

INCLUDE_ASM("asm/nonmatchings/AEFA0", func_800BB488_AF178);

INCLUDE_ASM("asm/nonmatchings/AEFA0", func_800BB5B0_AF2A0);

INCLUDE_ASM("asm/nonmatchings/AEFA0", func_800BB620_AF310);

void func_800BB6F4_AF3E4(func_800BB388_AF078_arg *arg0) {
    arg0->unk1E -= 0x10;

    if (arg0->unk1E == 0) {
        func_80069CF8_6A8F8();
    } else {
        func_800BB488_AF178(arg0);
        func_800BB2B0_AEFA0(arg0);
    }
}

void func_800BB74C_AF43C(func_800BB74C_AF43C_arg *arg0) {
    arg0->unk0 = 0;
    arg0->unk2 = 0x14;
    setCallback(func_800BB778_AF468);
}

INCLUDE_ASM("asm/nonmatchings/AEFA0", func_800BB778_AF468);

INCLUDE_ASM("asm/nonmatchings/AEFA0", func_800BB8E8_AF5D8);

INCLUDE_ASM("asm/nonmatchings/AEFA0", func_800BB9A4_AF694);

void func_800BBC2C_AF91C(func_800BBC2C_AF91C_arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

void func_800BBC64_AF954(func_800BBC64_AF954_arg *arg0) {
    s32 temp_v1;

    arg0->unk0 = func_80055D7C_5697C(9);
    arg0->unk4 = &D_800BC8C8_B05B8;
    arg0->unk1E = 0xFF;
    memcpy(&arg0->unk8, &D_800BC908_B05F8, 12);
    temp_v1 = arg0->unkC + arg0->unk24;
    arg0->unk28 = 0xB4;
    arg0->unkC = temp_v1;
    setCleanupCallback(func_800BBCE8_AF9D8);
    setCallbackWithContinue(func_800BBD14_AFA04);
}

void func_800BBCE8_AF9D8(void **arg0) {
    *arg0 = freeNodeMemory(*arg0);
}

INCLUDE_ASM("asm/nonmatchings/AEFA0", func_800BBD14_AFA04);

void func_800BBE84_AFB74(s16 *arg0) {
    *arg0 = 0xB4;
    setCallback(func_800BBEAC_AFB9C);
}

INCLUDE_ASM("asm/nonmatchings/AEFA0", func_800BBEAC_AFB9C);

typedef struct {
    void *unk0;
    void *unk4;
    s8 pad[0x1C];
    s16 unk24;
} func_800BBF4C_AFC3C_arg;

extern void func_800BC0D0_AFDC0(void **);
extern void func_800BBFC8_AFCB8(void);

void func_800BBF4C_AFC3C(func_800BBF4C_AFC3C_arg *arg0) {
    s16 index;

    arg0->unk0 = func_80055D7C_5697C(9);
    arg0->unk4 = &D_800BC920_B0610;

    index = arg0->unk24;
    if (D_800BC9DC_B06CC[index] == 7) {
        arg0->unk4 = &D_800BC960_B0650;
    }

    setCleanupCallback(func_800BC0D0_AFDC0);
    setCallback(func_800BBFC8_AFCB8);
}

INCLUDE_ASM("asm/nonmatchings/AEFA0", func_800BBFC8_AFCB8);

void func_800BC0D0_AFDC0(void **arg0) {
    *arg0 = freeNodeMemory(*arg0);
}

typedef struct {
    u8 _pad[0x24];
    s16 unk24;
} func_800BC0FC_Task;

void func_800BC0FC(s16 arg0) {
    func_800BC0FC_Task *task = (func_800BC0FC_Task *)scheduleTask(func_800BBF4C_AFC3C, 0, 0, 0xC8);
    if (task != NULL) {
        task->unk24 = arg0;
    }
}

void func_800BC13C_AFE2C(func_800BC340_B0030_arg *arg0) {
    arg0->unk4 = func_80055D7C_5697C(9);
    arg0->unk0 = 0;
    setCleanupCallback(func_800BC340_B0030);
    setCallback(func_800BC184_AFE74);
}

INCLUDE_ASM("asm/nonmatchings/AEFA0", func_800BC184_AFE74);

INCLUDE_ASM("asm/nonmatchings/AEFA0", func_800BC220_AFF10);

void func_800BC340_B0030(func_800BC340_B0030_arg *arg0) {
    arg0->unk0 = freeNodeMemory(arg0->unk0);
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

INCLUDE_ASM("asm/nonmatchings/AEFA0", func_800BC378_B0068);

void func_800BC72C_B041C(s16 *arg0) {
    *arg0 = 0;
    setCallback(func_800BC750_B0440);
}

INCLUDE_ASM("asm/nonmatchings/AEFA0", func_800BC750_B0440);
