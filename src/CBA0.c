#include "1DFAA0.h"
#include "6E840.h"
#include "common.h"
#include "displaylist.h"
#include "geometry.h"
#include "task_scheduler.h"

extern Gfx *gRegionAllocPtr;
extern u8 D_8016A000[];

void setColorImageToAuxBuffer(void *arg0) {
    gDPSetColorImage(gRegionAllocPtr++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 320, D_8016A000);
}

INCLUDE_ASM("asm/nonmatchings/CBA0", buildAuxBufferDisplayList);

extern void buildAuxBufferDisplayList(void *);

typedef struct {
    Node_70B00 *node;
} AuxBufferContext;

void enqueueAuxBufferRender(AuxBufferContext *ctx) {
    debugEnqueueCallback(ctx->node->slot_index, 7, buildAuxBufferDisplayList, ctx);
}

void enqueueAuxBufferSetup(AuxBufferContext *ctx) {
    debugEnqueueCallback(ctx->node->slot_index, 0, setColorImageToAuxBuffer, ctx);
}

void func_8000C268_CE68(Func8000C268Arg *arg0) {
    arg0->unk210 |= 1;
}

void func_8000C278_CE78(Func8000C268Arg *arg0) {
    arg0->unk210 &= ~1;
}

typedef struct {
    u8 pad[0x1E4];
    u8 unk1E4[0xC];
} Func8000C28CArg;

void func_8000C28C_CE8C(Func8000C28CArg *arg0, void *arg1) {
    memcpy(arg0->unk1E4, arg1, 0xC);
}

typedef struct {
    u8 pad[0x204];
    s32 unk204;
} Func8000C2ACArg;

void func_8000C2AC_CEAC(Func8000C2ACArg *arg0, s32 arg1) {
    arg0->unk204 = arg1;
}

typedef struct {
    u8 pad[0x218];
    s8 unk218;
} Func8000C2B4Arg;

void func_8000C2B4_CEB4(Func8000C2B4Arg *arg0, s32 arg1) {
    arg0->unk218 = arg1;
}

typedef struct {
    u8 pad[0x214];
    u8 unk214;
    u8 unk215;
    u8 unk216;
} Func8000C2BCArg;

void func_8000C2BC_CEBC(Func8000C2BCArg *arg0, u8 arg1, u8 arg2, u8 arg3) {
    arg0->unk214 = arg1;
    arg0->unk215 = arg2;
    arg0->unk216 = arg3;
}

typedef struct {
    u8 pad[0x204];
    s32 unk204;
    s32 unk208;
    s16 unk20C;
} Func8000C2CCArg;

void func_8000C2CC_CECC(Func8000C2CCArg *arg0, s32 arg1, s16 arg2) {
    if (arg2 == 0) {
        arg0->unk20C = 0;
        arg0->unk208 = 0;
        arg0->unk204 = arg1;
    } else {
        arg0->unk20C = arg2;
        arg0->unk208 = (arg1 - arg0->unk204) / arg2;
    }
}

typedef struct {
    u8 pad[0x1E4];
    s32 unk1E4;
    s32 unk1E8;
    s32 unk1EC;
    s32 unk1F0;
    s32 unk1F4;
    s32 unk1F8;
    s16 unk1FC;
    s16 unk1FE;
    s16 unk200;
} Func8000C334Arg;

void func_8000C334_CF34(Func8000C334Arg *arg0, s32 *arg1, s16 arg2) {
    s16 temp = arg2;

    if (arg2 == 0) {
        arg0->unk1FC = 0;
        arg0->unk1FE = 0;
        arg0->unk200 = 0;
        arg0->unk1F0 = 0;
        arg0->unk1F4 = 0;
        arg0->unk1F8 = 0;
        arg0->unk1E4 = arg1[0];
        arg0->unk1E8 = arg1[1];
        arg0->unk1EC = arg1[2];
    } else {
        arg0->unk1FC = temp;
        arg0->unk1FE = temp;
        arg0->unk200 = temp;
        arg0->unk1F0 = (arg1[0] - arg0->unk1E4) / arg2;
        arg0->unk1F4 = (arg1[1] - arg0->unk1E8) / arg2;
        arg0->unk1F8 = (arg1[2] - arg0->unk1EC) / arg2;
    }
}

