#include "common.h"
#include "data/model_display_lists.h"
#include "graphics/displaylist.h"
#include "math/geometry.h"
#include "system/task_scheduler.h"
#include "ui/level_preview_3d.h"

typedef struct {
    func_80002B50_3750_arg *model;
    DisplayListObject displayObject;
    u16 rotationAngle;
    s16 tiltAngle;
    s8 bobbingEnabled;
    s32 verticalOffset;
    s32 verticalVelocity;
} BobbingModelState;

DisplayLists gLtbl3DisplayLists = { 0x00000000, (Gfx *)0x01000180, NULL, NULL };

DisplayLists gLtbl2DisplayLists = { 0x00000000, (Gfx *)0x010003B0, (Gfx *)0x010003C0, NULL };

DisplayLists gLindaBDisplayLists = { 0x00000000, (Gfx *)0x01000480, (Gfx *)0x01000490, NULL };

DisplayLists gLtbl1DisplayLists = { 0x00000000, (Gfx *)0x01000180, NULL, NULL };

DisplayLists gLtbl0DisplayLists = { 0x00000000, (Gfx *)0x010002E0, (Gfx *)0x010002F0, NULL };

DisplayLists gLindaPDisplayLists = { 0x00000000, (Gfx *)0x01000488, (Gfx *)0x01000498, NULL };

DisplayLists gDoorDisplayLists = { 0x00000000, (Gfx *)0x010005A0, (Gfx *)0x010005B0, NULL };

DisplayLists gBomb2DisplayLists = { 0x00000000, (Gfx *)0x01000268, NULL, NULL };

DisplayLists gOtherHauntedPrimaryDisplayLists = { 0x00000000, (Gfx *)0x01000560, (Gfx *)0x01000570, NULL };

DisplayLists gOtherHauntedSecondaryPartDisplayLists[2] = {
    { 0x00000000, (Gfx *)0x010007C0, NULL, NULL },
    { 0x00000000, (Gfx *)0x01000838, NULL, NULL }
};

DisplayLists gHauntedDisplayLists = { 0x00000000, (Gfx *)0x01000210, NULL, NULL };

DisplayLists gMroboPartDisplayLists[10] = {
    { 0x00000000, (Gfx *)0x01000000, NULL,              NULL },
    { 0x00000000, (Gfx *)0x010002A8, NULL,              NULL },
    { 0x00000000, (Gfx *)0x010004F8, NULL,              NULL },
    { 0x00000000, (Gfx *)0x010008B8, NULL,              NULL },
    { 0x00000000, (Gfx *)0x01000B10, NULL,              NULL },
    { 0x00000000, (Gfx *)0x01000ED0, (Gfx *)0x01001320, NULL },
    { 0x00000000, (Gfx *)0x01001408, NULL,              NULL },
    { 0x00000000, (Gfx *)0x01001540, NULL,              NULL },
    { 0x00000000, (Gfx *)0x01001728, NULL,              NULL },
    { 0x00000000, (Gfx *)0x01001860, NULL,              NULL }
};

DisplayLists gBurgDisplayLists = { 0x00000000, (Gfx *)0x01000180, NULL, NULL };

DisplayLists gHeliDisplayLists = { 0x00000000, (Gfx *)0x01000760, NULL, NULL };

DisplayLists gBobbingModelOverlayDisplayLists = { 0x00000000, NULL, (Gfx *)0x01000888, NULL };

DisplayLists gUsagiDisplayLists = { 0x00000000, (Gfx *)0x01000270, NULL, NULL };

DisplayLists gPbox0DisplayLists = { 0x00000000, (Gfx *)0x01000140, NULL, NULL };

DisplayLists gPbox1DisplayLists = { 0x00000000, (Gfx *)0x01000140, NULL, NULL };

DisplayLists gItem1DisplayLists = { 0x00000000, (Gfx *)0x01000140, NULL, NULL };

DisplayLists gItemDebrisDisplayLists = { 0x00000000, (Gfx *)0x01000140, NULL, NULL };

