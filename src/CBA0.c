#include "1DFAA0.h"
#include "6E840.h"
#include "common.h"
#include "displaylist.h"
#include "geometry.h"
#include "task_scheduler.h"

extern Gfx *volatile gRegionAllocPtr;
extern u8 D_8016A000[];
extern s32 D_8009AFCC_9BBCC;
extern u8 gMemoryHeapEnd[];
extern Gfx D_8008CD20_8D920[];

void setColorImageToAuxBuffer(void *arg0) {
    gDPSetColorImage(gRegionAllocPtr++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 320, D_8016A000);
}

typedef struct {
    Node_70B00 *node;
} AuxBufferContext;

void buildAuxBufferDisplayList(AuxBufferContext *arg0) {
    Node_70B00 *node;
    Gfx *gfx;
    int new_var2;
    s32 frameIdx;
    s32 otherMode;
    s32 texCoord;
    f32 lrxf;
    f32 lryf;
    s32 lrx;
    s32 lry;
    int new_var;
    s32 ulx;
    s32 uly;

    node = arg0->node;
    gfx = gRegionAllocPtr;
    frameIdx = D_8009AFCC_9BBCC;

    gfx->words.w0 = 0xFF10013F;
    gfx->words.w1 = (u32)(gMemoryHeapEnd + frameIdx * 0x25800);

    gfx[1].words.w0 = 0xDE000000;
    gfx[1].words.w1 = (u32)D_8008CD20_8D920;

    {
        s32 ulxf = (s32)(node->unkB0 * 4.0f);
        s32 ulyf;
        otherMode = 0xE2001D00;
        gfx[2].words.w0 =
            ((unsigned long long)((ulxf & 0xFFF) << 12)) | ((((s32)(node->unkB2 * 4.0f)) & 0xFFF) | 0xED000000);
    }

    lrxf = node->unkB4;
    lryf = node->unkB6;
    texCoord = 0x04000400;

    gfx[3].words.w0 = 0xFA000000;
    new_var2 = 0xFF;
    gfx[3].words.w1 = new_var2;
    lrxf = lrxf * 4.0f;

    gfx[4].words.w0 = 0xEE000000;
    gfx[4].words.w1 = 0xFFFC0000;
    lryf = lryf * 4.0f;

    gfx[5].words.w0 = otherMode;
    gfx[5].words.w1 = 4;

    gfx[2].words.w1 = (((s32)lrxf & 0xFFF) << 12) | ((s32)lryf & 0xFFF);

    lrx = node->unkB4;
    lry = node->unkB6;
    gfx[6].words.w0 = (((lrx << 2) & 0xFFF) << 12) | (((node->unkB6 << 2) & 0xFFF) | (new_var = 0xE4000000));

    ulx = node->unkB0;
    uly = 1;
    gRegionAllocPtr = gfx + uly;
    gRegionAllocPtr = gfx + 2;
    gRegionAllocPtr = gfx + 3;
    gRegionAllocPtr = gfx + 4;
    gRegionAllocPtr = gfx + 5;
    gRegionAllocPtr = gfx + 6;
    gRegionAllocPtr = gfx + 7;
    gRegionAllocPtr = gfx + 8;

    uly = node->unkB2;
    gfx[6].words.w1 = (((ulx << 2) & 0xFFF) << 12) | ((uly << 2) & 0xFFF);

    gfx[7].words.w0 = 0xE1000000;
    gRegionAllocPtr = gfx + 9;

    gfx[8].words.w0 = 0xF1000000;
    gfx[7].words.w1 = 0;
    gfx[8].words.w1 = texCoord;

    gRegionAllocPtr = gfx + 10;

    frameIdx = 0;
    gfx[9].words.w0 = otherMode;
    gfx[9].words.w1 = frameIdx;
}

void enqueueAuxBufferRender(AuxBufferContext *ctx) {
    debugEnqueueCallback(ctx->node->slot_index, 7, buildAuxBufferDisplayList, ctx);
}

void enqueueAuxBufferSetup(AuxBufferContext *ctx) {
    debugEnqueueCallback(ctx->node->slot_index, 0, setColorImageToAuxBuffer, ctx);
}

void setAuxRenderEnabled(Func8000C268Arg *arg0) {
    arg0->unk210 |= 1;
}

void clearAuxRenderEnabled(Func8000C268Arg *arg0) {
    arg0->unk210 &= ~1;
}

typedef struct {
    u8 pad[0x1E4];
    s32 position[3];
} NodePositionArg;