extern void func_8000C5AC_D1AC(void *);

typedef struct {
    Node_70B00 base;
    ColorData unk1D8;
    ColorData unk1E0;
    u8 pad1E8[0x28];
    s32 unk210;
    u8 pad214[0x4];
    u8 unk218;
} Func8000C440Arg;

void func_8000C440_D040(Func8000C440Arg *arg0, u16 arg1, u8 arg2, u16 arg3, u8 arg4, u16 arg5, u16 arg6) {
    void *task;
    s32 pad[8];

    func_8006FAA4_706A4(&arg0->base, NULL, arg1, arg2, 1);
    func_8006F9BC_705BC(&arg0->base, 1.0f, 1.0f);
    func_8006FEF8_70AF8(&arg0->base, arg3);
    setModelCameraTransform(arg0, 0, 0, -0xA0, -0x78, 0x9F, 0x77);
    func_8006FA0C_7060C(&arg0->base, 40.0f, 1.3333334f, 10.0f, 10000.0f);

    arg0->unk1D8.r2 = 0;
    arg0->unk1D8.g2 = 0x7F;
    arg0->unk1D8.b2 = 0x7F;
    arg0->unk1D8.r = 0;
    arg0->unk1D8.g = 0;
    arg0->unk1D8.b = 0;
    arg0->unk1E0.r = 0;
    arg0->unk1E0.g = 0;
    arg0->unk1E0.b = 0;

    func_8006FC70_70870(arg0->base.id, 1, &arg0->unk1D8, &arg0->unk1E0);

    arg0->unk210 = 0;
    arg0->unk218 = arg4;

    task = scheduleTask(func_8000C5AC_D1AC, arg5, arg6, 0);
    if (task != NULL) {
        *(Func8000C440Arg **)task = arg0;
    }
}

void n_alSeqpDelete(Node_70B00 *arg0) {
    unlinkNode(arg0);
}

INCLUDE_ASM("asm/nonmatchings/CBA0", func_8000C5AC_D1AC);

extern s16 identityMatrix[];

typedef struct {
    u8 pad0[0x16];
    u16 slot_index;
    u8 pad18[0x108];
    u8 padding4[0x20]; // 0x120
    u8 pad140[0xA4];
    s32 unk1E4; // 0x1E4
    s32 unk1E8; // 0x1E8
    s32 unk1EC; // 0x1EC
    s32 unk1F0; // 0x1F0
    s32 unk1F4; // 0x1F4
    s32 unk1F8; // 0x1F8
    s16 unk1FC; // 0x1FC
    s16 unk1FE; // 0x1FE
    s16 unk200; // 0x200
    s16 pad202; // 0x202
    s32 unk204; // 0x204
    s32 unk208; // 0x208
    s16 unk20C; // 0x20C
    u16 unk20E; // 0x20E
    s32 unk210; // 0x210
    u8 pad214[0x4];
    s8 unk218; // 0x218
} ExtendedNode;

typedef struct {
    s16 m[9]; // 0x12 bytes
    u16 pad12;
    s32 pos[3]; // 0x14-0x1F
} MatrixWithPos;

typedef struct {
    ExtendedNode *unk0;
    MatrixWithPos unk4;
    u8 pad24[0x1C];
    MatrixWithPos unk40;
} Func8000C7A4Arg;

