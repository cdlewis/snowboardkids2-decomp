#include "1DFAA0.h"
#include "buffers.h"
#include "common.h"
#include "displaylist.h"
#include "geometry.h"
#include "graphics.h"
#include "rom_loader.h"
#include "task_scheduler.h"

typedef struct {
    /* 0x00 */ void *vertStart1;
    /* 0x04 */ void *vertEnd1;
    /* 0x08 */ void *compStart1;
    /* 0x0C */ void *compEnd1;
    /* 0x10 */ u16 compSize1;
    /* 0x12 */ u16 pad12;
    /* 0x14 */ void *dispList1;
    /* 0x18 */ void *vertStart2;
    /* 0x1C */ void *vertEnd2;
    /* 0x20 */ void *compStart2;
    /* 0x24 */ void *compEnd2;
    /* 0x28 */ u16 compSize2;
    /* 0x2A */ u16 pad2A;
    /* 0x2C */ void *dispList2;
} AssetDataBlock;

typedef struct {
    u8 pad0[0x1F0];
    s32 velocityX;
    s32 velocityY;
    s32 velocityZ;
    s16 posFramesX;
    s16 posFramesY;
    s16 posFramesZ;
    s16 pad202;
    s32 scale;
    s32 scaleVelocity;
    s16 scaleFrames;
    u16 rotationAngle;
} SceneRenderAnimationState;

typedef struct {
    /* 0x00 */ SceneRenderAnimationState *animState;
    /* 0x04 */ Transform3D transform1;
    /* 0x24 */ void *displayList1;
    /* 0x28 */ void *modelData1;
    /* 0x2C */ void *textureData1;
    /* 0x30 */ void *unk30;
    /* 0x34 */ u8 pad34[0xC];
    /* 0x40 */ Transform3D transform2;
    /* 0x60 */ void *displayList2;
    /* 0x64 */ void *modelData2;
    /* 0x68 */ void *textureData2;
    /* 0x6C */ void *unk6C;
} SceneRenderTaskLoadContext;

typedef struct {
    u8 pad0[0x28];
    void *modelData1;
    void *textureData1;
    void *spare1;
    u8 pad34[0x30];
    void *modelData2;
    void *textureData2;
    void *spare2;
} SceneRenderTaskData;

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
    SceneRenderNodeExt *node;
    Transform3D staticTransform;
    u8 pad24[0x1C];
    Transform3D rotatedTransform;
} SceneRenderTaskCtx;

typedef struct {
    ViewportNode *node;
} AuxBufferContext;

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

typedef struct {
    u8 pad[0x204];
    s32 unk204;
    s32 unk208;
    s16 unk20C;
} Func8000C2CCArg;

typedef struct {
    u8 pad[0x214];
    u8 wipeR;
    u8 wipeG;
    u8 wipeB;
} NodeWipeColorArg;

typedef struct {
    u8 pad[0x1E4];
    s32 position[3];
} NodePositionArg;

typedef struct {
    u8 pad[0x204];
    s32 unk204;
} Func8000C2ACArg;

typedef struct {
    u8 pad[0x218];
    s8 renderMode;
} NodeRenderModeArg;

typedef struct {
    ViewportNode base;
    ColorData lightColor;
    ColorData ambientColor;
    u8 pad1E8[0x28];
    s32 renderFlags;
    u8 pad214[0x4];
    u8 renderMode;
} SceneRenderNode;

void loadSceneRenderTaskData(SceneRenderTaskLoadContext *);
void updateSceneRenderTask(SceneRenderTaskCtx *ctx);
void cleanupSceneRenderTask(SceneRenderTaskData *task);

extern Gfx *volatile gRegionAllocPtr;
extern s32 gCurrentDisplayBufferIndex;
extern s32 identityMatrix[];

Gfx D_8008CCE0_8D8E0[] = {
    { .words = { 0x00000000, 0x010000A0 } },
    { .words = { 0x00000000, 0x00000000 } },
};

Gfx D_8008CCF0_8D8F0[] = {
    { .words = { 0x00000000, 0x01000088 } },
    { .words = { 0x00000000, 0x00000000 } },
};

u32 D_8008CD00_8D900[] = {
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
};

Gfx D_8008CD20_8D920[] = {
    gsDPPipeSync(),
    gsDPSetCycleType(G_CYC_1CYCLE),
    gsDPSetTexturePersp(G_TP_NONE),
    gsDPSetTextureFilter(G_TF_POINT),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPSetCombineMode(G_CC_MODULATEI_PRIM, G_CC_MODULATEI_PRIM),
    gsDPSetRenderMode(G_RM_AA_ZB_OPA_INTER, G_RM_AA_ZB_OPA_INTER2),
    gsDPSetTextureImage(G_IM_FMT_I, G_IM_SIZ_8b, 8, D_8008CD00_8D900),
    gsDPSetTile(
        G_IM_FMT_I,
        G_IM_SIZ_8b,
        1,
        0x0000,
        G_TX_LOADTILE,
        0,
        G_TX_NOMIRROR | G_TX_WRAP,
        2,
        G_TX_NOLOD,
        G_TX_NOMIRROR | G_TX_WRAP,
        4,
        G_TX_NOLOD
    ),
    gsDPLoadSync(),
    { .words = { 0xF4000000, 0x07020010 } },
    gsDPPipeSync(),
    { .words = { 0xF5800200, 0x00008040 } },
    { .words = { 0xF2000000, 0x00040010 } },
    gsSPEndDisplayList(),
};

