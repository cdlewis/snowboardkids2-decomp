#include "56910.h"
#include "common.h"
#include "displaylist.h"
#include "geometry.h"
#include "task_scheduler.h"

extern s32 gFrameCounter;

extern void func_800BC768_AEB28(void *);
extern void *freeNodeMemory(void *);

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
    /* 0x00 */ Mat3x3Padded node1;
    /* 0x20 */ void *unk20;
    /* 0x24 */ void *unk24;
    /* 0x28 */ void *unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ u8 pad30[0xC];
    /* 0x3C */ Mat3x3Padded node2;
    /* 0x5C */ void *unk5C;
    /* 0x60 */ void *unk60;
    /* 0x64 */ void *unk64;
    /* 0x68 */ s32 unk68;
    /* 0x6C */ u8 pad6C[0xC];
    /* 0x78 */ s32 unk78;
    /* 0x7C */ u8 unk7C;
} func_800BC6C4_AEA84_arg;

typedef struct {
    /* 0x00 */ Mat3x3Padded node1;
    /* 0x20 */ u8 _pad20[4];
    /* 0x24 */ void *unk24;
    /* 0x28 */ void *unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ u8 _pad30[0xC];
    /* 0x3C */ Mat3x3Padded node2;
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
    arg0->node1.unk14 = 0x130F588E;
    arg0->node1.unk18 = 0x03930000;
    arg0->node1.unk1C = 0x0DB72F2C;
    arg0->unk68 = 0;
    arg0->unk60 = arg0->unk24;
    arg0->unk64 = arg0->unk28;
    createYRotationMatrix(&arg0->node2, 0);
    arg0->node2.unk14 = 0;
    arg0->node2.unk18 = 0x30000000;
    arg0->node2.unk1C = 0x30458CB2;
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
    arg0->unk44 = ((u8 *)&arg0->unk44)[1];
    arg0->unk46 = ((u8 *)&arg0->unk46)[1];
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
extern void func_80063058_63C58(DisplayListObject *);

void func_800BBB0C_ADECC(void *arg0) {
    func_800BB75C_ADB1C(arg0);
    func_8006300C_63C0C(arg0);
}

void func_800BBB38_ADEF8(void *arg0) {
    func_800BB75C_ADB1C(arg0);
    func_80063058_63C58(arg0);
}

extern void func_800630A4_63CA4(void *);

void func_800BBB64_ADF24(void *arg0) {
    func_800BB75C_ADB1C(arg0);
    func_800630A4_63CA4(arg0);
}

typedef struct {
    /* 0x00 */ Mat3x3Padded mat;
    /* 0x20 */ u8 _pad20[0x5C];
} func_800BBB90_task;

void func_800BBB90(s16 arg0) {
    func_800BBB90_task *task;

    task = scheduleTask(func_800BB494_AD854, 0, 0, 0xD2);
    if (task != NULL) {
        if (arg0 == 0) {
            createYRotationMatrix(&task->mat, 0x98A);
            task->mat.unk14 = 0x130F588E;
            task->mat.unk18 = 0x03930000;
            task->mat.unk1C = 0x0DB72F2C;
        } else {
            createYRotationMatrix(&task->mat, 0);
            task->mat.unk14 = 0;
            task->mat.unk18 = 0x30000000;
            task->mat.unk1C = 0x30458CB2;
        }
    }
}

extern s32 D_800BCA30_AEDF0[][3];

extern void func_800BBEA0_AE260(func_800BB45C_AD81C_arg *);
extern void func_800BBCFC_AE0BC(void *);

typedef struct {
    /* 0x00 */ u8 _pad0[0x14];
    /* 0x14 */ s32 position[3];
    /* 0x20 */ u8 _pad20[4];
    /* 0x24 */ void *unk24;
    /* 0x28 */ void *unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ u8 _pad30[0xC];
    /* 0x3C */ Mat3x3Padded mat;
    /* 0x5C */ u8 _pad5C[4];
    /* 0x60 */ void *unk60;
    /* 0x64 */ void *unk64;
    /* 0x68 */ s32 unk68;
    /* 0x6C */ u8 _pad6C[0xC];
    /* 0x78 */ s16 unk78;
    /* 0x7A */ u8 _pad7A[2];
    /* 0x7C */ u8 unk7C;
} func_800BBC28_arg;

void func_800BBC28_ADFE8(func_800BBC28_arg *arg0) {
    arg0->unk24 = func_80055DC4_569C4(8);
    arg0->unk28 = func_80055DF8_569F8(8);
    arg0->unk2C = 0;
    memcpy(&arg0->position, &D_800BCA30_AEDF0[arg0->unk7C], 0xC);
    arg0->unk68 = 0;
    arg0->position[1] += 0x100000;
    arg0->unk60 = arg0->unk24;
    arg0->unk64 = arg0->unk28;
    createYRotationMatrix(&arg0->mat, 0);
    arg0->unk78 = 0x2000;
    arg0->mat.unk14 = arg0->position[0];
    arg0->mat.unk18 = arg0->position[1] + 0x180000;
    arg0->mat.unk1C = arg0->position[2];
    setCleanupCallback(&func_800BBEA0_AE260);
    setCallback(&func_800BBCFC_AE0BC);
}

INCLUDE_ASM("asm/nonmatchings/levels/starlight_highway", func_800BBCFC_AE0BC);

void func_800BBEA0_AE260(func_800BB45C_AD81C_arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

void func_800BBED8(void) {
    s32 i;

    for (i = 0; i < 7; i++) {
        Node *task = scheduleTask(func_800BBC28_ADFE8, 0, 0, 0x32);
        if (task != NULL) {
            *(u8 *)((u8 *)task + 0x7C) = i;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/levels/starlight_highway", func_800BBF28_AE2E8);

INCLUDE_ASM("asm/nonmatchings/levels/starlight_highway", func_800BC084_AE444);

INCLUDE_ASM("asm/nonmatchings/levels/starlight_highway", func_800BC1AC_AE56C);

extern s32 func_8005C250_5CE50(void *, s32, s32);
extern void func_80042340_42F40(void *);
extern s16 D_800BCB70_AEF30[];
extern s16 D_800BCB84_AEF44[];

typedef struct {
    u8 _pad[0x76];
    u8 unk76;
} Allocation_AE790;

typedef struct {
    u8 _pad0[0x14];
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    u8 _pad20[0x1C];
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s16 unk48;
    u16 unk4A;
    u16 unk4C;
    u8 unk4E;
} func_800BC3D0_AE790_arg;

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

typedef struct {
    u8 _pad[0x24];
    void *unk24;
    void *unk28;
} func_800BC4F0_AE8B0_arg;

void func_800BC4F0_AE8B0(func_800BC4F0_AE8B0_arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

typedef struct {
    s16 unk0;
} func_800BC528_AE8E8_arg;

extern void func_800BC550_AE910(void);

void func_800BC528_AE8E8(func_800BC528_AE8E8_arg *arg0) {
    arg0->unk0 = 0xF0;
    setCallback(&func_800BC550_AE910);
}

INCLUDE_ASM("asm/nonmatchings/levels/starlight_highway", func_800BC550_AE910);

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

INCLUDE_ASM("asm/nonmatchings/levels/starlight_highway", func_800BC768_AEB28);

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
