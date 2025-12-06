#include "56910.h"
#include "5E590.h"
#include "common.h"
#include "displaylist.h"
#include "geometry.h"
#include "rand.h"
#include "task_scheduler.h"

typedef struct {
    void *unk00;
    void *unk04;
    s8 unk08[0x16];
    u8 unk1E;
    s8 unk1F[0x5];
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s16 unk3C;
    s16 unk3E;
    s16 unk40;
    s16 unk42;
    s16 unk44;
    u8 unk46;
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
    u8 pad[0x30];
    void *unk30;
} AllocationUnk30;

typedef struct {
    u8 pad[0x76];
    u8 unk76;
} Allocation;

typedef struct {
    u8 pad[0x24];
    s32 unk24;
} Task;

typedef struct {
    u8 _pad[0x24];
    void *unk24;
    void *unk28;
} func_800BBC2C_AF91C_arg;

typedef struct {
    /* 0x00 */ u8 pad[0x20];
    /* 0x20 */ void *unk20;
    /* 0x24 */ void *unk24;
    /* 0x28 */ void *unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ u8 pad30[0xC];
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ s32 unk40;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ s16 unk48;
    /* 0x4A */ s16 unk4A;
    /* 0x4C */ s16 unk4C;
    /* 0x4E */ s16 unk4E;
    /* 0x50 */ s16 unk50;
} func_800BB8E8_AF5D8_arg;

typedef struct {
    void *unk0;
    void *unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    u8 pad[0xA];
    u8 unk1E;
    u8 pad2[0x5];
    s32 unk24;
    s16 unk28;
} func_800BBC64_AF954_arg;

extern void rotateVectorY(Vec3 *, s32, void *);
extern void *func_80055D7C_5697C(s32);
extern void func_80069CF8_6A8F8(void);
extern void func_800BB45C_AF14C(void **);
extern void func_800BB5B0_AF2A0(func_800BB388_AF078_arg *);
extern u16 func_80060A3C_6163C(void *, u16, void *);
extern void func_80060CDC_618DC(void *, u16, void *, s32, s32 *);
extern s32 func_8005CFC0_5DBC0(void *, u16, void *, s32);
extern void func_800BB620_AF310(void);
extern void func_800BB6F4_AF3E4(func_800BB388_AF078_arg *);
extern void func_800BB778_AF468(void);
extern void func_800BB9A4_AF694(void);
void func_800BBC2C_AF91C(func_800BBC2C_AF91C_arg *);
extern void func_800BBC64_AF954(func_800BBC64_AF954_arg *);
extern void setCleanupCallback(void *);
extern void setCallback(void *);
extern void func_800BBEAC_AFB9C(s16 *);
extern void func_800BBCE8_AF9D8(void **);
extern void func_800BBD14_AFA04(func_800BBC64_AF954_arg *);
extern void func_80066444_67044(s32, loadAssetMetadata_arg *);
extern void func_8006BFB8_6CBB8(void *, void *);
extern void func_80058530_59130(s32, s32);
extern void func_800BC184_AFE74(void **);
extern void func_800BC220_AFF10(void);
extern void func_800BC340_B0030(func_800BC340_B0030_arg *);
extern void func_800BC750_B0440(s16 *);

typedef struct {
    s8 unk0;
    s8 unk1;
} AnimationData;

extern s32 D_8009A8A4_9B4A4;
extern void *D_800BC7F0_B04E0;
extern Vec3 D_800BCAA0_B0790;
extern AnimationData D_800BC830_B0520[];
extern void *D_800BC8C8_B05B8;
extern s8 D_800BC908_B05F8[12];
extern void *D_800BC920_B0610;
extern void *D_800BC960_B0650;
extern s16 D_800BC9DC_B06CC[];
extern s32 D_800BCA00_B06F0;
extern s32 D_800BC914_B0604;
extern s32 D_800BC918_B0608;
extern s32 D_800BC91C_B060C;

void func_800BB2B0_AEFA0(func_800BB388_AF078_arg *arg0) {
    s32 i;

    arg0->unk40--;

    if (arg0->unk40 == 0) {
        loadAssetMetadata((loadAssetMetadata_arg *)&arg0->unk04, arg0->unk00, D_800BC830_B0520[arg0->unk44].unk1);

        arg0->unk40 = D_800BC830_B0520[arg0->unk44].unk0;
        arg0->unk44++;

        if (D_800BC830_B0520[arg0->unk44].unk0 == 0) {
            arg0->unk44 = 0;
        }
    }

    memcpy(&arg0->unk08, &arg0->unk24, 0xC);

    for (i = 0; i < 4; i++) {
        func_800677C0_683C0(i, (loadAssetMetadata_arg *)&arg0->unk04);
    }
}

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

s32 func_800BB488_AF178(func_800BB388_AF078_arg *arg0) {
    s32 sp18[3];
    AllocationUnk30 *allocation;
    void *allocationUnk30;
    s32 *argPos;
    s32 result;
    u16 temp;

    result = 0;
    allocation = (AllocationUnk30 *)getCurrentAllocation();

    if (((u8 *)allocation)[0x76] == 0) {
        allocationUnk30 = &allocation->unk30;

        arg0->unk24 = arg0->unk24 + arg0->unk30;
        arg0->unk2C = arg0->unk2C + arg0->unk38;

        argPos = &arg0->unk24;

        temp = func_80060A3C_6163C(allocationUnk30, arg0->unk3E, argPos);
        arg0->unk3E = temp;

        func_80060CDC_618DC(allocationUnk30, temp, argPos, 0x80000, sp18);

        arg0->unk28 = func_8005CFC0_5DBC0(allocationUnk30, arg0->unk3E, argPos, 0x100000) + 0x180000;

        if (arg0->unk3C == 0) {
            result = (arg0->unk3E != 0x1A);
        } else {
            u16 v = arg0->unk3E;
            if ((u16)(v - 0x58) >= 2) {
                result = 1;
            }
        }

        if (sp18[0] != 0) {
            result = 1;
        } else if (sp18[2] != 0) {
            result = 1;
        }

        if (arg0->unk42 == 0) {
            result = 1;
        } else {
            arg0->unk42 = arg0->unk42 - 1;
        }
    }

    return result;
}

