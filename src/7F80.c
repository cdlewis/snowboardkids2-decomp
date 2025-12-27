#include "19E80.h"
#include "20F0.h"
#include "6E840.h"
#include "common.h"
#include "geometry.h"
#include "overlay.h"
#include "task_scheduler.h"

USE_ASSET(_49B500);
USE_ASSET(_646CD0);

typedef struct {
    s32 unk0;
    u8 padding[0x17C];
} func_800078C4_84C4_arg_unk44;

typedef struct {
    SceneModel *unk0;
    void *unk4;
    OutputStruct_19E80 unk8;
    Mat3x3Padded unk14;
    s32 unk34;
    s32 *unk38;
    s32 unk3C;
    s32 unk40;
    func_800078C4_84C4_arg_unk44 *unk44;
} func_800078C4_84C4_arg;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    /* 0x10 */ u16 width;
    /* 0x12 */ u16 height;
    s32 unk14;
    u8 padding[0x1C];
    void *unk34;
    s32 unk38;
} func_80007560_8160_arg;

void func_80007958_8558(func_800078C4_84C4_arg *);
void func_8000799C_859C(func_800078C4_84C4_arg *);
void func_80007ABC_86BC(func_800078C4_84C4_arg *);
void func_800073E0_7FE0(void);
void func_80007558_8158(void);
void func_80007560_8160(func_80007560_8160_arg *);

extern u8 identityMatrix[];
void *func_8006C130_6CD30(void *, LookAt *);
extern s32 D_8008C200_8CE00[];
extern s16 gGraphicsMode;
extern Gfx *gRegionAllocPtr;

typedef struct {
    s32 unk0;
    s16 rotationX;
    s16 rotationY;
    s16 rotationZ;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 angularVelocityX;
    s16 angularVelocityY;
    s16 angularVelocityZ;
    s16 unk16;
} CameraRotationTaskState;

void initCameraRotationTask(CameraRotationTaskState *arg0) {
    setCleanupCallback(&func_80007558_8158);
    arg0->rotationZ = 0;
    arg0->rotationY = 0;
    arg0->rotationX = 0;
    arg0->unkE = 0;
    arg0->unkC = 0;
    arg0->unkA = 0;
    arg0->angularVelocityZ = 0;
    arg0->angularVelocityY = 0;
    arg0->angularVelocityX = 0;
    setCallback(&func_800073E0_7FE0);
}

INCLUDE_ASM("asm/nonmatchings/7F80", func_800073E0_7FE0);

void func_80007558_8158(void) {
}