USE_ASSET(_214E70);
USE_ASSET(_663330);
USE_ASSET(_214F20);
USE_ASSET(_6633B0);

AssetDataBlock D_8008CD98_8D998 = {
    .vertStart1 = &_214E70_ROM_START,
    .vertEnd1 = &_214E70_ROM_END,
    .compStart1 = &_663330_ROM_START,
    .compEnd1 = &_663330_ROM_END,
    .compSize1 = 0x0180,
    .pad12 = 0x0001,
    .dispList1 = D_8008CCE0_8D8E0,
    .vertStart2 = &_214F20_ROM_START,
    .vertEnd2 = &_214F20_ROM_END,
    .compStart2 = &_6633B0_ROM_START,
    .compEnd2 = &_6633B0_ROM_END,
    .compSize2 = 0x0120,
    .pad2A = 0x0001,
    .dispList2 = D_8008CCF0_8D8F0,
};

void setColorImageToAuxBuffer(void *arg0) {
    gDPSetColorImage(gRegionAllocPtr++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 320, &gFrameBuffer);
}

void buildAuxBufferDisplayList(AuxBufferContext *arg0) {
    ViewportNode *node;
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
    frameIdx = gCurrentDisplayBufferIndex;

    gfx->words.w0 = 0xFF10013F;
    gfx->words.w1 = (u32)(&gAuxFrameBuffers[frameIdx]);

    gfx[1].words.w0 = 0xDE000000;
    gfx[1].words.w1 = (u32)D_8008CD20_8D920;

    {
        s32 ulxf = (s32)(node->clipLeft * 4.0f);
        s32 ulyf;
        otherMode = 0xE2001D00;
        gfx[2].words.w0 =
            ((unsigned long long)((ulxf & 0xFFF) << 12)) | ((((s32)(node->clipTop * 4.0f)) & 0xFFF) | 0xED000000);
    }

    lrxf = node->clipRight;
    lryf = node->clipBottom;
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

    lrx = node->clipRight;
    lry = node->clipBottom;
    gfx[6].words.w0 = (((lrx << 2) & 0xFFF) << 12) | (((node->clipBottom << 2) & 0xFFF) | (new_var = 0xE4000000));

    ulx = node->clipLeft;
    uly = 1;
    gRegionAllocPtr = gfx + uly;
    gRegionAllocPtr = gfx + 2;
    gRegionAllocPtr = gfx + 3;
    gRegionAllocPtr = gfx + 4;
    gRegionAllocPtr = gfx + 5;
    gRegionAllocPtr = gfx + 6;
    gRegionAllocPtr = gfx + 7;
    gRegionAllocPtr = gfx + 8;

    uly = node->clipTop;
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
    arg0->renderFlags |= 1;
}

void clearAuxRenderEnabled(Func8000C268Arg *arg0) {
    arg0->renderFlags &= ~1;
}

void copyNodePosition(NodePositionArg *node, void *position) {
    memcpy(node->position, position, 0xC);
}

void setNodeScale(Func8000C2ACArg *arg0, s32 arg1) {
    arg0->unk204 = arg1;
}

void setNodeRenderMode(NodeRenderModeArg *arg0, s32 arg1) {
    arg0->renderMode = arg1;
}

void setNodeWipeColor(NodeWipeColorArg *node, u8 r, u8 g, u8 b) {
    node->wipeR = r;
    node->wipeG = g;
    node->wipeB = b;
}

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

    initViewportNode(&node->base, NULL, slotIndex, priority, 1);
    setViewportScale(&node->base, 1.0f, 1.0f);
    setViewportId(&node->base, sceneId);
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

    setViewportLightColors(node->base.id, 1, &node->lightColor, &node->ambientColor);

    node->renderFlags = 0;
    node->renderMode = renderMode;

    task = scheduleTask(loadSceneRenderTaskData, taskArg1, taskArg2, 0);
    if (task != NULL) {
        *(SceneRenderNode **)task = node;
    }
}

void n_alSeqpDelete(ViewportNode *arg0) {
    unlinkNode(arg0);
}

