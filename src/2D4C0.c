#include "297B0.h"
#include "2C8F0.h"
#include "2DD40.h"
#include "B040.h"
#include "common.h"
#include "gamestate.h"
#include "rand.h"
#include "task_scheduler.h"

typedef struct {
    s32 unk0;
    Mat3x3Padded unk4;
    /* 0x24 */ u8 pad2[0x8];
    /* 0x2C */ u16 unk2C;
    /* 0x2E */ u16 unk2E;
    /* 0x30 */ u8 pad3[0x10];
    /* 0x40 */ s32 unk40;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ u8 pad3b[0x8];
    /* 0x50 */ s16 unk50;
    /* 0x52 */ s16 unk52;
    /* 0x54 */ u8 pad4[0x8];
    /* 0x5C */ u8 unk5C;
    /* 0x5D */ u8 pad5[0x5];
    /* 0x62 */ s8 unk62;
    /* 0x63 */ s8 unk63;
} Func8002CB88Arg_arg0;

typedef struct Func8002CB88Arg Func8002CB88Arg;
struct Func8002CB88Arg {
    /* 0x00 */ Func8002CB88Arg_arg0 unk0[2];
    /* 0xC8 */ void (*unkC8)(Func8002CB88Arg *);
    /* 0xCC */ u16 unkCC[1];
    /* 0xCE */ u16 unkCE;
    /* 0xD0 */ u8 pad9[0x5];
    /* 0xD5 */ u8 unkD5;
};

extern u16 D_8009ADE0_9B9E0;

typedef struct Func8002CD3CArg Func8002CD3CArg;
struct Func8002CD3CArg {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u8 pad0[0x14];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 pad1[0x4];
    /* 0x20 */ s32 unk20;
    /* 0x24 */ u8 pad2[0x1C];
    /* 0x40 */ s32 unk40;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ u8 pad3[0x8];
    /* 0x50 */ s16 unk50;
    /* 0x52 */ u8 pad3b[0x10];
    /* 0x62 */ s8 unk62;
    /* 0x63 */ u8 pad5[0x1];
    /* 0x64 */ s32 unk64;
    /* 0x68 */ u8 pad6[0x3C];
    /* 0xA4 */ s32 unkA4;
    /* 0xA8 */ u8 pad7[0x20];
    /* 0xC8 */ void (*unkC8)(Func8002CD3CArg *);
    /* 0xCC */ u16 unkCC;
    /* 0xCE */ u16 unkCE;
    /* 0xD0 */ u8 pad12[0x5];
    /* 0xD5 */ u8 unkD5;
};

extern u8 identityMatrix[];
extern u16 D_8008EF70_8FB70[];

void func_8002CD3C_2D93C(Func8002CB88Arg *);

INCLUDE_ASM("asm/nonmatchings/2D4C0", func_8002C8C0_2D4C0);

void func_8002CA8C_2D68C(Func8002CB88Arg *arg0) {
    GameState *allocation;
    s32 i;
    Func8002CB88Arg_arg0 *ptr;

    allocation = getCurrentAllocation();

    for (i = 0; i < arg0->unkD5; i++) {
        ptr = &arg0->unk0[i];

        if (ptr->unk50 != 0x19) {
            if (ptr->unk50 == 0x1A) {
                ptr->unk62 = 0;
            }
        } else {
            if (ptr->unk62 != 0) {
                ptr->unk50 = 0x1A;
            }
        }

        func_8002A2D0_2AED0((Func297D8Arg *)ptr);
        allocation->unk408[i] = ptr->unk4.unk14;
        allocation->unk410[i] = ptr->unk4.unk1C;
    }

    if (allocation->unk42A == 0x11) {
        func_8002EBB0_2F7B0(arg0);
        arg0->unkC8 = func_8002CA8C_2D68C;
        setCallback(func_8002BEF4_2CAF4);
    }
}

