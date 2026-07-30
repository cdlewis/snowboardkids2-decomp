#include "credits/credits_controller.h"

#include "animation/easing_state.h"
#include "assets.h"
#include "audio/audio.h"
#include "common.h"
#include "credits/credits_decorations.h"
#include "credits/credits_subtitles.h"
#include "credits/credits_text.h"
#include "graphics/graphics.h"
#include "math/geometry.h"
#include "system/rom_loader.h"
#include "system/task_scheduler.h"
#include "text/font_render.h"
#include "ui/level_preview_3d.h"

USE_OVERLAY(credits)

typedef struct {
    void *start;
    void *end;
    s32 size;
} asset;

typedef struct {
    s16 modelId;
    s16 animationIndex;
    u8 unk4;
    s8 actionMode;
    u16 rotation;
    u16 unk8;
    s16 scale;
    s32 depthOffset;
} CreditsCharacterConfig; // size 0x10

typedef struct {
    SceneModel *model;
    s16 animState;
    s16 configIndex;
    u8 unk8;
    s8 direction;
    s16 isCleanedUp;
} CreditsCharacter;

static asset D_8008BFA0_8CBA0[6] = {
    { &CREDITS_TEXT_PALETTE_DATA_00_ROM_START, &CREDITS_TEXT_PALETTE_DATA_00_ROM_END, 0x9578 },
    { &CREDITS_TEXT_PALETTE_DATA_01_ROM_START, &CREDITS_TEXT_PALETTE_DATA_01_ROM_END, 0x9578 },
    { &CREDITS_TEXT_PALETTE_DATA_02_ROM_START, &CREDITS_TEXT_PALETTE_DATA_02_ROM_END, 0x9578 },
    { &CREDITS_TEXT_PALETTE_DATA_03_ROM_START, &CREDITS_TEXT_PALETTE_DATA_03_ROM_END, 0x9578 },
    { &CREDITS_TEXT_PALETTE_DATA_04_ROM_START, &CREDITS_TEXT_PALETTE_DATA_04_ROM_END, 0x9578 },
    { &CREDITS_TEXT_PALETTE_DATA_05_ROM_START, &CREDITS_TEXT_PALETTE_DATA_05_ROM_END, 0x9578 },
};

s32 padding[2] = { 0, 0 };

s32 creditsCharacterStartDepth = 0xB00000;

s32 creditsCharacterScrollSpeed = 0xB000;

s32 creditsCameraX = 0x0;

s32 creditsCameraY = 0x600333;

struct {
    s32 depth;
    s16 unk4;
    s16 pitch;
    s16 unk8;
    s16 yaw;
} creditsCameraConfig = {
    0x1400000,
    0,
    0,
    0,
};

CreditsCharacterConfig creditsCharacterConfigs[] = {
    { 0x64, 0,    0, 0, 0xF800, 0, 0x2000, 0          },
    { 0x65, 1,    0, 0, 0xF800, 0, 0x2000, 0          },
    { 0x71, 0xD,  0, 1, 0xF800, 0, 0x0CCC, 0          },
    { 0x66, 2,    0, 0, 0xF800, 0, 0x2000, 0          },
    { 0x67, 3,    0, 0, 0xF800, 0, 0x2000, 0          },
    { 0x74, 0x10, 0, 1, 0,      0, 0x2000, 0          },
    { 0x68, 4,    0, 0, 0xF800, 0, 0x2000, 0          },
    { 0x69, 5,    0, 0, 0xF800, 0, 0x2000, 0          },
    { 0x70, 0xC,  0, 0, 0xF800, 0, 0x2000, 0          },
    { 0x6A, 6,    0, 0, 0xF800, 0, 0x2000, 0          },
    { 0x6B, 7,    0, 0, 0xF800, 0, 0x2000, 0          },
    { 0x6C, 8,    0, 0, 0xF800, 0, 0x2000, 0          },
    { 0x73, 0xF,  0, 4, 0xF800, 0, 0x0CCC, 0x00100000 },
    { 0x6E, 0xA,  0, 0, 0xF800, 0, 0x0E66, 0          },
    { 0x6F, 0xB,  0, 0, 0xF800, 0, 0x0B33, 0          },
    { 0x6D, 9,    0, 0, 0xF800, 0, 0x0999, 0          },
    { 0x72, 0xE,  0, 0, 0xF800, 0, 0x2000, 0x00199999 }
};

struct {
    u16 unk0;
    u16 unk2;
} D_8008C11C_8CD1C = { 0x11, 0 };

