#include "36B80.h"
#include "36BE0.h"
#include "D_800AFE8C_A71FC_type.h"
#include "common.h"
#include "graphics.h"
#include "overlay.h"
#include "task_scheduler.h"

extern s32 gControllerInputs;

USE_ASSET(_45A890);

typedef struct {
    s16 unk0;
    s16 unk2;
    s32 unk4;
    s16 unk8;
    u8 padding[0x6];
    u8 unk10;
    u8 unk11;
    u8 unk12;
} func_80036920_37520_arg;

typedef struct {
    u8 padding0[0x10];
    u8 unk10;
    u8 padding1[0x62];
    u8 unk73;
    u8 padding2[0x38F];
    u8 unk403;
    u8 padding3[0x4];
    s32 unk408[1];
    u8 padding4[0x4];
    s32 unk410[1];
    u8 padding5[0x8];
    s32 numEntries;
    u8 unk420;
    u8 unk421;
    u8 unk422;
    u8 padding6[0x1];
    u8 unk424;
    u8 unk425;
    u8 padding7[0x4];
    u8 unk42A;
    u8 padding8[0x2];
    u8 unk42D;
} func_80036274_36E74_large_struct;

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s8 unk12;
    s8 unk13;
} func_80036328_36F28_arg_item;

typedef struct {
    func_80036328_36F28_arg_item entries[4];
    char __padding[6];
    void *unk58;
    s16 unk5C;
    s16 unk5E;
    u8 unk60;
    char __padding2[3];
    s16 unk64;
    s16 unk66;
    s16 unk68;
    s16 unk6A;
    s16 unk6C;
    s16 unk6E;
    s16 unk70;
    s8 unk72;
} func_80036328_36F28_arg;

typedef struct {
    char padding[64];
    void *unk40;
} func_800371CC_37DCC_arg;

typedef struct {
    u8 padding[1012];
    s16 unk3F4;
    s32 unk3F8;
    u8 padding2[7];
    u8 unk403;
    u8 padding3[28];
    u8 unk420;
    u8 padding5[3];
    u8 unk424;
    u8 unk425;
    u8 padding4[7];
    u8 unk42D;
} func_800698BC_6A4BC_return;

typedef struct {
    char padding;
    s32 *unk4;
    char padding2[78];
    s32 *unk58;
} func_80036848_37448_arg;

typedef struct {
    u8 unk0;
    u8 unk1;
} func_80036A3C_3763C_arg;

typedef struct {
    char padding[4];
    s32 *unk4;
} func_80036A10_37610_arg;

typedef struct {
    u16 unk0;
    u16 unk2;
    void *unk4;
    u16 unk8;
    u16 unkA;
    u8 unkC;
    u8 unkD;
    char padding[2];
    u8 unk10;
    u8 unk11;
    u8 unk12;
} func_80036880_37480_arg;

typedef struct {
    u8 padding0[0x34];
    s32 unk34;
    u8 padding1[0x4];
    s32 unk3C;
} func_80036C20_37820_arg;

void func_80036274_36E74(void *);
void func_80036424_37024(void *);
void func_80036848_37448(void *);
void func_80036328_36F28(func_80036328_36F28_arg *);
void func_80036880_37480(func_80036880_37480_arg *);
void func_80036A68_37668(void *);
void func_80036A10_37610(void *);

typedef struct {
    s32 unk0;
    s32 unk4;
} D_8008F810_90410_item;
extern D_8008F810_90410_item D_8008F810_90410[0x18];
extern s32 func_80012004_12C04;
extern void D_8008FAC0_906C0;
extern u16 D_8008FD10_90910[];
extern s8 D_8008FD1C_9091C[];
void func_80036920_37520(func_80036920_37520_arg *arg0);
extern s32 distance_2d(s32, s32);

void func_80036250_36E50(s8 *arg0) {
    *arg0 = 0;
    setCallback(&func_80036274_36E74);
}

