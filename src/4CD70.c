#include "19E80.h"
#include "56910.h"
#include "5E590.h"
#include "68CF0.h"
#include "6E840.h"
#include "D_800AFE8C_A71FC_type.h"
#include "common.h"
#include "gamestate.h"
#include "overlay.h"
#include "task_scheduler.h"

USE_ASSET(_3F3940);
USE_ASSET(_3F6950);
USE_ASSET(_3F3EF0);

extern void *loadAsset_34CB50(void);
extern u8 randA(void);
extern void func_8004E2D8_4EED8(void);
extern void func_8004E410_4F010(void);
extern void func_8000FED0_10AD0(void);
extern void func_80058530_59130(s32, s32);
extern s32 approximateSin(s16);
extern void func_80069CF8_6A8F8(void);

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
    u8 padA[0x2];
    s32 unkC;
    s32 unk10;
    s32 unk14;
} Struct_func_8004D134;

void func_8004D19C_4DD9C(Struct_func_8004D134 *arg0);
void func_8004D23C_4DE3C(Struct_func_8004D134 *arg0);
void func_8004D298_4DE98(Struct_func_8004D134 *arg0);
void func_8004D338_4DF38(Struct_func_8004D134 *arg0);

typedef struct {
    u8 pad0[0x4];
    void *unk4;
    u8 pad8[0x8];
    void *unk10;
} Struct_func_8004DCC4;

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    u16 unk8;
    u16 unkA;
    u32 unkC;
} Struct_func_8004D3A4;

void func_8004D3E4_4DFE4(Struct_func_8004D3A4 *);
void func_8004D464_4E064(Struct_func_8004D3A4 *);

typedef struct {
    void *unk0;
    s16 unk4;
    s16 unk6;
    void *unk8;
    s16 unkC;
    u8 padE[0x2];
    s16 unk10;
    s16 unk12;
    s16 unk14;
} Struct_func_8004FF28;

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
    u8 padA[0x2];
    void *unkC;
    s16 unk10;
} Struct_func_8004F04C;

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
    u8 padA[0x2];
    s16 unkC;
    s16 unkE;
    void *unk10;
    s16 unk14;
} Struct_func_8004FFB8;

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
    u8 padA[0x2];
    Player *unkC;
    s32 unk10;
} Struct_func_8004D8E4;

void func_8004D954_4E554(Struct_func_8004D8E4 *arg0);
void func_8004D98C_4E58C(Struct_func_8004F04C *arg0);

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
    u8 padA[0x2];
    void *unkC;
    char unk10[8];
    s32 unk18;
    u8 pad1C[0x2];
    s16 unk1E;
    s16 unk20;
    u8 pad22[0x2];
    void *unk24;
    u8 pad28[0x2];
    u16 unk2A;
    s16 unk2C;
    s16 unk2E;
} Struct_func_8004D784;

void func_8004D7D0_4E3D0(Struct_func_8004D784 *arg0);
void func_8004D784_4E384(Struct_func_8004D784 *arg0);
void func_8004D6FC_4E2FC(Struct_func_8004D784 *arg0);
void func_8004D858_4E458(Struct_func_8004F04C *arg0);
extern void func_8004D63C_4E23C(void *);
extern void func_8003BD60_3C960(char *, s16, s16, s16, void *, u16, u16);
extern char D_8009E8A8_9F4A8[];

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004C170_4CD70);

void func_8004C254_4CE54(Struct_func_8004D8E4 *arg0) {
    arg0->unk8 = arg0->unkC->unkBC4;
    debugEnqueueCallback((u16)(arg0->unk10 + 8), 0, func_8000FED0_10AD0, arg0);
}

void func_8004C294_4CE94(Struct_func_8004F04C *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004C2C0_4CEC0);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004C46C_4D06C);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004C5A8_4D1A8);

typedef struct {
    u8 pad0[0x4];
    void *unk4;
    u8 pad8[0x14];
    void *unk1C;
} Struct_func_8004C6F0;

void func_8004C6F0_4D2F0(Struct_func_8004C6F0 *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
    arg0->unk1C = freeNodeMemory(arg0->unk1C);
}

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004C728_4D328);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004C928_4D528);

typedef struct {
    u8 pad0[0x30];
    TextData unk30;
    u8 unk3C;
    u8 pad3D[0x7];
    Player *unk44;
    s32 unk48;
} Struct_func_8004C9E8;

void func_8004C9E8_4D5E8(Struct_func_8004C9E8 *arg0) {
    debugEnqueueCallback((u16)(arg0->unk48 + 8), 0, func_8000FED0_10AD0, arg0);
    arg0->unk3C = arg0->unk44->unkBC5 + 0x31;
    debugEnqueueCallback((u16)(arg0->unk48 + 8), 0, renderTextPalette, &arg0->unk30);
}

void func_8004CA58_4D658(Struct_func_8004DCC4 *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
    arg0->unk10 = freeNodeMemory(arg0->unk10);
}

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004CA90_4D690);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004CBF0_4D7F0);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004CCC4_4D8C4);

typedef struct {
    void *unk0;
    u8 pad4[0x8];
    void *unkC;
} Struct_func_8004CD88;

