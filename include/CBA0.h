#pragma once

#include "common.h"
#include "1DFAA0.h"

typedef struct {
    u8 pad[0x218];
    s8 renderMode;
} NodeRenderModeArg;

typedef struct {
    u8 padding[0x214];
    u8 wipeR;
    u8 wipeG;
    u8 wipeB;
} NodeWipeColorArg;

typedef struct {
    u8 pad[0x204];
    s32 unk204;
    s32 unk208;
    s16 unk20C;
} Func8000C2CCArg;

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

void setAuxRenderEnabled(Func8000C268Arg *arg0);
void clearAuxRenderEnabled(Func8000C268Arg *arg0);
void setNodeRenderMode(NodeRenderModeArg *arg0, s32 arg1);
void setNodeWipeColor(NodeWipeColorArg *node, u8 r, u8 g, u8 b);
void setNodeScaleTarget(Func8000C2CCArg *arg0, s32 arg1, s16 arg2);
void setNodePositionTarget(NodePositionTargetArg *arg0, s32 *arg1, s16 arg2);
void func_8000C440_D040(void *arg0, s32 arg1, s32 arg2, u16 arg3, s32 arg4, s32 arg5, s32 arg6);
