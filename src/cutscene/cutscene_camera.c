#include "cutscene/cutscene_camera.h"
#include "1E2BE0.h"

void cutsceneCameraReset_init(void) {
}

s32 cutsceneCameraReset_validate(void) {
    return 0;
}

void cutsceneCameraReset_exec(cutsceneCameraReset_exec_arg0 *arg0, cutsceneCameraReset_exec_arg1 *arg1) {
    initCutsceneCameraWithX(
        (CutsceneCameraState *)arg1->cameraState,
        arg0->rotX,
        arg0->rotY,
        arg0->posY,
        arg0->posZ,
        arg0->posX
    );
}

void cutsceneCameraMove_init(void) {
}

s32 cutsceneCameraMove_validate(void) {
    return 0;
}

void cutsceneCameraMove_exec(cutsceneCameraMove_exec_arg0 *arg0, cutsceneCameraMove_exec_arg1 *arg1) {
    animateCameraRotationX((CutsceneCameraState *)arg1->cameraState, arg0->targetRotX, arg0->duration);
    animateCameraRotationY((CutsceneCameraState *)arg1->cameraState, arg0->targetRotY, arg0->duration);
    animateCameraPositionX((CutsceneCameraState *)arg1->cameraState, arg0->targetX, arg0->duration);
    animateCameraPositionY((CutsceneCameraState *)arg1->cameraState, arg0->targetY, arg0->duration);
    animateCameraPositionZ((CutsceneCameraState *)arg1->cameraState, arg0->targetZ, arg0->duration);
}

s32 cutsceneCameraMove_isDone(void) {
    return 0;
}

void cutsceneCameraShake_init(void) {
}

s32 cutsceneCameraShake_validate(void) {
    return 0;
}

void cutsceneCameraShake_exec(cutsceneCameraRotate_exec_arg0 *arg0, cutsceneCameraRotate_exec_arg1 *arg1) {
    initCameraShake((CutsceneCameraShakeState *)arg1->cameraState, arg0->step * 0xA3D7, arg0->duration);
}

void cutsceneCameraRotate_init(void) {
}

s32 cutsceneCameraRotate_validate(void) {
    return 0;
}

void cutsceneCameraRotate_exec(cutsceneCameraRotate_exec_arg0 *arg0, cutsceneCameraRotate_exec_arg1 *arg1) {
    animateCameraRotationYContinuous((CutsceneCameraState *)arg1->cameraState, arg0->step, arg0->duration);
}

s32 cutsceneCameraRotate_isDone(void) {
    return 0;
}
