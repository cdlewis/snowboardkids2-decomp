#include "3E160.h"
#include "42170.h"
#include "51060.h"
#include "594E0.h"
#include "5AA90.h"
#include "5DBC0.h"
#include "5E590.h"
#include "common.h"
#include "displaylist.h"
#include "gamestate.h"
#include "geometry.h"
#include "graphics.h"
#include "levels/starlight_highway.h"
#include "rand.h"
#include "task_scheduler.h"

typedef struct {
    void *unk0;
    Vec3i unk4;
    u8 padding1[0x10];
    void *unk20;
    Vec3i vel; /* 0x24 */
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    s32 unk3C;
    u16 unk40;
    s16 unk42;
    s16 unk44;
    u16 unk46;
    s16 unk48;
    s16 unk4A;
    s16 unk4C;
    s8 unk4E;
} Struct_52880;

typedef struct {
    u8 padding1[0x74];
    s16 unk74;
    u8 padding1b[0xEE];
    s16 unk164[6];
    u8 padding_170_434[0x2C4];
    s32 unk434[3];
    u8 padding_440_44C[0xC];
    s32 unk44C[3];
    u8 padding_458_950[0x4F8];
    s16 unk950;
    u8 padding2[0x242];
    u16 unkB94;
    u8 padding3[0x43];
    u8 unkBD9;
    u8 padding4[0xE];
} Unk10Element_52880;

typedef struct {
    u8 padding1[0x10];
    Unk10Element_52880 *unk10;
    u8 padding2[0x30];
    void *unk44;
    s16 *unk48;
} Alloc_52880;

typedef struct {
    u8 padding1[0x10];
    Unk10Element_52880 *unk10;
    u8 padding2[0x1C];
    u8 unk30[0x46];
    u8 unk76;
    u8 padding3[0xF];
    u8 unk86;
} Alloc_55650;

typedef struct {
    u8 padding[0x20];
    void *unk20;
} func_80055864_56464_arg;

typedef struct {
    void *unk0;
    Vec3i unk4;
    u8 padding1[0x10];
    void *unk20;
    Vec3i unk24;
    u8 padding2[0xC];
    s32 unk3C;
    u16 unk40;
    s16 unk42;
    s16 unk44;
    s16 unk46;
    s16 unk48;
    s16 unk4A;
    s16 unk4C;
    s8 unk4E;
} func_800550B4_55CB4_arg;

typedef struct {
    u8 padding[0x20];
    void *unk20;
} func_80054CCC_558CC_arg;

void func_800545F8_551F8(Struct_52880 *arg0);
void func_80054658_55258(Struct_52880 *arg0);
void func_80054144_54D44(Struct_52880 *arg0);
void func_80053FE0_54BE0(Struct_52880 *arg0);
void func_80053E90_54A90(Struct_52880 *arg0);
void func_80053B38_54738(Struct_52880 *arg0);
void func_80053990_54590(Struct_52880 *arg0);
void func_800553A8_55FA8(func_80054CCC_558CC_arg *arg0);
void func_80054D0C_5590C(Struct_52880 *arg0);
void func_80054F44_55B44(Struct_52880 *arg0);
void func_80054D70_55970(void *arg0);
void func_800550B4_55CB4(func_800550B4_55CB4_arg *arg0);
void func_80055460_56060(Struct_52880 *arg0);
void func_800554FC_560FC(Struct_52880 *arg0);
void func_80055650_56250(Struct_52880 *arg0);
void func_800558A4_564A4(Struct_52880 *arg0);
void func_80055964_56564(Struct_52880 *arg0);
void func_80055A84_56684(Struct_52880 *);
s32 func_80052A24_53624(s32, s32);
s32 func_80053078_53C78(s32, s32);
s32 func_800537B0_543B0(s32, s32);
s32 func_80053DF0_549F0(s32, s32);
s32 func_80054470_55070(s32, s32);
s32 func_80055820_56420(s32, s32);
void func_80053434_54034(Struct_52880 *);
void func_80052DB4_539B4(Struct_52880 *);
void func_800523EC_52FEC(Struct_52880 *arg0);
void func_80051E34_52A34(Struct_52880 *arg0);
void func_80051FC4_52BC4(Struct_52880 *arg0);
void func_800524A4_530A4(Struct_52880 *arg0);
void func_800525F4_531F4(Struct_52880 *arg0);
void func_8005383C_5443C(Struct_52880 *arg0);
void func_800548C8_554C8(Struct_52880 *arg0);
void func_80054AE4_556E4(Struct_52880 *arg0);

extern s16 D_80090F60_91B60[];
extern s32 D_80090F68_91B68;
extern s32 D_80090F74_91B74;

void func_80051C80_52880(Vec3i *arg0, s32 arg1) {
    s32 dist;

    dist = distance_2d(arg0->x, arg0->z);

    if (!(dist >= 0x20000)) {
        if (dist != 0) {
            arg0->x = (s64)((s64)arg0->x * 0x20000) / dist;
            arg0->z = (s64)((s64)arg0->z * 0x20000) / dist;
        } else {
            arg0->z = 0x20000;
        }
    }

    dist = distance_3d(arg0->x, arg0->y, arg0->z);

    if (dist != 0) {
        arg0->x = (s64)((s64)arg0->x * arg1) / dist;
        arg0->y = (s64)((s64)arg0->y * arg1) / dist;
        arg0->z = (s64)((s64)arg0->z * arg1) / dist;
    }
}

void func_80051DEC_529EC(Struct_52880 *arg0) {
    arg0->unk4C = arg0->unk42;
    arg0->unk20 = load_3ECE40();
    setCleanupCallback(func_800523EC_52FEC);
    setCallbackWithContinue(func_80051E34_52A34);
}

void func_80051E34_52A34(Struct_52880 *arg0) {
    Alloc_52880 *alloc = getCurrentAllocation();
    void *ptr;
    loadAssetMetadata((loadAssetMetadata_arg *)arg0, arg0->unk20, 2);
    ptr = alloc->unk44;
    arg0->unk4E = 0;
    arg0->unk46 = 0;
    arg0->unk0 = ptr;
    setCallbackWithContinue(func_80051FC4_52BC4);
}

void func_80051E90_52A90(Struct_52880 *arg0) {
    GameState *alloc;
    Player *result;
    Vec3i *s1;
    s16 unk4C;
    s32 v28;
    s32 new_var;
    s32 v2C;
    s32 v24;
    s32 i;
    u8 temp;

    alloc = (GameState *)getCurrentAllocation();
    s1 = &arg0->unk4;
    result = func_8005B548_5C148(s1, arg0->unk42, 0x80000);
    if (result != NULL) {
        if (!(result->unkB84 & 0x1000)) {
            func_80058A10_59610(result);
            for (i = 0; i < 3; i++) {
                if (result->unkB6C >= 100) {
                    func_8004A9A8_4B5A8(&result->worldPos.x, result->unkB94, &result->unk44C);
                    func_80059A48_5A648(result, -100);
                }
            }
            unk4C = arg0->unk4C;
            if (unk4C >= 0) {
                func_80059C24_5A824(&alloc->players[unk4C]);
            }
            arg0->unk4E = arg0->unk4E + 1;
        } else {
            v28 = arg0->vel.y;
            v2C = (new_var = arg0->vel.z);
            arg0->unk42 = result->unkBB8;
            temp = result->unkBB8;
            arg0->vel.y = -v28;
            v24 = arg0->vel.x;
            arg0->vel.x = -v24;
            arg0->vel.z = -v2C;
            arg0->unk4C = temp;
            func_80050ECC_51ACC(s1);
            func_80056B7C_5777C(s1, 0x20);
        }
    }
}

void func_80052128_52D28(Struct_52880 *arg0);

void func_80051FC4_52BC4(Struct_52880 *arg0) {
    Alloc_52880 *alloc;
    s16 playerIdx;
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_a1;
    s32 temp_a2;
    s32 i;

    alloc = getCurrentAllocation();

    playerIdx = arg0->unk42;
    transformVector(alloc->unk48, &alloc->unk10[playerIdx].unk950, &arg0->unk4);

    playerIdx = arg0->unk42;
    transformVector(&alloc->unk48[6], &alloc->unk10[playerIdx].unk950, &arg0->vel);

    temp_v0 = arg0->unk4.x;
    temp_a2 = arg0->vel.x;
    temp_v1 = arg0->unk4.y;
    temp_a1 = arg0->vel.y;
    arg0->vel.x = temp_v0 - temp_a2;
    arg0->vel.y = temp_v1 - temp_a1;
    arg0->vel.z = arg0->unk4.z - arg0->vel.z;

    playerIdx = arg0->unk42;
    arg0->unk40 = alloc->unk10[playerIdx].unkB94;
    arg0->unk48 = 0xF0;

    func_80056B7C_5777C(&arg0->unk4, 0x10);
    setCallback(func_80052128_52D28);
    func_80051E90_52A90(arg0);

    if (arg0->unk4E != 0) {
        func_80050ECC_51ACC(&arg0->unk4);
        func_80056B7C_5777C(&arg0->unk4, 0x11);
        func_80069CF8_6A8F8();
    }

    for (i = 0; i < 4; i++) {
        func_80066444_67044(i, (func_80066444_67044_arg1 *)arg0);
    }
}

