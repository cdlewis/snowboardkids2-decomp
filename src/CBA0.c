#include "1DFAA0.h"
#include "6E840.h"
#include "common.h"
#include "displaylist.h"
#include "geometry.h"
#include "task_scheduler.h"

typedef struct {
    /* 0x00 */ s32 vertStart1;
    /* 0x04 */ s32 vertEnd1;
    /* 0x08 */ s32 compStart1;
    /* 0x0C */ s32 compEnd1;
    /* 0x10 */ u16 compSize1;
    /* 0x12 */ u16 pad12;
    /* 0x14 */ void *dispList1;
    /* 0x18 */ s32 vertStart2;
    /* 0x1C */ s32 vertEnd2;
    /* 0x20 */ s32 compStart2;
    /* 0x24 */ s32 compEnd2;
    /* 0x28 */ u16 compSize2;
    /* 0x2A */ u16 pad2A;
    /* 0x2C */ void *dispList2;
} AssetDataBlock;

typedef struct {
    u8 pad0[0x1F0];
    s32 unk1F0;
    s32 unk1F4;
    s32 unk1F8;
    s16 unk1FC;
    s16 unk1FE;
    s16 unk200;
    s16 pad202;
    s32 unk204;
    s32 unk208;
    s16 unk20C;
    u16 unk20E;
} Func8000C5AC_Node;

typedef struct {
    /* 0x00 */ Func8000C5AC_Node *unk0;
    /* 0x04 */ Transform3D unk4;
    /* 0x24 */ void *unk24;
    /* 0x28 */ void *unk28;
    /* 0x2C */ void *unk2C;
    /* 0x30 */ void *unk30;
    /* 0x34 */ u8 pad34[0xC];
    /* 0x40 */ Transform3D unk40;
    /* 0x60 */ void *unk60;
    /* 0x64 */ void *unk64;
    /* 0x68 */ void *unk68;
    /* 0x6C */ void *unk6C;
} Func8000C5AC_Arg;

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
    Node_70B00 *node;
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
    Node_70B00 base;
    ColorData lightColor;
    ColorData ambientColor;
    u8 pad1E8[0x28];
    s32 renderFlags;
    u8 pad214[0x4];
    u8 renderMode;
} SceneRenderNode;

void func_8000C5AC_D1AC(Func8000C5AC_Arg *);
void updateSceneRenderTask(SceneRenderTaskCtx *ctx);
void cleanupSceneRenderTask(SceneRenderTaskData *task);

extern Gfx *volatile gRegionAllocPtr;
extern u8 D_8016A000[];
extern s32 gCurrentDisplayBufferIndex;
extern u8 gMemoryHeapEnd[];
extern Gfx D_8008CD20_8D920[];
extern s32 identityMatrix[];
extern AssetDataBlock D_8008CD98_8D998;

void setColorImageToAuxBuffer(void *arg0) {
    gDPSetColorImage(gRegionAllocPtr++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 320, D_8016A000);
}

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
    frameIdx = gCurrentDisplayBufferIndex;

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

    task = scheduleTask(func_8000C5AC_D1AC, taskArg1, taskArg2, 0);
    if (task != NULL) {
        *(SceneRenderNode **)task = node;
    }
}

void n_alSeqpDelete(Node_70B00 *arg0) {
    unlinkNode(arg0);
}

void func_8000C5AC_D1AC(Func8000C5AC_Arg *arg0) {
    Func8000C5AC_Node *temp;
    int new_var;
    AssetDataBlock *data;
    s16 scale;
    arg0->unk30 = 0;
    arg0->unk2C = 0;
    arg0->unk28 = 0;
    arg0->unk6C = 0;
    arg0->unk68 = 0;
    arg0->unk64 = 0;
    arg0->unk0->unk204 = 0x10000;
    arg0->unk0->unk208 = 0;
    arg0->unk0->unk20C = 0;
    arg0->unk0->unk20E = 0;
    memcpy(&arg0->unk4, identityMatrix, sizeof(Transform3D));
    memcpy(&arg0->unk40, identityMatrix, sizeof(Transform3D));
    temp = arg0->unk0;
    temp->unk200 = 0;
    temp->unk1FE = 0;
    temp->unk1FC = 0;
    data = &D_8008CD98_8D998;
    temp = arg0->unk0;
    new_var = 0x2000;
    temp->unk1F8 = 0;
    temp->unk1F4 = 0;
    temp->unk1F0 = 0;
    if (((!(&D_8008CD98_8D998)->vertStart2) && (!(&D_8008CD98_8D998)->vertStart2)) &&
        (!(&D_8008CD98_8D998)->vertStart2)) {}
    arg0->unk28 = loadUncompressedData((void *)(&D_8008CD98_8D998)->vertStart1, (void *)(&D_8008CD98_8D998)->vertEnd1);
    arg0->unk2C = loadCompressedData(
        (void *)(&D_8008CD98_8D998)->compStart1,
        (void *)(&D_8008CD98_8D998)->compEnd1,
        (&D_8008CD98_8D998)->compSize1
    );
    arg0->unk30 = 0;
    arg0->unk24 = (&D_8008CD98_8D998)->dispList1;
    scale = (s16)((((s64)(arg0->unk0->unk204 >> 8)) * new_var) >> 8);
    scaleMatrix(&arg0->unk4, scale, scale, scale);
    arg0->unk64 = loadUncompressedData((void *)(&D_8008CD98_8D998)->vertStart2, (void *)(&D_8008CD98_8D998)->vertEnd2);
    arg0->unk68 = loadCompressedData((void *)data->compStart2, (void *)data->compEnd2, data->compSize2);
    arg0->unk6C = 0;
    arg0->unk60 = data->dispList2;
    scale = (s16)((((s64)(arg0->unk0->unk204 >> 8)) * 0x2000) >> 8);
    scaleMatrix(&arg0->unk40, scale, scale, scale);
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