void func_8004CD88_4D988(Struct_func_8004CD88 *arg0) {
    arg0->unk0 = freeNodeMemory(arg0->unk0);
    arg0->unkC = freeNodeMemory(arg0->unkC);
}

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004CDC0_4D9C0);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004CEC4_4DAC4);

typedef struct {
    void *unk0;
    u8 pad4[0x10];
} ArrayElement_D0D0; // size 0x14

typedef struct {
    u8 pad0[0x4];
    void *unk4;
    u8 pad8[0x8];
    ArrayElement_D0D0 arr[4]; // offset 0x10
} Struct_func_8004D0D0;

void func_8004D0D0_4DCD0(Struct_func_8004D0D0 *arg0) {
    s32 i;

    for (i = 0; i < 4; i++) {
        arg0->arr[i].unk0 = freeNodeMemory(arg0->arr[i].unk0);
    }
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_8004D134_4DD34(Struct_func_8004D134 *arg0) {
    arg0->unk4 = loadAsset_34CB50();
    arg0->unk8 = 0xD;
    func_80058530_59130(0x11B, 6);
    arg0->unk10 = 0x2D;
    arg0->unk14 = 0;
    arg0->unk2 = -8;
    setCleanupCallback(func_8004D338_4DF38);
    setCallback(func_8004D19C_4DD9C);
}

void func_8004D19C_4DD9C(Struct_func_8004D134 *arg0) {
    s32 sinVal;
    s32 temp;

    temp = arg0->unk14 + 0x80;
    arg0->unk14 = temp;
    sinVal = approximateSin((s16)temp);
    arg0->unk0 = (0x2000 - sinVal) / 20 - 0x38;
    if (arg0->unk14 == 0x800) {
        setCallback(func_8004D23C_4DE3C);
    }
    debugEnqueueCallback((u16)(arg0->unkC + 8), 6, func_8000FED0_10AD0, arg0);
}

void func_8004D23C_4DE3C(Struct_func_8004D134 *arg0) {
    arg0->unk10--;
    if (arg0->unk10 == 0) {
        setCallback(func_8004D298_4DE98);
    }
    debugEnqueueCallback((u16)(arg0->unkC + 8), 6, func_8000FED0_10AD0, arg0);
}

void func_8004D298_4DE98(Struct_func_8004D134 *arg0) {
    s32 sinVal;
    s32 temp;

    temp = arg0->unk14 + 0x80;
    arg0->unk14 = temp;
    sinVal = approximateSin((s16)temp);
    arg0->unk0 = -((0x2000 - sinVal) / 20) - 0x38;
    if (arg0->unk14 == 0x1000) {
        func_80069CF8_6A8F8();
    }
    debugEnqueueCallback((u16)(arg0->unkC + 8), 6, func_8000FED0_10AD0, arg0);
}

void func_8004D338_4DF38(Struct_func_8004D134 *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_8004D364_4DF64(s32 arg0) {
    Struct_func_8004D134 *task;

    task = scheduleTask(func_8004D134_4DD34, 0, 0, 0xE6);
    if (task != NULL) {
        task->unkC = arg0;
    }
}

void func_8004D3A4_4DFA4(Struct_func_8004D3A4 *arg0) {
    arg0->unk4 = loadAsset_34CB50();
    setCleanupCallback(func_8004D464_4E064);
    setCallback(func_8004D3E4_4DFE4);
}

void func_8004D3E4_4DFE4(Struct_func_8004D3A4 *arg0) {
    OutputStruct_19E80 output;

    getTableEntryByU16Index(arg0->unk4, arg0->unk8, &output);
    arg0->unk0 = -output.field1 / 2;
    arg0->unk2 = -output.field2 / 2;
    debugEnqueueCallback((u16)(arg0->unkC + 8), 6, func_8000FED0_10AD0, arg0);
}

void func_8004D464_4E064(Struct_func_8004D3A4 *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

extern u8 D_80090E60_91A60[];

void func_8004D490_4E090(u32 arg0, s32 arg1) {
    Struct_func_8004D3A4 *task;

    task = scheduleTask(func_8004D3A4_4DFA4, 0, 1, 0xE6);
    if (task != NULL) {
        task->unkC = arg0;
        task->unk8 = D_80090E60_91A60[arg1];

        switch (arg1) {
            case 0:
                func_80058530_59130(0x118, 6);
                break;
            case 1:
                func_80058530_59130(0x119, 6);
                break;
            case 2:
                func_80058530_59130(0x11A, 6);
                break;
            case 3:
                func_80058530_59130(0x134, 6);
                break;
        }
    }
}

INCLUDE_RODATA("asm/nonmatchings/4CD70", D_8009E884_9F484);

extern char D_8009E884_9F484[];

static const char D_8009E88C_9F48C[] = "%4dG";

void func_8004D544_4E144(Struct_func_8004D784 *arg0) {
    GameState *allocation = (GameState *)getCurrentAllocation();

    if (allocation->unk5F == 1) {
        arg0->unk2E = 0;
    } else {
        arg0->unk2E = 1;
    }
    arg0->unk4 = loadAsset_34CB50();
    if (arg0->unk2E == 0) {
        arg0->unk2 = -0x20;
        arg0->unk8 = 0xF;
        arg0->unkC = dmaRequestAndUpdateStateWithSize(&_3F6950_ROM_START, &_3F6950_ROM_END, 0x508);
        sprintf(arg0->unk10, D_8009E884_9F484, arg0->unk18);
    } else {
        arg0->unk2 = -0x18;
        arg0->unk8 = 0x10;
        arg0->unkC = 0;
        arg0->unk20 = 4;
        arg0->unk24 = arg0->unk10;
        arg0->unk1E = arg0->unk2;
        sprintf(arg0->unk10, D_8009E88C_9F48C, arg0->unk18);
    }
    arg0->unk2C = 0;
    arg0->unk2A = 0x1E;
    setCleanupCallback(func_8004D858_4E458);
    setCallback(func_8004D6FC_4E2FC);
}

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004D63C_4E23C);

void func_8004D6FC_4E2FC(Struct_func_8004D784 *arg0) {
    s32 sinVal;
    s16 angle;

    angle = arg0->unk2C + 0x80;
    arg0->unk2C = angle;
    sinVal = approximateSin(angle);
    arg0->unk0 = (0x2000 - sinVal) / 20 - 0x30;
    if (arg0->unk2C == 0x800) {
        setCallback(func_8004D784_4E384);
    }
    func_8004D63C_4E23C(arg0);
}

void func_8004D784_4E384(Struct_func_8004D784 *arg0) {
    arg0->unk2A--;
    if ((s16)arg0->unk2A == 0) {
        setCallback(func_8004D7D0_4E3D0);
    }
    func_8004D63C_4E23C(arg0);
}

void func_8004D7D0_4E3D0(Struct_func_8004D784 *arg0) {
    s32 sinVal;
    s16 angle;

    angle = arg0->unk2C + 0x80;
    arg0->unk2C = angle;
    sinVal = approximateSin(angle);
    arg0->unk0 = -((0x2000 - sinVal) / 20) - 0x30;
    if (arg0->unk2C == 0x1000) {
        func_80069CF8_6A8F8();
    }
    func_8004D63C_4E23C(arg0);
}

void func_8004D858_4E458(Struct_func_8004F04C *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
    arg0->unkC = freeNodeMemory(arg0->unkC);
}

typedef struct {
    u8 pad0[0x18];
    void *unk18;
    u8 pad1C[0xC];
    s16 unk28;
} func_8004D890_task;

void func_8004D890_4E490(s32 arg0, void *arg1) {
    func_8004D890_task *task;

    task = scheduleTask(func_8004D544_4E144, 0, 0, 0xE6);
    if (task != NULL) {
        task->unk28 = (s16)arg0;
        task->unk18 = arg1;
    }
}

void func_8004D8E4_4E4E4(Struct_func_8004D8E4 *arg0) {
    GameState *allocation = (GameState *)getCurrentAllocation();

    arg0->unkC = allocation->players;
    arg0->unk4 = dmaRequestAndUpdateStateWithSize(&_3F3940_ROM_START, &_3F3940_ROM_END, 0x888);
    arg0->unk0 = -0x48;
    arg0->unk2 = -0x38;
    setCleanupCallback(&func_8004D98C_4E58C);
    setCallback(&func_8004D954_4E554);
}

void func_8004D954_4E554(Struct_func_8004D8E4 *arg0) {
    arg0->unk8 = arg0->unkC->unkBC4;
    debugEnqueueCallback(8, 6, &func_8000FED0_10AD0, arg0);
}

void func_8004D98C_4E58C(Struct_func_8004F04C *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

typedef struct {
    u8 pad0[0xA];
    s16 unkA;
    u8 unkC;
    u8 unkD;
    u8 unkE;
} Struct_func_8004D9B8;

void func_8004D9B8_4E5B8(Struct_func_8004D9B8 *arg0) {
    arg0->unkC = 0;
    arg0->unkD = 0;
    arg0->unkE = 0;
    arg0->unkA = 0xFF;
}

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004D9D0_4E5D0);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004DB98_4E798);

void func_8004DCC4_4E8C4(Struct_func_8004DCC4 *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
    arg0->unk10 = freeNodeMemory(arg0->unk10);
}

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
    u8 padA[0x6];
    void *unk10;
    s32 unk14;
} Struct_func_8004DCFC;

