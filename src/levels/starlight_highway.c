#include "42170.h"
#include "56910.h"
#include "5AA90.h"
#include "common.h"
#include "rand.h"
#include "displaylist.h"
#include "gamestate.h"
#include "geometry.h"
#include "graphics.h"
#include "task_scheduler.h"

extern s32 gFrameCounter;


typedef struct {
    u8 _pad[0x80];
    u8 unk80;
    u8 unk81;
} Allocation_AD728;

typedef struct {
    DisplayListObject node1;
    DisplayListObject node2;
} func_800BB368_AD728_arg;

typedef struct {
    /* 0x00 */ Transform3D node1;
    /* 0x20 */ void *unk20;
    /* 0x24 */ void *unk24;
    /* 0x28 */ void *unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ u8 pad30[0xC];
    /* 0x3C */ Transform3D node2;
    /* 0x5C */ void *unk5C;
    /* 0x60 */ void *unk60;
    /* 0x64 */ void *unk64;
    /* 0x68 */ s32 unk68;
    /* 0x6C */ u8 pad6C[0xC];
    /* 0x78 */ s32 unk78;
    /* 0x7C */ u8 unk7C;
} func_800BC6C4_AEA84_arg;

typedef struct {
    /* 0x00 */ Transform3D node1;
    /* 0x20 */ u8 _pad20[4];
    /* 0x24 */ void *unk24;
    /* 0x28 */ void *unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ u8 _pad30[0xC];
    /* 0x3C */ Transform3D node2;
    /* 0x5C */ u8 _pad5C[4];
    /* 0x60 */ void *unk60;
    /* 0x64 */ void *unk64;
    /* 0x68 */ s32 unk68;
} func_800BB2B0_AD670_arg;

typedef struct {
    u8 _pad[0x24];
    void *unk24;
    void *unk28;
} func_800BB45C_AD81C_arg;

void func_800BB368_AD728(func_800BB368_AD728_arg *);
void func_800BB45C_AD81C(func_800BB45C_AD81C_arg *);

void func_800BB2B0_AD670(func_800BB2B0_AD670_arg *arg0) {
    arg0->unk24 = func_80055DC4_569C4(8);
    arg0->unk28 = func_80055DF8_569F8(8);
    arg0->unk2C = 0;
    createYRotationMatrix(&arg0->node1, 0x98A);
    arg0->node1.translation.x = 0x130F588E;
    arg0->node1.translation.y = 0x03930000;
    arg0->node1.translation.z = 0x0DB72F2C;
    arg0->unk68 = 0;
    arg0->unk60 = arg0->unk24;
    arg0->unk64 = arg0->unk28;
    createYRotationMatrix(&arg0->node2, 0);
    arg0->node2.translation.x = 0;
    arg0->node2.translation.y = 0x30000000;
    arg0->node2.translation.z = 0x30458CB2;
    setCleanupCallback(&func_800BB45C_AD81C);
    setCallback(&func_800BB368_AD728);
}

void func_800BB368_AD728(func_800BB368_AD728_arg *arg0) {
    Allocation_AD728 *allocation;
    s32 i;
    func_80055E68_56A68_result *result;

    allocation = (Allocation_AD728 *)getCurrentAllocation();

    if (allocation->unk80 != 0) {
        if (gFrameCounter & 1) {
            result = func_80055E68_56A68(8);
            arg0->node1.unk20 = (DisplayLists *)((u32)result + 0x10);
        } else {
            result = func_80055E68_56A68(8);
            arg0->node1.unk20 = (DisplayLists *)((u32)result + 0x20);
        }
    } else {
        arg0->node1.unk20 = (DisplayLists *)func_80055E68_56A68(8);
    }

    if (allocation->unk81 != 0) {
        if (gFrameCounter & 1) {
            result = func_80055E68_56A68(8);
            arg0->node2.unk20 = (DisplayLists *)((u32)result + 0x10);
        } else {
            result = func_80055E68_56A68(8);
            arg0->node2.unk20 = (DisplayLists *)((u32)result + 0x20);
        }
    } else {
        arg0->node2.unk20 = (DisplayLists *)func_80055E68_56A68(8);
    }

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, &arg0->node1);
        enqueueDisplayListWithFrustumCull(i, &arg0->node2);
    }
}