Vec3i D_8008C120_8CD20[] = {
    { 0x001428F5, 0x00099999, 0x00266666 },
    { 0xFFEBD70B, 0x00099999, 0x00266666 },
    { 0x001428F5, 0x00099999, 0xFFE2E148 },
    { 0xFFEBD70B, 0x00099999, 0xFFE2E148 }
};

void updateCreditsSequence(void);
void fadeOutCreditsSequence(void);
void onCreditsComplete(void);
void updateCreditsCharacter(CreditsCharacter *character);
void cleanupCreditsCharacter(CreditsCharacter *character);
void spawnCreditsCharacter(CreditsState *);
void initCreditsCharacter(CreditsCharacter *character);

void initSceneLighting(CreditsState *arg0) {
    arg0->sceneLights[0].r2 = 0;
    arg0->sceneLights[0].g2 = 0x7F;
    arg0->sceneLights[0].r = 0xE0;
    arg0->sceneLights[0].b = 0xA0;
    arg0->sceneLights[0].g = 0xE0;
    arg0->sceneLights[1].r = 0x32;
    arg0->sceneLights[1].g = 0x32;
    arg0->sceneLights[1].b = 0x32;
    arg0->sceneLights[2].r2 = -0x7F;
    arg0->sceneLights[2].r = 0x65;
    arg0->sceneLights[2].g = 0x65;
    arg0->sceneLights[2].b = 0x65;
    arg0->ambientLightR = 0x80;
    arg0->ambientLightB = 0xA0;
    arg0->sceneLights[0].b2 = 0x7F;
    arg0->sceneLights[1].r2 = 0x7F;
    arg0->sceneLights[1].g2 = 0x7F;
    arg0->sceneLights[1].b2 = 0;
    arg0->sceneLights[2].g2 = 0x7F;
    arg0->sceneLights[2].b2 = 0;
    arg0->ambientLightG = 0x90;

    setViewportLightColors(
        arg0->characterViewport.viewportId,
        3,
        &arg0->sceneLights[0],
        (ColorData *)&arg0->ambientLightR
    );
}

void initCreditsController(void) {
    s32 buffer[8];
    s32 i;
    CreditsState *taskMemory = (CreditsState *)allocateTaskMemory(0xE68);

    LOAD_OVERLAY(credits);

    setupTaskSchedulerNodes(0x40, 0, 0, 0, 0, 0, 0, 0);

    taskMemory->sceneOriginX = -0x90;
    taskMemory->sceneOriginY = 0x68;
    taskMemory->nextCharacterConfigIndex = 0;
    taskMemory->characterLaneIndex = 0;
    taskMemory->initialized = 0;
    taskMemory->frameCounter = 0;
    taskMemory->reserved944 = 0;
    taskMemory->scratchBuffer = &taskMemory->scratchBufferStorage;
    taskMemory->scratchBufferStorage = 0;
    taskMemory->textRenderAsset = loadTextRenderAsset(1);
    taskMemory->subtitleTextTable = loadDmaAsset(1);

    taskMemory->creditsTextTable = loadDmaAsset(3);
    taskMemory->cornerDecorationAsset =
        loadCompressedData(&cornerDecorationAsset_ROM_START, &cornerDecorationAsset_ROM_END, 0x2448);
    taskMemory->subtitleSchedule =
        loadCompressedData(&CREDITS_SUBTITLE_DATA_ROM_START, &CREDITS_SUBTITLE_DATA_ROM_END, 0x2B0);

    for (i = 0; i < 6; i++) {
        taskMemory->paletteDataTables[i] =
            loadCompressedData(D_8008BFA0_8CBA0[i].start, D_8008BFA0_8CBA0[i].end, D_8008BFA0_8CBA0[i].size);
    }

    initViewportNode(&taskMemory->backgroundViewport, 0, 0, 0xB, 0);
    setViewportScale(&taskMemory->backgroundViewport, 1.0f, 1.0f);
    setModelCameraTransform(&taskMemory->backgroundViewport, 0, 0, -0xA0, -0x78, 0x9F, 0x77);
    setViewportId(&taskMemory->backgroundViewport, 1);
    initViewportNode(&taskMemory->textViewport, 0, 1, 0xC, 0);
    setViewportScale(&taskMemory->textViewport, 1.0f, 1.0f);
    setViewportId(&taskMemory->textViewport, 1);
    setModelCameraTransform(&taskMemory->textViewport, 0, 0, -0xA0, -0x78, 0x9F, 0x77);
    initViewportNode(&taskMemory->subtitleShadowViewport, 0, 2, 0xB, 0);
    setViewportScale(&taskMemory->subtitleShadowViewport, 1.0f, 1.0f);
    setViewportId(&taskMemory->subtitleShadowViewport, 1);
    setModelCameraTransform(&taskMemory->subtitleShadowViewport, 0, 0, -0xA0, -0x78, 0x9F, 0x77);
    initViewportNode(&taskMemory->subtitleViewport, 0, 3, 0xB, 0);
    setViewportScale(&taskMemory->subtitleViewport, 1.0f, 1.0f);
    setModelCameraTransform(&taskMemory->subtitleViewport, 0, 0, -0xA0, -0x78, 0x9F, 0x77);
    setViewportId(&taskMemory->subtitleViewport, 1);
    initViewportNode(&taskMemory->characterViewport, 0, 4, 0xD, 1);
    setViewportScale(&taskMemory->characterViewport, 1.0f, 1.0f);
    setModelCameraTransform(&taskMemory->characterViewport, 0, 0, -0xA0, -0x78, 0x9F, 0x77);
    setViewportId(&taskMemory->characterViewport, 1);
    setViewportPerspective(&taskMemory->characterViewport, 40.0f, 1.3333334f, 10.0f, 10000.0f);
    createViewportTransform(buffer, 0, 0, 0x01400000, 0, 0, 0);
    setViewportTransformById(taskMemory->characterViewport.viewportId, buffer);
    setViewportEnvColor(&taskMemory->backgroundViewport, 0, 0, 0);
    setViewportFadeValue(&taskMemory->backgroundViewport, 0, 0);
    setViewportFadeValue(&taskMemory->textViewport, 0, 0);
    setViewportFadeValue(&taskMemory->subtitleShadowViewport, 0, 0);
    setViewportFadeValue(&taskMemory->subtitleViewport, 0, 0);
    setViewportFadeValue(&taskMemory->characterViewport, 0, 0);
    playMusicTrackWithFadeIn(0xB, 0x80, 0);
    initSceneLighting(taskMemory);
    setGameStateHandler(&updateCreditsSequence);
}