void func_8004DDD0_4E9D0(void);
void func_8004DEC0_4EAC0(Struct_func_8004DCC4 *);

void func_8004DCFC_4E8FC(Struct_func_8004DCFC *arg0) {
    GameState *allocation = (GameState *)getCurrentAllocation();

    arg0->unk14 = 0;
    arg0->unk4 = loadAsset_34CB50();
    func_8004D9B8_4E5B8((Struct_func_8004D9B8 *)arg0);
    arg0->unk8 = 0x15;
    arg0->unk10 = dmaRequestAndUpdateStateWithSize(&_3F6950_ROM_START, &_3F6950_ROM_END, 0x508);

    switch (allocation->unk7A) {
        case 5:
            arg0->unk0 = 0x10;
            arg0->unk2 = 8;
            break;
        case 6:
            arg0->unk0 = 0x10;
            arg0->unk2 = 8;
            break;
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        default:
            arg0->unk0 = 0x10;
            arg0->unk2 = -0x10;
            break;
    }

    setCleanupCallback(func_8004DEC0_4EAC0);
    setCallback(func_8004DDD0_4E9D0);
}

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004DDD0_4E9D0);

void func_8004DEC0_4EAC0(Struct_func_8004DCC4 *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
    arg0->unk10 = freeNodeMemory(arg0->unk10);
}

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
    u8 padA[0x6];
    void *unk10;
    s32 unk14;
    Player *unk18;
} Struct_func_8004DEF8;

