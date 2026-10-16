#pragma once

#include "graphics/displaylist.h"
#include "ui/level_preview_3d.h"

typedef struct {
    /* 0x00 */ SceneModel *model;
    /* 0x04 */ DisplayListObject displayObject;
    /* 0x40 */ u16 rotationAngle;
    /* 0x42 */ s16 tiltAngle;
    /* 0x44 */ s8 bobbingEnabled;
    /* 0x48 */ s32 verticalOffset;
    /* 0x4C */ s32 verticalVelocity;
} BobbingModelTaskState;

void initBobbingModelTask(BobbingModelTaskState *state);
void initTiltingModelTask(void);
