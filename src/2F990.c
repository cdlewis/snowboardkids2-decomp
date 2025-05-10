#include "615A0.h"
#include "69EF0.h"
#include "6B140.h"
#include "6E840.h"
#include "common.h"
#include "gamestate.h"

void func_8006BEDC_6CADC(void *, s32, s32, s32, s32, s32, s32);
void func_8006FD3C_7093C(u16, void *);
void func_8006FF90_70B90(s32, s32, void *, void *);
extern s32 D_800AB054_A23C4;
extern s32 D_800AFF14_A7284;
extern void func_80068128_68D28();
extern void func_8002FA44_30644();
extern void func_8002F290_2FE90();
void func_8000153C_213C(s32, void *);
void func_800021B8_2DB8(s32, s16);
void func_80002750_3350(s32);
extern void func_8002F024_2FC24();
void func_80069CE8_6A8E8(void *);
void func_8006A838_6B438(void *, s32, s32);
void func_8006B084_6BC84(void *, void *, void *);
extern s32 D_8009A870_9B470;
extern void func_8002F72C_3032C();
extern void func_8002F980_30580();
extern void func_8000FED0_10AD0();

typedef struct {
    u8 padding[0x2C];
    s32 unk2C;
} func_8002FA70_30670_arg;

typedef struct {
    u8 padding[0x20];
    s32 unk20;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    u8 padding2[0x30];
    s8 unk60;
} func_8002F658_30258_arg;

typedef struct {
    s32 unk0;
    s32 unk4;
    u8 padding[0x1B];
    s16 unk24;
} func_8002EFD8_2FBD8_arg;

typedef struct {
    u8 padding[0x2C];
    s32 unk2C;
} func_8002FA1C_3061C_arg;
typedef struct
{
    u8 padding[0x8];
    char *cameraYString;
    u8 padding2[0x8];
    char *cameraYRotationString;
    s32 cameraY;
    s16 cameraRotation;
} cameraState;

typedef struct {
    u8 padding[0x3C];
    s32 unk3C;
    u8 padding2[0x10];
    u32 unk50;
    u8 padding3[0xC];
    u8 unk60;
} S0;

void func_80069CC0_6A8C0(void (*func)(func_80063824_64424_arg *));
void func_8002F5C8_301C8(func_80063824_64424_arg *arg0);
void func_800394BC_3A0BC(func_8002FA1C_3061C_arg *, s32);

// 100% match but something weird is happening
// #include "initDebugCameraController.c"
INCLUDE_ASM("asm/nonmatchings/2F990", initDebugCameraController);

void updateDebugCameraYState(cameraState *arg0) {
    s32 sp20[8];
    u64 pad;
    char *cameraYRotation;
    char *cameraYString;
    GameState *temp_s0 = GameStateGet();
    if (D_800AB054_A23C4 & 0x10) {
        arg0->cameraRotation++;
    } else if (D_800AB054_A23C4 & 0x20) {
        arg0->cameraRotation--;
    }
    do {
        arg0->cameraRotation = arg0->cameraRotation & 0x1FFF;
    } while (0);
    if (D_800AFF14_A7284 & 0x800) {
        arg0->cameraY--;
    } else if (D_800AFF14_A7284 & 0x400) {
        arg0->cameraY++;
    }
    func_8006BEDC_6CADC(&sp20, 0, arg0->cameraY << 0x10, 0x600000, 0, (s32)((s16)arg0->cameraRotation), 0);
    func_8006FD3C_7093C(temp_s0->unkDA, &sp20);

    cameraYString = ((char *)((s32)arg0)) + 0x3E;
    sprintf(cameraYString, "CAMERA Y = %d", arg0->cameraY);
    cameraYRotation = ((char *)((s32)arg0)) + 0x1E;
    sprintf(cameraYRotation, "CAMERA RX = %X", (s16)arg0->cameraRotation);
    arg0->cameraYString = cameraYString;
    arg0->cameraYRotationString = cameraYRotation;

    debugEnqueueCallback(8, 7, &func_80068128_68D28, arg0);
    debugEnqueueCallback(8, 7, &func_80068128_68D28, ((void *)((s32)arg0)) + 0xC);
}

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002EF3C_2FB3C);

void func_8002EFD8_2FBD8(func_8002EFD8_2FBD8_arg *arg0) {
    func_8000153C_213C(arg0->unk0, (void *)((s32)arg0 + 4));
    func_800021B8_2DB8(arg0->unk0, arg0->unk24);
    func_80002750_3350(arg0->unk0);
    func_80069CC0_6A8C0(&func_8002F024_2FC24);
}

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002F024_2FC24);

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002F110_2FD10);

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002F12C_2FD2C);

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002F290_2FE90);

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002F36C_2FF6C);

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002F3E4_2FFE4);