void func_8004DFA0_4EBA0(void);
void func_8004E0BC_4ECBC(Struct_func_8004DCC4 *);

void func_8004DEF8_4EAF8(Struct_func_8004DEF8 *arg0) {
    D_800AFE8C_A71FC_type *global;

    getCurrentAllocation();
    arg0->unk14 = 0;
    arg0->unk4 = loadAsset_34CB50();
    func_8004D9B8_4E5B8((Struct_func_8004D9B8 *)arg0);
    global = D_800AFE8C_A71FC;
    arg0->unk8 = 0x16;
    arg0->unk0 = 0;

    if (global->unk9[arg0->unk18->unkBB8 + 0x11] < 10) {
        arg0->unk0 = -4;
    }

    arg0->unk2 = 0x10;
    arg0->unk10 = dmaRequestAndUpdateStateWithSize(&_3F6950_ROM_START, &_3F6950_ROM_END, 0x508);
    setCleanupCallback(func_8004E0BC_4ECBC);
    setCallback(func_8004DFA0_4EBA0);
}

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004DFA0_4EBA0);

void func_8004E0BC_4ECBC(Struct_func_8004DCC4 *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
    arg0->unk10 = freeNodeMemory(arg0->unk10);
}

void func_8004E0F4_4ECF4(Player *arg0) {
    Node *task;

    task = scheduleTask(func_8004DEF8_4EAF8, 1, 0, 0xE6);
    if (task != NULL) {
        task->unk18 = arg0;
    }
}

typedef struct {
    u8 pad0[0x4];
    void *unk4;
    u8 pad8[0x2];
    u8 unkA;
    u8 padB[0x7];
    s16 unk12;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
} Struct_func_8004E134;

void func_8004E614_4F214(Struct_func_8004E134 *arg0);

void func_8004E134_4ED34(Struct_func_8004E134 *arg0) {
    u8 temp;

    arg0->unk4 = loadAsset_34CB50();
    temp = randA();

    switch (temp & 1) {
        case 0:
            arg0->unk12 = ((randA() & 0xFF) - 0x80) << 4;
            arg0->unk14 = -0x780;
            arg0->unk16 = randA() & 0xF;
            arg0->unk1C = (randA() & 0xFF) << 4;
            if (randA() & 1) {
                arg0->unk1A = (randA() & 7) | 0x40;
            } else {
                arg0->unk1A = (randA() & 7) | 0x20;
            }
            setCallback(func_8004E410_4F010);
            break;
        case 1:
            arg0->unk12 = ((randA() & 0xFF) - 0x80) << 4;
            arg0->unk14 = -0x780;
            if (randA() & 1) {
                arg0->unk1A = (randA() & 7) + 0x3C;
            } else {
                arg0->unk1A = (randA() & 7) + 0x22;
            }
            arg0->unk1C = (randA() & 1) - 1;
            arg0->unk18 = (-((randA() & 0xFF) << 3)) & 0xF;
            arg0->unk1E = randA() & 0xF;
            __asm__ volatile("" ::: "memory");
            setCallback(func_8004E2D8_4EED8);
            break;
    }

    arg0->unkA = (randA() & 0xFF) % 5 + 12;
    setCleanupCallback(func_8004E614_4F214);
}

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004E2D8_4EED8);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004E410_4F010);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004E4EC_4F0EC);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004E580_4F180);

void func_8004E614_4F214(Struct_func_8004E134 *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

typedef struct {
    s16 unk0;
    s16 unk2;
} func_8004E6A4_task;

typedef struct {
    u8 pad0[0xC];
    s16 unkC;
    s16 unkE;
} func_8004E640_task;

extern u16 D_8009ADE0_9B9E0;

void func_8004E640_4F240(func_8004E6A4_task *arg0) {
    func_8004E640_task *task;

    if (D_8009ADE0_9B9E0 & 1) {
        task = scheduleTask(func_8004E134_4ED34, 2, 0, 0xE6);
        if (task != NULL) {
            task->unkC = arg0->unk0;
            task->unkE = arg0->unk2;
        }
    }
}

void func_8004E6A4_4F2A4(s16 arg0, s16 arg1) {
    func_8004E6A4_task *task;

    task = scheduleTask(&func_8004E640_4F240, 1, 0, 0xE5);
    if (task != NULL) {
        task->unk0 = arg0;
        task->unk2 = arg1;
    }
}

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
    u8 padA[0x2];
} Element_8004E6F8; // size 0xC

typedef struct {
    Element_8004E6F8 elements[3]; // offset 0x0, size 0x24
    void *unk24;                  // offset 0x24
} Struct_func_8004E6F8;

typedef struct {
    u8 pad0[0x4];
    void *unk4;
    u8 pad8[0x1C];
    void *unk24;
} Struct_func_8004E884;

void func_8004E794_4F394(Struct_func_8004E6F8 *);
void func_8004E884_4F484(Struct_func_8004E884 *);

