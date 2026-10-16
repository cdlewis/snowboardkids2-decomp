#include "graphics/model_loader.h"
#include "assets.h"
#include "common.h"
#include "core/buffers.h"
#include "gbi.h"
#include "generated/model_gfx.h"
#include "generated/modelpayload_asset_sizes.h"
#include "graphics/displaylist.h"
#include "graphics/graphics.h"
#include "math/geometry.h"
#include "system/task_scheduler.h"

typedef struct {
    /* 0x00 */ void *vertStart1;
    /* 0x04 */ void *vertEnd1;
    /* 0x08 */ void *compStart1;
    /* 0x0C */ void *compEnd1;
    /* 0x10 */ u16 compSize1;
    /* 0x12 */ u16 pad12;
    /* 0x14 */ DisplayLists *dispList1;
    /* 0x18 */ void *vertStart2;
    /* 0x1C */ void *vertEnd2;
    /* 0x20 */ void *compStart2;
    /* 0x24 */ void *compEnd2;
    /* 0x28 */ u16 compSize2;
    /* 0x2A */ u16 pad2A;
    /* 0x2C */ DisplayLists *dispList2;
} AssetDataBlock;

typedef struct {
    /* 0x00 */ SceneRenderNode *node;
    /* 0x04 */ DisplayListObject object1;
    /* 0x40 */ DisplayListObject object2;
} SceneRenderTask;

DisplayLists gCutsceneSceneRenderDisplayList1 = { 0, CUTSCENE_SCENE_RENDER_VERTEX_DATA_1_part_00A0, NULL, NULL };

DisplayLists gCutsceneSceneRenderDisplayList2 = { 0, CUTSCENE_SCENE_RENDER_VERTEX_DATA_2_part_0088, NULL, NULL };

u32 gViewportFillTexture[] = {
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
};

Gfx gViewportFillSetupDisplayList[] = {
    gsDPPipeSync(),
    gsDPSetCycleType(G_CYC_1CYCLE),
    gsDPSetTexturePersp(G_TP_NONE),
    gsDPSetTextureFilter(G_TF_POINT),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPSetCombineMode(G_CC_MODULATEI_PRIM, G_CC_MODULATEI_PRIM),
    gsDPSetRenderMode(G_RM_AA_ZB_OPA_INTER, G_RM_AA_ZB_OPA_INTER2),
    gsDPSetTextureImage(G_IM_FMT_I, G_IM_SIZ_8b, 8, gViewportFillTexture),
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
    gsDPLoadTile(G_TX_LOADTILE, 0, 0, 8 << G_TEXTURE_IMAGE_FRAC, 4 << G_TEXTURE_IMAGE_FRAC),
    gsDPPipeSync(),
    gsDPSetTile(G_IM_FMT_I, G_IM_SIZ_4b, 1, 0, G_TX_RENDERTILE, 0, G_TX_WRAP, 2, G_TX_NOLOD, G_TX_WRAP, 4, G_TX_NOLOD),
    gsDPSetTileSize(G_TX_RENDERTILE, 0, 0, 16 << G_TEXTURE_IMAGE_FRAC, 4 << G_TEXTURE_IMAGE_FRAC),
    gsSPEndDisplayList(),
};

AssetDataBlock gCutsceneSceneRenderAssetData = {
    .vertStart1 = &CUTSCENE_SCENE_RENDER_VERTEX_DATA_1_ROM_START,
    .vertEnd1 = &CUTSCENE_SCENE_RENDER_VERTEX_DATA_1_ROM_END,
    .compStart1 = &CUTSCENE_SCENE_RENDER_TEXTURE_1_ROM_START,
    .compEnd1 = &CUTSCENE_SCENE_RENDER_TEXTURE_1_ROM_END,
    .compSize1 = CUTSCENE_SCENE_RENDER_TEXTURE_1_DECOMPRESSED_SIZE,
    .pad12 = 0x0001,
    .dispList1 = &gCutsceneSceneRenderDisplayList1,
    .vertStart2 = &CUTSCENE_SCENE_RENDER_VERTEX_DATA_2_ROM_START,
    .vertEnd2 = &CUTSCENE_SCENE_RENDER_VERTEX_DATA_2_ROM_END,
    .compStart2 = &CUTSCENE_SCENE_RENDER_TEXTURE_2_ROM_START,
    .compEnd2 = &CUTSCENE_SCENE_RENDER_TEXTURE_2_ROM_END,
    .compSize2 = CUTSCENE_SCENE_RENDER_TEXTURE_2_DECOMPRESSED_SIZE,
    .pad2A = 0x0001,
    .dispList2 = &gCutsceneSceneRenderDisplayList2,
};

