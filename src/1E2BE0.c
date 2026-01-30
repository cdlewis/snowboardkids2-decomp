#include "1E2BE0.h"
#include "audio.h"
#include "common.h"
#include "geometry.h"
#include "task_scheduler.h"

extern Transform3D D_8009A890_9B490;

void setAnimationLoopMode(CutsceneCameraState *arg0, s8 mode) {
    arg0->inputMode = mode;
}

void initAnimationLoopState(CutsceneCameraState *, u16);

void *func_800B5B38_1E2BE8(u16 arg0) {
    CutsceneCameraState *temp = allocateNodeMemory(0x74);
    initAnimationLoopState(temp, arg0);
    return temp;
}

void initAnimationLoopState(CutsceneCameraState *arg0, u16 arg1) {
    arg0->rotXCurrent = 0;
    arg0->rotYCurrent = 0;
    arg0->rotXTarget = 0;
    arg0->rotYTarget = 0;
    arg0->rotXStep = 0;
    arg0->rotYStep = 0;
    arg0->rotXDuration = 0;
    arg0->rotYDuration = 0;
    arg0->rotXDurationCopy = 0;
    arg0->rotYDurationCopy = 0;
    arg0->posXCurrent = 0;
    arg0->posYCurrent = 0;
    arg0->posZCurrent = 0;
    arg0->posXTarget = 0;
    arg0->posYTarget = 0;
    arg0->posZTarget = 0;
    arg0->posXStep = 0;
    arg0->posYStep = 0;
    arg0->posZStep = 0;
    arg0->posXDuration = 0;
    arg0->posYDuration = 0;
    arg0->posZDuration = 0;
    arg0->posXDurationCopy = 0;
    arg0->posYDurationCopy = 0;
    arg0->posZDurationCopy = 0;
    arg0->posYOffset = 0;
    arg0->shakeAmplitude = 0;
    arg0->shakeDuration = 0;
    arg0->nodeId = arg1;
    arg0->animMode = 0;
    arg0->inputMode = 0;
}

void freeAnimationLoopState(void *arg0) {
    freeNodeMemory(arg0);
}

void finalizeAnimationLoop(AnimationLoopArg *arg0) {
    Transform3D sp10;
    Transform3D sp30;
    Transform3D sp50;
    Transform3D sp70;
    Transform3D *temp_s0;

    D_8009A890_9B490.translation.x = arg0->posXCurrent;
    D_8009A890_9B490.translation.y = arg0->posYCurrent + arg0->posYOffset;
    D_8009A890_9B490.translation.z = arg0->posZCurrent;

    createYRotationMatrix(&sp70, arg0->rotYCurrent);

    sp70.translation.x = 0;
    sp70.translation.y = 0;
    sp70.translation.z = 0;

    func_8006B084_6BC84(&D_8009A890_9B490, &sp70, &sp30);

    createXRotationMatrix(sp50.m, arg0->rotXCurrent);

    sp50.translation.x = 0;
    sp50.translation.y = 0;
    sp50.translation.z = 0;

    func_8006B084_6BC84(&sp50, &sp30, &sp10);

    temp_s0 = &sp10;
    memcpy(arg0, temp_s0, 0x20);
    queueAnonymousBufferData(temp_s0);
    setViewportTransformById(arg0->nodeId, temp_s0);
}

extern s32 gControllerInputs[];
extern s32 gButtonsPressed[];
extern s8 D_800AB04B;
extern s8 gAnalogStickY;
extern u8 gAnalogStickX;

