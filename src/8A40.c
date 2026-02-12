#include "20F0.h"
#include "common.h"
#include "geometry.h"
#include "task_scheduler.h"

typedef struct {
    func_80002B50_3750_arg *model;
    DisplayListObject displayObject;
    u16 rotationAngle;
    s16 tiltAngle;
    s8 bobbingEnabled;
    u8 _pad45[3];
    s32 verticalOffset;
    s32 verticalVelocity;
} BobbingModelState;

u32 D_80088130[] = {
    0x00000000,
    0x01000180,
    0x00000000,
    0x00000000,
};

u32 D_80088140[] = {
    0x00000000,
    0x010003B0,
    0x010003C0,
    0x00000000,
};

u32 D_80088150[] = {
    0x00000000,
    0x01000480,
    0x01000490,
    0x00000000,
};

u32 D_80088160[] = {
    0x00000000,
    0x01000180,
    0x00000000,
    0x00000000,
};

u32 D_80088170[] = {
    0x00000000,
    0x010002E0,
    0x010002F0,
    0x00000000,
};

u32 D_80088180[] = {
    0x00000000,
    0x01000488,
    0x01000498,
    0x00000000,
};

u32 D_80088190[] = {
    0x00000000,
    0x010005A0,
    0x010005B0,
    0x00000000,
};

u32 D_800881A0[] = {
    0x00000000,
    0x01000268,
    0x00000000,
    0x00000000,
};

u32 D_800881B0[] = {
    0x00000000,
    0x01000560,
    0x01000570,
    0x00000000,
};

u32 D_800881C0[] = {
    0x00000000, 0x010007C0, 0x00000000, 0x00000000, 0x00000000, 0x01000838, 0x00000000, 0x00000000,
};

u32 D_800881E0[] = {
    0x00000000,
    0x01000210,
    0x00000000,
    0x00000000,
};

u32 D_800881F0[] = {
    0x00000000, 0x01000000, 0x00000000, 0x00000000, 0x00000000, 0x010002A8, 0x00000000, 0x00000000,
    0x00000000, 0x010004F8, 0x00000000, 0x00000000, 0x00000000, 0x010008B8, 0x00000000, 0x00000000,
    0x00000000, 0x01000B10, 0x00000000, 0x00000000, 0x00000000, 0x01000ED0, 0x01001320, 0x00000000,
    0x00000000, 0x01001408, 0x00000000, 0x00000000, 0x00000000, 0x01001540, 0x00000000, 0x00000000,
    0x00000000, 0x01001728, 0x00000000, 0x00000000, 0x00000000, 0x01001860, 0x00000000, 0x00000000,
};

u32 D_80088290[] = {
    0x00000000,
    0x01000180,
    0x00000000,
    0x00000000,
};

u32 D_800882A0[] = {
    0x00000000,
    0x01000760,
    0x00000000,
    0x00000000,
};

u32 D_800882B0_88EB0[] = {
    0x00000000,
    0x00000000,
    0x01000888,
    0x00000000,
};

u32 D_800882C0[] = {
    0x00000000,
    0x01000270,
    0x00000000,
    0x00000000,
};

u32 D_800882D0[] = {
    0x00000000,
    0x01000140,
    0x00000000,
    0x00000000,
};

u32 D_800882E0[] = {
    0x00000000,
    0x01000140,
    0x00000000,
    0x00000000,
};

u32 D_800882F0[] = {
    0x00000000,
    0x01000140,
    0x00000000,
    0x00000000,
};

u32 D_80088300[] = {
    0x00000000,
    0x01000140,
    0x00000000,
    0x00000000,
};

u32 D_80088310[] = {
    0x00000000,
    0x010001A8,
    0x00000000,
    0x00000000,
};

u32 D_80088320[] = {
    0x00000000,
    0x010001E0,
    0x00000000,
    0x00000000,
};

u32 D_80088330[] = {
    0x00000000,
    0x01000238,
    0x01000248,
    0x00000000,
};

u32 D_80088340[] = {
    0x00000000,
    0x010002A8,
    0x010002B8,
    0x00000000,
};