void copyNodePosition(NodePositionArg *node, void *position) {
    memcpy(node->position, position, 0xC);
}

typedef struct {
    u8 pad[0x204];
    s32 unk204;
} Func8000C2ACArg;

void setNodeScale(Func8000C2ACArg *arg0, s32 arg1) {
    arg0->unk204 = arg1;
}

typedef struct {
    u8 pad[0x218];
    s8 renderMode;
} NodeRenderModeArg;

void setNodeRenderMode(NodeRenderModeArg *arg0, s32 arg1) {
    arg0->renderMode = arg1;
}

typedef struct {
    u8 pad[0x214];
    u8 wipeR;
    u8 wipeG;
    u8 wipeB;
} NodeWipeColorArg;

void setNodeWipeColor(NodeWipeColorArg *node, u8 r, u8 g, u8 b) {
    node->wipeR = r;
    node->wipeG = g;
    node->wipeB = b;
}

typedef struct {
    u8 pad[0x204];
    s32 unk204;
    s32 unk208;
    s16 unk20C;
} Func8000C2CCArg;

void setNodeScaleTarget(Func8000C2CCArg *arg0, s32 arg1, s16 arg2) {
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
    s32 posX;
    s32 posY;
    s32 posZ;
    s32 velocityX;
    s32 velocityY;
    s32 velocityZ;
    s16 framesRemainingX;
    s16 framesRemainingY;
    s16 framesRemainingZ;
} NodePositionTargetArg;

void setNodePositionTarget(NodePositionTargetArg *node, s32 *targetPos, s16 frames) {
    s16 temp = frames;

    if (frames == 0) {
        node->framesRemainingX = 0;
        node->framesRemainingY = 0;
        node->framesRemainingZ = 0;
        node->velocityX = 0;
        node->velocityY = 0;
        node->velocityZ = 0;
        node->posX = targetPos[0];
        node->posY = targetPos[1];
        node->posZ = targetPos[2];
    } else {
        node->framesRemainingX = temp;
        node->framesRemainingY = temp;
        node->framesRemainingZ = temp;
        node->velocityX = (targetPos[0] - node->posX) / frames;
        node->velocityY = (targetPos[1] - node->posY) / frames;
        node->velocityZ = (targetPos[2] - node->posZ) / frames;
    }
}

extern void func_8000C5AC_D1AC(void *);

typedef struct {
    Node_70B00 base;
    ColorData lightColor;
    ColorData ambientColor;
    u8 pad1E8[0x28];
    s32 unk210;
    u8 pad214[0x4];
    u8 renderMode;
} SceneRenderNode;

