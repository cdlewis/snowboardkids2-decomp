#include "56910.h"
#include "common.h"
#include "gamestate.h"
#include "geometry.h"
#include "graphics.h"
#include "task_scheduler.h"

extern void *D_8009A6B0_9B2B0;
extern void *D_8009A6C0_9B2C0;
extern void *D_8009A6D0_9B2D0;
extern void *D_8009A6E0_9B2E0;
extern void *D_8009A6F0_9B2F0;
extern void *D_8009A700_9B300;
extern void *D_8009A710_9B310;
extern void *D_8009A720_9B320;
extern void *D_8009A760_9B360;
extern void func_80041810_42410(void *);
extern void func_800415E8_421E8(void);
extern void func_80042160_42D60(void);
extern void func_80043E24_44A24(void);
extern void func_8004247C_4307C(void);
extern void func_800440B4_44CB4(void *);
extern void func_80044684_45284(void);

typedef struct {
    s16 matrix[3][3]; /* 0x00 */
    u8 pad12[0xE];    /* 0x12 */
    void *unk20;      /* 0x20 */
    void *unk24;      /* 0x24 */
    void *unk28;      /* 0x28 */
    s32 unk2C;        /* 0x2C */
    u8 pad30[0x10];   /* 0x30 */
    s32 unk40;        /* 0x40 */
    s32 unk44;        /* 0x44 */
} Func41570State;

typedef struct {
    u8 pad0[0x20];
    void *unk20;
    void *unk24;
    void *unk28;
    s32 unk2C;
    u8 pad30[0xC];
    void *unk3C;
    s32 unk40;
} Func4179CArg;

typedef struct {
    u8 pad0[0x14];  /* 0x00 */
    s32 posX;       /* 0x14 */
    s32 posY;       /* 0x18 */
    s32 posZ;       /* 0x1C */
    u8 pad20[0x1C]; /* 0x20 */
    Player *unk3C;  /* 0x3C */
} Func41A60Arg;

void func_800419AC_425AC(Func4179CArg *);

typedef struct {
    u8 pad0[0x14];  /* 0x00 */
    u8 unk14[0xC];  /* 0x14 */
    void *unk20;    /* 0x20 */
    void *unk24;    /* 0x24 */
    void *unk28;    /* 0x28 */
    s32 unk2C;      /* 0x2C */
    u8 pad30[0x10]; /* 0x30 */
    s16 unk40;      /* 0x40 */
    s16 unk42;      /* 0x42 */
    s16 unk44;      /* 0x44 */
} Func420E8State;

void func_80042308_42F08(Func420E8State *);

typedef struct {
    u8 pad0[0x20];  /* 0x00 */
    void *unk20;    /* 0x20 */
    void *unk24;    /* 0x24 */
    void *unk28;    /* 0x28 */
    s32 unk2C;      /* 0x2C */
    u8 pad30[0x16]; /* 0x30 */
    s16 unk46;      /* 0x46 */
} Func43DC0State;

void func_80044018_44C18(Func43DC0State *);

typedef struct {
    u8 pad0[0x20];    /* 0x00 */
    void *unk20;      /* 0x20 */
    void *unk24;      /* 0x24 */
    void *unk28;      /* 0x28 */
    s32 unk2C;        /* 0x2C */
    u8 pad30[0x0C];   /* 0x30 */
    void *unk3C;      /* 0x3C */
    s16 matrix[3][3]; /* 0x40 - just the 9 s16 values (0x12 bytes) */
    u8 pad52[2];      /* 0x52 - padding to 0x54 */
    s32 unk54;        /* 0x54 */
    s32 unk58;        /* 0x58 */
    s32 unk5C;        /* 0x5C */
    s32 unk60;        /* 0x60 */
} Func41F38State;

typedef struct {
    u8 _pad0[0x3C]; /* 0x00 */
    s16 unk3C;      /* 0x3C */
} Func44538TaskMem;

typedef struct {
    u8 pad0[0x20]; /* 0x00 */
    void *unk20;   /* 0x20 */
    void *unk24;   /* 0x24 */
    void *unk28;   /* 0x28 */
    s32 unk2C;     /* 0x2C */
    u8 pad30[0xC]; /* 0x30 */
    void *unk3C;   /* 0x3C */
    s32 unk40;     /* 0x40 */
    u8 pad44[2];   /* 0x44 */
    s16 unk46;     /* 0x46 */
} Func42410State;