void func_8004E6F8_4F2F8(Struct_func_8004E6F8 *arg0) {
    s32 i;
    s16 unk0val;
    s32 x;

    getCurrentAllocation();
    arg0->unk24 = loadAsset_34F7E0();
    arg0->elements[0].unk4 = loadAsset_34CB50();

    i = 0;
    unk0val = -0x1C;
    x = -0xC;
    do {
        arg0->elements[i].unk0 = unk0val;
        arg0->elements[i].unk2 = x;
        arg0->elements[i].unk4 = arg0->elements[0].unk4;
        x += 8;
        i++;
    } while (i < 3);

    arg0->elements[0].unk8 = 0x1B;
    arg0->elements[1].unk8 = 0x1C;
    arg0->elements[2].unk8 = 0x1D;

    setCleanupCallback(func_8004E884_4F484);
    setCallback(func_8004E794_4F394);
}

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004E794_4F394);

void func_8004E884_4F484(Struct_func_8004E884 *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
    arg0->unk24 = freeNodeMemory(arg0->unk24);
}

typedef struct {
    u8 pad0[0x10];
    void *unk10;
} Struct_func_8004E8BC_alloc;

typedef struct {
    u8 pad0[0x17C3];
    u8 unk17C3;
} Struct_func_8004E8BC_unk10_target;

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
    s16 unkA;
} Struct_func_8004E8BC_element;

typedef struct {
    Struct_func_8004E8BC_element elements[10];
    s16 unk78;
    s16 unk7A;
    s16 unk7C;
} Struct_func_8004E8BC;

void func_8004E940_4F540(void *);
void func_8004EA28_4F628(Struct_func_8004F04C *);

void func_8004E8BC_4F4BC(Struct_func_8004E8BC *arg0) {
    Struct_func_8004E8BC_alloc *allocation = (Struct_func_8004E8BC_alloc *)getCurrentAllocation();
    s32 i;
    u8 val;
    void *p;

    val = ((Struct_func_8004E8BC_unk10_target *)allocation->unk10)->unk17C3;
    arg0->unk7C = 0;
    arg0->unk7A = val;
    arg0->elements[0].unk4 = loadAssetByIndex_95470(arg0->unk78);

    for (i = 0; i < 10; i++) {
        p = arg0->elements[0].unk4;
        arg0->elements[i].unk8 = 0;
        arg0->elements[i].unk2 = 0x58;
        arg0->elements[i].unk4 = p;
    }

    setCleanupCallback(func_8004EA28_4F628);
    setCallback(func_8004E940_4F540);
}

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004E940_4F540);

void func_8004EA28_4F628(Struct_func_8004F04C *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
    u8 padA[0x2];
    s16 unkC;
    s16 unkE;
    void *unk10;
    s16 unk14;
    u8 pad16[0x2];
    void *unk18;
    Player *unk1C;
} Struct_func_8004EB90;

void func_8004EAF0_4F6F0(Struct_func_8004EB90 *arg0);
void func_8004EB90_4F790(Struct_func_8004EB90 *arg0);

void func_8004EA54_4F654(Struct_func_8004EB90 *arg0) {
    getCurrentAllocation();
    arg0->unk4 = loadAsset_34F9A0();
    arg0->unk8 = 2;
    arg0->unk0 = -0x88;
    arg0->unk2 = -0x60;
    arg0->unk14 = 3;
    arg0->unkC = -0x84;
    arg0->unkE = -0x54;
    arg0->unk10 = arg0->unk4;
    arg0->unk18 = dmaRequestAndUpdateStateWithSize(&_3F6950_ROM_START, &_3F6950_ROM_END, 0x508);
    setCleanupCallback(func_8004EB90_4F790);
    setCallback(func_8004EAF0_4F6F0);
}

void func_8004EAF0_4F6F0(Struct_func_8004EB90 *arg0) {
    char buf[16];

    sprintf(buf, D_8009E8A8_9F4A8, arg0->unk1C->unkBD3);
    func_8003BD60_3C960(buf, -0x70, -0x54, 0xFF, arg0->unk18, arg0->unk1C->unkBB8 + 8, 0);
    debugEnqueueCallback(8, 0, func_8000FED0_10AD0, arg0);
    debugEnqueueCallback(8, 0, func_8000FED0_10AD0, &arg0->unkC);
}

void func_8004EB90_4F790(Struct_func_8004EB90 *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
    arg0->unk18 = freeNodeMemory(arg0->unk18);
}

void func_8004EBC8_4F7C8(void *arg0) {
    Struct_func_8004EB90 *task;

    task = scheduleTask(func_8004EA54_4F654, 0, 1, 0xE6);
    if (task != NULL) {
        task->unk1C = arg0;
    }
}

void func_8004ED94_4F994(Struct_func_8004FF28 *arg0);
void func_8004ECA4_4F8A4(Struct_func_8004FF28 *arg0);