void func_80036274_36E74(void *arg0) {
    func_800698BC_6A4BC_return *temp_v0;
    func_80036274_36E74_large_struct *temp_v0_2;
    func_80036274_36E74_large_struct *temp_v0_3;

    temp_v0 = (func_800698BC_6A4BC_return *)getCurrentAllocation();
    if (temp_v0->unk424 != 0) {
        if (*((u8 *)arg0) == 0) {
            if ((u8)temp_v0->unk425 < 0xAU) {
                temp_v0_2 = scheduleTask(&func_80036328_36F28, 0, 0, 0x64);
                if (temp_v0_2 != NULL) {
                    temp_v0_2->unk73 = (u8)temp_v0->unk425;
                }
            } else {
                temp_v0_3 = scheduleTask(&func_80036880_37480, 0, 0, 0x64);
                if (temp_v0_3 != NULL) {
                    temp_v0_3->unk10 = (u8)temp_v0->unk425;
                }
            }
            *(u8 *)arg0 = 1;
        }
    } else {
        *(u8 *)arg0 = 0;
    }
}

void func_80036328_36F28(func_80036328_36F28_arg *arg0) {
    void *resource;
    void *temp_value;
    s32 i;
    s16 var_a0;

    func_80036328_36F28_arg_item *var_v1;

    resource = dmaRequestAndUpdateStateWithSize(&_45A890_ROM_START, &_45A890_ROM_END, 0x3108);
    temp_value = func_80035F80_36B80(1);

    setCleanupCallback(&func_80036848_37448);

    // Initialize main struct fields
    arg0->unk6E = 0x2CC;
    arg0->unk70 = 0x2CC;
    arg0->unk6A = 0x4000;
    arg0->unk6C = 0x4000;
    arg0->unk72 = 0;
    arg0->unk64 = 0;
    arg0->unk66 = 0;
    arg0->unk68 = 0xFF;

    for (i = 0; i < 4; i++) {
        var_v1 = &arg0->entries[i];
        var_v1->unk0 = 0;
        var_v1->unk2 = 0;
        var_v1->unk8 = i;
        var_v1->unk13 = 0;
        var_v1->unk12 = 0;
        var_v1->unkE = 0;
        var_v1->unk10 = 0xFF;
        var_v1->unkA = 0x400;
        var_v1->unkC = 0x400;
        var_v1->unk4 = resource;
    }

    arg0->unk58 = temp_value;
    arg0->unk5C = 0xFF;
    arg0->unk60 = 5;
    arg0->unk5E = 0xFF;

    setCallback(&func_80036424_37024);
}

INCLUDE_ASM("asm/nonmatchings/36E50", func_80036424_37024);

void func_80036848_37448(void *untypedArg) {
    func_80036848_37448_arg *arg0 = (func_80036848_37448_arg *)untypedArg;
    arg0->unk4 = freeNodeMemory(arg0->unk4);
    arg0->unk58 = freeNodeMemory(arg0->unk58);
}

void func_80036880_37480(func_80036880_37480_arg *arg0) {
    void *resource;

    resource = dmaRequestAndUpdateStateWithSize(&_45A890_ROM_START, &_45A890_ROM_END, 0x3108);
    setCleanupCallback(&func_80036A10_37610);
    arg0->unk8 = 8;
    arg0->unkA = 0xFF;
    arg0->unk0 = 0;
    arg0->unk2 = 0;
    arg0->unk4 = resource;
    arg0->unkD = 0;
    if (arg0->unk10 == 0xB) {
        arg0->unkC = 1;
    } else {
        arg0->unkC = 0;
    }
    arg0->unk11 = 0;
    arg0->unk12 = 0;
    setCallback(&func_80036920_37520);
}