void func_80007560_8160(func_80007560_8160_arg *arg0) {
    s32 var_a2;
    s32 new_var;
    u32 line;
    s32 lrs;
    u16 dxt;
    u16 width_div_16;
    Gfx *loadblock_ptr;
    long loadblock_value;
    volatile u8 padding[0x10];

    gDPPipeSync(gRegionAllocPtr++);

    gDPSetTextureLUT(gRegionAllocPtr++, G_TT_RGBA16);

    gDPSetTextureImage(gRegionAllocPtr++, G_IM_FMT_CI, G_IM_SIZ_16b, 1, arg0->unk8);

    gDPSetTile(
        gRegionAllocPtr++,
        G_IM_FMT_CI,
        G_IM_SIZ_16b,
        0,
        0x0000,
        G_TX_LOADTILE,
        0,
        G_TX_NOMIRROR | G_TX_WRAP,
        G_TX_NOMASK,
        G_TX_NOLOD,
        G_TX_NOMIRROR | G_TX_WRAP,
        G_TX_NOMASK,
        G_TX_NOLOD
    );

    gDPLoadSync(gRegionAllocPtr++);

    loadblock_ptr = gRegionAllocPtr++;
    loadblock_ptr->words.w0 = 0xF3000000;
    gGraphicsMode = -1;
    width_div_16 = arg0->width >> 4;
    var_a2 = 0x800;
    if (width_div_16 != 0) {
        var_a2 = width_div_16 + 0x7FF;
    }
    lrs = (((s32)((arg0->width * arg0->height) + 3)) >> 2) - 1;
    if (lrs < 0x800) {
    } else {
        lrs = 0x7FF;
    }
    line = lrs & 0xFFF;
    new_var = (line << 12) | 0x07000000;
    loadblock_value = new_var;
    if (width_div_16 != 0) {
        loadblock_value |= (var_a2 / width_div_16) & 0xFFF;
    } else {
        loadblock_value |= var_a2 & 0xFFF;
    }
    loadblock_ptr->words.w1 = loadblock_value;

    gDPPipeSync(gRegionAllocPtr++);

    line = (((arg0->width >> 1) + 7) >> 3) & 0x1FF;
    new_var = G_TX_NOMIRROR;
    gDPSetTile(gRegionAllocPtr++, G_IM_FMT_CI, G_IM_SIZ_4b, line, 0, G_TX_RENDERTILE, 0, 0, 0, 0, 0, 0, 0);

    gDPSetTileSize(gRegionAllocPtr++, G_TX_RENDERTILE, 0, 0, (arg0->width - 1) << 2, (arg0->height - 1) << 2);

    gDPSetTextureImage(gRegionAllocPtr++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, arg0->unkC);

    gDPTileSync(gRegionAllocPtr++);

    gDPSetTile(
        gRegionAllocPtr++,
        G_IM_FMT_RGBA,
        G_IM_SIZ_4b,
        0,
        0x0100,
        G_TX_LOADTILE,
        0,
        G_TX_NOMIRROR | G_TX_WRAP,
        G_TX_NOMASK,
        G_TX_NOLOD,
        G_TX_NOMIRROR | G_TX_WRAP,
        G_TX_NOMASK,
        G_TX_NOLOD
    );

    gDPLoadSync(gRegionAllocPtr++);

    gDPLoadTLUTCmd(gRegionAllocPtr++, G_TX_LOADTILE, 15);

    gDPPipeSync(gRegionAllocPtr++);

    if (arg0->unk34 == 0) {
        arg0->unk34 = arenaAlloc16(0x40);
        if (arg0->unk34 == 0) {
            return;
        }
        func_8006C130_6CD30(&arg0->unk14, arg0->unk34);
    }

    gDPPipeSync(gRegionAllocPtr++);

    gDPSetTexturePersp(gRegionAllocPtr++, G_TP_PERSP);

    gSPSegment(gRegionAllocPtr++, 0x02, arg0->unk38);

    gSPMatrix(gRegionAllocPtr++, arg0->unk34, (G_MTX_NOPUSH | G_MTX_LOAD) | G_MTX_MODELVIEW);

    gSPDisplayList(gRegionAllocPtr++, &D_8008C200_8CE00);
}

void func_800078C4_84C4(func_800078C4_84C4_arg *arg0) {
    setCleanupCallback(&func_80007ABC_86BC);
    arg0->unk4 = loadCompressedData(&_646CD0_ROM_START, &_646CD0_ROM_END, 0xE8);
    arg0->unk44 = loadCompressedData(&_49B500_ROM_START, &_49B500_ROM_END, 0xC00);
    if (arg0->unk0->index == 0x3E) {
        arg0->unk40 = 0;
    } else {
        arg0->unk40 = 1;
    }
    setCallback(&func_80007958_8558);
}

void func_80007958_8558(func_800078C4_84C4_arg *arg0) {
    getTableEntryByU16Index(arg0->unk4, arg0->unk40 != 0, &arg0->unk8);
    arg0->unk3C = 0;
    setCallback(&func_8000799C_859C);
}

void func_8000799C_859C(func_800078C4_84C4_arg *arg0) {
    u8 sp10[0x20];
    s32 temp_t0;
    s8 temp_v0;
    void *temp_s0;

    memcpy(&sp10, &identityMatrix, 0x20);

    if (arg0->unk0->isDestroyed == 1) {
        func_80069CF8_6A8F8();
        return;
    }

    temp_v0 = arg0->unk0->actionMode;
    if (arg0->unk0->actionMode == 0) {
        arg0->unk3C = 0;
    } else if (temp_v0 < 0 || arg0->unk0->actionMode >= 3) {
        arg0->unk3C = 0;
    } else {
        do {
            if (arg0->unk3C == 0) {
                memcpy(&arg0->unk14, &arg0->unk0->unk18, 0x20);
                scaleMatrix(&arg0->unk14, 0x1000, 0x1000, 0x1000);
            }
        } while (0);
        setModelDisplayEnabled(arg0->unk0, 0);
    }

    if (arg0->unk0->actionMode == 1) {
        temp_t0 = arg0->unk3C;
        if (arg0->unk3C < 7) {
            arg0->unk34 = 0;
            arg0->unk38 = &arg0->unk44[temp_t0].unk0;
            debugEnqueueCallback(0, 1, &func_80007560_8160, arg0);
            arg0->unk3C++;
        }
    }
}

void func_80007ABC_86BC(func_800078C4_84C4_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
    arg0->unk44 = freeNodeMemory(arg0->unk44);
}
