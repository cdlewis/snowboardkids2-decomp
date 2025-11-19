#include "5E590.h"
#include "common.h"
#include "rand.h"
#include "task_scheduler.h"

typedef struct {
    void *unk00;
    void *unk04;
    s8 unk08[0x16];
    s8 unk1E;
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

extern void rotateVectorY(Vec3 *, s32, void *);
extern void func_800BB45C_AF14C(void);
extern void func_800BB5B0_AF2A0(void);
extern void func_800BB778_AF468(void);
extern void func_800BBEAC_AFB9C(void);
extern void func_800BC750_B0440(void);

extern void *D_800BC7F0_B04E0;
extern Vec3 D_800BCAA0_B0790;

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

INCLUDE_ASM("asm/nonmatchings/AEFA0", func_800BB45C_AF14C);

INCLUDE_ASM("asm/nonmatchings/AEFA0", func_800BB488_AF178);

INCLUDE_ASM("asm/nonmatchings/AEFA0", func_800BB5B0_AF2A0);

INCLUDE_ASM("asm/nonmatchings/AEFA0", func_800BB620_AF310);

INCLUDE_ASM("asm/nonmatchings/AEFA0", func_800BB6F4_AF3E4);

void func_800BB74C_AF43C(func_800BB74C_AF43C_arg *arg0) {
    arg0->unk0 = 0;
    arg0->unk2 = 0x14;
    setCallback(func_800BB778_AF468);
}

INCLUDE_ASM("asm/nonmatchings/AEFA0", func_800BB778_AF468);

INCLUDE_ASM("asm/nonmatchings/AEFA0", func_800BB8E8_AF5D8);

INCLUDE_ASM("asm/nonmatchings/AEFA0", func_800BB9A4_AF694);

INCLUDE_ASM("asm/nonmatchings/AEFA0", func_800BBC2C_AF91C);

INCLUDE_ASM("asm/nonmatchings/AEFA0", func_800BBC64_AF954);

INCLUDE_ASM("asm/nonmatchings/AEFA0", func_800BBCE8_AF9D8);

INCLUDE_ASM("asm/nonmatchings/AEFA0", func_800BBD14_AFA04);

void func_800BBE84_AFB74(s16* arg0) {
    *arg0 = 0xB4;
    setCallback(func_800BBEAC_AFB9C);
}

INCLUDE_ASM("asm/nonmatchings/AEFA0", func_800BBEAC_AFB9C);

INCLUDE_ASM("asm/nonmatchings/AEFA0", func_800BBF4C_AFC3C);

INCLUDE_ASM("asm/nonmatchings/AEFA0", func_800BBFC8_AFCB8);

INCLUDE_ASM("asm/nonmatchings/AEFA0", func_800BC0D0_AFDC0);

INCLUDE_ASM("asm/nonmatchings/AEFA0", func_800BC0FC);

INCLUDE_ASM("asm/nonmatchings/AEFA0", func_800BC13C_AFE2C);

INCLUDE_ASM("asm/nonmatchings/AEFA0", func_800BC184_AFE74);

INCLUDE_ASM("asm/nonmatchings/AEFA0", func_800BC220_AFF10);

INCLUDE_ASM("asm/nonmatchings/AEFA0", func_800BC340_B0030);

INCLUDE_ASM("asm/nonmatchings/AEFA0", func_800BC378_B0068);

void func_800BC72C_B041C(s16 *arg0) {
    *arg0 = 0;
    setCallback(func_800BC750_B0440);
}

INCLUDE_ASM("asm/nonmatchings/AEFA0", func_800BC750_B0440);