void func_8004EC08_4F808(Struct_func_8004FF28 *arg0) {
    GameState *allocation = (GameState *)getCurrentAllocation();

    arg0->unk12 = allocation->unk5A;
    arg0->unk8 = loadAsset_34F9A0();
    arg0->unkC = 1;
    if (arg0->unk14 == 0) {
        arg0->unk4 = 0x68;
        arg0->unk6 = -0x60;
    } else {
        arg0->unk4 = -0x48;
        arg0->unk6 = -0x38;
    }
    arg0->unk10 = 0;
    arg0->unk0 = dmaRequestAndUpdateStateWithSize(&_3F6950_ROM_START, &_3F6950_ROM_END, 0x508);
    setCleanupCallback(func_8004ED94_4F994);
    setCallback(func_8004ECA4_4F8A4);
}

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004ECA4_4F8A4);

void func_8004ED94_4F994(Struct_func_8004FF28 *arg0) {
    arg0->unk8 = freeNodeMemory(arg0->unk8);
    arg0->unk0 = freeNodeMemory(arg0->unk0);
}

void func_8004EDCC_4F9CC(s16 arg0) {
    Struct_func_8004FF28 *task;

    if (arg0 == 0) {
        task = scheduleTask(func_8004EC08_4F808, 0, 1, 0xE6);
    } else {
        task = scheduleTask(func_8004EC08_4F808, 1, 1, 0xE6);
    }
    if (task != NULL) {
        task->unk14 = arg0;
    }
}

void func_8004EEB4_4FAB4(void *);
void func_8004F04C_4FC4C(Struct_func_8004F04C *);

void func_8004EE24_4FA24(Struct_func_8004F04C *arg0) {
    GameState *allocation = (GameState *)getCurrentAllocation();

    if (allocation->unk7A == 6) {
        arg0->unk10 = 0xA8C;
    } else {
        arg0->unk10 = 0x1194;
    }
    arg0->unkC = dmaRequestAndUpdateStateWithSize(&_3F6950_ROM_START, &_3F6950_ROM_END, 0x508);
    arg0->unk4 = loadAsset_34CB50();
    arg0->unk8 = 0x23;
    arg0->unk0 = 0x68;
    arg0->unk2 = 0x48;
    setCleanupCallback(func_8004F04C_4FC4C);
    setCallback(func_8004EEB4_4FAB4);
}

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004EEB4_4FAB4);

void func_8004F04C_4FC4C(Struct_func_8004F04C *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
    arg0->unkC = freeNodeMemory(arg0->unkC);
}

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
    u8 padA[0x2];
    s16 unkC;
} Struct_func_8004F084;

void func_8004F104_4FD04(Struct_func_8004F084 *);
void func_8004F168_4FD68(Struct_func_8004F04C *);

void func_8004F084_4FC84(Struct_func_8004F084 *arg0) {
    if (arg0->unkC == 0) {
        arg0->unk4 = loadAsset_34CB50();
        arg0->unk8 = 0x1E;
        arg0->unk0 = -0x48;
        arg0->unk2 = -0x10;
        arg0->unkC = 1;
        setCleanupCallback(func_8004F168_4FD68);
        setCallback(func_8004F104_4FD04);
    } else {
        arg0->unkC = arg0->unkC - 1;
    }
}

void func_8004F104_4FD04(Struct_func_8004F084 *arg0) {
    if (arg0->unkC == 0) {
        arg0->unkC = 1;
        if ((u16)arg0->unk8 != 0x22) {
            arg0->unk8 = arg0->unk8 + 1;
        }
    } else {
        arg0->unkC = arg0->unkC - 1;
    }
    debugEnqueueCallback(8, 0, func_8000FED0_10AD0, arg0);
}

void func_8004F168_4FD68(Struct_func_8004F04C *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

typedef struct {
    u8 _pad[0xC];
    s16 unkC;
} func_8004F194_task;

void func_8004F194_4FD94(s16 arg0) {
    func_8004F194_task *task = (func_8004F194_task *)scheduleTask(&func_8004F084_4FC84, 1, 1, 0xE6);
    if (task != NULL) {
        task->unkC = arg0;
    }
}

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
    s16 unkA;
    u8 unkC;
    u8 unkD;
    u8 unkE;
    u8 padF;
    void *unk10;
    s32 unk14;
} Struct_func_8004F1D4;

void func_8004F27C_4FE7C(Struct_func_8004F1D4 *);
void func_8004F3EC_4FFEC(Struct_func_8004DCC4 *);

void func_8004F1D4_4FDD4(Struct_func_8004F1D4 *arg0) {
    GameState *allocation = (GameState *)getCurrentAllocation();

    arg0->unk14 = 0;
    arg0->unk4 = loadAsset_34CB50();
    func_8004D9B8_4E5B8((Struct_func_8004D9B8 *)arg0);
    arg0->unk8 = 0x24;
    arg0->unk10 = dmaRequestAndUpdateStateWithSize(&_3F6950_ROM_START, &_3F6950_ROM_END, 0x508);
    if (allocation->unk7A == 4) {
        arg0->unk0 = 0xC;
        arg0->unk2 = -0x3C;
    } else {
        arg0->unk0 = 0xC;
        arg0->unk2 = -0x20;
    }
    setCleanupCallback(func_8004F3EC_4FFEC);
    setCallback(func_8004F27C_4FE7C);
}

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004F27C_4FE7C);

void func_8004F3EC_4FFEC(Struct_func_8004DCC4 *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
    arg0->unk10 = freeNodeMemory(arg0->unk10);
}

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
    u8 padA[0x2];
    void *unkC;
    s32 unk10;
    s16 unk14;
} Struct_func_8004F424;