void func_80052128_52D28(Struct_52880 *arg0) {
    Alloc_55650 *alloc;
    Vec3i sp18;
    Vec3i savedVec;
    Vec3i rotateVec;
    u8 pad1[32];
    s16 var_s3;
    s32 temp_v0;
    s32 i;

    alloc = (Alloc_55650 *)getCurrentAllocation();

    if (alloc->unk76 == 0) {

        func_80050604_51204(&arg0->unk4, &arg0->vel, 2);

        arg0->vel.y += 0xFFFC0000;

        func_80051C80_52880(&arg0->vel, 0x1C0000);

        memcpy(&savedVec, &arg0->unk4, sizeof(Vec3i));

        arg0->unk4.x += arg0->vel.x;
        arg0->unk4.y += arg0->vel.y;
        arg0->unk4.z += arg0->vel.z;

        arg0->unk40 = func_80060A3C_6163C(&alloc->unk30, arg0->unk40, &arg0->unk4);

        func_80060CDC_618DC(&alloc->unk30, arg0->unk40, &arg0->unk4, 0x80000, &sp18);

        if ((sp18.x != 0) || (sp18.z != 0)) {
            arg0->unk4.x = arg0->unk4.x + sp18.x;
            arg0->unk4.z = arg0->unk4.z + sp18.z;
            arg0->unk4E = arg0->unk4E + 1;
        }

        arg0->unk48--;
        if (arg0->unk48 == 0) {
            arg0->unk4E++;
        }

        temp_v0 = func_8005CFC0_5DBC0(&alloc->unk30, arg0->unk40, &arg0->unk4, 0x100000);

        if (arg0->unk4.y < temp_v0 + 0x100000) {
            arg0->unk4.y = temp_v0 + 0x100000;
        }

        arg0->vel.x = arg0->unk4.x - savedVec.x;
        arg0->vel.y = arg0->unk4.y - savedVec.y;
        arg0->vel.z = arg0->unk4.z - savedVec.z;

        func_80051E90_52A90(arg0);

        var_s3 =
            func_8005BF50_5CB50(&arg0->unk4, atan2Fixed(arg0->vel.x, arg0->vel.z), arg0->unk42, 0x3C00000, 0x1C0000);

        if ((var_s3 << 16) != 0) {
            var_s3 &= 0x1FFF;
            if (var_s3 >= 0x1001) {
                var_s3 |= 0xE000;
            }

            if ((s16)arg0->unk46 < var_s3) {
                var_s3 = arg0->unk46;
            }

            if (var_s3 < -(s16)arg0->unk46) {
                var_s3 = -(s16)arg0->unk46;
            }

            memcpy(&rotateVec, &arg0->vel, sizeof(Vec3i));
            rotateVectorY(&rotateVec, var_s3, &arg0->vel);
        }

        if ((s16)arg0->unk46 < 0x2C) {
            arg0->unk46 += 5;
        }
    }

    if (arg0->unk4E != 0) {
        func_80050ECC_51ACC(&arg0->unk4);
        func_80056B7C_5777C(&arg0->unk4, 0x11);
        func_80069CF8_6A8F8();
    }

    i = 0;
    do {
        func_80066444_67044(i, (func_80066444_67044_arg1 *)arg0);
        i++;
    } while (i < 4);
}

void func_800523EC_52FEC(Struct_52880 *arg0) {
    arg0->unk20 = freeNodeMemory(arg0->unk20);
}

s32 func_80052418_53018(s32 arg0, s32 arg1) {
    Struct_52880 *task;

    task = scheduleTask(func_80051DEC_529EC, (arg0 + 4) & 0xFF, 0, 0x6E);
    if (task != NULL) {
        task->unk42 = arg0;
    }
    return (s32)task;
}

void func_8005245C_5305C(Struct_52880 *arg0) {
    arg0->unk4C = arg0->unk42;
    arg0->unk20 = load_3ECE40();
    setCleanupCallback(func_800523EC_52FEC);
    setCallbackWithContinue(func_800524A4_530A4);
}

void func_800524A4_530A4(Struct_52880 *arg0) {
    Alloc_52880 *alloc = getCurrentAllocation();
    void *ptr;
    loadAssetMetadata((loadAssetMetadata_arg *)arg0, arg0->unk20, 3);
    ptr = alloc->unk44;
    arg0->unk4E = 0;
    arg0->unk46 = 0;
    arg0->unk0 = ptr;
    setCallbackWithContinue(func_800525F4_531F4);
}

void func_80052500_53100(Struct_52880 *arg0) {
    GameState *alloc;
    Player *result;
    void *s1;
    s16 unk4C;
    s32 v28;
    s32 new_var;
    s32 v2C;
    Struct_52880 *new_var3;
    s32 v24;
    s32 new_var2;

    alloc = (GameState *)getCurrentAllocation();
    new_var3 = arg0;
    s1 = &arg0->unk4;
    result = func_8005B548_5C148((Vec3i *)s1, new_var3->unk42, 0x80000);
    if (result == 0) {
        return;
    }

    new_var2 = result->unkB84 & 0x1000;
    if (new_var2 == 0) {
        func_80058A3C_5963C(result);
        arg0->unk4E++;
        unk4C = arg0->unk4C;
        if (unk4C >= 0) {
            func_80059C24_5A824(&alloc->players[unk4C]);
        }
    } else {
        v28 = arg0->vel.y;
        v2C = (new_var = arg0->vel.z);
        arg0->unk42 = result->unkBB8;
        arg0->unk4C = result->unkBB8;
        arg0->vel.y = -v28;
        v24 = arg0->vel.x;
        arg0->vel.x = -v24;
        arg0->vel.z = -v2C;
        func_80050ECC_51ACC(s1);
        func_80056B7C_5777C(s1, 0x20);
    }
}

void func_80052758_53358(Struct_52880 *arg0);

void func_800525F4_531F4(Struct_52880 *arg0) {
    Alloc_52880 *alloc;
    s16 playerIdx;
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_a1;
    s32 temp_a2;
    s32 i;

    alloc = getCurrentAllocation();

    playerIdx = arg0->unk42;
    transformVector(alloc->unk48, &alloc->unk10[playerIdx].unk950, &arg0->unk4);

    playerIdx = arg0->unk42;
    transformVector(&alloc->unk48[6], &alloc->unk10[playerIdx].unk950, &arg0->vel);

    temp_v0 = arg0->unk4.x;
    temp_a2 = arg0->vel.x;
    temp_v1 = arg0->unk4.y;
    temp_a1 = arg0->vel.y;
    arg0->vel.x = temp_v0 - temp_a2;
    arg0->vel.y = temp_v1 - temp_a1;
    arg0->vel.z = arg0->unk4.z - arg0->vel.z;

    playerIdx = arg0->unk42;
    arg0->unk40 = alloc->unk10[playerIdx].unkB94;
    arg0->unk48 = 0xF0;

    func_80056B7C_5777C(&arg0->unk4, 0x10);
    setCallback(func_80052758_53358);
    func_80052500_53100(arg0);

    if (arg0->unk4E != 0) {
        func_80050ECC_51ACC(&arg0->unk4);
        func_80056B7C_5777C(&arg0->unk4, 0xD);
        func_80069CF8_6A8F8();
    }

    for (i = 0; i < 4; i++) {
        func_80066444_67044(i, (func_80066444_67044_arg1 *)arg0);
    }
}

void func_80052758_53358(Struct_52880 *arg0) {
    Alloc_55650 *alloc;
    Vec3i sp18;
    Vec3i savedVec;
    Vec3i rotateVec;
    u8 pad1[32];
    s16 var_s3;
    s32 temp_v0;
    s32 i;

    alloc = (Alloc_55650 *)getCurrentAllocation();

    if (alloc->unk76 == 0) {

        func_80050604_51204(&arg0->unk4, &arg0->vel, 3);

        arg0->vel.y += 0xFFFC0000;

        func_80051C80_52880(&arg0->vel, 0x1C8000);

        memcpy(&savedVec, &arg0->unk4, sizeof(Vec3i));

        arg0->unk4.x += arg0->vel.x;
        arg0->unk4.y += arg0->vel.y;
        arg0->unk4.z += arg0->vel.z;

        arg0->unk40 = func_80060A3C_6163C(&alloc->unk30, arg0->unk40, &arg0->unk4);

        func_80060CDC_618DC(&alloc->unk30, arg0->unk40, &arg0->unk4, 0x80000, &sp18);

        if ((sp18.x != 0) || (sp18.z != 0)) {
            arg0->unk4.x = arg0->unk4.x + sp18.x;
            arg0->unk4.z = arg0->unk4.z + sp18.z;
            arg0->unk4E++;
        }

        arg0->unk48--;
        if (arg0->unk48 == 0) {
            arg0->unk4E++;
        }

        temp_v0 = func_8005CFC0_5DBC0(&alloc->unk30, arg0->unk40, &arg0->unk4, 0x100000);

        if (arg0->unk4.y < temp_v0 + 0x100000) {
            arg0->unk4.y = temp_v0 + 0x100000;
        }

        arg0->vel.x = arg0->unk4.x - savedVec.x;
        arg0->vel.y = arg0->unk4.y - savedVec.y;
        arg0->vel.z = arg0->unk4.z - savedVec.z;

        func_80052500_53100(arg0);

        var_s3 =
            func_8005BF50_5CB50(&arg0->unk4, atan2Fixed(arg0->vel.x, arg0->vel.z), arg0->unk42, 0x3600000, 0x1C8000);

        if ((var_s3 << 16) != 0) {
            var_s3 &= 0x1FFF;
            if (var_s3 >= 0x1001) {
                var_s3 |= 0xE000;
            }

            if ((s16)arg0->unk46 < var_s3) {
                var_s3 = arg0->unk46;
            }

            if (var_s3 < -(s16)arg0->unk46) {
                var_s3 = -(s16)arg0->unk46;
            }

            memcpy(&rotateVec, &arg0->vel, sizeof(Vec3i));
            rotateVectorY(&rotateVec, var_s3, &arg0->vel);
        }

        if ((s16)arg0->unk46 < 0x20) {
            arg0->unk46 += 4;
        }
    }

    if (arg0->unk4E != 0) {
        func_80050ECC_51ACC(&arg0->unk4);
        func_80056B7C_5777C(&arg0->unk4, 0xD);
        func_80069CF8_6A8F8();
    }

    i = 0;
    do {
        func_80066444_67044(i, (func_80066444_67044_arg1 *)arg0);
        i++;
    } while (i < 4);
}

