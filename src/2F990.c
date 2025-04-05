#include "common.h"
#include "gamestate.h"

void func_8006BEDC_6CADC(void *, s32, s32, s32, s32, s32, s32);
void func_8006FD3C_7093C(u16, void *);
void func_8006FF90_70B90(s32, s32, void *, void *);
extern s32 D_800AB054_A23C4;
extern s32 D_800AFF14_A7284;
extern void func_80068128_68D28();
typedef struct
{
    u8 padding[0x8];
    char *cameraYString;
    u8 padding2[0x8];
    char *cameraYRotationString;
    s32 cameraY;
    s16 cameraRotation;
} cameraState;

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

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002EFD8_2FBD8);

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002F024_2FC24);

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002F110_2FD10);

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002F12C_2FD2C);

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002F290_2FE90);

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002F36C_2FF6C);

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002F3E4_2FFE4);

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002F518_30118);

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002F5C8_301C8);

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002F614_30214);

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002F658_30258);

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002F72C_3032C);

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002F860_30460);

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002F88C_3048C);

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002F948_30548);

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002F980_30580);

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002F9C4_305C4);

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002FA1C_3061C);

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002FA44_30644);

INCLUDE_ASM("asm/nonmatchings/2F990", func_8002FA70_30670);

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

INCLUDE_ASM("asm/nonmatchings/2F990", func_80030974_31574);

INCLUDE_ASM("asm/nonmatchings/2F990", func_800309D4_315D4);

INCLUDE_ASM("asm/nonmatchings/2F990", func_80030A00_31600);

INCLUDE_ASM("asm/nonmatchings/2F990", func_80030AEC_316EC);

INCLUDE_ASM("asm/nonmatchings/2F990", func_80030B70_31770);

INCLUDE_ASM("asm/nonmatchings/2F990", func_80030BA8_317A8);

INCLUDE_RODATA("asm/nonmatchings/2F990", D_8009E47C_9F07C);

INCLUDE_RODATA("asm/nonmatchings/2F990", D_8009E480_9F080);

INCLUDE_RODATA("asm/nonmatchings/2F990", D_8009E488_9F088);

INCLUDE_RODATA("asm/nonmatchings/2F990", D_8009E48C_9F08C);