void func_80041724_42324(Func41570State *);

typedef struct {
    u8 _pad0[0x76];
    u8 unk76;
} Func43CA4GameState;

typedef struct {
    u8 _pad0[0xB44];
    u8 unkB44[0xC];   /* 0xB44 */
    u8 _padB50[0x24]; /* 0xB50 */
    s16 unkB74;       /* 0xB74 */
} Func44BBCPointerTarget;

typedef struct {
    u8 _pad0[0x14];                /* 0x00 */
    u8 unk14[0x4];                 /* 0x14 - first 4 bytes of 12-byte copy source */
    s32 unk18;                     /* 0x18 */
    u8 unk1C[0x4];                 /* 0x1C - last 4 bytes of 12-byte copy source */
    u8 _pad20[0x94];               /* 0x20 */
    Func44BBCPointerTarget *unkB4; /* 0xB4 */
    u8 _padB8[0x4];                /* 0xB8 */
    s32 unkBC;                     /* 0xBC */
    u8 _padC0[0x4];                /* 0xC0 */
    u16 unkC4;                     /* 0xC4 - counter */
    u16 unkC6;                     /* 0xC6 - value copied to unkB74 */
} Func44BBCArg;

void func_80044578_45178(Func44BBCArg *);
void func_80044C38_45838(Func44BBCArg *);

typedef struct {
    u8 _pad0[0x9F0];
    s16 unk9F0[3];
} Func43CA4Unk28;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    u8 _pad10[0x18];
    Func43CA4Unk28 *unk28;
    s16 unk2C[3];
    u8 _pad32[0xE];
    s16 unk40;
    s16 unk42;
} Func43CA4Arg;

void func_80043CA4_448A4(Func43CA4Arg *);

extern s32 D_80090964_91564;
extern s32 D_80090974_91574;
extern s32 func_80043718_44318(void *, void *);
extern void func_80066444_67044(s32, void *);

void func_80041570_42170(Func41570State *arg0) {
    getCurrentAllocation();
    createXRotationMatrix(arg0->matrix, 0x800);
    arg0->unk20 = &D_8009A6B0_9B2B0;
    arg0->unk24 = loadAsset_B7E70();
    arg0->unk28 = loadAsset_216290();
    arg0->unk40 = 0x2000;
    arg0->unk2C = 0;
    arg0->unk44 = 1;
    setCleanupCallback(func_80041724_42324);
    setCallbackWithContinue(func_800415E8_421E8);
}

INCLUDE_ASM("asm/nonmatchings/42170", func_800415E8_421E8);

INCLUDE_ASM("asm/nonmatchings/42170", func_8004168C_4228C);