s32 func_80052A24_53624(s32 arg0, s32 arg1) {
    Struct_52880 *task;

    task = scheduleTask(func_8005245C_5305C, (arg0 + 4) & 0xFF, 0, 0x6F);
    if (task != NULL) {
        task->unk42 = arg0;
    }
    return (s32)task;
}

void func_80052AB0_536B0(Struct_52880 *arg0);
void func_80052C00_53800(Struct_52880 *arg0);

void func_80052A68_53668(Struct_52880 *arg0) {
    arg0->unk4C = arg0->unk42;
    arg0->unk20 = load_3ECE40();
    setCleanupCallback(func_800523EC_52FEC);
    setCallbackWithContinue(func_80052AB0_536B0);
}

void func_80052AB0_536B0(Struct_52880 *arg0) {
    Alloc_52880 *alloc = getCurrentAllocation();
    void *ptr;
    loadAssetMetadata((loadAssetMetadata_arg *)arg0, arg0->unk20, 4);
    ptr = alloc->unk44;
    arg0->unk4E = 0;
    arg0->unk46 = 0;
    arg0->unk0 = ptr;
    setCallbackWithContinue(func_80052C00_53800);
}

void func_80052B0C_5370C(Struct_52880 *arg0) {
    GameState *alloc;
    Player *result;
    void *s1;
    s16 unk4C;
    s32 v28;
    s32 new_var;
    s32 v2C;
    Struct_52880 *new_var3;
    s32 v24;
    s32 new_var2;

    alloc = (GameState *)getCurrentAllocation();
    new_var3 = arg0;
    s1 = &arg0->unk4;
    result = func_8005B548_5C148((Vec3i *)s1, new_var3->unk42, 0x80000);
    if (result == 0) {
        return;
    }

    new_var2 = result->unkB84 & 0x1000;
    if (new_var2 == 0) {
        func_80058A68_59668(result);
        arg0->unk4E++;
        unk4C = arg0->unk4C;
        if (unk4C >= 0) {
            func_80059C24_5A824(&alloc->players[unk4C]);
        }
    } else {
        v28 = arg0->vel.y;
        v2C = (new_var = arg0->vel.z);
        arg0->unk42 = result->unkBB8;
        arg0->unk4C = result->unkBB8;
        arg0->vel.y = -v28;
        v24 = arg0->vel.x;
        arg0->vel.x = -v24;
        arg0->vel.z = -v2C;
        func_80050ECC_51ACC(s1);
        func_80056B7C_5777C(s1, 0x20);
    }
}

void func_80052C00_53800(Struct_52880 *arg0) {
    Alloc_52880 *alloc;
    s16 playerIdx;
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_a1;
    s32 temp_a2;
    s32 i;

    alloc = getCurrentAllocation();

    playerIdx = arg0->unk42;

    if (alloc->unk10[playerIdx].unkBD9 == 0) {
        transformVector(alloc->unk48, &alloc->unk10[playerIdx].unk950, &arg0->unk4);

        playerIdx = arg0->unk42;
        transformVector(&alloc->unk48[6], &alloc->unk10[playerIdx].unk950, &arg0->vel);
    } else {
        transformVector(&alloc->unk48[48], &alloc->unk10[playerIdx].unk74, &arg0->unk4);

        playerIdx = arg0->unk42;
        transformVector(&alloc->unk48[54], &alloc->unk10[playerIdx].unk74, &arg0->vel);
    }

    temp_v0 = arg0->unk4.x;
    temp_a2 = arg0->vel.x;
    temp_v1 = arg0->unk4.y;
    temp_a1 = arg0->vel.y;
    arg0->vel.x = temp_v0 - temp_a2;
    arg0->vel.y = temp_v1 - temp_a1;
    arg0->vel.z = arg0->unk4.z - arg0->vel.z;

    playerIdx = arg0->unk42;
    arg0->unk40 = alloc->unk10[playerIdx].unkB94;
    arg0->unk48 = 0xF0;

    func_80056B7C_5777C(&arg0->unk4, 0x10);
    setCallback(func_80052DB4_539B4);
    func_80052B0C_5370C(arg0);

    if (arg0->unk4E != 0) {
        func_80050ECC_51ACC(&arg0->unk4);
        func_80056B7C_5777C(&arg0->unk4, 0xD);
        func_80069CF8_6A8F8();
    }

    for (i = 0; i < 4; i++) {
        func_80066444_67044(i, (func_80066444_67044_arg1 *)arg0);
    }
}

void func_80052DB4_539B4(Struct_52880 *arg0) {
    Alloc_55650 *alloc;
    Vec3i sp18;
    Vec3i savedVec;
    Vec3i rotateVec;
    u8 pad1[32];
    s16 var_s3;
    s32 temp_v0;
    s32 i;

    alloc = (Alloc_55650 *)getCurrentAllocation();

    if (alloc->unk76 == 0) {

        func_80050604_51204(&arg0->unk4, &arg0->vel, 4);

        arg0->vel.y += 0xFFFC0000;

        func_80051C80_52880(&arg0->vel, 0x1D0000);

        memcpy(&savedVec, &arg0->unk4, sizeof(Vec3i));

        arg0->unk4.x += arg0->vel.x;
        arg0->unk4.y += arg0->vel.y;
        arg0->unk4.z += arg0->vel.z;

        arg0->unk40 = func_80060A3C_6163C(&alloc->unk30, arg0->unk40, &arg0->unk4);

        func_80060CDC_618DC(&alloc->unk30, arg0->unk40, &arg0->unk4, 0x80000, &sp18);

        if ((sp18.x != 0) || (sp18.z != 0)) {
            arg0->unk4.x = arg0->unk4.x + sp18.x;
            arg0->unk4.z = arg0->unk4.z + sp18.z;
            arg0->unk4E++;
        }

        arg0->unk48--;
        if (arg0->unk48 == 0) {
            arg0->unk4E++;
        }

        temp_v0 = func_8005CFC0_5DBC0(&alloc->unk30, arg0->unk40, &arg0->unk4, 0x100000);

        if (arg0->unk4.y < temp_v0 + 0x100000) {
            arg0->unk4.y = temp_v0 + 0x100000;
        }

        arg0->vel.x = arg0->unk4.x - savedVec.x;
        arg0->vel.y = arg0->unk4.y - savedVec.y;
        arg0->vel.z = arg0->unk4.z - savedVec.z;

        func_80052B0C_5370C(arg0);

        var_s3 =
            func_8005BF50_5CB50(&arg0->unk4, atan2Fixed(arg0->vel.x, arg0->vel.z), arg0->unk42, 0x3600000, 0x1D0000);

        if ((var_s3 << 16) != 0) {
            var_s3 &= 0x1FFF;
            if (var_s3 >= 0x1001) {
                var_s3 |= 0xE000;
            }

            if ((s16)arg0->unk46 < var_s3) {
                var_s3 = arg0->unk46;
            }

            if (var_s3 < -(s16)arg0->unk46) {
                var_s3 = -(s16)arg0->unk46;
            }

            memcpy(&rotateVec, &arg0->vel, sizeof(Vec3i));
            rotateVectorY(&rotateVec, var_s3, &arg0->vel);
        }

        if ((s16)arg0->unk46 < 0x20) {
            arg0->unk46 += 4;
        }
    }

    if (arg0->unk4E != 0) {
        func_80050ECC_51ACC(&arg0->unk4);
        func_80056B7C_5777C(&arg0->unk4, 0xD);
        func_80069CF8_6A8F8();
    }

    i = 0;
    do {
        func_80066444_67044(i, (func_80066444_67044_arg1 *)arg0);
        i++;
    } while (i < 4);
}

s32 func_80053078_53C78(s32 arg0, s32 arg1) {
    Struct_52880 *task;

    task = scheduleTask(func_80052A68_53668, (arg0 + 4) & 0xFF, 0, 0x6F);
    if (task != NULL) {
        task->unk42 = arg0;
    }
    return (s32)task;
}

void func_80053784_54384(Struct_52880 *arg0);
void func_80053104_53D04(Struct_52880 *arg0);
void func_80053254_53E54(Struct_52880 *arg0);

void func_800530BC_53CBC(Struct_52880 *arg0) {
    arg0->unk4C = arg0->unk42;
    arg0->unk20 = load_3ECE40();
    setCleanupCallback(func_80053784_54384);
    setCallbackWithContinue(func_80053104_53D04);
}

void func_80053104_53D04(Struct_52880 *arg0) {
    Alloc_52880 *alloc = getCurrentAllocation();
    void *ptr;
    loadAssetMetadata((loadAssetMetadata_arg *)arg0, arg0->unk20, 5);
    ptr = alloc->unk44;
    arg0->unk4E = 0;
    arg0->unk46 = 0;
    arg0->unk0 = ptr;
    setCallbackWithContinue(func_80053254_53E54);
}

void func_80053160_53D60(Struct_52880 *arg0) {
    GameState *alloc;
    Player *result;
    void *s1;
    s16 unk4C;
    s32 v28;
    s32 new_var;
    s32 v2C;
    Struct_52880 *new_var3;
    s32 v24;
    s32 new_var2;

    alloc = (GameState *)getCurrentAllocation();
    new_var3 = arg0;
    s1 = &arg0->unk4;
    result = func_8005B548_5C148((Vec3i *)s1, new_var3->unk42, 0x80000);
    if (result == 0) {
        return;
    }

    new_var2 = result->unkB84 & 0x1000;
    if (new_var2 == 0) {
        func_80058AC0_596C0(result);
        arg0->unk4E++;
        unk4C = arg0->unk4C;
        if (unk4C >= 0) {
            func_80059C24_5A824(&alloc->players[unk4C]);
        }
    } else {
        v28 = arg0->vel.y;
        v2C = (new_var = arg0->vel.z);
        arg0->unk42 = result->unkBB8;
        arg0->unk4C = result->unkBB8;
        arg0->vel.y = -v28;
        v24 = arg0->vel.x;
        arg0->vel.x = -v24;
        arg0->vel.z = -v2C;
        func_80050ECC_51ACC(s1);
        func_80056B7C_5777C(s1, 0x20);
    }
}

