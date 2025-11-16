#include "common.h"

typedef struct {
    s16 unk0;
    s16 unk2;
} cutsceneCameraRotate_exec_arg0;
typedef struct {
    u8 padding[0xC];
    s32 unkC;
} cutsceneCameraRotate_exec_arg1;

typedef struct {
    s16 unk0;
    s16 unk2;
    s32 unk4;
    s32 unk8;
    s32 unkC;
} cutsceneCameraReset_exec_arg0;
typedef struct {
    u8 padding[0xC];
    s32 unkC;
} cutsceneCameraReset_exec_arg1;
typedef struct {
    s16 unk0;
    s16 unk2;
    s32 unk4;
    s32 unk8;
    s16 unkC;
    s32 unk10;
} cutsceneCameraMove_exec_arg0;
typedef struct {
    u8 padding[0xC];
    s32 unkC;
} cutsceneCameraMove_exec_arg1;

void cutsceneCameraReset_init(void);
s32 cutsceneCameraReset_validate(void);
void cutsceneCameraReset_exec(cutsceneCameraReset_exec_arg0 *arg0, cutsceneCameraReset_exec_arg1 *arg1);
void cutsceneCameraMove_init(void);
s32 cutsceneCameraMove_validate(void);
void cutsceneCameraMove_exec(cutsceneCameraMove_exec_arg0 *arg0, cutsceneCameraMove_exec_arg1 *arg1);
s32 cutsceneCameraMove_isDone(void);
void cutsceneCameraShake_init(void);
s32 cutsceneCameraShake_validate(void);
void cutsceneCameraShake_exec(cutsceneCameraRotate_exec_arg0 *arg0, cutsceneCameraRotate_exec_arg1 *arg1);
void cutsceneCameraRotate_init(void);
s32 cutsceneCameraRotate_validate(void);
void cutsceneCameraRotate_exec(cutsceneCameraRotate_exec_arg0 *arg0, cutsceneCameraRotate_exec_arg1 *arg1);
s32 cutsceneCameraRotate_isDone(void);