void func_80036920_37520(func_80036920_37520_arg *arg0) {
    Vec3i sp10;
    s32 sp20;
    s32 sp24;
    s32 temp_sp20;
    s32 temp_sp24;
    func_800698BC_6A4BC_return *temp_v0;
    s32 temp_val;

    temp_v0 = getCurrentAllocation();
    sp10.x = (s32)D_8008F810_90410[arg0->unk10].unk0;
    temp_val = D_8008F810_90410[arg0->unk10].unk4;
    sp10.y = 0x4B0000;
    sp10.z = temp_val;

    func_80035FE0_36BE0(&sp20, &sp24, &sp10);

    arg0->unk0 = (s16)(sp20 - 0xA0);
    arg0->unk2 = (s16)(sp24 - 0x78);

    arg0->unk12 = (arg0->unk12 + 1) & 7;
    if (arg0->unk12 == 0) {
        volatile u8 padding[0x2];
        arg0->unk11 = (arg0->unk11 + 1) & 3;
    }

    arg0->unk8 = (s16)(arg0->unk11 + 8);

    if (temp_v0->unk424 == 0) {
        func_80069CF8_6A8F8();
        return;
    }

    debugEnqueueCallback(8, 7, &func_80012004_12C04, arg0);
}

void func_80036A10_37610(void *untypedArg) {
    func_80036A10_37610_arg *arg0 = (func_80036A10_37610_arg *)untypedArg;
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_80036A3C_3763C(func_80036A3C_3763C_arg *arg0) {
    arg0->unk0 = 0xA;
    arg0->unk1 = 0;
    setCallback(&func_80036A68_37668);
}

void func_80036A68_37668(void *arg0) {
    s16 temp_v0_2;
    s16 var_v1;
    func_800698BC_6A4BC_return *temp_v0 = (func_800698BC_6A4BC_return *)getCurrentAllocation();

    if (temp_v0->unk3F8 > 0x400000) {
        temp_v0_2 = temp_v0->unk3F4;
        var_v1 = temp_v0_2;
        if (temp_v0_2 >= 0x1001) {
            var_v1 -= 0x2000;
        }
        if (var_v1 >= 0x821) {
            temp_v0->unk424 = 1;
            temp_v0->unk425 = *(u8 *)arg0;
            return;
        }
        if (var_v1 < -0x844) {
            temp_v0->unk424 = 1;
            temp_v0->unk425 = *((u8 *)arg0) + 1;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/36E50", func_80036AF8_376F8);

s32 func_80036C20_37820(func_80036C20_37820_arg *arg0) {
    func_80036274_36E74_large_struct *allocation;
    struct {
        s32 distances[1];
        s32 maxDistance;
    } data;
    s32 i;
    s32 dx;
    s32 dy;

    allocation = (func_80036274_36E74_large_struct *)getCurrentAllocation();
    if (allocation->numEntries == 0) {
        return 0;
    }

    if (allocation->unk421 == 1) {
        return 0;
    }

    if (allocation->unk422 == 1) {
        return 0;
    }

    data.maxDistance = 0x01000000;
    if (allocation->unk42A == 0) {
        for (i = 0; i < allocation->numEntries; i++) {
            dx = allocation->unk408[i] - arg0->unk34;
            dy = allocation->unk410[i] - arg0->unk3C;
            dx = distance_2d(dx, dy);
            data.distances[i] = dx;

            if (dx <= 0x37FFFF) {
                if (gControllerInputs & A_BUTTON) {
                    allocation->unk42A = 0x11;
                }
            }
        }
    }

    if (allocation->unk42A != 0x11) {
        return 0;
    }

    if (data.distances[0] < data.maxDistance) {
        return 1;
    }

    return 2;
}

INCLUDE_ASM("asm/nonmatchings/36E50", func_80036D54_37954);

void func_800371CC_37DCC(func_800371CC_37DCC_arg *arg0) {
    func_800698BC_6A4BC_return *temp_v0 = (func_800698BC_6A4BC_return *)getCurrentAllocation();

    if (temp_v0->unk420 == 3 && temp_v0->unk42D == 8) {
        arg0->unk40 = &D_8008FAC0_906C0;
        func_80058220_58E20(D_8008FD10_90910[D_800AFE8C_A71FC->unk9[0]], 0);
        temp_v0->unk403 = D_8008FD1C_9091C[D_800AFE8C_A71FC->unk9[0]];
    }
}