void func_80053254_53E54(Struct_52880 *arg0) {
    Alloc_52880 *alloc;
    s16 playerIdx;
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_a0;
    s32 temp_a1;
    s32 temp_a2;
    s32 temp_a3;
    s32 i;
    Vec3i *s0;

    alloc = getCurrentAllocation();

    playerIdx = arg0->unk42;

    if (alloc->unk10[playerIdx].unkBD9 == 0) {
        transformVector(alloc->unk48, &alloc->unk10[playerIdx].unk950, &arg0->unk4);

        playerIdx = arg0->unk42;
        transformVector(&alloc->unk48[6], &alloc->unk10[playerIdx].unk950, &arg0->vel);
    } else {
        transformVector(&alloc->unk48[48], &alloc->unk10[playerIdx].unk74, &arg0->unk4);

        playerIdx = arg0->unk42;
        transformVector(&alloc->unk48[54], &alloc->unk10[playerIdx].unk74, &arg0->vel);
    }

    temp_v0 = arg0->unk4.x;
    temp_a1 = arg0->vel.x;
    temp_v1 = arg0->unk4.y;
    temp_a2 = arg0->vel.y;
    temp_a0 = arg0->unk4.z;
    temp_a3 = arg0->vel.z;
    playerIdx = arg0->unk42;
    arg0->vel.x = temp_v0 - temp_a1;
    arg0->vel.y = temp_v1 - temp_a2;
    arg0->vel.z = temp_a0 - temp_a3;

    playerIdx = arg0->unk42;
    arg0->unk40 = alloc->unk10[playerIdx].unkB94;
    arg0->unk48 = 0xF0;

    playerIdx = arg0->unk42;
    if (alloc->unk10[playerIdx].unkBD9 != 0) {
        arg0->unk48 = 0x78;
    }

    s0 = &arg0->unk4;
    func_80056B7C_5777C(s0, 0x10);
    setCallback(func_80053434_54034);
    func_80053160_53D60(arg0);

    if (arg0->unk4E != 0) {
        func_80050ECC_51ACC(s0);
        func_80056B7C_5777C(s0, 0xD);
        func_80069CF8_6A8F8();
    }

    for (i = 0; i < 4; i++) {
        func_80066444_67044(i, (func_80066444_67044_arg1 *)arg0);
    }
}

void func_80053434_54034(Struct_52880 *arg0) {
    Alloc_55650 *alloc;
    Vec3i sp18;
    Vec3i savedVec;
    Vec3i rotateVec;
    s32 pad[7];
    s16 var_s3;
    s32 temp_v0;
    s32 i;
    s32 s0;

    alloc = (Alloc_55650 *)getCurrentAllocation();

    if (alloc->unk76 == 0) {
        func_80050604_51204(&arg0->unk4, (Vec3i *)&arg0->vel, 5);

        arg0->vel.y += 0xFFFC0000;

        func_80051C80_52880((Vec3i *)&arg0->vel, 0x1E0000);

        memcpy(&savedVec, &arg0->unk4, sizeof(Vec3i));

        arg0->unk4.x += arg0->vel.x;
        arg0->unk4.y += arg0->vel.y;
        arg0->unk4.z += arg0->vel.z;

        arg0->unk40 = func_80060A3C_6163C(alloc->unk30, arg0->unk40, &arg0->unk4);

        func_80060CDC_618DC(alloc->unk30, arg0->unk40, &arg0->unk4, 0x80000, &sp18);

        if ((sp18.x != 0) || (sp18.z != 0)) {
            arg0->unk4.x = arg0->unk4.x + sp18.x;
            arg0->unk4.z = arg0->unk4.z + sp18.z;
            s0 = atan2Fixed(-sp18.x, -sp18.z);
            rotateVectorY(&arg0->vel, -s0, &rotateVec);
            if (rotateVec.z < 0) {
                rotateVec.z = -rotateVec.z;
            }
            rotateVectorY(&rotateVec, s0, &arg0->vel);
            arg0->unk42 = -1;
        } else {
            arg0->vel.x = arg0->unk4.x - savedVec.x;
            arg0->vel.z = arg0->unk4.z - savedVec.z;
        }

        arg0->unk48--;
        if (arg0->unk48 == 0) {
            arg0->unk4E++;
        }

        temp_v0 = func_8005CFC0_5DBC0(&alloc->unk30, arg0->unk40, &arg0->unk4, 0x100000);

        if (arg0->unk4.y < temp_v0 + 0x100000) {
            arg0->unk4.y = temp_v0 + 0x100000;
        }

        arg0->vel.y = arg0->unk4.y - savedVec.y;

        func_80053160_53D60(arg0);

        s0 = arg0->vel.z;
        var_s3 = func_8005BF50_5CB50(&arg0->unk4, atan2Fixed(arg0->vel.x, s0), arg0->unk42, 0x1E00000, 0x1E0000);
        if ((var_s3 << 16) != 0) {
            var_s3 &= 0x1FFF;
            if (var_s3 >= 0x1001) {
                var_s3 |= 0xE000;
            }

            if ((s16)arg0->unk46 < var_s3) {
                var_s3 = arg0->unk46;
            }

            if (var_s3 < -(s16)arg0->unk46) {
                var_s3 = -(s16)arg0->unk46;
            }

            memcpy(&rotateVec, &arg0->vel, sizeof(Vec3i));
            rotateVectorY(&rotateVec, var_s3, (Vec3i *)&arg0->vel);
        }

        if (alloc->unk10[arg0->unk42].unkBD9 == 0) {
            if ((s16)arg0->unk46 < 0x13) {
                arg0->unk46 += 2;
            }
        }
    }

    if (arg0->unk4E != 0) {
        func_80050ECC_51ACC(&arg0->unk4);
        func_80056B7C_5777C(&arg0->unk4, 0xD);
        func_80069CF8_6A8F8();
    }

    for (i = 0; i < 4; i++) {
        func_80066444_67044(i, (void *)arg0);
    }
}

void func_80053784_54384(Struct_52880 *arg0) {
    arg0->unk20 = freeNodeMemory(arg0->unk20);
}

s32 func_800537B0_543B0(s32 arg0, s32 arg1) {
    Struct_52880 *task;

    task = scheduleTask(func_800530BC_53CBC, (arg0 + 4) & 0xFF, 0, 0x6F);
    if (task != NULL) {
        task->unk42 = arg0;
    }
    return (s32)task;
}

void func_800537F4_543F4(Struct_52880 *arg0) {
    arg0->unk4C = arg0->unk42;
    arg0->unk20 = load_3ECE40();
    setCleanupCallback(func_800523EC_52FEC);
    setCallbackWithContinue(func_8005383C_5443C);
}

void func_8005383C_5443C(Struct_52880 *arg0) {
    Alloc_52880 *alloc = getCurrentAllocation();
    void *ptr;
    loadAssetMetadata((loadAssetMetadata_arg *)arg0, arg0->unk20, 6);
    ptr = alloc->unk44;
    arg0->unk4E = 0;
    arg0->unk46 = 0;
    arg0->unk0 = ptr;
    setCallbackWithContinue(func_80053990_54590);
}

void func_80053898_54498(Struct_52880 *arg0) {
    GameState *allocation;
    Player *player;
    Vec3i localVec;
    Vec3i *s1;
    u8 temp;

    allocation = (GameState *)getCurrentAllocation();
    s1 = &arg0->unk4;
    player = func_8005C454_5D054(s1, arg0->unk42, 0xC0000, &localVec);
    if (player != NULL) {
        if ((player->unkB84 & 0x1000) == 0) {
            func_80058AEC_596EC(player, &localVec);
            arg0->unk4E++;
            if (arg0->unk4C >= 0) {
                func_80059C24_5A824(&allocation->players[arg0->unk4C]);
            }
        } else {
            arg0->unk42 = player->unkBB8;
            temp = player->unkBB8;
            arg0->vel.x = -arg0->vel.x;
            arg0->vel.y = -arg0->vel.y;
            arg0->vel.z = -arg0->vel.z;
            arg0->unk4C = temp;
            func_80050ECC_51ACC(s1);
            func_80056B7C_5777C(s1, 0x20);
        }
    }
}

void func_80053990_54590(Struct_52880 *arg0) {
    Alloc_52880 *alloc;
    s16 playerIdx;
    void *player;
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_a1;
    s32 temp_a2;
    s32 i;

    alloc = getCurrentAllocation();

    if (arg0->unk4A == 0) {
        playerIdx = arg0->unk42;
        player = (void *)((u8 *)alloc->unk10 + (playerIdx * 3048) + 0x950);
        transformVector(alloc->unk48, player, &arg0->unk4);
        arg0->unk3C = 0x1B8000;
    } else {
        playerIdx = arg0->unk42;
        player = (void *)((u8 *)alloc->unk10 + (playerIdx * 3048) + 0x950);
        transformVector((void *)((u8 *)alloc->unk48 + 0x18), player, &arg0->unk4);
        arg0->unk3C = 0x1A8000;
    }

    playerIdx = arg0->unk42;
    player = (void *)((u8 *)alloc->unk10 + (playerIdx * 3048) + 0x950);
    transformVector((void *)((u8 *)alloc->unk48 + 0xC), player, &arg0->vel);

    temp_v0 = arg0->unk4.x;
    temp_a2 = arg0->vel.x;
    temp_v1 = arg0->unk4.y;
    temp_a1 = arg0->vel.y;
    arg0->vel.x = temp_v0 - temp_a2;
    arg0->vel.y = temp_v1 - temp_a1;
    arg0->vel.z = arg0->unk4.z - arg0->vel.z;

    playerIdx = arg0->unk42;
    arg0->unk40 = ((Unk10Element_52880 *)((u8 *)alloc->unk10 + (playerIdx * 3048)))->unkB94;
    arg0->unk48 = 0xF0;

    func_80056B7C_5777C(&arg0->unk4, 0x10);
    setCallback(func_80053B38_54738);
    func_80053898_54498(arg0);

    if (arg0->unk4E != 0) {
        func_80042340_42F40(&arg0->unk4);
        func_80069CF8_6A8F8();
    }

    for (i = 0; i < 4; i++) {
        func_80066444_67044(i, (func_80066444_67044_arg1 *)arg0);
    }
}