void func_800BC984_AED44(func_800BB45C_AD81C_arg *);

void func_800BB45C_AD81C(func_800BB45C_AD81C_arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

typedef struct {
    /* 0x00 */ u8 _pad0[0x20];
    /* 0x20 */ void *unk20;
    /* 0x24 */ void *unk24;
    /* 0x28 */ void *unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ u8 _pad30[0xC];
    /* 0x3C */ void *unk3C;
    /* 0x40 */ u8 _pad40[0x4];
    /* 0x44 */ s16 unk44;
    /* 0x46 */ s16 unk46;
    /* 0x48 */ s16 unk48;
    /* 0x4A */ s16 unk4A;
    /* 0x4C */ s16 unk4C;
    /* 0x4E */ u8 _pad4E[0x2];
    /* 0x50 */ s16 unk50;
} func_800BB494_AD854_arg;

typedef struct {
    u8 unk00[0x20];
    struct {
        s32 unk00;
        s32 unk04;
        s32 unk08;
        s32 unk0C;
    } *unk20;
    u8 unk24[0xC];
    s32 unk30;
    u8 unk34[0x10];
    u16 unk44;
    u16 unk46;
    u16 unk48;
    u16 unk4A;
    s32 unk4C;
    s16 unk50;
    s16 unk52;
} func_800BB664_arg;

typedef struct {
    u8 pad[0x76];
    u8 unk76;
} Allocation_ADA24;

extern void *D_800955B0;

typedef struct func_800BB718_ADAD8_arg func_800BB718_ADAD8_arg;
void func_800BB718_ADAD8(func_800BB718_ADAD8_arg *);
void func_800BB5FC_AD9BC(func_800BB664_arg *);

void func_800BB494_AD854(func_800BB494_AD854_arg *arg0) {
    arg0->unk24 = func_80055DC4_569C4(8);
    arg0->unk28 = func_80055DF8_569F8(8);
    arg0->unk2C = 0;
    arg0->unk3C = func_80055D7C_5697C(8);
    arg0->unk20 = &D_800955B0;
    arg0->unk4C = 0;
    arg0->unk48 = 8;
    arg0->unk4A = 0;
    arg0->unk44 = 0;
    arg0->unk46 = 0;
    arg0->unk50 = 0;
    setCleanupCallback(&func_800BB718_ADAD8);
    setCallback(&func_800BB5FC_AD9BC);
}

void func_800BBB0C_ADECC(void *);
void func_800BBB38_ADEF8(void *);
void func_800BBB64_ADF24(void *);

void func_800BB51C_AD8DC(func_800BB664_arg *arg0) {
    s32 i;

    arg0->unk44 += arg0->unk48;
    arg0->unk46 = arg0->unk46 + arg0->unk4A;
    arg0->unk30 = 0;
    arg0->unk44 = arg0->unk44 & 0xFF;
    arg0->unk46 = arg0->unk46 & 0xFF;
    ;

    for (i = 0; i < 4; i++) {
        if (arg0->unk20->unk04 != 0) {
            debugEnqueueCallback(i & 0xFFFF, 1, func_800BBB0C_ADECC, arg0);
        }

        if (arg0->unk20->unk08 != 0) {
            debugEnqueueCallback(i & 0xFFFF, 3, func_800BBB38_ADEF8, arg0);
        }

        if (arg0->unk20->unk0C == 0) {
            continue;
        }

        debugEnqueueCallback(i & 0xFFFF, 5, func_800BBB64_ADF24, arg0);
    }
}

extern void func_800BB6C0_ADA80(func_800BB664_arg *);
void func_800BB664_ADA24(func_800BB664_arg *);

void func_800BB5FC_AD9BC(func_800BB664_arg *arg0) {
    Allocation_ADA24 *allocation = getCurrentAllocation();

    if (allocation->unk76 == 0) {
        arg0->unk50 += 0x10;
    }

    if (arg0->unk50 == 0xC0) {
        setCallback(&func_800BB664_ADA24);
        arg0->unk52 = 0x40;
    }

    func_800BB51C_AD8DC(arg0);
}

void func_800BB664_ADA24(func_800BB664_arg *arg0) {
    Allocation_ADA24 *allocation = getCurrentAllocation();

    if (allocation->unk76 == 0) {
        arg0->unk52--;
    }

    if (arg0->unk52 == 0) {
        setCallback(&func_800BB6C0_ADA80);
    }

    func_800BB51C_AD8DC(arg0);
}

void func_800BB6C0_ADA80(func_800BB664_arg *arg0) {
    Allocation_ADA24 *allocation = getCurrentAllocation();

    if (allocation->unk76 == 0) {
        arg0->unk50 -= 0x10;
    }

    if (arg0->unk50 == 0) {
        func_80069CF8_6A8F8();
    }

    func_800BB51C_AD8DC(arg0);
}

struct func_800BB718_ADAD8_arg {
    u8 _pad[0x24];
    void *unk24;
    void *unk28;
    u8 _pad2[0x10];
    void *unk3C;
};

void func_800BB718_ADAD8(func_800BB718_ADAD8_arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
    arg0->unk3C = freeNodeMemory(arg0->unk3C);
}

INCLUDE_ASM("asm/nonmatchings/levels/starlight_highway", func_800BB75C_ADB1C);

extern void func_800BB75C_ADB1C(void *);

void func_800BBB0C_ADECC(void *arg0) {
    func_800BB75C_ADB1C(arg0);
    func_8006300C_63C0C(arg0);
}

void func_800BBB38_ADEF8(void *arg0) {
    func_800BB75C_ADB1C(arg0);
    func_80063058_63C58(arg0);
}

void func_800BBB64_ADF24(void *arg0) {
    func_800BB75C_ADB1C(arg0);
    func_800630A4_63CA4(arg0);
}

typedef struct {
    /* 0x00 */ Transform3D mat;
    /* 0x20 */ u8 _pad20[0x5C];
} func_800BBB90_task;

void func_800BBB90(s16 arg0) {
    func_800BBB90_task *task;

    task = scheduleTask(func_800BB494_AD854, 0, 0, 0xD2);
    if (task != NULL) {
        if (arg0 == 0) {
            createYRotationMatrix(&task->mat, 0x98A);
            task->mat.translation.x = 0x130F588E;
            task->mat.translation.y = 0x03930000;
            task->mat.translation.z = 0x0DB72F2C;
        } else {
            createYRotationMatrix(&task->mat, 0);
            task->mat.translation.x = 0;
            task->mat.translation.y = 0x30000000;
            task->mat.translation.z = 0x30458CB2;
        }
    }
}

extern s32 D_800BCA30_AEDF0[][3];

extern void func_800BBEA0_AE260(func_800BB45C_AD81C_arg *);

typedef struct {
    /* 0x00 */ Transform3D mat1;
    /* 0x20 */ DisplayLists *unk20;
    /* 0x24 */ void *unk24;
    /* 0x28 */ void *unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ u8 _pad30[0xC];
    /* 0x3C */ Transform3D mat2;
    /* 0x5C */ DisplayLists *unk5C;
    /* 0x60 */ void *unk60;
    /* 0x64 */ void *unk64;
    /* 0x68 */ s32 unk68;
    /* 0x6C */ u8 _pad6C[0xC];
    /* 0x78 */ s16 unk78;
    /* 0x7A */ u16 unk7A;
    /* 0x7C */ u8 unk7C;
} func_800BBC28_arg;

void func_800BBCFC_AE0BC(func_800BBC28_arg *);

void func_800BBC28_ADFE8(func_800BBC28_arg *arg0) {
    arg0->unk24 = func_80055DC4_569C4(8);
    arg0->unk28 = func_80055DF8_569F8(8);
    arg0->unk2C = 0;
    memcpy(&arg0->mat1.translation.x, &D_800BCA30_AEDF0[arg0->unk7C], 0xC);
    arg0->unk68 = 0;
    arg0->mat1.translation.y += 0x100000;
    arg0->unk60 = arg0->unk24;
    arg0->unk64 = arg0->unk28;
    createYRotationMatrix(&arg0->mat2, 0);
    arg0->unk78 = 0x2000;
    arg0->mat2.translation.x = arg0->mat1.translation.x;
    arg0->mat2.translation.y = arg0->mat1.translation.y + 0x180000;
    arg0->mat2.translation.z = arg0->mat1.translation.z;
    setCleanupCallback(&func_800BBEA0_AE260);
    setCallback(&func_800BBCFC_AE0BC);
}

void func_800BBCFC_AE0BC(func_800BBC28_arg *arg0) {
    GameState *allocation;
    s32 var_s0;
    s32 var_s1;
    s16 temp_v0;
    s16 temp_a1;
    u16 temp_v0_2;
    s32 temp;
    u8 pad[0x10];

    (void)pad;

    allocation = (GameState *)getCurrentAllocation();
    var_s0 = 0;
    temp = allocation->numPlayers;
    if (temp > 0) {
        var_s1 = 0;
loop_2:
        if (isPlayerInRangeAndPull((Vec3i *)&arg0->mat1.translation.x, 0x200000, (Player *)((u8 *)allocation->players + var_s1)) == 0) {
            var_s0 += 1;
            var_s1 += 0xBE8;
            if (var_s0 < (s32)allocation->numPlayers) {
                goto loop_2;
            }
        }
    }

    if ((var_s0 != allocation->numPlayers) && (arg0->unk78 == 0x2000)) {
        var_s0 = 0;
        if (allocation->numPlayers != 0) {
            var_s1 = 0;
            do {
                isPlayerInRangeAndPull((Vec3i *)&arg0->mat1.translation.x, 0x500000, (Player *)((u8 *)allocation->players + var_s1));
                var_s0 += 1;
                var_s1 += 0xBE8;
            } while (var_s0 < (s32)allocation->numPlayers);
        }
        arg0->unk78 = 0x4000;
        func_80056B7C_5777C(&arg0->mat1.translation.x, 0x4E);
    }

    temp_v0_2 = arg0->unk7A + 0x100;
    arg0->unk7A = temp_v0_2;
    createYRotationMatrix(&arg0->mat1, temp_v0_2 & 0xFFFF);

    temp_v0 = arg0->unk78;
    if (temp_v0 != 0x2000) {
        if (allocation->gamePaused == 0) {
            arg0->unk78 = (s16)(temp_v0 - 0x800);
        }
        temp_a1 = arg0->unk78;
        scaleMatrix(&arg0->mat1, temp_a1, 0x2000, temp_a1);
        if (arg0->unk78 != 0x2000) {
            goto block_c0f0;
        }
    }
    // D0/E0 block
    arg0->unk5C = (DisplayLists *)((arg0->unk20 = (DisplayLists *)((s32)func_80055E68_56A68(8) + 0xD0)), (s32)func_80055E68_56A68(8) + 0xE0);
    goto common;

block_c0f0:
    arg0->unk5C = (DisplayLists *)((arg0->unk20 = (DisplayLists *)((s32)func_80055E68_56A68(8) + 0xC0)), (s32)func_80055E68_56A68(8) + 0xF0);

common:
    var_s0 = 0;
    do {
        enqueueDisplayListWithFrustumCull(var_s0, (DisplayListObject *)&arg0->mat1);
        enqueueDisplayListWithFrustumCull(var_s0, (DisplayListObject *)&arg0->mat2);
        var_s0 += 1;
    } while (var_s0 < 4);
}

void func_800BBEA0_AE260(func_800BB45C_AD81C_arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

void func_800BBED8(void) {
    s32 i;

    for (i = 0; i < 7; i++) {
        func_800BBC28_arg *task = scheduleTask(func_800BBC28_ADFE8, 0, 0, 0x32);
        if (task != NULL) {
            task->unk7C = i;
        }
    }
}

extern s32 D_800BCA84_AEE44[][3];
extern s16 D_800BCAF0_AEEB0[];
extern s32 D_800BCB04_AEEC4[][3];

extern s16 D_800BCB70_AEF30[];
extern s16 D_800BCB84_AEF44[];

extern s32 D_800BCB98_AEF58[][3];
extern s32 D_800BCB9C_AEF5C[][3];
extern s32 D_800BCBA0_AEF60[][3];
extern s16 D_800BCBB0_AEF70[];

typedef struct {
    u8 _pad[0x76];
    u8 unk76;
} Allocation_AE790;

typedef struct {
    u8 _pad0[0x14];
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    void *unk20;
    void *unk24;
    void *unk28;
    s32 unk2C;
    u8 _pad30[0xC];
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s16 unk48;
    u16 unk4A;
    u16 unk4C;
    u8 unk4E;
} func_800BC3D0_AE790_arg;

void func_800BC084_AE444(func_800BC3D0_AE790_arg *);
void func_800BC1AC_AE56C(func_800BC3D0_AE790_arg *);
void func_800BC3D0_AE790(func_800BC3D0_AE790_arg *);

typedef struct {
    u8 _pad[0x24];
    void *unk24;
    void *unk28;
} func_800BC4F0_AE8B0_arg;

void func_800BC4F0_AE8B0(func_800BC4F0_AE8B0_arg *);

typedef struct {
    s16 rotation[3][3];
    u8 pad2[0xE];
} func_800BBF28_StackLocals;

void func_800BBF28_AE2E8(func_800BC3D0_AE790_arg *arg0) {
    u8 pad[0x20];
    func_800BBF28_StackLocals stack;
    s16 (*rotPtr)[3];
    void (*callback)(func_800BC3D0_AE790_arg *);

    (void)pad;

    arg0->unk24 = func_80055DC4_569C4(8);
    arg0->unk28 = func_80055DF8_569F8(8);
    arg0->unk2C = 0;
    arg0->unk20 = (void *)((u32)func_80055E68_56A68(8) + 0x90);
    memcpy(&arg0->unk14, D_800BCA84_AEE44[arg0->unk4E], 0xC);
    rotPtr = stack.rotation;
    createXRotationMatrix(rotPtr, D_800BCAF0_AEEB0[arg0->unk4E]);
    transformVector2(D_800BCB04_AEEC4[arg0->unk4E], rotPtr, &arg0->unk3C);
    arg0->unk14 = arg0->unk14 - arg0->unk3C * 0x78;
    arg0->unk18 = arg0->unk18 - arg0->unk40 * 0x78;
    arg0->unk1C = arg0->unk1C - arg0->unk44 * 0x78;
    setCleanupCallback(func_800BC4F0_AE8B0);
    if (arg0->unk4E < 3) {
        callback = func_800BC084_AE444;
        arg0->unk48 = 0xF0;
    } else {
        arg0->unk48 = 0x78;
        callback = func_800BC1AC_AE56C;
    }
    setCallback(callback);
}

void func_800BC084_AE444(func_800BC3D0_AE790_arg *arg0) {
    Allocation_AE790 *allocation;
    s32 i;
    void *posPtr;

    allocation = getCurrentAllocation();

    if (allocation->unk76 == 0) {
        arg0->unk14 += arg0->unk3C;
        arg0->unk18 += arg0->unk40;
        arg0->unk1C += arg0->unk44;
        arg0->unk48--;

        if (arg0->unk48 == 0) {
            func_80069CF8_6A8F8();
            return;
        }

        posPtr = &arg0->unk14;
        if (func_8005C250_5CE50(posPtr, -1, 0x300000) != 0) {
            func_80042340_42F40(posPtr);
            func_80069CF8_6A8F8();
        }

        arg0->unk4A += D_800BCB70_AEF30[arg0->unk4E];
        arg0->unk4C += D_800BCB84_AEF44[arg0->unk4E];
    }

    createCombinedRotationMatrix(arg0, arg0->unk4A, arg0->unk4C);

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
    }
}

void func_800BC1AC_AE56C(func_800BC3D0_AE790_arg *arg0) {
    Allocation_AE790 *allocation;
    s32 i;
    s16 rotation[3][3];
    s16 pad2[4];
    void (*callback)(func_800BC3D0_AE790_arg *);
    s16 timer;
    u8 state;
    void *posPtr;

    (void)pad2;

    allocation = getCurrentAllocation();

    if (allocation->unk76 == 0) {
        if (arg0->unk48 != 0) {
            arg0->unk14 = arg0->unk14 + arg0->unk3C;
            arg0->unk18 = arg0->unk18 + arg0->unk40;
            arg0->unk1C = arg0->unk1C + arg0->unk44;
            arg0->unk48--;
        } else {
            state = arg0->unk4E;
            switch (state) {
            case 3:
                createXRotationMatrix(rotation, 0xF300);
                transformVector2(D_800BCB04_AEEC4[arg0->unk4E], rotation, &arg0->unk3C);
                callback = func_800BC084_AE444;
                timer = 0x78;
                arg0->unk48 = timer;
                setCallbackWithContinue(callback);
                break;
            case 4:
                createXRotationMatrix(rotation, 0xE00);
                transformVector2(D_800BCB04_AEEC4[arg0->unk4E], rotation, &arg0->unk3C);
                callback = func_800BC3D0_AE790;
                arg0->unk48 = 0xA;
                setCallbackWithContinue(callback);
                break;
            case 5:
                createXRotationMatrix(rotation, 0xFC00);
                transformVector2(D_800BCB04_AEEC4[arg0->unk4E], rotation, &arg0->unk3C);
                callback = func_800BC084_AE444;
                timer = 0x78;
                arg0->unk48 = timer;
                setCallbackWithContinue(callback);
                break;
            case 6:
                createXRotationMatrix(rotation, 0);
                transformVector2(D_800BCB04_AEEC4[arg0->unk4E], rotation, &arg0->unk3C);
                callback = func_800BC3D0_AE790;
                arg0->unk48 = 0xA;
                setCallbackWithContinue(callback);
                break;
            case 7:
            case 8:
                func_80042340_42F40(&arg0->unk14);
                func_80069CF8_6A8F8();
                break;
            default:
                break;
            }
        }
        posPtr = &arg0->unk14;
        func_8005C250_5CE50(posPtr, -1, 0x300000);
        arg0->unk4A += D_800BCB70_AEF30[arg0->unk4E];
        arg0->unk4C += D_800BCB84_AEF44[arg0->unk4E];
    }

    createCombinedRotationMatrix(arg0, arg0->unk4A, arg0->unk4C);

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
    }
}