void handleAnimationLoopDebugInput(CutsceneCameraState *arg0) {
    s8 mode = arg0->inputMode;
    s32 temp;
    s32 buttonCheck;

    switch (mode) {
        case 0:
            if (gControllerInputs[3] & Z_TRIG) {
                arg0->posXCurrent = 0;
            } else {
                temp = arg0->posXCurrent;
                temp += D_800AB04B << 12;
                arg0->posXCurrent = temp;
            }

            buttonCheck = gButtonsPressed[0];

            if (buttonCheck & R_TRIG) {
                temp = arg0->posYCurrent;
                temp += gAnalogStickY << 12;
                arg0->posYCurrent = temp;
            } else if (buttonCheck & Z_TRIG) {
                temp = arg0->posZCurrent;
                temp -= gAnalogStickY << 12;
                arg0->posZCurrent = temp;
            } else if (buttonCheck & L_TRIG) {
                u16 temp_angle = arg0->rotXCurrent;
                temp_angle -= gAnalogStickY;
                arg0->rotXCurrent = temp_angle & 0x1FFF;
            } else {
                u16 temp_angle = arg0->rotYCurrent;
                temp_angle -= (s8)gAnalogStickX;
                arg0->rotYCurrent = temp_angle & 0x1FFF;
            }
            break;

        case 1:
            if (gControllerInputs[3] & Z_TRIG) {
                arg0->posXCurrent = 0;
            } else if (gButtonsPressed[3] & R_TRIG) {
                temp = arg0->posXCurrent;
                temp += D_800AB04B << 16;
                arg0->posXCurrent = temp;
            } else {
                temp = arg0->posXCurrent;
                temp += D_800AB04B << 12;
                arg0->posXCurrent = temp;
            }

            if (gButtonsPressed[0] & R_TRIG) {
                temp = arg0->posYCurrent;
                temp += gAnalogStickY << 12;
                arg0->posYCurrent = temp;
            }
            break;
    }
}

void initCutsceneCameraRotationAndPos(CutsceneCameraState *arg0, s16 rotX, s16 rotY, s32 posY, s32 posZ) {
    arg0->rotXCurrent = rotX;
    arg0->rotYCurrent = rotY;
    arg0->rotXTarget = rotX;
    arg0->rotYTarget = rotY;
    arg0->rotXStep = 0;
    arg0->rotYStep = 0;
    arg0->rotXDuration = 0;
    arg0->rotYDuration = 0;
    arg0->rotXDurationCopy = 0;
    arg0->rotYDurationCopy = 0;
    arg0->posYCurrent = posY;
    arg0->posYTarget = posY;
    arg0->posXStep = 0;
    arg0->posYStep = 0;
    arg0->posZStep = 0;
    arg0->posYDuration = 0;
    arg0->posZDuration = 0;
    arg0->posYDurationCopy = 0;
    arg0->posZDurationCopy = 0;
    arg0->animMode = 0;
    arg0->posZCurrent = posZ;
    arg0->posZTarget = posZ;
}

void initCutsceneCameraWithX(CutsceneCameraState *arg0, s16 rotX, s16 rotY, s32 posY, s32 posZ, s32 posX) {
    initCutsceneCameraRotationAndPos(arg0, rotX, rotY, posY, posZ);
    arg0->posXCurrent = posX;
    arg0->posXTarget = posX;
    arg0->posXStep = 0;
    arg0->posXDuration = 0;
    arg0->posXDurationCopy = 0;
}

void copyRotDurationToPosX(CutsceneCameraState *arg0, CutsceneCameraState *arg1) {
    arg0->posXCurrent = *(s32 *)&arg1->rotXDuration;
}

void animateCameraRotationX(CutsceneCameraState *arg0, s16 targetRotX, s16 duration) {
    s16 temp_a3;
    s16 temp_a1;
    int new_var;
    s32 var_v1;
    s32 var_v0;

    *(volatile u16 *)&arg0->rotXCurrent = *(u16 *)&arg0->rotXCurrent & 0x1FFF;
    temp_a3 = arg0->rotXCurrent;
    temp_a1 = targetRotX & 0x1FFF;
    new_var = temp_a1 - temp_a3;
    arg0->animMode = 0;
    var_v1 = new_var;
    arg0->rotXTarget = temp_a1;
    if (var_v1 >= 0x1001) {
        var_v0 = temp_a3 + 0x2000;
        var_v1 = temp_a1 - var_v0;
    } else if (var_v1 < -0x1000) {
        var_v0 = temp_a3 - 0x2000;
        var_v1 = temp_a1 - var_v0;
    }
    arg0->rotXDuration = duration;
    arg0->rotXDurationCopy = duration;
    arg0->rotXStep = var_v1 / duration;
}

