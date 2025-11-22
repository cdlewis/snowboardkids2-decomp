#include "cutscene/cutscene_camera.h"

extern void func_800B5F2C_1E2FDC(s32, s16, s16);
extern void func_800B5FB0_1E3060(s32, s16, s16);
extern void func_800B6034_1E30E4(s32, s32, s16);
extern void func_800B6088_1E3138(s32, s32, s16);
extern void func_800B60DC_1E318C(s32, s32, s16);
extern void func_800B5EC4_1E2F74(s32, s16, s16, s32, s32, s32);

void cutsceneCameraReset_init(void) {
}

s32 cutsceneCameraReset_validate(void) {
    return 0;
}

void cutsceneCameraReset_exec(cutsceneCameraReset_exec_arg0 *arg0, cutsceneCameraReset_exec_arg1 *arg1) {
    func_800B5EC4_1E2F74(arg1->unkC, arg0->unk0, arg0->unk2, arg0->unk4, arg0->unk8, arg0->unkC);
}

void cutsceneCameraMove_init(void) {
}

s32 cutsceneCameraMove_validate(void) {
    return 0;
}

void cutsceneCameraMove_exec(cutsceneCameraMove_exec_arg0 *arg0, cutsceneCameraMove_exec_arg1 *arg1) {
    func_800B5F2C_1E2FDC(arg1->unkC, arg0->unk0, arg0->unkC);
    func_800B5FB0_1E3060(arg1->unkC, arg0->unk2, arg0->unkC);
    func_800B6034_1E30E4(arg1->unkC, arg0->unk10, arg0->unkC);
    func_800B6088_1E3138(arg1->unkC, arg0->unk4, arg0->unkC);
    func_800B60DC_1E318C(arg1->unkC, arg0->unk8, arg0->unkC);
}

s32 cutsceneCameraMove_isDone(void) {
    return 0;
}

void cutsceneCameraShake_init(void) {
}

s32 cutsceneCameraShake_validate(void) {
    return 0;
}

typedef struct {
    u8 _pad[0x68];
    s32 unk68;
    s16 unk6C;
    u8 _pad2[2];
    u8 unk70;
} func_800B6180_1E3230_arg0;

extern void func_800B6180_1E3230(func_800B6180_1E3230_arg0 *, s32, s16);

void cutsceneCameraShake_exec(cutsceneCameraRotate_exec_arg0 *arg0, cutsceneCameraRotate_exec_arg1 *arg1) {
    func_800B6180_1E3230((func_800B6180_1E3230_arg0 *)arg1->unkC, arg0->unk0 * 0xA3D7, arg0->unk2);
}

void cutsceneCameraRotate_init(void) {
}

s32 cutsceneCameraRotate_validate(void) {
    return 0;
}

void func_800B6130_1E31E0(s32, s16, s16);

void cutsceneCameraRotate_exec(cutsceneCameraRotate_exec_arg0 *arg0, cutsceneCameraRotate_exec_arg1 *arg1) {
    func_800B6130_1E31E0(arg1->unkC, arg0->unk0, arg0->unk2);
}

s32 cutsceneCameraRotate_isDone(void) {
    return 0;
}