void func_8000C7A4_D3A4(Func8000C7A4Arg *arg0) {
    Mat3x3Padded sp10;
    Mat3x3Padded sp30;
    Mat3x3Padded *mat1;
    Mat3x3Padded *mat2;
    ExtendedNode *temp_v0;
    ExtendedNode *temp_v1;
    ExtendedNode *temp_a0;
    MatrixWithPos *temp_s0;
    ExtendedNode *temp_a1;
    s16 scale;

    temp_v1 = arg0->unk0;
    if (temp_v1->unk210 & 1) {
        mat1 = &sp10;
        memcpy(mat1, identityMatrix, 0x20);
        mat2 = &sp30;
        memcpy(mat2, identityMatrix, 0x20);
        func_8006FED8_70AD8(arg0->unk0);
        enqueueAuxBufferRender((AuxBufferContext *)arg0);
        temp_v1 = arg0->unk0;

        switch (temp_v1->unk218) {
            case 0:
                temp_v1->unk20E = 0;
                temp_a1 = arg0->unk0;
                temp_s0 = &arg0->unk4;
                memcpy(temp_s0, temp_a1->padding4, 0x20);
                memcpy(arg0->unk4.pos, &arg0->unk0->unk1E4, 0xC);
                temp_v0 = arg0->unk0;
                scale = (s16)((s64)(temp_v0->unk204 >> 8) * 0x2000 >> 8);
                scaleMatrix((Mat3x3Padded *)temp_s0, scale, scale, scale);
                enqueueDisplayListObject(arg0->unk0->slot_index, (DisplayListObject *)temp_s0);
                break;
            case 1:
                createZRotationMatrix(mat1, temp_v1->unk20E);
                memcpy(mat2, arg0->unk0->padding4, 0x20);
                func_8006B084_6BC84(mat1, mat2, &arg0->unk40);
                memcpy(arg0->unk40.pos, &arg0->unk0->unk1E4, 0xC);
                temp_v0 = arg0->unk0;
                scale = (s16)((s64)(temp_v0->unk204 >> 8) * 0x2000 >> 8);
                scaleMatrix((Mat3x3Padded *)&arg0->unk40, scale, scale, scale);
                enqueueDisplayListObject(arg0->unk0->slot_index, (DisplayListObject *)&arg0->unk40);
                temp_v1 = arg0->unk0;
                if (temp_v1->unk20C != 0) {
                    temp_v1->unk20E += 0xB6;
                }
                break;
        }

        enqueueAuxBufferSetup((AuxBufferContext *)arg0);
        temp_a0 = arg0->unk0;
        if (temp_a0->unk20C > 0) {
            temp_a0->unk204 += temp_a0->unk208;
            temp_v1 = arg0->unk0;
            temp_v1->unk20C--;
            temp_a0 = arg0->unk0;
        }
        if (temp_a0->unk1FC > 0) {
            temp_a0->unk1E4 += temp_a0->unk1F0;
            temp_v1 = arg0->unk0;
            temp_v1->unk1FC--;
        }
        temp_a0 = arg0->unk0;
        if (temp_a0->unk1FE > 0) {
            temp_a0->unk1E8 += temp_a0->unk1F4;
            temp_v1 = arg0->unk0;
            temp_v1->unk1FE--;
        }
        temp_a0 = arg0->unk0;
        if (temp_a0->unk200 > 0) {
            temp_a0->unk1EC += temp_a0->unk1F8;
            temp_v1 = arg0->unk0;
            temp_v1->unk200--;
        }
    } else {
        temp_v1->unk204 = 0x10000;
        arg0->unk0->unk208 = 0;
        arg0->unk0->unk20C = 0;
        arg0->unk0->unk20E = 0;
    }
}

typedef struct {
    u8 pad0[0x28];
    void *unk28;
    void *unk2C;
    void *unk30;
    u8 pad34[0x30];
    void *unk64;
    void *unk68;
    void *unk6C;
} Func8000CA80Arg;

void func_8000CA80_D680(Func8000CA80Arg *arg0) {
    arg0->unk30 = freeNodeMemory(arg0->unk30);
    arg0->unk2C = freeNodeMemory(arg0->unk2C);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
    arg0->unk6C = freeNodeMemory(arg0->unk6C);
    arg0->unk68 = freeNodeMemory(arg0->unk68);
    arg0->unk64 = freeNodeMemory(arg0->unk64);
}