void updateCreditsSequence(void) {
    CreditsState *state;

    state = (CreditsState *)getCurrentAllocation();

    if (state->frameCounter == 0x1C98) {
        state->frameCounter = 0x1C98;
        setMusicFadeOut(4);
        setViewportFadeValue(&state->backgroundViewport, 0xFF, 0x1E);
        setViewportFadeValue(&state->textViewport, 0xFF, 0x1E);
        setViewportFadeValue(&state->subtitleShadowViewport, 0xFF, 0x1E);
        setViewportFadeValue(&state->subtitleViewport, 0xFF, 0x1E);
        setViewportFadeValue(&state->characterViewport, 0xFF, 0x1E);
        setGameStateHandler(fadeOutCreditsSequence);
    }

    if (state->initialized == 0) {
        initCreditsSubtitles(state);
        initCreditsScrollingTextEffects(state);
        initCreditsCornerDecorationSprites(state);
        state->initialized = 1;
    }

    updateCreditsScrollingTextEffects(state);
    updateCreditsCornerDecorationSprites(state);
    spawnCreditsCharacter(state);

    state->frameCounter = (u16)state->frameCounter + 1;
}

void fadeOutCreditsSequence(void) {
    CreditsState *state;
    s32 i;

    state = (CreditsState *)getCurrentAllocation();

    if (state->frameCounter == 0x1CB8) {
        unlinkNode(&state->characterViewport);
        unlinkNode(&state->subtitleViewport);
        unlinkNode(&state->subtitleShadowViewport);
        unlinkNode(&state->textViewport);
        unlinkNode(&state->backgroundViewport);
        state->subtitleSchedule = freeNodeMemory(state->subtitleSchedule);
        state->creditsTextTable = freeNodeMemory(state->creditsTextTable);
        state->subtitleTextTable = freeNodeMemory(state->subtitleTextTable);
        state->textRenderAsset = freeNodeMemory(state->textRenderAsset);
        state->cornerDecorationAsset = freeNodeMemory(state->cornerDecorationAsset);
        for (i = 0; i < 6; i++) {
            state->paletteDataTables[i] = freeNodeMemory(state->paletteDataTables[i]);
        }
        terminateSchedulerWithCallback(onCreditsComplete);
    } else {
        updateCreditsScrollingTextEffects(state);
        updateCreditsCornerDecorationSprites(state);
        spawnCreditsCharacter(state);
        state->frameCounter = (u16)state->frameCounter + 1;
    }
}