void func_8004F4A8_500A8(void);
void func_8004F69C_5029C(Struct_func_8004F04C *arg0);

void func_8004F424_50024(Struct_func_8004F424 *arg0) {
    arg0->unk10 = 0x4293C;
    arg0->unkC = dmaRequestAndUpdateStateWithSize(&_3F6950_ROM_START, &_3F6950_ROM_END, 0x508);
    arg0->unk4 = loadAsset_34CB50();
    arg0->unk8 = 0x23;
    arg0->unk0 = 0x68;
    arg0->unk2 = 0x48;
    arg0->unk14 = 0;
    setCleanupCallback(func_8004F69C_5029C);
    setCallback(func_8004F4A8_500A8);
}

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004F4A8_500A8);

void func_8004F69C_5029C(Struct_func_8004F04C *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
    arg0->unkC = freeNodeMemory(arg0->unkC);
}

void func_8004F760_50360(void);
void func_8004F7F4_503F4(Struct_func_8004F04C *arg0);

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    u8 pad8[0x4];
    void *unkC;
    s32 unk10;
} Struct_func_8004F6D4;

typedef struct {
    u8 pad0[0x10];
    void *unk10;
} AllocationStruct_8004F6D4;

void func_8004F6D4_502D4(Struct_func_8004F6D4 *arg0) {
    AllocationStruct_8004F6D4 *alloc = getCurrentAllocation();
    s32 index = arg0->unk10;
    void *base = alloc->unk10;

    arg0->unk0 = -0x10;
    arg0->unk2 = -0x60;
    arg0->unkC = (void *)((u8 *)base + index * 3048);
    arg0->unk4 = dmaRequestAndUpdateStateWithSize(&_3F3EF0_ROM_START, &_3F3EF0_ROM_END, 0x2608);
    setCallbackWithContinue(func_8004F760_50360);
    setCleanupCallback(func_8004F7F4_503F4);
}

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004F760_50360);

void func_8004F7F4_503F4(Struct_func_8004F04C *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_8004F898_50498(Struct_func_8004F04C *);
void func_8004F9E8_505E8(Struct_func_8004F04C *);

void func_8004F820_50420(Struct_func_8004F04C *arg0) {
    arg0->unkC = dmaRequestAndUpdateStateWithSize(&_3F6950_ROM_START, &_3F6950_ROM_END, 0x508);
    arg0->unk4 = loadAsset_34CB50();
    arg0->unk8 = 0x23;
    arg0->unk0 = -0x4C;
    arg0->unk2 = -0x30;
    arg0->unk10 = 0;
    setCleanupCallback(func_8004F9E8_505E8);
    setCallback(func_8004F898_50498);
}

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004F898_50498);

void func_8004F9E8_505E8(Struct_func_8004F04C *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
    arg0->unkC = freeNodeMemory(arg0->unkC);
}

extern char D_8009E924_9F524[];

typedef struct {
    s16 unk0;
    s16 unk2;
    void *unk4;
    s16 unk8;
    u8 padA[0x2];
    void *unkC;
    char unk10[8];
    s32 unk18;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
} Struct_func_8004FA20;

void func_8004FCB8_508B8(Struct_func_8004F04C *arg0);
void func_8004FB64_50764(Struct_func_8004FA20 *arg0);
void func_8004FAB4_506B4(void *);
void func_8004FC34_50834(Struct_func_8004FA20 *arg0);

void func_8004FA20_50620(Struct_func_8004FA20 *arg0) {
    getCurrentAllocation();
    arg0->unk4 = loadAsset_3505F0();
    arg0->unk2 = -0x20;
    arg0->unk8 = 2;
    arg0->unkC = dmaRequestAndUpdateStateWithSize(&_3F6950_ROM_START, &_3F6950_ROM_END, 0x508);
    sprintf(arg0->unk10, D_8009E924_9F524, arg0->unk18);
    arg0->unk20 = 0;
    arg0->unk1E = 0x1E;
    setCleanupCallback(func_8004FCB8_508B8);
    setCallback(func_8004FB64_50764);
}

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004FAB4_506B4);

void func_8004FBE8_507E8(Struct_func_8004FA20 *arg0);

void func_8004FB64_50764(Struct_func_8004FA20 *arg0) {
    s32 sinVal;
    s16 angle;

    angle = arg0->unk20 + 0x80;
    arg0->unk20 = angle;
    sinVal = approximateSin(angle);
    arg0->unk1C = (0x2000 - sinVal) / 20;
    if (arg0->unk20 == 0x800) {
        setCallback(func_8004FBE8_507E8);
    }
    func_8004FAB4_506B4(arg0);
}

void func_8004FBE8_507E8(Struct_func_8004FA20 *arg0) {
    arg0->unk1E--;
    if (arg0->unk1E == 0) {
        setCallback(func_8004FC34_50834);
    }
    func_8004FAB4_506B4(arg0);
}