void func_80053B38_54738(Struct_52880 *arg0) {
    Alloc_55650 *alloc;
    Vec3i sp18;
    Vec3i savedVec;
    Vec3i rotateVec;
    u8 pad1[32];
    s16 var_s3;
    s32 temp_v0;
    s32 i;

    alloc = (Alloc_55650 *)getCurrentAllocation();

    if (alloc->unk76 == 0) {

        func_80050604_51204(&arg0->unk4, &arg0->vel, 6);

        arg0->vel.y += 0xFFFC0000;

        func_80051C80_52880((Vec3i *)&arg0->vel, arg0->unk3C);

        memcpy(&savedVec, &arg0->unk4, sizeof(Vec3i));

        arg0->unk4.x += arg0->vel.x;
        arg0->unk4.y += arg0->vel.y;
        arg0->unk4.z += arg0->vel.z;

        arg0->unk40 = func_80060A3C_6163C(&alloc->unk30, arg0->unk40, &arg0->unk4);

        func_80060CDC_618DC(&alloc->unk30, arg0->unk40, &arg0->unk4, 0x80000, &sp18);

        if ((sp18.x != 0) || (sp18.z != 0)) {
            arg0->unk4.x = arg0->unk4.x + sp18.x;
            arg0->unk4.z = arg0->unk4.z + sp18.z;
            arg0->unk4E = arg0->unk4E + 1;
        }

        arg0->unk48--;
        if (arg0->unk48 == 0) {
            arg0->unk4E++;
        }

        temp_v0 = func_8005CFC0_5DBC0(&alloc->unk30, arg0->unk40, &arg0->unk4, 0x100000);

        if (arg0->unk4.y < temp_v0 + 0x100000) {
            arg0->unk4.y = temp_v0 + 0x100000;
        }

        arg0->vel.x = arg0->unk4.x - savedVec.x;
        arg0->vel.y = arg0->unk4.y - savedVec.y;
        arg0->vel.z = arg0->unk4.z - savedVec.z;

        func_80053898_54498(arg0);

        var_s3 =
            func_8005BF50_5CB50(&arg0->unk4, atan2Fixed(arg0->vel.x, arg0->vel.z), arg0->unk42, 0x1800000, 0x1B8000);

        if ((var_s3 << 16) != 0) {
            var_s3 &= 0x1FFF;
            if (var_s3 >= 0x1001) {
                var_s3 |= 0xE000;
            }

            if ((s16)arg0->unk46 < var_s3) {
                var_s3 = arg0->unk46;
            }

            if (var_s3 < -(s16)arg0->unk46) {
                var_s3 = -(s16)arg0->unk46;
            }

            memcpy(&rotateVec, &arg0->vel, sizeof(Vec3i));
            rotateVectorY(&rotateVec, var_s3, (Vec3i *)&arg0->vel);
        }

        if ((s16)arg0->unk46 < 0x19) {
            arg0->unk46 += 3;
        }
    }

    if (arg0->unk4E != 0) {
        func_80042340_42F40(&arg0->unk4);
        func_80069CF8_6A8F8();
    }

    i = 0;
    do {
        func_80066444_67044(i, (func_80066444_67044_arg1 *)arg0);
        i++;
    } while (i < 4);
}

s32 func_80053DF0_549F0(s32 arg0, s32 arg1) {
    Struct_52880 *task;

    task = scheduleTask(func_800537F4_543F4, (arg0 + 4) & 0xFF, 0, 0x6F);
    if (task != NULL) {
        task->unk42 = arg0;
        task->unk4A = arg1;
    }
    return (s32)task;
}

void func_80053E48_54A48(Struct_52880 *arg0) {
    arg0->unk4C = arg0->unk42;
    arg0->unk20 = load_3ECE40();
    setCleanupCallback(func_800523EC_52FEC);
    setCallbackWithContinue(func_80053E90_54A90);
}

void func_80053E90_54A90(Struct_52880 *arg0) {
    Alloc_52880 *alloc = getCurrentAllocation();
    void *ptr;
    loadAssetMetadata((loadAssetMetadata_arg *)arg0, arg0->unk20, 7);
    ptr = alloc->unk44;
    arg0->unk4E = 0;
    arg0->unk46 = 0;
    arg0->unk0 = ptr;
    setCallbackWithContinue(func_80053FE0_54BE0);
}

void func_80053EEC_54AEC(Struct_52880 *arg0) {
    GameState *alloc;
    Player *result;
    void *s1;
    s16 unk4C;
    s32 v28;
    s32 new_var;
    s32 v2C;
    Struct_52880 *new_var3;
    s32 v24;
    s32 new_var2;

    alloc = (GameState *)getCurrentAllocation();
    new_var3 = arg0;
    s1 = &arg0->unk4;
    result = func_8005B548_5C148((Vec3i *)s1, new_var3->unk42, 0x80000);
    if (result == 0) {
        return;
    }

    new_var2 = result->unkB84 & 0x1000;
    if (new_var2 == 0) {
        func_80058A94_59694(result);
        arg0->unk4E++;
        unk4C = arg0->unk4C;
        if (unk4C >= 0) {
            func_80059C24_5A824(&alloc->players[unk4C]);
        }
    } else {
        v28 = arg0->vel.y;
        v2C = (new_var = arg0->vel.z);
        arg0->unk42 = result->unkBB8;
        arg0->unk4C = result->unkBB8;
        arg0->vel.y = -v28;
        v24 = arg0->vel.x;
        arg0->vel.x = -v24;
        arg0->vel.z = -v2C;
        func_80050ECC_51ACC(s1);
        func_80056B7C_5777C(s1, 0x20);
    }
}

void func_80053FE0_54BE0(Struct_52880 *arg0) {
    Alloc_52880 *alloc;
    s16 playerIdx;
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_a1;
    s32 temp_a2;
    s32 i;

    alloc = getCurrentAllocation();

    playerIdx = arg0->unk42;
    transformVector(alloc->unk48, &alloc->unk10[playerIdx].unk950, &arg0->unk4);

    playerIdx = arg0->unk42;
    transformVector(&alloc->unk48[6], &alloc->unk10[playerIdx].unk950, &arg0->vel);

    temp_v0 = arg0->unk4.x;
    temp_a2 = arg0->vel.x;
    temp_v1 = arg0->unk4.y;
    temp_a1 = arg0->vel.y;
    arg0->vel.x = temp_v0 - temp_a2;
    arg0->vel.y = temp_v1 - temp_a1;
    arg0->vel.z = arg0->unk4.z - arg0->vel.z;

    playerIdx = arg0->unk42;
    arg0->unk40 = alloc->unk10[playerIdx].unkB94;
    arg0->unk48 = 0xF0;

    func_80056B7C_5777C(&arg0->unk4, 0x10);
    setCallback(func_80054144_54D44);
    func_80053EEC_54AEC(arg0);

    if (arg0->unk4E != 0) {
        func_80050ECC_51ACC(&arg0->unk4);
        func_80056B7C_5777C(&arg0->unk4, 0xD);
        func_80069CF8_6A8F8();
    }

    for (i = 0; i < 4; i++) {
        func_80066444_67044(i, (func_80066444_67044_arg1 *)arg0);
    }
}

