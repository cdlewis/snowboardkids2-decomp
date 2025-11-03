#include "36BE0.h"
#include "common.h"
#include "task_scheduler.h"

extern float __cosf(float);

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
    s32 tx;
    s32 ty;
    s32 tz;
} TransformData;

void func_80035FE0_36BE0(s32 *outX, s32 *outY, Vec3i *pos) {
    TransformData *data;
    f32 m00, m01, m02, m10, m11, m12, m20, m21, m22;
    f32 dx, dy, dz;
    f32 projX, projY, projZ;
    f32 angle;
    s32 iprojX, iprojY;
    f32 fprojZ, invCos;
    s32 py;
    f32 tempY;
    f32 new_var2;
    s32 new_var;

    data = (TransformData *)getCurrentAllocation();
    m00 = data->m00 / 8192.0f;
    m01 = data->m01 / 8192.0f;
    m02 = data->m02 / 8192.0f;
    m10 = data->m10 / 8192.0f;

    m11 = data->m11 / 8192.0f;
    m12 = data->m12 / 8192.0f;
    m20 = data->m20 / 8192.0f;
    m22 = data->m22 / 8192.0f;

    py = pos->y;
    new_var = pos->z;
    new_var = new_var - data->tz;
    dx = pos->x - data->tx;
    dy = py - data->ty;
    dz = new_var;

    projX = dx * m00 + dy * m01 + dz * m02;
    projY = dx * m10 + dy * m11 + dz * m12;

    m21 = data->m21 / 8192.0f;

    projZ = dx * m20 + dy * m21 + dz * m22;

    iprojX = projX;
    iprojY = projY;

    angle = 0.43633232f;
    fprojZ = (s32)projZ;

    invCos = fprojZ;
    invCos = invCos * sinf(angle);
    invCos = invCos / __cosf(angle);
    new_var2 = -iprojY / invCos;

    tempY = new_var2 * 120.0f;
    *outY = -(s32)tempY + 120;
    new_var2 = iprojX / invCos;
    *outX = -(s32)(new_var2 * 160.0f) + 160;
}