void func_800BB5B0_AF2A0(func_800BB388_AF078_arg *arg0) {
    arg0->unk1E += 0x10;

    if (arg0->unk1E == 0xE0) {
        arg0->unk46 = 1;
        setCallback(func_800BB620_AF310);
    }

    if (func_800BB488_AF178(arg0)) {
        setCallback(func_800BB6F4_AF3E4);
    }

    func_800BB2B0_AEFA0(arg0);
}

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

void func_800BB8E8_AF5D8(func_800BB8E8_AF5D8_arg *arg0) {
    func_80055E68_56A68_result *temp_v0_3;
    u16 temp_a1;

    arg0->unk24 = func_80055DC4_569C4(9);
    arg0->unk28 = func_80055DF8_569F8(9);
    arg0->unk2C = 0;

    temp_v0_3 = func_80055E68_56A68(9);
    arg0->unk20 = (void *)((u8 *)temp_v0_3 + 0x90);

    arg0->unk4E = 0xF;
    arg0->unk3C = 0xFD71ABE1;
    arg0->unk40 = 0x1E37049C;
    arg0->unk44 = 0xF61F3676;
    arg0->unk4C = 0;
    arg0->unk48 = 0;
    arg0->unk4A = 0x400;
    arg0->unk50 = 0x1E;

    temp_a1 = arg0->unk4A + 0x2A0;
    createYRotationMatrix((Mat3x3Padded *)arg0, temp_a1 & 0xFFFF);
    setCleanupCallback(func_800BBC2C_AF91C);
    setCallback(func_800BB9A4_AF694);
}

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

void func_800BBD14_AFA04(func_800BBC64_AF954_arg *arg0) {
    Allocation *allocation;
    s32 i;

    allocation = getCurrentAllocation();

    if (allocation->unk76 == 0) {
        *(s16 *)((u8 *)arg0 + 0x28) -= 1;

        if (*(s16 *)((u8 *)arg0 + 0x28) == 0) {
            func_80069CF8_6A8F8();
            return;
        }

        *(s32 *)((u8 *)arg0 + 0x8) += (D_800BC914_B0604 - *(s32 *)((u8 *)arg0 + 0x8)) / *(s16 *)((u8 *)arg0 + 0x28);
        *(s32 *)((u8 *)arg0 + 0xC) += (D_800BC918_B0608 - (*(s32 *)((u8 *)arg0 + 0xC) + *(s32 *)((u8 *)arg0 + 0x24))) /
                                      *(s16 *)((u8 *)arg0 + 0x28);
        *(s32 *)((u8 *)arg0 + 0x10) += (D_800BC91C_B060C - *(s32 *)((u8 *)arg0 + 0x10)) / *(s16 *)((u8 *)arg0 + 0x28);
    }

    loadAssetMetadata((loadAssetMetadata_arg *)((u8 *)arg0 + 0x4), *(void **)arg0, 5);

    for (i = 0; i < 4; i++) {
        func_80066444_67044(i, (loadAssetMetadata_arg *)((u8 *)arg0 + 0x4));
    }
}

void func_800BBE84_AFB74(s16 *arg0) {
    *arg0 = 0xB4;
    setCallback(func_800BBEAC_AFB9C);
}

void func_800BBEAC_AFB9C(s16 *arg0) {
    Allocation *allocation;
    Task *task;
    u8 rand;
    s32 value;

    allocation = (Allocation *)getCurrentAllocation();
    if (allocation->unk76 != 0) {
        return;
    }

    *arg0 -= 1;
    if (*arg0 != 0) {
        return;
    }

    task = (Task *)scheduleTask(func_800BBC64_AF954, 0, 0, 0xC8);
    if (task != NULL) {
        rand = randA() & 0xFF;
        value = (rand * 5) << 13;
        task->unk24 = value;
    }

    rand = randA() & 0x1F;
    *arg0 = rand + 0xB4;
}

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

void func_800BC184_AFE74(void **arg0) {
    s32 i;
    s32 *global_buffer;
    s32 *src_ptr;

    *arg0 = allocateNodeMemory(0x200);

    i = 0;
    global_buffer = &D_8009A8A4_9B4A4;
    src_ptr = &D_800BCA00_B06F0;

    while (i < 8) {
        memcpy(global_buffer, src_ptr, 0xC);
        func_8006BFB8_6CBB8(global_buffer - 5, (void *)((u8 *)*arg0 + (i << 6)));
        src_ptr = (s32 *)((u8 *)src_ptr + 0xC);
        i++;
    }

    setCallback(func_800BC220_AFF10);
}

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

void func_800BC750_B0440(s16 *arg0) {
    s32 i;
    void *allocation;
    void *base;
    u32 offset;
    s16 value;

    allocation = getCurrentAllocation();

    for (i = 0; i < *(u8 *)((u8 *)allocation + 0x5F); i++) {
        offset = (u32)(i * 0xBE8);
        base = (void *)((u32)offset + (u32) * (u32 *)((u8 *)allocation + 0x10));
        value = *arg0;

        if (*(u8 *)((u8 *)base + 0xBC5) == value) {
            if ((u32)(*(u16 *)((u8 *)base + 0xB94) - 7) < 4) {
                *arg0 = value + 1;
                func_80058530_59130(0xC, 6);
            }
        }
    }
}
