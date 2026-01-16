#pragma once

#include "common.h"

typedef struct {
    u8 _pad[0x20];
    s16 rotXCurrent;
    s16 rotYCurrent;
    s16 rotXTarget;
    s16 rotYTarget;
    s16 rotXStep;
    s16 rotYStep;
    s16 rotXDuration;
    s16 rotYDuration;
    s16 rotXDurationCopy;
    s16 rotYDurationCopy;
    s32 posXCurrent;
    s32 posYCurrent;
    s32 posZCurrent;
    s32 posXTarget;
    s32 posYTarget;
    s32 posZTarget;
    s32 posXStep;
    s32 posYStep;
    s32 posZStep;
    s16 posXDuration;
    s16 posYDuration;
    s16 posZDuration;
    s16 posXDurationCopy;
    s16 posYDurationCopy;
    s16 posZDurationCopy;
    s32 posYOffset;
    s32 shakeAmplitude;
    s16 shakeDuration;
    s16 nodeId;
    s8 animMode;
    s8 inputMode;
} CutsceneCameraState;

typedef CutsceneCameraState AnimationLoopArg;

typedef struct {
    u8 _pad[0x68];
    s32 shakeAmplitude;
    s16 shakeDuration;
    s16 unk6E;
    u8 animMode;
    u8 unk71;
} CutsceneCameraShakeState;

void setAnimationLoopMode(CutsceneCameraState *arg0, s8 mode);
void freeAnimationLoopState(void *arg0);
void initCutsceneCameraWithX(CutsceneCameraState *arg0, s16 rotX, s16 rotY, s32 posY, s32 posZ, s32 posX);
void copyRotDurationToPosX(CutsceneCameraState *arg0, CutsceneCameraState *arg1);
void animateCameraRotationX(CutsceneCameraState *arg0, s16 targetRotX, s16 duration);
void animateCameraRotationY(CutsceneCameraState *arg0, s16 targetRotY, s16 duration);
void animateCameraPositionX(CutsceneCameraState *arg0, s32 targetX, s16 duration);
void animateCameraPositionY(CutsceneCameraState *arg0, s32 targetY, s16 duration);
void animateCameraPositionZ(CutsceneCameraState *arg0, s32 targetZ, s16 duration);
void animateCameraRotationYContinuous(CutsceneCameraState *camera, s16 step, s16 duration);
void initCameraShake(CutsceneCameraShakeState *arg0, s32 amplitude, s16 duration);
void finalizeAnimationLoop(AnimationLoopArg *arg0);
s16 advanceSceneManager(CutsceneCameraState *arg0);
