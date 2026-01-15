#include "cutscene/cutscene_camera.h"
#include "1E2BE0.h"

void cutsceneCameraReset_init(void) {
}

s32 cutsceneCameraReset_validate(void) {
    return 0;
}

void cutsceneCameraReset_exec(cutsceneCameraReset_exec_arg0 *arg0, cutsceneCameraReset_exec_arg1 *arg1) {
    initCutsceneCameraWithX(
        (CutsceneCameraState *)arg1->unkC,
        arg0->unk0,
        arg0->unk2,
        arg0->unk4,
        arg0->unk8,
        arg0->unkC
    );
}

void cutsceneCameraMove_init(void) {
}

s32 cutsceneCameraMove_validate(void) {
    return 0;
}

void cutsceneCameraMove_exec(cutsceneCameraMove_exec_arg0 *arg0, cutsceneCameraMove_exec_arg1 *arg1) {
    animateCameraRotationX((CutsceneCameraState *)arg1->unkC, arg0->unk0, arg0->unkC);
    animateCameraRotationY((CutsceneCameraState *)arg1->unkC, arg0->unk2, arg0->unkC);
    animateCameraPositionX((CutsceneCameraState *)arg1->unkC, arg0->unk10, arg0->unkC);
    animateCameraPositionY((CutsceneCameraState *)arg1->unkC, arg0->unk4, arg0->unkC);
    animateCameraPositionZ((CutsceneCameraState *)arg1->unkC, arg0->unk8, arg0->unkC);
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
    initCameraShake((CutsceneCameraShakeState *)arg1->unkC, arg0->unk0 * 0xA3D7, arg0->unk2);
}

void cutsceneCameraRotate_init(void) {
}

s32 cutsceneCameraRotate_validate(void) {
    return 0;
}

void cutsceneCameraRotate_exec(cutsceneCameraRotate_exec_arg0 *arg0, cutsceneCameraRotate_exec_arg1 *arg1) {
    animateCameraRotationYContinuous((CutsceneCameraState *)arg1->unkC, arg0->unk0, arg0->unk2);
}

s32 cutsceneCameraRotate_isDone(void) {
    return 0;
}