DisplayLists gDishDisplayLists = { 0x00000000, (Gfx *)0x010001A8, NULL, NULL };

DisplayLists gTutumDisplayLists = { 0x00000000, (Gfx *)0x010001E0, NULL, NULL };

DisplayLists gTableDisplayLists = { 0x00000000, (Gfx *)0x01000238, (Gfx *)0x01000248, NULL };

DisplayLists gRadioDisplayLists = { 0x00000000, (Gfx *)0x010002A8, (Gfx *)0x010002B8, NULL };

DisplayLists gPotDisplayLists = { 0x00000000, (Gfx *)0x01000398, (Gfx *)0x010003A8, NULL };

DisplayLists gCupDisplayLists = { 0x00000000, (Gfx *)0x01000418, (Gfx *)0x01000428, NULL };

DisplayLists gChairPartDisplayLists[3] = {
    { 0x00000000, (Gfx *)0x010001C8, NULL, NULL              },
    { 0x00000000, NULL,              NULL, (Gfx *)0x01000000 },
    { 0x00000000, NULL,              NULL, (Gfx *)0x010001E8 }
};

DisplayLists gSnow3PartDisplayLists[3] = {
    { 0x00000000, NULL, NULL, (Gfx *)0x01000000 },
    { 0x00000000, NULL, NULL, (Gfx *)0x010001E0 },
    { 0x00000000, NULL, NULL, (Gfx *)0x010003C0 }
};

DisplayLists gSnow2PartDisplayLists[2] = {
    { 0x00000000, (Gfx *)0x01000000, NULL, NULL },
    { 0x00000000, (Gfx *)0x010001E8, NULL, NULL }
};

DisplayLists gTownPrimaryDisplayLists = { 0x00000000, (Gfx *)0x01001A40, (Gfx *)0x01001A50, NULL };

DisplayLists gTownSwingingModel0DisplayLists = { 0x00000000, NULL, (Gfx *)0x01001B40, NULL };

DisplayLists gTownRotatingModel0DisplayLists = { 0x00000000, NULL, (Gfx *)0x01001CA0, NULL };

DisplayLists gTownSwingingModel1DisplayLists = { 0x00000000, NULL, (Gfx *)0x01001D90, NULL };

DisplayLists gTownSwingingModel2DisplayLists = { 0x00000000, NULL, (Gfx *)0x01001E80, NULL };

DisplayLists gTownSwingingModel3DisplayLists = { 0x00000000, NULL, (Gfx *)0x01001F70, NULL };

DisplayLists gTownSecondaryPartDisplayLists[2] = {
    { 0x00000000, (Gfx *)0x01002248, NULL,              NULL },
    { 0x00000000, (Gfx *)0x010020D8, (Gfx *)0x01002130, NULL }
};

DisplayLists gTownSwingingModel4DisplayLists = { 0x00000000, NULL, (Gfx *)0x01002268, NULL };

DisplayLists gTownRotatingModel1DisplayLists = { 0x00000000, NULL, (Gfx *)0x010023C8, NULL };

DisplayLists gTownSwingingModel5DisplayLists = { 0x00000000, NULL, (Gfx *)0x010024B8, NULL };

DisplayLists gTownSwingingModel6DisplayLists = { 0x00000000, NULL, (Gfx *)0x010025A8, NULL };

DisplayLists gTownSwingingModel7PartDisplayLists[2] = {
    { 0x00000000, NULL,              (Gfx *)0x01002698, NULL },
    { 0x00000000, (Gfx *)0x01002968, NULL,              NULL }
};

DisplayLists gJunglePrimaryDisplayLists = { 0x00000000, (Gfx *)0x01000658, (Gfx *)0x01000668, NULL };

DisplayLists gJungleSecondaryDisplayLists = { 0x00000000, (Gfx *)0x010008F8, NULL, NULL };

DisplayLists gJungleAnimatedModel0DisplayLists = { 0x00000000, (Gfx *)0x01000978, NULL, NULL };

DisplayLists gJungleAnimatedModel1DisplayLists = { 0x00000000, (Gfx *)0x01000A10, NULL, NULL };