void func_8002CB88_2D788(Func8002CB88Arg *arg0) {
    GameState *allocation;
    s32 vec3[3];
    s32 i;

    allocation = getCurrentAllocation();
    vec3[2] = 0;
    vec3[0] = 0;
    vec3[1] = 0x280000;

    for (i = 0; i < arg0->unkD5; i++) {
        memcpy(&arg0->unk0[i].unk4, identityMatrix, sizeof(Mat3x3Padded));
        arg0->unk0[i].unk62 = 0;
        arg0->unkCC[i] = 0;
        if (i == 0) {
            arg0->unk0[0].unk4.unk14 = 0xFFF40000;
            arg0->unk0[0].unk4.unk1C = 0xFFB80000;
        } else {
            arg0->unk0[1].unk4.unk14 = 0x180000;
            arg0->unk0[1].unk4.unk1C = 0xFFB80000;
        }

        arg0->unk0[i].unk2C = 0x800 + i * 0x1000;
        arg0->unk0[i].unk50 = 0x13;
        arg0->unk0[i].unk52 = 0x13;
        arg0->unk0[i].unk2E = 0x800 + i * 0x1000;

        createYRotationMatrix(&arg0->unk0[i].unk4, arg0->unk0[i].unk2C);

        memcpy(&arg0->unk0[i].unk40, vec3, 0xC);

        if (i == 0) {
            arg0->unk0[i].unk44 = 0x290000;
            arg0->unk0[i].unk40 += 0xFFF70000;
        } else {
            arg0->unk0[i].unk44 = 0x240000;
            arg0->unk0[i].unk40 += 0x60000;
        }

        func_8002A290_2AE90((Func297D8Arg *)&arg0->unk0[i]);
        allocation->unk408[i] = arg0->unk0[i].unk4.unk14;
        allocation->unk410[i] = arg0->unk0[i].unk4.unk1C;
        allocation->unk418[i] = D_8008EF70_8FB70[arg0->unk0[i].unk5C];
    }

    arg0->unkCC[0] = (randB() & 0x1F) + 0x28;
    setCallback(func_8002CD3C_2D93C);
}

void func_8002CD3C_2D93C(Func8002CB88Arg *arg0) {
    GameState *allocation;
    s32 i;
    Func8002CB88Arg_arg0 *ptr;
    s32 temp_a0;

    allocation = getCurrentAllocation();

    for (i = 0; i < arg0->unkD5; i++) {
        ptr = &arg0->unk0[i];

        switch (ptr->unk50) {
            case 0x13:
                if (ptr->unk62 != 0) {
                    ptr->unk50 = 0x14;
                }
                break;
            case 0x14:
                ptr->unk62 = 0;
                break;
        }

        func_8002A2D0_2AED0((Func297D8Arg *)ptr);
        allocation->unk408[i] = ptr->unk4.unk14;
        allocation->unk410[i] = ptr->unk4.unk1C;
    }

    if (arg0->unkCC[0] != 0) {
        arg0->unkCC[0]--;
        if (arg0->unkCC[0] == 0) {
            temp_a0 = randB() & 0x1F;
            arg0->unkCE = (u8)temp_a0 + 0x18;
            if (D_8009ADE0_9B9E0 & 1) {
                arg0->unk0[0].unk44 = 0x290000;
                spawnSpriteEffectEx(
                    (SceneModel *)arg0->unk0[0].unk0,
                    0,
                    8,
                    (s16)(arg0->unkCE - 4),
                    &arg0->unk0[0].unk40,
                    0x10000,
                    0,
                    2,
                    0,
                    0
                );
            } else {
                arg0->unkCE = (u8)temp_a0 + 0x2C;
                arg0->unk0[0].unk44 = 0x290000;
                spawnSpriteEffectEx(
                    (SceneModel *)arg0->unk0[0].unk0,
                    0,
                    0x3E,
                    (s16)(arg0->unkCE - 4),
                    &arg0->unk0[0].unk40,
                    0x10000,
                    0,
                    2,
                    0,
                    0x400
                );
                spawnSpriteEffectEx(
                    (SceneModel *)arg0->unk0[1].unk0,
                    0,
                    0x3E,
                    (s16)arg0->unkCE,
                    &arg0->unk0[1].unk40,
                    0x10000,
                    0,
                    2,
                    0,
                    0x1F00
                );
            }
        }
    } else {
        arg0->unkCE--;
        if (arg0->unkCE == 0) {
            arg0->unkCC[0] = (randB() & 0x1F) + 0x28;
        }
    }

    if (allocation->unk42A == 0x11) {
        func_8002EBB0_2F7B0(arg0);
        arg0->unkC8 = func_8002CD3C_2D93C;
        allocation->unk42E = 1;
        arg0->unkCC[0] = (randB() & 0x1F) + 0x28;
        arg0->unkCE = 0;
        setCallback(func_8002BEF4_2CAF4);
    }
}

INCLUDE_ASM("asm/nonmatchings/2D4C0", func_8002CFAC_2DBAC);