void loadSceneRenderTaskData(SceneRenderTask *);
void updateSceneRenderTask(SceneRenderTask *ctx);
void cleanupSceneRenderTask(SceneRenderTask *task);

void setColorImageToMainFramebuffer(void *arg0) {
    gDPSetColorImage(gDisplayListAllocPtr++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 320, &gFrameBuffer);
}

void buildViewportAuxFramebufferFillDisplayList(SceneRenderTask *arg0) {
    ViewportNode *node;
    node = &arg0->node->base;
    gDPSetColorImage(
        gDisplayListAllocPtr++,
        G_IM_FMT_RGBA,
        G_IM_SIZ_16b,
        320,
        &gAuxFrameBuffers[gCurrentDisplayBufferIndex]
    );
    gSPDisplayList(gDisplayListAllocPtr++, gViewportFillSetupDisplayList);
    gDPSetScissor(
        gDisplayListAllocPtr++,
        G_SC_NON_INTERLACE,
        node->clipLeft,
        node->clipTop,
        node->clipRight,
        node->clipBottom
    );
    gDPSetPrimColor(gDisplayListAllocPtr++, 0, 0, 0, 0, 0, 255);
    gDPSetPrimDepth(gDisplayListAllocPtr++, 0xFFFC, 0);
    gDPSetDepthSource(gDisplayListAllocPtr++, G_ZS_PRIM);
    gSPTextureRectangle(
        gDisplayListAllocPtr++,
        node->clipLeft << 2,
        node->clipTop << 2,
        node->clipRight << 2,
        node->clipBottom << 2,
        G_TX_RENDERTILE,
        0,
        0,
        0x400,
        0x400
    );
    gDPSetDepthSource(gDisplayListAllocPtr++, G_ZS_PIXEL);
}

void enqueueViewportAuxFramebufferFill(SceneRenderTask *ctx) {
    pushViewportCallbackBySlot(
        ctx->node->base.callbackSlotIndex,
        VIEWPORT_CALLBACK_LAYER_FINAL,
        buildViewportAuxFramebufferFillDisplayList,
        ctx
    );
}

void enqueueMainFramebufferTargetSetup(SceneRenderTask *ctx) {
    pushViewportCallbackBySlot(
        ctx->node->base.callbackSlotIndex,
        VIEWPORT_CALLBACK_LAYER_INITIAL,
        setColorImageToMainFramebuffer,
        ctx
    );
}

void enableSceneRenderViewportEffect(SceneRenderNode *node) {
    node->state.anim.renderFlags |= 1;
}

void disableSceneRenderViewportEffect(SceneRenderNode *node) {
    node->state.anim.renderFlags &= ~1;
}

void copyNodePosition(SceneRenderNode *node, const Vec3i *position) {
    memcpy(&node->state.anim.position, position, sizeof(Vec3i));
}

void setNodeScale(SceneRenderNode *node, s32 scale) {
    node->state.anim.scale = scale;
}

void setNodeRenderMode(SceneRenderNode *node, s32 renderMode) {
    node->state.anim.renderMode = renderMode;
}

void setNodeWipeColor(SceneRenderNode *node, u8 r, u8 g, u8 b) {
    node->state.anim.wipeR = r;
    node->state.anim.wipeG = g;
    node->state.anim.wipeB = b;
}

void setNodeScaleTarget(SceneRenderNode *node, s32 scale, s16 frames) {
    if (frames == 0) {
        node->state.anim.scaleFrames = 0;
        node->state.anim.scaleVelocity = 0;
        node->state.anim.scale = scale;
    } else {
        node->state.anim.scaleFrames = frames;
        node->state.anim.scaleVelocity = (scale - node->state.anim.scale) / frames;
    }
}

