#include "common.h"

typedef struct {
    s16 step;
    s16 duration;
} cutsceneCameraStepDuration_exec_arg0;
typedef struct {
    u8 padding[0xC];
    s32 cameraState;
} cutsceneCameraStepDuration_exec_arg1;

typedef struct {
    s16 rotX;
    s16 rotY;
    s32 posY;
    s32 posZ;
    s32 posX;
} cutsceneCameraReset_exec_arg0;
typedef struct {
    u8 padding[0xC];
    s32 cameraState;
} cutsceneCameraReset_exec_arg1;
typedef struct {
    s16 targetRotX;
    s16 targetRotY;
    s32 targetY;
    s32 targetZ;
    s16 duration;
    s32 targetX;
} cutsceneCameraMove_exec_arg0;
typedef struct {
    u8 padding[0xC];
    s32 cameraState;
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
void cutsceneCameraShake_exec(cutsceneCameraStepDuration_exec_arg0 *arg0, cutsceneCameraStepDuration_exec_arg1 *arg1);
void cutsceneCameraRotate_init(void);
s32 cutsceneCameraRotate_validate(void);
void cutsceneCameraRotate_exec(cutsceneCameraStepDuration_exec_arg0 *arg0, cutsceneCameraStepDuration_exec_arg1 *arg1);
s32 cutsceneCameraRotate_isDone(void);