void func_80054144_54D44(Struct_52880 *arg0) {
    Alloc_55650 *alloc;
    Vec3i sp18;
    Vec3i savedVec;
    Vec3i rotateVec;
    s32 pad[8];
    s16 var_s3;
    s32 temp_v0;
    s32 i;

    alloc = (Alloc_55650 *)getCurrentAllocation();

    if (alloc->unk76 == 0) {

        func_80050604_51204(&arg0->unk4, (Vec3i *)&arg0->vel, 7);

        arg0->vel.y += 0xFFFC0000;

        func_80051C80_52880((Vec3i *)&arg0->vel, 0x198000);

        memcpy(&savedVec, &arg0->unk4, sizeof(Vec3i));

        arg0->unk4.x += arg0->vel.x;
        arg0->unk4.y += arg0->vel.y;
        arg0->unk4.z += arg0->vel.z;

        arg0->unk40 = func_80060A3C_6163C(alloc->unk30, arg0->unk40, &arg0->unk4);

        func_80060CDC_618DC(alloc->unk30, arg0->unk40, &arg0->unk4, 0x80000, &sp18);

        if ((sp18.x != 0) || (sp18.z != 0)) {
            arg0->unk4.x = arg0->unk4.x + sp18.x;
            arg0->unk4.z = arg0->unk4.z + sp18.z;
            temp_v0 = atan2Fixed(-sp18.x, -sp18.z);
            rotateVectorY((Vec3i *)&arg0->vel, -temp_v0, &rotateVec);
            if (rotateVec.z < 0) {
                rotateVec.z = -rotateVec.z;
            }
            rotateVectorY(&rotateVec, temp_v0, (Vec3i *)&arg0->vel);
            arg0->unk42 = -1;
        } else {
            arg0->vel.x = arg0->unk4.x - savedVec.x;
            arg0->vel.z = arg0->unk4.z - savedVec.z;
        }

        arg0->unk48--;
        if (arg0->unk48 == 0) {
            arg0->unk4E++;
        }

        temp_v0 = func_8005CFC0_5DBC0(alloc->unk30, arg0->unk40, &arg0->unk4, 0x100000);

        if (arg0->unk4.y < temp_v0 + 0x100000) {
            arg0->unk4.y = temp_v0 + 0x100000;
        }

        arg0->vel.y = arg0->unk4.y - savedVec.y;

        func_80053EEC_54AEC(arg0);

        var_s3 =
            func_8005BF50_5CB50(&arg0->unk4, atan2Fixed(arg0->vel.x, arg0->vel.z), arg0->unk42, 0x1200000, 0x198000);

        if ((var_s3 << 16) != 0) {
            var_s3 &= 0x1FFF;
            if (var_s3 >= 0x1001) {
                var_s3 |= 0xE000;
            }

            if ((s16)arg0->unk46 < var_s3) {
                var_s3 = arg0->unk46;
            }

            if (var_s3 < -(s16)arg0->unk46) {
                var_s3 = -(s16)arg0->unk46;
            }

            memcpy(&rotateVec, (Vec3i *)&arg0->vel, sizeof(Vec3i));
            rotateVectorY(&rotateVec, var_s3, (Vec3i *)&arg0->vel);
        }

        if ((s16)arg0->unk46 < 0x19) {
            arg0->unk46 += 3;
        }
    }

    if (arg0->unk4E != 0) {
        func_80050ECC_51ACC(&arg0->unk4);
        func_80056B7C_5777C(&arg0->unk4, 0xD);
        func_80069CF8_6A8F8();
    }

    i = 0;
    do {
        func_80066444_67044(i, (func_80066444_67044_arg1 *)arg0);
        i++;
    } while (i < 4);
}

s32 func_80054470_55070(s32 arg0, s32 arg1) {
    Struct_52880 *task;

    task = scheduleTask(func_80053E48_54A48, (arg0 + 4) & 0xFF, 0, 0x6F);
    if (task != NULL) {
        task->unk42 = arg0;
    }
    return (s32)task;
}

s32 func_800544B4_550B4(s32 arg0, s32 arg1, s32 arg2) {
    switch (arg0) {
        case 0:
            return func_80052418_53018(arg1, arg2);
        case 1:
            return func_80052A24_53624(arg1, arg2);
        case 2:
            return func_80053078_53C78(arg1, arg2);
        case 3:
            return func_800537B0_543B0(arg1, arg2);
        case 4:
            return func_80053DF0_549F0(arg1, arg2);
        case 5:
            return func_80054470_55070(arg1, arg2);
        case 6:
            return func_80055820_56420(arg1, arg2);
    }
    return 0;
}

void func_80054568_55168(Struct_52880 *arg0) {
    getCurrentAllocation();
    if (func_8005C250_5CE50((&arg0->unk4), arg0->unk42, 0xC0000)) {
        arg0->unk4E++;
    }
}

void func_800545B0_551B0(Struct_52880 *arg0) {
    arg0->unk4C = arg0->unk42;
    arg0->unk20 = load_3ECE40();
    setCleanupCallback(func_800523EC_52FEC);
    setCallbackWithContinue(func_800545F8_551F8);
}

void func_800545F8_551F8(Struct_52880 *arg0) {
    Alloc_52880 *alloc;
    void *ptr;
    s32 pad[4];

    alloc = getCurrentAllocation();
    loadAssetMetadata((loadAssetMetadata_arg *)arg0, arg0->unk20, 6);
    ptr = alloc->unk44;
    arg0->unk40 = 0x65;
    arg0->unk4E = 0;
    arg0->unk0 = ptr;
    setCallbackWithContinue(func_80054658_55258);
}

void func_80054658_55258(Struct_52880 *arg0) {
    Alloc_55650 *alloc;
    Vec3i sp18;
    s32 pad1[15]; /* 60 bytes of padding */
    void *s1;
    s32 temp_v0;
    s32 i;

    alloc = (Alloc_55650 *)getCurrentAllocation();

    if (alloc->unk76 == 0) {
        func_80050604_51204(&arg0->unk4, (Vec3i *)&arg0->vel, 6);

        arg0->vel.y -= 0x3000;
        s1 = &alloc->unk30;
        arg0->unk4.x += arg0->vel.x;
        arg0->unk4.y += arg0->vel.y;
        arg0->unk4.z += arg0->vel.z;

        arg0->unk40 = func_80060A3C_6163C(s1, arg0->unk40, &arg0->unk4);
        func_80060CDC_618DC(s1, arg0->unk40, &arg0->unk4, 0x80000, &sp18);

        if ((sp18.x != 0) || (sp18.z != 0)) {
            arg0->unk4.x += sp18.x;
            arg0->unk4.z += sp18.z;
            arg0->unk4E++;
        }

        temp_v0 = func_8005CFC0_5DBC0(&alloc->unk30, arg0->unk40, &arg0->unk4, 0x100000);

        if (arg0->unk4.y < temp_v0 + 0x100000) {
            arg0->unk4.y = temp_v0 + 0x100000;
            arg0->unk4E += 1;
        }

        func_80054568_55168(arg0);
    }

    if (arg0->unk4E != 0) {
        func_80042340_42F40(&arg0->unk4);
        func_80069CF8_6A8F8();
    }

    for (i = 0; i < 4; i++) {
        func_80066444_67044(i, (func_80066444_67044_arg1 *)arg0);
    }
}

void func_800547E0_553E0(s16 arg0, s32 arg1) {
    Struct_52880 *task;
    Vec3i vec;

    task = scheduleTask(func_800545B0_551B0, 0, 0, 0x6F);
    if (task != NULL) {
        vec.x = 0;
        vec.y = 0;
        vec.z = arg1;
        rotateVectorY(&vec, arg0, (Vec3i *)&task->vel.x);
        task->unk4.x = 0xDD12D592;
        task->unk4.y = 0x0B1D4CA3;
        task->unk4.z = 0xE27836C1;
        task->unk42 = -1;
    }
}

void func_80054880_55480(Struct_52880 *arg0) {
    arg0->unk4C = arg0->unk42;
    arg0->unk20 = load_3ECE40();
    setCleanupCallback(func_800523EC_52FEC);
    setCallbackWithContinue(func_800548C8_554C8);
}

void func_800548C8_554C8(Struct_52880 *arg0) {
    Alloc_52880 *alloc;
    s16 playerIdx;
    s32 vel;
    Unk10Element_52880 *players;
    void *position;
    s16 *playerIdxPtr;
    void *velocity;
    s32 pad[4];

    alloc = getCurrentAllocation();
    playerIdxPtr = &arg0->unk42;
    arg0->unk40 = alloc->unk10[arg0->unk42].unkB94;
    loadAssetMetadata((loadAssetMetadata_arg *)arg0, arg0->unk20, 6);

    arg0->unk0 = alloc->unk44;
    arg0->unk4E = 0;
    playerIdx = *playerIdxPtr;
    players = (Unk10Element_52880 *)&arg0->unk4;
    transformVector(D_80090F60_91B60, alloc->unk10[arg0->unk42].unk164, position = players);

    playerIdx = arg0->unk42;
    transformVectorRelative(alloc->unk10[0].unk434, alloc->unk10[arg0->unk42].unk164, velocity = &arg0->vel);

    vel = arg0->vel.z;
    if (vel < 0) {
        arg0->vel.z = -vel;
    }

    vel = (arg0->vel.z = arg0->vel.z / 26);
    if (vel <= 0x1FFFF) {
        arg0->vel.z = 0x20000;
    }
    if (arg0->vel.z > 0x300000) {
        arg0->vel.z = 0x300000;
    }

    D_80090F74_91B74 = D_80090F68_91B68 - arg0->vel.z;

    playerIdx = *playerIdxPtr;
    transformVector(&D_80090F60_91B60[6], alloc->unk10[arg0->unk42].unk164, velocity);

    arg0->vel.x = arg0->unk4.x - arg0->vel.x;
    arg0->vel.y = arg0->unk4.y - arg0->vel.y;
    arg0->vel.z = arg0->unk4.z - arg0->vel.z;
    if ((!alloc->unk10[*playerIdxPtr].unkB94) && (!alloc->unk10[*playerIdxPtr].unkB94)) {}
    players = alloc->unk10;
    memcpy(&arg0->unk30, players[*playerIdxPtr].unk44C, 12);
    func_80056B7C_5777C(position, 0x23);
    setCallbackWithContinue(func_80054AE4_556E4);
}

void func_80054AE4_556E4(Struct_52880 *arg0) {
    Alloc_55650 *alloc;
    Vec3i sp18;
    s32 pad1[15];
    void *s1;
    s32 temp_v0;
    s32 i;

    alloc = (Alloc_55650 *)getCurrentAllocation();

    if (alloc->unk76 == 0) {
        func_80050604_51204(&arg0->unk4, (Vec3i *)&arg0->vel, 6);

        s1 = &alloc->unk30;
        arg0->vel.y -= 0x8000;
        arg0->unk4.x += arg0->vel.x + arg0->unk30;
        arg0->unk4.y += arg0->vel.y + arg0->unk34;
        arg0->unk4.z += arg0->vel.z + arg0->unk38;

        arg0->unk40 = func_80060A3C_6163C(s1, arg0->unk40, &arg0->unk4);
        func_80060CDC_618DC(s1, arg0->unk40, &arg0->unk4, 0x80000, &sp18);

        if ((sp18.x != 0) || (sp18.z != 0)) {
            arg0->unk4.x += sp18.x;
            arg0->unk4.z += sp18.z;
            arg0->unk4E++;
        }

        temp_v0 = func_8005CFC0_5DBC0(&alloc->unk30, arg0->unk40, &arg0->unk4, 0x100000);

        if (arg0->unk4.y < temp_v0 + 0x100000) {
            arg0->unk4.y = temp_v0 + 0x100000;
            arg0->unk4E += 1;
        }

        func_80054568_55168(arg0);
    }

    if (arg0->unk4E != 0) {
        func_800423A4_42FA4(&arg0->unk4, arg0->unk42);
        func_80069CF8_6A8F8();
    }

    for (i = 0; i < 4; i++) {
        func_80066444_67044(i, (func_80066444_67044_arg1 *)arg0);
    }
}