DisplayLists gJungleStaticModelDisplayLists = { 0x00000000, NULL, NULL, (Gfx *)0x01000B30 };

DisplayLists gJungle2PrimaryDisplayLists = { 0x00000000, (Gfx *)0x01000890, (Gfx *)0x010008A0, NULL };

DisplayLists gJungle2SecondaryDisplayLists = { 0x00000000, (Gfx *)0x01000C38, NULL, NULL };

DisplayLists gJungle2AnimatedModel0DisplayLists = { 0x00000000, (Gfx *)0x01000CB8, NULL, NULL };

DisplayLists gJungle2AnimatedModel1DisplayLists = { 0x00000000, (Gfx *)0x01000D50, NULL, NULL };

DisplayLists gSpacePrimaryDisplayLists = { 0x00000000, (Gfx *)0x01000910, (Gfx *)0x01000920, NULL };

DisplayLists gSpaceSecondaryDisplayLists = { 0x00000000, (Gfx *)0x01000D18, NULL, NULL };

DisplayLists gYamaPrimaryDisplayLists = { 0x00000000, (Gfx *)0x01000DD0, (Gfx *)0x01000DE0, NULL };

DisplayLists gYamaSecondaryDisplayLists = { 0x00000000, (Gfx *)0x010010A0, NULL, NULL };

DisplayLists gYamaYPrimaryDisplayLists = { 0x00000000, (Gfx *)0x01000D10, (Gfx *)0x01000D20, NULL };

DisplayLists gYamaYSecondaryDisplayLists = { 0x00000000, (Gfx *)0x01000F50, NULL, NULL };

DisplayLists gUfoDisplayLists = { 0x00000001, (Gfx *)0x010004E8, NULL, NULL };

void updateBobbingModelTask(BobbingModelState *);
void cleanupBobbingModelTask(BobbingModelTaskState *);
void updateTiltingModelTask(BobbingModelTaskState *);
void cleanupTiltingModelTask(void);

void initBobbingModelTask(BobbingModelTaskState *state) {
    memcpy(state->transformMatrix, &identityMatrix, sizeof(Transform3D));
    state->displayList = loadAssetGroupDisplayList(state->model);
    state->vertexData = loadAssetGroupCompressedData(state->model);
    state->displayLists = &gBobbingModelOverlayDisplayLists;
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

    createXRotationMatrix(state->model->unkF0.m, state->tiltAngle);
    state->model->unkF0.translation.x = 0;
    state->model->unkF0.translation.y = state->verticalOffset;
    state->model->unkF0.translation.z = 0;

    state->rotationAngle += 0x2AA;
    createYRotationMatrix(&rotationMatrix, state->rotationAngle);

    rotationMatrix.translation.x = 0;
    rotationMatrix.translation.y = 0x499999;
    rotationMatrix.translation.z = 0;

    composeTransform3D(&rotationMatrix, &state->model->unkF0, &combinedMatrix);
    composeTransform3D(&combinedMatrix, &state->model->matrix18, &state->displayObject.transform);
    enqueueModelDisplayList(state->model, &state->displayObject);
}

void cleanupBobbingModelTask(BobbingModelTaskState *state) {
    state->displayList = freeNodeMemory(state->displayList);
    state->vertexData = freeNodeMemory(state->vertexData);
}

void initTiltingModelTask(void) {
    setCleanupCallback(cleanupTiltingModelTask);
    setCallback(updateTiltingModelTask);
}

void updateTiltingModelTask(BobbingModelTaskState *state) {
    if (state->model->isDestroyed == 1) {
        terminateCurrentTask();
        return;
    }

    memcpy(&state->model->unkF0, &identityMatrix, sizeof(Transform3D));

    if (state->model->actionMode == 0) {
        return;
    }
    if (state->model->actionMode == 1) {
        createXRotationMatrix(state->model->unkF0.m, 0xFCE4);
        state->model->unkF0.translation.z += 0x66666;
    }
}

void cleanupTiltingModelTask(void) {
}