void loadSceneRenderTaskData(SceneRenderTaskLoadContext *ctx) {
    SceneRenderAnimationState *animState;
    int modelScale;
    AssetDataBlock *data;
    s16 scale;
    ctx->unk30 = 0;
    ctx->textureData1 = 0;
    ctx->modelData1 = 0;
    ctx->unk6C = 0;
    ctx->textureData2 = 0;
    ctx->modelData2 = 0;
    ctx->animState->scale = 0x10000;
    ctx->animState->scaleVelocity = 0;
    ctx->animState->scaleFrames = 0;
    ctx->animState->rotationAngle = 0;
    memcpy(&ctx->transform1, identityMatrix, sizeof(Transform3D));
    memcpy(&ctx->transform2, identityMatrix, sizeof(Transform3D));
    animState = ctx->animState;
    animState->posFramesZ = 0;
    animState->posFramesY = 0;
    animState->posFramesX = 0;
    data = &D_8008CD98_8D998;
    animState = ctx->animState;
    modelScale = 0x2000;
    animState->velocityZ = 0;
    animState->velocityY = 0;
    animState->velocityX = 0;
    if (((!(&D_8008CD98_8D998)->vertStart2) && (!(&D_8008CD98_8D998)->vertStart2)) &&
        (!(&D_8008CD98_8D998)->vertStart2)) {}
    ctx->modelData1 =
        loadUncompressedData((void *)(&D_8008CD98_8D998)->vertStart1, (void *)(&D_8008CD98_8D998)->vertEnd1);
    ctx->textureData1 = loadCompressedData(
        (void *)(&D_8008CD98_8D998)->compStart1,
        (void *)(&D_8008CD98_8D998)->compEnd1,
        (&D_8008CD98_8D998)->compSize1
    );
    ctx->unk30 = 0;
    ctx->displayList1 = (&D_8008CD98_8D998)->dispList1;
    scale = (s16)((((s64)(ctx->animState->scale >> 8)) * modelScale) >> 8);
    scaleMatrix(&ctx->transform1, scale, scale, scale);
    ctx->modelData2 =
        loadUncompressedData((void *)(&D_8008CD98_8D998)->vertStart2, (void *)(&D_8008CD98_8D998)->vertEnd2);
    ctx->textureData2 = loadCompressedData((void *)data->compStart2, (void *)data->compEnd2, data->compSize2);
    ctx->unk6C = 0;
    ctx->displayList2 = data->dispList2;
    scale = (s16)((((s64)(ctx->animState->scale >> 8)) * 0x2000) >> 8);
    scaleMatrix(&ctx->transform2, scale, scale, scale);
    setCleanupCallback(cleanupSceneRenderTask);
    setCallback(updateSceneRenderTask);
}

void updateSceneRenderTask(SceneRenderTaskCtx *ctx) {
    Transform3D sp10;
    Transform3D sp30;
    Transform3D *rotMatrix;
    Transform3D *tempMatrix;
    SceneRenderNodeExt *nodeA;
    SceneRenderNodeExt *nodeB;
    SceneRenderNodeExt *nodeC;
    Transform3D *transform;
    SceneRenderNodeExt *nodeD;
    s16 scaleFactor;

    nodeB = ctx->node;
    if (nodeB->renderFlags & 1) {
        rotMatrix = &sp10;
        memcpy(rotMatrix, identityMatrix, sizeof(Transform3D));
        tempMatrix = &sp30;
        memcpy(tempMatrix, identityMatrix, sizeof(Transform3D));
        enableViewportDisplayList(ctx->node);
        enqueueAuxBufferRender((AuxBufferContext *)ctx);
        nodeB = ctx->node;

        switch (nodeB->renderMode) {
            case 0:
                nodeB->rotationAngle = 0;
                nodeD = ctx->node;
                transform = &ctx->staticTransform;
                memcpy(transform, nodeD->baseTransform, sizeof(Transform3D));
                memcpy(&ctx->staticTransform.translation, &ctx->node->posX, sizeof(Vec3i));
                nodeA = ctx->node;
                scaleFactor = (s16)((s64)(nodeA->scale >> 8) * 0x2000 >> 8);
                scaleMatrix((Transform3D *)transform, scaleFactor, scaleFactor, scaleFactor);
                enqueueDisplayListObject(ctx->node->slot_index, (DisplayListObject *)transform);
                break;
            case 1:
                createZRotationMatrix(rotMatrix, nodeB->rotationAngle);
                memcpy(tempMatrix, ctx->node->baseTransform, sizeof(Transform3D));
                func_8006B084_6BC84(rotMatrix, tempMatrix, &ctx->rotatedTransform);
                memcpy(&ctx->rotatedTransform.translation, &ctx->node->posX, 0xC);
                nodeA = ctx->node;
                scaleFactor = (s16)((s64)(nodeA->scale >> 8) * 0x2000 >> 8);
                scaleMatrix(&ctx->rotatedTransform, scaleFactor, scaleFactor, scaleFactor);
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

void cleanupSceneRenderTask(SceneRenderTaskData *task) {
    task->spare1 = freeNodeMemory(task->spare1);
    task->textureData1 = freeNodeMemory(task->textureData1);
    task->modelData1 = freeNodeMemory(task->modelData1);
    task->spare2 = freeNodeMemory(task->spare2);
    task->textureData2 = freeNodeMemory(task->textureData2);
    task->modelData2 = freeNodeMemory(task->modelData2);
}
