#pragma once

#include "common.h"
#include "data/data_table.h"
#include "gamestate.h"
#include "gbi.h"
#include "math/geometry.h"
#include "ui/level_preview_3d.h"

typedef struct {
    /* 0x00 */ SceneModel *model;
    /* 0x04 */ DataTable_19E80 *textureTable;
    /* 0x08 */ OutputStruct_19E80 texture;
    /* 0x14 */ Transform3D transform;
    /* 0x34 */ Mtx *n64Transform;
    /* 0x38 */ ItemBoxBurstFrameData *frameData;
    /* 0x3C */ s32 frameIndex;
    /* 0x40 */ s32 textureIndex;
    /* 0x44 */ ItemBoxBurstFrameData *frames;
} ModelTransitionEffectState;

typedef struct {
    /* 0x00 */ SceneModel *model;
    /* 0x04 */ s16 rotationX;
    /* 0x06 */ s16 rotationY;
    /* 0x08 */ s16 rotationZ;
    /* 0x0A */ s16 angularAccelerationX;
    /* 0x0C */ s16 angularAccelerationY;
    /* 0x0E */ s16 angularAccelerationZ;
    /* 0x10 */ s16 angularVelocityX;
    /* 0x12 */ s16 angularVelocityY;
    /* 0x14 */ s16 angularVelocityZ;
    /* 0x16 */ s16 padding16;
} CameraRotationTaskState;

void initModelTransitionEffect(ModelTransitionEffectState *state);
void initCameraRotationTask(CameraRotationTaskState *state);