void onCreditsComplete(void) {
    getCurrentAllocation();
    returnToParentScheduler(1);
}

void spawnCreditsCharacter(CreditsState *arg0) {
    u16 temp_a0;
    s32 temp_a0_2;
    s32 temp_v1;
    s32 var_v0;
    CreditsCharacter *temp_v0;

    temp_a0 = arg0->frameCounter;
    if ((u32)(temp_a0 - 0x12C) >= 0x1717U) {
        arg0->nextCharacterSpawnFrame = 0x12C;
        return;
    }
    if ((s16)temp_a0 == arg0->nextCharacterSpawnFrame) {
        if (arg0->nextCharacterConfigIndex >= (s16)D_8008C11C_8CD1C.unk0) {
            temp_a0_2 = arg0->characterLaneIndex;
            arg0->nextCharacterConfigIndex = 0;
            temp_v1 = temp_a0_2 + 1;
            var_v0 = temp_v1;
            if (temp_v1 < 0) {
                var_v0 = temp_a0_2 + 4;
            }
            arg0->characterLaneIndex = temp_v1 - ((var_v0 >> 2) * 4);
        }
        temp_v0 = scheduleTask(initCreditsCharacter, 0, 0, 0);
        if (temp_v0 != NULL) {
            temp_v0->configIndex = arg0->nextCharacterConfigIndex;
            *(s16 *)&temp_v0->unk8 = arg0->characterLaneIndex;
        }
        arg0->nextCharacterConfigIndex = arg0->nextCharacterConfigIndex + 1;
        arg0->nextCharacterSpawnFrame = arg0->frameCounter + 0x3D;
    }
}

void initCreditsCharacter(CreditsCharacter *character) {
    s32 cameraTransform[8];
    CreditsState *creditsState;
    CreditsCharacterConfig *config;
    s16 scale;

    creditsState = getCurrentAllocation();
    config = &creditsCharacterConfigs[character->configIndex];
    character->isCleanedUp = 0;
    character->animState = 0;

    character->model = allocateNodeMemory(0x160);

    initializeGameEntity(
        character->model,
        config->modelId,
        &creditsState->characterViewport,
        character->direction,
        -1,
        -1,
        -1
    );

    memcpy((void *)((u8 *)character->model + 0x18), &identityMatrix, sizeof(Transform3D));

    createYRotationMatrix((Transform3D *)((u8 *)character->model + 0x18), config->rotation);

    scale = config->scale;
    scaleMatrix((Transform3D *)((u8 *)character->model + 0x18), scale, scale, scale);

    character->model->matrix18.translation.x = creditsCharacterStartDepth;
    character->model->matrix18.translation.y += config->depthOffset;

    setModelActionMode(character->model, config->actionMode);

    {
        s32 camX = creditsCameraX;
        s32 camY = creditsCameraY;
        s32 camDepth = creditsCameraConfig.depth;
        createViewportTransform(
            cameraTransform,
            camX,
            camY,
            camDepth,
            0,
            creditsCameraConfig.pitch,
            creditsCameraConfig.yaw
        );
    }

    setViewportTransformById(creditsState->characterViewport.viewportId, cameraTransform);

    setCleanupCallback(cleanupCreditsCharacter);
    setCallback(updateCreditsCharacter);
}

void updateCreditsCharacter(CreditsCharacter *character) {
    CreditsCharacterConfig *config;

    getCurrentAllocation();

    config = &creditsCharacterConfigs[character->configIndex];

    switch (character->animState) {
        case 0:
            setModelAnimation(character->model, config->animationIndex);
            character->animState = 1;
            break;
        case 1:
            if (config->modelId == 0x73) {
                setModelActionMode(character->model, 3);
                character->animState = 2;
            }
            clearModelRotation(character->model);
            break;
        case 2:
            if (config->modelId == 0x73) {
                setModelActionMode(character->model, 8);
            }
            clearModelRotation(character->model);
            break;
        default:
            break;
    }

    character->model->matrix18.translation.x -= creditsCharacterScrollSpeed;

    if (character->model->matrix18.translation.x < -creditsCharacterStartDepth) {
        cleanupSceneModel(character->model);
        character->isCleanedUp = 1;
        terminateCurrentTask();
    } else {
        updateModelGeometry(character->model);
    }
}

void cleanupCreditsCharacter(CreditsCharacter *character) {
    getCurrentAllocation();
    if (character->isCleanedUp == 0) {
        cleanupSceneModel(character->model);
    }
    character->model = freeNodeMemory(character->model);
}
