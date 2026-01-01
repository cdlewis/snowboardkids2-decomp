#include "36BE0.h"
#include "common.h"
#include "task_scheduler.h"

extern float __cosf(float);

#define FIXED_POINT_SCALE 8192.0f
#define FOV_HALF_ANGLE 0.43633232f
#define SCREEN_CENTER_X 160
#define SCREEN_CENTER_Y 120

typedef struct {
    u8 pad[0x120];
    s16 m00;
    s16 m01;
    s16 m02;
    s16 m10;
    s16 m11;
    s16 m12;
    s16 m20;
    s16 m21;
    s16 m22;
    s16 pad132;
    s32 camX;
    s32 camY;
    s32 camZ;
} CameraTransform;

void worldToScreenCoords(s32 *outX, s32 *outY, Vec3i *worldPos) {
    CameraTransform *cam;
    f32 m00, m01, m02, m10, m11, m12, m20, m21, m22;
    f32 relX, relY, relZ;
    f32 viewX, viewY, viewZ;
    f32 fovAngle;
    s32 iviewX, iviewY;
    f32 fviewZ, perspectiveScale;
    s32 posY;
    f32 screenY;
    f32 normalizedCoord;
    s32 posZ;

    cam = (CameraTransform *)getCurrentAllocation();
    m00 = cam->m00 / FIXED_POINT_SCALE;
    m01 = cam->m01 / FIXED_POINT_SCALE;
    m02 = cam->m02 / FIXED_POINT_SCALE;
    m10 = cam->m10 / FIXED_POINT_SCALE;

    m11 = cam->m11 / FIXED_POINT_SCALE;
    m12 = cam->m12 / FIXED_POINT_SCALE;
    m20 = cam->m20 / FIXED_POINT_SCALE;
    m22 = cam->m22 / FIXED_POINT_SCALE;

    posY = worldPos->y;
    posZ = worldPos->z;
    posZ = posZ - cam->camZ;
    relX = worldPos->x - cam->camX;
    relY = posY - cam->camY;
    relZ = posZ;

    viewX = relX * m00 + relY * m01 + relZ * m02;
    viewY = relX * m10 + relY * m11 + relZ * m12;

    m21 = cam->m21 / FIXED_POINT_SCALE;

    viewZ = relX * m20 + relY * m21 + relZ * m22;

    iviewX = viewX;
    iviewY = viewY;

    fovAngle = FOV_HALF_ANGLE;
    fviewZ = (s32)viewZ;

    perspectiveScale = fviewZ;
    perspectiveScale = perspectiveScale * sinf(fovAngle);
    perspectiveScale = perspectiveScale / __cosf(fovAngle);
    normalizedCoord = -iviewY / perspectiveScale;

    screenY = normalizedCoord * SCREEN_CENTER_Y;
    *outY = -(s32)screenY + SCREEN_CENTER_Y;
    normalizedCoord = iviewX / perspectiveScale;
    *outX = -(s32)(normalizedCoord * SCREEN_CENTER_X) + SCREEN_CENTER_X;
}