s32 func_80054C8C_5588C(s16 arg0) {
    Struct_52880 *task;
    s32 pad[4];

    task = scheduleTask(func_80054880_55480, 0, 0, 0x6F);
    if (task != NULL) {
        task->unk42 = arg0;
    }
}

void func_80054CCC_558CC(func_80054CCC_558CC_arg *arg0) {
    arg0->unk20 = load_3ECE40();
    setCleanupCallback(func_800553A8_55FA8);
    setCallbackWithContinue(func_80054D0C_5590C);
}

void func_80054D0C_5590C(Struct_52880 *arg0) {
    Alloc_52880 *alloc = getCurrentAllocation();
    void *ptr;
    loadAssetMetadata((loadAssetMetadata_arg *)arg0, arg0->unk20, 0x69);
    ptr = alloc->unk44;
    arg0->unk4E = 0;
    arg0->unk46 = 0;
    arg0->unk0 = ptr;
    arg0->unk4C = arg0->unk42;
    setCallbackWithContinue(func_80054F44_55B44);
}

void func_80054D70_55970(void *arg) {
    Struct_52880 *arg0 = arg;
    GameState *alloc;
    Player *player;
    s32 i;
    s16 unk4C;
    u32 randVal;

    alloc = (GameState *)getCurrentAllocation();
    randVal = ((u32)(randA() & 0xFF) % 5U) & 0xFF;

    switch (randVal) {
        case 0:
            player = func_8005B548_5C148((&arg0->unk4), arg0->unk42, 0x80000);
            if (player != NULL) {
                func_80058A10_59610(player);
                for (i = 0; i < 3; i++) {
                    if (player->unkB6C >= 100) {
                        func_8004A9A8_4B5A8(&player->worldPos.x, player->unkB94, &player->unk44C);
                        func_80059A48_5A648(player, -100);
                    }
                }
                arg0->unk4E = arg0->unk4E + 1;
                unk4C = arg0->unk4C;
                if (unk4C >= 0) {
                    func_80059C24_5A824(&alloc->players[unk4C]);
                }
            }
            break;
        case 1:
            player = func_8005B548_5C148((&arg0->unk4), arg0->unk42, 0x80000);
            if (player != NULL) {
                func_80058A3C_5963C(player);
                arg0->unk4E = arg0->unk4E + 1;
                unk4C = arg0->unk4C;
                if (unk4C >= 0) {
                    func_80059C24_5A824(&alloc->players[unk4C]);
                }
            }
            break;
        case 2:
            player = func_8005B548_5C148((&arg0->unk4), arg0->unk42, 0x80000);
            if (player != NULL) {
                func_80058A68_59668(player);
                arg0->unk4E = arg0->unk4E + 1;
                unk4C = arg0->unk4C;
                if (unk4C >= 0) {
                    func_80059C24_5A824(&alloc->players[unk4C]);
                }
            }
            break;
        case 3:
            player = func_8005B548_5C148((&arg0->unk4), arg0->unk42, 0x80000);
            if (player != NULL) {
                func_80058AC0_596C0(player);
                arg0->unk4E = arg0->unk4E + 1;
                unk4C = arg0->unk4C;
                if (unk4C >= 0) {
                    func_80059C24_5A824(&alloc->players[unk4C]);
                }
            }
            break;
        case 4:
            player = func_8005B548_5C148((&arg0->unk4), arg0->unk42, 0x80000);
            if (player != NULL) {
                func_80058A94_59694(player);
                arg0->unk4E = arg0->unk4E + 1;
                unk4C = arg0->unk4C;
                if (unk4C >= 0) {
                    func_80059C24_5A824(&alloc->players[unk4C]);
                }
            }
            break;
    }
}

void func_80054F44_55B44(Struct_52880 *arg0) {
    Alloc_52880 *alloc;
    s32 playerIdx;
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_a0;
    s32 temp_a1;
    s32 temp_a2;
    s32 temp_a3;
    s32 i;
    Vec3i *s1;

    alloc = getCurrentAllocation();
    transformVector(D_80090F60_91B60, alloc->unk10[arg0->unk42].unk164, s1 = &arg0->unk4);

    transformVector(&D_80090F60_91B60[12], alloc->unk10[arg0->unk42].unk164, &arg0->vel);

    temp_v0 = arg0->unk4.x;
    temp_a1 = arg0->vel.x;
    temp_v1 = arg0->unk4.y;
    temp_a2 = arg0->vel.y;
    temp_a0 = arg0->unk4.z;
    temp_a3 = arg0->vel.z;
    playerIdx = arg0->unk42;
    arg0->vel.x = temp_v0 - temp_a1;
    arg0->vel.y = temp_v1 - temp_a2;
    arg0->vel.z = temp_a0 - temp_a3;

    arg0->unk40 = alloc->unk10[playerIdx].unkB94;
    arg0->unk48 = 0xF0;

    setCallback(func_800550B4_55CB4);
    func_80054D70_55970(arg0);

    func_80056B7C_5777C(s1, 0x23);

    if (arg0->unk4E != 0) {
        func_80050ECC_51ACC(s1);
        func_80056B7C_5777C(s1, 0x11);
        func_80069CF8_6A8F8();
    }

    for (i = 0; i < 4; i++) {
        func_80066444_67044(i, (func_80066444_67044_arg1 *)arg0);
    }
}

void func_800550B4_55CB4(func_800550B4_55CB4_arg *arg0) {
    Alloc_55650 *alloc;
    Vec3i sp18;
    Vec3i savedVec;
    Vec3i rotateVec;
    u8 pad1[32];
    s16 angle;
    s16 var_s3;
    s32 i;
    s32 temp_v0;
    s32 temp_v1;

    alloc = (Alloc_55650 *)getCurrentAllocation();

    if (alloc->unk76 == 0) {

        func_80050604_51204(&arg0->unk4, &arg0->unk24, 0x6A);

        arg0->unk24.y += 0xFFFC0000;

        func_80051C80_52880(&arg0->unk24, 0x1C0000);

        memcpy(&savedVec, &arg0->unk4, sizeof(Vec3i));

        arg0->unk4.x += arg0->unk24.x;
        arg0->unk4.y += arg0->unk24.y;
        arg0->unk4.z += arg0->unk24.z;

        angle = func_80060A3C_6163C(&alloc->unk30, arg0->unk40, &arg0->unk4);
        arg0->unk40 = angle;

        func_80060CDC_618DC(&alloc->unk30, angle, &arg0->unk4, 0x80000, &sp18);

        if (sp18.x != 0 || sp18.z != 0) {
            arg0->unk4.x += sp18.x;
            arg0->unk4.z += sp18.z;
            arg0->unk4E = arg0->unk4E + 1;
        }

        arg0->unk48--;
        if (arg0->unk48 == 0) {
            arg0->unk4E++;
        }

        temp_v0 = func_8005CFC0_5DBC0(&alloc->unk30, arg0->unk40, &arg0->unk4, 0x100000);
        temp_v1 = temp_v0 + 0x100000;

        if (arg0->unk4.y < temp_v1) {
            arg0->unk4.y = temp_v1;
        }

        arg0->unk24.x = arg0->unk4.x - savedVec.x;
        arg0->unk24.y = arg0->unk4.y - savedVec.y;
        arg0->unk24.z = arg0->unk4.z - savedVec.z;

        func_80054D70_55970(arg0);

        angle = atan2Fixed(arg0->unk24.x, arg0->unk24.z);
        var_s3 = func_8005BF50_5CB50(&arg0->unk4, angle, arg0->unk42, 0x3C00000, 0x1B0000);

        if (var_s3 != 0) {
            var_s3 &= 0x1FFF;
            if (var_s3 >= 0x1001) {
                var_s3 |= 0xE000;
            }

            if (arg0->unk46 < var_s3) {
                var_s3 = arg0->unk46;
            }

            if (var_s3 < -arg0->unk46) {
                var_s3 = -arg0->unk46;
            }

            memcpy(&rotateVec, &arg0->unk24, sizeof(Vec3i));
            rotateVectorY(&rotateVec, var_s3, &arg0->unk24);
        }

        if (alloc->unk86 != 0) {
            if (arg0->unk46 < 0x20) {
                arg0->unk46 += 4;
            }
        } else {
            if (arg0->unk46 < 0xC) {
                arg0->unk46++;
            }
        }
    }
    if (arg0->unk4E != 0) {
        func_80050ECC_51ACC(&arg0->unk4);
        func_80056B7C_5777C(&arg0->unk4, 0x11);
        func_80069CF8_6A8F8();
    }

    for (i = 0; i < 4; i++) {
        func_80066444_67044(i, (func_80066444_67044_arg1 *)arg0);
    }
}

void func_800553A8_55FA8(func_80054CCC_558CC_arg *arg0) {
    arg0->unk20 = freeNodeMemory(arg0->unk20);
}

s32 func_800553D4_55FD4(s32 arg0) {
    Struct_52880 *task;

    task = scheduleTask(func_80054CCC_558CC, (arg0 + 4) & 0xFF, 0, 0x6E);
    if (task != NULL) {
        task->unk42 = arg0;
    }
}