void func_8002F518_30118(func_80063824_64424_arg *s0) {
    volatile u8 padding[0x20];
    u32 new_var;
    GameState *s1 = GameStateGet();
    u32 offset;
    new_var = s1->unk5C6;

    if (new_var == 2) {
        offset = 0xFFF80000;
    } else {
        offset = 0x00080000;
    }

    s0->unk50 += offset;

    memcpy(s0, (void *)((s32)s0 + 0x3C), 0x20);

    s0->unk60++;
    if (s0->unk60 == 4) {
        s0->unk60 = 0;
        s1->unk5C7++;
        func_80069CC0_6A8C0(func_8002F5C8_301C8);
    }

    func_800635CC_641CC(0, s0);
}

void func_8002F5C8_301C8(func_80063824_64424_arg *arg0) {
    volatile u8 pad[0x20];
    if (GameStateGet()->unk5C5 == 1) {
        func_80069CC0_6A8C0(&func_8002F290_2FE90);
    }
    func_800635CC_641CC(0, arg0);
}

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002F614_30214);

void func_8002F658_30258(func_8002F658_30258_arg *arg0) {
    u8 sp10[0x20];
    u8 sp30[0x20];
    u8(*new_var)[0x20];
    void *temp_s3;
    GameStateGet();
    new_var = &sp10;
    temp_s3 = ((void *)((s32)arg0)) + 0x3C;
    memcpy(temp_s3, &D_8009A870_9B470, 0x20);
    memcpy(&sp30, temp_s3, 0x20);
    memcpy(new_var, &sp30, 0x20);
    func_8006A838_6B438(&sp10, 0x1000, 0x800);
    createZRotationMatrix((s16(*)[3])(&sp30), 0x1F00);
    func_8006B084_6BC84(&sp10, &sp30, temp_s3);
    arg0->unk60 = 0;
    arg0->unk20 = 0;
    arg0->unk24 = 0;
    arg0->unk28 = 0;
    arg0->unk2C = 0;
    func_80069CE8_6A8E8(&func_8002F980_30580);
    func_80069CC0_6A8C0(&func_8002F72C_3032C);
}

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002F72C_3032C);

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002F860_30460);

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002F88C_3048C);

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002F948_30548);

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002F980_30580);

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002F9C4_305C4);

void func_8002FA1C_3061C(func_8002FA1C_3061C_arg *arg0) {
    func_800394BC_3A0BC(arg0, arg0->unk2C);
    func_80069CC0_6A8C0(&func_8002FA44_30644);
}

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002FA44_30644);

void func_8002FA70_30670(func_8002FA70_30670_arg *arg0) {
    arg0->unk2C = freeGameStateMemory(arg0->unk2C);
}

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002FA9C_3069C);

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002FB40_30740);

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002FCA8_308A8);

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002FCD4_308D4);

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002FDFC_309FC);

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002FF28_30B28);

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002FF54_30B54);

INCLUDE_ASM("asm/nonmatchings/2F990", func_8003006C_30C6C);

INCLUDE_ASM("asm/nonmatchings/2F990", func_80030194_30D94);

INCLUDE_ASM("asm/nonmatchings/2F990", func_800301C0_30DC0);

INCLUDE_ASM("asm/nonmatchings/2F990", func_80030238_30E38);

INCLUDE_ASM("asm/nonmatchings/2F990", func_80030280_30E80);

INCLUDE_ASM("asm/nonmatchings/2F990", func_800302AC_30EAC);

INCLUDE_ASM("asm/nonmatchings/2F990", func_80030378_30F78);

INCLUDE_ASM("asm/nonmatchings/2F990", func_80030480_31080);

INCLUDE_ASM("asm/nonmatchings/2F990", func_800304B8_310B8);

INCLUDE_ASM("asm/nonmatchings/2F990", func_80030540_31140);

INCLUDE_ASM("asm/nonmatchings/2F990", func_80030668_31268);

INCLUDE_ASM("asm/nonmatchings/2F990", func_80030694_31294);

INCLUDE_ASM("asm/nonmatchings/2F990", func_80030764_31364);

INCLUDE_ASM("asm/nonmatchings/2F990", func_800308C4_314C4);

INCLUDE_ASM("asm/nonmatchings/2F990", func_800308FC_314FC);

void func_80030974_31574(void *arg0) {
    GameState *state = GameStateGet();
    s8 index = state->unk5C8;

    if (state->unk5CA[index] >= 0x80 || state->unk5C9 == 0) {
        debugEnqueueCallback(8, 1, func_8000FED0_10AD0, arg0);
    }
}

INCLUDE_ASM("asm/nonmatchings/2F990", func_800309D4_315D4);

INCLUDE_ASM("asm/nonmatchings/2F990", func_80030A00_31600);

INCLUDE_ASM("asm/nonmatchings/2F990", func_80030AEC_316EC);

INCLUDE_ASM("asm/nonmatchings/2F990", func_80030B70_31770);

INCLUDE_ASM("asm/nonmatchings/2F990", func_80030BA8_317A8);

INCLUDE_RODATA("asm/nonmatchings/2F990", D_8009E47C_9F07C);

INCLUDE_RODATA("asm/nonmatchings/2F990", D_8009E480_9F080);

INCLUDE_RODATA("asm/nonmatchings/2F990", D_8009E488_9F088);

INCLUDE_RODATA("asm/nonmatchings/2F990", D_8009E48C_9F08C);