u32 D_80088350[] = {
    0x00000000,
    0x01000398,
    0x010003A8,
    0x00000000,
};

u32 D_80088360[] = {
    0x00000000,
    0x01000418,
    0x01000428,
    0x00000000,
};

u32 D_80088370[] = {
    0x00000000, 0x010001C8, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01000000, 0x00000000, 0x00000000, 0x00000000, 0x010001E8,
};

u32 D_800883A0[] = {
    0x00000000, 0x00000000, 0x00000000, 0x01000000, 0x00000000, 0x00000000,
    0x00000000, 0x010001E0, 0x00000000, 0x00000000, 0x00000000, 0x010003C0,
};

u32 D_800883D0[] = {
    0x00000000, 0x01000000, 0x00000000, 0x00000000, 0x00000000, 0x010001E8, 0x00000000, 0x00000000,
};

u32 D_800883F0[] = {
    0x00000000,
    0x01001A40,
    0x01001A50,
    0x00000000,
};

u32 D_80088400[] = {
    0x00000000,
    0x00000000,
    0x01001B40,
    0x00000000,
};

u32 D_80088410[] = {
    0x00000000,
    0x00000000,
    0x01001CA0,
    0x00000000,
};

u32 D_80088420[] = {
    0x00000000,
    0x00000000,
    0x01001D90,
    0x00000000,
};

u32 D_80088430[] = {
    0x00000000,
    0x00000000,
    0x01001E80,
    0x00000000,
};

u32 D_80088440[] = {
    0x00000000,
    0x00000000,
    0x01001F70,
    0x00000000,
};

u32 D_80088450[] = {
    0x00000000, 0x01002248, 0x00000000, 0x00000000, 0x00000000, 0x010020D8, 0x01002130, 0x00000000,
};

u32 D_80088470[] = {
    0x00000000,
    0x00000000,
    0x01002268,
    0x00000000,
};

u32 D_80088480[] = {
    0x00000000,
    0x00000000,
    0x010023C8,
    0x00000000,
};

u32 D_80088490[] = {
    0x00000000,
    0x00000000,
    0x010024B8,
    0x00000000,
};

u32 D_800884A0[] = {
    0x00000000,
    0x00000000,
    0x010025A8,
    0x00000000,
};

u32 D_800884B0[] = {
    0x00000000, 0x00000000, 0x01002698, 0x00000000, 0x00000000, 0x01002968, 0x00000000, 0x00000000,
};

u32 D_800884D0[] = {
    0x00000000,
    0x01000658,
    0x01000668,
    0x00000000,
};

u32 D_800884E0[] = {
    0x00000000,
    0x010008F8,
    0x00000000,
    0x00000000,
};

u32 D_800884F0[] = {
    0x00000000,
    0x01000978,
    0x00000000,
    0x00000000,
};

u32 D_80088500[] = {
    0x00000000,
    0x01000A10,
    0x00000000,
    0x00000000,
};

u32 D_80088510[] = {
    0x00000000,
    0x00000000,
    0x00000000,
    0x01000B30,
};

u32 D_80088520[] = {
    0x00000000,
    0x01000890,
    0x010008A0,
    0x00000000,
};

u32 D_80088530[] = {
    0x00000000,
    0x01000C38,
    0x00000000,
    0x00000000,
};

u32 D_80088540[] = {
    0x00000000,
    0x01000CB8,
    0x00000000,
    0x00000000,
};

u32 D_80088550[] = {
    0x00000000,
    0x01000D50,
    0x00000000,
    0x00000000,
};

u32 D_80088560[] = {
    0x00000000,
    0x01000910,
    0x01000920,
    0x00000000,
};

u32 D_80088570[] = {
    0x00000000,
    0x01000D18,
    0x00000000,
    0x00000000,
};

u32 D_80088580[] = {
    0x00000000,
    0x01000DD0,
    0x01000DE0,
    0x00000000,
};

u32 D_80088590[] = {
    0x00000000,
    0x010010A0,
    0x00000000,
    0x00000000,
};

u32 D_800885A0[] = {
    0x00000000,
    0x01000D10,
    0x01000D20,
    0x00000000,
};