void func_80055418_56018(Struct_52880 *arg0) {
    arg0->unk4C = arg0->unk42;
    arg0->unk20 = load_3ECE40();
    setCleanupCallback(func_800523EC_52FEC);
    setCallbackWithContinue(func_80055460_56060);
}

void func_80055460_56060(Struct_52880 *arg0) {
    Alloc_52880 *alloc = getCurrentAllocation();
    void *ptr;
    loadAssetMetadata((loadAssetMetadata_arg *)arg0, arg0->unk20, 0x6E);
    ptr = alloc->unk44;
    arg0->unk4E = 0;
    arg0->unk46 = 0;
    arg0->unk0 = ptr;
    setCallbackWithContinue(func_800554FC_560FC);
}

void func_800554BC_560BC(Struct_52880 *arg0) {
    if (func_800BB504(&arg0->unk4, 0x80000)) {
        arg0->unk4E++;
    }
}

void func_800554FC_560FC(Struct_52880 *arg0) {
    Alloc_52880 *alloc;
    s16 index;
    s32 i;

    alloc = getCurrentAllocation();

    index = arg0->unk42;
    transformVector(alloc->unk48, &alloc->unk10[index].unk950, &arg0->unk4);

    index = arg0->unk42;
    transformVector(&alloc->unk48[6], &alloc->unk10[index].unk950, &arg0->vel);

    arg0->vel.x = arg0->unk4.x - arg0->vel.x;
    arg0->vel.y = arg0->unk4.y - arg0->vel.y;
    arg0->vel.z = arg0->unk4.z - arg0->vel.z;

    index = arg0->unk42;
    arg0->unk40 = alloc->unk10[index].unkB94;

    func_80056B7C_5777C(&arg0->unk4, 0x10);
    setCallback(func_80055650_56250);
    func_800554BC_560BC(arg0);

    if (arg0->unk4E != 0) {
        func_80056B7C_5777C(&arg0->unk4, 0x43);
        func_80069CF8_6A8F8();
    }

    for (i = 0; i < 4; i++) {
        func_80066444_67044(i, (func_80066444_67044_arg1 *)arg0);
    }
}

void func_80055650_56250(Struct_52880 *arg0) {
    Vec3i sp18;
    s32 pad1;
    Vec3i savedVec;
    s32 pad2[12];
    Alloc_55650 *s3;
    Vec3i *s1;
    Vec3i *s0;
    s16 temp_v0_sh;
    s32 temp_v0;
    s32 i;

    s3 = (Alloc_55650 *)getCurrentAllocation();

    if (s3->unk76 != 0) {
        goto skip_main;
    }

    s1 = &arg0->unk4;
    s0 = &arg0->vel;

    func_80050604_51204(s1, s0, 0x6E);

    arg0->vel.y += 0xFFFC0000;

    func_80051C80_52880(s0, 0x1D0000);

    memcpy(&savedVec, s1, sizeof(Vec3i));

    arg0->unk4.x += arg0->vel.x;
    arg0->unk4.y += arg0->vel.y;
    arg0->unk4.z += arg0->vel.z;

    temp_v0_sh = func_80060A3C_6163C(&s3->unk30, arg0->unk40, s1);

    arg0->unk40 = temp_v0_sh;

    func_80060CDC_618DC(&s3->unk30, temp_v0_sh & 0xFFFF, s1, 0x80000, &sp18);

    if ((sp18.x != 0) || (sp18.z != 0)) {
        arg0->unk4.x += sp18.x;
        arg0->unk4.z += sp18.z;
        arg0->unk4E++;
    }

    temp_v0 = func_8005CFC0_5DBC0(&s3->unk30, arg0->unk40, &arg0->unk4, 0x100000);

    if (arg0->unk4.y < (temp_v0 + 0x100000)) {
        arg0->unk4.y = temp_v0 + 0x100000;
    }

    arg0->vel.x = arg0->unk4.x - savedVec.x;
    arg0->vel.y = arg0->unk4.y - savedVec.y;
    arg0->vel.z = arg0->unk4.z - savedVec.z;

    func_800554BC_560BC(arg0);

skip_main:
    if (arg0->unk4E != 0) {
        func_80056B7C_5777C(&arg0->unk4, 0x43);
        func_80069CF8_6A8F8();
    }

    for (i = 0; i < 4; i++) {
        func_80066444_67044(i, (func_80066444_67044_arg1 *)arg0);
    }
}

s32 func_80055820_56420(s32 arg0, s32 arg1) {
    Struct_52880 *task;

    task = scheduleTask(func_80055418_56018, (arg0 + 4) & 0xFF, 0, 0x6F);
    if (task != NULL) {
        task->unk42 = arg0;
    }
    return (s32)task;
}

void func_80055864_56464(func_80055864_56464_arg *arg0) {
    arg0->unk20 = load_3ECE40();
    setCleanupCallback(func_800523EC_52FEC);
    setCallbackWithContinue(func_800558A4_564A4);
}

void func_800558A4_564A4(Struct_52880 *arg0) {
    Alloc_52880 *alloc = getCurrentAllocation();
    void *ptr;
    loadAssetMetadata((loadAssetMetadata_arg *)arg0, arg0->unk20, 4);
    ptr = alloc->unk44;
    arg0->unk4E = 0;
    arg0->unk46 = 0;
    arg0->unk0 = ptr;
    setCallbackWithContinue(func_80055964_56564);
}

void func_80055900_56500(Struct_52880 *arg0) {
    Player *result;

    getCurrentAllocation();
    result = func_8005B548_5C148((&arg0->unk4), arg0->unk42, 0x80000);
    if (result != NULL) {
        if ((result->unkB84 & 0x1000) == 0) {
            func_80058A68_59668(result);
            arg0->unk4E++;
        }
    }
}

void func_80055964_56564(Struct_52880 *arg0) {
    Alloc_52880 *alloc;
    Vec3i localVec;
    s32 i;
    Vec3i *s0;

    alloc = getCurrentAllocation();

    if (arg0->unk4A == 0) {
        rotateVectorY((u8 *)alloc->unk48 + 0x210, arg0->unk44, &localVec);
    } else {
        rotateVectorY((u8 *)alloc->unk48 + 0x21C, arg0->unk44, &localVec);
    }

    rotateVectorY((u8 *)alloc->unk48 + 0x228, arg0->unk44, (Vec3i *)&arg0->vel);

    arg0->unk4.x += localVec.x;
    arg0->unk4.y += localVec.y;
    arg0->unk4.z += localVec.z;
    s0 = &arg0->unk4;
    arg0->unk40 = 60;
    arg0->unk48 = 45;

    func_80056B7C_5777C(s0, 16);
    setCallback(func_80055A84_56684);
    func_80055900_56500(arg0);

    if (arg0->unk4E != 0) {
        func_80050ECC_51ACC(s0);
        func_80056B7C_5777C(s0, 13);
        func_80069CF8_6A8F8();
    }
    i = 0;

    do {
        func_80066444_67044(i, (func_80066444_67044_arg1 *)arg0);
        i++;
    } while (i < 4);
}

void func_80055A84_56684(Struct_52880 *arg0) {
    Vec3i offset;
    Vec3i prevPos;
    s32 padding1[12];
    Alloc_55650 *alloc;
    Vec3i *vel;
    s32 i;
    s32 floorHeight;
    u16 temp;

    alloc = getCurrentAllocation();

    if (alloc->unk76 == 0) {
        vel = &arg0->vel;
        func_80050604_51204(&arg0->unk4, vel, 4);
        arg0->vel.y += 0xFFFC0000;
        func_80051C80_52880(vel, 0x190000);
        memcpy(&prevPos, &arg0->unk4, 0xC);

        arg0->unk4.x += arg0->vel.x;
        arg0->unk4.y += arg0->vel.y;
        arg0->unk4.z += arg0->vel.z;

        temp = func_80060A3C_6163C(&alloc->unk30, arg0->unk40, &arg0->unk4);
        arg0->unk40 = temp;
        func_80060CDC_618DC(&alloc->unk30, temp, &arg0->unk4, 0x80000, &offset);

        if (offset.x != 0 || offset.z != 0) {
            arg0->unk4.x += offset.x;
            arg0->unk4.z += offset.z;
            arg0->unk4E++;
        }

        temp = arg0->unk48 - 1;
        arg0->unk48 = temp;
        if ((temp << 16) == 0) {
            arg0->unk4E++;
        }

        floorHeight = func_8005CFC0_5DBC0(&alloc->unk30, arg0->unk40, &arg0->unk4, 0x100000) + 0x100000;
        if (arg0->unk4.y < floorHeight) {
            arg0->unk4.y = floorHeight;
        }

        arg0->vel.x = arg0->unk4.x - prevPos.x;
        arg0->vel.y = arg0->unk4.y - prevPos.y;
        arg0->vel.z = arg0->unk4.z - prevPos.z;
        func_80055900_56500(arg0);
    }

    if (arg0->unk4E != 0) {
        func_80050ECC_51ACC(&arg0->unk4);
        func_80056B7C_5777C(&arg0->unk4, 0xD);
        func_80069CF8_6A8F8();
    }

    i = 0;
    do {
        func_80066444_67044(i, (func_80066444_67044_arg1 *)arg0);
        i++;
    } while (i < 4);
}

Struct_52880 *func_80055C80_56880(s32 arg0, s16 arg1, void *arg2) {
    Struct_52880 *task;

    task = scheduleTask(func_80055864_56464, 0, 0, 0x6F);
    if (task != NULL) {
        task->unk42 = -1;
        task->unk4A = arg0;
        task->unk44 = arg1;
        memcpy(&task->unk4, arg2, sizeof(Vec3i));
    }
    return task;
}