void func_800BC3D0_AE790(func_800BC3D0_AE790_arg *arg0) {
    Allocation_AE790 *allocation;
    s32 i;
    void *posPtr;

    allocation = getCurrentAllocation();

    if (allocation->unk76 == 0) {
        arg0->unk14 += arg0->unk3C;
        arg0->unk18 += arg0->unk40;
        arg0->unk1C += arg0->unk44;
        arg0->unk48--;

        if (arg0->unk48 == 0) {
            func_80042340_42F40(&arg0->unk14);
            func_80069CF8_6A8F8();
        } else {
            posPtr = &arg0->unk14;
            if (func_8005C250_5CE50(posPtr, -1, 0x300000)) {
                func_80042340_42F40(posPtr);
                func_80069CF8_6A8F8();
            }
        }

        arg0->unk4A += D_800BCB70_AEF30[arg0->unk4E];
        arg0->unk4C += D_800BCB84_AEF44[arg0->unk4E];
    }

    createCombinedRotationMatrix(arg0, arg0->unk4A, arg0->unk4C);

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
    }
}

void func_800BC4F0_AE8B0(func_800BC4F0_AE8B0_arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

typedef struct {
    s16 unk0;
} func_800BC528_AE8E8_arg;

void func_800BC550_AE910(s16 *arg0);

void func_800BC528_AE8E8(func_800BC528_AE8E8_arg *arg0) {
    arg0->unk0 = 0xF0;
    setCallback(&func_800BC550_AE910);
}

void func_800BC550_AE910(s16 *arg0) {
    Allocation_AE790 *allocation;
    func_800BC3D0_AE790_arg *task;
    s32 new_var;
    s32 new_var2;
    u32 s2;

    allocation = getCurrentAllocation();
    if (allocation->unk76 != 0) {
        return;
    }

    *arg0 -= 1;
    if ((s16)*arg0 == 0x78) {
        task = scheduleTask(func_800BBF28_AE2E8, 0, 0, 0xC8);
        if (task != NULL) {
            task->unk4E = (u32)(randA() & 0xFF) % 3U;
        }
    }

    if ((s16)(s2 = *arg0) == 0) {
        if (getFreeNodeCount(0) >= 2) {
            s2 = 2;
            s2 = (u8)((u32)(randA() & 0xFF) % 3U) * s2;
            new_var2 = s2 + 3;
            task = scheduleTask(func_800BBF28_AE2E8, 0, 0, 0xC8);
            if (task != NULL) {
                do { task->unk4E = (new_var = new_var2); } while (0);
            }

            task = scheduleTask(func_800BBF28_AE2E8, 0, 0, 0xC8);
            if (task != NULL) {
                task->unk4E = s2 + 4;
            }
        }

        *arg0 = (randA() & 0x1F) + 0xF0;
    }
}

void func_800BC768_AEB28(func_800BC6C4_AEA84_arg *arg0);

void func_800BC6C4_AEA84(func_800BC6C4_AEA84_arg *arg0) {
    void *temp;

    arg0->unk24 = func_80055DC4_569C4(8);
    arg0->unk28 = func_80055DF8_569F8(8);
    arg0->unk2C = 0;
    temp = func_80055E68_56A68(8);
    arg0->unk20 = (void *)((u32)temp + 0xA0);
    arg0->unk68 = 0;
    arg0->unk60 = arg0->unk24;
    arg0->unk64 = arg0->unk28;
    temp = func_80055E68_56A68(8);
    arg0->unk5C = (void *)((u32)temp + 0xB0);
    createYRotationMatrix(&arg0->node1, 0x1BEC);
    createYRotationMatrix(&arg0->node2, 0x1BEC);
    arg0->unk78 = 0;
    setCleanupCallback(&func_800BC984_AED44);
    setCallback(&func_800BC768_AEB28);
}

void func_800BC768_AEB28(func_800BC6C4_AEA84_arg *arg0) {
    Vec3i vec;
    s32 pad[2];
    s32 found;
    s32 i;
    u8 temp;
    s32 numPlayers;
    GameState *allocation;
    Player *player;
    s32 unk78Val;
    s32 tempS;

    allocation = getCurrentAllocation();
    temp = allocation->numPlayers;
    found = 0;
    i = 0;
    tempS = temp;

    if (tempS > 0) {
        numPlayers = tempS;
        player = allocation->players;
        do {
            if (player->unkB94 == D_800BCBB0_AEF70[arg0->unk7C]) {
                found = 1;
            }
            i++;
            player++;
        } while (i < numPlayers);
    }

    if (found != 0) {
        if (allocation->gamePaused == 0) {
            if (arg0->unk78 != 0x600000) {
                arg0->unk78 = arg0->unk78 + 0x100000;
            }
        }
    } else if (allocation->gamePaused == 0) {
        if (arg0->unk78 != 0) {
            arg0->unk78 = arg0->unk78 - 0x100000;
        }
    }

    unk78Val = arg0->unk78;
    vec.y = 0;
    vec.z = 0;
    vec.x = -unk78Val;
    rotateVectorY(&vec, 0x1BEC, &arg0->node1.translation.x);
    vec.x = arg0->unk78;
    rotateVectorY(&vec, 0x1BEC, &arg0->node2.translation.x);

    arg0->node1.translation.x = arg0->node1.translation.x + D_800BCB98_AEF58[arg0->unk7C][0];
    arg0->node1.translation.y = arg0->node1.translation.y + D_800BCB9C_AEF5C[arg0->unk7C][0];
    arg0->node1.translation.z = arg0->node1.translation.z + D_800BCBA0_AEF60[arg0->unk7C][0];
    arg0->node2.translation.x = arg0->node2.translation.x + D_800BCB98_AEF58[arg0->unk7C][0];
    arg0->node2.translation.y = arg0->node2.translation.y + D_800BCB9C_AEF5C[arg0->unk7C][0];
    i = 0;
    arg0->node2.translation.z = arg0->node2.translation.z + D_800BCBA0_AEF60[arg0->unk7C][0];

    do {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)&arg0->node1);
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)&arg0->node2);
        i++;
    } while (i < 4);

    (void)pad;
}

void func_800BC984_AED44(func_800BB45C_AD81C_arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

void func_800BC9BC(void) {
    func_800BC6C4_AEA84_arg *task;

    task = (func_800BC6C4_AEA84_arg *)scheduleTask(func_800BC6C4_AEA84, 0, 0, 0xC8);
    if (task != NULL) {
        task->unk7C = 0;
    }

    task = (func_800BC6C4_AEA84_arg *)scheduleTask(func_800BC6C4_AEA84, 0, 0, 0xC8);
    if (task != NULL) {
        task->unk7C = 1;
    }
}