u32 D_800885B0[] = {
    0x00000000,
    0x01000F50,
    0x00000000,
    0x00000000,
};

u32 D_800885C0[] = {
    0x00000001,
    0x010004E8,
    0x00000000,
    0x00000000,
};

void updateBobbingModelTask(BobbingModelState *);

void cleanupBobbingModelTask(BobbingModelTaskState *);

void initBobbingModelTask(BobbingModelTaskState *state) {
    memcpy(state->transformMatrix, &identityMatrix, 0x20);
    state->displayList = loadAssetGroupDisplayList(state->model);
    state->vertexData = loadAssetGroupCompressedData(state->model);
    state->materialData = &D_800882B0_88EB0;
    state->renderFlags = 0;
    state->bobbingEnabled = 0;
    state->verticalOffset = 0;
    state->verticalVelocity = 0xA3D7;
    setCleanupCallback(&cleanupBobbingModelTask);
    setCallback(&updateBobbingModelTask);
}

void updateBobbingModelTask(BobbingModelState *state) {
    Transform3D rotationMatrix;
    Transform3D combinedMatrix;
    s32 delta;
    s32 temp;

    if (state->model->isDestroyed == 1) {
        terminateCurrentTask();
        return;
    }

    switch (state->model->actionMode) {
        case 0:
        default:
            state->tiltAngle = 0;
            break;
        case 1:
            delta = -state->tiltAngle / 12;
            if (delta == 0) {
                state->tiltAngle = 0;
            } else {
                state->tiltAngle += delta;
            }
            break;
        case 2:
            delta = (0x1C7 - state->tiltAngle) / 12;
            if (delta == 0) {
                state->tiltAngle = 0x1C7;
            } else {
                state->tiltAngle += delta;
            }
            break;
        case 3:
            state->bobbingEnabled = 1;
            break;
        case 4:
            state->bobbingEnabled = 0;
            state->verticalVelocity = 0xA3D7;
            break;
    }

    if (state->bobbingEnabled != 0) {
        state->verticalOffset = state->verticalOffset + state->verticalVelocity;
        if (state->verticalOffset > 0) {
            state->verticalVelocity -= 0x1062;
        } else {
            state->verticalVelocity += 0x1062;
        }
    } else {
        temp = -state->verticalOffset;
        if (-state->verticalOffset < 0) {
            temp += 0xF;
        }
        state->verticalOffset = temp >> 4;
    }

    createXRotationMatrix(state->model->unkF0, state->tiltAngle);
    state->model->unk104 = 0;
    state->model->unk108 = state->verticalOffset;
    state->model->unk10C = 0;

    state->rotationAngle += 0x2AA;
    createYRotationMatrix(&rotationMatrix, state->rotationAngle);

    rotationMatrix.translation.x = 0;
    rotationMatrix.translation.y = 0x499999;
    rotationMatrix.translation.z = 0;

    func_8006B084_6BC84(&rotationMatrix, state->model->unkF0, &combinedMatrix);
    func_8006B084_6BC84(&combinedMatrix, state->model->matrix18, &state->displayObject);
    enqueueModelDisplayList(state->model, &state->displayObject);
}

void cleanupBobbingModelTask(BobbingModelTaskState *state) {
    state->displayList = freeNodeMemory(state->displayList);
    state->vertexData = freeNodeMemory(state->vertexData);
}

void cleanupTiltingModelTask(void);
void updateTiltingModelTask(BobbingModelTaskState *);

void initTiltingModelTask(void) {
    setCleanupCallback(cleanupTiltingModelTask);
    setCallback(updateTiltingModelTask);
}

void updateTiltingModelTask(BobbingModelTaskState *state) {
    if (state->model->isDestroyed == 1) {
        terminateCurrentTask();
        return;
    }

    memcpy(&state->model->unkF0, &identityMatrix, 0x20);

    if (state->model->actionMode == 0) {
        return;
    }
    if (state->model->actionMode == 1) {
        createXRotationMatrix(state->model->unkF0, 0xFCE4);
        state->model->unk10C += 0x66666;
    }
}

void cleanupTiltingModelTask(void) {
}