void func_80041724_42324(Func41570State *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

INCLUDE_ASM("asm/nonmatchings/42170", func_8004175C_4235C);

void func_8004179C_4239C(Func4179CArg *arg0) {
    getCurrentAllocation();
    arg0->unk20 = &D_8009A6C0_9B2C0;
    arg0->unk24 = loadAsset_B7E70();
    arg0->unk28 = loadAsset_216290();
    arg0->unk2C = 0;
    arg0->unk40 = 0x400;
    func_80056B7C_5777C((u8 *)arg0->unk3C + 0x434, 0x13);
    setCleanupCallback(func_800419AC_425AC);
    setCallbackWithContinue(func_80041810_42410);
}

INCLUDE_ASM("asm/nonmatchings/42170", func_80041810_42410);

void func_800419AC_425AC(Func4179CArg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

void func_800419E4_425E4(void *arg0) {
    Func4179CArg *task;

    task = scheduleTask(&func_8004179C_4239C, 0, 0, 0x63);
    if (task != NULL) {
        task->unk3C = arg0;
    }
}

void func_80041A24_42624(Func41A60Arg *arg0) {
    arg0->posX = arg0->unk3C->worldPosX;
    arg0->posY = arg0->unk3C->worldPosY;
    arg0->posZ = arg0->unk3C->worldPosZ;
    func_800634E8_640E8((DisplayListObject *)arg0);
}

void func_80041A60_42660(Func41A60Arg *arg0) {
    arg0->posX = arg0->unk3C->worldPosX;
    arg0->posY = arg0->unk3C->worldPosY;
    arg0->posZ = arg0->unk3C->worldPosZ;
    func_80063534_64134((s32)arg0);
}

void func_80041A9C_4269C(Func41A60Arg *arg0) {
    arg0->posX = arg0->unk3C->worldPosX;
    arg0->posY = arg0->unk3C->worldPosY;
    arg0->posZ = arg0->unk3C->worldPosZ;
    func_80063580_64180((s32)arg0);
}

typedef struct {
    u8 pad0[0xD8];
    void *unkD8;
} Func41AD8Arg;

extern void *load_3ECE40(void);
void func_80041B18_42718(void);
void func_80041DE4_429E4(Func41AD8Arg *);

void func_80041AD8_426D8(Func41AD8Arg *arg0) {
    arg0->unkD8 = load_3ECE40();
    setCleanupCallback(func_80041DE4_429E4);
    setCallbackWithContinue(func_80041B18_42718);
}

INCLUDE_ASM("asm/nonmatchings/42170", func_80041B18_42718);

INCLUDE_ASM("asm/nonmatchings/42170", func_80041C28_42828);

void func_80041DE4_429E4(Func41AD8Arg *arg0) {
    arg0->unkD8 = freeNodeMemory(arg0->unkD8);
}

INCLUDE_ASM("asm/nonmatchings/42170", func_80041E10_42A10);

INCLUDE_ASM("asm/nonmatchings/42170", func_80041EA4_42AA4);

void func_80041FB4_42BB4(void);
void func_80042070_42C70(Func41F38State *);

void func_80041F38_42B38(Func41F38State *arg0) {
    getCurrentAllocation();
    arg0->unk20 = &D_8009A6D0_9B2D0;
    arg0->unk24 = loadAsset_B7E70();
    arg0->unk28 = loadAsset_216290();
    arg0->unk2C = 0;
    createYRotationMatrix((Mat3x3Padded *)arg0->matrix, 0xF800);
    arg0->unk54 = 0;
    arg0->unk58 = 0;
    arg0->unk5C = 0;
    arg0->unk60 = 1;
    setCleanupCallback(func_80042070_42C70);
    setCallbackWithContinue(func_80041FB4_42BB4);
}

INCLUDE_ASM("asm/nonmatchings/42170", func_80041FB4_42BB4);

void func_80042070_42C70(Func41F38State *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

void func_800420A8_42CA8(void *arg0) {
    Func41F38State *task;

    task = scheduleTask(&func_80041F38_42B38, 0, 0, 0xC8);
    if (task != NULL) {
        task->unk3C = arg0;
    }
}

void func_800420E8_42CE8(Func420E8State *arg0) {
    getCurrentAllocation();
    arg0->unk20 = &D_8009A6E0_9B2E0;
    arg0->unk24 = loadAsset_B7E70();
    arg0->unk28 = loadAsset_216290();
    arg0->unk2C = 0;
    arg0->unk40 = 0;
    arg0->unk42 = 0;
    arg0->unk44 = 0xFF;
    func_80056B7C_5777C(&arg0->unk14, 0x14);
    setCleanupCallback(func_80042308_42F08);
    setCallbackWithContinue(func_80042160_42D60);
}

INCLUDE_ASM("asm/nonmatchings/42170", func_80042160_42D60);

INCLUDE_ASM("asm/nonmatchings/42170", func_80042254_42E54);

void func_80042308_42F08(Func420E8State *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

INCLUDE_ASM("asm/nonmatchings/42170", func_80042340_42F40);

INCLUDE_ASM("asm/nonmatchings/42170", func_800423A4_42FA4);

void func_80042638_43238(Func42410State *);

void func_80042410_43010(Func42410State *arg0) {
    getCurrentAllocation();
    arg0->unk20 = &D_8009A6F0_9B2F0;
    arg0->unk24 = loadAsset_B7E70();
    arg0->unk28 = loadAsset_216290();
    arg0->unk40 = 0x200;
    arg0->unk2C = 0;
    arg0->unk46 = 1;
    setCleanupCallback(func_80042638_43238);
    setCallbackWithContinue(func_8004247C_4307C);
}

INCLUDE_ASM("asm/nonmatchings/42170", func_8004247C_4307C);

INCLUDE_ASM("asm/nonmatchings/42170", func_8004256C_4316C);

void func_80042638_43238(Func42410State *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

void func_80042670_43270(void *arg0) {
    Func42410State *task;

    task = (Func42410State *)scheduleTask(func_80042410_43010, 0, 0, 0xC8);
    if (task != NULL) {
        task->unk3C = arg0;
    }
}

typedef struct {
    u8 pad0[0x20];  /* 0x00 */
    void *unk20;    /* 0x20 */
    void *unk24;    /* 0x24 */
    void *unk28;    /* 0x28 */
    s32 unk2C;      /* 0x2C */
    u8 pad30[0x14]; /* 0x30 */
    s32 unk44;      /* 0x44 */
    s32 unk48;      /* 0x48 */
    s32 unk4C;      /* 0x4C */
    s32 unk50;      /* 0x50 */
} Func426B0State;

void func_8004273C_4333C(void *);

typedef struct {
    u8 pad0[0x24];
    void *unk24;
    void *unk28;
} Func429C4Arg;

void func_800429C4_435C4(Func429C4Arg *);

void func_800426B0_432B0(Func426B0State *arg0) {
    if (arg0->unk44 == 0) {
        getCurrentAllocation();
        arg0->unk20 = &D_8009A700_9B300;
        arg0->unk24 = loadAsset_B7E70();
        arg0->unk28 = loadAsset_216290();
        arg0->unk48 = 0x400;
        arg0->unk2C = 0;
        arg0->unk4C = 0x100000;
        arg0->unk50 = 0;
        setCleanupCallback(func_800429C4_435C4);
        setCallbackWithContinue(func_8004273C_4333C);
    } else {
        arg0->unk44 = arg0->unk44 - 1;
    }
}

INCLUDE_ASM("asm/nonmatchings/42170", func_8004273C_4333C);

INCLUDE_ASM("asm/nonmatchings/42170", func_80042820_43420);

INCLUDE_ASM("asm/nonmatchings/42170", func_8004290C_4350C);

void func_800429C4_435C4(Func429C4Arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

INCLUDE_ASM("asm/nonmatchings/42170", func_800429FC_435FC);

INCLUDE_ASM("asm/nonmatchings/42170", func_80042A6C_4366C);

void func_80042F2C_43B2C(void **);
void func_80042BA4_437A4(void);

void func_80042B64_43764(void **arg0) {
    *arg0 = load_3ECE40();
    setCleanupCallback(func_80042F2C_43B2C);
    setCallbackWithContinue(func_80042BA4_437A4);
}

INCLUDE_ASM("asm/nonmatchings/42170", func_80042BA4_437A4);

INCLUDE_ASM("asm/nonmatchings/42170", func_80042C98_43898);

INCLUDE_ASM("asm/nonmatchings/42170", func_80042D54_43954);

INCLUDE_ASM("asm/nonmatchings/42170", func_80042E40_43A40);

void func_80042F2C_43B2C(void **arg0) {
    *arg0 = freeNodeMemory(*arg0);
}

INCLUDE_ASM("asm/nonmatchings/42170", func_80042F58_43B58);

INCLUDE_ASM("asm/nonmatchings/42170", func_80042FC0_43BC0);

extern void func_8004309C_43C9C(void);

typedef struct {
    u8 pad0[0x24];
    void *unk24;
    void *unk28;
} Func432D8Arg;

void func_800432D8_43ED8(Func432D8Arg *);

typedef struct {
    u8 pad0[0x20];  /* 0x00 */
    void *unk20;    /* 0x20 */
    void *unk24;    /* 0x24 */
    void *unk28;    /* 0x28 */
    s32 unk2C;      /* 0x2C */
    u8 pad30[0x2C]; /* 0x30 */
    void *unk5C;    /* 0x5C */
    void *unk60;    /* 0x60 */
    void *unk64;    /* 0x64 */
    s32 unk68;      /* 0x68 */
    u8 pad6C[0x18]; /* 0x6C */
    s16 unk84;      /* 0x84 */
} Func43018State;

void func_80043018_43C18(Func43018State *arg0) {
    getCurrentAllocation();
    arg0->unk20 = &D_8009A710_9B310;
    arg0->unk24 = loadAsset_B7E70();
    arg0->unk28 = loadAsset_216290();
    arg0->unk5C = &D_8009A720_9B320;
    arg0->unk2C = 0;
    arg0->unk68 = 0;
    arg0->unk84 = 0x200;
    arg0->unk60 = arg0->unk24;
    arg0->unk64 = arg0->unk28;
    setCleanupCallback(func_800432D8_43ED8);
    setCallbackWithContinue(func_8004309C_43C9C);
}

INCLUDE_ASM("asm/nonmatchings/42170", func_8004309C_43C9C);

INCLUDE_ASM("asm/nonmatchings/42170", func_8004320C_43E0C);

void func_800432D8_43ED8(Func432D8Arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

INCLUDE_ASM("asm/nonmatchings/42170", func_80043310_43F10);

extern void *D_8009A730_9B330;
extern void func_800433EC_43FEC(void);
void func_8004367C_4427C(Func432D8Arg *);

typedef struct {
    u8 pad0[0x20];  /* 0x00 */
    void *unk20;    /* 0x20 */
    void *unk24;    /* 0x24 */
    void *unk28;    /* 0x28 */
    s32 unk2C;      /* 0x2C */
    u8 pad30[0x30]; /* 0x30 */
    void *unk60;    /* 0x60 */
    void *unk64;    /* 0x64 */
    s32 unk68;      /* 0x68 */
    u8 pad6C[0x16]; /* 0x6C */
    s16 unk82;      /* 0x82 */
} Func43374State;

void func_80043374_43F74(Func43374State *arg0) {
    getCurrentAllocation();
    arg0->unk20 = &D_8009A730_9B330;
    arg0->unk24 = loadAsset_B7E70();
    arg0->unk28 = loadAsset_216290();
    arg0->unk2C = 0;
    arg0->unk68 = 0;
    arg0->unk82 = 0x200;
    arg0->unk60 = arg0->unk24;
    arg0->unk64 = arg0->unk28;
    setCleanupCallback(func_8004367C_4427C);
    setCallbackWithContinue(func_800433EC_43FEC);
}

INCLUDE_ASM("asm/nonmatchings/42170", func_800433EC_43FEC);

INCLUDE_ASM("asm/nonmatchings/42170", func_800435F0_441F0);

void func_8004367C_4427C(Func432D8Arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

INCLUDE_ASM("asm/nonmatchings/42170", func_800436B4_442B4);

INCLUDE_ASM("asm/nonmatchings/42170", func_80043718_44318);

INCLUDE_ASM("asm/nonmatchings/42170", func_800437C4_443C4);

void func_80043D30_44930(void **);
void func_800438A0_444A0(void);

void func_80043860_44460(void **arg0) {
    *arg0 = load_3ECE40();
    setCleanupCallback(func_80043D30_44930);
    setCallbackWithContinue(func_800438A0_444A0);
}

INCLUDE_ASM("asm/nonmatchings/42170", func_800438A0_444A0);

INCLUDE_ASM("asm/nonmatchings/42170", func_8004393C_4453C);

INCLUDE_ASM("asm/nonmatchings/42170", func_800439F4_445F4);

INCLUDE_ASM("asm/nonmatchings/42170", func_80043AB4_446B4);

void func_80043C00_44800(Func43CA4Arg *arg0) {
    Func43CA4GameState *gameState;
    s32 i;

    gameState = (Func43CA4GameState *)getCurrentAllocation();
    if (gameState->unk76 == 0) {
        if (func_80043718_44318(arg0, &D_80090964_91564) != 0) {
            arg0->unk40 = 0;
            arg0->unk42 = 1;
            func_80056B7C_5777C(&arg0->unk8, 0x12);
            setCallback(func_80043CA4_448A4);
        }
    }

    transformVector(arg0->unk2C, arg0->unk28->unk9F0, &arg0->unk8);

    for (i = 0; i < 4; i++) {
        func_80066444_67044(i, &arg0->unk4);
    }
}

void func_80043CA4_448A4(Func43CA4Arg *arg0) {
    Func43CA4GameState *gameState;
    s32 i;

    gameState = (Func43CA4GameState *)getCurrentAllocation();
    if (gameState->unk76 == 0) {
        if (func_80043718_44318(arg0, &D_80090974_91574) != 0) {
            func_80069CF8_6A8F8();
        }
    }

    transformVector(arg0->unk2C, arg0->unk28->unk9F0, &arg0->unk8);

    for (i = 0; i < 4; i++) {
        func_80066444_67044(i, &arg0->unk4);
    }
}

void func_80043D30_44930(void **arg0) {
    *arg0 = freeNodeMemory(*arg0);
}

INCLUDE_ASM("asm/nonmatchings/42170", func_80043D5C_4495C);

void func_80043DC0_449C0(Func43DC0State *arg0) {
    getCurrentAllocation();
    arg0->unk20 = &D_8009A760_9B360;
    arg0->unk24 = loadAsset_B7E70();
    arg0->unk28 = loadAsset_216290();
    arg0->unk2C = 0;
    arg0->unk46 = 0x200;
    setCleanupCallback(func_80044018_44C18);
    setCallbackWithContinue(func_80043E24_44A24);
}

INCLUDE_ASM("asm/nonmatchings/42170", func_80043E24_44A24);

INCLUDE_ASM("asm/nonmatchings/42170", func_80043F8C_44B8C);

void func_80044018_44C18(Func43DC0State *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

INCLUDE_ASM("asm/nonmatchings/42170", func_80044050_44C50);

INCLUDE_ASM("asm/nonmatchings/42170", func_800440B4_44CB4);

INCLUDE_ASM("asm/nonmatchings/42170", func_800441A4_44DA4);

void func_80044500_45100(Func432D8Arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

void func_80044538_45138(s16 arg0) {
    Func44538TaskMem *task = (Func44538TaskMem *)scheduleTask(func_800440B4_44CB4, 0, 0, 0x32);
    if (task != NULL) {
        task->unk3C = arg0;
    }
}

INCLUDE_ASM("asm/nonmatchings/42170", func_80044578_45178);

INCLUDE_ASM("asm/nonmatchings/42170", func_80044684_45284);

INCLUDE_ASM("asm/nonmatchings/42170", func_800447D4_453D4);

INCLUDE_ASM("asm/nonmatchings/42170", func_80044888_45488);

INCLUDE_ASM("asm/nonmatchings/42170", func_80044990_45590);

INCLUDE_ASM("asm/nonmatchings/42170", func_80044AB8_456B8);

void func_80044BBC_457BC(Func44BBCArg *arg0) {
    Func43CA4GameState *gameState = (Func43CA4GameState *)getCurrentAllocation();
    s32 pad[4];

    if (gameState->unk76 == 0) {
        arg0->unkC4--;
        if ((s16)arg0->unkC4 == 0) {
            setCallback(func_80044C38_45838);
        }
        memcpy(&arg0->unkB4->unkB44, &arg0->unk14, 0xC);
        arg0->unkB4->unkB74 = arg0->unkC6;
    }

    func_80044578_45178(arg0);
}

void func_80044C38_45838(Func44BBCArg *arg0) {
    Func43CA4GameState *gameState = (Func43CA4GameState *)getCurrentAllocation();
    s32 pad[4];

    if (gameState->unk76 == 0) {
        arg0->unkBC -= 0x8000;
        if (arg0->unkBC < (s32)0xFFF00000) {
            func_80069CF8_6A8F8();
        }
        arg0->unk18 += arg0->unkBC;
    }

    func_80044578_45178(arg0);
}

void func_80044CA4_458A4(Func432D8Arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

typedef struct {
    u8 _pad0[0xB4];
    void *unkB4;
} Func44CDCTaskMem;

void func_80044CDC_458DC(void *arg0) {
    Func44CDCTaskMem *task;

    task = (Func44CDCTaskMem *)scheduleTask(func_80044684_45284, 0, 0, 0x32);
    if (task != NULL) {
        task->unkB4 = arg0;
    }
}

INCLUDE_ASM("asm/nonmatchings/42170", func_80044D1C_4591C);

INCLUDE_ASM("asm/nonmatchings/42170", func_80044DB0_459B0);

INCLUDE_ASM("asm/nonmatchings/42170", func_80044EC4_45AC4);

typedef struct {
    void *unk0;   /* 0x00 */
    u8 pad4[0x8]; /* 0x04 */
    void *unkC;   /* 0x0C */
    void *unk10;  /* 0x10 */
} Func45010Arg;

void func_80045010_45C10(Func45010Arg *arg0) {
    arg0->unk0 = freeNodeMemory(arg0->unk0);
    arg0->unkC = freeNodeMemory(arg0->unkC);
    arg0->unk10 = freeNodeMemory(arg0->unk10);
}

INCLUDE_ASM("asm/nonmatchings/42170", func_80045054_45C54);

INCLUDE_ASM("asm/nonmatchings/42170", func_80045434_46034);