void func_8004FC34_50834(Struct_func_8004FA20 *arg0) {
    s32 sinVal;
    s16 angle;

    angle = arg0->unk20 + 0x80;
    arg0->unk20 = angle;
    sinVal = approximateSin(angle);
    arg0->unk1C = -((0x2000 - sinVal) / 20);
    if (arg0->unk20 == 0x1000) {
        func_80069CF8_6A8F8();
    }
    func_8004FAB4_506B4(arg0);
}

void func_8004FCB8_508B8(Struct_func_8004F04C *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
    arg0->unkC = freeNodeMemory(arg0->unkC);
}

void func_8004FCF0_508F0(s32 arg0) {
    Struct_func_8004FA20 *task = (Struct_func_8004FA20 *)scheduleTask(func_8004FA20_50620, 0, 0, 0xE6);
    if (task != NULL) {
        task->unk18 = arg0;
    }
}

void func_8004FDD0_509D0(void);
void func_8004FF28_50B28(Struct_func_8004FF28 *);

void func_8004FD30_50930(Struct_func_8004FF28 *arg0) {
    GameState *allocation = (GameState *)getCurrentAllocation();

    arg0->unk12 = allocation->players->unkB70;
    arg0->unk8 = loadAsset_3505F0();
    arg0->unkC = 3;
    if (arg0->unk14 == 0) {
        arg0->unk4 = -0x10;
        arg0->unk6 = -0x60;
    } else {
        arg0->unk4 = -0x48;
        arg0->unk6 = -0x30;
    }
    arg0->unk10 = 0;
    arg0->unk0 = dmaRequestAndUpdateStateWithSize(&_3F6950_ROM_START, &_3F6950_ROM_END, 0x508);
    setCleanupCallback(func_8004FF28_50B28);
    setCallback(func_8004FDD0_509D0);
}

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8004FDD0_509D0);

void func_8004FF28_50B28(Struct_func_8004FF28 *arg0) {
    arg0->unk8 = freeNodeMemory(arg0->unk8);
    arg0->unk0 = freeNodeMemory(arg0->unk0);
}

void func_8004FF60_50B60(s16 arg0) {
    Struct_func_8004FF28 *task;

    if (arg0 == 0) {
        task = scheduleTask(func_8004FD30_50930, 0, 1, 0xE6);
    } else {
        task = scheduleTask(func_8004FD30_50930, 1, 1, 0xE6);
    }
    if (task != NULL) {
        task->unk14 = arg0;
    }
}

void func_80050098_50C98(Struct_func_8004FFB8 *);
void func_800500F0_50CF0(Struct_func_8004F04C *);

void func_8004FFB8_50BB8(Struct_func_8004FFB8 *arg0) {
    GameState *allocation = (GameState *)getCurrentAllocation();

    switch (allocation->unk7A) {
        case 4:
            arg0->unk4 = loadAsset_350140();
            arg0->unk8 = 0;
            arg0->unk14 = 1;
            break;
        case 5:
            arg0->unk4 = loadAsset_34F9A0();
            arg0->unk8 = 4;
            arg0->unk14 = 5;
            break;
        case 6:
            arg0->unk4 = loadAsset_3505F0();
            arg0->unk8 = 0;
            arg0->unk14 = 1;
            break;
    }
    arg0->unk10 = arg0->unk4;
    arg0->unk0 = -0x88;
    arg0->unk2 = 0x40;
    arg0->unkC = -0x88;
    arg0->unkE = 0x40;
    setCallbackWithContinue(func_80050098_50C98);
    setCleanupCallback(func_800500F0_50CF0);
}

void func_80050098_50C98(Struct_func_8004FFB8 *arg0) {
    debugEnqueueCallback(8, 0, func_8000FED0_10AD0, &arg0->unkC);
    debugEnqueueCallback(8, 0, func_8000FED0_10AD0, arg0);
}

void func_800500F0_50CF0(Struct_func_8004F04C *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

INCLUDE_RODATA("asm/nonmatchings/4CD70", D_8009E894_9F494);

INCLUDE_RODATA("asm/nonmatchings/4CD70", D_8009E89C_9F49C);

INCLUDE_RODATA("asm/nonmatchings/4CD70", D_8009E8A0_9F4A0);

INCLUDE_RODATA("asm/nonmatchings/4CD70", D_8009E8A8_9F4A8);

INCLUDE_RODATA("asm/nonmatchings/4CD70", D_8009E8AC_9F4AC);

INCLUDE_RODATA("asm/nonmatchings/4CD70", D_8009E8C0_9F4C0);

INCLUDE_RODATA("asm/nonmatchings/4CD70", D_8009E8D4_9F4D4);

INCLUDE_RODATA("asm/nonmatchings/4CD70", D_8009E8E0_9F4E0);

INCLUDE_RODATA("asm/nonmatchings/4CD70", D_8009E8EC_9F4EC);

INCLUDE_RODATA("asm/nonmatchings/4CD70", D_8009E8F8_9F4F8);

INCLUDE_RODATA("asm/nonmatchings/4CD70", D_8009E904_9F504);

INCLUDE_RODATA("asm/nonmatchings/4CD70", D_8009E914_9F514);

INCLUDE_RODATA("asm/nonmatchings/4CD70", D_8009E924_9F524);

INCLUDE_RODATA("asm/nonmatchings/4CD70", D_8009E928_9F528);

INCLUDE_ASM("asm/nonmatchings/4CD70", func_8005011C_50D1C);