void animateCameraRotationY(CutsceneCameraState *arg0, s16 targetRotY, s16 duration) {
    s16 temp_a3;
    s16 temp_a1;
    int new_var;
    s32 var_v1;
    s32 var_v0;

    *(volatile u16 *)&arg0->rotYCurrent = *(u16 *)&arg0->rotYCurrent & 0x1FFF;
    temp_a3 = arg0->rotYCurrent;
    temp_a1 = targetRotY & 0x1FFF;
    new_var = temp_a1 - temp_a3;
    arg0->animMode = 0;
    var_v1 = new_var;
    arg0->rotYTarget = temp_a1;
    if (var_v1 >= 0x1001) {
        var_v0 = temp_a3 + 0x2000;
        var_v1 = temp_a1 - var_v0;
    } else if (var_v1 < -0x1000) {
        var_v0 = temp_a3 - 0x2000;
        var_v1 = temp_a1 - var_v0;
    }
    arg0->rotYDuration = duration;
    arg0->rotYDurationCopy = duration;
    arg0->rotYStep = var_v1 / duration;
}

void animateCameraPositionX(CutsceneCameraState *arg0, s32 targetX, s16 duration) {
    s32 quotient;

    quotient = (targetX - arg0->posXCurrent) / duration;

    arg0->animMode = 0;
    arg0->posXTarget = targetX;
    arg0->posXDuration = duration;
    arg0->posXDurationCopy = duration;
    arg0->posXStep = quotient;
}

void animateCameraPositionY(CutsceneCameraState *arg0, s32 targetY, s16 duration) {
    s32 quotient;

    quotient = (targetY - arg0->posYCurrent) / duration;

    arg0->animMode = 0;
    arg0->posYTarget = targetY;
    arg0->posYDuration = duration;
    arg0->posYDurationCopy = duration;
    arg0->posYStep = quotient;
}

void animateCameraPositionZ(CutsceneCameraState *arg0, s32 targetZ, s16 duration) {
    s32 quotient;

    quotient = (targetZ - arg0->posZCurrent) / duration;

    arg0->animMode = 0;
    arg0->posZTarget = targetZ;
    arg0->posZDuration = duration;
    arg0->posZDurationCopy = duration;
    arg0->posZStep = quotient;
}

void animateCameraRotationYContinuous(CutsceneCameraState *camera, s16 step, s16 duration) {
    if ((step == 0) || (duration == 0)) {
        camera->animMode = 0;
        camera->rotYStep = 0;
        camera->rotYDuration = 0;
        camera->rotYDurationCopy = 0;
    } else {
        camera->animMode = 1;
        camera->rotYStep = step;
        camera->rotYDuration = duration;
        camera->rotYDurationCopy = duration;
    }
}

void initCameraShake(CutsceneCameraShakeState *cameraShake, s32 amplitude, s16 duration) {
    cameraShake->animMode = 0;
    cameraShake->shakeAmplitude = amplitude;
    cameraShake->shakeDuration = duration;
}

INCLUDE_ASM("asm/nonmatchings/1E2BE0", func_800B6190_1E3240);

s16 advanceCameraRotationYContinuous(CutsceneCameraState *camera) {
    if (camera->rotYDurationCopy != 0) {
        camera->rotYCurrent += camera->rotYStep;
        if (camera->rotYDurationCopy > 0) {
            camera->rotYDurationCopy--;
        }
    } else {
        camera->animMode = 2;
    }
    return 0;
}

extern s16 func_800B6190_1E3240(CutsceneCameraState *);

s16 advanceSceneManager(CutsceneCameraState *arg0) {
    s16 result = 0;
    s8 temp;

    if (arg0->inputMode == 1) {
        arg0->posYTarget = 0x166666;
        arg0->posYCurrent = 0x166666;
    }

    temp = arg0->animMode;

    if (temp != 1) {
        if (temp >= 2) {
        } else if (temp == 0) {
            result = func_800B6190_1E3240(arg0);
        }
    } else {
        result = advanceCameraRotationYContinuous(arg0);
    }

    return result;
}