void setNodePositionTarget(SceneRenderNode *node, Vec3i *targetPosition, s16 frames) {
    s16 temp = frames;

    if (frames == 0) {
        node->state.anim.posFramesX = 0;
        node->state.anim.posFramesY = 0;
        node->state.anim.posFramesZ = 0;
        node->state.anim.velocity.x = 0;
        node->state.anim.velocity.y = 0;
        node->state.anim.velocity.z = 0;
        node->state.anim.position.x = targetPosition->x;
        node->state.anim.position.y = targetPosition->y;
        node->state.anim.position.z = targetPosition->z;
    } else {
        node->state.anim.posFramesX = temp;
        node->state.anim.posFramesY = temp;
        node->state.anim.posFramesZ = temp;
        node->state.anim.velocity.x = (targetPosition->x - node->state.anim.position.x) / frames;
        node->state.anim.velocity.y = (targetPosition->y - node->state.anim.position.y) / frames;
        node->state.anim.velocity.z = (targetPosition->z - node->state.anim.position.z) / frames;
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
    setModelCameraTransform(&node->base, 0, 0, -0xA0, -0x78, 0x9F, 0x77);
    setViewportPerspective(&node->base, 40.0f, 1.3333334f, 10.0f, 10000.0f);

    node->state.lighting.lightColor.directionX = 0;
    node->state.lighting.lightColor.directionY = 0x7F;
    node->state.lighting.lightColor.directionZ = 0x7F;
    node->state.lighting.lightColor.r = 0;
    node->state.lighting.lightColor.g = 0;
    node->state.lighting.lightColor.b = 0;
    node->state.lighting.ambientLight.color.r = 0;
    node->state.lighting.ambientLight.color.g = 0;
    node->state.lighting.ambientLight.color.b = 0;

    setViewportLightColors(
        node->base.viewportId,
        1,
        &node->state.lighting.lightColor,
        &node->state.lighting.ambientLight.color
    );

    node->state.anim.renderFlags = 0;
    node->state.anim.renderMode = renderMode;

    task = scheduleTask(loadSceneRenderTaskData, taskArg1, taskArg2, 0);
    if (task != NULL) {
        *(SceneRenderNode **)task = node;
    }
}

void unlinkViewportNode(ViewportNode *arg0) {
    unlinkNode(arg0);
}

void loadSceneRenderTaskData(SceneRenderTask *ctx) {
    SceneRenderNode *node;
    int modelScale;
    AssetDataBlock *data;
    s16 scale;
    ctx->object1.segment3 = 0;
    ctx->object1.segment2 = 0;
    ctx->object1.segment1 = 0;
    ctx->object2.segment3 = 0;
    ctx->object2.segment2 = 0;
    ctx->object2.segment1 = 0;
    ctx->node->state.anim.scale = 0x10000;
    ctx->node->state.anim.scaleVelocity = 0;
    ctx->node->state.anim.scaleFrames = 0;
    ctx->node->state.anim.rotationAngle = 0;
    memcpy(&ctx->object1.transform, &identityMatrix, sizeof(Transform3D));
    memcpy(&ctx->object2.transform, &identityMatrix, sizeof(Transform3D));
    node = ctx->node;
    node->state.anim.posFramesZ = 0;
    node->state.anim.posFramesY = 0;
    node->state.anim.posFramesX = 0;
    data = &gCutsceneSceneRenderAssetData;
    node = ctx->node;
    modelScale = 0x2000;
    node->state.anim.velocity.z = 0;
    node->state.anim.velocity.y = 0;
    node->state.anim.velocity.x = 0;
    if (((!(&gCutsceneSceneRenderAssetData)->vertStart2) && (!(&gCutsceneSceneRenderAssetData)->vertStart2)) &&
        (!(&gCutsceneSceneRenderAssetData)->vertStart2)) {}
    ctx->object1.segment1 =
        loadUncompressedData((&gCutsceneSceneRenderAssetData)->vertStart1, (&gCutsceneSceneRenderAssetData)->vertEnd1);
    ctx->object1.segment2 = loadCompressedData(
        (&gCutsceneSceneRenderAssetData)->compStart1,
        (&gCutsceneSceneRenderAssetData)->compEnd1,
        (&gCutsceneSceneRenderAssetData)->compSize1
    );
    ctx->object1.segment3 = 0;
    ctx->object1.displayLists = (&gCutsceneSceneRenderAssetData)->dispList1;
    scale = (s16)((((s64)(ctx->node->state.anim.scale >> 8)) * modelScale) >> 8);
    scaleMatrix(&ctx->object1.transform, scale, scale, scale);
    ctx->object2.segment1 =
        loadUncompressedData((&gCutsceneSceneRenderAssetData)->vertStart2, (&gCutsceneSceneRenderAssetData)->vertEnd2);
    ctx->object2.segment2 = loadCompressedData(data->compStart2, data->compEnd2, data->compSize2);
    ctx->object2.segment3 = 0;
    ctx->object2.displayLists = data->dispList2;
    scale = (s16)((((s64)(ctx->node->state.anim.scale >> 8)) * 0x2000) >> 8);
    scaleMatrix(&ctx->object2.transform, scale, scale, scale);
    setCleanupCallback(cleanupSceneRenderTask);
    setCallback(updateSceneRenderTask);
}

void updateSceneRenderTask(SceneRenderTask *ctx) {
    Transform3D sp10;
    Transform3D sp30;
    Transform3D *rotMatrix;
    Transform3D *tempMatrix;
    SceneRenderNode *nodeA;
    SceneRenderNode *nodeB;
    SceneRenderNode *nodeC;
    Transform3D *transform;
    SceneRenderNode *nodeD;
    s16 scaleFactor;

    nodeB = ctx->node;
    if (nodeB->state.anim.renderFlags & 1) {
        rotMatrix = &sp10;
        memcpy(rotMatrix, &identityMatrix, sizeof(Transform3D));
        tempMatrix = &sp30;
        memcpy(tempMatrix, &identityMatrix, sizeof(Transform3D));
        enableViewportDisplayList(ctx->node);
        enqueueViewportAuxFramebufferFill(ctx);
        nodeB = ctx->node;

        switch (nodeB->state.anim.renderMode) {
            case 0:
                nodeB->state.anim.rotationAngle = 0;
                nodeD = ctx->node;
                transform = &ctx->object1.transform;
                memcpy(transform, &nodeD->base.viewTransform, sizeof(Transform3D));
                memcpy(&ctx->object1.transform.translation, &ctx->node->state.anim.position, sizeof(Vec3i));
                nodeA = ctx->node;
                scaleFactor = (s16)((s64)(nodeA->state.anim.scale >> 8) * 0x2000 >> 8);
                scaleMatrix(transform, scaleFactor, scaleFactor, scaleFactor);
                enqueueDisplayListObject(ctx->node->base.callbackSlotIndex, &ctx->object1);
                break;
            case 1:
                createZRotationMatrix(rotMatrix, nodeB->state.anim.rotationAngle);
                memcpy(tempMatrix, &ctx->node->base.viewTransform, sizeof(Transform3D));
                composeTransform3D(rotMatrix, tempMatrix, &ctx->object2.transform);
                memcpy(&ctx->object2.transform.translation, &ctx->node->state.anim.position, sizeof(Vec3i));
                nodeA = ctx->node;
                scaleFactor = (s16)((s64)(nodeA->state.anim.scale >> 8) * 0x2000 >> 8);
                scaleMatrix(&ctx->object2.transform, scaleFactor, scaleFactor, scaleFactor);
                enqueueDisplayListObject(ctx->node->base.callbackSlotIndex, &ctx->object2);
                nodeB = ctx->node;
                if (nodeB->state.anim.scaleFrames != 0) {
                    nodeB->state.anim.rotationAngle += 0xB6;
                }
                break;
        }

        enqueueMainFramebufferTargetSetup(ctx);
        nodeC = ctx->node;
        if (nodeC->state.anim.scaleFrames > 0) {
            nodeC->state.anim.scale += nodeC->state.anim.scaleVelocity;
            nodeB = ctx->node;
            nodeB->state.anim.scaleFrames--;
            nodeC = ctx->node;
        }
        if (nodeC->state.anim.posFramesX > 0) {
            nodeC->state.anim.position.x += nodeC->state.anim.velocity.x;
            nodeB = ctx->node;
            nodeB->state.anim.posFramesX--;
        }
        nodeC = ctx->node;
        if (nodeC->state.anim.posFramesY > 0) {
            nodeC->state.anim.position.y += nodeC->state.anim.velocity.y;
            nodeB = ctx->node;
            nodeB->state.anim.posFramesY--;
        }
        nodeC = ctx->node;
        if (nodeC->state.anim.posFramesZ > 0) {
            nodeC->state.anim.position.z += nodeC->state.anim.velocity.z;
            nodeB = ctx->node;
            nodeB->state.anim.posFramesZ--;
        }
    } else {
        nodeB->state.anim.scale = 0x10000;
        ctx->node->state.anim.scaleVelocity = 0;
        ctx->node->state.anim.scaleFrames = 0;
        ctx->node->state.anim.rotationAngle = 0;
    }
}

void cleanupSceneRenderTask(SceneRenderTask *task) {
    task->object1.segment3 = freeNodeMemory(task->object1.segment3);
    task->object1.segment2 = freeNodeMemory(task->object1.segment2);
    task->object1.segment1 = freeNodeMemory(task->object1.segment1);
    task->object2.segment3 = freeNodeMemory(task->object2.segment3);
    task->object2.segment2 = freeNodeMemory(task->object2.segment2);
    task->object2.segment1 = freeNodeMemory(task->object2.segment1);
}