void initSceneRenderNode(
    SceneRenderNode *node,
    u16 slotIndex,
    u8 priority,
    u16 sceneId,
    u8 renderMode,
    u16 taskArg1,
    u16 taskArg2
) {
    void *task;
    s32 pad[8];

    func_8006FAA4_706A4(&node->base, NULL, slotIndex, priority, 1);
    func_8006F9BC_705BC(&node->base, 1.0f, 1.0f);
    func_8006FEF8_70AF8(&node->base, sceneId);
    setModelCameraTransform(node, 0, 0, -0xA0, -0x78, 0x9F, 0x77);
    func_8006FA0C_7060C(&node->base, 40.0f, 1.3333334f, 10.0f, 10000.0f);

    node->lightColor.r2 = 0;
    node->lightColor.g2 = 0x7F;
    node->lightColor.b2 = 0x7F;
    node->lightColor.r = 0;
    node->lightColor.g = 0;
    node->lightColor.b = 0;
    node->ambientColor.r = 0;
    node->ambientColor.g = 0;
    node->ambientColor.b = 0;

    func_8006FC70_70870(node->base.id, 1, &node->lightColor, &node->ambientColor);

    node->unk210 = 0;
    node->renderMode = renderMode;

    task = scheduleTask(func_8000C5AC_D1AC, taskArg1, taskArg2, 0);
    if (task != NULL) {
        *(SceneRenderNode **)task = node;
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
    u8 baseTransform[0x20]; // 0x120
    u8 pad140[0xA4];
    s32 posX;          // 0x1E4
    s32 posY;          // 0x1E8
    s32 posZ;          // 0x1EC
    s32 velocityX;     // 0x1F0
    s32 velocityY;     // 0x1F4
    s32 velocityZ;     // 0x1F8
    s16 posFramesX;    // 0x1FC
    s16 posFramesY;    // 0x1FE
    s16 posFramesZ;    // 0x200
    s16 pad202;        // 0x202
    s32 scale;         // 0x204
    s32 scaleVelocity; // 0x208
    s16 scaleFrames;   // 0x20C
    u16 rotationAngle; // 0x20E
    s32 renderFlags;   // 0x210
    u8 pad214[0x4];
    s8 renderMode; // 0x218
} SceneRenderNodeExt;

typedef struct {
    s16 m[9]; // 0x12 bytes
    u16 pad12;
    s32 pos[3]; // 0x14-0x1F
} MatrixWithPos;

typedef struct {
    SceneRenderNodeExt *node;
    MatrixWithPos staticTransform;
    u8 pad24[0x1C];
    MatrixWithPos rotatedTransform;
} SceneRenderTaskCtx;

void updateSceneRenderTask(SceneRenderTaskCtx *ctx) {
    Transform3D sp10;
    Transform3D sp30;
    Transform3D *rotMatrix;
    Transform3D *tempMatrix;
    SceneRenderNodeExt *nodeA;
    SceneRenderNodeExt *nodeB;
    SceneRenderNodeExt *nodeC;
    MatrixWithPos *transform;
    SceneRenderNodeExt *nodeD;
    s16 scaleFactor;

    nodeB = ctx->node;
    if (nodeB->renderFlags & 1) {
        rotMatrix = &sp10;
        memcpy(rotMatrix, identityMatrix, 0x20);
        tempMatrix = &sp30;
        memcpy(tempMatrix, identityMatrix, 0x20);
        func_8006FED8_70AD8(ctx->node);
        enqueueAuxBufferRender((AuxBufferContext *)ctx);
        nodeB = ctx->node;

        switch (nodeB->renderMode) {
            case 0:
                nodeB->rotationAngle = 0;
                nodeD = ctx->node;
                transform = &ctx->staticTransform;
                memcpy(transform, nodeD->baseTransform, 0x20);
                memcpy(ctx->staticTransform.pos, &ctx->node->posX, 0xC);
                nodeA = ctx->node;
                scaleFactor = (s16)((s64)(nodeA->scale >> 8) * 0x2000 >> 8);
                scaleMatrix((Transform3D *)transform, scaleFactor, scaleFactor, scaleFactor);
                enqueueDisplayListObject(ctx->node->slot_index, (DisplayListObject *)transform);
                break;
            case 1:
                createZRotationMatrix(rotMatrix, nodeB->rotationAngle);
                memcpy(tempMatrix, ctx->node->baseTransform, 0x20);
                func_8006B084_6BC84(rotMatrix, tempMatrix, &ctx->rotatedTransform);
                memcpy(ctx->rotatedTransform.pos, &ctx->node->posX, 0xC);
                nodeA = ctx->node;
                scaleFactor = (s16)((s64)(nodeA->scale >> 8) * 0x2000 >> 8);
                scaleMatrix((Transform3D *)&ctx->rotatedTransform, scaleFactor, scaleFactor, scaleFactor);
                enqueueDisplayListObject(ctx->node->slot_index, (DisplayListObject *)&ctx->rotatedTransform);
                nodeB = ctx->node;
                if (nodeB->scaleFrames != 0) {
                    nodeB->rotationAngle += 0xB6;
                }
                break;
        }

        enqueueAuxBufferSetup((AuxBufferContext *)ctx);
        nodeC = ctx->node;
        if (nodeC->scaleFrames > 0) {
            nodeC->scale += nodeC->scaleVelocity;
            nodeB = ctx->node;
            nodeB->scaleFrames--;
            nodeC = ctx->node;
        }
        if (nodeC->posFramesX > 0) {
            nodeC->posX += nodeC->velocityX;
            nodeB = ctx->node;
            nodeB->posFramesX--;
        }
        nodeC = ctx->node;
        if (nodeC->posFramesY > 0) {
            nodeC->posY += nodeC->velocityY;
            nodeB = ctx->node;
            nodeB->posFramesY--;
        }
        nodeC = ctx->node;
        if (nodeC->posFramesZ > 0) {
            nodeC->posZ += nodeC->velocityZ;
            nodeB = ctx->node;
            nodeB->posFramesZ--;
        }
    } else {
        nodeB->scale = 0x10000;
        ctx->node->scaleVelocity = 0;
        ctx->node->scaleFrames = 0;
        ctx->node->rotationAngle = 0;
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
