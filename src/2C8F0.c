#include "20F0.h"
#include "297B0.h"
#include "D_800AFE8C_A71FC_type.h"
#include "common.h"
#include "geometry.h"
#include "task_scheduler.h"

extern void *D_8008EBF0_8F7F0[];
extern void func_8002BFEC_2CBEC(void *);
extern void func_8002C570_2D170(void *);

extern void func_8000A49C_B09C(s32, s32, s32, s32, void *, s32, s32, s32, s32, s32);
extern u8 D_8008EBE0_8F7E0[];
extern void *D_8008ECF0_8F8F0;
extern void *D_8008ED00_8F900;

typedef struct {
    /* 0x00 */ SceneModel *unk0;
    /* 0x04 */ u8 pad4[0x3C];
    /* 0x40 */ s32 unk40;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ u8 pad48[0x4];
    /* 0x4C */ void *unk4C;
    /* 0x50 */ u8 pad50[0x14];
} Elem2C8F0; // size 0x64

typedef struct {
    /* 0x00 */ Elem2C8F0 elems[2];
    /* 0xC8 */ u8 padC8[0xC];
    /* 0xD4 */ u8 unkD4;
    /* 0xD5 */ u8 unkD5;
} Struct2C8F0;

INCLUDE_ASM("asm/nonmatchings/2C8F0", func_8002BCF0_2C8F0);

void func_8002BE44_2CA44(Struct2C8F0 *arg0) {
    GameState *allocation = (GameState *)getCurrentAllocation();

    if (allocation->unk429 == 0) {
        setCallback(D_8008EBF0_8F7F0[arg0->unkD4]);
    }
}

void func_8002BE8C_2CA8C(void) {
}

void func_8002BE94_2CA94(Struct2C8F0 *arg0) {
    s32 i;

    for (i = 0; i < arg0->unkD5; i++) {
        arg0->elems[i].unk0 = func_80002014_2C14(arg0->elems[i].unk0);
    }
}

typedef struct {
    /* 0x00 */ void *unk0;
    /* 0x04 */ Mat3x3Padded unk4;
    /* 0x24 */ u8 pad24[0xC];
    /* 0x30 */ u16 unk30;
    /* 0x32 */ u8 pad32[0x96];
    /* 0xC8 */ void *unkC8;
    /* 0xCC */ u8 padCC[0x7];
    /* 0xD3 */ u8 unkD3;
} Func8002BEF4Arg;

void func_8002BEF4_2CAF4(Func8002BEF4Arg *arg0) {
    GameState *allocation;
    s32 i;
    Func8002BEF4Arg *ptr;

    allocation = (GameState *)getCurrentAllocation();

    switch (arg0->unkD3) {
        case 0:
            func_8002BFEC_2CBEC(arg0);
            for (i = 0; i < allocation->unk41C; i++) {
                ptr = (Func8002BEF4Arg *)((u8 *)arg0 + i * 0x64);
                createYRotationMatrix(&ptr->unk4, ptr->unk30);
                func_8002A2D0_2AED0(ptr);
            }
            break;
        case 1:
            func_8002C570_2D170(arg0);
            for (i = 0; i < allocation->unk41C; i++) {
                ptr = (Func8002BEF4Arg *)((u8 *)arg0 + i * 0x64);
                createYRotationMatrix(&ptr->unk4, ptr->unk30);
                func_8002A2D0_2AED0(ptr);
            }
            break;
    }

    if (allocation->unk42A == 0) {
        setCallback(arg0->unkC8);
    }
}

INCLUDE_ASM("asm/nonmatchings/2C8F0", func_8002BFEC_2CBEC);

INCLUDE_ASM("asm/nonmatchings/2C8F0", func_8002C570_2D170);

void func_8002C798_2D398(Struct2C8F0 *arg0, s32 arg1) {
    Elem2C8F0 *elem;
    u8 d4;
    s32 offset;

    d4 = arg0->unkD4;

    if ((d4 == 1) & (arg1 == 0)) {
        if (D_800AFE8C_A71FC->unk9[0] == 3) {
            elem = &arg0->elems[arg1];
            elem->unk4C = &D_8008ED00_8F900;
            arg0->elems[0].unk44 = 0x300000;
            func_8000A49C_B09C(
                (s32)elem->unk0,
                0,
                0x1F,
                D_8008EBE0_8F7E0[arg0->unkD4 * 2 + arg1] - 4,
                &elem->unk40,
                0x10000,
                0,
                2,
                0,
                0
            );
            return;
        }
    }

    if ((arg0->unkD4 == 6) & (arg1 == 1)) {
        if (D_800AFE8C_A71FC->unk9[0] == 3) {
            offset = arg1 * 0x64;
            ((Elem2C8F0 *)((u8 *)arg0 + offset))->unk4C = &D_8008ECF0_8F8F0;
        }
    }
}
